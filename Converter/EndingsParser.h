#pragma once

#include "Globals.h"
#include "SqliteWrapper.h"
#include "ExtString.h"

enum ET_EndingClass
{
    ECLASS_UNDEFINED,
    ECLASS_NOUN,
    ECLASS_ADJECTIVE, 
    ECLASS_PRONOUN, 
    ECLASS_SHORT_ADJECTIVE,
    ECLASS_PRESENT_TENSE,
    ECLASS_PAST_TENSE
};

enum ET_EndingStressType
{
    ESTRESS_UNDEFINED,
    ESTRESS_STRESSED,
    ESTRESS_UNSTRESSED
};

/*
enum ET_EndingGender
{
    EGENDER_START,
    EGENDER_MASCULINE = EGENDER_START, 
    EGENDER_FEMININE, 
    EGENDER_NEUTER, 
    EGENDER_END,
    EGENDER_UNDEFINED
};

enum ET_EndingCase
{
    ECASE_START,
    ECASE_NOMINATIVE = ECASE_START, 
    ECASE_GENITIVE, 
    ECASE_DATIVE, 
    ECASE_ACCUSATIVE,
    ECASE_INSTRUMENTAL,
    ECASE_LOCATIVE,
    ECASE_UNDEFINED
};
*/

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
//    bool b_Animate;
    ET_EndingStressType e_Stress;
    int i_Conjugation;
//    bool b_Stressed;
    bool b_Plural;
    int i_Person;
    bool b_StemAugment;
    int i_CommonDeviation;

protected:
    void v_Init();
    bool b_SaveToDb();

};
