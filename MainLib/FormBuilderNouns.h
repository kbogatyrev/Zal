#pragma once
#include "WordForm.h"
#include "SqliteWrapper.h"
#include "Endings.h"
#include "FormBuilderBaseDecl.h"

using namespace std;
using namespace std::tr1;

class CFormBuilderNouns : public CFormBuilderBaseDecl
{
public:
    CFormBuilderNouns (CLexeme * pLexeme) : CFormBuilderBaseDecl (pLexeme, SUBPARADIGM_NOUN)
    {
        m_pEndings = new CNounEndings();
    }

protected:
    int iGramHash (ET_Gender eGender,
                   ET_Animacy eAnimacy,
                   ET_Case eCase,
                   ET_Number eNumber);

    ET_ReturnCode eGetEndings();

    ET_ReturnCode eHandleStemAugment (CEString& sLemma, ET_Number, ET_Case);

    ET_ReturnCode eGetStressType (ET_Number eNumber,                 // in
                                  ET_Case eCase,                     // in
                                  ET_StressLocation& eStressType);   // out

    ET_ReturnCode eHandleAccEnding (ET_Number, ET_Case&);

    ET_ReturnCode eGetStressPositions (const CEString& sLemma, 
                                       const CEString& sEnding,
                                       ET_StressLocation eStressType,
                                       vector<int>& vecStressPos);

    ET_ReturnCode eCreateFormTemplate (ET_Number eNumber, ET_Case eCase, const CEString& sLemma, CWordForm *&);

    ET_ReturnCode eCheckIrregularForms (ET_Gender eGender, 
                                        ET_Animacy eAnimacy, 
                                        ET_Case eCase,
                                        ET_Case eEndingsCase,
                                        ET_Number eNumber, 
                                        bool& bHandled);

public:
    ET_ReturnCode eBuild();

};
