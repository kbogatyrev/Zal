#pragma once

#include "Globals.h"
#include "SqliteWrapper.h"

//static wstring str_CyrillicLowercase = L"-абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
//static wstring str_Vowels = L"аеёиоуыэюя";

/*
enum ET_PartOfSpeech 
{
    POS_START,
    POS_NOUN = POS_START, 
    POS_VERB, 
    POS_ADJ, 
    POS_PRONOUN,
    POS_NUM,
    POS_ADV, 
    POS_COMPAR, 
    POS_PREDIC, 
    POS_PREP, 
    POS_CONJ, 
    POS_PARTICLE, 
    POS_INTERJ, 
    POS_PARENTH,     // ?
    POS_END,
    POS_UNDEFINED
};
static void operator++ (ET_PartOfSpeech& eo_pos) 
{
    eo_pos = (ET_PartOfSpeech)(eo_pos + 1);
}
*/

enum ET_Category 
{
    GT_START,
    GT_NOUN = GT_START,
    GT_VERB, 
    GT_ADJ, 
    GT_UNINFLECTED,
    GT_END
};

enum ET_MainSymbol
{
    MS_START,
    MS_M = MS_START,
    MS_MO,
    MS_ZH,
    MS_ZHO,
    MS_S,
    MS_SO,
    MS_MO_ZHO,
    MS_MN,
    MS_MN_NEOD,
    MS_MN_ODUSH,
    MS_MN_OT,
    MS_P,
    MS_MS,
    MS_MS_P,
    MS_CHISL,
    MS_CHISL_P,
    MS_SV,
    MS_NSV,
    MS_SV_NSV,
    MS_N,
    MS_PREDL,
    MS_SOJUZ,
    MS_PREDIK,
    MS_VVODN,
    MS_SRAVN,
    MS_CHAST,
    MS_MEZHD,
    MS_END,
    MS_UNDEFINED
};
static void operator++ (ET_MainSymbol& eo_ms) 
{
    eo_ms = (ET_MainSymbol)(eo_ms + 1);
}

/*
enum ET_AccentType
{
    AT_START,
    AT_A = AT_START,
    AT_A1,
    AT_B,
    AT_B1,
    AT_C,
    AT_C1,
    AT_C2,
    AT_D,
    AT_D1,
    AT_E,
    AT_F,
    AT_F1,
    AT_F2,
    AT_END,
    AT_UNDEFINED
};
static void operator++ (ET_AccentType& eo_at) 
{
    eo_at = (ET_AccentType)(eo_at + 1);
}
*/

//enum ET_StressType
//{
//    STRESS_PRIMARY,
//    STRESS_SECONDARY
//};

struct ST_Headword
{
    vector<int> vec_IHomonyms;
    wstring str_Source;
    wstring str_PluralOf;
    wstring str_Comment;
    wstring str_Usage;
    map<int, ET_StressType> map_Stress;
    wstring str_Variant;      // subentry in square brackets
    wstring str_SeeAlso;      // "_см._"; probably same as str_Variant
    wstring str_Origination;  // valid for subentries: барж<а ж 4в [//б<аржа ж 4а]
                              //                       ^^^^^^

    bool b_SaveToDb (CT_Sqlite * pco_dbHandle, __int64& ll_id);
    bool b_SaveStressData (CT_Sqlite * pco_dbHandle,
                           __int64 ll_headwordId, 
                           int i_position, 
                           ET_StressType eo_type);
};

struct ST_CommonDeviation
{
    bool b_Optional;
    int i_Type;
};

struct ST_InflectionGroup
{
    bool b_HasData;

    int i_Type;

    ET_AccentType e_AccentType1;
    ET_AccentType e_AccentType2;

    bool b_X;
    bool b_BoxedX;
    bool b_FleetingVowel;
//    bool b_StemAugment;
    int i_StemAugmentType;

    vector<ST_CommonDeviation> vec_stCommonDeviations;

    ST_InflectionGroup() :
        b_HasData (false),
        i_Type (-1), 
        e_AccentType1 (AT_UNDEFINED),
        e_AccentType2 (AT_UNDEFINED),
        b_X (false),
        b_BoxedX (false),
        b_FleetingVowel (false),
        i_StemAugmentType (-1)
    {}
};

struct ST_Descriptor
{
    ST_Descriptor() : 
        b_Variant (false),
        b_PluralOf (false), 
        b_Intransitive (false),
        e_MainSymbol (MS_UNDEFINED),
        e_InflectionSymbol (MS_UNDEFINED),
        e_PartOfSpeech (POS_UNDEFINED), 
        i_Section (-1),
        b_NoComparative (false),
        b_AssumedForms (false),
        b_Yo (false),
        b_O (false),
        b_Gen2 (false),
        b_Loc2 (false),
        b_Loc2Optional (false),
        b_HasAspectPair (false),
        b_HasIrregularForms (false),
        i_AspectPairType (0)
    {}

    bool b_Variant;
    wstring str_MainSymbol;
    wstring str_InflectionSymbol; // usually same as MainSymbol
    bool b_PluralOf;
    bool b_Intransitive;
    ET_MainSymbol e_MainSymbol;
    ET_MainSymbol e_InflectionSymbol;
    wstring str_MainSymbolPluralOf;
    wstring str_AltMainSymbol;
    ET_PartOfSpeech e_PartOfSpeech;
    wstring str_Comment;
    wstring str_AltMainSymbolComment;
    wstring str_AltInflectionComment;
    wstring str_GraphicStem;

    ST_InflectionGroup st_InflectionGroup;
    ST_InflectionGroup st_AltInflectionGroup;

    wstring str_VerbAlternation;

    int i_Section;  // number after "#"

    bool b_NoComparative;
    bool b_AssumedForms;
    bool b_Yo;
    bool b_O;
    bool b_Gen2;
    bool b_Loc2;
    bool b_Loc2Optional;
    bool b_HasIrregularForms;

    wstring str_Loc2Preposition;
        
    wstring str_Difficulties;      // List of forms that usually are not used
    wstring str_IrregularForms;    // @ [triangle], list of pairs <_parameter_ value>
    wstring str_RestrictedForms;   // % [diamond]
    wstring str_Contexts;          // Context after :
    bool b_HasAspectPair;
    int i_AspectPairType;          // Number after $ (negative if Roman, 0 if $ lacks a number)
                                   // a.k.a. hatched circle; aspect pair info
    wstring str_AspectPairComment; // Comment after $, if any
    int i_Sharp;                   // Number after # -- redundant, see i_Section

    wstring str_TrailingComment;

    bool b_SaveToDb (CT_Sqlite * pco_dbHandle, __int64 ll_wordId);
    bool b_SaveInflectionGroup (CT_Sqlite * pco_dbHandle,
                                __int64 ll_descriptorId, 
                                const ST_InflectionGroup& st_data, 
                                bool b_isPrimary);
    bool b_SaveIrregularStressData (CT_Sqlite * pco_dbHandle, 
                                    __int64 ll_formId, 
                                    const map<int, ET_StressType>& map_stress);

};  //  struct ST_Descriptor

class CT_ZalEntryParser
{
public:
    CT_ZalEntryParser (const wstring& str_entry, CT_Sqlite * pco_db, bool b_secondPass = false) 
        : str_Source (str_entry), pco_Db (pco_db), b_SecondPass (b_secondPass)
    {
        v_Init();
    };

    bool b_Parse (const wstring& str_head = L"", const wstring str_usageComment = L"");

private:

protected:
    CT_Sqlite * pco_Db;
    bool b_SecondPass;
    bool b_CommaAfterMainSymbol;
    wstring str_Source;
    map<wstring, ET_PartOfSpeech> map_Pos;
    map<wstring, ET_MainSymbol> map_MainSymbol;
    map<wstring, ET_AccentType> map_AccentType;

    ST_Headword st_Head;
    vector<ST_Descriptor> vec_Descriptors;

protected:
    void v_Init();

    bool b_ExtractHead (wstring& str_source);
    bool b_ExtractStress (const wstring& str_headWord, map<int, ET_StressType>& map_pos2Type); 
    bool b_CheckMnOt (wstring& str_source, ST_Descriptor&);
    bool b_GetMainSymbol (wstring& str_source, ST_Descriptor&);
    bool b_CheckMainSymbComment (wstring& str_source, ST_Descriptor&);
    bool b_CheckInflectionType (wstring& str_source, ST_Descriptor&);
    bool b_ParseInflectionGroup (wstring& str_descriptor, ST_InflectionGroup&);
    bool b_CheckCircledDigits (wstring& str_descriptor, ST_InflectionGroup&);
    bool b_CheckTilde (wstring& str_descriptor, ST_Descriptor&);
    bool b_CheckSquareBrackets (wstring& str_descriptor, ST_Descriptor&);
    bool b_CheckHatchedCircle (wstring& str_descriptor, ST_Descriptor&);
    bool b_CheckSharp (wstring& str_descriptor, ST_Descriptor&);
    bool b_CheckDifficulties (wstring& str_descriptor, ST_Descriptor&);
    bool b_CheckTriangle (wstring& str_descriptor, ST_Descriptor&);
    bool b_CheckColon (wstring& str_descriptor, ST_Descriptor&);
    bool b_CheckDollar (wstring& str_descriptor, ST_Descriptor&);
    bool b_CheckPercent (wstring& str_descriptor, ST_Descriptor&);
    bool b_DeleteDoubleStress (wstring& str_source);
    bool b_DetalizeSlashes (wstring& str_source);
    bool b_DetalizeCategories (wstring& str_source);
    bool b_CheckComma (wstring& str_descriptor, ST_Descriptor&);
    bool b_MakeGraphicStem (ST_Descriptor& st_entry);

};
