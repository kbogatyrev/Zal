#include "StdAfx.h"
#include "Endings.h"
#include "FormBuilderAdjComparative.h"

HRESULT CT_FormBuilderComparative::h_Build()
{
    HRESULT h_r = S_OK;

    CT_ExtString xstr_graphicStem (pco_Lexeme->str_GraphicStem);
    wstring str_lemma (pco_Lexeme->str_GraphicStem);

    if (xstr_graphicStem.b_EndsWithOneOf (L"кгх"))
    {
        if (xstr_graphicStem.b_EndsWith (L"к"))
        {
            str_lemma[str_lemma.length()-1] = L'ч';
        }
        if (xstr_graphicStem.b_EndsWith (L"г"))
        {
            str_lemma[str_lemma.length()-1] = L'ж';
        }
        if (xstr_graphicStem.b_EndsWith (L"х"))
        {
            str_lemma[str_lemma.length()-1] = L'ш';
        }

        CComObject<CT_WordForm> * sp_wordForm;
        h_r = CComObject<CT_WordForm>::CreateInstance (&sp_wordForm);
        if (S_OK != h_r)
        {
            return h_r;
        }

        sp_wordForm->eo_POS = pco_Lexeme->eo_PartOfSpeech;
        sp_wordForm->eo_Subparadigm = SUBPARADIGM_COMPARATIVE;
        sp_wordForm->str_Lemma = str_lemma;
        sp_wordForm->str_WordForm = str_lemma + L"е";

        int i_lastStemVowel = str_lemma.find_last_of (str_Vowels);
        if (wstring::npos != i_lastStemVowel)
        {
//            sp_wordForm->i_Stress = i_lastStemVowel;
        }
        else
        {
            ERROR_LOG (L"Warning: can't find stressed vowel in comparative.");
//            sp_wordForm->i_Stress = -1;
        }

        pco_Lexeme->m_coll.push_back (sp_wordForm);
    }
    else
    {
        CComObject<CT_WordForm> * sp_wordForm1;
        h_r = CComObject<CT_WordForm>::CreateInstance (&sp_wordForm1);
        if (S_OK != h_r)
        {
            return h_r;
        }
        CComObject<CT_WordForm> * sp_wordForm2;
        h_r = CComObject<CT_WordForm>::CreateInstance (&sp_wordForm2);
        if (S_OK != h_r)
        {
            return h_r;
        }

        int i_stressPos = -1;
        if ((AT_A == pco_Lexeme->eo_AccentType1) && (AT_UNDEFINED == pco_Lexeme->eo_AccentType2))
        {
            i_stressPos = -1;
//            h_r = h_GetDeclStemStressPosition (str_lemma, i_stressPos, SUBPARADIGM_COMPARATIVE);
//            if (S_OK != h_r || i_stressPos < 0 || i_stressPos >= (int)str_lemma.length())
//            {
//                ATLASSERT(0);
//                ERROR_LOG (L"h_GetDeclStemStressPosition() failed");
//                return E_FAIL;
 //           }
        }
        else
        {
            i_stressPos = str_lemma.length();
        }

        sp_wordForm1->eo_POS = pco_Lexeme->eo_PartOfSpeech;
        sp_wordForm1->eo_Subparadigm = SUBPARADIGM_COMPARATIVE;
        sp_wordForm1->str_Lemma = str_lemma;
        sp_wordForm1->str_WordForm = str_lemma + L"ей";
//        sp_wordForm1->i_Stress = i_stressPos;

        pco_Lexeme->m_coll.push_back (sp_wordForm1);

        sp_wordForm1->eo_POS = pco_Lexeme->eo_PartOfSpeech;
        sp_wordForm2->eo_Subparadigm = SUBPARADIGM_COMPARATIVE;
        sp_wordForm2->str_Lemma = str_lemma;
        sp_wordForm2->str_WordForm = str_lemma + L"ее";
//        sp_wordForm2->i_Stress = i_stressPos;

        pco_Lexeme->m_coll.push_back (sp_wordForm2);
    }

    return S_OK;

}   //  h_Build()
