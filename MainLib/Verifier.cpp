#include "StdAfx.h"
#include "Dictionary.h"
#include "WordForm.h"
#include "Lexeme.h"
#include "Verifier.h"

HRESULT CT_Verifier::put_DbPath (BSTR bstr_dbPath)
{
    USES_CONVERSION;

    str_DbPath = OLE2W (bstr_dbPath);

    if (str_DbPath.empty())
    {
        ATLASSERT(0);
        return E_INVALIDARG;
    }

    return S_OK;
}

HRESULT CT_Verifier::put_Headword (BSTR bstr_headWord)
{
    USES_CONVERSION;
    str_Headword = OLE2W (bstr_headWord);
    return S_OK;
}

HRESULT CT_Verifier::get_Headword (BSTR * pbstr_val)
{
    CComBSTR spbstr_headword (str_Headword.c_str());
    *pbstr_val = spbstr_headword.Detach();
    return S_OK;
}

HRESULT CT_Verifier::get_LexemeId (ULONG * pl_val)
{
    *pl_val = ul_LexemeHash;
    return S_OK;
}

HRESULT CT_Verifier::put_LexemeId (ULONG ul_lexemeId)
{
    ul_LexemeHash = ul_lexemeId;
    return S_OK;
}

HRESULT CT_Verifier::Verify (ULONG ul_lexemeId, ET_TestResult * pe_result)
{
    USES_CONVERSION;

    HRESULT h_r = S_OK;

    ul_LexemeHash = ul_lexemeId;

    //
    // Generate word forms for selected lexeme
    // 
    CComObject<CT_Dictionary> * pco_dictionary;
    h_r = CComObject<CT_Dictionary>::CreateInstance (&pco_dictionary);
    if (S_OK != h_r)
    {
        ERROR_LOG (L"CreateInstance failed on CT_Dictionary.");
        return h_r;
    }

    pco_dictionary->put_DbPath (CComBSTR (str_DbPath.c_str()));

    CComPtr<ILexeme> sp_lexeme;
//    h_r = pco_dictionary->GetLexeme (i_DescriptorId, &sp_lexeme);
//    if (S_OK != h_r || !sp_lexeme)
//    {
//        ERROR_LOG (L"GetLexeme() failed.");
//        return h_r == S_OK ? E_FAIL : h_r;
//    }

    h_r = pco_dictionary->GetLexemeByHash (ul_LexemeHash, &sp_lexeme);
    if (S_OK != h_r || !sp_lexeme)
    {
        ERROR_LOG (L"GetLexemesByHash() failed.");
        return h_r == S_OK ? E_FAIL : h_r;
    }

    CComBSTR spbstrHeadword;
    h_r = sp_lexeme->get_InitialForm (&spbstrHeadword);
    if (S_OK != h_r)
    {
        ERROR_LOG (L"ILexeme::get_InitialForm() failed.");
        return h_r;
    }

    str_Headword = OLE2W (spbstrHeadword);

    h_r = sp_lexeme->GenerateWordForms();
    if (S_OK != h_r)
    {
        ERROR_LOG (L"GenerateWordForms() failed.");
        return h_r;
    }

    h_r = h_LoadStoredForms();
    if (S_OK != h_r)
    {
        return h_r;
    }

    long l_generatedForms = 0;
    h_r = sp_lexeme->get_Count (&l_generatedForms);
    if (S_OK != h_r)
    {
        ATLASSERT(0);
        ERROR_LOG (L"get_Count() failed.");
        return h_r;
    }

    *pe_result = TEST_RESULT_OK;

    for (long l_at = 0; l_at < l_generatedForms; ++l_at)
    {
        CComVariant sp_var;
        h_r = sp_lexeme->get_Item (l_at+1, &sp_var);
        if (S_OK != h_r)
        {
            ATLASSERT(0);
            ERROR_LOG (L"get_Item() failed.");
            continue;
        }

        if (sp_var.vt != VT_UNKNOWN && sp_var.vt != VT_DISPATCH)
        {
            ATLASSERT(0);
            ERROR_LOG (L"IUnknown or IDispatch ptr expected.");
            continue;
        }

        CComQIPtr<IWordForm> spqi_generatedWf = sp_var.punkVal;
        if (!spqi_generatedWf)
        {
            ATLASSERT(0);
            ERROR_LOG (L"Unable to extract IWordForm.");
            continue;
        }

        CT_WordForm * pco_generatedWf = (CT_WordForm *)spqi_generatedWf.p;
        int i_hash = pco_generatedWf->i_GramHash();

        pair<multimap<int, IWordForm *>::iterator, 
             multimap<int, IWordForm *>::iterator> pair_range = 
                mmap_StoredForms.equal_range (i_hash);

        if (pair_range.first == pair_range.second)
        {
            ATLASSERT(0);
            ERROR_LOG (L"No stored forms loaded.");
            return E_FAIL;
        }

        eo_Result = TEST_RESULT_FAIL;
        multimap<int, IWordForm *>::iterator it_storedWf = pair_range.first;
        for (; it_storedWf != pair_range.second; ++it_storedWf)
        {
            //
            // More than one word form may be generated for a single hash; if there is
            // a generated form that matches the saved form, we mark this as a
            // success; if none of the generated forms matched the saved form,
            // we return fail.
            //
//pig
            CT_WordForm * pco_storedWf = (CT_WordForm *)(*it_storedWf).second;
            if (pco_generatedWf->str_WordForm == pco_storedWf->str_WordForm &&
                pco_generatedWf->map_Stress == pco_storedWf->map_Stress)
            {
                eo_Result = TEST_RESULT_OK;
                break;
            }
        }       // for ...

        if (TEST_RESULT_FAIL == eo_Result)
        {
            *pe_result = eo_Result;
            break;
        }

    }   //  for ...

    return h_r;

}   //  Verify (...)

HRESULT CT_Verifier::get_Result (ET_TestResult * peo_result)
{
    *peo_result = eo_Result;
    return S_OK;
}

//
//  Helpers
//
HRESULT CT_Verifier::h_LoadStoredForms()
{
    HRESULT h_r = S_OK;

    mmap_StoredForms.clear();

    if (pco_Db)
    {
        delete pco_Db;
    }

    pco_Db = new CT_Sqlite (str_DbPath);
    if (!pco_Db)
    {
        return E_FAIL;
    }

    wstring str_query (L"SELECT DISTINCT id, hash, wordform FROM test_data");
    str_query += L" WHERE lexeme_id = ";
    str_query += str_ToString (ul_LexemeHash);
    str_query += L";";

    try
    {
        pco_Db->v_PrepareForSelect (str_query);
        while (pco_Db->b_GetRow())
        {
            int i_formId = -1;
            int i_hash = -1;
            wstring str_savedWf;
            pco_Db->v_GetData (0, i_formId);
            pco_Db->v_GetData (1, i_hash);
            pco_Db->v_GetData (2, str_savedWf);

            CComObject<CT_WordForm> * pco_savedWf;
            h_r = CComObject<CT_WordForm>::CreateInstance (&pco_savedWf);
            if (S_OK != h_r)
            {
                ERROR_LOG (L"CreateInstance failed on CT_WordForm.");
                return h_r;
            }

            CT_Hasher co_hasher;
            co_hasher.v_FromHash (i_hash, *pco_savedWf);
            pco_savedWf->str_WordForm = str_savedWf;

            wstring str_stressQuery 
                (L"SELECT position, is_primary FROM test_data_stress WHERE form_id = ");
            str_stressQuery += str_ToString (i_formId);
            unsigned int ui_stressHandle = pco_Db->ui_PrepareForSelect (str_stressQuery);
            map<int, bool> map_savedStress;
            while (pco_Db->b_GetRow (ui_stressHandle))
            {
                int i_pos = -1;
                bool b_primary = false;
                pco_Db->v_GetData (0, i_pos, ui_stressHandle);
                pco_Db->v_GetData (1, b_primary, ui_stressHandle);

                pco_savedWf->map_Stress[i_pos] = b_primary ? STRESS_PRIMARY : STRESS_SECONDARY;

                mmap_StoredForms.insert (pair<int, IWordForm *>(i_hash, pco_savedWf));
            }
            pco_Db->v_Finalize();

        }   // while (...)
        pco_Db->v_Finalize();
    }
    catch (CT_Exception& co_exc)
    {
        ERROR_LOG (co_exc.str_GetDescription());
 
        return E_FAIL;
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

}   //  h_LoadStoredForms (...)
