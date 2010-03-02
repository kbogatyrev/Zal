#pragma once
#include "FormBuilderBaseDecl.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderShortAdj : public CT_FormBuilderBaseDecl
{
public:
    CT_FormBuilderShortAdj (CT_Lexeme * pco_lexeme) : CT_FormBuilderBaseDecl (pco_lexeme)
    {
        pco_Endings = new CT_AdjShortEndings();
    }

public:
    HRESULT h_GetEndings();
    HRESULT h_StressOnEnding (ET_Number eo_number, ET_Gender eo_gender);
    HRESULT h_GetStressTypes (ET_Number eo_number, 
                              ET_Gender eo_gender, 
                              vector<ET_EndingStressType>& vec_eoStressTypes);
    HRESULT h_CreateWordForm (ET_Number eo_number, 
                              ET_Gender eo_gender, 
                              ET_EndingStressType eo_stressType,
                              const wstring& str_ending,
                              wstring& str_lemma);
    HRESULT h_Build();
};
