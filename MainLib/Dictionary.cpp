// Dictionary.cpp : Implementation of CDictionary

#include "stdafx.h"
#include "Dictionary.h"
#include "Lexeme.h"
#include "StoredForms.h"

static wstring str_queryBase (L"SELECT \
headword.source, \
headword.id, \
descriptor.id, \
descriptor.graphic_stem, \
descriptor.is_variant, \
descriptor.main_symbol, \
descriptor.is_plural_of, \
descriptor.is_intransitive, \
descriptor.main_symbol_plural_of, \
descriptor.alt_main_symbol, \
descriptor.inflection_type, \
descriptor.part_of_speech, \
descriptor.comment, \
descriptor.alt_main_symbol_comment, \
descriptor.alt_inflection_comment, \
descriptor.verb_stem_alternation,  \
descriptor.section, \
descriptor.no_comparative, \
descriptor.assumed_forms, \
descriptor.yo_alternation, \
descriptor.o_alternation, \
descriptor.second_genitive, \
descriptor.has_aspect_pair, \
descriptor.aspect_pair_type, \
descriptor.aspect_pair_comment, \
descriptor.questionable_forms, \
descriptor.has_irregular_forms, \
descriptor.restricted_forms, \
descriptor.contexts, \
descriptor.trailing_comment, \
inflection.id, \
inflection.is_primary, \
inflection.inflection_type, \
inflection.accent_type1, \
inflection.accent_type2, \
inflection.short_form_restrictions, \
inflection.past_part_restrictions, \
inflection.no_short_form, \
inflection.no_past_part, \
inflection.fleeting_vowel, \
inflection.stem_augment \
FROM \
headword INNER JOIN descriptor ON descriptor.word_id = headword.id \
LEFT OUTER JOIN inflection ON descriptor.id = inflection.descriptor_id ");

HRESULT CT_Dictionary::put_DbPath (BSTR bstr_dbPath)
{
    USES_CONVERSION;

    str_DbPath = OLE2W (bstr_dbPath);

    if (pco_Db)
    {
        delete pco_Db;
    }

    pco_Db = new CT_Sqlite (str_DbPath);

    return S_OK;
}

HRESULT CT_Dictionary::GetLexeme (long l_Id, ILexeme ** pp_lexeme)
{
    HRESULT h_r = S_OK;

    wstring str_query (str_queryBase);
    str_query += L"WHERE descriptor.id = ";
    str_query += str_ToString (l_Id);
    str_query += L";";
    h_r = h_GetData (str_query);
    if (S_OK == h_r)
    {
        if (m_coll.size() < 1)
        {
            pp_lexeme = NULL;
            ERROR_LOG (L"No lexemes in dictionary object.");
            return E_FAIL;
        }

        CComVariant sp_var = m_coll[0];
        if (VT_DISPATCH == sp_var.vt)
        {
            CComQIPtr<ILexeme> sp_qiLex = sp_var.pdispVal;
            *pp_lexeme = sp_qiLex.Detach();
        }
    }

    return h_r;
}

HRESULT CT_Dictionary::GetLexemesByGraphicStem (BSTR bstr_headword)
{
    USES_CONVERSION;

    HRESULT h_r = S_OK;

    wstring str_query (str_queryBase);
    str_query += L"WHERE descriptor.graphic_stem = \"";
    str_query += OLE2W (bstr_headword);
    str_query += L"\";";
    h_r = h_GetData (str_query);

    return h_r;
}

HRESULT CT_Dictionary::GetLexemesByInitialForm (BSTR bstr_stem)
{
    USES_CONVERSION;

    HRESULT h_r = S_OK;

    wstring str_query (str_queryBase);
    str_query += L"WHERE headword.source = \"";
    str_query += OLE2W (bstr_stem);
    str_query += L"\";";
    h_r = h_GetData (str_query);

    return h_r;
}

HRESULT CT_Dictionary::LoadStoredLexemes (long l_lowBound, long l_highBound)
{
    HRESULT hr = S_OK;

    if (l_lowBound < 0)
    {
        l_lowBound = 0;
    }

    if (l_highBound < 0)
    {
        l_highBound = 0;
    }

    wstring str_query (L"SELECT DISTINCT lexeme_id, headword.source FROM test_data");
    str_query += L" INNER JOIN descriptor ON descriptor.id = test_data.lexeme_id";
    str_query += L" INNER JOIN headword ON headword.id = descriptor.word_id";

    if (l_lowBound < l_highBound)
    {
        str_query += L" WHERE lexeme_id >= ";
        str_query += str_ToString (l_lowBound);
        str_query += L" AND lexeme_id <= ";
        str_query += str_ToString (l_highBound);
    }

    str_query += L";";

    hr = h_GetStoredLexemeData (str_query);

    return S_OK;

}   //  LoadStoredLexemes (...)


//
// Helpers
//

HRESULT CT_Dictionary::h_GetData (const wstring& str_select)
{
    HRESULT h_r = S_OK;

    if (NULL == pco_Db)
    {
        ERROR_LOG (L"DB pointer is NULL.");
        return E_POINTER;
    }

    m_coll.clear();

    try
    {
        pco_Db->v_PrepareForSelect (str_select);

        while (pco_Db->b_GetRow())
        {
            CComObject<CT_Lexeme> * sp_lexeme;
            h_r = CComObject<CT_Lexeme>::CreateInstance (&sp_lexeme);
            if (S_OK != h_r)
            {
                ERROR_LOG (L"CreateInstance failed on CT_Lexeme.");
                return h_r;
            }

            sp_lexeme->v_SetDb (str_DbPath);

            pco_Db->v_GetData (0, sp_lexeme->xstr_SourceForm);
            int i_headwordId = -1;
            pco_Db->v_GetData (1, i_headwordId);
            pco_Db->v_GetData (2, sp_lexeme->i_DbKey);
            pco_Db->v_GetData (3, sp_lexeme->xstr_GraphicStem);
            pco_Db->v_GetData (4, sp_lexeme->b_IsVariant);
            pco_Db->v_GetData (5, sp_lexeme->str_MainSymbol);
            pco_Db->v_GetData (6, sp_lexeme->b_IsPluralOf);
            pco_Db->v_GetData (7, sp_lexeme->b_Transitive);
            sp_lexeme->b_Transitive = !sp_lexeme->b_Transitive;     // "нп" == !b_transitive
            pco_Db->v_GetData (8, sp_lexeme->str_MainSymbolPluralOf);
            pco_Db->v_GetData (9, sp_lexeme->str_AltMainSymbol);
            pco_Db->v_GetData (10, sp_lexeme->str_InflectionType);
            pco_Db->v_GetData (11, (int&)sp_lexeme->eo_PartOfSpeech);
            pco_Db->v_GetData (12, sp_lexeme->str_Comment);
            pco_Db->v_GetData (13, sp_lexeme->str_AltMainSymbolComment);
            pco_Db->v_GetData (14, sp_lexeme->str_AltInflectionComment);
            pco_Db->v_GetData (15, sp_lexeme->str_VerbStemAlternation);
            pco_Db->v_GetData (16, sp_lexeme->i_Section);
            pco_Db->v_GetData (17, sp_lexeme->b_NoComparative);
            pco_Db->v_GetData (18, sp_lexeme->b_AssumedForms);
            pco_Db->v_GetData (19, sp_lexeme->b_YoAlternation);
            pco_Db->v_GetData (20, sp_lexeme->b_OAlternation);
            pco_Db->v_GetData (21, sp_lexeme->b_SecondGenitive);
            pco_Db->v_GetData (22, sp_lexeme->b_HasAspectPair);
            pco_Db->v_GetData (23, sp_lexeme->i_AspectPairType);
            pco_Db->v_GetData (24, sp_lexeme->str_AspectPairComment);
            pco_Db->v_GetData (25, sp_lexeme->str_QuestionableForms);
            pco_Db->v_GetData (26, sp_lexeme->b_HasIrregularForms);
            pco_Db->v_GetData (27, sp_lexeme->str_RestrictedFroms);
            pco_Db->v_GetData (28, sp_lexeme->str_Contexts);
            pco_Db->v_GetData (29, sp_lexeme->str_TrailingComment);
            int i_inflectionId = -1;
            pco_Db->v_GetData (30, i_inflectionId);
            pco_Db->v_GetData (31, sp_lexeme->b_PrimaryInflectionGroup);
            pco_Db->v_GetData (32, sp_lexeme->i_Type);
            pco_Db->v_GetData (33, (int&)sp_lexeme->eo_AccentType1);
            pco_Db->v_GetData (34, (int&)sp_lexeme->eo_AccentType2);
            pco_Db->v_GetData (35, sp_lexeme->b_ShortFormsRestricted);
            pco_Db->v_GetData (36, sp_lexeme->b_PastParticipleRestricted);
            pco_Db->v_GetData (37, sp_lexeme->b_NoShortForms);
            pco_Db->v_GetData (38, sp_lexeme->b_NoPastParticiple);
            pco_Db->v_GetData (39, sp_lexeme->b_FleetingVowel);
            pco_Db->v_GetData (40, sp_lexeme->b_StemAugment);
            
            wstring str_stressQuery (L"SELECT stress_position, is_primary FROM stress WHERE headword_id = ");
            str_stressQuery += str_ToString (i_headwordId);
            unsigned int ui_stressHandle = pco_Db->ui_PrepareForSelect (str_stressQuery);
            while (pco_Db->b_GetRow (ui_stressHandle))
            {
                int i_pos = -1;
                bool b_primary = false;
                pco_Db->v_GetData (0, i_pos, ui_stressHandle);
                pco_Db->v_GetData (1, b_primary, ui_stressHandle);
                int i_stressedSyll = sp_lexeme->xstr_SourceForm.i_GetSyllableFromVowelPos (i_pos);
                if (b_primary)
                {
                    sp_lexeme->vec_SourceStressPos.push_back (i_stressedSyll);
                }
                else
                {
                    sp_lexeme->vec_SecondaryStressPos.push_back (i_stressedSyll);
                }
            }

            if (sp_lexeme->vec_SourceStressPos.empty())
            {
                ATLASSERT (sp_lexeme->vec_SecondaryStressPos.empty());
                wstring str_hwCommentQuery (L"SELECT comment FROM headword WHERE id = ");
                str_hwCommentQuery += str_ToString (i_headwordId);
                unsigned int ui_hwCommentHandle = pco_Db->ui_PrepareForSelect (str_hwCommentQuery);
                while (pco_Db->b_GetRow (ui_hwCommentHandle))
                {
                    wstring str_hwComment;
                    pco_Db->v_GetData (0, str_hwComment, ui_hwCommentHandle);
                    if (L"_без удар._" == str_hwComment)
                    {
                        sp_lexeme->b_IsUnstressed = true;
                    }
                }
                if (!sp_lexeme->b_IsUnstressed)
                {
                    ATLASSERT (sp_lexeme->xstr_SourceForm.i_GetNumOfSyllables() == 1);
                    sp_lexeme->vec_SourceStressPos.push_back (0);
                }
            }

            wstring str_homonymsQuery (L"SELECT homonym_number FROM homonyms WHERE headword_id = ");
            str_homonymsQuery += str_ToString (i_headwordId);
            unsigned int ui_homonymsHandle = pco_Db->ui_PrepareForSelect (str_homonymsQuery);
            while (pco_Db->b_GetRow (ui_homonymsHandle))
            {
                int i_homonym = -1;
                pco_Db->v_GetData (0, i_homonym, ui_homonymsHandle);
                sp_lexeme->vec_Homonyms.push_back (i_homonym);
            }

            wstring str_deviationQuery 
                (L"SELECT deviation_type, is_optional FROM common_deviation WHERE inflection_id = ");
            str_deviationQuery += str_ToString (i_inflectionId);
            unsigned int ui_deviationHandle = pco_Db->ui_PrepareForSelect (str_deviationQuery);
            while (pco_Db->b_GetRow (ui_deviationHandle))
            {
                int i_type = -1;
                bool b_optional = false;
                pco_Db->v_GetData (0, i_type, ui_deviationHandle);
                pco_Db->v_GetData (1, b_optional, ui_deviationHandle);
                sp_lexeme->map_CommonDeviations[i_type] = b_optional;
            }

            CComQIPtr<ILexeme> spqi_lexeme (sp_lexeme);
            if (!spqi_lexeme)
            {
                ATLASSERT(0);
                ERROR_LOG (L"QI failed on ILexeme");
                return E_NOINTERFACE;
            }
            m_coll.push_back (CComVariant (spqi_lexeme));
        }
    }
    catch (...)
    {
        wstring str_msg;
        try
        {
            wstring str_error;
            pco_Db->v_GetLastError (str_error);
            str_msg = L"DB error %d: ";
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
        
}   //  b_Getdata (...)

HRESULT CT_Dictionary::h_GetStoredLexemeData (const wstring& str_select)
{
    HRESULT h_r = S_OK;

    if (NULL == pco_Db)
    {
        ERROR_LOG (L"DB pointer is NULL.");
        return E_POINTER;
    }

    m_coll.clear();

    try
    {
        pco_Db->v_PrepareForSelect (str_select);

        while (pco_Db->b_GetRow())
        {
            CComObject<CT_StoredForms> * pco_sf;
            h_r = CComObject<CT_StoredForms>::CreateInstance (&pco_sf);
            if (S_OK != h_r)
            {
                ERROR_LOG (L"CreateInstance failed on CT_Lexeme.");
                return h_r;
            }

//            sp_lexeme->v_SetDb (str_DbPath);

            pco_Db->v_GetData (0, pco_sf->i_LexemeId);
            pco_Db->v_GetData (1, pco_sf->str_Headword);
            CComQIPtr<IStoredForms> spqi_sf (pco_sf);
            if (!spqi_sf)
            {
                ATLASSERT(0);
                ERROR_LOG (L"QI failed on IStoredForm");
                return E_NOINTERFACE;
            }

            m_coll.push_back (CComVariant (spqi_sf));
        }
    }
    catch (...)
    {
        wstring str_msg;
        try
        {
            wstring str_error;
            pco_Db->v_GetLastError (str_error);
            str_msg = L"DB error %d: ";
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
        
}   //  h_GetStoredLexemeData (...)
