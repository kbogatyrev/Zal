/*
	Implementation of an error reporting class.

	File name: Error.cpp
	
*/

#include "Error.h"

//
// Ctors and dtors
//
Error::Error (const wstring& str_briefDescription,
              const wstring& str_location,
			  const wstring& str_detailedDescription,
              int i_errCode) : 
	str_BriefDescription_ (str_briefDescription),
    str_Location_ (str_location),
	str_DetailedDescription_ (str_detailedDescription),
	i_ErrCode_ (i_errCode)
{
    str_FormattedMsg_ = str_Format_ (str_BriefDescription_, 
                                     str_Location_, 
                                     str_DetailedDescription_, 
                                     i_ErrCode_);
    b_WriteLog (str_FormattedMsg_);
}


Error& Error::operator = (const Error& co_rhs)
{
	str_BriefDescription_ = co_rhs.str_BriefDescription_;
    str_Location_ = co_rhs.str_Location_;
	str_DetailedDescription_ = co_rhs.str_DetailedDescription_;
	i_ErrCode_  = co_rhs.i_ErrCode_;
	
	return *this;
}

template <typename T>
wstring str_ToString_ (T from)
{
	wstringstream io_;
	io_ << from;
	return io_.str();
};

wstring Error::str_Format_ (const wstring& str_briefDescription, 
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
	str_timeStamp += L".";
	str_timeStamp += str_ToString_ (st_localTime.tm_min);
	str_timeStamp += L".";
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

bool Error::b_WriteLog (const wstring& str_msg)
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
}
