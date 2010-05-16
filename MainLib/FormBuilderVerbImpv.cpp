#include "StdAfx.h"
#include "FormBuilderVerbImpv.h"

HRESULT CT_FormBuilderImperative::h_GetLemma (wstring& str_lemma)
{
    int i_hash = -1;
    if (4 == pco_Lexeme->i_Type || 5 == pco_Lexeme->i_Type)
    {
        CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_UNDEFINED, 
                                 TENSE_UNDEFINED, PERSON_3, ANIM_UNDEFINED, pco_Lexeme->eo_Reflexive,
                                 VOICE_UNDEFINED, ASPECT_UNDEFINED);
        i_hash = co_hasher.i_GramHash();
    }
    else
    {
        CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_UNDEFINED, 
                                 TENSE_UNDEFINED, PERSON_1, ANIM_UNDEFINED, pco_Lexeme->eo_Reflexive,
                                 VOICE_UNDEFINED, ASPECT_UNDEFINED);
        i_hash = co_hasher.i_GramHash();
    }

    CT_WordForm * pco_template = NULL;
    h_WordFormFromHash (i_hash, 0, pco_template);
    str_lemma = pco_template->str_Lemma;

    if (11 == pco_Lexeme->i_Type)
    {
        ATLASSERT (CT_ExtString(str_lemma).b_EndsWith (L"ь"));
        str_lemma.erase (str_lemma.length()-1);
    }

    return S_OK;

}   //  h_GetLemma (...)

HRESULT CT_FormBuilderImperative::h_GetEnding (const wstring& str_lemma, wstring& str_ending)
{
    HRESULT h_r = S_OK;

    ET_AccentType eo_accentType1 = pco_Lexeme->eo_AccentType1;
    ET_AccentType eo_accentType2 = pco_Lexeme->eo_AccentType2;

    int i_type = pco_Lexeme->i_Type;

    CT_ExtString xstr_lemma (str_lemma);

    if (11 == i_type)
    {
        str_ending = L"ей";
    }
    else if (xstr_lemma.b_EndsWithOneOf (str_Vowels))
    {
        str_ending = L"й";
        if (4 == i_type)
        {
            if (AT_B == eo_accentType1 || AT_C == eo_accentType1)   // кроить, поить
            {
                str_ending = L"и";
            }
            if (AT_A == eo_accentType1 && xstr_lemma.b_StartsWith (L"вы"))
            {
                str_ending = L"и";
            }
        }
    }
    else
    {
        if (AT_B == eo_accentType1 || AT_C == eo_accentType1)
        {
            str_ending = L"и";
        }
        if (AT_A == eo_accentType1)
        {
            if (xstr_lemma.b_StartsWith (L"вы"))
            {
                str_ending = L"и";
            }
            else if (xstr_lemma.b_EndsWith (L"щ"))      // 2 consonants or щ or CьC
            {
                str_ending = L"и";
            }
            else if (xstr_lemma.length() >= 2 && 
                str_Consonants.find (xstr_lemma[xstr_lemma.length()-1]) != wstring::npos &&
                str_Consonants.find (xstr_lemma[xstr_lemma.length()-2]) != wstring::npos)
            {
                str_ending = L"и";
            }
            else if (xstr_lemma.length() >= 3 && 
                     str_Consonants.find (xstr_lemma[xstr_lemma.length()-1]) != wstring::npos &&
                     L'ь' == xstr_lemma[xstr_lemma.length()-2] &&
                     str_Consonants.find (xstr_lemma[xstr_lemma.length()-3]) != wstring::npos)
            {
                str_ending = L"и";
            }
            else
            {
                str_ending = L"ь";
            }
        }
    }   // else

    return S_OK;

}   //  h_GetEnding (...)

HRESULT CT_FormBuilderImperative::h_GetStressPositions (wstring& str_lemma, 
                                                        wstring& str_ending,
                                                        vector<int>& vec_stressPositions)
{
    HRESULT h_r = S_OK;

    ET_StressLocation eo_stress = STRESS_LOCATION_UNDEFINED;
    switch (pco_Lexeme->eo_AccentType1)
    {
        case AT_A:
        {
            eo_stress = STRESS_LOCATION_STEM;
            break;
        }
        case AT_B:
        case AT_C:
        {
            eo_stress = STRESS_LOCATION_ENDING;
            break;
        }
        default:
        {
            ATLASSERT(0);
            wstring str_msg (L"Illegal accent type.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_FAIL, str_msg);
        }
    }

    int i_stressPos = -1;
    if (STRESS_LOCATION_STEM == eo_stress)
    {
        h_GetStemStressPositions (str_lemma, vec_stressPositions);
    }
    else if (STRESS_LOCATION_ENDING == eo_stress)
    {
        h_GetEndingStressPosition (str_lemma, str_ending, i_stressPos);
        vec_stressPositions.push_back (i_stressPos);
    }
    else
    {
        ATLASSERT(0);
        wstring str_msg (L"Illegal stress type.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_INVALIDARG, str_msg);
    }

    return h_r;

}   //  h_GetStressPositions (...)

HRESULT CT_FormBuilderImperative::h_CreateFormTemplate (const wstring& str_lemma,
                                                        wstring& str_ending,
                                                        ET_Number eo_number,
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

    pco_wordForm->pco_Lexeme = pco_Lexeme;
    pco_wordForm->eo_POS = pco_Lexeme->eo_PartOfSpeech;
    pco_wordForm->eo_Subparadigm = SUBPARADIGM_IMPERATIVE;
    pco_wordForm->eo_Reflexive = pco_Lexeme->eo_Reflexive;
    pco_wordForm->str_Lemma = str_lemma;
    if (NUM_PL == eo_number)
    {
        str_ending += L"те";
    }
    pco_wordForm->str_WordForm = str_lemma + str_ending;
    if (REFL_YES == pco_Lexeme->eo_Reflexive)
    {
        int i_at = pco_wordForm->str_WordForm.length() - 1;
        if (wstring::npos != str_Vowels.find (pco_wordForm->str_WordForm[i_at]))
        {
            pco_wordForm->str_WordForm += L"сь";
        }
        else
        {
            pco_wordForm->str_WordForm += L"ся";
        }
    }

    pco_wordForm->eo_Number = eo_number;

    v_AssignSecondaryStress (pco_wordForm);

    return S_OK;

}   //  h_CreateFormTemplate (...)

HRESULT CT_FormBuilderImperative::h_HandleCommonDeviations (CT_WordForm * pco_wordForm)
{
    HRESULT h_r = S_OK;

    if (!b_HasCommonDeviation (2) && !b_HasCommonDeviation (3))
    {
        return S_FALSE;
    }

    int i_endingOffset = (REFL_NO == pco_Lexeme->eo_Reflexive)
        ? pco_wordForm->str_WordForm.length() - 1
        : pco_wordForm->str_WordForm.length() - 3;

    if (NUM_PL == pco_wordForm->eo_Number)
    {
        i_endingOffset -= 2;
    }

    if (i_endingOffset < 1)
    {
        ATLASSERT(0);
        wstring str_msg (L"Bad ending offset.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_POINTER, str_msg);
    }

    if (b_HasCommonDeviation (2))
    {
        if (b_DeviationOptional (2))   // store both forms
        {
            CComObject<CT_WordForm> * pco_variant = NULL;
            h_CloneWordForm (pco_wordForm, pco_variant);
            pco_Lexeme->h_AddWordForm (pco_variant);
            pco_wordForm = pco_variant;
        }

        if (str_Vowels.find (pco_wordForm->str_WordForm[i_endingOffset-1]) != wstring.npos)
        {
            pco_wordForm->str_WordForm[i_endingOffset] = L'й';
        }
        else
        {
            pco_wordForm->str_WordForm[i_endingOffset] = L'ь';
        }
    }   //      if (b_HasCommonDeviation (2))

    if (b_HasCommonDeviation (3))
    {
        if (NUM_SG == pco_wordForm->eo_Number)
        {
            if (b_DeviationOptional (3))   // store both forms
            {
                pco_wordForm->str_WordForm[i_endingOffset] = L'ь';
                CComObject<CT_WordForm> * pco_variant = NULL;
                h_CloneWordForm (pco_wordForm, pco_variant);
                pco_Lexeme->h_AddWordForm (pco_variant);
                pco_wordForm = pco_variant;
            }
            pco_wordForm->str_WordForm[i_endingOffset] = L'и';
        }
        else
        {
            pco_wordForm->str_WordForm[i_endingOffset] = L'ь';
        }
    }

    return S_OK;

}   //  h_HandleCommonDeviations (...)

HRESULT CT_FormBuilderImperative::h_Build()
{
    HRESULT h_r = S_OK;

    try
    {
        wstring str_lemma;
        h_GetLemma (str_lemma);
        
        wstring str_ending;
        h_GetEnding (str_lemma, str_ending);

        vector<int> vec_iStress;
        h_GetStressPositions (str_lemma, str_ending, vec_iStress);

        for (ET_Number eo_number = NUM_SG; eo_number < NUM_COUNT; ++eo_number)
        {
            vector<int>::iterator it_stressPos = vec_iStress.begin();

            CComObject<CT_WordForm> * pco_wordForm = NULL;
            h_CreateFormTemplate (str_lemma, str_ending, eo_number, pco_wordForm);
            long i_gramHash = pco_wordForm->i_GramHash();

            if (1 == vec_iStress.size() || b_MultiStress (str_lemma, vec_iStress))
            {
                vector<int>::iterator it_stressPos = vec_iStress.begin();
                for (; it_stressPos != vec_iStress.end(); ++it_stressPos)
                {
                    pco_wordForm->map_Stress[*it_stressPos] = true;  // primary
                }
                h_HandleCommonDeviations (pco_wordForm);
                pco_Lexeme->h_AddWordForm (pco_wordForm);
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
                    h_HandleCommonDeviations (pco_wordForm);
                    pco_Lexeme->h_AddWordForm (pco_wordForm);
                }
            }
        }       //  for (; it_stressPos != vec_sourceStressPos.end(); ...)
    }
    catch (CT_Exception co_ex)
    {
        return co_ex.i_GetErrorCode();  // logging should be always done by callee
    }

    return h_r;

}   //  h_BuildImperativeForms()
