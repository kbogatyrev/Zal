#pragma once
#include "resource.h"       // main symbols
#include "MainLib_i.h"
#include "WordForm.h"
#include "SqliteWrapper.h"
#include "Endings.h"
#include "FormBuilderBaseConj.h"

using namespace std;
using namespace std::tr1;

class CFormBuilderNonFinite : public CFormBuilderBaseConj
{
public:
    CFormBuilderNonFinite (CLexeme * pLexeme) : 
        CFormBuilderBaseConj (pLexeme, SUBPARADIGM_UNDEFINED)
    {}

public:
    HRESULT hBuild();

private:
    HRESULT hBuildInfinitive();
    HRESULT hBuildPresentActiveParticiple();
    HRESULT hBuildPresentAdverbial();
    HRESULT hBuildPresentPassiveParticiple();
    HRESULT hBuildPastActiveParticiple();
    HRESULT hBuildPastAdverbial();
    HRESULT hBuildPastPassiveParticiple();

    void GetParticipleStressPos (ET_Subparadigm eSubparadigm, vector<int>& vecPositions);

    int iStressPosFromTemplate (const CWordForm& wfTemplate, const CEString& sLemma, int iAt = 0);

    HRESULT hPresAdvType13();
    HRESULT hPresAdvGeneral();

    //
    // Regular derivation off of irregularly derived stem
    //
    HRESULT hDeriveIrregPresAdverbial();
    HRESULT hDeriveIrregPresActiveParticiple();
    HRESULT hDeriveIrregPresPassiveParticiple();

    HRESULT hDeriveIrregPastActPartAndAdverbial();
    
    HRESULT hBuildIrregParticipialFormsLong (ET_Subparadigm eSp);
    
    HRESULT hDeriveIrregPartPresPassShort (ET_Status eStatus);
    HRESULT hBuildIrregPartPastPassShort (bool& bIsVariant);
    HRESULT hDeriveIrregPartPastPassShort();

    bool bHasIrregPartPastPassShort();
};
