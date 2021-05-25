#ifndef C_PARSER_H_INCLUDED
#define C_PARSER_H_INCLUDED

#include <memory>
#include <vector>

#include "Dictionary.h"
#include "Enums.h"
#include "EString.h"

#include "IParser.h"

namespace Hlib
{

class CSqlite;
class CParsingTree;
struct CWordForm;
class CLexeme;

class CParser : public IParser
{

public:
    CParser() = delete;
    CParser(CDictionary *);
    ~CParser();

public:
    virtual ET_ReturnCode eParseWord(const CEString& sWord);
    virtual ET_ReturnCode eGetFirstWordForm(IWordForm *& pWordForm);
    virtual ET_ReturnCode eGetNextWordForm(IWordForm *& pWordForm);
    virtual void ClearResults();

//  Non-virtual versions to be used internally:
    ET_ReturnCode eGetFirstWordForm(CWordForm *& wordForm);
    ET_ReturnCode eGetNextWordForm(CWordForm *& wordForm);

    void SetDb(shared_ptr<CSqlite> pDb);

private:
    CDictionary * m_pDictionary;
    shared_ptr<CSqlite> m_pDb;
    CParsingTree * m_pEndingsTree;
    vector<CWordForm *> m_vecWordForms;
    vector<CWordForm *>::iterator m_itCurrentWordForm;

    ET_ReturnCode eIrregularFormLookup(const CEString&, bool bIsSpryazgSm = false);
    ET_ReturnCode eWholeWordLookup(const CEString&);
    ET_ReturnCode eFormLookup(const CEString&);
    ET_ReturnCode eLexemeLookup(const CLexeme&);
    ET_ReturnCode eRemoveFalsePositives();
    ET_ReturnCode eQueryDb(const CEString& sSelect, uint64_t& uiQueryHandle);

    void HandleDbException(CException& ex);
};

}   // namespace Hlib

#endif // C_PARSER_H_INCLUDED