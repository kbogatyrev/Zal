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
    HRESULT h_r = hGenerateParadigm();
    return h_r;
}

HRESULT CLexeme::get_FormDescriptor (IFormFinder ** pp_fd)
{
    HRESULT h_r = S_OK;

    CComObject<CT_FormDescriptor> * pco_fd = NULL;
    h_r = CComObject<CT_FormDescriptor>::CreateInstance (&pco_fd);
    if (S_OK != h_r)
    {
        ERROR_LOG (L"CreateInstance failed for CT_FormDescriptor.");
        ATLASSERT(0);
        return h_r;
    }

    pco_fd->pco_Lexeme = this;
    CComPtr<CT_FormDescriptor> sp_ = pco_fd;
    h_r = pco_fd->QueryInterface<IFormFinder> (pp_fd);
    if (S_OK != h_r || !pp_fd)
    {
        ATLASSERT(0);
        ERROR_LOG (L"QI failed for IFormFinder.");
        return E_NOINTERFACE;
    }

    return h_r;
}

HRESULT CLexeme::get_LexemeId (LONG * pl_id)
{
    *pl_id = i_DbKey;
    return S_OK;
}

HRESULT CLexeme::get_GraphicStem (BSTR * pbstr_lemma)
{
    if (xstr_GraphicStem.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (xstr_GraphicStem.c_str());
    *pbstr_lemma = sp_bstr.Detach();

    return S_OK;
}   

HRESULT CLexeme::get_InitialForm (BSTR * pbstr_sourceForm)
{
    if (xstr_SourceForm.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (xstr_SourceForm.c_str());
    *pbstr_sourceForm = sp_bstr.Detach();

    return S_OK;
}

HRESULT CLexeme::get_IsVariant (BOOL * pb_variant)
{
    *pb_variant = b_IsVariant ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_MainSymbol (BSTR * pbstr_mainSymbol)
{
    if (str_MainSymbol.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_MainSymbol.c_str());
    *pbstr_mainSymbol = sp_bstr.Detach();

    return S_OK;
}

HRESULT CLexeme::get_IsPluralOf (BOOL * pui_isPluralOf)
{
    *pui_isPluralOf = b_IsPluralOf ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_IsTransitive (BOOL * pui_isTransitive)
{
    *pui_isTransitive = b_Transitive;
    return S_OK;
}

HRESULT CLexeme::get_Aspect (ET_Aspect * pe_aspect)
{
    *pe_aspect = eo_Aspect;
    return S_OK;
}

HRESULT CLexeme::get_IsReflexive (ET_Reflexive * pe_isReflexive)
{
    *pe_isReflexive = eo_Reflexive;
    return S_OK;
}

HRESULT CLexeme::get_MainSymbolPluralOf (BSTR * pbstr_msPluralOf)
{
    if (str_MainSymbolPluralOf.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_MainSymbolPluralOf.c_str());
    *pbstr_msPluralOf = sp_bstr.Detach();

    return S_OK;
}

HRESULT CLexeme::get_AltMainSymbol (BSTR * pbstr_altMainSymbol)
{
    if (str_AltMainSymbol.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_AltMainSymbol.c_str());
    *pbstr_altMainSymbol = sp_bstr.Detach();

    return S_OK;
}

HRESULT CLexeme::get_InflectionType (BSTR * pbm_sInflectionType)
{
    if (m_sInflectionType.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (m_sInflectionType.c_str());
    *pbm_sInflectionType = sp_bstr.Detach();

    return S_OK;
}

HRESULT CLexeme::get_PartOfSpeech (ET_PartOfSpeech * pe_pos)
{
    *pe_pos = eo_PartOfSpeech;
    return S_OK;
}

HRESULT CLexeme::get_Comment (BSTR * pbstr_comment)
{
    if (str_Comment.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_Comment.c_str());
    *pbstr_comment = sp_bstr.Detach();

    return S_OK;
}

HRESULT CLexeme::get_AltMainSymbolComment (BSTR * pbstr_altMsComment)
{
    if (str_AltMainSymbolComment.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_AltMainSymbolComment.c_str());
    *pbstr_altMsComment = sp_bstr.Detach();

    return S_OK;
}

HRESULT CLexeme::get_AltInflectionComment (BSTR * pbstr_altInflectionComment)
{
    if (str_AltInflectionComment.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_AltInflectionComment.c_str());
    *pbstr_altInflectionComment = sp_bstr.Detach();

    return S_OK;
}

HRESULT CLexeme::get_VerbStemAlternation (BSTR * bstr_verbStemAlternation)
{
    if (str_VerbStemAlternation.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_VerbStemAlternation.c_str());
    *bstr_verbStemAlternation = sp_bstr.Detach();

    return S_OK;
}

HRESULT CLexeme::get_Section (LONG * pl_section)
{
    *pl_section = (long)i_Section;
    return S_OK;
}

HRESULT CLexeme::get_NoComparative (BOOL * pui_noComparative)
{
    *pui_noComparative = b_NoComparative ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_HasAssumedForms (BOOL * pui_hasAssumedForms)
{
    *pui_hasAssumedForms = b_AssumedForms ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_HasYoAlternation (BOOL * pui_hasYoAlteration)
{
    *pui_hasYoAlteration = b_YoAlternation ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_HasOAlternation (BOOL * pui_hasOAlteration)
{
    *pui_hasOAlteration = b_OAlternation ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_HasSecondGenitive (BOOL * pui_hasSecondaryGenetive)
{
    *pui_hasSecondaryGenetive = b_SecondGenitive ? TRUE : FALSE;
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

HRESULT CLexeme::get_TrailingComment (BSTR * pbstr_trailingComment)
{
    if (str_TrailingComment.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_TrailingComment.c_str());
    *pbstr_trailingComment = sp_bstr.Detach();

    return S_OK;
}

// From Inflection table:
HRESULT CLexeme::get_IsPrimaryInflectionGroup (BOOL * pui_isPrimaryInflectionGroup)
{
    *pui_isPrimaryInflectionGroup = b_PrimaryInflectionGroup ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_Type (LONG * pl_type)
{
    *pl_type = i_Type;
    return S_OK;
}

HRESULT CLexeme::get_AccentType1 (ET_AccentType * pe_accentType1)
{
    *pe_accentType1 = eo_AccentType1;
    return S_OK;
}

HRESULT CLexeme::get_AccentType2 (ET_AccentType * pe_accentType2)
{
    *pe_accentType2 = eo_AccentType2;
    return S_OK;
}

HRESULT CLexeme::get_ShortFormsAreRestricted (BOOL * pui_shortFormsAreRestricted)
{
    *pui_shortFormsAreRestricted = b_ShortFormsRestricted ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_PastParticipleIsRestricted (BOOL * pui_pastParticipleIsRestricted)
{
    *pui_pastParticipleIsRestricted = b_PastParticipleRestricted ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_NoShortForms (BOOL * pui_noShortForms)
{
    *pui_noShortForms = b_NoShortForms ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_NoPastParticiple (BOOL * pui_noPastParticiple)
{
    *pui_noPastParticiple = b_NoPastParticiple ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_HasFleetingVowel (BOOL * pui_hasFleetingVowel)
{
    *pui_hasFleetingVowel = m_bFleetingVowel ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_HasStemAugment (BOOL * pui_hasStemAugment)
{
    *pui_hasStemAugment = i_StemAugment > -1 ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::get_HasIrregularForms (BOOL * pui_hasIrregularForms)
{
    *pui_hasIrregularForms = mmap_IrregularForms.empty() ? FALSE : TRUE;
    return S_OK;
}

HRESULT CLexeme::get_IsUnstressed (BOOL * pui_isUnstressed)
{
    *pui_isUnstressed = b_IsUnstressed ? TRUE : FALSE;
    return S_OK;
}

HRESULT CLexeme::SaveTestData()
{
    HRESULT h_r = S_OK;

    try
    {
        m_pDb->v_BeginTransaction();

        multimap<int, CComVariant>::iterator it_ = m_coll.begin();
        for (; it_ != m_coll.end(); ++it_)
        {
            int i_hash = (*it_).first;
            CComVariant sp_wf = (*it_).second;
            CComQIPtr<IWordForm> spqi_wf = sp_wf.punkVal;
            if (!spqi_wf)
            {
                ATLASSERT(0);
                ERROR_LOG (L"QI for ILexeme failed.");
                return E_NOINTERFACE;
            }        

            CWordForm * pco_wf = (CWordForm *)spqi_wf.p;
            h_r = pco_wf->SaveTestData();
            if (FAILED (h_r))
            {
                return h_r;
            }
        }

        m_pDb->v_CommitTransaction();
    }
    catch (...)
    {
        wstring str_msg;
        wstring str_error;
        try
        {
            m_pDb->GetLastError (str_error);
            str_msg += wstring (L", error %d: ");
            str_msg += str_error;
        }
        catch (...)
        {
            str_msg = L"Apparent DB error ";
        }
    
        CString cs_msg;
        cs_msg.Format (str_msg.c_str(), m_pDb->i_GetLastError());
        ERROR_LOG ((LPCTSTR)cs_msg);

        return E_FAIL;
    }
    
    return S_OK;

}   // SaveTestData()


//
//  Helpers
//

void CLexeme::Init()
{
    i_DbKey = -1;
    b_IsUnstressed = false;
    b_IsVariant = false;
    b_IsPluralOf = false;
    b_Transitive = false;
    eo_Reflexive = REFL_UNDEFINED;
    eo_PartOfSpeech = POS_UNDEFINED;
    i_Section = -1;
    b_PartPastPassZhd = false;
    b_NoComparative = false;
    b_AssumedForms = false;
    b_YoAlternation = false;
    b_SecondGenitive = false;
    b_HasAspectPair = false;
    i_AspectPairType = -1;
    b_HasIrregularForms = false;
    b_HasIrregularVariants = false;
    b_PrimaryInflectionGroup = false;
    i_Type = -1;
    eo_AccentType1 = AT_UNDEFINED;
    eo_AccentType2 = AT_UNDEFINED;
    b_ShortFormsRestricted = false;
    b_PastParticipleRestricted = false;
    b_NoLongForms = false;
    b_NoShortForms = false;
    b_NoPastParticiple = false;
    m_bFleetingVowel = false;
    i_StemAugment = -1;

    wstring arr_strMainSymbol[] = { L"м", L"мо", L"ж", L"жо", L"с", L"со", L"мо-жо", L"мн.",
     L"мн. неод.", L"мн. одуш.", L"мн. _от_", L"п", L"мс", L"мс-п", L"числ.", L"числ.-п", 
     L"св", L"нсв", L"св-нсв", L"н", L"предл.", L"союз", L"предик.", L"вводн.", L"сравн.", 
     L"част.", L"межд." };

    for (ET_MainSymbol eo_ms = MS_START; eo_ms < MS_END; ++eo_ms)
    {
        map_MainSymbol[arr_strMainSymbol[eo_ms]] = eo_ms; 
    }

    for (int i_ms = 0; i_ms < (int)MS_END; ++i_ms)
    {
        wstring str_ms = arr_strMainSymbol[i_ms];
        ET_MainSymbol eo_ms = map_MainSymbol[str_ms];
        switch (eo_ms)
        {
            case MS_M:
            {
                m_mapMainSymbolToAnimacy[str_ms] = ANIM_NO;
                m_mapMainSymbolToGender[str_ms] = GENDER_M;
                break;
            }
            case MS_MO:
            {
                m_mapMainSymbolToAnimacy[str_ms] = ANIM_YES;
                m_mapMainSymbolToGender[str_ms] = GENDER_M;
                break;
            }
            case MS_ZH:
            {
                m_mapMainSymbolToAnimacy[str_ms] = ANIM_NO;
                m_mapMainSymbolToGender[str_ms] = GENDER_F;
                break;
            }
            case MS_ZHO:
            case MS_MO_ZHO:
            {
                m_mapMainSymbolToAnimacy[str_ms] = ANIM_YES;
                m_mapMainSymbolToGender[str_ms] = GENDER_F;
                break;
            }
            case MS_S:
            {
                m_mapMainSymbolToAnimacy[str_ms] = ANIM_NO;
                m_mapMainSymbolToGender[str_ms] = GENDER_N;
                break;
            }
            case MS_SO:
            {
                m_mapMainSymbolToAnimacy[str_ms] = ANIM_YES;
                m_mapMainSymbolToGender[str_ms] = GENDER_N;
                break;
            }
            case MS_MN:
            {
                m_mapMainSymbolToAnimacy[str_ms] = ANIM_UNDEFINED;
                m_mapMainSymbolToGender[str_ms] = GENDER_UNDEFINED;
                break;
            }
            case MS_MN_NEOD:
            {
                m_mapMainSymbolToAnimacy[str_ms] = ANIM_NO;
                m_mapMainSymbolToGender[str_ms] = GENDER_UNDEFINED;
                break;
            }
            case MS_MN_ODUSH:
            {
                m_mapMainSymbolToAnimacy[str_ms] = ANIM_YES;
                m_mapMainSymbolToGender[str_ms] = GENDER_UNDEFINED;
                break;
            }
            case MS_MN_OT:
            {
                m_mapMainSymbolToAnimacy[str_ms] = ANIM_NO;
                m_mapMainSymbolToGender[str_ms] = GENDER_UNDEFINED;
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
                m_mapMainSymbolToAnimacy[str_ms] = ANIM_UNDEFINED;
                m_mapMainSymbolToGender[str_ms] = GENDER_UNDEFINED;
                break;
            }
            default:
            {
                ATLASSERT(0);
                ERROR_LOG (L"Unrecognized ending.");
                return;
            }

        }    // switch

    }   //  for (int i_ms = 0; ... )

    wstring arr_1stStage[] = 
        {L"б",  L"п",  L"в",  L"ф",  L"м",  L"з", L"с", L"д", L"т", L"ст", L"г", L"к", L"х", L"ск"};
    wstring arr_2ndStage[] = 
        {L"бл", L"пл", L"вл", L"фл", L"мл", L"ж", L"ш", L"ж", L"ч", L"щ",  L"ж", L"ч", L"ш", L"щ"}; 

    for (int i_sa = 0; i_sa < sizeof (arr_1stStage)/sizeof (wstring); ++i_sa)
    {
        map_StandardAlternations[arr_1stStage[i_sa]] = arr_2ndStage[i_sa];
    }

    wstring arr_preverbs[] = {L"в", L"над", L"об", L"от", L"под", L"пред", L"с"};
    for (int i_ap = 0; i_ap < sizeof (arr_preverbs)/sizeof (wstring); ++i_ap)
    {
        vec_AlternatingPreverbs.push_back (arr_preverbs[i_ap]);
    }

    wstring arr_preverbsV[] = {L"вс", L"вз", L"вос", L"воз", L"ис", L"из", L"нис", L"низ", L"рас", L"раз"};
    for (int i_ap = 0; i_ap < sizeof (arr_preverbsV)/sizeof (wstring); ++i_ap)
    {
        vec_AlternatingPreverbsWithVoicing.push_back (arr_preverbsV[i_ap]);
    }

    m_pDb = NULL;

    xstr_SourceForm.v_SetVowels (g_sVowels);
    xstr_GraphicStem.v_SetVowels (g_sVowels);
    xstr_1SgStem.v_SetVowels (g_sVowels);
    xstr_3SgStem.v_SetVowels (g_sVowels);
    xstr_Infinitive.v_SetVowels (g_sVowels);
    xstr_InfStem.v_SetVowels (g_sVowels);                        

}   // Init()

void CLexeme::v_SetDb (const wstring& str_dbPath)
{
    m_pDb = new CSqlite (str_dbPath);
}

HRESULT CLexeme::h_AddWordForm (CComObject<CWordForm> * pco_wordForm)
{
    if (NULL == pco_wordForm)
    {
        throw CT_Exception (E_POINTER, L"");
    }

    pair<int, CComVariant> co_pair (pco_wordForm->i_GramHash(), CComVariant (pco_wordForm));
    m_coll.insert (co_pair);

    return S_OK;

}  //  h_AddWordForm (...)

HRESULT CLexeme::h_GetIrregularForms()
{
    HRESULT hr = S_OK;

    if (!b_HasIrregularForms)
    {
        return S_FALSE;
    }

    mmap_IrregularForms.clear();

    b_HasIrregularVariants = false;

    wstring str_query 
        (L"SELECT id, gram_hash, wordform, is_alternative FROM irregular_forms WHERE descriptor_id = ");
    str_query += str_ToString (i_DbKey);
    str_query += L";";

    try
    {
        m_pDb->v_PrepareForSelect (str_query);
        while (m_pDb->b_GetRow())
        {
            StIrregularForm st_form;
            int i_id = -1;
            int i_hash = -1;
            m_pDb->v_GetData (0, i_id);
            m_pDb->v_GetData (1, i_hash);
            m_pDb->v_GetData (2, st_form.str_Form);
            m_pDb->v_GetData (3, st_form.b_IsVariant);

            if (st_form.b_IsVariant)
            {
                b_HasIrregularVariants = true;
            }

            wstring str_stressQuery (L"SELECT position, is_primary FROM irregular_stress WHERE form_id = ");
            str_stressQuery += str_ToString (i_id);
            str_stressQuery += L";";

            unsigned int ui_stressHandle = m_pDb->ui_PrepareForSelect (str_stressQuery);
            while (m_pDb->b_GetRow (ui_stressHandle))
            {
                int i_pos = -1;
                bool b_primary = false;
                m_pDb->v_GetData (0, i_pos, ui_stressHandle);
                m_pDb->v_GetData (1, b_primary, ui_stressHandle);
                int i_stressedSyll = CT_ExtString (st_form.str_Form).uiGetSyllableFromVowelPos (i_pos);
                st_form.map_Stress[i_stressedSyll] = b_primary ? STRESS_PRIMARY : STRESS_SECONDARY;
            }
            m_pDb->v_Finalize (ui_stressHandle);
            h_AddIrregularForm (i_hash, st_form);
        
        }   //  while (m_pDb->b_GetRow())

        m_pDb->v_Finalize();
    }
    catch (...)
    {
        wstring str_msg;
        wstring str_error;
        try
        {
            m_pDb->GetLastError (str_error);
            str_msg += wstring (L", error %d: ");
            str_msg += str_error;
        }
        catch (...)
        {
            str_msg = L"Apparent DB error ";
        }
    
        CString cs_msg;
        cs_msg.Format (str_msg.c_str(), m_pDb->i_GetLastError());
        ERROR_LOG ((LPCTSTR)cs_msg);
    }

    return S_OK;

}   //  h_GetIrregularForms()

HRESULT CLexeme::h_GetMissingForms()
{
    HRESULT hr = S_OK;

    if (!b_HasDeficiencies)
    {
        return S_FALSE;
    }

    vec_MissingForms.clear();

    wstring str_query (L"SELECT id, gram_hash FROM nonexistent_forms WHERE descriptor_id = ");
    str_query += str_ToString (i_DbKey);
    str_query += L";";

    try
    {
        m_pDb->v_PrepareForSelect (str_query);
        while (m_pDb->b_GetRow())
        {
            StIrregularForm st_form;
            int i_id = -1;
            int i_hash = -1;
            m_pDb->v_GetData (0, i_id);
            m_pDb->v_GetData (1, i_hash);
            vec_MissingForms.push_back (i_hash);

         }   //  while (m_pDb->b_GetRow())

        m_pDb->v_Finalize();
    }
    catch (...)
    {
        wstring str_msg;
        wstring str_error;
        try
        {
            m_pDb->GetLastError (str_error);
            str_msg += wstring (L", error %d: ");
            str_msg += str_error;
        }
        catch (...)
        {
            str_msg = L"Apparent DB error ";
        }
    
        CString cs_msg;
        cs_msg.Format (str_msg.c_str(), m_pDb->i_GetLastError());
        ERROR_LOG ((LPCTSTR)cs_msg);
    }

    return S_OK;

}   //  h_GetMissingForms()

HRESULT CLexeme::hGenerateParadigm()
{
    HRESULT h_r = S_OK;

    h_r = h_GetIrregularForms();
    if (FAILED(h_r))
    {
        return h_r;
    }

    h_r = h_GetMissingForms();
    if (FAILED(h_r))
    {
        return h_r;
    }

    m_coll.clear();

    switch (eo_PartOfSpeech)
    {
        case POS_NOUN:
        {
            CFormBuilderNouns co_b (this);
            h_r = co_b.h_Build();
            break;
        }
        case POS_ADJ:
        {
            CFormBuilderLongAdj co_lfb (this, xstr_GraphicStem.c_str(), eo_AccentType1, SUBPARADIGM_LONG_ADJ);
            h_r = co_lfb.h_Build();
            ATLASSERT (SUCCEEDED (h_r));

            if (!b_NoShortForms)
            {
                CFormBuilderShortAdj co_sfb (this);
                h_r = co_sfb.h_Build();
                ATLASSERT (S_OK == h_r);
            }

            if (!b_NoComparative)
            {
                CFormBuilderComparative co_cfb (this);
                co_cfb.h_Build();
            }
            break;
        }
        case POS_PRONOUN_ADJ:
        {
            CFormBuilderPronounAdj co_b (this, xstr_GraphicStem.c_str(), eo_AccentType1);
            h_r = co_b.h_Build();
            ATLASSERT (S_OK == h_r);
            break;
        }
        case POS_VERB:
        {
            eo_Aspect = ASPECT_UNDEFINED;
//            if (L"нсв" == str_MainSymbol)
            if (L"нсв" == m_sInflectionType)
            {
                eo_Aspect = ASPECT_IMPERFECTIVE;
            }
            else if (L"св" == m_sInflectionType)
            {
                eo_Aspect = ASPECT_PERFECTIVE;
            }

            CFormBuilderPersonal co_personal (this);
            h_r = co_personal.h_Build();

            CFormBuilderPast co_past (this);
            h_r = co_past.h_Build();

            CFormBuilderImperative co_imperative (this);
            h_r = co_imperative.h_Build();

            CFormBuilderNonFinite co_nonFinite (this);
            h_r = co_nonFinite.h_Build();

            break;
        }
        case POS_PRONOUN:       // All forms are irregular
        case POS_NUM:
        {
            for (multimap<int, StIrregularForm>::iterator iter_wf = mmap_IrregularForms.begin();
                 iter_wf != mmap_IrregularForms.end();
                 ++iter_wf)
            {
                CComObject<CWordForm>* pco_wordForm = NULL;
                h_r = CComObject<CWordForm>::CreateInstance(&pco_wordForm);
                if (S_OK != h_r)
                {
                    ATLASSERT(0);
                    CString cs_msg;
                    cs_msg.Format (L"CreateInstance() failed, error %x.", h_r);
                    ERROR_LOG ((LPCTSTR)cs_msg);
                    throw CT_Exception (h_r, (LPCTSTR)cs_msg);
                }
                pco_wordForm->pco_Lexeme = this;
                pco_wordForm->eo_POS = eo_PartOfSpeech;
                pco_wordForm->str_Lemma = xstr_GraphicStem.c_str();     // is that correct?
                pco_wordForm->h_DecodeHash(iter_wf->first);
                pco_wordForm->str_WordForm = iter_wf->second.str_Form;
                pco_wordForm->map_Stress = iter_wf->second.map_Stress;
                h_AddWordForm(pco_wordForm);
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
            CComObject<CWordForm>* pco_wordForm = NULL;
            h_r = CComObject<CWordForm>::CreateInstance(&pco_wordForm);
            if (S_OK != h_r)
            {
                ATLASSERT(0);
                CString cs_msg;
                cs_msg.Format (L"CreateInstance() failed, error %x.", h_r);
                ERROR_LOG ((LPCTSTR)cs_msg);
                throw CT_Exception (h_r, (LPCTSTR)cs_msg);
            }
            pco_wordForm->pco_Lexeme = this;
            pco_wordForm->eo_POS = eo_PartOfSpeech;
            pco_wordForm->str_Lemma = xstr_GraphicStem.c_str();
            pco_wordForm->str_WordForm = xstr_GraphicStem.c_str();
            for (vector<int>::iterator it_stress = vec_SourceStressPos.begin();
                 it_stress != vec_SourceStressPos.end(); ++it_stress)
            {
                pco_wordForm->map_Stress[*it_stress] = STRESS_PRIMARY;
            }
            h_AddWordForm(pco_wordForm);
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

wstring CLexeme::str_Hash()
{
    ST_LexemeHasher st_lh;
    st_lh.str_SourceForm = xstr_SourceForm.c_str();
    st_lh.str_MainSymbol = str_MainSymbol;
    st_lh.i_InflectionType = i_Type;
    st_lh.i_AccentType1 = (int)eo_AccentType1;
    st_lh.i_AccentType2 = (int)eo_AccentType2;
    st_lh.str_Comment = str_HeadwordComment;
    st_lh.str_Comment += str_Comment;
    st_lh.str_Comment += str_TrailingComment;

    vector<int>::iterator it_stress = vec_SourceStressPos.begin();
    for (; it_stress != vec_SourceStressPos.end(); ++it_stress)
    {
        int i_stressedChar = xstr_SourceForm.i_GetVowelPos (*it_stress);
        st_lh.map_Stress[i_stressedChar] = STRESS_PRIMARY;
    }

    it_stress = vec_SecondaryStressPos.begin();
    for (; it_stress != vec_SecondaryStressPos.end(); ++it_stress)
    {
        int i_stressedChar = xstr_SourceForm.i_GetVowelPos (*it_stress);
        st_lh.map_Stress[i_stressedChar] = STRESS_SECONDARY;
    }

    return st_lh.str_Hash();

}   //  str_Hash()

HRESULT CLexeme::h_AddIrregularForm (unsigned int ui_hash, const StIrregularForm& st_if)
{
    HRESULT h_r = S_OK;

    mmap_IrregularForms.insert (pair<int, StIrregularForm> (ui_hash, st_if));

    return S_OK;
}

HRESULT CLexeme::h_GetFirstIrregularForm (int i_hash, StIrregularForm& st_if)
{
    HRESULT h_r = S_OK;

    pair_ItIfRange = mmap_IrregularForms.equal_range (i_hash);
    if (pair_ItIfRange.first == pair_ItIfRange.second)
    {
        return S_FALSE;
    }
    else
    {
        it_CurrentIrregularForm = pair_ItIfRange.first;
        st_if = (*it_CurrentIrregularForm).second;
        if ((*it_CurrentIrregularForm).first != i_hash)
        {
            return S_FALSE;
        }
        else
        {
            return S_OK;
        }
        ++it_CurrentIrregularForm;
//        ++it_CurrentIrregularForm;
    }

    return E_FAIL;

}   //  h_GetIrregularForm (...)

HRESULT CLexeme::h_GetNextIrregularForm (int i_hash, StIrregularForm& st_if)
{
    HRESULT h_r = S_OK;

    if (pair_ItIfRange.first == pair_ItIfRange.second)
    {
        return E_UNEXPECTED;
    }

    if (i_hash != (*pair_ItIfRange.first).first)
    {
        return E_UNEXPECTED;
    }

    if ((it_CurrentIrregularForm == mmap_IrregularForms.end() || 
        (*it_CurrentIrregularForm).first != i_hash))
    {
        return E_UNEXPECTED;
    }

    ++it_CurrentIrregularForm;

    if ((it_CurrentIrregularForm == mmap_IrregularForms.end() || 
        (*it_CurrentIrregularForm).first != i_hash))
    {
        return S_FALSE;
    }

    st_if = (*it_CurrentIrregularForm).second;

    return S_OK;

}   //  h_GetNextIrregularForm (...)

HRESULT CLexeme::h_FormExists (CGramHasher& co_hasher)
{
    HRESULT h_r = S_OK;

    if (!b_HasDeficiencies)
    {
        return S_TRUE;
    }

    int i_hash = co_hasher.i_GramHash();

    vector<int>::iterator it_mf = vec_MissingForms.begin();
    for (; it_mf != vec_MissingForms.end(); ++it_mf)
    {
        if (i_hash == *it_mf)
        {
            return S_FALSE;
        }
    }

    return TRUE;

}   //  h_FormExists (...)
