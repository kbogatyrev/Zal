#include "StdAfx.h"
#include "Endings.h"
#include "FormBuilderAdjLong.h"

HRESULT CT_FormBuilderLongAdj::h_GetEndings()
{
    HRESULT h_r = S_OK;

    int i_type = -1;

    wstring str_select (L"SELECT DISTINCT ending, gender, number, case_value, animacy, stress FROM endings");
    str_select += L" WHERE inflection_class = ";

    if (L"мс" == pco_Lexeme->str_InflectionType)
    {
        str_select += str_ToString (ENDING_CLASS_PRONOUN);
    }
    else
    {
        str_select += str_ToString (ENDING_CLASS_ADJECTIVE);
    }

    str_select += L" AND inflection_type = ";
    if ((SUBPARADIGM_PARTICIPLE_PRESENT_ACTIVE == eo_Subparadigm) ||
        (SUBPARADIGM_PARTICIPLE_PAST_ACTIVE == eo_Subparadigm))
    {
        str_select += L"4";
    }
    else
    {
        str_select += str_ToString (pco_Lexeme->i_Type);
    }
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
            pco_Db->v_GetData (3, (int&)st_d.eo_Case);
            pco_Db->v_GetData (4, (int&)st_d.eo_Animacy);
            pco_Db->v_GetData (5, (int&)st_d.eo_Stress);
            HRESULT h_r = pco_Endings->h_AddEnding (str_ending, st_d);
        }
    }
    catch (...)
    {
        v_ReportDbError();    
        throw CT_Exception (E_FAIL, L"Error getting endings.");
    }
    
    return S_OK;
        
}   //  b_GetEndings()

HRESULT CT_FormBuilderLongAdj::h_GetStressPositions (const wstring& str_ending,
                                                     ET_StressLocation eo_stressType,
                                                     vector<int>& vec_iStressPos)
{
    HRESULT h_r = S_OK;

    int i_stressPos = -1;

    if (STRESS_LOCATION_STEM == eo_stressType)
    {
        h_GetStemStressPositions (str_Lemma, eo_Subparadigm, vec_iStressPos);
    }
    else if (STRESS_LOCATION_ENDING == eo_stressType)
    {
        int i_stressPos = -1;
        h_GetEndingStressPosition (str_Lemma, str_ending, i_stressPos);
        vec_iStressPos.push_back (i_stressPos);
    }
    else
    {
        ATLASSERT (0);
        wstring str_msg (L"Illegal stress type.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_INVALIDARG, str_msg);
    }
    if (S_OK != h_r)
    {
        wstring str_msg (L"Error getting stress positions.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_INVALIDARG, str_msg);
    }

    return S_OK;

}   //  h_GetStressPositions (...)

HRESULT CT_FormBuilderLongAdj::h_CreateFormTemplate (ET_Gender eo_gender,
                                                     ET_Number eo_number, 
                                                     ET_Case eo_case, 
                                                     ET_Animacy eo_animacy,
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
    pco_wordForm->str_Lemma = str_Lemma;
    pco_wordForm->eo_Gender = eo_gender;
    pco_wordForm->eo_Case = eo_case;
    pco_wordForm->eo_Number = eo_number;
    pco_wordForm->eo_Animacy = eo_animacy;

    v_AssignStress (pco_wordForm, pco_Lexeme->vec_SecondaryStressPos, false);

    return S_OK;

}   //  h_CreateFormTemplate (...)

HRESULT CT_FormBuilderLongAdj::h_Build()
{
    HRESULT h_r = S_OK;

    try
    {
        h_GetEndings();

        for (ET_Gender eo_gender = GENDER_M; eo_gender <= GENDER_N; ++eo_gender)
            for (ET_Number eo_number = NUM_SG; eo_number <= NUM_PL; ++eo_number)
                for (ET_Case eo_case = CASE_NOM; eo_case < CASE_NUM; ++eo_case)
                    for (ET_Animacy eo_animacy = ANIM_YES; eo_animacy < ANIM_COUNT; ++eo_animacy)
                    {
                        ET_StressLocation eo_stressType = STRESS_LOCATION_UNDEFINED;
                        if (AT_A == eo_AccentType|| AT_A1 == eo_AccentType)
                        {
                            eo_stressType = STRESS_LOCATION_STEM;
                        }
                        else
                        {
                            if (AT_B == eo_AccentType)
                            {
                                eo_stressType = STRESS_LOCATION_ENDING;
                            }
                            else
                            {
                                ERROR_LOG (L"Unknown stress type.");
                                continue;
                            }
                        }

                        ST_EndingDescriptor st_d (eo_gender, eo_number, eo_case, eo_animacy, eo_stressType);
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

                            if (L"мс" == pco_Lexeme->str_InflectionType && pco_Lexeme->b_FleetingVowel)
                            {
                                h_FleetingVowelCheck (eo_number, eo_case, eo_gender, eo_stressType,
                                                      SUBPARADIGM_LONG_ADJ, str_ending, str_Lemma);
                            }

                            CComObject<CT_WordForm> * pco_wordForm = NULL;
                            h_CreateFormTemplate (eo_gender, eo_number, eo_case, eo_animacy, pco_wordForm);
                            pco_wordForm->str_WordForm = str_Lemma + str_ending;

                            vector<int> vec_iStress;
                            h_GetStressPositions (str_ending, eo_stressType, vec_iStress);
                            if (1 == vec_iStress.size() || b_MultiStress (str_Lemma, vec_iStress))
                            {
                                vector<int>::iterator it_stressPos = vec_iStress.begin();
                                for (; it_stressPos != vec_iStress.end(); ++it_stressPos)
                                {
                                    pco_wordForm->map_Stress[*it_stressPos] = true;  // primary
                                }
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
                                    pco_Lexeme->h_AddWordForm (pco_wordForm);
                                }
                            }

                        }   //  for (int i_ending = 0; i_ending < i_numEndings; ++i_ending)
                    }   //  for (ET_Animacy ...)
    }
    catch (CT_Exception co_ex)
    {
        return co_ex.i_GetErrorCode();  // logging should be always done by callee
    }

    return S_OK;

}   //  h_Build (...)

int CT_FormBuilderLongAdj::i_GetStressTemplateFormHash (ET_Subparadigm eo_subparadigm)
{
    int i_hash = -1;

    switch (eo_subparadigm)
    {
        case SUBPARADIGM_PARTICIPLE_PRESENT_ACTIVE:
        {
            CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_SG, 
                                     GENDER_UNDEFINED, TENSE_UNDEFINED, PERSON_UNDEFINED, ANIM_UNDEFINED, 
                                     pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO, VOICE_UNDEFINED, 
                                     ASPECT_UNDEFINED);
            if (4 == pco_Lexeme->i_Type || 5 == pco_Lexeme->i_Type)
            {
                co_hasher.eo_Person = PERSON_1;
            }
            else
            {
                co_hasher.eo_Person = PERSON_3;
            }

            i_hash = co_hasher.i_GramHash();

            break;
        }
        case SUBPARADIGM_PARTICIPLE_PAST_ACTIVE:
        {
            if ((pco_Lexeme->map_CommonDeviations.end() != pco_Lexeme->map_CommonDeviations.find (1)) &&
                (9 != pco_Lexeme->i_Type))
            {
                CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_INFINITIVE, CASE_UNDEFINED, NUM_UNDEFINED, 
                                         GENDER_UNDEFINED, TENSE_UNDEFINED, PERSON_UNDEFINED, ANIM_UNDEFINED, 
                                         pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO, VOICE_UNDEFINED, 
                                         ASPECT_UNDEFINED);
                i_hash = co_hasher.i_GramHash();
            }
            else
            {
                CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_PAST_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_M, 
                                         TENSE_UNDEFINED, PERSON_UNDEFINED, ANIM_UNDEFINED, 
                                         pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO, VOICE_UNDEFINED, 
                                         ASPECT_UNDEFINED);
                i_hash = co_hasher.i_GramHash();
            }

            break;
        }
        default:
        {
            ATLASSERT (0);
            wstring str_msg (L"Bad subparadigm.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_INVALIDARG, str_msg);
        }
    }

    return i_hash;

}   //  i_GetStressTemplateFormHash (...)

HRESULT CT_FormBuilderLongAdj::h_BuildParticiple()
{
    HRESULT h_r = S_OK;

    try
    {
        h_GetEndings();

        for (ET_Gender eo_gender = GENDER_M; eo_gender <= GENDER_N; ++eo_gender)
            for (ET_Number eo_number = NUM_SG; eo_number <= NUM_PL; ++eo_number)
                for (ET_Case eo_case = CASE_NOM; eo_case < CASE_NUM; ++eo_case)
                    for (ET_Animacy eo_animacy = ANIM_YES; eo_animacy < ANIM_COUNT; ++eo_animacy)
                    {
                        ST_EndingDescriptor st_d (eo_gender, eo_number, eo_case, eo_animacy, STRESS_LOCATION_STEM);
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

                            int i_hash = i_GetStressTemplateFormHash (eo_Subparadigm);
                            CT_WordForm * pco_stressTemplate = NULL;
                            int i_nForms = i_NForms (i_hash);
                            for (int i_f = 0; i_f < i_nForms; ++i_f)
                            {
                                CComObject<CT_WordForm> * pco_wordForm = NULL;
                                h_CreateFormTemplate (eo_gender, eo_number, eo_case, eo_animacy, pco_wordForm);
                                pco_wordForm->eo_Reflexive = pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO;
                                pco_wordForm->str_WordForm = str_Lemma + str_ending;
// &&&& TODO reflexive not handled
                                h_WordFormFromHash (i_hash, i_f, pco_stressTemplate);
                                if (NULL == pco_stressTemplate)
                                {
                                    ATLASSERT(0);
                                    wstring str_msg (L"Failed to obtain stress template.");
                                    ERROR_LOG (str_msg);
                                    throw CT_Exception (E_POINTER, str_msg);
                                }
                                pco_wordForm->map_Stress = pco_stressTemplate->map_Stress;
                                if (pco_Lexeme->b_YoAlternation)
                                {
                                    ATLASSERT (7 == pco_Lexeme->i_Type &&   // according to GDRL, p. 85??
                                               CT_ExtString (pco_Lexeme->str_SourceForm).b_EndsWith (L"сти") &&
                                               (L"т" == pco_Lexeme->str_VerbStemAlternation || 
                                                L"д" == pco_Lexeme->str_VerbStemAlternation));
                                    map<int, bool>::iterator it_stress = pco_wordForm->map_Stress.begin();
                                    for (; it_stress != pco_wordForm->map_Stress.end(); ++it_stress)
                                    {
                                        int i_eOffset = str_Lemma.rfind (_T("е"));    // last "e" in graphic stem (?)
                                        if (wstring::npos == i_eOffset)
                                        {
                                            ATLASSERT(0);
                                            wstring str_msg (L"Unstressed stem with yo alternation has no e.");
                                            ERROR_LOG (str_msg);
                                            throw CT_Exception (E_FAIL, str_msg);
                                        }

                                        if ((*it_stress).first == i_eOffset)
                                        {
                                            if ((*it_stress).second)
                                            {
                                                str_Lemma[i_eOffset] = L'ё';
                                            }
                                        }                                        
                                    }
                                }
                                pco_wordForm->str_WordForm = str_Lemma + str_ending;

                                pco_Lexeme->h_AddWordForm (pco_wordForm);
                            }

                        }   //  for (int i_ending = 0; i_ending < i_numEndings; ++i_ending)

                    }   //  for (ET_Animacy ...)
    }
    catch (CT_Exception co_ex)
    {
        return co_ex.i_GetErrorCode();  // logging should be always done by callee
    }

    return S_OK;

}   //  h_Build (...)
