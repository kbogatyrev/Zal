enum e_pos {NOUN, VERB, ADJ, ADV, COMPAR, PREDIC, PREP, CONJ, PART, INTERJ, PARENTH};
enum e_gt {GT_NOUN, GT_VERB, GT_ADJ, GT_UNCHANGEABLE};

class CT_Lexeme
{
public:
	wstring str_Lemma;
	wstring str_MainSymbol;
	int i_PrimaryStress;
	int i_SecondaryStress;
	e_pos e_PartOfSpeech;
	e_gt e_GramType;

//	virtual wstring str_InsertQuery();
};

class CT_Noun : public CT_Lexeme
{
public:
	int i_Declension;
	wstring str_AccentType;
	bool b_Asterisc;
	bool b_Circle;
	bool b_Minus;
	bool b_Loc;
	bool b_Part;
	bool b_Yo;
	int i_Deviation;
	int i_Ref;

	CT_Noun();
	wstring str_InsertQuery();
};

class CT_Adj : public CT_Lexeme
{
public:
	int i_Declension;
	wstring str_AccentType1;
	wstring str_AccentType2;
	bool b_Asterisc;
	bool b_Circle;
	bool b_Minus;
	bool b_Cross;
	bool b_BoxCross;
	bool b_Tilde;
	bool b_Yo;
	int i_Deviation;
	int i_Ref;

	CT_Adj();
	wstring str_InsertQuery();
};

class CT_Verb : public CT_Lexeme
{
public:
	int i_Conjugation;
	wstring str_AccentType1;
	wstring str_AccentType2;
	wstring str_StemModifier;
	wstring str_AspectPair;
	bool b_Asterisc;
	bool b_Circle;
	bool b_Cross;
	bool b_BoxCross;
	bool b_Yo;
	bool b_O;
	int i_Deviation;
	int i_Ref;

	CT_Verb();
    wstring str_InsertQuery();
};

class CT_Uninflected : public CT_Lexeme
{
public:
	CT_Uninflected();
	wstring str_InsertQuery();
};
