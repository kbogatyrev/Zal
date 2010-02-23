#include "StdAfx.h"
#include "FormBuilderBaseConj.h"

HRESULT CT_FormBuilderBaseConj::h_StressOnPastTenseEnding (ET_AccentType eo_accentType,
                                                           ET_Number eo_number, 
                                                           ET_Gender eo_gender)
{
    HRESULT h_r = S_OK;

    if (POS_VERB != pco_Lexeme->eo_PartOfSpeech)
    {
        ATLASSERT(0);
        ERROR_LOG (L"Unexpected part of speech value.");
        return E_FAIL;
    }

    vector<ET_EndingStressType> vec_stress;
    switch (eo_accentType)
    {
        case AT_A:
        {
            return S_FALSE;
        }
        case AT_B:
        {
            return S_TRUE;
        }
        case AT_C:
        {
            if (NUM_SG == eo_number && GENDER_N == eo_gender)
            {
                return S_FALSE;
            }
            if (NUM_PL == eo_number)
            {
                return S_FALSE;
            }
            return S_TRUE;
        }
        case AT_C2:
        {
            if (!pco_Lexeme->b_Reflexive)
            {
                ATLASSERT(0);
                ERROR_LOG (L"Accent type C2 with a non-reflexive verb.");
                return h_r;
            }
            if (!(GENDER_F == eo_gender && NUM_SG == eo_number))
            {
                return S_BOTH;
            }
            return S_TRUE;
        }
        default:
        {
            ERROR_LOG (L"Unexpected past tense accent type.");
            return h_r;
        }

        return E_FAIL;

    }   // switch (eo_accentType)

}   //  h_StressOnPastTenseEnding (...)

HRESULT CT_FormBuilderBaseConj::h_GetStemStressPosition (const wstring& str_lemma, 
                                                         vector<int>& vec_iPosition)
{
    //
    // Find the sequence # of the stressed vowel in infinitive
    //
    vector<int>& vec_sourceStressPos = pco_Lexeme->vec_SourceStressPos;

    vector<int>::iterator it_sourceStressPos = vec_sourceStressPos.begin();
    for (; it_sourceStressPos != vec_sourceStressPos.end(); ++it_sourceStressPos)
    {
        int i_stressPos = -1;
        if (*it_sourceStressPos >= (int)pco_Lexeme->str_SourceForm.length())
        {
            ATLASSERT (0);
            ERROR_LOG (L"Illegal source stress position.");
            return E_FAIL;
        }

        CT_ExtString xstr_infinitive (pco_Lexeme->str_SourceForm);
        xstr_infinitive.v_SetVowels (str_Vowels);
        int i_syllables = xstr_infinitive.i_NSyllables();
        int i_stressedSyllable = 0;
        for (; i_stressedSyllable < i_syllables; ++i_stressedSyllable)
        {
            i_stressPos = xstr_infinitive.i_GetVowelPos (i_stressedSyllable);
            if (*it_sourceStressPos == i_stressPos)
            {
                break;
            }
        }

        if (i_stressedSyllable == i_syllables)
        {
            ATLASSERT (0);
            i_stressPos = -1;
            ERROR_LOG (L"Illegal source stress position.");
            return E_FAIL;
        }

        CT_ExtString xstr_lemma (str_lemma);
        xstr_lemma.v_SetVowels (str_Vowels);
        i_syllables = xstr_lemma.i_NSyllables();
        if (i_syllables < 1)
        {
            ATLASSERT (0);
            i_stressPos = -1;
            ERROR_LOG (L"No vowels in verb stem.");
            return E_FAIL;
        }

        if (i_stressedSyllable > i_syllables)
        {
            i_stressPos = xstr_lemma.i_GetVowelPos (i_syllables-1);  // last syllable
        }
        else
        {
            i_stressPos = xstr_lemma.i_GetVowelPos (i_stressedSyllable); // same syll as in infinitive
        }

        vec_iPosition.push_back (i_stressPos);

    }   //  for (; it_sourceStressPos != vec_SourceStressPos.end(); ... )

    return S_OK;

}   //  h_GetStemStressPosition (...)

HRESULT CT_FormBuilderBaseConj::h_GetEndingStressPosition (const wstring& str_lemma, 
                                                           const wstring& str_ending,
                                                           int& i_position)
{
   HRESULT h_r = S_OK;

    if (POS_VERB != pco_Lexeme->eo_PartOfSpeech)
    {
        ATLASSERT (0);
        ERROR_LOG (L"Unexpected part of speech.");
        return E_UNEXPECTED;
    }

    int i_endingVowel = str_ending.find_first_of (str_Vowels);
    if (wstring::npos != i_endingVowel)
    {
        i_position = str_lemma.length() + i_endingVowel;
    }
    else
    {
        int i_position = str_lemma.find_last_of (str_Vowels);
        if (wstring::npos == i_position)
        {
            ERROR_LOG (L"Warning: can't find stressed vowel.");
            i_position = -1;
            return E_FAIL;                        
        }
// &&&& Past masc. + -sya + c'': sorvalsyA
    }

    return h_r;

}   //  h_GetEndingStressPosition (...)

HRESULT CT_FormBuilderBaseConj::h_BuildPastTenseStem (CT_ExtString& xstr_lemma)
{
    HRESULT h_r = S_OK;

    int i_type = pco_Lexeme->i_Type;
    CT_ExtString& xstr_1SgStem = pco_Lexeme->xstr_1SgStem;
    CT_ExtString& xstr_infStem = pco_Lexeme->xstr_InfStem;

    if (7 == i_type || 8 == i_type)
    {
        xstr_lemma = xstr_1SgStem;
        if (xstr_lemma.b_EndsWithOneOf (L"тд"))
        {
            xstr_lemma = xstr_lemma.erase (xstr_lemma.length()-1);
        }
    }
    else
    {
        xstr_lemma = xstr_infStem;
    }
    if (9 == i_type)
    {
        ATLASSERT (xstr_lemma.b_EndsWith (L"е"));
        xstr_lemma = xstr_lemma.erase (xstr_lemma.length()-1);
    }
    if (3 == i_type && pco_Lexeme->b_StemAugment)
    {
        ATLASSERT (xstr_lemma.b_EndsWith (L"ну"));
        xstr_lemma = xstr_lemma.erase (xstr_lemma.length()-2);
    }

    return h_r;

}   //  h_BuildPastTenseStem (...)
