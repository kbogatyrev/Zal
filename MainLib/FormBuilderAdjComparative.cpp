#include "StdAfx.h"
#include "FormBuilderAdjComparative.h"

HRESULT CFormBuilderComparative::hCreateFormTemplate (const CEString& sLemma, 
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
    pWordForm->m_eSubparadigm = SUBPARADIGM_COMPARATIVE;
    pWordForm->m_sLemma = sLemma;
    AssignSecondaryStress (pWordForm);
                                                                    //  ^-- only secondary   
    return S_OK;

}   //  hCreateFormTemplate (...)

HRESULT CFormBuilderComparative::hHandleStressAndAdd (CComObject<CWordForm> * pWordForm,
                                                      vector<int>& vecStress)
{
    HRESULT hr = S_OK;

    long lGramHash = pWordForm->iGramHash();

    if (1 == vecStress.size() || bMultiStress (pWordForm->m_sLemma, vecStress))
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

    return S_OK;

}   //  hHandleStress (...)

HRESULT CFormBuilderComparative::hBuild()
{
    HRESULT hr = S_OK;

    try
    {
//        CEString& xstr_graphicStem = pLexeme->xstr_GraphicStem;
        CEString sLemma (m_pLexeme->m_sGraphicStem);

        CComObject<CWordForm> * pWordForm = NULL;
        hCreateFormTemplate (sLemma, pWordForm);
        bool bIsVariant = false;
        hr = hHandleIrregularForms (pWordForm, bIsVariant);
        if (FAILED (hr))
        {
            ATLASSERT (0);
            CString csMsg;
            csMsg.Format (L"hHandleIrregularForms() failed, error %x.", hr);
            ERROR_LOG ((LPCTSTR)csMsg);
            return hr;
        }

        if ((S_OK == hr) && !bIsVariant)
        {
            return S_OK;
        }

        if (m_pLexeme->m_sGraphicStem.bEndsWithOneOf (L"кгх"))
        {
            if (m_pLexeme->m_sGraphicStem.bEndsWith (L"к"))
            {
                sLemma[sLemma.uiLength()-1] = L'ч';
            }
            if (m_pLexeme->m_sGraphicStem.bEndsWith (L"г"))
            {
                sLemma[sLemma.uiLength()-1] = L'ж';
            }
            if (m_pLexeme->m_sGraphicStem.bEndsWith (L"х"))
            {
                sLemma[sLemma.uiLength()-1] = L'ш';
            }

//            CComObject<CWordForm> * pWordForm = NULL;
//            h_CreateFormTemplate (str_lemma, pWordForm);
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
            m_pLexeme->hAddWordForm (pWordForm);
        }
        else
        {
//            CComObject<CWordForm> * pWordForm = NULL;
//            h_CreateFormTemplate (str_lemma, pWordForm);
            pWordForm->m_sLemma = sLemma;
            pWordForm->m_sWordForm = sLemma + L"ей";

            vector<int> vecStress;
            if ((AT_A == m_pLexeme->m_eAccentType1) && (AT_UNDEFINED == m_pLexeme->m_eAccentType2))
            {
                // &&&& Never has fleeting vowel???
                hr = hGetStemStressPositions (sLemma, SUBPARADIGM_COMPARATIVE, vecStress);
            }
            else
            {
                int iStress = sLemma.uiNSyllables();
                vecStress.push_back (iStress);
//                vecStress.push_back ((int)str_lemma.length());
            }

            CComObject<CWordForm> * pWordForm1 = NULL;
            hCreateFormTemplate (sLemma, pWordForm1);
            pWordForm1->m_sWordForm = sLemma + L"ей";
            hHandleStressAndAdd (pWordForm1, vecStress);

            CComObject<CWordForm> * pWordForm2 = NULL;
            hCreateFormTemplate (sLemma, pWordForm2);
            pWordForm2->m_sWordForm = sLemma + L"ее";
            hHandleStressAndAdd (pWordForm2, vecStress);

        }
    }
    catch (CException ex)
    {
        return ex.iGetErrorCode();  // logging should be always done by callee
    }

    return S_OK;

}   //  hBuild()
