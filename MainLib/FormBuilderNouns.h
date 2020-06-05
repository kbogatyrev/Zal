#ifndef FORMBUILDERNOUNS_H_INCLUDED
#define FORMBUILDERNOUNS_H_INCLUDED

#include "Enums.h"
#include "EString.h"

#include "WordForm.h"
#include "SqliteWrapper.h"
#include "Endings.h"
#include "FormBuilderBaseDecl.h"

using namespace std;
//using namespace std::tr1;

namespace Hlib
{

class CFormBuilderNouns : public CFormBuilderDecl
{
public:
    CFormBuilderNouns (CLexeme * pLexeme) : CFormBuilderDecl (pLexeme, SUBPARADIGM_NOUN)
    {}

protected:
    CEString sGramHash (ET_Gender eGender,
                        ET_Animacy eAnimacy,
                        ET_Case eCase,
                        ET_Number eNumber);

    ET_ReturnCode eHandleStemAugment (CEString& sStem, ET_Number, ET_Case);

    ET_ReturnCode eGetStressType (ET_Number eNumber,                 // in
                                  ET_Case eCase,                     // in
                                  ET_StressLocation& eStressType);   // out

    ET_ReturnCode eHandleAccEnding (ET_Number, ET_Case&);

    ET_ReturnCode eGetStressPositions (const CEString& sStem,
                                       const CEString& sEnding,
                                       ET_Number eNumber,
                                       ET_StressLocation eStressType,
                                       vector<int>& vecStressPos);

    ET_ReturnCode eCreateFormTemplate (ET_Number eNumber, ET_Case eCase, const CEString& sStem, CWordForm *&);

    ET_ReturnCode eCheckIrregularForms (ET_Gender eGender, 
                                        ET_Animacy eAnimacy, 
                                        ET_Case eCase,
                                        ET_Case eEndingsCase,
                                        ET_Number eNumber, 
                                        bool& bHandled);
    void CreateWordForm(vector<int> vecStressPositions, 
                        ET_StressLocation eStressType, 
                        const CEString sStem, 
                        const CEString sEnding, 
                        long long llEndingKey, 
                        CWordForm * pWordForm);

public:
    ET_ReturnCode eBuild();

};

}   //  namespace Hlib

#endif  //  FORMBUILDERNOUNS_H_INCLUDED