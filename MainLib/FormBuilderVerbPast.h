#pragma once
#include "FormBuilderBaseConj.h"

using namespace std;
using namespace std::tr1;

class CFormBuilderPast : public CFormBuilderBaseConj
{
    struct stStressData
    {
        ET_StressLocation eType;
        int iPosition;

        stStressData (ET_StressLocation eType, int iPos) : eType (eType), iPosition (iPos)
        {}
    };

public:
    CFormBuilderPast (CLexeme * pLexeme) : 
        CFormBuilderBaseConj (pLexeme, SUBPARADIGM_PAST_TENSE)
    {}

public:
    HRESULT hGetEnding (CEString& sLemma, ET_Number eNumber, ET_Gender eGender, CEString& sEnding);

    HRESULT hGetStressPositions (const CEString& sLemma,
                                 const CEString& sEnding,
                                 ET_Number eNumber, 
                                 ET_Gender eGender, 
                                 vector<int>& vecStressPositions);

    void CreateFormTemplate (const CEString& sLemma,
                             const CEString& sEnding,
                             ET_Number eNumber, 
                             ET_Gender eGender, 
                             CWordForm *& pWordForm);

    void Assemble (CWordForm * pWordForm, int iStressPos, CEString& sLemma, CEString& sEnding);

    HRESULT hBuild();

    HRESULT hBuildIrregular (bool&);

    void RetractStressToPreverb (CWordForm *, bool bIsOptional);

};
