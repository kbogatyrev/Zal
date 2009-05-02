/*

*/

#include "stdafx.h"
#include "SqliteWrapper.h"
#include "ExtString.h"
#include "Lexeme.h"
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

HRESULT CT_ZalSourceReader::ConvertSourceFile (BSTR bstr_sourcePath,
                                               BSTR bstr_dbPath,
                                               int i_maxEntries)
{
    USES_CONVERSION;

    HRESULT h_r = S_OK;

    StatusUpdate (0);

    FILE * io_stream = NULL;
    errno_t i_error = _tfopen_s (&io_stream, OLE2W (bstr_sourcePath), L"r, ccs=UNICODE");
    if (0 != i_error)
    {
        CString cs_msg;
        cs_msg.Format (L"Unable to open source, error %d", i_error);
        ERROR_LOG ((LPCTSTR)cs_msg);
        return E_FAIL;
    }

    CT_Sqlite co_sqlite;
    int i_r = co_sqlite.i_Open (OLE2W (bstr_dbPath));
    if (SQLITE_OK != i_r)
    {
        CString cs_msg;
        cs_msg.Format (L"Unable to open database, error %d", i_r);
        ERROR_LOG ((LPCTSTR)cs_msg);
        return MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, 0xF000 + i_r);
    }

 //   i_r = co_sqlite.i_Exec (L"CREATE TABLE Test (Entry TEXT)");
 //   if (SQLITE_OK != i_r)
 //   {
 //       CString cs_msg;
 //       cs_msg.Format (L"Unable to create DB table, error %d", i_r);
 //       ERROR_LOG ((LPCTSTR)cs_msg);
 //       return MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, 0xF000 + i_r);
 //   }

    long l_fileLength = _filelength (_fileno (io_stream))/sizeof (TCHAR);
    long l_charsRead = 0;
    int i_percentDone = 0;
    int i_entriesRead = 0;

    TCHAR sz_lineBuf[10000];
    BOOL ui_abort = false;
    for (; (!feof (io_stream) && !ui_abort); ++i_entriesRead)
    {
        if (i_entriesRead == i_maxEntries)
        {
            break;
        }

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

        CT_Lexeme co_entry (str_line);
        bool b_parseResult = co_entry.b_Parse();
        if (!b_parseResult)
        {
            wstring str_msg(L"*** Parsing error: \n");
            str_msg += str_line;
            ERROR_LOG (str_msg.c_str());
        }

        if (i_maxEntries > 0)
        {
            i_percentDone = (int) (((double)i_entriesRead/(double)i_maxEntries) * 100);
        }
        else
        {
            int i_pd = (int) (((double)l_charsRead/(double)l_fileLength) * 100);
            if (i_pd > i_percentDone)
            {
                i_percentDone = i_pd;
            }
        }
        StatusUpdate (i_percentDone);

        if (b_parseResult)
        {
            wstring str_query (L"INSERT INTO Test VALUES('");
            str_query += sz_lineBuf;
            str_query += L"')";
            co_sqlite.i_Exec (str_query);
        }
        else
        {
            wstring str_query (L"INSERT INTO Errors VALUES('");
            str_query += sz_lineBuf;
            str_query += L"')";
            co_sqlite.i_Exec (str_query);
        }

        StatusCheck (ui_abort);
    }

    if (!ui_abort)
    {
        StatusUpdate (100);
    }

    return S_OK;

}   //  ConvertSourceFile (...)

HRESULT CT_ZalSourceReader::PreprocessSourceFile (BSTR bstr_sourcePath,
                                                  BSTR bstr_outPath,
                                                  BSTR bstr_errPath)
{
    USES_CONVERSION;

    HRESULT h_r = S_OK;

    StatusUpdate (0);

    FILE * io_stream = NULL;
    errno_t i_error = _tfopen_s (&io_stream, OLE2W (bstr_sourcePath), L"r, ccs=UNICODE");
    if (0 != i_error)
    {
        CString cs_msg;
        cs_msg.Format (L"Unable to open source, error %d", i_error);
        ERROR_LOG ((LPCTSTR)cs_msg);
        return E_FAIL;
    }


    long l_fileLength = _filelength (_fileno (io_stream))/sizeof (TCHAR);
    long l_charsRead = 0;
    int i_percentDone = 0;
    int i_entriesRead = 0;

    TCHAR sz_lineBuf[10000];
    BOOL ui_abort = false;
    for (; (!feof (io_stream) && !ui_abort); ++i_entriesRead)
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


        StatusUpdate (i_percentDone);


        StatusCheck (ui_abort);
    }

    if (!ui_abort)
    {
        StatusUpdate (100);
    }

    return S_OK;

}   //  PreprocessSourceFile (...)

HRESULT CT_ZalSourceReader::StatusUpdate (int i_progress)
{
    HRESULT h_r = S_OK;

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
