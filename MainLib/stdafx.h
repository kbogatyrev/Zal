// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#define S_TRUE MAKE_HRESULT (SEVERITY_SUCCESS, FACILITY_ITF, 10001)
#define S_BOTH MAKE_HRESULT (SEVERITY_SUCCESS, FACILITY_ITF, 10002)
#define S_MORE MAKE_HRESULT (SEVERITY_SUCCESS, FACILITY_ITF, 10003)


//
// ATL
//
#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlstr.h>

using namespace ATL;

//
// STL
//
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <strstream>
#include <ctime>
#include <tchar.h>

//
// TR1
//
#include <regex>

using namespace std::tr1;

//
//
// CRT
#include <io.h>


//
// Zal
//
#include "ErrorHandlers.h"

/*
#define ERROR_LOG(str_Msg) wstringstream io__; \
        io__ << __LINE__; \
        CT_Error::CreateInstance (str_Msg, \
                  std::wstring (_T(__FILE__)) + \
                  std::wstring (_T("\t")) + \
                  io__.str() + \
                  std::wstring (_T("\t")) + \
                  std::wstring (_T(__FUNCTION__)), \
                  std::wstring (_T("")),  \
                  -1, \
                  false);
*/
/*
#define ERROR_LOG(str_Msg) wstringstream io__; \
        io__ << __LINE__; \
        CT_Error::v_HandleError (str_Msg, \
                                 std::wstring (_T(__FILE__)) + \
                                 std::wstring (_T("\t")) + \
                                 io__.str() + \
                                 std::wstring (_T("\t")) + \
                                 std::wstring (_T(__FUNCTION__)), \
                                 std::wstring (_T("")), \
                                 -1, \
                                 false);
*/

#define CYRILLIC

#include "Exception.h"
//#include "ExtString.h"
#include "EString.h"
#include "Globals.h"
//#include "GenericHash.h"
#include "Callbacks.h"
#include "ErrorComWrapper.h"

#include "SqliteWrapper.h"

#include <stdlib.h>
#include <crtdbg.h>

using namespace std;
