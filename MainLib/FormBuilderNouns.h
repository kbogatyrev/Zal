#pragma once
#include "resource.h"       // main symbols
#include "MainLib_i.h"
#include "WordForm.h"
#include "SqliteWrapper.h"
#include "Endings.h"
#include "FormBuilderBase.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderNouns : public CT_FormBuilderBase
{
public:
    CT_FormBuilderNouns (CT_Lexeme * pco_lexeme) : CT_FormBuilderBase (pco_lexeme)
    {
        pco_Endings = new CT_NounEndings();
    }

public:
    HRESULT h_GetEndings();
    HRESULT h_HandleStemAugment (wstring& str_lemma);
    HRESULT h_GetStressType (ET_Number eo_number,                 // in
                             ET_Case eo_case,                     // in
                             ET_EndingStressType& eo_stressType); // out
    HRESULT h_Build();

};
