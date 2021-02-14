#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <memory>
#include <cassert>

#include "Transcriber.h"
#include "SqliteWrapper.h"

using namespace Hlib;

CTranscriber::CTranscriber(CSqlite* pCSqlite) : m_pDb(pCSqlite)
{
    if (!m_pDb)
    {
        ERROR_LOG(L"No database.");
        return;
    }

    auto eRet = eLoadTranscriptionRules();
    if (H_NO_ERROR != eRet)
    {
        ERROR_LOG(L"Unable to load transcription rules.");
    }

    if (m_mapRules.empty())
    {
        ERROR_LOG(L"No transcription rules.");
    }
}

ET_ReturnCode CTranscriber::eFormatInputs(CEString& sSource, PairInput& pairParsedInputs)
{
    //
    //  E.g., "a+b" --> "ab" = 'these two characters together, "ab" --> vector of chars = 'any of the chars in the vector
    //

    if (sSource.bRegexMatch(L"^([абвгдеёжзийклмнопрстуфхцчшщъыьэюя]+)\\s*\\+\\s*([абвгдеёжзийклмнопрстуфхцчшщъыьэюя]+)\\s*"))
    {
        if (sSource.uiGetNumOfRegexMatches() != 2)
        {
            ERROR_LOG(L"Unexpected number of \"+\"-separated strings");
            return H_ERROR_UNEXPECTED;
        }

        pairParsedInputs.first = sSource.sGetRegexMatch(0);
        pairParsedInputs.second = sSource.sGetRegexMatch(1);
    }
    else
    {
        pairParsedInputs.first = sSource;
    }

    return H_NO_ERROR;
}

ET_ReturnCode CTranscriber::eSplitSource(CEString& sSource, vector<CEString>& vecTarget)
{
    sSource.ResetSeparators();
    sSource.SetBreakChars(L", \t");

    int iNWords = sSource.uiNFields();
    if (iNWords < 1)
    {
        return H_ERROR_UNEXPECTED;
    }

    for (int iAt = 0; iAt < iNWords; ++iAt)
    {
        vecTarget.push_back(sSource.sGetField(iAt));
    }

    return H_NO_ERROR;
}

ET_ReturnCode CTranscriber::eParseContexts(CEString& sSource, set<vector<PhonemicContextAtom>>& setTarget)
{
    assert(!sSource.bIsEmpty());

    vector<CEString> vecContextStrings;
    ET_ReturnCode eRet = eSplitSource(sSource, vecContextStrings);
    if (eRet != H_NO_ERROR)
    {
        ERROR_LOG(L"unable to split context string.");
        return H_ERROR_INVALID_ARG;
    }

    vector<PhonemicContextAtom> vecContexts;

//    for (auto sContext : vecContextStrings)
    for (unsigned int uiAt = 0; uiAt < vecContextStrings.size(); ++uiAt)
    {
        auto sContext = vecContextStrings[uiAt];

        auto itContext = m_mapStringToContext.find(sContext);
        PhonemicContextAtom context = ET_PhonemicContext::PHONEMIC_CONTEXT_UNDEFINED;
        if (m_mapStringToContext.end() != itContext)
        {
            context = itContext->second;
        }
        else
        {
            if (CEString::bStringOverAlphabet(sContext, m_LowerCaseCyrillic))
            {
                context = sContext;
            }
            else
            {
                ERROR_LOG(L"Phonemic context characters not recognized.");
                continue;
            }
        }

        if (context != PhonemicContextAtom(ET_PhonemicContext::PHONEMIC_CONTEXT_UNDEFINED))
        {
            vecContexts.push_back(context);
        }

        if (!(uiAt < vecContextStrings.size()-1 && L"+" == vecContextStrings[uiAt + 1]))
        {
            setTarget.insert(vecContexts);
            vecContexts.clear();
        }
    }

    return H_NO_ERROR;
}

/*
ET_ReturnCode CTranscriber::eFormatContexts(const CEString& sContext, EC_CONTEXT_TYPE eLeftOrRight)
{

    return H_NO_ERROR;
}
*/

/*
ET_ReturnCode eSearchInSortedStrings(vector<CEString&> vecSortedStrings, const CEString& sFindMe)
{
    // No need to optimize: vector will be tiny


    return H_TRUE;
}
*/

ET_ReturnCode CTranscriber::eLoadTranscriptionRules()
{
    if (nullptr == m_pDb)
    {
        ERROR_LOG(L"Database not available.");
        return H_ERROR_DB;
    }

    ET_ReturnCode eRet = H_NO_ERROR;

    static const CEString sQuery
    (L"SELECT ti.input_chars, tr.stress, tr.left_contexts, tr.right_contexts, tr.morpheme_type,  tr.subparadigm, \
    tr.gramm_gender, tr.gramm_number, tr.gramm_case, tr.strength, tr.target FROM transcription_inputs \
    AS ti INNER JOIN transcription_rules as tr ON ti.id = tr.input_id");

    try
    {
        m_pDb->PrepareForSelect(sQuery);
        while (m_pDb->bGetRow())
        {
            CEString sInputs;
            m_pDb->GetData(0, sInputs);

            PairInput pairInputs;
            eRet = eFormatInputs(sInputs, pairInputs);

            StRule rule;

            // Stress (a string)
            CEString sStressType;
            m_pDb->GetData(1, sStressType);
            if (!sStressType.bIsEmpty())
            {
                auto itStressType = m_mapStringToStressRelation.find(sStressType);
                if (m_mapStringToStressRelation.end() == itStressType)
                {
                    ERROR_LOG(L"Stress context not recognized.");
                    continue;
                }
                rule.m_setStressContexts.insert((*itStressType).second);
            }

            // Left contexts
            CEString sLeftContexts;
            m_pDb->GetData(2, sLeftContexts);
            if (!sLeftContexts.bIsEmpty())
            {
                eRet = eParseContexts(sLeftContexts, rule.m_setLeftContexts);
            }

            // Right contexts
            CEString sRightContexts;
            m_pDb->GetData(3, sRightContexts);
            if (!sRightContexts.bIsEmpty())
            {
                eRet = eParseContexts(sRightContexts, rule.m_setRightContexts);
            }

            // Morphemic context
            CEString sMorphemeType;
            m_pDb->GetData(4, sMorphemeType);
            if (!sMorphemeType.bIsEmpty())
            {
                vector<CEString> vecMorphemeTypeStrings;
                eRet = eSplitSource(sMorphemeType, vecMorphemeTypeStrings);
                if (eRet != H_NO_ERROR)
                {
                    ERROR_LOG(L"unable to split morphemic contexts string.");
                    continue;
                }

                MorphemicContextAtom context;
                for (auto sMorphemeType : vecMorphemeTypeStrings)
                {
                    auto itMorphemeType = m_mapStringToMorphemicContext.find(sMorphemeType);
                    if (itMorphemeType != m_mapStringToMorphemicContext.end())
                    {
                        context = itMorphemeType->second;
                    }
                    else
                    {
                        if (CEString::bStringOverAlphabet(sMorphemeType, m_LowerCaseCyrillic))
                        {
                            context = sMorphemeType;
                        }
                        else
                        {
                            ERROR_LOG(L"Phonemic context characters not recognized.");
                            continue;
                        }
                    }
                    rule.m_setMorphemicContexts.insert(context);
                }
            }

            // Subparadigm (a string)
            CEString sSubparadigm;
            m_pDb->GetData(5, sSubparadigm);
            if (!sSubparadigm.bIsEmpty())
            {
                auto itSubparadigm = m_mapStringToSubparadigm.find(sSubparadigm);
                if (m_mapStringToSubparadigm.end() == itSubparadigm)
                {
                    ERROR_LOG(L"Subparadigm not recognized.");
                    continue;
                }
                rule.m_setSubparadigms.insert((*itSubparadigm).second);
            }

            // Gender
            CEString sGender;
            m_pDb->GetData(6, sGender);
            if (!sGender.bIsEmpty())
            {
                auto itGender = m_mapStringToGender.find(sGender);
                if (m_mapStringToGender.end() == itGender)
                {
                    ERROR_LOG(L"Gender not recognized.");
                    continue;
                }
                rule.m_setGenders.insert((*itGender).second);
            }

            // Number 
            CEString sNumber;
            m_pDb->GetData(7, sNumber);
            if (!sNumber.bIsEmpty())
            {
                auto itNumber = m_mapStringToNumber.find(sNumber);
                if (m_mapStringToNumber.end() == itNumber)
                {
                    ERROR_LOG(L"Number not recognized.");
                    continue;
                }
                rule.m_setNumbers.insert((*itNumber).second);
            }

            // Case 
            CEString sCase;
            m_pDb->GetData(8, sCase);
            if (!sCase.bIsEmpty())
            {
                auto itCase = m_mapStringToCase.find(sCase);
                if (m_mapStringToCase.end() == itCase)
                {
                    ERROR_LOG(L"Case not recognized.");
                    continue;
                }
                rule.m_setCases.insert((*itCase).second);
            }

            // Strength
            CEString sStrength;
            m_pDb->GetData(9, sStrength);
            if (!sStrength.bIsEmpty())
            {
                auto itStrength = m_mapStringToRuleStrength.find(sStrength);
                if (m_mapStringToRuleStrength.end() == itStrength)
                {
                    ERROR_LOG(L"Strength not recognized.");
                    continue;
                }
                rule.m_eStrength = (*itStrength).second;
            }

            // IsVariant
            m_pDb->GetData(9, rule.m_bIsVariant);

            m_mapRules[pairInputs].push_back(rule);
        }
    }
    catch (CException& ex)
    {
        ERROR_LOG(ex.szGetDescription());
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}       //  eLoadTranscriptionRules()

ET_ReturnCode CTranscriber::eTranscribe()
{
    if (!m_pDb)
    {
        ERROR_LOG(L"No database.");
        return H_ERROR_DB;
    }

    vector<pair<long long, long long>> vecIds;

    CEString sQuery(L"SELECT id, line_id FROM tact_group");
    m_pDb->PrepareForSelect(sQuery);
    while (m_pDb->bGetRow())
    {
        long long llId = -1;
        long long llLineId = -1;
        m_pDb->GetData(0, llId);
        m_pDb->GetData(1, llLineId);
        if (llId < 0)
        {
            ERROR_LOG(L"Illegal tact group ID.");
            continue;
        }

        if (llLineId < 0)
        {
            ERROR_LOG(L"Illegal line ID.");
            continue;
        }

        vecIds.push_back(make_pair(llId, llLineId));
    }

    m_pDb->Finalize();

    if (vecIds.size() < 1)
    {
        ERROR_LOG(L"No tact groups.");
        return H_FALSE;
    }

    CEString sQueryBase = L"SELECT first_word_position, main_word, num_of_words, source, gram_hash, num_of_syllables, \
                            stressed_syllable, reverse_stressed_syllable, secondary_stressed_syllable  \
                            FROM tact_group ";

    clock_t dbProcTime = clock();

    for (auto pairIds : vecIds)
    {
        vector<StWord> vecTactGroup;

        CEString sQuery = sQueryBase + L"WHERE id = " + CEString::sToString(pairIds.first) +    \
                          L" AND line_id = " + CEString::sToString(pairIds.second) + L";";
        m_pDb->PrepareForSelect(sQuery);
        while (m_pDb->bGetRow())
        {
            StWord stWord;
            stWord.llTactGroupId = pairIds.first;
            stWord.llLineId = pairIds.second;
            m_pDb->GetData(0, stWord.iPosition);
            m_pDb->GetData(1, stWord.iMainWord);
            m_pDb->GetData(2, stWord.iNumOfWords);
            m_pDb->GetData(3, stWord.sSource);
            m_pDb->GetData(4, stWord.sGramHash);
            m_pDb->GetData(5, stWord.iNumOfSyllables);
            m_pDb->GetData(6, stWord.iStressedSyllable);
            m_pDb->GetData(7, stWord.iReverseStressedSyllable);
            m_pDb->GetData(8, stWord.iSecondaryStressSyllable);

            vecTactGroup.push_back(stWord);
        }
        m_pDb->Finalize();

    }

    auto dDuration = (clock() - dbProcTime)/(double)CLOCKS_PER_SEC;
    CEString msg(L"Time: ");
    msg += CEString::sToString(dDuration);
    ::MessageBoxW(NULL, msg, L"Transcription", MB_ICONINFORMATION);

    return H_NO_ERROR;

}       //  Transcribe()

ET_ReturnCode eTranscribeTactGroup(vector<StWord>& vecTactGroup)
{
    for (auto& stWord : vecTactGroup)
    {
        for (int iAt = 0; iAt < (int)stWord.sSource.uiLength(); ++iAt)
        {
            auto chr = stWord.sSource.chrGetAt(iAt);
//  1. check for this and next char
//  2. check for this char only
//  3. error if neither
        }
    }

    return H_NO_ERROR;
}
