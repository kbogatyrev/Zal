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

HRESULT CLexeme::GenerateWordForms()
{
    HRESULT hr = hGenerateParadigm();
    return hr;
}

HRESULT CLexeme::get_FormDescriptor (IFormFinder ** pp_fd)
{
    HRESULT hr = S_OK;

    CComObject<CFormDescriptor> * pFd = NULL;
    hr = CComObject<CFormDescriptor>::CreateInstance (&pFd);
    if (S_OK != hr)
    {
        ERROR_LOG (L"CreateInstance failed for CFormDescriptor.");
        ATLASSERT(0);
        return hr;
    }

    pFd->m_pLexeme = this;
//    CComPtr<CFormDescriptor> sp_ = pFd;
    hr = pFd->QueryInterface<IFormFinder> (pp_fd);
    if (S_OK != hr || !pp_fd)
    {
        ATLASSERT(0);
        ERROR_LOG (L"QI failed for IFormFinder.");
        return E_NOINTERFACE;
    }

    return hr;
}

HRESULT CLexeme::get_LexemeId (LONG * plId)
{
    *plId = m_iDbKey;
    return S_OK;
}

HRESULT CLexeme::get_InflectionId (LONG * plId)
{
    *plId = m_iInflectionId;
    return S_OK;
}

HRESULT CLexeme::get_GraphicStem (BSTR * pbstrLemma)
{
    if (m_sGraphicStem.bIsEmpty())
    {
        return S_FALSE;
    }
    CComBSTR spBstr (m_sGraphicStem);
    *pbstrLemma = spBstr.Detach();

    return S_OK;
}   

HRESULT CLexeme::get_InitialForm (BSTR * pbstrSourceForm)
{
    if (m_sSourceForm.bIsEmpty())
    {
        return S_FALSE;
    }
    CComBSTR spBstr (m_sSourceForm);
    *pbstrSourceForm = spBstr.Detach();

    return S_OK;
}

HRESULT CLexeme::get_IsVariant (BOOL * pbVariant)
{
    *pbVariant = m_bIsVariant ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_MainSymbol (BSTR * pbstrMainSymbol)
{
    if (m_sMainSymbol.bIsEmpty())
    {
        return S_FALSE;
    }
    CComBSTR spBstr (m_sMainSymbol);
    *pbstrMainSymbol = spBstr.Detach();

    return S_OK;
}

HRESULT CLexeme::get_IsPluralOf (BOOL * puiIsPluralOf)
{
    *puiIsPluralOf = m_bIsPluralOf ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_IsTransitive (BOOL * puiIsTransitive)
{
    *puiIsTransitive = m_bTransitive;
    return S_OK;
}

HRESULT CLexeme::get_Aspect (ET_Aspect * peAspect)
{
    *peAspect = m_eAspect;
    return S_OK;
}

HRESULT CLexeme::get_IsReflexive (ET_Reflexive * peIsReflexive)
{
    *peIsReflexive = m_eReflexive;
    return S_OK;
}

HRESULT CLexeme::get_MainSymbolPluralOf (BSTR * pbstrMsPluralOf)
{
    if (m_sMainSymbolPluralOf.bIsEmpty())
    {
        return S_FALSE;
    }
    CComBSTR spBstr (m_sMainSymbolPluralOf);
    *pbstrMsPluralOf = spBstr.Detach();

    return S_OK;
}

HRESULT CLexeme::get_AltMainSymbol (BSTR * pbstrAltMainSymbol)
{
    if (m_sAltMainSymbol.bIsEmpty())
    {
        return S_FALSE;
    }
    CComBSTR spBstr (m_sAltMainSymbol);
    *pbstrAltMainSymbol = spBstr.Detach();

    return S_OK;
}

HRESULT CLexeme::get_InflectionType (BSTR * pbmSInflectionType)
{
    if (m_sInflectionType.bIsEmpty())
    {
        return S_FALSE;
    }
    CComBSTR spBstr (m_sInflectionType);
    *pbmSInflectionType = spBstr.Detach();

    return S_OK;
}

HRESULT CLexeme::get_PartOfSpeech (ET_PartOfSpeech * pePos)
{
    *pePos = m_ePartOfSpeech;
    return S_OK;
}

HRESULT CLexeme::get_Comment (BSTR * pbstrComment)
{
    if (m_sComment.bIsEmpty())
    {
        return S_FALSE;
    }
    CComBSTR spBstr (m_sComment);
    *pbstrComment = spBstr.Detach();

    return S_OK;
}

HRESULT CLexeme::get_AltMainSymbolComment (BSTR * pbstrAltMsComment)
{
    if (m_sAltMainSymbolComment.bIsEmpty())
    {
        return S_FALSE;
    }
    CComBSTR spBstr (m_sAltMainSymbolComment);
    *pbstrAltMsComment = spBstr.Detach();

    return S_OK;
}

HRESULT CLexeme::get_AltInflectionComment (BSTR * pbstrAltInflectionComment)
{
    if (m_sAltInflectionComment.bIsEmpty())
    {
        return S_FALSE;
    }
    CComBSTR spBstr (m_sAltInflectionComment);
    *pbstrAltInflectionComment = spBstr.Detach();

    return S_OK;
}

HRESULT CLexeme::get_VerbStemAlternation (BSTR * bstrVerbStemAlternation)
{
    if (m_sVerbStemAlternation.bIsEmpty())
    {
        return S_FALSE;
    }
    CComBSTR spBstr (m_sVerbStemAlternation);
    *bstrVerbStemAlternation = spBstr.Detach();

    return S_OK;
}

HRESULT CLexeme::get_Section (LONG * plSection)
{
    *plSection = (long)m_iSection;
    return S_OK;
}

HRESULT CLexeme::get_NoComparative (BOOL * puiNoComparative)
{
    *puiNoComparative = m_bNoComparative ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_HasAssumedForms (BOOL * puiHasAssumedForms)
{
    *puiHasAssumedForms = m_bAssumedForms ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_HasYoAlternation (BOOL * puiHasYoAlteration)
{
    *puiHasYoAlteration = m_bYoAlternation ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_HasOAlternation (BOOL * puiHasOAlteration)
{
    *puiHasOAlteration = m_bOAlternation ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_HasSecondGenitive (BOOL * puiHasSecondaryGenetive)
{
    *puiHasSecondaryGenetive = m_bSecondGenitive ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_QuestionableForms (BSTR *)
{
    return E_NOTIMPL;
}

HRESULT CLexeme::get_IrregularForms (BSTR *)
{
    return E_NOTIMPL;
}

HRESULT CLexeme::get_RestrictedForms (BSTR *)
{
    return E_NOTIMPL;
}

HRESULT CLexeme::get_Contexts (BSTR *)
{
    return E_NOTIMPL;
}

HRESULT CLexeme::get_TrailingComment (BSTR * pbstrTrailingComment)
{
    if (m_sTrailingComment.bIsEmpty())
    {
        return S_FALSE;
    }
    CComBSTR spBstr (m_sTrailingComment);
    *pbstrTrailingComment = spBstr.Detach();

    return S_OK;
}

// From Inflection table:
HRESULT CLexeme::get_IsPrimaryInflectionGroup (BOOL * puiIsPrimaryInflectionGroup)
{
    *puiIsPrimaryInflectionGroup = m_bPrimaryInflectionGroup ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_Type (LONG * plType)
{
    *plType = m_iType;
    return S_OK;
}

HRESULT CLexeme::get_AccentType1 (ET_AccentType * peAccentType1)
{
    *peAccentType1 = m_eAccentType1;
    return S_OK;
}

HRESULT CLexeme::get_AccentType2 (ET_AccentType * peAccentType2)
{
    *peAccentType2 = m_eAccentType2;
    return S_OK;
}

HRESULT CLexeme::get_ShortFormsAreRestricted (BOOL * puiShortFormsAreRestricted)
{
    *puiShortFormsAreRestricted = m_bShortFormsRestricted ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_PastParticipleIsRestricted (BOOL * puiPastParticipleIsRestricted)
{
    *puiPastParticipleIsRestricted = m_bPastParticipleRestricted ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_NoShortForms (BOOL * puiNoShortForms)
{
    *puiNoShortForms = m_bNoShortForms ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_NoPastParticiple (BOOL * puiNoPastParticiple)
{
    *puiNoPastParticiple = m_bNoPastParticiple ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_HasFleetingVowel (BOOL * puiHasFleetingVowel)
{
    *puiHasFleetingVowel = m_bFleetingVowel ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_HasStemAugment (BOOL * puiHasStemAugment)
{
    *puiHasStemAugment = m_iStemAugment > -1 ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_HasIrregularForms (BOOL * puiHasIrregularForms)
{
    *puiHasIrregularForms = m_mmapIrregularForms.empty() ? FALSE : TRUE;
    return S_OK;
}

HRESULT CLexeme::get_IsUnstressed (BOOL * puiIsUnstressed)
{
    *puiIsUnstressed = m_bIsUnstressed ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::SaveTestData()
{
    HRESULT hr = S_OK;

    try
    {
        m_pDb->BeginTransaction();

        multimap<int, CComVariant>::iterator it_ = m_coll.begin();
        for (; it_ != m_coll.end(); ++it_)
        {
            int iHash = (*it_).first;
            CComVariant spWf = (*it_).second;
            CComQIPtr<IWordForm> spqiWf = spWf.punkVal;
            if (!spqiWf)
            {
                ATLASSERT(0);
                ERROR_LOG (L"QI for ILexeme failed.");
                return E_NOINTERFACE;
            }        

            CWordForm * pWf = (CWordForm *)spqiWf.p;
            hr = pWf->SaveTestData();
            if (FAILED (hr))
            {
                return hr;
            }
        }

        m_pDb->CommitTransaction();
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

        return E_FAIL;
    }
    
    return S_OK;

}   // SaveTestData()


//
//  Helpers
//

void CLexeme::Init()
{
    m_iDbKey = -1;
    m_bIsUnstressed = false;
    m_bIsVariant = false;
    m_bIsPluralOf = false;
    m_bTransitive = false;
    m_eReflexive = REFL_UNDEFINED;
    m_ePartOfSpeech = POS_UNDEFINED;
    m_iSection = -1;
    m_bPartPastPassZhd = false;
    m_bNoComparative = false;
    m_bAssumedForms = false;
    m_bYoAlternation = false;
    m_bSecondGenitive = false;
    m_bHasAspectPair = false;
    m_iAspectPairType = -1;
    m_bHasIrregularForms = false;
    m_bHasIrregularVariants = false;
    m_bPrimaryInflectionGroup = false;
    m_iType = -1;
    m_eAccentType1 = AT_UNDEFINED;
    m_eAccentType2 = AT_UNDEFINED;
    m_bShortFormsRestricted = false;
    m_bPastParticipleRestricted = false;
    m_bNoLongForms = false;
    m_bNoShortForms = false;
    m_bNoPastParticiple = false;
    m_bFleetingVowel = false;
    m_iStemAugment = -1;

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

    m_sSourceForm.SetVowels (g_szVowels);
    m_sGraphicStem.SetVowels (g_szVowels);
    m_s1SgStem.SetVowels (g_szVowels);
    m_s3SgStem.SetVowels (g_szVowels);
    m_sInfinitive.SetVowels (g_szVowels);
    m_sInfStem.SetVowels (g_szVowels);                        

}   // Init()

void CLexeme::SetDb (const CEString& sDbPath)
{
    m_pDb = new CSqlite (sDbPath);
}

HRESULT CLexeme::hAddWordForm (CComObject<CWordForm> * pWordForm)
{
    if (NULL == pWordForm)
    {
        throw CException (E_POINTER, L"");
    }

    pair<int, CComVariant> pairHW (pWordForm->iGramHash(), CComVariant (pWordForm));
    m_coll.insert (pairHW);

    return S_OK;

}  //  hAddWordForm (...)

HRESULT CLexeme::hGetIrregularForms()
{
    HRESULT hr = S_OK;

    if (!m_bHasIrregularForms)
    {
        return S_FALSE;
    }

    m_mmapIrregularForms.clear();

    m_bHasIrregularVariants = false;

    CEString sQuery 
        (L"SELECT id, gram_hash, wordform, is_alternative FROM irregular_forms WHERE descriptor_id = ");
    sQuery += CEString::sToString (m_iDbKey);
    sQuery += L";";

    try
    {
        m_pDb->PrepareForSelect (sQuery);
        while (m_pDb->bGetRow())
        {
            StIrregularForm stForm;
            int iId = -1;
            int iHash = -1;
            m_pDb->GetData (0, iId);
            m_pDb->GetData (1, iHash);
            m_pDb->GetData (2, stForm.sForm);
            m_pDb->GetData (3, stForm.bIsVariant);

            if (stForm.bIsVariant)
            {
                m_bHasIrregularVariants = true;
            }

            CEString sStressQuery (L"SELECT position, is_primary FROM irregular_stress WHERE form_id = ");
            sStressQuery += CEString::sToString (iId);
            sStressQuery += L";";

            unsigned int uiStressHandle = m_pDb->uiPrepareForSelect (sStressQuery);
            while (m_pDb->bGetRow (uiStressHandle))
            {
                int iPos = -1;
                bool bPrimary = false;
                m_pDb->GetData (0, iPos, uiStressHandle);
                m_pDb->GetData (1, bPrimary, uiStressHandle);
                int iStressedSyll = stForm.sForm.uiGetSyllableFromVowelPos (iPos);
                stForm.mapStress[iStressedSyll] = bPrimary ? STRESS_PRIMARY : STRESS_SECONDARY;
            }
            m_pDb->Finalize (uiStressHandle);
            hAddIrregularForm (iHash, stForm);
        
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

    return S_OK;

}   //  hGetIrregularForms()

HRESULT CLexeme::hGetMissingForms()
{
    HRESULT hr = S_OK;

    if (!m_bHasDeficiencies)
    {
        return S_FALSE;
    }

    m_vecMissingForms.clear();

    CEString sQuery (L"SELECT id, gram_hash FROM nonexistent_forms WHERE descriptor_id = ");
    sQuery += CEString::sToString (m_iDbKey);
    sQuery += L";";

    try
    {
        m_pDb->PrepareForSelect (sQuery);
        while (m_pDb->bGetRow())
        {
            StIrregularForm stForm;
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

    return S_OK;

}   //  hGetMissingForms()

HRESULT CLexeme::hGenerateParadigm()
{
    HRESULT hr = S_OK;

    hr = hGetIrregularForms();
    if (FAILED(hr))
    {
        return hr;
    }

    hr = hGetMissingForms();
    if (FAILED(hr))
    {
        return hr;
    }

    m_coll.clear();

    switch (m_ePartOfSpeech)
    {
        case POS_NOUN:
        {
            CFormBuilderNouns bn (this);
            hr = bn.hBuild();
            break;
        }
        case POS_ADJ:
        {
            CFormBuilderLongAdj lfb (this, m_sGraphicStem, m_eAccentType1, SUBPARADIGM_LONG_ADJ);
            hr = lfb.hBuild();
            ATLASSERT (SUCCEEDED (hr));

            if (!m_bNoShortForms)
            {
                CFormBuilderShortAdj sfb (this);
                hr = sfb.hBuild();
                ATLASSERT (S_OK == hr);
            }

            if (!m_bNoComparative)
            {
                CFormBuilderComparative cfb (this);
                cfb.hBuild();
            }
            break;
        }
        case POS_PRONOUN_ADJ:
        {
            CFormBuilderPronounAdj pab (this, m_sGraphicStem, m_eAccentType1);
            hr = pab.hBuild();
            ATLASSERT (S_OK == hr);
            break;
        }
        case POS_VERB:
        {
            m_eAspect = ASPECT_UNDEFINED;
//            if (L"нсв" == str_MainSymbol)
            if (L"нсв" == m_sInflectionType)
            {
                m_eAspect = ASPECT_IMPERFECTIVE;
            }
            else if (L"св" == m_sInflectionType)
            {
                m_eAspect = ASPECT_PERFECTIVE;
            }

            CFormBuilderPersonal personal (this);
            hr = personal.hBuild();

            CFormBuilderPast past (this);
            hr = past.hBuild();

            CFormBuilderImperative imperative (this);
            hr = imperative.hBuild();

            CFormBuilderNonFinite nonFinite (this);
            hr = nonFinite.hBuild();

            break;
        }
        case POS_PRONOUN:       // All forms are irregular
        case POS_NUM:
        {
            for (multimap<int, StIrregularForm>::iterator itWf = m_mmapIrregularForms.begin();
                 itWf != m_mmapIrregularForms.end();
                 ++itWf)
            {
                CComObject<CWordForm>* pWordForm = NULL;
                hr = CComObject<CWordForm>::CreateInstance(&pWordForm);
                if (S_OK != hr)
                {
                    ATLASSERT(0);
                    CString csMsg;
                    csMsg.Format (L"CreateInstance() failed, error %x.", hr);
                    ERROR_LOG ((LPCTSTR)csMsg);
                    throw CException (hr, (LPCTSTR)csMsg);
                }
                pWordForm->m_pLexeme = this;
                pWordForm->m_ePos = m_ePartOfSpeech;
                pWordForm->m_sLemma = m_sGraphicStem;     // is that correct?
                pWordForm->hDecodeHash (itWf->first);
                pWordForm->m_sWordForm = itWf->second.sForm;
                pWordForm->m_mapStress = itWf->second.mapStress;
                hAddWordForm (pWordForm);
            }
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
            CComObject<CWordForm>* pWordForm = NULL;
            hr = CComObject<CWordForm>::CreateInstance (&pWordForm);
            if (S_OK != hr)
            {
                ATLASSERT(0);
                CString csMsg;
                csMsg.Format (L"CreateInstance() failed, error %x.", hr);
                ERROR_LOG ((LPCTSTR)csMsg);
                throw CException (hr, (LPCTSTR)csMsg);
            }
            pWordForm->m_pLexeme = this;
            pWordForm->m_ePos = m_ePartOfSpeech;
            pWordForm->m_sLemma = m_sGraphicStem;
            pWordForm->m_sWordForm = m_sGraphicStem;
            for (vector<int>::iterator itStress = m_vecSourceStressPos.begin();
                 itStress != m_vecSourceStressPos.end(); ++itStress)
            {
                pWordForm->m_mapStress[*itStress] = STRESS_PRIMARY;
            }
            hAddWordForm (pWordForm);
            break;
        }
        default:
        {
            ERROR_LOG (L"Part of speech value not recognized.");
            return E_UNEXPECTED;
        }
    }

    return S_OK;

}   // hGenerateParadigm()

CEString CLexeme::sHash()
{
    StLexemeHasher stLh;
    stLh.sSourceForm = m_sSourceForm;
    vector<int>::iterator itHomonym = m_vecHomonyms.begin();
    for (; itHomonym != m_vecHomonyms.end(); ++itHomonym)
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
    stLh.sMainSymbol = m_sMainSymbol;
    stLh.sComment = m_sHeadwordComment;
    stLh.sComment += m_sComment;
    stLh.sComment += m_sTrailingComment;

    if (m_iType > -1)
    {
        stLh.iInflectionType = m_iType;
        stLh.iAccentType1 = (int)m_eAccentType1;
        stLh.iAccentType2 = (int)m_eAccentType2;
    }

    return stLh.sHash();

}   //  str_Hash()

HRESULT CLexeme::hAddIrregularForm (int iHash, const StIrregularForm& stIf)
{
    HRESULT hr = S_OK;

    m_mmapIrregularForms.insert (pair<int, StIrregularForm> (iHash, stIf));

    return S_OK;
}

HRESULT CLexeme::hGetFirstIrregularForm (int iHash, StIrregularForm& stIf)
{
    HRESULT hr = S_OK;

    pairItIfRange = m_mmapIrregularForms.equal_range (iHash);
    if (pairItIfRange.first == pairItIfRange.second)
    {
        return S_FALSE;
    }
    else
    {
        m_itCurrentIrregularForm = pairItIfRange.first;
        stIf = (*m_itCurrentIrregularForm).second;
        if ((*m_itCurrentIrregularForm).first != iHash)
        {
            return S_FALSE;
        }
        else
        {
            return S_OK;
        }
        ++m_itCurrentIrregularForm;
//        ++m_itCurrentIrregularForm;
    }

    return E_FAIL;

}   //  h_GetIrregularForm (...)

HRESULT CLexeme::hGetNextIrregularForm (int iHash, StIrregularForm& stIf)
{
    HRESULT hr = S_OK;

    if (pairItIfRange.first == pairItIfRange.second)
    {
        return E_UNEXPECTED;
    }

    if (iHash != (*pairItIfRange.first).first)
    {
        return E_UNEXPECTED;
    }

    if ((m_itCurrentIrregularForm == m_mmapIrregularForms.end() || 
        (*m_itCurrentIrregularForm).first != iHash))
    {
        return E_UNEXPECTED;
    }

    ++m_itCurrentIrregularForm;

    if ((m_itCurrentIrregularForm == m_mmapIrregularForms.end() || 
        (*m_itCurrentIrregularForm).first != iHash))
    {
        return S_FALSE;
    }

    stIf = (*m_itCurrentIrregularForm).second;

    return S_OK;

}   //  hGetNextIrregularForm (...)
    
HRESULT CLexeme::hFormExists (CGramHasher& hasher)
{
    HRESULT hr = S_OK;

    if (!m_bHasDeficiencies)
    {
        return S_TRUE;
    }

    int iHash = hasher.iGramHash();

    vector<int>::iterator itMf = m_vecMissingForms.begin();
    for (; itMf != m_vecMissingForms.end(); ++itMf)
    {
        if (iHash == *itMf)
        {
            return S_FALSE;
        }
    }

    return TRUE;

}   //  hFormExists (...)
