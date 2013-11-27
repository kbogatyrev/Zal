#include "StdAfx.h"

#include "WordForm.h"
#include "Lexeme.h"
#include "FormBuilderVerbImpv.h"

ET_ReturnCode CFormBuilderImperative::eGetLemma (CEString& sLemma)
{
    try
    {
        int iHash = -1;
        if (4 == m_pLexeme->iType() || 5 == m_pLexeme->iType())
        {
            CGramHasher hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_UNDEFINED, 
                                PERSON_3, ANIM_UNDEFINED, m_pLexeme->eAspect(), m_pLexeme->eIsReflexive());
            iHash = hasher.iGramHash();
        }
        else
        {
            CGramHasher hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_UNDEFINED, 
                                PERSON_1, ANIM_UNDEFINED, m_pLexeme->eAspect(), m_pLexeme->eIsReflexive());
            iHash = hasher.iGramHash();
        }

        CWordForm * pTemplate = NULL;
        ET_ReturnCode rc = m_pLexeme->eWordFormFromHash (iHash, 0, pTemplate);
        if (rc != H_NO_ERROR)
        {
            return rc;
        }
        if (NULL == pTemplate)
        {
            ATLASSERT(0);
            ERROR_LOG (L"Failed to obtain form template.");
            return H_ERROR_POINTER;
        }

        sLemma = pTemplate->m_sLemma;

        if (11 == m_pLexeme->iType())
        {
            ATLASSERT (sLemma.bEndsWith (L"ь"));
            sLemma.sErase (sLemma.uiLength()-1);
        }
    }
    catch (CException&)
    {
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}   //  eGetLemma (...)

ET_ReturnCode CFormBuilderImperative::eGetEnding (const CEString& sLemma, CEString& sEnding)
{
    ET_ReturnCode rc = H_NO_ERROR;

    ET_AccentType eAccentType1 = m_pLexeme->eAccentType1();
    ET_AccentType eAccentType2 = m_pLexeme->eAccentType2();

    int iType = m_pLexeme->iType();

    if (11 == iType)
    {
        sEnding = L"ей";
    }
    else if (sLemma.bEndsWithOneOf (g_szVowels))
    {
        sEnding = L"й";
        if (4 == iType)
        {
            if (AT_B == eAccentType1 || AT_C == eAccentType1)   // кроить, поить
            {
                sEnding = L"и";
            }
            if (AT_A == eAccentType1 && sLemma.bStartsWith (L"вы"))
            {
                sEnding = L"и";
            }
        }
    }
    else
    {
        if (AT_B == eAccentType1 || AT_C == eAccentType1 || AT_C1 == eAccentType1)
        {
            sEnding = L"и";
        }
        if (AT_A == eAccentType1)
        {
            if (sLemma.bStartsWith (L"вы"))
            {
                sEnding = L"и";
            }
            else if (sLemma.bEndsWith (L"щ"))      // 2 consonants or щ or CьC
            {
                sEnding = L"и";
            }
            else if (sLemma.uiLength() >= 2 && 
                CEString::bIn (sLemma[sLemma.uiLength()-1], g_szConsonants) && 
                CEString::bIn (sLemma[sLemma.uiLength()-2], g_szConsonants))
            {
                sEnding = L"и";
            }
            else if (sLemma.uiLength() >= 3 && 
                     CEString::bIn (sLemma[sLemma.uiLength()-1], g_szConsonants) &&
                     L'ь' == sLemma[sLemma.uiLength()-2] &&
                     CEString::bIn (sLemma[sLemma.uiLength()-3], g_szConsonants))
            {
                sEnding = L"и";
            }
            else
            {
                sEnding = L"ь";
            }
        }
    }   // else

    return H_NO_ERROR;

}   //  eGetEnding (...)

ET_ReturnCode CFormBuilderImperative::eGetStressPositions (CEString& sLemma, 
                                                           CEString& sEnding,
                                                           vector<int>& vecStressPositions)
{
    ET_ReturnCode rc = H_NO_ERROR;

    ET_StressLocation eStress = STRESS_LOCATION_UNDEFINED;
    switch (m_pLexeme->eAccentType1())
    {
        case AT_A:
        {
            eStress = STRESS_LOCATION_STEM;
            break;
        }
        case AT_B:
        case AT_C:
        case AT_C1:            // хотеть and derivatives
        {
            eStress = STRESS_LOCATION_ENDING;
            break;
        }
        default:
        {
            ATLASSERT(0);
            ERROR_LOG (L"Illegal accent type.");
            return H_ERROR_GENERAL;
        }
    }

    int iStressPos = -1;
    if (STRESS_LOCATION_STEM == eStress)
    {
        rc = m_pLexeme->eGetStemStressPositions (sLemma, vecStressPositions);
    }
    else if (STRESS_LOCATION_ENDING == eStress)
    {
        rc = eGetEndingStressPosition (sLemma, sEnding, iStressPos);
        vecStressPositions.push_back (iStressPos);
    }
    else
    {
        ATLASSERT(0);
        ERROR_LOG (L"Illegal stress type.");
        return H_ERROR_INVALID_ARG;
    }

    return rc;

}   //  eGetStressPositions (...)

ET_ReturnCode CFormBuilderImperative::eCreateFormTemplate (const CEString& sLemma,
                                                           CEString& sEnding,
                                                           ET_Number eNumber,
                                                           CWordForm *& pWordForm)
{
    ET_ReturnCode rc = H_NO_ERROR;

    pWordForm = new CWordForm;
    if (NULL == pWordForm)
    {
        ATLASSERT(0);
        ERROR_LOG (L"Unable to instantiate word form object.");
        return H_ERROR_POINTER;
    }

    pWordForm->m_pLexeme = m_pLexeme;
    pWordForm->m_ePos = m_pLexeme->ePartOfSpeech();
    pWordForm->m_eSubparadigm = SUBPARADIGM_IMPERATIVE;
    pWordForm->m_eAspect = m_pLexeme->eAspect();
    pWordForm->m_eReflexive = m_pLexeme->eIsReflexive();
    pWordForm->m_sLemma = sLemma;
    if (NUM_PL == eNumber)
    {
        sEnding += L"те";
    }
    pWordForm->m_sWordForm = sLemma + sEnding;
    if (REFL_YES == m_pLexeme->eIsReflexive())
    {
        int iAt = pWordForm->m_sWordForm.uiLength() - 1;
        if (CEString::bIn (pWordForm->m_sWordForm[iAt], g_szVowels))
        {
            pWordForm->m_sWordForm += L"сь";
        }
        else
        {
            pWordForm->m_sWordForm += L"ся";
        }
    }

    pWordForm->m_eNumber = eNumber;
    pWordForm->m_ePerson = PERSON_2;

//    rc = eAssignSecondaryStress (pWordForm);

    return rc;

}   //  eCreateFormTemplate (...)

ET_ReturnCode CFormBuilderImperative::eHandleCommonDeviations (CWordForm * pWordForm)
{
    ET_ReturnCode rc = H_NO_ERROR;

    if (!m_pLexeme->bHasCommonDeviation(2) && !m_pLexeme->bHasCommonDeviation(3))
    {
        return H_NO_MORE;
    }

    int iEndingOffset = (REFL_NO == m_pLexeme->eIsReflexive())
        ? pWordForm->m_sWordForm.uiLength() - 1
        : pWordForm->m_sWordForm.uiLength() - 3;

    if (NUM_PL == pWordForm->m_eNumber)
    {
        iEndingOffset -= 2;
    }

    if (iEndingOffset < 1)
    {
        ATLASSERT(0);
        ERROR_LOG (L"Bad ending offset.");
        return H_ERROR_UNEXPECTED;
    }

    try
    {
        if (m_pLexeme->bHasCommonDeviation(2))
        {
            if (m_pLexeme->bDeviationOptional (2))   // store both forms
            {
                CWordForm * pVariant = NULL;
                CloneWordForm (pWordForm, pVariant);
                m_pLexeme->AddWordForm (pVariant);
                pWordForm = pVariant;
            }

            if (CEString::bIn (pWordForm->m_sWordForm[iEndingOffset-1], g_szVowels))
            {
                pWordForm->m_sWordForm[iEndingOffset] = L'й';
            }
            else
            {
                pWordForm->m_sWordForm[iEndingOffset] = L'ь';
            }
        }   //      if (bHasCommonDeviation (2))

        if (m_pLexeme->bHasCommonDeviation(3))
        {
            if (NUM_SG == pWordForm->m_eNumber)
            {
                if (m_pLexeme->bDeviationOptional(3))   // store both forms
                {
                    pWordForm->m_sWordForm[iEndingOffset] = L'ь';
                    CWordForm * pVariant = NULL;
                    CloneWordForm (pWordForm, pVariant);
                    m_pLexeme->AddWordForm (pVariant);
                    pWordForm = pVariant;
                }
                pWordForm->m_sWordForm[iEndingOffset] = L'и';
            }
            else
            {
                pWordForm->m_sWordForm[iEndingOffset] = L'ь';
            }
        }
    }
    catch (CException&)
    {
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}   //  eHandleCommonDeviations (...)

/*
ET_ReturnCode CFormBuilderImperative::hCheckForIrregularForms (bool& bIsVariant)
                                                // check if irregular forms were built and
                                                // regular processing must be skipped
{
    ET_ReturnCode hr = H_NO_ERROR;

    if (bHasIrregularImperative() || bHasIrregularPresent())
    {
        hr = hBuildIrregularForms();
        if (H_NO_ERROR != hr)
        {
            return hr;
        }
    }
    else
    {
        return H_NO_MORE;
    }
        
    bIsVariant = false;
    for (ET_Number eNumber = NUM_SG; eNumber < NUM_COUNT; ++eNumber)
    {
        CComObject<CWordForm> * pWordForm = NULL;
        hr = CComObject<CWordForm>::CreateInstance (&pWordForm);
        if (H_NO_ERROR != hr)
        {
            ATLASSERT(0);
            CString csMsg;
            csMsg.Format (L"CreateInstance() failed, error %x.", hr);
            ERROR_LOG ((LPCTSTR)csMsg);
            throw CException (hr, (LPCTSTR)csMsg);
        }

        pWordForm->m_pLexeme = m_pLexeme;
        pWordForm->m_ePos = m_pLexeme->ePartOfSpeech();
        pWordForm->m_eSubparadigm = SUBPARADIGM_IMPERATIVE;
        pWordForm->m_eAspect = m_pLexeme->eAspect();
        pWordForm->m_eReflexive = m_pLexeme->eIsReflexive();
        pWordForm->m_ePerson = PERSON_2;
        pWordForm->m_eNumber = eNumber;

        AssignSecondaryStress (pWordForm);

    }       //  for (ET_Number eo_number = NUM_SG; ...

    return hr;

  }     //  hCheckForIrregularForms (...)
*/

ET_ReturnCode CFormBuilderImperative::eBuild()
{
    ET_ReturnCode rc = H_NO_ERROR;

    if (bHasIrregularImperative() || bHasIrregularPresent())
    {
        rc = eBuildIrregularForms();
        if (H_NO_ERROR != rc)
        {
            return rc;
        }
    }

    try
    {
        CEString sLemma;

        if (17 == m_pLexeme->iSection())
        {
            ATLASSERT (14 == m_pLexeme->iType());
            return H_NO_MORE;
        }

        if (4 == m_pLexeme->iType() || 5 == m_pLexeme->iType())
        {
            sLemma = m_pLexeme->s3SgStem();
        }
        else
        {
            sLemma = m_pLexeme->s1SgStem();
        }

        if (sLemma.bIsEmpty())
        {
            ATLASSERT(0);
            CEString sMsg (L"Empty lemma for ");
            sMsg += m_pLexeme->sInfinitive();
            ERROR_LOG (sMsg);
            return H_ERROR_GENERAL;
        }

        if (11 == m_pLexeme->iType())
        {
            ATLASSERT (sLemma.bEndsWith (L"ь"));
            sLemma.sErase (sLemma.uiLength()-1);
        }
        
        rc = eFleetingVowelCheck (sLemma);
        if (rc != H_NO_ERROR)
        {
            return rc;
        }

        CEString sEnding;
        rc = eGetEnding (sLemma, sEnding);
        if (rc != H_NO_ERROR)
        {
            return rc;
        }

        vector<int> vecStress;
        rc = eGetStressPositions (sLemma, sEnding, vecStress);
        if (rc != H_NO_ERROR)
        {
            return rc;
        }

        for (ET_Number eNumber = NUM_SG; eNumber < NUM_COUNT; ++eNumber)
        {
            vector<int>::iterator itStressPos = vecStress.begin();

            CWordForm * pWordForm = NULL;
            rc = eCreateFormTemplate (sLemma, sEnding, eNumber, pWordForm);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }

            if (1 == vecStress.size() || m_pLexeme->bIsMultistressedCompound())
            {
                vector<int>::iterator itStressPos = vecStress.begin();
                for (; itStressPos != vecStress.end(); ++itStressPos)
                {
                    pWordForm->m_mapStress[*itStressPos] = STRESS_PRIMARY;  // primary
                }
                rc = eHandleCommonDeviations (pWordForm);
                if (rc != H_NO_ERROR)
                {
                    return rc;
                }

                m_pLexeme->AddWordForm (pWordForm);
            }
            else
            {
                vector<int>::iterator itStressPos = vecStress.begin();
                for (; itStressPos != vecStress.end(); ++itStressPos)
                {
                    if (itStressPos != vecStress.begin())
                    {
                        CWordForm * pWfVariant = NULL;
                        CloneWordForm (pWordForm, pWfVariant);
                        pWordForm = pWfVariant;
                    }
                    pWordForm->m_mapStress[*itStressPos] = STRESS_PRIMARY;
                    rc = eHandleCommonDeviations (pWordForm);
                    if (rc != H_NO_ERROR)
                    {
                        return rc;
                    }

                    m_pLexeme->AddWordForm (pWordForm);
                }
            }
        }       //  for (; itStressPos != vecSourceStressPos.end(); ...)
    }
    catch (CException& ex)
    {
        CEString sMsg (L"Exception: ");
        sMsg += ex.szGetDescription();
        ERROR_LOG (sMsg);
        return H_EXCEPTION;
    }

    return rc;

}   //  eBuildImperativeForms()

//
// Irregular forms may or may not be in the database
//
ET_ReturnCode CFormBuilderImperative::eBuildIrregularForms()
{
    ET_ReturnCode rc = H_NO_ERROR;

    CGramHasher sg2ImpvHash (SUBPARADIGM_IMPERATIVE, 
                             NUM_SG, 
                             GENDER_UNDEFINED, 
                             PERSON_2, 
                             ANIM_UNDEFINED, 
                             m_pLexeme->eAspect(),
                             CASE_UNDEFINED, 
                             m_pLexeme->eIsReflexive());

    CGramHasher pl2ImpvHash (sg2ImpvHash);
    pl2ImpvHash.m_eNumber = NUM_PL;

    if (bHasIrregularImperative())
    {
        // Just add the plural form
        map<CWordForm *, bool> mapSg2Impv;
        rc = m_pLexeme->eGetIrregularForms(sg2ImpvHash.iGramHash(), mapSg2Impv);
        if (rc != H_NO_ERROR)
        {
            return rc;
        }

        map<CWordForm *, bool>::iterator it = mapSg2Impv.begin();
        for (; it != mapSg2Impv.end(); ++it)
        {
            CWordForm * pSgWf = it->first;
            pSgWf->m_sWordForm += L"те";
            if (REFL_YES == m_pLexeme->eIsReflexive())
            {
                (*it).first->m_sWordForm += L"сь";
            }
            m_pLexeme->AddWordForm(pSgWf);
        }
        
        return H_NO_ERROR;

    }   //   if (bHasIrregularImperative())

    //
    // No irregular forms in DB: derive from present tense
    //
    if (!bHasIrregularPresent())
    {
        ATLASSERT(0);
        CEString sMsg (L"No irregular present.");
        sMsg += m_pLexeme->sInfinitive();
        ERROR_LOG (sMsg);
        return H_ERROR_UNEXPECTED;
    }

    //
    // No imperative forms provided -- construct imperative from the present tense
    //
    CGramHasher pl3Hash (SUBPARADIGM_PRESENT_TENSE, NUM_PL, GENDER_UNDEFINED, PERSON_3, ANIM_UNDEFINED, 
                         m_pLexeme->eAspect(), CASE_UNDEFINED, m_pLexeme->eIsReflexive());

    map<CWordForm *, bool> mapPl3;
    rc = m_pLexeme->eGetIrregularForms(pl3Hash.iGramHash(), mapPl3);
    if (rc != H_NO_ERROR)
    {
        return rc;
    }

    map<CWordForm *, bool>::iterator it = mapPl3.begin();
    for (; it != mapPl3.end(); ++it)
    {
        CEString s3Pl ((*it).first->m_sWordForm);
        if (REFL_NO == m_pLexeme->eIsReflexive())
        {
            if (!s3Pl.bEndsWith (L"ют") && !s3Pl.bEndsWith (L"ут") && !s3Pl.bEndsWith (L"ят") && 
                !s3Pl.bEndsWith (L"ат"))
            {
                ATLASSERT(0);
                CEString sMsg (L"Unexpected 3 Pl ending: ");
                sMsg += s3Pl;
                ERROR_LOG (sMsg);
                return H_ERROR_UNEXPECTED;
            }
        }
        else if (REFL_YES == m_pLexeme->eIsReflexive())
        {
            if (!s3Pl.bEndsWith (L"ются") && !s3Pl.bEndsWith (L"утся") && 
                !s3Pl.bEndsWith (L"ятся") && !s3Pl.bEndsWith (L"атся"))
            {
                ATLASSERT(0);
                CEString sMsg (L"Unexpected 3 Pl refl. ending: ");
                sMsg += s3Pl;
                ERROR_LOG (sMsg);
                return H_ERROR_UNEXPECTED;
            }
        }
        else
        {
            ATLASSERT(0);
            CEString sMsg (L"Unexpected eo_Reflexive value for: ");
            sMsg += m_pLexeme->sInfinitive();
            ERROR_LOG (sMsg);
            return H_ERROR_UNEXPECTED;
        }

        int iCharsToErase = REFL_YES == m_pLexeme->eIsReflexive() ? 4 : 2;
        s3Pl.sErase (s3Pl.uiLength()-iCharsToErase);

        CWordForm * pSg = new CWordForm;
        CWordForm * pPl = new CWordForm;
        if (NULL == pSg || NULL == pPl)
        {
            ATLASSERT(0);
            CEString sMsg (L"Unable to instantiate CWordForm for ");
            sMsg += m_pLexeme->sInfinitive();
            ERROR_LOG (sMsg);
            return H_ERROR_UNEXPECTED;
        }

        if (s3Pl.bEndsWithOneOf (g_szConsonants))
        {
            pSg->m_sWordForm = s3Pl + L"и";
            pSg->m_mapStress[s3Pl.uiGetNumOfSyllables()] = STRESS_PRIMARY;
        }
        else
        {
            pSg->m_sWordForm = s3Pl + L"й";
            pSg->m_mapStress = (*it).first->m_mapStress;
        }

        pPl->m_sWordForm = pSg->m_sWordForm + L"те";
        pPl->m_mapStress = pSg->m_mapStress;

        if (REFL_YES == m_pLexeme->eIsReflexive())
        {
            int iAt = pSg->m_sWordForm.uiLength()-1;
            if (CEString::bIn (pSg->m_sWordForm[iAt], g_szVowels))
            {
                pSg->m_sWordForm += L"сь";
            }
            else
            {
                pSg->m_sWordForm += L"ся";
            }
            pPl->m_sWordForm += L"сь";
        }

        m_pLexeme->AddWordForm (pSg);
        m_pLexeme->AddWordForm (pPl);
    }

    return H_NO_ERROR;

}   //  eBuildIrregularForms()
