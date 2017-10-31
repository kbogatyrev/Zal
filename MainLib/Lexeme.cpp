#include "StdAfx.h"
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
    m_vecMissingForms = source.m_vecMissingForms;
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
    }
    
//    multimap<int, StIrregularForm>::iterator itIf = m_mmapIrregularForms.begin();
//    for (; itIf != m_mmapIrregularForms.end(); ++itIf)
//    {
//        delete(itIf->second.pWordForm);
//    }
}

void CLexeme::Init()
{
    CEString arrMainSymbol[] = { L"м", L"мо", L"ж", L"жо", L"с", L"со", L"мо-жо", L"мн.",
     L"мн. неод.", L"мн. одуш.", L"мн. _от_", L"п", L"мс", L"мс-п", L"числ.", L"числ.-п", 
     L"св", L"нсв", L"св-нсв", L"н", L"предл.", L"союз", L"предик.", L"вводн.", L"сравн.", 
     L"част.", L"межд." };

    for (ET_MainSymbol eMs = MS_START; eMs < MS_END; ++eMs)
    {
        m_mapMainSymbol[arrMainSymbol[eMs]] = eMs; 
    }

    for (int iMs = 0; iMs < (int)MS_END; ++iMs)
    {
        CEString sMs = arrMainSymbol[iMs];
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
                ERROR_LOG (L"Unrecognized type.");
                return;
            }

        }    // switch

    }   //  for (int iMs = 0; ... )

    const wchar_t * arr1stStage[] = 
        {L"б",  L"п",  L"в",  L"ф",  L"м",  L"з", L"с", L"д", L"т", L"ст", L"г", L"к", L"х", L"ск"};
    const wchar_t * arr2ndStage[] = 
        {L"бл", L"пл", L"вл", L"фл", L"мл", L"ж", L"ш", L"ж", L"ч", L"щ",  L"ж", L"ч", L"ш", L"щ"}; 

    for (int i_sa = 0; i_sa < sizeof (arr1stStage)/sizeof (wchar_t *); ++i_sa)
    {
        m_mapStandardAlternations[arr1stStage[i_sa]] = arr2ndStage[i_sa];
    }

    const wchar_t * arrPreverbs[] = {L"в", L"над", L"об", L"от", L"под", L"пред", L"с"};
    for (int i_ap = 0; i_ap < sizeof (arrPreverbs)/sizeof (wchar_t *); ++i_ap)
    {
        m_vecAlternatingPreverbs.push_back (arrPreverbs[i_ap]);
    }

    const wchar_t * arrPreverbsV[] = {L"вс", L"вз", L"вос", L"воз", L"ис", L"из", L"нис", L"низ", L"рас", L"раз"};
    for (int i_ap = 0; i_ap < sizeof (arrPreverbsV)/sizeof (wchar_t *); ++i_ap)
    {
        m_vecAlternatingPreverbsWithVoicing.push_back (arrPreverbsV[i_ap]);
    }

    m_stProperties.s1SgStem.SetVowels (g_szRusVowels);
    m_stProperties.s3SgStem.SetVowels (g_szRusVowels);
    m_stProperties.sInfinitive.SetVowels (g_szRusVowels);
    m_stProperties.sInfStem.SetVowels (g_szRusVowels);

}   // Init()

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

ET_ReturnCode CLexeme::eGetStemStressPositions (const CEString& sLemma, vector<int>& vecPosition)
{
    //
    // Find the sequence # of the stressed vowel in infinitive
    //
    CEString sCopy(sLemma);

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

    vector<CEString>::iterator itP = m_vecAlternatingPreverbs.begin();
    for (; itP != m_vecAlternatingPreverbs.end()&&!bPreverb; ++itP)
    {
        if (sVerbForm.bStartsWith (*itP))
        {
            sPreverb = *itP;
            bPreverb = true;
        }
    }

    if (!bPreverb)
    {
        itP = m_vecAlternatingPreverbsWithVoicing.begin();
        for (; itP != m_vecAlternatingPreverbsWithVoicing.end()&&!bPreverb; ++itP)
        {
            if (sVerbForm.bStartsWith (*itP))
            {
                sPreverb = *itP;    
                bPreverb = true;
                bVoicing = true;
            }
        }
    }

    if (!bPreverb)
    {
        return H_FALSE;
    }

    if (sVerbForm.uiLength() < sPreverb.uiLength() + 2)
    {
        ASSERT(0);
        ERROR_LOG (L"Stem too short.");
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
    pair<CEString, CWordForm *> pairHW (pWordForm->sGramHash(), pWordForm);
    m_mmWordForms.insert (pairHW);

}  //  AddWordForm (...)


uint64_t CLexeme::uiTotalWordForms()
{
    return m_mmWordForms.size();
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

ET_ReturnCode CLexeme::eWordFormFromHash(CEString sHash, int iAt, CWordForm *& pWf)
{
    pair<multimap<CEString, CWordForm *>::iterator, multimap<CEString, CWordForm *>::iterator> pairRange;
    pairRange = m_mmWordForms.equal_range(sHash);
    if (pairRange.first == pairRange.second)
    {
        ASSERT(0);
        ERROR_LOG (L"Unable to locate word form in collection.");
        return H_ERROR_UNEXPECTED;
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

ET_ReturnCode CLexeme::eWordFormFromHash (CEString sHash, int iAt, IWordForm *& pIWordForm)
{
    CWordForm * pCWordForm = NULL;
    ET_ReturnCode rc = eWordFormFromHash (sHash, iAt, pCWordForm);
    if (H_NO_ERROR == rc)
    {
        pIWordForm = pCWordForm;
    }
    return rc;
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
            CEString sMsg (L"hGetFirstIrregularForm() failed, error ");
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

bool CLexeme::bIsFormMissing(CEString sGramHash)
{
    if (m_stProperties.bHasDeficiencies)
    {
        vector<CEString>::iterator itMf = m_vecMissingForms.begin();
        for (; itMf != m_vecMissingForms.end(); ++itMf)
        {
            if (sGramHash == *itMf)
            {
                return true;
            }
        }
        return false;
    }
    return false;
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

        if (pWf->m_sLemma != sCurrentStem)
        {
            bool bSaved = (*itWf).second->bSaveStemToDb();
            if (!bSaved)
            {
                eRet = H_ERROR_GENERAL;
                ERROR_LOG(L"Unable to save a stem in the DB.");
                continue;
            }
            sCurrentStem = (*itWf).second->m_sLemma;
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
        if (0 == (*itWf).second->m_llStemId)    // stem string not found, look up in DB
        {
            CEString sQuery(L"SELECT id from stems WHERE stem_string=\"");
            sQuery += (*itWf).second->m_sLemma;
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
                    sMsg += (*itWf).second->m_sLemma;
                    sMsg += L'"';
                    ERROR_LOG(sMsg);
                }
                pDbHandle->Finalize();
            }
            catch (CException& ex)
            {
                ERROR_LOG(ex.szGetDescription());
                eRet = H_ERROR_DB;
            }
        }
    }       // for (...)

    return eRet;

}       //  eAssignStemIds()

ET_ReturnCode CLexeme::eSaveDataToDb()
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
            sMsg += (*itWf).second->m_sLemma;
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

}   // eSaveDataToDb()

ET_ReturnCode CLexeme::eLoadIrregularForms()
{
    ET_ReturnCode rc = H_NO_ERROR;

    if (!m_stProperties.bHasIrregularForms)
    {
        return H_FALSE;
    }

    m_stProperties.bHasIrregularVariants = false;

    CEString sQuery 
        (L"SELECT id, gram_hash, wordform, is_alternative FROM irregular_forms WHERE descriptor_id = ");
    sQuery += CEString::sToString (m_stProperties.iDbKey);
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
            pDb->GetData (0, iId, uiQueryHandle);
            pDb->GetData (1, sHash, uiQueryHandle);
            pDb->GetData (2, sForm, uiQueryHandle);
            pDb->GetData (3, bIsVariant, uiQueryHandle);

            if (bIsVariant)
            {
                m_stProperties.bHasIrregularVariants = true;
            }

            CEString sStressQuery (L"SELECT position, is_primary FROM irregular_stress WHERE form_id = ");
            sStressQuery += CEString::sToString (iId);
            sStressQuery += L";";

            CWordForm * pWf = new CWordForm(sHash);
            pWf->m_pLexeme = this;
            pWf->m_bIrregular = true;
            pWf->m_sWordForm = sForm;

            uint64_t uiStressHandle = pDb->uiPrepareForSelect (sStressQuery);
            while (pDb->bGetRow (uiStressHandle))
            {
                int iPos = -1;
                bool bPrimary = false;
                pDb->GetData (0, iPos, uiStressHandle);
                pDb->GetData (1, bPrimary, uiStressHandle);
                int iStressedSyll = sForm.uiGetSyllableFromVowelPos (iPos);
                pWf->m_mapStress[iStressedSyll] = bPrimary ? STRESS_PRIMARY : STRESS_SECONDARY;
            }
            pDb->Finalize (uiStressHandle);

            StIrregularForm stIf(pWf, bIsVariant);
            pair<CEString, StIrregularForm> pairHashToWordForm (sHash, stIf);
            m_mmapIrregularForms.insert (pairHashToWordForm);

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

ET_ReturnCode CLexeme::eLoadMissingForms()
{
    ET_ReturnCode rc = H_NO_ERROR;

    if (!m_stProperties.bHasDeficiencies)
    {
        return H_FALSE;
    }

    m_vecMissingForms.clear();

    CEString sQuery (L"SELECT id, gram_hash FROM nonexistent_forms WHERE descriptor_id = ");
    sQuery += CEString::sToString (m_stProperties.iDbKey);
    sQuery += L";";

    CSqlite * pDb = NULL;

    try
    {
        pDb = m_pDictionary->pGetDbHandle();
        pDb->PrepareForSelect (sQuery);
        while (pDb->bGetRow())
        {
            int iId = -1;
            CEString sHash;
            pDb->GetData (0, iId);
            pDb->GetData (1, sHash);
            m_vecMissingForms.push_back (sHash);

         }   //  while (m_pDb->b_GetRow())

        pDb->Finalize();
    }
    catch (...)
    {
        CEString sMsg;
        CEString sError;
        try
        {
            pDb->GetLastError (sError);
            sMsg += CEString (L", error %d: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }
    
        sMsg += CEString::sToString(pDb->iGetLastError());
        ERROR_LOG (sMsg);
    }

    if (POS_ADJ == m_stProperties.ePartOfSpeech)
    {
        if (bIsFormMissing(sGramHashNSgMLong()))
        {
            m_stProperties.bNoLongForms = true;
        }
        else
        {
            m_stProperties.bNoLongForms = false;
        }
    }

    return H_NO_ERROR;

}   //  hGetMissingForms()

ET_ReturnCode CLexeme::eGenerateParadigm()
{
    ET_ReturnCode rc = H_NO_ERROR;

    try
    {
        if (m_stProperties.bHasIrregularForms)
        {
            rc = eLoadIrregularForms();
            if (rc != H_NO_ERROR && rc != H_FALSE && rc != H_NO_MORE)
            {
                return rc;
            }
        }

        if (m_stProperties.bHasDeficiencies)
        {
            rc = eLoadMissingForms();
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
        }
            
        if (L"п" == m_stProperties.sInflectionType)
        {
            CFormBuilderLongAdj lfb (this, m_stProperties.sGraphicStem, m_stProperties.eAccentType1, SUBPARADIGM_LONG_ADJ);
            rc = lfb.eBuild();
            if (rc != H_NO_ERROR)
            {
                return rc;
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

        if (L"мс-п" == m_stProperties.sInflectionType)
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

            CFormBuilderPersonal personal (this);
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

        if (POS_PRONOUN_PREDIC == m_stProperties.ePartOfSpeech || POS_ADV == POS_COMPAR || POS_PREDIC == POS_PREP 
            || POS_CONJ == m_stProperties.ePartOfSpeech || POS_PARTICLE == m_stProperties.ePartOfSpeech 
            || POS_INTERJ == m_stProperties.ePartOfSpeech || POS_PARENTH == m_stProperties.ePartOfSpeech
            || POS_NULL == m_stProperties.ePartOfSpeech)
        {
            CWordForm * pWordForm = new CWordForm;
            pWordForm->m_pLexeme = this;
            pWordForm->m_ePos = m_stProperties.ePartOfSpeech;
            pWordForm->m_sLemma = m_stProperties.sGraphicStem;
            pWordForm->m_sWordForm = m_stProperties.sGraphicStem;
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
        ERROR_LOG (ex.szGetDescription());
        return H_EXCEPTION;
    }

	m_bFormsGenerated = true;

    return H_NO_ERROR;

}   // eGenerateParadigm()

ET_ReturnCode CLexeme::eGetAspectPair(CEString& sAspectPair, int& iStressPos)
{
    sAspectPair = L"";
    iStressPos = -1;

    if (NULL == m_pDictionary)
    {
        return H_ERROR_POINTER;
    }

    if (!bHasAspectPair())
    {
        return H_FALSE;
    }

    CFormBuilderAspectPair bap(m_pDictionary, this);
    ET_ReturnCode rc = bap.eBuild();
    if (rc != H_NO_ERROR)
    {
        return rc;
    }

    rc = bap.eGetPairedInfinitive(sAspectPair,iStressPos);

    return rc;

}       //  eGetAspectPair()

ET_ReturnCode CLexeme::eGetAltAspectPair(CEString& sAltAspectPair, int& iAltStressPos)
{
    sAltAspectPair = L"";
    iAltStressPos = -1;

    if (!bHasAltAspectPair())
    {
        return H_FALSE;
    }

    if (NULL == m_pDictionary)
    {
        return H_ERROR_POINTER;
    }

    CFormBuilderAspectPair bap(m_pDictionary, this);
    ET_ReturnCode rc = bap.eBuild();
    if (rc != H_NO_ERROR)
    {
        return rc;
    }

    rc = bap.eGetPairedInfinitive(sAltAspectPair, iAltStressPos);

    return rc;

}       //  eGetAltAspectPair()

ET_ReturnCode CLexeme::eSaveTestData()
{
    if (m_mmWordForms.empty())
    {
        return H_FALSE;
    }

    ET_ReturnCode eRet = H_NO_ERROR;

    CSqlite * pDbHandle = NULL;

    try
    {
        pDbHandle = m_pDictionary->pGetDbHandle();
        pDbHandle->PrepareForInsert(L"lexeme_hash_to_descriptor", 3);
        pDbHandle->Bind(1, sHash());
        pDbHandle->Bind(2, m_stProperties.iDbKey);
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

	pDbHandle->BeginTransaction();

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

	pDbHandle->CommitTransaction();

	return H_NO_ERROR;

}   // eSaveTestData()

ET_ReturnCode CLexeme::eClone(ILexeme *& pClonedObject)
{
    pClonedObject = new CLexeme(const_cast<const CLexeme&>(*this));
    return H_NO_ERROR;
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

    sSource += m_stProperties.sHeadwordComment;
    sSource += m_stProperties.sComment;
    sSource += m_stProperties.sTrailingComment;

    CMD5 md5;
    return md5.sHash(sSource);

}   //  sHash()

ET_ReturnCode CLexeme::eFormExists (CGramHasher& hasher)
{
    ET_ReturnCode rc = H_NO_ERROR;

    if (!m_stProperties.bHasDeficiencies)
    {
        return H_TRUE;
    }

    CEString sHash = hasher.sGramHash();

    vector<CEString>::iterator itMf = m_vecMissingForms.begin();
    for (; itMf != m_vecMissingForms.end(); ++itMf)
    {
        if (sHash == *itMf)
        {
            return H_NO_MORE;
        }
    }

    return H_TRUE;

}   //  eFormExists (...)

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
