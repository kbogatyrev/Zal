#include "StdAfx.h"
#include "Lexeme.h"
#include "FormDescriptor.h"

//
// IFormDescriptor
//

HRESULT CT_FormDescriptor::FindForms()
{
    try
    {
        m_coll.clear();
        v_FormsFromHash();
    }
    catch (CT_Exception co_ex)
    {
        wstring str_msg (L"Exception in v_FormsFromHash(): ");
        str_msg += co_ex.str_GetDescription();
        ERROR_LOG (str_msg);
        return E_FAIL;  // logging should be always done by callee
    }

    return S_OK;
}

HRESULT CT_FormDescriptor::put_PartOfSpeech (ET_PartOfSpeech eo_pos)
{
    co_Hasher.eo_POS = eo_pos;
    return S_OK;
}

HRESULT CT_FormDescriptor::put_Subparadigm (ET_Subparadigm eo_sp)
{
    co_Hasher.eo_Subparadigm = eo_sp;
    return S_OK;
}

HRESULT CT_FormDescriptor::put_Case (ET_Case eo_case)
{
    co_Hasher.eo_Case = eo_case;
    return S_OK;
}

HRESULT CT_FormDescriptor::put_Number (ET_Number eo_number)
{
    co_Hasher.eo_Number = eo_number;
    return S_OK;
}

HRESULT CT_FormDescriptor::put_Gender (ET_Gender eo_gender)
{
    co_Hasher.eo_Gender = eo_gender;
    return S_OK;
}

HRESULT CT_FormDescriptor::put_Tense (ET_Tense eo_tense)
{
    co_Hasher.eo_Tense = eo_tense;
    return S_OK;
}

HRESULT CT_FormDescriptor::put_Person (ET_Person eo_person)
{
    co_Hasher.eo_Person = eo_person;
    return S_OK;
}

HRESULT CT_FormDescriptor::put_Animacy (ET_Animacy eo_animacy)
{
    co_Hasher.eo_Animacy = eo_animacy;
    return S_OK;
}

HRESULT CT_FormDescriptor::put_Reflexivity (ET_Reflexive eo_reflexivity)
{
    co_Hasher.eo_Reflexive = eo_reflexivity;
    return S_OK;
}

HRESULT CT_FormDescriptor::put_Voice (ET_Voice eo_voice)
{
    co_Hasher.eo_Voice = eo_voice;
    return S_OK;
}

HRESULT CT_FormDescriptor::put_Aspect (ET_Aspect eo_aspect)
{
    co_Hasher.eo_Aspect = eo_aspect;
    return S_OK;
}

void CT_FormDescriptor::v_FormsFromHash()
{
    pair<multimap<int, CComVariant>::iterator, multimap<int, CComVariant>::iterator> pair_range;
    pair_range = pco_Lexeme->m_coll.equal_range (co_Hasher.i_GramHash());

    multimap<int, CComVariant>::iterator it_wf (pair_range.first);
    for (; it_wf != pair_range.second; ++it_wf)
    {
        if ((*it_wf).first != co_Hasher.i_GramHash())
        {
            ATLASSERT(0);
            wstring str_msg (L"Error extracting map element.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_INVALIDARG, str_msg);
        }

        CComQIPtr<IWordForm> sp_wf = (*it_wf).second.pdispVal;
        if (NULL == sp_wf)
        {
            ATLASSERT(0);
            wstring str_msg (L"QI for IWordForm failed.");
            ERROR_LOG (str_msg);
            throw CT_Exception (E_POINTER, str_msg);
        }

        m_coll.push_back ((*it_wf).second.pdispVal);
    }

}   //  v_FormsFromHash()
