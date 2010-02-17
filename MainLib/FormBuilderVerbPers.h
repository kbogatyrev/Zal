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
    CT_FormBuilderPersonal (CT_Lexeme * pco_lexeme) : CT_FormBuilderBase (pco_lexeme)
    {
        pco_Endings = new CT_PersonalEndings();
    }

public:
    HRESULT h_GetEndings();
    HRESULT h_HandleFleetingVowel (CT_ExtString& xstr_verbForm);
    HRESULT h_Build();
};
