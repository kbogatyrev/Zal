#pragma once
#include "FormBuilderBaseDecl.h"

using namespace std;
using namespace std::tr1;

class CFormBuilderComparative : public CFormBuilderBaseDecl
{
public:
    CFormBuilderComparative (CLexeme * pLexeme) 
        : CFormBuilderBaseDecl (pLexeme, SUBPARADIGM_COMPARATIVE)
    {}

public:
    void CreateFormTemplate (const CEString& sLemma, CWordForm *& pWordForm);
    void HandleStressAndAdd (CWordForm * pWordForm, vector<int>& vecIStress);
    HRESULT hBuild();

};
