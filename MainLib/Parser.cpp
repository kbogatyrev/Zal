// Parse.cpp : Implementation of CParser

#include "stdafx.h"

#include <ctime>

#include "SqliteWrapper.h"
#include "Endings.h"
#include "WordForm.h"
#include "Parser.h"

using namespace Hlib;

CParser::CParser(CSqlite * pDb) : m_pDb(pDb), m_pEndingsTree(NULL)
{}

CParser::~CParser()
{
    delete m_pEndingsTree;
    ClearResults();
}

ET_ReturnCode CParser::eAnalyze(const CEString& sWord)
{
    clock_t startTime = clock();

    m_vecWordForms.clear();

    if (!m_pDb)
    {
        return H_ERROR_DB;
    }

    if (NULL == m_pEndingsTree)
    {
        m_pEndingsTree = new CParsingTree(m_pDb);
    }

    ET_ReturnCode rc = H_NO_ERROR;

    rc = eIrregularFormLookup(sWord);
    if (rc != H_NO_ERROR)
    {
        ERROR_LOG(L"Irregular form lookup failed.");
    }

    rc = eWholeWordLookup(sWord);
    if (rc != H_NO_ERROR)
    {
        ERROR_LOG(L"Irregular form lookup failed.");
    }

    rc = eFormLookup(sWord);
    if (rc != H_NO_ERROR)
    {
        ERROR_LOG(L"Regular form lookup failed.");
    }

    clock_t totalTime = clock() - startTime;

    return m_vecWordForms.empty() ? H_FALSE : H_NO_ERROR;

}       // eAnalyze()

ET_ReturnCode CParser::eGetFirstWordForm(IWordForm *& pWordForm)
{
    m_itCurrentWordForm = m_vecWordForms.begin();
    if (m_vecWordForms.end() == m_itCurrentWordForm)
    {
        return H_FALSE;
    }

    pWordForm = *m_itCurrentWordForm;

    return H_NO_ERROR;
}

ET_ReturnCode CParser::eGetNextWordForm(IWordForm *& pWordForm)
{
    if (m_itCurrentWordForm != m_vecWordForms.end())
    {
        ++m_itCurrentWordForm;
    }

    if (m_vecWordForms.end() == m_itCurrentWordForm)
    {
        return H_NO_MORE;
    }

    pWordForm = *m_itCurrentWordForm;

    return H_NO_ERROR;
}

void CParser::ClearResults()
{
    vector<CWordForm *>::iterator itWf = m_vecWordForms.begin();
    for (; itWf != m_vecWordForms.end(); ++itWf)
    {
        delete *itWf;
    }
}


//
// Helpers
//

ET_ReturnCode CParser::eQueryDb(const CEString& sSelect, unsigned int& uiQueryHandle)
{
    ET_ReturnCode rc = H_NO_ERROR;

    if (NULL == m_pDb)
    {
        ASSERT(0);
        ERROR_LOG(L"DB pointer is NULL.");
        return H_ERROR_POINTER;
    }

    try
    {
        uiQueryHandle = m_pDb->uiPrepareForSelect(sSelect);
    }
    catch (CException& ex)
    {
        HandleDbException(ex);
        rc = H_ERROR_DB;
    }

    return rc;
}

ET_ReturnCode CParser::eIrregularFormLookup(const CEString& sWord)
{
    ET_ReturnCode rc = H_NO_ERROR;

    CEString sIrregFormQuery(L"SELECT DISTINCT id, descriptor_id, gram_hash, wordform, is_alternative FROM irregular_forms WHERE wordform = \"");
    sIrregFormQuery += sWord;
    sIrregFormQuery += L'\"';

    try
    {
        unsigned int uiFormQueryHandle = m_pDb->uiPrepareForSelect(sIrregFormQuery);
        while (m_pDb->bGetRow(uiFormQueryHandle))
        {
            __int64 llFormId = -1;
            m_pDb->GetData(0, llFormId, uiFormQueryHandle);

            __int64 llDescriptorId = -1;
            m_pDb->GetData(1, llDescriptorId, uiFormQueryHandle);

            int iGramHash = -1;
            m_pDb->GetData(2, iGramHash, uiFormQueryHandle);

            CEString sWordForm;
            sWordForm.SetVowels(g_szRusVowels);
            m_pDb->GetData(3, sWordForm, uiFormQueryHandle);

            bool bIsAlternative = false;
            m_pDb->GetData(4, bIsAlternative, uiFormQueryHandle);       // what to do with this one?

            CWordForm * pWf = new CWordForm(iGramHash);
            pWf->m_bIrregular = true;
            pWf->m_llLexemeId = llDescriptorId;
            pWf->m_sWordForm = sWordForm;

            CEString sIrregStressQuery(L"SELECT position, is_primary FROM irregular_stress WHERE form_id = \"");
            sIrregStressQuery += CEString::sToString(llFormId);
            sIrregStressQuery += L'\"';
            unsigned int uiStresshandle = m_pDb->uiPrepareForSelect(sIrregStressQuery);
            while (m_pDb->bGetRow(uiStresshandle))
            {
                int iPosition = -1;
                m_pDb->GetData(0, iPosition, uiStresshandle);

                ET_StressType eType = STRESS_TYPE_UNDEFINED;
                m_pDb->GetData(1, (int&)eType, uiStresshandle);
                int iStressedSyll = sWordForm.uiGetSyllableFromVowelPos(iPosition);
                pWf->m_mapStress[iStressedSyll] = eType;
            }

            m_vecWordForms.push_back(pWf);
        }
    }
    catch (CException& ex)
    {
        HandleDbException(ex);
        rc = H_ERROR_DB;
    }

    return H_NO_ERROR;

}       //  eIrregularFormLookup()

static CEString s_sWholeWordQuery(L"SELECT DISTINCT sd.gram_hash, sd.lexeme_id, wf.id, wf.ending_data_id \
                                   FROM stems AS s INNER JOIN stem_data as sd \
                                   ON (sd.stem_id = s.id) INNER JOIN wordforms as wf \
                                   ON sd.id = wf.stem_data_id  \
                                   WHERE s.stem_string='#WORDFORM#'");

ET_ReturnCode CParser::eWholeWordLookup(const CEString& sWord)
{
    CEString sQuery(s_sWholeWordQuery);
    sQuery = sQuery.sReplace(L"#WORDFORM#", sWord);
    unsigned int uiFormQueryHandle = m_pDb->uiPrepareForSelect(sQuery);
    while (m_pDb->bGetRow(uiFormQueryHandle))
    {
        int iGramHash = -1;
        m_pDb->GetData(0, iGramHash, uiFormQueryHandle);

        __int64 llLexemeId = -1;
        m_pDb->GetData(1, llLexemeId, uiFormQueryHandle);

        __int64 llFormId = -1;
        m_pDb->GetData(2, llFormId, uiFormQueryHandle);

        __int64 llEndingId = -1;
        m_pDb->GetData(3, llEndingId, uiFormQueryHandle);

        if (llEndingId < 0 || H_TRUE == m_pEndingsTree->eIsEmptyEnding(llEndingId))
        {
            CWordForm * pWf = new CWordForm(iGramHash);
            pWf->m_bIrregular = false;
            pWf->m_llLexemeId = llLexemeId;
            pWf->m_sWordForm = sWord;

            CEString sStressQuery(L"SELECT position, is_primary FROM stress_data WHERE form_id = \"");
            sStressQuery += CEString::sToString(llFormId);
            sStressQuery += L'\"';
            unsigned int uiStresshandle = m_pDb->uiPrepareForSelect(sStressQuery);
            while (m_pDb->bGetRow(uiStresshandle))
            {
                int iPosition = -1;
                m_pDb->GetData(0, iPosition, uiStresshandle);

                ET_StressType eType = STRESS_TYPE_UNDEFINED;
                m_pDb->GetData(1, (int&)eType, uiStresshandle);
                pWf->m_mapStress[iPosition] = eType;
            }

            m_vecWordForms.push_back(pWf);
        }

    }       //  while (m_pDb->bGetRow(...))           

    return H_NO_ERROR;

}   //  eWholeWordLookup()

static CEString s_sWordFormQuery (L"SELECT DISTINCT wf.id, sd.gram_hash, sd.lexeme_id FROM stems AS s \
                                  INNER JOIN stem_data AS sd ON(s.id = sd.stem_id) \
                                  INNER JOIN wordforms AS wf ON(sd.id = wf.stem_data_id) \
                                  WHERE s.stem_string = '#STEM#' AND wf.ending_data_id \
                                  IN (SELECT ending_data.id FROM endings \
                                  INNER JOIN ending_data ON endings.id = ending_data.ending_id \
                                  WHERE endings.ending_string = '#ENDING#')");

ET_ReturnCode CParser::eFormLookup(const CEString& sWord)
{
    if (NULL == m_pEndingsTree)
    {
        return H_ERROR_POINTER;
    }

    m_pEndingsTree->eSplit(sWord);

    int iEndingLength = -1;
    ET_ReturnCode rc = m_pEndingsTree->eGetFirstMatch(iEndingLength);
    while (H_NO_ERROR == rc || H_FALSE == rc)
    {
        if (iEndingLength > 0)
        {
            try
            {
                CEString sStemCandidate = sWord.sSubstr(0, sWord.uiLength() - iEndingLength);
                CEString sEndingCandidate = sWord.sSubstr(sWord.uiLength() - iEndingLength);

                CEString sQuery(s_sWordFormQuery);
                sQuery = sQuery.sReplace(L"#STEM#", sStemCandidate);
                sQuery = sQuery.sReplace(L"#ENDING#", sEndingCandidate);

                unsigned int uiFormQueryHandle = m_pDb->uiPrepareForSelect(sQuery);

                while (m_pDb->bGetRow(uiFormQueryHandle))
                {
                    __int64 llFormId = -1;
                    m_pDb->GetData(0, llFormId, uiFormQueryHandle);

                    int iGramHash = -1;
                    m_pDb->GetData(1, iGramHash, uiFormQueryHandle);

                    __int64 llLexemeId = -1;
                    m_pDb->GetData(2, llLexemeId, uiFormQueryHandle);

                    CWordForm * pWf = new CWordForm(iGramHash);
                    pWf->m_bIrregular = false;
                    pWf->m_llLexemeId = llLexemeId;
                    pWf->m_sWordForm = sWord;

                    CEString sStressQuery(L"SELECT position, is_primary FROM stress_data WHERE form_id = \"");
                    sStressQuery += CEString::sToString(llFormId);
                    sStressQuery += L'\"';

                    unsigned int uiStresshandle = m_pDb->uiPrepareForSelect(sStressQuery);
                    while (m_pDb->bGetRow(uiStresshandle))
                    {
                        int iPosition = -1;
                        m_pDb->GetData(0, iPosition, uiStresshandle);

                        ET_StressType eType = STRESS_TYPE_UNDEFINED;
                        m_pDb->GetData(1, (int&)eType, uiStresshandle);
                        pWf->m_mapStress[iPosition] = eType;
                    }
                    m_vecWordForms.push_back(pWf);

                }       //  while (m_pDb->bGetRow(...))            
            }
            catch (CException& ex)
            {
                HandleDbException(ex);
                rc = H_ERROR_DB;
            }
        }

        rc = m_pEndingsTree->eGetNextMatch(iEndingLength);

        if (H_FALSE == rc)
        {
            break;
        }

    }       //  while (H_NO_ERROR == rc)

    return H_NO_ERROR;

}       //  eFormLookup()

void CParser::HandleDbException(CException& ex)
{
    CEString sMsg(ex.szGetDescription());
    CEString sError;
    try
    {
        m_pDb->GetLastError(sError);
        sMsg += CEString(L", error description: ");
        sMsg += sError;
    }
    catch (...)
    {
        sMsg = L"Apparent DB error ";
    }

    sMsg += L", error code = ";
    sMsg += CEString::sToString(m_pDb->iGetLastError());
    ERROR_LOG(sMsg);

}
