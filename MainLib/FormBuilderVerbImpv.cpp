#include "StdAfx.h"
#include "FormBuilderVerbImpv.h"

HRESULT CFormBuilderImperative::h_GetLemma (wstring& str_lemma)
{
    int i_hash = -1;
    if (4 == pco_Lexeme->i_Type || 5 == pco_Lexeme->i_Type)
    {
        CGramHasher co_hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_UNDEFINED, 
                                 PERSON_3, ANIM_UNDEFINED, pco_Lexeme->eo_Aspect, pco_Lexeme->eo_Reflexive);
        i_hash = co_hasher.i_GramHash();
    }
    else
    {
        CGramHasher co_hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_UNDEFINED, 
                                 PERSON_1, ANIM_UNDEFINED, pco_Lexeme->eo_Aspect, pco_Lexeme->eo_Reflexive);
        i_hash = co_hasher.i_GramHash();
    }

    CWordForm * pco_template = NULL;
    h_WordFormFromHash (i_hash, 0, pco_template);
    str_lemma = pco_template->str_Lemma;

    if (11 == pco_Lexeme->i_Type)
    {
        ATLASSERT (CT_ExtString(str_lemma).b_EndsWith (L"ь"));
        str_lemma.erase (str_lemma.length()-1);
    }

    return S_OK;

}   //  h_GetLemma (...)

HRESULT CFormBuilderImperative::hGetEnding (const wstring& str_lemma, wstring& str_ending)
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
    else if (xstr_lemma.b_EndsWithOneOf (g_sVowels))
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
        if (AT_B == eo_accentType1 || AT_C == eo_accentType1 || AT_C1 == eo_accentType1)
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
                g_szConsonants.find (xstr_lemma[xstr_lemma.length()-1]) != wstring::npos &&
                g_szConsonants.find (xstr_lemma[xstr_lemma.length()-2]) != wstring::npos)
            {
                str_ending = L"и";
            }
            else if (xstr_lemma.length() >= 3 && 
                     g_szConsonants.find (xstr_lemma[xstr_lemma.length()-1]) != wstring::npos &&
                     L'ь' == xstr_lemma[xstr_lemma.length()-2] &&
                     g_szConsonants.find (xstr_lemma[xstr_lemma.length()-3]) != wstring::npos)
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

}   //  hGetEnding (...)

HRESULT CFormBuilderImperative::h_GetStressPositions (wstring& str_lemma, 
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
		case AT_C1:			// хотеть and derivatives
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
        hGetEndingStressPosition (str_lemma, str_ending, i_stressPos);
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

HRESULT CFormBuilderImperative::h_CreateFormTemplate (const wstring& str_lemma,
                                                        wstring& str_ending,
                                                        ET_Number eo_number,
                                                        CComObject<CWordForm> *& pco_wordForm)
{
    HRESULT h_r = S_OK;

    h_r = CComObject<CWordForm>::CreateInstance (&pco_wordForm);
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
    pco_wordForm->eo_Aspect = pco_Lexeme->eo_Aspect;
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
        if (wstring::npos != g_sVowels.find (pco_wordForm->str_WordForm[i_at]))
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

HRESULT CFormBuilderImperative::h_HandleCommonDeviations (CWordForm * pco_wordForm)
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
            CComObject<CWordForm> * pco_variant = NULL;
            h_CloneWordForm (pco_wordForm, pco_variant);
            pco_Lexeme->h_AddWordForm (pco_variant);
            pco_wordForm = pco_variant;
        }

        if (g_sVowels.find (pco_wordForm->str_WordForm[i_endingOffset-1]) != wstring.npos)
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
                CComObject<CWordForm> * pco_variant = NULL;
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

HRESULT CFormBuilderImperative::h_CheckForIrregularForms (bool& b_isVariant)
                                                // check if irregular forms were built and
                                                // regular processing must be skipped
{
    HRESULT h_r = S_OK;

    if (b_HasIrregularImperative() || b_HasIrregularPresent())
    {
        h_r = h_BuildIrregularForms();
        if (S_OK != h_r)
        {
            return h_r;
        }
    }
    else
    {
        return S_FALSE;
    }
        
    b_isVariant = false;
    for (ET_Number eo_number = NUM_SG; eo_number < NUM_COUNT; ++eo_number)
    {
        CComObject<CWordForm> * pco_wordForm = NULL;
        h_r = CComObject<CWordForm>::CreateInstance (&pco_wordForm);
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
        pco_wordForm->eo_Aspect = pco_Lexeme->eo_Aspect;
        pco_wordForm->eo_Reflexive = pco_Lexeme->eo_Reflexive;
        pco_wordForm->eo_Person = PERSON_2;
        pco_wordForm->eo_Number = eo_number;

        v_AssignSecondaryStress (pco_wordForm);
        
        h_r = h_HandleIrregularForms (pco_wordForm, b_isVariant);
        if (S_OK != h_r)
        {
            ATLASSERT (0);
            CString cs_msg;
            cs_msg.Format (L"h_HandleIrregularForms() failed, error %x.", h_r);
            ERROR_LOG ((LPCTSTR)cs_msg);
            return h_r;
        }
    }       //  for (ET_Number eo_number = NUM_SG; ...

    return h_r;

  }     //  h_CheckForIrregularForms (...)

HRESULT CFormBuilderImperative::h_Build()
{
    HRESULT h_r = S_OK;

    bool b_isVariant = false;
    h_r = h_CheckForIrregularForms (b_isVariant);
    if (S_OK == h_r && !b_isVariant)
    {
        return h_r;
    }

    try
    {
        wstring str_lemma;

        if (17 == pco_Lexeme->i_Section)
        {
            ATLASSERT (14 == pco_Lexeme->i_Type);
            return S_FALSE;
        }

        if (4 == pco_Lexeme->i_Type || 5 == pco_Lexeme->i_Type)
        {
            str_lemma = pco_Lexeme->xstr_3SgStem.c_str();
        }
        else
        {
            str_lemma = pco_Lexeme->xstr_1SgStem.c_str();
        }

        if (str_lemma.empty())
        {
            ATLASSERT(0);
            wstring str_msg (L"Empty lemma for ");
            str_msg += pco_Lexeme->xstr_Infinitive;
            ERROR_LOG (str_msg);
            return E_FAIL;
        }

        if (11 == pco_Lexeme->i_Type)
        {
            ATLASSERT (CT_ExtString(str_lemma).b_EndsWith (L"ь"));
            str_lemma.erase (str_lemma.length()-1);
        }
        
        h_FleetingVowelCheck (str_lemma);

        wstring str_ending;
        hGetEnding (str_lemma, str_ending);

        vector<int> vec_iStress;
        h_GetStressPositions (str_lemma, str_ending, vec_iStress);

        for (ET_Number eo_number = NUM_SG; eo_number < NUM_COUNT; ++eo_number)
        {
            vector<int>::iterator it_stressPos = vec_iStress.begin();

            CComObject<CWordForm> * pco_wordForm = NULL;
            h_CreateFormTemplate (str_lemma, str_ending, eo_number, pco_wordForm);

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
                        CComObject<CWordForm> * pco_wfVariant = NULL;
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

//
// Irregular forms may or may not be in the database
//
HRESULT CFormBuilderImperative::h_BuildIrregularForms()
{
    HRESULT h_r = S_OK;

    CGramHasher co_sgHash (SUBPARADIGM_IMPERATIVE, 
                             NUM_SG, 
                             GENDER_UNDEFINED, 
                             PERSON_2, 
                             ANIM_UNDEFINED, 
							 pco_Lexeme->eo_Aspect,
                             CASE_UNDEFINED, 
                             pco_Lexeme->eo_Reflexive);
    CGramHasher co_plHash (co_sgHash);
    co_plHash.eo_Number = NUM_PL;

    if (b_HasIrregularImperative())
    {
        StIrregularForm st_ifSg;
        h_r = pco_Lexeme->h_GetFirstIrregularForm (co_sgHash.i_GramHash(), st_ifSg);
        if (FAILED (h_r))
        {
            ATLASSERT(0);
            wstring str_msg (L"Error retrieving irregular imperative singular for ");
            str_msg += pco_Lexeme->xstr_Infinitive;
            ERROR_LOG (str_msg);
            return h_r;
        }

        StIrregularForm st_ifPl;
        HRESULT h_plFormRet = pco_Lexeme->h_GetFirstIrregularForm (co_plHash.i_GramHash(), st_ifPl);
        if (FAILED (h_plFormRet))
        {
            ATLASSERT(0);
            wstring str_msg (L"Error retrieving irregular imperative plural for ");
            str_msg += pco_Lexeme->xstr_Infinitive;
            ERROR_LOG (str_msg);
            return h_plFormRet;
        }

        // Derive from Sg if not in DB
        if (S_OK != h_plFormRet)
        {
            st_ifPl = st_ifSg;
            st_ifPl.str_Form += L"те";
            if (REFL_YES == pco_Lexeme->eo_Reflexive)
            {
                st_ifPl.str_Form += L"сь";
            }
        }
        
        pco_Lexeme->h_AddIrregularForm (co_plHash.i_GramHash(), st_ifPl);
            
        return S_OK;

    }   //   if (b_HasIrregularImperative())

    //
    // No irregular forms in DB: derive from present tense
    //
    if (!b_HasIrregularPresent())
    {
        ATLASSERT(0);
        wstring str_msg (L"No irregular present.");
        str_msg += pco_Lexeme->xstr_Infinitive;
        ERROR_LOG (str_msg);
        return E_UNEXPECTED;
    }

    //
    // No imperative forms provided -- construct imperative from present tense
    //
    StIrregularForm st_3Pl;
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

    StIrregularForm st_ifSg, st_ifPl;

    if (xstr_3Pl.b_EndsWithOneOf (g_szConsonants))
    {
        st_ifSg.str_Form = xstr_3Pl + L"и";
        st_ifSg.map_Stress[xstr_3Pl.i_GetNumOfSyllables()] = STRESS_PRIMARY;
    }
    else
    {
        st_ifSg.str_Form = xstr_3Pl + L"й";
        st_ifSg.map_Stress = st_3Pl.map_Stress;
    }

    st_ifPl.str_Form = st_ifSg.str_Form + L"те";
    st_ifPl.map_Stress = st_ifSg.map_Stress;

    if (REFL_YES == pco_Lexeme->eo_Reflexive)
    {
        int i_at = st_ifSg.str_Form.length() - 1;
        if (wstring::npos != g_sVowels.find (st_ifSg.str_Form[i_at]))
        {
            st_ifSg.str_Form += L"сь";
        }
        else
        {
            st_ifSg.str_Form += L"ся";
        }
        st_ifPl.str_Form += L"сь";
    }

    pco_Lexeme->h_AddIrregularForm (co_sgHash.i_GramHash(), st_ifSg);
    pco_Lexeme->h_AddIrregularForm (co_plHash.i_GramHash(), st_ifPl);

    return S_OK;

}   //  h_BuildIrregularForms()
