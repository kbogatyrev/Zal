#include "StdAfx.h"
#include "FormBuilderBaseConj.h"

HRESULT CT_FormBuilderBaseConj::h_GetPastTenseStressTypes (ET_AccentType eo_accentType,
                                                           ET_Number eo_number, 
                                                           ET_Gender eo_gender,
                                                           vector<ET_StressLocation>& vec_eoStressType)
{
    HRESULT h_r = S_OK;

    if (POS_VERB != pco_Lexeme->eo_PartOfSpeech)
    {
        ATLASSERT(0);
        wstring str_msg (L"Unexpected part of speech value.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_FAIL, str_msg);
    }

    vector<ET_StressLocation> vec_stress;
    switch (eo_accentType)
    {
        case AT_A:
        {
            vec_eoStressType.push_back (STRESS_LOCATION_STEM);
            break;
        }
        case AT_B:
        {
            vec_eoStressType.push_back (STRESS_LOCATION_ENDING);
            break;
        }
        case AT_C:
        {
            if (NUM_SG == eo_number && GENDER_N == eo_gender)
            {
                vec_eoStressType.push_back (STRESS_LOCATION_STEM);
            }
            else if (NUM_PL == eo_number)
            {
                vec_eoStressType.push_back (STRESS_LOCATION_STEM);
                break;
            }
            else
            {
                vec_eoStressType.push_back (STRESS_LOCATION_ENDING);
            }
            break;
        }
        case AT_C2:
        {
            if (!pco_Lexeme->b_Reflexive)
            {
                ATLASSERT(0);
                wstring str_msg (L"Unexpected part of speech value.");
                ERROR_LOG (str_msg);
                throw CT_Exception (E_FAIL, str_msg);
            }
            else if (!(GENDER_F == eo_gender && NUM_SG == eo_number))
            {
                vec_eoStressType.push_back (STRESS_LOCATION_STEM);
                vec_eoStressType.push_back (STRESS_LOCATION_ENDING);
            }
            else
            {
                vec_eoStressType.push_back (STRESS_LOCATION_ENDING);
            }
            break;
        }
        default:
        {
            ATLASSERT(0);
            wstring str_msg (L"Unexpected past tense accent type.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_FAIL, str_msg);
        }

    }   // switch (eo_accentType)

    return S_OK;

}   //  h_GetPastTenseStressType (...)

HRESULT CT_FormBuilderBaseConj::h_GetStemStressPositions (const wstring& str_lemma, 
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
            ATLASSERT(0);
            wstring str_msg (L"Illegal source stress position.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_FAIL, str_msg);
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
            ATLASSERT(0);
            i_stressPos = -1;
            wstring str_msg (L"Illegal source stress position.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_FAIL, str_msg);
        }

        CT_ExtString xstr_lemma (str_lemma);
        xstr_lemma.v_SetVowels (str_Vowels);
        i_syllables = xstr_lemma.i_NSyllables();
        if (i_syllables < 1)
        {
            ATLASSERT(0);
            wstring str_msg (L"No vowels in verb stem.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_FAIL, str_msg);
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

}   //  h_GetStemStressPositions (...)

HRESULT CT_FormBuilderBaseConj::h_GetEndingStressPosition (const wstring& str_lemma, 
                                                           const wstring& str_ending,
                                                           int& i_position)
{
   HRESULT h_r = S_OK;

    if (POS_VERB != pco_Lexeme->eo_PartOfSpeech)
    {
        ATLASSERT(0);
        wstring str_msg (L"Unexpected part of speech.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_UNEXPECTED, str_msg);
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
            ATLASSERT(0);
            i_position = -1;
            wstring str_msg (L"Warning: can't find stressed vowel.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_FAIL, str_msg);
        }
// &&&& TODO Past masc. + -sya + c'': sorvalsyA
    }

    return h_r;

}   //  h_GetEndingStressPosition (...)

HRESULT CT_FormBuilderBaseConj::h_BuildPastTenseStem (wstring& str_lemma)
{
    HRESULT h_r = S_OK;

    CT_ExtString xstr_lemma;

    int i_type = pco_Lexeme->i_Type;
    if (7 == i_type || 8 == i_type)
    {
        xstr_lemma = pco_Lexeme->xstr_1SgStem;
        if (xstr_lemma.b_EndsWithOneOf (L"тд"))
        {
            xstr_lemma = xstr_lemma.erase (xstr_lemma.length()-1);
        }
    }
    else
    {
        xstr_lemma = pco_Lexeme->xstr_InfStem;
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

    str_lemma = xstr_lemma.c_str();

    return h_r;

}   //  h_BuildPastTenseStem (...)

HRESULT CT_FormBuilderBaseConj::h_HandleYoAlternation (int i_stressPos,
                                                       ET_Subparadigm eo_subParadigm, 
                                                       wstring& str_lemma)
{
    HRESULT h_r = S_OK;

    if (!pco_Lexeme->b_YoAlternation)
    {
        return S_OK;
    }

    if (SUBPARADIGM_PAST_TENSE != eo_subParadigm && 
        SUBPARADIGM_PARTICIPLE_PAST_ACTIVE != eo_subParadigm &&
        SUBPARADIGM_PARTICIPLE_PAST_PASSIVE != eo_subParadigm &&
        SUBPARADIGM_ADVERBIAL_PAST != eo_subParadigm &&
        SUBPARADIGM_PARTICIPLE_PRESENT_PASSIVE != eo_subParadigm)
    {
        return S_OK;
    }

    if (SUBPARADIGM_PARTICIPLE_PAST_ACTIVE == eo_subParadigm)
    {
        ATLASSERT (7 == pco_Lexeme->i_Type &&                  // GDRL, p. 85
        CT_ExtString (pco_Lexeme->str_SourceForm).b_EndsWith (L"сти") &&
        (L"т" == pco_Lexeme->str_VerbStemAlternation || 
         L"д" == pco_Lexeme->str_VerbStemAlternation));
    }

    int i_eOffset = str_lemma.rfind (_T("е"));    // last "e" in graphic stem (?)
    if (wstring::npos == i_eOffset)
    {
        ATLASSERT(0);
        wstring str_msg (L"Unstressed stem with yo alternation has no e.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_FAIL, str_msg);
    }

    if (i_stressPos == i_eOffset)
    {
        str_lemma[i_stressPos] = L'ё';
    }

    return S_OK;

}   //  h_HandleYoAlternation (...)
