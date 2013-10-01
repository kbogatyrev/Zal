#pragma once
#include "FormBuilderBase.h"

using namespace std;
using namespace std::tr1;

class CFormBuilderBaseConj : public CFormBuilderBase
{
public:
    CFormBuilderBaseConj (CLexeme * pLexeme, ET_Subparadigm eSubparadigm) : 
        CFormBuilderBase (pLexeme, eSubparadigm)
    {}

protected:
    HRESULT hBuildVerbStems();
    HRESULT hStandardAlternation (CEString& sPresentStem);
    HRESULT hGetStemStressPositions (CEString& sLemma, vector<int>& vecPosition);
    HRESULT hGetEndingStressPosition (CEString& sLemma, CEString& sEnding, int& iPosition);
    HRESULT hBuildPastTenseStem (CEString& sLemma);
    HRESULT hFleetingVowelCheck (CEString& sVerbForm);
    HRESULT hHandleYoAlternation (int iStressPos, ET_Subparadigm eSubParadigm, CEString& sLemma);
    HRESULT hGetPastTenseStressTypes (ET_AccentType eAccentType, ET_Number eNumber, ET_Gender eGender, vector<ET_StressLocation>& vecStress);
    bool bHasIrregularPresent();
    bool bHasIrregularPast();
    bool bHasIrregularImperative();
    bool bHasIrregularActivePartPast();
    bool bHasIrregularPassivePartPast();

//    void Get1SgIrregular (vector<CWordForm *>&);
//    void Get3SgIrregular (vector<CWordForm *>&);
//    void Get3PlIrregular (vector<CWordForm *>&);
    void GetIrregularForms(int iHash, vector<CWordForm *>&); // get irregular form; fill stress pos for monosyll

//    void GetPastMIrregular (vector<CWordForm *>&);
//    HRESULT h_GetPastFIrregular (StIrregularForm& stIf);

//    HRESULT h_GetPastPartActIrregular (StIrregularForm& stIf);

//    HRESULT hGetPastPartPassNSgIrregular (StIrregularForm& stIf);
};
