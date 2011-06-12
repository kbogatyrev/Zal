#include "StdAfx.h"
#include "SqliteWrapper.h"
#include "SqliteComWrapper.h"

int CStatusUpdate::operator()(int iPercentDone) const
{
    pParent->StatusUpdate (iPercentDone);
    return iPercentDone; 
}

HRESULT CSqliteComWrapper::StatusUpdate (int iProgress)
{
    HRESULT hr = S_OK;

    int iNConnections = m_vec.GetSize();
    for (int iC = 0; iC < iNConnections; ++iC)
    {
        Lock();
        CComPtr<IUnknown> spUnkConnection = m_vec.GetAt (iC);
        CComQIPtr<IZalNotification> spQiConnection = spUnkConnection;
        Unlock();
        if (spQiConnection)
        {
            hr = spQiConnection->ProgressNotification (iProgress);
        }
    }

    return hr;

}   //  StatusUpdate (...)

HRESULT CSqliteComWrapper::put_DbPath (BSTR bstrDbPath)
{
    USES_CONVERSION;

    sDbPath = OLE2W (bstrDbPath);

    if (pDb)
    {
        delete pDb;
    }

    pDb = new CSqlite (sDbPath);
    if (!pDb)
    {
        return E_FAIL;
    }

    return S_OK;

}   //  put_DbPath (...)

HRESULT CSqliteComWrapper::TableExists (BSTR bstrTable, BOOL * bExists)
{
    USES_CONVERSION;

    HRESULT hr = S_OK;

    if (NULL == pDb)
    {
        ATLASSERT(0);
        ERROR_LOG (L"DB pointer is NULL.");
        return E_POINTER;
    }

    try
    {
        *bExists = pDb->bTableExists (OLE2W (bstrTable));
    }
    catch (...)
    {
        CEString sMsg;
        try
        {
            CEString sError;
            pDb->GetLastError (sError);
            sMsg = L"DB error %d: ";
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }
    
        CString csMsg;
        csMsg.Format (sMsg, pDb->iGetLastError());
        ERROR_LOG ((LPCTSTR)csMsg);
 
        return E_FAIL;
    }
    
    return S_OK;

}   //  TableExists (...)

HRESULT CSqliteComWrapper::Export (BSTR bstrPath, SAFEARRAY * sarrNames)
{
    USES_CONVERSION;

    HRESULT hr = S_OK;

    if (NULL == pDb)
    {
        ATLASSERT(0);
        ERROR_LOG (L"DB pointer is NULL.");
        return E_POINTER;
    }

//    CComVariant spVarName;
    vector<CEString> vecTableNames;
    
    long lUBound = 0;
    long lLBound = 0;
    hr = SafeArrayGetUBound (sarrNames, 1, &lUBound);
    if (S_OK != hr)
    {
        ATLASSERT(0);
        ERROR_LOG (L"Error getting safearray's lower bound.");
        return E_FAIL;
    }

    hr = SafeArrayGetLBound (sarrNames, 1, &lLBound);
    if (S_OK != hr)
    {
        ATLASSERT(0);
        ERROR_LOG (L"Error getting safearray's upper bound.");
        return E_FAIL;
    }

    unsigned int iArrSize = lUBound - lLBound + 1;
    for (unsigned int uiAt = 0; uiAt < iArrSize; ++uiAt)
    {
        CComBSTR bstrName;
        hr = SafeArrayGetElement (sarrNames, (long *)&uiAt, &bstrName);
        if (S_OK != hr)
        {
            ATLASSERT(0);
            ERROR_LOG (L"Error extracting table name from safearray.");
            return E_FAIL;
        }
        vecTableNames.push_back (OLE2W (bstrName));
    }

    try
    {
        CStatusUpdate progress;
        progress.pParent = this;
        bool bRet = pDb->bExportTables  (OLE2W (bstrPath), vecTableNames, progress);
    }
    catch (...)
    {
        CEString sMsg;
        try
        {
            CEString sError;
            pDb->GetLastError (sError);
            sMsg = L"DB error %d: ";
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }
    
        CString csMsg;
        csMsg.Format (sMsg, pDb->iGetLastError());
        ERROR_LOG ((LPCTSTR)csMsg);
 
        return E_FAIL;
    }

    return S_OK;

}   //  Export (...)

HRESULT CSqliteComWrapper::Import (BSTR bstrPath)
{
    USES_CONVERSION;

    HRESULT hr = S_OK;

    if (NULL == pDb)
    {
        ATLASSERT(0);
        ERROR_LOG (L"DB pointer is NULL.");
        return E_POINTER;
    }

    try
    {
        CStatusUpdate progress;
        progress.pParent = this;
        bool b_ret = pDb->bImportTables (OLE2W (bstrPath), progress);
        if (!b_ret)
        {
            return E_FAIL;
        }
    }
    catch (...)
    {
        CEString sMsg;
        try
        {
            CEString sError;
            pDb->GetLastError (sError);
            sMsg = L"DB error %d: ";
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }
    
        CString csMsg;
        csMsg.Format (sMsg, pDb->iGetLastError());
        ERROR_LOG ((LPCTSTR)csMsg);
 
        return E_FAIL;
    }

    return S_OK;

}   //  Import (...)
