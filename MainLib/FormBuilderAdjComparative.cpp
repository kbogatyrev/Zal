#include "StdAfx.h"
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

        CComObject<CT_WordForm> * pco_wordForm;
        h_r = CComObject<CT_WordForm>::CreateInstance (&pco_wordForm);
        if (S_OK != h_r)
        {
            return h_r;
        }

        pco_wordForm->eo_POS = pco_Lexeme->eo_PartOfSpeech;
        pco_wordForm->eo_Subparadigm = SUBPARADIGM_COMPARATIVE;
        pco_wordForm->str_Lemma = str_lemma;
        pco_wordForm->str_WordForm = str_lemma + L"е";

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

        long l_gramHash = 0;
        pco_wordForm->get_GramHash (&l_gramHash);
        pco_Lexeme->v_AddWordForm (l_gramHash, pco_wordForm);
    }
    else
    {
        CComObject<CT_WordForm> * pco_wordForm1;
        h_r = CComObject<CT_WordForm>::CreateInstance (&pco_wordForm1);
        if (S_OK != h_r)
        {
            return h_r;
        }
        CComObject<CT_WordForm> * pco_wordForm2;
        h_r = CComObject<CT_WordForm>::CreateInstance (&pco_wordForm2);
        if (S_OK != h_r)
        {
            return h_r;
        }

        vector<int> vec_iStressPos;
        if ((AT_A == pco_Lexeme->eo_AccentType1) && (AT_UNDEFINED == pco_Lexeme->eo_AccentType2))
        {
            h_r = h_GetStemStressPositions (str_lemma, SUBPARADIGM_COMPARATIVE, vec_iStressPos);
            if (S_OK != h_r )
            {
                ATLASSERT(0);
                ERROR_LOG (L"h_GetDeclStemStressPosition() failed");
                return E_FAIL;
           }
        }
        else
        {
            vec_iStressPos.push_back ((int)str_lemma.length());
        }

        pco_wordForm1->eo_POS = pco_Lexeme->eo_PartOfSpeech;
        pco_wordForm1->eo_Subparadigm = SUBPARADIGM_COMPARATIVE;
        pco_wordForm1->str_Lemma = str_lemma;
        pco_wordForm1->str_WordForm = str_lemma + L"ей";
        pco_wordForm1->vec_Stress = vec_iStressPos;

//        pco_Lexeme->m_coll.push_back (pco_wordForm1);
        long l_gramHash = 0;
        pco_wordForm1->get_GramHash (&l_gramHash);
        pco_Lexeme->v_AddWordForm (l_gramHash, pco_wordForm1);

        pco_wordForm1->eo_POS = pco_Lexeme->eo_PartOfSpeech;
        pco_wordForm2->eo_Subparadigm = SUBPARADIGM_COMPARATIVE;
        pco_wordForm2->str_Lemma = str_lemma;
        pco_wordForm2->str_WordForm = str_lemma + L"ее";
        pco_wordForm2->vec_Stress = vec_iStressPos;

//        pco_Lexeme->m_coll.push_back (pco_wordForm2);
        pco_wordForm2->get_GramHash (&l_gramHash);   // should be the same?
        pco_Lexeme->v_AddWordForm (l_gramHash, pco_wordForm2);
    }

    return S_OK;

}   //  h_Build()
