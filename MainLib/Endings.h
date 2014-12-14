#ifndef ENDINGS_H_INCLUDED
#define ENDINGS_H_INCLUDED

#include <vector>
#include <map>
#include "SqliteWrapper.h"
#include "Enums.h"
#include "EString.h"

using namespace std;

namespace Hlib
{

class CLexeme;

struct StEndingDescriptor
{
    __int64 llDbKey;
    ET_Subparadigm eSubparadigm;
    int iInflectionType;
    ET_Gender eGender;
    ET_Number eNumber;
    ET_Case eCase;
    ET_Animacy eAnimacy;
    ET_Person ePerson;
    ET_StressLocation eStress;
    ET_StemAuslaut eStemAuslaut;

    StEndingDescriptor() : llDbKey(-1),
                           eSubparadigm(SUBPARADIGM_UNDEFINED),
                           iInflectionType (INFLECTION_TYPE_UNDEFINED),
                           eGender (GENDER_UNDEFINED), 
                           eNumber (NUM_UNDEFINED), 
                           eCase (CASE_UNDEFINED), 
                           eAnimacy (ANIM_UNDEFINED), 
                           ePerson (PERSON_UNDEFINED),
                           eStress (STRESS_LOCATION_UNDEFINED),
                           eStemAuslaut (STEM_AUSLAUT_UNDEFINED)
    {}

    StEndingDescriptor(ET_Subparadigm eSubparadigm) :
                            llDbKey(-1),
                            eSubparadigm(eSubparadigm),
                            iInflectionType(INFLECTION_TYPE_UNDEFINED),
                            eGender(GENDER_UNDEFINED),
                            eNumber(NUM_UNDEFINED),
                            eCase(CASE_UNDEFINED),
                            eAnimacy(ANIM_UNDEFINED),
                            ePerson(PERSON_UNDEFINED),
                            eStress(STRESS_LOCATION_UNDEFINED),
                            eStemAuslaut(STEM_AUSLAUT_UNDEFINED)
    {}

    // Overload for nouns
    StEndingDescriptor (ET_Number eN, 
                        ET_Case eC, 
                        ET_StressLocation eS) :
                            llDbKey(-1),
                            eSubparadigm(SUBPARADIGM_NOUN),
                            iInflectionType(INFLECTION_TYPE_UNDEFINED),
                            eGender(GENDER_UNDEFINED), 
                            eNumber(eN),
                            eCase(eC), 
                            eAnimacy(ANIM_UNDEFINED), 
                            ePerson(PERSON_UNDEFINED),
                            eStress(eS),
                            eStemAuslaut(STEM_AUSLAUT_UNDEFINED)
    {}

    // Overload for long adjectves
    StEndingDescriptor (ET_Gender eG,
                        ET_Number eN, 
                        ET_Case eC,
                        ET_Animacy eA,
                        ET_StressLocation eS) :
                            llDbKey(-1),
                            eSubparadigm(SUBPARADIGM_LONG_ADJ),
                            iInflectionType(INFLECTION_TYPE_UNDEFINED),
                            eGender(eG), 
                            eNumber(eN),
                            eCase(eC),
                            eAnimacy(eA),
                            ePerson(PERSON_UNDEFINED),
                            eStress(eS),
                            eStemAuslaut(STEM_AUSLAUT_UNDEFINED)
    {}

    // Overload for short adjectives
    StEndingDescriptor (ET_Gender eG,
                        ET_Number eN,
                        ET_StressLocation eS) :
                            llDbKey(-1),
                            eSubparadigm(SUBPARADIGM_SHORT_ADJ),
                            iInflectionType(INFLECTION_TYPE_UNDEFINED),
                            eGender(eG), 
                            eNumber(eN),
                            eCase(CASE_UNDEFINED), 
                            eAnimacy(ANIM_UNDEFINED), 
                            ePerson(PERSON_UNDEFINED),
                            eStress(eS),
                            eStemAuslaut(STEM_AUSLAUT_UNDEFINED)
    {}

    // Overload for personal forms
    StEndingDescriptor (int iInflectionType,
                        ET_Person eP,
                        ET_Number eN,
                        ET_StressLocation eS,
                        ET_StemAuslaut eSa) :
                            llDbKey(-1),
                            eSubparadigm(SUBPARADIGM_PRESENT_TENSE),
                            iInflectionType(iInflectionType),
                            eGender(GENDER_UNDEFINED),
                            eNumber(eN),
                            eCase(CASE_UNDEFINED),
                            eAnimacy(ANIM_UNDEFINED),
                            ePerson(eP), 
                            eStress(eS),
                            eStemAuslaut(eSa)
    {}

    // Overload for past tense forms
    StEndingDescriptor(ET_Gender eG,
                       ET_Number eN,
                       ET_StemAuslaut eA) : 
                            llDbKey(-1),
                            eSubparadigm(SUBPARADIGM_PAST_TENSE),
                            iInflectionType(INFLECTION_TYPE_UNDEFINED),
                            eGender(eG),
                            eNumber(eN),
                            eCase(CASE_UNDEFINED),
                            eAnimacy(ANIM_UNDEFINED),
                            ePerson(PERSON_UNDEFINED),
                            eStress(STRESS_LOCATION_UNDEFINED),
                            eStemAuslaut(eA)
    {}

    CEString sToString() const
    {
        CEString s_ (L"Inflection type = ");
        s_ += CEString::sToString (iInflectionType);
        s_ += L"; Gender = ";
        s_ += CEString::sToString (eGender);
        s_ += L"; Number = ";
        s_ += CEString::sToString (eNumber);
        s_ += L"; Case = ";
        s_ += CEString::sToString (eCase);
        s_ += L"; Animacy = ";
        s_ += CEString::sToString (eAnimacy);
        s_ += L"; Person = ";
        s_ += CEString::sToString (ePerson);
        s_ += L"; Stress = ";
        s_ += CEString::sToString (eStress);
        s_ += L"; StemAuslaut = ";
        s_ += CEString::sToString (eStemAuslaut);

        return s_;
    }

    __int64 llHash() const
    {
        __int64 llHash = iInflectionType * SUBPARADIGM_COUNT * GENDER_COUNT * NUM_COUNT * CASE_COUNT * ANIM_COUNT * PERSON_COUNT * STRESS_LOCATION_COUNT * STEM_AUSLAUT_COUNT +
                         eSubparadigm *                        GENDER_COUNT * NUM_COUNT * CASE_COUNT * ANIM_COUNT * PERSON_COUNT * STRESS_LOCATION_COUNT * STEM_AUSLAUT_COUNT +
                         eGender *                                            NUM_COUNT * CASE_COUNT * ANIM_COUNT * PERSON_COUNT * STRESS_LOCATION_COUNT * STEM_AUSLAUT_COUNT +
                         eNumber *                                                        CASE_COUNT * ANIM_COUNT * PERSON_COUNT * STRESS_LOCATION_COUNT * STEM_AUSLAUT_COUNT +
                         eCase  *                                                                      ANIM_COUNT * PERSON_COUNT * STRESS_LOCATION_COUNT * STEM_AUSLAUT_COUNT +
                         eAnimacy *                                                                                 PERSON_COUNT * STRESS_LOCATION_COUNT * STEM_AUSLAUT_COUNT +
                         ePerson *                                                                                                 STRESS_LOCATION_COUNT * STEM_AUSLAUT_COUNT +
                         eStress *                                                                                                                         STEM_AUSLAUT_COUNT +
                         eStemAuslaut;

        return llHash;
    }

};      //  struct StEndingDescriptor

//
// Possible wordform division
//
struct StSplit
{
    unsigned int uiEndingLength;
    vector<__int64> vecEndingKeys;
};

//
// Endings parser node
//
struct StNode
{
    wchar_t cLetter;
    StNode * pPrevious;
    vector<StNode *> vecChildren;
};

struct StReverseComparisonFunctor
{
    bool operator() (const CEString& sLeft, const CEString& sRight)
    {
        int iLeftLength = sLeft.uiLength();
        int iRightLength = sRight.uiLength();
        if (iLeftLength < 1 && iRightLength < 1)
        {
            return false;
        }
        if (iLeftLength < 1)
        {
            return true;
        }
        if (iRightLength < 1)
        {
            return false;
        }

        if (iLeftLength > 10 || iRightLength > 10)
        {
            throw CException(H_ERROR_INVALID_ARG, L"Ending too long.");
        }

        int iLeftOffset = iLeftLength - 1;
        int iRightOffset = iRightLength - 1;
        for (; iLeftOffset >= 0 && iRightOffset >= 0; --iLeftOffset, --iRightOffset)
        {
            if (sLeft[iLeftOffset] < sRight[iRightOffset])
            {
                return true;
            }
            else if (sLeft[iLeftOffset] > sRight[iRightOffset])
            {
                return false;
            }
        }

        if (iLeftOffset >= 0)
        {
            return false;
        }
        else if (iRightOffset >= 0)
        {
            return true;;
        }

        return false;

    }   //  bool operator() (const CEString& sLeft, const CEString& sRight)

};  //  struct StReverseComparisonFunctor

//
// Endings parser tree
//
class CParsingTree
{
public:
    CParsingTree(CSqlite * pDb);
    ~CParsingTree();

public:
    void eSplit(const CEString& sWord);
    ET_ReturnCode eGetFirstMatch(int& iEndingLength);
    ET_ReturnCode eGetNextMatch(int& iEndingLength);
    ET_ReturnCode eIsEmptyEnding(__int64 llEndingId);

private:
    typedef map<CEString, vector<__int64>, StReverseComparisonFunctor> MapEndingToDescriptors;


    MapEndingToDescriptors m_mapSortedEndingsList;
    StNode * m_pRoot;
    vector<int> m_vecMatches;    // length of ending
    vector<int>::iterator m_itCurrentMatch;
    set<CEString, StReverseComparisonFunctor> m_setEndingStrings;
    set<__int64> m_setEmptyEndingIds;

protected:
    CParsingTree() {};
    ET_ReturnCode eLoad(CSqlite * pDb);
    void CParsingTree::AddLevel(unsigned int uiOffset, StNode * pParent, vector<CEString> vecEndings);
    void Traverse(StNode * pRoot, const CEString& sWord);
    ET_ReturnCode eTraverseAndDelete(StNode * pRoot);

private:
    /*
    void ClearBuckets()
    {
        vector<VecBucket>::iterator it_ = m_BuildHelper.begin();
        for (; it_ != m_BuildHelper.end(); ++it_)
        {
            (*it_).clear();
        }
    }
    */

    /*
    void Traverse(StNode * pRoot)     // testing
    {
        CEString sMsg(L" === ");
        for (int i = 0; i < s_iLevel; ++i)
        {
            sMsg += L'\t';
        }

        sMsg += pRoot->cLetter;
        CLogger::bWriteLog(wstring(sMsg));

        vector<StNode *>::iterator it_ = pRoot->vecChildren.begin();
        for (; it_ != pRoot->vecChildren.end(); ++it_)
        {
            Traverse(*it_);
        }
    }
    */
    
};      // CParsingTree

class CEndings
{
public:
    CEndings(CLexeme * pLexeme) : m_pLexeme(pLexeme)
    {}

    virtual ~CEndings()
    {}

    virtual void Reset()
    {
        m_mmEndings.clear();
    }

//    virtual __int64 llHash (const StEndingDescriptor&) = 0;

    int iCount (const StEndingDescriptor& stD)
    {
        __int64 llH = stD.llHash();
        return m_mmEndings.count (llH);
    }

    virtual ET_ReturnCode eLoad() = 0;
    virtual ET_ReturnCode eAddEnding (const CEString&, const StEndingDescriptor&) = 0;

    ET_ReturnCode eGetEnding (const StEndingDescriptor& stDescriptor, int iSeqNum, CEString& sEnding, __int64& llEndingKey)
    {
        __int64 llKey = stDescriptor.llHash();

        ItPair pair_Range = m_mmEndings.equal_range (llKey);
        for (int iItem = 0; iItem < iSeqNum; ++iItem)
        {
            if (++pair_Range.first == pair_Range.second)
            {
                CEString sMsg (L"Error getting ending from hash for: ");
                sMsg += stDescriptor.sToString();
                ERROR_LOG (sMsg);
                return H_ERROR_GENERAL;
            }
        }

        pair<CEString, __int64> pairEndingData = (*pair_Range.first).second;
        sEnding = pairEndingData.first;
        llEndingKey = pairEndingData.second;

        return H_NO_ERROR;
    }

    ET_ReturnCode eGetEnding (const StEndingDescriptor& stD, CEString& s_, __int64& llKey)
    {
        return eGetEnding (stD, 0, s_, llKey);
    }

protected:
    void ReportDbError();
    std::multimap<__int64, pair<CEString, __int64> > m_mmEndings;       // ending hash (64-bit) to ending data

    typedef std::multimap<__int64, pair<CEString, __int64> > EndingsMultiMap;
    typedef pair<EndingsMultiMap::const_iterator, EndingsMultiMap::const_iterator> ItPair;

    CLexeme * m_pLexeme;

};      // class CEndings

class CNounEndings : public CEndings
{
public:
    CNounEndings(CLexeme *);

    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding (const CEString&, const StEndingDescriptor&);

private:
//    virtual __int64 llHash(const StEndingDescriptor&);
};

class CAdjLongEndings : public CEndings
{
public:
    CAdjLongEndings(CLexeme *, ET_Subparadigm);
    
    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding (const CEString&, const StEndingDescriptor&);

private:
//    virtual __int64 llHash (const StEndingDescriptor&);

    ET_Subparadigm m_eSubparadigm;
};

class CAdjShortEndings : public CEndings
{
public:
    CAdjShortEndings(CLexeme * pLexeme);

    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding (const CEString&, const StEndingDescriptor&);

private:
//    virtual __int64 llHash(const StEndingDescriptor&);
};

class CPersonalEndings : public CEndings
{
public:
    CPersonalEndings(CLexeme * pLexeme);

    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding (const CEString&, const StEndingDescriptor&);

private:
//    virtual __int64 llHash(const StEndingDescriptor&);
};

class CInfinitiveEndings : public CEndings
{
public:
    CInfinitiveEndings(CLexeme * pLexeme);

    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding(const CEString&, const StEndingDescriptor&);

private:
//    virtual __int64 llHash(const StEndingDescriptor&);
};

class CPastTenseEndings : public CEndings
{
public:
    CPastTenseEndings(CLexeme * pLexeme);

    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding(const CEString&, const StEndingDescriptor&);

private:
//    virtual __int64 llHash(const StEndingDescriptor&);
};

/*
class CNullEnding : public CEndings
{
public:
    CNullEnding(CLexeme * pLexeme);

    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding(const CEString&, const StEndingDescriptor&);

private:
    virtual int iHash(const StEndingDescriptor&);
};
*/

} // namespace Hlib

#endif // ENDINGS_H_INCLUDED
