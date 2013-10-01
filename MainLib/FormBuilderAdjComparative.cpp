#include "StdAfx.h"
#include "FormBuilderAdjComparative.h"

void CFormBuilderComparative::CreateFormTemplate (const CEString& sLemma, CWordForm *& pWordForm)
{
    pWordForm = new CWordForm;
    pWordForm->m_pLexeme = m_pLexeme;
    pWordForm->m_ePos = m_pLexeme->m_ePartOfSpeech;
    pWordForm->m_eSubparadigm = SUBPARADIGM_COMPARATIVE;
    pWordForm->m_sLemma = sLemma;
    AssignSecondaryStress (pWordForm);

}   //  CreateFormTemplate (...)

void CFormBuilderComparative::HandleStressAndAdd (CWordForm * pWordForm, vector<int>& vecStress)
{
    long lGramHash = pWordForm->iGramHash();

    if (1 == vecStress.size() || bMultiStress (pWordForm->m_sLemma, vecStress))
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

}   //  HandleStress (...)

HRESULT CFormBuilderComparative::hBuild()
{
    HRESULT hr = S_OK;

    try
    {
        CEString sLemma (m_pLexeme->m_sGraphicStem);
        CWordForm * pWordForm = NULL;
        CreateFormTemplate (sLemma, pWordForm);
        
        if (m_pLexeme->bNoRegularForms(pWordForm->iGramHash()))
        {
            return S_FALSE;
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

            unsigned int uiLastStemVowel = sLemma.uiFindLastOf (g_szVowels);
            if (ecNotFound != uiLastStemVowel)
            {
                pWordForm->m_mapStress[uiLastStemVowel] = STRESS_PRIMARY;
            }
            else
            {
                ERROR_LOG (L"Warning: can't find stressed vowel in comparative.");
            }

            int iGramHash = pWordForm->iGramHash();
            m_pLexeme->AddWordForm (pWordForm);
        }
        else
        {
            pWordForm->m_sLemma = sLemma;
            pWordForm->m_sWordForm = sLemma + L"ей";

            vector<int> vecStress;
            if ((AT_A == m_pLexeme->m_eAccentType1) && (AT_UNDEFINED == m_pLexeme->m_eAccentType2))
            {
                // &&&& Never has fleeting vowel???
                GetStemStressPositions (sLemma, SUBPARADIGM_COMPARATIVE, vecStress);
            }
            else
            {
                int iStress = sLemma.uiNSyllables();
                vecStress.push_back (iStress);
            }

            CWordForm * pWordForm1 = NULL;
            CreateFormTemplate (sLemma, pWordForm1);
            pWordForm1->m_sWordForm = sLemma + L"ей";
            HandleStressAndAdd (pWordForm1, vecStress);

            CWordForm * pWordForm2 = NULL;
            CreateFormTemplate (sLemma, pWordForm2);
            pWordForm2->m_sWordForm = sLemma + L"ее";
            HandleStressAndAdd (pWordForm2, vecStress);
        }
    }
    catch (CException ex)
    {
        return ex.iGetErrorCode();  // logging should be always done by callee
    }

    return S_OK;

}   //  hBuild()
