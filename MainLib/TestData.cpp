#include "stdafx.h"
#include "Verifier.h"
#include "TestData.h"

HRESULT CT_TestData::put_DbPath (BSTR bstr_path)
{
    USES_CONVERSION;

    HRESULT h_r = S_OK;

    wstring str_path = OLE2W (bstr_path);
    if (str_path == str_DbPath)
    {
        return S_OK;
    }
    else
    {
        str_DbPath = str_path;
    }

    if (pco_Db)
    {
        delete pco_Db;
    }

    pco_Db = new CT_Sqlite (str_DbPath);
    if (!pco_Db)
    {
        return E_FAIL;
    }

    h_r = LoadStoredLexemes();

    return S_OK;

}   //  put_DbPath (...)

HRESULT CT_TestData::LoadStoredLexemes() //(long l_lowBound, long l_highBound)
{
    HRESULT h_r = S_OK;

    if (!pco_Db)
    {
        ERROR_LOG (L"Database not initialized.");
        return h_r;
    }

    //if (l_lowBound < 0)
    //{
    //    l_lowBound = 0;
    //}

    //if (l_highBound < 0)
    //{
    //    l_highBound = 0;
    //}

    wstring str_query (L"SELECT DISTINCT l.lexeme_hash, l.descriptor_id, h.source FROM test_data AS t");
    str_query += L" INNER JOIN lexeme_hash_to_descriptor AS l ON t.lexeme_id = l.lexeme_hash";
    str_query += L" INNER JOIN descriptor AS d on l.descriptor_id = d.id";
    str_query += L" INNER JOIN headword AS h ON h.id = d.word_id;";

    //if (l_lowBound < l_highBound)
    //{
    //    str_query += L" WHERE lexeme_id >= ";
    //    str_query += str_ToString (l_lowBound);
    //    str_query += L" AND lexeme_id <= ";
    //    str_query += str_ToString (l_highBound);
    //}

    str_query += L";";

    h_r = h_GetStoredLexemeData (str_query);

    return S_OK;

}   //  LoadStoredLexemes (...)

HRESULT CT_TestData::h_GetStoredLexemeData (const wstring& str_select)
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
            CComObject<CT_Verifier> * pco_v;
            h_r = CComObject<CT_Verifier>::CreateInstance (&pco_v);
            if (S_OK != h_r)
            {
                ERROR_LOG (L"CreateInstance failed on CT_Verifier.");
                return h_r;
            }

            pco_Db->v_GetData (0, (__int64&)pco_v->ul_LexemeHash);
//            pco_Db->v_GetData (1, pco_v->i_DescriptorId);
            pco_Db->v_GetData (2, pco_v->str_Headword);

            pco_v->str_DbPath = str_DbPath;;

            CComQIPtr<IVerifier> spqi_sf (pco_v);
            if (!spqi_sf)
            {
                ATLASSERT(0);
                ERROR_LOG (L"QI failed on IVerifier");
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
