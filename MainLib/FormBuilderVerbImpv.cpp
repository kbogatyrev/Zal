#include "StdAfx.h"
#include "FormBuilderVerbImpv.h"

HRESULT CT_FormBuilderImperative::h_GetLemma (wstring& str_lemma)
{
    int i_hash = -1;
    if (4 == pco_Lexeme->i_Type || 5 == pco_Lexeme->i_Type)
    {
//        CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_UNDEFINED, 
//                                 TENSE_UNDEFINED, PERSON_3, ANIM_UNDEFINED, pco_Lexeme->eo_Reflexive,
//                                 VOICE_UNDEFINED, ASPECT_UNDEFINED);
        CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_UNDEFINED, 
                                 PERSON_3, ANIM_UNDEFINED, ASPECT_UNDEFINED, pco_Lexeme->eo_Reflexive);
        i_hash = co_hasher.i_GramHash();
    }
    else
    {
//        CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_UNDEFINED, 
//                                 TENSE_UNDEFINED, PERSON_1, ANIM_UNDEFINED, pco_Lexeme->eo_Reflexive,
//                                 VOICE_UNDEFINED, ASPECT_UNDEFINED);
        CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_UNDEFINED, 
                                 PERSON_1, ANIM_UNDEFINED, ASPECT_UNDEFINED, pco_Lexeme->eo_Reflexive);
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
    pco_wordForm->eo_Person = PERSON_2;

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
        if (4 == pco_Lexeme->i_Type || 5 == pco_Lexeme->i_Type)
        {
            str_lemma = pco_Lexeme->xstr_3SgStem.c_str();
        }
        else
        {
            str_lemma = pco_Lexeme->xstr_1SgStem.c_str();
        }

//        h_GetLemma (str_lemma);
        if (11 == pco_Lexeme->i_Type)
        {
            ATLASSERT (CT_ExtString(str_lemma).b_EndsWith (L"ь"));
            str_lemma.erase (str_lemma.length()-1);
        }
        
        h_FleetingVowelCheck (str_lemma);

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

            bool b_isVariant = false;
            h_r = h_CheckIrregular (pco_wordForm, b_isVariant);
            if (S_OK == h_r)
            {
                if (!b_isVariant)
                {
                    continue;
                }
            }
            else
            {
                if (FAILED (h_r))
                {
                    ATLASSERT (0);
                    CString cs_msg;
                    cs_msg.Format (L"h_CheckIrregular() failed, error %x.", h_r);
                    ERROR_LOG ((LPCTSTR)cs_msg);
//                            throw CT_Exception (h_r, (LPCTSTR)cs_msg);
                    continue;
                }
            }

            if (1 == vec_iStress.size() || b_MultiStress (str_lemma, vec_iStress))
            {
                vector<int>::iterator it_stressPos = vec_iStress.begin();
                for (; it_stressPos != vec_iStress.end(); ++it_stressPos)
                {
                    pco_wordForm->map_Stress[*it_stressPos] = STRESS_PRIMARY;  // primary
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
                    pco_wordForm->map_Stress[*it_stressPos] = STRESS_PRIMARY;
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

HRESULT CT_FormBuilderImperative::h_BuildIrregular()
{
    HRESULT h_r = S_OK;

    CT_GramHasher co_sgHash (SUBPARADIGM_IMPERATIVE, 
                             NUM_SG, 
                             GENDER_UNDEFINED, 
                             PERSON_2, 
                             ANIM_UNDEFINED, 
                             CASE_UNDEFINED, 
                             pco_Lexeme->eo_Reflexive);
    ST_IrregularForm st_ifSg;
    h_r = pco_Lexeme->h_GetIrregularForm (co_sgHash.i_GramHash(), st_ifSg);
    if (FAILED (h_r))
    {
        ATLASSERT(0);
        wstring str_msg (L"Error retrieving irregular imperative singular for ");
        str_msg += pco_Lexeme->xstr_Infinitive;
        ERROR_LOG (str_msg);
    }
    while (S_OK == h_r)
    {
        CT_GramHasher co_plHash (co_sgHash);
        co_plHash.eo_Number = NUM_PL;
        ST_IrregularForm st_ifPl;
        HRESULT h_plFormRet = pco_Lexeme->h_GetIrregularForm (co_plHash.i_GramHash(), st_ifPl);
        if (FAILED (h_plFormRet))
        {
            ATLASSERT(0);
            wstring str_msg (L"Error retrieving irregular imperative plural for ");
            str_msg += pco_Lexeme->xstr_Infinitive;
            ERROR_LOG (str_msg);
        }
        if (S_OK != h_plFormRet)
        {
            st_ifPl = st_ifSg;
            st_ifPl.str_Form += L"те";
            if (REFL_YES == pco_Lexeme->eo_Reflexive)
            {
                st_ifPl.str_Form += L"сь";
            }
            pco_Lexeme->h_AddIrregularForm (co_plHash.i_GramHash(), st_ifPl);
        }
    }

    ST_IrregularForm st_3Pl;
    h_r = h_Get3PlIrregular (st_3Pl);
    if (S_OK != h_r)
    {
        return h_r;
    }

    CT_ExtString xstr_3Pl (st_3Pl.str_Form);
    if (REFL_NO == pco_Lexeme->eo_Reflexive)
    {
        if (!xstr_3Pl.b_EndsWith (L"ют") && 
            !xstr_3Pl.b_EndsWith (L"ут") &&
            !xstr_3Pl.b_EndsWith (L"ят") &&
            !xstr_3Pl.b_EndsWith (L"ат"))
        {
            ATLASSERT(0);
            wstring str_msg (L"Unexpected 3 Pl ending: ");
            str_msg += xstr_3Pl.c_str();
            ERROR_LOG (str_msg);
            return E_UNEXPECTED;
        }
    }
    else if (REFL_YES == pco_Lexeme->eo_Reflexive)
    {
        if (!xstr_3Pl.b_EndsWith (L"ются") && 
            !xstr_3Pl.b_EndsWith (L"утся") &&
            !xstr_3Pl.b_EndsWith (L"ятся") &&
            !xstr_3Pl.b_EndsWith (L"атся"))
        {
            ATLASSERT(0);
            wstring str_msg (L"Unexpected 3 Pl refl. ending: ");
            str_msg += xstr_3Pl.c_str();
            ERROR_LOG (str_msg);
            return E_UNEXPECTED;
        }
    }
    else
    {
            ATLASSERT(0);
            wstring str_msg (L"Unexpected eo_Reflexive value for: ");
            str_msg += pco_Lexeme->xstr_Infinitive;
            ERROR_LOG (str_msg);
            return E_UNEXPECTED;
    }

    int i_charsToErase = REFL_YES == pco_Lexeme->eo_Reflexive ? 4 : 2;
    xstr_3Pl.erase (xstr_3Pl.length() - i_charsToErase);

    for (ET_Number eo_number = NUM_SG; eo_number < NUM_COUNT; ++eo_number)
    {
        CComObject<CT_WordForm> * pco_wordForm;
        h_r = CComObject<CT_WordForm>::CreateInstance (&pco_wordForm);
        if (S_OK != h_r)
        {
            ATLASSERT(0);
            CString cs_msg;
            cs_msg.Format (L"CreateInstance() failed, error %x.", h_r);
            ERROR_LOG ((LPCTSTR)cs_msg);
            throw CT_Exception (E_INVALIDARG, (LPCTSTR)cs_msg);
        }

        pco_wordForm->pco_Lexeme = pco_Lexeme;
        pco_wordForm->eo_POS = pco_Lexeme->eo_PartOfSpeech;
        pco_wordForm->eo_Subparadigm = SUBPARADIGM_IMPERATIVE;
        pco_wordForm->eo_Reflexive = pco_Lexeme->eo_Reflexive;
        pco_wordForm->eo_Number = eo_number;
        pco_wordForm->eo_Person = PERSON_2;

        multimap<int, ST_IrregularForm>& mmap_if = pco_Lexeme->mmap_IrregularForms;    // alias
        multimap<int, ST_IrregularForm>::iterator it_if = mmap_if.find (pco_wordForm->i_GramHash());
        if (mmap_if.end() != it_if)
        {
            pco_wordForm->str_WordForm = (*it_if).second.str_Form;
            pco_wordForm->map_Stress = (*it_if).second.map_Stress;
        }
        else
        {
            v_AssignSecondaryStress (pco_wordForm);
            pco_wordForm->str_Lemma = xstr_3Pl.c_str();
            if (xstr_3Pl.b_EndsWithOneOf (str_Consonants))
            {
                pco_wordForm->str_WordForm = pco_wordForm->str_Lemma + L"и";
                pco_wordForm->map_Stress[xstr_3Pl.i_GetNumOfSyllables()] = STRESS_PRIMARY;
            }
            else
            {
                pco_wordForm->str_WordForm = pco_wordForm->str_Lemma + L"й";
                pco_wordForm->map_Stress = st_3Pl.map_Stress;
            }

            if (NUM_PL == eo_number)
            {
                pco_wordForm->str_WordForm += L"те";
            }
        }

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

        pco_Lexeme->h_AddWordForm (pco_wordForm);

    }       //  for ...

    return h_r;

}   //  h_BuildIrregular()
