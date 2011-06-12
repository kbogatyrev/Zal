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
    HRESULT hCreateFormTemplate (const CEString& sLemma, CComObject<CWordForm> *& pWordForm);
    HRESULT hHandleStressAndAdd (CComObject<CWordForm> * pWordForm, vector<int>& vecIStress);
    HRESULT hBuild();

};
