#include "sqlite3.h"
#include <string>
#include <iostream>

using namespace std;

int sqlite3_exec16(sqlite3* db, wstring str_query, void (*cf_callback)(sqlite3_stmt* stmt));
void print_wcout(sqlite3_stmt* stmt);
