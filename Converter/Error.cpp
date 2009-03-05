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
// TODO log
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

    wstring str_msg = str_timeStamp + L"\r\n" +
                        str_briefDescription + L"\r\n" + 
                        str_location + L"\r\n";
    if (i_errCode >= 0)
    {
	    str_msg += L"\r\n" + str_ToString_ (i_errCode);
    }

    if (!str_detailedDescription.empty())
    {
        str_msg += L"\r\n" + str_detailedDescription;
    }

    return str_msg;
	
}	//  
