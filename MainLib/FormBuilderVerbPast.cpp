#include "StdAfx.h"
#include "FormBuilderVerbPast.h"

static wchar_t arr_endings[][4] = { L"л", L"ла", L"ло", L"ли" };

// &&&& TODO -- standard deviations not handled
HRESULT CT_FormBuilderPast::h_GetEnding (CT_ExtString& xstr_lemma, 
                                         ET_Number eo_number, 
                                         ET_Gender eo_gender, 
                                         CT_ExtString& xstr_ending)
{
    HRESULT h_r = S_OK;

    if (NUM_SG == eo_number)
    {
        if (GENDER_M == eo_gender && xstr_lemma.b_EndsWithOneOf (str_Consonants))
        {
            xstr_ending = L"";
        }
        else
        {
            xstr_ending = arr_endings[eo_gender-1];
        }
    }
    else
    {
        xstr_ending = arr_endings[3];
    }

    return S_OK;

}   //  h_GetEnding (...)

HRESULT CT_FormBuilderPast::h_GetStressPositions (const CT_ExtString& xstr_lemma,
                                                  const wstring& str_ending,
                                                  ET_Number eo_number, 
                                                  ET_Gender eo_gender, 
                                                  vector<int>& vec_iStressPositions)
{
    HRESULT h_r = S_OK;

    ET_AccentType eo_accentType = AT_UNDEFINED;
    if (AT_UNDEFINED == pco_Lexeme->eo_AccentType2)
    {
        eo_accentType = AT_A;
    }
    else
    {
        eo_accentType = pco_Lexeme->eo_AccentType2;
    }

    vector<ET_StressLocation> vec_eoStressType;
    h_r = h_GetPastTenseStressTypes (eo_accentType, eo_number, eo_gender, vec_eoStressType);
    if (S_OK != h_r)
    {
        return h_r;
    }

    vector<ET_StressLocation>::iterator it_stressType = vec_eoStressType.begin();
    for (; it_stressType != vec_eoStressType.end(); ++it_stressType)
    {
        if (STRESS_LOCATION_STEM == *it_stressType)
        {
            h_r = h_GetStemStressPositions (xstr_lemma, vec_iStressPositions);
            if (S_OK != h_r)
            {
                return h_r;
            }
        }

        if (STRESS_LOCATION_ENDING == *it_stressType)
        {
            int i_pos = -1;
            h_r = h_GetEndingStressPosition (xstr_lemma, str_ending, i_pos);
            vec_iStressPositions.push_back (i_pos);
        }
    }

    return S_OK;

}   //  h_GetStressPositions (...)

HRESULT CT_FormBuilderPast::h_CreateFormTemplate (const CT_ExtString& xstr_lemma,
                                                  const CT_ExtString& xstr_ending,
                                                  ET_Number eo_number, 
                                                  ET_Gender eo_gender, 
                                                  CComObject<CT_WordForm> *& pco_wordForm)
{
    HRESULT h_r = S_OK;

    h_r = CComObject<CT_WordForm>::CreateInstance (&pco_wordForm);
    if (S_OK != h_r)
    {
        CString cs_msg;
        cs_msg.Format (L"CreateInstance() failed, error %x.", h_r);
        ERROR_LOG ((LPCTSTR)cs_msg);
        return h_r;
    }

    pco_wordForm->eo_POS = pco_Lexeme->eo_PartOfSpeech;
    pco_wordForm->eo_Subparadigm = SUBPARADIGM_PAST_TENSE;
    pco_wordForm->eo_Reflexive = pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO;
    pco_wordForm->str_Lemma = xstr_lemma;
    pco_wordForm->str_WordForm = xstr_lemma + xstr_ending;
    pco_wordForm->eo_Gender = eo_gender;
    pco_wordForm->eo_Number = eo_number;

    return h_r;

}   //  h_CreateFormTemplate (...)

HRESULT CT_FormBuilderPast::h_Build()
{
    HRESULT h_r = S_OK;

    vector<int>& vec_sourceStressPos = pco_Lexeme->vec_SourceStressPos;
 
    CT_ExtString xstr_lemma;
    h_r = h_BuildPastTenseStem (xstr_lemma);
    ET_AccentType eo_at = AT_UNDEFINED;
    if (AT_UNDEFINED == pco_Lexeme->eo_AccentType2)
    {
        eo_at = AT_A;
    }
    else
    {
        eo_at = pco_Lexeme->eo_AccentType2;
    }

    CT_ExtString xstr_ending;
    for (ET_Number eo_number = NUM_SG; eo_number < NUM_COUNT; ++eo_number)
        for (ET_Gender eo_gender = GENDER_M; eo_gender < GENDER_COUNT; ++eo_gender)
        {
            h_r = h_GetEnding (xstr_lemma, eo_number, eo_gender, xstr_ending);

            if (pco_Lexeme->b_Reflexive)
            {
                if (xstr_ending.b_EndsWithOneOf (str_Vowels))
                {
                    xstr_ending += L"сь";
                }
                else
                {
                    xstr_ending += L"ся";
                }
            }

            vector<int> vec_stressPositions;
            h_r = h_GetStressPositions (xstr_lemma, xstr_ending, eo_number, eo_gender, vec_stressPositions);
            if (S_OK != h_r)
            {
                return h_r;
            }

            vector<int>::iterator it_stress = vec_stressPositions.begin();
            for (; it_stress != vec_stressPositions.end(); ++it_stress)
            {
                CComObject<CT_WordForm> * pco_wordForm = NULL;
                h_r = h_CreateFormTemplate (xstr_lemma, xstr_ending, eo_number, eo_gender, pco_wordForm);
                if (S_OK != h_r)
                {
                    return h_r;
                }

                h_r = h_HandleYoAlternation (*it_stress, SUBPARADIGM_PAST_TENSE, xstr_lemma);
                if (S_OK != h_r)
                {
                    return h_r;
                }

                pco_wordForm->str_Lemma = xstr_lemma;
                pco_wordForm->str_WordForm = xstr_lemma + xstr_ending;
                pco_wordForm->vec_Stress.push_back (*it_stress);

                long l_gramHash = 0;
                pco_wordForm->get_GramHash (&l_gramHash);
                pco_Lexeme->v_AddWordForm (l_gramHash, pco_wordForm);

            }
        }   // for ...(ET_Number / ET_Gender ... )

    return h_r;

}   //  h_Build()
