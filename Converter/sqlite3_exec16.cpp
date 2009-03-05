#include "Test1.h"

using namespace std;

int sqlite3_exec16(sqlite3* db, wstring str_query, void (*cf_callback)(sqlite3_stmt* stmt)) {
	int i_res = SQLITE_OK;
	wchar_t* str;
	sqlite3_stmt* stmt; // prepared statement
	i_res = sqlite3_prepare16_v2(db, str_query.c_str(), 1024, &stmt, NULL);
	if (i_res) return i_res;
	i_res = sqlite3_step(stmt);
	while (i_res == SQLITE_ROW) {
		(*cf_callback)(stmt);
		i_res = sqlite3_step(stmt);
	}
	if (i_res != SQLITE_ROW && i_res != SQLITE_DONE) {
		sqlite3_finalize(stmt);
		return i_res;
	}
	i_res = sqlite3_finalize(stmt);
	return i_res;
}

void print_wcout(sqlite3_stmt* stmt) {
	wchar_t* str_query_res;
	str_query_res = (wchar_t*)sqlite3_column_text16(stmt, 1);
	wcout << str_query_res << endl;
	return;
}
