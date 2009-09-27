// Dictionary.cpp : Implementation of CDictionary

#include "stdafx.h"
#include "Lexeme.h"
#include "Dictionary.h"

template <typename T>
wstring static str_ToString (T from)
{
    wstringstream io_;
	io_ << from;
	return io_.str();
};

HRESULT CT_Dictionary::get_Lexeme (long lId, ILexeme ** p_lexeme)
{
    HRESULT h_r = S_OK;

    *p_lexeme = NULL;

    wstring str_query (L"select from descriptor, left outer join inflection \
                        where descriptor.id = inflection.descriptor_id and descriptor.id = ");
    str_query += str_ToString (lId);

    h_r = h_GetData (str_query);

    return h_r;
}

HRESULT CT_Dictionary::get_Lexemes (BSTR bstr_stem)//, IZalCollection ** pi_lexemes)
{
    USES_CONVERSION;

    HRESULT h_r = S_OK;

    wstring str_query (L"select distinct * from descriptor left outer join inflection \
                        where descriptor.id = inflection.descriptor_id and descriptor.graphic_stem = \"");
    str_query += OLE2W (bstr_stem);
    str_query += '\"';

    h_r = h_GetData (str_query);

    return h_r;
}

//
//
//

HRESULT CT_Dictionary::h_GetData (const wstring& str_select)
{
    HRESULT h_r = S_OK;

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
    
            pco_Db->v_GetData (0, sp_lexeme->i_DbKey);         // 0-based
            pco_Db->v_GetData (2, sp_lexeme->str_GraphicStem);
            pco_Db->v_GetData (3, sp_lexeme->b_IsVariant);
            pco_Db->v_GetData (4, sp_lexeme->str_MainSymbol);
            pco_Db->v_GetData (5, sp_lexeme->b_IsPluralOf);
            pco_Db->v_GetData (6, sp_lexeme->b_Transitive);
            pco_Db->v_GetData (7, sp_lexeme->str_MainSymbolPluralOf);
            pco_Db->v_GetData (8, sp_lexeme->str_AltMainSymbol);
            pco_Db->v_GetData (9, sp_lexeme->str_InflectionType);
            pco_Db->v_GetData (10, (int&)sp_lexeme->eo_PartOfSpeech);
            pco_Db->v_GetData (11, sp_lexeme->str_Comment);
            pco_Db->v_GetData (12, sp_lexeme->str_AltMainSymbolComment);
            pco_Db->v_GetData (13, sp_lexeme->str_AltInflectionComment);
            pco_Db->v_GetData (14, sp_lexeme->str_VerbStemAlternation);
            pco_Db->v_GetData (15, sp_lexeme->i_Section);
            pco_Db->v_GetData (16, sp_lexeme->b_NoComparative);
            pco_Db->v_GetData (17, sp_lexeme->b_AssumedForms);
            pco_Db->v_GetData (18, sp_lexeme->b_YoAlternation);
            pco_Db->v_GetData (19, sp_lexeme->b_OAlternation);
            pco_Db->v_GetData (20, sp_lexeme->b_SecondGenitive);
            pco_Db->v_GetData (21, sp_lexeme->str_QuestionableForms);
            pco_Db->v_GetData (22, sp_lexeme->str_IrregularForms);
            pco_Db->v_GetData (23, sp_lexeme->str_RestrictedFroms);
            pco_Db->v_GetData (24, sp_lexeme->str_Contexts);
            pco_Db->v_GetData (25, sp_lexeme->str_TrailingComment);
            pco_Db->v_GetData (28, sp_lexeme->b_PrimaryInflectionGroup);
            pco_Db->v_GetData (29, sp_lexeme->i_Type);
            pco_Db->v_GetData (30, (int&)sp_lexeme->eo_AccentType1);
            pco_Db->v_GetData (31, (int&)sp_lexeme->eo_AccentType2);
            pco_Db->v_GetData (32, sp_lexeme->b_ShortFormsRestricted);
            pco_Db->v_GetData (33, sp_lexeme->b_PastParticipleRestricted);
            pco_Db->v_GetData (34, sp_lexeme->b_NoShortForms);
            pco_Db->v_GetData (35, sp_lexeme->b_NoPastParticiple);
            pco_Db->v_GetData (36, sp_lexeme->b_FleetingVowel);
            pco_Db->v_GetData (37, sp_lexeme->b_StemAugment);
            pco_Db->v_GetData (38, sp_lexeme->i_CommonDeviation);

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
