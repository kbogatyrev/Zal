#include "StdAfx.h"
#include "Endings.h"
#include "FormBuilderNouns.h"

int CFormBuilderNouns::i_GramHash (ET_Gender eo_gender,
                                     ET_Animacy eo_animacy,
                                     ET_Case eo_case,
                                     ET_Number eo_number)
{
    CGramHasher co_hasher (eo_gender,
                             eo_animacy,
                             eo_case,
                             eo_number);

    int i_hash = co_hasher.i_GramHash();

    return i_hash;
}

HRESULT CFormBuilderNouns::hGetEndings()
{
    HRESULT h_r = S_OK;

    if (NULL == pLexeme)
    {
        ATLASSERT(0);
        wstring str_msg (L"Null pointer to lexeme object.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_POINTER, str_msg);
    }

    if (NULL == pco_Endings)
    {
        ATLASSERT(0);
        wstring str_msg (L"Null pointer to endings object.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_POINTER, str_msg);
    }

    wstring str_select (L"SELECT DISTINCT ending, number, case_value, stress FROM endings");
    str_select += L" WHERE inflection_class = ";
    str_select += str_ToString (ENDING_CLASS_NOUN);
    str_select += L" AND inflection_type = ";
    str_select += str_ToString (pLexeme->i_Type);
    str_select += L" AND (gender = ";
    str_select += str_ToString (pLexeme->eo_GetGender());
    str_select += L" OR gender = ";
    str_select += str_ToString (GENDER_UNDEFINED) + L")";
    str_select += L" AND (animacy = ";
    str_select += str_ToString (pLexeme->eo_GetAnimacy());
    str_select += L" OR animacy = ";
    str_select += str_ToString (ANIM_UNDEFINED) + L")";
    str_select += L" AND stem_augment = ";
//    str_select += pLexeme->b_StemAugment ? L"1" : L"0";
    str_select += str_ToString (pLexeme->i_StemAugment);

    try
    {
        wstring str_selectBase (str_select);
        str_selectBase += L" AND common_deviation = 0;";

        pco_Db->v_PrepareForSelect (str_selectBase);
        while (pco_Db->b_GetRow())
        {
            wstring str_ending;
            StEndingDescriptor st_d;
            pco_Db->v_GetData (0, str_ending);
            pco_Db->v_GetData (1, (int&)st_d.eo_Number);
            pco_Db->v_GetData (2, (int&)st_d.eo_Case);
            pco_Db->v_GetData (3, (int&)st_d.eo_Stress);

            map<int, bool>::iterator it_cd = pLexeme->map_CommonDeviations.find (1);
            if (pLexeme->map_CommonDeviations.end() != it_cd)
            {
                if (NUM_PL == st_d.eo_Number && CASE_NOM == st_d.eo_Case && GENDER_F != st_d.eo_Gender)
                {
					if (!(*it_cd).second)	// optional?
                    {
                        continue;
                    }
                }
            }

            it_cd = pLexeme->map_CommonDeviations.find (2);
            if (pLexeme->map_CommonDeviations.end() != it_cd)
            {
                if (NUM_PL == st_d.eo_Number && CASE_GEN == st_d.eo_Case)
                {
					if (!(*it_cd).second)	// optional?
                    {
                        continue;
                    }
                }
            }
            pco_Endings->hAddEnding (str_ending, st_d);

        }   //  while (pco_Db->b_GetRow())

        pco_Db->v_Finalize();
    }
    catch (...)
    {
        v_ReportDbError();    
        throw CT_Exception (E_FAIL, L"DB error");
    }

    for (int i_cd = 1; i_cd <= 2; ++i_cd)
    {
        map<int, bool>::iterator it_cd = pLexeme->map_CommonDeviations.find (i_cd);
        if (pLexeme->map_CommonDeviations.end() == it_cd)
        {
            continue;
        }

        wstring str_selectCD (str_select);
        str_selectCD += L" AND common_deviation = ";
        str_selectCD += str_ToString (i_cd);
        str_selectCD += L";";

        try
        {
            pco_Db->v_PrepareForSelect (str_selectCD);
            while (pco_Db->b_GetRow())
            {
                wstring str_ending;
                StEndingDescriptor st_d;
                pco_Db->v_GetData (0, str_ending);
                pco_Db->v_GetData (1, (int&)st_d.eo_Number);
                pco_Db->v_GetData (2, (int&)st_d.eo_Case);
                pco_Db->v_GetData (3, (int&)st_d.eo_Stress);
                pco_Endings->hAddEnding (str_ending, st_d);
            }
            pco_Db->v_Finalize();
        }
        catch (...)
        {
            v_ReportDbError();        
            throw CT_Exception (E_FAIL, L"DB error");
        }

    }   // for ...

    return S_OK;

}   //  hGetEndings()

HRESULT CFormBuilderNouns::h_HandleStemAugment (wstring& str_lemma, ET_Number eo_number, ET_Case eo_case)
{
    if (pLexeme->i_StemAugment < 1)
    {
        return S_OK;
    }

    if (1 == pLexeme->i_Type)
    {
        str_lemma.erase (str_lemma.length()-2, 2);  // римлянин, южанин, армянин
    }
    if (3 == pLexeme->i_Type)
    {
        CT_ExtString& xstr_gs = pLexeme->xstr_GraphicStem;
        if (NUM_SG == eo_number)
        {
            if ((CASE_NOM == eo_case) || 
                (ANIM_NO == pLexeme->eo_GetAnimacy() && CASE_ACC == eo_case))
            {
                return S_OK;
            }
            else
            {
                str_lemma.erase (str_lemma.length()-2, 1);
                return S_OK;
            }
        }
        if (NUM_PL == eo_number)
        {
            if (xstr_gs.b_EndsWith (L"онок"))
            {
                str_lemma.erase (str_lemma.length()-4, 4);
                str_lemma += L"ат";
                return S_OK;
            }
            if (xstr_gs.b_EndsWith (L"ёнок"))
            {
                str_lemma.erase (str_lemma.length()-4, 4);
                str_lemma += L"ят";
                return S_OK;
            }
            if (xstr_gs.b_EndsWith (L"оночек"))
            {
                str_lemma.erase (str_lemma.length()-6, 6);
                if ((CASE_GEN == eo_case) ||                        
                    (CASE_ACC == eo_case && ANIM_YES == pLexeme->eo_GetAnimacy()))
                                                     // they all should be animate?
                {
                    str_lemma += L"аток";
                }
                else
                {
                    str_lemma += L"атк";
                }
                return S_OK;
            }
            if (xstr_gs.b_EndsWith (L"ёночек"))
            {
                str_lemma.erase (str_lemma.length()-6, 6);
                if ((CASE_GEN == eo_case) ||                        
                    (CASE_ACC == eo_case && ANIM_YES == pLexeme->eo_GetAnimacy()))
                                                     // they all should be animate?
                {
                    str_lemma += L"яток";
                }
                else
                {
                    str_lemma += L"ятк";
                }
                return S_OK;
            }
        }
    }       //  if (3 == pLexeme->i_Type)

    return S_OK;

}   //  h_HandleStemAugment (...)

HRESULT CFormBuilderNouns::h_GetStressType (ET_Number eo_number, ET_Case eo_case, ET_StressLocation& eo_stressType)
{
    if (NUM_UNDEFINED == eo_number)
    {
        ATLASSERT(0);
        wstring str_msg (L"Undefined number.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_INVALIDARG, str_msg);
    }

// &&&& why default??
    eo_stressType = STRESS_LOCATION_STEM;

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
        case AT_B1:
        {
            if (NUM_SG == eo_number && CASE_INST == eo_case)
            {
                eo_stressType = STRESS_LOCATION_STEM;
            }
            else
            {
                eo_stressType = STRESS_LOCATION_ENDING;
            }
            break;
        }
        case AT_C:
        {
            if (NUM_SG == eo_number)
            {
                eo_stressType = STRESS_LOCATION_STEM;
            }
            else
            {
                eo_stressType = STRESS_LOCATION_ENDING;
            }
            break;
        }
        case AT_D:
        {
            if (NUM_SG == eo_number)
            {
                eo_stressType = STRESS_LOCATION_ENDING;
            }
            else
            {
                eo_stressType = STRESS_LOCATION_STEM;
            }
            break;
        }
        case AT_D1:
        {
            if (GENDER_F != pLexeme->eo_GetGender())
            {
                ATLASSERT(0);
                wstring str_msg (L"Non-feminine noun with D1 stress.");
                ERROR_LOG (str_msg);
                throw CT_Exception (E_FAIL, str_msg);
            }
            if (NUM_SG == eo_number && CASE_ACC == eo_case) // assume fem -a stems only?
            {
                eo_stressType = STRESS_LOCATION_STEM;
            }
            else if (NUM_SG == eo_number)
            {
                eo_stressType = STRESS_LOCATION_ENDING;
            }
            else
            {
                eo_stressType = STRESS_LOCATION_STEM;
            }
            break;
        }
        case AT_E:
        {
            if (NUM_SG == eo_number)
            {
                eo_stressType = STRESS_LOCATION_STEM;
            }
            else if (CASE_NOM == eo_case)
            {
                eo_stressType = STRESS_LOCATION_STEM;
            }
            else if (CASE_ACC == eo_case && ANIM_NO == pLexeme->eo_GetAnimacy())
            {
                eo_stressType = STRESS_LOCATION_STEM;
            }
            else
            {
                eo_stressType = STRESS_LOCATION_ENDING;
            }
            break;
        }

        case AT_F:
        {
            if (NUM_SG == eo_number)
            {
                eo_stressType = STRESS_LOCATION_ENDING;
            }
            else if (CASE_NOM == eo_case)
            {
                eo_stressType = STRESS_LOCATION_STEM;
            }
            else if (CASE_ACC == eo_case && ANIM_NO == pLexeme->eo_GetAnimacy())
            {
                eo_stressType = STRESS_LOCATION_STEM;
            }
            else
            {
                eo_stressType = STRESS_LOCATION_ENDING;
            }
            break;
        }
        case AT_F1:
        {
            if (GENDER_F != pLexeme->eo_GetGender())
            {
                ATLASSERT(0);
                wstring str_msg (L"Non-feminine noun with F1 stress.");
                ERROR_LOG (str_msg);
                throw CT_Exception (E_FAIL, str_msg);
            }
            if (NUM_PL == eo_number && CASE_NOM == eo_case)
            {
                eo_stressType = STRESS_LOCATION_STEM;
            }
            else if (NUM_SG == eo_number && CASE_ACC == eo_case) // fem only?
            {
                eo_stressType = STRESS_LOCATION_STEM;
            }
            else
            {
                eo_stressType = STRESS_LOCATION_ENDING;
            }
            break;
        }
        case AT_F2:
        {
            if (GENDER_F != pLexeme->eo_GetGender() || 8 != pLexeme->i_Type)
            {
                ATLASSERT(0);   // assume f -i stems only?
                wstring str_msg (L"Non-feminine/type 8 noun with F2 stress.");
                ERROR_LOG (str_msg);
                throw CT_Exception (E_FAIL, str_msg);
            }
            if (ANIM_YES == pLexeme->eo_GetAnimacy())
            {
                ATLASSERT(0);                   // inanimate only?
                wstring str_msg (L"Animate noun with F2 stress.");
                ERROR_LOG (str_msg);
                throw CT_Exception (E_FAIL, str_msg);
            }
            if (NUM_PL == eo_number && (CASE_NOM == eo_case || CASE_ACC == eo_case))
            {
                eo_stressType = STRESS_LOCATION_STEM;
            }
            else if (NUM_SG == eo_number && CASE_INST == eo_case)
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

    return S_OK;

}   // h_GetStressType()

HRESULT CFormBuilderNouns::h_HandleAccEnding (ET_Number eo_number, ET_Case& eo_case)
{
    if (NUM_SG == eo_number)
    {
        if (GENDER_M == pLexeme->eo_GetGender())
        {
            ANIM_YES == pLexeme->eo_GetAnimacy()
                ? eo_case = CASE_GEN
                : eo_case = CASE_NOM;
        }
        if (GENDER_N == pLexeme->eo_GetGender())
        {
            eo_case = CASE_NOM;
        }
    }
    if (NUM_PL == eo_number)
    {
        ANIM_YES == pLexeme->eo_GetAnimacy()
            ? eo_case = CASE_GEN
            : eo_case = CASE_NOM;
    }

    return S_OK;

}   //  h_HandleAccEnding

HRESULT CFormBuilderNouns::h_GetStressPositions (const wstring& str_lemma, 
                                                   const wstring& str_ending,
                                                   ET_StressLocation eo_stressType,
                                                   vector<int>& vec_iStressPos)
{
    HRESULT h_r = S_OK;

    int i_stressPos = -1;
    if (STRESS_LOCATION_STEM == eo_stressType)
    {
        h_GetStemStressPositions (str_lemma, SUBPARADIGM_NOUN, vec_iStressPos);
    }
    else if (STRESS_LOCATION_ENDING == eo_stressType)
    {
        hGetEndingStressPosition (str_lemma, str_ending, i_stressPos);
        vec_iStressPos.push_back (i_stressPos);
    }
    else
    {
        ATLASSERT(0);
        wstring str_msg (L"Bad stress type.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_FAIL, str_msg);
    }

    if (vec_iStressPos.empty())
    {
        vec_iStressPos.push_back (-1);
    }

    return S_OK;

}   //  h_GetStressPositions (...)

HRESULT CFormBuilderNouns::h_CreateFormTemplate (ET_Number eo_number, 
                                                   ET_Case eo_case, 
                                                   const wstring& str_lemma,
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
    pco_wordForm->eo_Subparadigm = SUBPARADIGM_NOUN;
    pco_wordForm->eo_Case = eo_case;
    pco_wordForm->str_Lemma = str_lemma;
    pco_wordForm->eo_Number = eo_number;
    pco_wordForm->eo_Gender = pLexeme->eo_GetGender();
    pco_wordForm->eo_Animacy = pLexeme->eo_GetAnimacy();
    v_AssignSecondaryStress (pco_wordForm);

    return h_r;

}   //  h_CreateFormTemplate (...)

HRESULT CFormBuilderNouns::h_Assemble (CWordForm * pco_wordForm, 
                                         ET_StressLocation eo_stressLocation,
                                         int i_stressPos, 
                                         const wstring& str_ending)
{
    pco_wordForm->map_Stress[i_stressPos] = STRESS_PRIMARY;  // primary
    h_HandleYoAlternation (eo_stressLocation, i_stressPos, pco_wordForm->str_Lemma);
    pco_wordForm->str_WordForm = pco_wordForm->str_Lemma + str_ending;

    return S_OK;
}

HRESULT CFormBuilderNouns::h_CheckIrregularForms (ET_Gender eo_gender, 
                                                    ET_Animacy eo_animacy, 
                                                    ET_Case eo_case, 
                                                    ET_Number eo_number, 
                                                    bool& b_handled)
{
    HRESULT h_r = S_OK;

    b_handled = false;

    multimap<int, StIrregularForm>& mmap_if = pLexeme->mmap_IrregularForms;
    int i_hash = i_GramHash (eo_gender, eo_animacy, eo_case, eo_number);
    StIrregularForm st_if;
    h_r = pLexeme->h_GetFirstIrregularForm (i_hash, st_if);
    while (S_OK == h_r)
    {
        if (st_if.b_IsVariant)
        {
            b_handled = false;
        }
        else
        {
            b_handled = true;
        }
        CComObject<CWordForm> * pco_wordForm = NULL;
        h_CreateFormTemplate (eo_number, eo_case, pLexeme->xstr_GraphicStem.c_str(), pco_wordForm);
        pco_wordForm->str_WordForm = st_if.str_Form;
        pco_wordForm->map_Stress = st_if.map_Stress;
        pLexeme->h_AddWordForm (pco_wordForm);
        h_r = pLexeme->h_GetNextIrregularForm (i_hash, st_if);
    }

    return h_r;

}   //  h_CheckIrregularForms (...)

HRESULT CFormBuilderNouns::h_Build()
{
    HRESULT h_r = S_OK;

    try
    {
        h_r = hGetEndings();

        ET_Animacy eo_animacy = pLexeme->eo_GetAnimacy();
        ET_Gender eo_gender = pLexeme->eo_GetGender();

        CT_Hasher co_gram_tmp;
        co_gram_tmp.v_Initialize(eo_gender, eo_animacy);
        do
        {
            wstring str_lemma (pLexeme->xstr_GraphicStem.c_str());
            if (pLexeme->b_HasIrregularForms)
            {
                bool b_skipRegular = false;
                h_CheckIrregularForms (co_gram_tmp.eo_Gender,
                                       co_gram_tmp.eo_Animacy,
                                       co_gram_tmp.eo_Case,
                                       co_gram_tmp.eo_Number,
                                       b_skipRegular);
                if (b_skipRegular)
                {
                    continue;
                }
            }

            h_r = h_HandleStemAugment (str_lemma, co_gram_tmp.eo_Number, co_gram_tmp.eo_Case);

            ET_StressLocation eo_stress = STRESS_LOCATION_UNDEFINED;
            h_GetStressType (co_gram_tmp.eo_Number, co_gram_tmp.eo_Case, eo_stress);

            // Handle acc ending
            ET_Case eo_endingCase = co_gram_tmp.eo_Case;
            if (CASE_ACC == co_gram_tmp.eo_Case)
            {
                h_HandleAccEnding (co_gram_tmp.eo_Number, eo_endingCase);
            }

            int i_numEndings = pco_Endings->i_Count (StEndingDescriptor (co_gram_tmp.eo_Number, 
                                                                          eo_endingCase, 
                                                                          eo_stress));
            if (i_numEndings < 1)
            {
//                ATLASSERT(0);
                ERROR_LOG (L"No endings");
                continue;
            }

            for (int i_ending = 0; i_ending < i_numEndings; ++i_ending)
            {
                // Get ending and modify as necessary
                std::wstring str_ending;
                h_r = pco_Endings->hGetEnding (StEndingDescriptor (co_gram_tmp.eo_Number, eo_endingCase, eo_stress),
                                                i_ending,
                                                str_ending);
                if (8 == pLexeme->i_Type)
                {
                    if (wstring::npos == wstring (str_ShSounds + L'ц').find (str_lemma[str_lemma.length()-1]))
                    {
                        if (CT_ExtString (str_ending).b_StartsWith (L"а"))
                        {
                            continue;
                        }
                    }
                    else
                    {
                        if (CT_ExtString (str_ending).b_StartsWith (L"я"))
                        {
                            continue;
                        }
                    }
                }

                bool b_hasFleetingVowel = false;
                h_r = h_FleetingVowelCheck (co_gram_tmp.eo_Number, 
                                            eo_endingCase,
                                            co_gram_tmp.eo_Gender, 
                                            eo_stress,
                                            SUBPARADIGM_NOUN,
                                            str_ending,
                                            str_lemma);

                vector<int> vec_iStress;
                h_GetStressPositions (str_lemma, str_ending, eo_stress, vec_iStress);
                CComObject<CWordForm> * pco_wordForm = NULL;
                h_CreateFormTemplate (co_gram_tmp.eo_Number, co_gram_tmp.eo_Case, str_lemma, pco_wordForm);
                if (1 == vec_iStress.size() || b_MultiStress (str_lemma, vec_iStress))
                {
                    vector<int>::iterator it_stressPos = vec_iStress.begin();
                    for (; it_stressPos != vec_iStress.end(); ++it_stressPos)
                    {
                        h_Assemble (pco_wordForm, eo_stress, *it_stressPos, str_ending);
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
                        h_Assemble (pco_wordForm, eo_stress, *it_stressPos, str_ending);
                        pLexeme->h_AddWordForm (pco_wordForm);
                    }
                }
            }   //  for (int i_ending = 0; ... )
        } while (co_gram_tmp.b_Increment());
    }
    catch (CT_Exception co_ex)
    {
        return co_ex.i_GetErrorCode();  // logging should be always done by callee
    }

    return S_OK;

}    //  h_BuildNounForms()
