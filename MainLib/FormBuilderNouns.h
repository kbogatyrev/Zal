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

    void GetEndings();

    HRESULT hHandleStemAugment (CEString& sLemma, ET_Number, ET_Case);

    HRESULT hGetStressType (ET_Number eNumber,                 // in
                            ET_Case eCase,                     // in
                            ET_StressLocation& eStressType);   // out

    HRESULT hHandleAccEnding (ET_Number, ET_Case&);

    void GetStressPositions (const CEString& sLemma, 
                             const CEString& sEnding,
                             ET_StressLocation eStressType,
                             vector<int>& vecStressPos);

    void CreateFormTemplate (ET_Number eNumber, ET_Case eCase, const CEString& sLemma, CWordForm *&);

    HRESULT hCheckIrregularForms (ET_Gender eGender, 
                                  ET_Animacy eAnimacy, 
                                  ET_Case eCase,
                                  ET_Case eEndingsCase,
                                  ET_Number eNumber, 
                                  bool& bHandled);

public:
    HRESULT hBuild();

};
