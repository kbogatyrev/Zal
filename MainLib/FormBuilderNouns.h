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
                             ET_StressLocation& eo_stressType);   // out

    HRESULT h_HandleAccEnding (ET_Number, ET_Case&);

    HRESULT h_GetStressPositions (const wstring& str_lemma, 
                                  const wstring& str_ending,
                                  ET_StressLocation eo_stressType,
								  bool b_hasFleetingVowel,
                                  vector<int>& vec_iStressPos);

    HRESULT h_CreateFormTemplate (ET_Number eo_number, 
                                  ET_Case eo_case, 
                                  const wstring& str_lemma, 
                                  CComObject<CT_WordForm> *&);

    HRESULT h_HandleStress (wstring& str_lemma, 
                            const wstring& str_ending, 
                            ET_StressLocation eo_stress, 
                            CComObject<CT_WordForm> * pco_wordForm);

    HRESULT h_Build();

};
