/**
*/

#pragma once

#include "StdAfx.h"

using namespace std;

class CT_Error
{
private:
    
    static vector<wstring> vec_Log_;

    CT_Error() {};

public:
    static void v_HandleError (const wstring& str_briefDescription,
                               const wstring& str_location,
		                       const wstring& str_detailedDescription = L"",
                               int i_errCode = -1,
                               bool b_writeLog = true) 
    {
        wstring str_formattedMsg = str_Format_ (str_briefDescription, 
                                                str_location, 
                                                str_detailedDescription, 
                                                i_errCode);
        if (b_writeLog)
        {
            b_WriteLog (str_formattedMsg);
        }
        else
        {
            vec_Log_.push_back (str_formattedMsg);
        }
   }

public:
    virtual ~CT_Error() {};

    static wstring str_GetLastError()
    {
        if (vec_Log_.size() > 0)
        {
            return vec_Log_.back();
        }
        else
        {
            wstring str_msg (L"No last error description.");
            return str_msg;
        }
    }

    template <typename T>
    wstring static str_ToString_ (T from)
    {
	    wstringstream io_;
	    io_ << from;
	    return io_.str();
    };

    wstring static str_Format (const wstring& str_briefDescription, 
                               const wstring& str_location, 
                               const wstring& str_detailedDescription = L"", 
                               int i_errCode = -1)
    {
        return str_Format_ (str_briefDescription,
                            str_location,
                            str_detailedDescription,
                            i_errCode);
    }

private:
    wstring static str_Format_ (const wstring& str_briefDescription, 
                                const wstring& str_location, 
                                const wstring& str_detailedDescription, 
                                int i_errCode)
    {
	    time_t time_current;
	    time (&time_current);
        tm st_localTime;
        errno_t i_ret = localtime_s (&st_localTime,  &time_current);

	    wstring str_timeStamp = str_ToString_ (st_localTime.tm_year + 1900);
	    str_timeStamp += L"-";
	    str_timeStamp += str_ToString_ (st_localTime.tm_mon + 1);
	    str_timeStamp += L"-";
	    str_timeStamp += str_ToString_ (st_localTime.tm_mday);
	    str_timeStamp += L"-";
	    str_timeStamp += str_ToString_ (st_localTime.tm_hour);
        str_timeStamp += L":";
	    str_timeStamp += str_ToString_ (st_localTime.tm_min);
        str_timeStamp += L":";
	    str_timeStamp += str_ToString_ (st_localTime.tm_sec);

        wstring str_msg = str_timeStamp + L"\t" +
                            str_briefDescription + L"\t" + 
                            str_location + L"\t";
        if (i_errCode >= 0)
        {
	        str_msg += L"\t" + str_ToString_ (i_errCode);
        }

        if (!str_detailedDescription.empty())
        {
            str_msg += L"\t" + str_detailedDescription;
        }

        return str_msg;
	
    }	//  str_Format_ (...)

    static bool b_WriteLog (const wstring& str_msg)
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

    }   // b_WriteLog()

};
