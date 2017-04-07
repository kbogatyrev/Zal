#include "StdAfx.h"

#include "Endings.h"
#include "WordForm.h"
#include "Lexeme.h"
#include "FormBuilderNouns.h"

using namespace Hlib;

CEString CFormBuilderNouns::sGramHash (ET_Gender eoGender, ET_Animacy eAnimacy, ET_Case eCase, ET_Number eNumber)
{
    CGramHasher hasher (eoGender, eAnimacy, eCase, eNumber);
    CEString sHash = hasher.sGramHash();
    return sHash;
}

ET_ReturnCode CFormBuilderNouns::eHandleStemAugment (CEString& sLemma, ET_Number eNumber, ET_Case eCase)
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    if (m_pLexeme->iStemAugment() < 1)
    {
        return H_NO_ERROR;
    }

    if (1 == m_pLexeme->iType())
    {
        sLemma.sErase (sLemma.uiLength()-2, 2);  // римлянин, южанин, армянин
    }
    if (3 == m_pLexeme->iType())
    {
        CEString& sGs = m_pLexeme->sGraphicStem();
        if (NUM_SG == eNumber)
        {
            if ((CASE_NOM == eCase) || 
                (ANIM_NO == m_pLexeme->eInflectionTypeToAnimacy() && CASE_ACC == eCase))
            {
                return H_NO_ERROR;
            }
            else
            {
                sLemma.sErase (sLemma.uiLength()-2, 1);
                return H_NO_ERROR;
            }
        }
        if (NUM_PL == eNumber)
        {
            if (sGs.bEndsWith (L"онок"))
            {
                sLemma.sErase (sLemma.uiLength()-4, 4);
                sLemma += L"ат";
                return H_NO_ERROR;
            }
            if (sGs.bEndsWith (L"ёнок"))
            {
                sLemma.sErase (sLemma.uiLength()-4, 4);
                sLemma += L"ят";
                return H_NO_ERROR;
            }
            if (sGs.bEndsWith (L"оночек"))
            {
                sLemma.sErase (sLemma.uiLength()-6, 6);
                if ((CASE_GEN == eCase) ||                        
                    (CASE_ACC == eCase && ANIM_YES == m_pLexeme->eInflectionTypeToAnimacy()))
                                                     // they all should be animate?
                {
                    sLemma += L"аток";
                }
                else
                {
                    sLemma += L"атк";
                }
                return H_NO_ERROR;
            }
            if (sGs.bEndsWith (L"ёночек"))
            {
                sLemma.sErase (sLemma.uiLength()-6, 6);
                if ((CASE_GEN == eCase) ||                        
                    (CASE_ACC == eCase && ANIM_YES == m_pLexeme->eInflectionTypeToAnimacy()))
                                                     // they all should be animate?
                {
                    sLemma += L"яток";
                }
                else
                {
                    sLemma += L"ятк";
                }
                return H_NO_ERROR;
            }
        }
    }       //  if (3 == m_pLexeme->i_Type)
    if (8 == m_pLexeme->iType())
    {
        if (NUM_SG == eNumber)
        {
            if ((CASE_NOM == eCase) || 
                (ANIM_NO == m_pLexeme->eInflectionTypeToAnimacy() && CASE_ACC == eCase))
            {
                return H_NO_ERROR;
            }
            else
            {
                sLemma += L"ен";
            }
        }
        else
        {
            sLemma += L"ен";
        }
    }

    return H_NO_ERROR;

}   //  eHandleStemAugment (...)

ET_ReturnCode CFormBuilderNouns::eGetStressType (ET_Number eNumber, ET_Case eCase, ET_StressLocation& eStressType)
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    if (NUM_UNDEFINED == eNumber)
    {
        ASSERT(0);
        ERROR_LOG (L"Undefined number.");
        return H_ERROR_INVALID_ARG;
    }

    if (CASE_LOC == eCase)
    {
        eStressType = STRESS_LOCATION_ENDING;
        return H_NO_ERROR;
    }

    eStressType = STRESS_LOCATION_STEM;

    if (CASE_PART == eCase)
    {
        eCase = CASE_DAT;
    }

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
            if (GENDER_F != m_pLexeme->eInflectionTypeToGender())
            {
                ASSERT(0);
                CEString sMsg (L"Non-feminine noun with D1 stress.");
                ERROR_LOG (sMsg);
                throw CException (H_ERROR_GENERAL, sMsg);
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
            else if (CASE_ACC == eCase && ANIM_NO == m_pLexeme->eInflectionTypeToAnimacy())
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
            else if (CASE_ACC == eCase && ANIM_NO == m_pLexeme->eInflectionTypeToAnimacy())
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
            if (GENDER_F != m_pLexeme->eInflectionTypeToGender())
            {
                ASSERT(0);
                CEString sMsg (L"Non-feminine noun with F1 stress.");
                ERROR_LOG (sMsg);
                throw CException (H_ERROR_GENERAL, sMsg);
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
            if (GENDER_F != m_pLexeme->eInflectionTypeToGender() || 8 != m_pLexeme->iType())
            {
                ASSERT(0);   // assume f -i stems only?
                CEString sMsg (L"Non-feminine/type 8 noun with F2 stress.");
                ERROR_LOG (sMsg);
                throw CException (E_FAIL, sMsg);
            }
            if (ANIM_YES == m_pLexeme->eInflectionTypeToAnimacy())
            {
                ASSERT(0);                   // inanimate only?
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
        case AT_UNDEFINED:
        {
            if (m_pLexeme->iType() != 0)
            {
                ASSERT(0);
                CEString sMsg(L"Illegal accent type.");
                ERROR_LOG(L"Illegal accent type.");
                return H_ERROR_GENERAL;
            }
            break;
        }
        default:
        {
            ASSERT(0);
            CEString sMsg (L"Illegal accent type.");
            ERROR_LOG (L"Illegal accent type.");
            return H_ERROR_GENERAL;
        }
    }

    return H_NO_ERROR;

}   // eGetStressType()

ET_ReturnCode CFormBuilderNouns::eHandleAccEnding (ET_Number eNumber, ET_Case& eCase)
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    if (NUM_SG == eNumber)
    {
        if (GENDER_M == m_pLexeme->eInflectionTypeToGender())
        {
            ANIM_YES == m_pLexeme->eInflectionTypeToAnimacy()
                ? eCase = CASE_GEN
                : eCase = CASE_NOM;
        }
        if (GENDER_N == m_pLexeme->eInflectionTypeToGender())
        {
            eCase = CASE_NOM;
        }
    }
    if (NUM_PL == eNumber)
    {
        ANIM_YES == m_pLexeme->eInflectionTypeToAnimacy()
            ? eCase = CASE_GEN
            : eCase = CASE_NOM;
    }

    return H_NO_ERROR;

}   //  eHandleAccEnding

ET_ReturnCode CFormBuilderNouns::eGetStressPositions (const CEString& sLemma, 
                                                      const CEString& sEnding,
                                                      ET_Number eNumber,
                                                      ET_StressLocation eStressType,
                                                      vector<int>& vecStressPos)
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    int iStressPos = -1;
    if (STRESS_LOCATION_STEM == eStressType)
    {
        rc = eGetStemStressPositions (sLemma, SUBPARADIGM_NOUN, eNumber, vecStressPos);
        if (rc != H_NO_ERROR)
        {
            return rc;
        }
    }
    else if (STRESS_LOCATION_ENDING == eStressType)
    {
        ET_ReturnCode rc = eGetEndingStressPosition (sLemma, sEnding, iStressPos);
        if (rc != H_NO_ERROR)
        {
            ASSERT(0);
            ERROR_LOG (L"Bad stress type.");
            return H_ERROR_GENERAL;
        }
        vecStressPos.push_back (iStressPos);
    }
    else
    {
        ASSERT(0);
        ERROR_LOG (L"Bad stress type.");
        return H_ERROR_GENERAL;
    }

    if (vecStressPos.empty())
    {
        vecStressPos.push_back (-1);
    }

    return H_NO_ERROR;

}   //  eGetStressPositions (...)

ET_ReturnCode CFormBuilderNouns::eCreateFormTemplate (ET_Number eNumber, 
                                                      ET_Case eCase, 
                                                      const CEString& sLemma, 
                                                      CWordForm *& pWordForm)
{
    ET_ReturnCode rc = H_NO_ERROR;

    pWordForm = new CWordForm;
    if (NULL == pWordForm)
    {
        ASSERT(0);
        ERROR_LOG (L"Unable to instantiate CWordForm.");
        return H_ERROR_POINTER;
    }

    pWordForm->m_pLexeme = m_pLexeme;
    pWordForm->m_ePos = m_pLexeme->ePartOfSpeech();
    pWordForm->m_eSubparadigm = SUBPARADIGM_NOUN;
    pWordForm->m_eCase = eCase;
    pWordForm->m_sLemma = sLemma;
    pWordForm->m_eNumber = eNumber;
    pWordForm->m_eGender = m_pLexeme->eInflectionTypeToGender();
    pWordForm->m_eAnimacy = m_pLexeme->eInflectionTypeToAnimacy();
    if (NUM_PL == eNumber && m_pLexeme->bAssumedForms())
    {
        pWordForm->m_eStatus = STATUS_ASSUMED;
    }
    //rc = eAssignSecondaryStress (pWordForm);
    //if (rc != H_NO_ERROR)
    //{
    //    return rc;
    //}

    return H_NO_ERROR;

}   //  eCreateFormTemplate (...)

ET_ReturnCode CFormBuilderNouns::eCheckIrregularForms (ET_Gender eoGender, 
                                                       ET_Animacy eAnimacy, 
                                                       ET_Case eCase, 
                                                       ET_Case eEndingsCase,
                                                       ET_Number eNumber, 
                                                       bool& bHandled)
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    if (!m_pLexeme->bHasIrregularForms())
    {
        return H_FALSE;
    }

    bHandled = false;

    CEString sHash = sGramHash (eoGender, eAnimacy, eEndingsCase, eNumber);

    map<CWordForm *, bool> mapIrreg;
    rc = m_pLexeme->eGetIrregularForms(sHash, mapIrreg);
    if (rc != H_NO_ERROR)
    {
        return rc;
    }

    map<CWordForm *, bool>::iterator it = mapIrreg.begin();
    for (; it != mapIrreg.end(); ++it)
    {
        if ((*it).second)   // optional?
        {
            bHandled = false;
        }
        else
        {
            bHandled = true;
        }
        
        CWordForm * pWordForm = new CWordForm((*it).first);
        pWordForm->m_eCase = eCase;   // ending case may differ from actual case, e.g. A.Sg.
        m_pLexeme->AddWordForm(pWordForm);
    }

    return H_NO_ERROR;

}   //  eCheckIrregularForms (...)

ET_ReturnCode CFormBuilderNouns::eBuild()
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    m_pEndings = new CNounEndings(m_pLexeme);
    if (NULL == m_pEndings)
    {
        return H_ERROR_POINTER;
    }

    if (rc != H_NO_ERROR)
    {
        return rc;
    }

    ET_Animacy eAnimacy = m_pLexeme->eInflectionTypeToAnimacy();
    ET_Gender eoGender = m_pLexeme->eInflectionTypeToGender();

    CHasher gramIterator;
    gramIterator.Initialize(eoGender, eAnimacy);
    do
    {
        if ((L"мн." == m_pLexeme->sMainSymbol()) && (gramIterator.m_eNumber == NUM_SG))
        {
            continue;
        }

        if (NUM_PL == gramIterator.m_eNumber && 
            (CASE_PART == gramIterator.m_eCase || CASE_LOC == gramIterator.m_eCase))
        {
            continue;
        }

        if (CASE_PART == gramIterator.m_eCase && !m_pLexeme->bSecondGenitive())
        {
            continue;
        }

        if (CASE_LOC == gramIterator.m_eCase && !m_pLexeme->bSecondLocative())
        {
            continue;
        }

        // Handle acc ending
        ET_Case eEndingCase = gramIterator.m_eCase;
        if (CASE_ACC == gramIterator.m_eCase)
        {
            rc = eHandleAccEnding (gramIterator.m_eNumber, eEndingCase);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }
        }
        if (CASE_PART == gramIterator.m_eCase || CASE_LOC == gramIterator.m_eCase)
        {
            eEndingCase = CASE_DAT;
        }

        CEString sLemma (m_pLexeme->sGraphicStem());
        if (m_pLexeme->bHasIrregularForms())
        {
            bool bSkipRegular = false;
            rc = eCheckIrregularForms (gramIterator.m_eGender, 
                                       gramIterator.m_eAnimacy,
                                       gramIterator.m_eCase,
                                       eEndingCase,
                                       gramIterator.m_eNumber,
                                       bSkipRegular);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }
            if (bSkipRegular)
            {
                // Workaround for lack of "исх. форма иррег." mark in current source
                if (GENDER_M == gramIterator.m_eGender && NUM_SG == gramIterator.m_eNumber && CASE_NOM == gramIterator.m_eCase)
                {
                    m_bIrregularSourceForm = true;
                }
                continue;
            }
        }

        rc = eHandleStemAugment (sLemma, gramIterator.m_eNumber, gramIterator.m_eCase);
        if (rc != H_NO_ERROR)
        {
            return rc;
        }

        ET_StressLocation eStress = STRESS_LOCATION_UNDEFINED;
        if (CASE_LOC == gramIterator.m_eCase)
        {
            eStress = STRESS_LOCATION_ENDING;
        }
        else
        {
            rc = eGetStressType (gramIterator.m_eNumber, eEndingCase, eStress);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }
        }

        ((CNounEndings *)m_pEndings)->eSelect(gramIterator.m_eNumber, eEndingCase, eStress);
        int64_t iNumEndings = m_pEndings->iCount();
        if (iNumEndings < 1)
        {
            if (m_pLexeme->iType() != 0)
            {
                ASSERT(0);
                ERROR_LOG(L"No endings");
            }
            continue;
        }

        CEString sSavedLemma (sLemma);      // lemma can change, e.g. because of a fleetimg vowel
        for (int iEnding = 0; iEnding < iNumEndings; ++iEnding, sLemma = sSavedLemma)
        {
            // Get ending and modify as necessary
            CEString sEnding;
            int64_t llEndingKey = -1;
            rc = m_pEndings->eGetEnding(iEnding, sEnding, llEndingKey);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }

            if (8 == m_pLexeme->iType() && GENDER_N != m_pLexeme->eInflectionTypeToGender())
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
            rc = eFleetingVowelCheck (gramIterator.m_eNumber, 
                                      eEndingCase,
                                      gramIterator.m_eGender, 
                                      eStress,
                                      SUBPARADIGM_NOUN,
                                      sEnding,
                                      sLemma);
            if (rc != H_NO_ERROR)
            {
                continue;
            }

            vector<int> vecStress;
            rc = eGetStressPositions (sLemma, sEnding, gramIterator.m_eNumber, eStress, vecStress);
            if (rc != H_NO_ERROR)
            {
                continue;
            }

            CWordForm * pWordForm = NULL;
            rc = eCreateFormTemplate (gramIterator.m_eNumber, gramIterator.m_eCase, sLemma, pWordForm);
            if (rc != H_NO_ERROR)
            {
                continue;
            }

            if (1 == vecStress.size() || m_pLexeme->bIsMultistressedCompound())
            {
                vector<int>::iterator itStressPos = vecStress.begin();
                for (; itStressPos != vecStress.end(); ++itStressPos)
                {
                    pWordForm->m_mapStress[*itStressPos] = STRESS_PRIMARY;  // primary
                    rc = eHandleYoAlternation (eStress, *itStressPos, pWordForm->m_sLemma, sEnding);
                    if (rc != H_NO_ERROR)
                    {
                        continue;
                    }
                    pWordForm->m_sEnding = sEnding;
                    pWordForm->m_llEndingDataId = llEndingKey;
                    pWordForm->m_sWordForm = pWordForm->m_sLemma + sEnding;
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
                        CWordForm * pwfVariant = NULL;
                        CloneWordForm (pWordForm, pwfVariant);
                        pwfVariant->m_mapStress.clear();
                        pWordForm = pwfVariant;
                    }
                    pWordForm->m_mapStress[*itStressPos] = STRESS_PRIMARY;  // primary
                    rc = eHandleYoAlternation (eStress, *itStressPos, pWordForm->m_sLemma, sEnding);
                    if (rc != H_NO_ERROR)
                    {
                        continue;
                    }
                    pWordForm->m_sWordForm = pWordForm->m_sLemma + sEnding;
                    pWordForm->m_sEnding = sEnding;
                    pWordForm->m_llEndingDataId = llEndingKey;

                    m_pLexeme->AddWordForm (pWordForm);
                }
            }
        }   //  for (int iEnding = 0; ... )
    
    } while (gramIterator.bIncrement());

    return H_NO_ERROR;

}    //  eBuildNounForms()
