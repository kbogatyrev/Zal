#pragma once
#include "FormBuilderBaseConj.h"

using namespace std;
using namespace std::tr1;

class CFormBuilderPersonal : public CFormBuilderBaseConj
{
public:
    CFormBuilderPersonal (CLexeme * pco_lexeme) : 
        CFormBuilderBaseConj (pco_lexeme, SUBPARADIGM_PRESENT_TENSE)
    {
        pco_Endings = new CPersonalEndings();
    }

public:
    HRESULT hGetEndings();
    HRESULT h_GetStressType (ET_Number, ET_Person,  ET_StressLocation&);
    HRESULT h_GetAuslautType (ET_Number, ET_Person, ET_StemAuslaut&);
    HRESULT h_GetLemma (ET_Number, ET_Person, wstring&);
    HRESULT h_GetStressPositions (const wstring& str_lemma,
                                  const wstring& str_ending,
                                  ET_StressLocation eo_stressType,
                                  vector<int>& vec_iStressPositions);
//    HRESULT h_FleetingVowelCheck (wstring& str_verbForm);
    HRESULT h_CreateFormTemplate (ET_Number eo_number, 
                                  ET_Person eo_person,
                                  CComObject<CWordForm> *& pco_wordForm);
    HRESULT h_Build();
    HRESULT h_BuildIrregular();
    HRESULT h_BuildIrregularForm (ET_Number, ET_Person, StIrregularForm& it_if);

};
