#include "sqlite3.h"

// CT_Sqlite

class CT_Sqlite
{
public:
	CT_Sqlite()
	{
	}

protected:
    sqlite3 * po_db;    

public:

    int i_Open (const wstring& str_path);
    int i_Exec (const wstring& str_stmt);
    bool b_GetErrorText (wstring& str_errorText);
};
