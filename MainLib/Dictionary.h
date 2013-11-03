#ifndef CDICTIONARY_H_INCLUDED
#define CDICTIONARY_H_INCLUDED

class CLexeme;

class CDictionary
{

friend class CVerifier;

public:
    CDictionary() : m_pDb(NULL)
    {}

    ~CDictionary()
    {
        // ??? WHO DELETES CLexeme objects?
        delete m_pDb;
    }

public:
    ET_ReturnCode eSetDbPath (const CEString& sDbPath);
    ET_ReturnCode eGetLexemeById (long Id, CLexeme *& pLexeme);
    ET_ReturnCode eGetLexemesByMd5 (const CEString& sMd5);
    ET_ReturnCode eGetLexemesByGraphicStem (const CEString&);
    ET_ReturnCode eGetLexemesByInitialForm (const CEString&);
    ET_ReturnCode eGenerateAllForms(CProgressCallback& progress);
    ET_ReturnCode eCountLexemes (int& iLexemes);

    void Clear();

private:
    CEString m_sDbPath;
    CSqlite * m_pDb;
    vector<CLexeme *> m_vecLexemes;

    // Execute DB query and retrieve results
    ET_ReturnCode eQueryDb (const CEString& sSelect);
    ET_ReturnCode eGetSingleRow (CLexeme *& pLexeme);    // enumeraror; lexeme ptr must be deleted by caller

    ET_ReturnCode eReadFromDb (__int64 llLexemeId);
    ET_ReturnCode eReadFromDb (const CEString& sStem);

    ET_ReturnCode eGetL2Data (__int64 llLexemeId, CLexeme& Lexeme);

    void HandleDbException (CException&);
};

#endif // CDICTIONARY_H_INCLUDED