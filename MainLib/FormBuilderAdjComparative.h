#pragma once
#include "FormBuilderBaseDecl.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderComparative : public CT_FormBuilderBaseDecl
{
public:
    CT_FormBuilderComparative (CT_Lexeme * pco_lexeme) 
        : CT_FormBuilderBaseDecl (pco_lexeme, SUBPARADIGM_COMPARATIVE)
    {}

public:
    HRESULT h_CreateFormTemplate (const wstring& str_lemma, CComObject<CT_WordForm> *& pco_wordForm);
    HRESULT h_HandleStressAndAdd (CComObject<CT_WordForm> * pco_wordForm, vector<int>& vec_iStress);
    HRESULT h_Build();

};
