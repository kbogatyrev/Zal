#include "StdAfx.h"
#include "Endings.h"
#include "FormBuilderNouns.h"

HRESULT CT_FormBuilderNouns::h_GetEndings()
{
    HRESULT h_r = S_OK;

    if (NULL == pco_Lexeme)
    {
        ATLASSERT(0);
        return E_POINTER;
    }

    if (NULL == pco_Endings)
    {
        ATLASSERT(0);
        return E_POINTER;
    }

    wstring str_select (L"SELECT DISTINCT ending, number, case_value, stress FROM endings");
    str_select += L" WHERE inflection_class = ";
    str_select += str_ToString (ENDING_CLASS_NOUN);
    str_select += L" AND inflection_type = ";
    str_select += str_ToString (pco_Lexeme->i_Type);
    str_select += L" AND (gender = ";
    str_select += str_ToString (pco_Lexeme->eo_GetGender());
    str_select += L" OR gender = ";
    str_select += str_ToString (GENDER_UNDEFINED) + L")";
    str_select += L" AND (animacy = ";
    str_select += str_ToString (pco_Lexeme->eo_GetAnimacy());
    str_select += L" OR animacy = ";
    str_select += str_ToString (ANIM_UNDEFINED) + L")";
    str_select += L" AND stem_augment = ";
    str_select += pco_Lexeme->b_StemAugment ? L"1" : L"0";

    try
    {
        wstring str_selectBase (str_select);
        str_selectBase += L" AND common_deviation = 0;";

        pco_Db->v_PrepareForSelect (str_selectBase);

        while (pco_Db->b_GetRow())
        {
            wstring str_ending;
            ST_EndingDescriptor st_d;
            pco_Db->v_GetData (0, str_ending);
            pco_Db->v_GetData (1, (int&)st_d.eo_Number);
            pco_Db->v_GetData (2, (int&)st_d.eo_Case);
            pco_Db->v_GetData (3, (int&)st_d.eo_Stress);

            St_CommonDeviation st_cd;
            if (S_OK == h_GetCommonDeviation (1, st_cd))
            {
                if (NUM_PL == st_d.eo_Number && CASE_NOM == st_d.eo_Case && GENDER_F != st_d.eo_Gender)
                {
                    if (!st_cd.b_Optional)
                    {
                        continue;
                    }
                }
            }

            if (S_OK == h_GetCommonDeviation (2, st_cd))
            {
                if (NUM_PL == st_d.eo_Number && CASE_GEN == st_d.eo_Case)
                {
                    if (!st_cd.b_Optional)
                    {
                        continue;
                    }
                }
            }

            pco_Endings->h_AddEnding (str_ending, st_d);
        }
    }
    catch (...)
    {
        v_ReportDbError();    
        return E_FAIL;
    }

    for (int i_at = 1; i_at <= 2; ++i_at)
    {
        St_CommonDeviation st_cd;
        if (S_OK != h_GetCommonDeviation (i_at, st_cd))
        {
            continue;
        }

        wstring str_selectCD (str_select);
        str_selectCD += L" AND common_deviation = ";
        str_selectCD += str_ToString (i_at);
        str_select += L";";

        try
        {
            pco_Db->v_PrepareForSelect (str_selectCD);

            while (pco_Db->b_GetRow())
            {
                wstring str_ending;
                ST_EndingDescriptor st_d;
                pco_Db->v_GetData (0, str_ending);
                pco_Db->v_GetData (1, (int&)st_d.eo_Number);
                pco_Db->v_GetData (2, (int&)st_d.eo_Case);
                pco_Db->v_GetData (3, (int&)st_d.eo_Stress);
                pco_Endings->h_AddEnding (str_ending, st_d);
            }
        }
        catch (...)
        {
            v_ReportDbError();        
            return E_FAIL;
        }

    }   // for ...

    return S_OK;

}   //  h_GetEndings()

HRESULT CT_FormBuilderNouns::h_HandleStemAugment (wstring& str_lemma)
{
    if (!pco_Lexeme->b_StemAugment)
    {
        return S_OK;
    }

    if (1 == pco_Lexeme->i_Type)
    {
        str_lemma.erase (str_lemma.length()-2, 2);  // римлянин, южанин, армянин
    }
    if (3 == pco_Lexeme->i_Type)
    {
        CT_ExtString xstr_gs (pco_Lexeme->str_GraphicStem);
        if (xstr_gs.b_EndsWith (L"ёнок"))
        {
            str_lemma.erase (str_lemma.length()-4, 4);  // цыплёнок, опёнок
        }
        if (xstr_gs.b_EndsWith (L"онок"))
        {
            str_lemma.erase (str_lemma.length()-4, 4);  // мышонок
        }
        if (xstr_gs.b_EndsWith (L"ёночек"))
        {
            str_lemma.erase (str_lemma.length()-6, 6);  // цыплёночек
        }
        if (xstr_gs.b_EndsWith (L"оночек"))
        {
            str_lemma.erase (str_lemma.length()-6, 6);  // мышоночек
        }
    }

    return S_OK;

}   //  h_HandleStemAugment (...)

HRESULT CT_FormBuilderNouns::h_GetStressType (ET_Number eo_number, 
                                              ET_Case eo_case, 
                                              ET_EndingStressType& eo_stressType)
{
    if (NUM_UNDEFINED == eo_number)
    {
        ATLASSERT(0);
        return E_INVALIDARG;
    }

// &&&& why default??
    eo_stressType = ENDING_STRESS_UNSTRESSED;

    switch (pco_Lexeme->eo_AccentType1)
    {
        case AT_A:
        {
            eo_stressType = ENDING_STRESS_UNSTRESSED;
            break;
        }
        case AT_B:
        {
            eo_stressType = ENDING_STRESS_STRESSED;
            break;
        }
        case AT_B1:
        {
            if (NUM_SG == eo_number && CASE_INST == eo_case)
            {
                eo_stressType = ENDING_STRESS_UNSTRESSED;
            }
            else
            {
                eo_stressType = ENDING_STRESS_STRESSED;
            }
            break;
        }
        case AT_C:
        {
            if (NUM_SG == eo_number)
            {
                eo_stressType = ENDING_STRESS_UNSTRESSED;
            }
            else
            {
                eo_stressType = ENDING_STRESS_STRESSED;
            }
            break;
        }
        case AT_D:
        {
            if (NUM_SG == eo_number)
            {
                eo_stressType = ENDING_STRESS_STRESSED;
            }
            else
            {
                eo_stressType = ENDING_STRESS_UNSTRESSED;
            }
            break;
        }
        case AT_D1:
        {
            if (GENDER_F != pco_Lexeme->eo_GetGender())
            {
                ATLASSERT(0);
                ERROR_LOG (L"Non-fem. noun has a.p. D1.");
                return E_FAIL;
            }
            if (NUM_SG == eo_number && CASE_ACC == eo_case) // assume fem -a stems only?
            {
                eo_stressType = ENDING_STRESS_UNSTRESSED;
            }
            else if (NUM_SG == eo_number)
            {
                eo_stressType = ENDING_STRESS_STRESSED;
            }
            else
            {
                eo_stressType = ENDING_STRESS_UNSTRESSED;
            }
            break;
        }
        case AT_E:
        {
            if (NUM_SG == eo_number)
            {
                eo_stressType = ENDING_STRESS_UNSTRESSED;
            }
            else if (CASE_NOM == eo_case)
            {
                eo_stressType = ENDING_STRESS_UNSTRESSED;
            }
            else if (CASE_ACC == eo_case && ANIM_NO == pco_Lexeme->eo_GetAnimacy())
            {
                eo_stressType = ENDING_STRESS_UNSTRESSED;
            }
            else
            {
                eo_stressType = ENDING_STRESS_STRESSED;
            }
            break;
        }

        case AT_F:
        {
            if (NUM_SG == eo_number)
            {
                eo_stressType = ENDING_STRESS_STRESSED;
            }
            else if (CASE_NOM == eo_case)
            {
                eo_stressType = ENDING_STRESS_UNSTRESSED;
            }
            else if (CASE_ACC == eo_case && ANIM_NO == pco_Lexeme->eo_GetAnimacy())
            {
                eo_stressType = ENDING_STRESS_UNSTRESSED;
            }
            else
            {
                eo_stressType = ENDING_STRESS_STRESSED;
            }
            break;
        }
        case AT_F1:
        {
            if (GENDER_F != pco_Lexeme->eo_GetGender())
            {
                ATLASSERT(0);                   // assume f -a stems only?
                ERROR_LOG (L"Non-fem. noun has a.p. F1.");
                return E_FAIL;
            }
            if (NUM_PL == eo_number && CASE_NOM == eo_case)
            {
                eo_stressType = ENDING_STRESS_UNSTRESSED;
            }
            else if (NUM_SG == eo_number && CASE_ACC == eo_case) // fem only?
            {
                eo_stressType = ENDING_STRESS_UNSTRESSED;
            }
            else
            {
                eo_stressType = ENDING_STRESS_STRESSED;
            }
            break;
        }
        case AT_F2:
        {
            if (GENDER_F != pco_Lexeme->eo_GetGender() || 8 != pco_Lexeme->i_Type)
            {
                ATLASSERT(0);                   // assume f -i stems only?
                ERROR_LOG (L"Non-fem./type 8 noun has a.p. F2.");
                return E_FAIL;
            }
            if (ANIM_YES == pco_Lexeme->eo_GetAnimacy())
            {
                ATLASSERT(0);                   // inanimate only?
                ERROR_LOG (L"Animate noun has a.p. F2.");
                return E_FAIL;
            }
            if (NUM_PL == eo_number && (CASE_NOM == eo_case || CASE_ACC == eo_case))
            {
                eo_stressType = ENDING_STRESS_UNSTRESSED;
            }
            else if (NUM_SG == eo_number && CASE_INST == eo_case)
            {
                eo_stressType = ENDING_STRESS_UNSTRESSED;
            }
            else
            {
                eo_stressType = ENDING_STRESS_STRESSED;
            }
            break;
        }
        default:
        {
            ERROR_LOG (L"Illegal accent type.");
            return E_FAIL;
        }
    }

    return S_OK;

}   // h_GetStressType()

HRESULT CT_FormBuilderNouns::h_Build()
{
    HRESULT h_r = S_OK;
    
    h_r = h_GetEndings();
    if (S_OK != h_r)
    {
        return h_r;
    }

    for (ET_Number eo_number = NUM_SG; eo_number <= NUM_PL; ++eo_number)
    {
        for (ET_Case eo_case = CASE_NOM; eo_case < CASE_NUM; ++eo_case)
        {
            wstring str_lemma (pco_Lexeme->str_GraphicStem);

            h_r = h_HandleStemAugment (str_lemma);
            if (S_OK != h_r)
            {
                return h_r;
            }

            ET_EndingStressType eo_stress = ENDING_STRESS_UNDEFINED;
            h_r = h_GetStressType (eo_number, eo_case, eo_stress);

            // Handle acc ending
            ET_Case eo_endingCase = eo_case;
            if (CASE_ACC == eo_case)
            {
                if (NUM_SG == eo_number)
                {
                    if (GENDER_M == pco_Lexeme->eo_GetGender())
                    {
                        ANIM_YES == pco_Lexeme->eo_GetAnimacy()
                            ? eo_endingCase = CASE_GEN
                            : eo_endingCase = CASE_NOM;
                    }
                    if (GENDER_N == pco_Lexeme->eo_GetGender())
                    {
                        eo_endingCase = CASE_NOM;
                    }
                }
                if (NUM_PL == eo_number)
                {
                    ANIM_YES == pco_Lexeme->eo_GetAnimacy()
                        ? eo_endingCase = CASE_GEN
                        : eo_endingCase = CASE_NOM;
                }
            }

            int i_numEndings = pco_Endings->i_Count (ST_EndingDescriptor (eo_number, 
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
                h_r = pco_Endings->h_GetEnding (ST_EndingDescriptor (eo_number, eo_endingCase, eo_stress),
                                                i_ending,
                                                str_ending);
                if (S_OK != h_r)
                {
                    ERROR_LOG (L"Error getting ending from hash.");
                    continue;
                }

                if (8 == pco_Lexeme->i_Type)
                {
                    if (wstring::npos == wstring (str_ShSounds + L'ц').find (str_lemma[str_lemma.length()-1]))
                    {
                        if (str_ending.length() > 0)
                        {
                            if (L'а' == str_ending[0])
                            {
                                continue;
                            }
                        }
                    }
                    else
                    {
                        if (str_ending.length() > 0)
                        {
                            if (L'я' == str_ending[0])
                            {
                                continue;
                            }
                        }
                    }
                }

                if (pco_Lexeme->b_FleetingVowel)
                {
                    h_r = h_HandleFleetingVowel (eo_number, 
                                                 eo_endingCase,
                                                 pco_Lexeme->eo_GetGender(), 
                                                 eo_stress,
                                                 SUBPARADIGM_UNDEFINED,
                                                 str_ending,
                                                 str_lemma);
                }

                vector<int> vec_iStress;
                int i_stressPos = -1;
                if (ENDING_STRESS_UNSTRESSED == eo_stress)
                {
                    h_r = h_GetStemStressPosition (str_lemma, SUBPARADIGM_NOUN, vec_iStress);
                    if (S_OK != h_r)
                    {
                        return h_r;
                    }
                }
                else if (ENDING_STRESS_STRESSED == eo_stress)
                {
                    h_r = h_GetEndingStressPosition (str_lemma, str_ending, i_stressPos);
                    if (S_OK != h_r)
                    {
                        return h_r;
                    }
                    vec_iStress.push_back (i_stressPos);
                }
                else
                {
                    ATLASSERT (0);
                    ERROR_LOG (L"Illegal stress type.");
                    return E_INVALIDARG;
                }

                if (vec_iStress.empty())
                {
                    vec_iStress.push_back (-1);
                }

// &&&& distinguish btw stress variants and polytonic forms
                vector<int>::iterator it_stressPos = vec_iStress.begin();
                for (; it_stressPos != vec_iStress.end(); ++it_stressPos)
                {
                    CComObject<CT_WordForm> * pco_wordForm;
                    h_r = CComObject<CT_WordForm>::CreateInstance (&pco_wordForm);
                    if (S_OK != h_r)
                    {
                        return h_r;
                    }
                    pco_wordForm->eo_POS = pco_Lexeme->eo_PartOfSpeech;
                    pco_wordForm->str_WordForm = str_lemma + str_ending;
                    pco_wordForm->eo_Case = eo_case;
                    pco_wordForm->eo_Number = eo_number;
                    pco_wordForm->eo_Animacy = pco_Lexeme->eo_GetAnimacy();

                    if (pco_Lexeme->b_YoAlternation)
                    {
                        wstring str_processedLemma (str_lemma);
                        h_r = h_HandleYoAlternation (eo_stress, 
                                                     *it_stressPos, 
                                                     SUBPARADIGM_NOUN, 
                                                     str_processedLemma);
                        if (S_OK != h_r)
                        {
                            return h_r;
                        }
                        pco_wordForm->str_Lemma = str_processedLemma;
                    }
                    else
                    {
                        pco_wordForm->str_Lemma = str_lemma;
                    }

                    pco_wordForm->vec_Stress.push_back (*it_stressPos);

                    long l_gramHash = 0;
                    pco_wordForm->get_GramHash (&l_gramHash);
                    pco_Lexeme->v_AddWordForm (l_gramHash, pco_wordForm);
                }

            }   //  for (int i_ending = 0; ... )

        }   //  for (ET_Case eo_case = CASE_NOM; ... )

    }   //  for (ET_Number eo_number = NUM_SG; ... )

    return S_OK;

}    //  h_BuildNounForms()
