#pragma once

#include "Globals.h"
#include "SqliteWrapper.h"
#include "LexemeHasher.h"

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
    MS_MS_PREDIK,
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
    MS_NULL,
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
    wstring str_RawSource;      // with stress marks
    wstring str_Source;
    wstring str_PluralOf;
    wstring str_Comment;
    wstring str_Usage;
    map<int, ET_StressType> map_Stress;
    wstring str_Variant;        // subentry in square brackets
    wstring str_VariantComment; // ак<ант м 1а [//_устар._ ак<анф]
                                //               ^^^^^^^^^
    wstring str_SeeRef;         // "_см._"
    wstring str_Origination;    // valid for subentries: барж<а ж 4в [//б<аржа ж 4а]
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
    int i_InflectedParts;   // 0: the last part is inflected (normal)
                            // 1: the first part is inflected (какой-то)
                            // 2: both parts are inflected (конёк-горбунок)

    ET_AccentType e_AccentType1;
    ET_AccentType e_AccentType2;

    bool b_X;
    bool b_BoxedX;
    bool b_FleetingVowel;
//    bool b_StemAugment;
    int i_StemAugmentType;
    wstring str_Comment;

    vector<ST_CommonDeviation> vec_stCommonDeviations;

    ST_InflectionGroup() :
        b_HasData (false),
        i_Type (-1), 
        i_InflectedParts (0), 
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
        b_Reflexive (false),
//        e_MainSymbol (MS_UNDEFINED),
        e_InflectionSymbol (MS_UNDEFINED),
        e_PartOfSpeech (POS_UNDEFINED), 
        b_PastPartPassZhD (false),
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
        b_HasDifficulties (false),
        b_HasDeficiencies (false),
        b_IsImpersonal (false), 
        b_IsIterative (false)
    {}

    bool b_Variant;
    wstring str_MainSymbol;
    wstring str_InflectionSymbol; // usually same as MainSymbol
    wstring str_AltInflectionSymbol;
    bool b_PluralOf;
    bool b_Intransitive;
    bool b_Reflexive;
//    ET_MainSymbol e_MainSymbol;
    ET_MainSymbol e_InflectionSymbol;
    wstring str_MainSymbolPluralOf;
    wstring str_AltMainSymbol;
    ET_PartOfSpeech e_PartOfSpeech;
    wstring str_Comment;
    wstring str_Cognate;          // (_от_ ...); (_женск. к_ ...)
    wstring str_AltMainSymbolComment;
    wstring str_AltInflectionComment;
    wstring str_GraphicStem;

    ST_InflectionGroup st_InflectionGroup;
    ST_InflectionGroup st_AltInflectionGroup;

    wstring str_VerbAlternation;
    bool b_PastPartPassZhD;

    int i_Section;  // number after "#"

    bool b_NoComparative;
    bool b_AssumedForms;
    bool b_Yo;
    bool b_O;
    bool b_Gen2;
    bool b_Loc2;
    bool b_Loc2Optional;
    bool b_HasIrregularForms;
    bool b_HasDifficulties;
    bool b_HasDeficiencies;

    bool b_IsIterative;           // _многокр._
    bool b_IsImpersonal;          // _безл._

    wstring str_Loc2Preposition;
        
    CT_ExtString xstr_Deficient;         // List of nonexistent forms
    CT_ExtString xstr_Difficulties;      // List of forms that usually are not used
    CT_ExtString xstr_Obsolete;          // List of obsolete forms
    CT_ExtString xstr_Colloquial;        // List of colloquial forms ("<ехай")
    CT_ExtString xstr_IrregularForms;    // @ [triangle], list of pairs <_parameter_ value>
    CT_ExtString xstr_AltIrregularForms; // @ [triangle], forms parallel to the regular ones; list of pairs <_parameter_ value>
    wstring str_RestrictedForms;   // % [diamond]
    wstring str_Contexts;          // Context(s) after :
    bool b_HasAspectPair;
    vector<int> vec_i_AspectPairType;          // Number after $ (negative if Roman, 0 if $ lacks a number)
                                               // a.k.a. hatched circle; aspect pair info
    vector<wstring> vec_str_AspectPairComment; // Comment after $, if any
    int i_Sharp;                   // Number after # -- redundant, see i_Section

    wstring str_TrailingComment;

//    ST_XRef st_XRef;

    bool b_SaveToDb (CT_Sqlite * pco_dbHandle, 
                     __int64 ll_wordId, 
                     __int64& ll_descriptorId);
    
    bool b_SaveInflectionGroup (CT_Sqlite * pco_dbHandle,
                                __int64 ll_descriptorId, 
                                const ST_InflectionGroup& st_data, 
                                bool b_isPrimary);
    
    bool b_SaveIrregularStressData (CT_Sqlite * pco_dbHandle, 
                                    __int64 ll_formId, 
                                    const map<int, ET_StressType>& map_stress);

    bool b_SaveIrregularForms (CT_Sqlite * pco_dbHandle, 
                               __int64 ll_descriptorId, 
                               bool b_IsAlternative);

    bool b_SaveDifficultForms (CT_Sqlite * pco_dbHandle, 
                               __int64 ll_descriptorId, 
                               int i_Type);

    bool b_SaveNonexistentForms (CT_Sqlite * pco_dbHandle, 
                                 __int64 ll_descriptorId);

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

//    ST_Headword st_Head;
    vector<ST_Descriptor> vec_Descriptors;
//    vector<ST_XRef> vec_XReferences;

    ST_LexemeHasher st_LexemeHasher;

protected:
    void v_Init();

    bool b_ExtractHead (CT_ExtString& str_source, ST_Headword& st_hw);
    bool b_ExtractStress (CT_ExtString& xstr_headWord, ST_Headword&, map<int, ET_StressType>& map_pos2Type); 
    bool b_CheckMnOt (CT_ExtString& xstr_source, ST_Headword&, ST_Descriptor&);
    bool b_GetMainSymbol (CT_ExtString& xstr_source, ST_Descriptor&);
    bool b_CheckMainSymbComment (CT_ExtString& xstr_source, ST_Descriptor&);
    bool b_CheckInflectionType (CT_ExtString& xstr_source, ST_Descriptor&);
    bool b_ParseInflectionGroup (CT_ExtString& xstr_descriptor, const ST_Headword&, ST_Descriptor&);
    bool b_CheckInflectedParts (CT_ExtString& xstr_source, ST_Descriptor&);
    bool b_CheckCircledDigits (CT_ExtString& xstr_descriptor, ST_InflectionGroup&);
    bool b_CheckTilde (CT_ExtString& xstr_descriptor, ST_Descriptor&);
    bool b_CheckSquareBrackets (CT_ExtString& xstr_descriptor, ST_Headword&, ST_Descriptor&);
    bool b_CheckHatchedCircle (CT_ExtString& xstr_descriptor, ST_Descriptor&);
    bool b_CheckVerbType (CT_ExtString& xstr_descriptor, ST_Descriptor&); // безл., многокр.
    bool b_CheckCognate (CT_ExtString& xstr_descriptor, ST_Descriptor&);
    bool b_CheckSharp (CT_ExtString& xstr_descriptor, ST_Descriptor&);
    bool b_CheckDifficulties (CT_ExtString& xstr_descriptor, ST_Descriptor&);
    bool b_CheckTriangle (const ST_Headword&, 
                          CT_ExtString& xstr_descriptor, 
                          ST_Descriptor&, bool b_alternative = false);
    bool b_CheckColon (CT_ExtString& xstr_descriptor, ST_Descriptor&);
    bool b_CheckPercent (CT_ExtString& xstr_descriptor, ST_Descriptor& st_entry);
    bool b_CheckDollar (CT_ExtString& xstr_descriptor, ST_Descriptor&);
    bool b_DeleteDoubleStress (CT_ExtString& xstr_source);
    bool b_DetalizeDoubleSlashes(CT_ExtString& xstr_source);
    bool b_DetalizeSlashes (CT_ExtString& xstr_source);
    bool b_DetalizeCategories (CT_ExtString& xstr_source);
    bool b_CheckComma (CT_ExtString& xstr_descriptor, ST_Descriptor&);
    bool b_MakeGraphicStem (ST_Descriptor& st_entry, const ST_Headword&);
    bool b_SaveToDb (ST_Headword& st_head, vector<ST_Descriptor>& vec_descriptors);
    bool b_SaveLexemeHash (ST_Headword&, ST_Descriptor&, __int64 ll_descriptorId);
//    bool b_SaveXReference (ST_Headword&, ST_XRef&, __int64 ll_descriptorId);

};
