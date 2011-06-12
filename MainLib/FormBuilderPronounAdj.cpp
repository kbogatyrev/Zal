#include "StdAfx.h"
#include "Endings.h"
#include "FormBuilderPronounAdj.h"

int CFormBuilderPronounAdj::i_GramHash (ET_PartOfSpeech eo_pos, ET_Case eo_case, 
                                          ET_Number eo_number, ET_Gender eo_gender, 
                                          ET_Animacy eo_animacy)
{
    CGramHasher co_hasher (eo_pos, 
                             SUBPARADIGM_LONG_ADJ, 
                             eo_case, 
                             eo_number, 
                             eo_gender,
                             PERSON_UNDEFINED,                             
                             eo_animacy, 
                             ASPECT_UNDEFINED, 
                             REFL_UNDEFINED); 

    int i_hash = co_hasher.i_GramHash();
    return i_hash;
}

HRESULT CFormBuilderPronounAdj::hGetEndings()
{
    HRESULT h_r = S_OK;

    int i_type = -1;

    wstring str_select (L"SELECT DISTINCT ending, gender, number, case_value, animacy, stress FROM endings");
    str_select += L" WHERE inflection_class = " + str_ToString(ENDING_CLASS_PRONOUN);
    str_select += L" AND inflection_type = " + str_ToString(pLexeme->i_Type) + L";";

    try
    {
        pco_Db->v_PrepareForSelect(str_select);
        while (pco_Db->b_GetRow())
        {
            wstring str_ending;
            StEndingDescriptor st_d;
            pco_Db->v_GetData (0, str_ending);
            pco_Db->v_GetData (1, (int&)st_d.eo_Gender);
            pco_Db->v_GetData (2, (int&)st_d.eo_Number);
            pco_Db->v_GetData (3, (int&)st_d.eo_Case);
            pco_Db->v_GetData (4, (int&)st_d.eo_Animacy);
            pco_Db->v_GetData (5, (int&)st_d.eo_Stress);
            HRESULT h_r = pco_Endings->hAddEnding(str_ending, st_d);
        }
        pco_Db->v_Finalize();
    }
    catch (...)
    {
        v_ReportDbError();    
        throw CT_Exception (E_FAIL, L"Error getting endings.");
    }
    
    return S_OK;  
}   //  b_GetEndings()

HRESULT CFormBuilderPronounAdj::h_GetStressPositions (const wstring& str_ending,
                                                        ET_StressLocation eo_stressType,
                                                        vector<int>& vec_iStressPos)
{
    HRESULT h_r = S_OK;

    CT_ExtString xstr_lemma (str_Lemma);
    xstr_lemma.v_SetVowels (g_sVowels);

    int i_stressPos = -1;

    if (STRESS_LOCATION_STEM == eo_stressType)
    {
        h_GetStemStressPositions (str_Lemma, eo_Subparadigm, vec_iStressPos);
    }
    else if (STRESS_LOCATION_ENDING == eo_stressType)
    {
        if (CT_ExtString (str_ending).i_NSyllables() < 1)
        {
            i_stressPos = xstr_lemma.i_NSyllables() - 1;
        }
        else
        {
            if (L"мс-п" == pLexeme->m_sInflectionType &&
                (L"его" == str_ending || L"ему" == str_ending ||
                 L"ого" == str_ending || L"ому" == str_ending))
            {
                i_stressPos = xstr_lemma.i_NSyllables() + 1;    // одног<о, твоем<у
            }
            else
            {
                i_stressPos = xstr_lemma.i_NSyllables();
            }
        }

//        hGetEndingStressPosition (str_Lemma, str_ending, i_stressPos);
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

HRESULT CFormBuilderPronounAdj::h_CreateFormTemplate (ET_Gender eo_gender,
                                                        ET_Number eo_number, 
                                                        ET_Case eo_case, 
                                                        ET_Animacy eo_animacy,
                                                        const wstring& str_ending,
                                                        CComObject<CWordForm> *& pco_wordForm)
{
    HRESULT h_r = S_OK;

    h_r = CComObject<CWordForm>::CreateInstance(&pco_wordForm);
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
    pco_wordForm->eo_Subparadigm = SUBPARADIGM_LONG_ADJ;
    pco_wordForm->str_Lemma = str_Lemma;
    pco_wordForm->eo_Gender = eo_gender;
    pco_wordForm->eo_Case = eo_case;
    pco_wordForm->eo_Number = eo_number;
    pco_wordForm->eo_Animacy = eo_animacy;
    pco_wordForm->str_WordForm = str_Lemma + str_ending;
    pco_wordForm->eo_Status = eo_Status;

    v_AssignSecondaryStress (pco_wordForm);

    return S_OK;

}   //  h_CreateFormTemplate (...)

HRESULT CFormBuilderPronounAdj::h_HandleCommonDeviations (CWordForm * pco_wordForm)
{
    return S_OK;
}   //  h_HandleCommonDeviations (...)

HRESULT CFormBuilderPronounAdj::h_Build()
{
    HRESULT h_r = S_OK;

    try
    {
        hGetEndings();

        CT_Hasher co_gram_tmp;
        co_gram_tmp.v_Initialize(POS_PRONOUN_ADJ);
        co_gram_tmp.v_SetParadigm(SUBPARADIGM_LONG_ADJ);
        do
        {
            if (pLexeme->b_HasIrregularForms)
            {
                multimap<int, StIrregularForm>& mmap_if = pLexeme->mmap_IrregularForms;
                int i_hash = i_GramHash(pLexeme->eo_PartOfSpeech,
                                        co_gram_tmp.eo_Case, co_gram_tmp.eo_Number,
                                        co_gram_tmp.eo_Gender, co_gram_tmp.eo_Animacy);
                StIrregularForm st_if;
                h_r = pLexeme->h_GetFirstIrregularForm(i_hash, st_if);
                bool b_skipRegular = false;
                while (S_OK == h_r)
                {
                    if (st_if.b_IsVariant)
                    {
                        b_skipRegular = false;
                    }
                    else
                    {
                        b_skipRegular = true;
                    }
                    CComObject<CWordForm> * pco_wordForm = NULL;
                    h_CreateFormTemplate(co_gram_tmp.eo_Gender, 
                                         co_gram_tmp.eo_Number, 
                                         co_gram_tmp.eo_Case, 
                                         co_gram_tmp.eo_Animacy, 
                                         L"", 
                                         pco_wordForm);
                    pco_wordForm->str_WordForm = st_if.str_Form;
                    pco_wordForm->map_Stress = st_if.map_Stress;
                    pLexeme->h_AddWordForm(pco_wordForm);
                    h_r = pLexeme->h_GetNextIrregularForm(i_hash, st_if);
                }
                if (b_skipRegular)
                {
                    continue;
                }
            }

            ET_StressLocation eo_stressType = STRESS_LOCATION_UNDEFINED;
            switch (pLexeme->eo_AccentType1)
            {
                case AT_A:
                {
                    eo_stressType = STRESS_LOCATION_STEM;
                    break;
                }
                case AT_B:
                {
                    eo_stressType = STRESS_LOCATION_ENDING;
                    break;
                }
                case AT_F:
                {
                    if (NUM_SG == co_gram_tmp.eo_Number)
                    {
                        eo_stressType = STRESS_LOCATION_ENDING;
                    }
                    else if (CASE_NOM == co_gram_tmp.eo_Case)
                    {
                        eo_stressType = STRESS_LOCATION_STEM;
                    }
                    else if (CASE_ACC == co_gram_tmp.eo_Case && ANIM_NO == co_gram_tmp.eo_Animacy)
                    {
                        eo_stressType = STRESS_LOCATION_STEM;
                    }
                    else
                    {
                        eo_stressType = STRESS_LOCATION_ENDING;
                    }
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

            StEndingDescriptor st_d (co_gram_tmp.eo_Gender, co_gram_tmp.eo_Number,
                                      co_gram_tmp.eo_Case, co_gram_tmp.eo_Animacy, eo_stressType);
            int i_numEndings = pco_Endings->i_Count(st_d);
            if (i_numEndings < 1)
            {
                ERROR_LOG (L"No endings");
                continue;
            }

            for (int i_ending = 0; i_ending < i_numEndings; ++i_ending)
            {
                std::wstring str_ending;
                pco_Endings->hGetEnding(st_d, i_ending, str_ending);

                if (pLexeme->b_FleetingVowel &&
                    (co_gram_tmp.eo_Case != CASE_NOM || co_gram_tmp.eo_Gender != GENDER_M) &&
                    (co_gram_tmp.eo_Case != CASE_ACC || co_gram_tmp.eo_Gender != GENDER_M ||
                                                        co_gram_tmp.eo_Animacy != ANIM_NO))
                {
                    h_FleetingVowelCheck(co_gram_tmp.eo_Number, co_gram_tmp.eo_Case,
                                         co_gram_tmp.eo_Gender, eo_stressType,
                                         SUBPARADIGM_LONG_ADJ, str_ending, str_Lemma);
                }

                CComObject<CWordForm> * pco_wordForm = NULL;
                h_CreateFormTemplate(co_gram_tmp.eo_Gender, 
                                     co_gram_tmp.eo_Number, 
                                     co_gram_tmp.eo_Case, 
                                     co_gram_tmp.eo_Animacy, 
                                     str_ending, 
                                     pco_wordForm);
//                            pco_wordForm->str_WordForm = str_Lemma + str_ending;

                vector<int> vec_iStress;
                h_GetStressPositions(str_ending, eo_stressType, vec_iStress);
                if (1 == vec_iStress.size() || b_MultiStress(str_Lemma, vec_iStress))
                {
                    vector<int>::iterator it_stressPos = vec_iStress.begin();
                    for (; it_stressPos != vec_iStress.end(); ++it_stressPos)
                    {
                        pco_wordForm->map_Stress[*it_stressPos] = STRESS_PRIMARY;
                    }
                    pLexeme->h_AddWordForm(pco_wordForm);
                }
                else
                {
                    vector<int>::iterator it_stressPos = vec_iStress.begin();
                    for (; it_stressPos != vec_iStress.end(); ++it_stressPos)
                    {
                        if (it_stressPos != vec_iStress.begin())
                        {
                            CComObject<CWordForm> * pco_wfVariant = NULL;
                            h_CloneWordForm(pco_wordForm, pco_wfVariant);
                            pco_wordForm = pco_wfVariant;
                        }
                        pco_wordForm->map_Stress[*it_stressPos] = STRESS_PRIMARY;
                        pLexeme->h_AddWordForm(pco_wordForm);
                    }
                }
            }
        } while (co_gram_tmp.b_Increment());
    }
    catch (CT_Exception co_ex)
    {
        return co_ex.i_GetErrorCode();  // logging should be always done by callee
    }

    return S_OK;

}   //  h_Build (...)
