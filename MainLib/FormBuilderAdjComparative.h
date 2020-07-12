#ifndef FORMBUILDERADJCOMPARATIVE_H_INCLUDED
#define FORMBUILDERADJCOMPARATIVE_H_INCLUDED

#include "Enums.h"
#include "EString.h"

#include "FormBuilderBaseDecl.h"

using namespace std;

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
    ET_ReturnCode eHandleStressAndAdd(CWordForm * pWordForm, 
        vector<int>& vecIStress, ET_StressLocation, CEString& sStem, const CEString& sEnding, long long llEndingKey);
    ET_ReturnCode eBuild();

};

}   // namespace Hlib

#endif
