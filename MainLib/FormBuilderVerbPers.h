#pragma once
#include "resource.h"       // main symbols
#include "MainLib_i.h"
#include "WordForm.h"
#include "SqliteWrapper.h"
#include "Endings.h"
#include "FormBuilderBase.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderPersonal : public CT_FormBuilderBase
{
public:
    CT_FormBuilderPersonal (CT_Lexeme * pco_Lexeme) : CT_FormBuilderBase (pco_Lexeme)
    {}

public:
    HRESULT h_GetEndings();
    HRESULT h_HandleFleetingVowel (CT_ExtString& xstr_verbForm);
    HRESULT h_Build();
};
