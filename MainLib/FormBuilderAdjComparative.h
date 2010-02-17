#pragma once
#include "resource.h"       // main symbols
#include "MainLib_i.h"
#include "WordForm.h"
#include "SqliteWrapper.h"
#include "Endings.h"
#include "FormBuilderBase.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderComparative : public CT_FormBuilderBase
{
public:
    CT_FormBuilderComparative (CT_Lexeme * pco_lexeme) : CT_FormBuilderBase (pco_lexeme)
    {}

public:
    HRESULT h_Build();

};
