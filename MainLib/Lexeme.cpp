#include "StdAfx.h"
#include "FormBuilderNouns.h"
#include "FormBuilderAdjLong.h"
#include "FormBuilderAdjShort.h"
#include "FormBuilderAdjComparative.h"
#include "FormBuilderVerbPers.h"
#include "FormBuilderVerbImpv.h"
#include "FormBuilderVerbPast.h"
#include "FormBuilderVerbNonFinite.h"
#include "LexemeHasher.h"
#include "Lexeme.h"

HRESULT CT_Lexeme::GenerateWordForms()
{
    HRESULT h_r = h_GenerateParadigm();
    return h_r;
}

HRESULT CT_Lexeme::get_FormDescriptor (IFormFinder ** pp_fd)
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

HRESULT CT_Lexeme::get_LexemeId (LONG * pl_id)
{
    *pl_id = i_DbKey;
    return S_OK;
}

HRESULT CT_Lexeme::get_GraphicStem (BSTR * pbstr_lemma)
{
    if (xstr_GraphicStem.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (xstr_GraphicStem.c_str());
    *pbstr_lemma = sp_bstr.Detach();

    return S_OK;
}   

HRESULT CT_Lexeme::get_InitialForm (BSTR * pbstr_sourceForm)
{
    if (xstr_SourceForm.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (xstr_SourceForm.c_str());
    *pbstr_sourceForm = sp_bstr.Detach();

    return S_OK;
}

HRESULT CT_Lexeme::get_IsVariant (BOOL * pb_variant)
{
    *pb_variant = b_IsVariant ? TRUE : FALSE;
    return S_OK;
}

HRESULT CT_Lexeme::get_MainSymbol (BSTR * pbstr_mainSymbol)
{
    if (str_MainSymbol.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_MainSymbol.c_str());
    *pbstr_mainSymbol = sp_bstr.Detach();

    return S_OK;
}

HRESULT CT_Lexeme::get_IsPluralOf (BOOL * pui_isPluralOf)
{
    *pui_isPluralOf = b_IsPluralOf ? TRUE : FALSE;
    return S_OK;
}

HRESULT CT_Lexeme::get_IsTransitive (BOOL * pui_isTransitive)
{
    *pui_isTransitive = b_Transitive;
    return S_OK;
}

HRESULT CT_Lexeme::get_IsReflexive (ET_Reflexive * pe_isReflexive)
{
    *pe_isReflexive = eo_Reflexive;
    return S_OK;
}

HRESULT CT_Lexeme::get_MainSymbolPluralOf (BSTR * pbstr_msPluralOf)
{
    if (str_MainSymbolPluralOf.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_MainSymbolPluralOf.c_str());
    *pbstr_msPluralOf = sp_bstr.Detach();

    return S_OK;
}

HRESULT CT_Lexeme::get_AltMainSymbol (BSTR * pbstr_altMainSymbol)
{
    if (str_AltMainSymbol.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_AltMainSymbol.c_str());
    *pbstr_altMainSymbol = sp_bstr.Detach();

    return S_OK;
}

HRESULT CT_Lexeme::get_InflectionType (BSTR * pbstr_inflectionType)
{
    if (str_InflectionType.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_InflectionType.c_str());
    *pbstr_inflectionType = sp_bstr.Detach();

    return S_OK;
}

HRESULT CT_Lexeme::get_PartOfSpeech (ET_PartOfSpeech * pe_pos)
{
    *pe_pos = eo_PartOfSpeech;
    return S_OK;
}

HRESULT CT_Lexeme::get_Comment (BSTR * pbstr_comment)
{
    if (str_Comment.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_Comment.c_str());
    *pbstr_comment = sp_bstr.Detach();

    return S_OK;
}

HRESULT CT_Lexeme::get_AltMainSymbolComment (BSTR * pbstr_altMsComment)
{
    if (str_AltMainSymbolComment.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_AltMainSymbolComment.c_str());
    *pbstr_altMsComment = sp_bstr.Detach();

    return S_OK;
}

HRESULT CT_Lexeme::get_AltInflectionComment (BSTR * pbstr_altInflectionComment)
{
    if (str_AltInflectionComment.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_AltInflectionComment.c_str());
    *pbstr_altInflectionComment = sp_bstr.Detach();

    return S_OK;
}

HRESULT CT_Lexeme::get_VerbStemAlternation (BSTR * bstr_verbStemAlternation)
{
    if (str_VerbStemAlternation.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_VerbStemAlternation.c_str());
    *bstr_verbStemAlternation = sp_bstr.Detach();

    return S_OK;
}

HRESULT CT_Lexeme::get_Section (LONG * pl_section)
{
    *pl_section = (long)i_Section;
    return S_OK;
}

HRESULT CT_Lexeme::get_NoComparative (BOOL * pui_noComparative)
{
    *pui_noComparative = b_NoComparative ? TRUE : FALSE;
    return S_OK;
}

HRESULT CT_Lexeme::get_HasAssumedForms (BOOL * pui_hasAssumedForms)
{
    *pui_hasAssumedForms = b_AssumedForms ? TRUE : FALSE;
    return S_OK;
}

HRESULT CT_Lexeme::get_HasYoAlternation (BOOL * pui_hasYoAlteration)
{
    *pui_hasYoAlteration = b_YoAlternation ? TRUE : FALSE;
    return S_OK;
}

HRESULT CT_Lexeme::get_HasOAlternation (BOOL * pui_hasOAlteration)
{
    *pui_hasOAlteration = b_OAlternation ? TRUE : FALSE;
    return S_OK;
}

HRESULT CT_Lexeme::get_HasSecondGenitive (BOOL * pui_hasSecondaryGenetive)
{
    *pui_hasSecondaryGenetive = b_SecondGenitive ? TRUE : FALSE;
    return S_OK;
}

HRESULT CT_Lexeme::get_QuestionableForms (BSTR *)
{
    return E_NOTIMPL;
}

HRESULT CT_Lexeme::get_IrregularForms (BSTR *)
{
    return E_NOTIMPL;
}

HRESULT CT_Lexeme::get_RestrictedForms (BSTR *)
{
    return E_NOTIMPL;
}

HRESULT CT_Lexeme::get_Contexts (BSTR *)
{
    return E_NOTIMPL;
}

HRESULT CT_Lexeme::get_TrailingComment (BSTR * pbstr_trailingComment)
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
HRESULT CT_Lexeme::get_IsPrimaryInflectionGroup (BOOL * pui_isPrimaryInflectionGroup)
{
    *pui_isPrimaryInflectionGroup = b_PrimaryInflectionGroup ? TRUE : FALSE;
    return S_OK;
}

HRESULT CT_Lexeme::get_Type (LONG * pl_type)
{
    *pl_type = i_Type;
    return S_OK;
}

HRESULT CT_Lexeme::get_AccentType1 (ET_AccentType * pe_accentType1)
{
    *pe_accentType1 = eo_AccentType1;
    return S_OK;
}

HRESULT CT_Lexeme::get_AccentType2 (ET_AccentType * pe_accentType2)
{
    *pe_accentType2 = eo_AccentType2;
    return S_OK;
}

HRESULT CT_Lexeme::get_ShortFormsAreRestricted (BOOL * pui_shortFormsAreRestricted)
{
    *pui_shortFormsAreRestricted = b_ShortFormsRestricted ? TRUE : FALSE;
    return S_OK;
}

HRESULT CT_Lexeme::get_PastParticipleIsRestricted (BOOL * pui_pastParticipleIsRestricted)
{
    *pui_pastParticipleIsRestricted = b_PastParticipleRestricted ? TRUE : FALSE;
    return S_OK;
}

HRESULT CT_Lexeme::get_NoShortForms (BOOL * pui_noShortForms)
{
    *pui_noShortForms = b_NoShortForms ? TRUE : FALSE;
    return S_OK;
}

HRESULT CT_Lexeme::get_NoPastParticiple (BOOL * pui_noPastParticiple)
{
    *pui_noPastParticiple = b_NoPastParticiple ? TRUE : FALSE;
    return S_OK;
}

HRESULT CT_Lexeme::get_HasFleetingVowel (BOOL * pui_hasFleetingVowel)
{
    *pui_hasFleetingVowel = b_FleetingVowel ? TRUE : FALSE;
    return S_OK;
}

HRESULT CT_Lexeme::get_HasStemAugment (BOOL * pui_hasStemAugment)
{
    *pui_hasStemAugment = i_StemAugment > -1 ? TRUE : FALSE;
    return S_OK;
}

HRESULT CT_Lexeme::get_IsUnstressed (BOOL * pui_isUnstressed)
{
    *pui_isUnstressed = b_IsUnstressed ? TRUE : FALSE;
    return S_OK;
}

HRESULT CT_Lexeme::SaveTestData()
{
    HRESULT h_r = S_OK;

    try
    {
        pco_Db->v_BeginTransaction();

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

            CT_WordForm * pco_wf = (CT_WordForm *)spqi_wf.p;
            h_r = pco_wf->SaveTestData();
            if (FAILED (h_r))
            {
                return h_r;
            }
        }

        pco_Db->v_CommitTransaction();
    }
    catch (...)
    {
        wstring str_msg;
        wstring str_error;
        try
        {
            pco_Db->v_GetLastError (str_error);
            str_msg += wstring (L", error %d: ");
            str_msg += str_error;
        }
        catch (...)
        {
            str_msg = L"Apparent DB error ";
        }
    
        CString cs_msg;
        cs_msg.Format (str_msg.c_str(), pco_Db->i_GetLastError());
        ERROR_LOG ((LPCTSTR)cs_msg);

        return E_FAIL;
    }
    
    return S_OK;

}   // SaveTestData()


//
//  Helpers
//

void CT_Lexeme::v_Init()
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
    b_NoShortForms = false;
    b_NoPastParticiple = false;
    b_FleetingVowel = false;
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
                map_MainSymbolToAnimacy[str_ms] = ANIM_NO;
                map_MainSymbolToGender[str_ms] = GENDER_M;
                break;
            }
            case MS_MO:
            {
                map_MainSymbolToAnimacy[str_ms] = ANIM_YES;
                map_MainSymbolToGender[str_ms] = GENDER_M;
                break;
            }
            case MS_ZH:
            {
                map_MainSymbolToAnimacy[str_ms] = ANIM_NO;
                map_MainSymbolToGender[str_ms] = GENDER_F;
                break;
            }
            case MS_ZHO:
            case MS_MO_ZHO:
            {
                map_MainSymbolToAnimacy[str_ms] = ANIM_YES;
                map_MainSymbolToGender[str_ms] = GENDER_F;
                break;
            }
            case MS_S:
            {
                map_MainSymbolToAnimacy[str_ms] = ANIM_NO;
                map_MainSymbolToGender[str_ms] = GENDER_N;
                break;
            }
            case MS_SO:
            {
                map_MainSymbolToAnimacy[str_ms] = ANIM_YES;
                map_MainSymbolToGender[str_ms] = GENDER_N;
                break;
            }
            case MS_MN:
            {
                map_MainSymbolToAnimacy[str_ms] = ANIM_UNDEFINED;
                map_MainSymbolToGender[str_ms] = GENDER_UNDEFINED;
                break;
            }
            case MS_MN_NEOD:
            {
                map_MainSymbolToAnimacy[str_ms] = ANIM_NO;
                map_MainSymbolToGender[str_ms] = GENDER_UNDEFINED;
                break;
            }
            case MS_MN_ODUSH:
            {
                map_MainSymbolToAnimacy[str_ms] = ANIM_YES;
                map_MainSymbolToGender[str_ms] = GENDER_UNDEFINED;
                break;
            }
            case MS_MN_OT:
            {
                map_MainSymbolToAnimacy[str_ms] = ANIM_NO;
                map_MainSymbolToGender[str_ms] = GENDER_UNDEFINED;
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
                map_MainSymbolToAnimacy[str_ms] = ANIM_UNDEFINED;
                map_MainSymbolToGender[str_ms] = GENDER_UNDEFINED;
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

    pco_Db = NULL;

    xstr_SourceForm.v_SetVowels (str_Vowels);
    xstr_GraphicStem.v_SetVowels (str_Vowels);
    xstr_1SgStem.v_SetVowels (str_Vowels);
    xstr_3SgStem.v_SetVowels (str_Vowels);
    xstr_Infinitive.v_SetVowels (str_Vowels);
    xstr_InfStem.v_SetVowels (str_Vowels);                        

}   // v_Init()

void CT_Lexeme::v_SetDb (const wstring& str_dbPath)
{
    pco_Db = new CT_Sqlite (str_dbPath);
}

HRESULT CT_Lexeme::h_AddWordForm (CComObject<CT_WordForm> * pco_wordForm)
{
    if (NULL == pco_wordForm)
    {
        throw CT_Exception (E_POINTER, L"");
    }

    CComQIPtr<IWordForm> spqi_wf (pco_wordForm);
    if (!spqi_wf)
    {
        ATLASSERT(0);
        ERROR_LOG (L"QI for IWordForm failed.");
        return E_NOINTERFACE;
    }
    pair<int, CComVariant> co_pair (pco_wordForm->i_GramHash(), CComVariant (spqi_wf));
    m_coll.insert (co_pair);

    return S_OK;

}  //  h_AddWordForm (...)

HRESULT CT_Lexeme::h_GetIrregularForms()
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
        pco_Db->v_PrepareForSelect (str_query);

        while (pco_Db->b_GetRow())
        {
            ST_IrregularForm st_form;
            int i_id = -1;
            int i_hash = -1;
            pco_Db->v_GetData (0, i_id);
            pco_Db->v_GetData (1, i_hash);
            pco_Db->v_GetData (2, st_form.str_Form);
            pco_Db->v_GetData (3, st_form.b_IsVariant);

            if (st_form.b_IsVariant)
            {
                b_HasIrregularVariants = true;
            }

            wstring str_stressQuery (L"SELECT position, is_primary FROM irregular_stress WHERE form_id = ");
            str_stressQuery += str_ToString (i_id);
            str_stressQuery += L";";
            unsigned int ui_stressHandle = pco_Db->ui_PrepareForSelect (str_stressQuery);
            while (pco_Db->b_GetRow (ui_stressHandle))
            {
                int i_pos = -1;
                bool b_primary = false;
                pco_Db->v_GetData (0, i_pos, ui_stressHandle);
                pco_Db->v_GetData (1, b_primary, ui_stressHandle);
                int i_stressedSyll = CT_ExtString (st_form.str_Form).i_GetSyllableFromVowelPos (i_pos);
                st_form.map_Stress[i_stressedSyll] = b_primary ? STRESS_PRIMARY : STRESS_SECONDARY;
            }
            h_AddIrregularForm (i_hash, st_form);
        }
    }
    catch (...)
    {
        wstring str_msg;
        wstring str_error;
        try
        {
            pco_Db->v_GetLastError (str_error);
            str_msg += wstring (L", error %d: ");
            str_msg += str_error;
        }
        catch (...)
        {
            str_msg = L"Apparent DB error ";
        }
    
        CString cs_msg;
        cs_msg.Format (str_msg.c_str(), pco_Db->i_GetLastError());
        ERROR_LOG ((LPCTSTR)cs_msg);
    }

    return S_OK;

}   //  h_GetIrregularForms()

HRESULT CT_Lexeme::h_GenerateParadigm()
{
    HRESULT h_r = S_OK;

    h_r = h_GetIrregularForms();
    if (FAILED(h_r))
    {
        return h_r;
    }

    m_coll.clear();

    switch (eo_PartOfSpeech)
    {
        case POS_NOUN:
        {
            CT_FormBuilderNouns co_b (this);
            h_r = co_b.h_Build();
            break;
        }
        case POS_ADJ:
        {
            CT_FormBuilderLongAdj co_lfb (this, xstr_GraphicStem.c_str(), eo_AccentType1, SUBPARADIGM_LONG_ADJ);
            h_r = co_lfb.h_Build();
            ATLASSERT (S_OK == h_r);
            if (L"мс" == str_InflectionType)
            {
                break;
            }

            if (!b_NoShortForms)
            {
                CT_FormBuilderShortAdj co_sfb (this);
                h_r = co_sfb.h_Build();
                ATLASSERT (S_OK == h_r);
            }

            if (!b_NoComparative)
            {
                CT_FormBuilderComparative co_cfb (this);
                co_cfb.h_Build();
            }
            break;
        }
        case POS_VERB:
        {
            CT_FormBuilderPersonal co_personal (this);
            h_r = co_personal.h_Build();

            CT_FormBuilderPast co_past (this);
            h_r = co_past.h_Build();

            CT_FormBuilderImperative co_imperative (this);
            h_r = co_imperative.h_Build();

            CT_FormBuilderNonFinite co_nonFinite (this);
            h_r = co_nonFinite.h_Build();

            break;
        }
        default:
        {
            ERROR_LOG (L"Part of speech value not recognized.");
            return E_UNEXPECTED;
        }
    }

    return S_OK;

}   // h_GenerateParadigm()

wstring CT_Lexeme::str_Hash()
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

HRESULT CT_Lexeme::h_AddIrregularForm (unsigned int ui_hash, const ST_IrregularForm& st_if)
{
    HRESULT h_r = S_OK;

    mmap_IrregularForms.insert (pair<int, ST_IrregularForm> (ui_hash, st_if));

    return S_OK;
}

HRESULT CT_Lexeme::h_GetIrregularForm (int i_hash, ST_IrregularForm& st_if)
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
    }

    return E_FAIL;

}   //  h_GetIrregularForm (...)

HRESULT CT_Lexeme::h_GetNextIrregularForm (int i_hash, ST_IrregularForm& st_if)
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

    st_if = (*it_CurrentIrregularForm).second;
    ++it_CurrentIrregularForm;
    if ((it_CurrentIrregularForm == mmap_IrregularForms.end() || 
        (*it_CurrentIrregularForm).first != i_hash))
    {
        return S_FALSE;
    }
    else
    {
        return S_OK;
    }

    return S_OK;

}   //  h_GetNextIrregularForm (...)
