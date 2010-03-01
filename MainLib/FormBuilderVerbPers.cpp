#include "StdAfx.h"
#include "Endings.h"
#include "FormBuilderVerbPers.h"

HRESULT CT_FormBuilderPersonal::h_GetEndings()
{
    HRESULT h_r = S_OK;

    if (POS_VERB != pco_Lexeme->eo_PartOfSpeech)
    {
        return E_INVALIDARG;
    }

    wstring str_select (L"SELECT DISTINCT ending, person, number, stress, stem_auslaut, conjugation FROM endings");
    str_select += L" WHERE inflection_class = 5;";

    try
    {
        pco_Db->v_PrepareForSelect (str_select);

        while (pco_Db->b_GetRow())
        {
            wstring str_ending;
            ST_EndingDescriptor st_d;
            pco_Db->v_GetData (0, str_ending);
            pco_Db->v_GetData (1, (int&)st_d.eo_Person);
            pco_Db->v_GetData (2, (int&)st_d.eo_Number);
            pco_Db->v_GetData (3, (int&)st_d.eo_Stress);
            pco_Db->v_GetData (4, (int&)st_d.eo_StemAuslaut);
            pco_Db->v_GetData (5, st_d.i_InflectionType);
            pco_Endings->h_AddEnding (str_ending, st_d);
        }
    }
    catch (...)
    {
        v_ReportDbError();    
        return E_FAIL;
    }
    
    return S_OK;

}   //  b_GetEndings()

HRESULT CT_FormBuilderPersonal::h_HandleFleetingVowel (CT_ExtString& xstr_verbForm)
{
    vector<wstring>& vec_alternatingPreverbs = pco_Lexeme->vec_AlternatingPreverbs;
    vector<wstring>& vec_alternatingPreverbsWithVoicing = pco_Lexeme->vec_AlternatingPreverbsWithVoicing;


// types 5, 6, 7, 8, 9, 11, 14
    bool b_preverb = false;
    bool b_voicing = false;

    vector<wstring>::iterator it_p = vec_alternatingPreverbs.begin();
    for (; it_p != vec_alternatingPreverbs.end()&&!b_preverb; ++it_p)
    {
        if (xstr_verbForm.b_StartsWith (*it_p))
        {
            b_preverb = true;
        }
    }

    if (!b_preverb)
    {
        it_p = vec_alternatingPreverbsWithVoicing.begin();
        for (; it_p != vec_alternatingPreverbsWithVoicing.end()&&!b_preverb; ++it_p)
        {
            if (xstr_verbForm.b_StartsWith (*it_p))
            {
                b_preverb = true;
                b_voicing = true;
            }
        }
    }

    if (!b_preverb)
    {
        return S_FALSE;
    }

    wstring str_preverb = *it_p;

    if (xstr_verbForm.length() < str_preverb.length() + 2)
    {
        ATLASSERT(0);
        ERROR_LOG (L"Stem too short.");
        return E_INVALIDARG;
    }

    // o after prefix?
    if (xstr_verbForm[str_preverb.length()-1] == L'о')
    {
        // Remove o if followed by CV
        if ((str_Consonants.find (xstr_verbForm[str_preverb.length()]) != wstring::npos) &&
            (str_Vowels.find (xstr_verbForm[str_preverb.length()+1]) != wstring::npos))
        {
            xstr_verbForm.erase (xstr_verbForm.begin() + str_preverb.length()-1);
        }
    }
    else
    {
        // Insert o if followed by CC or Cь
        if ((str_Consonants.find (xstr_verbForm[str_preverb.length()]) != wstring::npos) &&
            (wstring (str_Consonants + L'ь').find (xstr_verbForm[str_preverb.length()+1]) != wstring::npos))
        {
            xstr_verbForm.insert (xstr_verbForm.begin() + str_preverb.length(), L'о');
        }
    }

    return S_OK;

}   //  h_HandleFleetingVowel (...)

HRESULT CT_FormBuilderPersonal::h_Build()
{
    HRESULT h_r = S_OK;

    h_r = h_GetEndings();
    if (S_OK != h_r)
    {
        return h_r;
    }

    CT_ExtString& xstr_1SgStem = pco_Lexeme->xstr_1SgStem;
    CT_ExtString& xstr_3SgStem = pco_Lexeme->xstr_3SgStem;
    int i_type = pco_Lexeme->i_Type;

    for (ET_Number eo_number = NUM_SG; eo_number <= NUM_PL; ++eo_number)
    {
        for (ET_Person eo_person = PERSON_1; eo_person <= PERSON_3; ++eo_person)
        {
            // Choose stress type
            ET_EndingStressType eo_stress = ENDING_STRESS_UNDEFINED;
            switch (pco_Lexeme->eo_AccentType1)
            {
                case AT_A:
                {
                    eo_stress = ENDING_STRESS_UNSTRESSED;
                    break;
                }
                case AT_B:
                {
                    eo_stress = ENDING_STRESS_STRESSED;
                    break;
                }
                case AT_C:
                {
                    if (NUM_SG == eo_number && PERSON_1 == eo_person)
                    {
                        eo_stress = ENDING_STRESS_STRESSED;
                    }
                    else
                    {
                        eo_stress = ENDING_STRESS_UNSTRESSED;
                    }
                    break;
                }
                default:
                {
                    ERROR_LOG (L"Incompatible stress type.");
                    continue;
                }
            }
        
            // Handle auslaut
            ET_StemAuslaut eo_auslaut = STEM_AUSLAUT_UNDEFINED;
            if (i_type >= 4 && i_type <= 6)
            {
                if ((PERSON_1 == eo_person) && (NUM_SG == eo_number))
                {
                    if (xstr_1SgStem.b_EndsWithOneOf (str_ShSounds))
                    {
                        eo_auslaut = STEM_AUSLAUT_SH;
                    }
                    else
                    {
                        eo_auslaut = STEM_AUSLAUT_NOT_SH;
                    }
                }
                if ((PERSON_3 == eo_person) && (NUM_PL == eo_number))
                {
                    if (xstr_3SgStem.b_EndsWithOneOf (str_ShSounds))
                    {
                        eo_auslaut = STEM_AUSLAUT_SH;
                    }
                    else
                    {
                        eo_auslaut = STEM_AUSLAUT_NOT_SH;
                    }
                }
            }

            int i_conjugation = INFLECTION_TYPE_UNDEFINED;
            if ((4 == i_type || 5 == i_type) || 
                (PERSON_1 == eo_person && NUM_SG == eo_number) || 
                (PERSON_3 == eo_person && NUM_PL == eo_number))
            {
                i_conjugation = i_type;
            }

            ST_EndingDescriptor st_d (i_conjugation, eo_person, eo_number, eo_stress, eo_auslaut);
            int i_numEndings = pco_Endings->i_Count (st_d);
            if (i_numEndings < 1)
            {
                ERROR_LOG (L"No endings");
                continue;
            }

            for (int i_ending = 0; i_ending < i_numEndings; ++i_ending)
            {
                std::wstring str_ending;
                h_r = pco_Endings->h_GetEnding (st_d, i_ending, str_ending);
                if (S_OK != h_r)
                {
                    ERROR_LOG (L"Error getting ending from hash.");
                    
                }

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

                CT_ExtString xstr_lemma;
                if (PERSON_1 == eo_person && NUM_SG == eo_number)
                {
                    xstr_lemma = pco_Lexeme->xstr_1SgStem;
                }
                else
                {
                    if (4 == pco_Lexeme->i_Type || 5 == pco_Lexeme->i_Type)
                    {
                        xstr_lemma = pco_Lexeme->xstr_3SgStem;
                    }
                    else
                    {
                        if (PERSON_3 == eo_person && NUM_PL == eo_number)
                        {
                            xstr_lemma = xstr_1SgStem;
                        }
                        else
                        {
                            xstr_lemma = xstr_3SgStem;
                        }
                    }
                }

                vector<int> vec_iStress;
                int i_stressPos = -1;

                if (ENDING_STRESS_UNSTRESSED == eo_stress)
                {
                    h_r = h_GetStemStressPositions (xstr_lemma, vec_iStress);
                    if (S_OK != h_r)
                    {
                        return h_r;
                    }
                }
                else if (ENDING_STRESS_STRESSED == eo_stress)
                {
                    h_r = h_GetEndingStressPosition (xstr_lemma, str_ending, i_stressPos);
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

                CT_ExtString xstr_wf = xstr_lemma + str_ending;
                if (pco_Lexeme->b_FleetingVowel)
                {
                    h_r = h_HandleFleetingVowel (xstr_wf);
                    if (S_OK != h_r)
                    {
                        continue;
                    }
                }

                vector<int>::iterator it_stressPos = vec_iStress.begin();
                for (; it_stressPos != vec_iStress.end(); ++it_stressPos)
                {
                    CComObject<CT_WordForm> * pco_wordForm;
                    h_r = CComObject<CT_WordForm>::CreateInstance (&pco_wordForm);
                    if (S_OK != h_r)
                    {
                        return h_r;
                    }

                    pco_wordForm->str_Lemma = xstr_lemma;
                    pco_wordForm->eo_POS = pco_Lexeme->eo_PartOfSpeech;
                    pco_wordForm->eo_Subparadigm = SUBPARADIGM_PRESENT_TENSE;
                    pco_wordForm->eo_Reflexive = pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO;
                    pco_wordForm->str_Lemma = xstr_lemma;
                    pco_wordForm->str_WordForm = xstr_wf;
                    pco_wordForm->eo_Person = eo_person;
                    pco_wordForm->eo_Number = eo_number;
                    pco_wordForm->vec_Stress.push_back (*it_stressPos);

                    long l_gramHash = 0;
                    pco_wordForm->get_GramHash (&l_gramHash);
                    pco_Lexeme->v_AddWordForm (l_gramHash, pco_wordForm);
                }

            }   //  for (int i_ending = 0; ... )

        }   //  for (ET_Person eo_person = PERSON_1; ... )

    }   //  for (ET_Number eo_number = NUM_SG; ... )

    return h_r;

}   //  h_Build()
