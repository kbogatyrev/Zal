/**
*/

#pragma once

#include "StdAfx.h"

using namespace std;

class Error
{
public:

private:
	int i_ErrCode_;
	wstring str_BriefDescription_;
    wstring str_Location_;
	wstring str_DetailedDescription_;

    wstring str_FormattedMsg_;

public:
    Error() {};

    Error  (const wstring& str_briefDescription,
            const wstring& str_location,
		    const wstring& str_detailedDescription = L"",
            int i_errCode = -1);

    Error (const Error&);

	//
	//  Non-virtual destructor: this is not a base class
	//
    ~Error() {};

	Error& operator = (const Error&);

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
                                int i_errCode);


};
