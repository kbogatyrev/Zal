#include "stdafx.h"
#include "MainLib_i.h"

using namespace std;
using namespace std::tr1;

class CT_EndingsTable
{
public:
    wstring str_Stem;
    vector<wstring>* pvec_Endings;
    vector<int>* pvec_Gram;
    vector<int>* pvec_Stress;

    CT_EndingsTable();
    ~CT_EndingsTable();
    void Clear();
    int i_AddEnding(int i_Hash, wstring str_Ending, int i_StressPos);
    int i_AddEnding(long l_Hash, wstring str_Ending, int i_StressPos);
    vector<int> vec_db_id(CT_Sqlite* pco_dbHandle); // Tries to find the table in the db, returns -1 if there's no such table
    int i_db_Write(CT_Sqlite* pco_dbHandle); // Write the whole table to db, return subtable_id
};

template <typename WF>
class CT_Wordform_Iterator
{
public:
    ET_PartOfSpeech eo_POS;
    WF co_wf;
    bool b_end;

    CT_Wordform_Iterator();
    ~CT_Wordform_Iterator();
    int i_Initialize(ET_PartOfSpeech eo_in_POS);
    WF get_Wordform();

    CT_Wordform_Iterator operator++();
};
