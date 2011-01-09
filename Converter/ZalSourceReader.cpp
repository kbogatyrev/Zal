/*

*/

#include "stdafx.h"
#include "SqliteWrapper.h"
#include "ExtString.h"
#include "ZalEntryParser.h"
#include "EndingsParser.h"
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
                                               BSTR bstr_unprocessedPath,
                                               int i_maxEntries,
                                               BOOL b_endings)
{
    USES_CONVERSION;

    HRESULT h_r = S_OK;

    StatusUpdate (0);

    b_Show (L"### Conversion started...");

    FILE * io_inStream = NULL;
    errno_t i_error = _tfopen_s (&io_inStream, OLE2W (bstr_sourcePath), L"r, ccs=UNICODE");
    if (0 != i_error)
    {
        CString cs_msg;
        cs_msg.Format (L"Unable to open source, error %d", i_error);
        ERROR_LOG ((LPCTSTR)cs_msg);
        return E_FAIL;
    }

    CT_Sqlite co_sqlite;
//    CT_Sqlite * pco_sqlite = CT_Sqlite::pco_CreateInstance();

    try
    {
        co_sqlite.v_Open (OLE2W (bstr_dbPath));
    }
    catch (CT_Exception& co_exc)
    {
        wstring str_msg (co_exc.str_GetDescription());
        wstring str_error;
        try
        {
            co_sqlite.v_GetLastError (str_error);
            str_msg += wstring (L", error %d: ");
            str_msg += str_error;
        }
        catch (...)
        {
            str_msg = L"Apparent DB error ";
        }
    
        CString cs_msg;
        cs_msg.Format (str_msg.c_str(), co_sqlite.i_GetLastError());
        ERROR_LOG ((LPCTSTR)cs_msg);

        return MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, 0xF000 + co_sqlite.i_GetLastError());
    }

    FILE * io_outStream = NULL;
    i_error = _tfopen_s (&io_outStream, OLE2W (bstr_unprocessedPath), L"w, ccs=UNICODE");
    if (0 != i_error)
    {
        CString cs_msg;
        cs_msg.Format (L"Unable to open unprocessed entries file, error %d", i_error);
        ERROR_LOG ((LPCTSTR)cs_msg);
        return E_FAIL;
    }

    long l_fileLength = _filelength (_fileno (io_inStream))/sizeof (TCHAR);
    long l_charsRead = 0;
    int i_percentDone = 0;
    int i_entriesRead = 0;

    TCHAR sz_lineBuf[10000];
    BOOL ui_abort = false;

    co_sqlite.v_BeginTransaction();

    for (; (!feof (io_inStream) && !ui_abort); ++i_entriesRead)
    {
        if (i_entriesRead == i_maxEntries)
        {
            break;
        }

        TCHAR * sz_ret = _fgetts (sz_lineBuf, 10000, io_inStream);
        if (NULL == sz_ret)
        {
            i_error = ferror (io_inStream);
            if (0 != i_error)
            {
                break;
            }
            else
            {
                continue;
            }
        }

        wstring str_line (sz_lineBuf);
        l_charsRead += str_line.length();

        if (L';' == str_line[0])
        {
            continue;
        }

        CT_ExtString xstr_line (str_line, L" \n");
        if (xstr_line.i_GetNumOfFields() < 1)
        {
            continue;
        }

        xstr_line.v_Trim();

        if ((i_entriesRead % 10000) == 0 && i_entriesRead > 0)
        {
            co_sqlite.v_CommitTransaction();
            co_sqlite.v_BeginTransaction();
        }

//        if ((i_entriesRead % 10) == 0)
//        {
            CT_ExtString xstr_word (xstr_line.c_str(), L" ");
            CString cstr_word (xstr_word.str_GetField (0).c_str());
            ShowCurrentWord (cstr_word.AllocSysString());
//        }

        if (b_endings)
        {
            CT_EndingsParser co_entry (xstr_line.c_str(), &co_sqlite);
            bool b_ParseResult = co_entry.b_Parse();
            int i_pd = (int) (((double)l_charsRead/(double)l_fileLength) * 100);
            if (i_pd > i_percentDone)
            {
                i_percentDone = min (i_pd, 100);
                StatusUpdate (i_percentDone);
            }
        }
        else
        {
            CT_ZalEntryParser co_entry (xstr_line.c_str(), &co_sqlite);
            bool b_parseResult = co_entry.b_Parse();
            if (!b_parseResult)
            {
                wstring str_msg(L"*** Parsing error: \n");
                str_msg += str_line;
                ERROR_LOG (str_msg.c_str());
                i_error = _fputts (str_line.c_str(), io_outStream);
                if (0 != i_error)
                {
                    ERROR_LOG (L"*** Error writing unprocessed entry. \n");
                }
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
                    i_percentDone = min (i_pd, 100);
                    StatusUpdate (i_percentDone);
                }
            }
        }

        StatusCheck (ui_abort);

    }   // for (; (!feof (io_inStream) && !ui_abort); ++i_entriesRead)

    co_sqlite.v_CommitTransaction();

    if (!ui_abort)
    {
        StatusUpdate (100);
    }

    return S_OK;

}   //  ConvertSourceFile (...)

HRESULT CT_ZalSourceReader::PreprocessSourceFile (BSTR bstr_sourcePath, BSTR bstr_outputDir)
{
    USES_CONVERSION;

    HRESULT h_r = S_OK;

    wstring arr_strMainSymbol[] = { L"м", L"мо", L"ж", L"жо", L"с", L"со", L"мо-жо", L"мн.",
     L"мн. неод.", L"мн. одуш.", L"мн. _от_", L"п", L"мс", L"мс-п", L"числ.",
     L"числ.-п", L"св", L"нсв", L"св-нсв", L"межд.", L"н", L"предик.", L"предл.",
     L"част.", L"союз", L"вводн.", L"сравн." };

    for (int i_ = 0; i_ < sizeof (arr_strMainSymbol)/sizeof (wstring); ++i_)
    {
        set_MainSymbols.insert (arr_strMainSymbol[i_]);
    }

    wstring str_outputDir (OLE2W (bstr_outputDir));

    StatusUpdate (0);

    b_Show (L"### Pre-processing started...");

    FILE * io_inStream = NULL;
    errno_t i_error = _tfopen_s (&io_inStream, OLE2W (bstr_sourcePath), L"r, ccs=UNICODE");
    if (0 != i_error)
    {
        CString cs_msg;
        cs_msg.Format (L"Unable to open file %s, error %d", CString (bstr_sourcePath), i_error);
        ERROR_LOG ((LPCTSTR)cs_msg);
        return E_FAIL;
    }

    FILE * io_outStream = NULL;
    wstring str_path = str_outputDir + L"\\out.txt";
    i_error = _tfopen_s (&io_outStream, str_path.c_str(), L"w, ccs=UNICODE");
    if (0 != i_error)
    {
        CString cs_msg;
        cs_msg.Format (L"Unable to open %s, error %d", str_path.c_str(), i_error);
        ERROR_LOG ((LPCTSTR)cs_msg);
        return E_FAIL;
    }

    FILE * io_semicolons = NULL;
    str_path = str_outputDir + L"\\semicolons.txt";
    i_error = _tfopen_s (&io_semicolons, str_path.c_str(), L"w, ccs=UNICODE");
    if (0 != i_error)
    {
        CString cs_msg;
        cs_msg.Format (L"Unable to open file %s, error %d", str_path, i_error);
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

        wstring str_line (sz_lineBuf);
        l_charsRead += str_line.length();

        if (L';' == str_line[0])
        {
            continue;
        }

        bool b_suspiciousEntry = false;
        bool b_ret = b_CheckSemicolons (str_line, b_suspiciousEntry);
        if (b_suspiciousEntry)
        {
            i_error = _fputts (str_line.c_str(), io_semicolons);
            continue;
        }
    
        i_error = _fputts (str_line.c_str(), io_outStream);
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

HRESULT CT_ZalSourceReader::SearchSourceFile (BSTR bstr_sourcePath, 
                                              BSTR bstr_searchString, 
                                              BOOL b_regexSearch)
{
    USES_CONVERSION;

    StatusUpdate (0);

    b_Show (L"### Search started...");

    FILE * io_inStream = NULL;
    errno_t i_error = _tfopen_s (&io_inStream, OLE2W (bstr_sourcePath), L"r, ccs=UNICODE");
    if (0 != i_error)
    {
        CString cs_msg;
        cs_msg.Format (L"Unable to open file %s, error %d", CString (bstr_sourcePath), i_error);
        ERROR_LOG ((LPCTSTR)cs_msg);
        return E_FAIL;
    }

    wchar_t sz_dirPath[512];
    DWORD dw_ret = GetTempPath (512, sz_dirPath);
    if ((dw_ret > 512) || (0 == dw_ret))
    {
        CString cs_msg;
        cs_msg.Format (L"Error retrieving temp. directory path, error %d", GetLastError());
        ERROR_LOG ((LPCTSTR)cs_msg);
        return E_FAIL;
    }

    wchar_t sz_path[512];
    UINT ui_ret = GetTempFileName (sz_dirPath, // dir path
                                   L"Zal_",    // temp file name prefix 
                                   0,          // create unique name 
                                   sz_path);   // buffer for name 
    if (0 == ui_ret)
    {
        CString cs_msg;
        cs_msg.Format (L"Unable to create temporary file name in %s, error %d", sz_dirPath, ui_ret);
        ERROR_LOG ((LPCTSTR)cs_msg);
        return E_FAIL;
    }

    FILE * io_outStream = NULL;
    i_error = _tfopen_s (&io_outStream, sz_path, L"w, ccs=UNICODE");
    if (0 != i_error)
    {
        CString cs_msg;
        cs_msg.Format (L"Unable to open %s, error %d", sz_path, i_error);
        ERROR_LOG ((LPCTSTR)cs_msg);
        return E_FAIL;
    }

    str_TempFile = sz_path;

    long l_fileLength = _filelength (_fileno (io_inStream))/sizeof (TCHAR);
    long l_charsRead = 0;
    int i_percentDone = 0;
    int i_entriesRead = 0;

    wstring str_header (L";   ");
    if (b_regexSearch)
    {
        str_header += L"*** Regex expression = ";
    }
    else
    {
        str_header += L"*** Match string = ";
    }
    str_header += OLE2W (bstr_searchString);
    str_header += L"\n\n";
    i_error = _fputts (str_header.c_str(), io_outStream);

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

        wstring str_line (sz_lineBuf);
        l_charsRead += str_line.length();

        bool b_match = false;
        wstring str_search = OLE2W (bstr_searchString);
        if (b_regexSearch)
        {
            CT_ExtString xstr_line (str_line, L" \n");
            if (xstr_line.i_GetNumOfFields() < 1)
            {
                CString cs_msg (L"Empty line: ");
                cs_msg += str_line.c_str();
                ERROR_LOG ((LPCTSTR)cs_msg);
                continue;
            }

            xstr_line.v_Trim();

            b_match = xstr_line.b_RegexMatch (str_search);
        }
        else
        {
            int i_match = str_line.find (str_search);
            if (wstring::npos != i_match)
            {
                b_match = true;
            }
        }

        if (b_match)
        {
            i_error = _fputts (str_line.c_str(), io_outStream);
            str_line = wstring (L"*** NO_LOG *** ") + str_line; // tell UI that we don't want it to be logged
            bool b_ = b_Show (str_line);
        }

        int i_pd = (int) (((double)l_charsRead/(double)l_fileLength) * 100);
        if (i_pd > i_percentDone)
        {
            i_percentDone = i_pd;
        }

        StatusUpdate (i_percentDone);
        StatusCheck (ui_abort);
        if ((i_entriesRead % 10) == 0)
        {
            CT_ExtString xstr_word (str_line, L" ");
            CString cstr_word (xstr_word.str_GetField (0).c_str());
            ShowCurrentWord (cstr_word.AllocSysString());
        }
    }

    if (!ui_abort)
    {
        StatusUpdate (100);
    }

    fclose (io_outStream);

    return S_OK;

}   // SearchSourceFile (...)

HRESULT CT_ZalSourceReader::SaveOutput (BSTR bstr_path)
{
    BOOL ui_ret = MoveFileEx (str_TempFile.c_str(), 
                              CString (bstr_path), 
                              MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED);
    if (0 == ui_ret)
    {
        CString cs_msg;
        cs_msg.Format (L"Unable to move temp file %s to %s, error %d", 
                       str_TempFile.c_str(), 
                       CString (bstr_path),
                       GetLastError());
        ERROR_LOG ((LPCTSTR)cs_msg);
        return E_FAIL;
    }
    return S_OK;

}   //  SaveOutput (...)

bool CT_ZalSourceReader::b_CheckSemicolons (wstring& str_entry, bool& b_suspiciousEntry)
{
    b_suspiciousEntry = false;
    wstring str_copy (str_entry);

    CT_ExtString xstr_line (str_entry, L";");
    if (xstr_line.i_NFields() < 2)
    {
        return true;
    }

    //bool b_match = xstr_line.b_RegexMatch (L"^.+?[^\\(].+?;.+?[^\\)].*");

    // We should omit semicolons in parentheses or curly brackets.
    // Here, we take into account than parentheses can occur inside
    // curly brackets, but the converse doesn't hold.
    bool b_match = xstr_line.b_RegexMatch (L"^([^\\{]*\\{[^\\}]*\\})*?([^\\{\\(]*\\([^\\)]*\\))*?[^\\{\\(;]*;.+");
    if (!b_match)
    {
        return true;
    }

    xstr_line.v_Reset();
    xstr_line.v_SetBreakChars (L";");
    for (int i_at = 0; !b_suspiciousEntry && (i_at < xstr_line.i_NFields()); ++i_at)
    {
        CT_ExtString xstr_segment (xstr_line.str_GetField (i_at));
        //if (xstr_segment.i_NFields() > 1)
        //{
        //    wstring str_parenth = xstr_segment.str_GetToken (xstr_segment.i_NTokens()-2);
        //    if (L"(" == str_parenth)
        //    {
        //        continue;
        //    }
        //}

        if (i_at < xstr_line.i_NFields()-1)
        {
            CT_ExtString xstr_next (xstr_line.str_GetField (i_at+1), L" \n");
            xstr_next.v_Trim();
            // If it was a semicolon inside a group, go further
            int i_offset = xstr_line.i_GetFieldOffset(i_at+1);
            CT_ExtString xstr_rest (xstr_line.substr(i_offset, xstr_line.length() - i_offset), L" \n");
            ERROR_LOG(L"xstr_rest: " + xstr_rest);
            if (xstr_rest.b_RegexMatch (L"^[^\\{]*?\\}") ||
                xstr_rest.b_RegexMatch (L"^[^\\(]*?\\)"))
            {
                continue;
            }

            if ((_istdigit (xstr_next[0])) || (set_MainSymbols.count (xstr_next.str_GetField (0)) > 0))
            {
                CT_ExtString::ST_Token st_t;
                bool b_ = xstr_line.b_GetField (i_at+1, CT_ExtString::ec_TokenText, st_t);
                if (!b_)
                {
                    ERROR_LOG (L"GetField() returned error");
                    return false;
                }
                int i_semicolonPos = st_t.i_Offset - 1;
                if (L';' != xstr_line[i_semicolonPos])
                {
                    ERROR_LOG (L"Semicolon expected");
                    return false;
                }
                if (st_t.i_Offset < 2)
                {
                    ERROR_LOG (L"Bad offset");
                    return false;
                }
                str_copy[i_semicolonPos] = L'|';
            }
            else
            {
                b_suspiciousEntry = true;
            }
        }
    }   //  for (int i_at = 0; i_at < xstr_line.i_NFields(); ++i_at)

    str_entry = str_copy;

    return true;
}

HRESULT CT_ZalSourceReader::StatusUpdate (int i_progress)
{
    HRESULT h_r = S_OK;

    int i_nConnections = m_vec.GetSize();
    for (int i_c = 0; i_c < i_nConnections; ++i_c)
    {
        Lock();
        CComPtr<IUnknown> sp_unkConnection = m_vec.GetAt (i_c);
        CComQIPtr<IZalNotification2> sp_qiConnection = sp_unkConnection;
        Unlock();
        if (sp_qiConnection)
        {
            h_r = sp_qiConnection->ProgressNotification (i_progress);
        }
    }

    return h_r;
}

HRESULT CT_ZalSourceReader::ShowCurrentWord (BSTR bstr_word)
{
    HRESULT h_r = S_OK;

    int i_nConnections = m_vec.GetSize();
    for (int i_c = 0; i_c < i_nConnections; ++i_c)
    {
        Lock();
        CComPtr<IUnknown> sp_unkConnection = m_vec.GetAt (i_c);
        CComQIPtr<IZalNotification2> sp_qiConnection = sp_unkConnection;
        Unlock();
        if (sp_qiConnection)
        {
            h_r = sp_qiConnection->ShowCurrentWord (bstr_word);
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
        CComQIPtr<IZalNotification2> sp_qiConnection = sp_unkConnection;
        Unlock();
        if (sp_qiConnection)
        {
            h_r = sp_qiConnection->StatusCheck (&b_cancel);
        }
    }

    return h_r;
}

bool CT_ZalSourceReader::b_Show (const wstring& str_msg)
{
    LPTSTR sz_name = _T("\\\\.\\pipe\\ZalConversionLog"); 
  
    HANDLE h_pipe = NULL;
    while (1) 
    { 
        h_pipe = CreateFile (sz_name, 
                             GENERIC_WRITE,
                             0,              // no sharing 
                             NULL,           // default security attributes
                             OPEN_EXISTING,  // opens existing pipe 
                             0,              // default attributes 
                             NULL);          // no template file 
 
      if (INVALID_HANDLE_VALUE != h_pipe) 
      {
          break;
      }
 
      if (GetLastError() != ERROR_PIPE_BUSY) 
      {
          return false;
      }
 
      if (!WaitNamedPipe (sz_name, 500)) 
      { 
         return false;
      } 
   } 
 
/*
   dwMode = PIPE_READMODE_MESSAGE; 
   fSuccess = SetNamedPipeHandleState( 
      hPipe,    // pipe handle 
      &dwMode,  // new pipe mode 
      NULL,     // don't set maximum bytes 
      NULL);    // don't set maximum time 
   if (!fSuccess) 
   {
      printf("SetNamedPipeHandleState failed"); 
      return 0;
   }
*/

    DWORD dw_written = 0;
    UINT ui_ret = WriteFile (h_pipe,            // pipe handle 
                             str_msg.c_str(),   // message 
                             str_msg.length()  * sizeof (wchar_t),  // message length 
                             &dw_written,       // bytes written 
                             NULL);             // not overlapped 
   CloseHandle (h_pipe); 
 
   return ui_ret ? true : false; 

}   //  b_Show (...)
