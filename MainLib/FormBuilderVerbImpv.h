#pragma once
#include "WordForm.h"
#include "SqliteWrapper.h"
#include "FormBuilderBaseConj.h"

using namespace std;
using namespace std::tr1;

class CFormBuilderImperative : public CFormBuilderBaseConj
{
public:
    CFormBuilderImperative (CLexeme * pLexeme) : 
        CFormBuilderBaseConj (pLexeme, SUBPARADIGM_IMPERATIVE)
    {}

public:
    HRESULT hGetLemma (CEString& sLemma);
    HRESULT hGetEnding (const CEString& sLemma, CEString& sEnding);
    HRESULT hGetStressPositions (CEString& sLemma, 
                                  CEString& sEnding,
                                  vector<int>& vecStressPositions);
    HRESULT hCreateFormTemplate (const CEString& sLemma,
                                  CEString& sEnding,
                                  ET_Number eNumber,
                                  CComObject<CWordForm> *& pWordForm);
    HRESULT hHandleCommonDeviations (CWordForm * pWordForm);
    HRESULT hCheckForIrregularForms (bool& bIsVariant); // check if irregular forms were built and
                                                        // regular processing must be skipped

    HRESULT hBuild();
    HRESULT hBuildIrregularForms();

};
