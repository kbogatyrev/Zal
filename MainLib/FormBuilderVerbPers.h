#pragma once
#include "FormBuilderBaseConj.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderPersonal : public CT_FormBuilderBaseConj
{
public:
    CT_FormBuilderPersonal (CT_Lexeme * pco_lexeme) : CT_FormBuilderBaseConj (pco_lexeme)
    {
        pco_Endings = new CT_PersonalEndings();
    }

public:
    HRESULT h_GetEndings();
    HRESULT h_GetStressType (ET_Number, ET_Person,  ET_StressLocation&);
    HRESULT h_GetAuslautType (ET_Number, ET_Person, ET_StemAuslaut&);
    HRESULT h_GetLemma (ET_Number, ET_Person, CT_ExtString&);
    HRESULT h_GetStressPositions (const CT_ExtString& xstr_lemma,
                                  const wstring& str_ending,
                                  ET_StressLocation eo_stressType,
                                  vector<int>& vec_iStressPositions);
    HRESULT h_HandleFleetingVowel (CT_ExtString& xstr_verbForm);
    HRESULT h_CreateFormTemplate (const CT_ExtString& xstr_lemma,
                                  const CT_ExtString& xstr_wordForm,
                                  ET_Number eo_number, 
                                  ET_Person eo_person,
                                  CComObject<CT_WordForm> *& pco_wordForm);
    HRESULT h_Build();
};
