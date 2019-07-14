// Parse.cpp : Implementation of CParser

#include <ctime>
#include <algorithm>

#include "SqliteWrapper.h"
#include "Endings.h"
#include "WordForm.h"
#include "Parser.h"
#include "ParsingTree.h"

using namespace Hlib;

CParser::CParser(CSqlite * pDb) : m_pDb(pDb), m_pEndingsTree(NULL)
{}

CParser::~CParser()
{
    ClearResults();
}

ET_ReturnCode CParser::eParseWord(const CEString& sWord)
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

    eRemoveFalsePositives();

    return m_vecWordForms.empty() ? H_FALSE : H_NO_ERROR;

}       // eParseWord()

ET_ReturnCode CParser::eParseText(const CEString& sTextName, const CEString& sMetadata, const CEString& sText)
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
        CREATE TABLE text(id INTEGER PRIMARY KEY ASC, name TEXT, metadata TEXT, contents TEXT);

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

    if (m_sTextName.bIsEmpty() || m_sTextMetaData.bIsEmpty())
    {
        ERROR_LOG(L"No text or text descriptor.");
        return H_ERROR_UNEXPECTED;
    }

    sQuery = L"SELECT id FROM text WHERE name = '#NAME#' AND metadata = '#METADATA#';";
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

        StTactGroup stCurrentTactGroup;
        multimap<int, StWordParse> mmapWordParses;
        vector<StTactGroup> vecTactGroups;
        for (int iField = 0; iField < (int)sLine.uiNFields(); ++iField)
        {
            CEString sWord = sLine.sGetField(iField);
            sWord.ToLower();

            eRet = eParseWord(sWord);

            int iOffset = m_sText.uiGetFieldOffset(iField);
            long long llDbKey = -1;
            eRet = eSaveWord(llLineDbId, iLine, iField, iOffset, sWord.uiLength(), sWord, llDbKey);   // words_in_line
            if (eRet != H_NO_ERROR)
            {
                ERROR_LOG(L"Unable to save a word.");
                continue;
            }

            for (auto pWf : m_vecWordForms)
            {
                // Save in the DB ...
                long long llWordToWordFormId = -1;
                eRet = eSaveWordParse(llDbKey, pWf->llDbKey(), llWordToWordFormId);   //  word_to_wordform
                if (eRet != H_NO_ERROR)
                {
                    ERROR_LOG(L"Unable to save a word parse.");
                    continue;
                }

                //  ... and store locally 
                StWordParse wp;
                wp.iNumber = iField;
                wp.iLineOffset = iOffset;
                wp.iLength = sWord.uiLength();
                wp.llLineDbId = llLineDbId;
                wp.llWordFormDbId = llDbKey;
                wp.pWordForm = pWf;
                wp.llWordToWordFormId = llWordToWordFormId;
                wp.iPosInTactGroup = 0;
                mmapWordParses.insert(make_pair(iField, wp));
                
            }       //  for (auto pWf : m_vecWordForms)
        }       //  for (int iField = 0; iField < (int)sLine.uiNFields(); ++iField)

        //
        //  Assemble tact groups
        //
        stCurrentTactGroup.Reset();
        stCurrentTactGroup.llLineId = llLineDbId;
        stCurrentTactGroup.iFirstWordNum = 0;

        for (auto it = mmapWordParses.begin(); 
                it != mmapWordParses.end(); 
                it = mmapWordParses.upper_bound(it->first))
        {
            auto pairParses = mmapWordParses.equal_range(it->first); // all parses for this word (usually just one)
            for (auto itParse = pairParses.first; itParse != pairParses.second; ++itParse)
            {
                StWordParse stWordParse = itParse->second;
                if (itParse->first >= (int)sLine.uiNFields())
                {
                    ERROR_LOG(L"Illegal word position.");
                    continue;
                }

                // ASSERT(iWordPos == stWordParse.iNumber)
                if (stWordParse.iNumber > 0 && stCurrentTactGroup.iNumOfWords > 0)
                {
                    //  ASSERT(stCurrentTactGroup.vecWords.size() > 0)
                    //  ASSERT(iWordPos < (int)sLine.uiNFields())
                    StWordParse& stPreviousParse = stCurrentTactGroup.vecWords.back();
                    if (!(bIsProclitic(stPreviousParse.pWordForm) || bIsEnclitic(stWordParse.pWordForm)))
                    {
                        //  Autonomous stressed word not after proclitic
                        vecTactGroups.push_back(stCurrentTactGroup);
                        stCurrentTactGroup.Reset();
                        stCurrentTactGroup.llLineId = llLineDbId;
                        stCurrentTactGroup.iFirstWordNum = stWordParse.iNumber;
                    }
                }

                stWordParse.iPosInTactGroup = stCurrentTactGroup.iNumOfWords;
                stCurrentTactGroup.vecWords.push_back(stWordParse);
                if (stCurrentTactGroup.iNumOfWords > 0)
                {
                    stCurrentTactGroup.sSource += L" ";
                }
                stCurrentTactGroup.sSource += stWordParse.pWordForm->sWordForm();

                ET_StressType eType = ET_StressType::STRESS_TYPE_UNDEFINED;
                int iStressPos = -1;
                eRet = stWordParse.pWordForm->eGetFirstStressPos(iStressPos, eType);
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
                    eRet = stWordParse.pWordForm->eGetNextStressPos(iStressPos, eType);
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

    return H_NO_ERROR;

}       // eParseText()

// Save tact groups for a line
//  CREATE TABLE tact_group(id INTEGER PRIMARY KEY ASC, line_id INTEGER, first_word_position INTEGER, 
//  num_of_words INTEGER, source TEXT, transcription TEXT, stressed_syllable INTEGER, 
//  reverse_stressed_syllable INTEGER, SECONDARY_STRESSED_SYLLABLE, FOREIGN KEY(line_id) 
//  REFERENCES lines_in_text(id));
ET_ReturnCode CParser::eSaveTactGroups(vector<StTactGroup>& vecTactGroups)
{
    try
    { 
        for (auto stTactGroup : vecTactGroups)
        {
            if (NULL == m_pDb)
            {
                ERROR_LOG(L"No database access.");
                return H_ERROR_POINTER;
            }

            m_pDb->PrepareForInsert(L"tact_group", 7);

            m_pDb->Bind(1, stTactGroup.llLineId);
            m_pDb->Bind(2, stTactGroup.iFirstWordNum);
            m_pDb->Bind(3, stTactGroup.iNumOfWords);
            m_pDb->Bind(4, stTactGroup.sSource);
            m_pDb->Bind(5, stTactGroup.sTranscription);
            m_pDb->Bind(6, stTactGroup.iStressPos);
            m_pDb->Bind(7, stTactGroup.iReverseStressPos);

            m_pDb->InsertRow();
            m_pDb->Finalize();

            long long llTactGroupId = m_pDb->llGetLastKey();

            // CREATE TABLE word_to_tact_group(id INTEGER PRIMARY KEY ASC, word_to_wordform_id INTEGER, 
            // tact_group_id INTEGER, position_in_tact_group INTEGER, FOREIGN KEY(word_to_wordform_id) 
            // REFERENCES word_to_word_form(id));
            m_pDb->PrepareForInsert(L"word_to_tact_group", 3);
            for (auto stWordParse : stTactGroup.vecWords)
            {                
                m_pDb->Bind(1, stWordParse.llWordToWordFormId);
                m_pDb->Bind(2, llTactGroupId);
                m_pDb->Bind(3, stWordParse.iPosInTactGroup);
                m_pDb->InsertRow();
            }
            m_pDb->Finalize();
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

        return H_ERROR_DB;
    }

    return H_NO_ERROR;

}       //  eSaveTactGroups()

ET_ReturnCode CParser::eClearTextData(long long llTextId)
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

// Remove regular forms mistakingly generated by the engine when
// an irregular form exists in the db, regular form with the same 
// gramm hash must be ignored
// TODO: variants!
ET_ReturnCode CParser::eRemoveFalsePositives()
{
    using pwfIterator = vector<CWordForm*>::iterator;

    set<pwfIterator> setFalsePositives;
    for (auto pMe : m_vecWordForms)
    {
        if (!pMe->bIrregular())
        {
            continue;
        }

        for (pwfIterator itOther = m_vecWordForms.begin(); itOther != m_vecWordForms.end(); ++itOther)
        {
            if (pMe->llLexemeId() == (*itOther)->llLexemeId())
            {
                if (pMe->sGramHash() == (*itOther)->sGramHash())
                {
                    if (!(*itOther)->bIrregular())
                    {
                        setFalsePositives.insert(itOther);
                        break;
                    }
                }
            }
        }
    }

    for (auto itFalsePositive : setFalsePositives)
    {
        m_vecWordForms.erase(itFalsePositive);
    }

    return H_NO_ERROR;

}       //  eRemoveFalsePositives()

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

void CParser::SetDb(CSqlite * pDb)
{
    m_pDb = pDb;
}


//
// Helpers
//

ET_ReturnCode CParser::eQueryDb(const CEString& sSelect, uint64_t& uiQueryHandle)
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
        uint64_t uiFormQueryHandle = m_pDb->uiPrepareForSelect(sIrregFormQuery);
        while (m_pDb->bGetRow(uiFormQueryHandle))
        {
            long long llFormId = -1;
            m_pDb->GetData(0, llFormId, uiFormQueryHandle);

            long long llDescriptorId = -1;
            m_pDb->GetData(1, llDescriptorId, uiFormQueryHandle);

            CEString sGramHash = -1;
            m_pDb->GetData(2, sGramHash, uiFormQueryHandle);

            CEString sWordForm;
            sWordForm.SetVowels(g_szRusVowels);
            m_pDb->GetData(3, sWordForm, uiFormQueryHandle);

            bool bIsAlternative = false;
            m_pDb->GetData(4, bIsAlternative, uiFormQueryHandle);       // what to do with this one?

            CWordForm * pWf = new CWordForm(sGramHash);
            pWf->m_bIrregular = true;
            pWf->m_llLexemeId = llDescriptorId;
            pWf->m_llIrregularFormId = llFormId;
            pWf->m_sWordForm = sWordForm;

            CEString sIrregStressQuery(L"SELECT position, is_primary FROM irregular_stress WHERE form_id = \"");
            sIrregStressQuery += CEString::sToString(llFormId);
            sIrregStressQuery += L'\"';
            uint64_t uiStressHandle = m_pDb->uiPrepareForSelect(sIrregStressQuery);
            while (m_pDb->bGetRow(uiStressHandle))
            {
                int iPosition = -1;
                m_pDb->GetData(0, iPosition, uiStressHandle);

                ET_StressType eType = STRESS_TYPE_UNDEFINED;
                m_pDb->GetData(1, (int&)eType, uiStressHandle);
                int iStressedSyll = sWordForm.uiGetSyllableFromVowelPos(iPosition);
                pWf->m_mapStress[iStressedSyll] = eType;
            }
            m_pDb->Finalize(uiStressHandle);

            m_vecWordForms.push_back(pWf);
        }
        m_pDb->Finalize(uiFormQueryHandle);
    }
    catch (CException& ex)
    {
        CEString sMsg(L"Irregular form lookup error: ");
        sMsg += ex.szGetDescription();
        ERROR_LOG(sMsg);
        return H_EXCEPTION;
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
    uint64_t uiFormQueryHandle = m_pDb->uiPrepareForSelect(sQuery);
    while (m_pDb->bGetRow(uiFormQueryHandle))
    {
        CEString sGramHash;
        m_pDb->GetData(0, sGramHash, uiFormQueryHandle);

        long long llLexemeId = -1;
        m_pDb->GetData(1, llLexemeId, uiFormQueryHandle);

        long long llFormId = -1;
        m_pDb->GetData(2, llFormId, uiFormQueryHandle);

        long long llEndingId = -1;
        m_pDb->GetData(3, llEndingId, uiFormQueryHandle);

        if (llEndingId < 0 || H_TRUE == m_pEndingsTree->eIsEmptyEnding(llEndingId))
        {
            CWordForm * pWf = NULL;
            
            try
            {
                pWf = new CWordForm(sGramHash);
            }
            catch (CException& ex)
            {
                CEString sMsg (L"Zal exception, error code = ");
                sMsg += CEString::sToString(ex.iGetErrorCode());
                ERROR_LOG(sMsg);

                return H_ERROR_GENERAL;
            }
            catch (...)
            {
                CEString sMsg(L"Exception, error code = ");
                sMsg += CEString::sToString(m_pDb->iGetLastError());
                ERROR_LOG(sMsg);

                return H_ERROR_GENERAL;
            }

            pWf->m_bIrregular = false;
            pWf->m_llLexemeId = llLexemeId;
            pWf->m_sWordForm = sWord;

            CEString sStressQuery(L"SELECT position, is_primary FROM stress_data WHERE form_id = \"");
            sStressQuery += CEString::sToString(llFormId);
            sStressQuery += L'\"';
            uint64_t uiStressHandle = m_pDb->uiPrepareForSelect(sStressQuery);
            while (m_pDb->bGetRow(uiStressHandle))
            {
                int iPosition = -1;
                m_pDb->GetData(0, iPosition, uiStressHandle);

                bool bIsPrimary = false;
                m_pDb->GetData(1, bIsPrimary, uiStressHandle);
                ET_StressType eType = bIsPrimary ? STRESS_PRIMARY : STRESS_SECONDARY;
                pWf->m_mapStress[iPosition] = eType;
            }
            m_pDb->Finalize(uiStressHandle);

            m_vecWordForms.push_back(pWf);
        }
    }       //  while (m_pDb->bGetRow(...)) 
    m_pDb->Finalize(uiFormQueryHandle);

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

                uint64_t uiFormQueryHandle = m_pDb->uiPrepareForSelect(sQuery);

                while (m_pDb->bGetRow(uiFormQueryHandle))
                {
                    long long llFormId = -1;
                    m_pDb->GetData(0, llFormId, uiFormQueryHandle);

                    CEString sGramHash = -1;
                    m_pDb->GetData(1, sGramHash, uiFormQueryHandle);

                    long long llLexemeId = -1;
                    m_pDb->GetData(2, llLexemeId, uiFormQueryHandle);

                    CWordForm * pWf = new CWordForm(sGramHash);
                    pWf->m_llDbKey = llFormId;
                    pWf->m_bIrregular = false;
                    pWf->m_llLexemeId = llLexemeId;
                    pWf->m_sWordForm = sWord;

                    CEString sStressQuery(L"SELECT position, is_primary FROM stress_data WHERE form_id = \"");
                    sStressQuery += CEString::sToString(llFormId);
                    sStressQuery += L'\"';

                    uint64_t uiStressHandle = m_pDb->uiPrepareForSelect(sStressQuery);
                    while (m_pDb->bGetRow(uiStressHandle))
                    {
                        int iPosition = -1;
                        m_pDb->GetData(0, iPosition, uiStressHandle);

                        bool bIsPrimary = false;
                        m_pDb->GetData(1, bIsPrimary, uiStressHandle);
                        ET_StressType eType = STRESS_TYPE_UNDEFINED;
                        pWf->m_mapStress[iPosition] = bIsPrimary ? STRESS_PRIMARY : STRESS_SECONDARY;
                    }
                    m_pDb->Finalize(uiStressHandle);
                    m_vecWordForms.push_back(pWf);

                }       //  while (m_pDb->bGetRow(...))
                m_pDb->Finalize(uiFormQueryHandle);
            }
            catch (CException& ex)
            {
                CEString sMsg(L"Parse error: ");
                sMsg += ex.szGetDescription();
                ERROR_LOG(sMsg);
                return H_EXCEPTION;
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

//  CREATE TABLE lines_in_text(id INTEGER PRIMARY KEY ASC, text_id INTEGER, line_number INTEGER, text_offset INTEGER, number_of_words INTEGER, 
//  source TEXT, FOREIGN KEY(text_id) REFERENCES text(id));
ET_ReturnCode CParser::eSaveLine(long long llTextId, int iLineNum, int iTextOffset, int iLength, int iNumOfWords, const CEString& sText, long long& llDbKey)
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
ET_ReturnCode CParser::eSaveWord(long long llLineDbId, int iLine, int iWord, int iLineOffset, int iSegmentLength, const CEString& sWord, long long& llWordDbKey)
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
ET_ReturnCode CParser::eSaveWordParse(long long llWordId, long long llWordFormId, long long& llWordToWordFormId)
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

bool CParser::bIsProclitic(CWordForm* pWordForm)
{
    return false;       // TODO: implement
}

bool CParser::bIsEnclitic(CWordForm* pWordForm)
{
    return false;       // TODO: implement
}

bool CParser::bArePhoneticallyIdentical(CWordForm* pWf1, CWordForm* pWf2)
{
    if (pWf1->sWordForm() != pWf2->sWordForm())
    {
        return false;
    }

    int iStressPos1, iStressPos2 = -1;
    ET_StressType eStressType1, eStressType2 = STRESS_TYPE_UNDEFINED;
    auto eRet1 = pWf1->eGetFirstStressPos(iStressPos1, eStressType1);
    auto eRet2 = pWf2->eGetFirstStressPos(iStressPos2, eStressType2);
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

    if (H_FALSE != eRet1 || H_FALSE != eRet2)
    {
        return false;
    }

    if (eStressType1 != eStressType2 || iStressPos1 != iStressPos2)
    {
        return false;
    }

    for (int iCount = 0; iCount < 100; ++iCount)        // arbitraty max to avoid infinit loop
    {
        eRet1 = pWf1->eGetNextStressPos(iStressPos1, eStressType1);
        eRet2 = pWf2->eGetNextStressPos(iStressPos2, eStressType2);

        if ((eRet1 != H_NO_ERROR && eRet1 != H_FALSE) || (eRet2 != H_NO_ERROR && eRet2 != H_FALSE))
        { 
            ASSERT(0);
            ERROR_LOG(L"Error getting 1st stress position.");
            return false;
        }

        if (H_FALSE == eRet1 && H_FALSE == eRet2)
        {
            return true;
        }

        if (H_FALSE != eRet1 || H_FALSE != eRet2)
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
