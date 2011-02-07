#pragma once
#include "WordForm.h"
#include "FormBuilderBaseDecl.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderPronounAdj : public CT_FormBuilderBaseDecl
{
public:
    CT_FormBuilderPronounAdj (CT_Lexeme * pco_lexeme, 
                              const wstring& str_lemma,
                              ET_AccentType eo_accentType) :
        CT_FormBuilderBaseDecl (pco_lexeme, SUBPARADIGM_LONG_ADJ), 
            str_Lemma (str_lemma),
            eo_AccentType (eo_accentType)
    {
        pco_Endings = new CT_AdjLongEndings();
    }

protected:
    int i_GramHash (ET_PartOfSpeech eo_pos, ET_Case eo_case, ET_Number eo_num,
                    ET_Gender eo_gender, ET_Animacy eo_animacy);

    HRESULT h_GetEndings();

    HRESULT h_GetStressPositions (const wstring&,
                                  ET_StressLocation,
                                  vector<int>& vec_iStressPos);

    HRESULT h_CreateFormTemplate (ET_Gender, 
                                  ET_Number, 
                                  ET_Case, 
                                  ET_Animacy, 
                                  const wstring& str_ending,
                                  CComObject<CT_WordForm> *&); 

    HRESULT h_HandleCommonDeviations (CT_WordForm *);

public:
    HRESULT h_Build();

private:
    wstring str_Lemma;
    ET_AccentType eo_AccentType;
    ET_Subparadigm eo_Subparadigm;
    ET_Status eo_Status;
    bool b_Irregular;
};
