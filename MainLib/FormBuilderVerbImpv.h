#pragma once
#include "WordForm.h"
#include "SqliteWrapper.h"
#include "FormBuilderBaseConj.h"

using namespace std;
using namespace std::tr1;

class CFormBuilderImperative : public CFormBuilderBaseConj
{
public:
    CFormBuilderImperative (CLexeme * pco_lexeme) : 
        CFormBuilderBaseConj (pco_lexeme, SUBPARADIGM_IMPERATIVE)
    {}

public:
    HRESULT h_GetLemma (wstring& str_lemma);
    HRESULT hGetEnding (const wstring& str_lemma, wstring& str_ending);
    HRESULT h_GetStressPositions (wstring& str_lemma, 
                                  wstring& str_ending,
                                  vector<int>& vec_stressPositions);
    HRESULT h_CreateFormTemplate (const wstring& str_lemma,
                                  wstring& str_ending,
                                  ET_Number eo_number,
                                  CComObject<CWordForm> *& pco_wordForm);
    HRESULT h_HandleCommonDeviations (CWordForm * pco_wordForm);
    HRESULT h_CheckForIrregularForms (bool& b_isVariant); // check if irregular forms were built and
                                                          // regular processing must be skipped

    HRESULT h_Build();
    HRESULT h_BuildIrregularForms();

};
