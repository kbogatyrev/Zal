#include "StdAfx.h"
#include "FormBuilderVerbPast.h"

static wchar_t arr_endings[][4] = { L"л", L"ла", L"ло", L"ли" };

HRESULT CFormBuilderPast::hGetEnding (wstring& str_lemma, 
                                         ET_Number eo_number, 
                                         ET_Gender eo_gender, 
                                         wstring& str_ending)
{
    HRESULT h_r = S_OK;

    if (NUM_SG == eo_number)
    {
        if (GENDER_M == eo_gender && CT_ExtString(str_lemma).b_EndsWithOneOf (g_szConsonants))
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

}   //  hGetEnding (...)

HRESULT CFormBuilderPast::h_GetStressPositions (const wstring& str_lemma,
                                                  const wstring& str_ending,
                                                  ET_Number eo_number, 
                                                  ET_Gender eo_gender, 
                                                  vector<int>& vec_iStressPositions)
{
    HRESULT h_r = S_OK;

    ET_AccentType eo_accentType = AT_UNDEFINED;
    if (AT_UNDEFINED == pLexeme->eo_AccentType2)
    {
        eo_accentType = AT_A;
    }
    else
    {
        eo_accentType = pLexeme->eo_AccentType2;
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
            if (AT_C2 == eo_accentType && GENDER_M == eo_gender && NUM_SG == eo_number)
            {
                if (REFL_NO == pLexeme->eo_Reflexive)
                {
                    ATLASSERT(0);
                    wstring str_msg (L"Unexpected part of speech value.");
                    ERROR_LOG (str_msg);
                    throw CT_Exception (E_FAIL, str_msg);
                }
                // This is or is becoming obsolete? (GDRL, p. 80-81)
                vec_iStressPositions.push_back (CT_ExtString (str_lemma).i_NSyllables());
            }
            else
            {
                int i_pos = -1;
                hGetEndingStressPosition (str_lemma, str_ending, i_pos);
                vec_iStressPositions.push_back (i_pos);
            }
        }
    }

    return S_OK;

}   //  h_GetStressPositions (...)

HRESULT CFormBuilderPast::h_CreateFormTemplate (const wstring& str_lemma,
                                                  const wstring& str_ending,
                                                  ET_Number eo_number, 
                                                  ET_Gender eo_gender, 
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

    pco_wordForm->pLexeme = pLexeme;
    pco_wordForm->eo_POS = pLexeme->eo_PartOfSpeech;
    pco_wordForm->eo_Subparadigm = SUBPARADIGM_PAST_TENSE;
    pco_wordForm->eo_Aspect = pLexeme->eo_Aspect;
    pco_wordForm->eo_Reflexive = pLexeme->eo_Reflexive;
    pco_wordForm->str_Lemma = str_lemma;
    pco_wordForm->str_WordForm = str_lemma + str_ending;
    pco_wordForm->eo_Gender = eo_gender;
    pco_wordForm->eo_Number = eo_number;

    v_AssignSecondaryStress (pco_wordForm);

    return S_OK;

}   //  h_CreateFormTemplate (...)

HRESULT CFormBuilderPast::h_Assemble (CWordForm * pco_wordForm, 
                                        int i_stressPos, 
                                        wstring& str_lemma, 
                                        wstring& str_ending)
{
    pco_wordForm->map_Stress[i_stressPos] = STRESS_PRIMARY;
    pco_wordForm->str_Lemma = str_lemma;
    h_HandleYoAlternation (i_stressPos, SUBPARADIGM_PAST_TENSE, pco_wordForm->str_Lemma);
    pco_wordForm->str_WordForm = pco_wordForm->str_Lemma + str_ending;
    if (b_HasCommonDeviation (1) && 
        !(NUM_SG == pco_wordForm->eo_Number && 
          GENDER_F == pco_wordForm->eo_Gender))
    {
        h_RetractStressToPreverb (pco_wordForm, b_DeviationOptional (1));
    }

    if (b_HasCommonDeviation (5) && 
        3 == pLexeme->i_Type && 
        1 == pLexeme->i_StemAugment && 
        NUM_SG == pco_wordForm->eo_Number &&
        GENDER_M == pco_wordForm->eo_Gender)
    {
        if (b_DeviationOptional (5))
        {
            CComObject<CWordForm> * pco_variant = NULL;
            h_CloneWordForm (pco_wordForm, pco_variant);
            pLexeme->h_AddWordForm (pco_variant);    // store both versions
            pco_wordForm = pco_variant;
        }

        pco_wordForm->str_WordForm = str_lemma + L"нул";
    }

    return S_OK;

}   // h_Assemble (...)

HRESULT CFormBuilderPast::h_Build()
{
    HRESULT h_r = S_OK;

    bool b_isVariant = false;

    if (b_HasIrregularPast())
    {
        h_r = h_BuildIrregular (b_isVariant);
        if (!b_isVariant)
        {
            return h_r;
        }
    }

    try
    {
        vector<int>& vec_sourceStressPos = pLexeme->vec_SourceStressPos;
     
        wstring str_lemma;
        h_r = h_BuildPastTenseStem (str_lemma);
		if (S_OK != h_r)
		{
			return h_r;
		}
		if (str_lemma.empty())
		{
			return E_UNEXPECTED;
		}

        ET_AccentType eo_at = AT_UNDEFINED;
        if (AT_UNDEFINED == pLexeme->eo_AccentType2)
        {
            eo_at = AT_A;
        }
        else
        {
            eo_at = pLexeme->eo_AccentType2;
        }

        wstring str_ending;
        for (ET_Number eo_number = NUM_SG; eo_number < NUM_COUNT; ++eo_number)
        {
            for (ET_Gender eo_gender = GENDER_UNDEFINED; eo_gender < GENDER_COUNT; ++eo_gender)
            {
                if (NUM_PL == eo_number && eo_gender != GENDER_UNDEFINED)
                {
                    continue;
                }
                if (NUM_SG == eo_number && eo_gender == GENDER_UNDEFINED)
                {
                    continue;
                }

                hGetEnding (str_lemma, eo_number, eo_gender, str_ending);

                if (REFL_YES == pLexeme->eo_Reflexive)
                {
                    if (str_ending.empty())
                    {
                        str_ending += L"ся";
                    }
                    else
                    {
                        if (CT_ExtString (str_ending).b_EndsWithOneOf (g_sVowels))
                        {
                            str_ending += L"сь";
                        }
                        else
                        {
                            str_ending += L"ся";
                        }
                    }
                }

                vector<int> vec_iStress;
                h_GetStressPositions (str_lemma, str_ending, eo_number, eo_gender, vec_iStress);

                CComObject<CWordForm> * pco_wordForm = NULL;
                h_CreateFormTemplate (str_lemma, str_ending, eo_number, eo_gender, pco_wordForm);
                
                bool b_isVariant = false;
                h_r = h_HandleIrregularForms (pco_wordForm, b_isVariant);
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
                        cs_msg.Format (L"h_HandleIrregularForms() failed, error %x.", h_r);
                        ERROR_LOG ((LPCTSTR)cs_msg);
//                            throw CT_Exception (h_r, (LPCTSTR)cs_msg);
                        continue;
                    }
                }

                int i_gramHash = pco_wordForm->i_GramHash();
                if (1 == vec_iStress.size() || b_MultiStress (str_lemma, vec_iStress))
                {
                    vector<int>::iterator it_stressPos = vec_iStress.begin();
                    for (; it_stressPos != vec_iStress.end(); ++it_stressPos)
                    {
                        h_Assemble (pco_wordForm, *it_stressPos, str_lemma, str_ending);
                    }
                    pLexeme->h_AddWordForm (pco_wordForm);
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
                            pco_wfVariant->map_Stress.clear();
                            pco_wordForm = pco_wfVariant;
                        }
                        h_Assemble (pco_wordForm, *it_stressPos, str_lemma, str_ending);
                        pLexeme->h_AddWordForm (pco_wordForm);
                    }
                }
            }   //  for (ET_Gender eo_gender = ... )
        }   // for ...(ET_Number = ... )
    }
    catch (CT_Exception co_ex)
    {
        return co_ex.i_GetErrorCode();  // logging should be always done by callee
    }

    return h_r;

}   //  h_Build()

HRESULT CFormBuilderPast::h_BuildIrregular (bool& b_isVariant)
{
    HRESULT h_r = S_OK;

    for (ET_Number eo_number = NUM_SG; eo_number < NUM_COUNT; ++eo_number)
    {
        for (ET_Gender eo_gender = GENDER_UNDEFINED; eo_gender < GENDER_COUNT; ++eo_gender)
        {
            if (NUM_PL == eo_number && eo_gender != GENDER_UNDEFINED)
            {
                continue;
            }
            if (NUM_SG == eo_number && eo_gender == GENDER_UNDEFINED)
            {
                continue;
            }

            CComObject<CWordForm> * pco_wordForm = NULL;
            h_r = h_CreateFormTemplate (L"", L"", eo_number, eo_gender, pco_wordForm);
            if (!pco_wordForm || S_OK != h_r)
            {
                return S_OK == h_r ? E_POINTER : h_r;
            }
            
            h_r = h_HandleIrregularForms (pco_wordForm, b_isVariant);
            if (S_OK != h_r)
            {
                if (FAILED (h_r))
                {
                    ATLASSERT (0);
                    CString cs_msg;
                    cs_msg.Format (L"h_HandleIrregularForms() failed, error %x.", h_r);
                    ERROR_LOG ((LPCTSTR)cs_msg);
                    continue;
                }
            }

        }   //  for (ET_Gender eo_gender ...

    }   //  for (ET_Number eo_number ...

    return S_OK;

}   //  h_BuildIrregular (...)

//
// The older version is no longer needed since Tim's code already does that.
//
/*
//
// Needed when for abbreviated subparadigm rendering (GDRL 15, 16, 35, 89)
//
HRESULT CFormBuilderPast::h_BuildIrregular()
{
    HRESULT h_r = S_OK;

    for (ET_Number eo_number = NUM_SG; eo_number < NUM_COUNT; ++eo_number)
    {
        for (ET_Gender eo_gender = GENDER_UNDEFINED; eo_gender < GENDER_COUNT; ++eo_gender)
        {
            if (NUM_PL == eo_number && eo_gender != GENDER_UNDEFINED)
            {
                continue;
            }
            if (NUM_SG == eo_number && eo_gender == GENDER_UNDEFINED)
            {
                continue;
            }

            CGramHasher co_hash (SUBPARADIGM_PAST_TENSE, 
                                   eo_number, 
                                   eo_gender, 
                                   PERSON_UNDEFINED, 
                                   ANIM_UNDEFINED,
                                   pLexeme->eo_Aspect,
                                   CASE_UNDEFINED, 
                                   pLexeme->eo_Reflexive);

            multimap<int, StIrregularForm>& mmap_if = pLexeme->mmap_IrregularForms;
            multimap<int, StIrregularForm>::iterator it_if = mmap_if.find (co_hash.i_GramHash());
            
            if (GENDER_M == eo_gender || GENDER_F == eo_gender)
            {
                // These two forms must be in the db id the verb has 
                // irregular past tense subparadigm; GDRL p. 89
                if (pLexeme->mmap_IrregularForms.end() == it_if)
                {
                    ATLASSERT(0);
                    ERROR_LOG (L"Irregular form not found.");
                    return E_UNEXPECTED;
                }

                if ((*it_if).second.str_Form.empty())
                {
                    ATLASSERT(0);
                    ERROR_LOG (L"Empty irregular form.");
                    return E_UNEXPECTED;
                }

                pLexeme->h_AddIrregularForm (co_hash.i_GramHash(), (*it_if).second);

                continue;
            }

            if (GENDER_N == eo_gender || NUM_PL == eo_number)
            {
                if (pLexeme->mmap_IrregularForms.end() != it_if)
                {
                    if ((*it_if).second.str_Form.empty())
                    {
                        ATLASSERT(0);
                        ERROR_LOG (L"Empty irregular form.");
                        return E_UNEXPECTED;
                    }
                    
                    pLexeme->h_AddIrregularForm (co_hash.i_GramHash(), (*it_if).second);

                    continue;
                }

                // Construct per GDRL instructions if not found in the db
                StIrregularForm st_template;
                h_r = h_GetPastFIrregular (st_template);
                if (S_OK != h_r)
                {
                    return h_r;
                }

                if (st_template.str_Form.empty())
                {
                    ATLASSERT(0);
                    ERROR_LOG (L"Empty irregular f. sg. past.");
                    return E_UNEXPECTED;
                }

                if (st_template.str_Form.length() < 2)
                {
                    ATLASSERT(0);
                    ERROR_LOG (L"f. sg. past too short.");
                    return E_UNEXPECTED;
                }
                if (st_template.str_Form[st_template.str_Form.length()-1] != L'а')
                {
                    ATLASSERT(0);
                    ERROR_LOG (L"Unexpected ending in irregular f. sg. past.");
                    return E_UNEXPECTED;
                }

                StIrregularForm st_if (st_template);
                st_if.str_Form = st_template.str_Form.substr (0, st_if.str_Form.length()-1);
                if (GENDER_N == eo_gender && NUM_SG == eo_number)
                {
                    st_if.str_Form += L'о';
                }
                else if (NUM_PL == eo_number && GENDER_UNDEFINED == eo_gender)
                {
                    st_if.str_Form += L'и';
                }
                else
                {
                    ATLASSERT(0);
                    ERROR_LOG (L"Unexpected number or gender value.");
                    return E_UNEXPECTED;
                }

                pLexeme->h_AddIrregularForm (co_hash.i_GramHash(), st_if);

            }       //  if (GENDER_N == eo_gender || NUM_PL == eo_number)

        }   //  for (ET_Gender eo_gender ...
    }   //  for (ET_Number eo_number ...

    return S_OK;

}   //  h_BuildIrregular()
*/
