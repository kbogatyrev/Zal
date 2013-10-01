#include "StdAfx.h"
#include "Endings.h"
#include "FormBuilderVerbPers.h"

void CFormBuilderPersonal::GetEndings()
{
    if (POS_VERB != m_pLexeme->m_ePartOfSpeech)
    {
        CEString sMsg (L"Bad part of speech value.");
        ERROR_LOG (sMsg);
        throw CException (E_UNEXPECTED, sMsg);
    }

    CEString sSelect 
        (L"SELECT DISTINCT ending, person, number, stress, stem_auslaut, conjugation FROM endings");
    sSelect += L" WHERE inflection_class = 5;";

    try
    {
        m_pDb->PrepareForSelect (sSelect);
        while (m_pDb->bGetRow())
        {
            CEString sEnding;
            StEndingDescriptor stD;
            m_pDb->GetData (0, sEnding);
            m_pDb->GetData (1, (int&)stD.ePerson);
            m_pDb->GetData (2, (int&)stD.eNumber);
            m_pDb->GetData (3, (int&)stD.eStress);
            m_pDb->GetData (4, (int&)stD.eStemAuslaut);
            m_pDb->GetData (5, stD.iInflectionType);
            m_pEndings->hAddEnding (sEnding, stD);
        }
        m_pDb->Finalize();
    }
    catch (...)
    {
        ReportDbError();    
        throw CException (E_FAIL, sMsg);
    }

}   //  GetEndings()

HRESULT CFormBuilderPersonal::hGetStressType (ET_Number eNumber, ET_Person ePerson,  ET_StressLocation& eStress)
{
    HRESULT hr = S_OK;

    switch (m_pLexeme->m_eAccentType1)
    {
        case AT_A:
        {
            eStress = STRESS_LOCATION_STEM;
            break;
        }
        case AT_B:
        {
            eStress = STRESS_LOCATION_ENDING;
            break;
        }
        case AT_C:
        {
            if (NUM_SG == eNumber && PERSON_1 == ePerson)
            {
                eStress = STRESS_LOCATION_ENDING;
            }
            else
            {
                eStress = STRESS_LOCATION_STEM;
            }
            break;
        }
        case AT_C1:				// хотеть and derivatives
        {
            if (NUM_SG == eNumber && (PERSON_2 == ePerson || PERSON_3 == ePerson))
            {
                eStress = STRESS_LOCATION_STEM;
            }
            else
            {
                eStress = STRESS_LOCATION_ENDING;
            }
            break;
        }
        default:
        {
            ATLASSERT(0);
            CEString sMsg (L"Incompatible stress type.");
            ERROR_LOG (sMsg);
            throw CException (E_FAIL, sMsg);
        }
    }

    return hr;

}   //  h_GetStressType (...)

HRESULT CFormBuilderPersonal::hGetAuslautType (ET_Number eNumber, ET_Person ePerson, ET_StemAuslaut& eAuslaut)
{
    HRESULT hr = S_OK;

    if (m_pLexeme->m_iType >= 4 && m_pLexeme->m_iType <= 6)
    {
        if ((PERSON_1 == ePerson) && (NUM_SG == eNumber))
        {
            if (m_pLexeme->m_s1SgStem.bEndsWithOneOf (L"шжчщц"))
            {
                eAuslaut = STEM_AUSLAUT_SH;
            }
            else
            {
                eAuslaut = STEM_AUSLAUT_NOT_SH;
            }
        }
        if ((PERSON_3 == ePerson) && (NUM_PL == eNumber))
        {
            if (m_pLexeme->m_s3SgStem.bEndsWithOneOf (L"шжчщц"))
            {
                eAuslaut = STEM_AUSLAUT_SH;
            }
            else
            {
                eAuslaut = STEM_AUSLAUT_NOT_SH;
            }
        }
    }

    return S_OK;

}   //  hGetAuslautType (...)

HRESULT CFormBuilderPersonal::hGetLemma (ET_Number eNumber, ET_Person ePerson, CEString& sLemma)
{
    HRESULT hr = S_OK;

    if (PERSON_1 == ePerson && NUM_SG == eNumber)
    {
        sLemma = m_pLexeme->m_s1SgStem;
    }
    else
    {
        if (4 == m_pLexeme->m_iType || 5 == m_pLexeme->m_iType)
        {
            sLemma = m_pLexeme->m_s3SgStem;
        }
        else
        {
            if (PERSON_3 == ePerson && NUM_PL == eNumber)
            {
                sLemma = m_pLexeme->m_s1SgStem;
            }
            else
            {
                sLemma = m_pLexeme->m_s3SgStem;
            }
        }
    }

    return hr;

}       //  hGetLemma (...)

HRESULT CFormBuilderPersonal::hGetStressPositions (const CEString& sLemma,
                                                   const CEString& sEnding,
                                                   ET_StressLocation eStressType,
                                                   vector<int>& vecStressPositions)
{
    HRESULT hr = S_OK;

    if (STRESS_LOCATION_STEM == eStressType)
    {
        hGetStemStressPositions (const_cast<CEString&>(sLemma), vecStressPositions);
                                    // needed to set vowels
    }
    else if (STRESS_LOCATION_ENDING == eStressType)
    {
        int iStressPos = -1;
        hGetEndingStressPosition (const_cast<CEString&>(sLemma), const_cast<CEString&>(sEnding), iStressPos);
        vecStressPositions.push_back (iStressPos);
    }
    else
    {
        ATLASSERT(0);
        CEString sMsg (L"Illegal stress type.");
        ERROR_LOG (sMsg);
        throw CException (E_FAIL, sMsg);
    }

    return S_OK;

}   //  hGetStressPositions (...)

HRESULT CFormBuilderPersonal::hCreateFormTemplate (ET_Number eNumber, 
                                                   ET_Person ePerson,
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
        throw CException (E_INVALIDARG, (LPCTSTR)csMsg);
    }

    pWordForm->m_pLexeme = m_pLexeme;
    pWordForm->m_ePos = m_pLexeme->m_ePartOfSpeech;
    pWordForm->m_eSubparadigm = SUBPARADIGM_PRESENT_TENSE;
    pWordForm->m_eReflexive = m_pLexeme->m_eReflexive;
    pWordForm->m_eNumber = eNumber;
    pWordForm->m_ePerson = ePerson;
    pWordForm->m_eAspect = m_pLexeme->m_eAspect;

    AssignSecondaryStress (pWordForm);

    return hr;

}   //  hCreateFormTemplate (...)

HRESULT CFormBuilderPersonal::hBuild()
{
    HRESULT hr = S_OK;

    try
    {
		bool bNoRegularStems = false;

        hr = hBuildVerbStems();
		if (FAILED (hr))
		{
			return hr;
		}
		if (S_FALSE == hr)
		{
			bNoRegularStems = true;
		}

        GetEndings();

        int iType = m_pLexeme->m_iType;

        for (ET_Number eNumber = NUM_SG; eNumber <= NUM_PL; ++eNumber)
        {
            for (ET_Person ePerson = PERSON_1; ePerson <= PERSON_3; ++ePerson)
            {
                CComObject<CWordForm> * pWordForm = NULL;
                hCreateFormTemplate (eNumber, ePerson, pWordForm);
                bool bIsVariant = false;
                hr = hHandleIrregularForms (pWordForm, bIsVariant);
                if (S_OK == hr)
                {
                    if (!bIsVariant)
                    {
                        continue;
                    }
                }
                else
                {
                    if (FAILED (hr))
                    {
                        ATLASSERT (0);
                        CString csMsg;
                        csMsg.Format (L"h_HandleIrregularForms() failed, error %x.", hr);
                        ERROR_LOG ((LPCTSTR)csMsg);
                        continue;
                    }
                }
				if (bNoRegularStems)
				{
					continue;
				}

                ET_StressLocation eStress = STRESS_LOCATION_UNDEFINED;
                hGetStressType (eNumber, ePerson, eStress);
            
                ET_StemAuslaut eAuslaut = STEM_AUSLAUT_UNDEFINED;
                hGetAuslautType (eNumber, ePerson, eAuslaut);

                int iConjugation = INFLECTION_TYPE_UNDEFINED;
                if ((4 == iType || 5 == iType) || 
                    (PERSON_1 == ePerson && NUM_SG == eNumber) || 
                    (PERSON_3 == ePerson && NUM_PL == eNumber))
                {
                    iConjugation = iType;
                }

                StEndingDescriptor stD (iConjugation, ePerson, eNumber, eStress, eAuslaut);
                int iNumEndings = m_pEndings->iCount (stD);
                if (iNumEndings < 1)
                {
                    ERROR_LOG (L"No endings");
                    continue;
                }

                for (int iEnding = 0; iEnding < iNumEndings; ++iEnding)
                {
                    CEString sEnding;
                    m_pEndings->hGetEnding (stD, iEnding, sEnding);

                    if (6 == iType && 1 == m_pLexeme->m_iStemAugment)
                    {
                        if ((PERSON_1 == ePerson && NUM_SG == eNumber) ||
                            (PERSON_3 == ePerson && NUM_PL == eNumber))
                        {
                            if (sEnding.bIsEmpty() || L'ю' != sEnding[0])
                            {
                                ATLASSERT(0);
                                CString csMsg (L"Unexpected ending");
                                ERROR_LOG ((LPCTSTR)csMsg);
                                throw CException (E_INVALIDARG, (LPCTSTR)csMsg);
                            }
                            sEnding[0] = L'у';
                        }
                    }

                    if (REFL_YES == m_pLexeme->m_eReflexive)
                    {
                        if (sEnding.bEndsWithOneOf (g_szVowels))
                        {
                            sEnding += L"сь";
                        }
                        else
                        {
                            sEnding += L"ся";
                        }
                    }

                    CEString sLemma;
                    hGetLemma (eNumber, ePerson, sLemma);
                    hFleetingVowelCheck (sLemma);

                    pWordForm->m_sLemma = sLemma;
                    pWordForm->m_sWordForm = sLemma + sEnding;

                    vector<int> vecStress;
                    hGetStressPositions (sLemma, sEnding, eStress, vecStress);

                    if (1 == vecStress.size() || bMultiStress (sLemma, vecStress))
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

                }   //  for (int iEnding = 0; ... )

            }   //  for (ET_Person ePerson = PERSON_1; ... )

        }   //  for (ET_Number eNumber = NUM_SG; ... )

    }
    catch (CException ex)
    {
        return ex.iGetErrorCode();  // logging should be always done by callee
    }

    return hr;

}   //  hBuild()

//
// Needed when for abbreviated subparadigm rendering (GDRL 15, 16, 35, 89)
//
HRESULT CFormBuilderPersonal::hBuildIrregular()
{
    HRESULT hr = S_OK;

    try
    {
        for (ET_Number eNumber = NUM_SG; eNumber <= NUM_PL; ++eNumber)
        {
            for (ET_Person ePerson = PERSON_1; ePerson <= PERSON_3; ++ePerson)
            {
                CGramHasher hash (SUBPARADIGM_PRESENT_TENSE, 
                                  eNumber, 
                                  GENDER_UNDEFINED, 
                                  ePerson, 
                                  ANIM_UNDEFINED,                                       
                                  m_pLexeme->m_eAspect,
                                  CASE_UNDEFINED, 
                                  m_pLexeme->m_eReflexive);

                multimap<int, StIrregularForm>& mmapIf = m_pLexeme->m_mmapIrregularForms;    // alias
                multimap<int, StIrregularForm>::iterator itIf = mmapIf.find (hash.iGramHash());
                if (mmapIf.end() == itIf)
                {
                    if (NUM_SG == eNumber && (PERSON_1 == ePerson || PERSON_3 == ePerson))
                    {
                        ATLASSERT(0);
                        ERROR_LOG (L"Irregular form not in database.");
                        return E_UNEXPECTED;
                    }
                    hBuildIrregularForm (eNumber, ePerson, (*itIf).second);
                }

                AssignSecondaryStress ((*itIf).second);
                m_pLexeme->hAddIrregularForm (hash.iGramHash(), (*itIf).second);

            }   //  for (ET_Person ePerson = PERSON_1; ... )

        }   //  for (ET_Number eNumber = NUM_SG; ... )

    }
    catch (CException ex)
    {
        return ex.iGetErrorCode();  // logging should be always done by callee
    }

    return hr;

}   //  hBuildIrregular()

HRESULT CFormBuilderPersonal::hBuildIrregularForm (ET_Number eNumber, 
                                                   ET_Person ePerson, 
                                                   StIrregularForm& stIf)
{
    // GDRL p. 89

    HRESULT hr = S_OK;
    
    StIrregularForm st3sg;
    hr = hGet3SgIrregular (st3sg);
    if (S_OK != hr)
    {
        return hr;
    }

    CEString sIf (st3sg.sForm);
    if (!sIf.bEndsWith (L"т"))
    {
        ATLASSERT(0);
        ERROR_LOG (L"3 Sg form does not end in \'t\'.");
        return E_UNEXPECTED;
    }
    sIf.sErase (sIf.uiLength() - 1);

    stIf.mapStress = st3sg.mapStress;

    if (NUM_SG == eNumber && PERSON_2 == ePerson)
    {
        sIf += L"шь";
        return S_OK;
    }

    if (NUM_PL != eNumber)
    {
        ATLASSERT(0);
        ERROR_LOG (L"Unexpected ET_Number value.");
        return E_UNEXPECTED;
    }

    //  NUM_PL == eNumber

    switch (ePerson)
    {
        case PERSON_1:
        {
            sIf += L"м";
            break;
        }
        case PERSON_2:
        {
            sIf += L"те";
            break;
        }
        case PERSON_3:
        {
            if (sIf.bEndsWith (L"е") || sIf.bEndsWith (L"ё"))
            {
                StIrregularForm st1sg;
                hr = hGet3SgIrregular (st1sg);
                if (S_OK != hr)
                {
                    return hr;
                }
                sIf = st1sg.sForm;
                sIf += L"т";
            }
            else if (sIf.bEndsWith (L"и"))
            {
                sIf.sErase (sIf.uiLength()-1);
                if (sIf.bEndsWithOneOf (L"шжчщц"))
                {
                    sIf += L"ат";
                }
                else
                {
                    sIf += L"ят";
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

}       //  hBuildIrregularForm (...)
