#pragma once

class CT_ZalEntryParser
{
public:
    enum ET_PartOfSpeech 
    {
        POS_UNDEFINED,
        POS_NOUN, 
        POS_VERB, 
        POS_ADJ, 
        POS_PRONOUN,
        POS_ADV, 
        POS_COMPAR, 
        POS_PREDIC, 
        POS_PREP, 
        POS_CONJ, 
        POS_PARTICLE, 
        POS_INTERJ, 
        POS_PARENTH     // ?
    };

    enum ET_Category 
    { 
        GT_NOUN, 
        GT_VERB, 
        GT_ADJ, 
        GT_UNINFLECTED 
    };

    enum ET_AccentType
    {
        AT_UNDEFINED,
        AT_A,
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
        AT_F2
    };

    struct ST_Headword
    {
        vector<int> vec_IHomonyms;
        wstring str_Source;
        wstring str_PluralOf;
        wstring str_Comment;
        wstring str_Usage;
	    int i_PrimaryStress;
	    int i_SecondaryStress;
        wstring str_Variant;      // subentry in square brackets
        wstring str_SeeAlso;      // "_см._"; probably same as str_Variant
        wstring str_Origination;  // valid for subentries: барж<а ж 4в [//б<аржа ж 4а]
                                  //                       ^^^^^^                                    
    };

    struct ST_InflectionGroup
    {
        int i_Type;

        ET_AccentType e_AccentType1;
	    ET_AccentType e_AccentType2;

        bool b_X;
	    bool b_BoxedX;
	    bool b_Asterisk;
	    bool b_Circle;

        vector<int> vec_iNumbersInCircle;

        ST_InflectionGroup() :
            i_Type (-1), 
            e_AccentType1 (AT_UNDEFINED),
            e_AccentType2 (AT_UNDEFINED),
            b_X (false),
            b_BoxedX (false),
            b_Asterisk (false),
  	        b_Circle (false)
            {}
    };

    struct ST_Descriptor
    {
        ST_Descriptor() : 
            b_Variant (false),
            b_PluralOf (false), 
            b_Intransitive (false),
            e_PartOfSpeech (POS_UNDEFINED), 
            i_Section (-1),
	        b_Tilde (false),
	        b_Dash (false),
	        b_Yo (false),
	        b_O (false),
            b_Gen2 (false),
            b_Loc2 (false),
            b_Loc2Optional (false)
        {}

        bool b_Variant;
//        wstring str_UsageComment;
        wstring str_MainSymbol;
        bool b_PluralOf;
        bool b_Intransitive;
        wstring str_MainSymbolPluralOf;
        wstring str_AltMainSymbol;
        wstring str_InflectionType; // usually same as MainSymbol
        ET_PartOfSpeech e_PartOfSpeech;
        wstring str_Comment;
        wstring str_AltMainSymbolComment;
        wstring str_AltInflectionComment;

        ST_InflectionGroup st_InflectionGroup;
        ST_InflectionGroup st_AltInflectionGroup;

        wstring str_VerbAlternation;

        int i_Section;  // number after "#"

	    bool b_Tilde;
	    bool b_Dash;
	    bool b_Yo;
	    bool b_O;
        bool b_Gen2;
        bool b_Loc2;
        bool b_Loc2Optional;

        wstring str_Loc2Preposition;
        
//        vector<int> vec_iNumbersInCircle;

		wstring str_Triangle; //@
		wstring str_Percent; //%
		wstring str_Colon; //Context after :
		int i_Dollar; //Number after $ (negative if Roman, 0 if $ lacks a number)
		int i_Sharp; //Number after #
		wstring str_Dollar; //Comment after $, if any

        wstring str_TrailingComment;

    };

    CT_ZalEntryParser (const wstring& str_entry, bool b_secondPass = false) 
        : str_Source (str_entry), b_SecondPass (b_secondPass)
    {
        v_Init();
    };

    bool b_Parse (const wstring& str_head = L"", const wstring str_usageComment = L"");

private:

protected:
    bool b_SecondPass;
    bool b_CommaAfterMainSymbol;
    wstring str_Source;
    map<wstring, ET_PartOfSpeech> map_Pos;
    map<wstring, ET_AccentType> map_AccentType;

    ST_Headword st_Head;
    vector<ST_Descriptor> vec_Descriptors;

protected:
    void v_Init();

    bool b_ExtractHead (wstring& str_source);
    bool b_CheckMnOt (wstring& str_source, ST_Descriptor&);
    bool b_GetMainSymbol (wstring& str_source, ST_Descriptor&);
    bool b_CheckMainSymbComment (wstring& str_source, ST_Descriptor&);
    bool b_CheckInflectionType (wstring& str_source, ST_Descriptor&);
    bool b_ParseInflectionGroup (wstring& str_descriptor, ST_InflectionGroup&);
    bool b_CheckCircledDigit (wstring& str_descriptor, ST_InflectionGroup&);
    bool b_CheckTilde (wstring& str_descriptor, ST_Descriptor&);
    bool b_CheckSquareBrackets (wstring& str_descriptor, ST_Descriptor&);
    bool b_CheckHatchedCircle (wstring& str_descriptor, ST_Descriptor&);
	bool b_CheckSharp (wstring& str_descriptor, ST_Descriptor&);
    bool b_CheckTriangle (wstring& str_descriptor, ST_Descriptor&);
	bool b_CheckColon (wstring& str_descriptor, ST_Descriptor&);
	bool b_CheckDollar (wstring& str_descriptor, ST_Descriptor&);
	bool b_CheckPercent (wstring& str_descriptor, ST_Descriptor&);
    bool b_DeleteDoubleStress (wstring& str_source);
    bool b_CheckComma (wstring& str_descriptor, ST_Descriptor&);
    
};
