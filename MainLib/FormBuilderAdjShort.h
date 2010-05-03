#pragma once
#include "FormBuilderBaseDecl.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderShortAdj : public CT_FormBuilderBaseDecl
{
public:

    //
    // Use with adjectives
    //
    CT_FormBuilderShortAdj (CT_Lexeme * pco_lexeme) : 
        CT_FormBuilderBaseDecl (pco_lexeme, SUBPARADIGM_SHORT_ADJ),
        str_Lemma (pco_lexeme->xstr_GraphicStem.c_str()),
        i_Type (pco_lexeme->i_Type),
        eo_AccentType1 (pco_Lexeme->eo_AccentType1),
        eo_AccentType2 (pco_Lexeme->eo_AccentType2),
        b_FleetingVowel (pco_Lexeme->b_FleetingVowel),
        map_CommonDeviations (pco_lexeme->map_CommonDeviations)
    {
        if (AT_UNDEFINED == eo_AccentType2)
        {
            switch (eo_AccentType1)
            {
                case AT_A:
                {
                    eo_AccentType2 = AT_A;
                    break;
                }
                case AT_A1:
                {
                    eo_AccentType2 = AT_A1;
                    break;
                }
                case AT_B:
                {
                    eo_AccentType2 = AT_B;
                    break;
                }
                default:
                {
                    ATLASSERT(0);
                    wstring str_msg (L"Accent type undefined for short forms.");
                    ERROR_LOG (str_msg);
                    throw CT_Exception (E_FAIL, str_msg);
                }
            }       // switch
        }
        pco_Endings = new CT_AdjShortEndings();
    }

    //
    // Use with participles
    //
    CT_FormBuilderShortAdj (CT_Lexeme * pco_lexeme, 
                            bool b_yoAlternation, 
                            const wstring& str_lemma,
                            ET_Subparadigm eo_subparadigm,
                            ET_AccentType eo_accentType1,
                            ET_AccentType eo_accentType2,
                            vector<int>& vec_iStressPos,
                            bool b_fleetingVowel,
                            int i_deviation = -1) : 
        CT_FormBuilderBaseDecl (pco_lexeme, eo_subparadigm, b_fleetingVowel, b_yoAlternation), 
        str_Lemma (str_lemma), 
        i_Type (1),
        vec_IStressPos (vec_iStressPos),
        eo_AccentType1 (eo_accentType1),
        eo_AccentType2 (eo_accentType2),
        b_FleetingVowel (b_fleetingVowel)
    {
        pco_Endings = new CT_AdjShortEndings();
        if (i_deviation > 0)
        {
            if (i_deviation != 2)
            {
                ATLASSERT(0);
                wstring str_msg (L"Unexpected common deviation in short participle.");
                ERROR_LOG (str_msg);
                throw CT_Exception (E_INVALIDARG, str_msg);
            }
            map_CommonDeviations[i_deviation] = false; // not optional
        }
    }

    HRESULT h_GetEndings();

    HRESULT h_StressOnEnding (ET_Number eo_number, ET_Gender eo_gender);

    HRESULT h_GetStressTypes (ET_Number eo_number, 
                              ET_Gender eo_gender, 
                              vector<ET_StressLocation>& vec_eoStressType);

    HRESULT h_CreateFormTemplate (const wstring& str_lemma,
                                  const wstring& str_ending,
                                  ET_Gender eo_gender,
                                  ET_Number eo_number,
                                  CComObject<CT_WordForm> *& pco_wordForm);

    HRESULT h_CreateWordForm (ET_Number eo_number, 
                              ET_Gender eo_gender, 
                              ET_StressLocation eo_stressType,
                              const wstring& str_ending,
                              wstring& str_lemma);

    HRESULT h_HandleDeviations (CT_WordForm *);

    HRESULT h_Build();

private:
    wstring str_Lemma;
    int i_Type;
    vector<int> vec_IStressPos;        // Participles only
    ET_AccentType eo_AccentType1, eo_AccentType2;
    bool b_FleetingVowel;
    map<int, bool> map_CommonDeviations;

};
