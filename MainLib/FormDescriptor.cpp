#include "StdAfx.h"
#include "Lexeme.h"
#include "FormDescriptor.h"

//
// IFormFinder
//

HRESULT CFormDescriptor::FindForms()
{
    try
    {
        m_coll.clear();
        FormsFromHash();
    }
    catch (CException ex)
    {
        CEString sMsg (L"Exception in v_FormsFromHash(): ");
        sMsg += ex.szGetDescription();
        ERROR_LOG (sMsg);
        return E_FAIL;  // logging should be always done by callee
    }

    return S_OK;
}

HRESULT CFormDescriptor::put_PartOfSpeech (ET_PartOfSpeech ePos)
{
    m_Hasher.m_ePos = ePos;
    return S_OK;
}

HRESULT CFormDescriptor::put_Subparadigm (ET_Subparadigm eSp)
{
    m_Hasher.m_eSubparadigm = eSp;
    return S_OK;
}

HRESULT CFormDescriptor::put_Case (ET_Case eCase)
{
    m_Hasher.m_eCase = eCase;
    return S_OK;
}

HRESULT CFormDescriptor::put_Number (ET_Number eNumber)
{
    m_Hasher.m_eNumber = eNumber;
    return S_OK;
}

HRESULT CFormDescriptor::put_Gender (ET_Gender eGender)
{
    m_Hasher.m_eGender = eGender;
    return S_OK;
}

//HRESULT CFormDescriptor::put_Tense (ET_Tense eo_tense)
//{
//    m_Hasher.eo_Tense = eo_tense;
//    return S_OK;
//}

HRESULT CFormDescriptor::put_Person (ET_Person ePerson)
{
    m_Hasher.m_ePerson = ePerson;
    return S_OK;
}

HRESULT CFormDescriptor::put_Animacy (ET_Animacy eAnimacy)
{
    m_Hasher.m_eAnimacy = eAnimacy;
    return S_OK;
}

HRESULT CFormDescriptor::put_Reflexivity (ET_Reflexive eReflexivity)
{
    m_Hasher.m_eReflexive = eReflexivity;
    return S_OK;
}

//HRESULT CFormDescriptor::put_Voice (ET_Voice eo_voice)
//{
//    m_Hasher.eo_Voice = eo_voice;
//    return S_OK;
//}

HRESULT CFormDescriptor::put_Aspect (ET_Aspect eAspect)
{
    m_Hasher.m_eAspect = eAspect;
    return S_OK;
}

void CFormDescriptor::FormsFromHash()
{
    pair<multimap<int, CComVariant>::iterator, multimap<int, CComVariant>::iterator> pairRange;
    pairRange = m_pLexeme->m_coll.equal_range (m_Hasher.iGramHash());

    multimap<int, CComVariant>::iterator itWf (pairRange.first);
    for (; itWf != pairRange.second; ++itWf)
    {
        if ((*itWf).first != m_Hasher.iGramHash())
        {
            ATLASSERT(0);
            CEString sMsg (L"Error extracting map element.");
            ERROR_LOG (sMsg);
            throw CException (E_INVALIDARG, sMsg);
        }

        CComQIPtr<IWordForm> spWf = (*itWf).second.pdispVal;
        if (NULL == spWf)
        {
            ATLASSERT(0);
            CEString sMsg (L"QI for IWordForm failed.");
            ERROR_LOG (sMsg);
            throw CException (E_POINTER, sMsg);
        }

        m_coll.push_back ((*itWf).second.pdispVal);
    }

}   //  FormsFromHash()
