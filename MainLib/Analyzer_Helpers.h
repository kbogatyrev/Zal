#include "stdafx.h"
#include "MainLib_i.h"

using namespace std;
using namespace std::tr1;

void v_GetTableField_int(sqlite3_stmt *po_statement, void *v_FieldData);
void v_GetTableField_int64(sqlite3_stmt *po_statement, void *v_FieldData);
void v_GetTableField_str(sqlite3_stmt *po_statement, void *v_FieldData);

template <typename Type>
struct S_Field
{
    int i_ColNumber;
    vector<Type> *pvec_FieldData;
};

class CT_GramHelper
{
    public:
    wstring str_wordform;
    wstring str_lemma;
    __int64 ll_lexeme_id;
    ET_PartOfSpeech eo_POS;
    ET_Case eo_Case;
    ET_Number eo_Number;
    ET_AdjForm eo_AdjForm;
    ET_Gender eo_Gender;
    ET_Tense eo_Tense;
    ET_Person eo_Person;
    ET_Animacy eo_Animacy;
    ET_Reflexive eo_Reflexive;
    ET_Voice eo_Voice;
    ET_SpecialForm eo_Special;
    
    CT_GramHelper();
    CT_GramHelper(wstring str_wf);
    CT_GramHelper(wstring str_wf, __int64 ll_id);
    ~CT_GramHelper();

    void GramClear();
    void Clear();
    int i_GramNumber();
    int i_DecodeNumber(int i_gram);
    int i_DecodeString(wstring str_gram);
};

class CT_EndingsTable
{
public:
    wstring str_Stem;
    vector<wstring>* pvec_Endings;
    vector<int>* pvec_Gram;

    CT_EndingsTable();
    ~CT_EndingsTable();
    void Clear();
    int i_AddEnding(int i_Gram, wstring str_Ending);
    vector<int> vec_db_id(CT_Sqlite* pco_dbHandle); // Tries to find the table in the db, returns -1 if there's no such table
    int i_db_Write(CT_Sqlite* pco_dbHandle); // Write the whole table to db, return subtable_id
};
