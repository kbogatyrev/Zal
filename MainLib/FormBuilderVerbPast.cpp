#include "StdAfx.h"
#include "FormBuilderVerbPast.h"

static wchar_t arr_endings[][4] = { L"л", L"ла", L"ло", L"ли" };

// &&&& TODO -- standard deviations not handled
HRESULT CT_FormBuilderPast::h_GetEnding (wstring& str_lemma, 
                                         ET_Number eo_number, 
                                         ET_Gender eo_gender, 
                                         wstring& str_ending)
{
    HRESULT h_r = S_OK;

    if (NUM_SG == eo_number)
    {
        if (GENDER_M == eo_gender && CT_ExtString(str_lemma).b_EndsWithOneOf (str_Consonants))
        {
            str_ending = L"";
        }
        else
        {
            str_ending = arr_endings[eo_gender-1];
        }
    }
    else
    {
        str_ending = arr_endings[3];
    }

    return S_OK;

}   //  h_GetEnding (...)

HRESULT CT_FormBuilderPast::h_GetStressPositions (const wstring& str_lemma,
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
    h_GetPastTenseStressTypes (eo_accentType, eo_number, eo_gender, vec_eoStressType);

    vector<ET_StressLocation>::iterator it_stressType = vec_eoStressType.begin();
    for (; it_stressType != vec_eoStressType.end(); ++it_stressType)
    {
        if (STRESS_LOCATION_STEM == *it_stressType)
        {
            h_GetStemStressPositions (str_lemma, vec_iStressPositions);
        }

        if (STRESS_LOCATION_ENDING == *it_stressType)
        {
            int i_pos = -1;
            h_GetEndingStressPosition (str_lemma, str_ending, i_pos);
            vec_iStressPositions.push_back (i_pos);
        }
    }

    return S_OK;

}   //  h_GetStressPositions (...)

HRESULT CT_FormBuilderPast::h_CreateFormTemplate (const wstring& str_lemma,
                                                  const wstring& str_ending,
                                                  ET_Number eo_number, 
                                                  ET_Gender eo_gender, 
                                                  CComObject<CT_WordForm> *& pco_wordForm)
{
    HRESULT h_r = S_OK;

    h_r = CComObject<CT_WordForm>::CreateInstance (&pco_wordForm);
    if (S_OK != h_r)
    {
        ATLASSERT(0);
        CString cs_msg;
        cs_msg.Format (L"CreateInstance() failed, error %x.", h_r);
        ERROR_LOG ((LPCTSTR)cs_msg);
        throw CT_Exception (h_r, (LPCTSTR)cs_msg);
    }

    pco_wordForm->eo_POS = pco_Lexeme->eo_PartOfSpeech;
    pco_wordForm->eo_Subparadigm = SUBPARADIGM_PAST_TENSE;
    pco_wordForm->eo_Reflexive = pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO;
    pco_wordForm->str_Lemma = str_lemma;
    pco_wordForm->str_WordForm = str_lemma + str_ending;
    pco_wordForm->eo_Gender = eo_gender;
    pco_wordForm->eo_Number = eo_number;

    v_AssignStress (pco_wordForm, pco_Lexeme->vec_SecondaryStressPos, false);

    return S_OK;

}   //  h_CreateFormTemplate (...)

HRESULT CT_FormBuilderPast::h_Build()
{
    HRESULT h_r = S_OK;

    try
    {
        vector<int>& vec_sourceStressPos = pco_Lexeme->vec_SourceStressPos;
     
        wstring str_lemma;
        h_BuildPastTenseStem (str_lemma);
        ET_AccentType eo_at = AT_UNDEFINED;
        if (AT_UNDEFINED == pco_Lexeme->eo_AccentType2)
        {
            eo_at = AT_A;
        }
        else
        {
            eo_at = pco_Lexeme->eo_AccentType2;
        }

        wstring str_ending;
        for (ET_Number eo_number = NUM_SG; eo_number < NUM_COUNT; ++eo_number)
            for (ET_Gender eo_gender = GENDER_M; eo_gender < GENDER_COUNT; ++eo_gender)
            {
                h_GetEnding (str_lemma, eo_number, eo_gender, str_ending);

                if (pco_Lexeme->b_Reflexive)
                {
                    if (CT_ExtString (str_ending).b_EndsWithOneOf (str_Vowels))
                    {
                        str_ending += L"сь";
                    }
                    else
                    {
                        str_ending += L"ся";
                    }
                }

                vector<int> vec_iStress;
                h_GetStressPositions (str_lemma, str_ending, eo_number, eo_gender, vec_iStress);

                vector<int>::iterator it_stress = vec_iStress.begin();
                for (; it_stress != vec_iStress.end(); ++it_stress)
                {
                    CComObject<CT_WordForm> * pco_wordForm = NULL;
                    h_CreateFormTemplate (str_lemma, str_ending, eo_number, eo_gender, pco_wordForm);
                    int i_gramHash = pco_wordForm->i_GramHash();
                    if (1 == vec_iStress.size() || b_MultiStress (str_lemma, vec_iStress))
                    {
                        vector<int>::iterator it_stressPos = vec_iStress.begin();
                        for (; it_stressPos != vec_iStress.end(); ++it_stressPos)
                        {
                            pco_wordForm->map_Stress[*it_stressPos] = true;  // primary
                            h_HandleYoAlternation (*it_stress, SUBPARADIGM_PAST_TENSE, str_lemma);
                            pco_wordForm->str_Lemma = str_lemma;
                            pco_wordForm->str_WordForm = str_lemma + str_ending;
                        }
                        pco_Lexeme->h_AddWordForm (i_gramHash, pco_wordForm);
                    }
                    else
                    {
                        vector<int>::iterator it_stressPos = vec_iStress.begin();
                        for (; it_stressPos != vec_iStress.end(); ++it_stressPos)
                        {
                            if (it_stressPos != vec_iStress.begin())
                            {
                                CComObject<CT_WordForm> * pco_wfVariant = NULL;
                                h_CloneWordForm (pco_wordForm, pco_wfVariant);
                                pco_wordForm = pco_wfVariant;
                            }
                            pco_wordForm->map_Stress[*it_stressPos] = true;  // primary
                            h_HandleYoAlternation (*it_stress, SUBPARADIGM_PAST_TENSE, str_lemma);
                            pco_wordForm->str_Lemma = str_lemma;
                            pco_wordForm->str_WordForm = str_lemma + str_ending;
                            pco_Lexeme->h_AddWordForm (i_gramHash, pco_wordForm);
                        }
                    }
                }
            }   // for ...(ET_Number / ET_Gender ... )
    }
    catch (CT_Exception co_ex)
    {
        return co_ex.i_GetErrorCode();  // logging should be always done by callee
    }

    return h_r;

}   //  h_Build()
