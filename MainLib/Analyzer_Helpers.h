#include "stdafx.h"
#include "MainLib_i.h"

using namespace std;
using namespace std::tr1;

class CT_GramHelper
{
private:
    static const int MIN_NOUN = 0;
    static const int MIN_ADJ = 1000;
    static const int MIN_NUM = 2000;
    static const int MIN_PRONOUN = 3000;
    static const int MIN_VERB = 4000;
    static const int MIN_UNCHANGEABLE = 20000;

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
