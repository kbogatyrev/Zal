#pragma once

#include "Globals.h"
#include "SqliteWrapper.h"
#include "ExtString.h"

enum ET_EndingClass
{
    ENDING_CLASS_UNDEFINED,
    ENDING_CLASS_NOUN,
    ENDING_CLASS_ADJECTIVE, 
    ENDING_CLASS_PRONOUN, 
    ENDING_CLASS_SHORT_ADJECTIVE,
    ENDING_CLASS_PRESENT_TENSE,
    ENDING_CLASS_PAST_TENSE
};

enum ET_EndingStressType
{
    ENDING_STRESS_UNDEFINED,
    ENDING_STRESS_STRESSED,
    ENDING_STRESS_UNSTRESSED
};


class CT_EndingsParser
{
public:
    CT_EndingsParser (const wstring& str_entry, CT_Sqlite * pco_db) 
        : str_Source (str_entry), pco_Db (pco_db)
    {
        v_Init();
    };

    bool b_Parse();

private:

protected:
    CT_Sqlite * pco_Db;
    wstring str_Source;

    CT_ExtString xstr_Ending;
    ET_EndingClass e_Class;
    ET_Gender e_Gender;
    ET_Case e_Case;
    int i_InflectionType;
    ET_Animacy e_Animacy;
    ET_EndingStressType e_Stress;
    int i_Conjugation;
    ET_Number e_Number;
    ET_Person e_Person;
    bool b_StemAugment;
    int i_CommonDeviation;

protected:
    void v_Init();
    bool b_SaveToDb();

};
