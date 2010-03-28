#include "StdAfx.h"
#include "FormBuilderNouns.h"
#include "FormBuilderAdjLong.h"
#include "FormBuilderAdjShort.h"
#include "FormBuilderAdjComparative.h"
#include "FormBuilderVerbPers.h"
#include "FormBuilderVerbImpv.h"
#include "FormBuilderVerbPast.h"
#include "FormBuilderVerbNonFinite.h"

#include "Lexeme.h"

HRESULT CT_Lexeme::GenerateWordForms()
{
    HRESULT h_r = h_GenerateParadigm();
    return h_r;
}

HRESULT CT_Lexeme::get_LexemeId (LONG * pl_id)
{
    *pl_id = i_DbKey;
    return S_OK;
}

HRESULT CT_Lexeme::get_GraphicStem (BSTR * pbstr_lemma)
{
    if (str_GraphicStem.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_GraphicStem.c_str());
    *pbstr_lemma = sp_bstr.Detach();

    return S_OK;
}   

HRESULT CT_Lexeme::get_InitialForm (BSTR * pbstr_sourceForm)
{
    if (str_SourceForm.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_SourceForm.c_str());
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
    *pui_hasStemAugment = b_StemAugment ? TRUE : FALSE;
    return S_OK;
}

/*
HRESULT CT_Lexeme::get_HasSecondaryStress (BOOL * pui_hasSecondaryStress)
{
    *pui_hasSecondaryStress = (vec_SecondaryStressPos.size() > 0) ? TRUE : FALSE;
    return S_OK;
}

HRESULT CT_Lexeme::get_IsPolytonic (BOOL * pui_isPolytonic)
{
    return E_NOTIMPL;
}

HRESULT CT_Lexeme::get_HasStressVariations (BOOL * pui_hasStressVariations)
{
    return E_NOTIMPL;
}
*/

//
//  Helpers
//

void CT_Lexeme::v_Init()
{
    i_DbKey = -1;
    b_IsVariant = false;
    b_IsPluralOf = false;
    b_Transitive = false;
    eo_PartOfSpeech = POS_UNDEFINED;
    i_Section = -1;
    b_NoComparative = false;
    b_AssumedForms = false;
    b_YoAlternation = false;
    b_SecondGenitive = false;
    b_PrimaryInflectionGroup = false;
    i_Type = -1;
    eo_AccentType1 = AT_UNDEFINED;
    eo_AccentType2 = AT_UNDEFINED;
    b_ShortFormsRestricted = false;
    b_PastParticipleRestricted = false;
    b_NoShortForms = false;
    b_NoPastParticiple = false;
    b_FleetingVowel = false;
    b_StemAugment = false;
//    i_CommonDeviation = -1;

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
    for (int i_ap = 0; i_ap < sizeof (arr_preverbs)/sizeof (wstring); ++i_ap)
    {
        vec_AlternatingPreverbsWithVoicing.push_back (arr_preverbs[i_ap]);
    }

    pco_Db = NULL;
//    pco_Endings = NULL;

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

    pair<int, CComVariant> co_pair (pco_wordForm->i_GramHash(), CComVariant (pco_wordForm));
    m_coll.insert (co_pair);

    return S_OK;
}

/*
HRESULT CT_Lexeme::h_CloneWordForm (CComObject<CT_WordForm> * p_source, 
                                    CComObject<CT_WordForm> *& p_clone)
{
    HRESULT h_r = S_OK;

    h_r = CComObject<CT_WordForm>::CreateInstance (&p_clone);
    if (S_OK != h_r)
    {
        ATLASSERT(0);
        return h_r;
    }
    p_clone->str_WordForm = p_source->str_WordForm;
    p_clone->str_Lemma = p_source->str_Lemma;
    p_clone->ll_Lexeme_id = p_source->ll_Lexeme_id;
    p_clone->vec_Stress = p_source->vec_Stress;
    p_clone->eo_POS = p_source->eo_POS;
    p_clone->eo_Case = p_source->eo_Case;
    p_clone->eo_Number = p_source->eo_Number;
    p_clone->eo_Subparadigm = p_source->eo_Subparadigm;
    p_clone->eo_Gender = p_source->eo_Gender;
    p_clone->eo_Tense = p_source->eo_Tense;
    p_clone->eo_Person = p_source->eo_Person;
    p_clone->eo_Animacy = p_source->eo_Animacy;
    p_clone->eo_Reflexive = p_source->eo_Reflexive;
    p_clone->eo_Voice = p_source->eo_Voice;

    return S_OK;
}
*/

HRESULT CT_Lexeme::h_GenerateParadigm()
{
    HRESULT h_r = S_OK;

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
            CT_FormBuilderLongAdj co_lfb (this, str_GraphicStem, eo_AccentType1, SUBPARADIGM_LONG_ADJ);
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
            h_r = h_BuildVerbStems();
            if (S_OK != h_r)
            {
                return h_r;
            }

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
            return E_UNEXPECTED;
        }
    }

    return S_OK;

}   // h_GenerateParadigm()

HRESULT CT_Lexeme::h_BuildVerbStems()
{
    HRESULT h_r = S_OK;
    
    xstr_Infinitive = str_SourceForm;
    if (xstr_Infinitive.b_EndsWith (L"ться") || 
        xstr_Infinitive.b_EndsWith (L"тись") || 
        xstr_Infinitive.b_EndsWith (L"чься"))
    {
        b_Reflexive = true;
    }
    else
    {
        b_Reflexive = false;
    }

    xstr_InfStem = str_GraphicStem;

    switch (i_Type)
    {
        case 1:         // делать, читать(ся), верстать, терять, жалеть, читать, стараться
        {
            xstr_1SgStem = xstr_3SgStem = str_GraphicStem;
            break;
        }

        case 2:         // требовать, рисовать(ся), тушевать, малевать, ковать, жевать, клевать, радоваться
        {
            xstr_1SgStem = xstr_3SgStem = wstring (str_GraphicStem).erase (xstr_InfStem.length()-3, 3);
            if (xstr_Infinitive.b_EndsWith (L"овать") || xstr_Infinitive.b_EndsWith (L"оваться"))
            {
                xstr_1SgStem += L"у";
                xstr_3SgStem += L"у";
            }
            else if (xstr_Infinitive.b_EndsWith (L"евать") || xstr_Infinitive.b_EndsWith (L"еваться"))
            {
                if (xstr_InfStem.b_EndsWithOneOf (str_ShSounds + L'ц'))
                {
                    xstr_1SgStem += L"у";
                    xstr_3SgStem += L"у";
                }
                else
                {
                    xstr_1SgStem += L"ю";
                    xstr_3SgStem += L"ю";
                }
            }
            break;
        }

        case 3:         // Without circle: (вы)тянуть, тронуть, стукнуть, гнуть, повернуть, тянуть
        {               // With circle: вымокнуть, (за)вянуть, (по)гибнуть, стыть//стынуть, достигнуть 
                        //              (достичь), свыкнуться
            xstr_1SgStem = xstr_3SgStem = wstring (str_GraphicStem).erase (xstr_InfStem.length()-1, 1);
            break;
        }

        case 4:         // строить(ся), множить, грабить, тратить, смолить, крушить, томить, щадить, хоронить, 
                        // точить, ловить, просить, молиться

        case 5:         // (вы)стоять, выглядеть, слышать, видеть, кричать, бежать, звенеть, шуметь, висеть
        {
            h_r = h_StandardAlternation (xstr_1SgStem);
            if (S_OK != h_r)
            {
                return h_r;
            }

            xstr_3SgStem = wstring (str_GraphicStem).erase (xstr_InfStem.length()-1, 1);
            break;
        }

        case 6:         // Without circle: сеять(ся), глаголать, колебать, сыпать, прятать, смеяться, ржать, 
                        //                 слать, стлать, трепать, скакать, казаться
                        // With circle: (вы)сосать(ся), жаждать, рвать(ся), ткать, лгать, брать, звать, стонать
        {
            h_r = h_StandardAlternation (xstr_1SgStem);
            if (S_OK != h_r)
            {
                return h_r;
            }            

            xstr_3SgStem = xstr_1SgStem;
            break;
        }

        case 7:         // (вы)грызть, лезть, сесть, красть(ся), ползти, везти, пасти, нести, блюсти, вести, 
                        // мести(сь), (у)честь, расти, грести
        {
            if (xstr_InfStem.b_EndsWith (L"с"))
            {
                if (str_VerbStemAlternation.empty())
                {
                    ATLASSERT(0);
                    ERROR_LOG (L"Missing stem alternation for type 7");
                    return E_FAIL;
                }

                xstr_1SgStem = xstr_InfStem.substr (0, xstr_InfStem.length()-1) + str_VerbStemAlternation;
            }
            else
            {
                ATLASSERT (xstr_InfStem.b_EndsWith (L"з"));
                xstr_1SgStem = xstr_InfStem;
            }

            xstr_3SgStem = xstr_1SgStem;
            break;
        }

        case 8:         // (вы)стричь, лечь, запрячь, беречь(ся), волочь, печь(ся), жечь, толочь, мочь
        {
            if (str_VerbStemAlternation.empty())
            {
                ATLASSERT(0);
                ERROR_LOG (L"Missing stem alternation for type 8");
                return E_FAIL;
            }

            xstr_1SgStem = xstr_InfStem += str_VerbStemAlternation;
            if (L"к" == str_VerbStemAlternation)
            {
                xstr_3SgStem = xstr_InfStem + L"ч";
            }
            else
            {
                if (L"г" == str_VerbStemAlternation)
                {
                    xstr_3SgStem = xstr_InfStem + L"ж";
                }
                else
                {
                    ATLASSERT(0);
                    ERROR_LOG (L"Bad stem finale");
                    return E_FAIL;
                }
            }
            break;
        }

        case 9:             // (вы-, с)тереть(ся), запереть, умереть, отпереть
        {
            xstr_1SgStem = xstr_3SgStem = wstring (str_GraphicStem).erase (xstr_InfStem.length()-1, 1);
            break;
        }
        case 10:            // (вы)пороть, (у)колоть(ся), молоть, бороться
        {
            xstr_1SgStem = xstr_3SgStem = wstring (str_GraphicStem).erase (xstr_InfStem.length()-1, 1);
            break;
        }
        case 11:            // шить(ся), (вы-, до)бить(ся), лить(ся), пить(ся)
        {
            xstr_1SgStem = xstr_3SgStem = wstring (str_GraphicStem).erase (xstr_InfStem.length()-1, 1);
            break;
        }
        case 12:            // выть, крыть, мыть, ныть,...
        {
            if (xstr_InfStem.b_EndsWith (L"ы"))
            {
                xstr_1SgStem = xstr_3SgStem = wstring (xstr_InfStem).replace (xstr_InfStem.length()-1, 1, L'о', 1);
            }
            break;
        }
        case 13:            // давать(ся), оставаться
        {
            xstr_1SgStem = xstr_3SgStem = xstr_InfStem;
            break;
        }
        case 14:            // мять(ся), жать(ся), сжать, отнять, занять, ...
        {
//&&&& #17
            if (str_VerbStemAlternation.empty())
            {
                    ATLASSERT(0);
                    ERROR_LOG (L"Missing stem alternation for type 14");
                    return E_FAIL;
            }

            if (L"н" != str_VerbStemAlternation && 
                L"м" != str_VerbStemAlternation && 
                L"им" != str_VerbStemAlternation)
            {
                xstr_1SgStem = xstr_3SgStem = str_VerbStemAlternation;
            }
            else
            {
                xstr_1SgStem = xstr_3SgStem = wstring (str_GraphicStem).erase (xstr_InfStem.length()-1, 1);
            }
            break;
        }
        case 15:            // одеть, достать, остаться
        case 16:            // жить(ся), плыть, слыть
        {
            xstr_1SgStem = xstr_3SgStem = xstr_InfStem;            
            break;
        }
        default:
        {
            ATLASSERT(0);
            ERROR_LOG (L"Unrecognized conjugation type.");
            return E_FAIL;
        }

    }

    return h_r;

}    //  h_BuildVerbStems()

HRESULT CT_Lexeme::h_HandleVerbYoAlternation (int i_stressPos,               // in
                                              ET_Subparadigm eo_subparadigm, // in
                                              wstring& str_lemma)            // in/out
{
    HRESULT h_r = S_OK;

    if (!b_YoAlternation && !b_OAlternation)
    {
        ERROR_LOG (L"No o/yo alternation.");
        return E_UNEXPECTED;
    }

    if (vec_SourceStressPos.size() > 1)
    {
        ATLASSERT(0);
        ERROR_LOG (L"Multiple stress positions are not allowed here.");
        return E_UNEXPECTED;
    }

    if (b_YoAlternation)
    {
        if (SUBPARADIGM_PAST_TENSE != eo_subparadigm && 
            SUBPARADIGM_PARTICIPLE_PAST_ACTIVE != eo_subparadigm &&
            SUBPARADIGM_PARTICIPLE_PAST_PASSIVE != eo_subparadigm &&
            SUBPARADIGM_ADVERBIAL_PAST != eo_subparadigm &&
            SUBPARADIGM_PARTICIPLE_PRESENT_PASSIVE != eo_subparadigm)
        {
            return S_OK;        // do nothing
        }

        unsigned int ui_ePos = xstr_InfStem.find_last_of (L"е");
        if (wstring::npos == ui_ePos)
        {
            ATLASSERT(0);
            ERROR_LOG (L"Vowel 'e' not found.");
            return E_UNEXPECTED;
        }

        if (9 == i_Type && xstr_Infinitive.b_EndsWith (L"ереть"))
        {
            wstring str_left (xstr_InfStem.substr (0, ui_ePos));
            if (str_left.empty())
            {
                ATLASSERT(0);
                ERROR_LOG (L"Inf. stem starts with the only 'e' vowel.");
                return E_UNEXPECTED;  
            }

            ui_ePos = str_left.find_last_of (L"е");
            if (wstring::npos == ui_ePos)
            {
                ATLASSERT(0);
                ERROR_LOG (L"Penultimate 'e' not found in eret' verb.");
                return E_UNEXPECTED;
            }
        }

        if (str_lemma.length() <= ui_ePos)
        {
            ATLASSERT(0);
            ERROR_LOG (L"Lemma too short.");
            return E_UNEXPECTED;
        }

        if (i_stressPos == (int)ui_ePos)
        {
            str_lemma[ui_ePos] = L'ё';
        }
    }       // yo alternation

    if (b_OAlternation)
    {
        if (i_Type != 2 || xstr_Infinitive.b_EndsWith (L"цевать"))
        {
            // TODO pass part ends in ovannyi
        }
    }

    return S_OK;

}   //  h_HandleVerbYoAlternation()

HRESULT CT_Lexeme::h_StandardAlternation (CT_ExtString& xstr_presentStem)
{
    if (str_GraphicStem.length() <= 3)
    {
        ATLASSERT(0);
        ERROR_LOG (L"Graphic stem too short.");
        return E_INVALIDARG;
    }

    xstr_presentStem = str_GraphicStem;

    wstring str_finale;
    if (xstr_presentStem.b_EndsWith (L"ск"))
    {
        str_finale = L"ск";
    }
    else
    {
        if (xstr_presentStem.b_EndsWith (L"ст"))
        {
            str_finale = L"ст";
        }
        else
        {
            str_finale = xstr_presentStem[xstr_presentStem.length()-2];
        }
    }
    map<wstring, wstring>::iterator it_alt = map_StandardAlternations.find (str_finale);
    if (map_StandardAlternations.end() != it_alt)
    {
        if (L"щ" == str_VerbStemAlternation)
        {
            ATLASSERT (L"т" == str_finale);
            xstr_presentStem = str_VerbStemAlternation;
        }
        else
        {
            xstr_presentStem = xstr_presentStem.substr (0, xstr_presentStem.length()-2) + (*it_alt).second;
        }
    }

    return S_OK;

}   //  h_StandardAlternation()
