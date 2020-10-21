#include "Dictionary.h"
#include "FormBuilderNouns.h"
#include "FormBuilderAdjLong.h"
#include "FormBuilderAdjShort.h"
#include "FormBuilderPronounAdj.h"
#include "FormBuilderAdjComparative.h"
#include "FormBuilderVerbPers.h"
#include "FormBuilderVerbImpv.h"
#include "FormBuilderVerbPast.h"
#include "FormBuilderVerbNonFinite.h"
#include "FormBuilderAspectPair.h"
#include "md5.h"
#include "Lexeme.h"

using namespace Hlib;

CLexeme::CLexeme(CDictionary * pD) : m_bFormsGenerated(false), m_pDictionary(pD)
{
    Init();
}

CLexeme::CLexeme(const CLexeme& source)
{
    Init();

    m_stProperties = source.m_stProperties;
    m_pDictionary = source.m_pDictionary;
    m_itCurrentWordForm = source.m_itCurrentWordForm;
    m_mmapIrregularForms = source.m_mmapIrregularForms;
    m_itCurrentIrregularForm = source.m_itCurrentIrregularForm;
//    m_vecMissingForms = source.m_vecMissingForms;
    m_itCurrentStressPos = source.m_itCurrentStressPos;

    auto it = source.m_mmWordForms.begin();
    for (;  it != source.m_mmWordForms.end(); ++it)
    {
        CWordForm * pWfCopy = new CWordForm(*(*it).second);
        pWfCopy->m_pLexeme = this;
        pair<CEString, CWordForm *> pairHW(pWfCopy->sGramHash(), pWfCopy);
        m_mmWordForms.insert(pairHW);
    }
}

CLexeme::~CLexeme()
{
    multimap<CEString, CWordForm *>::iterator itWf = m_mmWordForms.begin();
    for (; itWf != m_mmWordForms.end(); ++itWf)
    {
        delete(itWf->second);
        auto itRange = m_mmWordForms.equal_range(itWf->first);
        for (auto itForm = itRange.first; itForm != itRange.second; ++itForm)
        {
            itForm->second = NULL;
        }
    }

//    multimap<int, StIrregularForm>::iterator itIf = m_mmapIrregularForms.begin();
//    for (; itIf != m_mmapIrregularForms.end(); ++itIf)
//    {
//        delete(itIf->second.pWordForm);
//    }
}

void CLexeme::Init()
{
    vector<CEString> vecMainSymbol = { L"м", L"мо", L"ж", L"жо", L"с", L"со", L"мо-жо", L"мн.",
     L"мн. неод.", L"мн. одуш.", L"мн. _от_", L"п", L"мс", L"мс-п", L"числ.", L"числ.-п", 
     L"св", L"нсв", L"св-нсв", L"н", L"предл.", L"союз", L"предик.", L"вводн.", L"сравн.", 
     L"част.", L"межд." };

    try
    {
        for (ET_MainSymbol eMs = MS_START; eMs < MS_END; ++eMs)
        {
            m_mapMainSymbol[vecMainSymbol[eMs]] = eMs;
        }

        for (int iMs = 0; iMs < (int)MS_END; ++iMs)
        {
            CEString sMs = vecMainSymbol[iMs];
            ET_MainSymbol eMs = m_mapMainSymbol[sMs];
            switch (eMs)
            {
            case MS_M:
            {
                m_mapMainSymbolToAnimacy[sMs] = ANIM_NO;
                m_mapMainSymbolToGender[sMs] = GENDER_M;
                break;
            }
            case MS_MO:
            {
                m_mapMainSymbolToAnimacy[sMs] = ANIM_YES;
                m_mapMainSymbolToGender[sMs] = GENDER_M;
                break;
            }
            case MS_ZH:
            {
                m_mapMainSymbolToAnimacy[sMs] = ANIM_NO;
                m_mapMainSymbolToGender[sMs] = GENDER_F;
                break;
            }
            case MS_ZHO:
            case MS_MO_ZHO:
            {
                m_mapMainSymbolToAnimacy[sMs] = ANIM_YES;
                m_mapMainSymbolToGender[sMs] = GENDER_F;
                break;
            }
            case MS_S:
            {
                m_mapMainSymbolToAnimacy[sMs] = ANIM_NO;
                m_mapMainSymbolToGender[sMs] = GENDER_N;
                break;
            }
            case MS_SO:
            {
                m_mapMainSymbolToAnimacy[sMs] = ANIM_YES;
                m_mapMainSymbolToGender[sMs] = GENDER_N;
                break;
            }
            case MS_MN:
            {
                m_mapMainSymbolToAnimacy[sMs] = ANIM_UNDEFINED;
                m_mapMainSymbolToGender[sMs] = GENDER_UNDEFINED;
                break;
            }
            case MS_MN_NEOD:
            {
                m_mapMainSymbolToAnimacy[sMs] = ANIM_NO;
                m_mapMainSymbolToGender[sMs] = GENDER_UNDEFINED;
                break;
            }
            case MS_MN_ODUSH:
            {
                m_mapMainSymbolToAnimacy[sMs] = ANIM_YES;
                m_mapMainSymbolToGender[sMs] = GENDER_UNDEFINED;
                break;
            }
            case MS_MN_OT:
            {
                m_mapMainSymbolToAnimacy[sMs] = ANIM_NO;
                m_mapMainSymbolToGender[sMs] = GENDER_UNDEFINED;
                break;
            }
            case MS_SV:
            case MS_NSV:
            case MS_SV_NSV:
            case MS_P:
            case MS_MS:
            case MS_MS_P:
            case MS_N:
            case MS_CHISL:
            case MS_CHISL_P:
            case MS_CHAST:
            case MS_PREDL:
            case MS_SOJUZ:
            case MS_VVODN:
            case MS_PREDIK:
            case MS_MEZHD:
            case MS_SRAVN:
            {
                m_mapMainSymbolToAnimacy[sMs] = ANIM_UNDEFINED;
                m_mapMainSymbolToGender[sMs] = GENDER_UNDEFINED;
                break;
            }
            default:
            {
                ASSERT(0);
                ERROR_LOG(L"Unrecognized type.");
                return;
            }

            }    // switch

        }   //  for (int iMs = 0; ... )

        const wchar_t * arr1stStage[] =
        { L"б",  L"п",  L"в",  L"ф",  L"м",  L"з", L"с", L"д", L"т", L"ст", L"г", L"к", L"х", L"ск" };
        const wchar_t * arr2ndStage[] =
        { L"бл", L"пл", L"вл", L"фл", L"мл", L"ж", L"ш", L"ж", L"ч", L"щ",  L"ж", L"ч", L"ш", L"щ" };

        for (int i_sa = 0; i_sa < sizeof(arr1stStage) / sizeof(wchar_t *); ++i_sa)
        {
            m_mapStandardAlternations[arr1stStage[i_sa]] = arr2ndStage[i_sa];
        }

        const wchar_t * arrPreverbs[] = { L"в", L"над", L"об", L"от", L"под", L"пред", L"с" };
        for (int i_ap = 0; i_ap < sizeof(arrPreverbs) / sizeof(wchar_t *); ++i_ap)
        {
            m_vecAlternatingPreverbs.push_back(arrPreverbs[i_ap]);
        }

        const wchar_t * arrPreverbsV[] = { L"вс", L"вз", L"вос", L"воз", L"ис", L"из", L"нис", L"низ", L"рас", L"раз" };
        for (int i_ap = 0; i_ap < sizeof(arrPreverbsV) / sizeof(wchar_t *); ++i_ap)
        {
            m_vecAlternatingPreverbsWithVoicing.push_back(arrPreverbsV[i_ap]);
        }

        m_stProperties.s1SgStem.SetVowels(g_szRusVowels);
        m_stProperties.s3SgStem.SetVowels(g_szRusVowels);
        m_stProperties.sInfinitive.SetVowels(g_szRusVowels);
        m_stProperties.sInfStem.SetVowels(g_szRusVowels);
    }
    catch (...)
    {
        ERROR_LOG(L"Exception during Lexeme class instantiation");
    }


}   // Init()

ET_ReturnCode CLexeme::eAddCommonDeviation(int iValue, bool bIsOptional)
{
    if (iValue < 1)
    {
        ERROR_LOG(L"Negative value for common deviation");
        return H_ERROR_UNEXPECTED;
    }
    if (POS_NOUN == m_stProperties.ePartOfSpeech || POS_ADJ == m_stProperties.ePartOfSpeech)
    {
        ERROR_LOG(L"Unexpected value for common deviation; must be 1, 2, or 3");
        return H_ERROR_UNEXPECTED;
    }
    else if (POS_VERB == m_stProperties.ePartOfSpeech)
    {
        ERROR_LOG(L"Unexpected value for common deviation; must be 1, 2, or 3");
        return H_ERROR_UNEXPECTED;
    }

    m_stProperties.mapCommonDeviations[iValue] = bIsOptional;

    return H_NO_ERROR;
}

bool CLexeme::bFindCommonDeviation (int iNum, bool& bIsOptional)
{
    bIsOptional = false;

    map<int, bool>::iterator itCommonDev = m_stProperties.mapCommonDeviations.find(iNum);
    if (m_stProperties.mapCommonDeviations.end() == itCommonDev)
    {
        return false;
    }

    bIsOptional = (*itCommonDev).second;

    return true;
}

bool CLexeme::bFindStandardAlternation (const CEString& sKey, CEString& sValue)
{
    map<CEString, CEString>::iterator itAlt = m_mapStandardAlternations.find (sKey);
    if (m_mapStandardAlternations.end() != itAlt)
    {
        sValue = (*itAlt).second;
        return true;
    }
    else
    {
        sValue = L"";
        return false;
    }
}

ET_ReturnCode CLexeme::eGetStemStressPositions (const CEString& sStem, vector<int>& vecPosition)
{
    //
    // Find the sequence # of the stressed vowel in infinitive
    //
    CEString sCopy(sStem);

    vector<int>::iterator itInfStresPos = m_stProperties.vecSourceStressPos.begin();
    for (; itInfStresPos != m_stProperties.vecSourceStressPos.end(); ++itInfStresPos)
    {
        sCopy.SetVowels(g_szRusVowels);
        int iStemSyllables = sCopy.uiNSyllables();
        if (iStemSyllables < 1)
        {
            ASSERT(0);
            ERROR_LOG (L"No vowels in verb stem.");
            return H_ERROR_GENERAL;
        }

        if (*itInfStresPos >= iStemSyllables)
        {
            vecPosition.push_back (iStemSyllables - 1);  // last stem syllable
        }
        else
        {
            vecPosition.push_back (*itInfStresPos);    // same syllable as in infinitive
        }

    }   //  for (; it_sourceStressPos != vec_SourceStressPos.end(); ... )

    return H_NO_ERROR;

}   //  hGetStemStressPositions (...)

ET_ReturnCode CLexeme::eGetAlternatingPreverb (const CEString& sVerbForm, CEString& sPreverb, bool& bVoicing)
{
    if (!m_stProperties.bFleetingVowel)
    {
        return H_NO_MORE;
    }

// types 5, 6, 7, 8, 9, 11, 14
    bool bPreverb = false;

    auto itP = m_vecAlternatingPreverbsWithVoicing.begin();
    for (; itP != m_vecAlternatingPreverbsWithVoicing.end() && !bPreverb; ++itP)
    {
        if (sVerbForm.bStartsWith(*itP))
        {
            sPreverb = *itP;
            bPreverb = true;
            bVoicing = true;
        }
    }

    if (!bPreverb)
    {
        auto itP = m_vecAlternatingPreverbs.begin();
        for (; itP != m_vecAlternatingPreverbs.end() && !bPreverb; ++itP)
        {
            if (sVerbForm.bStartsWith(*itP))
            {
                sPreverb = *itP;
                bPreverb = true;
            }
        }
    }

    if (!bPreverb)
    {
        return H_FALSE;
    }

    if (sVerbForm.uiLength() < sPreverb.uiLength() + 2)
    {
//        ASSERT(0);
        CEString sMsg(L"Stem too short; lexeme = ");
        ERROR_LOG(sMsg + m_stProperties.sSourceForm);
        return H_ERROR_INVALID_ARG;
    }

    return H_NO_ERROR;

}       //  eGetAlternatingPreverb(...)

void CLexeme::AssignPrimaryStress (CWordForm * pWordForm)
{
    vector<int>::iterator itStressPos = m_stProperties.vecSourceStressPos.begin();
    for (; itStressPos != m_stProperties.vecSourceStressPos.end(); ++itStressPos)
    {
        pWordForm->m_mapStress[*itStressPos] = STRESS_PRIMARY;
    }
}

void CLexeme::AssignSecondaryStress (CWordForm * pWordForm)
{
    vector<int>::iterator itPos = m_stProperties.vecSecondaryStressPos.begin();
    for (; itPos != m_stProperties.vecSecondaryStressPos.end(); ++itPos)
    {
        int iStressedSyll = *itPos; // it _is_ the syllable number, so we don't need that conversion
        if (iStressedSyll < 0 || iStressedSyll >= (int)m_stProperties.sGraphicStem.uiGetNumOfSyllables())
        {
            throw CException (H_ERROR_UNEXPECTED, L"Secondary stress position out of bounds.");
        }

        pWordForm->m_mapStress[iStressedSyll] = STRESS_SECONDARY;
    }
}

void CLexeme::AddWordForm (CWordForm * pWordForm)
{
    if (NULL == pWordForm)
    {
        throw CException (H_ERROR_POINTER, L"Invalid wordform");
    }

    AssignSecondaryStress(pWordForm);

    pair<CEString, CWordForm *> pairHW (pWordForm->sGramHash(), pWordForm);
    m_mmWordForms.insert (pairHW);

}  //  AddWordForm (...)


uint64_t CLexeme::uiTotalWordForms()
{
    return m_mmWordForms.size();
}

ET_ReturnCode CLexeme::eSaveDescriptorInfo(CLexeme * pLexeme)
{
    return m_pDictionary->eSaveDescriptorInfo(pLexeme);
}

ET_ReturnCode CLexeme::eGetWordForm(unsigned int uiAt, CWordForm *& pWordForm)
{
    if (uiAt >= m_mmWordForms.size())
    {
        return H_ERROR_INVALID_ARG;
    }

    auto it = m_mmWordForms.begin();
    if (m_mmWordForms.end() == it)
    {
        return H_ERROR_UNEXPECTED;
    }

    for (unsigned int uiWf = 0; uiWf < uiAt; ++uiWf)
    {
        ++it;
        if (m_mmWordForms.end() == it)
        {
            return H_ERROR_UNEXPECTED;
        }
    }

    pWordForm = (*it).second;

    return H_NO_ERROR;

}       //  eGetWordForm()

ET_ReturnCode CLexeme::eCreateWordForm(IWordForm *& pIwf)
{
    CWordForm * pWf = new CWordForm();
    pWf->m_pLexeme = this;
    pWf->m_llLexemeId = m_stProperties.llDescriptorId;

    pIwf = pWf;     // CWordForm -> IWordForm

    return H_NO_ERROR;
}

ET_ReturnCode CLexeme::eWordFormFromHash(CEString sHash, int iAt, CWordForm *& pWf)
{
    pair<multimap<CEString, CWordForm *>::iterator, multimap<CEString, CWordForm *>::iterator> pairRange;
    pairRange = m_mmWordForms.equal_range(sHash);
    if (pairRange.first == pairRange.second)
    {
//        ASSERT(0);
//        CEString sMsg(L"Unable to locate word form in collection, hash = ");
//        sMsg += sHash;
//        sMsg += L".";
//        ERROR_LOG(sMsg);
//        return H_ERROR_UNEXPECTED;
        return H_FALSE;     // form does not exist; this is common, cf. locative, non-existent participia, etc.
    }

    if (distance (pairRange.first, pairRange.second) - 1 < iAt)
    {
        ASSERT(0);
        ERROR_LOG (L"Bad form number.");
        return H_ERROR_UNEXPECTED;
    }

    auto itWf (pairRange.first);
    advance (itWf, iAt);
    if ((*itWf).first != sHash)
    {
        ASSERT(0);
        ERROR_LOG (L"Error extracting map element.");
        return H_ERROR_INVALID_ARG;
    }

    pWf = itWf->second;

    return H_NO_ERROR;

}   //  WordFormFromHash (...)

ET_ReturnCode CLexeme::eWordFormFromHash(CEString sHash, int iAt, IWordForm *& pIWordForm)
{
    CWordForm * pCWordForm = NULL;
    ET_ReturnCode rc = eWordFormFromHash(sHash, iAt, pCWordForm);
    if (H_NO_ERROR == rc)
    {
        pIWordForm = pCWordForm;
    }
    return rc;
}

ET_ReturnCode CLexeme::eRemoveWordForm(CEString sHash, int iAt)
{
    pair<multimap<CEString, CWordForm *>::iterator, multimap<CEString, CWordForm *>::iterator> pairRange;
    pairRange = m_mmWordForms.equal_range(sHash);
    if (pairRange.first == pairRange.second)
    {
        ASSERT(0);
        ERROR_LOG(L"Unable to locate word form in collection.");
        return H_ERROR_UNEXPECTED;
    }

    if (distance(pairRange.first, pairRange.second) - 1 < iAt)
    {
        ASSERT(0);
        ERROR_LOG(L"Bad form number.");
        return H_ERROR_UNEXPECTED;
    }

    auto itWf(pairRange.first);
    advance(itWf, iAt);
    if ((*itWf).first != sHash)
    {
        ASSERT(0);
        ERROR_LOG(L"Error extracting map element.");
        return H_ERROR_INVALID_ARG;
    }

    m_mmWordForms.erase(itWf);

    return H_NO_ERROR;

}   //  eRemoveWordForm (...)

ET_ReturnCode CLexeme::eRemoveWordForms(CEString sHash)
{
    auto pairRange = m_mmapIrregularForms.equal_range(sHash);
    if (pairRange.first == pairRange.second)
    {
        ASSERT(0);
        ERROR_LOG(L"Unable to locate word form in collection.");
        return H_ERROR_UNEXPECTED;
    }

    m_mmapIrregularForms.erase(pairRange.first, pairRange.second);

    return H_NO_ERROR;
}

// Invoked from the UI hence need for dynamic cast
void CLexeme::AddWordForm(IWordForm* pItfWordForm)
{
    CWordForm* pCWf = dynamic_cast<CWordForm*>(pItfWordForm);
    if (pItfWordForm->bIrregular())
    {
        StIrregularForm stIf(pCWf, pCWf->bIsVariant());
        m_mmapIrregularForms.insert(pair<CEString, StIrregularForm>(pItfWordForm->sGramHash(), stIf));
    }
    else
    {
        m_mmWordForms.insert(pair<CEString, CWordForm*>(pItfWordForm->sGramHash(), pCWf));
    }
}

bool CLexeme::bNoRegularForms (CEString sGramHash)
{
    IWordForm * pWf = NULL;         // currently not nused
    bool bIsVariant = false;
    ET_ReturnCode rc = eGetFirstIrregularForm (sGramHash, pWf, bIsVariant);
    if (H_FALSE == rc)
    {
        return false;
    }
    if (rc != H_NO_ERROR && rc != H_FALSE)
    {
        ASSERT(0);
        CEString sMsg (L"hGetFirstIrregularForm() failed, error ");
        sMsg += CEString::sToString(rc);
        ERROR_LOG (sMsg);
        throw CException (E_FAIL, sMsg);
    }

    if (bIsVariant)    // irregular form is optional i.e. we still need to produce regular one  
    {
        return false;
    }
    
    while (H_NO_ERROR == rc)
    {
        bIsVariant = false;        
        rc = eGetNextIrregularForm (pWf, bIsVariant);
        if (rc != H_NO_ERROR && rc != H_NO_MORE)
        {
            ASSERT(0);
            CEString sMsg (L"hGetNextIrregularForm() failed, error ");
            sMsg += CEString::sToString(rc);
            ERROR_LOG (sMsg);
            throw CException (E_FAIL, sMsg);
        }
        if (bIsVariant)
        {
            return false;
        }  
    }
    return true;

}   //  bNoRegularForms()

bool CLexeme::bHasIrregularForm (CEString sGramHash)
{
    ET_ReturnCode rc = H_NO_ERROR;

    IWordForm * pWf = NULL;
    bool bIsOptional = false;       // ignored
    rc = eGetFirstIrregularForm(sGramHash, pWf, bIsOptional);
    if ((rc != H_NO_ERROR && rc != H_FALSE) || (H_NO_ERROR == rc && NULL == pWf))
    {
        ASSERT(0);
        CEString sMsg (L"Irregular form not found in database.");
        ERROR_LOG (sMsg);
        throw CException (rc, sMsg);
    }

    return (H_NO_ERROR == rc) ? true : false;
}

ET_ReturnCode CLexeme::eGetIrregularForms(CEString sHash, map<CWordForm *, bool>& mapResult)
{
    ET_ReturnCode rc = H_NO_ERROR;

    CWordForm * pWf = NULL;
    bool bIsOptional = false;
    rc = eGetFirstIrregularForm(sHash, pWf, bIsOptional);
    if (rc != H_NO_ERROR && rc != H_FALSE && rc != H_NO_MORE)
    {
        ASSERT(0);
        ERROR_LOG (L"Irregular form not found in database.");
        return H_ERROR_UNEXPECTED;
    }

    // No irregular form for this hash?
    if (H_FALSE == rc)
    {
        return H_NO_ERROR;  // expected
    }

    // Fill in data for the first irregular form
    mapResult[pWf] = bIsOptional;

    // Continue for other irregular forms for this hash 
    // (which, in all likelyhood do not exist)
    do
    {
        rc = eGetNextIrregularForm(pWf, bIsOptional);
        if (rc != H_NO_ERROR && rc != H_NO_MORE)
        {
            ASSERT(0);
            ERROR_LOG (L"Error accessing irregular m Sg Past form.");
            return H_ERROR_UNEXPECTED;
        }

        if (H_NO_ERROR == rc)
        {
            map<CWordForm *, bool>::iterator it = mapResult.find(pWf);
            if (it != mapResult.end())
            {
                ASSERT(0);
                ERROR_LOG (L"Duplicate irregular form.");
                return H_ERROR_UNEXPECTED;
            }

            mapResult[pWf] = bIsOptional;
        }

    }  while (H_NO_ERROR == rc);

    return H_NO_ERROR;

}   //  eGetIrregularForms()

CEString CLexeme::sGramHashNSgMLong()
{
    CGramHasher hasher (POS_ADJ, SUBPARADIGM_LONG_ADJ, CASE_UNDEFINED, NUM_UNDEFINED, GENDER_UNDEFINED, PERSON_UNDEFINED,                             
                        ANIM_NO, ASPECT_UNDEFINED, REFL_UNDEFINED);
    auto sHash = hasher.sGramHash();
    return sHash;
}

int CLexeme::iFormCount (CEString sHash)
{
    return (int)m_mmWordForms.count (sHash);
}

bool CLexeme::bHasCommonDeviation (int iCd)
{
    bool bRet = false;
    map<int, bool>::iterator itCd = m_stProperties.mapCommonDeviations.find (iCd);
    return (itCd != m_stProperties.mapCommonDeviations.end());
}

bool CLexeme::bDeviationOptional (int iCd)
{
    bool bRet = false;
    map<int, bool>::iterator itCd = m_stProperties.mapCommonDeviations.find (iCd);
    if (itCd != m_stProperties.mapCommonDeviations.end())
    {
        return (*itCd).second;
    }
    else
    {
        ASSERT(0);
        CEString sMsg (L"Common deviation expected.");
        ERROR_LOG (sMsg);
        throw CException (E_FAIL, sMsg);
    }
}

ET_ReturnCode CLexeme::eFormExists(const CEString& sFH)
{
    if (L"AdjComp" == sFH && m_stProperties.bNoComparative)
    {
        return H_FALSE;
    }

    if (sFH.bStartsWith(L"AdjL_") && m_stProperties.bNoLongForms)
    {
        return H_FALSE;
    }

    if (m_stProperties.bIsImpersonal && L"Pres_Sg_3" != sFH && L"Past_N" != sFH && L"Inf" != sFH)
    {
        return H_FALSE;
    }

    if (m_stProperties.bIsIterative)
    {
        if (sFH.bStartsWith(L"Pres") || sFH.bStartsWith(L"PPres") || sFH.bStartsWith(L"VAdv_Pres") || 
            sFH.bStartsWith(L"Impv"))
        {
            return H_FALSE;
        }
    }

    auto sFormHash = const_cast<CEString&> (sFH);
    sFormHash.SetBreakChars(L"_");

    if (L"AdjS" == sFormHash.sGetField(0) || L"AdjComp" == sFormHash.sGetField(0))
    {
        if (L"мс-п" == m_stProperties.sMainSymbol || L"мс" == m_stProperties.sInflectionType)
        {
            return H_FALSE;
        }

        if (m_stProperties.sSourceForm.bEndsWith(L"ийся"))
        {
            return H_FALSE;
        }
    }

    if (m_stProperties.bShortFormsIncomplete && L"AdjS_M" == sFormHash)
    {
        return H_FALSE;
    }
    
    if (m_stProperties.bNoPassivePastParticiple && sFormHash.uiNFields() > 0)
    {
        if (sFormHash.sGetField(0).bStartsWith(L"PPastP"))
        {
            return H_FALSE;
        }
    }

    if ((sFormHash.bStartsWith(L"PPresA") || L"VAdv_Pres" == sFormHash) && L"св" == m_stProperties.sMainSymbol)
    {
        return H_FALSE;
    }

    if (sFormHash.bStartsWith(L"VAdv_Pres"))
    {
        int aNoAdvTypes[] = { 3, 8, 9, 11, 14, 15 };
        auto size = aNoAdvTypes + sizeof(aNoAdvTypes) / sizeof(int);
        if (find(aNoAdvTypes, size, m_stProperties.iType) != size)
        {
            return H_FALSE;
        }
    }

    if (sFormHash.bStartsWith(L"PPresP") && !m_stProperties.bHasPresPassParticiple)
    {
        return H_FALSE;
    }

    if (sFormHash.bStartsWith(L"PPastP") && (!m_stProperties.bTransitive || REFL_YES == m_stProperties.eReflexive))
    {
        return H_FALSE;
    }

    if (!m_stProperties.bHasMissingForms)
    {
        return H_TRUE;
    }

    if (sFH.uiLength() < 1)
    {
        ERROR_LOG(L"Empty form hash.");
        return H_ERROR_UNEXPECTED;
    }

    bool bNegate = false;

    for (auto sMissing : m_vecMissingForms)
    {
        if (L'!' == sMissing[0])
        {
            bNegate = true;
            sMissing = sMissing.sSubstr(1);
        }

        sMissing.SetBreakChars(L"_");
        bool bMatch = true;
        if (sMissing.uiNFields() != sFormHash.uiNFields())
        {
            bMatch = false;
        }
        else
        {
            for (int iAt = 0; iAt < (int)sFormHash.uiNFields(); ++iAt)
            {
                auto sFormHashFragment = (sFormHash.uiNFields() == 0) ? sFormHash : sFormHash.sGetField(iAt);
                auto sMissingHashFragment = (sMissing.uiNFields() == 0) ? sMissing : sMissing.sGetField(iAt);

                if (sMissingHashFragment == L"*")
                {
                    continue;
                }

                if (sMissingHashFragment != sFormHashFragment)
                {
                    bMatch = false;
                    break;
                }
            }
        }

        if (bMatch)
        {
            return bNegate ? H_TRUE : H_FALSE;
        }
    }

    return bNegate ? H_FALSE : H_TRUE;

}   //  eFormExists (...)

ET_ReturnCode CLexeme::eSetFormExists(const CEString& sGramHash, bool bExists)
{
    if (L"AdjComp" == sGramHash)
    {
        m_stProperties.bNoComparative = bExists;
        return H_NO_ERROR;
    }

    if (bExists)
    {
        auto itFound = find(m_vecMissingForms.begin(), m_vecMissingForms.end(), sGramHash);
        if (itFound == m_vecMissingForms.end())
        {
            CEString sMsg(L"Hash " + sGramHash + L" not found.");
            ERROR_LOG(sMsg);
            return H_ERROR_UNEXPECTED;
        }

        m_vecMissingForms.erase(itFound);
    }
    else
    {
        m_vecMissingForms.push_back(sGramHash);
    }

    return H_NO_ERROR;
}

ET_ReturnCode CLexeme::eIsFormDifficult(const CEString& sFH)
{
    auto sFormHash = const_cast<CEString&> (sFH);
    sFormHash.SetBreakChars(L"_");

    if (m_stProperties.bShortFormsRestricted && sFormHash.uiNFields() > 0)      // pometa "x"
    {
        if (sFormHash.sGetField(0).bStartsWith(L"AdjS"))
        {
            return H_TRUE;
        }
        if (sFormHash.sGetField(0).bStartsWith(L"PPastP"))
        {
            return H_TRUE;
        }
        if (m_stProperties.bShortFormsIncomplete && L"AdjS_M" != sFormHash)     // boxed "x"
        {
            return H_TRUE;
        }
    }

    if (!m_stProperties.bHasDifficultForms)
    {
        return H_FALSE;
    }

    if (sFH.uiLength() < 1)
    {
        ERROR_LOG(L"Empty form hash.");
        return H_ERROR_UNEXPECTED;
    }

    bool bNegate = false;

    sFormHash.SetBreakChars(L"_");

    for (auto sDifficult : m_vecDifficultForms)
    {
        if (L'!' == sDifficult[0])
        {
            bNegate = true;
            sDifficult = sDifficult.sSubstr(1);
        }

        sDifficult.SetBreakChars(L"_");
        if (sDifficult.uiNFields() != sFormHash.uiNFields())
        {
            return bNegate ? H_TRUE : H_FALSE;
        }

        bool bMatch = true;
        for (int iAt = 0; iAt < (int)sFormHash.uiNFields(); ++iAt)
        {
            auto sFormHashFragment = (sFormHash.uiNFields() == 0) ? sFormHash : sFormHash.sGetField(iAt);
            auto sDifficultHashFragment = (sDifficult.uiNFields() == 0) ? sDifficult : sDifficult.sGetField(iAt);

            if (sDifficultHashFragment == L"*")
            {
                continue;
            }

            if (sDifficultHashFragment != sFormHashFragment)
            {
                bMatch = false;
                break;
            }
        }

        if (bMatch && !bNegate)
        {
            return H_TRUE;
        }
    }

    return bNegate ? H_TRUE : H_FALSE;

}   //  eIsFormDifficult()

ET_ReturnCode CLexeme::eSetFormDifficult(const CEString& sGramHash, bool bIsDifficult)
{
    if (bIsDifficult)
    {
        auto itFound = find(m_vecMissingForms.begin(), m_vecMissingForms.end(), sGramHash);
        if (itFound == m_vecDifficultForms.end())
        {
            CEString sMsg(L"Hash " + sGramHash + L" not found.");
            ERROR_LOG(sMsg);
            return H_ERROR_UNEXPECTED;
        }

        m_vecDifficultForms.erase(itFound);
    }
    else
    {
        m_vecDifficultForms.push_back(sGramHash);
    }

    return H_NO_ERROR;
}

ET_ReturnCode CLexeme::eDifficultFormsHashes(vector<CEString>& vecHashes)
{
    vecHashes = m_vecDifficultForms;
    return H_NO_ERROR;
}

ET_ReturnCode CLexeme::eIsFormAssumed(const CEString& sFH)
{
    auto sFormHash = const_cast<CEString&> (sFH);
    sFormHash.SetBreakChars(L"_");

    if (m_stProperties.bAssumedForms)
    {
        if (L"Noun" == sFormHash.sGetField(0) && L"Pl" == sFormHash.sGetField(1))
        {
            return H_TRUE;
        }

        if (L"AdjS_M" == sFormHash)
        {
            return H_TRUE;
        }
    }

    return H_FALSE;

}   //  eIsFormAssumed()

ET_ReturnCode CLexeme::eSetHasAssumedForms(bool bIsAssumed)
{
    m_stProperties.bAssumedForms = bIsAssumed;
    return H_NO_ERROR;
}

//
// GDRL p. 11: a dash in a word with two main stress marks means a compound form
// with two main stresses; otherwise 
//
bool CLexeme::bIsMultistressedCompound()
{
    if (m_stProperties.vecSourceStressPos.size() < 2)
    {
        return false;
    }

    unsigned int uiDashPos = m_stProperties.sGraphicStem.uiFind (L"-");
    if (ecNotFound == uiDashPos || uiDashPos < 1)
    {
        return false;
    }

    // find any two that are separated by dash
    vector<int>::iterator itLeft = m_stProperties.vecSourceStressPos.begin();
    unsigned int uiLeftVowelPos = m_stProperties.sGraphicStem.uiGetVowelPos (*itLeft);
    if (uiLeftVowelPos < 0)
    {
        ASSERT(0);
        CEString sMsg (L"Can't find stressed vowel.");
        ERROR_LOG (sMsg);
        throw CException (E_FAIL, sMsg);
    }
    if (uiLeftVowelPos > uiDashPos)
    {
        return false;
    }

    vector<int>::iterator itRight = itLeft + 1;
    for (; itRight != m_stProperties.vecSourceStressPos.end(); ++itRight)
    {
        if (m_stProperties.sGraphicStem.uiGetVowelPos (*itRight) > uiDashPos)
        {
            return true;
        }
    }

    return false;

}   //  bMultiStress (...)

ET_ReturnCode CLexeme::eSetDb (const CEString& sDbPath)
{
    return m_pDictionary->eSetDbPath(sDbPath);
}

ET_ReturnCode CLexeme::eMakeGraphicStem(const CEString& sSource, CEString& sGraphicStem)
{
    if (sSource.uiLength() < 1)
    {
        ERROR_LOG(L"No source form.");
        return H_ERROR_UNEXPECTED;
    }

    if (L"м" == m_stProperties.sInflectionType || L"мо" == m_stProperties.sInflectionType || L"ж" == m_stProperties.sInflectionType
        || L"жо" == m_stProperties.sInflectionType || L"мо-жо" == m_stProperties.sInflectionType || L"с" == m_stProperties.sInflectionType
        || L"со" == m_stProperties.sInflectionType || L"мс-п" == m_stProperties.sInflectionType)
    {
        if (0 == m_stProperties.iInflectionId)
        {
            sGraphicStem = sSource;
        }

        if (sSource.bEndsWithOneOf(L"аеёиоуыэюяйь"))
        {
            if (sSource.uiLength() < 2)
            {
                ERROR_LOG(L"Source form too short.");
                return H_ERROR_UNEXPECTED;
            }
            sGraphicStem = sSource;
            sGraphicStem.sRemoveCharsFromEnd(1);
        }
        else
        {
            sGraphicStem = sSource;
        }

        return H_NO_ERROR;
    }

    if (L"мс" == m_stProperties.sMainSymbol)
    {
        sGraphicStem = sSource;
        return H_NO_ERROR;
    }

    if (L"мн." == m_stProperties.sInflectionType || L"мн. неод." == m_stProperties.sInflectionType || L"мн. одуш." == m_stProperties.sInflectionType
        || L"мн. от" == m_stProperties.sInflectionType)
    {
        if (sSource.uiLength() < 2)
        {
            ERROR_LOG(L"Source form too short.");
            return H_ERROR_UNEXPECTED;
        }
        sGraphicStem = sSource;
        sGraphicStem.sRemoveCharsFromEnd(1);
        return H_NO_ERROR;
    }

    if (L"п" == m_stProperties.sInflectionType)
    {
        if (m_stProperties.bNoLongForms)
        {
            sGraphicStem = sSource;
        }

        if (sSource.bEndsWith(L"ся"))
        {
            if (sSource.uiLength() < 5)
            {
                ERROR_LOG(L"Source form too short.");
                return H_ERROR_UNEXPECTED;
            }
            sGraphicStem = sSource;
            sGraphicStem.sRemoveCharsFromEnd(4);
        }
        else
        {
            if (sSource.uiLength() < 3)
            {
                ERROR_LOG(L"Source form too short.");
                return H_ERROR_UNEXPECTED;
            }
            sGraphicStem = sSource;
            sGraphicStem.sRemoveCharsFromEnd(2);
        }
        return H_NO_ERROR;
    }

    if (L"св" == m_stProperties.sInflectionType || L"нсв" == m_stProperties.sInflectionType || L"св-нсв" == m_stProperties.sInflectionType)
    {
        if (sSource.bEndsWith(L"ти") || sSource.bEndsWith(L"ть")
            || sSource.bEndsWith(L"чь"))
        {
            if (sSource.uiLength() < 3)
            {
                ERROR_LOG(L"Source form too short.");
                return H_ERROR_UNEXPECTED;
            }
            sGraphicStem = sSource;
            sGraphicStem.sRemoveCharsFromEnd(2);
        }
        else if (sSource.bEndsWith(L"тись") || sSource.bEndsWith(L"ться")
            || sSource.bEndsWith(L"чься"))
        {
            if (sSource.uiLength() < 5)
            {
                ERROR_LOG(L"Source form too short.");
                return H_ERROR_UNEXPECTED;
            }
            sGraphicStem = sSource;
            sGraphicStem.sRemoveCharsFromEnd(4);
            m_stProperties.eReflexive = REFL_YES;
        }
        else
        {
            sGraphicStem = sSource;
            // Assume verbal wordforms with no infinitive form, like "поезжай"
        }
        return H_NO_ERROR;
    }

    m_stProperties.sGraphicStem = sSource;

    return H_NO_ERROR;

}   //  eMakeGraphicStem(const CEString& sSource, CEString& sGraphicStem)

ET_ReturnCode CLexeme::eMakeGraphicStem()
{
    ET_ReturnCode eRet = H_NO_ERROR;
    if (m_stProperties.bIsSecondPart)
    {
        if (m_stProperties.sGraphicStem2.uiLength() < 1)
        {
            ERROR_LOG(L"No second graphic stem.");
            return H_ERROR_UNEXPECTED;
            eRet = eMakeGraphicStem(m_stProperties.sSourceForm, m_stProperties.sGraphicStem2);
        }
    }
    else
    {
        eRet = eMakeGraphicStem(m_stProperties.sSourceForm, m_stProperties.sGraphicStem);
    }
    return eRet;

}       //  eMakeGraphicStem()

ET_ReturnCode CLexeme::eExtractStressSymbols()
{
    m_stProperties.vecSourceStressPos.clear();
    m_stProperties.vecSecondaryStressPos.clear();

    vector<int> vecMainStressedVowelPos, vecSecondaryStressedVowelPos;

    try
    {
        unsigned int iAt = 0;
        while (iAt < m_stProperties.sSourceForm.uiLength() - 1)
        {
            auto uiPos = m_stProperties.sSourceForm.uiFindOneOf(iAt, L"/\\");
            if (ecNotFound == uiPos)
            {
                break;
            }

            if (uiPos >= m_stProperties.sSourceForm.uiLength() - 1)
            {
                ERROR_LOG(L"Stress mark at end of source form.");
                return H_ERROR_UNEXPECTED;
            }

            wchar_t wchrStressMark = m_stProperties.sSourceForm[uiPos];
            if (!CEString::bIsVowel(m_stProperties.sSourceForm[uiPos + 1]))
            {
                ERROR_LOG(L"Stress mark not over vowel.");
                return H_ERROR_UNEXPECTED;
            }

            if (L'/' == m_stProperties.sSourceForm[uiPos])
            {
                m_stProperties.vecSourceStressPos.push_back(uiPos);
            }
            else if (L'\\' == m_stProperties.sSourceForm[uiPos])
            {
                m_stProperties.vecSecondaryStressPos.push_back(uiPos);
            }

            m_stProperties.sSourceForm.sErase(uiPos, 1);
        }

        //for (auto iVowelPos : vecMainStressedVowelPos)
        //{
        //    m_stProperties.vecSourceStressPos.push_back(m_stProperties.sSourceForm.uiGetSyllableFromVowelPos(iVowelPos));
        //}

        //for (auto iVowelPos : vecSecondaryStressedVowelPos)
        //{
        //    m_stProperties.vecSecondaryStressPos.push_back(m_stProperties.sSourceForm.uiGetSyllableFromVowelPos(iVowelPos));
        //}
    }
    catch (CException& ex)
    {
        ERROR_LOG(ex.szGetDescription());
        return H_EXCEPTION;
    }
    catch (...)
    {
        ERROR_LOG(L"Unknown exception.");
    }

    return H_NO_ERROR;
}

ET_ReturnCode CLexeme::eInitializeFromProperties()
{
    if (m_stProperties.sSourceForm.uiLength() < 1)
    {
        CEString sError(L"No source form.");
        ERROR_LOG(sError);
        m_sLastErrorMsg = sError;
        return H_ERROR_UNEXPECTED;
    }

    ET_ReturnCode eRet = eExtractStressSymbols();
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    if (m_stProperties.sMainSymbol.uiLength() < 1)
    {
        CEString sError(L"No main symbol.");
        ERROR_LOG(sError);
        m_sLastErrorMsg = sError;
        return H_ERROR_UNEXPECTED;
    }

    if (m_stProperties.sMainSymbol.uiLength() < 1)
    {
        CEString sError(L"No main symbol.");
        ERROR_LOG(sError);
        m_sLastErrorMsg = sError;
        return H_ERROR_UNEXPECTED;
    }

    if (0 == m_mapMainSymbol.count(m_stProperties.sMainSymbol))
    {
        CEString sError(L"Unknown main symbol.");
        ERROR_LOG(sError);
        m_sLastErrorMsg = sError;
        return H_ERROR_UNEXPECTED;
    }

    if (m_stProperties.sInflectionType.uiLength() < 1)
    {
        m_stProperties.sInflectionType = m_stProperties.sMainSymbol;
    }

    auto eMainSymbol = m_mapMainSymbol[m_stProperties.sMainSymbol];
    switch (eMainSymbol)
    {
    case MS_M:
    case MS_MO:
    case MS_ZH:
    case MS_ZHO:
    case MS_S:
    case MS_SO:
    case MS_MO_ZHO:
    case MS_MN:
    case MS_MN_NEOD:
    case MS_MN_ODUSH:
    case MS_MN_OT:
        if (m_stProperties.iType < 1 || m_stProperties.iType > 8)
        {
            CEString sError(L"Bad inflection type.");
            ERROR_LOG(sError);
            m_sLastErrorMsg = sError;
            return H_ERROR_UNEXPECTED;
        }
        if (m_stProperties.eAccentType1 <= AT_UNDEFINED || m_stProperties.eAccentType1 >= AT_COUNT)
        {
            CEString sError(L"Bad accent type.");
            ERROR_LOG(sError);
            m_sLastErrorMsg = sError;
            return H_ERROR_UNEXPECTED;
        }

        m_stProperties.ePartOfSpeech = POS_NOUN;
        break;

    case MS_P:
        m_stProperties.ePartOfSpeech = POS_ADJ;
        break;

    case MS_MS:
        m_stProperties.ePartOfSpeech = POS_PRONOUN;
        break;

    case MS_MS_P:
        m_stProperties.ePartOfSpeech = POS_PRONOUN_ADJ;
        break;

    case MS_CHISL:
        m_stProperties.ePartOfSpeech = POS_NUM;
        break;

    case MS_CHISL_P:
        m_stProperties.ePartOfSpeech = POS_NUM_ADJ;
        break;

    case MS_SV:
    case MS_NSV:
    case MS_SV_NSV:
        m_stProperties.ePartOfSpeech = POS_VERB;
        break;

    case MS_N:
        m_stProperties.ePartOfSpeech = POS_ADV;
        break;

    case MS_PREDL:
        m_stProperties.ePartOfSpeech = POS_PREPOSITION;
        break;

    case MS_SOJUZ:
        m_stProperties.ePartOfSpeech = POS_CONJUNCTION;
        break;

    case MS_PREDIK:
        m_stProperties.ePartOfSpeech = POS_PREDIC;
        break;

    case MS_VVODN:
        m_stProperties.ePartOfSpeech = POS_PARENTH;
        break;

    case MS_SRAVN:
        m_stProperties.ePartOfSpeech = POS_COMPAR;
        break;

    case MS_CHAST:
        m_stProperties.ePartOfSpeech = POS_PARTICLE;
        break;

    case MS_MEZHD:
        m_stProperties.ePartOfSpeech = POS_INTERJ;
        break;

    case MS_END:
    case MS_UNDEFINED:
    default:
        break;
    }               //  switch (eMainSymbol)

    //eRet = eMakeGraphicStem();

    return eRet;

}       //  eInitializeFromProperties()

//
// Private
//
ET_ReturnCode CLexeme::eSaveStemsToDb()
{
    if (m_mmWordForms.empty())
    {
        //        throw CException(H_ERROR_UNEXPECTED, L"No wordforms.");
    }

    ET_ReturnCode eRet = H_NO_ERROR;

    CEString sCurrentStem;
    auto itWf = m_mmWordForms.begin();
    for (; itWf != m_mmWordForms.end(); ++itWf)
    {
        CWordForm * pWf = (*itWf).second;
        if (NULL == pWf)
        {
            eRet = H_ERROR_POINTER;
            ERROR_LOG(L"No wordform.");
            continue;
        }

        if (pWf->m_bIrregular)
        {
            continue;
        }

        if (pWf->m_sStem != sCurrentStem)
        {
            bool bSaved = (*itWf).second->bSaveStemToDb();
            if (!bSaved)
            {
                eRet = H_ERROR_GENERAL;
                ERROR_LOG(L"Unable to save a stem in the DB.");
                continue;
            }
            sCurrentStem = (*itWf).second->m_sStem;
        }
    }

    return eRet;

}   // eSaveStemsToDb()

ET_ReturnCode CLexeme::eAssignStemIds()
{
    ET_ReturnCode eRet = H_NO_ERROR;

    auto itWf = m_mmWordForms.begin();
    for (; itWf != m_mmWordForms.end(); ++itWf)
    {
//        if (0 == (*itWf).second->m_llStemId)    // stem string not found, look up in DB
        {
            CEString sQuery(L"SELECT id FROM stems WHERE stem_string=\"");
            sQuery += (*itWf).second->m_sStem;
            sQuery += L'"';

           try
           {
                CSqlite * pDbHandle = m_pDictionary->pGetDbHandle();
                pDbHandle->PrepareForSelect(sQuery);
                if (pDbHandle->bGetRow())
                {
                    pDbHandle->GetData(0, (*itWf).second->m_llStemId);
                }
                else
                {
                    CEString sMsg(L"Unable to find stem id for \"");
                    sMsg += (*itWf).second->m_sStem;
                    sMsg += L"\"; lexeme = ";
                    ERROR_LOG(sMsg += m_stProperties.sSourceForm);
                }
                pDbHandle->Finalize();
            }
            catch (CException& ex)
            {
                ERROR_LOG(ex.szGetDescription());
                eRet = H_ERROR_DB;
            }
            catch (...)
            {
                ERROR_LOG(L"Unknown exception.");
            }

        }
    }       // for (...)

    return eRet;

}       //  eAssignStemIds()

ET_ReturnCode CLexeme::eSaveWordFormsToDb()
{
    if (m_mmWordForms.empty())
    {
        return H_FALSE;
    }

    ET_ReturnCode eRet = H_NO_ERROR;

    auto itWf = m_mmWordForms.begin();
    for (; itWf != m_mmWordForms.end(); ++itWf)
    {
        if (0 == (*itWf).second->m_llStemId)
        {
            CEString sMsg(L"Unable to find stem id for \"");
            sMsg += (*itWf).second->m_sStem;
            sMsg += L'"';
            ERROR_LOG(sMsg);

            continue;
        }

        bool bSaved = (*itWf).second->bSaveToDb();
        if (!bSaved)
        {
            eRet = H_ERROR_GENERAL;
        }

    }       //  for (...)

    return eRet;

}   // eSaveWordFormsToDb()

ET_ReturnCode CLexeme::eLoadIrregularForms()
{
    ET_ReturnCode rc = H_NO_ERROR;

    if (!(m_stProperties.bHasIrregularForms || m_stProperties.ePartOfSpeech == POS_NUM))
    {
        return H_FALSE;
    }

    m_stProperties.bHasIrregularVariants = false;

    CEString sQuery 
        (L"SELECT id, gram_hash, wordform, is_alternative, lead_comment, trailing_comment, is_edited FROM irregular_forms WHERE descriptor_id = ");
    sQuery += CEString::sToString (m_stProperties.llDescriptorId);
    sQuery += L";";

    CSqlite * pDb = NULL;

    m_mmapIrregularForms.clear();

    try
    {
        pDb = m_pDictionary->pGetDbHandle();            
        uint64_t uiQueryHandle = pDb->uiPrepareForSelect (sQuery);
        while (pDb->bGetRow(uiQueryHandle))
        {
            //StIrregularForm stForm;
            int iId = -1;
            CEString sHash;
            CEString sForm;
            bool bIsVariant = false;
            CEString sLeadComment;
            CEString sTrailingComment;
            bool bIsEdited;

            pDb->GetData (0, iId, uiQueryHandle);
            pDb->GetData (1, sHash, uiQueryHandle);
            pDb->GetData (2, sForm, uiQueryHandle);
            pDb->GetData (3, bIsVariant, uiQueryHandle);
            pDb->GetData(4, sLeadComment, uiQueryHandle);
            pDb->GetData(5, sTrailingComment, uiQueryHandle);
            pDb->GetData(6, bIsEdited, uiQueryHandle);

            if (bIsVariant)
            {
                m_stProperties.bHasIrregularVariants = true;
            }

            CEString sStressQuery (L"SELECT position, is_primary FROM irregular_stress WHERE form_id = ");
            sStressQuery += CEString::sToString (iId);
            sStressQuery += L";";

            sForm.SetVowels(g_szRusVowels);

            uint64_t uiStressHandle = pDb->uiPrepareForSelect (sStressQuery);
            while (pDb->bGetRow (uiStressHandle))
            {
                CWordForm* pWf = new CWordForm(sHash);
                pWf->m_pLexeme = this;
                pWf->m_bIrregular = true;
                pWf->m_llDbKey = iId;
                pWf->m_sWordForm = sForm;
                pWf->m_llLexemeId = this->llLexemeId();
                pWf->m_bIsVariant = bIsVariant;
                pWf->m_sLeadComment = sLeadComment;
                pWf->m_sTrailingComment = sTrailingComment;

                int iPos = -1;
                bool bPrimary = false;
                pDb->GetData (0, iPos, uiStressHandle);
                pDb->GetData (1, bPrimary, uiStressHandle);
                int iStressedSyll = sForm.uiGetSyllableFromVowelPos (iPos);
                pWf->m_mapStress[iStressedSyll] = bPrimary ? STRESS_PRIMARY : STRESS_SECONDARY;

                StIrregularForm stIf(pWf, bIsVariant);
                pair<CEString, StIrregularForm> pairHashToWordForm(sHash, stIf);
                m_mmapIrregularForms.insert(pairHashToWordForm);
            }
            pDb->Finalize (uiStressHandle);

        }   //  while (pDb->b_GetRow())

        pDb->Finalize(uiQueryHandle);
    }
    catch (CException ex)
    {
        ERROR_LOG (ex.szGetDescription());
        rc = H_EXCEPTION;
    }
    catch (...)
    {
        CEString sMsg;
        CEString sError;
        try
        {
            pDb->GetLastError (sError);
            sMsg += CEString (L", error: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }
    
        sMsg += CEString::sToString(pDb->iGetLastError());
        ERROR_LOG (sMsg);
        rc = H_EXCEPTION;
    }

    return rc;

}   //  eLoadIrregularForms()

ET_ReturnCode CLexeme::eLoadDifficultForms()
{
    ET_ReturnCode rc = H_NO_ERROR;

    if (!m_stProperties.bHasDifficultForms)
    {
        return H_FALSE;
    }

    m_vecDifficultForms.clear();

    CEString sQuery(L"SELECT content FROM difficult_forms WHERE descriptor_id = ");
    sQuery += CEString::sToString(m_stProperties.llDescriptorId);
    sQuery += L";";

    CSqlite* pDb = NULL;

    try
    {
        pDb = m_pDictionary->pGetDbHandle();
        pDb->PrepareForSelect(sQuery);
        while (pDb->bGetRow())
        {
            CEString sLabel;
            pDb->GetData(0, sLabel);
            m_vecDifficultForms.push_back(sLabel);
        }
        pDb->Finalize();
    }
    catch (...)
    {
        CEString sMsg;
        CEString sError;
        try
        {
            pDb->GetLastError(sError);
            sMsg += CEString(L", error %d: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(pDb->iGetLastError());
        ERROR_LOG(sMsg);
    }

    return H_NO_ERROR;

}       //  eLoadDifficultForms()

ET_ReturnCode CLexeme::eLoadMissingForms()
{
    ET_ReturnCode rc = H_NO_ERROR;

    if (!m_stProperties.bHasMissingForms)
    {
        return H_FALSE;
    }

    m_vecMissingForms.clear();

    CEString sQuery(L"SELECT content FROM missing_forms WHERE descriptor_id = ");
    sQuery += CEString::sToString(m_stProperties.llDescriptorId);
    sQuery += L";";

    CSqlite* pDb = NULL;

    try
    {
        pDb = m_pDictionary->pGetDbHandle();
        pDb->PrepareForSelect(sQuery);
        while (pDb->bGetRow())
        {
            CEString sLabel;
            pDb->GetData(0, sLabel);
            m_vecMissingForms.push_back(sLabel);
            if (L"AdjL_*_*_*" == sLabel)
            {
                m_stProperties.bNoLongForms = true;
            }
        }
        pDb->Finalize();
    }
    catch (...)
    {
        CEString sMsg;
        CEString sError;
        try
        {
            pDb->GetLastError(sError);
            sMsg += CEString(L", error %d: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(pDb->iGetLastError());
        ERROR_LOG(sMsg);
    }

     return H_NO_ERROR;

}   //  eLoadMissingForms()

ET_ReturnCode CLexeme::eSaveIrregularForms(long long llDescriptorDbKey)
{
    ET_ReturnCode rc = H_NO_ERROR;

    if (!m_stProperties.bHasIrregularForms)
    {
        return H_FALSE;
    }

    auto pDb = m_pDictionary->pGetDbHandle();
    if (NULL == pDb)
    {
        return H_ERROR_POINTER;
    }

    try
    {
        pDb->BeginTransaction();

        long long llInsertHandle = 0;
        pDb->uiPrepareForInsert(L"irregular_forms", 7, (sqlite3_stmt *&)llInsertHandle, false);

        for (auto pairIf : m_mmapIrregularForms)
        {
            pDb->Bind(1, llDescriptorDbKey, llInsertHandle);
            pDb->Bind(2, pairIf.second.pWordForm->sGramHash(), llInsertHandle);
            pDb->Bind(3, pairIf.second.pWordForm->sWordForm(), llInsertHandle);
            pDb->Bind(4, pairIf.second.bIsOptional, llInsertHandle);
            pDb->Bind(5, pairIf.second.pWordForm->sLeadComment(), llInsertHandle);      //  lead comment
            pDb->Bind(6, pairIf.second.pWordForm->sTrailingComment(), llInsertHandle);  //  trailing comment
            pDb->Bind(7, true, llInsertHandle);     //  is_edited

            pDb->uiPrepareForInsert(L"irregular_stress", 4, (sqlite3_stmt *&)llInsertHandle, false);
            pDb->InsertRow();
            pDb->Finalize();

            long long llId = pDb->llGetLastKey();
            for (auto posToType : pairIf.second.pWordForm->m_mapStress)
            {
                pDb->Bind(1, llId, llInsertHandle);
                int iVowelPos = pairIf.second.pWordForm->sWordForm().uiGetVowelPos(posToType.first);
                pDb->Bind(2, iVowelPos, llInsertHandle);            // position
                pDb->Bind(3, posToType.second, llInsertHandle);     // type
                pDb->Bind(4, true, llInsertHandle);                 // is_edited
                pDb->InsertRow();
                pDb->Finalize();
            }
        }

        pDb->CommitTransaction();
    }
    catch (CException& ex)
    {
        ERROR_LOG(ex.szGetDescription());
        rc = H_ERROR_DB;
    }
    catch (...)
    {
        CEString sMsg;
        CEString sError;
        try
        {
            pDb->GetLastError(sError);
            sMsg += CEString(L", error: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(pDb->iGetLastError());
        ERROR_LOG(sMsg);
        rc = H_EXCEPTION;
    }

    return rc;

}   //  eSaveIrregularForms()

ET_ReturnCode CLexeme::eGenerateParadigm()
{
    ET_ReturnCode rc = H_NO_ERROR;

    try
    {
        if (m_stProperties.bHasIrregularForms || POS_NUM == m_stProperties.ePartOfSpeech)
        {
            rc = eLoadIrregularForms();
            if (rc != H_NO_ERROR && rc != H_FALSE && rc != H_NO_MORE)
            {
//                return rc;
            }
        }

        if (m_stProperties.bHasMissingForms)
        {
            rc = eLoadMissingForms();
            if (rc != H_NO_ERROR && rc != H_FALSE && rc != H_NO_MORE)
            {
                return rc;
            }
        }

        if (m_stProperties.bHasDifficultForms)
        {
            rc = eLoadDifficultForms();
            if (rc != H_NO_ERROR && rc != H_FALSE && rc != H_NO_MORE)
            {
                return rc;
            }
        }

        m_mmWordForms.clear();

        if (L"мо" == m_stProperties.sInflectionType || L"м" == m_stProperties.sInflectionType || L"жо" == m_stProperties.sInflectionType
            || L"ж" == m_stProperties.sInflectionType || L"со" == m_stProperties.sInflectionType || L"с" == m_stProperties.sInflectionType
            || L"мо-жо" == m_stProperties.sInflectionType || L"мн." == m_stProperties.sInflectionType || L"мн. неод." == m_stProperties.sInflectionType
            || L"мн. одуш." == m_stProperties.sInflectionType || L"мн. от" == m_stProperties.sInflectionType)
        {
            CFormBuilderNouns bn (this);
            rc = bn.eBuild();
            if (m_stProperties.bIsSecondPart)
            {
                rc = eAlignInflectedParts();
            }
        }
            
        if (L"п" == m_stProperties.sInflectionType)
        {
            if (!m_stProperties.bNoLongForms)
            {
                CFormBuilderLongAdj lfb(this, m_stProperties.sGraphicStem, m_stProperties.eAccentType1, SUBPARADIGM_LONG_ADJ);
                rc = lfb.eBuild();
                if (rc != H_NO_ERROR)
                {
                    return rc;
                }
            }

            if (L"п" == m_stProperties.sMainSymbol)
            {
                CFormBuilderShortAdj sfb(this);
                rc = sfb.eBuild();
                if (rc != H_NO_ERROR)
                {
                    return rc;
                }

                if (!m_stProperties.bNoComparative && !m_stProperties.sSourceForm.bEndsWith(L"ийся"))
                {
                    CFormBuilderComparative cfb(this);
                    rc = cfb.eBuild();
                }
            }
        }

        if (L"мс-п" == m_stProperties.sInflectionType || L"числ.-п" == m_stProperties.sInflectionType || 
           (L"мс" == m_stProperties.sInflectionType && L"п" == m_stProperties.sMainSymbol))
        {
            CFormBuilderPronounAdj pab (this, m_stProperties.sGraphicStem, m_stProperties.eAccentType1);
            rc = pab.eBuild();
        }
            
        if (m_stProperties.ePartOfSpeech == POS_VERB)
        {
            m_stProperties.eAspect = ASPECT_UNDEFINED;
//            if (L"нсв" == str_MainSymbol)
            if (L"нсв" == m_stProperties.sInflectionType)
            {
                m_stProperties.eAspect = ASPECT_IMPERFECTIVE;
            }
            else if (L"св" == m_stProperties.sInflectionType)
            {
                m_stProperties.eAspect = ASPECT_PERFECTIVE;
            }

            CFormBuilderPersonal personal(this);
            rc = personal.eBuild();
            //                if (rc != H_NO_ERROR)
            //                {
            //                    return rc;
            //                }

            CFormBuilderPast past (this);
            rc = past.eBuild();
//                if (rc != H_NO_ERROR)
//                {
//                    return rc;
//                }

            CFormBuilderImperative imperative (this);
            rc = imperative.eBuild();
//                if (rc != H_NO_ERROR)
//                {
//                    return rc;
//                }

            CFormBuilderNonFinite nonFinite (this);
            rc = nonFinite.eBuild();

            if (bHasAspectPair())
            {
                CFormBuilderAspectPair aspectPair(this);
                rc = aspectPair.eBuild();
                if (H_NO_ERROR == rc)
                {
                    rc = aspectPair.eGetAspectPair(m_sAspectPair, m_iAspectPairStressPos);
                    if (H_NO_ERROR == rc && bHasAltAspectPair())
                    {
                        rc = aspectPair.eGetAltAspectPair(m_sAltAspectPair, m_iAltAspectPairStressPos);
                    }
                }
            }
        }
            
        if (POS_PRONOUN == m_stProperties.ePartOfSpeech || POS_NUM == m_stProperties.ePartOfSpeech)
        {
            CWordForm * pWordForm = NULL;
            bool bOptional = false;
            rc = eGetFirstIrregularForm(pWordForm, bOptional);
            while (H_NO_ERROR == rc)
            {
                AddWordForm(pWordForm);
                rc = eGetNextIrregularForm(pWordForm, bOptional);
            }
        }

        if (1 == m_stProperties.iSection || 2 == m_stProperties.iSection)
        {
            m_stProperties.ePartOfSpeech = POS_NULL;
        }

        if (POS_PRONOUN_PREDIC == m_stProperties.ePartOfSpeech || POS_ADV == m_stProperties.ePartOfSpeech 
            || POS_COMPAR == m_stProperties.ePartOfSpeech || POS_PREDIC == m_stProperties.ePartOfSpeech 
            || POS_PREPOSITION == m_stProperties.ePartOfSpeech || POS_CONJUNCTION == m_stProperties.ePartOfSpeech 
            || POS_PARTICLE == m_stProperties.ePartOfSpeech || POS_INTERJ == m_stProperties.ePartOfSpeech 
            || POS_PARENTH == m_stProperties.ePartOfSpeech || POS_NULL == m_stProperties.ePartOfSpeech)
        {
            CWordForm * pWordForm = new CWordForm;
            pWordForm->m_pLexeme = this;
            pWordForm->m_ePos = m_stProperties.ePartOfSpeech;
            pWordForm->m_sStem = m_stProperties.sGraphicStem;
            pWordForm->m_sWordForm = m_stProperties.sGraphicStem;
            pWordForm->m_llLexemeId = this->llLexemeId();
            for (vector<int>::iterator itStress = m_stProperties.vecSourceStressPos.begin();
                    itStress != m_stProperties.vecSourceStressPos.end(); ++itStress)
            {
                pWordForm->m_mapStress[*itStress] = STRESS_PRIMARY;
            }

            pair<CEString, CWordForm *> pairHashToWf(pWordForm->sGramHash(), pWordForm);
            m_mmWordForms.insert(pairHashToWf);
        }
    }
    catch (CException& ex)
    {
        CEString sMsg(ex.szGetDescription());
        sMsg += L" lexeme = ";
        sMsg += m_stProperties.sSourceForm;
        return H_EXCEPTION;
    }
    catch (...)
    {
        ERROR_LOG(L"Unknown exception.");
    }

	m_bFormsGenerated = true;

    return H_NO_ERROR;

}   // eGenerateParadigm()

ET_ReturnCode CLexeme::eSaveTestData()
{
    if (m_mmWordForms.empty())
    {
        return H_FALSE;
    }

    ET_ReturnCode eRet = H_NO_ERROR;

    CSqlite * pDbHandle = NULL;
    pDbHandle = m_pDictionary->pGetDbHandle();

    pDbHandle->BeginTransaction();

    try
    {
        CEString sSelectQuery = L"SELECT td.id FROM test_data AS td WHERE td.lexeme_id = \'";
        sSelectQuery += sHash();
        sSelectQuery += L"\'; ";

        long long llTestDataId = -1;
        pDbHandle->PrepareForSelect(sSelectQuery);
        vector <long long> vecTestDataIds;
        while (pDbHandle->bGetRow())
        {
            pDbHandle->GetData(0, llTestDataId);
            vecTestDataIds.push_back(llTestDataId);

        }

        CEString sDeleteQuery = L"DELETE FROM test_data_stress WHERE test_data_id = ";
        for (auto llId : vecTestDataIds)
        {
            pDbHandle->Delete(sDeleteQuery + CEString::sToString(llId) + L"; ");
        }

        sDeleteQuery = L"DELETE FROM test_data WHERE lexeme_id = \'";
        sDeleteQuery += sHash();
        sDeleteQuery += L"\' ";
        pDbHandle->Delete(sDeleteQuery);

        auto itWf = m_mmWordForms.begin();
        for (; itWf != m_mmWordForms.end(); ++itWf)
        {
            ET_ReturnCode eRet = (*itWf).second->eSaveTestData();
            if (Hlib::CErrorCode::bError(eRet))
            {
                pDbHandle->RollbackTransaction();
                return eRet;
                //            eRet = H_ERROR_GENERAL;
            }
        }       //  for (...)
    }
    catch (CException& exc)
    {
        CEString sMsg(exc.szGetDescription());
        CEString sError;
        try
        {
            pDbHandle->GetLastError(sError);
            sMsg += CEString(L", error %d: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(pDbHandle->iGetLastError());
        ERROR_LOG(sMsg);
    }

    try
    {
        pDbHandle = m_pDictionary->pGetDbHandle();

        CEString sDeleteQuery(L"DELETE FROM lexeme_hash_to_descriptor WHERE descriptor_id = ");
        sDeleteQuery += CEString::sToString(m_stProperties.llDescriptorId);
        pDbHandle->Exec(sDeleteQuery);

        pDbHandle->PrepareForInsert(L"lexeme_hash_to_descriptor", 3);
        pDbHandle->Bind(1, sHash());
        pDbHandle->Bind(2, m_stProperties.llDescriptorId);
        pDbHandle->Bind(3, m_stProperties.iInflectionId);
        pDbHandle->InsertRow();
        pDbHandle->Finalize();
    }
    catch (CException& exc)
    {
        CEString sMsg(exc.szGetDescription());
        CEString sError;
        try
        {
            pDbHandle->GetLastError(sError);
            sMsg += CEString(L", error %d: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(pDbHandle->iGetLastError());
        ERROR_LOG(sMsg);
    }

    if (bHasAspectPair())
    {
        CEString sAspectPairHash(L"AspectPair");

        pDbHandle->PrepareForInsert(L"test_data", 3);
        pDbHandle->Bind(1, sHash());    // lexeme hash
        pDbHandle->Bind(2, sAspectPairHash);
        pDbHandle->Bind(3, m_sAspectPair);
        pDbHandle->InsertRow();
        pDbHandle->Finalize();
        auto llId = pDbHandle->llGetLastKey();

        pDbHandle->PrepareForInsert(L"test_data_stress", 3);
        pDbHandle->Bind(1, llId);
        pDbHandle->Bind(2, m_iAspectPairStressPos);
        ET_StressType eType = STRESS_PRIMARY;
        pDbHandle->Bind(3, eType);
        pDbHandle->InsertRow();
        pDbHandle->Finalize();

        if (bHasAltAspectPair())
        {
            CEString sAltAspectPairHash(L"AltAspectPair");

            pDbHandle->PrepareForInsert(L"test_data", 3);
            pDbHandle->Bind(1, sHash());    // lexeme hash
            pDbHandle->Bind(2, sAltAspectPairHash);
            pDbHandle->Bind(3, m_sAltAspectPair);
            pDbHandle->InsertRow();
            pDbHandle->Finalize();
            llId = pDbHandle->llGetLastKey();

            pDbHandle->PrepareForInsert(L"test_data_stress", 3);
            pDbHandle->Bind(1, llId);
            pDbHandle->Bind(2, m_iAltAspectPairStressPos);
            ET_StressType eType = STRESS_PRIMARY;
            pDbHandle->Bind(3, eType);
            pDbHandle->InsertRow();
            pDbHandle->Finalize();
        }
    }

    pDbHandle->CommitTransaction();

	return H_NO_ERROR;

}   // eSaveTestData()

ET_ReturnCode CLexeme::eDeleteIrregularForm(const CEString& sFormHash)
{
    // Delete irregular form and variants
    auto itRange = m_mmapIrregularForms.equal_range(sFormHash);
    if (itRange.first == itRange.second)
    {
        return H_FALSE;
    }
    
    CSqlite * pDbHandle = nullptr;
    for (auto itCurrentForm = itRange.first; itCurrentForm != itRange.second; ++itCurrentForm)
    {
        try
        {
            CEString sSelectIrregularForms(L"SELECT id FROM irregular_forms WHERE descriptor_id=\"");
            sSelectIrregularForms += CEString::sToString(m_stProperties.llDescriptorId);
            sSelectIrregularForms += L"\" AND gram_hash = \"";
            sSelectIrregularForms += sFormHash;
            sSelectIrregularForms += L"\"";

            pDbHandle = pGetDb();
            pDbHandle->PrepareForSelect(sSelectIrregularForms);
            long long llDbKey = 0;
            while (pDbHandle->bGetRow())
            {
                pDbHandle->GetData(0, (unsigned __int64&)llDbKey);
                CEString sDeleteForms(L"DELETE FROM irregular_forms WHERE id = \"");
                sDeleteForms += CEString::sToString(llDbKey);
                sDeleteForms += L"\"";
                pDbHandle->Exec(sDeleteForms);

                CEString sDeleteStress(L"DELETE FROM irregular_stress WHERE form_id = \"");
                sDeleteStress += CEString::sToString(llDbKey);
                sDeleteStress += L"\"";
                pDbHandle->Exec(sDeleteStress);
            }
        }
        catch (CException ex)
        {
            ERROR_LOG(ex.szGetDescription());
            return H_EXCEPTION;
        }
        catch (...)
        {
            CEString sMsg;
            CEString sError;
            try
            {
                pDbHandle->GetLastError(sError);
                sMsg += CEString(L", error: ");
                sMsg += sError;
            }
            catch (...)
            {
                sMsg = L"Apparent DB error ";
            }

            sMsg += CEString::sToString(pDbHandle->iGetLastError());
            ERROR_LOG(sMsg);
            return H_EXCEPTION;
        }
    }

    m_mmapIrregularForms.erase(sFormHash);

    if (m_mmapIrregularForms.empty())
    {
        m_stProperties.bHasIrregularForms = false;
        try
        {
            vector<CEString> vecColumns = { L"has_irregular_forms" };
            pDbHandle->PrepareForUpdate(L"descriptor", vecColumns, m_stProperties.llDescriptorId);
            pDbHandle->Bind(1, false);

            pDbHandle->UpdateRow();
            pDbHandle->Finalize();
        }
        catch (CException& exc)
        {
            CEString sMsg(exc.szGetDescription());
            CEString sError;
            try
            {
                pDbHandle->GetLastError(sError);
                sMsg += CEString(L", error %d: ");
                sMsg += sError;
            }
            catch (...)
            {
                sMsg = L"Apparent DB error ";
            }

            sMsg += CEString::sToString(pDbHandle->iGetLastError());
            ERROR_LOG(sMsg);

            return H_ERROR_DB;
        }
    }

    return H_NO_ERROR;

}       //  eDeleteIrregularForm()

ET_ReturnCode CLexeme::eSaveIrregularForm(const CEString& sFormHash, IWordForm *& pWordForm)
{
    CEString sSelectIrregularForms(L"SELECT id FROM irregular_forms WHERE descriptor_id=\"");
    sSelectIrregularForms += CEString::sToString(m_stProperties.llDescriptorId);
    sSelectIrregularForms += L"\" AND gram_hash = \"";
    sSelectIrregularForms += sFormHash;
    sSelectIrregularForms += L"\"";

    CSqlite * pDbHandle = pGetDb();
    CEString sLeadComment;
    CEString sTrailingComment;

    try
    {
        bool bIgnoreOnConflict = true;
        pDbHandle->PrepareForInsert(L"irregular_forms", 7, bIgnoreOnConflict);
        pDbHandle->Bind(1, m_stProperties.llDescriptorId);
        pDbHandle->Bind(2, sFormHash);
        pDbHandle->Bind(3, pWordForm->sWordForm());
        pDbHandle->Bind(4, pWordForm->bIsVariant());
        pDbHandle->Bind(5, pWordForm->sLeadComment());            // TODO
        pDbHandle->Bind(6, pWordForm->sTrailingComment());        // TODO
        pDbHandle->Bind(7, true);                                 // is_edited

        pDbHandle->InsertRow();
        pDbHandle->Finalize();

        long long llFormKey = pDbHandle->llGetLastKey();

        int iStressPos = -1;
        ET_StressType eStressType = ET_StressType::STRESS_TYPE_UNDEFINED;
        ET_ReturnCode eRet = pWordForm->eGetFirstStressPos(iStressPos, eStressType);        
        while (H_NO_ERROR == eRet)
        {
            pDbHandle->PrepareForInsert(L"irregular_stress", 4, bIgnoreOnConflict);
            pDbHandle->Bind(1, llFormKey);
            pDbHandle->Bind(2, iStressPos);
            bool bIsPrimary = (ET_StressType::STRESS_PRIMARY == eStressType) ? true : false;
            pDbHandle->Bind(3, bIsPrimary);
            pDbHandle->Bind(4, true);       // is_edited

            pDbHandle->InsertRow();
            pDbHandle->Finalize();

            eRet = pWordForm->eGetNextStressPos(iStressPos, eStressType);
        }

        if (eRet != ET_ReturnCode::H_NO_MORE)
        {
            CEString sMsg(L"Error ");
            sMsg += CEString::sToString(eRet);
            sMsg += L" while reading stress data.";
            ERROR_LOG(sMsg);
        }
    }
    catch (CException& exc)
    {
        CEString sMsg(exc.szGetDescription());
        CEString sError;
        try
        {
            pDbHandle->GetLastError(sError);
            sMsg += CEString(L", error %d: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(pDbHandle->iGetLastError());
        ERROR_LOG(sMsg);

        return H_ERROR_DB;
    }

    try
    {
        vector<CEString> vecColumns = { L"has_irregular_forms", L"is_edited" };
        pDbHandle->PrepareForUpdate(L"descriptor", vecColumns, m_stProperties.llDescriptorId);
        pDbHandle->Bind(1, true);
        pDbHandle->Bind(2, true);

        pDbHandle->UpdateRow();
        pDbHandle->Finalize();
    }
    catch (CException& exc)
    {
        CEString sMsg(exc.szGetDescription());
        CEString sError;
        try
        {
            pDbHandle->GetLastError(sError);
            sMsg += CEString(L", error %d: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(pDbHandle->iGetLastError());
        ERROR_LOG(sMsg);

        return H_ERROR_DB;
    }

    return H_NO_ERROR;

}       //  eSaveIrregularForm()

ET_ReturnCode CLexeme::eSaveIrregularForms(const CEString& sGramHash)
{
    ET_ReturnCode rc = H_NO_ERROR;

    auto pDb = m_pDictionary->pGetDbHandle();
    if (NULL == pDb)
    {
        return H_ERROR_POINTER;
    }

    try
    {
//        pDb->BeginTransaction();

        CEString sStressDelQuery(L"DELETE FROM irregular_stress WHERE form_id IN (SELECT id FROM irregular_forms WHERE descriptor_id = ");
        sStressDelQuery += CEString::sToString(m_stProperties.llDescriptorId);
        sStressDelQuery += L" AND gram_hash = '";
        sStressDelQuery += sGramHash + L"')";
        pDb->Delete(sStressDelQuery);

        CEString sDelQuery(L"DELETE FROM irregular_forms WHERE descriptor_id = ");
        sDelQuery += CEString::sToString(m_stProperties.llDescriptorId);
        sDelQuery += L" AND gram_hash = '";
        sDelQuery += sGramHash + L"'";
        pDb->Delete(sDelQuery);

        long long llFormInsertHandle = 0;
        pDb->uiPrepareForInsert(L"irregular_forms", 7, (sqlite3_stmt*&)llFormInsertHandle, false);

        auto pairFormsForHash = m_mmapIrregularForms.equal_range(sGramHash);
        for (auto it = pairFormsForHash.first; it != pairFormsForHash.second; ++it)
        {
            pDb->Bind(1, m_stProperties.llDescriptorId, llFormInsertHandle);
            pDb->Bind(2, sGramHash, llFormInsertHandle);
            pDb->Bind(3, it->second.pWordForm->m_sWordForm, llFormInsertHandle);
            pDb->Bind(4, it->second.bIsOptional, llFormInsertHandle);
            pDb->Bind(5, it->second.pWordForm->m_sLeadComment, llFormInsertHandle);
            pDb->Bind(6, it->second.pWordForm->m_sTrailingComment, llFormInsertHandle);
            pDb->Bind(7, true, llFormInsertHandle);     //  is_edited

            pDb->InsertRow(llFormInsertHandle);
            long long llFormId = pDb->llGetLastKey(llFormInsertHandle);

            long long llStressInsertHandle = 0;
            pDb->uiPrepareForInsert(L"irregular_stress", 4, (sqlite3_stmt*&)llStressInsertHandle, false);

            for (auto pairPosToType : it->second.pWordForm->m_mapStress)
            {
                pDb->Bind(1, llFormId, llStressInsertHandle);
                int iVowelPos = it->second.pWordForm->sWordForm().uiGetVowelPos(pairPosToType.first);
                pDb->Bind(2, iVowelPos, llStressInsertHandle);                // char position
                pDb->Bind(3, pairPosToType.second, llStressInsertHandle);     // type
                pDb->Bind(4, true, llStressInsertHandle);                     // is_edited
                pDb->InsertRow(llStressInsertHandle);
            }

            pDb->Finalize(llStressInsertHandle);
        }

        pDb->Finalize(llFormInsertHandle);


        m_stProperties.bHasIrregularForms = true;
        vector<CEString> vecColumns = { L"has_irregular_forms", L"is_edited" };
        pDb->PrepareForUpdate(L"descriptor", vecColumns, m_stProperties.llDescriptorId);
        pDb->Bind(1, true);
        pDb->Bind(2, true);

        pDb->UpdateRow();
        pDb->Finalize();

  //      pDb->CommitTransaction();
        
    }
    catch (CException & ex)
    {
        ERROR_LOG(ex.szGetDescription());
        rc = H_ERROR_DB;
    }
    catch (...)
    {
        CEString sMsg;
        CEString sError;
        try
        {
            pDb->GetLastError(sError);
            sMsg += CEString(L", error: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(pDb->iGetLastError());
        ERROR_LOG(sMsg);
        rc = H_EXCEPTION;
    }

    return rc;

}   //  eSaveIrregularForms()

ET_ReturnCode CLexeme::eClone(ILexeme *& pClonedObject)
{
    pClonedObject = new CLexeme(const_cast<const CLexeme&>(*this));
    return H_NO_ERROR;
}

ET_ReturnCode CLexeme::eCheckLexemeProperties()
{
    return eInitializeFromProperties();
}

ET_ReturnCode CLexeme::eGetErrorMsg(CEString& sErrorMsg)
{
    sErrorMsg = m_sLastErrorMsg;
    return sErrorMsg.bIsEmpty() ? H_FALSE : H_NO_ERROR;
}

CSqlite * CLexeme::pGetDb()
{
    CSqlite * pDb = m_pDictionary->pGetDbHandle();
    if (!pDb)
    {
        throw CException (H_ERROR_POINTER, L"No database handle.");
    }
    return pDb;
}

CEString CLexeme::sHash()
{
    CEString sSource(m_stProperties.sSourceForm);
    vector<int>::iterator itHomonym = m_stProperties.vecHomonyms.begin();
    for (; itHomonym != m_stProperties.vecHomonyms.end(); ++itHomonym)
    {
        sSource += CEString::sToString (*itHomonym);
    }

    auto itStress = m_stProperties.vecSourceStressPos.begin();
    for (; itStress != m_stProperties.vecSourceStressPos.end(); ++itStress)
    {
        auto iPos = *itStress;
        sSource += 32 + iPos;
    }

    itStress = m_stProperties.vecSecondaryStressPos.begin();
    for (; itStress != m_stProperties.vecSecondaryStressPos.end(); ++itStress)
    {
        auto iPos = *itStress;
        sSource += 32 + iPos;
    }

    sSource += m_stProperties.sMainSymbol;
    sSource += 32 + m_stProperties.iType;
    sSource += 32 + m_stProperties.eAccentType1;
    sSource += 32 + m_stProperties.eAccentType2;

    auto charIsSecondPart = m_stProperties.bIsSecondPart ? '1' : '0';
    if ('1' == charIsSecondPart)
    {
        sSource += charIsSecondPart;
    }

    sSource += m_stProperties.sHeadwordComment;
    sSource += m_stProperties.sComment;
    sSource += m_stProperties.sTrailingComment;

    CMD5 md5;
    return md5.sHash(sSource);

}   //  sHash()

CEString CLexeme::sParadigmHash()
{
    CEString sSource(m_stProperties.sSourceForm);
    vector<int>::iterator itHomonym = m_stProperties.vecHomonyms.begin();
    for (; itHomonym != m_stProperties.vecHomonyms.end(); ++itHomonym)
    {
        sSource += CEString::sToString(*itHomonym);
    }

    auto itStress = m_stProperties.vecSourceStressPos.begin();
    for (; itStress != m_stProperties.vecSourceStressPos.end(); ++itStress)
    {
        auto iPos = *itStress;
        sSource += 32 + iPos;
    }

    itStress = m_stProperties.vecSecondaryStressPos.begin();
    for (; itStress != m_stProperties.vecSecondaryStressPos.end(); ++itStress)
    {
        auto iPos = *itStress;
        sSource += 32 + iPos;
    }

    sSource += m_stProperties.sMainSymbol;
    sSource += m_stProperties.sInflectionType;
    sSource += m_stProperties.sAltMainSymbol;
    sSource += 32 + m_stProperties.iType;
    sSource += 32 + m_stProperties.eAccentType1;
    sSource += 32 + m_stProperties.eAccentType2;

    auto charIsSecondPart = m_stProperties.bIsSecondPart ? '1' : '0';
    if ('1' == charIsSecondPart)
    {
        sSource += charIsSecondPart;
    }

    sSource += m_stProperties.sHeadwordComment;
    sSource += m_stProperties.sComment;
    sSource += m_stProperties.sTrailingComment;

    sSource += m_stProperties.bFleetingVowel ? '1' : '0';
    sSource += m_stProperties.bNoComparative ? '1' : '0';
    sSource += m_stProperties.bNoLongForms ? '1' : '0';
    sSource += m_stProperties.bYoAlternation ? '1' : '0';
    sSource += m_stProperties.bOAlternation ? '1' : '0';
    sSource += 32 + m_stProperties.ePartOfSpeech;
    sSource += m_stProperties.bPartPastPassZhd ? '1' : '0';
    sSource += 32 + m_stProperties.iSection;
    sSource += 32 + m_stProperties.iStemAugment;
    for (auto pairCd : m_stProperties.mapCommonDeviations)
    {
        sSource += 32 + pairCd.first;
        sSource += pairCd.second ? '1' : '0';
    }

    CMD5 md5;
    return md5.sHash(sSource);

}   //  sParadigmHash()

ET_ReturnCode CLexeme::eGetFirstWordForm (IWordForm *& pWf)
{
    m_itCurrentWordForm = m_mmWordForms.begin();
    if (m_mmWordForms.end() == m_itCurrentWordForm)
    {
        pWf = NULL;
        return H_FALSE;
    }
    pWf = m_itCurrentWordForm->second;
    return H_NO_ERROR;

}

ET_ReturnCode CLexeme::eGetNextWordForm (IWordForm *& pWf)
{
    if (m_mmWordForms.end() != m_itCurrentWordForm)
    {
        ++m_itCurrentWordForm;
    }
    if (m_mmWordForms.end() == m_itCurrentWordForm)
    {
        pWf = NULL;
        return H_NO_MORE;
    }

    pWf = m_itCurrentWordForm->second;
    return H_NO_ERROR;
}

ET_ReturnCode CLexeme::eGetFirstIrregularForm (CEString sHash, IWordForm*& pIWordForm, bool& bIsOptional)
{
    CWordForm * pCWordForm = NULL;
    ET_ReturnCode rc = eGetFirstIrregularForm (sHash, pCWordForm, bIsOptional);
    if (H_NO_ERROR == rc)
    {
        pIWordForm = pCWordForm;
    }
    return rc;
}

ET_ReturnCode CLexeme::eGetFirstIrregularForm (CEString sHash, CWordForm*& pWordForm, bool& bIsOptional)
{
    m_pairItIfRange = m_mmapIrregularForms.equal_range (sHash);
    if (m_pairItIfRange.first == m_pairItIfRange.second)
    {
        return H_FALSE;
    }
    else
    {
        m_itCurrentIrregularForm = m_pairItIfRange.first;
        if ((*m_itCurrentIrregularForm).first != sHash)
        {
            return H_FALSE;
        }
        else
        {
            pWordForm = (*m_itCurrentIrregularForm).second.pWordForm;
            bIsOptional = (*m_itCurrentIrregularForm).second.bIsOptional;
            return H_NO_ERROR;
        }
    }
}

ET_ReturnCode CLexeme::eGetFirstIrregularForm(IWordForm *& pIWordForm, bool& bIsOptional)
{
    CWordForm * pCWordForm = NULL;
    ET_ReturnCode rc = eGetFirstIrregularForm(pCWordForm, bIsOptional);
    if (H_NO_ERROR == rc)
    {
        pIWordForm = pCWordForm;
    }
    return rc;
}

ET_ReturnCode CLexeme::eGetFirstIrregularForm(CWordForm *& pWordForm, bool& bIsOptional)
{
    m_pairItIfRange.first = m_mmapIrregularForms.begin();
    m_pairItIfRange.second = m_mmapIrregularForms.end();
    if (m_pairItIfRange.first == m_pairItIfRange.second)
    {
        return H_FALSE;
    }
    else
    {
        m_itCurrentIrregularForm = m_pairItIfRange.first;
        pWordForm = (*m_itCurrentIrregularForm).second.pWordForm;
        bIsOptional = (*m_itCurrentIrregularForm).second.bIsOptional;
        return H_NO_ERROR;
    }
}

ET_ReturnCode CLexeme::eGetNextIrregularForm (IWordForm *& pIWordForm, bool& bIsOptional)
{
    CWordForm * pCWordForm = NULL;
    ET_ReturnCode rc = eGetNextIrregularForm (pCWordForm, bIsOptional);
    if (H_NO_ERROR == rc)
    {
        pIWordForm = pCWordForm;
    }
    return rc;
}

ET_ReturnCode CLexeme::eGetNextIrregularForm (CWordForm *& pWordForm, bool& bIsOptional)
{
    if (m_pairItIfRange.first == m_pairItIfRange.second)
    {
        return H_ERROR_UNEXPECTED;
    }

//    if (iHash != (*pairItIfRange.first).first)
//    {
//        return H_ERROR_UNEXPECTED;
//    }

    if (m_itCurrentIrregularForm == m_mmapIrregularForms.end())
//        || (*m_itCurrentIrregularForm).first != iHash))
    {
        return H_ERROR_UNEXPECTED;
    }

    ++m_itCurrentIrregularForm;

    if (m_itCurrentIrregularForm == m_mmapIrregularForms.end() || m_itCurrentIrregularForm == m_pairItIfRange.second)
//        || (*m_itCurrentIrregularForm).first != iHash))
    {
        return H_NO_MORE;
    }

    if ((*m_itCurrentIrregularForm).first != (*m_itCurrentIrregularForm).second.pWordForm->sGramHash())
    {
        ERROR_LOG(L"Irregular form: gramm hash does not match.");
        return H_ERROR_UNEXPECTED;
    }

    pWordForm = (*m_itCurrentIrregularForm).second.pWordForm;
    bIsOptional = (*m_itCurrentIrregularForm).second.bIsOptional;

    return H_NO_ERROR;

}   //  eGetNextIrregularForm (...)

ET_ReturnCode CLexeme::eGetSourceFormWithStress(CEString& sSourceForm, bool bIsVariant)
{
    CEString sRet = bIsVariant ? m_stProperties.sHeadwordVariant : m_stProperties.sSourceForm;
    vector<int> vecStressedSyllables = bIsVariant ? m_stProperties.vecSourceVariantStressPos : m_stProperties.vecSourceStressPos;
    vector<int> vecSecondaryStressedSyllables = bIsVariant ? m_stProperties.vecSecondaryVariantStressPos : m_stProperties.vecSecondaryStressPos;

    map<int, bool> mapStressedVowelPositions;

    try
    {
        for (auto iSyll : vecStressedSyllables)
        {
            int iVowelPos = m_stProperties.sSourceForm.uiGetVowelPos(iSyll);
            mapStressedVowelPositions[iVowelPos] = true;
        }

        for (auto iSyll : vecSecondaryStressedSyllables)
        {
            int iVowelPos = m_stProperties.sSourceForm.uiGetVowelPos(iSyll);
            mapStressedVowelPositions[iVowelPos] = false;
        }

        int iIncrement = 0;
        for (auto&& pairVowelPos : mapStressedVowelPositions)
        {
            if (pairVowelPos.second)
            {
                sRet.sInsert((unsigned int)(pairVowelPos.first + iIncrement), L'/');
                ++iIncrement;
            }
            else
            {
                sRet.sInsert((unsigned int)(pairVowelPos.first + iIncrement), L'\\');
                ++iIncrement;
            }
        }
    }
    catch (CException& ex)
    {
        ERROR_LOG(ex.szGetDescription());
        return H_EXCEPTION;
    }
    catch (...)
    {
        ERROR_LOG(L"Unknown exception.");
    }

    sSourceForm = sRet;

    return H_NO_ERROR;

}   //  eGetSourceFormWithStress()

ET_ReturnCode CLexeme::eGetFirstStemStressPos (int& iPos)
{
    m_itCurrentStressPos = m_stProperties.vecSourceStressPos.begin();
    if (m_stProperties.vecSourceStressPos.end() == m_itCurrentStressPos)
    {
        iPos = -1;
        return H_FALSE;
    }
    
    iPos = *m_itCurrentStressPos;

    return H_NO_ERROR;
}

ET_ReturnCode CLexeme::eGetNextStemStressPos (int& iPos)
{
    if (m_itCurrentStressPos != m_stProperties.vecSourceStressPos.end())
    {
        ++m_itCurrentStressPos;
    }

    if (m_stProperties.vecSourceStressPos.end() == m_itCurrentStressPos)
    {
        iPos = -1;
        return H_NO_MORE;
    }

    iPos = *m_itCurrentStressPos;

    return H_NO_ERROR;
}

ET_ReturnCode CLexeme::eGetFirstSecondaryStemStressPos(int& iPos)
{
    m_itCurrentStressPos = m_stProperties.vecSecondaryStressPos.begin();
    if (m_stProperties.vecSecondaryStressPos.end() == m_itCurrentStressPos)
    {
        iPos = -1;
        return H_FALSE;
    }

    iPos = *m_itCurrentStressPos;

    return H_NO_ERROR;
}

ET_ReturnCode CLexeme::eGetNextSecondaryStemStressPos(int& iPos)
{
    if (m_itCurrentStressPos != m_stProperties.vecSecondaryStressPos.end())
    {
        ++m_itCurrentStressPos;
    }

    if (m_stProperties.vecSecondaryStressPos.end() == m_itCurrentStressPos)
    {
        iPos = -1;
        return H_NO_MORE;
    }

    iPos = *m_itCurrentStressPos;

    return H_NO_ERROR;
}

ET_ReturnCode CLexeme::eGetAspectPair(CEString& sAspectPair, int& iStressPos)
{
    if (!bHasAspectPair())
    {
        return H_ERROR_UNEXPECTED;
    }

    sAspectPair = m_sAspectPair;
    iStressPos = m_iAspectPairStressPos;

    return H_NO_ERROR;
}

ET_ReturnCode CLexeme::eGetAltAspectPair(CEString& sAltAspectPair, int& iAltStressPos)
{
    if (!bHasAltAspectPair())
    {
        return H_ERROR_UNEXPECTED;
    }

    sAltAspectPair = m_sAspectPair;
    iAltStressPos = m_iAspectPairStressPos;

    return H_NO_ERROR;
}

ET_ReturnCode CLexeme::eAlignInflectedParts()
{
    if (!m_pDictionary)
    {
        ERROR_LOG(L"No dictionary.");
        return H_ERROR_POINTER;
    }

    if (!m_stProperties.bIsSecondPart)
    {
        return H_NO_ERROR;
    }

    ET_ReturnCode eRet = H_NO_ERROR;

    CLexeme * pOther = nullptr;

    CLexemeEnumerator le(m_pDictionary);

    eRet =  le.eGetFirstLexeme(pOther);

    CLexeme * pLeft = nullptr;
    while (H_NO_ERROR == eRet)
    {
        if (!pOther)
        {
            ERROR_LOG(L"Lexeme object is null.");
            return H_ERROR_POINTER;
        }

        if (pOther != this && pOther->m_stProperties.llDescriptorId == m_stProperties.llDescriptorId)
        {
            if (pOther->m_stProperties.bIsSecondPart)
            {
                ERROR_LOG(L"Another second part of an inflected compound?");
                return H_ERROR_UNEXPECTED;

            }

            pLeft = pOther;
            break;
        }

        eRet = le.eGetNextLexeme(pOther);
    }
    
    if (!pLeft)
    {
        ERROR_LOG(L"Unable to find left side of an inflected compound.");
        return H_ERROR_UNEXPECTED;
    }

    if (m_mmWordForms.size() != pLeft->m_mmWordForms.size())
    {
        ERROR_LOG(L"Unable to match second part wordforms: sizes are different.");
        return H_ERROR_UNEXPECTED;
    }

    for (auto keyValPairLeft = pLeft->m_mmWordForms.begin(); 
         keyValPairLeft != pLeft->m_mmWordForms.end(); 
         keyValPairLeft = pLeft->m_mmWordForms.upper_bound((*keyValPairLeft).first))        // next gramm hash
    {
        auto sGramHash = (*keyValPairLeft).first;
        auto pairLeftValueRange = pLeft->m_mmWordForms.equal_range(sGramHash);   // get left and right word forms for the same gram hash
        auto pairRightValueRange = m_mmWordForms.equal_range(sGramHash);
        int iCountLeft = 0;
        for (auto itValueLeft = pairLeftValueRange.first; itValueLeft != pairLeftValueRange.second; ++itValueLeft, ++iCountLeft)
        { 
            int iCountRight = 0;
            for (auto itValueRight = pairRightValueRange.first; itValueRight != pairRightValueRange.second; ++itValueRight, ++iCountRight)
            {
                if (iCountLeft == iCountRight)
                {
                    CWordForm* pWfLeft = (*itValueLeft).second;
                    CWordForm* pWfRight = (*itValueRight).second;

                    int iStressPos = -1;
                    ET_StressType eStressType = ET_StressType::STRESS_TYPE_UNDEFINED;
                    for (auto pairPosType : pWfRight->m_mapStress)
                    { 
                        auto sLeft = pWfLeft->sWordForm();
                        sLeft.SetVowels(g_szRusVowels);
                        auto sRight = pWfRight->sWordForm();
                        sRight.SetVowels(g_szRusVowels);
                        auto iPos = pairPosType.first + sLeft.uiNSyllables();
                        pWfLeft->m_mapStress[iPos] = pairPosType.second;
                    }
                    pWfLeft->SetWordForm(pWfLeft->sWordForm() + L"-" + pWfRight->sWordForm());
                }
            }
        }
    }

    return H_NO_ERROR;
}
