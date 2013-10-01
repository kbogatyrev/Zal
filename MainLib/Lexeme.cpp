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

CLexeme::CLexeme() : 
    m_iDbKey(-1),
    m_bIsUnstressed(false),
    m_bIsVariant(false),
    m_bIsPluralOf(false),
    m_bTransitive(false),
    m_eReflexive(REFL_UNDEFINED),
    m_ePartOfSpeech(POS_UNDEFINED),
    m_iSection(-1),
    m_bPartPastPassZhd(false),
    m_bNoComparative(false),
    m_bAssumedForms(false),
    m_bYoAlternation(false),
    m_bSecondGenitive(false),
    m_bHasAspectPair(false),
    m_iAspectPairType(-1),
    m_bHasIrregularForms(false),
    m_bHasIrregularVariants(false),
    m_bPrimaryInflectionGroup(false),
    m_iType(-1),
    m_eAccentType1(AT_UNDEFINED),
    m_eAccentType2(AT_UNDEFINED),
    m_bShortFormsRestricted(false),
    m_bPastParticipleRestricted(false),
    m_bNoLongForms(false),
    m_bNoShortForms(false),
    m_bNoPastParticiple(false),
    m_bFleetingVowel(false),
    m_iStemAugment(-1)
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

    m_sSourceForm.SetVowels (g_szVowels);
    m_sGraphicStem.SetVowels (g_szVowels);
    m_s1SgStem.SetVowels (g_szVowels);
    m_s3SgStem.SetVowels (g_szVowels);
    m_sInfinitive.SetVowels (g_szVowels);
    m_sInfStem.SetVowels (g_szVowels);                        

}   // Ctor

void CLexeme::SetDb (const CEString& sDbPath)
{
    m_pDb = new CSqlite (sDbPath);
}

void CLexeme::AddWordForm (CWordForm * pWordForm)
{
    if (NULL == pWordForm)
    {
        throw CException (E_POINTER, L"");
    }

    pair<int, CWordForm *> pairHW (pWordForm->iGramHash(), pWordForm);
    m_mmapWordForms.insert (pairHW);

}  //  AddWordForm (...)

void CLexeme::WordFormFromHash (int iHash, unsigned int uiAt, CWordForm *& pWf)
{
    pair<multimap<int, CWordForm *>::iterator, multimap<int, CWordForm *>::iterator> pairRange;
    pairRange = m_mmapWordForms.equal_range (iHash);
    if (pairRange.first == pairRange.second)
    {
        ATLASSERT(0);
        CEString sMsg (L"Unable to locate word form in collection.");
        ERROR_LOG (sMsg);
        throw CException (E_FAIL, sMsg);
    }

    if (distance (pairRange.first, pairRange.second) - 1 < (int)uiAt)
    {
        ATLASSERT(0);
        CEString sMsg (L"Bad form number.");
        ERROR_LOG (sMsg);
        throw CException (E_INVALIDARG, sMsg);
    }

    multimap<int, CWordForm *>::iterator itWf (pairRange.first);
    advance (itWf, uiAt);
    if ((*itWf).first != iHash)
    {
        ATLASSERT(0);
        CEString sMsg (L"Error extracting map element.");
        ERROR_LOG (sMsg);
        throw CException (E_INVALIDARG, sMsg);
    }

	pWf = itWf->second;

}   //  WordFormFromHash (...)

bool CLexeme::bNoRegularForms (int iGramHash)
{
    CWordForm * pWf = NULL;         // currently not nused
    bool bIsVariant = false;
    HRESULT hr = hGetFirstIrregularForm (iGramHash, pWf, bIsVariant);
    if (S_FALSE == hr)
    {
        return false;
    }
    if (hr != S_OK && hr != S_FALSE)
    {
        ATLASSERT(0);
        CEString sMsg (L"hGetFirstIrregularForm() failed, error ");
        sMsg += CEString::sToString(hr);
        ERROR_LOG (sMsg);
        throw CException (E_FAIL, sMsg);
    }

    if (bIsVariant)    // irregular form is optional i.e. we still need to produce regular one  
    {
        return false;
    }
    
    while (S_OK == hr)
    {
        bIsVariant = false;        
        hr = hGetNextIrregularForm (pWf, bIsVariant);
        if (hr != S_OK && hr != S_FALSE)
        {
            ATLASSERT(0);
            CEString sMsg (L"hGetFirstIrregularForm() failed, error ");
            sMsg += CEString::sToString(hr);
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
    HRESULT hr = S_OK;

    CWordForm * pWf = NULL;
    bool bIsOptional = false;       // ignored
    hr = hGetFirstIrregularForm(iGramHash, pWf, bIsOptional);
    if (NULL == pWf || (hr != S_OK && hr != S_FALSE))
    {
        ATLASSERT(0);
        CEString sMsg (L"Irregular form not found in database.");
        ERROR_LOG (sMsg);
        throw CException (hr, sMsg);
    }

    return (S_OK == hr) ? true : false;
}

void CLexeme::GetIrregularForms(int iHash, map<CWordForm *, bool>& mapResult)
{
    HRESULT hr = S_OK;

    CWordForm * pWf = NULL;
    bool bIsOptional = false;
    hr = hGetFirstIrregularForm(iHash, pWf, bIsOptional);
    if (NULL == pWf || (hr != S_OK && hr != S_FALSE))
    {
        ATLASSERT(0);
        CEString sMsg (L"Irregular form not found in database.");
        ERROR_LOG (sMsg);
        throw CException (hr, sMsg);
    }

    if (S_OK == hr)
    {
        mapResult[pWf] = bIsOptional;
    }

    do
    {
        hr = hGetNextIrregularForm(pWf, bIsOptional);
        if (hr != S_OK && hr != S_FALSE)
        {
            ATLASSERT(0);
            CEString sMsg (L"Error accessing irregular m Sg Past form.");
            ERROR_LOG (sMsg);
            throw CException (hr, sMsg);
        }

        if (S_OK == hr)
        {
            map<CWordForm *, bool>::iterator it = mapResult.find(pWf);
            if (it != mapResult.end())
            {
                ATLASSERT(0);
                CEString sMsg (L"Duplicate irregulars.");
                ERROR_LOG (sMsg);
                throw CException (E_FAIL, sMsg);
            }

            mapResult[pWf] = bIsOptional;
        }

    }  while (S_OK == hr);

}   //  GetIrregularForms()

unsigned int CLexeme::uiFormCount (int iHash)
{
    return m_mmWordForms.count (iHash);
}

bool CLexeme::bHasCommonDeviation (int iCd)
{
    bool bRet = false;
    map<int, bool>::iterator itCd = m_mapCommonDeviations.find (iCd);
    return (itCd != m_mapCommonDeviations.end());
}

bool CLexeme::bDeviationOptional (int iCd)
{
    bool bRet = false;
    map<int, bool>::iterator itCd = m_mapCommonDeviations.find (iCd);
    if (itCd != m_mapCommonDeviations.end())
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


//
// Private
//
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
                m_bHasIrregularVariants = true;
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
            
            pair<int, StIrregularForm> pairHashToStruct (iHash, StIrregularForm(pWf, bIsVariant));
            m_mmapIrregularForms.insert(pairHashToStruct);

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

	m_mmWordForms.clear();

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
//            for (multimap<int, bool>::iterator itWf = m_mmapIrregularForms.begin();
//                 itWf != m_mmapIrregularForms.end();
//                 ++itWf)
//            {
//				CWordForm * pWordForm = new CWordForm;
//                HRESULT hInit = pWordForm->hInitFromHash (itWf->first);
//				if (hInit != S_OK)
//				{
//					CEString sMsg = L"Unable to decode gram hash " + CEString::sToString<int>(itWf->first) + L".";
//					ERROR_LOG (sMsg);
//					continue;
//				}
//                pWordForm->m_pLexeme = this;
////                pWordForm->m_ePos = m_ePartOfSpeech;
//                pWordForm->m_sLemma = m_sGraphicStem;     // is that correct?
//                pWordForm->m_sWordForm = itWf->second.sForm;
//                pWordForm->m_mapStress = itWf->second.mapStress;
//				m_vecWordForms.push_back(pWordForm);
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
            pWordForm->m_ePos = m_ePartOfSpeech;
            pWordForm->m_sLemma = m_sGraphicStem;
            pWordForm->m_sWordForm = m_sGraphicStem;
            for (vector<int>::iterator itStress = m_vecSourceStressPos.begin();
                 itStress != m_vecSourceStressPos.end(); ++itStress)
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

//HRESULT CLexeme::hAddIrregularForm (int iHash, const StIrregularForm& stIf)
//{
//    HRESULT hr = S_OK;

//    m_mmapIrregularForms.insert (pair<int, StIrregularForm> (iHash, stIf));

//    return S_OK;
//}

HRESULT CLexeme::hGetFirstIrregularForm (int iHash, CWordForm*& pWordForm, bool& bIsOptional)
{
    HRESULT hr = S_OK;

    m_pairItIfRange = m_mmapIrregularForms.equal_range (iHash);
    if (m_pairItIfRange.first == m_pairItIfRange.second)
    {
        return S_FALSE;
    }
    else
    {
        m_itCurrentIrregularForm = m_pairItIfRange.first;
        if ((*m_itCurrentIrregularForm).first != iHash)
        {
            return S_FALSE;
        }
        else
        {
            pWordForm = (*m_itCurrentIrregularForm).second.pWordForm;
            bIsOptional = (*m_itCurrentIrregularForm).second.bIsOptional;
            return S_OK;
        }
    }

}   //  hGetFirstIrregularForm (...)

HRESULT CLexeme::hGetNextIrregularForm (CWordForm *& pWordForm, bool& bIsOptional)
{
    HRESULT hr = S_OK;

    if (m_pairItIfRange.first == m_pairItIfRange.second)
    {
        return E_UNEXPECTED;
    }

//    if (iHash != (*pairItIfRange.first).first)
//    {
//        return E_UNEXPECTED;
//    }

    if (m_itCurrentIrregularForm == m_mmapIrregularForms.end())
//        || (*m_itCurrentIrregularForm).first != iHash))
    {
        return E_UNEXPECTED;
    }

    ++m_itCurrentIrregularForm;

    if (m_itCurrentIrregularForm == m_mmapIrregularForms.end())
//        || (*m_itCurrentIrregularForm).first != iHash))
    {
        return S_FALSE;
    }

    pWordForm = (*m_itCurrentIrregularForm).second.pWordForm;
    bIsOptional = (*m_itCurrentIrregularForm).second.bIsOptional;

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
