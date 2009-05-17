/*

*/

#include "stdafx.h"
#include "SqliteWrapper.h"
#include "ExtString.h"
#include "ZalEntryParser.h"
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

        CT_ZalEntryParser co_entry (str_line);
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

    wstring arr_strMainSymbol[] = { L"м", L"мо", L"ж", L"жо", L"с", L"со", L"мо-жо", L"мн.",
     L"мн. неод.", L"мн. одуш.", L"мн. _от_", L"п", L"мс", L"мс-п", L"числ.",
     L"числ.-п", L"св", L"нсв", L"св-нсв", L"межд.", L"н", L"предик.", L"предл.",
     L"част.", L"союз", L"вводн.", L"сравн." };

    set<wstring> set_mainSymbols;
    for (int i_ = 0; i_ < sizeof (arr_strMainSymbol)/sizeof (wstring); ++i_)
    {
        set_mainSymbols.insert (arr_strMainSymbol[i_]);
    }

    StatusUpdate (0);

    FILE * io_inStream = NULL;
    errno_t i_error = _tfopen_s (&io_inStream, OLE2W (bstr_sourcePath), L"r, ccs=UNICODE");
    if (0 != i_error)
    {
        CString cs_msg;
        cs_msg.Format (L"Unable to open source, error %d", i_error);
        ERROR_LOG ((LPCTSTR)cs_msg);
        return E_FAIL;
    }

    FILE * io_outStream = NULL;
    i_error = _tfopen_s (&io_outStream, OLE2W (bstr_outPath), L"w, ccs=UNICODE");
    if (0 != i_error)
    {
        CString cs_msg;
        cs_msg.Format (L"Unable to open output file, error %d", i_error);
        ERROR_LOG ((LPCTSTR)cs_msg);
        return E_FAIL;
    }

    FILE * io_errStream = NULL;
    i_error = _tfopen_s (&io_errStream, OLE2W (bstr_errPath), L"w, ccs=UNICODE");
    if (0 != i_error)
    {
        CString cs_msg;
        cs_msg.Format (L"Unable to open error file, error %d", i_error);
        ERROR_LOG ((LPCTSTR)cs_msg);
        return E_FAIL;
    }

    long l_fileLength = _filelength (_fileno (io_inStream))/sizeof (TCHAR);
    long l_charsRead = 0;
    int i_percentDone = 0;
    int i_entriesRead = 0;

    TCHAR sz_lineBuf[10000];
    BOOL ui_abort = false;
    for (; (!feof (io_inStream) && !ui_abort); ++i_entriesRead)
    {
        TCHAR * sz_ret = _fgetts (sz_lineBuf, 10000, io_inStream);
        if (NULL == sz_ret)
        {
            int i_error = ferror (io_inStream);
            if (0 != i_error)
            {
                break;
            }
        }

        CT_ExtString xstr_line (sz_lineBuf, L";");
        CT_ExtString xstr_newLine (xstr_line);
        l_charsRead += xstr_line.length();
        if (xstr_line.i_NFields() < 2)
        {
            i_error = _fputts (xstr_line.c_str(), io_outStream);
            continue;
        }
        
        bool b_suspiciousSemicolon = false;
        for (int i_at = 0; 
             !b_suspiciousSemicolon && (i_at < xstr_line.i_NFields()); 
             ++i_at)
        {
            CT_ExtString xstr_left (xstr_line.str_GetField (i_at), L"()");
            if (xstr_left.i_NFields() > 1)
            {
                wstring str_parenth = xstr_left.str_GetToken (xstr_left.i_NTokens()-2);
                if (L"(" == str_parenth)
                {
                    continue;
                }
            }
            if (i_at < xstr_line.i_NFields()-1)
            {
                unsigned int ui_at = xstr_left.find_last_of (L"%@");
                if (wstring::npos != ui_at)
                {
                    b_suspiciousSemicolon = true;
                    continue;
                }
                CT_ExtString xstr_next (xstr_line.str_GetField (i_at+1), L" \n");
                xstr_next.v_Trim();
                if ((_istdigit (xstr_next[0])) || (set_mainSymbols.count (xstr_next.str_GetField (0)) > 0))
                {
                    CT_ExtString::ST_Token st_t;
                    bool b_ = xstr_line.b_GetField (i_at+1, CT_ExtString::ec_TokenText, st_t);
                    if (!b_)
                    {
                        ERROR_LOG (L"GetField() returned error");
                        return E_FAIL;
                    }
                    int i_semicolonPos = st_t.i_Offset - 1;
                    if (L';' != xstr_line[i_semicolonPos])
                    {
                        ERROR_LOG (L"Semicolon expected");
                        return E_FAIL;
                    }
                    if (st_t.i_Offset < 2)
                    {
                        ERROR_LOG (L"Bad offset");
                        return E_FAIL;
                    }
                    xstr_newLine[i_semicolonPos] = L'|';
                }
                else
                {
                    b_suspiciousSemicolon = true;
                }
            }
        }   //  for (int i_at = 0; i_at < xstr_line.i_NFields(); ++i_at)

        if (b_suspiciousSemicolon)
        {
            i_error = _fputts (xstr_line.c_str(), io_errStream);
        }
        else
        {
            i_error = _fputts (xstr_newLine.c_str(), io_outStream);
        }
    
        int i_pd = (int) (((double)l_charsRead/(double)l_fileLength) * 100);
        if (i_pd > i_percentDone)
        {
            i_percentDone = i_pd;
        }

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
