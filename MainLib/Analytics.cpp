#include <set>
#include <vector>

#include "SqliteWrapper.h"
#include "WordForm.h"
#include "Parser.h"
#include "Analytics.h"

using namespace Hlib;

CAnalytics::CAnalytics() : m_pDb(nullptr), m_llTextDbId(-1) 
{};

CAnalytics::CAnalytics(CSqlite* pDb, CParser* pParser) : m_pDb(pDb), m_pParser(pParser), m_llTextDbId(-1)
{}

CAnalytics::~CAnalytics()
{}

ET_ReturnCode CAnalytics::eParseText(const CEString& sTextName, const CEString& sMetadata, const CEString& sText)
{
    ET_ReturnCode eRet = H_NO_ERROR;

    m_sTextName = sTextName;
    m_sTextMetaData = sMetadata;
    m_sText = sText;

    const_cast<CEString&>(sText).SetBreakChars(L"\r\n");
    const_cast<CEString&>(sText).SetTabs(L"");
    const_cast<CEString&>(sText).SetPunctuation(L"");
    const_cast<CEString&>(sText).SetEscapeChars(L"");

    CEString sQuery;

/*
    CREATE TABLE lines_in_text(id INTEGER PRIMARY KEY ASC, text_id INTEGER, line_number INTEGER, text_offset INTEGER, length INTEGER, number_of_words INTEGER, source TEXT, FOREIGN KEY(text_id) REFERENCES text(id) ON DELETE CASCADE);

    --word_text redundant, needed for testing
        CREATE TABLE words_in_line(id INTEGER PRIMARY KEY ASC, line_id INTEGER, word_position INTEGER, line_offset INTEGER, word_length INTEGER, word_text TEXT, FOREIGN KEY(line_id) REFERENCES lines_in_text(id) ON DELETE CASCADE);

    CREATE TABLE word_to_wordform(id INTEGER PRIMARY KEY ASC, word_in_line_id INTEGER, wordform_id INTEGER, FOREIGN KEY(word_in_line_id) REFERENCES words_in_line(id), FOREIGN KEY(wordform_id) REFERENCES wordforms(id) ON DELETE CASCADE);

    -- this table is added to simplify retrieval of transcriptionand stress positions
        CREATE TABLE tact_group(id INTEGER PRIMARY KEY ASC, line_id INTEGER, first_word_position INTEGER, num_of_words INTEGER, source TEXT, transcription TEXT, stressed_syllable INTEGER, reverse_stressed_syllable INTEGER, FOREIGN KEY(line_id) REFERENCES lines_in_text(id) ON DELETE CASCADE);

    CREATE TABLE word_to_tact_group(id INTEGER PRIMARY KEY ASC, word_to_wordform_id INTEGER, tact_group_id INTEGER, position_in_tact_group INTEGER, FOREIGN KEY(word_to_wordform_id) REFERENCES word_to_word_form(id) ON DELETE CASCADE);

*/

    if (NULL == m_pDb)
    {
        ERROR_LOG(L"No database access.");
        return H_ERROR_POINTER;
    }

    if (NULL == m_pParser)
    {
        ERROR_LOG(L"No parser object.");
        return H_ERROR_POINTER;
    }

    eRet = eRegisterText(sTextName, sMetadata, sText);
    if (eRet != H_NO_ERROR)
    {
        ERROR_LOG(L"Unable to register text");
        return eRet;
    }

    //
    // Parse text word by word
    //
    int iNLines = const_cast<CEString&>(sText).uiNFields();
    if (iNLines < 1)
    {
        return H_FALSE;
    }

    for (int iLine = 0; iLine < iNLines; ++iLine)
    {
        int iTextOffset = const_cast<CEString&>(sText).uiGetFieldOffset(iLine);
        
        CEString sLine = const_cast<CEString&>(sText).sGetField(iLine);
        sLine.EnableBreaks();
        sLine.EnablePunctuation();
        sLine.EnableEscapeChars();
        sLine.EnableTabs();
        sLine.EnableVowels();

        int iNWords = sLine.uiNFields();
        if (iNWords < 1)
        {
            continue;
        }

        long long llLineDbId = -1;
        eRet = eSaveLine(m_llTextDbId, iLine, iTextOffset, sLine.uiLength(), sLine.uiNFields(), sLine, llLineDbId);
        if (eRet != H_NO_ERROR)
        {
            continue;
        }

        m_mmapWordParses.clear();

        for (int iField = 0; iField < (int)sLine.uiNFields(); ++iField)
        {
            CEString sWord = sLine.sGetField(iField);
            sWord.ToLower();
            eRet = eParseWord(sWord, iLine, iField, llLineDbId);

        }       //  for (int iField = 0; iField < (int)sLine.uiNFields(); ++iField)

        eRet = eFindEquivalencies(sLine);
        if (eRet != H_NO_ERROR)
        {
            ERROR_LOG(L"Failed fo find equivalencies.");
            continue;
        }

        eRet = eAssembleTactGroups(sLine);

        for (auto itTg = m_mapTactGroups.begin(); itTg != m_mapTactGroups.end(); ++itTg)
        {
            eRet = eSaveTactGroup((*itTg).second);
        }

    }       //  for (int iLine = 0; iLine < iNLines; ++iLine)

    return H_NO_ERROR;

}       // eParseText()

void CAnalytics::ClearResults()
{}

//  CREATE TABLE text(id INTEGER PRIMARY KEY ASC, name TEXT, metadata TEXT, contents TEXT);
ET_ReturnCode CAnalytics::eRegisterText(const CEString& sTextName, const CEString sTextMetadata, const CEString& sText)
{
    ET_ReturnCode eRet = H_NO_ERROR;

    if (m_sTextName.bIsEmpty() || m_sTextMetaData.bIsEmpty())
    {
        ERROR_LOG(L"No text or text descriptor.");
        return H_ERROR_UNEXPECTED;
    }

    CEString sQuery = L"SELECT id FROM text WHERE name = '#NAME#' AND metadata = '#METADATA#';";
    sQuery = sQuery.sReplace(L"#NAME#", m_sTextName);
    sQuery = sQuery.sReplace(L"#METADATA#", m_sTextMetaData);

    vector<long long> vecTextIds;
    try
    {
        m_pDb->PrepareForSelect(sQuery);
        while (m_pDb->bGetRow())
        {
            long long llTextId = -1;
            m_pDb->GetData(0, llTextId);
            if (llTextId < 0)
            {
                ERROR_LOG(L"Illegal text id.");
                continue;
            }
            vecTextIds.push_back(llTextId);
        }
        m_pDb->Finalize();

        if (vecTextIds.size() > 1)
        {
            ERROR_LOG(L"Warning: multiple DB entries for the same text.");
        }

        for (long long llId : vecTextIds)
        {
            eRet = eClearTextData(llId);
            sQuery = L"DELETE FROM text WHERE id = ";
            sQuery += CEString::sToString(llId);
            m_pDb->Delete(sQuery);
        }
    }
    catch (CException& exc)
    {
        CEString sMsg(exc.szGetDescription());
        CEString sError;
        try
        {
            m_pDb->GetLastError(sError);
            sMsg += CEString(L", error: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(m_pDb->iGetLastError());
        ERROR_LOG(sMsg);
    }

    //
    // Create DB entry for the text
    //
    try
    {
        m_pDb->PrepareForInsert(L"text", 3);

        m_pDb->Bind(1, m_sTextName);
        m_pDb->Bind(2, m_sTextMetaData);
        m_pDb->Bind(3, m_sText);

        m_pDb->InsertRow();
        m_pDb->Finalize();

        m_llTextDbId = m_pDb->llGetLastKey();
    }
    catch (CException& exc)
    {
        CEString sMsg(exc.szGetDescription());
        CEString sError;
        try
        {
            m_pDb->GetLastError(sError);
            sMsg += CEString(L", error: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(m_pDb->iGetLastError());
        ERROR_LOG(sMsg);

        return H_ERROR_DB;
    }

    return eRet;

}       //  eRegisterText()

ET_ReturnCode CAnalytics::eParseWord(const CEString& sWord, int iLine, int iNumInLine, long long llLineDbKey)
{
    if (nullptr == m_pParser)
    {
        ERROR_LOG(L"Parser not available.");
        return H_ERROR_POINTER;
    }

    ET_ReturnCode eRet = H_NO_ERROR;

    int iOffset = m_sText.uiGetFieldOffset(iNumInLine);
    long long llWordInLineDbKey = -1;
    eRet = eSaveWord(llLineDbKey, iLine, iNumInLine, iOffset, sWord.uiLength(), sWord, llWordInLineDbKey);   // words_in_line
    if (eRet != H_NO_ERROR)
    {
        ERROR_LOG(L"Unable to save a word.");
        return eRet;
    }

    eRet = m_pParser->eParseWord(sWord);
    if (eRet != H_NO_ERROR)
    {
        return eRet;
    }

    CWordForm* pWf;
    eRet = m_pParser->eGetFirstWordForm(pWf);
    while (H_NO_ERROR == eRet)
    {
        long long llWordToWordFormId = -1;
        eRet = eSaveWordParse(llLineDbKey, pWf->llDbKey(), llWordToWordFormId);   //  word_to_wordform
        if (eRet != H_NO_ERROR)
        {
            ERROR_LOG(L"Unable to save a word parse.");
            continue;
        }

        StWordParse stParse;
        stParse.iNumber = iNumInLine;
        stParse.iLineOffset = iOffset;
        stParse.iLength = sWord.uiLength();
        stParse.llLineDbId = llLineDbKey;
        stParse.llWordInLIneDbId = llWordInLineDbKey;
        stParse.llWordToWordFormId = llWordToWordFormId;
        stParse.iPosInTactGroup = 0;
        stParse.WordForm = *pWf;
        m_mmapWordParses.insert(make_pair(iNumInLine, stParse));

        eRet = m_pParser->eGetNextWordForm(pWf);
    
    }       //  while...

    m_pParser->ClearResults();

    return H_NO_ERROR;
}

ET_ReturnCode CAnalytics::eFindEquivalencies(CEString& sLine)
{
    m_mmapEquivalencies.clear();

    for (int iField = 0; iField < (int)sLine.uiNFields(); ++iField)
    {
        auto it = m_mmapWordParses.find(iField);
        if (m_mmapWordParses.end() == it)
        {
            ERROR_LOG(L"Cant's find parse for a word.");
            continue;
        }

        // All parses for this word
        auto pairParses = m_mmapWordParses.equal_range(iField);
        if (pairParses.first == pairParses.second)
        {
            ERROR_LOG(L"Empty parse list.");
            continue;
        }

        // Find phonetic invariants for each word parse
        for (auto itParse = pairParses.first; itParse != pairParses.second; ++itParse)
        {
            bool bIsInvariant = false;

            auto pairEquivalentParses = m_mmapEquivalencies.equal_range((*itParse).first);
            for (auto itEquivalency = pairEquivalentParses.first;
                itEquivalency != pairEquivalentParses.second;
                ++itEquivalency)
            {
                StWordParse& stLeft = (*itParse).second;
                vector<StWordParse>& setEquivalencies = (*itEquivalency).second;
                if (setEquivalencies.empty())
                {
                    ERROR_LOG(L"Empty phonetical invariants list.");
                    continue;
                }

                StWordParse stRight = *(*itEquivalency).second.begin();      // any element will do
                if (bArePhoneticallyIdentical(stLeft.WordForm, stRight.WordForm))
                {
                    (*itEquivalency).second.push_back(stLeft);
                    bIsInvariant = true;
                    break;
                }
            }               //  for (auto itEquivalency...

            if (!bIsInvariant)
            {
                vector<StWordParse> s{ (*itParse).second };
                m_mmapEquivalencies.insert(make_pair((*itParse).first, s));
            }
        }       //  for...
    }

    return H_NO_ERROR;
}

ET_ReturnCode CAnalytics::eAssembleTactGroups(CEString& sLine)
{
    for (int iField = 0; iField < (int)sLine.uiNFields(); ++iField)
    {
        auto pairInvariants = m_mmapEquivalencies.equal_range(iField);
        for (auto itWordToParses = pairInvariants.first; itWordToParses != pairInvariants.second; ++itWordToParses)
        {
            vector<StWordParse>& vecVariants = (*itWordToParses).second;
            const StWordParse& stRandomVariant = *(vecVariants.begin());
            const CWordForm& wordForm = stRandomVariant.WordForm;

            StTactGroup stTg;
            stTg.iFirstWordNum = iField;
            stTg.iNumOfWords = 1;
            stTg.sSource = const_cast<CWordForm&>(wordForm).sWordForm();
            stTg.llLineId = stRandomVariant.llLineDbId;
            stTg.vecWords = vecVariants;

            if (bIsProclitic(wordForm))
            {
                // insert before 1st autonomously stressed word, ajust iFirstWordNum
            }

            if (bIsEnclitic(wordForm))
            {
            }

            m_mapTactGroups.insert(make_pair(iField, stTg));
        }
    }
            /*
                            // ASSERT(iWordPos == stWordParse.iNumber)
                            if (bIsProclitic(stWordParse.WordForm) || bIsEnclitic(stWordParse.WordForm))
                            {
                                continue;
                            }

                            if (iField > 0)
                            {
                                auto pairProcliticParses = m_mmapWordParses.equal_range(iField - 1);
                                for (auto itProcliticParse = pairProcliticParses.first; itProcliticParse != pairParses.second; ++itParse)
                                {
                                    StWordParse& stProcliticParse = (*itProcliticParse).second;
                                    if (bIsProclitic(stProcliticParse.WordForm))
                                    {
                                        stCurrentTactGroup.iFirstWordNum = stProcliticParse.iNumber;
                                        stCurrentTactGroup.iNumOfWords = 1;
                                        stCurrentTactGroup.sSource = stProcliticParse.WordForm.sWordForm();

                                        auto pairInvariants = m_mmapEquivalencies.equal_range(iField);
                                        for (auto itInv = pairInvariants.first; itInv != pairInvariants.second; ++itInv)
                                        {
                                            set<StWordParse>& setInvariants = (*itInv).second;
                                            if (setInvariants.find(stProcliticParse) != setInvariants.end())
                                            {
                                                stCurrentTactGroup.setWords = setInvariants;
                                            }
                                        }
                                        //&&&&
                                        //set<StWordParse>& setInvariants =

            //                            stCurrentTactGroup.setWords &&&&
                                    }
                                }
                                StWordParse stPreviousWord;

                                vecTactGroups.push_back(stCurrentTactGroup);
                                stCurrentTactGroup.Reset();
                                stCurrentTactGroup.llLineId = llLineDbId;
                                stCurrentTactGroup.iFirstWordNum = stWordParse.iNumber;
                            }
                            
            

            stWordParse.iPosInTactGroup = stCurrentTactGroup.iNumOfWords;
            //                stCurrentTactGroup.vecWords.push_back(stWordParse);
            if (stCurrentTactGroup.iNumOfWords > 0)
            {
                stCurrentTactGroup.sSource += L" ";
            }
            stCurrentTactGroup.sSource += stWordParse.WordForm.sWordForm();

            ET_StressType eType = ET_StressType::STRESS_TYPE_UNDEFINED;
            int iStressPos = -1;
            eRet = stWordParse.WordForm.eGetFirstStressPos(iStressPos, eType);
            if (eRet != H_NO_ERROR)
            {
                CEString sMsg(L"Unable to get stress position, error ");
                sMsg += CEString::sToString(eRet);
                ERROR_LOG(sMsg);
            }

            if (ET_StressType::STRESS_PRIMARY == eType)
            {
                stCurrentTactGroup.iStressPos = iStressPos;
            }
            else if (ET_StressType::STRESS_SECONDARY == eType)
            {
                stCurrentTactGroup.iSecondaryStressPos = iStressPos;
            }
            else
            {
                CEString sMsg(L"Unknown stress type.");
                ERROR_LOG(sMsg);
            }

            while (H_NO_ERROR == eRet)
            {
                eRet = stWordParse.WordForm.eGetNextStressPos(iStressPos, eType);
                if (eRet != H_NO_ERROR && eRet != H_NO_MORE)
                {
                    CEString sMsg(L"Unable to get next stress position, error ");
                    sMsg += CEString::sToString(eRet);
                    ERROR_LOG(sMsg);
                }
                if (ET_StressType::STRESS_PRIMARY == eType)
                {
                    if (-1 == stCurrentTactGroup.iStressPos)
                    {
                        stCurrentTactGroup.iStressPos = iStressPos;
                    }
                    else
                    {
                        ERROR_LOG(L"Multiple stress positions.");
                    }
                }
                else if (ET_StressType::STRESS_SECONDARY == eType)
                {
                    if (-1 == stCurrentTactGroup.iSecondaryStressPos)
                    {
                        stCurrentTactGroup.iSecondaryStressPos = iStressPos;
                    }
                    else
                    {
                        ERROR_LOG(L"Multiple secondary stress positions.");
                    }
                }
                else
                {
                    CEString sMsg(L"Unknown stress type.");
                    ERROR_LOG(sMsg);
                }
            }

            ++stCurrentTactGroup.iNumOfWords;
        }
    }       //  for (auto iWordPos : vecWordPos)

    eRet = eSaveTactGroups(vecTactGroups);
    if (eRet != H_NO_ERROR)
    {
        CEString sMsg(L"Unable to save tact group, error ");
        sMsg += CEString::sToString(eRet);
        ERROR_LOG(sMsg);
    }
}       //  for (int iLine = 0; iLine < iNLines; ++iLine)
*/
return H_NO_ERROR;

}       //  eAssembleTactGroups()

//  CREATE TABLE lines_in_text(id INTEGER PRIMARY KEY ASC, text_id INTEGER, line_number INTEGER, text_offset INTEGER, number_of_words INTEGER, 
//  source TEXT, FOREIGN KEY(text_id) REFERENCES text(id));
ET_ReturnCode CAnalytics::eSaveLine(long long llTextId, int iLineNum, int iTextOffset, int iLength, int iNumOfWords, const CEString& sText, long long& llDbKey)
{
    try
    {
        if (NULL == m_pDb)
        {
            ERROR_LOG(L"No database access.");
            return H_ERROR_POINTER;
        }

        m_pDb->PrepareForInsert(L"lines_in_text", 6);

        m_pDb->Bind(1, m_llTextDbId);
        m_pDb->Bind(2, iLineNum);
        m_pDb->Bind(3, iTextOffset);
        m_pDb->Bind(4, iLength);
        m_pDb->Bind(5, iNumOfWords);
        m_pDb->Bind(6, sText);

        m_pDb->InsertRow();
        m_pDb->Finalize();

        llDbKey = m_pDb->llGetLastKey();

    }
    catch (CException& exc)
    {
        CEString sMsg(exc.szGetDescription());
        CEString sError;
        try
        {
            m_pDb->GetLastError(sError);
            sMsg += CEString(L", error: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(m_pDb->iGetLastError());
        ERROR_LOG(sMsg);

        return H_ERROR_GENERAL;
    }

    return H_NO_ERROR;
}

//  CREATE TABLE words_in_line(id INTEGER PRIMARY KEY ASC, line_id INTEGER, word_position INTEGER, line_offset INTEGER, word_length INTEGER, 
//  word_text TEXT, FOREIGN KEY(line_id) REFERENCES lines_in_text(id));
ET_ReturnCode CAnalytics::eSaveWord(long long llLineDbId, int iLine, int iWord, int iLineOffset, int iSegmentLength, const CEString& sWord, long long& llWordDbKey)
{
    try
    {
        if (NULL == m_pDb)
        {
            ERROR_LOG(L"No database access.");
            return H_ERROR_POINTER;
        }

        if (m_sTextName.bIsEmpty() || m_sTextMetaData.bIsEmpty())
        {
            ERROR_LOG(L"No text or text descriptor.");
            return H_ERROR_UNEXPECTED;
        }

        m_pDb->PrepareForInsert(L"words_in_line", 5);

        m_pDb->Bind(1, llLineDbId);
        m_pDb->Bind(2, iWord);
        m_pDb->Bind(3, iLineOffset);
        m_pDb->Bind(4, iSegmentLength);
        m_pDb->Bind(5, sWord);

        m_pDb->InsertRow();
        m_pDb->Finalize();

        llWordDbKey = m_pDb->llGetLastKey();
    }
    catch (CException& exc)
    {
        CEString sMsg(exc.szGetDescription());
        CEString sError;
        try
        {
            m_pDb->GetLastError(sError);
            sMsg += CEString(L", error: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(m_pDb->iGetLastError());
        ERROR_LOG(sMsg);

        return H_ERROR_GENERAL;
    }

    return H_NO_ERROR;

}       // eSaveWord()

//  CREATE TABLE word_to_wordform(id INTEGER PRIMARY KEY ASC, word_in_line_id INTEGER, wordform_id INTEGER, 
//  FOREIGN KEY(word_in_line_id) REFERENCES words_in_line(id), FOREIGN KEY(wordform_id) REFERENCES wordforms(id));
ET_ReturnCode CAnalytics::eSaveWordParse(long long llWordId, long long llWordFormId, long long& llWordToWordFormId)
{
    if (NULL == m_pDb)
    {
        ERROR_LOG(L"No database access.");
        return H_ERROR_POINTER;
    }

    try
    {
        m_pDb->PrepareForInsert(L"word_to_wordform", 2);

        m_pDb->Bind(1, llWordId);
        m_pDb->Bind(2, llWordFormId);

        m_pDb->InsertRow();
        m_pDb->Finalize();

        llWordToWordFormId = m_pDb->llGetLastKey();
    }
    catch (CException& exc)
    {
        CEString sMsg(exc.szGetDescription());
        CEString sError;
        try
        {
            m_pDb->GetLastError(sError);
            sMsg += CEString(L", error: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(m_pDb->iGetLastError());
        ERROR_LOG(sMsg);

        return H_ERROR_GENERAL;
    }

    return H_NO_ERROR;

}       // eSaveWordParse()

bool CAnalytics::bIsProclitic(const CWordForm& WordForm)
{
    return false;       // TODO: implement
}

bool CAnalytics::bIsEnclitic(const CWordForm& pWordForm)
{
    return false;       // TODO: implement
}

bool CAnalytics::bArePhoneticallyIdentical(CWordForm& wf1, CWordForm& wf2)
{
    if (wf1.sWordForm() != wf2.sWordForm())
    {
        return false;
    }

    int iStressPos1, iStressPos2 = -1;
    ET_StressType eStressType1, eStressType2 = STRESS_TYPE_UNDEFINED;
    auto eRet1 = wf1.eGetFirstStressPos(iStressPos1, eStressType1);
    auto eRet2 = wf2.eGetFirstStressPos(iStressPos2, eStressType2);
    if (H_FALSE == eRet1 && H_FALSE == eRet2)       //  both are unstressed
    {
        return true;
    }

    if (eRet1 != H_NO_ERROR || eRet2 != H_NO_ERROR)
    {
        ASSERT(0);
        ERROR_LOG(L"Error getting 1st stress position.");
        return false;
    }

    if (eStressType1 != eStressType2 || iStressPos1 != iStressPos2)
    {
        return false;
    }

    for (int iCount = 0; iCount < 100; ++iCount)        // arbitraty max to avoid infinite loop
    {
        eRet1 = wf1.eGetNextStressPos(iStressPos1, eStressType1);
        eRet2 = wf2.eGetNextStressPos(iStressPos2, eStressType2);

        //  This is the most common scenario
        if (H_NO_MORE == eRet1 && H_NO_MORE == eRet2)
        {
            return true;
        }

        if ((eRet1 != H_NO_ERROR && eRet1 != H_NO_MORE) || (eRet2 != H_NO_ERROR && eRet2 != H_NO_MORE))
        {
            ASSERT(0);
            ERROR_LOG(L"Error getting 1st stress position.");
            return false;
        }

        if (H_NO_ERROR != eRet1 || H_NO_ERROR != eRet2)
        {
            return false;
        }

        if (eStressType1 != eStressType2 || iStressPos1 != iStressPos2)
        {
            return false;
        }
    }

    ASSERT(0);
    ERROR_LOG(L"Error getting next stress position.");

    return false;

}       //  bArePhoneticallyIdentical()

// Save tact groups for a line
//  CREATE TABLE tact_group(id INTEGER PRIMARY KEY ASC, line_id INTEGER, first_word_position INTEGER, 
//  num_of_words INTEGER, source TEXT, transcription TEXT, stressed_syllable INTEGER, 
//  reverse_stressed_syllable INTEGER, SECONDARY_STRESSED_SYLLABLE, FOREIGN KEY(line_id) 
//  REFERENCES lines_in_text(id));
ET_ReturnCode CAnalytics::eSaveTactGroup(StTactGroup& stTg)
{
    if (NULL == m_pDb)
    {
        ERROR_LOG(L"No database access.");
        return H_ERROR_POINTER;
    }

    try
    {
        m_pDb->PrepareForInsert(L"tact_group", 7);

        m_pDb->Bind(1, stTg.llLineId);
        m_pDb->Bind(2, stTg.iFirstWordNum);
        m_pDb->Bind(3, stTg.iNumOfWords);
        m_pDb->Bind(4, stTg.sSource);
        m_pDb->Bind(5, stTg.sTranscription);
        m_pDb->Bind(6, stTg.iStressPos);
        m_pDb->Bind(7, stTg.iReverseStressPos);

        m_pDb->InsertRow();
        m_pDb->Finalize();

        long long llTactGroupId = m_pDb->llGetLastKey();

        // CREATE TABLE word_to_tact_group(id INTEGER PRIMARY KEY ASC, word_to_wordform_id INTEGER, 
        // tact_group_id INTEGER, position_in_tact_group INTEGER, FOREIGN KEY(word_to_wordform_id) 
        // REFERENCES word_to_word_form(id));

        unsigned long long llInsertHandle = 0;
        m_pDb->uiPrepareForInsert(L"word_to_tact_group", 3, (sqlite3_stmt * &)llInsertHandle);
        for (StWordParse& stWordParse : stTg.vecWords)
        {
            m_pDb->Bind(1, stWordParse.llWordToWordFormId, llInsertHandle);
            m_pDb->Bind(2, llTactGroupId, llInsertHandle);
            m_pDb->Bind(3, stWordParse.iPosInTactGroup, llInsertHandle);
            m_pDb->InsertRow(llInsertHandle);
        }
        m_pDb->Finalize(llInsertHandle);
    }
    catch (CException& exc)
    {
        CEString sMsg(exc.szGetDescription());
        CEString sError;
        try
        {
            m_pDb->GetLastError(sError);
            sMsg += CEString(L", error: ");
            sMsg += sError;
        }
        catch (...)
        {
            sMsg = L"Apparent DB error ";
        }

        sMsg += CEString::sToString(m_pDb->iGetLastError());
        ERROR_LOG(sMsg);

        return H_ERROR_DB;
    }

    return H_NO_ERROR;

}       //  eSaveTactGroups()

ET_ReturnCode CAnalytics::eClearTextData(long long llTextId)
{
    if (!m_pDb)
    {
        return H_ERROR_DB;
    }

    ET_ReturnCode eRet = H_NO_ERROR;

    CEString sQuery;
    vector<long long> vecLineIds;

    // Get Line Id's
    sQuery = L"SELECT id FROM lines_in_text WHERE text_id = ";
    sQuery += CEString::sToString(llTextId);

    try
    {
        m_pDb->PrepareForSelect(sQuery);
        while (m_pDb->bGetRow())
        {
            long long llLineId = -1;
            m_pDb->GetData(0, llLineId);
            vecLineIds.push_back(llLineId);
        }
        m_pDb->Finalize();

        vector<long long> vecWordsInLineIds;
        for (auto llLineId : vecLineIds)
        {
            sQuery = L"SELECT id FROM words_in_line WHERE line_id = ";
            sQuery += CEString::sToString(llLineId);
            m_pDb->PrepareForSelect(sQuery);
            while (m_pDb->bGetRow())
            {
                long long llWordInLineId = -1;
                m_pDb->GetData(0, llWordInLineId);
                vecWordsInLineIds.push_back(llWordInLineId);
            }
            m_pDb->Finalize();
        }

        vector<long long> vecWordToWordFormIds;
        for (auto llWordInLineId : vecWordsInLineIds)
        {
            sQuery = L"SELECT id FROM word_to_wordform WHERE word_in_line_id = ";
            sQuery += CEString::sToString(llWordInLineId);
            m_pDb->PrepareForSelect(sQuery);
            while (m_pDb->bGetRow())
            {
                long long llWordToWordFormId = -1;
                m_pDb->GetData(0, llWordToWordFormId);
                vecWordToWordFormIds.push_back(llWordToWordFormId);
            }
            m_pDb->Finalize();
        }

        vector<long long> vecTactGroupIds;
        for (auto llLineId : vecLineIds)
        {
            sQuery = L"SELECT id FROM tact_group WHERE line_id = ";
            sQuery += CEString::sToString(llLineId);
            m_pDb->PrepareForSelect(sQuery);
            while (m_pDb->bGetRow())
            {
                long long llTactGroupId = -1;
                m_pDb->GetData(0, llTactGroupId);
                vecTactGroupIds.push_back(llTactGroupId);
            }
            m_pDb->Finalize();
        }

        vector<long long> vecWordToTactGroupIds;
        for (auto llTactGroupId : vecTactGroupIds)
        {
            sQuery = L"SELECT id FROM word_to_tact_group WHERE tact_group_id = ";
            sQuery += CEString::sToString(llTactGroupId);
            m_pDb->PrepareForSelect(sQuery);
            while (m_pDb->bGetRow())
            {
                long long llWordToTactGroupId = -1;
                m_pDb->GetData(0, llWordToTactGroupId);
                vecWordToTactGroupIds.push_back(llWordToTactGroupId);
            }
            m_pDb->Finalize();
        }

        for (auto llWordToTactGroupId : vecWordToTactGroupIds)
        {
            sQuery = L"DELETE FROM word_to_tact_group WHERE id = ";
            sQuery += CEString::sToString(llWordToTactGroupId);
            m_pDb->Delete(sQuery);
        }

        for (auto llTactGroupId : vecTactGroupIds)
        {
            sQuery = L"DELETE FROM tact_group WHERE id = ";
            sQuery += CEString::sToString(llTactGroupId);
            m_pDb->Delete(sQuery);
        }

        for (auto llWordFormId : vecWordToWordFormIds)
        {
            sQuery = L"DELETE FROM word_to_wordform WHERE id = ";
            sQuery += CEString::sToString(llWordFormId);
            m_pDb->Delete(sQuery);
        }

        for (auto llWordInLineId : vecWordsInLineIds)
        {
            sQuery = L"DELETE FROM words_in_line WHERE id = ";
            sQuery += CEString::sToString(llWordInLineId);
            m_pDb->Delete(sQuery);
        }

        for (auto llLineId : vecLineIds)
        {
            sQuery = L"DELETE FROM lines_in_text WHERE id = ";
            sQuery += CEString::sToString(llLineId);
            m_pDb->Delete(sQuery);
        }
    }
    catch (CException& ex)
    {
        CEString sMsg(L"Warning: Unable to read db, exception: ");
        sMsg += ex.szGetDescription();
        ERROR_LOG(sMsg);
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}       //  eClearTextData()
