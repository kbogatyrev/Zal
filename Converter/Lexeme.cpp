#include "StdAfx.h"
#include "ExtString.h"
#include "Lexeme.h"

// Nouns

CT_Noun::CT_Noun()
{
	e_GramType = GT_NOUN;
}

wstring CT_Noun::str_InsertQuery()
{
	wstring str_Query = L"INSERT INTO Nouns VALUES('";
	str_Query += str_Lemma + L"', '" + str_MainSymbol + L"')"; // POS INTEGER, PrimaryStress INTEGER, SecondaryStress INTEGER, Declension INTEGER, Star INTEGER, Circ INTEGER, Minus INTEGER, Loc INTEGER, Part INTEGER, Yo INTEGER, Deviation INTEGER, Ref INTEGER
	return str_Query;
}

// Adjectives

CT_Adj::CT_Adj()
{
	e_GramType = GT_ADJ;
}

wstring CT_Adj::str_InsertQuery()
{
	wstring str_Query = L"INSERT INTO Adjectives VALUES('";
	str_Query += str_Lemma + L"', '" + str_MainSymbol + L"')";
	return str_Query;
}

// Verbs

CT_Verb::CT_Verb()
{
	e_PartOfSpeech = VERB;
	e_GramType = GT_VERB;
}

wstring CT_Verb::str_InsertQuery()
{
	wstring str_Query = L"INSERT INTO Verbs VALUES('";
	str_Query += str_Lemma + L"', '" + str_MainSymbol + L"')";
	return str_Query;
}

// All the rest

CT_Uninflected::CT_Uninflected()
{
	e_GramType = GT_UNCHANGEABLE;
}

wstring CT_Uninflected::str_InsertQuery()
{
	wstring str_Query = L"INSERT INTO Unchangeable VALUES('";
	str_Query += str_Lemma + L"', '" + str_MainSymbol + L"')";
	return str_Query;
}
