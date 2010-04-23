#include "StdAfx.h"
#include "Endings.h"
#include "FormBuilderAdjShort.h"

HRESULT CT_FormBuilderShortAdj::h_GetEndings()
{
    HRESULT h_r = S_OK;

    wstring str_select (L"SELECT DISTINCT ending, gender, number, stress FROM endings");
    str_select += L" WHERE inflection_class = ";
    str_select += str_ToString (ENDING_CLASS_SHORT_ADJECTIVE);
    str_select += L" AND inflection_type = ";
    str_select += str_ToString (i_Type);
    str_select += L";";

    try
    {
        pco_Db->v_PrepareForSelect (str_select);
        while (pco_Db->b_GetRow())
        {
            wstring str_ending;
            ST_EndingDescriptor st_d;
            pco_Db->v_GetData (0, str_ending);
            pco_Db->v_GetData (1, (int&)st_d.eo_Gender);
            pco_Db->v_GetData (2, (int&)st_d.eo_Number);
            pco_Db->v_GetData (3, (int&)st_d.eo_Stress);
            HRESULT h_r = pco_Endings->h_AddEnding (str_ending, st_d);
        }
    }
    catch (...)
    {
        v_ReportDbError();    
        throw CT_Exception (E_FAIL, L"");
    }
    
    return S_OK;
        
}   //  h_GetShortFormEndings()

HRESULT CT_FormBuilderShortAdj::h_GetStressTypes (ET_Number eo_number, 
                                                  ET_Gender eo_gender, 
                                                  vector<ET_StressLocation>& vec_eoStressType)
{
    HRESULT h_r = S_OK;

    //if (POS_ADJ != pco_Lexeme->eo_PartOfSpeech)
    //{
    //    ATLASSERT(0);
    //    wstring str_msg (L"Wrong part of speech.");
    //    ERROR_LOG (str_msg);
    //    throw CT_Exception (E_FAIL, str_msg);
    //}

    if (NUM_PL == eo_number && GENDER_UNDEFINED != eo_gender)
    {
        ATLASSERT(0);
        wstring str_msg (L"Unexpected gender/number values.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_INVALIDARG, str_msg);
    }

    if (NUM_SG == eo_number && GENDER_UNDEFINED == eo_gender)
    {
        ATLASSERT(0);
        wstring str_msg (L"Unexpected gender/number values.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_INVALIDARG, str_msg);
    }

    if (GENDER_M == eo_gender)
    {
        ATLASSERT (NUM_SG == eo_number);
        vec_eoStressType.push_back (STRESS_LOCATION_STEM);
        return S_OK;
    }

    ET_AccentType eo_at = AT_UNDEFINED;
    if (AT_UNDEFINED == eo_AccentType2)
    {
        eo_at = eo_AccentType1;
    }
    else
    {
        eo_at = eo_AccentType2;
    }

    switch (eo_at)
    {
        case AT_UNDEFINED:
        {
            ATLASSERT(0);
            wstring str_msg (L"Undefined accent type.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_FAIL, str_msg);
        }
        case AT_A:
        {
            vec_eoStressType.push_back (STRESS_LOCATION_STEM);
            return S_OK;
        }
        case AT_A1:
        {
            if (GENDER_F == eo_gender)
            {
                ATLASSERT (NUM_SG == eo_number);
                vec_eoStressType.push_back (STRESS_LOCATION_STEM);
                vec_eoStressType.push_back (STRESS_LOCATION_ENDING);
                return S_OK;
            }
            if (GENDER_N == eo_gender)
            {
                ATLASSERT (NUM_SG == eo_number);
                vec_eoStressType.push_back (STRESS_LOCATION_STEM);
                return S_OK;
            }
            if (NUM_PL == eo_number)
            {
                vec_eoStressType.push_back (STRESS_LOCATION_STEM);
                return S_OK;
            }

            ATLASSERT(0);
            wstring str_msg (L"Bad arguments.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_INVALIDARG, str_msg);
        }
        case AT_B:
        {
            vec_eoStressType.push_back (STRESS_LOCATION_ENDING);
            return S_OK;
        }
        case AT_B1:
        {
            if (GENDER_F == eo_gender || GENDER_N == eo_gender)
            {
                ATLASSERT (NUM_SG == eo_number);
                vec_eoStressType.push_back (STRESS_LOCATION_ENDING);
                return S_OK;
            }
            if (NUM_PL == eo_number)
            {
                ATLASSERT (GENDER_UNDEFINED == eo_gender);
                vec_eoStressType.push_back (STRESS_LOCATION_STEM);
                vec_eoStressType.push_back (STRESS_LOCATION_ENDING);
                return S_OK;
            }

            ATLASSERT(0);
            wstring str_msg (L"Bad arguments.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_INVALIDARG, str_msg);
        }
        case AT_C:
        {
            if (GENDER_F == eo_gender)
            {
                ATLASSERT (NUM_SG == eo_number);
                vec_eoStressType.push_back (STRESS_LOCATION_ENDING);
                return S_OK;
            }
            if (GENDER_N == eo_gender || NUM_PL == eo_number)
            {
                vec_eoStressType.push_back (STRESS_LOCATION_STEM);
                return S_OK;
            }

            ATLASSERT(0);
            wstring str_msg (L"Bad arguments.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_INVALIDARG, str_msg);
        }
        case AT_C1:
        {
            if (GENDER_F == eo_gender)
            {
                ATLASSERT (NUM_SG == eo_number);
                vec_eoStressType.push_back (STRESS_LOCATION_ENDING);
                return S_OK;
            }
            if (GENDER_N == eo_gender)
            {
                ATLASSERT (NUM_SG == eo_number);
                vec_eoStressType.push_back (STRESS_LOCATION_STEM);
                return S_OK;
            }
            if (NUM_PL == eo_number)
            {
                ATLASSERT (GENDER_UNDEFINED == eo_gender);
                vec_eoStressType.push_back (STRESS_LOCATION_STEM);
                vec_eoStressType.push_back (STRESS_LOCATION_ENDING);
                return S_OK;
            }

            ATLASSERT(0);
            wstring str_msg (L"Bad arguments.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_INVALIDARG, str_msg);
        }
        case AT_C2:
        {
            if (GENDER_F == eo_gender)
            {
                ATLASSERT (NUM_SG == eo_number);
                vec_eoStressType.push_back (STRESS_LOCATION_ENDING);
                return S_OK;
            }
            if (GENDER_N == eo_gender)
            {
                ATLASSERT (NUM_SG == eo_number);
                vec_eoStressType.push_back (STRESS_LOCATION_STEM);
                vec_eoStressType.push_back (STRESS_LOCATION_ENDING);
                return S_OK;
            }
            if (NUM_PL == eo_number)
            {
                ATLASSERT (GENDER_UNDEFINED == eo_gender);
                vec_eoStressType.push_back (STRESS_LOCATION_STEM);
                vec_eoStressType.push_back (STRESS_LOCATION_ENDING);
                return S_OK;
            }

            ATLASSERT(0);
            wstring str_msg (L"Bad arguments.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_INVALIDARG, str_msg);
        }
        default:
        {
            ATLASSERT(0);
            wstring str_msg (L"Illegal accent type.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_INVALIDARG, str_msg);
        }
    }

    throw CT_Exception (E_INVALIDARG, L"");

}   // h_GetStressType()

HRESULT CT_FormBuilderShortAdj::h_CreateFormTemplate (const wstring& str_lemma,
                                                      const wstring& str_ending,
                                                      ET_Gender eo_gender,
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

    pco_wordForm->eo_POS = pco_Lexeme->eo_PartOfSpeech;
    pco_wordForm->eo_Subparadigm = eo_Subparadigm;
    pco_wordForm->str_Lemma = str_lemma;
    pco_wordForm->str_WordForm = str_lemma + str_ending;
    pco_wordForm->eo_Gender = eo_gender;
    pco_wordForm->eo_Number = eo_number;

    v_AssignSecondaryStress (pco_wordForm);

    return S_OK;

}   //  h_CreateFormTemplate (...)

HRESULT CT_FormBuilderShortAdj::h_HandleDeviations()
{
    HRESULT h_r = S_OK;

    bool b_optionalCD = false;
    int i_cd = 1;
    for (; i_cd <= 2; ++i_cd)
    {
        map<int, bool>::iterator it_cd = map_CommonDeviations.find (i_cd);
        if (map_CommonDeviations.end() != it_cd)
        {
            b_optionalCD = (*it_cd).second;
            break;
        }
    }

    if (3 == i_cd)  // only 1 and 2 are valid
    {
        return S_OK;
    }

    //
    // Only adjectives in -nnyj/-nnij or participia
    //
    for (ET_Gender eo_gender = GENDER_UNDEFINED; eo_gender <= GENDER_N; ++eo_gender)
    {
        ET_Number eo_number = (GENDER_UNDEFINED == eo_gender) ? NUM_PL : NUM_SG;

        if (1 == i_cd && GENDER_M != eo_gender)
        {
            continue;
        }

        CT_GramHasher co_hasher (pco_Lexeme->eo_PartOfSpeech, eo_Subparadigm, CASE_UNDEFINED, eo_number, 
                                 eo_gender, TENSE_UNDEFINED, PERSON_UNDEFINED, ANIM_UNDEFINED, REFL_UNDEFINED, 
                                 VOICE_UNDEFINED, ASPECT_UNDEFINED);
        CT_WordForm * pco_template = NULL;
        h_WordFormFromHash (co_hasher.i_GramHash(), 0, pco_template);
        if (NULL == pco_template)
        {
            ATLASSERT(0);
            wstring str_msg (L"Failed to obtain short form.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_POINTER, str_msg);
        }

        if (b_optionalCD)   // store both forms
        {
            CComObject<CT_WordForm> * pco_mVariant = NULL;
            h_CloneWordForm (pco_template, pco_mVariant);
            pco_Lexeme->h_AddWordForm (pco_mVariant);
            pco_template = pco_mVariant;
        }

        if (GENDER_M == eo_gender)
        {
            pco_template->str_WordForm = 
                pco_template->str_WordForm.erase (pco_template->str_WordForm.length()-1);
            if (b_FleetingVowel)
            {
                pco_template->str_WordForm = 
                    pco_template->str_WordForm.erase (pco_template->str_WordForm.length()-1);
            }
        }
        else
        {
            pco_template->str_WordForm = 
                pco_template->str_WordForm.erase (pco_template->str_WordForm.length()-2, 1);
        }
    }

    return S_OK;

}   //  h_HandleDeviations()

HRESULT CT_FormBuilderShortAdj::h_Build()
{
    HRESULT h_r = S_OK;

    try
    {
        h_GetEndings();

        for (ET_Gender eo_gender = GENDER_UNDEFINED; eo_gender <= GENDER_N; ++eo_gender)
        {
            ET_Number eo_number = (GENDER_UNDEFINED == eo_gender) ? NUM_PL : NUM_SG;

            vector<ET_StressLocation> vec_eoStressType;
            h_GetStressTypes (eo_number, eo_gender, vec_eoStressType);

            vector<ET_StressLocation>::iterator it_stressType = vec_eoStressType.begin();
            for (; it_stressType != vec_eoStressType.end(); ++it_stressType)
            {
                ST_EndingDescriptor st_d (eo_gender, eo_number, *it_stressType);
                int i_numEndings = pco_Endings->i_Count (st_d);
                if (i_numEndings < 1)
                {
                    ERROR_LOG (L"No endings");
                    continue;
                }

                for (int i_ending = 0; i_ending < i_numEndings; ++i_ending)
                {
                    std::wstring str_ending;
                    pco_Endings->h_GetEnding (st_d, i_ending, str_ending);
                    wstring str_lemma (str_Lemma);
                    h_FleetingVowelCheck (eo_number, 
                                          CASE_UNDEFINED, 
                                          eo_gender, 
                                          *it_stressType, 
                                          eo_Subparadigm,
                                          str_ending, 
                                          str_lemma);

                    // For participles, stress is assigned in ctor
                    vector<int> vec_iStress;
                    if (STRESS_LOCATION_STEM == *it_stressType)
                    {
                        if (SUBPARADIGM_SHORT_ADJ == eo_Subparadigm)
                        {
                            h_GetStemStressPositions (str_lemma, SUBPARADIGM_SHORT_ADJ, vec_iStress);
                        }
                    }
                    else if (STRESS_LOCATION_ENDING == *it_stressType)
                    {
                        int i_stressPos = -1;
                        h_GetEndingStressPosition (str_lemma, str_ending, i_stressPos);
                        vec_iStress.push_back (i_stressPos);
                    }
                    else
                    {
                        ATLASSERT(0);
                        wstring str_msg (L"Illegal stress type.");
                        ERROR_LOG (L"Illegal stress type.");
                        return E_FAIL;
                    }

                    CComObject<CT_WordForm> * pco_wordForm = NULL;
                    h_CreateFormTemplate (str_lemma, str_ending, eo_gender, eo_number, pco_wordForm);

                    long i_gramHash = pco_wordForm->i_GramHash();
                    if (1 == vec_iStress.size() || b_MultiStress (str_lemma, vec_iStress))
                    {
                        vector<int>::iterator it_stressPos = vec_iStress.begin();
                        for (; it_stressPos != vec_iStress.end(); ++it_stressPos)
                        {
                            pco_wordForm->map_Stress[*it_stressPos] = true;  // primary
                            h_HandleYoAlternation (*it_stressType, *it_stressPos, str_lemma);
                        }
                        pco_wordForm->str_WordForm = str_lemma + str_ending;
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
                            h_HandleYoAlternation (*it_stressType, *it_stressPos, str_lemma);
                            pco_wordForm->str_WordForm = str_lemma + str_ending;
                            pco_Lexeme->h_AddWordForm (pco_wordForm);
                        }
                    }

                }   //  for (int i_ending = 0; i_ending < i_numEndings; ++i_ending)

            }   //  for (; it_stress != vec_eoStress.end(); ++it_stress)

        }   //  for (ET_Gender eo_gender = GENDER_UNDEFINED; ...

        if (!map_CommonDeviations.empty())
        {
            h_HandleDeviations();
        }
    }
    catch (CT_Exception co_ex)
    {
        return co_ex.i_GetErrorCode();  // logging should be always done by callee
    }

    return S_OK;

}   //  h_Build()
