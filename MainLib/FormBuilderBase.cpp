#include "StdAfx.h"

#include "WordForm.h"
#include "Lexeme.h"
#include "FormBuilderBase.h"

using namespace Hlib;

CFormBuilder::CFormBuilder (CLexeme * pLexeme, ET_Subparadigm eSubparadigm) : 
    m_pLexeme (pLexeme), 
    m_eSubparadigm (eSubparadigm), 
    m_pEndings (NULL), 
    m_eStatus (STATUS_COMMON)
{
    if (NULL == pLexeme)
    {
        ASSERT(0);
        CEString sMsg(L"CLexeme ptr is NULL.");
        ERROR_LOG (sMsg);
        throw CException (H_ERROR_UNEXPECTED, sMsg);
    }   
}

void CFormBuilder::ReportDbError()
{
    if (!m_pLexeme)
    {
        ERROR_LOG (L"No lexeme handle.");
    }

    CSqlite * pDb = NULL;
    CEString sMsg;
    try
    {
        pDb = m_pLexeme->pGetDb();
        CEString sError;
        pDb->GetLastError (sError);
        sMsg = L"DB error: ";
        sMsg += sError;
        sMsg += L"; code = ";
        sMsg += CEString::sToString (pDb->iGetLastError());
    }
    catch (...)
    {
        sMsg = L"Apparent DB error ";
    }

    sMsg += CEString::sToString (pDb->iGetLastError());
    ERROR_LOG (sMsg);
}

/*
ET_ReturnCode CFormBuilder::eAssignSecondaryStress (CWordForm * pWf)
{
    try
    {
        CEString& sGrStem = m_pLexeme->m_sGraphicStem;
        vector<int>::iterator itPos = m_pLexeme->m_vecSecondaryStressPos.begin();
        for (; itPos != m_pLexeme->m_vecSecondaryStressPos.end(); ++itPos)
        {
//            unsigned int uiStressedSyll = sGrStem.uiGetSyllableFromVowelPos (*itPos);
            unsigned int uiStressedSyll = (unsigned int)(*itPos); // it _is_ the syllable number, so we don't need that conversion
            pWf->m_mapStress[uiStressedSyll] = STRESS_SECONDARY;
        }
    }
    catch (CException ex)
    {
        ATLASSERT(0);
        CEString sMsg (L"Stressed syllable not found, error: ");
        sMsg += ex.szGetDescription();
        ERROR_LOG (sMsg);
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}       //  eAssignSecondaryStress (...)
*/

/*
void CFormBuilder::AssignSecondaryStress (StIrregularForm& stIf)
{
    CEString& sGrStem = m_pLexeme->m_sGraphicStem;
    vector<int>::iterator itPos = m_pLexeme->m_vecSecondaryStressPos.begin();
    for (; itPos != m_pLexeme->m_vecSecondaryStressPos.end(); ++itPos)
    {
//        int iStressedSyll = sGrStem.uiGetSyllableFromVowelPos (*itPos);
        int iStressedSyll = *itPos; // it _is_ the syllable number, so we don't need that conversion
        if (iStressedSyll < 0)
        {
            ATLASSERT(0);
            CEString sMsg (L"Stressed syllable not found.");
            ERROR_LOG (sMsg);
            throw CException (E_FAIL, sMsg);
        }
        stIf.pWordForm->m_mapStress[iStressedSyll] = STRESS_SECONDARY;
    }
}
*/

//
// GDRL p. 11: a dash in a word with two main stress marks means a compound form
// with two main stresses; otherwise 
//
/*
bool CFormBuilder::bMultiStress (const CEString& sLemma, vector<int>& vecStressPos)
{
    if (vecStressPos.size() < 2)
    {
        return false;
    }

    unsigned int uiDashPos = sLemma.uiFind (L"-");
    if (ecNotFound == uiDashPos || uiDashPos < 1)
    {
        return false;
    }

    // find any two that are separated by dash
    vector<int>::iterator itLeft = vecStressPos.begin();
    unsigned int uiLeftVowelPos = sLemma.uiGetVowelPos (*itLeft);
    if (uiLeftVowelPos < 0)
    {
        ATLASSERT(0);
        CEString sMsg (L"Can't find stressed vowel.");
        ERROR_LOG (sMsg);
        throw CException (E_FAIL, sMsg);
    }
    if (uiLeftVowelPos > uiDashPos)
    {
        return false;
    }

    vector<int>::iterator itRight = itLeft + 1;
    for (; itRight != vecStressPos.end(); ++itRight)
    {
        if (sLemma.uiGetVowelPos (*itRight) > uiDashPos)
        {
            return true;
        }
    }

    return false;

}   //  bMultiStress (...)
*/

/*
ET_ReturnCode CFormBuilder::hCloneWordForm (const CComObject<CWordForm> * pSource, 
                                          CComObject<CWordForm> *& pClone)
{
    ET_ReturnCode hr = H_NO_ERROR;

    hr = CComObject<CWordForm>::CreateInstance (&pClone);
    if (H_NO_ERROR != hr)
    {
        ATLASSERT(0);
        ERROR_LOG (L"CreateInstance failed for CWordForm.");
        return hr;
    }

    pClone->m_pLexeme = pSource->m_pLexeme;
    pClone->m_sWordForm = pSource->m_sWordForm;
    pClone->m_sLemma = pSource->m_sLemma;
    pClone->m_llLexemeId = pSource->m_llLexemeId;
    pClone->m_mapStress = pSource->m_mapStress;
    pClone->m_ePos = pSource->m_ePos;
    pClone->m_eCase = pSource->m_eCase;
    pClone->m_eNumber = pSource->m_eNumber;
    pClone->m_eSubparadigm = pSource->m_eSubparadigm;
    pClone->m_eGender = pSource->m_eGender;
//    pClone->m_eTense = pSource->m_eTense;
    pClone->m_ePerson = pSource->m_ePerson;
    pClone->m_eAnimacy = pSource->m_eAnimacy;
    pClone->m_eReflexive = pSource->m_eReflexive;
//    pClone->m_eVoice = pSource->m_eVoice;
    pClone->m_eStatus = pSource->m_eStatus;

    return H_NO_ERROR;

}  // h_CloneWordForm (...)

ET_ReturnCode CFormBuilder::hCloneWordForm (const  CWordForm * pSource, CComObject<CWordForm> *& pClone)
{
    ET_ReturnCode hr = H_NO_ERROR;

    hr = CComObject<CWordForm>::CreateInstance (&pClone);
    if (H_NO_ERROR != hr)
    {
        ERROR_LOG (L"CreateInstance failed for CWordForm.");
        ATLASSERT(0);
        return hr;
    }
    pClone->m_pLexeme = pSource->m_pLexeme;
    pClone->m_sWordForm = pSource->m_sWordForm;
    pClone->m_sLemma = pSource->m_sLemma;
    pClone->m_llLexemeId = pSource->m_llLexemeId;
    pClone->m_mapStress = pSource->m_mapStress;
    pClone->m_ePos = pSource->m_ePos;
    pClone->m_eCase = pSource->m_eCase;
    pClone->m_eNumber = pSource->m_eNumber;
    pClone->m_eSubparadigm = pSource->m_eSubparadigm;
    pClone->m_eGender = pSource->m_eGender;
//    pClone->m_eTense = pSource->m_eTense;
    pClone->m_ePerson = pSource->m_ePerson;
    pClone->m_eAnimacy = pSource->m_eAnimacy;
    pClone->m_eReflexive = pSource->m_eReflexive;
//    pClone->m_eVoice = pSource->m_eVoice;
//    pClone->m_eSpecialForm = pSource->m_eSpecialForm;
//    pClone->m_eAspect = pSource->m_eAspect;
    pClone->m_eStatus = pSource->m_eStatus;

    return H_NO_ERROR;

}  // h_CloneWordForm (...)
*/

void CFormBuilder::CloneWordForm (const  CWordForm * pSource, CWordForm *& pClone)
{
    pClone = new CWordForm;
    if (NULL == pClone)
    {
        CEString sMsg (L"Unable to create instance of CWordForm.");
        ASSERT(0);
        throw CException (H_ERROR_POINTER, sMsg);   
    }

    pClone->m_pLexeme = pSource->m_pLexeme;
    pClone->m_sWordForm = pSource->m_sWordForm;
    pClone->m_sLemma = pSource->m_sLemma;
    pClone->m_llLexemeId = pSource->m_llLexemeId;
    pClone->m_mapStress = pSource->m_mapStress;
    pClone->m_ePos = pSource->m_ePos;
    pClone->m_eCase = pSource->m_eCase;
    pClone->m_eNumber = pSource->m_eNumber;
    pClone->m_eSubparadigm = pSource->m_eSubparadigm;
    pClone->m_eGender = pSource->m_eGender;
//    pClone->m_eTense = pSource->m_eTense;
    pClone->m_ePerson = pSource->m_ePerson;
    pClone->m_eAnimacy = pSource->m_eAnimacy;
    pClone->m_eReflexive = pSource->m_eReflexive;
//    pClone->m_eVoice = pSource->m_eVoice;
//    pClone->m_eSpecialForm = pSource->m_eSpecialForm;
//    pClone->m_eAspect = pSource->m_eAspect;
    pClone->m_eStatus = pSource->m_eStatus;

}  // CloneWordForm (...)

/*
ET_ReturnCode CFormBuilder::hHandleIrregularForms (CWordForm& wordForm, bool& bIsVariant)
{
    ET_ReturnCode hr = H_NO_ERROR;
 
    int iHash = wordForm.iGramHash();
    StIrregularForm stIf;
    hr = m_pLexeme->hGetFirstIrregularForm (iHash, stIf);
    if (H_NO_ERROR != hr)
    {
        return hr;
    }

    while (H_NO_ERROR == hr)
    {
        bIsVariant = stIf.bIsVariant;
        wordForm.m_sWordForm = stIf.sForm;
        wordForm.m_mapStress = stIf.mapStress;
        m_pLexeme->hAddWordForm (&wordForm);            
        
        hr = m_pLexeme->hGetNextIrregularForm (iHash, stIf);
        if (H_NO_ERROR == hr)
        {
            CWordForm * pNext = NULL;
            hr = hCloneWordForm (pWordForm, pNext);
            if (H_NO_ERROR != hr)
            {
                return hr;
            }
            pWordForm = pNext;
        }
    }       //  while (H_NO_ERROR == hr)

    return H_NO_ERROR;

}       //  hHandleIrregularForms (...)
*/

/*
unsigned int CFormBuilder::uiFormCount (int iHash)
{
    return m_pLexeme->m_mmWordForms.count (iHash);
}

bool CFormBuilder::bHasCommonDeviation (int iCd)
{
    bool bRet = false;
    map<int, bool>::iterator itCd = m_pLexeme->m_mapCommonDeviations.find (iCd);
    return (itCd != m_pLexeme->m_mapCommonDeviations.end());
}

bool CFormBuilderBase::bDeviationOptional (int iCd)
{
    bool bRet = false;
    map<int, bool>::iterator itCd = m_pLexeme->m_mapCommonDeviations.find (iCd);
    if (itCd != m_pLexeme->m_mapCommonDeviations.end())
    {
        return (*itCd).second;
    }
    else
    {
        ATLASSERT(0);
        CEString sMsg (L"Common deviation expected.");
        ERROR_LOG (sMsg);
        throw CException (E_FAIL, sMsg);
    }
}       //  bDeviationOptional (...)
*/