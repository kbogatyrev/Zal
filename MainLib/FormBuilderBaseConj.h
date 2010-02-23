#pragma once
#include "FormBuilderBase.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderBaseConj : public CT_FormBuilderBase
{
protected:
    HRESULT h_GetCommonDeviation (int i_cdNum, St_CommonDeviation& st_data);

    HRESULT h_GetStemStressPosition (const wstring& str_lemma, 
                                     vector<int>& vec_iPosition);

    HRESULT h_GetEndingStressPosition (const wstring& str_lemma, 
                                       const wstring& str_ending,
                                       int& i_position);

    HRESULT h_BuildPastTenseStem (CT_ExtString& xstr_lemma);

    HRESULT h_StressOnPastTenseEnding (ET_AccentType eo_accentType,
                                       ET_Number eo_number, 
                                       ET_Gender eo_gender);

public:
    CT_FormBuilderBaseConj (CT_Lexeme * pco_lexeme) : CT_FormBuilderBase (pco_lexeme)
    {}
};
