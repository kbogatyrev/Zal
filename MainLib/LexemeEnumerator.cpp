#include "Dictionary.h"
#include "Lexeme.h"
#include "LexemeEnumerator.h"

using namespace Hlib;

ET_ReturnCode CLexemeEnumerator::eReset()
{
    if (nullptr == m_pDictionary)
    {
        return H_ERROR_POINTER;
    }

    m_itCurrentLexeme = m_pDictionary->m_vecLexemes.begin();
    return H_NO_ERROR;
}

ET_ReturnCode CLexemeEnumerator::eGetFirstLexeme(ILexeme*& pLexeme)
{
    if (nullptr == m_pDictionary)
    {
        return H_ERROR_POINTER;
    }

    m_itCurrentLexeme = m_pDictionary->m_vecLexemes.begin();
    if (m_pDictionary->m_vecLexemes.end() == m_itCurrentLexeme)
    {
        return H_FALSE;
    }

    pLexeme = *m_itCurrentLexeme;

    return H_NO_ERROR;
}

ET_ReturnCode CLexemeEnumerator::eGetFirstLexeme(CLexeme*& pLexeme)
{
    if (nullptr == m_pDictionary)
    {
        return H_ERROR_POINTER;
    }

    m_itCurrentLexeme = m_pDictionary->m_vecLexemes.begin();
    if (m_pDictionary->m_vecLexemes.end() == m_itCurrentLexeme)
    {
        return H_FALSE;
    }

    pLexeme = *m_itCurrentLexeme;

    return H_NO_ERROR;
}

ET_ReturnCode CLexemeEnumerator::eGetNextLexeme(ILexeme*& pLexeme)
{
    if (nullptr == m_pDictionary)
    {
        return H_ERROR_POINTER;
    }

    if (m_itCurrentLexeme != m_pDictionary->m_vecLexemes.end())
    {
        ++m_itCurrentLexeme;
    }

    if (m_pDictionary->m_vecLexemes.end() == m_itCurrentLexeme)
    {
        return H_NO_MORE;
    }

    pLexeme = *m_itCurrentLexeme;

    return H_NO_ERROR;
}

ET_ReturnCode CLexemeEnumerator::eGetNextLexeme(CLexeme*& pLexeme)
{
    if (nullptr == m_pDictionary)
    {
        return H_ERROR_POINTER;
    }

    if (m_itCurrentLexeme != m_pDictionary->m_vecLexemes.end())
    {
        ++m_itCurrentLexeme;
    }

    if (m_pDictionary->m_vecLexemes.end() == m_itCurrentLexeme)
    {
        return H_NO_MORE;
    }

    pLexeme = *m_itCurrentLexeme;

    return H_NO_ERROR;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

