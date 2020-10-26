#include <memory>

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
                rule.m_vecStressContexts.push_back((*itStressType).second);
            }

            // Left contexts (an array of strings)
            CEString sLeftContexts;
            m_pDb->GetData(2, sLeftContexts);
            if (!sLeftContexts.bIsEmpty())
            {
                vector<CEString> vecLeftContexts;
                eRet = eSplitSource(sLeftContexts, vecLeftContexts);
                if (eRet != H_NO_ERROR)
                {
                    ERROR_LOG(L"unable to split left context string.");
                    continue;
                }
                for (auto sLeftContext : vecLeftContexts)
                {
                    auto itLeftContext = m_mapStringToContext.find(sLeftContext);
                    if (m_mapStringToContext.end() == itLeftContext)
                    {
                        for (int iAt = 0; iAt < (int)sLeftContext.uiLength(); ++iAt)
                        {
                            auto chrAt = sLeftContext.chrGetAt(iAt);
                            if (L'\0' == chrAt)
                            {
                                CEString sMsg(L"Unable to get character at pos. ");
                                sMsg += CEString::sToString(iAt);
                                ERROR_LOG(sMsg);
                                continue;
                            }
// &&&&                            rule.m_vecLeftContexts.push_back
                        }
//                        ERROR_LOG(L"Left context not recognized.");
//                        continue;
                    }
                    else
                    {
                        rule.m_vecLeftContexts.push_back((*itLeftContext).second);
                    }
                }
            }       //  if (!sLeftContexts.bIsEmpty())

            // Right contexts (an array of strings)
            CEString sRightContexts;
            m_pDb->GetData(3, sRightContexts);
            if (!sRightContexts.bIsEmpty())
            {
                vector<CEString> vecRightContexts;
                eRet = eSplitSource(sRightContexts, vecRightContexts);
                if (eRet != H_NO_ERROR)
                {
                    ERROR_LOG(L"unable to split right context string.");
                    continue;
                }
                for (auto sRightContext : vecRightContexts)
                {
                    auto itRightContext = m_mapStringToContext.find(sRightContext);
                    if (m_mapStringToContext.end() == itRightContext)
                    {
                        ERROR_LOG(L"Right context not recognized.");
                        continue;
                    }
                    rule.m_vecRightContexts.push_back((*itRightContext).second);
                }
            }

            // Morphemic context: an array of strings
            CEString sMorphemeType;
            m_pDb->GetData(4, sMorphemeType);
            if (!sMorphemeType.bIsEmpty())
            {
                vector<CEString> vecMorphemeTypes;
                eRet = eSplitSource(sMorphemeType, vecMorphemeTypes);
                if (eRet != H_NO_ERROR)
                {
                    ERROR_LOG(L"unable to split morphemic contexts string.");
                    continue;
                }
                for (auto sMorphemeType : vecMorphemeTypes)
                {
                    auto itMorphemeType = m_mapStringToMorphemicContext.find(sMorphemeType);
                    if (m_mapStringToMorphemicContext.end() == itMorphemeType)
                    {
                        ERROR_LOG(L"Morpheme not recognized.");
                        continue;
                    }
                    rule.m_vecMorphemicContexts.push_back((*itMorphemeType).second);
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
                rule.m_vecSubparadigms.push_back((*itSubparadigm).second);
            }

            // Gender (a string)
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
                rule.m_vecGenders.push_back((*itGender).second);
            }

            // Number (a string)
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
                rule.m_vecNumbers.push_back((*itNumber).second);
            }

            // Case (a string)
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
                rule.m_vecCases.push_back((*itCase).second);
            }

            // Strength (a string)
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

            // IsVariant (boolean value)
            m_pDb->GetData(9, rule.m_bIsVariant);

            // push back to the vector of rules for this input
            m_mapRules[pairInputs].push_back(rule);
        }
    }
    catch (CException & ex)
    {
        ERROR_LOG(ex.szGetDescription());
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}       //  eLoadTranscriptionRules()

ET_ReturnCode CTranscriber::eFormatInputs(CEString& sSource, PairInput& pairParsedInputs)
{
    //
    //  E.g., "a+b" --> "ab" = 'these two characters together, "ab" --> vector of chars = 'any of the chars in the vector
    //


    if (sSource.bRegexMatch(L"^[абвгдеёжзийклмнопрстуфхцчшщъыьэюя]+\\s*\\+\\s*[абвгдеёжзийклмнопрстуфхцчшщъыьэюя]+\\s*"))
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