#pragma once

#include "ExtString.h"
#include "Callbacks.h"
#include "sqlite3.h"


// CT_Sqlite

static const wchar_t * SZ_SEPARATOR = L"|";

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
        v_BeginTransaction (po_Stmt_);
    }

    void v_BeginTransaction (unsigned int ui_handle)
    {
        v_BeginTransaction ((sqlite3_stmt *)ui_handle);
    }

    void v_BeginTransaction (sqlite3_stmt * po_stmt)
    {
        if (NULL == po_Db_)
        {
            throw CT_Exception (-1, L"No DB handle");
        }

        int i_ret = SQLITE_OK;
        i_ret = sqlite3_prepare16_v2 (po_Db_, L"BEGIN;", -1, &po_stmt, NULL);
	    if (SQLITE_OK != i_ret) 
        {
            throw CT_Exception (i_ret, L"sqlite3_prepare16_v2 failed");
        }

	    i_ret = sqlite3_step (po_stmt);
	    if (SQLITE_DONE != i_ret) 
        {
            throw CT_Exception (i_ret, L"sqlite3_step failed");
        }

        i_ret = sqlite3_finalize (po_stmt);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_finalize failed");
        }
    }

    void v_CommitTransaction()
    {
        v_CommitTransaction (po_Stmt_);
    }

    void v_CommitTransaction (unsigned int ui_handle)
    {
        v_CommitTransaction ((sqlite3_stmt *)ui_handle);
    }

    void v_CommitTransaction (sqlite3_stmt * po_stmt)
    {
        if (NULL == po_Db_)
        {
            throw CT_Exception (-1, L"No DB handle");
        }

        int i_ret = SQLITE_OK;
        i_ret = sqlite3_prepare16_v2 (po_Db_, L"COMMIT;", -1, &po_stmt, NULL);
	    if (SQLITE_OK != i_ret) 
        {
            throw CT_Exception (i_ret, L"sqlite3_prepare16_v2 failed");
        }

	    i_ret = sqlite3_step (po_stmt);
	    if (SQLITE_DONE != i_ret) 
        {
            throw CT_Exception (i_ret, L"sqlite3_step failed");
        }

	    i_ret = sqlite3_finalize (po_stmt);
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
        v_PrepareForSelect (str_stmt, po_Stmt_);
    }

    unsigned int ui_PrepareForSelect (const wstring& str_stmt)
    {
        sqlite3_stmt * po_stmt = NULL;
        v_PrepareForSelect (str_stmt, po_stmt);
        return (unsigned int)po_stmt;
    }

    void v_PrepareForSelect (const wstring& str_stmt, sqlite3_stmt *& po_stmt)
    {
        int i_ret = sqlite3_prepare16_v2 (po_Db_, str_stmt.c_str(), -1, &po_stmt, NULL);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_prepare16_v2 failed");
        }
    }

    void v_PrepareForInsert (const wstring& str_table, int i_columns)
    {
        ui_PrepareForInsert (str_table, i_columns, po_Stmt_);
    }

    unsigned int ui_PrepareForInsert (const wstring& str_table, int i_columns, sqlite3_stmt *& po_stmt)
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

        int i_ret = sqlite3_prepare16_v2 (po_Db_, str_stmt.c_str(), -1, &po_stmt, NULL);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_prepare16_v2 failed");
        }

        return (unsigned int)po_stmt;
    }

    void v_Bind (int i_column, bool b_value)
    {
        v_Bind (i_column, b_value, po_Stmt_);
    }

    void v_Bind (int i_column, bool b_value, unsigned int ui_handle)
    {
        v_Bind (i_column, b_value, (sqlite3_stmt *)ui_handle);
    }

    void v_Bind (int i_column, bool b_value, sqlite3_stmt * po_stmt)
    {
        int i_ret = sqlite3_bind_int (po_stmt, i_column, b_value);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_bind_int failed");
        }
    }

    void v_Bind (int i_column, int i_value)
    {
        v_Bind (i_column, i_value, po_Stmt_);
    }

    void v_Bind (int i_column, int i_value, unsigned int ui_handle)
    {
        v_Bind (i_column, i_value, (sqlite3_stmt *)ui_handle);
    }

    void v_Bind (int i_column, int i_value, sqlite3_stmt * po_stmt)
    {
        int i_ret = sqlite3_bind_int (po_stmt, i_column, i_value);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_bind_int failed");
        }
    }

    void v_Bind (int i_column, unsigned int ui_value)
    {
        v_Bind (i_column, ui_value, po_Stmt_);
    }

    void v_Bind (int i_column, unsigned int ui_value, unsigned int ui_handle)
    {
        v_Bind (i_column, ui_value, (sqlite3_stmt *)ui_handle);
    }

    void v_Bind (int i_column, unsigned int ui_value, sqlite3_stmt * po_stmt)
    {
        int i_ret = sqlite3_bind_int64 (po_stmt, i_column, (__int64)ui_value);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_bind_int64 failed");
        }
    }

    void v_Bind (int i_column, __int64 ll_value)
    {
        v_Bind (i_column, ll_value, po_Stmt_);
    }

    void v_Bind (int i_column, __int64 ll_value, unsigned int ui_handle)
    {
        v_Bind (i_column, ll_value, (sqlite3_stmt *)ui_handle);
    }

    void v_Bind (int i_column, __int64 ll_value, sqlite3_stmt * po_stmt)
    {
        int i_ret = sqlite3_bind_int64 (po_stmt, i_column, ll_value);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_bind_int64 failed");
        }
    }

    void v_Bind (int i_column, const wstring& str_value)
    {
        v_Bind (i_column, str_value, po_Stmt_);
    }

    void v_Bind (int i_column, const wstring& str_value, unsigned int ui_handle)
    {
        v_Bind (i_column, str_value, (sqlite3_stmt *)ui_handle);
    }

    void v_Bind (int i_column, const wstring& str_value, sqlite3_stmt * po_stmt)
    {
        int i_ret = sqlite3_bind_text16 (po_stmt, i_column, str_value.c_str(), -1, SQLITE_STATIC);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_bind_text16 failed");
        }
    }

    void v_InsertRow()
    {
        v_InsertRow (po_Stmt_);
    }

    void v_InsertRow (unsigned int ui_handle)
    {
        v_InsertRow ((sqlite3_stmt *)ui_handle);
    }

    void v_InsertRow (sqlite3_stmt * po_stmt)
    {
        if (NULL == po_Db_)
        {
            throw CT_Exception (-1, L"No DB handle");
        }

        if (NULL == po_stmt)
        {
            throw CT_Exception (-1, L"No statement");
        }

        int i_ret = sqlite3_step (po_stmt);
        if (SQLITE_DONE != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_step failed");
        }

        i_ret = sqlite3_reset (po_stmt);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_reset failed");
        }

    }   // v_InsertRow()

    bool b_GetRow()
    {
        return b_GetRow (po_Stmt_);
    }

    bool b_GetRow (unsigned int ui_handle)
    {
        return b_GetRow ((sqlite3_stmt *)ui_handle);
    }

    bool b_GetRow (sqlite3_stmt * po_stmt)
    {
        if (NULL == po_Db_)
        {
            throw CT_Exception (-1, L"No DB handle");
        }

        if (NULL == po_stmt)
        {
            throw CT_Exception (-1, L"No statement");
        }

        int i_ret = sqlite3_step (po_stmt);
        if (SQLITE_DONE == i_ret)
        {
            i_ret = sqlite3_reset (po_stmt);
            return false;
        }

        if (SQLITE_ROW != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_step failed");
        }

        return true;

    }   // b_GetRow (...)

    void v_GetData (int i_column, bool& b_value)
    {
        v_GetData (i_column, b_value, po_Stmt_);
    }

    void v_GetData (int i_column, bool& b_value, unsigned int ui_handle)
    {
        v_GetData (i_column, b_value, (sqlite3_stmt *)ui_handle);
    }

    void v_GetData (int i_column, bool& b_value, sqlite3_stmt * po_stmt)
    {
        int i_ret = sqlite3_column_int (po_stmt, i_column);
        b_value = (i_ret != 0);
    }

    void v_GetData (int i_column, int& i_value)
    {
        v_GetData (i_column, i_value, po_Stmt_);
    }

    void v_GetData (int i_column, int& i_value, unsigned int ui_handle)
    {
        v_GetData (i_column, i_value, (sqlite3_stmt *)ui_handle);
    }

    void v_GetData (int i_column, int& i_value, sqlite3_stmt * po_stmt)
    {
        i_value = sqlite3_column_int (po_stmt, i_column);
    }

    void v_GetData (int i_column, __int64& ll_value)
    {
        v_GetData (i_column, ll_value, po_Stmt_);
    }

    void v_GetData (int i_column, __int64& ll_value, unsigned int ui_handle)
    {
        v_GetData (i_column, ll_value, (sqlite3_stmt *)ui_handle);
    }

    void v_GetData (int i_column, __int64& ll_value, sqlite3_stmt * po_stmt)
    {
        ll_value = sqlite3_column_int64 (po_stmt, i_column);
    }

    void v_GetData (int i_column, wstring& str_value)
    {
        v_GetData (i_column, str_value, po_Stmt_);
    }

    void v_GetData (int i_column, wstring& str_value, unsigned int ui_handle)
    {
        v_GetData (i_column, str_value, (sqlite3_stmt *)ui_handle);
    }

    void v_GetData (int i_column, wstring& str_value, sqlite3_stmt * po_stmt)
    {
        const void * p_ = sqlite3_column_text16 (po_stmt, i_column);
        if (p_)
        {
            str_value = static_cast<wchar_t *>(const_cast<void *>(p_));
        }
    }

    void v_Finalize()
    {
        v_Finalize (po_Stmt_);
    }

    void v_Finalize (unsigned int ui_handle)
    {
        v_Finalize ((sqlite3_stmt *)ui_handle);
    }

    void v_Finalize (sqlite3_stmt * po_stmt)
    {
        if (NULL == po_Db_)
        {
            throw CT_Exception (-1, L"No DB handle");
        }

        if (NULL == po_stmt)
        {
            throw CT_Exception (-1, L"No statement handle");
        }

        int i_ret = sqlite3_finalize (po_stmt);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_finalize failed");
        }
    }

    __int64 ll_GetLastKey()
    {
        return ll_GetLastKey (po_Stmt_);
    }

    __int64 ll_GetLastKey (unsigned int ui_handle)
    {
        return ll_GetLastKey ((sqlite3_stmt *)ui_handle);
    }

    __int64 ll_GetLastKey (sqlite3_stmt * po_stmt)
    {
        if (NULL == po_Db_)
        {
            throw CT_Exception (-1, L"No DB handle");
        }

        if (NULL == po_stmt)
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

    bool b_TableExists (const wstring& str_table)
    {
        wstring str_query (L"SELECT ");
        str_query += str_table;
        str_query += L" FROM sqlite_master WHERE type='table';";
        int i_ret = sqlite3_prepare16_v2 (po_Db_, str_query.c_str(), -1, &po_Stmt_, NULL);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_prepare16_v2 failed");
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
    }

    bool b_ExportTable (const wstring& str_table, const wstring& str_path)
    {
        if (NULL == po_Db_)
        {
            throw CT_Exception (-1, L"No DB handle");
        }

        FILE * io_outStream = NULL;
        errno_t i_error = _tfopen_s (&io_outStream, str_path.c_str(), L"w, ccs=UNICODE");
        if (0 != i_error)
        {
            CString cs_msg;
            cs_msg.Format (L"Unable to open export file, error %d", i_error);
            throw CT_Exception (-1, wstring (cs_msg));
        }

        wstring str_query (L"SELECT * FROM ");
        str_query += str_table;
        str_query += L";";

        sqlite3_stmt * po_stmt = NULL;
        int i_ret = sqlite3_prepare16_v2 (po_Db_, str_query.c_str(), -1, &po_stmt, NULL);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_prepare16_v2 failed");
        }

        wstring str_header;
        int i_columns = sqlite3_column_count (po_stmt);
        for (int i_colName = 0; i_colName < i_columns; ++i_colName)
        {
            str_header += (wchar_t *)sqlite3_column_name16 (po_stmt, i_colName);
            if (i_colName < i_columns - 1)
            {
                str_header += SZ_SEPARATOR;
            }
        }
        str_header += L"\n";

        i_error = _fputts (str_header.c_str(), io_outStream);
        if (0 != i_error)
        {
            ERROR_LOG (L"Error writing export table header. \n");
        }

        while (b_GetRow (po_stmt))
        {
            wstring str_out;
            for (int i_col = 0; i_col < i_columns; ++i_col)
            {
                wstring str_col;
                v_GetData (i_col, str_col, po_stmt);
                if (str_out.length() > 0)
                {
                    str_out += SZ_SEPARATOR;
                }
                str_out += str_col;
            }
            str_out += L"\n";

            i_error = _fputts (str_out.c_str(), io_outStream);
            if (0 != i_error)
            {
                throw CT_Exception (i_error, L"Error writing export table header.");
            }
        }       //  while (...)

        fclose (io_outStream);

        return true;

    }   //  b_ExportTable (...)

    bool b_ImportTable (const wstring& str_table, 
                        const wstring& str_path, 
                        CT_ProgressCallback& co_progress)
    {
        if (NULL == po_Db_)
        {
            throw CT_Exception (-1, L"No DB handle");
        }

        FILE * io_inStream = NULL;
        errno_t i_error = _tfopen_s (&io_inStream, str_path.c_str(), L"r, ccs=UNICODE");
        if (0 != i_error)
        {
            CString cs_msg;
            cs_msg.Format (L"Unable to open import file, error %d", i_error);
            throw CT_Exception (-1, wstring (cs_msg));
        }

        long l_fileLength = _filelength (_fileno (io_inStream))/sizeof (wchar_t);
        long l_charsRead = 0;
        int i_percentDone = 0;
//        int i_entriesRead = 0;

        TCHAR sz_lineBuf[10000];
        
        TCHAR * sz_ret = _fgetts (sz_lineBuf, 10000, io_inStream);
        if (NULL == sz_ret)
        {
            i_error = ferror (io_inStream);
            if (0 != i_error)
            {
                throw CT_Exception (i_error, L"Error reading import file header.");
            }
            else
            {
                throw CT_Exception (i_error, L"Empty header.");
            }
        }

        wstring str_separators (SZ_SEPARATOR);
        str_separators += L"\n";
        CT_ExtString xstr_header (sz_lineBuf, str_separators);
        if (xstr_header.i_GetNumOfFields() < 1)
        {
            throw CT_Exception (-1, L"Parsing error: no fields.");
        }

        int i_columns = xstr_header.i_NFields();

        wstring str_createStmt (L"CREATE TABLE ");
        str_createStmt += str_table;
        str_createStmt += L" (";
        str_createStmt += xstr_header.str_GetField (0);
        str_createStmt += L" INTEGER ";
        for (int i_col = 1; i_col < i_columns; ++i_col)
        {
            str_createStmt += L", ";
            str_createStmt += xstr_header.str_GetField (i_col);
            str_createStmt += L" TEXT";
        }
        str_createStmt += L");";

        sqlite3_stmt * po_stmt = NULL;
        int i_ret = sqlite3_prepare16_v2 (po_Db_, str_createStmt.c_str(), -1, &po_stmt, NULL);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_prepare16_v2 failed for create.");
        }

	    i_ret = sqlite3_step (po_stmt);
	    if (SQLITE_DONE != i_ret) 
        {
            throw CT_Exception (i_ret, L"sqlite3_step failed for create.");
        }

        i_ret = sqlite3_finalize (po_stmt);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_finalize failed for create.");
        }

        wstring str_stmt = L"INSERT INTO ";
        str_stmt += str_table;
        str_stmt += L" VALUES (";
        for (int i_col = 0; i_col < i_columns; ++i_col)
        {
            if (i_col > 0)
            {
                str_stmt += L",";
            }
            str_stmt += L"?";
        }
        str_stmt += L")";

        i_ret = sqlite3_prepare16_v2 (po_Db_, str_stmt.c_str(), -1, &po_stmt, NULL);
        if (SQLITE_OK != i_ret)
        {
            throw CT_Exception (i_ret, L"sqlite3_prepare16_v2 failed");
        }

        v_BeginTransaction (po_stmt);
       
        int i_entriesRead = 0;
        for (; !feof (io_inStream); ++i_entriesRead)
        {
            sz_ret = _fgetts (sz_lineBuf, 10000, io_inStream);
            CT_ExtString xstr_line (sz_lineBuf, str_separators);
            if (NULL == sz_ret)
            {
                i_error = ferror (io_inStream);
                if (0 != i_error)
                {
                    throw CT_Exception (i_ret, L"Error reading import file.");
                }
                else
                {
                    continue;
                }
            }

            l_charsRead += xstr_line.length();

            if (xstr_line.i_NFields() != i_columns)
            {
                throw CT_Exception (-1, L"Number of fields does not match number of columns.");
            }

            __int64 ll_id = _wtoi64 ((xstr_line.str_GetField (0)).c_str());
            v_Bind (1, ll_id, po_stmt);

            for (int i_col = 2; i_col <= i_columns; ++i_col)
            {
                v_Bind (i_col, xstr_line.str_GetField (i_col-1), po_stmt);
            }
            
            v_InsertRow (po_stmt);

            int i_pd = (int) (((double)l_charsRead/(double)l_fileLength) * 100);
            if (i_pd > i_percentDone)
            {
                i_percentDone = min (i_pd, 100);
//                if (pStatusUpdate)
//                {
                    co_progress (i_percentDone);
//                }
            }

        }   //  for (; !feof (io_inStream); ++i_entriesRead)

        co_progress (100);

        v_Finalize (po_stmt);

        v_CommitTransaction (po_stmt);

        fclose (io_inStream);

        return true;

    }   //  b_ImportTable (...)
};
