#pragma once
#include "resource.h"       // main symbols
#include "MainLib_i.h"
#include "WordForm.h"
#include "SqliteWrapper.h"
#include "Endings.h"
#include "FormBuilderBase.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderImperative : public CT_FormBuilderBase
{
public:
    CT_FormBuilderImperative (CT_Lexeme * pco_lexeme) : CT_FormBuilderBase (pco_Lexeme)
    {}

public:
    HRESULT h_Build();
};
