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
	    int i_PrimaryStress;
	    int i_SecondaryStress;
        wstring str_SeeAlso;
    };

    struct ST_Descriptor
    {
        ST_Descriptor() : 
            b_Variant (false),
            b_PluralOf (false), 
            e_PartOfSpeech (POS_UNDEFINED), 
            i_Type (-1), 
            e_AccentType1 (AT_UNDEFINED),
            e_AccentType2 (AT_UNDEFINED),
            b_X (false),
	        b_BoxedX (false),
	        b_Tilde (false),
	        b_Asterisk (false),
	        b_Circle (false),
	        b_Dash (false),
	        b_Loc (false),
	        b_Part (false),
	        b_Yo (false),
	        b_O (false)
        {}

        bool b_Variant;
        wstring str_UsageComment;
        wstring str_MainSymbol;
        bool b_PluralOf;
        wstring str_MainSymbolPluralOf;
        wstring str_AltMainSymbol;
        wstring str_InflectionType; // usually same as MainSymbol
        ET_PartOfSpeech e_PartOfSpeech;
        wstring str_Comment;
        wstring str_AltMainSymbolComment;
        wstring str_AltInflectionComment;

        int i_Type;

        ET_AccentType e_AccentType1;
	    ET_AccentType e_AccentType2;

        bool b_X;
	    bool b_BoxedX;
	    bool b_Tilde;
	    bool b_Asterisk;
	    bool b_Circle;
	    bool b_Dash;
	    bool b_Loc;
	    bool b_Part;
	    bool b_Yo;
	    bool b_O;
        
        vector<int> vec_iNumbersInCircle;

		wstring str_Triangle; //@
		wstring str_Colon; //Context after :

// TODO: Add hatched circle, #,
    };

    CT_ZalEntryParser (const wstring& str_entry) : str_Source (str_entry) 
    {
        v_Init();
    };

    bool b_Parse();

private:

protected:
    wstring str_Source;
    CT_ExtString xstr_Entry;
    int i_Offset;
    map<wstring, ET_PartOfSpeech> map_Pos;
    map<wstring, ET_AccentType> map_AccentType;

    ST_Headword st_Head;
    vector<ST_Descriptor> vec_Descriptors;

protected:
    void v_Init();

    bool b_ExtractHead();
    bool b_CheckMnOt (wstring& str_source, ST_Descriptor&);
    bool b_GetMainSymbol (wstring& str_source, ST_Descriptor&);
    bool b_CheckMainSymbComment (wstring& str_source, ST_Descriptor&);
    bool b_CheckInflectionType (wstring& str_source, ST_Descriptor&);
    bool b_ParseInflectionGroup (wstring& str_descriptor, ST_Descriptor&);
    bool b_CheckCircledDigit (wstring& str_descriptor, ST_Descriptor&);
    bool b_CheckSquareBrackets (wstring& str_descriptor, ST_Descriptor&);
    bool b_CheckHatchedCircle (wstring& str_descriptor, ST_Descriptor&);
	bool b_CheckTriangle (wstring& str_descriptor, ST_Descriptor&);
	bool b_CheckColon (wstring& str_descriptor, ST_Descriptor&);

	bool b_DeleteDoubleStress (wstring& str_source);
};
