#include "StdAfx.h"
#include "Lexeme.h"
#include "SqliteWrapper.h"
#include "Endings.h"

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

    if (s_iLevel > 0 && sWord[sWord.uiLength()-s_iLevel] != pNode->cLetter)
    {
        return;
    }

    StSplit stSplit;
    stSplit.uiEndingLength = s_iLevel;
    stSplit.vecDescriptors = pNode->vecTerms;
    m_vecMatches.push_back(stSplit);

    ++s_iLevel;
    vector<StNode *>::iterator it_ = pNode->vecChildren.begin();
    for (; it_ != pNode->vecChildren.end(); ++it_)
    {
        Traverse(*it_, sWord);
    }
    --s_iLevel;
}

ET_ReturnCode CParsingTree::eGetFirstMatch(StSplit& stSplit)
{
    m_itCurrentMatch = m_vecMatches.begin();
    if (m_vecMatches.end() == m_itCurrentMatch)
    {
        return H_FALSE;
    }

    stSplit = *m_itCurrentMatch;

    return H_NO_ERROR;
}

ET_ReturnCode CParsingTree::eGetNextMatch(StSplit& stSplit)
{
    if (m_vecMatches.end() == ++m_itCurrentMatch)
    {
        return H_FALSE;
    }

    stSplit = *m_itCurrentMatch;

    return H_NO_ERROR;
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

    try
    {
        pDb->PrepareForSelect(sSelectBase);
        while (pDb->bGetRow())
        {
            CEString sEnding;
            StEndingDescriptor stD;
            pDb->GetData(0, (__int64&)stD.llDbKey);
            pDb->GetData(1, sEnding);
            pDb->GetData(2, (int&)stD.eNumber);
            pDb->GetData(3, (int&)stD.eCase);
            pDb->GetData(4, (int&)stD.eStress);

            m_mapSortedEndingsList[sEnding].push_back (stD);
        }
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
        m_pRoot->vecTerms.insert(m_pRoot->vecTerms.end(), (*itEnding).second.begin(), (*itEnding).second.end());
        ++itEnding;
    }

    VecBucket vecAll;
    while (itEnding != m_mapSortedEndingsList.end())
    {
        vecAll.push_back(itEnding++);
    }

    AddLevel(0, m_pRoot, vecAll);

    return H_NO_ERROR;

}       //  ET_ReturnCode CParsingTree::Load(CSqlite * pDb)

void CParsingTree::AddLevel(unsigned int uiOffset, StNode * pParent, VecBucket vecInBucket)
{    
    wchar_t chrCurrent = L'\0';
    StNode * pCurrentNode = NULL;
    VecBucket vecOutBucket;

    VecBucket::iterator itEnding = vecInBucket.begin();        
    for (; itEnding != vecInBucket.end(); ++itEnding)
    {
        CEString sNext = (*itEnding)->first;
        if (sNext.uiLength() - uiOffset <= 0)
        {
            return; // ERROR?
        }

        wchar_t chrNext = sNext[sNext.uiLength()-uiOffset-1];
        if (chrNext != chrCurrent)
        {
            if (!vecOutBucket.empty())
            {
                AddLevel(uiOffset+1, pCurrentNode, vecOutBucket);
                vecOutBucket.clear();
            }

            pCurrentNode = new StNode;
            pCurrentNode->pPrevious = pParent;
            pCurrentNode->cLetter = chrNext;
            pParent->vecChildren.push_back(pCurrentNode);
            chrCurrent = chrNext;
        }

        if ((int)sNext.uiLength()-(int)uiOffset-2 < 0)
        {
            pCurrentNode->vecTerms = (*itEnding)->second;
        }
        else
        {
            vecOutBucket.push_back(*itEnding);
        }        
    }   // for...

    if (!vecOutBucket.empty())
    {
        AddLevel(uiOffset+1, pCurrentNode, vecOutBucket);
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

void CEndings::ReportDbError()
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

}   //  ReportDbError()

CNounEndings::CNounEndings(CLexeme * pLexeme) : CEndings(pLexeme)
{}

ET_ReturnCode CNounEndings::eLoad()
{
    ET_ReturnCode rc = H_NO_ERROR;

//    CEString sSelect 
//        (L"SELECT DISTINCT d.id, e.ending_string, number, case_value, stress FROM endings AS e INNER JOIN ending_data AS d ON (e.id = d.ending_id)");
    CEString sSelect(sSelectBase);
    sSelect += L" WHERE inflection_class = ";
    sSelect += CEString::sToString (ENDING_CLASS_NOUN);
    sSelect += L" AND inflection_type = ";
    sSelect += CEString::sToString (m_pLexeme->iType());
    sSelect += L" AND (gender = ";
    sSelect += CEString::sToString (m_pLexeme->eGender());
    sSelect += L" OR gender = ";
    sSelect += CEString::sToString (GENDER_UNDEFINED) + L")";
    sSelect += L" AND (animacy = ";
    sSelect += CEString::sToString (m_pLexeme->eAnimacy());
    sSelect += L" OR animacy = ";
    sSelect += CEString::sToString (ANIM_UNDEFINED) + L")";
    sSelect += L" AND stem_augment = ";
    sSelect += CEString::sToString (m_pLexeme->iStemAugment());

    CSqlite * pDb = NULL;
    try
    {
        pDb = m_pLexeme->pGetDb();

        CEString sSelectBase (sSelect);
        sSelectBase += L" AND common_deviation = 0;";

        pDb->PrepareForSelect (sSelectBase);
        while (pDb->bGetRow())
        {
            CEString sEnding;
            StEndingDescriptor stD;
            pDb->GetData (0, (__int64&)stD.llDbKey);
            pDb->GetData (1, sEnding);
            pDb->GetData (2, (int&)stD.eNumber);
            pDb->GetData (3, (int&)stD.eCase);
            pDb->GetData (4, (int&)stD.eStress);

            bool bIsOptional = false;
            if (m_pLexeme->bFindCommonDeviation(1, bIsOptional))
            {
                if (NUM_PL == stD.eNumber && CASE_NOM == stD.eCase && GENDER_F != stD.eGender)
                {
                    if (!bIsOptional)
                    {
                        continue;
                    }
                }
            }

            if (m_pLexeme->bFindCommonDeviation(2, bIsOptional))
            {
                if (NUM_PL == stD.eNumber && CASE_GEN == stD.eCase)
                {
                    if (!bIsOptional)    // optional?
                    {
                        continue;
                    }
                }
            }
            rc = eAddEnding (sEnding, stD);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }

        }   //  while (pDb->bGetRow())

        pDb->Finalize();
    }
    catch (...)
    {
        ReportDbError();
        return H_ERROR_GENERAL;
    }

    for (int iCd = 1; iCd <= 2; ++iCd)
    {
        if (!m_pLexeme->bHasCommonDeviation(iCd))
        {
            continue;
        }

        CEString sSelectCD (sSelect);
        sSelectCD += L" AND common_deviation = ";
        sSelectCD += CEString::sToString (iCd);
        sSelectCD += L";";

        try
        {
            pDb->PrepareForSelect (sSelectCD);
            while (pDb->bGetRow())
            {
                CEString sEnding;
                StEndingDescriptor stD;
                pDb->GetData (0, (__int64&)stD.llDbKey);
                pDb->GetData (1, sEnding);
                pDb->GetData (2, (int&)stD.eNumber);
                pDb->GetData (3, (int&)stD.eCase);
                pDb->GetData (4, (int&)stD.eStress);
                rc = eAddEnding (sEnding, stD);
                if (rc != H_NO_ERROR)
                {
                    return rc;
                }
            }
            pDb->Finalize();
        }
        catch (...)
        {
            ReportDbError();
            return H_ERROR_DB;
        }

    }   // for ...

    return H_NO_ERROR;

}   //  eLoad()

int CNounEndings::iHash (const StEndingDescriptor& stD)
{
    ASSERT (stD.eAnimacy == ANIM_UNDEFINED && stD.eGender == GENDER_UNDEFINED);

    int iKey = stD.eNumber * CASE_COUNT * STRESS_LOCATION_COUNT +
                stD.eCase * STRESS_LOCATION_COUNT +
                stD.eStress;

    return iKey;
}

ET_ReturnCode CNounEndings::eAddEnding (const CEString& sEnding, const StEndingDescriptor& stDescriptor)
{
    std::vector<ET_Number> vecNumber;
    if (NUM_UNDEFINED == stDescriptor.eNumber)
    {
        for (int iNum = (int)NUM_UNDEFINED; iNum < (int)NUM_COUNT; ++iNum)
        {
            vecNumber.push_back ((ET_Number)iNum);
        }
    }
    else
    {
        vecNumber.push_back (stDescriptor.eNumber);
    }

    //
    // Case must be defined for nouns
    //
    if (CASE_UNDEFINED == stDescriptor.eCase)
    {
        ERROR_LOG (L"Undefined case in noun ending.");
        return H_ERROR_INVALID_ARG;
    }

    std::vector<ET_StressLocation> vecStress;
    if (STRESS_LOCATION_UNDEFINED == stDescriptor.eStress)
    {
        for (int iStress = STRESS_LOCATION_UNDEFINED; 
             iStress < (int)STRESS_LOCATION_COUNT; 
             ++iStress)
        {
            vecStress.push_back ((ET_StressLocation)iStress);
        }
    }
    else
    {
        vecStress.push_back (stDescriptor.eStress);
    }

    for (int iN = 0; iN < (int)vecNumber.size(); ++iN)
        for (int iS = 0; iS < (int)vecStress.size(); ++iS)
        {
            StEndingDescriptor stD (vecNumber[iN], stDescriptor.eCase, vecStress[iS]);
            int iKey = iHash (stD);
            std::pair<CEString, __int64> pairEndingData(sEnding, stDescriptor.llDbKey);
            m_mmEndings.insert(std::pair<int, std::pair<CEString, __int64> >(iKey, pairEndingData));
        }

    return H_NO_ERROR;

}   // CNounEndings::eAddEnding (...)

////////////////////////////////////////////////////////////////////////////////////

CAdjLongEndings::CAdjLongEndings(CLexeme * pLexeme, ET_Subparadigm eSubparadigm)
: CEndings(pLexeme), m_eSubparadigm(eSubparadigm)
{}

ET_ReturnCode CAdjLongEndings::eLoad()
{
    ET_ReturnCode rc = H_NO_ERROR;

    CEString sSelect 
        (L"SELECT DISTINCT d.id, e.ending_string, d.gender, d.number, d.case_value, d.animacy, d.stress FROM endings AS e INNER JOIN ending_data AS d ON (e.id = d.ending_id)");
    sSelect += L" WHERE inflection_class = ";

    if (L"мс" == m_pLexeme->sInflectionType())
    {
        sSelect += CEString::sToString (ENDING_CLASS_PRONOUN);
    }
    else
    {
        sSelect += CEString::sToString (ENDING_CLASS_ADJECTIVE);
    }

    sSelect += L" AND inflection_type = ";
    if (SUBPARADIGM_PART_PRES_ACT == m_eSubparadigm ||
        SUBPARADIGM_PART_PAST_ACT == m_eSubparadigm)
    {
        sSelect += L"4";
    }
    else if (SUBPARADIGM_PART_PRES_PASS_LONG == m_eSubparadigm ||
             SUBPARADIGM_PART_PAST_PASS_LONG == m_eSubparadigm)
    {
        sSelect += L"1";
    }
    else
    {
        sSelect += CEString::sToString (m_pLexeme->iType());
    }
    sSelect += L";";

    CSqlite * pDb = NULL;

    try
    {
        pDb = m_pLexeme->pGetDb();
        pDb->PrepareForSelect (sSelect);
        while (pDb->bGetRow())
        {
            CEString sEnding;
            StEndingDescriptor stD;
            pDb->GetData (0, (__int64&)stD.llDbKey);
            pDb->GetData (1, sEnding);
            pDb->GetData (2, (int&)stD.eGender);
            pDb->GetData (3, (int&)stD.eNumber);
            pDb->GetData (4, (int&)stD.eCase);
            pDb->GetData (5, (int&)stD.eAnimacy);
            pDb->GetData (6, (int&)stD.eStress);
            rc = eAddEnding (sEnding, stD);
            if (rc != H_NO_ERROR)
            {
                return rc;
            }
        }
        pDb->Finalize();
    }
    catch (...)
    {
        ReportDbError();    
        return H_ERROR_DB; 
    }

    return rc;
        
}   //  eLoad()

int CAdjLongEndings::iHash (const StEndingDescriptor& stD)
{
    int iKey = stD.eGender * NUM_COUNT * CASE_COUNT * ANIM_COUNT * STRESS_LOCATION_COUNT +
               stD.eNumber * CASE_COUNT * ANIM_COUNT * STRESS_LOCATION_COUNT +
               stD.eCase * ANIM_COUNT * STRESS_LOCATION_COUNT +
               stD.eAnimacy * STRESS_LOCATION_COUNT +
               stD.eStress;

    return iKey;
}

ET_ReturnCode CAdjLongEndings::eAddEnding (const CEString& sEnding, const StEndingDescriptor& stDescriptor)
{
    //
    // Gender
    //
    std::vector<ET_Gender> vecGender;
    if (GENDER_UNDEFINED == stDescriptor.eGender)
    {
        for (int iGen = (int)GENDER_UNDEFINED; iGen < GENDER_COUNT; ++iGen)
        {
            vecGender.push_back ((ET_Gender)iGen);
        }
    }
    else
    {
        vecGender.push_back (stDescriptor.eGender);
    }

    //
    // Number
    //
    std::vector<ET_Number> vecNumber;
    if (NUM_UNDEFINED == stDescriptor.eNumber)
    {
        for (int iNum = (int)NUM_UNDEFINED; iNum < (int)NUM_COUNT; ++iNum)
        {
            vecNumber.push_back ((ET_Number)iNum);
        }
    }
    else
    {
        vecNumber.push_back (stDescriptor.eNumber);
    }

    //
    // Animacy
    //
    std::vector<ET_Animacy> vecAnimacy;
    if (ANIM_UNDEFINED == stDescriptor.eAnimacy)
    {
        for (int iAnim = ANIM_UNDEFINED; iAnim < (int)ANIM_COUNT; ++iAnim)
        {
            vecAnimacy.push_back ((ET_Animacy)iAnim);
        }
    }
    else
    {
        vecAnimacy.push_back (stDescriptor.eAnimacy);
    }

    //
    // Ending stressed/unstressed
    //
    std::vector<ET_StressLocation> vecStress;
    if (STRESS_LOCATION_UNDEFINED == stDescriptor.eStress)
    {
        for (int iStress = STRESS_LOCATION_UNDEFINED; 
             iStress < (int)STRESS_LOCATION_COUNT; 
             ++iStress)
        {
            vecStress.push_back ((ET_StressLocation)iStress);
        }
    }
    else
    {
        vecStress.push_back (stDescriptor.eStress);
    }

    for (int iG = 0; iG < (int)vecGender.size(); ++iG)
        for (int iN = 0; iN < (int)vecNumber.size(); ++iN)
            for (int iA = 0; iA < (int)vecAnimacy.size(); ++iA)
                for (int iS = 0; iS < (int)vecStress.size(); ++iS)
                {
                    StEndingDescriptor stD (vecGender[iG], 
                                            vecNumber[iN], 
                                            stDescriptor.eCase, 
                                            vecAnimacy[iA], 
                                            vecStress[iS]);
                    int iKey = iHash (stD);
                    std::pair<CEString, __int64> pairEndingData(sEnding, stDescriptor.llDbKey);
                    m_mmEndings.insert(std::pair<int, std::pair<CEString, __int64> >(iKey, pairEndingData));
                }

    return H_NO_ERROR;

}   //  CAdjLongEndings::eAddEnding (...)

////////////////////////////////////////////////////////////////////////////////////

CAdjShortEndings::CAdjShortEndings(CLexeme * pLexeme) : CEndings(pLexeme)
{}

ET_ReturnCode CAdjShortEndings::eLoad()
{
    ET_ReturnCode rc = H_NO_ERROR;

    CEString sSelect (L"SELECT DISTINCT d.id, e.ending_string, d.gender, d.number, d.stress FROM endings AS e INNER JOIN ending_data AS d ON (e.id = d.ending_id)");
    sSelect += L" WHERE inflection_class = ";
    sSelect += CEString::sToString (ENDING_CLASS_SHORT_ADJECTIVE);
    sSelect += L" AND inflection_type = ";
    sSelect += CEString::sToString (m_pLexeme->iType());
    sSelect += L";";

    CSqlite * pDb = NULL;

    try
    {
        pDb = m_pLexeme->pGetDb();
        pDb->PrepareForSelect (sSelect);
        while (pDb->bGetRow())
        {
            CEString sEnding;
            StEndingDescriptor stD;
            pDb->GetData (0, (__int64&)stD.llDbKey);
            pDb->GetData (1, sEnding);
            pDb->GetData (2, (int&)stD.eGender);
            pDb->GetData (3, (int&)stD.eNumber);
            pDb->GetData (4, (int&)stD.eStress);
            ET_ReturnCode rc = eAddEnding (sEnding, stD);
        }
        pDb->Finalize();
    }
    catch (...)
    {
        ReportDbError();    
        return H_ERROR_DB;
    }

    return rc;
        
}   //  eLoad()

int CAdjShortEndings::iHash (const StEndingDescriptor& stD)
{
    int iKey = stD.eGender * NUM_COUNT * STRESS_LOCATION_COUNT +
               stD.eNumber * STRESS_LOCATION_COUNT +
               stD.eStress;
    return iKey;
}

ET_ReturnCode CAdjShortEndings::eAddEnding (const CEString& sEnding, const StEndingDescriptor& stDescriptor)
{
    //
    // Gender
    //
    std::vector<ET_Gender> vecGender;
    if (GENDER_UNDEFINED == stDescriptor.eGender)
    {
        for (int iGen = (int)GENDER_UNDEFINED; iGen < GENDER_COUNT; ++iGen)
        {
            vecGender.push_back ((ET_Gender)iGen);
        }
    }
    else
    {
        vecGender.push_back (stDescriptor.eGender);
    }

    //
    // Number
    //
    if (NUM_UNDEFINED == stDescriptor.eNumber)
    {
        ERROR_LOG (L"Undefined number in short form ending.");
        return H_ERROR_INVALID_ARG;
    }

    //
    // Ending stressed/unstressed
    //
    std::vector<ET_StressLocation> vecStress;
    if (STRESS_LOCATION_UNDEFINED == stDescriptor.eStress)
    {
        for (int iStress = STRESS_LOCATION_UNDEFINED; 
             iStress < (int)STRESS_LOCATION_COUNT; 
             ++iStress)
        {
            vecStress.push_back ((ET_StressLocation)iStress);
        }
    }
    else
    {
        vecStress.push_back (stDescriptor.eStress);
    }

    for (int iG = 0; iG < (int)vecGender.size(); ++iG)
        for (int iS = 0; iS < (int)vecStress.size(); ++iS)
        {
            StEndingDescriptor stD (vecGender[iG], stDescriptor.eNumber, vecStress[iS]);
            int iKey = iHash (stD);
            std::pair<CEString, __int64> pairEndingData(sEnding, stD.llDbKey);
            m_mmEndings.insert(std::pair<int, std::pair<CEString, __int64> >(iKey, pairEndingData));
        }

    return H_NO_ERROR;

}   //  CAdjShortEndings::hAddEnding (...)


////////////////////////////////////////////////////////////////////////////////////

CPersonalEndings::CPersonalEndings(CLexeme * pLexeme) : CEndings(pLexeme)
{}

ET_ReturnCode CPersonalEndings::eLoad()
{
    ET_ReturnCode rc = H_NO_ERROR;

    if (POS_VERB != m_pLexeme->ePartOfSpeech())
    {
        ERROR_LOG (L"Bad part of speech value.");
        return H_ERROR_UNEXPECTED;
    }

    CEString sSelect 
        (L"SELECT DISTINCT d.id, e.ending_string, d.person, d.number, d.stress, d.stem_auslaut, d.conjugation FROM endings AS e INNER JOIN ending_data AS d ON (e.id = d.ending_id)");
    sSelect += L" WHERE inflection_class = " + CEString::sToString (ENDING_CLASS_PRESENT_TENSE);

    CSqlite * pDb = NULL;

    try
    {
        pDb = m_pLexeme->pGetDb();
        pDb->PrepareForSelect (sSelect);
        while (pDb->bGetRow())
        {
            CEString sEnding;
            StEndingDescriptor stD;
            pDb->GetData (0, (__int64&)stD.llDbKey);
            pDb->GetData (1, sEnding);
            pDb->GetData (2, (int&)stD.ePerson);
            pDb->GetData (3, (int&)stD.eNumber);
            pDb->GetData (4, (int&)stD.eStress);
            pDb->GetData (5, (int&)stD.eStemAuslaut);
            pDb->GetData (6, stD.iInflectionType);
            rc = eAddEnding (sEnding, stD);
        }
        pDb->Finalize();
    }
    catch (...)
    {
        ReportDbError();    
        rc = H_EXCEPTION;
    }

    return rc;

}   //  eLoad()

int CPersonalEndings::iHash (const StEndingDescriptor& stD)
{
    int iKey = stD.iInflectionType * PERSON_COUNT * NUM_COUNT * STRESS_LOCATION_COUNT * STEM_AUSLAUT_COUNT +
               stD.ePerson * NUM_COUNT * STRESS_LOCATION_COUNT * STEM_AUSLAUT_COUNT +
               stD.eNumber * STRESS_LOCATION_COUNT * STEM_AUSLAUT_COUNT +
               stD.eStress * STEM_AUSLAUT_COUNT + stD.eStemAuslaut;
    return iKey;
}

ET_ReturnCode CPersonalEndings::eAddEnding (const CEString& sEnding, const StEndingDescriptor& stDescriptor)
{
    //
    // Person
    //
    std::vector<ET_Person> vecPerson;
    if (PERSON_UNDEFINED == stDescriptor.ePerson)
    {
        ERROR_LOG (L"Undefined person.");
        return H_ERROR_INVALID_ARG;
    }
    else
    {
        vecPerson.push_back (stDescriptor.ePerson);
    }

    //
    // Number
    //
    if (NUM_UNDEFINED == stDescriptor.eNumber)
    {
        ERROR_LOG (L"Undefined number in personal ending.");
        return H_ERROR_INVALID_ARG;
    }

    //
    // Ending stressed/unstressed
    //
    std::vector<ET_StressLocation> vecStress;
    if (STRESS_LOCATION_UNDEFINED == stDescriptor.eStress)
    {
        for (int iStress = STRESS_LOCATION_UNDEFINED; 
             iStress < (int)STRESS_LOCATION_COUNT; 
             ++iStress)
        {
            vecStress.push_back ((ET_StressLocation)iStress);
        }
    }
    else
    {
        vecStress.push_back (stDescriptor.eStress);
    }

    //
    // Ending after sh/not after sh
    //
    std::vector<ET_StemAuslaut> vecStemAuslaut;
    if (STEM_AUSLAUT_UNDEFINED == stDescriptor.eStemAuslaut)
    {
        for (int iStemAuslaut = STEM_AUSLAUT_UNDEFINED; 
             iStemAuslaut < (int)STEM_AUSLAUT_COUNT; 
             ++iStemAuslaut)
        {
            vecStemAuslaut.push_back ((ET_StemAuslaut)iStemAuslaut);
        }
    }
    else
    {
        vecStemAuslaut.push_back (stDescriptor.eStemAuslaut);
    }

    for (int iS = 0; iS < (int)vecStress.size(); ++iS)
        for (int iSa = 0; iSa < (int)vecStemAuslaut.size(); ++iSa)
        {
            StEndingDescriptor stD (stDescriptor.iInflectionType,
                                    stDescriptor.ePerson, 
                                    stDescriptor.eNumber, 
                                    vecStress[iS], 
                                    vecStemAuslaut[iSa]);
            int iKey = iHash (stD);
            std::pair<CEString, __int64> pairEndingData(sEnding, stDescriptor.llDbKey);
            m_mmEndings.insert(std::pair<int, std::pair<CEString, __int64> >(iKey, pairEndingData));
        }

    return H_NO_ERROR;

}   //  CPersonalEndings::eAddEnding (...)
