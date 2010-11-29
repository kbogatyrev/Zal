#pragma once

#include "StdAfx.h"
#include <Regex>
#include "MainLib_i.h"      // constants used by the parser from IDL declaration

using namespace std;

template <typename T>
wstring static str_ToString (T from)
{
    wstringstream io_;
	io_ << from;
	return io_.str();
};

static wstring str_CyrillicLowercase = L"-абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
static wstring str_Vowels = L"аеёиоуыэюя";
static wstring str_Consonants = L"бвгджзйклмнпрстфхцчшщ";
static wstring str_VoicedConsonants = L"бвгджзйлмнр";
static wstring str_NonVoicedConsonants = L"кпстфхцчшщ";
static wstring str_ShSounds = L"шжчщц";

static void operator++ (ET_PartOfSpeech& eo_pos) 
{
    eo_pos = (ET_PartOfSpeech)(eo_pos + 1);
}

static void operator++ (ET_AccentType& eo_at) 
{
    eo_at = (ET_AccentType)(eo_at + 1);
}

