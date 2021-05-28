#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <ctime>
#include <algorithm>

#include "SqliteWrapper.h"
#include "Endings.h"
#include "WordForm.h"
#include "Lexeme.h"
#include "Parser.h"
#include "ParsingTree.h"

using namespace Hlib;

CParser::CParser(CDictionary * pDictionary) : m_pDictionary(pDictionary), m_pEndingsTree(nullptr)
{}

CParser::~CParser()
{
    ClearResults();
}

ET_ReturnCode CParser::eParseWord(const CEString& sWord)
{
    clock_t startTime = clock();

    m_vecWordForms.clear();

    if (!m_pDictionary)
    {
        return H_ERROR_DB;
    }

    if (NULL == m_pEndingsTree)
    {
        m_pEndingsTree = new CParsingTree(m_pDictionary->pGetDb());
    }

    ET_ReturnCode rc = H_NO_ERROR;

    rc = eIrregularFormLookup(sWord);
    if (rc != H_NO_ERROR)
    {
        ERROR_LOG(L"Irregular form lookup failed.");
    }

    auto bSpryazhSm = true;
    rc = eIrregularFormLookup(sWord, bSpryazhSm);
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

    for (auto& pWf : m_vecWordForms)
    {
        shared_ptr<CLexeme> pLexeme = nullptr;
        m_pDictionary->eGetLexemeById(pWf->llLexemeId(), pLexeme);
        if (nullptr == pLexeme)
        {
            CEString sMsg(L"Failed to get lexeme by ID, ID = ");
            sMsg += CEString::sToString(pWf->llLexemeId());
            ERROR_LOG(sMsg);
            continue;
        }
        pWf->SetPos(pLexeme->ePartOfSpeech());
        pWf->m_pLexeme = pLexeme.get();
    }

    return m_vecWordForms.empty() ? H_FALSE : H_NO_ERROR;

}       // eParseWord()


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

//  Overloaded non-virtual versions for internal use
ET_ReturnCode CParser::eGetFirstWordForm(CWordForm *& wordForm)
{
    m_itCurrentWordForm = m_vecWordForms.begin();
    if (m_vecWordForms.end() == m_itCurrentWordForm)
    {
        return H_FALSE;
    }

    wordForm = *m_itCurrentWordForm;

    return H_NO_ERROR;
}

ET_ReturnCode CParser::eGetNextWordForm(CWordForm *& pWordForm)
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

void CParser::SetDb(shared_ptr<CSqlite> pDb)
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

ET_ReturnCode CParser::eIrregularFormLookup(const CEString& sWord, bool bSpryazhSm)
{
    ET_ReturnCode rc = H_NO_ERROR;

    CEString sIrregFormQuery(L"SELECT DISTINCT id, descriptor_id, gram_hash, wordform, is_alternative FROM ");
    if (bSpryazhSm)
    {
        sIrregFormQuery += L"irregular_forms_spryazh_sm WHERE wordform = \"";
    }
    else
    {
        sIrregFormQuery += L"irregular_forms WHERE wordform = \"";
    }

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

            CEString sIrregStressQuery(L"SELECT position, is_primary FROM ");
            if (bSpryazhSm)
            {
                sIrregStressQuery += L"irregular_stress_spryazh_sm WHERE form_id = \"";
            }
            else
            {
                sIrregStressQuery += L"irregular_stress WHERE form_id = \"";
            }
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
            pWf->m_llDbKey = llFormId;
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

ET_ReturnCode eLexemeLookup(const CLexeme& lexeme)
{
    return H_NO_ERROR;
}

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
