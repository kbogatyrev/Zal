#include "StdAfx.h"
#include "Endings.h"
#include "FormBuilderNouns.h"

int CFormBuilderNouns::i_GramHash (ET_Gender eoGender,
                                     ET_Animacy eAnimacy,
                                     ET_Case eCase,
                                     ET_Number eNumber)
{
    CGramHasher hasher (eoGender, eAnimacy, eCase, eNumber);

    int iHash = hasher.iGramHash();

    return iHash;
}

HRESULT CFormBuilderNouns::hGetEndings()
{
    HRESULT hr = S_OK;

    if (NULL == m_pLexeme)
    {
        ATLASSERT(0);
        CEString sMsg (L"Null pointer to lexeme object.");
        ERROR_LOG (sMsg);
        throw CException (E_POINTER, sMsg);
    }

    if (NULL == m_pEndings)
    {
        ATLASSERT(0);
        CEString sMsg (L"Null pointer to endings object.");
        ERROR_LOG (sMsg);
        throw CException (E_POINTER, sMsg);
    }

    CEString sSelect (L"SELECT DISTINCT ending, number, case_value, stress FROM endings");
    sSelect += L" WHERE inflection_class = ";
    sSelect += CEString::sToString (ENDING_CLASS_NOUN);
    sSelect += L" AND inflection_type = ";
    sSelect += CEString::sToString (m_pLexeme->m_iType);
    sSelect += L" AND (gender = ";
    sSelect += CEString::sToString (m_pLexeme->eGetGender());
    sSelect += L" OR gender = ";
    sSelect += CEString::sToString (GENDER_UNDEFINED) + L")";
    sSelect += L" AND (animacy = ";
    sSelect += CEString::sToString (m_pLexeme->eGetAnimacy());
    sSelect += L" OR animacy = ";
    sSelect += CEString::sToString (ANIM_UNDEFINED) + L")";
    sSelect += L" AND stem_augment = ";
//    sSelect += m_pLexeme->b_StemAugment ? L"1" : L"0";
    sSelect += CEString::sToString (m_pLexeme->m_iStemAugment);

    try
    {
        CEString sSelectBase (sSelect);
        sSelectBase += L" AND common_deviation = 0;";

        m_pDb->PrepareForSelect (sSelectBase);
        while (m_pDb->bGetRow())
        {
            CEString sEnding;
            StEndingDescriptor stD;
            m_pDb->GetData (0, sEnding);
            m_pDb->GetData (1, (int&)stD.eNumber);
            m_pDb->GetData (2, (int&)stD.eCase);
            m_pDb->GetData (3, (int&)stD.eStress);

            map<int, bool>::iterator itCd = m_pLexeme->m_mapCommonDeviations.find (1);
            if (m_pLexeme->m_mapCommonDeviations.end() != itCd)
            {
                if (NUM_PL == stD.eNumber && CASE_NOM == stD.eCase && GENDER_F != stD.eGender)
                {
					if (!(*itCd).second)	// optional?
                    {
                        continue;
                    }
                }
            }

            itCd = m_pLexeme->m_mapCommonDeviations.find (2);
            if (m_pLexeme->m_mapCommonDeviations.end() != itCd)
            {
                if (NUM_PL == stD.eNumber && CASE_GEN == stD.eCase)
                {
					if (!(*itCd).second)	// optional?
                    {
                        continue;
                    }
                }
            }
            m_pEndings->hAddEnding (sEnding, stD);

        }   //  while (m_pDb->bGetRow())

        m_pDb->Finalize();
    }
    catch (...)
    {
        ReportDbError();    
        throw CException (E_FAIL, L"DB error");
    }

    for (int iCd = 1; iCd <= 2; ++iCd)
    {
        map<int, bool>::iterator itCd = m_pLexeme->m_mapCommonDeviations.find (iCd);
        if (m_pLexeme->m_mapCommonDeviations.end() == itCd)
        {
            continue;
        }

        CEString sSelectCD (sSelect);
        sSelectCD += L" AND common_deviation = ";
        sSelectCD += CEString::sToString (iCd);
        sSelectCD += L";";

        try
        {
            m_pDb->PrepareForSelect (sSelectCD);
            while (m_pDb->bGetRow())
            {
                CEString sEnding;
                StEndingDescriptor stD;
                m_pDb->GetData (0, sEnding);
                m_pDb->GetData (1, (int&)stD.eNumber);
                m_pDb->GetData (2, (int&)stD.eCase);
                m_pDb->GetData (3, (int&)stD.eStress);
                m_pEndings->hAddEnding (sEnding, stD);
            }
            m_pDb->Finalize();
        }
        catch (...)
        {
            ReportDbError();        
            throw CException (E_FAIL, L"DB error");
        }

    }   // for ...

    return S_OK;

}   //  hGetEndings()

HRESULT CFormBuilderNouns::hHandleStemAugment (CEString& sLemma, ET_Number eNumber, ET_Case eCase)
{
    if (m_pLexeme->m_iStemAugment < 1)
    {
        return S_OK;
    }

    if (1 == m_pLexeme->m_iType)
    {
        sLemma.sErase (sLemma.uiLength()-2, 2);  // римлянин, южанин, армянин
    }
    if (3 == m_pLexeme->m_iType)
    {
        CEString& sGs = m_pLexeme->m_sGraphicStem;
        if (NUM_SG == eNumber)
        {
            if ((CASE_NOM == eCase) || 
                (ANIM_NO == m_pLexeme->eGetAnimacy() && CASE_ACC == eCase))
            {
                return S_OK;
            }
            else
            {
                sLemma.sErase (sLemma.uiLength()-2, 1);
                return S_OK;
            }
        }
        if (NUM_PL == eNumber)
        {
            if (sGs.bEndsWith (L"онок"))
            {
                sLemma.sErase (sLemma.uiLength()-4, 4);
                sLemma += L"ат";
                return S_OK;
            }
            if (sGs.bEndsWith (L"ёнок"))
            {
                sLemma.sErase (sLemma.uiLength()-4, 4);
                sLemma += L"ят";
                return S_OK;
            }
            if (sGs.bEndsWith (L"оночек"))
            {
                sLemma.sErase (sLemma.uiLength()-6, 6);
                if ((CASE_GEN == eCase) ||                        
                    (CASE_ACC == eCase && ANIM_YES == m_pLexeme->eGetAnimacy()))
                                                     // they all should be animate?
                {
                    sLemma += L"аток";
                }
                else
                {
                    sLemma += L"атк";
                }
                return S_OK;
            }
            if (sGs.bEndsWith (L"ёночек"))
            {
                sLemma.sErase (sLemma.uiLength()-6, 6);
                if ((CASE_GEN == eCase) ||                        
                    (CASE_ACC == eCase && ANIM_YES == m_pLexeme->eGetAnimacy()))
                                                     // they all should be animate?
                {
                    sLemma += L"яток";
                }
                else
                {
                    sLemma += L"ятк";
                }
                return S_OK;
            }
        }
    }       //  if (3 == m_pLexeme->i_Type)

    return S_OK;

}   //  hHandleStemAugment (...)

HRESULT CFormBuilderNouns::hGetStressType (ET_Number eNumber, ET_Case eCase, ET_StressLocation& eStressType)
{
    if (NUM_UNDEFINED == eNumber)
    {
        ATLASSERT(0);
        CEString sMsg (L"Undefined number.");
        ERROR_LOG (sMsg);
        throw CException (E_INVALIDARG, sMsg);
    }

// &&&& why default??
    eStressType = STRESS_LOCATION_STEM;

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
        case AT_B1:
        {
            if (NUM_SG == eNumber && CASE_INST == eCase)
            {
                eStressType = STRESS_LOCATION_STEM;
            }
            else
            {
                eStressType = STRESS_LOCATION_ENDING;
            }
            break;
        }
        case AT_C:
        {
            if (NUM_SG == eNumber)
            {
                eStressType = STRESS_LOCATION_STEM;
            }
            else
            {
                eStressType = STRESS_LOCATION_ENDING;
            }
            break;
        }
        case AT_D:
        {
            if (NUM_SG == eNumber)
            {
                eStressType = STRESS_LOCATION_ENDING;
            }
            else
            {
                eStressType = STRESS_LOCATION_STEM;
            }
            break;
        }
        case AT_D1:
        {
            if (GENDER_F != m_pLexeme->eGetGender())
            {
                ATLASSERT(0);
                CEString sMsg (L"Non-feminine noun with D1 stress.");
                ERROR_LOG (sMsg);
                throw CException (E_FAIL, sMsg);
            }
            if (NUM_SG == eNumber && CASE_ACC == eCase) // assume fem -a stems only?
            {
                eStressType = STRESS_LOCATION_STEM;
            }
            else if (NUM_SG == eNumber)
            {
                eStressType = STRESS_LOCATION_ENDING;
            }
            else
            {
                eStressType = STRESS_LOCATION_STEM;
            }
            break;
        }
        case AT_E:
        {
            if (NUM_SG == eNumber)
            {
                eStressType = STRESS_LOCATION_STEM;
            }
            else if (CASE_NOM == eCase)
            {
                eStressType = STRESS_LOCATION_STEM;
            }
            else if (CASE_ACC == eCase && ANIM_NO == m_pLexeme->eGetAnimacy())
            {
                eStressType = STRESS_LOCATION_STEM;
            }
            else
            {
                eStressType = STRESS_LOCATION_ENDING;
            }
            break;
        }

        case AT_F:
        {
            if (NUM_SG == eNumber)
            {
                eStressType = STRESS_LOCATION_ENDING;
            }
            else if (CASE_NOM == eCase)
            {
                eStressType = STRESS_LOCATION_STEM;
            }
            else if (CASE_ACC == eCase && ANIM_NO == m_pLexeme->eGetAnimacy())
            {
                eStressType = STRESS_LOCATION_STEM;
            }
            else
            {
                eStressType = STRESS_LOCATION_ENDING;
            }
            break;
        }
        case AT_F1:
        {
            if (GENDER_F != m_pLexeme->eGetGender())
            {
                ATLASSERT(0);
                CEString sMsg (L"Non-feminine noun with F1 stress.");
                ERROR_LOG (sMsg);
                throw CException (E_FAIL, sMsg);
            }
            if (NUM_PL == eNumber && CASE_NOM == eCase)
            {
                eStressType = STRESS_LOCATION_STEM;
            }
            else if (NUM_SG == eNumber && CASE_ACC == eCase) // fem only?
            {
                eStressType = STRESS_LOCATION_STEM;
            }
            else
            {
                eStressType = STRESS_LOCATION_ENDING;
            }
            break;
        }
        case AT_F2:
        {
            if (GENDER_F != m_pLexeme->eGetGender() || 8 != m_pLexeme->m_iType)
            {
                ATLASSERT(0);   // assume f -i stems only?
                CEString sMsg (L"Non-feminine/type 8 noun with F2 stress.");
                ERROR_LOG (sMsg);
                throw CException (E_FAIL, sMsg);
            }
            if (ANIM_YES == m_pLexeme->eGetAnimacy())
            {
                ATLASSERT(0);                   // inanimate only?
                CEString sMsg (L"Animate noun with F2 stress.");
                ERROR_LOG (sMsg);
                throw CException (E_FAIL, sMsg);
            }
            if (NUM_PL == eNumber && (CASE_NOM == eCase || CASE_ACC == eCase))
            {
                eStressType = STRESS_LOCATION_STEM;
            }
            else if (NUM_SG == eNumber && CASE_INST == eCase)
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
    }

    return S_OK;

}   // hGetStressType()

HRESULT CFormBuilderNouns::hHandleAccEnding (ET_Number eNumber, ET_Case& eCase)
{
    if (NUM_SG == eNumber)
    {
        if (GENDER_M == m_pLexeme->eGetGender())
        {
            ANIM_YES == m_pLexeme->eGetAnimacy()
                ? eCase = CASE_GEN
                : eCase = CASE_NOM;
        }
        if (GENDER_N == m_pLexeme->eGetGender())
        {
            eCase = CASE_NOM;
        }
    }
    if (NUM_PL == eNumber)
    {
        ANIM_YES == m_pLexeme->eGetAnimacy()
            ? eCase = CASE_GEN
            : eCase = CASE_NOM;
    }

    return S_OK;

}   //  hHandleAccEnding

HRESULT CFormBuilderNouns::hGetStressPositions (const CEString& sLemma, 
                                                const CEString& sEnding,
                                                ET_StressLocation eStressType,
                                                vector<int>& vecStressPos)
{
    HRESULT hr = S_OK;

    int iStressPos = -1;
    if (STRESS_LOCATION_STEM == eStressType)
    {
        hGetStemStressPositions (sLemma, SUBPARADIGM_NOUN, vecStressPos);
    }
    else if (STRESS_LOCATION_ENDING == eStressType)
    {
        hGetEndingStressPosition (sLemma, sEnding, iStressPos);
        vecStressPos.push_back (iStressPos);
    }
    else
    {
        ATLASSERT(0);
        CEString sMsg (L"Bad stress type.");
        ERROR_LOG (sMsg);
        throw CException (E_FAIL, sMsg);
    }

    if (vecStressPos.empty())
    {
        vecStressPos.push_back (-1);
    }

    return S_OK;

}   //  hGetStressPositions (...)

HRESULT CFormBuilderNouns::hCreateFormTemplate (ET_Number eNumber, 
                                                ET_Case eCase, 
                                                const CEString& sLemma,
                                                CComObject<CWordForm> *& pWordForm)
{
    HRESULT hr = S_OK;

    hr = CComObject<CWordForm>::CreateInstance (&pWordForm);
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
    pWordForm->m_eSubparadigm = SUBPARADIGM_NOUN;
    pWordForm->m_eCase = eCase;
    pWordForm->m_sLemma = sLemma;
    pWordForm->m_eNumber = eNumber;
    pWordForm->m_eGender = m_pLexeme->eGetGender();
    pWordForm->m_eAnimacy = m_pLexeme->eGetAnimacy();
    AssignSecondaryStress (pWordForm);

    return hr;

}   //  h_CreateFormTemplate (...)

HRESULT CFormBuilderNouns::hAssemble (CWordForm * pWordForm, 
                                      ET_StressLocation eStressLocation,
                                      int iStressPos, 
                                      const CEString& sEnding)
{
    pWordForm->m_mapStress[iStressPos] = STRESS_PRIMARY;  // primary
    hHandleYoAlternation (eStressLocation, iStressPos, pWordForm->m_sLemma);
    pWordForm->m_sWordForm = pWordForm->m_sLemma + sEnding;

    return S_OK;
}

HRESULT CFormBuilderNouns::hCheckIrregularForms (ET_Gender eoGender, 
                                                 ET_Animacy eAnimacy, 
                                                 ET_Case eCase, 
                                                 ET_Number eNumber, 
                                                 bool& bHandled)
{
    HRESULT hr = S_OK;

    bHandled = false;

    multimap<int, StIrregularForm>& mmapIf = m_pLexeme->m_mmapIrregularForms;
    int iHash = i_GramHash (eoGender, eAnimacy, eCase, eNumber);
    StIrregularForm stIf;
    hr = m_pLexeme->hGetFirstIrregularForm (iHash, stIf);
    while (S_OK == hr)
    {
        if (stIf.bIsVariant)
        {
            bHandled = false;
        }
        else
        {
            bHandled = true;
        }
        CComObject<CWordForm> * pWordForm = NULL;
        hCreateFormTemplate (eNumber, eCase, m_pLexeme->m_sGraphicStem, pWordForm);
        pWordForm->m_sWordForm = stIf.sForm;
        pWordForm->m_mapStress = stIf.mapStress;
        m_pLexeme->hAddWordForm (pWordForm);
        hr = m_pLexeme->hGetNextIrregularForm (iHash, stIf);
    }

    return hr;

}   //  hCheckIrregularForms (...)

HRESULT CFormBuilderNouns::hBuild()
{
    HRESULT hr = S_OK;

    try
    {
        hr = hGetEndings();

        ET_Animacy eAnimacy = m_pLexeme->eGetAnimacy();
        ET_Gender eoGender = m_pLexeme->eGetGender();

        CHasher gramIterator;
        gramIterator.Initialize(eoGender, eAnimacy);
        do
        {
            if ((L"мн." == m_pLexeme->m_sMainSymbol) && (gramIterator.m_eNumber == NUM_SG))
            {
                continue;
            }

            CEString sLemma (m_pLexeme->m_sGraphicStem);
            if (m_pLexeme->m_bHasIrregularForms)
            {
                bool bSkipRegular = false;
                hCheckIrregularForms (gramIterator.m_eGender, 
                                      gramIterator.m_eAnimacy,
                                      gramIterator.m_eCase,
                                      gramIterator.m_eNumber,
                                      bSkipRegular);
                if (bSkipRegular)
                {
                    continue;
                }
            }

            hr = hHandleStemAugment (sLemma, gramIterator.m_eNumber, gramIterator.m_eCase);

            ET_StressLocation eStress = STRESS_LOCATION_UNDEFINED;
            hGetStressType (gramIterator.m_eNumber, gramIterator.m_eCase, eStress);

            // Handle acc ending
            ET_Case eEndingCase = gramIterator.m_eCase;
            if (CASE_ACC == gramIterator.m_eCase)
            {
                hHandleAccEnding (gramIterator.m_eNumber, eEndingCase);
            }

            int iNumEndings = m_pEndings->iCount (StEndingDescriptor (gramIterator.m_eNumber, eEndingCase, eStress));
            if (iNumEndings < 1)
            {
//                ATLASSERT(0);
                ERROR_LOG (L"No endings");
                continue;
            }

            for (int iEnding = 0; iEnding < iNumEndings; ++iEnding)
            {
                // Get ending and modify as necessary
                CEString sEnding;
                hr = m_pEndings->hGetEnding (StEndingDescriptor (gramIterator.m_eNumber, eEndingCase, eStress),
                                             iEnding,
                                             sEnding);
                if (8 == m_pLexeme->m_iType)
                {
                    if (sLemma.bEndsWithOneOf (L"шжчщц"))
                    {
                        if (sEnding.bStartsWith (L"я"))
                        {
                            continue;
                        }
                    }
                    else
                    {
                        if (sEnding.bStartsWith (L"а"))
                        {
                            continue;
                        }
                    }
                }

                bool bHasFleetingVowel = false;
                hr = hFleetingVowelCheck (gramIterator.m_eNumber, 
                                          eEndingCase,
                                          gramIterator.m_eGender, 
                                          eStress,
                                          SUBPARADIGM_NOUN,
                                          sEnding,
                                          sLemma);

                vector<int> vecStress;
                hGetStressPositions (sLemma, sEnding, eStress, vecStress);
                CComObject<CWordForm> * pWordForm = NULL;
                hCreateFormTemplate (gramIterator.m_eNumber, gramIterator.m_eCase, sLemma, pWordForm);
                if (1 == vecStress.size() || bMultiStress (sLemma, vecStress))
                {
                    vector<int>::iterator itStressPos = vecStress.begin();
                    for (; itStressPos != vecStress.end(); ++itStressPos)
                    {
                        hAssemble (pWordForm, eStress, *itStressPos, sEnding);
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
                            CComObject<CWordForm> * pwfVariant = NULL;
                            hCloneWordForm (pWordForm, pwfVariant);
                            pwfVariant->m_mapStress.clear();
                            pWordForm = pwfVariant;
                        }
                        hAssemble (pWordForm, eStress, *itStressPos, sEnding);
                        m_pLexeme->hAddWordForm (pWordForm);
                    }
                }
            }   //  for (int iEnding = 0; ... )
        
        } while (gramIterator.bIncrement());
    }
    catch (CException ex)
    {
        return ex.iGetErrorCode();  // logging should be always done by callee
    }

    return S_OK;

}    //  hBuildNounForms()
