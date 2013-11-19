#include "StdAfx.h"
#include "FormBuilderNouns.h"
#include "FormBuilderAdjLong.h"
#include "FormBuilderAdjShort.h"
#include "FormBuilderPronounAdj.h"
#include "FormBuilderAdjComparative.h"
#include "FormBuilderVerbPers.h"
#include "FormBuilderVerbImpv.h"
#include "FormBuilderVerbPast.h"
#include "FormBuilderVerbNonFinite.h"
#include "LexemeHasher.h"
#include "Lexeme.h"

CLexeme::CLexeme()
{
    Init();    
}

CLexeme::~CLexeme()
{
    delete m_pDb;

    multimap<int, CWordForm *>::iterator it = m_mmWordForms.begin();
    for (; it != m_mmWordForms.end(); ++it)
    {
        delete(it->second);
    }
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
                ATLASSERT(0);
                ERROR_LOG (L"Unrecognized ending.");
                return;
            }

        }    // switch

    }   //  for (int iMs = 0; ... )

    wchar_t * arr1stStage[] = 
        {L"б",  L"п",  L"в",  L"ф",  L"м",  L"з", L"с", L"д", L"т", L"ст", L"г", L"к", L"х", L"ск"};
    wchar_t * arr2ndStage[] = 
        {L"бл", L"пл", L"вл", L"фл", L"мл", L"ж", L"ш", L"ж", L"ч", L"щ",  L"ж", L"ч", L"ш", L"щ"}; 

    for (int i_sa = 0; i_sa < sizeof (arr1stStage)/sizeof (wchar_t *); ++i_sa)
    {
        m_mapStandardAlternations[arr1stStage[i_sa]] = arr2ndStage[i_sa];
    }

    wchar_t * arrPreverbs[] = {L"в", L"над", L"об", L"от", L"под", L"пред", L"с"};
    for (int i_ap = 0; i_ap < sizeof (arrPreverbs)/sizeof (wchar_t *); ++i_ap)
    {
        m_vecAlternatingPreverbs.push_back (arrPreverbs[i_ap]);
    }

    wchar_t * arrPreverbsV[] = {L"вс", L"вз", L"вос", L"воз", L"ис", L"из", L"нис", L"низ", L"рас", L"раз"};
    for (int i_ap = 0; i_ap < sizeof (arrPreverbsV)/sizeof (wchar_t *); ++i_ap)
    {
        m_vecAlternatingPreverbsWithVoicing.push_back (arrPreverbsV[i_ap]);
    }

    m_pDb = NULL;

    m_stProperties.s1SgStem.SetVowels (g_szVowels);
    m_stProperties.s3SgStem.SetVowels (g_szVowels);
    m_stProperties.sInfinitive.SetVowels (g_szVowels);
    m_stProperties.sInfStem.SetVowels (g_szVowels);

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

ET_ReturnCode CLexeme::eGetStemStressPositions (CEString& sLemma, vector<int>& vecPosition)
{
    //
    // Find the sequence # of the stressed vowel in infinitive
    //
    vector<int>::iterator itInfStresPos = m_stProperties.vecSourceStressPos.begin();
    for (; itInfStresPos != m_stProperties.vecSourceStressPos.end(); ++itInfStresPos)
    {
        sLemma.SetVowels (g_szVowels);
        int iStemSyllables = sLemma.uiNSyllables();
        if (iStemSyllables < 1)
        {
            ATLASSERT(0);
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
        ATLASSERT(0);
        ERROR_LOG (L"Stem too short.");
        return H_ERROR_INVALID_ARG;
    }

    return H_NO_ERROR;

}       //  eGetAlternatingPreverb(...)

ET_ReturnCode CLexeme::SetDb (const CEString& sDbPath)
{
    try
    {
        m_pDb = new CSqlite (sDbPath);
    }
    catch (CException& ex)
    {
        CEString sMsg (L"Error creating instance of SQLite wrapper: ");
        sMsg += ex.szGetDescription();
        ERROR_LOG (sMsg);
        return H_ERROR_DB;
    }

    return H_NO_ERROR;
}

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
        throw CException (H_ERROR_POINTER, L"");
    }
    pair<int, CWordForm *> pairHW (pWordForm->iGramHash(), pWordForm);
    m_mmapWordForms.insert (pairHW);

}  //  AddWordForm (...)

ET_ReturnCode CLexeme::eWordFormFromHash (int iHash, unsigned int uiAt, CWordForm *& pWf)
{
    pair<multimap<int, CWordForm *>::iterator, multimap<int, CWordForm *>::iterator> pairRange;
    pairRange = m_mmapWordForms.equal_range (iHash);
    if (pairRange.first == pairRange.second)
    {
        ATLASSERT(0);
        ERROR_LOG (L"Unable to locate word form in collection.");
        return H_ERROR_UNEXPECTED;
    }

    if (distance (pairRange.first, pairRange.second) - 1 < (int)uiAt)
    {
        ATLASSERT(0);
        ERROR_LOG (L"Bad form number.");
        return H_ERROR_UNEXPECTED;
    }

    multimap<int, CWordForm *>::iterator itWf (pairRange.first);
    advance (itWf, uiAt);
    if ((*itWf).first != iHash)
    {
        ATLASSERT(0);
        ERROR_LOG (L"Error extracting map element.");
        return H_ERROR_INVALID_ARG;
    }

    pWf = itWf->second;

    return H_NO_ERROR;

}   //  WordFormFromHash (...)

ET_ReturnCode CLexeme::eWordFormFromHash (int iHash, unsigned int uiAt, IWordForm *& pIWordForm)
{
    CWordForm * pCWordForm = NULL;
    ET_ReturnCode rc = eWordFormFromHash (iHash, uiAt, pCWordForm);
    if (H_NO_ERROR == rc)
    {
        pIWordForm = pCWordForm;
    }
    return rc;
}

bool CLexeme::bNoRegularForms (int iGramHash)
{
    IWordForm * pWf = NULL;         // currently not nused
    bool bIsVariant = false;
    ET_ReturnCode rc = eGetFirstIrregularForm (iGramHash, pWf, bIsVariant);
    if (H_NO_MORE == rc)
    {
        return false;
    }
    if (rc != H_NO_ERROR && rc != H_NO_MORE)
    {
        ATLASSERT(0);
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
            ATLASSERT(0);
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

bool CLexeme::bHasIrregularForm (int iGramHash)
{
    ET_ReturnCode rc = H_NO_ERROR;

    IWordForm * pWf = NULL;
    bool bIsOptional = false;       // ignored
    rc = eGetFirstIrregularForm(iGramHash, pWf, bIsOptional);
    if (NULL == pWf || (rc != H_NO_ERROR && rc != H_NO_MORE))
    {
        ATLASSERT(0);
        CEString sMsg (L"Irregular form not found in database.");
        ERROR_LOG (sMsg);
        throw CException (rc, sMsg);
    }

    return (H_NO_ERROR == rc) ? true : false;
}

ET_ReturnCode CLexeme::eGetIrregularForms(int iHash, map<CWordForm *, bool>& mapResult)
{
    ET_ReturnCode rc = H_NO_ERROR;

    CWordForm * pWf = NULL;
    bool bIsOptional = false;
    rc = eGetFirstIrregularForm(iHash, pWf, bIsOptional);
    if (NULL == pWf || (rc != H_NO_ERROR && rc != H_NO_MORE))
    {
        ATLASSERT(0);
        ERROR_LOG (L"Irregular form not found in database.");
        return H_ERROR_UNEXPECTED;
    }

    if (H_NO_ERROR == rc)
    {
        mapResult[pWf] = bIsOptional;
    }

    do
    {
        rc = eGetNextIrregularForm(pWf, bIsOptional);
        if (rc != H_NO_ERROR && rc != H_NO_MORE)
        {
            ATLASSERT(0);
            ERROR_LOG (L"Error accessing irregular m Sg Past form.");
            return H_ERROR_UNEXPECTED;
        }

        if (H_NO_ERROR == rc)
        {
            map<CWordForm *, bool>::iterator it = mapResult.find(pWf);
            if (it != mapResult.end())
            {
                ATLASSERT(0);
                ERROR_LOG (L"Duplicate irregular form.");
                return H_ERROR_UNEXPECTED;
            }

            mapResult[pWf] = bIsOptional;
        }

    }  while (H_NO_ERROR == rc);

    return H_NO_ERROR;

}   //  eGetIrregularForms()

int CLexeme::iGramHashNSgMLong()
{
    CGramHasher hasher (POS_ADJ, SUBPARADIGM_LONG_ADJ, CASE_UNDEFINED, NUM_UNDEFINED, GENDER_UNDEFINED, PERSON_UNDEFINED,                             
                        ANIM_NO, ASPECT_UNDEFINED, REFL_UNDEFINED);
    int iHash = hasher.iGramHash();
    return iHash;
}


unsigned int CLexeme::uiFormCount (int iHash)
{
    return m_mmWordForms.count (iHash);
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
        ATLASSERT(0);
        CEString sMsg (L"Common deviation expected.");
        ERROR_LOG (sMsg);
        throw CException (E_FAIL, sMsg);
    }
}

bool CLexeme::bIsFormMissing(int iGramHash)
{
    if (m_stProperties.bHasDeficiencies)
    {
        vector<int>::iterator itMf = m_vecMissingForms.begin();
        for (; itMf != m_vecMissingForms.end(); ++itMf)
        {
            if (iGramHash == *itMf)
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
        ATLASSERT(0);
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

//
// Private
//
ET_ReturnCode CLexeme::eLoadIrregularForms()
{
    ET_ReturnCode rc = H_NO_ERROR;

    if (!m_stProperties.bHasIrregularForms)
    {
        return H_NO_MORE;
    }

//    m_mmapIrregularForms.clear();

    m_stProperties.bHasIrregularVariants = false;

    CEString sQuery 
        (L"SELECT id, gram_hash, wordform, is_alternative FROM irregular_forms WHERE descriptor_id = ");
    sQuery += CEString::sToString (m_stProperties.iDbKey);
    sQuery += L";";

    try
    {
        m_pDb->PrepareForSelect (sQuery);
        while (m_pDb->bGetRow())
        {
            //StIrregularForm stForm;
            int iId = -1;
            int iHash = -1;
            CEString sForm;
            bool bIsVariant = false;
            m_pDb->GetData (0, iId);
            m_pDb->GetData (1, iHash);
            m_pDb->GetData (2, sForm);
            m_pDb->GetData (3, bIsVariant);

            if (bIsVariant)
            {
                m_stProperties.bHasIrregularVariants = true;
            }

            CEString sStressQuery (L"SELECT position, is_primary FROM irregular_stress WHERE form_id = ");
            sStressQuery += CEString::sToString (iId);
            sStressQuery += L";";

            CWordForm * pWf = new CWordForm(iHash);
            pWf->m_bIrregular = true;
            pWf->m_sWordForm = sForm;

            unsigned int uiStressHandle = m_pDb->uiPrepareForSelect (sStressQuery);
            while (m_pDb->bGetRow (uiStressHandle))
            {
                int iPos = -1;
                bool bPrimary = false;
                m_pDb->GetData (0, iPos, uiStressHandle);
                m_pDb->GetData (1, bPrimary, uiStressHandle);
                int iStressedSyll = sForm.uiGetSyllableFromVowelPos (iPos);
                pWf->m_mapStress[iStressedSyll] = bPrimary ? STRESS_PRIMARY : STRESS_SECONDARY;
            }
            m_pDb->Finalize (uiStressHandle);
//            hAddIrregularForm (iHash, stForm);

            pair<int, CWordForm *> pairHashToWordForm (iHash, pWf);
            m_mmapWordForms.insert (pairHashToWordForm);
            
//            pair<int, StIrregularForm> pairHashToStruct (iHash, StIrregularForm(pWf, bIsVariant));
//            m_mmapIrregularForms.insert(pairHashToStruct);

        }   //  while (m_pDb->b_GetRow())

        m_pDb->Finalize();
    }
    catch (CException ex)
    {
        ERROR_LOG (ex.szGetDescription());
    }
    catch (...)
    {
        CEString sMsg;
        CEString sError;
        try
        {
            m_pDb->GetLastError (sError);
            sMsg += CEString (L", error %d: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }
    
        CString csMsg;
        csMsg.Format (sMsg, m_pDb->iGetLastError());
        ERROR_LOG ((LPCTSTR)csMsg);
    }

    return H_NO_ERROR;

}   //  hGetIrregularForms()

ET_ReturnCode CLexeme::eLoadMissingForms()
{
    ET_ReturnCode rc = H_NO_ERROR;

    if (!m_stProperties.bHasDeficiencies)
    {
        return H_NO_MORE;
    }

    m_vecMissingForms.clear();

    CEString sQuery (L"SELECT id, gram_hash FROM nonexistent_forms WHERE descriptor_id = ");
    sQuery += CEString::sToString (m_stProperties.iDbKey);
    sQuery += L";";

    try
    {
        m_pDb->PrepareForSelect (sQuery);
        while (m_pDb->bGetRow())
        {
            int iId = -1;
            int iHash = -1;
            m_pDb->GetData (0, iId);
            m_pDb->GetData (1, iHash);
            m_vecMissingForms.push_back (iHash);

         }   //  while (m_pDb->b_GetRow())

        m_pDb->Finalize();
    }
    catch (...)
    {
        CEString sMsg;
        CEString sError;
        try
        {
            m_pDb->GetLastError (sError);
            sMsg += CEString (L", error %d: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }
    
        CString csMsg;
        csMsg.Format (sMsg, m_pDb->iGetLastError());
        ERROR_LOG ((LPCTSTR)csMsg);
    }

    if (bIsFormMissing(iGramHashNSgMLong()))
    {
        m_stProperties.bNoLongForms = true;
    }
    else
    {
        m_stProperties.bNoLongForms = false;
    }

    return H_NO_ERROR;

}   //  hGetMissingForms()

ET_ReturnCode CLexeme::eGenerateParadigm()
{
    ET_ReturnCode rc = H_NO_ERROR;

    rc = eLoadIrregularForms();
    if (FAILED(rc))
    {
        return rc;
    }

    rc = eLoadMissingForms();
    if (FAILED(rc))
    {
        return rc;
    }

    m_mmWordForms.clear();

    switch (m_stProperties.ePartOfSpeech)
    {
        case POS_NOUN:
        {
            CFormBuilderNouns bn (this);
            rc = bn.eBuild();
            break;
        }
        case POS_ADJ:
        {
            CFormBuilderLongAdj lfb (this, m_stProperties.sGraphicStem, m_stProperties.eAccentType1, SUBPARADIGM_LONG_ADJ);
            rc = lfb.eBuild();
            ATLASSERT (H_NO_ERROR == rc);

            if (!m_stProperties.bNoShortForms)
            {
                CFormBuilderShortAdj sfb (this);
                rc = sfb.eBuild();
                ATLASSERT (H_NO_ERROR == rc);
            }

            if (!m_stProperties.bNoComparative)
            {
                CFormBuilderComparative cfb (this);
                cfb.eBuild();
            }
            break;
        }
        case POS_PRONOUN_ADJ:
        {
            CFormBuilderPronounAdj pab (this, m_stProperties.sGraphicStem, m_stProperties.eAccentType1);
            rc = pab.eBuild();
            ATLASSERT (H_NO_ERROR == rc);
            break;
        }
        case POS_VERB:
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

            CFormBuilderPast past (this);
            rc = past.eBuild();

            CFormBuilderImperative imperative (this);
            rc = imperative.eBuild();

            CFormBuilderNonFinite nonFinite (this);
            rc = nonFinite.eBuild();

            break;
        }
        case POS_PRONOUN:       // All forms are irregular
        case POS_NUM:
        {
//            for (multimap<int, bool>::iterator itWf = m_mmapIrregularForms.begin();
//                 itWf != m_mmapIrregularForms.end();
//                 ++itWf)
//            {
//                CWordForm * pWordForm = new CWordForm;
//                ET_ReturnCode hInit = pWordForm->hInitFromHash (itWf->first);
//                if (hInit != H_NO_ERROR)
//                {
//                    CEString sMsg = L"Unable to decode gram hash " + CEString::sToString<int>(itWf->first) + L".";
//                    ERROR_LOG (sMsg);
//                    continue;
//                }
//                pWordForm->m_pLexeme = this;
////                pWordForm->m_ePos = m_ePartOfSpeech;
//                pWordForm->m_sLemma = m_sGraphicStem;     // is that correct?
//                pWordForm->m_sWordForm = itWf->second.sForm;
//                pWordForm->m_mapStress = itWf->second.mapStress;
//                m_vecWordForms.push_back(pWordForm);
//            }
            break;
        }
        case POS_PRONOUN_PREDIC:
        case POS_ADV:
        case POS_COMPAR:
        case POS_PREDIC:
        case POS_PREP:
        case POS_CONJ:
        case POS_PARTICLE:
        case POS_INTERJ:
        case POS_PARENTH:
        case POS_NULL:          // There is only one form
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

            pair<int, CWordForm *> pairHashToWf(pWordForm->iGramHash(), pWordForm);
            m_mmWordForms.insert(pairHashToWf);
            break;
        }
        default:
        {
            ERROR_LOG (L"Part of speech value not recognized.");
            return H_ERROR_UNEXPECTED;
        }
    }

    return H_NO_ERROR;

}   // eGenerateParadigm()

CEString CLexeme::sHash()
{
    StLexemeHasher stLh;
    stLh.sSourceForm = m_stProperties.sSourceForm;
    vector<int>::iterator itHomonym = m_stProperties.vecHomonyms.begin();
    for (; itHomonym != m_stProperties.vecHomonyms.end(); ++itHomonym)
    {
        stLh.sSourceForm += CEString::sToString (*itHomonym);
    }
/*
    vector<int>::iterator itStress = m_vecSourceStressPos.begin();
    for (; itStress != m_vecSourceStressPos.end(); ++itStress)
    {
        int iStressedChar = m_sSourceForm.uiGetVowelPos (*itStress);
        stLh.mapStress[iStressedChar] = STRESS_PRIMARY;
    }

    itStress = m_vecSecondaryStressPos.begin();
    for (; itStress != m_vecSecondaryStressPos.end(); ++itStress)
    {
        int iStressedChar = m_sSourceForm.uiGetVowelPos (*itStress);
        stLh.mapStress[iStressedChar] = STRESS_SECONDARY;
    }
*/
    stLh.sMainSymbol = m_stProperties.sMainSymbol;
    stLh.sComment = m_stProperties.sHeadwordComment;
    stLh.sComment += m_stProperties.sComment;
    stLh.sComment += m_stProperties.sTrailingComment;

    if (m_stProperties.iType > -1)
    {
        stLh.iInflectionType = m_stProperties.iType;
        stLh.iAccentType1 = (int)m_stProperties.eAccentType1;
        stLh.iAccentType2 = (int)m_stProperties.eAccentType2;
    }

    return stLh.sHash();

}   //  str_Hash()

ET_ReturnCode CLexeme::eFormExists (CGramHasher& hasher)
{
    ET_ReturnCode rc = H_NO_ERROR;

    if (!m_stProperties.bHasDeficiencies)
    {
        return H_TRUE;
    }

    int iHash = hasher.iGramHash();

    vector<int>::iterator itMf = m_vecMissingForms.begin();
    for (; itMf != m_vecMissingForms.end(); ++itMf)
    {
        if (iHash == *itMf)
        {
            return H_NO_MORE;
        }
    }

    return H_TRUE;

}   //  eFormExists (...)

ET_ReturnCode CLexeme::eGetFirstIrregularForm (int iHash, IWordForm*& pIWordForm, bool& bIsOptional)
{
    CWordForm * pCWordForm = NULL;
    ET_ReturnCode rc = eGetFirstIrregularForm (iHash, pCWordForm, bIsOptional);
    if (H_NO_ERROR == rc)
    {
        pIWordForm = pCWordForm;
    }
    return rc;
}

ET_ReturnCode CLexeme::eGetFirstIrregularForm (int iHash, CWordForm*& pWordForm, bool& bIsOptional)
{
    m_pairItIfRange = m_mmapIrregularForms.equal_range (iHash);
    if (m_pairItIfRange.first == m_pairItIfRange.second)
    {
        return H_NO_MORE;
    }
    else
    {
        m_itCurrentIrregularForm = m_pairItIfRange.first;
        if ((*m_itCurrentIrregularForm).first != iHash)
        {
            return H_NO_MORE;
        }
        else
        {
            pWordForm = (*m_itCurrentIrregularForm).second.pWordForm;
            bIsOptional = (*m_itCurrentIrregularForm).second.bIsOptional;
            return H_NO_ERROR;
        }
    }

}   //  eGetFirstIrregularForm (...)

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

    if (m_itCurrentIrregularForm == m_mmapIrregularForms.end())
//        || (*m_itCurrentIrregularForm).first != iHash))
    {
        return H_NO_MORE;
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
