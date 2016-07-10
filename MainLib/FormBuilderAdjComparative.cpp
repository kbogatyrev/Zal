#include "StdAfx.h"

#include "WordForm.h"
#include "Lexeme.h"
#include "FormBuilderAdjComparative.h"

using namespace Hlib;

ET_ReturnCode CFormBuilderComparative::eCreateFormTemplate (const CEString& sLemma, CWordForm *& pWordForm)
{
    pWordForm = new CWordForm;
    if (NULL == pWordForm)
    {
        return H_ERROR_POINTER;
    }

    pWordForm->m_pLexeme = m_pLexeme;
    pWordForm->m_ePos = m_pLexeme->ePartOfSpeech();
    pWordForm->m_eSubparadigm = SUBPARADIGM_COMPARATIVE;
    pWordForm->m_sLemma = sLemma;

    return H_NO_ERROR;

}   //  eCreateFormTemplate (...)

ET_ReturnCode CFormBuilderComparative::eHandleStressAndAdd (CWordForm * pWordForm, vector<int>& vecStress)
{
    ET_ReturnCode rc = H_NO_ERROR;

    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    CEString sGramHash = pWordForm->sGramHash();

    try
    {
        if (1 == vecStress.size() || m_pLexeme->bIsMultistressedCompound())
        {
            m_pLexeme->AssignPrimaryStress(pWordForm);
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
    catch (CException& ex)
    {
        CEString sMsg (L"Exception: ");
        sMsg += ex.szGetDescription();
        ERROR_LOG (sMsg);
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}   //  eHandleStressAndAdd (...)

ET_ReturnCode CFormBuilderComparative::eBuild()
{
    ET_ReturnCode rc = H_NO_ERROR;

    ASSERT(m_pLexeme);   // we assume base class ctor took care of this

    try
    {
        CEString sLemma (m_pLexeme->sGraphicStem());
        CWordForm * pWordForm = NULL;
        rc = eCreateFormTemplate (sLemma, pWordForm);
        if (rc != H_NO_ERROR)
        {
            return rc;
        }
        
        if (m_pLexeme->bNoRegularForms(pWordForm->sGramHash()))
        {
            return H_NO_MORE;
        }

        CEString sGraphicStem = m_pLexeme->sGraphicStem();
        if (sGraphicStem.bEndsWithOneOf (L"кгх"))
        {
            if (sGraphicStem.bEndsWith (L"к"))
            {
                sLemma[sLemma.uiLength()-1] = L'ч';
            }
            if (sGraphicStem.bEndsWith (L"г"))
            {
                sLemma[sLemma.uiLength()-1] = L'ж';
            }
            if (sGraphicStem.bEndsWith (L"х"))
            {
                sLemma[sLemma.uiLength()-1] = L'ш';
            }

            pWordForm->m_sLemma = sLemma;
            pWordForm->m_sWordForm = sLemma + L"е";

            unsigned int uiLastStemVowel = sLemma.uiFindLastOf (g_szRusVowels);
            if (ecNotFound != uiLastStemVowel)
            {
                pWordForm->m_mapStress[uiLastStemVowel] = STRESS_PRIMARY;
            }
            else
            {
                ERROR_LOG (L"Warning: can't find stressed vowel in comparative.");
            }

//            int iGramHash = pWordForm->sGramHash();
            m_pLexeme->AddWordForm (pWordForm);
        }
        else
        {
            pWordForm->m_sLemma = sLemma;
            pWordForm->m_sWordForm = sLemma + L"ей";

            vector<int> vecStress;
            if ((AT_A == m_pLexeme->eAccentType1()) && (AT_UNDEFINED == m_pLexeme->eAccentType2()))
            {
                // &&&& Never has fleeting vowel???
                rc = eGetStemStressPositions (sLemma, SUBPARADIGM_COMPARATIVE, vecStress);
                if (rc != H_NO_ERROR)
                {
                    return rc;
                }
            }
            else
            {
                int iStress = sLemma.uiNSyllables();
                vecStress.push_back (iStress);
            }

            CWordForm * pWordForm1 = NULL;
            rc = eCreateFormTemplate (sLemma, pWordForm1);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }

            pWordForm1->m_sWordForm = sLemma + L"ей";
            rc = eHandleStressAndAdd (pWordForm1, vecStress);
            {
                if (rc != H_NO_ERROR)
                {
                    return rc;
                }
            }

            CWordForm * pWordForm2 = NULL;
            rc = eCreateFormTemplate (sLemma, pWordForm2);
            if (rc != H_NO_ERROR)
            {
            }
            pWordForm2->m_sWordForm = sLemma + L"ее";
            rc = eHandleStressAndAdd (pWordForm2, vecStress);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }
        }
    }
    catch (CException ex)
    {
        return H_EXCEPTION;  // logging should be always done by callee
    }

    return H_NO_ERROR;

}   //  eBuild()
