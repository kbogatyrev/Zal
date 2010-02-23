#pragma once
#include "FormBuilderBaseConj.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderPast : public CT_FormBuilderBaseConj
{
public:
    CT_FormBuilderPast (CT_Lexeme * pco_lexeme) : CT_FormBuilderBaseConj (pco_lexeme)
    {}

public:
    HRESULT h_HandleFleetingVowel (CT_ExtString& xstr_verbForm);
    HRESULT h_Build();
};
