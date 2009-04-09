/*

*/

#include "stdafx.h"
#include "SqliteWrapper.h"

void v_Callback_ (sqlite3_stmt * po_stmt) 
{
    wchar_t * str_queryRes = (wchar_t*) sqlite3_column_text16 (po_stmt, 1);
	return;
}

int CT_Sqlite::i_Open (const wstring& str_path)
{
    int i_r = sqlite3_open16 (str_path.c_str(), &po_db);
    if (SQLITE_OK != i_r)
    {
        int i_extErrCode = sqlite3_extended_errcode (po_db);
        return i_extErrCode;
    }

    return S_OK;
}

int CT_Sqlite::i_Exec (const wstring& str_stmt)
{

    if (NULL == po_db)
    {
        return -1;
    }

    int i_r = SQLITE_OK;
    sqlite3_stmt * po_stmt; // prepared statement
    i_r = sqlite3_prepare16_v2 (po_db, str_stmt.c_str(), -1, &po_stmt, NULL);
	if (SQLITE_OK != i_r) 
    {
        int i_extErrCode = sqlite3_extended_errcode (po_db);
        return i_extErrCode;
    }

	i_r = sqlite3_step (po_stmt);
	while (i_r == SQLITE_ROW) 
    {
	    (*v_Callback_)(po_stmt);
		i_r = sqlite3_step (po_stmt);
    }
	if (SQLITE_ROW != i_r && SQLITE_DONE != i_r) 
    {
        int i_extErrCode = sqlite3_extended_errcode (po_db);
        return i_extErrCode;
    }

	i_r = sqlite3_finalize (po_stmt);
    if (SQLITE_OK != i_r)
    {
        int i_extErrCode = sqlite3_extended_errcode (po_db);
        return i_extErrCode;
    }
    return (i_r);
}

bool CT_Sqlite::b_GetErrorText (wstring& str_error)
{
    if (NULL == po_db)
    {
        return false;
    }

    wchar_t * sz_error = (wchar_t *)sqlite3_errmsg16 (po_db);
    str_error = wstring (sz_error);

    return true;
}
