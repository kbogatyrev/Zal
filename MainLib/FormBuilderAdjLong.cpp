#include "Endings.h"
#include "FormBuilderAdjLong.h"

using namespace Hlib;

/*
CEString CFormBuilderLongAdj::sGramHash (ET_PartOfSpeech ePos, ET_Subparadigm eSubparadigm,
                                         ET_Case eCase, ET_Number eNumber, ET_Gender eGender, 
                                         ET_Animacy eAnimacy, ET_Reflexive eRefl)
{
    CGramHasher hasher (ePos, eSubparadigm, eCase, eNumber, eGender, PERSON_UNDEFINED, eAnimacy, 
                        ASPECT_UNDEFINED, eRefl); 

    CEString sHash = hasher.sGramHash();

    return sHash;
}

CEString CFormBuilderLongAdj::sGramHashNSgMLong()
{
    CGramHasher hasher (POS_ADJ, SUBPARADIGM_LONG_ADJ, CASE_UNDEFINED, NUM_UNDEFINED, GENDER_UNDEFINED, PERSON_UNDEFINED,                             
                        ANIM_NO, ASPECT_UNDEFINED, REFL_UNDEFINED);

    CEString sHash = hasher.sGramHash();

    return sHash;
}
*/

ET_ReturnCode CFormBuilderLongAdj::eGetStressPositions (const CEString& sEnding,
                                                        ET_StressLocation eStressType,
                                                        vector<int>& vecStressPos)
{
    ET_ReturnCode rc = H_NO_ERROR;

    m_sStem.SetVowels (g_szRusVowels);

    int iStressPos = -1;

    if (STRESS_LOCATION_STEM == eStressType)
    {
        rc = eGetStemStressPositions (m_sStem, m_eSubparadigm, NUM_UNDEFINED, vecStressPos);
                                                                // Number not needed for this subparadigm
    }
    else if (STRESS_LOCATION_ENDING == eStressType)
    {
        if (sEnding.uiNSyllables() < 1)
        {
            iStressPos = m_sStem.uiNSyllables() - 1;
        }
        else
        {
            iStressPos = m_sStem.uiNSyllables();
        }

        vecStressPos.push_back (iStressPos);
    }
    else
    {
        ASSERT (0);
        ERROR_LOG (L"Illegal stress type.");
        return H_ERROR_INVALID_ARG;
    }

    return rc;

}   //  eGetStressPositions (...)

ET_ReturnCode CFormBuilderLongAdj::eCreateFormTemplate (ET_Gender eGender,
                                                        ET_Number eNumber, 
                                                        ET_Case eCase, 
                                                        ET_Animacy eAnimacy,
                                                        const CEString& sEnding,
                                                        __int64 llEndingKey,
                                                        CWordForm *& pWordForm)
{
    if (m_sStem.bIsEmpty())
    {
        ERROR_LOG (L"No stem"); 
        return H_ERROR_UNEXPECTED;
    }

    pWordForm = new CWordForm;
    if (NULL == pWordForm)
    {
        ASSERT(0);
        ERROR_LOG (L"Error creating CWordForm object");
        return H_ERROR_POINTER;
    }

    pWordForm->m_pLexeme = m_pLexeme;
    pWordForm->m_ePos = m_pLexeme->ePartOfSpeech();
    pWordForm->m_eSubparadigm = m_eSubparadigm;
    pWordForm->m_sStem = m_sStem;
    pWordForm->m_sEnding = sEnding;
    pWordForm->m_llEndingDataId = llEndingKey;
    pWordForm->m_eGender = eGender;
    pWordForm->m_eCase = eCase;
    pWordForm->m_eNumber = eNumber;
    pWordForm->m_eAnimacy = eAnimacy;
    pWordForm->m_eReflexivity = m_pLexeme->eIsReflexive();
    pWordForm->m_sWordForm = m_sStem + sEnding;
    pWordForm->m_eStatus = m_eStatus;
    pWordForm->m_llLexemeId = m_pLexeme->llLexemeId();

    return H_NO_ERROR;

}   //  eCreateFormTemplate (...)

ET_ReturnCode CFormBuilderLongAdj::eHandleCommonDeviations (CWordForm * pWordForm)
{
    ET_ReturnCode rc = H_NO_ERROR;

    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    try
    {
        if (m_pLexeme->bHasCommonDeviation(4) && SUBPARADIGM_PART_PRES_ACT == m_eSubparadigm)
        {
            if (m_pLexeme->bDeviationOptional(4))
            {
                CWordForm * pVariant = NULL;
                CloneWordForm (pWordForm, pVariant);
                m_pLexeme->AddWordForm (pVariant);    // store both versions
                pWordForm = pVariant;
            }

            map<int, ET_StressType> mapCorrectedStress;
            map<int, ET_StressType>::iterator itStressPos = pWordForm->m_mapStress.begin();
            for (; itStressPos != pWordForm->m_mapStress.end(); ++itStressPos)
            {
                if (STRESS_SECONDARY == (*itStressPos).second)
                {
                    mapCorrectedStress[(*itStressPos).first] = STRESS_SECONDARY;
                    continue;
                }
                if ((*itStressPos).first < 1)
                {
                    ASSERT(0);
                    ERROR_LOG (L"Unexpected stress position in cd-4 participle.");
                    return H_ERROR_UNEXPECTED;
                }
                mapCorrectedStress[(*itStressPos).first-1] = STRESS_PRIMARY;
            }
            pWordForm->m_mapStress = mapCorrectedStress;        

        }   //  if (bHasCommonDeviation (4) ...

        if (m_pLexeme->bHasCommonDeviation(6) && 
            SUBPARADIGM_PART_PAST_ACT == m_eSubparadigm &&
            3 == m_pLexeme->iType() &&
            1 == m_pLexeme->iStemAugment())
        {
            if (m_pLexeme->bDeviationOptional(6))
            {
                CWordForm * pVariant = NULL;
                CloneWordForm (pWordForm, pVariant);
                m_pLexeme->AddWordForm (pVariant);    // store both versions
                pWordForm = pVariant;
            }

            CEString sEnding = pWordForm->m_sWordForm.sSubstr (pWordForm->m_sStem.uiLength());
            CEString sStem = pWordForm->m_sStem;
            int iLength = -1;
            if (sStem.bEndsWith (L"вш"))
            {
                iLength = sStem.uiLength()-2;
            }
            else
            {
                iLength = sStem.uiLength()-1;
            }

            sStem = sStem.sSubstr (0, iLength);

            if (sStem.bEndsWith (L"ну"))
            {
                if (m_pLexeme->bDeviationOptional(6))
                {
                    // For optional (6) we store both variants
                    sStem = sStem.sSubstr (0, iLength-2);
                    sStem += sStem.bEndsWithOneOf (g_szRusConsonants) ? L"ш" : L"вш";
                }
                else
                {
                    // Stem already has -nu-; nothing to do (note that we cannot
                    // predict which form was used for derivation)
//                    return H_NO_MORE;
                    return H_NO_ERROR;
                }
            }
            else
            {
                sStem += L"нувш";
            }
            pWordForm->m_sWordForm = sStem + sEnding;

        }   //  if (bHasCommonDeviation (6) ... )

        int iCd = -1;
        if (m_pLexeme->bHasCommonDeviation(7))
        {
            iCd = 7;
        }
        else if (m_pLexeme->bHasCommonDeviation(8))
        {
            iCd = 8;
        }

        if (iCd > 0 && SUBPARADIGM_PART_PAST_PASS_LONG == m_eSubparadigm)
        {
            CEString sStem(pWordForm->m_sStem);
            int iNSyll = sStem.uiNSyllables();
            if (iNSyll < 1)        //  < 2??
            {
                ASSERT(0);
                CEString sMsg (L"No syllables in part pass past stem.");
                ERROR_LOG (sMsg);
                return H_ERROR_UNEXPECTED;
            }

            if (m_pLexeme->bDeviationOptional(iCd))
            {
                CWordForm * pVariant = NULL;
                CloneWordForm (pWordForm, pVariant);
                m_pLexeme->AddWordForm (pVariant);    // store both versions
                pWordForm = pVariant;
            }

            map<int, ET_StressType> mapCorrectedStress;
            map<int, ET_StressType>::iterator itStressPos = pWordForm->m_mapStress.begin();
            int iNewStressPos = -1;
            for (; itStressPos != pWordForm->m_mapStress.end(); ++itStressPos)
            {
                if (STRESS_SECONDARY == (*itStressPos).second)
                {
                    mapCorrectedStress[(*itStressPos).first] = STRESS_SECONDARY;
                    continue;
                }
                if (8 == iCd && (*itStressPos).first < 1)
                {
                    ASSERT(0);
                    ERROR_LOG (L"Unexpected stress position in cd-7 or cd-8 participle.");
                    return H_ERROR_UNEXPECTED;
                }

                if (7 == iCd)
                {
                    iNewStressPos = (*itStressPos).first + 1;
                    iNewStressPos = sStem.uiNSyllables() - 1;
                }
                else
                {
                    if (8 == iCd)
                    {
                        iNewStressPos = pWordForm->m_sStem.uiNSyllables() - 2;
                    }
                    else
                    {
                        ASSERT(0);
                        ERROR_LOG (L"Unexpected cd number.");
                        return H_ERROR_UNEXPECTED;
                    }
                }

                mapCorrectedStress[iNewStressPos] = STRESS_PRIMARY;

                // Remove yo at old stress pos
                int iAt = sStem.uiGetVowelPos ((*itStressPos).first);
                if (L'ё' == pWordForm->m_sWordForm[iAt])
                {
                    pWordForm->m_sStem[iAt] = pWordForm->m_sWordForm[iAt] = L'е';
                }

                // E --> yo at new stress pos
                iAt = sStem.uiGetVowelPos (iNewStressPos);
                if (L'е' == pWordForm->m_sWordForm[iAt])
                {
                    pWordForm->m_sStem[iAt] = pWordForm->m_sWordForm[iAt] = L'ё';
                }
            }
            pWordForm->m_mapStress = mapCorrectedStress;
        }
    }
    catch (CException& ex)
    {
        CEString sMsg (L"Exception: ");
        sMsg += ex.szGetDescription();
        ERROR_LOG (sMsg);
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}   //  eHandleCommonDeviations (...)

ET_ReturnCode CFormBuilderLongAdj::eBuild()
{
    ET_ReturnCode rc = H_NO_ERROR;

    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    if (m_pLexeme->bNoLongForms())
    {
        return H_NO_MORE;
    }

    try
    {
        m_pEndings = new CAdjLongEndings(m_pLexeme, m_eSubparadigm);
        if (NULL == m_pEndings)
        {
            return H_ERROR_POINTER;
        }

        CGramHasher gram_tmp;
//        gram_tmp.Initialize(POS_ADJ);
        gram_tmp.Initialize(m_pLexeme->ePartOfSpeech());
        ET_Subparadigm eSubParadigm = SUBPARADIGM_LONG_ADJ;
        if (m_pLexeme->ePartOfSpeech() == POS_NOUN)
        {
            gram_tmp.Initialize(m_pLexeme->eMainSymbolToGender(), m_pLexeme->eMainSymbolToAnimacy());
            eSubParadigm = SUBPARADIGM_NOUN;
        }
        else
        {
            gram_tmp.Initialize(POS_ADJ);
            eSubParadigm = SUBPARADIGM_LONG_ADJ;
        }
        gram_tmp.SetParadigm(eSubParadigm);

        bool bPluralNoun = false;
        if (m_pLexeme->sInflectionType() == L"мн." || m_pLexeme->sInflectionType() == L"мн. неод." ||
            m_pLexeme->sInflectionType() == L"мн. одуш." || m_pLexeme->sInflectionType() == L"мн. от")
        {
            bPluralNoun = true;
        }

        do
        {
            ET_StressLocation eStressType = STRESS_LOCATION_UNDEFINED;
            if (AT_A == m_eAccentType|| AT_A1 == m_eAccentType)
            {
                eStressType = STRESS_LOCATION_STEM;
            }
            else
            {
                if (AT_B == m_eAccentType)
                {
                    eStressType = STRESS_LOCATION_ENDING;
                }
                else
                {
                    ERROR_LOG (L"Unknown stress type.");
                    continue;
                }
            }

            ET_Animacy eAnimacy = ANIM_UNDEFINED;
            if (CASE_ACC == gram_tmp.m_eCase)
            {
                if ((GENDER_M == gram_tmp.m_eGender && NUM_SG == gram_tmp.m_eNumber) || (NUM_PL == gram_tmp.m_eNumber))
                {
                    eAnimacy = m_pLexeme->ePartOfSpeech() == POS_ADJ ? gram_tmp.m_eAnimacy : m_pLexeme->eMainSymbolToAnimacy();
                }
            }

            ET_Gender eGender = GENDER_UNDEFINED;
            if (m_pLexeme->ePartOfSpeech() == POS_ADJ)
            {
                eGender = gram_tmp.m_eGender;
            }
            else if (m_pLexeme->ePartOfSpeech() == POS_NOUN && NUM_SG == gram_tmp.m_eNumber)
            {
                eGender = m_pLexeme->eMainSymbolToGender();
            }

            rc = ((CAdjLongEndings *)m_pEndings)->eSelect(SUBPARADIGM_LONG_ADJ, eGender, gram_tmp.m_eNumber, gram_tmp.m_eCase, eAnimacy);
            int iNumEndings = m_pEndings->iCount();
            if (iNumEndings < 1)
            {
                ERROR_LOG (L"No endings");
                continue;
            }

            for (int iEnding = 0; iEnding < iNumEndings; ++iEnding)
            {
                CEString sEnding;
                int64_t llEndingKey;
                m_pEndings->eGetEnding (iEnding, sEnding, llEndingKey);

                CWordForm * pWordForm = NULL;
                rc = eCreateFormTemplate (gram_tmp.m_eGender, 
                                          gram_tmp.m_eNumber, 
                                          gram_tmp.m_eCase, 
                                          gram_tmp.m_eAnimacy, 
                                          sEnding, 
                                          llEndingKey,
                                          pWordForm);
                if (rc != H_NO_ERROR)
                {
                    return rc;
                }

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

            }   //  for (int iEnding = 0; iEnding < iNumEndings; ++iEnding)
        } while (gram_tmp.bIncrement());
    }
    catch (CException ex)
    {
        return H_EXCEPTION;  // logging should be always done by callee
    }

    return H_NO_ERROR;

}   //  eBuild (...)

ET_ReturnCode CFormBuilderLongAdj::eBuildParticiple()
{
    ET_ReturnCode rc = H_NO_ERROR;

    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    if (-1 == m_iStemStressPos)
    {
        ASSERT(0);
        ERROR_LOG (L"No stress position for a participle.");
        return H_ERROR_GENERAL;
    }

    try
    {
        m_pEndings = new CAdjLongEndings(m_pLexeme, m_eSubparadigm);
        if (NULL == m_pEndings)
        {
            return H_ERROR_POINTER;
        }

        CGramHasher gramHash;
        gramHash.Initialize(POS_VERB);
        gramHash.m_eAspect = m_pLexeme->eAspect();
        gramHash.SetParadigm (m_eSubparadigm);
        bool bInitialForm = true;
        do
        {
            ((CAdjLongEndings *)m_pEndings)->eSelect(m_eSubparadigm, gramHash.m_eGender, gramHash.m_eNumber, gramHash.m_eCase, gramHash.m_eAnimacy);
            int iNumEndings = m_pEndings->iCount();
            if (iNumEndings < 1)
            {
                ERROR_LOG (L"No endings");
                continue;
            }

            for (int iEnding = 0; iEnding < iNumEndings; ++iEnding)
            {
                CEString sEnding;
                int64_t llEndingKey = -1;
                rc = m_pEndings->eGetEnding(iEnding, sEnding, llEndingKey);
                if (rc != H_NO_ERROR)
                {
                    return rc;
                }

                if (!m_bIrregular)
                {
                    rc = eHandleYoAlternation (STRESS_LOCATION_STEM, m_iStemStressPos, m_sStem, sEnding);
                    if (rc != H_NO_ERROR)
                    {
                        return rc;
                    }
                }

//                CWordForm * pStressTemplate = NULL;
                CWordForm * pWordForm = NULL;
                rc = eCreateFormTemplate (gramHash.m_eGender,
                                          gramHash.m_eNumber,
                                          gramHash.m_eCase,
                                          gramHash.m_eAnimacy,
                                          sEnding, 
                                          llEndingKey,
                                          pWordForm);
                if (rc != H_NO_ERROR)
                {
                    return rc;
                }

                pWordForm->m_eAspect = gramHash.m_eAspect;
                pWordForm->m_mapStress[m_iStemStressPos] = STRESS_PRIMARY;
                if (!m_bIrregular)
                {
                    rc = eHandleCommonDeviations (pWordForm);
                    if (rc != H_NO_ERROR)
                    {
                        return rc;
                    }
                }

                if (m_bIrregular && bInitialForm)
                {
                    pWordForm->m_bIrregular = true;
                }

                bInitialForm = false;

                m_pLexeme->AddWordForm (pWordForm);

            }   //  for (int iEnding = 0; iEnding < iNumEndings; ++iEnding)

        } while (gramHash.bIncrement());
    }
    catch (CException ex)
    {
        return H_EXCEPTION;  // logging should be always done by callee
    }

    return H_NO_ERROR;

}   //  eBuildParticiple (...)
