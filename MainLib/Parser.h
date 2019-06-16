#ifndef C_PARSER_H_INCLUDED
#define C_PARSER_H_INCLUDED

#include "IParser.h"

namespace Hlib
{

class CSqlite;
class CParsingTree;
struct CWordForm;

class CParser : public IParser
{

public:
    CParser(CSqlite *);
    ~CParser();

public:
    virtual ET_ReturnCode eParseText(const CEString& sName, const CEString& sMetaData, const CEString& sText);
    virtual ET_ReturnCode eParseWord(const CEString& sWord);
    virtual ET_ReturnCode eGetFirstWordForm(IWordForm *& pWordForm);
    virtual ET_ReturnCode eGetNextWordForm(IWordForm *& pWordForm);
    virtual void ClearResults();

    void SetDb(CSqlite * pDb);

private:
    CParser();  // no use

    CSqlite * m_pDb;
    CParsingTree * m_pEndingsTree;
    vector<CWordForm *> m_vecWordForms;
    vector<CWordForm *>::iterator m_itCurrentWordForm;
    CEString m_sTextName;
    CEString m_sTextTitle;
    CEString m_sTextMetaData;
    CEString m_sText;
    long long m_llTextDbId;

    ET_ReturnCode eIrregularFormLookup(const CEString&);
    ET_ReturnCode eWholeWordLookup(const CEString&);
    ET_ReturnCode eFormLookup(const CEString&);
    ET_ReturnCode eRemoveFalsePositives();
    ET_ReturnCode eQueryDb(const CEString& sSelect, uint64_t& uiQueryHandle);
    ET_ReturnCode eSaveWord(int iLine, int iWord, int iLineOffset, int iSegmentLength, long long& llWordDbKey);
    ET_ReturnCode eSaveWordParse(long long llSegmentId, long long llWordFormId);
    void HandleDbException(CException& ex);
};

}   // namespace Hlib

#endif // C_PARSER_H_INCLUDED