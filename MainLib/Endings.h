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
        m_vecEndings.clear();
    }

    int iCount()
    {
        return m_vecEndings.size();
    }

    ET_ReturnCode eGetEnding(int iSeqNum, CEString& sEnding, unsigned __int64& llEndingKey);
    ET_ReturnCode eGetEnding (CEString& s_, unsigned __int64& llKey)
    {
        return eGetEnding (0, s_, llKey);
    }

protected:
    void ReportDbError();
    std::vector<pair<unsigned __int64, CEString> > m_vecEndings;

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
    ET_ReturnCode eSelect(ET_Number, ET_Case, ET_StressLocation);                                            // nouns    

};      //  CNounEndings

class CAdjLongEndings : public CEndings
{
public:
    CAdjLongEndings(CLexeme *, ET_Subparadigm);

    ET_ReturnCode eSelect(ET_Subparadigm, ET_Gender, ET_Number, ET_Case, ET_Animacy);                     // long adjectives

    ET_Subparadigm m_eSubparadigm;

};   //  CAdjLongEndings

class CAdjShortEndings : public CEndings
{
public:
    CAdjShortEndings(CLexeme * pLexeme);

    ET_ReturnCode eSelect(ET_Gender, ET_Number, ET_StressLocation);                                          // short adjectives

};    //  CAdjShortEndings

class CPersonalEndings : public CEndings
{
public:
    CPersonalEndings(CLexeme * pLexeme);

    ET_ReturnCode eSelect(ET_Person, ET_Number, ET_StressLocation, ET_StemAuslaut);     // personal endings

};   //  CPersonalEndings

class CInfinitiveEndings : public CEndings
{
public:
    CInfinitiveEndings(CLexeme * pLexeme);

    void Select(int iInflectionType);                                                              // infinitive

};    //   CInfinitiveEndings

class CPastTenseEndings : public CEndings
{
public:
    CPastTenseEndings(CLexeme * pLexeme);

    void Select(ET_Gender, ET_Number, ET_StemAuslaut);                                             // past tense

};    //  CPastTenseEndings

} // namespace Hlib

#endif // ENDINGS_H_INCLUDED
