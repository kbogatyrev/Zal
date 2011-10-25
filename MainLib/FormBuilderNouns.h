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
    int i_GramHash (ET_Gender eGender,
                    ET_Animacy eAnimacy,
                    ET_Case eCase,
                    ET_Number eNumber);

    HRESULT hGetEndings();

    HRESULT hHandleStemAugment (CEString& sLemma, ET_Number, ET_Case);

    HRESULT hGetStressType (ET_Number eNumber,                 // in
                            ET_Case eCase,                     // in
                            ET_StressLocation& eStressType);   // out

    HRESULT hHandleAccEnding (ET_Number, ET_Case&);

    HRESULT hGetStressPositions (const CEString& sLemma, 
                                 const CEString& sEnding,
                                 ET_StressLocation eStressType,
                                 vector<int>& vecStressPos);

    HRESULT hCreateFormTemplate (ET_Number eNumber, 
                                 ET_Case eCase, 
                                 const CEString& sLemma, 
                                 CComObject<CWordForm> *&);

    HRESULT hAssemble (CWordForm *, 
                       ET_StressLocation,
                       int iStressPos, 
                       const CEString& sEnding);

    HRESULT hCheckIrregularForms (ET_Gender, ET_Animacy, ET_Case, ET_Number, bool& bHandled);

public:
    HRESULT hBuild();

};
