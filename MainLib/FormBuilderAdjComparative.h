#ifndef FORMBUILDERADJCOMPARATIVE_H_INCLUDED
#define FORMBUILDERADJCOMPARATIVE_H_INCLUDED

#include "FormBuilderBaseDecl.h"

using namespace std;
using namespace std::tr1;

namespace Hlib
{

    class CLexeme;

class CFormBuilderComparative : public CFormBuilderDecl
{
public:
    CFormBuilderComparative (CLexeme * pLexeme) 
        : CFormBuilderDecl (pLexeme, SUBPARADIGM_COMPARATIVE)
    {}

public:
    ET_ReturnCode eCreateFormTemplate (const CEString& sLemma, CWordForm *& pWordForm);
    ET_ReturnCode eHandleStressAndAdd (CWordForm * pWordForm, vector<int>& vecIStress);
    ET_ReturnCode eBuild();

};

}   // namespace Hlib

#endif
