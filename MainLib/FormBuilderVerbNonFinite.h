#pragma once
#include "resource.h"       // main symbols
#include "MainLib_i.h"
#include "WordForm.h"
#include "SqliteWrapper.h"
#include "Endings.h"
#include "FormBuilderBaseConj.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderNonFinite : public CT_FormBuilderBaseConj
{
public:
    CT_FormBuilderNonFinite (CT_Lexeme * pco_lexeme) : 
        CT_FormBuilderBaseConj (pco_lexeme, SUBPARADIGM_UNDEFINED)
    {}

public:
    HRESULT h_Build();

private:
    HRESULT h_BuildInfinitive();
    HRESULT h_BuildPresentActiveParticiple();
    HRESULT h_BuildPresentAdverbial();
    HRESULT h_BuildPresentPassiveParticiple();
    HRESULT h_BuildPastActiveParticiple();
    HRESULT h_BuildPastAdverbial();
    HRESULT h_BuildPastPassiveParticiple();

    void v_GetParticipleStressPos (ET_Subparadigm eo_subparadigm, vector<int>& vec_positions);

    int i_StressPosFromTemplate (const CT_WordForm& co_template, const wstring& str_lemma, int i_at = 0);

    HRESULT h_PresAdvType13();
    HRESULT h_PresAdvGeneral();

    //
    // Regular derivation of off irregularly derived stem
    //
    HRESULT h_BuildIrregPresAdverbial();
    HRESULT h_BuildIrregPresActiveParticiple();
    HRESULT h_BuildIrregPresPassiveParticiple();

    HRESULT h_BuildIrregPastActPartAndAdverbial();
    HRESULT h_BuildIrregPastPassiveParticiple();

    //
    // Irregularly formed participle (all types)
    //
    HRESULT h_BuildIrregParticipialFormsLong (ET_Subparadigm eo_sp);

};
