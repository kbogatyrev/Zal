#pragma once
#include "resource.h"       // main symbols
#include "MainLib_i.h"
#include "WordForm.h"
#include "SqliteWrapper.h"
#include "Endings.h"
#include "FormBuilderBase.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderPast : public CT_FormBuilderBase
{
public:
    CT_FormBuilderPast (CT_Lexeme * pco_lexeme) : CT_FormBuilderBase (pco_lexeme)
    {}

public:
    HRESULT h_HandleFleetingVowel (CT_ExtString& xstr_verbForm);
    HRESULT h_Build();
};
