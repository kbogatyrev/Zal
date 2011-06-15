#include "StdAfx.h"
#include "Dictionary.h"
#include "WordForm.h"
#include "Lexeme.h"
#include "Verifier.h"

HRESULT CVerifier::put_DbPath (BSTR bstrDbPath)
{
    USES_CONVERSION;

    m_sDbPath = OLE2W (bstrDbPath);

    if (m_sDbPath.bIsEmpty())
    {
        ATLASSERT(0);
        return E_INVALIDARG;
    }

    return S_OK;
}

HRESULT CVerifier::put_Headword (BSTR bstrHeadWord)
{
    USES_CONVERSION;
    m_sHeadword = OLE2W (bstrHeadWord);
    return S_OK;
}

HRESULT CVerifier::get_Headword (BSTR * pbstrVal)
{
    CComBSTR spbstrHeadword (m_sHeadword);
    *pbstrVal = spbstrHeadword.Detach();
    return S_OK;
}

HRESULT CVerifier::get_LexemeId (ULONG * plVal)
{
    *plVal = m_ulLexemeHash;
    return S_OK;
}

HRESULT CVerifier::put_LexemeId (ULONG ulLexemeId)
{
    m_ulLexemeHash = ulLexemeId;
    return S_OK;
}

HRESULT CVerifier::Verify (ULONG ulLexemeId, ET_TestResult * peResult)
{
    USES_CONVERSION;

    HRESULT hr = S_OK;

    m_ulLexemeHash = ulLexemeId;

    //
    // Generate word forms for selected lexeme
    // 
    CComObject<CDictionary> * pDictionary;
    hr = CComObject<CDictionary>::CreateInstance (&pDictionary);
    if (S_OK != hr)
    {
        ERROR_LOG (L"CreateInstance failed on CDictionary.");
        return hr;
    }

    pDictionary->put_DbPath (CComBSTR (m_sDbPath));

    CComPtr<ILexeme> spLexeme;
//    hr = pDictionary->GetLexeme (i_DescriptorId, &spLexeme);
//    if (S_OK != hr || !spLexeme)
//    {
//        ERROR_LOG (L"GetLexeme() failed.");
//        return hr == S_OK ? E_FAIL : hr;
//    }

    hr = pDictionary->GetLexemeByHash (m_ulLexemeHash, &spLexeme);
    if (S_OK != hr || !spLexeme)
    {
        ERROR_LOG (L"GetLexemesByHash() failed.");
        return hr == S_OK ? E_FAIL : hr;
    }

    CComBSTR spbstrHeadword;
    hr = spLexeme->get_InitialForm (&spbstrHeadword);
    if (S_OK != hr)
    {
        ERROR_LOG (L"ILexeme::get_InitialForm() failed.");
        return hr;
    }

    m_sHeadword = OLE2W (spbstrHeadword);

    hr = spLexeme->GenerateWordForms();
    if (S_OK != hr)
    {
        ERROR_LOG (L"GenerateWordForms() failed.");
        return hr;
    }

    hr = hLoadStoredForms();
    if (S_OK != hr)
    {
        return hr;
    }

    long lGeneratedForms = 0;
    hr = spLexeme->get_Count (&lGeneratedForms);
    if (S_OK != hr)
    {
        ATLASSERT(0);
        ERROR_LOG (L"get_Count() failed.");
        return hr;
    }

    *peResult = TEST_RESULT_OK;

    for (long lAt = 0; lAt < lGeneratedForms; ++lAt)
    {
        CComVariant spVar;
        hr = spLexeme->get_Item (lAt+1, &spVar);
        if (S_OK != hr)
        {
            ATLASSERT(0);
            ERROR_LOG (L"get_Item() failed.");
            continue;
        }

        if (spVar.vt != VT_UNKNOWN && spVar.vt != VT_DISPATCH)
        {
            ATLASSERT(0);
            ERROR_LOG (L"IUnknown or IDispatch ptr expected.");
            continue;
        }

        CComQIPtr<IWordForm> spqiGeneratedWf = spVar.punkVal;
        if (!spqiGeneratedWf)
        {
            ATLASSERT(0);
            ERROR_LOG (L"Unable to extract IWordForm.");
            continue;
        }

        CWordForm * pGeneratedWf = (CWordForm *)spqiGeneratedWf.p;
        int iHash = pGeneratedWf->iGramHash();

        pair<multimap<int, IWordForm *>::iterator, 
             multimap<int, IWordForm *>::iterator> pairRange = 
                m_mmapStoredForms.equal_range (iHash);

        if (pairRange.first == pairRange.second)
        {
            ATLASSERT(0);
            ERROR_LOG (L"No stored forms loaded.");
            return E_FAIL;
        }

        m_eResult = TEST_RESULT_FAIL;
        multimap<int, IWordForm *>::iterator itStoredWf = pairRange.first;
        for (; itStoredWf != pairRange.second; ++itStoredWf)
        {
            //
            // More than one word form may be generated for a single hash; if there is
            // a generated form that matches the saved form, we mark this as a
            // success; if none of the generated forms matched the saved form,
            // we return fail.
            //
//pig
            CWordForm * pStoredWf = (CWordForm *)(*itStoredWf).second;
            if (pGeneratedWf->m_sWordForm == pStoredWf->m_sWordForm &&
                pGeneratedWf->m_mapStress == pStoredWf->m_mapStress)
            {
                m_eResult = TEST_RESULT_OK;
                break;
            }
        }       // for ...

        if (TEST_RESULT_FAIL == m_eResult)
        {
            *peResult = m_eResult;
            break;
        }

    }   //  for ...

    return hr;

}   //  Verify (...)

HRESULT CVerifier::get_Result (ET_TestResult * peoResult)
{
    *peoResult = m_eResult;
    return S_OK;
}

//
//  Helpers
//
HRESULT CVerifier::hLoadStoredForms()
{
    HRESULT hr = S_OK;

    m_mmapStoredForms.clear();

    if (m_pDb)
    {
        delete m_pDb;
    }

    m_pDb = new CSqlite (m_sDbPath);
    if (!m_pDb)
    {
        return E_FAIL;
    }

    CEString sQuery (L"SELECT DISTINCT id, hash, wordform FROM test_data");
    sQuery += L" WHERE lexeme_id = ";
    sQuery += CEString::sToString (m_ulLexemeHash);
    sQuery += L";";

    try
    {
        m_pDb->PrepareForSelect (sQuery);
        while (m_pDb->bGetRow())
        {
            int iFormId = -1;
            int iHash = -1;
            CEString sSavedWf;
            m_pDb->GetData (0, iFormId);
            m_pDb->GetData (1, iHash);
            m_pDb->GetData (2, sSavedWf);

            CComObject<CWordForm> * pSavedWf;
            hr = CComObject<CWordForm>::CreateInstance (&pSavedWf);
            if (S_OK != hr)
            {
                ERROR_LOG (L"CreateInstance failed on CWordForm.");
                return hr;
            }

            CHasher hasher;
            hasher.FromHash (iHash, *pSavedWf);
            pSavedWf->m_sWordForm = sSavedWf;

            CEString sStressQuery 
                (L"SELECT position, is_primary FROM test_data_stress WHERE form_id = ");
            sStressQuery += CEString::sToString (iFormId);
            unsigned int uiStressHandle = m_pDb->uiPrepareForSelect (sStressQuery);
            map<int, bool> mapSavedStress;
            while (m_pDb->bGetRow (uiStressHandle))
            {
                int iPos = -1;
                bool bPrimary = false;
                m_pDb->GetData (0, iPos, uiStressHandle);
                m_pDb->GetData (1, bPrimary, uiStressHandle);

                pSavedWf->m_mapStress[iPos] = bPrimary ? STRESS_PRIMARY : STRESS_SECONDARY;

                m_mmapStoredForms.insert (pair<int, IWordForm *>(iHash, pSavedWf));
            }
            m_pDb->Finalize();

        }   // while (...)
        
        m_pDb->Finalize();
    }
    catch (CException& exc)
    {
        ERROR_LOG (exc.szGetDescription());
 
        return E_FAIL;
    }
    catch (...)
    {
        CEString sMsg;
        try
        {
            CEString sError;
            m_pDb->GetLastError (sError);
            sMsg = L"DB error %d: ";
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

}   //  hLoadStoredForms (...)
