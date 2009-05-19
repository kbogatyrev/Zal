#pragma once

class CT_ZalEntryParser
{
public:
    enum ET_PartOfSpeech 
    { 
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
    
    wstring str_Headword;
    wstring str_HeadWordPluralOf;
    wstring str_HeadComment;
    wstring str_DescriptorComment;
    wstring str_MainSymbolVariantComment;

    vector<int> vec_iHomonyms;
	wstring str_Lemma;
	wstring str_MainSymbol;
    wstring str_MainSymbolVariant;
    wstring str_MainSymbolPluralOf;
    wstring str_InflectionType; // usually same as MainSymbol, except for cases like 
	int i_PrimaryStress;
	int i_SecondaryStress;
    ET_PartOfSpeech e_PartOfSpeech;
    ET_Category e_GramType;

//	int i_Declension;
//	int i_Conjugation;
    int i_Type;
    bool b_PluralOf;

    ET_AccentType e_AccentType1;
	ET_AccentType e_AccentType2;

    bool b_Cross;
	bool b_BoxCross;
	bool b_Tilde;
	bool b_Asterisk;
	bool b_Circle;
	bool b_Minus;
	bool b_Loc;
	bool b_Part;
	bool b_Yo;
	bool b_O;

    int i_Deviation;
	int i_Ref;

    wstring str_StemModifier;
	wstring str_AspectPair;
        
    map<wstring, ET_PartOfSpeech> map_Pos;
    map<wstring, ET_AccentType> map_AccentType;

protected:
    void v_Init();
    bool b_ExtractHead();
    bool b_CheckMnOt (wstring& str_source);
    bool b_GetMainSymbol (wstring& str_source);
    bool b_CheckDescriptorComment (wstring& str_source);
    bool b_CheckAlternateInflection (wstring& str_source);
};
