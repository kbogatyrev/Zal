#pragma once

#include "Globals.h"
#include "SqliteWrapper.h"
#include "ExtString.h"

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
    ET_StressLocation e_Stress;
    int i_Conjugation;
    ET_Number e_Number;
    ET_Person e_Person;
    bool b_StemAugment;
    int i_CommonDeviation;
    ET_StemAuslaut e_StemAuslaut;

protected:
    void v_Init();
    bool b_SaveToDb();

};
