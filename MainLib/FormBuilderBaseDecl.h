#pragma once
#include "FormBuilderBase.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderBaseDecl : public CT_FormBuilderBase
{
public:
    CT_FormBuilderBaseDecl (CT_Lexeme * pco_lexeme) : 
        CT_FormBuilderBase (pco_lexeme),
        b_YoAlternation (pco_lexeme->b_YoAlternation),
        b_OAlternation (pco_Lexeme->b_OAlternation),
        b_ContainsFleetingVowel (false)
    {}

    CT_FormBuilderBaseDecl (CT_Lexeme * pco_lexeme, bool b_yoAlternation) : 
        CT_FormBuilderBase (pco_lexeme),
        b_YoAlternation (b_yoAlternation),
        b_OAlternation (pco_Lexeme->b_OAlternation),
        b_ContainsFleetingVowel (false)
    {}

protected:
    HRESULT h_FleetingVowelCheck (ET_Number eo_number,              // in
                                  ET_Case eo_case,                  // in
                                  ET_Gender eo_gender,              // in
                                  ET_StressLocation eo_stressType,  // in
                                  ET_Subparadigm eo_subparadigm,    // in
                                  wstring& str_ending,              // in/out (can be modified in forms
                                                                    // башен/древен, GDRL p. 30
                                  wstring& str_lemma);              // out

    HRESULT h_FleetingVowelAdd (ET_Number eo_number,                // in
                                ET_Case eo_case,                    // in
                                ET_Gender eo_gender,                // in
                                ET_StressLocation eo_stressType,    // in
                                ET_Subparadigm eo_subparadigm,      // in
                                wstring& str_lemma);                // out

    HRESULT h_FleetingVowelRemove (ET_Gender eo_gender,             // in
                                   const wstring& str_ending,       // in
                                   wstring& str_lemma);             // out

    HRESULT h_GetStemStressPositions (const wstring& str_lemma, 
                                      ET_Subparadigm eo_subParadigm,
                                      vector<int>& vec_iPositions);

    HRESULT h_GetEndingStressPosition (const wstring& str_lemma, 
                                       const wstring& str_ending,
                                       int& i_position);

    HRESULT h_HandleYoAlternation (ET_StressLocation eo_stressType, 
                                   int i_stressPos,
                                   ET_Subparadigm eo_subParadigm, 
                                   wstring& str_lemma);

protected:
    bool b_YoAlternation;
    bool b_OAlternation;
    bool b_ContainsFleetingVowel;

};
