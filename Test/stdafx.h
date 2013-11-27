// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
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

#include <atlbase.h>

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

#include "Enums.h"
#include "Exception.h"
//#include "ExtString.h"
#include "EString.h"
#include "Globals.h"
//#include "GenericHash.h"
#include "Callbacks.h"
//#include "ErrorComWrapper.h"
#include "GramHasher.h"

#include "SqliteWrapper.h"
