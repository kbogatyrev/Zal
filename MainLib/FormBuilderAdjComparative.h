#pragma once
#include "FormBuilderBaseDecl.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderComparative : public CT_FormBuilderBaseDecl
{
public:
    CT_FormBuilderComparative (CT_Lexeme * pco_lexeme) : CT_FormBuilderBaseDecl (pco_lexeme)
    {}

public:
    HRESULT h_Build();

};
