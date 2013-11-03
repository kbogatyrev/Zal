#include "StdAfx.h"
#include "SqliteWrapper.h"
#include "SqliteComWrapper.h"

int CStatusUpdate::operator()(int iPercentDone) const
{
    pParent->StatusUpdate (iPercentDone);
    return iPercentDone; 
}

ET_ReturnCode CSqliteComWrapper::StatusUpdate (int iProgress)
{
    ET_ReturnCode hr = H_NO_ERROR;

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

ET_ReturnCode CSqliteComWrapper::put_DbPath (BSTR bstrDbPath)
{
    USES_CONVERSION;

    m_sDbPath = OLE2W (bstrDbPath);

    if (m_pDb)
    {
        delete m_pDb;
    }

    m_pDb = new CSqlite (m_sDbPath);
    if (!m_pDb)
    {
        return E_FAIL;
    }

    return H_NO_ERROR;

}   //  put_DbPath (...)

ET_ReturnCode CSqliteComWrapper::TableExists (BSTR bstrTable, BOOL * bExists)
{
    USES_CONVERSION;

    ET_ReturnCode hr = H_NO_ERROR;

    if (NULL == m_pDb)
    {
        ATLASSERT(0);
        ERROR_LOG (L"DB pointer is NULL.");
        return H_ERROR_POINTER;
    }

    try
    {
        *bExists = m_pDb->bTableExists (OLE2W (bstrTable));
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
    
    return H_NO_ERROR;

}   //  TableExists (...)

ET_ReturnCode CSqliteComWrapper::Export (BSTR bstrPath, SAFEARRAY * sarrNames)
{
    USES_CONVERSION;

    ET_ReturnCode hr = H_NO_ERROR;

    if (NULL == m_pDb)
    {
        ATLASSERT(0);
        ERROR_LOG (L"DB pointer is NULL.");
        return H_ERROR_POINTER;
    }

//    CComVariant spVarName;
    vector<CEString> vecTableNames;
    
    long lUBound = 0;
    long lLBound = 0;
    hr = SafeArrayGetUBound (sarrNames, 1, &lUBound);
    if (H_NO_ERROR != hr)
    {
        ATLASSERT(0);
        ERROR_LOG (L"Error getting safearray's lower bound.");
        return E_FAIL;
    }

    hr = SafeArrayGetLBound (sarrNames, 1, &lLBound);
    if (H_NO_ERROR != hr)
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
        if (H_NO_ERROR != hr)
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
        bool bRet = m_pDb->bExportTables  (OLE2W (bstrPath), vecTableNames, progress);
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

    return H_NO_ERROR;

}   //  Export (...)

ET_ReturnCode CSqliteComWrapper::Import (BSTR bstrPath)
{
    USES_CONVERSION;

    ET_ReturnCode hr = H_NO_ERROR;

    if (NULL == m_pDb)
    {
        ATLASSERT(0);
        ERROR_LOG (L"DB pointer is NULL.");
        return H_ERROR_POINTER;
    }

    try
    {
        CStatusUpdate progress;
        progress.pParent = this;
        bool b_ret = m_pDb->bImportTables (OLE2W (bstrPath), progress);
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

    return H_NO_ERROR;

}   //  Import (...)
