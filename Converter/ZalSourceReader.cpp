/*

*/

#include "stdafx.h"
#include "DbHandler_i.h"
#include "ZalSourceReader.h"

HRESULT CT_ZalSourceReader::FinalConstruct()
{
    CoInitialize (NULL);

    return S_OK;
}

void CT_ZalSourceReader::FinalRelease()
{
    // TODO: check for and report errors
    CoUninitialize();
}

HRESULT CT_ZalSourceReader::ProcessSourceFile (BSTR bstr_path)
{
    StatusUpdate (0);

    ERROR_LOG (L"GOGO ERRor");

    CString cs_path (bstr_path);
    FILE * io_stream = NULL;
    errno_t i_error = _tfopen_s (&io_stream, cs_path.GetBuffer(), _T("r, ccs=UNICODE"));
    if (0 != i_error)
    {
        return E_FAIL;
    }

    CComPtr<ISqliteWrapper> co_sqlite;
    HRESULT h_r = co_sqlite.CoCreateInstance (__uuidof(SqliteWrapper));
    if (S_OK != h_r)
    {
        return h_r;
    }

    co_sqlite->Open (L"test.db");
	co_sqlite->Exec (L"CREATE TABLE Test (Entry TEXT)");

    long l_fileLength = _filelength (_fileno (io_stream))/sizeof (TCHAR);
    long l_charsRead = 0;
    int i_percentDone = 0;

    TCHAR sz_lineBuf[10000];
    BOOL ui_abort = false;
    while (!feof (io_stream) && !ui_abort)
    {
        TCHAR * sz_ret = _fgetts (sz_lineBuf, 10000, io_stream);
        if (NULL == sz_ret)
        {
            int i_error = ferror (io_stream);
            if (0 != i_error)
            {
                break;
            }
        }

        wstring str_line (sz_lineBuf);
        l_charsRead += str_line.length();
        int i_pd = (int) (((double)l_charsRead/(double)l_fileLength) * 100);
        if (i_pd > i_percentDone)
        {
            i_percentDone = i_pd;
            StatusUpdate (i_percentDone);
        }

        CString cstr_query (L"INSERT INTO Test VALUES('");
        cstr_query += CString (sz_lineBuf);
        cstr_query += L"')";
        CComBSTR co_bstrQuery (cstr_query);
        co_sqlite->Exec (co_bstrQuery);

        StatusCheck (ui_abort);
    }

    if (!ui_abort)
    {
        StatusUpdate (100);
    }

    return S_OK;
}

HRESULT CT_ZalSourceReader::StatusUpdate (int i_progress)
{
    HRESULT h_r = S_OK;

//    IConnectionPointImpl<CT_ZalSourceReader,
//                         &IID_IZalNotification,
//                         CComDynamicUnkArray> * p_this = this;

    int i_nConnections = m_vec.GetSize();
    for (int i_c = 0; i_c < i_nConnections; ++i_c)
    {
        Lock();
        CComPtr<IUnknown> sp_unkConnection = m_vec.GetAt (i_c);
        CComQIPtr<IZalNotification> sp_qiConnection = sp_unkConnection;
        Unlock();
        if (sp_qiConnection)
        {
            h_r = sp_qiConnection->ProgressNotification (i_progress);
        }
    }

    return h_r;
}

HRESULT CT_ZalSourceReader::StatusCheck (BOOL& b_cancel)
{
    HRESULT h_r = S_OK;

//    IConnectionPointImpl<CT_ZalSourceReader,
//                         &IID_IZalNotification,
//                         CComDynamicUnkArray> * p_this = this;

    int i_nConnections = m_vec.GetSize();
    for (int i_c = 0; i_c < i_nConnections; ++i_c)
    {
        Lock();
        CComPtr<IUnknown> sp_unkConnection = m_vec.GetAt (i_c);
        CComQIPtr<IZalNotification> sp_qiConnection = sp_unkConnection;
        Unlock();
        if (sp_qiConnection)
        {
            h_r = sp_qiConnection->StatusCheck (&b_cancel);
        }
    }

    return h_r;
}
