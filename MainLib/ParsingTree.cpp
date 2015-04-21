#include "StdAfx.h"
#include "ParsingTree.h"

using namespace Hlib;

static CEString sSelectBase
(L"SELECT DISTINCT d.id, e.ending_string, number, case_value, stress FROM endings AS e INNER JOIN ending_data AS d ON (e.id = d.ending_id)");

CParsingTree::CParsingTree(CSqlite * pDb)
{
    if (NULL == pDb)
    {
        throw CException(H_ERROR_POINTER, L"Database handle not initialized.");
    }

    ET_ReturnCode rc = eLoad(pDb);
    if (rc != H_NO_ERROR)
    {
        throw CException(H_ERROR_GENERAL, L"Unable to initalize parsing tree.");
    }
}

CParsingTree::~CParsingTree()
{
    eTraverseAndDelete(m_pRoot);
}

static int s_iLevel;
void CParsingTree::eSplit(const CEString& sWord)
{
    s_iLevel = 0;
    m_vecMatches.clear();
    Traverse(m_pRoot, sWord);
}

void CParsingTree::Traverse(StNode * pNode, const CEString& sWord)
{
    //    CEString sMsg(L" === ");
    //    for (int i = 0; i < s_iLevel; ++i)
    //    {
    //        sMsg += L'\t';
    //    }
    //    sMsg += pRoot->cLetter;
    //    CLogger::bWriteLog(wstring(sMsg));

    if (s_iLevel > 0 && sWord[sWord.uiLength() - s_iLevel] != pNode->cLetter)
    {
        return;
    }

    m_vecMatches.push_back(s_iLevel);

    ++s_iLevel;
    vector<StNode *>::iterator it_ = pNode->vecChildren.begin();
    for (; it_ != pNode->vecChildren.end(); ++it_)
    {
        Traverse(*it_, sWord);
    }
    --s_iLevel;
}

ET_ReturnCode CParsingTree::eGetFirstMatch(int& iEndingLength)
{
    m_itCurrentMatch = m_vecMatches.begin();
    if (m_vecMatches.end() == m_itCurrentMatch)
    {
        return H_FALSE;
    }

    iEndingLength = *m_itCurrentMatch;

    return H_NO_ERROR;
}

ET_ReturnCode CParsingTree::eGetNextMatch(int& iEndingLength)
{
    if (m_vecMatches.end() == ++m_itCurrentMatch)
    {
        return H_FALSE;
    }

    iEndingLength = *m_itCurrentMatch;

    return H_NO_ERROR;
}

ET_ReturnCode CParsingTree::eIsEmptyEnding(__int64 llEndingId)
{
    if (m_setEmptyEndingIds.empty())
    {
        return H_ERROR_UNEXPECTED;
    }

    if (m_setEmptyEndingIds.end() == m_setEmptyEndingIds.find(llEndingId))
    {
        return H_FALSE;
    }
    else
    {
        return H_TRUE;
    }
}

ET_ReturnCode CParsingTree::eLoad(CSqlite * pDb)
{
    // ся/сь!!!!!!
    // common dev????

    if (NULL == pDb)
    {
        return H_ERROR_POINTER;
    }

    m_mapSortedEndingsList.clear();
    m_setEmptyEndingIds.clear();
    m_setEndingStrings.clear();

    try
    {
        pDb->PrepareForSelect(sSelectBase);
        while (pDb->bGetRow())
        {
            CEString sEnding;
            __int64 llDbKey = 0;
            pDb->GetData(0, (__int64)llDbKey);
            pDb->GetData(1, sEnding);
            m_mapSortedEndingsList[sEnding].push_back(llDbKey);
            m_setEndingStrings.insert(sEnding);
        }
        pDb->Finalize();
    }
    catch (...)
    {
        CEString sMsg;
        try
        {
            CEString sError;
            pDb->GetLastError(sError);
            sMsg = L"DB error: ";
            sMsg += sError;
            sMsg += L"; code = ";
            sMsg += CEString::sToString(pDb->iGetLastError());
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(pDb->iGetLastError());
        ERROR_LOG(sMsg);
        return H_ERROR_DB;
    }

    MapEndingToDescriptors::iterator itEnding = m_mapSortedEndingsList.begin();

    // TEST ONLY
    //    for (; itEnding != m_mapSortedEndingsList.end(); ++itEnding)
    //    {
    //        const CEString& sEnding = (*itEnding).first;
    //        CLogger::bWriteLog(wstring(sEnding));
    //    }
    //    itEnding = m_mapSortedEndingsList.begin();
    // END TEST ONLY

    /*
    wchar_t cLastChar = '\0';
    for (; itEnding != m_mapSortedEndingsList.end(); ++itEnding)
    {
    const CEString& sEnding = (*itEnding).first;
    if (sEnding[sEnding.uiLength - 1] != cLastChar)
    {
    StNode * pNode = new StNode;
    }
    }
    */

    m_pRoot = new StNode;
    m_pRoot->cLetter = L'\0';
    m_pRoot->pPrevious = NULL;

    while ((*itEnding).first.uiLength() < 1)
    {
        vector<__int64>::iterator itEndingDataKey = (*itEnding).second.begin();
        for (; itEndingDataKey != (*itEnding).second.end(); ++itEndingDataKey)
        {
            m_setEmptyEndingIds.insert(*itEndingDataKey);
        }
        ++itEnding;
    }

    vector<CEString> vecAll;
    while (itEnding != m_mapSortedEndingsList.end())
    {
        vecAll.push_back((*itEnding++).first);
    }

    AddLevel(0, m_pRoot, vecAll);

    return H_NO_ERROR;

}       //  ET_ReturnCode CParsingTree::Load(CSqlite * pDb)

void CParsingTree::AddLevel(unsigned int uiOffset, StNode * pParent, vector<CEString> vecInBucket)
{
    wchar_t chrCurrent = L'\0';
    StNode * pCurrentNode = NULL;
    vector<CEString> vecOutBucket;

    vector<CEString>::iterator itEnding = vecInBucket.begin();
    for (; itEnding != vecInBucket.end(); ++itEnding)
    {
        CEString sNext = *itEnding;
        if (sNext.uiLength() - uiOffset <= 0)
        {
            return; // ERROR?
        }

        wchar_t chrNext = sNext[sNext.uiLength() - uiOffset - 1];
        if (chrNext != chrCurrent)
        {
            if (!vecOutBucket.empty())
            {
                AddLevel(uiOffset + 1, pCurrentNode, vecOutBucket);
                vecOutBucket.clear();
            }

            pCurrentNode = new StNode;
            pCurrentNode->pPrevious = pParent;
            pCurrentNode->cLetter = chrNext;
            pParent->vecChildren.push_back(pCurrentNode);
            chrCurrent = chrNext;
        }

        if ((int)sNext.uiLength() - (int)uiOffset - 2 >= 0)
        {
            vecOutBucket.push_back(*itEnding);
        }
    }   // for...

    if (!vecOutBucket.empty())
    {
        AddLevel(uiOffset + 1, pCurrentNode, vecOutBucket);
        vecOutBucket.clear();
    }

}       //  AddLevel()

ET_ReturnCode CParsingTree::eTraverseAndDelete(StNode * pRoot)
{
    if (NULL == pRoot)
    {
        ERROR_LOG(L"Root node does not exit.");
    }

    vector<StNode *>::iterator it_ = pRoot->vecChildren.begin();
    for (; it_ != pRoot->vecChildren.end(); ++it_)
    {
        eTraverseAndDelete(*it_);
    }
    delete pRoot;

    return H_NO_ERROR;
}

