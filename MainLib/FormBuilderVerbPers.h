#pragma once
#include "FormBuilderBaseConj.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderPersonal : public CT_FormBuilderBaseConj
{
public:
    CT_FormBuilderPersonal (CT_Lexeme * pco_lexeme) : CT_FormBuilderBaseConj (pco_lexeme)
    {
        pco_Endings = new CT_PersonalEndings();
    }

public:
    HRESULT h_GetEndings();
    HRESULT h_HandleFleetingVowel (CT_ExtString& xstr_verbForm);
    HRESULT h_Build();
};
