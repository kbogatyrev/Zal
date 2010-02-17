#pragma once
#include "resource.h"       // main symbols
#include "MainLib_i.h"
#include "WordForm.h"
#include "SqliteWrapper.h"
#include "Endings.h"
#include "FormBuilderBase.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderShortAdj : public CT_FormBuilderBase
{
public:
    CT_FormBuilderShortAdj (CT_Lexeme * pco_lexeme) : CT_FormBuilderBase (pco_Lexeme)
    {
        pco_Endings = new CT_AdjShortEndings();
    }

public:
    HRESULT h_GetEndings();
    HRESULT h_StressOnEnding (ET_Number eo_number, ET_Gender eo_gender);
    HRESULT h_CreateWordForm (ET_Number eo_number, 
                              ET_Gender eo_gender, 
                              ET_EndingStressType eo_stressType,
                              const wstring& str_ending,
                              wstring& str_lemma);
    HRESULT h_Build();
};
