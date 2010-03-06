#pragma once
#include "FormBuilderBaseConj.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderPast : public CT_FormBuilderBaseConj
{
    struct st_StressData
    {
        ET_StressLocation eo_Type;
        int i_Position;

        st_StressData (ET_StressLocation eo_type, int i_pos)
            : eo_Type (eo_type), i_Position (i_pos)
        {}
    };

public:
    CT_FormBuilderPast (CT_Lexeme * pco_lexeme) : CT_FormBuilderBaseConj (pco_lexeme)
    {}

public:
    HRESULT h_GetEnding (CT_ExtString& xstr_lemma,  
                         ET_Number eo_number, 
                         ET_Gender eo_gender, 
                         CT_ExtString& xstr_ending);

    HRESULT h_GetStressPositions (const CT_ExtString& xstr_lemma,
                                  const wstring& str_ending,
                                  ET_Number eo_number, 
                                  ET_Gender eo_gender, 
                                  vector<int>& vec_iStressPositions);

    HRESULT h_CreateFormTemplate (const CT_ExtString& xstr_lemma,
                                  const CT_ExtString& xstr_ending,
                                  ET_Number eo_number, 
                                  ET_Gender eo_gender, 
                                  CComObject<CT_WordForm> *& pco_wordForm);

    HRESULT h_HandleFleetingVowel (CT_ExtString& xstr_verbForm);

    HRESULT h_Build();
};
