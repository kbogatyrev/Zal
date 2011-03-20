#include "stdafx.h"
#include "MainLib_i.h"

using namespace std;
using namespace std::tr1;

struct struct_stem_links
{
public:
    int i_stem_id;
    __int64 ll_lexeme_id;
    int i_endings_link;
    int i_stress_pos;
    wstring str_lemma;
};

class CT_EndingsTable
{
public:
    vector<wstring> vec_Endings;
    vector<int> vec_Gram;
    vector<int> vec_Stress;

    int i_lexemes_count, i_cut_right;
    wstring str_stem_ending, str_lemma_ending;

    CT_EndingsTable();
    ~CT_EndingsTable();
    void Clear();
    int i_AddEnding(int i_Hash, wstring str_Ending, int i_StressPos);
    int i_AddEnding(long l_Hash, wstring str_Ending, int i_StressPos);
    vector<int> vec_Find(wstring str_ending, int i_stress_pos = -2);
    vector<int> vec_db_id(CT_Sqlite* pco_dbHandle); // Tries to find the table in the db, returns -1 if there's no such table
    int i_db_Write(CT_Sqlite* pco_dbHandle); // Write the whole table to the db, return subtable_id
};
