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
    HRESULT hGetEndings();
    HRESULT hGetStressType (ET_Number, ET_Person,  ET_StressLocation&);
    HRESULT hGetAuslautType (ET_Number, ET_Person, ET_StemAuslaut&);
    HRESULT hGetLemma (ET_Number, ET_Person, CEString&);
    HRESULT hGetStressPositions (const CEString& sLemma,
                                 const CEString& sEnding,
                                 ET_StressLocation eStressType,
                                 vector<int>& vecStressPositions);
//    HRESULT h_FleetingVowelCheck (wstring& str_verbForm);
    HRESULT hCreateFormTemplate (ET_Number eNumber, 
                                 ET_Person ePerson,
                                 CComObject<CWordForm> *& pWordForm);
    HRESULT hBuild();
    HRESULT hBuildIrregular();
    HRESULT hBuildIrregularForm (ET_Number, ET_Person, StIrregularForm& itIf);

};
