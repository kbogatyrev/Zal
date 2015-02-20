#ifndef C_DICTIONARY_H_INCLUDED
#define C_DICTIONARY_H_INCLUDED

#include "LexemeProperties.h"
#include "IDictionary.h"

namespace Hlib
{

class CLexeme;
class CSqlite;
struct CWordForm;
class CParser;
class CVerifier;

class CDictionary : public IDictionary
{

public:
    CDictionary();
    ~CDictionary();

public:
    virtual ET_ReturnCode eSetDbPath(const CEString& sDbPath);
    virtual CEString sGetDbPath();
    virtual ET_ReturnCode eGetLexemeById(int Id);
    virtual ET_ReturnCode eGetLexemesByHash(const CEString& sMd5);
    virtual ET_ReturnCode eGetLexemesByGraphicStem(const CEString&);
    virtual ET_ReturnCode eGetLexemesByInitialForm(const CEString&);
    virtual ET_ReturnCode eGenerateAllForms();
    virtual ET_ReturnCode eGenerateFormsForSelectedLexemes();
    virtual ET_ReturnCode eCountLexemes(int& iLexemes);

    virtual ET_ReturnCode eGetFirstLexeme(ILexeme *& pLexeme);
    virtual ET_ReturnCode eGetNextLexeme(ILexeme *& pLexeme);

    virtual void Clear();

    virtual ET_ReturnCode eGetParser(IParser *& p);
    virtual ET_ReturnCode eGetVerifier(IVerifier *& pVerifier);
    virtual ET_ReturnCode eExportTestData(const CEString& sPath, PROGRESS_CALLBACK_CLR);
    virtual ET_ReturnCode eImportTestData(const CEString& sPath, PROGRESS_CALLBACK_CLR);

    // Not part ot interface:
    CSqlite * pGetDbHandle();

private:
    CEString m_sDbPath;
    CSqlite * m_pDb;
    CParser * m_pParser;
    CVerifier * m_pVerifier;
    vector<CLexeme *> m_vecLexemes;
    vector<CWordForm *> m_vecWordForms;
    vector<CLexeme *>::iterator m_itCurrentLexeme;
    vector<CWordForm *>::iterator m_itCurrentWordForm;

    // Populate DB tables
    ET_ReturnCode ePopulateStemsTable();
    ET_ReturnCode eUpdateStemIds();
    ET_ReturnCode ePopulateWordFormDataTables();

    // Execute DB query and retrieve results
    ET_ReturnCode eQueryDb(const CEString& sSelect, unsigned int& uiQuertHandle);
    ET_ReturnCode eGetSingleRow(StLexemeProperties&, unsigned int uiQuertHandle);

    ET_ReturnCode eReadFromDb(__int64 llLexemeId);
    ET_ReturnCode eReadFromDb(const CEString& sStem);

    ET_ReturnCode eGetL2Data(__int64 llLexemeId, StLexemeProperties& properties);

    void HandleDbException(CException&);
};

}   // namespace Hlib

#endif // C_DICTIONARY_H_INCLUDED