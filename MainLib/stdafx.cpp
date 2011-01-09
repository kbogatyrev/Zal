// stdafx.cpp : source file that includes just the standard includes
// MainLib.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

vector<wstring> CT_Error::vec_Log_;
sqlite3 * CT_Sqlite::spo_Db = NULL;
int CT_Sqlite::si_Refcount = 0;
