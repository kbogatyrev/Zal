#include "StdAfx.h"
#include "Endings.h"

#include "WordForm.h"
#include "Lexeme.h"
#include "FormBuilderPronounAdj.h"

int CFormBuilderPronounAdj::iGramHash (ET_PartOfSpeech ePos, 
                                       ET_Case eCase, 
                                       ET_Number eNumber, 
                                       ET_Gender eGender, 
                                       ET_Animacy eAnimacy)
{
    CGramHasher hasher (ePos, 
                        SUBPARADIGM_LONG_ADJ, 
                        eCase, 
                        eNumber, 
                        eGender,
                        PERSON_UNDEFINED,                             
                        eAnimacy, 
                        ASPECT_UNDEFINED, 
                        REFL_UNDEFINED); 

    int iHash = hasher.iGramHash();
    return iHash;
}

ET_ReturnCode CFormBuilderPronounAdj::eGetEndings()
{
    CEString sSelect (L"SELECT DISTINCT ending, gender, number, case_value, animacy, stress FROM endings");
    sSelect += L" WHERE inflection_class = " + CEString::sToString (ENDING_CLASS_PRONOUN);
    sSelect += L" AND inflection_type = " + CEString::sToString (m_pLexeme->iType()) + L";";

    try
    {
        m_pDb->PrepareForSelect(sSelect);
        while (m_pDb->bGetRow())
        {
            CEString sEnding;
            StEndingDescriptor stD;
            m_pDb->GetData (0, sEnding);
            m_pDb->GetData (1, (int&)stD.eGender);
            m_pDb->GetData (2, (int&)stD.eNumber);
            m_pDb->GetData (3, (int&)stD.eCase);
            m_pDb->GetData (4, (int&)stD.eAnimacy);
            m_pDb->GetData (5, (int&)stD.eStress);
            ET_ReturnCode rc = m_pEndings->eAddEnding (sEnding, stD);
        }
        m_pDb->Finalize();
    }
    catch (...)
    {
        ReportDbError();    
        return H_ERROR_GENERAL;;
    }

    return H_NO_ERROR;

}   //  eGetEndings()

ET_ReturnCode CFormBuilderPronounAdj::eGetStressPositions (const CEString& sEnding,
                                                           ET_StressLocation eStressType,
                                                           vector<int>& vecStressPos)
{
    ET_ReturnCode rc = H_NO_ERROR;

//    CEString sLemma (sLemma);
    m_sLemma.SetVowels (g_szVowels);

    int iStressPos = -1;

    if (STRESS_LOCATION_STEM == eStressType)
    {
        rc = eGetStemStressPositions (m_sLemma, m_eSubparadigm, vecStressPos);
        if (rc != H_NO_ERROR)
        {
            return rc;
        }
    }
    else if (STRESS_LOCATION_ENDING == eStressType)
    {
        if (sEnding.uiNSyllables() < 1)
        {
            iStressPos = m_sLemma.uiNSyllables() - 1;
        }
        else
        {
            if (L"мс-п" == m_pLexeme->sInflectionType() &&
                (L"его" == sEnding || L"ему" == sEnding ||
                 L"ого" == sEnding || L"ому" == sEnding))
            {
                iStressPos = m_sLemma.uiNSyllables() + 1;    // одног<о, твоем<у
            }
            else
            {
                iStressPos = m_sLemma.uiNSyllables();
            }
        }

//        hGetEndingStressPosition (str_Lemma, sEnding, i_stressPos);
        vecStressPos.push_back (iStressPos);
    }
    else
    {
        ATLASSERT (0);
        ERROR_LOG (L"Illegal stress type.");
        return H_ERROR_INVALID_ARG;
    }

    return rc;

}   //  eGetStressPositions (...)

ET_ReturnCode CFormBuilderPronounAdj::eCreateFormTemplate (ET_Gender eGender,
                                                           ET_Number eNumber, 
                                                           ET_Case eCase, 
                                                           ET_Animacy eAnimacy,
                                                           const CEString& sEnding,
                                                           CWordForm *& pWordForm)
{
    ET_ReturnCode rc = H_NO_ERROR;
    
    pWordForm = new CWordForm;
    if (NULL == pWordForm)
    {
        ATLASSERT(0);
        ERROR_LOG (L"Unable to instantiate CWordForm.");
        return H_ERROR_POINTER;
    }

    pWordForm->m_pLexeme = m_pLexeme;
    pWordForm->m_ePos = m_pLexeme->ePartOfSpeech();
    pWordForm->m_eSubparadigm = SUBPARADIGM_LONG_ADJ;
    pWordForm->m_sLemma = m_sLemma;
    pWordForm->m_eGender = eGender;
    pWordForm->m_eCase = eCase;
    pWordForm->m_eNumber = eNumber;
    pWordForm->m_eAnimacy = eAnimacy;
    pWordForm->m_sWordForm = m_sLemma + sEnding;
    pWordForm->m_eStatus = m_eStatus;

//    rc = eAssignSecondaryStress (pWordForm);

    return rc;

}   //  eCreateFormTemplate (...)

ET_ReturnCode CFormBuilderPronounAdj::eHandleCommonDeviations (CWordForm * pWordForm)
{
    return H_NO_ERROR;

}   //  eHandleCommonDeviations (...)

ET_ReturnCode CFormBuilderPronounAdj::eBuild()
{
    ET_ReturnCode rc = H_NO_ERROR;

    rc = eGetEndings();
    if (rc != H_NO_ERROR)
    {
        return rc;
    }

    CHasher gramTmp;
    gramTmp.Initialize (POS_PRONOUN_ADJ);
    gramTmp.SetParadigm (SUBPARADIGM_LONG_ADJ);
    do
    {
        if (m_pLexeme->bHasIrregularForms())
        {
            int iPronAdj = iGramHash (m_pLexeme->ePartOfSpeech(),
                                      gramTmp.m_eCase, gramTmp.m_eNumber,
                                      gramTmp.m_eGender, gramTmp.m_eAnimacy);

            map<CWordForm *, bool> mapPronAdj;
            rc = m_pLexeme->eGetIrregularForms(iPronAdj, mapPronAdj);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }

            bool bSkipRegular = false;
            map<CWordForm *, bool>::iterator it = mapPronAdj.begin();
            for (; it != mapPronAdj.end(); ++it)
            {                    
                CWordForm * pWordForm = NULL;
                rc = eCreateFormTemplate (gramTmp.m_eGender, 
                                          gramTmp.m_eNumber, 
                                          gramTmp.m_eCase, 
                                          gramTmp.m_eAnimacy, 
                                          L"", 
                                          pWordForm);
                if (rc != H_NO_ERROR)
                {
                    return rc;
                }

                pWordForm->m_sWordForm = (*it).first->m_sWordForm;
                pWordForm->m_mapStress = (*it).first->m_mapStress;
                m_pLexeme->AddWordForm (pWordForm);
            }

            if (!(*it).second)  // i.e. no regular variant
            {
                continue;
            }
        }       //  if (m_pLexeme->m_bHasIrregularForms)

        ET_StressLocation eStressType = STRESS_LOCATION_UNDEFINED;
        switch (m_pLexeme->eAccentType1())
        {
            case AT_A:
            {
                eStressType = STRESS_LOCATION_STEM;
                break;
            }
            case AT_B:
            {
                eStressType = STRESS_LOCATION_ENDING;
                break;
            }
            case AT_F:
            {
                if (NUM_SG == gramTmp.m_eNumber)
                {
                    eStressType = STRESS_LOCATION_ENDING;
                }
                else if (CASE_NOM == gramTmp.m_eCase)
                {
                    eStressType = STRESS_LOCATION_STEM;
                }
                else if (CASE_ACC == gramTmp.m_eCase && ANIM_NO == gramTmp.m_eAnimacy)
                {
                    eStressType = STRESS_LOCATION_STEM;
                }
                else
                {
                    eStressType = STRESS_LOCATION_ENDING;
                }
                break;
            }
            default:
            {
                ATLASSERT(0);
                ERROR_LOG (L"Illegal accent type.");
                return H_ERROR_UNEXPECTED;
            }

        }   //  switch (m_pLexeme->eAccentType1())

        StEndingDescriptor stD (gramTmp.m_eGender, gramTmp.m_eNumber,
                                gramTmp.m_eCase, gramTmp.m_eAnimacy, eStressType);
        int iNumEndings = m_pEndings->iCount(stD);
        if (iNumEndings < 1)
        {
            ERROR_LOG (L"No endings");
            continue;
        }

        for (int iEnding = 0; iEnding < iNumEndings; ++iEnding)
        {
            CEString sEnding;
            rc = m_pEndings->eGetEnding(stD, iEnding, sEnding);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }

            if (m_pLexeme->bFleetingVowel() &&
                (gramTmp.m_eCase != CASE_NOM || gramTmp.m_eGender != GENDER_M) &&
                (gramTmp.m_eCase != CASE_ACC || gramTmp.m_eGender != GENDER_M ||
                                                    gramTmp.m_eAnimacy != ANIM_NO))
            {
                rc = eFleetingVowelCheck (gramTmp.m_eNumber, gramTmp.m_eCase,
                                          gramTmp.m_eGender, eStressType,
                                          SUBPARADIGM_LONG_ADJ, sEnding, m_sLemma);
                if (rc != H_NO_ERROR)
                {
                    return rc;
                }
            }

            CWordForm * pWordForm = NULL;
            rc = eCreateFormTemplate(gramTmp.m_eGender, 
                                     gramTmp.m_eNumber, 
                                     gramTmp.m_eCase, 
                                     gramTmp.m_eAnimacy, 
                                     sEnding, 
                                     pWordForm);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }
//                            pWordForm->str_WordForm = str_Lemma + sEnding;

            vector<int> vecStress;
            rc = eGetStressPositions (sEnding, eStressType, vecStress);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }

            if (1 == vecStress.size() || m_pLexeme->bIsMultistressedCompound())
            {
                vector<int>::iterator itStressPos = vecStress.begin();
                for (; itStressPos != vecStress.end(); ++itStressPos)
                {
                    pWordForm->m_mapStress[*itStressPos] = STRESS_PRIMARY;
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
                    m_pLexeme->AddWordForm (pWordForm);
                }
            }
        }
    } while (gramTmp.bIncrement());

    return H_NO_ERROR;

}   //  eBuild (...)
