/*

*/

#include "stdafx.h"
#include "SqliteWrapper.h"
//#include "ExtString.h"
#include "ZalEntryParser.h"
#include "EndingsParser.h"
#include "ZalSourceReader.h"

HRESULT CZalSourceReader::FinalConstruct()
{
    CoInitialize (NULL);

    return S_OK;
}

void CZalSourceReader::FinalRelease()
{
    // TODO: check for and report errors
    CoUninitialize();
}

HRESULT CZalSourceReader::ConvertSourceFile (BSTR bstrSourcePath,
                                             BSTR bstrDbPath,
                                             BSTR bstrUnprocessedPath,
                                             int iMaxEntries,
                                             BOOL bEndings)
{
    USES_CONVERSION;

    HRESULT hr = S_OK;

    StatusUpdate (0);

    bShow (L"### Conversion started...");

    FILE * ioInStream = NULL;
    errno_t iError = _tfopen_s (&ioInStream, OLE2W (bstrSourcePath), L"r, ccs=UNICODE");
    if (0 != iError)
    {
        CString csMsg;
        csMsg.Format (L"Unable to open source, error %d", iError);
        ERROR_LOG ((LPCTSTR)csMsg);
        return E_FAIL;
    }

    CSqlite Sqlite;
//    CSqlite * pco_sqlite = CSqlite::pco_CreateInstance();

    try
    {
        Sqlite.Open (OLE2W (bstrDbPath));
    }
    catch (CException& ex)
    {
        CEString sMsg (ex.szGetDescription());
        CEString sError;
        try
        {
            Sqlite.GetLastError (sError);
            sMsg += L", error %d: ";
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }
    
        CString csMsg;
        csMsg.Format (sMsg, Sqlite.iGetLastError());
        ERROR_LOG ((LPCTSTR)csMsg);

        return MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, 0xF000 + Sqlite.iGetLastError());
    }

    FILE * ioOutStream = NULL;
    iError = _tfopen_s (&ioOutStream, OLE2W (bstrUnprocessedPath), L"w, ccs=UNICODE");
    if (0 != iError)
    {
        CString csMsg;
        csMsg.Format (L"Unable to open unprocessed entries file, error %d", iError);
        ERROR_LOG ((LPCTSTR)csMsg);
        return E_FAIL;
    }

    long lFileLength = _filelength (_fileno (ioInStream))/sizeof (TCHAR);
    long lCharsRead = 0;
    int iPercentDone = 0;
    int iEntriesRead = 0;

    TCHAR szLineBuf[10000];
    BOOL uiAbort = false;

    Sqlite.BeginTransaction();

    for (; (!feof (ioInStream) && !uiAbort); ++iEntriesRead)
    {
        if (iEntriesRead == iMaxEntries)
        {
            break;
        }

        TCHAR * szRet = _fgetts (szLineBuf, 10000, ioInStream);
        if (NULL == szRet)
        {
            iError = ferror (ioInStream);
            if (0 != iError)
            {
                break;
            }
            else
            {
                continue;
            }
        }

        CEString sLine (szLineBuf);
        lCharsRead += sLine.uiLength();

        if (L';' == sLine[0])
        {
            continue;
        }

        sLine.SetBreakChars (L" \n");
        if (sLine.uiGetNumOfFields() < 1)
        {
            continue;
        }

        sLine.Trim();

        if ((iEntriesRead % 10000) == 0 && iEntriesRead > 0)
        {
            Sqlite.CommitTransaction();
            Sqlite.BeginTransaction();
        }

//        if ((iEntriesRead % 10) == 0)
//        {
            CEString sWord (sLine);
            sWord.SetBreakChars (L" ");
            CString csWord (sWord.sGetField (0));
            ShowCurrentWord (csWord.AllocSysString());
//        }

        if (bEndings)
        {
            CEndingsParser Entry (sLine, &Sqlite);
            bool bParseResult = Entry.bParse();
            int iPd = (int) (((double)lCharsRead/(double)lFileLength) * 100);
            if (iPd > iPercentDone)
            {
                iPercentDone = min (iPd, 100);
                StatusUpdate (iPercentDone);
            }
        }
        else
        {
            CZalEntryParser Entry (sLine, &Sqlite);
            bool bParseResult = Entry.bParse();
            if (!bParseResult)
            {
                wstring sMsg(L"*** Parsing error: \n");
                sMsg += sLine;
                ERROR_LOG (sMsg.c_str());
                iError = _fputts (sLine, ioOutStream);
                if (0 != iError)
                {
                    ERROR_LOG (L"*** Error writing unprocessed entry. \n");
                }
            }

            if (iMaxEntries > 0)
            {
                iPercentDone = (int) (((double)iEntriesRead/(double)iMaxEntries) * 100);
            }
            else
            {
                int iPd = (int) (((double)lCharsRead/(double)lFileLength) * 100);
                if (iPd > iPercentDone)
                {
                    iPercentDone = min (iPd, 100);
                    StatusUpdate (iPercentDone);
                }
            }
        }

        StatusCheck (uiAbort);

    }   // for (; (!feof (ioInStream) && !uiAbort); ++iEntriesRead)

    Sqlite.CommitTransaction();

    if (!uiAbort)
    {
        StatusUpdate (100);
    }

    return S_OK;

}   //  ConvertSourceFile (...)

HRESULT CZalSourceReader::PreprocessSourceFile (BSTR bstrSourcePath, BSTR bstrOutputDir)
{
    USES_CONVERSION;

    HRESULT hr = S_OK;

    wchar_t * arrMainSymbol[] = { L"м", L"мо", L"ж", L"жо", L"с", L"со", L"мо-жо", L"мн.",
     L"мн. неод.", L"мн. одуш.", L"мн. _от_", L"п", L"мс", L"мс-п", L"числ.",
     L"числ.-п", L"св", L"нсв", L"св-нсв", L"межд.", L"н", L"предик.", L"предл.",
     L"част.", L"союз", L"вводн.", L"сравн." };

    for (int i_ = 0; i_ < sizeof (arrMainSymbol)/sizeof (wchar_t *); ++i_)
    {
        m_setMainSymbols.insert (arrMainSymbol[i_]);
    }

    CEString sOutputDir (OLE2W (bstrOutputDir));

    StatusUpdate (0);

    bShow (L"### Pre-processing started...");

    FILE * ioInStream = NULL;
    errno_t iError = _tfopen_s (&ioInStream, OLE2W (bstrSourcePath), L"r, ccs=UNICODE");
    if (0 != iError)
    {
        CString csMsg;
        csMsg.Format (L"Unable to open file %s, error %d", CString (bstrSourcePath), iError);
        ERROR_LOG ((LPCTSTR)csMsg);
        return E_FAIL;
    }

    FILE * ioOutStream = NULL;
    CEString sPath = sOutputDir + L"\\out.txt";
    iError = _tfopen_s (&ioOutStream, sPath, L"w, ccs=UNICODE");
    if (0 != iError)
    {
        CString csMsg;
        csMsg.Format (L"Unable to open %s, error %d", sPath, iError);
        ERROR_LOG ((LPCTSTR)csMsg);
        return E_FAIL;
    }

    FILE * ioSemicolons = NULL;
    sPath = sOutputDir + L"\\semicolons.txt";
    iError = _tfopen_s (&ioSemicolons, sPath, L"w, ccs=UNICODE");
    if (0 != iError)
    {
        CString csMsg;
        csMsg.Format (L"Unable to open file %s, error %d", sPath, iError);
        ERROR_LOG ((LPCTSTR)csMsg);
        return E_FAIL;
    }

    long lFileLength = _filelength (_fileno (ioInStream))/sizeof (TCHAR);
    long lCharsRead = 0;
    int iPercentDone = 0;
    int iEntriesRead = 0;

    TCHAR szLineBuf[10000];
    BOOL uiAbort = false;
    for (; (!feof (ioInStream) && !uiAbort); ++iEntriesRead)
    {
        TCHAR * szRet = _fgetts (szLineBuf, 10000, ioInStream);
        if (NULL == szRet)
        {
            int iError = ferror (ioInStream);
            if (0 != iError)
            {
                break;
            }
        }

        CEString sLine (szLineBuf);
        lCharsRead += sLine.uiLength();

        if (L';' == sLine[0])
        {
            continue;
        }

        bool bSuspiciousEntry = false;
        bool bRet = bCheckSemicolons (sLine, bSuspiciousEntry);
        if (bSuspiciousEntry)
        {
            iError = _fputts (sLine, ioSemicolons);
            continue;
        }
    
        iError = _fputts (sLine, ioOutStream);
        int iPd = (int) (((double)lCharsRead/(double)lFileLength) * 100);
        if (iPd > iPercentDone)
        {
            iPercentDone = iPd;
        }

        StatusUpdate (iPercentDone);
        StatusCheck (uiAbort);
    }

    if (!uiAbort)
    {
        StatusUpdate (100);
    }

    return S_OK;

}   //  PreprocessSourceFile (...)

HRESULT CZalSourceReader::SearchSourceFile (BSTR bstrSourcePath, 
                                            BSTR bstrSearchString, 
                                            BOOL bRegexSearch)
{
    USES_CONVERSION;

    StatusUpdate (0);

    bShow (L"### Search started...");

    FILE * ioInStream = NULL;
    errno_t iError = _tfopen_s (&ioInStream, OLE2W (bstrSourcePath), L"r, ccs=UNICODE");
    if (0 != iError)
    {
        CString csMsg;
        csMsg.Format (L"Unable to open file %s, error %d", CString (bstrSourcePath), iError);
        ERROR_LOG ((LPCTSTR)csMsg);
        return E_FAIL;
    }

    wchar_t szDirPath[512];
    DWORD dwRet = GetTempPath (512, szDirPath);
    if ((dwRet > 512) || (0 == dwRet))
    {
        CString csMsg;
        csMsg.Format (L"Error retrieving temp. directory path, error %d", GetLastError());
        ERROR_LOG ((LPCTSTR)csMsg);
        return E_FAIL;
    }

    wchar_t szPath[512];
    UINT uiRet = GetTempFileName (szDirPath, // dir path
                                  L"Zal_",    // temp file name prefix 
                                  0,          // create unique name 
                                  szPath);   // buffer for name 
    if (0 == uiRet)
    {
        CString csMsg;
        csMsg.Format (L"Unable to create temporary file name in %s, error %d", szDirPath, uiRet);
        ERROR_LOG ((LPCTSTR)csMsg);
        return E_FAIL;
    }

    FILE * ioOutStream = NULL;
    iError = _tfopen_s (&ioOutStream, szPath, L"w, ccs=UNICODE");
    if (0 != iError)
    {
        CString csMsg;
        csMsg.Format (L"Unable to open %s, error %d", szPath, iError);
        ERROR_LOG ((LPCTSTR)csMsg);
        return E_FAIL;
    }

    m_sTempFile = szPath;

    long lFileLength = _filelength (_fileno (ioInStream))/sizeof (TCHAR);
    long lCharsRead = 0;
    int iPercentDone = 0;
    int iEntriesRead = 0;

    CEString sHeader (L";   ");
    if (bRegexSearch)
    {
        sHeader += L"*** Regex expression = ";
    }
    else
    {
        sHeader += L"*** Match string = ";
    }
    sHeader += OLE2W (bstrSearchString);
    sHeader += L"\n\n";
    iError = _fputts (sHeader, ioOutStream);

    TCHAR szLineBuf[10000];
    BOOL uiAbort = false;
    for (; (!feof (ioInStream) && !uiAbort); ++iEntriesRead)
    {
        TCHAR * szRet = _fgetts (szLineBuf, 10000, ioInStream);
        if (NULL == szRet)
        {
            int iError = ferror (ioInStream);
            if (0 != iError)
            {
                break;
            }
        }

        CEString sLine (szLineBuf);
        lCharsRead += sLine.uiLength();

        bool bMatch = false;
        CEString sSearch = OLE2W (bstrSearchString);
        if (bRegexSearch)
        {
            CEString sLine (sLine);
            sLine.SetBreakChars (L" \n");
            if (sLine.uiGetNumOfFields() < 1)
            {
                CString csMsg (L"Empty line: ");
                csMsg += sLine;
                ERROR_LOG ((LPCTSTR)csMsg);
                continue;
            }

            sLine.Trim();

            bRegexSearch = sLine.bRegexMatch (sSearch);
        }
        else
        {
            unsigned int uiMatch = sLine.uiFind (sSearch);
            if (ecNotFound != uiMatch)
            {
                bMatch = true;
            }
        }

        if (bMatch)
        {
            iError = _fputts (sLine, ioOutStream);
            sLine = CEString (L"*** NO_LOG *** ") + sLine; // tell UI that we don't want it to be logged
            bool b_ = bShow (sLine);
        }

        int iPd = (int) (((double)lCharsRead/(double)lFileLength) * 100);
        if (iPd > iPercentDone)
        {
            iPercentDone = iPd;
        }

        StatusUpdate (iPercentDone);
        StatusCheck (uiAbort);
        if ((iEntriesRead % 10) == 0)
        {
            CEString sWord (sLine);
            sWord.SetBreakChars (L" ");
            CString csWord (sWord.sGetField (0));
            ShowCurrentWord (csWord.AllocSysString());
        }
    }

    if (!uiAbort)
    {
        StatusUpdate (100);
    }

    fclose (ioOutStream);

    return S_OK;

}   // SearchSourceFile (...)

HRESULT CZalSourceReader::SaveOutput (BSTR bstrPath)
{
    BOOL uiRet = MoveFileEx (m_sTempFile, 
                             CString (bstrPath), 
                             MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED);
    if (0 == uiRet)
    {
        CString csMsg;
        csMsg.Format (L"Unable to move temp file %s to %s, error %d", 
                      m_sTempFile, 
                      CString (bstrPath),
                      GetLastError());
        ERROR_LOG ((LPCTSTR)csMsg);
        return E_FAIL;
    }
    return S_OK;

}   //  SaveOutput (...)

bool CZalSourceReader::bCheckSemicolons (CEString& sEntry, bool& bSuspiciousEntry)
{
    bSuspiciousEntry = false;
    CEString sCopy (sEntry);

    CEString sLine (sEntry);
    sLine.SetBreakChars (L";");
    if (sLine.uiNFields() < 2)
    {
        return true;
    }

    //bool bMatch = sLine.b_RegexMatch (L"^.+?[^\\(].+?;.+?[^\\)].*");

    // We should omit semicolons in parentheses or curly brackets.
    // Here, we take into account than parentheses can occur inside
    // curly brackets, but the converse doesn't hold.
    bool bMatch = sLine.bRegexMatch (L"^([^\\{]*\\{[^\\}]*\\})*?([^\\{\\(]*\\([^\\)]*\\))*?[^\\{\\(;]*;.+");
    if (!bMatch)
    {
        return true;
    }

//    sLine.Reset();
    sLine.SetBreakChars (L";");
    for (int iAt = 0; !bSuspiciousEntry && (iAt < sLine.uiNFields()); ++iAt)
    {
        CEString sSegment (sLine.sGetField (iAt));
        //if (sSegment.i_NFields() > 1)
        //{
        //    wstring str_parenth = sSegment.str_GetToken (sSegment.i_NTokens()-2);
        //    if (L"(" == str_parenth)
        //    {
        //        continue;
        //    }
        //}

        if (iAt < sLine.uiNFields()-1)
        {
            CEString sNext (sLine.sGetField (iAt+1));
            sNext.SetBreakChars (L" \n");
            sNext.Trim();
            // If it was a semicolon inside a group, go further
            int iOffset = sLine.uiGetFieldOffset (iAt+1);
            CEString sRest (sLine.sSubstr (iOffset, sLine.uiLength() - iOffset));
            sRest.SetBreakChars (L" \n");
            ERROR_LOG(L"sRest: " + sRest);
            if (sRest.bRegexMatch (L"^[^\\{]*?\\}") || sRest.bRegexMatch (L"^[^\\(]*?\\)"))
            {
                continue;
            }

            if ((_istdigit (sNext[0])) || (m_setMainSymbols.count (sNext.sGetField (0)) > 0))
            {
//                StToken stT;
//                bool b_ = sLine.bGetField (iAt+1, ec_TokenText, st_t);
                StToken stT = sLine.stGetField (iAt+1, ecTokenText);
                //if (!b_)
                //{
                //    ERROR_LOG (L"GetField() returned error");
                //    return false;
                //}
                int iSemicolonPos = stT.uiOffset - 1;
                if (L';' != sLine[iSemicolonPos])
                {
                    ERROR_LOG (L"Semicolon expected");
                    return false;
                }
                if (stT.uiOffset < 2)
                {
                    ERROR_LOG (L"Bad offset");
                    return false;
                }
                sCopy[iSemicolonPos] = L'|';
            }
            else
            {
                bSuspiciousEntry = true;
            }
        }
    }   //  for (int iAt = 0; iAt < sLine.i_NFields(); ++iAt)

    sEntry = sCopy;

    return true;

}   //  bCheckSemicolons (...)

HRESULT CZalSourceReader::StatusUpdate (int iProgress)
{
    HRESULT hr = S_OK;

    int iNConnections = m_vec.GetSize();
    for (int iC = 0; iC < iNConnections; ++iC)
    {
        Lock();
        CComPtr<IUnknown> spUnkConnection = m_vec.GetAt (iC);
        CComQIPtr<IZalNotification2> spQiConnection = spUnkConnection;
        Unlock();
        if (spQiConnection)
        {
            hr = spQiConnection->ProgressNotification (iProgress);
        }
    }

    return hr;

}   // StatusUpdate (...)

HRESULT CZalSourceReader::ShowCurrentWord (BSTR bstrWord)
{
    HRESULT hr = S_OK;

    int iNConnections = m_vec.GetSize();
    for (int iC = 0; iC < iNConnections; ++iC)
    {
        Lock();
        CComPtr<IUnknown> spUnkConnection = m_vec.GetAt (iC);
        CComQIPtr<IZalNotification2> spQiConnection = spUnkConnection;
        Unlock();
        if (spQiConnection)
        {
            hr = spQiConnection->ShowCurrentWord (bstrWord);
        }
    }

    return hr;

}   //  ShowCurrentWord (...)

HRESULT CZalSourceReader::StatusCheck (BOOL& bCancel)
{
    HRESULT hr = S_OK;

    int iNConnections = m_vec.GetSize();
    for (int iC = 0; iC < iNConnections; ++iC)
    {
        Lock();
        CComPtr<IUnknown> spUnkConnection = m_vec.GetAt (iC);
        CComQIPtr<IZalNotification2> spQiConnection = spUnkConnection;
        Unlock();
        if (spQiConnection)
        {
            hr = spQiConnection->StatusCheck (&bCancel);
        }
    }

    return hr;
}

bool CZalSourceReader::bShow (const CEString& sMsg)
{
    LPTSTR szName = _T("\\\\.\\pipe\\ZalConversionLog"); 
  
    HANDLE hPipe = NULL;
    while (1) 
    { 
        hPipe = CreateFile (szName, 
                            GENERIC_WRITE,
                            0,              // no sharing 
                            NULL,           // default security attributes
                            OPEN_EXISTING,  // opens existing pipe 
                            0,              // default attributes 
                            NULL);          // no template file 
 
      if (INVALID_HANDLE_VALUE != hPipe) 
      {
          break;
      }
 
      if (GetLastError() != ERROR_PIPE_BUSY) 
      {
          return false;
      }
 
      if (!WaitNamedPipe (szName, 500)) 
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

    DWORD dwWritten = 0;
    UINT uiRet = WriteFile (hPipe,            // pipe handle 
                            sMsg,             // message 
                            sMsg.uiLength() * sizeof (wchar_t),  // message length 
                            &dwWritten,       // bytes written 
                            NULL);            // not overlapped 
   CloseHandle (hPipe); 
 
   return uiRet ? true : false; 

}   //  bShow (...)
