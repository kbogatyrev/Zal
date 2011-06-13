#include "StdAfx.h"
#include "Endings.h"
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

HRESULT CFormBuilderPronounAdj::hGetEndings()
{
    HRESULT hr = S_OK;

//    int iType = -1;

    CEString sSelect (L"SELECT DISTINCT ending, gender, number, case_value, animacy, stress FROM endings");
    sSelect += L" WHERE inflection_class = " + CEString::sToString (ENDING_CLASS_PRONOUN);
    sSelect += L" AND inflection_type = " + CEString::sToString (m_pLexeme->m_iType) + L";";

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
            HRESULT hr = m_pEndings->hAddEnding (sEnding, stD);
        }
        m_pDb->Finalize();
    }
    catch (...)
    {
        ReportDbError();    
        throw CException (E_FAIL, L"Error getting endings.");
    }
    
    return S_OK;

}   //  b_GetEndings()

HRESULT CFormBuilderPronounAdj::hGetStressPositions (const CEString& sEnding,
                                                     ET_StressLocation eStressType,
                                                     vector<int>& vecStressPos)
{
    HRESULT hr = S_OK;

//    CEString sLemma (sLemma);
    m_sLemma.SetVowels (g_szVowels);

    int iStressPos = -1;

    if (STRESS_LOCATION_STEM == eStressType)
    {
        hGetStemStressPositions (m_sLemma, m_eSubparadigm, vecStressPos);
    }
    else if (STRESS_LOCATION_ENDING == eStressType)
    {
        if (sEnding.uiNSyllables() < 1)
        {
            iStressPos = m_sLemma.uiNSyllables() - 1;
        }
        else
        {
            if (L"мс-п" == m_pLexeme->m_sInflectionType &&
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
        CEString sMsg (L"Illegal stress type.");
        ERROR_LOG (sMsg);
        throw CException (E_INVALIDARG, sMsg);
    }
    if (S_OK != hr)
    {
        CEString sMsg (L"Error getting stress positions.");
        ERROR_LOG (sMsg);
        throw CException (E_INVALIDARG, sMsg);
    }

    return S_OK;

}   //  h_GetStressPositions (...)

HRESULT CFormBuilderPronounAdj::hCreateFormTemplate (ET_Gender eGender,
                                                     ET_Number eNumber, 
                                                     ET_Case eCase, 
                                                     ET_Animacy eAnimacy,
                                                     const CEString& sEnding,
                                                     CComObject<CWordForm> *& pWordForm)
{
    HRESULT hr = S_OK;

    hr = CComObject<CWordForm>::CreateInstance(&pWordForm);
    if (S_OK != hr)
    {
        ATLASSERT(0);
        CString csMsg;
        csMsg.Format (L"CreateInstance() failed, error %x.", hr);
        ERROR_LOG ((LPCTSTR)csMsg);
        throw CException (hr, (LPCTSTR)csMsg);
    }

    pWordForm->m_pLexeme = m_pLexeme;
    pWordForm->m_ePos = m_pLexeme->m_ePartOfSpeech;
    pWordForm->m_eSubparadigm = SUBPARADIGM_LONG_ADJ;
    pWordForm->m_sLemma = m_sLemma;
    pWordForm->m_eGender = eGender;
    pWordForm->m_eCase = eCase;
    pWordForm->m_eNumber = eNumber;
    pWordForm->m_eAnimacy = eAnimacy;
    pWordForm->m_sWordForm = m_sLemma + sEnding;
    pWordForm->m_eStatus = m_eStatus;

    AssignSecondaryStress (pWordForm);

    return S_OK;

}   //  hCreateFormTemplate (...)

HRESULT CFormBuilderPronounAdj::hHandleCommonDeviations (CWordForm * pWordForm)
{
    return S_OK;

}   //  h_HandleCommonDeviations (...)

HRESULT CFormBuilderPronounAdj::hBuild()
{
    HRESULT hr = S_OK;

    try
    {
        hGetEndings();

        CHasher gramTmp;
        gramTmp.Initialize (POS_PRONOUN_ADJ);
        gramTmp.SetParadigm (SUBPARADIGM_LONG_ADJ);
        do
        {
            if (m_pLexeme->m_bHasIrregularForms)
            {
                multimap<int, StIrregularForm>& mmapIf = m_pLexeme->m_mmapIrregularForms;
                int iHash = iGramHash (m_pLexeme->m_ePartOfSpeech,
                                       gramTmp.m_eCase, gramTmp.m_eNumber,
                                       gramTmp.m_eGender, gramTmp.m_eAnimacy);
                StIrregularForm stIf;
                hr = m_pLexeme->hGetFirstIrregularForm (iHash, stIf);
                bool bSkipRegular = false;
                while (S_OK == hr)
                {
                    if (stIf.bIsVariant)
                    {
                        bSkipRegular = false;
                    }
                    else
                    {
                        bSkipRegular = true;
                    }
                    
                    CComObject<CWordForm> * pWordForm = NULL;
                    hCreateFormTemplate (gramTmp.m_eGender, 
                                         gramTmp.m_eNumber, 
                                         gramTmp.m_eCase, 
                                         gramTmp.m_eAnimacy, 
                                         L"", 
                                         pWordForm);
                    pWordForm->m_sWordForm = stIf.sForm;
                    pWordForm->m_mapStress = stIf.mapStress;
                    m_pLexeme->hAddWordForm (pWordForm);
                    hr = m_pLexeme->hGetNextIrregularForm (iHash, stIf);
                }

                if (bSkipRegular)
                {
                    continue;
                }
            }       //  if (m_pLexeme->m_bHasIrregularForms)

            ET_StressLocation eStressType = STRESS_LOCATION_UNDEFINED;
            switch (m_pLexeme->m_eAccentType1)
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
                    CEString sMsg (L"Illegal accent type.");
                    ERROR_LOG (sMsg);
                    throw CException (E_FAIL, sMsg);
                }

            }   //  switch (m_pLexeme->m_eAccentType1)

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
                m_pEndings->hGetEnding(stD, iEnding, sEnding);

                if (m_pLexeme->m_bFleetingVowel &&
                    (gramTmp.m_eCase != CASE_NOM || gramTmp.m_eGender != GENDER_M) &&
                    (gramTmp.m_eCase != CASE_ACC || gramTmp.m_eGender != GENDER_M ||
                                                        gramTmp.m_eAnimacy != ANIM_NO))
                {
                    hFleetingVowelCheck (gramTmp.m_eNumber, gramTmp.m_eCase,
                                         gramTmp.m_eGender, eStressType,
                                         SUBPARADIGM_LONG_ADJ, sEnding, m_sLemma);
                }

                CComObject<CWordForm> * pWordForm = NULL;
                hCreateFormTemplate(gramTmp.m_eGender, 
                                     gramTmp.m_eNumber, 
                                     gramTmp.m_eCase, 
                                     gramTmp.m_eAnimacy, 
                                     sEnding, 
                                     pWordForm);
//                            pWordForm->str_WordForm = str_Lemma + sEnding;

                vector<int> vecStress;
                hGetStressPositions (sEnding, eStressType, vecStress);
                if (1 == vecStress.size() || bMultiStress (m_sLemma, vecStress))
                {
                    vector<int>::iterator itStressPos = vecStress.begin();
                    for (; itStressPos != vecStress.end(); ++itStressPos)
                    {
                        pWordForm->m_mapStress[*itStressPos] = STRESS_PRIMARY;
                    }
                    m_pLexeme->hAddWordForm (pWordForm);
                }
                else
                {
                    vector<int>::iterator itStressPos = vecStress.begin();
                    for (; itStressPos != vecStress.end(); ++itStressPos)
                    {
                        if (itStressPos != vecStress.begin())
                        {
                            CComObject<CWordForm> * pWfVariant = NULL;
                            hCloneWordForm (pWordForm, pWfVariant);
                            pWordForm = pWfVariant;
                        }
                        pWordForm->m_mapStress[*itStressPos] = STRESS_PRIMARY;
                        m_pLexeme->hAddWordForm (pWordForm);
                    }
                }
            }
        } while (gramTmp.bIncrement());
    }
    catch (CException ex)
    {
        return ex.iGetErrorCode();  // logging should be always done by callee
    }

    return S_OK;

}   //  hBuild (...)
