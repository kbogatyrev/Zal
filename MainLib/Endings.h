#ifndef ENDINGS_H_INCLUDED
#define ENDINGS_H_INCLUDED

#include <vector>
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
    int iInflectionType;
    ET_Gender eGender;
    ET_Number eNumber;
    ET_Case eCase;
    ET_Animacy eAnimacy;
    ET_Person ePerson;
    ET_StressLocation eStress;
    ET_StemAuslaut eStemAuslaut;

    StEndingDescriptor() : llDbKey(-1),
                           iInflectionType (INFLECTION_TYPE_UNDEFINED),
                           eGender (GENDER_UNDEFINED), 
                           eNumber (NUM_UNDEFINED), 
                           eCase (CASE_UNDEFINED), 
                           eAnimacy (ANIM_UNDEFINED), 
                           ePerson (PERSON_UNDEFINED),
                           eStress (STRESS_LOCATION_UNDEFINED),
                           eStemAuslaut (STEM_AUSLAUT_UNDEFINED)
    {}

    // Overload for nouns
    StEndingDescriptor (ET_Number eN, 
                        ET_Case eC, 
                        ET_StressLocation eS) :
                            llDbKey(-1),
                            iInflectionType (INFLECTION_TYPE_UNDEFINED),
                            eGender (GENDER_UNDEFINED), 
                            eNumber (eN),
                            eCase (eC), 
                            eAnimacy (ANIM_UNDEFINED), 
                            ePerson (PERSON_UNDEFINED),
                            eStress (eS),
                            eStemAuslaut (STEM_AUSLAUT_UNDEFINED)
    {}

    // Overload for long adjectves
    StEndingDescriptor (ET_Gender eG,
                        ET_Number eN, 
                        ET_Case eC,
                        ET_Animacy eA,
                        ET_StressLocation eS) :
                            llDbKey(-1),
                            iInflectionType (INFLECTION_TYPE_UNDEFINED),
                            eGender (eG), 
                            eNumber (eN),
                            eCase (eC),
                            eAnimacy (eA),
                            ePerson (PERSON_UNDEFINED),
                            eStress (eS),
                            eStemAuslaut (STEM_AUSLAUT_UNDEFINED)
    {}

    // Overload for short adjectives
    StEndingDescriptor (ET_Gender eG,
                        ET_Number eN,
                        ET_StressLocation eS) :
                            llDbKey(-1),
                            iInflectionType (INFLECTION_TYPE_UNDEFINED),
                            eGender (eG), 
                            eNumber (eN),
                            eCase (CASE_UNDEFINED), 
                            eAnimacy (ANIM_UNDEFINED), 
                            ePerson (PERSON_UNDEFINED),
                            eStress (eS),
                            eStemAuslaut (STEM_AUSLAUT_UNDEFINED)
    {}

    // Overload for personal forms
    StEndingDescriptor (int iInflectionType,
                        ET_Person eP,
                        ET_Number eN,
                        ET_StressLocation eS,
                        ET_StemAuslaut eSa) :
                            llDbKey(-1),
                            iInflectionType (iInflectionType),
                            eGender (GENDER_UNDEFINED),
                            eNumber (eN),
                            eCase (CASE_UNDEFINED),
                            eAnimacy (ANIM_UNDEFINED),
                            ePerson (eP), 
                            eStress (eS),
                            eStemAuslaut (eSa)
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

};      //  struct StEndingDescriptor

//
// Endings parser node
//
struct StNode
{
    wchar_t cLetter;
    StNode * pPrevious;
    vector<StNode *> vecNext;
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

        return false;

    }   //  bool operator() (const CEString& sLeft, const CEString& sRight)
};  //  struct StReverseComparisonFunctor

//
// Endings parser tree
//
class CParsingTree
{
public:
    ET_ReturnCode eGetFirstMatch(StEndingDescriptor& stEnding);
    ET_ReturnCode eGetNextMatch(StEndingDescriptor& stEnding);

private:
    typedef map<CEString, StEndingDescriptor, StReverseComparisonFunctor> MapEndingToDescriptor;
    MapEndingToDescriptor m_mapSortedEndingsList;
//    MapEndingToDescriptor::iterator itEnding;

    vector<StNode *> m_vecFinales;
    vector<StEndingDescriptor> m_vecMatches;

public:
    ET_ReturnCode Load(CSqlite * pDb);
    void AddLevel(int iPos, MapEndingToDescriptor::iterator itFirst, MapEndingToDescriptor::iterator itEnd);

};

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

    virtual int iHash (const StEndingDescriptor&) = 0;

    int iCount (const StEndingDescriptor& stD)
    {
        int iH = iHash (stD);
        return m_mmEndings.count (iH);
    }

    virtual ET_ReturnCode eLoad() = 0;
    virtual ET_ReturnCode eAddEnding (const CEString&, const StEndingDescriptor&) = 0;

    ET_ReturnCode eGetEnding (const StEndingDescriptor& stDescriptor, int iSeqNum, CEString& sEnding, __int64& llEndingKey)
    {
        int iKey = iHash (stDescriptor);

        ItPair pair_Range = m_mmEndings.equal_range (iKey);
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
    std::multimap<int, pair<CEString, __int64> > m_mmEndings;       // ending hash to ending data

    typedef std::multimap<int, pair<CEString, __int64> > EndingsMultiMap;
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
    virtual int iHash (const StEndingDescriptor&);
};

class CAdjLongEndings : public CEndings
{
public:
    CAdjLongEndings(CLexeme *, ET_Subparadigm);

    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding (const CEString&, const StEndingDescriptor&);

private:
    virtual int iHash (const StEndingDescriptor&);

    ET_Subparadigm m_eSubparadigm;
};

class CAdjShortEndings : public CEndings
{
public:
    CAdjShortEndings(CLexeme * pLexeme);

    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding (const CEString&, const StEndingDescriptor&);

private:
    virtual int iHash (const StEndingDescriptor&);
};

class CPersonalEndings : public CEndings
{
public:
    CPersonalEndings(CLexeme * pLexeme);

    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding (const CEString&, const StEndingDescriptor&);

private:
    virtual int iHash (const StEndingDescriptor&);
};

} // namespace Hlib

#endif // ENDINGS_H_INCLUDED
