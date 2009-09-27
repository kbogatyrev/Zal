// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include "resource.h"

//
// ATL
//
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
//
// CRT
#include <io.h>

//
// Zal
//
#include "Error.h"
#include "Exception.h"


#define ERROR_LOG(str_Msg) wstringstream io__; \
        io__ << __LINE__; \
        CT_Error (str_Msg, \
           std::wstring (_T(__FILE__)) + \
           std::wstring (_T("\t")) + \
           io__.str() + \
           std::wstring (_T("\t")) + \
           std::wstring (_T(__FUNCTION__)));
