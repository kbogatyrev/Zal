#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <memory>
#include <cassert>

#include "Analytics.h"
#include "Transcriber.h"
#include "SqliteWrapper.h"

using namespace Hlib;

CTranscriber::CTranscriber(shared_ptr<CSqlite> pCSqlite) : m_pDb(pCSqlite)
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
            StRule stRule;

            CEString sInputs, sKeys;
            m_pDb->GetData(0, sInputs);

            //
            //  E.g., "a+b" --> "ab" = 'these two characters together, "ab" --> vector of chars = 'any of the chars in the vector
            //
            if (sInputs.bRegexMatch(L"^([абвгдеёжзийклмнопрстуфхцчшщъыьэюя]+)\\s*\\+\\s*([абвгдеёжзийклмнопрстуфхцчшщъыьэюя]+)\\s*"))
            {
                if (sInputs.uiGetNumOfRegexMatches() != 2)
                {
                    ERROR_LOG(L"Unexpected number of \"+\"-separated strings");
                    return H_ERROR_UNEXPECTED;
                }

                sKeys = sInputs.sGetRegexMatch(0);
                stRule.sFollowedBy = sInputs.sGetRegexMatch(1);
            }
            else
            {
                if (sInputs.bRegexMatch(L"^([абвгдеёжзийклмнопрстуфхцчшщъыьэюя]+)\\s*"))
                {
                    sKeys = sInputs.sGetRegexMatch(0);
                }
//                else if (sInputs.bRegexMatch(L"^([A-Z]+)"))
//                {
//                    sKeys = sInputs.sGetRegexMatch(0);
//                }
                else
                {
                    ERROR_LOG(L"Unable to parse transcription rule inputs.");
                    continue;
                }
            }

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
                stRule.m_setStressContexts.insert((*itStressType).second);
            }

            // Left contexts
            CEString sLeftContexts;
            m_pDb->GetData(2, sLeftContexts);
            if (!sLeftContexts.bIsEmpty())
            {
                eRet = eParseContexts(sLeftContexts, stRule.m_setLeftContexts);
            }

            // Right contexts
            CEString sRightContexts;
            m_pDb->GetData(3, sRightContexts);
            if (!sRightContexts.bIsEmpty())
            {
                eRet = eParseContexts(sRightContexts, stRule.m_setRightContexts);
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
                    stRule.m_setMorphemicContexts.insert(context);
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
                stRule.m_setSubparadigms.insert((*itSubparadigm).second);
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
                stRule.m_setGenders.insert((*itGender).second);
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
                stRule.m_setNumbers.insert((*itNumber).second);
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
                stRule.m_setCases.insert((*itCase).second);
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
                stRule.m_eStrength = (*itStrength).second;
            }

            // IsVariant
            m_pDb->GetData(9, stRule.m_bIsVariant);

            for (int iAt = 0; iAt < (int)sKeys.uiLength(); ++iAt)
            {
                m_mapRules[sKeys[iAt]].push_back(stRule);
            }
        }
    }
    catch (CException& ex)
    {
        ERROR_LOG(ex.szGetDescription());
        return H_EXCEPTION;
    }

    return H_NO_ERROR;

}       //  eLoadTranscriptionRules()

ET_ReturnCode CTranscriber::eTranscribeTactGroup(StTactGroup& stTactGroup)
{
    stTactGroup.sSource.SetVowels(g_szRusVowels);

    int iAt = 0;
    while (iAt < (int)stTactGroup.sSource.uiLength())
    {
        if (CEString::bIsVowel(stTactGroup.sSource[iAt]))
        {
            auto eRet = eHandleVowel(stTactGroup, iAt);
        }
/*
        vector<StRule>* pvecRules = nullptr;
        auto chr = stTactGroup.sSource.chrGetAt(iAt);
        auto secondChr = ((int)stTactGroup.sSource.uiLength() > iAt + 1) ? stTactGroup.sSource.chrGetAt(iAt + 1) : L'\0';
        pair<CEString, CEString> pairInput = make_pair(chr, secondChr);
        pairInput.second = secondChr;
        auto itTwoCharHit = m_mapRules.find(make_pair(chr, secondChr));
        if (m_mapRules.end() != itTwoCharHit)
        {
            pvecRules = &itTwoCharHit->second;
        }
        else
        {
            auto itSingleCharHit = m_mapRules.find(make_pair(chr, L""));
            if (m_mapRules.end() != itSingleCharHit)
            {
                pvecRules = &itSingleCharHit->second;
            }
            else
            {
                // ERROR
                int i = 0;
            }
        }
        int ii = 0;
        iAt++;
    }

    for (int iAt = 0; iAt < (int)stWord.sSource.uiLength(); ++iAt)
    {
        auto chr = stWord.sSource.chrGetAt(iAt);
        auto secondChr = ((int)stWord.sSource.uiLength() > iAt + 1) ? stWord.sSource.chrGetAt(iAt + 1) : L'\0';
        auto itRule = m_mapRules.find(make_pair)
//  1. check for this and next char
//  2. check for this char only
//  3. error if neither
    }
*/
        ++iAt;
    }

    return H_NO_ERROR;
}

ET_ReturnCode CTranscriber::eHandleVowel(StTactGroup& stTg, int iPos)
{
    if (iPos < 0 || iPos >= (int)stTg.sSource.uiLength())
    {
        CEString sMsg(L"Illegal character position: ");
        sMsg += CEString::sToString(iPos);
        ERROR_LOG(sMsg);
        return H_ERROR_INVALID_ARG;
    }

    wchar_t chrVowel = stTg.sSource.chrGetAt(iPos);
    if (!CEString::bIsVowel(chrVowel))
    { 
        CEString sMsg(L"Character at position ");
        sMsg += CEString::sToString(iPos);
        sMsg += L" is not a vowel";
        ERROR_LOG(sMsg);
        return H_ERROR_INVALID_ARG;
    }

    auto itRules = m_mapRules.find(chrVowel);
    if (m_mapRules.end() == itRules)
    {
        CEString sMsg(L"No rules for vowel ");
        sMsg += chrVowel;
        return H_ERROR_UNEXPECTED;
    }

    auto eStressStatus = ET_VowelStressRelation::VOWEL_STRESS_RELATION_UNDEFINED;
    auto eRet = eGetStressStatus(stTg, iPos, eStressStatus);
    vector<StRule> vecAvailableRules;
    for (auto& stRule : itRules->second)
    { 
        if (stRule.m_setStressContexts.empty())
        {
            vecAvailableRules.emplace_back(stRule);
        }
        else if (stRule.m_setStressContexts.find(eStressStatus) != stRule.m_setStressContexts.end())
        {
            vecAvailableRules.emplace_back(stRule);
        }
    }

    for (auto& stRule : vecAvailableRules)
    {
        for (auto varLeftContext : stRule.m_setLeftContexts)
        {
            auto eRet = eLeftContextMatch(stTg, varLeftContext, iPos);
        }
    }

    return H_NO_ERROR;
}

ET_ReturnCode CTranscriber::eGetStressStatus(StTactGroup& stTg, int iPos, ET_VowelStressRelation& eStressStatus)
{
    eStressStatus = ET_VowelStressRelation::VOWEL_STRESS_RELATION_UNDEFINED;

    if (iPos < 0 || iPos >= (int)stTg.sSource.uiLength())
    {
        CEString sMsg(L"Invalid character at position: ");
        sMsg += CEString::sToString(iPos);
        sMsg += L".";
        ERROR_LOG(sMsg);
        return H_ERROR_INVALID_ARG;
    }

    wchar_t chrVowel = stTg.sSource.chrGetAt(iPos);
    if (!CEString::bIsVowel(chrVowel))
    {
        CEString sMsg(L"Character at position ");
        sMsg += CEString::sToString(iPos);
        sMsg += L" is not a vowel";
        ERROR_LOG(sMsg);
        return H_ERROR_INVALID_ARG;
    }

    int iSyllPos = -1;
    try
    {
        iSyllPos = static_cast<int>(stTg.sSource.uiGetSyllableFromVowelPos(iPos));
    }
    catch (CException& ex)
    {
        ERROR_LOG(ex.szGetDescription());
        return H_EXCEPTION;
    }

    if (iSyllPos == stTg.iStressedSyllable)
    {
        eStressStatus = ET_VowelStressRelation::STRESSED;
        return H_NO_ERROR;
    }

    if (stTg.iStressedSyllable - iSyllPos == 1)
    {
        eStressStatus = ET_VowelStressRelation::FIRST_PRETONIC;
        return H_NO_ERROR;
    }

    if (stTg.iStressedSyllable - iSyllPos > 1)
    {
        eStressStatus = ET_VowelStressRelation::OTHER_PRETONIC;
        return H_NO_ERROR;
    }

    if (stTg.iStressedSyllable - iSyllPos < 1)
    {
        eStressStatus = ET_VowelStressRelation::POSTTONIC;
        return H_NO_ERROR;
    }

    CEString sMsg(L"Unable to determine stress status.");
    ERROR_LOG(sMsg);
 
    return H_ERROR_UNEXPECTED;

}       //  eGetStressStatus()

ET_ReturnCode CTranscriber::eLeftContextMatch(StTactGroup& stTg, const vector<PhonemicContextAtom>& vecContext, int iPos)
{
    struct StMatchTypes
    {
        StTactGroup* m_pStTg;
        int m_iPos;

        StMatchTypes(StTactGroup* pStTg, int iPos) : m_pStTg(pStTg), m_iPos(iPos) {};

        bool operator()(ET_PhonemicContext& eContext) 
        {
            if (m_iPos < 1)
            {
                CEString sMsg(L"No left context. ");
                ERROR_LOG(sMsg);
                return false;
            }

            wchar_t chrPreceding = m_pStTg->sSource[m_iPos-1];
            auto bMatch = false;

            switch (eContext)
            {
            case ET_PhonemicContext::VOWEL:
                if (CEString::bIn(chrPreceding, m_Vowels))
                {
                    bMatch = true;
                }
                break;
            case ET_PhonemicContext::CONSONANT:
                if (CEString::bIn(chrPreceding, m_Consonants))
                {
                    bMatch = true;
                }
                break;
            case ET_PhonemicContext::HARD_CONSONANT:
                if (CEString::bIn(chrPreceding, m_HardConsonants))
                {
                    bMatch = true;
                }
                break;
            case ET_PhonemicContext::HARD_PAIRED_CONSONANT:
                break;
            case ET_PhonemicContext::SOFT_CONSONANT:
                break;
            case ET_PhonemicContext::SOFT_CONSONANT_NO_CH_SHCH:
                break;
            case ET_PhonemicContext::VOICELESS:
                break;
            case ET_PhonemicContext::BOUNDARY_WORD:
                break;
            case ET_PhonemicContext::BOUNDARY_NOT_PROCLITIC:
                break;
            case ET_PhonemicContext::BOUNDARY_SYNTAGM:
                break;
            default:
                CEString sMsg(L"Left context not recognized: ");
                sMsg += to_wstring(eContext).c_str();
                ERROR_LOG(sMsg);
            }

            return bMatch; 
        }
        
        bool operator()(CEString& sContext) 
        { 
            if (m_iPos > 0 && CEString::bIn(m_pStTg->sSource[m_iPos - 1], sContext))
            {
                return true;
            }
            return false;
        }
    };

    for (PhonemicContextAtom atom : vecContext)
    {
        auto match = visit(StMatchTypes(&stTg, iPos), atom);
        int i = 0;
    }

    return H_TRUE;
}
