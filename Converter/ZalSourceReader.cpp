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

    //FILE * io_brackets = NULL;
    //str_path = str_outputDir + L"\\brackets.txt";
    //i_error = _tfopen_s (&io_brackets, str_path.c_str(), L"w, ccs=UNICODE");
    //if (0 != i_error)
    //{
    //    CString cs_msg;
    //    cs_msg.Format (L"Unable to open file %s, error %d", str_path, i_error);
    //    ERROR_LOG ((LPCTSTR)cs_msg);
    //    return E_FAIL;
    //}

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

        bool b_suspiciousEntry = false;
        bool b_ret = b_CheckSemicolons (str_line, b_suspiciousEntry);
        if (b_suspiciousEntry)
        {
            i_error = _fputts (str_line.c_str(), io_semicolons);
            continue;
        }
    
//        b_ret = b_CheckSquareBrackets (str_line, b_suspiciousEntry);
//        if (b_suspiciousEntry)
//        {
//            i_error = _fputts (str_line.c_str(), io_brackets);
//            continue;
//        }

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
            CT_ExtString xstr_line (str_line);
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
            bool b_ = b_WriteLog (str_line);
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

    fclose (io_outStream);

    return S_OK;

}   // SearchSourceFile (...)

HRESULT CT_ZalSourceReader::SaveOutput (BSTR bstr_path)
{
    BOOL ui_ret = MoveFile (str_TempFile.c_str(), CString (bstr_path));
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

    bool b_match = xstr_line.b_RegexMatch (L"^.+?[^\\(].+?;.+?[^\\)].*");
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
            unsigned int ui_at = xstr_segment.find_last_of (L"%@");
            if (wstring::npos != ui_at)
            {
                b_suspiciousEntry = true;
                return true;
            }
            CT_ExtString xstr_next (xstr_line.str_GetField (i_at+1), L" \n");
            xstr_next.v_Trim();
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

/*
bool CT_ZalSourceReader::b_CheckSquareBrackets (const wstring& str_entry, bool& b_suspiciousEntry)
{
    b_suspiciousEntry = false;

// [//< restriction><headword><main symb, inflection>]
// also possible: [<restriction...] (without "//")...]
// Everything except for headowrd is optional

// Using regex may be tricky since we need to handle nested brackets

    int i_startOffset = str_entry.find (L"[");
    if (wstring::npos == i_startOffset)
    {
        return true;
    }

    int i_endOffset = str_entry.rfind (L"]");
    if (wstring::npos == i_endOffset)
    {
        ATLASSERT(0);
        wstring str_msg (L"No closing bracket: ");
        str_msg += str_entry;
        ERROR_LOG (str_msg);
        return false;
    }

    CT_ExtString xstr_descriptor (str_descriptor);
    bool b_match = xstr_descriptor.b_RegexMatch (L"(.*)?(?:\\[{1,}?(_.+_)*(.+)\\])(.*)");
    if (!b_match)
    {
        return true;
    }

//    ATLASSERT (xstr_descriptor.i_NRegexMatches() == 3);
//    ATLASSERT (xstr_descriptor.i_GetFieldLength (0, CT_ExtString::ec_TokenRegexMatch) == 0);

    wstring str_alternate = str_entry.substr (i_startOffset + 1, 
                                              i_endOffset - i_startOffset - 1);
    CT_ExtString xstr_alternate (str_alternate);
    bool b_match = xstr_alternate.b_RegexMatch (L"^(//)?(_.+_)?(.+)");
    if (!b_match)
    {
        ATLASSERT(0);
        wstring str_msg (L"Unable to parse bracketed alternate: ");
        str_msg += str_entry;
        ERROR_LOG (str_msg);
        return false;
    }

    if (xstr_alternate.i_NRegexMatches() < 1)
    {
        ATLASSERT(0);
        wstring str_msg (L"Empty bracketed alternate: ");
        str_msg += str_entry;
        ERROR_LOG (str_msg);
        return false;
    }

    for (int i_ = 0; i_ < xstr_alternate.i_NFields (CT_ExtString::ec_TokenRegexMatch); ++i_)
    {
        wstring str_token = xstr_alternate.str_GetRegexMatch (i_);
        if (L"//" == str_token)
        {
            // do we care?
            continue;
        }
        if ((L'_' == str_token[0]) && (L'_' == str_token[str_token.length()-1]))
        {
            continue;
        }

        CT_ExtString xstr_token (str_token, L" ");
        if (xstr_token.i_NFields() == 1)
        {
            b_suspiciousEntry = true;
            continue;
        }
        
    }

    return true;

}   //  b_CheckSquareBrackets (...)
*/

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

bool CT_ZalSourceReader::b_WriteLog (const wstring& str_msg)
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

}   //  b_WriteLog (...)
