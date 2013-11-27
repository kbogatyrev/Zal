#ifndef C_DICTIONARY_H_INCLUDED
#define C_DICTIONARY_H_INCLUDED

#include "LexemeProperties.h"
#include "IDictionary.h"

class CLexeme;
class CSqlite;

class CDictionary : public IDictionary
{

public:
    CDictionary() : m_pDb(NULL)
    {}

    ~CDictionary();

public:
    virtual ET_ReturnCode eSetDbPath (const CEString& sDbPath);
    virtual ET_ReturnCode eGetLexemeById (long Id);
    virtual ET_ReturnCode eGetLexemesByMd5 (const CEString& sMd5);
    virtual ET_ReturnCode eGetLexemesByGraphicStem (const CEString&);
    virtual ET_ReturnCode eGetLexemesByInitialForm (const CEString&);
    virtual ET_ReturnCode eGenerateAllForms(CProgressCallback& progress);
    virtual ET_ReturnCode eCountLexemes (int& iLexemes);

    virtual ET_ReturnCode eGetFirstLexeme (ILexeme *& pLexeme);
    virtual ET_ReturnCode eGetNextLexeme (ILexeme *& pLexeme);

    virtual void Clear();

    // Not part ot interface:

    CSqlite * pGetDbHandle();

private:
    CEString m_sDbPath;
    CSqlite * m_pDb;
    vector<CLexeme *> m_vecLexemes;
    vector<CLexeme *>::iterator m_itCurrentLexeme;

    // Execute DB query and retrieve results
    ET_ReturnCode eQueryDb (const CEString& sSelect);
    ET_ReturnCode eGetSingleRow (StLexemeProperties&);

    ET_ReturnCode eReadFromDb (__int64 llLexemeId);
    ET_ReturnCode eReadFromDb (const CEString& sStem);

    ET_ReturnCode eGetL2Data (__int64 llLexemeId, StLexemeProperties& properties);

    void HandleDbException (CException&);
};

#endif // C_DICTIONARY_H_INCLUDED