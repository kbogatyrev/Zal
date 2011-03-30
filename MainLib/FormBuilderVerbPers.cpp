#include "StdAfx.h"
#include "Endings.h"
#include "FormBuilderVerbPers.h"

HRESULT CT_FormBuilderPersonal::h_GetEndings()
{
    HRESULT h_r = S_OK;

    if (POS_VERB != pco_Lexeme->eo_PartOfSpeech)
    {
        ERROR_LOG (L"Bad part of speech value.");
        return E_INVALIDARG;
    }

    wstring str_select 
        (L"SELECT DISTINCT ending, person, number, stress, stem_auslaut, conjugation FROM endings");
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
        pco_Db->v_Finalize();
    }
    catch (...)
    {
        v_ReportDbError();    
        return E_FAIL;
    }
    
    return S_OK;

}   //  b_GetEndings()

HRESULT CT_FormBuilderPersonal::h_GetStressType (ET_Number eo_number, 
                                                 ET_Person eo_person,  
                                                 ET_StressLocation& eo_stress)
{
    HRESULT h_r = S_OK;

    switch (pco_Lexeme->eo_AccentType1)
    {
        case AT_A:
        {
            eo_stress = STRESS_LOCATION_STEM;
            break;
        }
        case AT_B:
        {
            eo_stress = STRESS_LOCATION_ENDING;
            break;
        }
        case AT_C:
        {
            if (NUM_SG == eo_number && PERSON_1 == eo_person)
            {
                eo_stress = STRESS_LOCATION_ENDING;
            }
            else
            {
                eo_stress = STRESS_LOCATION_STEM;
            }
            break;
        }
        default:
        {
            ATLASSERT(0);
            wstring str_msg (L"Incompatible stress type.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_FAIL, str_msg);
        }
    }

    return h_r;

}   //  h_GetStressType (...)

HRESULT CT_FormBuilderPersonal::h_GetAuslautType (ET_Number eo_number, 
                                                  ET_Person eo_person, 
                                                  ET_StemAuslaut& eo_auslaut)
{
    HRESULT h_r = S_OK;

    if (pco_Lexeme->i_Type >= 4 && pco_Lexeme->i_Type <= 6)
    {
        if ((PERSON_1 == eo_person) && (NUM_SG == eo_number))
        {
            if (pco_Lexeme->xstr_1SgStem.b_EndsWithOneOf (str_ShSounds))
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
            if (pco_Lexeme->xstr_3SgStem.b_EndsWithOneOf (str_ShSounds))
            {
                eo_auslaut = STEM_AUSLAUT_SH;
            }
            else
            {
                eo_auslaut = STEM_AUSLAUT_NOT_SH;
            }
        }
    }

    return S_OK;

}   //  h_GetAuslautType (...)

HRESULT CT_FormBuilderPersonal::h_GetLemma (ET_Number eo_number, ET_Person eo_person, wstring& str_lemma)
{
    HRESULT h_r = S_OK;

    if (PERSON_1 == eo_person && NUM_SG == eo_number)
    {
        str_lemma = pco_Lexeme->xstr_1SgStem.c_str();
    }
    else
    {
        if (4 == pco_Lexeme->i_Type || 5 == pco_Lexeme->i_Type)
        {
            str_lemma = pco_Lexeme->xstr_3SgStem.c_str();
        }
        else
        {
            if (PERSON_3 == eo_person && NUM_PL == eo_number)
            {
                str_lemma = pco_Lexeme->xstr_1SgStem.c_str();
            }
            else
            {
                str_lemma = pco_Lexeme->xstr_3SgStem.c_str();
            }
        }
    }

    return h_r;

}       //  h_GetLemma (...)

HRESULT CT_FormBuilderPersonal::h_GetStressPositions (const wstring& str_lemma,
                                                      const wstring& str_ending,
                                                      ET_StressLocation eo_stressType,
                                                      vector<int>& vec_iStressPositions)
{
    HRESULT h_r = S_OK;

    if (STRESS_LOCATION_STEM == eo_stressType)
    {
        h_GetStemStressPositions (str_lemma, vec_iStressPositions);
    }
    else if (STRESS_LOCATION_ENDING == eo_stressType)
    {
        int i_stressPos = -1;
        h_GetEndingStressPosition (str_lemma, str_ending, i_stressPos);
        vec_iStressPositions.push_back (i_stressPos);
    }
    else
    {
        ATLASSERT(0);
        wstring str_msg (L"Illegal stress type.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_FAIL, str_msg);
    }

    return S_OK;

}   //  h_GetStressPositions (...)

HRESULT CT_FormBuilderPersonal::h_CreateFormTemplate (ET_Number eo_number, 
                                                      ET_Person eo_person,
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
        throw CT_Exception (E_INVALIDARG, (LPCTSTR)cs_msg);
    }

    pco_wordForm->pco_Lexeme = pco_Lexeme;
    pco_wordForm->eo_POS = pco_Lexeme->eo_PartOfSpeech;
    pco_wordForm->eo_Subparadigm = SUBPARADIGM_PRESENT_TENSE;
    pco_wordForm->eo_Reflexive = pco_Lexeme->eo_Reflexive;
    pco_wordForm->eo_Number = eo_number;
    pco_wordForm->eo_Person = eo_person;
    pco_wordForm->eo_Aspect = pco_Lexeme->eo_Aspect;

    v_AssignSecondaryStress (pco_wordForm);

    return h_r;

}   //  h_CreateFormTemplate (...)

HRESULT CT_FormBuilderPersonal::h_Build()
{
    HRESULT h_r = S_OK;

    try
    {
		bool b_noRegularStems = false;

        h_r = h_BuildVerbStems();
		if (FAILED (h_r))
		{
			return h_r;
		}
		if (S_FALSE == h_r)
		{
			b_noRegularStems = true;
		}

        h_r = h_GetEndings();
        if (S_OK != h_r)
        {
            return h_r;
        }

        int i_type = pco_Lexeme->i_Type;

        for (ET_Number eo_number = NUM_SG; eo_number <= NUM_PL; ++eo_number)
        {
            for (ET_Person eo_person = PERSON_1; eo_person <= PERSON_3; ++eo_person)
            {
                CComObject<CT_WordForm> * pco_wordForm = NULL;
                h_CreateFormTemplate (eo_number, eo_person, pco_wordForm);
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
                        continue;
                    }
                }
				if (b_noRegularStems)
				{
					continue;
				}

                ET_StressLocation eo_stress = STRESS_LOCATION_UNDEFINED;
                h_GetStressType (eo_number, eo_person, eo_stress);
            
                ET_StemAuslaut eo_auslaut = STEM_AUSLAUT_UNDEFINED;
                h_GetAuslautType (eo_number, eo_person, eo_auslaut);

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
                    pco_Endings->h_GetEnding (st_d, i_ending, str_ending);

                    if (6 == i_type && 1 == pco_Lexeme->i_StemAugment)
                    {
                        if ((PERSON_1 == eo_person && NUM_SG == eo_number) ||
                            (PERSON_3 == eo_person && NUM_PL == eo_number))
                        {
                            if (str_ending.empty() || L'ю' != str_ending[0])
                            {
                                ATLASSERT(0);
                                CString cs_msg (L"Unexpected ending");
                                ERROR_LOG ((LPCTSTR)cs_msg);
                                throw CT_Exception (E_INVALIDARG, (LPCTSTR)cs_msg);
                            }
                            str_ending[0] = L'у';
                        }
                    }

                    if (REFL_YES == pco_Lexeme->eo_Reflexive)
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

                    wstring str_lemma;
                    h_GetLemma (eo_number, eo_person, str_lemma);
                    h_FleetingVowelCheck (str_lemma);

                    pco_wordForm->str_Lemma = str_lemma;
                    pco_wordForm->str_WordForm = str_lemma + str_ending;

                    vector<int> vec_iStress;
                    h_GetStressPositions (str_lemma, str_ending, eo_stress, vec_iStress);

                    if (1 == vec_iStress.size() || b_MultiStress (str_lemma, vec_iStress))
                    {
                        vector<int>::iterator it_stressPos = vec_iStress.begin();
                        for (; it_stressPos != vec_iStress.end(); ++it_stressPos)
                        {
                            pco_wordForm->map_Stress[*it_stressPos] = STRESS_PRIMARY;
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
                            pco_wordForm->map_Stress[*it_stressPos] = STRESS_PRIMARY;
                            pco_Lexeme->h_AddWordForm (pco_wordForm);
                        }
                    }

                }   //  for (int i_ending = 0; ... )

            }   //  for (ET_Person eo_person = PERSON_1; ... )

        }   //  for (ET_Number eo_number = NUM_SG; ... )

    }
    catch (CT_Exception co_ex)
    {
        return co_ex.i_GetErrorCode();  // logging should be always done by callee
    }

    return h_r;

}   //  h_Build()

//
// Needed when for abbreviated subparadigm rendering (GDRL 15, 16, 35, 89)
//
HRESULT CT_FormBuilderPersonal::h_BuildIrregular()
{
    HRESULT h_r = S_OK;

    try
    {
        for (ET_Number eo_number = NUM_SG; eo_number <= NUM_PL; ++eo_number)
        {
            for (ET_Person eo_person = PERSON_1; eo_person <= PERSON_3; ++eo_person)
            {
                CT_GramHasher co_hash (SUBPARADIGM_PRESENT_TENSE, 
                                       eo_number, 
                                       GENDER_UNDEFINED, 
                                       eo_person, 
                                       ANIM_UNDEFINED,                                       
                                       pco_Lexeme->eo_Aspect,
                                       CASE_UNDEFINED, 
                                       pco_Lexeme->eo_Reflexive);

                multimap<int, ST_IrregularForm>& mmap_if = pco_Lexeme->mmap_IrregularForms;    // alias
                multimap<int, ST_IrregularForm>::iterator it_if = mmap_if.find (co_hash.i_GramHash());
                if (mmap_if.end() == it_if)
                {
                    if (NUM_SG == eo_number && (PERSON_1 == eo_person || PERSON_3 == eo_person))
                    {
                        ATLASSERT(0);
                        ERROR_LOG (L"Irregular form not in database.");
                        return E_UNEXPECTED;
                    }
                    h_BuildIrregularForm (eo_number, eo_person, (*it_if).second);
                }

                v_AssignSecondaryStress ((*it_if).second);
                pco_Lexeme->h_AddIrregularForm (co_hash.i_GramHash(), (*it_if).second);

            }   //  for (ET_Person eo_person = PERSON_1; ... )

        }   //  for (ET_Number eo_number = NUM_SG; ... )

    }
    catch (CT_Exception co_ex)
    {
        return co_ex.i_GetErrorCode();  // logging should be always done by callee
    }

    return h_r;

}   //  h_BuildIrregular()

HRESULT CT_FormBuilderPersonal::h_BuildIrregularForm (ET_Number eo_number, 
                                                      ET_Person eo_person, 
                                                      ST_IrregularForm& st_if)
{
    // GDRL p. 89

    HRESULT h_r = S_OK;
    
    ST_IrregularForm st_3sg;
    h_r = h_Get3SgIrregular (st_3sg);
    if (S_OK != h_r)
    {
        return h_r;
    }

    CT_ExtString xstr_if (st_3sg.str_Form);
    if (!xstr_if.b_EndsWith (L"т"))
    {
        ATLASSERT(0);
        ERROR_LOG (L"3 Sg form does not end in \'t\'.");
        return E_UNEXPECTED;
    }
    xstr_if.erase (xstr_if.length() - 1);

    st_if.map_Stress = st_3sg.map_Stress;

    if (NUM_SG == eo_number && PERSON_2 == eo_person)
    {
        xstr_if += L"шь";
        return S_OK;
    }

    if (NUM_PL != eo_number)
    {
        ATLASSERT(0);
        ERROR_LOG (L"Unexpected ET_Number value.");
        return E_UNEXPECTED;
    }

    //  NUM_PL == eo_number

    switch (eo_person)
    {
        case PERSON_1:
        {
            xstr_if += L"м";
            break;
        }
        case PERSON_2:
        {
            xstr_if += L"те";
            break;
        }
        case PERSON_3:
        {
            if (xstr_if.b_EndsWith (L"е") || xstr_if.b_EndsWith (L"ё"))
            {
                ST_IrregularForm st_1sg;
                h_r = h_Get3SgIrregular (st_1sg);
                if (S_OK != h_r)
                {
                    return h_r;
                }
                xstr_if = st_1sg.str_Form;
                xstr_if += L"т";
            }
            else if (xstr_if.b_EndsWith (L"и"))
            {
                xstr_if.erase (xstr_if.length()-1);
                if (xstr_if.b_EndsWithOneOf (str_ShSounds))
                {
                    xstr_if += L"ат";
                }
                else
                {
                    xstr_if += L"ят";
                }
            }
            else
            {
                ATLASSERT(0);
                ERROR_LOG (L"3 Sg form does not end in \'et\' or \'it\'.");
                return E_UNEXPECTED;
            }
            break;
        
        }   //  PERSON_3            
        default:
        {
            ATLASSERT(0);
            ERROR_LOG (L"Illegal person value.");
            return E_UNEXPECTED;
        }
    
    }       //  switch

    return S_OK;

}       //  h_BuildIrregularForm (...)
