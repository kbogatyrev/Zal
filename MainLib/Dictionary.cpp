// Dictionary.cpp : Implementation of CDictionary

#include "stdafx.h"
#include "Lexeme.h"
#include "Dictionary.h"

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
descriptor.questionable_forms, \
descriptor.irregular_forms, \
descriptor.restricted_forms, \
descriptor.contexts, \
descriptor.trailing_comment, \
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

HRESULT CT_Dictionary::h_GetData (const wstring& str_select)
{
    HRESULT h_r = S_OK;

    if (NULL == pco_Db)
    {
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
                return h_r;
            }

            sp_lexeme->v_SetDb (str_DbPath);

            int i_headwordId = -1;
            pco_Db->v_GetData (0, sp_lexeme->str_SourceForm);
            pco_Db->v_GetData (1, i_headwordId);
            pco_Db->v_GetData (2, sp_lexeme->i_DbKey);
            pco_Db->v_GetData (3, sp_lexeme->str_GraphicStem);
            pco_Db->v_GetData (4, sp_lexeme->b_IsVariant);
            pco_Db->v_GetData (5, sp_lexeme->str_MainSymbol);
            pco_Db->v_GetData (6, sp_lexeme->b_IsPluralOf);
            pco_Db->v_GetData (7, sp_lexeme->b_Transitive);
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
            pco_Db->v_GetData (22, sp_lexeme->str_QuestionableForms);
            pco_Db->v_GetData (23, sp_lexeme->str_IrregularForms);
            pco_Db->v_GetData (24, sp_lexeme->str_RestrictedFroms);
            pco_Db->v_GetData (25, sp_lexeme->str_Contexts);
            pco_Db->v_GetData (26, sp_lexeme->str_TrailingComment);
            pco_Db->v_GetData (27, sp_lexeme->b_PrimaryInflectionGroup);
            pco_Db->v_GetData (28, sp_lexeme->i_Type);
            pco_Db->v_GetData (29, (int&)sp_lexeme->eo_AccentType1);
            pco_Db->v_GetData (30, (int&)sp_lexeme->eo_AccentType2);
            pco_Db->v_GetData (31, sp_lexeme->b_ShortFormsRestricted);
            pco_Db->v_GetData (32, sp_lexeme->b_PastParticipleRestricted);
            pco_Db->v_GetData (33, sp_lexeme->b_NoShortForms);
            pco_Db->v_GetData (34, sp_lexeme->b_NoPastParticiple);
            pco_Db->v_GetData (35, sp_lexeme->b_FleetingVowel);
            pco_Db->v_GetData (36, sp_lexeme->b_StemAugment);
            
            wstring str_stressQuery (L"SELECT stress_position, is_primary FROM stress WHERE headword_id = ");
            str_stressQuery += str_ToString (i_headwordId);
            unsigned int ui_stressHandle = pco_Db->ui_PrepareForSelect (str_stressQuery);
            while (pco_Db->b_GetRow (ui_stressHandle))
            {
                int i_pos = -1;
                bool b_primary = false;
                pco_Db->v_GetData (2, i_pos, ui_stressHandle);
                pco_Db->v_GetData (3, b_primary, ui_stressHandle);
                if (b_primary)
                {
                    sp_lexeme->vec_SourceStressPos.push_back (i_pos);
                }
                else
                {
                    sp_lexeme->vec_SecondaryStressPos.push_back (i_pos);
                }
            }

            wstring str_homonymsQuery (L"SELECT homonym_number FROM homonyms WHERE headword_id = ");
            str_stressQuery += str_ToString (i_headwordId);
            unsigned int ui_homonymsHandle = pco_Db->ui_PrepareForSelect (str_stressQuery);
            while (pco_Db->b_GetRow (ui_homonymsHandle))
            {
                int i_homonym = -1;
                pco_Db->v_GetData (2, i_homonym, ui_homonymsHandle);
                sp_lexeme->vec_Homonyms.push_back (i_homonym);
            }

            m_coll.push_back (sp_lexeme);
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
