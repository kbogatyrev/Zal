#pragma once

#include "sqlite3.h"

// CT_Sqlite

class CT_Sqlite
{
public:
    CT_Sqlite() : po_Db_ (NULL), po_Stmt_ (NULL)
    {}

    CT_Sqlite (const wstring& str_dbPath) : po_Stmt_ (NULL)
    {
        int i_ret = sqlite3_open16 (str_dbPath.c_str(), &po_Db_);
        if (SQLITE_OK != i_ret)
        {
            po_Db_ = NULL;
        }
    }

protected:
    sqlite3 * po_Db_;
    sqlite3_stmt * po_Stmt_;

    int i_ExtendedErrCode_;

public:
    void v_Open (const wstring& str_path)
    {
        int i_ret = sqlite3_open16 (str_path.c_str(), &po_Db_);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"Unable to open database");
        }
    }

    void v_BeginTransaction()
    {
        if (NULL == po_Db_)
        {
            throw CT_Exception (-1, L"No DB handle");
        }

        int i_ret = SQLITE_OK;
        i_ret = sqlite3_prepare16_v2 (po_Db_, L"BEGIN;", -1, &po_Stmt_, NULL);
	    if (SQLITE_OK != i_ret) 
        {
            throw CT_Exception (i_ret, L"sqlite3_prepare16_v2 failed");
        }

	    i_ret = sqlite3_step (po_Stmt_);
	    if (SQLITE_DONE != i_ret) 
        {
            throw CT_Exception (i_ret, L"sqlite3_step failed");
        }

        i_ret = sqlite3_finalize (po_Stmt_);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_finalize failed");
        }
    }

    void v_CommitTransaction()
    {
        if (NULL == po_Db_)
        {
            throw CT_Exception (-1, L"No DB handle");
        }

        int i_ret = SQLITE_OK;
        i_ret = sqlite3_prepare16_v2 (po_Db_, L"COMMIT;", -1, &po_Stmt_, NULL);
	    if (SQLITE_OK != i_ret) 
        {
            throw CT_Exception (i_ret, L"sqlite3_prepare16_v2 failed");
        }

	    i_ret = sqlite3_step (po_Stmt_);
	    if (SQLITE_DONE != i_ret) 
        {
            throw CT_Exception (i_ret, L"sqlite3_step failed");
        }

	    i_ret = sqlite3_finalize (po_Stmt_);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_finalize failed");
        }
    }

    void v_Exec (const wstring& str_stmt, void (*v_Callback_)(sqlite3_stmt*, void*), void* po_Arguments)
    {
        if (NULL == po_Db_)
        {
            throw CT_Exception (-1, L"No DB handle");
        }

        int i_ret = SQLITE_OK;
        i_ret = sqlite3_prepare16_v2 (po_Db_, str_stmt.c_str(), -1, &po_Stmt_, NULL);
	    if (SQLITE_OK != i_ret) 
        {
            throw CT_Exception (i_ret, L"sqlite3_prepare16_v2 failed");
        }

	    i_ret = sqlite3_step (po_Stmt_);
	    while (i_ret == SQLITE_ROW) 
        {
	        (*v_Callback_)(po_Stmt_, po_Arguments);
		    i_ret = sqlite3_step (po_Stmt_);
        }
	    if (SQLITE_ROW != i_ret && SQLITE_DONE != i_ret) 
        {
            throw CT_Exception (i_ret, L"sqlite3_step failed");
        }

	    i_ret = sqlite3_finalize (po_Stmt_);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_finalize failed");
        }

    }   // v_Exec()

    void v_PrepareForSelect (const wstring& str_stmt)
    {
        int i_ret = sqlite3_prepare16_v2 (po_Db_, str_stmt.c_str(), -1, &po_Stmt_, NULL);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_prepare16_v2 failed");
        }
    }

    void v_PrepareForInsert (const wstring& str_table, int i_columns)
    {
        wstring str_stmt = L"INSERT INTO ";
        str_stmt += str_table;
        str_stmt += L" VALUES (NULL, ";
        for (int i_col = 0; i_col < i_columns; ++i_col)
        {
            if (i_col > 0)
            {
                str_stmt += L",";
            }
            str_stmt += L"?";
        }
        str_stmt += L")";

        int i_ret = sqlite3_prepare16_v2 (po_Db_, str_stmt.c_str(), -1, &po_Stmt_, NULL);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_prepare16_v2 failed");
        }
    }

    void v_Bind (int i_column, bool b_value)
    {
        int i_ret = sqlite3_bind_int (po_Stmt_, i_column, b_value);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_bind_int failed");
        }
    }

    void v_Bind (int i_column, int i_value)
    {
        int i_ret = sqlite3_bind_int (po_Stmt_, i_column, i_value);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_bind_int failed");
        }
    }

    void v_Bind (int i_column, __int64 ll_value)
    {
        int i_ret = sqlite3_bind_int64 (po_Stmt_, i_column, ll_value);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_bind_int64 failed");
        }
    }

    void v_Bind (int i_column, const wstring& str_value)
    {
        int i_ret = sqlite3_bind_text16 (po_Stmt_, i_column, str_value.c_str(), -1, SQLITE_STATIC);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_bind_text16 failed");
        }
    }

    void v_InsertRow()
    {
        if (NULL == po_Db_)
        {
            throw CT_Exception (-1, L"No DB handle");
        }

        if (NULL == po_Stmt_)
        {
            throw CT_Exception (-1, L"No statement");
        }

        int i_ret = sqlite3_step (po_Stmt_);
        if (SQLITE_DONE != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_step failed");
        }

        i_ret = sqlite3_reset (po_Stmt_);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_reset failed");
        }

    }   // v_InsertRow()

    bool b_GetRow()
    {
        if (NULL == po_Db_)
        {
            throw CT_Exception (-1, L"No DB handle");
        }

        if (NULL == po_Stmt_)
        {
            throw CT_Exception (-1, L"No statement");
        }

        int i_ret = sqlite3_step (po_Stmt_);
        if (SQLITE_DONE == i_ret)
        {
            i_ret = sqlite3_reset (po_Stmt_);
            return false;
        }

        if (SQLITE_ROW != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_step failed");
        }

        return true;

    }   // b_GetRow()

    void v_GetData (int i_column, bool& b_value)
    {
        int i_ret = sqlite3_column_int (po_Stmt_, i_column);
        b_value = (i_ret != 0);
    }

    void v_GetData (int i_column, int& i_value)
    {
        i_value = sqlite3_column_int (po_Stmt_, i_column);
    }

    void v_GetData (int i_column, __int64& ll_value)
    {
        ll_value = sqlite3_column_int64 (po_Stmt_, i_column);
    }

    void v_GetData (int i_column, wstring& str_value)
    {
        const void * p_ = sqlite3_column_text16 (po_Stmt_, i_column);
        if (p_)
        {
            str_value = static_cast<wchar_t *>(const_cast<void *>(p_));
        }
    }

    void v_Finalize()
    {
        if (NULL == po_Db_)
        {
            throw CT_Exception (-1, L"No DB handle");
        }

        if (NULL == po_Stmt_)
        {
            throw CT_Exception (-1, L"No statement handle");
        }

        int i_ret = sqlite3_finalize (po_Stmt_);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_finalize failed");
        }
    }

    __int64 ll_GetLastKey()
    {
        if (NULL == po_Db_)
        {
            throw CT_Exception (-1, L"No DB handle");
        }

        if (NULL == po_Stmt_)
        {
            throw CT_Exception (-1, L"No statement handle");
        }

        return sqlite3_last_insert_rowid (po_Db_);    
    }

    int i_GetLastError()
    {
        if (NULL == po_Db_)
        {
            return -1;
        }

        return sqlite3_extended_errcode (po_Db_);
    }

    void v_GetLastError (wstring& str_error)
    {
        if (NULL == po_Db_)
        {
            throw CT_Exception (-1, L"No DB handle");
        }

        wchar_t * sz_error = (wchar_t *)sqlite3_errmsg16 (po_Db_);
        str_error = wstring (sz_error);
    }
};
