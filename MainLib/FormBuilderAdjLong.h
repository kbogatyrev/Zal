#pragma once
#include "WordForm.h"
#include "FormBuilderBaseDecl.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderLongAdj : public CT_FormBuilderBaseDecl
{
public:
    CT_FormBuilderLongAdj (CT_Lexeme * pco_lexeme, 
                           const wstring& str_lemma,
                           ET_AccentType eo_accentType, 
                           ET_Subparadigm eo_subparadigm,
                           int i_stemStressPos = -1) 
        : CT_FormBuilderBaseDecl (pco_lexeme, SUBPARADIGM_LONG_ADJ),
          str_Lemma (str_lemma),
          eo_AccentType (eo_accentType),
          eo_Subparadigm (eo_subparadigm),
          i_StemStressPos (i_stemStressPos)
    {
        pco_Endings = new CT_AdjLongEndings();
    }

public:
    HRESULT h_GetEndings();
    HRESULT h_GetStressPositions (const wstring&,
                                  ET_StressLocation,
                                  vector<int>& vec_iStressPos);
    HRESULT h_CreateFormTemplate (ET_Gender, ET_Number, ET_Case, ET_Animacy, CComObject<CT_WordForm> *&);
    HRESULT h_HandleCommonDeviations (CT_WordForm *);
    HRESULT h_Build();
    HRESULT h_BuildParticiple();

private:
    wstring str_Lemma;
    ET_AccentType eo_AccentType;
    ET_Subparadigm eo_Subparadigm;
    int i_StemStressPos;        // participles only
};
