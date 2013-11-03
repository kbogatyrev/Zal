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
    ET_ReturnCode eGetLemma (CEString& sLemma);
    ET_ReturnCode eGetEnding (const CEString& sLemma, CEString& sEnding);
    ET_ReturnCode eGetStressPositions (CEString& sLemma, 
                                       CEString& sEnding,
                                       vector<int>& vecStressPositions);
    ET_ReturnCode eCreateFormTemplate (const CEString& sLemma,
                                       CEString& sEnding,
                                       ET_Number eNumber,
                                       CWordForm *& pWordForm);
    ET_ReturnCode eHandleCommonDeviations (CWordForm * pWordForm);
    ET_ReturnCode eCheckForIrregularForms (bool& bIsVariant); // check if irregular forms were built and
                                                        // regular processing must be skipped

    ET_ReturnCode eBuild();
    ET_ReturnCode eBuildIrregularForms();

};
