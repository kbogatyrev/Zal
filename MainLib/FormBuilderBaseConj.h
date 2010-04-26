#pragma once
#include "FormBuilderBase.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderBaseConj : public CT_FormBuilderBase
{
protected:
    HRESULT h_GetStemStressPositions (const wstring& str_lemma, 
                                      vector<int>& vec_iPosition);

    HRESULT h_GetEndingStressPosition (const wstring& str_lemma, 
                                       const wstring& str_ending,
                                       int& i_position);

    HRESULT h_BuildPastTenseStem (wstring& xstr_lemma);

    HRESULT h_HandleYoAlternation (int i_stressPos,
                                   ET_Subparadigm eo_subParadigm, 
                                   wstring& str_lemma);

    HRESULT h_GetPastTenseStressTypes (ET_AccentType eo_accentType,
                                       ET_Number eo_number, 
                                       ET_Gender eo_gender,
                                       vector<ET_StressLocation>& vec_eoStress);

    HRESULT h_HandleCommonDeviations();

    HRESULT h_CommonDeviation_1 (bool b_isOptional);

    HRESULT h_CommonDeviations_2_3 (int i_deviation, bool b_isOptional);

    HRESULT h_RetractStress (int i_hash, bool b_isOptional);

public:
    CT_FormBuilderBaseConj (CT_Lexeme * pco_lexeme) : CT_FormBuilderBase (pco_lexeme)
    {}
};
