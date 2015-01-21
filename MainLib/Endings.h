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

class CEndings
{
public:
    CEndings(CLexeme * pLexeme) : m_pLexeme(pLexeme), m_ullDbKey(0)
    {}

    virtual ~CEndings()
    {}

    virtual void Reset()
    {
        m_mmEndings.clear();
    }

    int iCount()
    {
//        unsigned __int64 ullH = ullHash();
//        return m_mmEndings.count (ullH);
        return m_vecEndings.size();
    }

    virtual ET_ReturnCode eLoad() = 0;
    virtual ET_ReturnCode eAddEnding() = 0;

    // Set ending data for inflection category
    ET_ReturnCode Select(ET_Number, ET_Case, ET_StressLocation);                                            // nouns    
    ET_ReturnCode Select(ET_Subparadigm, ET_Gender, ET_Number, ET_Case, ET_Animacy);                     // long adjectives
    void Select(ET_Gender, ET_Number, ET_StressLocation);                                          // short adjectives
    ET_ReturnCode Select(ET_Person, ET_Number, ET_StressLocation, ET_StemAuslaut, int iInflectionType, ET_Reflexive);     // personal endings
    void Select(int iInflectionType);                                                              // infinitive
    void Select(ET_Gender, ET_Number, ET_StemAuslaut);                                             // past tense

    ET_ReturnCode eGetEnding(int iSeqNum, CEString& sEnding, unsigned __int64& llEndingKey);

/*
    ET_ReturnCode eGetEnding (int iSeqNum, CEString& sEnding, unsigned __int64& llEndingKey)
    {
        unsigned __int64 llKey = ullHash();

        ItPair pair_Range = m_mmEndings.equal_range (llKey);
        for (int iItem = 0; iItem < iSeqNum; ++iItem)
        {
            if (++pair_Range.first == pair_Range.second)
            {
                CEString sMsg (L"Error getting ending from hash.");
                ERROR_LOG (sMsg);
                return H_ERROR_GENERAL;
            }
        }

        pair<CEString, unsigned __int64> pairEndingData = (*pair_Range.first).second;
        sEnding = pairEndingData.first;
        llEndingKey = pairEndingData.second;

        return H_NO_ERROR;
    }
*/

    ET_ReturnCode eGetEnding (CEString& s_, unsigned __int64& llKey)
    {
        return eGetEnding (0, s_, llKey);
    }

protected:
    virtual unsigned __int64 ullHash() const = 0;
    void ReportDbError();
    std::multimap<unsigned __int64, pair<CEString, unsigned __int64> > m_mmEndings;       // ending hash (64-bit) to ending data key
    std::vector<pair<unsigned __int64, CEString> > m_vecEndings;

    typedef std::multimap<unsigned __int64, pair<CEString, unsigned __int64> > EndingsMultiMap;
    typedef pair<EndingsMultiMap::const_iterator, EndingsMultiMap::const_iterator> ItPair;

    CLexeme * m_pLexeme;
    unsigned __int64 m_ullDbKey;
    CEString m_sEnding;

    ET_Gender m_eGender;
    ET_Number m_eNumber;
    ET_Case m_eCase;
    ET_Person m_ePerson;
    ET_Animacy m_eAnimacy;
    ET_StressLocation m_eStressPos;
    ET_StemAuslaut m_eStemAuslaut;
    int m_iInflectionType;

};      // class CEndings

class CNounEndings : public CEndings
{
public:
    CNounEndings(CLexeme *);

    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding();

private:
    virtual unsigned __int64 ullHash() const;

};      //  CNounEndings

class CAdjLongEndings : public CEndings
{
public:
    CAdjLongEndings(CLexeme *, ET_Subparadigm);

    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding();

private:
    virtual unsigned __int64 ullHash() const;

    ET_Subparadigm m_eSubparadigm;

};   //  CAdjLongEndings

class CAdjShortEndings : public CEndings
{
public:
    CAdjShortEndings(CLexeme * pLexeme);

    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding();

private:
    virtual unsigned __int64 ullHash() const;

};    //  CAdjShortEndings

class CPersonalEndings : public CEndings
{
public:
    CPersonalEndings(CLexeme * pLexeme);

    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding();

private:
    virtual unsigned __int64 ullHash() const;

};   //  CPersonalEndings

class CInfinitiveEndings : public CEndings
{
public:
    CInfinitiveEndings(CLexeme * pLexeme);

    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding();

private:
    virtual unsigned __int64 ullHash() const;

};    //   CInfinitiveEndings

class CPastTenseEndings : public CEndings
{
public:
    CPastTenseEndings(CLexeme * pLexeme);

    virtual ET_ReturnCode eLoad();
    virtual ET_ReturnCode eAddEnding();

private:
    virtual unsigned __int64 ullHash() const;
};    //  CPastTenseEndings

} // namespace Hlib

#endif // ENDINGS_H_INCLUDED
