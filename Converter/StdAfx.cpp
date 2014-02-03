// stdafx.cpp : source file that includes just the standard includes
// ConversionLib.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

//vector<wstring> CT_Error::vec_Log_;
sqlite3 * Hlib::CSqlite::m_spDb_ = NULL;
int Hlib::CSqlite::m_iRefcount_ = 0;
