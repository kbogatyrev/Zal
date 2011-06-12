#pragma once
#include "FormBuilderBaseConj.h"

using namespace std;
using namespace std::tr1;

class CFormBuilderPast : public CFormBuilderBaseConj
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
    CFormBuilderPast (CLexeme * pco_lexeme) : 
        CFormBuilderBaseConj (pco_lexeme, SUBPARADIGM_PAST_TENSE)
    {}

public:
    HRESULT hGetEnding (wstring& str_lemma,  
                         ET_Number eo_number, 
                         ET_Gender eo_gender, 
                         wstring& str_ending);

    HRESULT h_GetStressPositions (const wstring& str_lemma,
                                  const wstring& str_ending,
                                  ET_Number eo_number, 
                                  ET_Gender eo_gender, 
                                  vector<int>& vec_iStressPositions);

    HRESULT h_CreateFormTemplate (const wstring& str_lemma,
                                  const wstring& str_ending,
                                  ET_Number eo_number, 
                                  ET_Gender eo_gender, 
                                  CComObject<CWordForm> *& pco_wordForm);

    HRESULT h_Assemble (CWordForm * pco_wordForm, 
                        int i_stressPos, 
                        wstring& str_lemma, 
                        wstring& str_ending);

    HRESULT h_Build();

    HRESULT h_BuildIrregular (bool&);

};
