#pragma once
#include "WordForm.h"
#include "SqliteWrapper.h"
#include "Endings.h"
#include "FormBuilderBaseDecl.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderNouns : public CT_FormBuilderBaseDecl
{
public:
    CT_FormBuilderNouns (CT_Lexeme * pco_lexeme) : CT_FormBuilderBaseDecl (pco_lexeme)
    {
        pco_Endings = new CT_NounEndings();
    }

public:
    HRESULT h_GetEndings();
    HRESULT h_HandleStemAugment (wstring& str_lemma);
    HRESULT h_GetStressType (ET_Number eo_number,                 // in
                             ET_Case eo_case,                     // in
                             ET_EndingStressType& eo_stressType); // out
    HRESULT h_HandleAccEnding (ET_Number, ET_Case&);
    HRESULT h_GetStressPositions (const wstring& str_lemma, 
                                  const wstring& str_ending,
                                  ET_EndingStressType eo_stressType,
                                  vector<int>& vec_iStressPos);
    HRESULT h_CreateFormTemplate (ET_Number, ET_Case, CComObject<CT_WordForm> *&);
    HRESULT h_Build();

};
