#pragma once
#include "WordForm.h"
#include "SqliteWrapper.h"
#include "FormBuilderBaseConj.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderImperative : public CT_FormBuilderBaseConj
{
public:
    CT_FormBuilderImperative (CT_Lexeme * pco_lexeme) : CT_FormBuilderBaseConj (pco_lexeme)
    {}

public:
    HRESULT h_GetLemma (CT_ExtString& xstr_lemma);
    HRESULT h_GetEnding (CT_ExtString& xstr_lemma, wstring& str_ending);
    HRESULT h_GetStressPositions (CT_ExtString& xstr_lemma, 
                                  wstring& str_ending,
                                  vector<int>& vec_stressPositions);
    HRESULT h_Build();
};
