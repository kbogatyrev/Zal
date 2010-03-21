#pragma once

#include "StdAfx.h"
#include <Regex>
#include "MainLib_i.h"      // constants used by the parser from IDL declaration

using namespace std;
using namespace std::tr1;

template <typename T>
wstring static str_ToString (T from)
{
    wstringstream io_;
	io_ << from;
	return io_.str();
};

static wstring str_CyrillicLowercase = L"-абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
static wstring str_Vowels = L"аеёиоуыэюя";

static void operator++ (ET_PartOfSpeech& eo_pos) 
{
    eo_pos = (ET_PartOfSpeech)(eo_pos + 1);
}

/*
enum ET_AdjForm
{
    ADJ_FORM_UNDEFINED,
    ADJ_FORM_SHORT,
    ADJ_FORM_LONG,
    ADJ_COUNT_COUNT
};
*/

static void operator++ (ET_AccentType& eo_at) 
{
    eo_at = (ET_AccentType)(eo_at + 1);
}

