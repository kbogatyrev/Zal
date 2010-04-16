#include "StdAfx.h"
#include "Endings.h"
#include "FormBuilderAdjLong.h"
#include "FormBuilderAdjShort.h"
#include "FormBuilderVerbNonFinite.h"

HRESULT CT_FormBuilderNonFinite::h_Build()
{
    HRESULT h_r = S_OK;

    try
    {
        h_BuildInfinitive();
        h_BuildPresentActiveParticiple();
        h_BuildPresentAdverbial();
        h_BuildPastActiveParticiple();
        h_BuildPastAdverbial();
        h_BuildPresentPassiveParticiple();
        h_BuildPastPassiveParticiple();
    }
    catch (CT_Exception co_ex)
    {
        return co_ex.i_GetErrorCode();  // logging should be always done by callee
    }
    
    return S_OK;
}

HRESULT CT_FormBuilderNonFinite::h_BuildInfinitive()
{
    HRESULT h_r = S_OK;

    CComObject<CT_WordForm> * pco_wordForm = NULL;
    h_r = CComObject<CT_WordForm>::CreateInstance (&pco_wordForm);
    if (S_OK != h_r)
    {
        ATLASSERT(0);
        CString cs_msg;
        cs_msg.Format (L"CreateInstance() failed, error %x.", h_r);
        ERROR_LOG ((LPCTSTR)cs_msg);
        throw CT_Exception (h_r, (LPCTSTR)cs_msg);
    }

    pco_wordForm->eo_POS = POS_VERB;
    pco_wordForm->eo_Subparadigm = SUBPARADIGM_INFINITIVE;
    pco_wordForm->eo_Reflexive = pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO;
    pco_wordForm->str_Lemma = pco_Lexeme->xstr_InfStem.c_str();
    pco_wordForm->str_WordForm = pco_Lexeme->xstr_Infinitive;

    vector<int> vec_iStress = pco_Lexeme->vec_SourceStressPos;
    v_AssignSecondaryStress (pco_wordForm);

    if (1 == vec_iStress.size() || b_MultiStress (pco_wordForm->str_Lemma, vec_iStress))
    {
        vector<int>::iterator it_stressedSyll = vec_iStress.begin();
        for (; it_stressedSyll != vec_iStress.end(); ++it_stressedSyll)
        {
            pco_wordForm->map_Stress[*it_stressedSyll] = true;  // primary
        }
        pco_Lexeme->h_AddWordForm (pco_wordForm);
    }
    else
    {
        vector<int>::iterator it_stressedSyll = vec_iStress.begin();
        for (; it_stressedSyll != vec_iStress.end(); ++it_stressedSyll)
        {
            if (it_stressedSyll != vec_iStress.begin())
            {
                CComObject<CT_WordForm> * pco_wfVariant = NULL;
                h_CloneWordForm (pco_wordForm, pco_wfVariant);
                pco_wordForm = pco_wfVariant;
            }
            pco_wordForm->map_Stress[*it_stressedSyll] = true;  // primary
            pco_Lexeme->h_AddWordForm (pco_wordForm);
        }
    }

    return S_OK;

}   //  h_BuildInfinitive()

HRESULT CT_FormBuilderNonFinite::h_BuildPresentActiveParticiple()
{
    HRESULT h_r = S_OK;

    if (L"св" == pco_Lexeme->str_MainSymbol)
    {
        return S_OK;
    }

    int i_type = pco_Lexeme->i_Type;
    wstring str_lemma;

    CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_PL, GENDER_UNDEFINED, 
                             TENSE_UNDEFINED, PERSON_3, ANIM_UNDEFINED, pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO,
                             VOICE_UNDEFINED, ASPECT_UNDEFINED);

    CT_WordForm * pco_3PlWf = NULL;
    h_WordFormFromHash (co_hasher.i_GramHash(), 0, pco_3PlWf);
    if (NULL == pco_3PlWf)
    {
        ATLASSERT(0);
        wstring str_msg (L"Failed to obtain praes. 3 pl. form.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_POINTER, str_msg);
    }

    str_lemma = pco_3PlWf->str_WordForm;
    int i_erase = pco_Lexeme->b_Reflexive ? 3 : 1;
    if (i_erase >= (int)str_lemma.length())
    {
        ATLASSERT(0);
        wstring str_msg (L"Illegal stem length.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_FAIL, str_msg);
    }
    str_lemma = str_lemma.erase (str_lemma.length() - i_erase);
    str_lemma += L'щ';

    ET_Subparadigm eo_sp (SUBPARADIGM_PART_PRES_ACT);
    vector<int> vec_stressPos;
    v_GetParticipleStressPos (eo_sp, str_lemma, vec_stressPos);
    vector<int>::iterator it_stress = vec_stressPos.begin();
    for (; it_stress != vec_stressPos.end(); ++it_stress)
    {
        CT_FormBuilderLongAdj co_builder (pco_Lexeme, str_lemma, AT_A, eo_sp, *it_stress);
        co_builder.h_BuildParticiple();
    }

    return h_r;

}   //  h_BuildPresentActiveParticiple()

HRESULT CT_FormBuilderNonFinite::h_BuildPresentAdverbial()
{
    HRESULT h_r = S_OK;

    if (L"св" == pco_Lexeme->str_MainSymbol)
    {
        return S_OK;
    }

    int i_type = pco_Lexeme->i_Type;

    if (3 == i_type || 8 == i_type || 9 == i_type || 11 == i_type || 14 == i_type || 15 == i_type)
    {
        return S_OK;
    }

    CComObject<CT_WordForm> * pco_wordForm = NULL;
    h_r = CComObject<CT_WordForm>::CreateInstance (&pco_wordForm);
    if (S_OK != h_r)
    {
        ATLASSERT(0);
        CString cs_msg;
        cs_msg.Format (L"CreateInstance() failed, error %x.", h_r);
        ERROR_LOG ((LPCTSTR)cs_msg);
        throw CT_Exception (h_r, (LPCTSTR)cs_msg);
    }

    pco_wordForm->eo_POS = POS_VERB;
    pco_wordForm->eo_Subparadigm = SUBPARADIGM_ADVERBIAL_PRESENT;
    pco_wordForm->eo_Reflexive = pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO;
    v_AssignSecondaryStress (pco_wordForm);

    vector<int> vec_iStress;
    wstring str_lemma;
    wstring str_ending (L"я");     // for the lack of a better term...
    if (13 == i_type)
    {
        CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_INFINITIVE, CASE_UNDEFINED, NUM_UNDEFINED, GENDER_UNDEFINED, 
                                 TENSE_UNDEFINED, PERSON_UNDEFINED, ANIM_UNDEFINED, 
                                 pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO, VOICE_UNDEFINED, ASPECT_UNDEFINED);

        int i_nForms = i_NForms (co_hasher.i_GramHash());
        CT_WordForm * pco_infinitive = NULL;
        for (int i_f = 0; i_f < i_nForms; ++i_f)
        {
            h_WordFormFromHash (co_hasher.i_GramHash(), i_f, pco_infinitive);
            if (NULL == pco_infinitive)
            {
                ATLASSERT(0);
                wstring str_msg (L"Failed to obtain infinitive.");
                ERROR_LOG (str_msg);
                throw CT_Exception (E_POINTER, str_msg);
            }

            pco_wordForm->str_Lemma = pco_infinitive->str_Lemma;
            pco_wordForm->str_WordForm = pco_wordForm->str_Lemma + str_ending;

            vec_iStress = pco_Lexeme->vec_SourceStressPos;
            if (1 == vec_iStress.size() || b_MultiStress (pco_wordForm->str_Lemma, vec_iStress))
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
        }
    }
    else
    {
        CT_GramHasher co_hasher3Sg (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_PL, 
                                    GENDER_UNDEFINED, TENSE_UNDEFINED, PERSON_3, ANIM_UNDEFINED, 
                                    pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO, VOICE_UNDEFINED, 
                                    ASPECT_UNDEFINED);

        CT_WordForm * pco_3PlWf = NULL;
        h_WordFormFromHash (co_hasher3Sg.i_GramHash(), 0, pco_3PlWf);
        if (NULL == pco_3PlWf)
        {
            ATLASSERT(0);
            wstring str_msg (L"Failed to obtain 3 Pl word form.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_POINTER, str_msg);
        }

        CT_GramHasher co_hasher1Sg (co_hasher3Sg);
        co_hasher1Sg.eo_Number = NUM_SG;
        co_hasher1Sg.eo_Person = PERSON_1;

        CT_WordForm * pco_1SgWf = NULL;
        int i_nForms = i_NForms (co_hasher1Sg.i_GramHash());
        for (int i_f = 0; i_f < i_nForms; ++i_f)
        {
            h_WordFormFromHash (co_hasher1Sg.i_GramHash(), i_f, pco_1SgWf);
            if (NULL == pco_1SgWf)
            {
                ATLASSERT(0);
                wstring str_msg (L"Failed to obtain 1 Sg word form.");
                ERROR_LOG (str_msg);
                throw CT_Exception (E_POINTER, str_msg);
            }

            if (CT_ExtString (str_lemma).b_EndsWithOneOf (str_ShSounds))
            {
                str_ending = L"а";
            }
            pco_wordForm->str_Lemma = pco_3PlWf->str_Lemma;
            pco_wordForm->str_WordForm = pco_3PlWf->str_Lemma + str_ending;
            int i_stressPos = i_StressPosFromTemplate (*pco_1SgWf, pco_wordForm->str_Lemma);
            pco_wordForm->map_Stress[i_stressPos] = true;
            v_AssignSecondaryStress (pco_wordForm);
            pco_Lexeme->h_AddWordForm (pco_wordForm);
        }
    }

    return h_r;

}   //  h_BuildPresentAdverbial()

HRESULT CT_FormBuilderNonFinite::h_BuildPastActiveParticiple()
{
    HRESULT h_r = S_OK;

    int i_type = pco_Lexeme->i_Type;

    CComObject<CT_WordForm> * pco_wordForm = NULL;
    h_r = CComObject<CT_WordForm>::CreateInstance (&pco_wordForm);
    if (S_OK != h_r)
    {
        ATLASSERT(0);
        CString cs_msg;
        cs_msg.Format (L"CreateInstance() failed, error %x.", h_r);
        ERROR_LOG ((LPCTSTR)cs_msg);
        throw CT_Exception (h_r, (LPCTSTR)cs_msg);
    }

    //
    // Type 7 ending in -сти with -т- or -д- alternation mark:
    // 1 Sg stem present tense + -ший
    //
    if (7 == i_type && 
        (pco_Lexeme->xstr_SourceForm).b_EndsWith (L"сти") &&
        (L"т" == pco_Lexeme->str_VerbStemAlternation || L"д" == pco_Lexeme->str_VerbStemAlternation))
    {
        pco_wordForm->str_Lemma = pco_Lexeme->xstr_1SgStem.c_str();
        pco_wordForm->str_Lemma += L"ш";
    }
    else
    {
        // All other types: derived from Past Sg. m 
        CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_PAST_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_M, 
                                 TENSE_UNDEFINED, PERSON_UNDEFINED, ANIM_UNDEFINED, 
                                 pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO, VOICE_UNDEFINED, 
                                 ASPECT_UNDEFINED);
        CT_WordForm * pco_pastM = NULL;
        h_WordFormFromHash (co_hasher.i_GramHash(), 0, pco_pastM);
        if (NULL == pco_pastM)
        {
            ATLASSERT(0);
            wstring str_msg (L"Failed to obtain past tense m. form.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_POINTER, str_msg);
        }
        
        //
        // If past Sg. m ends in -л, replace it with -вший, otherwise add -ший
        //
        CT_ExtString xstr_wf (pco_pastM->str_WordForm);
        if (xstr_wf.b_EndsWith (L"л"))
        {
            pco_wordForm->str_Lemma = xstr_wf.erase (xstr_wf.length()-1).c_str();
            pco_wordForm->str_Lemma += L"вш";
        }
        else
        {
            pco_wordForm->str_Lemma = xstr_wf.c_str();
            pco_wordForm->str_Lemma += L"ш";
        }
    }

    ET_Subparadigm eo_sp (SUBPARADIGM_PART_PAST_ACT);
    vector<int> vec_stressPos;
    v_GetParticipleStressPos (eo_sp, pco_wordForm->str_Lemma, vec_stressPos);
    vector<int>::iterator it_stress = vec_stressPos.begin();
    for (; it_stress != vec_stressPos.end(); ++it_stress)
    {
        CT_FormBuilderLongAdj co_builder (pco_Lexeme, pco_wordForm->str_Lemma, AT_A, eo_sp, *it_stress);
        co_builder.h_BuildParticiple();
    }

    return h_r;

}   //  h_BuildPastActiveParticiple()

HRESULT CT_FormBuilderNonFinite::h_BuildPastAdverbial()
{
// &&&& st. dev. (9); refl.
    HRESULT h_r = S_OK;

    CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_PART_PAST_ACT, CASE_NOM, NUM_SG, GENDER_M, 
                             TENSE_UNDEFINED, PERSON_UNDEFINED, ANIM_NO, 
                             pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO, VOICE_UNDEFINED, 
                             ASPECT_UNDEFINED);

    CT_WordForm * pco_pastPart = NULL;
    int i_nForms = i_NForms (co_hasher.i_GramHash());
    for (int i_f = 0; i_f < i_nForms; ++i_f)
    {
        h_WordFormFromHash (co_hasher.i_GramHash(), i_f, pco_pastPart);
        if (NULL == pco_pastPart)
        {
            ATLASSERT(0);
            wstring str_msg (L"Failed to obtain past active participle form.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_POINTER, str_msg);
        }
       
        CComObject<CT_WordForm> * pco_wordForm = NULL;
        h_r = CComObject<CT_WordForm>::CreateInstance (&pco_wordForm);
        if (S_OK != h_r)
        {
            ATLASSERT(0);
            CString cs_msg;
            cs_msg.Format (L"CreateInstance() failed, error %x.", h_r);
            ERROR_LOG ((LPCTSTR)cs_msg);
            throw CT_Exception (h_r, (LPCTSTR)cs_msg);
        }

        pco_wordForm->eo_POS = POS_VERB;
        pco_wordForm->eo_Subparadigm = SUBPARADIGM_ADVERBIAL_PAST;
        pco_wordForm->eo_Reflexive = pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO;
        v_AssignSecondaryStress (pco_wordForm);
        pco_wordForm->str_Lemma = pco_pastPart->str_Lemma;
        pco_wordForm->str_Lemma.erase (pco_pastPart->str_Lemma.length()-1);
        pco_wordForm->str_WordForm = pco_wordForm->str_Lemma + L"ши";

        if (L"нсв" == pco_Lexeme->str_MainSymbol)
        {
            pco_wordForm->eo_Status = STATUS_RARE;
        }

        int i_stressPos = i_StressPosFromTemplate (*pco_pastPart, pco_wordForm->str_Lemma);
        pco_wordForm->map_Stress[i_stressPos] = true;

        pco_Lexeme->h_AddWordForm (pco_wordForm);

        if (CT_ExtString (pco_wordForm->str_WordForm).b_EndsWith (L"вши"))
        {
            CComObject<CT_WordForm> * pco_wordForm2 = NULL;
            h_CloneWordForm (pco_wordForm, pco_wordForm2);
            pco_wordForm2->str_WordForm = pco_wordForm->str_WordForm;
            pco_wordForm2->str_WordForm.erase (pco_wordForm->str_WordForm.length()-2);
            pco_Lexeme->h_AddWordForm (pco_wordForm2);
        }

    }   //  for (int i_f = 0; i_f < i_nForms; ++i_f)

    if (9 == pco_Lexeme->i_Type && L"нсв" == pco_Lexeme->str_MainSymbol && !pco_Lexeme->b_Reflexive)
    {
        CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_INFINITIVE, CASE_UNDEFINED, NUM_UNDEFINED, GENDER_UNDEFINED, 
                                 TENSE_UNDEFINED, PERSON_UNDEFINED, ANIM_UNDEFINED, REFL_NO, VOICE_UNDEFINED, 
                                 ASPECT_UNDEFINED);

        CT_WordForm * pco_infinitive = NULL;
        ATLASSERT (i_NForms (co_hasher.i_GramHash()) == 1);
        h_WordFormFromHash (co_hasher.i_GramHash(), 0, pco_infinitive);
        if (NULL == pco_infinitive)
        {
            ATLASSERT(0);
            wstring str_msg (L"Failed to obtain infinitive.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_POINTER, str_msg);
        }
       
        CComObject<CT_WordForm> * pco_wordForm = NULL;
        h_r = CComObject<CT_WordForm>::CreateInstance (&pco_wordForm);
        if (S_OK != h_r)
        {
            ATLASSERT(0);
            CString cs_msg;
            cs_msg.Format (L"CreateInstance() failed, error %x.", h_r);
            ERROR_LOG ((LPCTSTR)cs_msg);
            throw CT_Exception (h_r, (LPCTSTR)cs_msg);
        }

        pco_wordForm->str_Lemma = pco_Lexeme->xstr_InfStem.c_str();
        pco_wordForm->str_WordForm = pco_wordForm->str_Lemma + L"в";
        int i_stressPos = i_StressPosFromTemplate (*pco_infinitive, pco_wordForm->str_Lemma);
        pco_wordForm->map_Stress[i_stressPos] = true;

        pco_Lexeme->h_AddWordForm (pco_wordForm);
    }

    return S_OK;

}   //  h_BuildPastAdverbial()

HRESULT CT_FormBuilderNonFinite::h_BuildPresentPassiveParticiple()
{
    HRESULT h_r = S_OK;

    if (L"св" == pco_Lexeme->str_MainSymbol || !pco_Lexeme->b_Transitive)
    {
        return S_OK;
    }

    int i_group = -1;       // GDRL, p.86

    CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_PL, GENDER_UNDEFINED, 
                             TENSE_UNDEFINED, PERSON_1, ANIM_UNDEFINED, pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO,
                             VOICE_UNDEFINED, ASPECT_UNDEFINED);
    CT_WordForm * pco_1Pl = NULL;
    h_WordFormFromHash (co_hasher.i_GramHash(), 0, pco_1Pl);
    CT_ExtString xstr_1Pl (pco_1Pl->str_WordForm);
    if (xstr_1Pl.b_EndsWith (L"ем") &&
        pco_1Pl->map_Stress.find (xstr_1Pl.length()-2) == pco_1Pl->map_Stress.end() &&
        str_Vowels.find (xstr_1Pl[xstr_1Pl.length()-3]) != wstring::npos)
    {
        ATLASSERT ((1 == pco_Lexeme->i_Type && AT_A == pco_Lexeme->eo_AccentType1)  ||
                   (2 == pco_Lexeme->i_Type && AT_A == pco_Lexeme->eo_AccentType1)  ||
                   (12 == pco_Lexeme->i_Type && AT_A == pco_Lexeme->eo_AccentType1) ||
                   (6 == pco_Lexeme->i_Type && AT_A == pco_Lexeme->eo_AccentType1 && 
                                    pco_Lexeme->xstr_Infinitive.b_EndsWith (L"ять")) || 
                   13 == pco_Lexeme->i_Type);
        i_group = 1;
    }
    else if (4 == pco_Lexeme->i_Type)
    {
        i_group = 2;
    }
    else if (5 == pco_Lexeme->i_Type || 
             (6 == pco_Lexeme->i_Type && pco_Lexeme->xstr_Infinitive.b_EndsWith (L"ать")) ||
             (6 == pco_Lexeme->i_Type && pco_Lexeme->b_StemAugment))
    {
        i_group = 3;
    }

    wstring str_lemma;
    if (13 == pco_Lexeme->i_Type)
    {
        str_lemma = pco_Lexeme->xstr_InfStem.c_str();
        str_lemma += L"ем";
    }
    else
    {
        str_lemma = xstr_1Pl.c_str();
        CT_ExtString xstr_lemma (str_lemma);
        if (xstr_lemma.b_EndsWith (L"ём"))
        {
            if (str_Consonants.find (str_lemma[str_lemma.length()-3]) != wstring::npos)
            {
                str_lemma.replace (str_lemma.length()-2, 2, L"ом");
            }
        }
    }

    ET_Subparadigm eo_sp (SUBPARADIGM_PART_PRES_PASS_LONG);
    vector<int> vec_stress;
    v_GetParticipleStressPos (eo_sp, str_lemma, vec_stress);
    vector<int>::iterator it_stress = vec_stress.begin();
    for (; it_stress != vec_stress.end(); ++it_stress)
    {
        CT_FormBuilderLongAdj co_long (pco_Lexeme, str_lemma, AT_A, eo_sp, *it_stress);
        co_long.h_BuildParticiple();
    }

    CT_FormBuilderShortAdj co_short (pco_Lexeme);
    co_short.h_Build();

    return S_OK;

}   //  h_BuildPresentPassiveParticiple()

HRESULT CT_FormBuilderNonFinite::h_BuildPastPassiveParticiple()
{
    HRESULT h_r = S_OK;

    ET_Status eo_status = STATUS_UNDEFINED;

    if (!pco_Lexeme->b_Transitive)
    {
        return S_OK;
    }

    if (L"нсв" == pco_Lexeme->str_MainSymbol)
    {
        if (pco_Lexeme->b_HasAspectPair)
        {
            return S_OK;
        }
        else
        {
            eo_status = STATUS_RARE;
        }
    }

    wstring str_lemma;
    if ((pco_Lexeme->xstr_Infinitive.b_EndsWith (L"ать") || pco_Lexeme->xstr_Infinitive.b_EndsWith (L"ять")) &&
        14 != pco_Lexeme->i_Type)
    {
        str_lemma = pco_Lexeme->xstr_InfStem.c_str();
        str_lemma += L"нн";
    }

    if (4 == pco_Lexeme->i_Type || (5 == pco_Lexeme->i_Type && L"еть" == pco_Lexeme->xstr_Infinitive))
    {
        str_lemma = pco_Lexeme->xstr_1SgStem.c_str();
        str_lemma += L"енн";
    }

    if (1 == pco_Lexeme->i_Type && L"еть" == pco_Lexeme->xstr_Infinitive)
    {
        if (pco_Lexeme->xstr_Infinitive.b_EndsWith (L"одолеть") ||
            pco_Lexeme->xstr_Infinitive.b_EndsWith (L"печатлеть"))
        {
            str_lemma = pco_Lexeme->xstr_Infinitive.c_str();
            str_lemma.erase (pco_Lexeme->xstr_Infinitive.length()-3);
            str_lemma += L"ённ";
        }
    }

    if (7 == pco_Lexeme->i_Type || 8 == pco_Lexeme->i_Type)
    {
        str_lemma = pco_Lexeme->xstr_3SgStem.c_str();
        str_lemma += L"енн";
    }

    if (3 == pco_Lexeme->i_Type || 10 == pco_Lexeme->i_Type)        // NB also 3 + circle
    {
        str_lemma = pco_Lexeme->xstr_InfStem;
        str_lemma += L"т";
    }

    if (9 == pco_Lexeme->i_Type || 11 == pco_Lexeme->i_Type  || 12 == pco_Lexeme->i_Type || 
        14 == pco_Lexeme->i_Type || 15 == pco_Lexeme->i_Type || 16 == pco_Lexeme->i_Type)
    {
        CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_PAST_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_M, 
                                 TENSE_UNDEFINED, PERSON_UNDEFINED, ANIM_UNDEFINED, 
                                 pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO, VOICE_UNDEFINED, 
                                 ASPECT_UNDEFINED);
        CT_WordForm * pco_pastM = NULL;
        h_WordFormFromHash (co_hasher.i_GramHash(), 0, pco_pastM);
        if (NULL == pco_pastM)
        {
            ATLASSERT(0);
            wstring str_msg (L"Failed to obtain past tense m. form.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_POINTER, str_msg);
        }

        CT_ExtString xstr_lemma (pco_pastM->str_WordForm);
        if (xstr_lemma.b_EndsWith (L"л"))
        {
            xstr_lemma.erase (xstr_lemma.length()-1);
        }
        str_lemma += L'т';
    }

    ET_Subparadigm eo_sp (SUBPARADIGM_PART_PAST_PASS_LONG);
    vector<int> vec_stress;
    v_GetParticipleStressPos (eo_sp, str_lemma, vec_stress);
    vector<int>::iterator it_stress = vec_stress.begin();
    for (; it_stress != vec_stress.end(); ++it_stress)
    {
        if (4 == pco_Lexeme->i_Type || 5 == pco_Lexeme->i_Type || 
            7 == pco_Lexeme->i_Type || 8 == pco_Lexeme->i_Type)
        {
            if (*it_stress >= (int)str_lemma.length())
            {
                ATLASSERT(0);
                wstring str_msg (L"Bad stress position.");
                ERROR_LOG (str_msg);
                throw CT_Exception (E_FAIL, str_msg);
            }
            if (L'е' == str_lemma[*it_stress])
            {
                str_lemma[*it_stress] = L'ё';
            }
        }
        CT_FormBuilderLongAdj co_builder (pco_Lexeme, str_lemma, AT_A, eo_sp, *it_stress);
        co_builder.h_BuildParticiple();
    }

    return S_OK;

}   //  h_BuildPastPassiveParticiple()

void CT_FormBuilderNonFinite::v_GetParticipleStressPos (ET_Subparadigm eo_subparadigm, 
                                                        wstring& str_lemma, 
                                                        vector<int>& vec_positions)
{
    int i_hash = -1;
    vec_positions.clear();

    switch (eo_subparadigm)
    {
        case SUBPARADIGM_PART_PRES_ACT:
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
        case SUBPARADIGM_PART_PAST_ACT:
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
        case SUBPARADIGM_PART_PRES_PASS_LONG:
        {
            if (13 == pco_Lexeme->i_Type)
            {
                CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_INFINITIVE, CASE_UNDEFINED, NUM_UNDEFINED, 
                                         GENDER_UNDEFINED, TENSE_UNDEFINED, PERSON_UNDEFINED, ANIM_UNDEFINED, 
                                         pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO, VOICE_UNDEFINED, 
                                         ASPECT_UNDEFINED);
                i_hash = co_hasher.i_GramHash();
            }
            else
            {
                if (4 == pco_Lexeme->i_Type || 5 == pco_Lexeme->i_Type)
                {
                    CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_SG, 
                                             GENDER_UNDEFINED, TENSE_UNDEFINED, PERSON_1, ANIM_UNDEFINED, 
                                             pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO, VOICE_UNDEFINED, 
                                             ASPECT_UNDEFINED);
                    i_hash = co_hasher.i_GramHash();
                }
                else
                {
                    CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_PL, 
                                             GENDER_UNDEFINED, TENSE_UNDEFINED, PERSON_1, ANIM_UNDEFINED, 
                                             pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO, VOICE_UNDEFINED, 
                                             ASPECT_UNDEFINED);
                    i_hash = co_hasher.i_GramHash();
                }
            }

            break;
        }
        case SUBPARADIGM_PART_PAST_PASS_LONG:
        {
            if ((pco_Lexeme->xstr_Infinitive.b_EndsWith (L"ать") || pco_Lexeme->xstr_Infinitive.b_EndsWith (L"ять")) &&
                14 != pco_Lexeme->i_Type)
            {
                vector<int>::iterator it_stress = pco_Lexeme->vec_SourceStressPos.begin();
                for (; it_stress != pco_Lexeme->vec_SourceStressPos.end(); ++it_stress)
                {
                    int i_nSyll = pco_Lexeme->xstr_Infinitive.i_NSyllables();
                    if (i_nSyll > 1 && *it_stress == i_nSyll - 1)	// polysyllabic infinitive with 
                    {                                               // stress on last syllable
                        vec_positions.push_back (i_nSyll - 2);
                    }
                    else
                    {
                        vec_positions.push_back (*it_stress);
                    }
                }
            }

            if (4 == pco_Lexeme->i_Type)    // same syllable (counting from right) as in 3 Sg Praes
            {
                CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_UNDEFINED, 
                                         TENSE_UNDEFINED, PERSON_3, ANIM_UNDEFINED, 
                                         pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO, VOICE_UNDEFINED, 
                                         ASPECT_UNDEFINED);
                int i_nForms = i_NForms (co_hasher.i_GramHash());
                CT_WordForm * pco_3Sg = NULL;
                for (int i_f = 0; i_f < i_nForms; ++i_f)
                {
                    h_WordFormFromHash (co_hasher.i_GramHash(), i_f, pco_3Sg);
                    if (NULL == pco_3Sg)
                    {
                        ATLASSERT(0);
                        wstring str_msg (L"Failed to obtain template form.");
                        ERROR_LOG (str_msg);
                        throw CT_Exception (E_POINTER, str_msg);
                    }

                    map<int, bool>::iterator it_stressPos = pco_3Sg->map_Stress.begin();
                    for (; pco_3Sg->map_Stress.end() != it_stressPos; ++it_stressPos)
                    {
                        if ((*it_stressPos).second) // true == primary stress
                        {
                            vec_positions.push_back ((*it_stressPos).first);
                        }
                    }
                }       //  for (int i_f = 0; i_f < i_nForms; ++i_f)

            }   //   if (4 == pco_Lexeme->i_Type)
                                               
            if (5 == pco_Lexeme->i_Type && L"еть" == pco_Lexeme->xstr_Infinitive)
            {
                vector<int>::iterator it_stress = pco_Lexeme->vec_SourceStressPos.begin();
                for (; it_stress != pco_Lexeme->vec_SourceStressPos.end(); ++it_stress)
                {
                    int i_nSyll = pco_Lexeme->xstr_Infinitive.i_NSyllables();
                    if (i_nSyll > 1)
                    {
                        vec_positions.push_back (i_nSyll - 2);
                    }
                    else
                    {
                        vec_positions.push_back (*it_stress);
                    }
                }
            }

            if (1 == pco_Lexeme->i_Type && pco_Lexeme->xstr_Infinitive.b_EndsWith(L"еть"))
            {                                                           // GDRL p. 86, footnote 3
                vec_positions.push_back ((int)pco_Lexeme->xstr_Infinitive.i_GetNumOfSyllables() - 2);
            }

            if (7 == pco_Lexeme->i_Type || 8 == pco_Lexeme->i_Type)
            {
                CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_PAST_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_F,
                                         TENSE_UNDEFINED, PERSON_UNDEFINED, ANIM_UNDEFINED, 
                                         pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO, VOICE_UNDEFINED, 
                                         ASPECT_UNDEFINED);
                CT_WordForm * pco_pastF = NULL;
                int i_nForms = i_NForms (co_hasher.i_GramHash());
                for (int i_f = 0; i_f < i_nForms; ++i_f)
                {
                    h_WordFormFromHash (co_hasher.i_GramHash(), i_f, pco_pastF);
                    if (NULL == pco_pastF)
                    {
                        ATLASSERT(0);
                        wstring str_msg (L"Failed to obtain template form.");
                        ERROR_LOG (str_msg);
                        throw CT_Exception (E_POINTER, str_msg);
                    }

                    map<int, bool>::iterator it_stressPos = pco_pastF->map_Stress.begin();
                    for (; pco_pastF->map_Stress.end() != it_stressPos; ++it_stressPos)
                    {
                        if ((*it_stressPos).second) // true == primary stress
                        {
                            vec_positions.push_back ((*it_stressPos).first);
                        }
                    }
                }       //  for (int i_f = 0; i_f < i_nForms; ++i_f)
            }

            if (3 == pco_Lexeme->i_Type || 10 == pco_Lexeme->i_Type)        // NB also 3 + circle
            {
                vector<int>::iterator it_stress = pco_Lexeme->vec_SourceStressPos.begin();
                for (; it_stress != pco_Lexeme->vec_SourceStressPos.end(); ++it_stress)
                {
                    int i_nSyll = pco_Lexeme->xstr_Infinitive.i_NSyllables();
                    if (i_nSyll > 1)
                    {
                        if (*it_stress == i_nSyll - 1)
                        {
                            vec_positions.push_back (i_nSyll - 2);
                        }
                        else
                        {
                            vec_positions.push_back (*it_stress);
                        }
                    }
                }
            }

            if (9 == pco_Lexeme->i_Type || 11 == pco_Lexeme->i_Type  || 12 == pco_Lexeme->i_Type || 
                14 == pco_Lexeme->i_Type || 15 == pco_Lexeme->i_Type || 16 == pco_Lexeme->i_Type)
            {
                CT_GramHasher co_hasher (POS_VERB, SUBPARADIGM_PAST_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_M, 
                                         TENSE_UNDEFINED, PERSON_UNDEFINED, ANIM_UNDEFINED, 
                                         pco_Lexeme->b_Reflexive ? REFL_YES : REFL_NO, VOICE_UNDEFINED, 
                                         ASPECT_UNDEFINED);
                CT_WordForm * pco_pastM = NULL;
                int i_nForms = i_NForms (co_hasher.i_GramHash());
                for (int i_f = 0; i_f < i_nForms; ++i_f)
                {
                    h_WordFormFromHash (co_hasher.i_GramHash(), i_f, pco_pastM);
                    if (NULL == pco_pastM)
                    {
                        ATLASSERT(0);
                        wstring str_msg (L"Failed to obtain template form.");
                        ERROR_LOG (str_msg);
                        throw CT_Exception (E_POINTER, str_msg);
                    }

                    map<int, bool>::iterator it_stressPos = pco_pastM->map_Stress.begin();
                    for (; pco_pastM->map_Stress.end() != it_stressPos; ++it_stressPos)
                    {
                        if ((*it_stressPos).second) // true == primary stress
                        {
                            vec_positions.push_back ((*it_stressPos).first);
                        }
                    }
                }       //  for (int i_f = 0; i_f < i_nForms; ++i_f)
            }
            break;

        }   //  case SUBPARADIGM_PART_PAST_PASS
 
        default:
        {
            ATLASSERT (0);
            wstring str_msg (L"Bad subparadigm.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_INVALIDARG, str_msg);
        }
    }

    if (SUBPARADIGM_PART_PAST_PASS_LONG != eo_subparadigm)
    {
        CT_WordForm * pco_stressTemplate = NULL;
        h_WordFormFromHash (i_hash, 0, pco_stressTemplate);
        map<int, bool>::iterator it_stressPos = pco_stressTemplate->map_Stress.begin();

        // Skip secondary stress if any
        for (; pco_stressTemplate->map_Stress.end() != it_stressPos; ++it_stressPos)
        {
            if ((*it_stressPos).second)
            {
                vec_positions.push_back ((*it_stressPos).first);
            }
        }

        if (vec_positions.empty())
        {
            ATLASSERT (0);
            wstring str_msg (L"Can't find main stress.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_FAIL, str_msg);
        }
    }

}   //  v_GetParticipleStressPos (...)

int CT_FormBuilderNonFinite::i_StressPosFromTemplate (const CT_WordForm& co_template, 
                                                      const wstring& str_lemma, 
                                                      int i_at)
{
    map<int, bool>::const_iterator it_stressPos = co_template.map_Stress.begin();
    int i_mainStress = 0;
    for (; it_stressPos != co_template.map_Stress.end(); ++it_stressPos)
    {
        if ((*it_stressPos).second)     // main stress == true
        {
            if (i_mainStress == i_at)
            {
                break;
            }
            ++i_mainStress;
        }
    }

    if (co_template.map_Stress.end() == it_stressPos)
    {
        ATLASSERT (0);
        wstring str_msg (L"Can't find main stress.");
        ERROR_LOG (str_msg);
        throw CT_Exception (E_FAIL, str_msg);
    }

    return (*it_stressPos).first;

}   //  i_StressPosFromTemplate (...)
