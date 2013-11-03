#pragma once
#include "FormBuilderBaseConj.h"

using namespace std;
using namespace std::tr1;

class CFormBuilderPersonal : public CFormBuilderBaseConj
{
public:
    CFormBuilderPersonal (CLexeme * pLexeme) : 
        CFormBuilderBaseConj (pLexeme, SUBPARADIGM_PRESENT_TENSE)
    {
        m_pEndings = new CPersonalEndings();
    }

public:
    ET_ReturnCode eGetEndings();
    ET_ReturnCode eGetStressType (ET_Number, ET_Person,  ET_StressLocation&);
    ET_ReturnCode eGetAuslautType (ET_Number, ET_Person, ET_StemAuslaut&);
    ET_ReturnCode eGetLemma (ET_Number, ET_Person, CEString&);
    ET_ReturnCode eGetStressPositions (const CEString& sLemma,
                                 const CEString& sEnding,
                                 ET_StressLocation eStressType,
                                 vector<int>& vecStressPositions);
//    ET_ReturnCode h_FleetingVowelCheck (wstring& str_verbForm);
    ET_ReturnCode eCreateFormTemplate (ET_Number, ET_Person, CWordForm *& pWordForm);
    ET_ReturnCode eBuild();
    ET_ReturnCode eHandleIrregularForms(); // get from the dictionary or generate if abbreviated paradigm was used
    ET_ReturnCode eBuildIrregularForms (ET_Number, ET_Person); // ... when the dictionary only provides base forms

};
