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
                           ET_Subparadigm eo_subparadigm) 
        : CT_FormBuilderBaseDecl (pco_lexeme),
          str_Lemma (str_lemma),
          eo_AccentType (eo_accentType),
          eo_Subparadigm (eo_subparadigm)
    {
        pco_Endings = new CT_AdjLongEndings();
    }

public:
    HRESULT h_GetEndings();
    HRESULT h_Build();

private:
    wstring str_Lemma;
    ET_AccentType eo_AccentType;
    ET_Subparadigm eo_Subparadigm;
};
