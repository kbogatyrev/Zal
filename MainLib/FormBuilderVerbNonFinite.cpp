#include "StdAfx.h"
#include "Endings.h"

using namespace Hlib;

#include "WordForm.h"
#include "Lexeme.h"

#include "FormBuilderAdjLong.h"
#include "FormBuilderAdjShort.h"
#include "FormBuilderVerbNonFinite.h"

ET_ReturnCode CFormBuilderNonFinite::eBuild()
{
    ET_ReturnCode rc = H_NO_ERROR;

    rc = eBuildInfinitive();
    if (rc != H_NO_ERROR)
    {
//        return rc;
    }

    rc = eBuildPresentActiveParticiple();
    if (rc != H_NO_ERROR)
    {
//        return rc;
    }

    rc = eBuildPresentAdverbial();
    if (rc != H_NO_ERROR)
    {
//        return rc;
    }

    rc = eBuildPastActiveParticiple();
    if (rc != H_NO_ERROR)
    {
//        return rc;
    }

    rc = eBuildPastAdverbial();
    if (rc != H_NO_ERROR)
    {
//        return rc;
    }

    rc = eBuildPresentPassiveParticiple();
    if (rc != H_NO_ERROR)
    {
//        return rc;
    }

    rc = eBuildPastPassiveParticiple();
    
    return rc;
}

ET_ReturnCode CFormBuilderNonFinite::eBuildInfinitive()
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    try
    {
        m_pEndings = new CInfinitiveEndings(m_pLexeme);
        if (rc != H_NO_ERROR)
        {
            return rc;
        }

        ((CInfinitiveEndings *)m_pEndings)->eSelect(m_pLexeme->iType());
        int64_t iNumEndings = m_pEndings->iCount();
        if (iNumEndings < 1)
        {
            ERROR_LOG(L"No ending or too many endings");
            return H_ERROR_UNEXPECTED;
        }

        vector <CWordForm *> vecWordForms;
        for (int iEnding = 0; (rc == H_NO_ERROR && iEnding < iNumEndings); ++iEnding)
        {
            CEString sEnding;
            int64_t llEndingKey = -1;
            rc = m_pEndings->eGetEnding(iEnding, sEnding, llEndingKey);
            if (rc != H_NO_ERROR)
            {
                continue;
            }

            CWordForm * pWordForm = new CWordForm;
            if (NULL == pWordForm)
            {
                ASSERT(0);
                ERROR_LOG(L"Unable to instantiate CWordForm.");
                return H_ERROR_POINTER;
            }

            pWordForm->m_pLexeme = m_pLexeme;
            pWordForm->m_ePos = POS_VERB;
            pWordForm->m_eSubparadigm = SUBPARADIGM_INFINITIVE;
            pWordForm->m_eAspect = m_pLexeme->eAspect();
            pWordForm->m_eReflexive = m_pLexeme->eIsReflexive();
            pWordForm->m_sStem = m_pLexeme->sInfStem();
            pWordForm->m_sWordForm = m_pLexeme->sSourceForm();
            pWordForm->m_llEndingDataId = llEndingKey;

            vector<int> vecStress;

            const StLexemeProperties& stLexemeProperties = m_pLexeme->stGetProperties();
            if (stLexemeProperties.vecSourceStressPos.size() != 1)
            {
                ERROR_LOG(L"Multiple primary stress positions in infnitive.")
            }

//            rc = m_pLexeme->eGetStemStressPositions(m_pLexeme->sInfStem(), vecStress);
//            if (rc != H_NO_ERROR)
//            {
//                return rc;
//            }

            vecStress = stLexemeProperties.vecSourceStressPos;
            if (1 == vecStress.size() || m_pLexeme->bIsMultistressedCompound())
            {
                vector<int>::iterator itStressedSyll = vecStress.begin();
                for (; itStressedSyll != vecStress.end(); ++itStressedSyll)
                {
                    pWordForm->m_mapStress[*itStressedSyll] = STRESS_PRIMARY;
                }
                m_pLexeme->AddWordForm(pWordForm);
            }
            else
            {
                vector<int>::iterator itStressedSyll = vecStress.begin();
                for (; itStressedSyll != vecStress.end(); ++itStressedSyll)
                {
                    if (itStressedSyll != vecStress.begin())
                    {
                        CWordForm * pWfVariant = NULL;
                        CloneWordForm(pWordForm, pWfVariant);
                        pWordForm = pWfVariant;
                    }
                    pWordForm->m_mapStress[*itStressedSyll] = STRESS_PRIMARY;
                    m_pLexeme->AddWordForm(pWordForm);
                }
            }
        }       //  for (int iEnding = 0;...)
    }
    catch (CException& ex)
    {
        CEString sMsg (L"Exception: ");
        sMsg += ex.szGetDescription();
        ERROR_LOG (sMsg);
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}   //  eBuildInfinitive()

ET_ReturnCode CFormBuilderNonFinite::eBuildPresentActiveParticiple()
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    //
    // No participle?
    //
    if (m_pLexeme->bHasDeficiencies())
    {
        CGramHasher hasher (POS_VERB, SUBPARADIGM_PART_PRES_ACT, CASE_NOM, NUM_SG, GENDER_M, 
                            PERSON_UNDEFINED, ANIM_UNDEFINED, m_pLexeme->eAspect(), m_pLexeme->eIsReflexive());
        if (m_pLexeme->bIsFormMissing(hasher.sGramHash()))
        {
            return H_NO_MORE;
        }
    }

    //
    // Irregular participle
    //
    rc = eBuildIrregParticipialFormsLong (SUBPARADIGM_PART_PRES_ACT);
    if (H_NO_ERROR == rc)
    {
        return rc;
    }

    if (L"св" == m_pLexeme->sMainSymbol())
    {
        return H_NO_ERROR;
    }

    //
    // Irregular present tense
    //
    try
    {
        if (bHasIrregularPresent())
        {
            rc = eDeriveIrregPresActiveParticiple();
            return rc;
        }
    }
    catch (CException&)
    {
        return H_EXCEPTION;
    }

    //
    // Regular present tense
    //
    int iType = m_pLexeme->iType();
    CEString sStem;

    CGramHasher hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_PL, GENDER_UNDEFINED, 
                        PERSON_3, ANIM_UNDEFINED, m_pLexeme->eAspect(), m_pLexeme->eIsReflexive());

    auto nFormCount = m_pLexeme->iFormCount(hasher.sGramHash());
    if (nFormCount < 1)
    {
        ASSERT(0);
        ERROR_LOG(L"Failed to obtain praes. 3 pl. form.");
        return H_ERROR_UNEXPECTED;
    }

    for (auto n3PlWf = 0; n3PlWf < nFormCount; ++n3PlWf)
    {
        CWordForm * p3PlWf = NULL;
        rc = m_pLexeme->eWordFormFromHash(hasher.sGramHash(), n3PlWf, p3PlWf);
        if (rc != H_NO_ERROR)
        {
            return rc;
        }
        if (NULL == p3PlWf)
        {
            ASSERT(0);
            ERROR_LOG(L"Failed to obtain praes. 3 pl. form.");
            return H_ERROR_POINTER;
        }

        sStem = p3PlWf->m_sWordForm;
        int iErase = (REFL_YES == m_pLexeme->eIsReflexive()) ? 3 : 1;
        if (iErase >= (int)sStem.uiLength())
        {
            ASSERT(0);
            ERROR_LOG(L"Illegal stem length.");
            return H_ERROR_UNEXPECTED;
        }
        sStem.sErase(sStem.uiLength() - iErase);
        sStem += L'щ';

        ET_Subparadigm eSp(SUBPARADIGM_PART_PRES_ACT);
        vector<int> vecStressPos;
        rc = eGetParticipleStressPos(eSp, vecStressPos);
        if (rc != H_NO_ERROR)
        {
            return rc;
        }

        vector<int>::iterator itStress = vecStressPos.begin();
        for (; itStress != vecStressPos.end(); ++itStress)
        {
            CFormBuilderLongAdj builder(m_pLexeme, sStem, AT_A, eSp, *itStress);
            rc = builder.eBuildParticiple();
        }
    }

    return rc;

}   //  eBuildPresentActiveParticiple()

ET_ReturnCode CFormBuilderNonFinite::eBuildPresentAdverbial()
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    if (L"св" == m_pLexeme->sMainSymbol())
    {
        return H_NO_ERROR;
    }

    ET_ReturnCode rc = H_NO_ERROR;

    CGramHasher advPresHash(POS_VERB, SUBPARADIGM_ADVERBIAL_PRESENT, CASE_UNDEFINED, NUM_UNDEFINED,
                             GENDER_UNDEFINED, PERSON_UNDEFINED, ANIM_UNDEFINED, m_pLexeme->eAspect(), 
                             m_pLexeme->eIsReflexive());

    if (m_pLexeme->bHasDeficiencies())
    {
        CEString iHash = advPresHash.sGramHash();
        if (m_pLexeme->bIsFormMissing(iHash))
        {
            return H_FALSE;
        }
    }

    //
    // Irregular adverbial?
    //
    map<CWordForm *, bool> mapIrreg;
    rc = m_pLexeme->eGetIrregularForms(advPresHash.sGramHash(), mapIrreg);
    if (rc != H_NO_ERROR)
    {
        return rc;
    }

    map<CWordForm *, bool>::iterator it = mapIrreg.begin();
    for (; it != mapIrreg.end(); ++it)
    {
        CWordForm * pWordForm = it->first;
        //rc = eAssignSecondaryStress (pWordForm);  // really needed??
        //if (rc != H_NO_ERROR)
        //{
        //    return rc;
        //}

        try
        {
            m_pLexeme->AddWordForm(pWordForm);
        }
        catch (CException& ex)
        {
            ERROR_LOG(ex.szGetDescription());
            return H_EXCEPTION;
        }
    }
    
    if (m_pLexeme->bNoRegularForms(advPresHash.sGramHash()))
    {
        return mapIrreg.empty() ? H_ERROR_UNEXPECTED : H_NO_ERROR;
    }

    int iType = m_pLexeme->iType();
    if (3 == iType || 8 == iType || 9 == iType || 11 == iType || 14 == iType || 15 == iType)
    {
        return H_NO_ERROR;
    }

    //
    // Irregular 3 Pl pres.? 
    //
    try
    {
        CGramHasher pl3Hash (SUBPARADIGM_PRESENT_TENSE, NUM_PL, GENDER_UNDEFINED, PERSON_3, ANIM_UNDEFINED,
                             m_pLexeme->eAspect(), CASE_UNDEFINED, m_pLexeme->eIsReflexive());
        if (m_pLexeme->bHasIrregularForm(pl3Hash.sGramHash()))
        {
            rc = eDeriveIrregPresAdverbial();
//            if (rc != H_NO_ERROR)
//            {
                return rc;
//            }
        }
    }
    catch (CException& ex)
    {
        CEString sMsg (L"Exception: ");
        sMsg += ex.szGetDescription();
        ERROR_LOG (sMsg);
        return H_EXCEPTION;
    }

    //
    // Regular present tense
    //
    if (13 == iType)
    {
        rc = ePresAdvType13();
    }
    else
    {
        rc = ePresAdvGeneral();
    }

    return rc;

}   //  eBuildPresentAdverbial()

ET_ReturnCode CFormBuilderNonFinite::ePresAdvGeneral(ET_Subparadigm eSubparadigm)
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    try
    {
        vector<int> vecStress;
        CEString sEnding (L"я");     // for the lack of a better term...

        // Stem
        CGramHasher hasher3Pl (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_PL, 
                               GENDER_UNDEFINED, PERSON_3, ANIM_UNDEFINED, m_pLexeme->eAspect(), 
                               m_pLexeme->eIsReflexive());

        CWordForm * p3PlWf = NULL;
        auto n3PlForms = m_pLexeme->iFormCount(hasher3Pl.sGramHash());
        if (n3PlForms < 1)
        {
            ASSERT(0);
            ERROR_LOG(L"No regular 3 Pl forms.");
            return H_ERROR_UNEXPECTED;
        }

        for (auto nSourceForm = 0; nSourceForm < n3PlForms; ++nSourceForm)
        {
            rc = m_pLexeme->eWordFormFromHash(hasher3Pl.sGramHash(), nSourceForm, p3PlWf);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }
            if (NULL == p3PlWf)
            {
                ASSERT(0);
                ERROR_LOG(L"Failed to obtain 3 Pl word form.");
                return H_ERROR_POINTER;
            }

            if (p3PlWf->m_sStem.bIsEmpty())
            {
                if (!m_pLexeme->bHasIrregularForm(hasher3Pl.sGramHash()))
                {
                    ASSERT(0);
                    ERROR_LOG(L"No stem in regular 3 Pl form.");
                }

                p3PlWf->m_sStem = p3PlWf->sWordForm();
                p3PlWf->m_sStem = p3PlWf->m_sStem.sSubstr(0, p3PlWf->m_sStem.uiLength() - 2);
            }

            // Stress
            CGramHasher hasher1Sg(hasher3Pl);
            hasher1Sg.m_eNumber = NUM_SG;
            hasher1Sg.m_ePerson = PERSON_1;

            CWordForm * p1SgWf = NULL;
            int iNForms = m_pLexeme->iFormCount(hasher1Sg.sGramHash());
            for (int iF = 0; iF < iNForms; ++iF)
            {
                rc = m_pLexeme->eWordFormFromHash(hasher1Sg.sGramHash(), iF, p1SgWf);
                if (rc != H_NO_ERROR)
                {
                    return rc;
                }
                if (NULL == p1SgWf)
                {
                    ASSERT(0);
                    ERROR_LOG(L"Failed to obtain 1 Sg word form.");
                    return H_ERROR_POINTER;
                }

                if (p3PlWf->m_sStem.bEndsWithOneOf(L"шжчщц"))
                {
                    sEnding = L"а";
                }

                if (REFL_YES == m_pLexeme->eIsReflexive())
                {
                    sEnding += L"сь";
                }

                CWordForm * pWordForm = new CWordForm;
                if (NULL == pWordForm)
                {
                    ASSERT(0);
                    ERROR_LOG(L"Unable to instantiate CWordForm.");
                    return H_ERROR_POINTER;
                }
                pWordForm->m_pLexeme = m_pLexeme;
                pWordForm->m_ePos = POS_VERB;
                pWordForm->m_eSubparadigm = eSubparadigm;   // usually SUBPARADIGM_ADVERBIAL_PRESENT 
                                                            // but can be SUBPARADIGM_ADVERBIAL_PAST with
                                                            // std deviation "9", see p. 83
                pWordForm->m_eAspect = m_pLexeme->eAspect();
                pWordForm->m_eReflexive = m_pLexeme->eIsReflexive();
                //rc = eAssignSecondaryStress (pWordForm);
                //if (rc != H_NO_ERROR)
                //{
                //    return rc;
                //}            
                pWordForm->m_sStem = p3PlWf->m_sStem;
                pWordForm->m_sWordForm = p3PlWf->m_sStem + sEnding;
                int iStressPos = iStressPosFromTemplate(*p1SgWf, pWordForm->m_sStem);
                pWordForm->m_mapStress[iStressPos] = STRESS_PRIMARY;
                //rc = eAssignSecondaryStress (pWordForm);
                //if (rc != H_NO_ERROR)
                //{
                //    return rc;
                //}

                m_pLexeme->AddWordForm(pWordForm);
            }
        }       //  for (auto nSourceForm = 0;...
    }
    catch (CException& ex)
    {
        CEString sMsg (L"Exception: ");
        sMsg += ex.szGetDescription();
        ERROR_LOG (sMsg);
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}   //  ePresAdvGeneral()

ET_ReturnCode CFormBuilderNonFinite::ePresAdvType13()
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    try
    {
        CWordForm * pWordForm = new CWordForm;
        if (NULL == pWordForm)
        {
            ASSERT(0);
            ERROR_LOG (L"Unable to instantiate CWordForm.");
            return H_ERROR_POINTER;
        }

        pWordForm->m_pLexeme = m_pLexeme;
        pWordForm->m_ePos = POS_VERB;
        pWordForm->m_eSubparadigm = SUBPARADIGM_ADVERBIAL_PRESENT;
        pWordForm->m_eAspect = m_pLexeme->eAspect();
        pWordForm->m_eReflexive = m_pLexeme->eIsReflexive();
        //rc = eAssignSecondaryStress (pWordForm);
        //if (rc != H_NO_ERROR)
        //{
        //    return rc;
        //}

        vector<int> vecStress;

        CEString sEnding (L"я");     // for the lack of a better term...
        if (REFL_YES == m_pLexeme->eIsReflexive())
        {
            sEnding += L"сь";
        }

        CGramHasher hasher (POS_VERB, SUBPARADIGM_INFINITIVE, CASE_UNDEFINED, NUM_UNDEFINED, 
                            GENDER_UNDEFINED, PERSON_UNDEFINED, ANIM_UNDEFINED, 
                            m_pLexeme->eAspect(), m_pLexeme->eIsReflexive());

        int iNForms = m_pLexeme->iFormCount (hasher.sGramHash());
        CWordForm * pInfinitive = NULL;
        for (int iF = 0; iF < iNForms; ++iF)
        {
            rc = m_pLexeme->eWordFormFromHash (hasher.sGramHash(), iF, pInfinitive);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }
            if (NULL == pInfinitive)
            {
                ASSERT(0);
                ERROR_LOG (L"Failed to obtain infinitive.");
                return H_ERROR_POINTER;
            }

            pWordForm->m_sStem = pInfinitive->m_sStem;
            pWordForm->m_sWordForm = pWordForm->m_sStem + sEnding;

            vector<int> vecStress;
            rc = m_pLexeme->eGetStemStressPositions (m_pLexeme->sInfStem(), vecStress);
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
    }
    catch (CException& ex)
    {
        CEString sMsg (L"Exception: ");
        sMsg += ex.szGetDescription();
        ERROR_LOG (sMsg);
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}   //  ePresAdvType13()

ET_ReturnCode CFormBuilderNonFinite::eBuildPresentPassiveParticiple()
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    //
    // Irregular participle
    //
    rc = eBuildIrregParticipialFormsLong (SUBPARADIGM_PART_PRES_PASS_LONG);
    if (H_NO_ERROR == rc)    // found irregular part. pass. praes.
    {
        rc = eDeriveIrregPartPresPassShort (STATUS_COMMON); // i don't know how to set it
        return rc;
    }

    if (L"св" == m_pLexeme->sMainSymbol() || !m_pLexeme->bTransitive() || REFL_YES == m_pLexeme->eIsReflexive())
    {
        return H_NO_ERROR;
    }

    //
    // Irregular present tense
    //
    try
    {
        if (bHasIrregularPresent())
        {
            rc = eDeriveIrregPresPassiveParticiple();
            if (H_NO_ERROR == rc)
            {
                rc = eDeriveIrregPartPresPassShort (STATUS_COMMON); // i don't know how to set it
            }
            return rc;
        }
    }
    catch (CException&)
    {
        return H_EXCEPTION;
    }

    //
    // Regular present tense
    //
    int iGroup = -1;       // GDRL, p.86

    CGramHasher hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_PL, GENDER_UNDEFINED, 
                        PERSON_1, ANIM_UNDEFINED, m_pLexeme->eAspect(), m_pLexeme->eIsReflexive());
    CWordForm * p1Pl = NULL;

    auto n1PlForms = m_pLexeme->iFormCount(hasher.sGramHash());
    if (n1PlForms < 1)
    {
        ASSERT(0);
        ERROR_LOG(L"Unable to acquire 1 Pl form.");
        return H_ERROR_UNEXPECTED;
    }

    for (auto nSourceForm = 0; nSourceForm < n1PlForms; ++nSourceForm)
    {
        rc = m_pLexeme->eWordFormFromHash(hasher.sGramHash(), nSourceForm, p1Pl);
        if (rc != H_NO_ERROR)
        {
            return rc;
        }
        if (NULL == p1Pl)
        {
            ASSERT(0);
            ERROR_LOG(L"Failed to obtain infinitive.");
            return H_ERROR_POINTER;
        }

        CEString& s1Pl = p1Pl->m_sWordForm;
        if (s1Pl.bEndsWith(L"ем") &&
            p1Pl->m_mapStress.find(s1Pl.uiLength() - 2) == p1Pl->m_mapStress.end() &&
            CEString::bIn(s1Pl[s1Pl.uiLength() - 3], g_szRusVowels))
        {
            ASSERT((1 == m_pLexeme->iType() && AT_A == m_pLexeme->eAccentType1()) ||
                (2 == m_pLexeme->iType() && AT_A == m_pLexeme->eAccentType1()) ||
                (12 == m_pLexeme->iType() && AT_A == m_pLexeme->eAccentType1()) ||
                (6 == m_pLexeme->iType() && AT_A == m_pLexeme->eAccentType1() &&
                    m_pLexeme->sSourceForm().bEndsWith(L"ять")) || 13 == m_pLexeme->iType());
            iGroup = 1;
        }
        else if (4 == m_pLexeme->iType())
        {
            iGroup = 2;
        }
        else if (5 == m_pLexeme->iType() ||
            (6 == m_pLexeme->iType() && m_pLexeme->sSourceForm().bEndsWith(L"ать")) ||
            (6 == m_pLexeme->iType() && 1 == m_pLexeme->bStemAugment()) ||
            (7 == m_pLexeme->iType()))
        {
            //        слышать видеть гнать (терпеть зреть?)
            if (5 == m_pLexeme->iType())
            {
                vector<CEString> vecAllowed{ L"слышать", L"видеть", L"гнать" };
                auto sSourceForm = m_pLexeme->sSourceForm();
                if (!any_of(vecAllowed.begin(), vecAllowed.end(), [sSourceForm](CEString sAllowedForm) { return sSourceForm == sAllowedForm; }))
                {
                    return H_NO_ERROR;
                }
            }
            //        глаголать колебать колыхать
            if (6 == m_pLexeme->iType())
            {
                vector<CEString> vecAllowed{ L"глаголать", L"колебать", L"колыхать", L"двигать" };
                auto sSourceForm = m_pLexeme->sSourceForm();
                if (!any_of(vecAllowed.begin(), vecAllowed.end(), [sSourceForm](CEString sAllowedForm) { return sSourceForm == sAllowedForm; }))
                {
                    return H_NO_ERROR;
                }
            }

            //        везти пасти нести вести
            if (7 == m_pLexeme->iType())
            {
                vector<CEString> vecAllowed{ L"везти", L"пасти", L"нести", L"вести" };
                auto sSourceForm = m_pLexeme->sSourceForm();
                if (!any_of(vecAllowed.begin(), vecAllowed.end(), [sSourceForm](CEString sAllowedForm) { return sSourceForm == sAllowedForm; }))
                {
                    return H_NO_ERROR;
                }
            }

            iGroup = 3;
        }

        if (iGroup < 1)
        {
            return H_NO_ERROR;
        }

        CEString sStem;
        if (13 == m_pLexeme->iType())
        {
            sStem = m_pLexeme->sInfStem();
            sStem += L"ем";
        }
        else
        {
            sStem = p1Pl->m_sWordForm;
            if (sStem.bEndsWith(L"ём"))
            {
                if (CEString::bIn(sStem[sStem.uiLength() - 3], g_szRusConsonants))
                {
                    sStem.sReplace(sStem.uiLength() - 2, 2, L"ом");
                }
            }

            if (3 == iGroup && 6 == m_pLexeme->iType() && m_pLexeme->sSourceForm() == L"двигать")
            {
                sStem = L"движим";
            }
        }

        ET_Status eStatus = (1 != iGroup) ? STATUS_RARE : STATUS_COMMON;

        ET_Subparadigm eSp(SUBPARADIGM_PART_PRES_PASS_LONG);
        vector<int> vecStress;
        rc = eGetParticipleStressPos(eSp, vecStress);
        if (rc != H_NO_ERROR)
        {
            return rc;
        }

        vector<int>::iterator itStress = vecStress.begin();
        for (; (H_NO_ERROR == rc) && (itStress != vecStress.end()); ++itStress)
        {
            CFormBuilderLongAdj co_long(m_pLexeme, sStem, AT_A, eSp, *itStress, eStatus);
            rc = co_long.eBuildParticiple();

            eSp = SUBPARADIGM_PART_PRES_PASS_SHORT;
            bool bYoAlternation = false;
            bool bFleetingVowel = false;
            CFormBuilderShortAdj shortAdj(m_pLexeme,
                bYoAlternation,
                sStem,
                eSp,
                AT_A,
                AT_A,
                vector<int>(1, *itStress),
                bFleetingVowel);
            shortAdj.SetUsageStatus(eStatus);
            rc = shortAdj.eBuild();
        }
    }       //  for (auto nSourceForm = 0; ...

    return H_NO_ERROR;

}   //  eBuildPresentPassPart()

ET_ReturnCode CFormBuilderNonFinite::eBuildPastActiveParticiple()
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    //
    // Irregular participle
    //
    rc = eBuildIrregParticipialFormsLong (SUBPARADIGM_PART_PAST_ACT);
    if (H_NO_ERROR == rc)
    {
        return rc;
    }

    //
    // Irregular past tense
    //
    try
    {
        if (bHasIrregularPast())
        {
            rc = eDeriveIrregPastActPartAndAdverbial();
            return rc;
        }
    }
    catch (CException&)
    {
        return H_EXCEPTION;
    }

    //
    // Regular participle
    //
    int iType = m_pLexeme->iType();

    CWordForm * pWordForm = new CWordForm;
    if (NULL == pWordForm)
    {
        ASSERT(0);
        ERROR_LOG (L"Unable to instantiate CWordForm");
        return H_ERROR_POINTER;
    }

    pWordForm->m_pLexeme = m_pLexeme;
    pWordForm->m_eAspect = m_pLexeme->eAspect();

    //
    // Type 7 ending in -сти with -т- or -д- alternation mark:
    // 1 Sg stem present tense + -ший
    //
    if (7 == iType && 
        (m_pLexeme->sSourceForm()).bEndsWith (L"сти") &&
        (L"т" == m_pLexeme->sVerbStemAlternation() || L"д" == m_pLexeme->sVerbStemAlternation()))
    {
        pWordForm->m_sStem = m_pLexeme->s1SgStem();
        pWordForm->m_sStem += L"ш";
    }
    else
    {
        // All other types: derived from Past Sg. m 
        CGramHasher hasher(POS_VERB, SUBPARADIGM_PAST_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_M,
            PERSON_UNDEFINED, ANIM_UNDEFINED, m_pLexeme->eAspect(),
            m_pLexeme->eIsReflexive());
        CWordForm * pPastM = NULL;
        auto nPastM = m_pLexeme->iFormCount(hasher.sGramHash());
        if (nPastM < 1)
        {
            ASSERT(0);
            ERROR_LOG(L"Failed to obtain past tense m. form.");
            return H_ERROR_UNEXPECTED;
        }

        for (auto nSourceForm = 0; nSourceForm < nPastM; ++nSourceForm)
        {
            rc = m_pLexeme->eWordFormFromHash(hasher.sGramHash(), nSourceForm, pPastM);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }
            if (NULL == pPastM)
            {
                ASSERT(0);
                ERROR_LOG(L"Failed to obtain past tense m. form.");
                return H_ERROR_POINTER;
            }

            CEString sWf(pPastM->m_sWordForm);
            if (REFL_YES == m_pLexeme->eIsReflexive())
            {
                if (sWf.uiLength() < 3)
                {
                    ASSERT(0);
                    ERROR_LOG(L"Illegal stem length.");
                    return H_ERROR_GENERAL;
                }

                sWf.sErase(sWf.uiLength() - 2);
            }

            //
            // If past Sg. m ends in -л, replace it with -вший, otherwise add -ший
            //
            if (sWf.bEndsWith(L"л"))
            {
                pWordForm->m_sStem = sWf.sErase(sWf.uiLength() - 1);
                pWordForm->m_sStem += L"вш";
            }
            else
            {
                pWordForm->m_sStem = sWf;
                pWordForm->m_sStem += L"ш";
            }
        }       //  for (auto nSourceForm = 0; nSourceForm < nPastM; ++nSourceForm)

        ET_Subparadigm eSp(SUBPARADIGM_PART_PAST_ACT);
        vector<int> vecStressPos;
        rc = eGetParticipleStressPos(eSp, vecStressPos);
        if (rc != H_NO_ERROR)
        {
            return rc;
        }

        vector<int>::iterator itStress = vecStressPos.begin();
        for (; itStress != vecStressPos.end(); ++itStress)
        {
            CFormBuilderLongAdj builder(m_pLexeme, pWordForm->m_sStem, AT_A, eSp, *itStress);
            rc = builder.eBuildParticiple();
        }
    }

    return rc;

}   //  h_BuildPastActiveParticiple()

ET_ReturnCode CFormBuilderNonFinite::eBuildPastAdverbial()
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    //
    // Irregular past adverbial
    //

// TODO: Verify the followoing has already happened in CLexeme
/*
    try
    {
        CGramHasher pastAdv (POS_VERB, SUBPARADIGM_ADVERBIAL_PAST, CASE_UNDEFINED, 
                             NUM_UNDEFINED, GENDER_UNDEFINED, PERSON_UNDEFINED, ANIM_UNDEFINED, 
                             m_pLexeme->eAspect(), m_pLexeme->eIsReflexive());
        map<CWordForm *, bool> mapIrreg;
        m_pLexeme->GetIrregularForms(pastAdv.iGramHash(), mapIrreg);
        map<CWordForm *, bool>::iterator it = mapIrreg.begin();
        for (; it != mapIrreg.end; ++it)
        {
            m_pLexeme->AddWordForm(it->first);
        }
    }
    catch (CException ex)
    {
        return ex.iGetErrorCode();
    }
*/
/*
    if (bHasIrregularPast())
    {
        return hr; // has been done already -- combined with past part act?
    }
*/
    try
    {
        CGramHasher partPastAct (POS_VERB, SUBPARADIGM_PART_PAST_ACT, CASE_NOM, NUM_SG, GENDER_M, 
                                 PERSON_UNDEFINED, ANIM_NO, m_pLexeme->eAspect(), m_pLexeme->eIsReflexive());

        CWordForm * pPastPart = NULL;
        int iNForms = m_pLexeme->iFormCount (partPastAct.sGramHash());
        for (int iF = 0; iF < iNForms; ++iF)
        {
            rc = m_pLexeme->eWordFormFromHash(partPastAct.sGramHash(), iF, pPastPart);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }
            if (NULL == pPastPart)
            {
                ASSERT(0);
                ERROR_LOG (L"Failed to obtain past participle.");
                return H_ERROR_POINTER;
            }

            CWordForm * pWordForm = new CWordForm;
            if (NULL == pWordForm)
            {
                ASSERT(0);
                ERROR_LOG (L"Unable to instantiate CWordForm");
                return H_ERROR_POINTER;
            }

            pWordForm->m_pLexeme = m_pLexeme;
            pWordForm->m_ePos = POS_VERB;
            pWordForm->m_eSubparadigm = SUBPARADIGM_ADVERBIAL_PAST;
            pWordForm->m_eAspect = m_pLexeme->eAspect();
            pWordForm->m_eReflexive = m_pLexeme->eIsReflexive();
            //rc = eAssignSecondaryStress (pWordForm);
            //if (rc != H_NO_ERROR)
            //{
            //    return rc;
            //}

            pWordForm->m_sStem = pPastPart->m_sStem;
            int iAt = (REFL_NO == pWordForm->m_eReflexive) 
                ? pPastPart->m_sWordForm.uiLength() - 3
                : pPastPart->m_sWordForm.uiLength() - 5;
            pWordForm->m_sWordForm = pPastPart->m_sWordForm;
            if (REFL_YES == pWordForm->m_eReflexive)
            {
                pWordForm->m_sWordForm.sReplace (iAt, 4, L"шись");
                pWordForm->m_sWordForm.sErase (pWordForm->m_sWordForm.uiLength()-1);
            }
            else
            {
                pWordForm->m_sWordForm.sErase (pWordForm->m_sWordForm.uiLength()-1);
            }

            if (L"нсв" == m_pLexeme->sMainSymbol())
            {
                pWordForm->m_eStatus = STATUS_RARE;
            }

            if (L"св" == m_pLexeme->sMainSymbol() && m_pLexeme->bHasCommonDeviation (9))
            {
                pWordForm->m_eStatus = STATUS_OBSOLETE;
            }

            int iStressPos = iStressPosFromTemplate (*pPastPart, pWordForm->m_sStem);
            pWordForm->m_mapStress[iStressPos] = STRESS_PRIMARY;

            m_pLexeme->AddWordForm (pWordForm);

            if (CEString (pWordForm->m_sWordForm).bEndsWith (L"вши"))
            {
                CWordForm * pWordForm2 = new CWordForm;
                if (NULL == pWordForm2)
                {
                    ASSERT(0);
                    ERROR_LOG (L"Unable to instantiate CWordForm");
                    return H_ERROR_POINTER;
                }

                CloneWordForm (pWordForm, pWordForm2);
                pWordForm2->m_sWordForm = pWordForm->m_sWordForm;
                pWordForm2->m_sWordForm.sErase (pWordForm->m_sWordForm.uiLength()-2);
                m_pLexeme->AddWordForm (pWordForm2);
            }

            if (m_pLexeme->bHasCommonDeviation (9))
            {
                if (L"св" != m_pLexeme->sMainSymbol())
                {
                    ASSERT(0);
                    ERROR_LOG (L"Non-perfective verb with CD-9");
                    return H_ERROR_UNEXPECTED;
                }
                
                rc = ePresAdvGeneral(SUBPARADIGM_ADVERBIAL_PAST); // GDRL p. 83: use present form
                if (rc != H_NO_ERROR)
                {
                    return rc;
                }
            }

        }   //  for (int iF = 0; iF < iNForms; ++iF)

        // GDRL p. 85
        if (L"св" == m_pLexeme->sMainSymbol() && REFL_NO == m_pLexeme->eIsReflexive() &&
            ((3 == m_pLexeme->iType() && m_pLexeme->bStemAugment() == 1) ||                   // погибнуть
                9 == m_pLexeme->iType()))                                                     // растереть
        {
            CGramHasher inf(POS_VERB, SUBPARADIGM_INFINITIVE, CASE_UNDEFINED, NUM_UNDEFINED, GENDER_UNDEFINED,
                PERSON_UNDEFINED, ANIM_UNDEFINED, m_pLexeme->eAspect(), REFL_NO);

            CWordForm * pInfinitive = NULL;
            auto nInf = m_pLexeme->iFormCount(inf.sGramHash());
            //            ASSERT(m_pLexeme->iFormCount(inf.sGramHash()) == 1);
            for (auto nSourceForm = 0; nSourceForm < nInf; ++nSourceForm)
            {
                rc = m_pLexeme->eWordFormFromHash(inf.sGramHash(), nSourceForm, pInfinitive);
                if (rc != H_NO_ERROR)
                {
                    return rc;
                }
                if (NULL == pInfinitive)
                {
                    ASSERT(0);
                    ERROR_LOG(L"Failed to obtain infinitive.");
                    return H_ERROR_POINTER;
                }

                CWordForm * pWordForm = new CWordForm;
                if (NULL == pWordForm)
                {
                    ASSERT(0);
                    ERROR_LOG(L"Unable to instantiate CWordForm");
                    return H_ERROR_POINTER;
                }

                pWordForm->m_pLexeme = m_pLexeme;
                pWordForm->m_ePos = POS_VERB;
                pWordForm->m_eSubparadigm = SUBPARADIGM_ADVERBIAL_PAST;
                pWordForm->m_eAspect = m_pLexeme->eAspect();
                pWordForm->m_eReflexive = m_pLexeme->eIsReflexive();
                //rc = eAssignSecondaryStress (pWordForm);
                //if (rc != H_NO_ERROR)
                //{
                //    return rc;
                //}

                pWordForm->m_sStem = m_pLexeme->sInfStem();
                pWordForm->m_sWordForm = pWordForm->m_sStem + L"в";
                int iStressPos = iStressPosFromTemplate(*pInfinitive, pWordForm->m_sStem);
                pWordForm->m_mapStress[iStressPos] = STRESS_PRIMARY;
                m_pLexeme->AddWordForm(pWordForm);

                if (3 == m_pLexeme->iType())        // погибнувши, but not растеревши? -- see p. 85
                {
                    CWordForm * pWordForm2 = NULL;
                    CloneWordForm(pWordForm, pWordForm2);
                    pWordForm2->m_sWordForm += L"ши";
                    m_pLexeme->AddWordForm(pWordForm2);
                }
            }       //  for (auto nSourceForm = 0;...
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

}   //  eBuildPastAdverbial()

ET_ReturnCode CFormBuilderNonFinite::eBuildPastPassiveParticiple()
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    //
    // Irregular participle
    //
    rc = eBuildIrregParticipialFormsLong (SUBPARADIGM_PART_PAST_PASS_LONG);
    if (H_NO_ERROR == rc)
    {
        if (bHasIrregPartPastPassShort())
        {
            bool bIsVariant = false;
            rc = eBuildIrregPartPastPassShort (bIsVariant);
            if (!bIsVariant || rc != H_NO_ERROR)
            {
                return rc;
            }
        }
        else
        {
            rc = eDeriveIrregPartPastPassShort();
            return rc;
        }
    }

    ET_Status eStatus = STATUS_COMMON;

    if (!m_pLexeme->bTransitive() || REFL_YES == m_pLexeme->eIsReflexive())
    {
        return H_NO_ERROR;
    }

    if (m_pLexeme->bNoPastParticiple())
    {
        return H_NO_ERROR;
    }

    if (L"нсв" == m_pLexeme->sMainSymbol())
    {
        if (m_pLexeme->bHasAspectPair())
        {
            return H_NO_ERROR;
        }
        else
        {
            eStatus = STATUS_RARE;
        }
    }

    if (m_pLexeme->bPastParticipleRestricted())
    {
        eStatus = STATUS_QUESTIONABLE;
    }

    CEString sStem;
    if ((m_pLexeme->sSourceForm().bEndsWith (L"ать") || 
         m_pLexeme->sSourceForm().bEndsWith (L"ять")) &&
        14 != m_pLexeme->iType())
    {
        sStem = m_pLexeme->sInfStem();
        if (2 == m_pLexeme->iType() && m_pLexeme->bHasOAlternation())
        {
            if (sStem.uiLength() <= 3 || !sStem.bEndsWith (L"ева"))
            {
                ASSERT(0);
                CEString sMsg (L"Stem with o-alternation does not end in 'eva'.");
                ERROR_LOG (sMsg);
                throw CException (H_ERROR_UNEXPECTED, sMsg);
            }
            sStem[sStem.uiLength()-3] = L'о';
        }

        sStem += L"нн";
    }

    if (4 == m_pLexeme->iType() || 
        (5 == m_pLexeme->iType() && m_pLexeme->sSourceForm().bEndsWith (L"еть")))
    {
        sStem = m_pLexeme->s1SgStem();
        if (m_pLexeme->bPartPastPassZhd())
        {
            ASSERT(sStem.bEndsWith(L"ж"));
            sStem += L"д";
        }
        sStem += L"енн";
    }

    if (1 == m_pLexeme->iType() && m_pLexeme->sSourceForm().bEndsWith (L"еть"))
    {
        if (m_pLexeme->sSourceForm().bEndsWith (L"одолеть") ||
            m_pLexeme->sSourceForm().bEndsWith (L"печатлеть"))
        {
            sStem = m_pLexeme->sSourceForm();
            sStem.sErase (m_pLexeme->sSourceForm().uiLength()-3);
            sStem += L"ённ";
        }
    }

    if (7 == m_pLexeme->iType() || 8 == m_pLexeme->iType())
    {
        sStem = m_pLexeme->s3SgStem();
        sStem += L"енн";
    }

    if (3 == m_pLexeme->iType() || 10 == m_pLexeme->iType())        // NB also 3 + circle
    {
        sStem = m_pLexeme->sInfStem();
        sStem += L"т";
    }

    if (9 == m_pLexeme->iType() || 11 == m_pLexeme->iType()  || 12 == m_pLexeme->iType() || 
        14 == m_pLexeme->iType() || 15 == m_pLexeme->iType() || 16 == m_pLexeme->iType())
    {
        CGramHasher hasher (POS_VERB, SUBPARADIGM_PAST_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_M, 
                            PERSON_UNDEFINED, ANIM_UNDEFINED, m_pLexeme->eAspect(), 
                            m_pLexeme->eIsReflexive());
        CWordForm * pPastM = NULL;
        int iNForms = m_pLexeme->iFormCount (hasher.sGramHash());
        for (int iWf = 0; iWf < iNForms; ++iWf)
        {
            rc = m_pLexeme->eWordFormFromHash (hasher.sGramHash(), iWf, pPastM);  
            if (rc != H_NO_ERROR)
            {
                return rc;
            }
            if (NULL == pPastM)
            {
                ASSERT(0);
                ERROR_LOG (L"Failed to obtain past tense m. form.");
                return H_ERROR_POINTER;
            }

            sStem = pPastM->m_sWordForm;
            if (sStem.bEndsWith (L"л"))
            {
                sStem.sErase (sStem.uiLength()-1);
            }
            sStem += L"т";
        }

    }   //      if (9 == m_pLexeme->iType() ...)

    ET_Subparadigm eSp (SUBPARADIGM_PART_PAST_PASS_LONG);
    vector<int> vecStress;
    rc = eGetParticipleStressPos (eSp, vecStress);
    if (rc != H_NO_ERROR || vecStress.empty())
    {
        ASSERT(0);
        ERROR_LOG (L"Failed to obtain part pass past stress positions.");
        return H_ERROR_GENERAL;
    }

    if (sStem.bIsEmpty())
    {
        return H_ERROR_UNEXPECTED;
    }

    vector<int>::iterator itStress = vecStress.begin();
    for (; (H_NO_ERROR == rc) && (itStress != vecStress.end()); ++itStress)
    {
        if (4 == m_pLexeme->iType() || 5 == m_pLexeme->iType() || 
            7 == m_pLexeme->iType() || 8 == m_pLexeme->iType())
        {
            if (*itStress >= (int)sStem.uiLength())
            {
                ASSERT(0);
                ERROR_LOG (L"Bad stress position.");
                rc = H_ERROR_GENERAL;
            }

            if (*itStress == sStem.uiNSyllables() - 1)
            {
                int iStressPos = sStem.uiGetVowelPos (*itStress);
                if (L'е' == sStem[iStressPos])
                {
                    sStem[iStressPos] = L'ё';
                }
            }
        }
        CFormBuilderLongAdj builder (m_pLexeme, sStem, AT_A, eSp, *itStress, eStatus);
        rc = builder.eBuildParticiple();
    
    }   // for...

    if (rc != H_NO_ERROR)
    {
        return rc;
    }

    //
    // Short forms
    //
    CGramHasher partPastPassLong (POS_VERB, SUBPARADIGM_PART_PAST_PASS_LONG, CASE_NOM, NUM_SG, 
                                  GENDER_M, PERSON_UNDEFINED, ANIM_NO, m_pLexeme->eAspect(), 
                                  m_pLexeme->eIsReflexive());
    CWordForm * pNSgMLong = NULL;
    int iNForms = m_pLexeme->iFormCount (partPastPassLong.sGramHash());
    for (int iWf = 0; iWf < iNForms; ++iWf)
    {
        rc = m_pLexeme->eWordFormFromHash (partPastPassLong.sGramHash(), iWf, pNSgMLong);
        if (rc != H_NO_ERROR)
        {
            return rc;
        }
        if (NULL == pNSgMLong)
        {
            ASSERT(0);
            ERROR_LOG (L"Failed to obtain N Sg m of the long form.");
            return H_ERROR_POINTER;
        }

        CEString sNSgMLong (pNSgMLong->m_sWordForm);
        sNSgMLong.SetVowels (g_szRusVowels);
        if (sNSgMLong.bEndsWith (L"анный") || sNSgMLong.bEndsWith (L"янный") ||
            sNSgMLong.bEndsWith (L"енный"))
        {
            map<int, ET_StressType>::iterator itStress = pNSgMLong->m_mapStress.begin();
            for (; itStress != pNSgMLong->m_mapStress.end(); ++itStress)
            {

                if (sNSgMLong.uiNSyllables()-2 == (*itStress).first && 
                    STRESS_PRIMARY == (*itStress).second)
                {
                    ET_AccentType eAccentType = ET_AccentType::AT_UNDEFINED;
                    bool bYoAlternation = false;
                    bool bFleetingVowel = true;

                    //  -Анный/-Янный: rare, see GDRL p. 86 footnote 4
                    if (m_pLexeme->sSourceForm().uiNSyllables() == 1)
                    {
                        eAccentType = m_pLexeme->eAccentType2();
                    }
                    else if ((m_pLexeme->sSourceForm().bEndsWithNoCase(L"ать") 
                        || m_pLexeme->sSourceForm().bEndsWithNoCase(L"ять"))
                        && m_pLexeme->bHasCommonDeviation(7))
                    {
                        eAccentType = AT_A;
                    }
                    else
                    {
                        ERROR_LOG(L"Unexpected part. past pass. ending in -Янный or -Анный");
                        return H_ERROR_UNEXPECTED;
                    }
                    CFormBuilderShortAdj shortAdj(m_pLexeme,
                                                  bYoAlternation,
                                                  pNSgMLong->m_sStem,
                                                  SUBPARADIGM_PART_PAST_PASS_SHORT,
                                                  AT_A,
                                                  eAccentType,
                                                  vector<int>(1, (*itStress).first),
                                                  bFleetingVowel,
                                                  2,
                                                  eStatus);
                    if (STATUS_COMMON != eStatus)
                    {
                        shortAdj.SetUsageStatus(eStatus);
                    }
                    rc = shortAdj.eBuild();
                    if (rc != H_NO_ERROR)
                    {
                        return rc;
                    }

                }
                else
                {
                    bool bYoAlternation = false;
                    bool bFleetingVowel = true;
                    CFormBuilderShortAdj shortAdj (m_pLexeme, 
                                                   bYoAlternation, 
                                                   pNSgMLong->m_sStem,
                                                   SUBPARADIGM_PART_PAST_PASS_SHORT, 
                                                   AT_A, 
                                                   AT_A,
                                                   vector<int>(1, (*itStress).first),
                                                   bFleetingVowel,
                                                   2,
                                                   eStatus);
                    if (STATUS_COMMON != eStatus)
                    {
                        shortAdj.SetUsageStatus (eStatus);
                    }
                    rc = shortAdj.eBuild();
                    if (rc != H_NO_ERROR)
                    {
                        return rc;
                    }
                }
            }
        }

        if (sNSgMLong.bEndsWith (L"ённый"))
        {
            bool bYoAlternation = true;
            bool bFleetingVowel = true;
            map<int, ET_StressType>::iterator itStress = pNSgMLong->m_mapStress.begin();
            if ((*itStress).second != STRESS_PRIMARY || 
                (*itStress).first != sNSgMLong.uiGetNumOfSyllables()-2)
            {
                ASSERT(0);
                ERROR_LOG (L"Unexpected stress position.");
                return H_ERROR_POINTER;
            }
            CFormBuilderShortAdj shortAdj (m_pLexeme, 
                                           bYoAlternation, 
                                           pNSgMLong->m_sStem,
                                           SUBPARADIGM_PART_PAST_PASS_SHORT, 
                                           AT_A, 
                                           AT_B,
                                           vector<int>(1, (*itStress).first),
                                           bFleetingVowel,
                                           2,
                                           eStatus);
            rc = shortAdj.eBuild();
            if (rc != H_NO_ERROR)
            {
                return rc;
            }
        }

        if (sNSgMLong.bEndsWith (L"тый"))
        {
            bool bYoAlternation = false;
            bool bFleetingVowel = false;
            map<int, ET_StressType>::iterator itStress = pNSgMLong->m_mapStress.begin();

            ET_AccentType eAt2;
            if (AT_A == m_pLexeme->eAccentType2() || AT_UNDEFINED == m_pLexeme->eAccentType2())
            {
                eAt2 = AT_A;
            }
            else if (AT_C == m_pLexeme->eAccentType2() || AT_C1 == m_pLexeme->eAccentType2() 
                || AT_C2 == m_pLexeme->eAccentType2())
            {
                eAt2 = m_pLexeme->eAccentType2();
            }
            else
            {
                ASSERT(0);
                ERROR_LOG (L"Unexpected accent type.");
                return H_ERROR_POINTER;
            }

            CFormBuilderShortAdj shortAdj (m_pLexeme, 
                                           bYoAlternation, 
                                           sStem,
                                           SUBPARADIGM_PART_PAST_PASS_SHORT, 
                                           AT_A, 
                                           eAt2,
                                           vector<int>(1, (*itStress).first),
                                           bFleetingVowel);
            if (STATUS_COMMON != eStatus)
            {
                shortAdj.SetUsageStatus (eStatus);
            }
            rc = shortAdj.eBuild();
            if (rc != H_NO_ERROR)
            {
                return rc;
            }
        }

    }   //  for (int iWf = 0 ...)

    return rc;

}   //  eBuildPastPassiveParticiple()

ET_ReturnCode CFormBuilderNonFinite::eGetParticipleStressPos (ET_Subparadigm eSubparadigm, vector<int>& vecPositions)
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    CEString sHash;
    vecPositions.clear();

    try
    {
        switch (eSubparadigm)
        {
            case SUBPARADIGM_PART_PRES_ACT:
            {
                CGramHasher hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_SG, 
                                    GENDER_UNDEFINED, PERSON_UNDEFINED, ANIM_UNDEFINED, 
                                    m_pLexeme->eAspect(), m_pLexeme->eIsReflexive());
                if (4 == m_pLexeme->iType() || 5 == m_pLexeme->iType())
                {
                    hasher.m_ePerson = PERSON_1;
                }
                else
                {
                    hasher.m_ePerson = PERSON_3;
                }

                sHash = hasher.sGramHash();

                break;
            }
            case SUBPARADIGM_PART_PAST_ACT:
            {
                if (m_pLexeme->bHasCommonDeviation(1) && (m_pLexeme->iType() != 9))
                {
                    CGramHasher hasher (POS_VERB, SUBPARADIGM_INFINITIVE, CASE_UNDEFINED, NUM_UNDEFINED, 
                                        GENDER_UNDEFINED, PERSON_UNDEFINED, ANIM_UNDEFINED, 
                                        m_pLexeme->eAspect(), m_pLexeme->eIsReflexive());
                    sHash = hasher.sGramHash();
                }
                else
                {
                    CGramHasher hasher (POS_VERB, SUBPARADIGM_PAST_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_M, 
                                        PERSON_UNDEFINED, ANIM_UNDEFINED, m_pLexeme->eAspect(), 
                                        m_pLexeme->eIsReflexive());
                    sHash = hasher.sGramHash();
                }

                break;
            }
            case SUBPARADIGM_PART_PRES_PASS_LONG:
            {
                if (13 == m_pLexeme->iType())
                {
                    CGramHasher hasher (POS_VERB, SUBPARADIGM_INFINITIVE, CASE_UNDEFINED, NUM_UNDEFINED, 
                                        GENDER_UNDEFINED, PERSON_UNDEFINED, ANIM_UNDEFINED, 
                                        m_pLexeme->eAspect(), m_pLexeme->eIsReflexive());
                    sHash = hasher.sGramHash();
                }
                else
                {
                    if (4 == m_pLexeme->iType() || 5 == m_pLexeme->iType())
                    {
                        CGramHasher hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_SG, 
                                            GENDER_UNDEFINED, PERSON_1, ANIM_UNDEFINED, 
                                            m_pLexeme->eAspect(), m_pLexeme->eIsReflexive());
                        sHash = hasher.sGramHash();
                    }
                    else
                    {
                        CGramHasher hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_PL, 
                                            GENDER_UNDEFINED, PERSON_1, ANIM_UNDEFINED, 
                                            m_pLexeme->eAspect(), m_pLexeme->eIsReflexive());
                        sHash = hasher.sGramHash();
                    }
                }

                break;
            }
            case SUBPARADIGM_PART_PAST_PASS_LONG:
            {
                if ((m_pLexeme->sSourceForm().bEndsWith (L"ать") || m_pLexeme->sSourceForm().bEndsWith (L"ять")) &&
                    14 != m_pLexeme->iType())
                {
                    int iStressPos = -1;
                    rc = m_pLexeme->eGetFirstStemStressPos(iStressPos);
                    while (H_NO_ERROR == rc)
                    {
                        if (iStressPos < 0)
                        {
                            ASSERT(0);
                            CEString sMsg (L"Illegal stress position.");
                            ERROR_LOG (sMsg);
                            return H_ERROR_UNEXPECTED;
                        }

                        int iNSyll = m_pLexeme->sSourceForm().uiNSyllables();
                        if (iNSyll > 1 && iStressPos == iNSyll - 1)         // polysyllabic infinitive with 
                        {                                                   // stress on last syllable
                            vecPositions.push_back (iNSyll - 2);
                        }
                        else
                        {
                            vecPositions.push_back (iStressPos);
                        }
                        rc = m_pLexeme->eGetNextStemStressPos(iStressPos);
                    }
                }

                if (4 == m_pLexeme->iType())    // same syllable (counting from right) as in 3 Sg Praes
                {
                    CGramHasher sg3Pres (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_SG, 
                                         GENDER_UNDEFINED, PERSON_3, ANIM_UNDEFINED, 
                                         m_pLexeme->eAspect(), m_pLexeme->eIsReflexive());
                    int iNForms = m_pLexeme->iFormCount (sg3Pres.sGramHash());
                    CWordForm * p3Sg = NULL;
                    for (int iF = 0; iF < iNForms; ++iF)
                    {
                        rc = m_pLexeme->eWordFormFromHash (sg3Pres.sGramHash(), iF, p3Sg);
                        if (rc != H_NO_ERROR)
                        {
                            return rc;
                        }
                        if (NULL == p3Sg)
                        {
                            ASSERT(0);
                            ERROR_LOG (L"Failed to obtain template form.");
                            return H_ERROR_POINTER;
                        }

                        map<int, ET_StressType>::iterator itStressPos = p3Sg->m_mapStress.begin();
                        for (; p3Sg->m_mapStress.end() != itStressPos; ++itStressPos)
                        {
                            if (STRESS_PRIMARY == (*itStressPos).second)
                            {
                                vecPositions.push_back ((*itStressPos).first);
                            }
                        }
                    }       //  for (int iF = 0; iF < iNForms; ++iF)

                }   //   if (4 == m_pLexeme->iType())
                                                   
                if (5 == m_pLexeme->iType() && m_pLexeme->sSourceForm().bEndsWith(L"еть"))
                {
                    int iStressPos = -1;
                    rc = m_pLexeme->eGetFirstStemStressPos(iStressPos);
                    while (H_NO_ERROR == rc)
                    {
                        if (iStressPos < 0)
                        {
                            ASSERT(0);
                            CEString sMsg (L"Illegal stress position.");
                            ERROR_LOG (sMsg);
                            return H_ERROR_UNEXPECTED;
                        }

                        int iNSyll = m_pLexeme->sSourceForm().uiNSyllables();
                        if (iNSyll > 1)
                        {
                            vecPositions.push_back (iNSyll - 2);
                        }
                        else
                        {
                            vecPositions.push_back (iStressPos);
                        }

                        rc = m_pLexeme->eGetNextStemStressPos(iStressPos);

                        if (vecPositions.size() > 100)
                        {
                            ASSERT(0);
                            CEString sMsg(L"Too many stress positions.");
                            ERROR_LOG(sMsg);
                            return H_ERROR_UNEXPECTED;
                        }
                    }
                }

                if (1 == m_pLexeme->iType() && m_pLexeme->sSourceForm().bEndsWith(L"еть"))
                {                                                           // GDRL p. 86, footnote 3
                    vecPositions.push_back ((int)m_pLexeme->sSourceForm().uiGetNumOfSyllables() - 1);
                }

                if (7 == m_pLexeme->iType() || 8 == m_pLexeme->iType())
                {
                    CGramHasher pastF (POS_VERB, SUBPARADIGM_PAST_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_F,
                                       PERSON_UNDEFINED, ANIM_UNDEFINED, m_pLexeme->eAspect(), 
                                       m_pLexeme->eIsReflexive());
                    CWordForm * pPastF = NULL;
                    int iNForms = m_pLexeme->iFormCount (pastF.sGramHash());
                    for (int iF = 0; iF < iNForms; ++iF)
                    {
                        rc = m_pLexeme->eWordFormFromHash (pastF.sGramHash(), iF, pPastF);
                        if (rc != H_NO_ERROR)
                        {
                            return rc;
                        }
                        if (NULL == pPastF)
                        {
                            ASSERT(0);
                            ERROR_LOG (L"Failed to obtain template form.");
                            return H_ERROR_POINTER;
                        }

                        map<int, ET_StressType>::iterator itStressPos = pPastF->m_mapStress.begin();
                        for (; pPastF->m_mapStress.end() != itStressPos; ++itStressPos)
                        {
                            if (STRESS_PRIMARY == (*itStressPos).second)
                            {
                                vecPositions.push_back ((*itStressPos).first);
                            }
                        }
                    }       //  for (int iF = 0; iF < iNForms; ++iF)
                }

                if (3 == m_pLexeme->iType() || 10 == m_pLexeme->iType())        // NB also 3 + circle
                {
                    int iStressPos = -1;
                    rc = m_pLexeme->eGetFirstStemStressPos(iStressPos);
//                    while (H_NO_ERROR == rc)      // ???
                    if (H_NO_ERROR == rc)
                    {
                        if (iStressPos < 0)
                        {
                            ASSERT(0);
                            CEString sMsg (L"Illegal stress position.");
                            ERROR_LOG (sMsg);
                            return H_ERROR_UNEXPECTED;
                        }

                        int iNSyll = m_pLexeme->sSourceForm().uiNSyllables();
                        if (iNSyll > 1)
                        {
                            if (iStressPos == iNSyll - 1)
                            {
                                vecPositions.push_back (iNSyll - 2);
                            }
                            else
                            {
                                vecPositions.push_back (iStressPos);
                            }
                        }
                        else
                        {
                            vecPositions.push_back (iStressPos);
                        }
                    }
                }

                if (9 == m_pLexeme->iType() || 11 == m_pLexeme->iType()  || 12 == m_pLexeme->iType() || 
                    14 == m_pLexeme->iType() || 15 == m_pLexeme->iType() || 16 == m_pLexeme->iType())
                {
                    CGramHasher pastM (POS_VERB, SUBPARADIGM_PAST_TENSE, CASE_UNDEFINED, NUM_SG, GENDER_M, 
                                       PERSON_UNDEFINED, ANIM_UNDEFINED, m_pLexeme->eAspect(), 
                                       m_pLexeme->eIsReflexive());
                    CWordForm * pPastM = NULL;
                    int iNForms = m_pLexeme->iFormCount (pastM.sGramHash());
                    for (int iF = 0; iF < iNForms; ++iF)
                    {
                        rc = m_pLexeme->eWordFormFromHash (pastM.sGramHash(), iF, pPastM);
                        if (rc != H_NO_ERROR)
                        {
                            return rc;
                        }
                        if (NULL == pPastM)
                        {
                            ASSERT(0);
                            ERROR_LOG (L"Failed to obtain template form.");
                            return H_ERROR_POINTER;
                        }

                        map<int, ET_StressType>::iterator itStressPos = pPastM->m_mapStress.begin();
                        for (; pPastM->m_mapStress.end() != itStressPos; ++itStressPos)
                        {
                            if (STRESS_PRIMARY == (*itStressPos).second) // true == primary stress
                            {
                                vecPositions.push_back ((*itStressPos).first);
                            }
                        }
                    }       //  for (int iF = 0; iF < iNForms; ++iF)
                }
                break;

            }   //  case SUBPARADIGM_PART_PAST_PASS
     
            default:
            {
                       ASSERT(0);
                ERROR_LOG (L"Bad subparadigm.");
                return H_ERROR_INVALID_ARG;
            }
        }       // switch

        if (SUBPARADIGM_PART_PAST_PASS_LONG != eSubparadigm)
        {
            CWordForm * pStressTemplate = NULL;
            rc = m_pLexeme->eWordFormFromHash (sHash, 0, pStressTemplate);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }
            if (NULL == pStressTemplate)
            {
                ASSERT(0);
                ERROR_LOG (L"Failed to obtain template form.");
                return H_ERROR_POINTER;
            }
            map<int, ET_StressType>::iterator itStressPos = pStressTemplate->m_mapStress.begin();

            // Skip secondary stress if any
            for (; pStressTemplate->m_mapStress.end() != itStressPos; ++itStressPos)
            {
                if (STRESS_PRIMARY == (*itStressPos).second)
                {
                    vecPositions.push_back ((*itStressPos).first);
                }
            }

            if (vecPositions.empty())
            {
                ASSERT(0);
                ERROR_LOG (L"Can't find main stress.");
                return H_ERROR_GENERAL;
            }
        }
    }
    catch (CException& ex)
    {
        CEString sMsg (L"Exception: ");
        sMsg += ex.szGetDescription();
        ERROR_LOG(sMsg);
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}   //  eGetParticipleStressPos (...)

int CFormBuilderNonFinite::iStressPosFromTemplate (const CWordForm& wfTemplate, const CEString& sStem, int iAt)
{
    map<int, ET_StressType>::const_iterator itStressPos = wfTemplate.m_mapStress.begin();
    int iMainStress = 0;
    for (; itStressPos != wfTemplate.m_mapStress.end(); ++itStressPos)
    {
        if (STRESS_PRIMARY == (*itStressPos).second)     // main stress == true
        {
            if (iMainStress == iAt)
            {
                break;
            }
            ++iMainStress;
        }
    }

    if (wfTemplate.m_mapStress.end() == itStressPos)
    {
        ASSERT(0);
        CEString sMsg (L"Can't find main stress.");
        ERROR_LOG (sMsg);
        throw CException (E_FAIL, sMsg);
    }

    return (*itStressPos).first;

}   //  i_StressPosFromTemplate (...)

//
// Irregular forms
//
ET_ReturnCode CFormBuilderNonFinite::eDeriveIrregPresAdverbial()
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    try
    {
        CWordForm * pWordForm = new CWordForm;
        if (NULL == pWordForm)
        {
            ASSERT(0);
            ERROR_LOG (L"Unable to instantiate CWordForm.");
            return H_ERROR_POINTER;
        }

        pWordForm->m_pLexeme = m_pLexeme;
        pWordForm->m_ePos = POS_VERB;
        pWordForm->m_eSubparadigm = SUBPARADIGM_ADVERBIAL_PRESENT;
        pWordForm->m_eAspect = m_pLexeme->eAspect();
        pWordForm->m_eReflexive = m_pLexeme->eIsReflexive();
        pWordForm->m_bIrregular = true;

        //rc = eAssignSecondaryStress (pWordForm);
        //if (rc != H_NO_ERROR)
        //{
        //    return rc;
        //}
        
        CGramHasher pl3Hash (SUBPARADIGM_PRESENT_TENSE, 
                             NUM_PL, 
                             GENDER_UNDEFINED, 
                             PERSON_3, 
                             ANIM_UNDEFINED,
                             m_pLexeme->eAspect(),
                             CASE_UNDEFINED, 
                             m_pLexeme->eIsReflexive());
        vector<CWordForm *> vecPl3Forms;
        
        rc = eGetIrregularForms(pl3Hash.sGramHash(), vecPl3Forms);
        if (rc != H_NO_ERROR)
        {
            return rc;
        }

        if (vecPl3Forms.empty())
        {
            return H_ERROR_UNEXPECTED;
        }

        CGramHasher sg1Hash (SUBPARADIGM_PRESENT_TENSE, 
                             NUM_SG, 
                             GENDER_UNDEFINED, 
                             PERSON_1, 
                             ANIM_UNDEFINED,
                             m_pLexeme->eAspect(),
                             CASE_UNDEFINED, 
                             m_pLexeme->eIsReflexive());

        vector<CWordForm *> vecSg1Forms;
        rc = eGetIrregularForms(sg1Hash.sGramHash(), vecSg1Forms);
        if (rc != H_NO_ERROR)
        {
            return rc;
        }
        if (vecSg1Forms.empty())
        {
            return H_ERROR_UNEXPECTED;
        }

        vector<CWordForm *>::iterator it3PlForm = vecPl3Forms.begin();
        for (; it3PlForm != vecPl3Forms.end(); ++it3PlForm)
        {
            CEString s3Pl ((*it3PlForm)->m_sWordForm);
            if (REFL_YES == m_pLexeme->eIsReflexive())
            {
                s3Pl.sErase (s3Pl.uiLength()-2);
            }

            if (!s3Pl.bEndsWith (L"ют") && !s3Pl.bEndsWith (L"ут") && !s3Pl.bEndsWith (L"ят") && 
                !s3Pl.bEndsWith (L"ат"))
            {
                ASSERT(0);
                CEString sMsg (L"Unexpected 3 Pl ending: ");
                sMsg += s3Pl;
                ERROR_LOG (sMsg);
                return H_ERROR_UNEXPECTED;
            }

            CEString sStem(s3Pl);
            sStem.sErase (sStem.uiLength()-2);
            pWordForm->m_sStem = sStem;
            if (sStem.bEndsWithOneOf (L"шжчщц"))
            {
                pWordForm->m_sWordForm = sStem + L"а";
            }
            else
            {
                pWordForm->m_sWordForm = sStem + L"я";
            }

            if (REFL_YES == m_pLexeme->eIsReflexive())
            {
                pWordForm->m_sWordForm += L"сь";
            }

            vector<CWordForm *>::iterator itSg1Form = vecSg1Forms.begin();
            pWordForm->m_mapStress = (*itSg1Form)->m_mapStress;
            m_pLexeme->AddWordForm (pWordForm);
            ++itSg1Form;

            // Will the ever happen??
            for (; itSg1Form != vecSg1Forms.end(); ++itSg1Form)
            {
                CWordForm * pNewWordForm = NULL;
                CloneWordForm(pWordForm, pNewWordForm);
                pWordForm->m_mapStress = (*itSg1Form)->m_mapStress;
                m_pLexeme->AddWordForm (pWordForm);
            }
        }
    }
    catch (CException& ex)
    {
        CEString sMsg(L"Exception: ");
        sMsg += ex.szGetDescription();
        ERROR_LOG(sMsg);
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}   //  eDeriveIrregPresAdverbial()

//
// "Abbreviated notation", GDRL pp. 88ff
//
ET_ReturnCode CFormBuilderNonFinite::eDeriveIrregPresActiveParticiple()
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    CGramHasher pl3Hash (SUBPARADIGM_PRESENT_TENSE, 
                         NUM_PL, 
                         GENDER_UNDEFINED, 
                         PERSON_3, 
                         ANIM_UNDEFINED,
                         m_pLexeme->eAspect(),
                         CASE_UNDEFINED, 
                         m_pLexeme->eIsReflexive());
    map<CWordForm *, bool> map3PlIrreg;
    rc = m_pLexeme->eGetIrregularForms(pl3Hash.sGramHash(), map3PlIrreg);
    if (rc != H_NO_ERROR)
    {
        return rc;
    }

    if (map3PlIrreg.empty())
    {
        ASSERT(0);
        ERROR_LOG (L"Irregular 3 Pl form not found in database.");
        return H_ERROR_UNEXPECTED;
    }

    map<CWordForm *, bool>::iterator itWf = map3PlIrreg.begin();
    for (; itWf != map3PlIrreg.end(); ++itWf)
    {
        CEString sStem(itWf->first->m_sWordForm);
        if (REFL_YES == m_pLexeme->eIsReflexive())
        {
            sStem.sErase (sStem.uiLength()-2);
        }

        if (!sStem.bEndsWith (L"т"))
        {
            ASSERT(0);
            CEString sMsg (L"Unexpected 3 Pl ending: ");
            sMsg += itWf->first->m_sWordForm;
            ERROR_LOG (sMsg);
            return H_ERROR_UNEXPECTED;
        }

        sStem.sErase (sStem.uiLength()-1);
        
        vector<int> vecStressPos;
        if (sStem.bEndsWith (L"у") || sStem.bEndsWith (L"ю"))
        {
            map<int, ET_StressType>::iterator itStressPos = itWf->first->m_mapStress.begin();
            for (; itStressPos != itWf->first->m_mapStress.end(); ++itStressPos)
            {
                if (STRESS_PRIMARY == (*itStressPos).second)
                {
                    vecStressPos.push_back ((*itStressPos).first);
                }
            }
        }
        else if (sStem.bEndsWith (L"а") || sStem.bEndsWith (L"я"))
        {
            CGramHasher sg1Hash (SUBPARADIGM_PRESENT_TENSE, NUM_SG, GENDER_UNDEFINED, PERSON_1, 
                                 ANIM_UNDEFINED, m_pLexeme->eAspect(), CASE_UNDEFINED, 
                                 m_pLexeme->eIsReflexive());

            map<CWordForm *, bool> mapSg1Irreg;
            rc = m_pLexeme->eGetIrregularForms(sg1Hash.sGramHash(), mapSg1Irreg);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }

            if (mapSg1Irreg.empty())
            {
                ASSERT(0);
                ERROR_LOG (L"Irregular 1 Sg form not found in database.");
                return H_ERROR_UNEXPECTED;
            }

            map<CWordForm *, bool>::iterator it = mapSg1Irreg.begin();
            for (; it != mapSg1Irreg.end(); ++it)
            {
                map<int, ET_StressType>::iterator itStressPos = (*it).first->m_mapStress.begin();
                for (; itStressPos != (*it).first->m_mapStress.end(); ++itStressPos)
                {
                    if (STRESS_PRIMARY == (*itStressPos).second)
                    {
                        vecStressPos.push_back ((*itStressPos).first);
                    }
                }
            }
        }
        else
        {
            ASSERT(0);
            CEString sMsg (L"Unexpected stem auslaut: ");
            sMsg += itWf->first->m_sWordForm;
            ERROR_LOG (sMsg);
            return H_ERROR_UNEXPECTED;
        }
        
        sStem += L'щ';

        ET_Subparadigm eSp (SUBPARADIGM_PART_PRES_ACT);
        vector<int>::iterator itStress = vecStressPos.begin();
        for (; itStress != vecStressPos.end(); ++itStress)
        {
            CFormBuilderLongAdj builder (m_pLexeme, sStem, AT_A, eSp, *itStress);
            rc = builder.eBuildParticiple();
        }
    }
    return rc;

}   //  eDeriveIrregPresActiveParticiple()

//
// If present tense is irregular
//
ET_ReturnCode CFormBuilderNonFinite::eDeriveIrregPresPassiveParticiple()
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    CGramHasher sg3Hash (SUBPARADIGM_PRESENT_TENSE, NUM_SG, GENDER_UNDEFINED, PERSON_3, 
                         ANIM_UNDEFINED, m_pLexeme->eAspect(), CASE_UNDEFINED, m_pLexeme->eIsReflexive());

    map<CWordForm *, bool> mapSg3Irreg;
    rc = m_pLexeme->eGetIrregularForms(sg3Hash.sGramHash(), mapSg3Irreg);
    if (rc != H_NO_ERROR)
    {
        return rc;
    }

    if (mapSg3Irreg.empty())
    {
        ASSERT(0);
        ERROR_LOG (L"Irregular 3 Sg form not found in database.");
        return H_ERROR_UNEXPECTED;
    }

    map<CWordForm *, bool>::iterator it = mapSg3Irreg.begin();
    for (; it != mapSg3Irreg.end(); ++it)
    {
        CEString sStem((*it).first->m_sWordForm);
        if (!sStem.bEndsWith (L"т"))
        {
            ASSERT(0);
            CEString sMsg (L"Unexpected 3 Sg ending: ");
            sMsg += sStem;
            ERROR_LOG (sMsg);
            return H_ERROR_UNEXPECTED;
        }

        sStem.sErase (sStem.uiLength()-1);

        bool bExists = false;
        if (sStem.bEndsWith (L"и"))
        {
            bExists = true;
            break;
        }
        else
        {
            if (sStem.bEndsWith (L"е"))
            {
                if (sStem.uiLength() < 2)
                {
                    ASSERT(0);
                    CEString sMsg (L"Unexpected 3 Sg ending: ");
                    sMsg += sStem;
                    ERROR_LOG (sMsg);
                    return H_ERROR_UNEXPECTED;
                }
            
                // vowel + unstressed -e[t]
                wchar_t chr_penult = sStem[sStem.uiLength()-2];
                if (CEString::bIn (chr_penult, g_szRusVowels))
                {
                    int iLastStemSyll = sStem.uiGetSyllableFromVowelPos (sStem.uiLength()-1);
                    map<int, ET_StressType>::iterator itAt = (*it).first->m_mapStress.find (iLastStemSyll);
                    if ((*it).first->m_mapStress.end() == itAt)
                    {
                        bExists = true;
                    }
                    else if ((*itAt).second != STRESS_PRIMARY)
                    {
                        bExists = true;
                        break;
                    }
                }
            }   // -e- verb
        }

        if (!bExists)
        {
            return H_NO_MORE;
        }

//        StIrregularForm st1Sg;
//        hr = hGet1SgIrregular (st1Sg);
//        if (H_NO_ERROR != hr)
//        {
//            return hr;
//        }        
    }   //  for (; it != mapSg3Irreg.end(); ++it)

    CGramHasher hasher (POS_VERB, SUBPARADIGM_PRESENT_TENSE, CASE_UNDEFINED, NUM_PL, GENDER_UNDEFINED, 
                        PERSON_1, ANIM_UNDEFINED, m_pLexeme->eAspect(), m_pLexeme->eIsReflexive());
    CWordForm * p1Pl = NULL;
    auto n1Pl = m_pLexeme->iFormCount(hasher.sGramHash());
    if (n1Pl < 1)
    {
        ASSERT(0);
        ERROR_LOG(L"Failed to obtain 1st person plural form count.");
        return H_ERROR_UNEXPECTED;
    }

    for (auto nSourceForm = 0; nSourceForm < n1Pl; ++nSourceForm)
    {
        rc = m_pLexeme->eWordFormFromHash(hasher.sGramHash(), nSourceForm, p1Pl);
        if (rc != H_NO_ERROR)
        {
            return rc;
        }

        if (NULL == p1Pl)
        {
            ASSERT(0);
            ERROR_LOG(L"Failed to obtain 1st person plural form.");
            return H_ERROR_POINTER;
        }

        ET_Subparadigm eSp(SUBPARADIGM_PART_PRES_PASS_LONG);

        CEString s1Pl(p1Pl->m_sWordForm);
        if (s1Pl.bEndsWith(L"ем"))
        {
            map<int, ET_StressType>::iterator itStress = p1Pl->m_mapStress.begin();
            for (; itStress != p1Pl->m_mapStress.end(); ++itStress)
            {
                if (STRESS_PRIMARY == (*itStress).second)
                {
                    CFormBuilderLongAdj builder(m_pLexeme, s1Pl, AT_A, eSp, (*itStress).first);
                    rc = builder.eBuildParticiple();
                    if (rc != H_NO_ERROR)
                    {
                        return rc;
                    }
                }
            }
        }
        else if (s1Pl.bEndsWith(L"им"))
        {
            CGramHasher sg1Hash(SUBPARADIGM_PRESENT_TENSE, NUM_SG, GENDER_UNDEFINED, PERSON_1,
                ANIM_UNDEFINED, m_pLexeme->eAspect(), CASE_UNDEFINED,
                m_pLexeme->eIsReflexive());

            map<CWordForm *, bool> mapSg1;
            rc = m_pLexeme->eGetIrregularForms(sg1Hash.sGramHash(), mapSg1);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }

            if (mapSg1.empty())
            {
                ASSERT(0);
                CEString sMsg(L"No irregular 1 Sg forms: ");
                sMsg += m_pLexeme->sSourceForm();
                ERROR_LOG(sMsg);
                return H_ERROR_UNEXPECTED;
            }

            map<CWordForm *, bool>::iterator it = mapSg1.begin();
            for (; it != mapSg1.end(); ++it)
            {
                map<int, ET_StressType>::iterator itStress = (*it).first->m_mapStress.begin();
                for (; itStress != (*it).first->m_mapStress.end(); ++itStress)
                {
                    if (STRESS_PRIMARY == (*itStress).second)
                    {
                        CFormBuilderLongAdj builder(m_pLexeme, s1Pl, AT_A, eSp, (*itStress).first);
                        rc = builder.eBuildParticiple();
                        if (rc != H_NO_ERROR)
                        {
                            return rc;
                        }
                    }
                }
            }
        }       // "им"
        else
        {
            ASSERT(0);
            CEString sMsg(L"Unexpected 1 Pl ending: ");
            sMsg += s1Pl;
            ERROR_LOG(sMsg);
            return H_ERROR_UNEXPECTED;
        }

        s1Pl.sErase(s1Pl.uiLength() - 2);       // ???
    }
    
    return rc;

}   //  eDeriveIrregPresPassiveParticiple()

ET_ReturnCode CFormBuilderNonFinite::eDeriveIrregPastActPartAndAdverbial()
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    if (!bHasIrregularPast())
    {
        ASSERT(0);
        CEString sMsg (L"Irregular past tense expected.");
        ERROR_LOG (sMsg);
        return H_ERROR_UNEXPECTED;
    }

    // Derive from past tense
    CGramHasher mSgHash (SUBPARADIGM_PAST_TENSE, NUM_SG, GENDER_M, PERSON_UNDEFINED, ANIM_UNDEFINED,
                         m_pLexeme->eAspect(), CASE_UNDEFINED, m_pLexeme->eIsReflexive());

    map<CWordForm *, bool> mapMSgIrreg;
    rc = m_pLexeme->eGetIrregularForms(mSgHash.sGramHash(), mapMSgIrreg);
    if (rc != H_NO_ERROR)
    {
        return rc;
    }

    if (mapMSgIrreg.empty())
    {
        ASSERT(0);
        CEString sMsg (L"Unable to acquire M Sg Past of .");
        sMsg += m_pLexeme->sSourceForm();
        ERROR_LOG (sMsg);
        return H_ERROR_UNEXPECTED;
    }

    map<CWordForm *, bool>::iterator it = mapMSgIrreg.begin();
    for (; it != mapMSgIrreg.end(); ++it)
    {
        CEString sStem = it->first->m_sWordForm;
        CEString sSuffix;

        int iCharsToRemove = 0;
        if (REFL_YES == m_pLexeme->eIsReflexive())
        {
            if (sStem.bEndsWith (L"лся"))
            {
                iCharsToRemove = 3;
                sSuffix = L"вш";
            }
            else
            {
                iCharsToRemove = 2;
                sSuffix = L"ш";
            }
        }
        else
        {
            if (sStem.bEndsWith (L"л"))
            {
                iCharsToRemove = 1;
                sSuffix = L"вш";
            }
            else
            {
                iCharsToRemove = 0;
                sSuffix = L"ш";
            }
        }

        sStem.sErase (sStem.uiLength() - iCharsToRemove);
        sStem += sSuffix;
    
        map<int, ET_StressType>::iterator itStress = (*it).first->m_mapStress.begin();
        for (; itStress != (*it).first->m_mapStress.end(); ++itStress)
        {
            if (STRESS_PRIMARY == (*itStress).second)
            {
                CFormBuilderLongAdj builder (m_pLexeme, 
                                             sStem, 
                                             AT_A, 
                                             SUBPARADIGM_PART_PAST_ACT, 
                                             (*itStress).first);
                builder.eBuildParticiple();
                if (rc != H_NO_ERROR)
                {
                    return rc;
                }

                CWordForm * pAdverbial = new CWordForm;
                if (NULL == pAdverbial)
                {
                    ASSERT(0);
                    ERROR_LOG (L"Unable to instantiate CWordForm.");
                    return H_ERROR_POINTER;
                }

                if (L"св" == m_pLexeme->sMainSymbol() && m_pLexeme->bHasCommonDeviation (9))
                {
                    pAdverbial->m_eStatus = STATUS_OBSOLETE;
                }

                pAdverbial->m_pLexeme = m_pLexeme;
                pAdverbial->m_ePos = POS_VERB;
                pAdverbial->m_eSubparadigm = SUBPARADIGM_ADVERBIAL_PAST;
                pAdverbial->m_eReflexive = m_pLexeme->eIsReflexive();
                //rc = eAssignSecondaryStress (pAdverbial);
                //if (rc != H_NO_ERROR)
                //{
                //    return rc;
                //}

                pAdverbial->m_sStem = sStem;
//              pAdverbial->m_sWordForm = sStem.c_str() + sSuffix;
                pAdverbial->m_sWordForm = sStem;
                pAdverbial->m_sWordForm += L"и";
                if (REFL_YES == pAdverbial->m_eReflexive)
                {
                    pAdverbial->m_sWordForm += L"сь";
                }

                if (m_pLexeme->bHasCommonDeviation(9))
                {
                    pAdverbial->m_eStatus = STATUS_OBSOLETE;
                }
                pAdverbial->m_mapStress [(*itStress).first] = STRESS_PRIMARY;

                try
                {
                    m_pLexeme->AddWordForm(pAdverbial);
                }
                catch (CException& ex)
                {
                    ERROR_LOG(ex.szGetDescription());
                    return H_EXCEPTION;
                }

                if (L"вш" == sSuffix)
                {
                    CWordForm * pAdverbial2 = NULL;
                    CloneWordForm (pAdverbial, pAdverbial2);
                    pAdverbial2->m_sWordForm = pAdverbial->m_sWordForm;
                    pAdverbial2->m_sWordForm.sErase (pAdverbial->m_sWordForm.uiLength()-2);
                    if (REFL_YES == pAdverbial2->m_eReflexive)
                    {
                        pAdverbial->m_sWordForm += L"ся";
                    }

                    try
                    {
                        m_pLexeme->AddWordForm(pAdverbial2);
                    }
                    catch (CException& ex)
                    {
                        ERROR_LOG(ex.szGetDescription());
                        return H_EXCEPTION;
                    }
                }

                if (m_pLexeme->bHasCommonDeviation(9))
                {
                    if (L"св" != m_pLexeme->sMainSymbol())
                    {
                        ASSERT(0);
                        CEString sMsg(L"Non-perfective verb with CD-9");
                        ERROR_LOG(sMsg);
                        throw CException(H_ERROR_UNEXPECTED, sMsg);
                    }

                    CGramHasher hasher(POS_VERB, SUBPARADIGM_ADVERBIAL_PRESENT, CASE_UNDEFINED,
                        NUM_UNDEFINED, GENDER_UNDEFINED, PERSON_UNDEFINED,
                        ANIM_UNDEFINED, m_pLexeme->eAspect(), m_pLexeme->eIsReflexive());

                    CWordForm * pPresAdv = NULL;

                    auto nPresAdv = m_pLexeme->iFormCount(hasher.sGramHash());
                    if (nPresAdv < 1)
                    {
                        ASSERT(0);
                        ERROR_LOG(L"Failed to obtain template form count.");
                        return H_ERROR_UNEXPECTED;
                    }

                    for (auto nSourceForm = 0; nSourceForm < nPresAdv; ++nSourceForm)
                    {
                        rc = m_pLexeme->eWordFormFromHash(hasher.sGramHash(), nPresAdv, pPresAdv);
                        if (rc != H_NO_ERROR)
                        {
                            return rc;
                        }
                        if (NULL == pPresAdv)
                        {
                            ASSERT(0);
                            ERROR_LOG(L"Failed to obtain template form.");
                            return H_ERROR_POINTER;
                        }

                        pPresAdv->m_eSubparadigm = SUBPARADIGM_ADVERBIAL_PAST;
                        CWordForm * pCopy = NULL;
                        CloneWordForm(pPresAdv, pCopy);
                        try
                        {
                            m_pLexeme->AddWordForm(pCopy);
                        }
                        catch (CException& ex)
                        {
                            ERROR_LOG(ex.szGetDescription());
                            return H_EXCEPTION;
                        }
                    }       //  for (auto nSourceForm = 0; nSourceForm < nPresAdv; ++nSourceForm)
                }       //  if (m_pLexeme->bHasCommonDeviation(9))
            }       //  if (STRESS_PRIMARY == (*itStress).second)
        }       // stress pos. loop

    }   //  for (; it != mapMSgIrreg.end(); ++it)...

    return H_NO_ERROR;

}   //  eDeriveIrregPastActPartAndAdverbial()

ET_ReturnCode CFormBuilderNonFinite::eBuildIrregParticipialFormsLong (ET_Subparadigm eSp)   
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    if (SUBPARADIGM_PART_PRES_ACT != eSp && SUBPARADIGM_PART_PRES_PASS_LONG  != eSp && 
        SUBPARADIGM_PART_PAST_ACT != eSp && SUBPARADIGM_PART_PAST_PASS_LONG != eSp)
    {
        ASSERT(0);
        return H_ERROR_UNEXPECTED;
    }

    CGramHasher irreg (POS_VERB, eSp, CASE_NOM, NUM_SG, GENDER_M, PERSON_UNDEFINED, 
                       ANIM_UNDEFINED, m_pLexeme->eAspect(), m_pLexeme->eIsReflexive());
    
    map<CWordForm *, bool> mapIrregForms;
    rc = m_pLexeme->eGetIrregularForms(irreg.sGramHash(), mapIrregForms);
    if (rc != H_NO_ERROR)
    {
        return rc;
    }

    if (mapIrregForms.empty())
    {
        return H_NO_MORE;
    }

    map<CWordForm *, bool>::iterator it = mapIrregForms.begin();
    for (; it != mapIrregForms.end(); ++it)
    {
        CWordForm * pWf = (*it).first;
        CEString sWordForm = (*it).first->m_sWordForm;
        unsigned int uiCharsToRemove = (REFL_YES == m_pLexeme->eIsReflexive()) ? 4 : 2;
        if (sWordForm.uiLength() <= uiCharsToRemove)
        {
            ASSERT(0);
            CEString sMsg (L"Irregular past active participle is too short: ");
            sMsg += sWordForm;
            ERROR_LOG (sMsg);
            return H_ERROR_UNEXPECTED;
        }

        CEString sStem = sWordForm.sErase (sWordForm.uiLength()-uiCharsToRemove, uiCharsToRemove);
        std::map<int, ET_StressType>::iterator itStressPos = pWf->m_mapStress.begin();
        for (; itStressPos != pWf->m_mapStress.end(); ++itStressPos)
        {
            if (STRESS_PRIMARY == itStressPos->second)
            {
                CFormBuilderLongAdj builder (m_pLexeme, 
                                             sStem,
                                             AT_A, 
                                             eSp, 
                                             itStressPos->first,
                                             STATUS_COMMON,    // don't know how to determine status
                                             true);    // irregular
                rc = builder.eBuildParticiple();
            }
        }
    
    }   //  for (; it != mapIrregForms.end(); ++it)

    return rc;

}       //  eBuildIrregParticipialFormsLong

ET_ReturnCode CFormBuilderNonFinite::eDeriveIrregPartPresPassShort (ET_Status eStatus)
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    try
    {
        CWordForm * pSource = NULL;
        CEString sStem;

        CGramHasher nSgLong (POS_VERB, SUBPARADIGM_PART_PRES_PASS_LONG, CASE_NOM, NUM_SG, 
                                  GENDER_M, PERSON_UNDEFINED, ANIM_UNDEFINED, m_pLexeme->eAspect(), 
                                  m_pLexeme->eIsReflexive());

        auto nNSgLong = m_pLexeme->iFormCount(nSgLong.sGramHash());
        if (nNSgLong < 1)
        {
            ASSERT(0);
            ERROR_LOG(L"Failed to obtain long form count.");
            return H_ERROR_UNEXPECTED;
        }

        for (auto nSourceForm = 0; nSourceForm < nNSgLong; ++nSourceForm)
        {
            rc = m_pLexeme->eWordFormFromHash(nSgLong.sGramHash(), nSourceForm, pSource);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }
            if (NULL == pSource)
            {
                ASSERT(0);
                ERROR_LOG(L"Failed to obtain long form.");
                return H_ERROR_POINTER;
            }

            sStem = pSource->m_sWordForm;
            if (sStem.uiLength() <= 2)
            {
                ASSERT(0);
                CEString sMsg(L"Long form not long enough.");
                ERROR_LOG(sMsg);
                return H_ERROR_UNEXPECTED;
            }

            sStem.sErase(sStem.uiLength() - 2, 2);

            bool bYoAlternation = false;
            bool bFleetingVowel = false;
            vector<int> vecStressPos;
            map<int, ET_StressType>::iterator itStressPos = pSource->m_mapStress.begin();
            for (; pSource->m_mapStress.end() != itStressPos; ++itStressPos)
            {
                if (STRESS_PRIMARY == (*itStressPos).second)
                {
                    vecStressPos.push_back((*itStressPos).first);
                }
            }
            CFormBuilderShortAdj shortAdj(m_pLexeme,
                bYoAlternation,
                sStem,
                SUBPARADIGM_PART_PRES_PASS_SHORT,
                AT_A,
                AT_A,
                vecStressPos,
                bFleetingVowel);
            shortAdj.SetUsageStatus(eStatus);   //  where  can we get this info for irregular forms?
            rc = shortAdj.eBuild();
        
        }       //  for (auto nSourceForm = 0; nSourceForm < nNSgLong; ++nSourceForm)
    }
    catch (CException& ex)
    {
        CEString sMsg (L"Exception: ");
        sMsg += ex.szGetDescription();
        ERROR_LOG (sMsg);
        return H_EXCEPTION;
    }

    return rc;

}   //  eDeriveIrregPartPresPassShort()

//
// Irregular short forms (as opposed to derived from irregular long)
//
ET_ReturnCode CFormBuilderNonFinite::eBuildIrregPartPastPassShort (bool& bIsVariant)
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    try
    {
        for (ET_Gender eGender = GENDER_UNDEFINED; eGender <= GENDER_N; ++eGender)
        {
            ET_Number eNumber = (GENDER_UNDEFINED == eGender) ? NUM_PL : NUM_SG;

            CWordForm * pWordForm = new CWordForm;
            if (NULL == pWordForm)
            {
                ASSERT(0);
                CEString sMsg(L"Unable to instantiate CWordForm.");
                ERROR_LOG (L"Unable to instantiate CWordForm.");
                return H_ERROR_POINTER;
            }

            pWordForm->m_pLexeme = m_pLexeme;
            pWordForm->m_ePos = m_pLexeme->ePartOfSpeech();
            pWordForm->m_eSubparadigm = SUBPARADIGM_PART_PAST_PASS_SHORT;
            pWordForm->m_eAspect = m_pLexeme->eAspect();
            pWordForm->m_eReflexive = m_pLexeme->eIsReflexive();
            pWordForm->m_eGender = eGender;
            pWordForm->m_eNumber = eNumber;

            m_pLexeme->AddWordForm(pWordForm);
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

}   //  eBuildIrregPartPastPassShort (...)

ET_ReturnCode CFormBuilderNonFinite::eDeriveIrregPartPastPassShort()
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    ET_ReturnCode rc = H_NO_ERROR;

    try
    {
        //
        // Part. pass long must be available
        //
        CGramHasher nSgLong (POS_VERB, SUBPARADIGM_PART_PAST_PASS_LONG, CASE_NOM, NUM_SG, GENDER_M, 
                             PERSON_UNDEFINED, ANIM_UNDEFINED, m_pLexeme->eAspect(), 
                             m_pLexeme->eIsReflexive());

        auto nPartPassLong = m_pLexeme->iFormCount(nSgLong.sGramHash());
        if (nPartPassLong < 1)
        {
            ASSERT(0);
            CEString sMsg(L"No irregular long forms.");
            ERROR_LOG(sMsg);
            return H_ERROR_POINTER;
        }

        for (auto nLongForm = 0; nLongForm < nPartPassLong; ++nLongForm)
        {
            CWordForm * pNSgLong = NULL;
            rc = m_pLexeme->eWordFormFromHash(nSgLong.sGramHash(), nLongForm, pNSgLong);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }
            if (NULL == pNSgLong)
            {
                ASSERT(0);
                CEString sMsg(L"Failed to obtain long form.");
                ERROR_LOG(sMsg);
                return H_ERROR_POINTER;
            }

            //
            // Short forms
            //
            map<ET_Gender, CEString> mapEndings;
            mapEndings[GENDER_M] = L"";
            mapEndings[GENDER_F] = L"а";
            mapEndings[GENDER_N] = L"о";
            mapEndings[GENDER_UNDEFINED] = (L"ы");

            ET_StressLocation eStressLocation = STRESS_LOCATION_UNDEFINED;

            CEString sLong(pNSgLong->m_sWordForm);
            CEString sStem;
            if (sLong.bEndsWith(L"енный") ||
                sLong.bEndsWith(L"анный") ||
                sLong.bEndsWith(L"янный"))
            {
                sStem = sLong.sSubstr(0, sLong.uiLength() - 3);
                eStressLocation = STRESS_LOCATION_STEM;
            }
            else if (sLong.bEndsWith(L"ённый"))
            {
                sStem = sLong.sSubstr(0, sLong.uiLength() - 3);
                eStressLocation = STRESS_LOCATION_ENDING;
            }
            else if (sLong.bEndsWith(L"тый"))
            {
                sStem = sLong.sSubstr(0, sLong.uiLength() - 2);
                eStressLocation = STRESS_LOCATION_STEM;
            }
            else
            {
                ASSERT(0);
                CEString sMsg(L"Unable to deduce irregular short forms for ");
                sMsg += sLong;
                ERROR_LOG(sMsg);
                return H_ERROR_GENERAL;
            }

            for (ET_Gender eGender = GENDER_UNDEFINED; eGender <= GENDER_N; ++eGender)
            {
                ET_Number eNumber = (GENDER_UNDEFINED == eGender) ? NUM_PL : NUM_SG;

                CWordForm * pWordForm = new CWordForm;
                if (NULL == pWordForm)
                {
                    ASSERT(0);
                    ERROR_LOG(L"Unable to instantiate CWordForm.");
                    return H_ERROR_POINTER;
                }

                pWordForm->m_pLexeme = m_pLexeme;
                pWordForm->m_ePos = m_pLexeme->ePartOfSpeech();
                pWordForm->m_eSubparadigm = SUBPARADIGM_PART_PAST_PASS_SHORT;
                pWordForm->m_eAspect = m_pLexeme->eAspect();
                pWordForm->m_eReflexive = m_pLexeme->eIsReflexive();
                pWordForm->m_sStem = sStem;
                pWordForm->m_sWordForm = sStem + mapEndings[eGender];
                pWordForm->m_eGender = eGender;
                pWordForm->m_eNumber = eNumber;

                if (STRESS_LOCATION_STEM == eStressLocation)
                {
                    pWordForm->m_mapStress = pNSgLong->m_mapStress;
                }
                else if (STRESS_LOCATION_ENDING == eStressLocation)
                {
                    CEString sWf(pWordForm->m_sWordForm);
                    sWf.SetVowels(g_szRusVowels);
                    int iStressedSyll = sWf.uiNSyllables() - 1;
                    int iStressPos = sWf.uiGetVowelPos(iStressedSyll);
                    pWordForm->m_mapStress[iStressPos] = STRESS_PRIMARY;
                    unsigned int uiYoPos = pWordForm->m_sWordForm.uiFindLastOf(L"ё");
                    if (ecNotFound != uiYoPos)
                    {
                        if (uiYoPos != (unsigned int)iStressPos)
                        {
                            pWordForm->m_sWordForm[uiYoPos] = L'е';
                        }
                    }
                }
                else
                {
                    ASSERT(0);
                    ERROR_LOG(L"Illegal stress type.");
                    return H_ERROR_UNEXPECTED;
                }

                //multimap<int, StIrregularForm>& map_if = m_pLexeme->map_IrregularForms;
                //multimap<int, StIrregularForm>::iterator it_if = map_if.find (pWordForm->i_GramHash());
                //if (map_if.end() != it_if)
                //{
                //    pWordForm->m_mapStress = (*it_if).second.m_mapStress;
                //    pWordForm->m_sWordForm = (*it_if).second.m_sForm;
                //}

                //rc = eAssignSecondaryStress (pWordForm);
                //if (rc != H_NO_ERROR)
                //{
                //    return rc;
                //}

                m_pLexeme->AddWordForm(pWordForm);
            }
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

}   //  eDeriveIrregPartPastPassShort()

bool CFormBuilderNonFinite::bHasIrregPartPastPassShort()
{
    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    if (m_pLexeme->bHasIrregularForms())
    {
        return false;
    }

    CGramHasher mSgHash (SUBPARADIGM_PART_PAST_PASS_SHORT, 
                         NUM_SG, 
                         GENDER_M, 
                         PERSON_UNDEFINED, 
                         ANIM_UNDEFINED,
                         m_pLexeme->eAspect(),
                         CASE_UNDEFINED, 
                         m_pLexeme->eIsReflexive());

    return m_pLexeme->bHasIrregularForm(mSgHash.sGramHash());

}   // bHasIrregPartPastPassShort()
