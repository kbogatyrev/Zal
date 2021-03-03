#ifndef C_DICTIONARY_H_INCLUDED
#define C_DICTIONARY_H_INCLUDED

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <memory>

#include "Enums.h"
#include "EString.h"
#include "IDictionary.h"
#include "LexemeProperties.h"

namespace Hlib
{

class CLexeme;
class CSqlite;
struct CWordForm;
class CParser;
class CAnalytics;
class CVerifier;

class CLexemeEnumerator : public ILexemeEnumerator
{
public:
    CLexemeEnumerator() = delete;
    CLexemeEnumerator(CDictionary * dict) : m_pDictionary(dict) {}

    virtual ET_ReturnCode eReset();

    virtual ET_ReturnCode eGetFirstLexeme(ILexeme*& pLexemeItf);
    virtual ET_ReturnCode eGetNextLexeme(ILexeme*& pLexemeItf);

    virtual ET_ReturnCode eGetFirstLexeme(CLexeme*& pLexeme);
    virtual ET_ReturnCode eGetNextLexeme(CLexeme*& pLexeme);

private:
    vector<CLexeme*>::iterator m_itCurrentLexeme;
    CDictionary * m_pDictionary;
};

class CDictionary : public IDictionary
{
    friend class CLexemeEnumerator;

public:
    CDictionary();
    ~CDictionary();

public:
    virtual ET_ReturnCode eSetDbPath(const CEString& sDbPath);
    virtual CEString sGetDbPath();
    virtual ET_ReturnCode eCreateLexemeForEdit(ILexeme *&);
    virtual ET_ReturnCode eCopyLexemeForEdit(const ILexeme * pSource, ILexeme *& pCopy);
    virtual ET_ReturnCode eGetLexemeById(long long Id, ILexeme *& pLexeme);
    virtual ET_ReturnCode eGetLexemesByHash(const CEString& sMd5);
    virtual ET_ReturnCode eGetLexemesByGraphicStem(const CEString&);
    virtual ET_ReturnCode eGetLexemesByInitialForm(const CEString&);
    virtual ET_ReturnCode eGenerateAllForms();
    virtual ET_ReturnCode eGenerateFormsForSelectedLexemes();
    virtual ET_ReturnCode eCountLexemes(int64_t& iLexemes);

//    virtual ET_ReturnCode eGetFirstLexeme(ILexeme *& pLexemeItf);
//    virtual ET_ReturnCode eGetNextLexeme(ILexeme *& pLexemeItf);

    virtual int nLexemesFound();
//    virtual ET_ReturnCode eGetFirstLexeme(CLexeme *& pLexeme);
//    virtual ET_ReturnCode eGetNextLexeme(CLexeme *& pLexeme);

    virtual void Clear();
    virtual ET_ReturnCode Clear(ILexeme *);

    virtual ET_ReturnCode eCreateLexemeEnumerator(ILexemeEnumerator*&);
    virtual void DeleteLexemeEnumerator(ILexemeEnumerator*);

    virtual ET_ReturnCode eGetParser(IParser *& p);
    virtual ET_ReturnCode eGetAnalytics(IAnalytics*& p);
    virtual ET_ReturnCode eGetVerifier(IVerifier *& pVerifier);

    virtual ET_ReturnCode eExportTestData(const CEString& sPath, PROGRESS_CALLBACK_CLR);
    virtual ET_ReturnCode eImportTestData(const CEString& sPath, PROGRESS_CALLBACK_CLR);

    virtual ET_ReturnCode eDeleteLexeme(ILexeme * pLexeme);
    virtual ET_ReturnCode eUpdateHeadword(ILexeme* pLexeme);
    virtual ET_ReturnCode eSaveNewHeadword(ILexeme* pLexeme);
    virtual ET_ReturnCode eSaveHeadwordStress(ILexeme* pLexeme);
    virtual ET_ReturnCode eSaveHomonyms(ILexeme* pLexeme);
    virtual ET_ReturnCode eSaveAspectPairInfo(ILexeme * pLexeme);
    virtual ET_ReturnCode eSaveP2Info(ILexeme * pLexeme);
    virtual ET_ReturnCode eUpdateDescriptorInfo(ILexeme * pLexeme);
    virtual ET_ReturnCode eSaveDescriptorInfo(ILexeme* pLexeme);
    virtual ET_ReturnCode eSaveCommonDeviation(ILexeme * pLexeme);
    virtual ET_ReturnCode eSaveInflectionInfo(ILexeme * pLexeme);
//    virtual ET_ReturnCode eSaveMissingFormHashes(ILexeme * pLexeme);
//    virtual ET_ReturnCode eSaveLexeme(ILexeme * pLexeme);

    virtual ET_ReturnCode ePopulateHashToDescriptorTable(PROGRESS_CALLBACK_CLR, PROGRESS_CALLBACK_PYTHON=nullptr);

    // Not part ot interface:
    CSqlite * pGetDbHandle();

private:
    CEString m_sDbPath;
    shared_ptr<CSqlite> m_pDb;
    shared_ptr<CParser> m_pParser;
    CAnalytics* m_pAnalytics;
    CVerifier * m_pVerifier;
    vector<CLexeme *> m_vecLexemes;
    vector<CLexeme *>::iterator m_itCurrentLexeme;
    vector<CWordForm *>::iterator m_itCurrentWordForm;

    // Populate DB tables
    ET_ReturnCode ePopulateStemsTable();
    ET_ReturnCode eUpdateStemIds();
    ET_ReturnCode ePopulateWordFormDataTables();

    // Execute DB query and retrieve results
    ET_ReturnCode eQueryDb(const CEString& sSelect, uint64_t& uiQuertHandle);
    ET_ReturnCode eGetSingleRow(StLexemeProperties&, uint64_t uiQuertHandle);

    ET_ReturnCode eReadFromDb(__int64 llLexemeId);
    ET_ReturnCode eReadFromDb(const CEString& sStem);

    ET_ReturnCode eGetP2Data(__int64 llLexemeId, StLexemeProperties& properties);
    ET_ReturnCode eGetWordIdFromLemexeId(__int64 llLexemeId, __int64& llWordId);

    ET_ReturnCode eMarkLexemeAsEdited(ILexeme * pLexeme);

    ET_ReturnCode eDifficultFormLabelToGramHash(const CEString sLabel);

    void HandleDbException(CException&);
};

}   // namespace Hlib

#endif // C_DICTIONARY_H_INCLUDED