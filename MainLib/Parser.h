#ifndef C_PARSER_H_INCLUDED
#define C_PARSER_H_INCLUDED

#include <vector>
#include <memory>

#include "Enums.h"
#include "EString.h"

#include "IParser.h"

namespace Hlib
{

class CSqlite;
class CParsingTree;
struct CWordForm;

class CParser : public IParser
{

public:
    CParser(shared_ptr<CSqlite>);
    ~CParser();

public:
    virtual ET_ReturnCode eParseWord(const CEString& sWord);
    virtual ET_ReturnCode eGetFirstWordForm(IWordForm *& pWordForm);
    virtual ET_ReturnCode eGetNextWordForm(IWordForm *& pWordForm);
    virtual void ClearResults();

//  Non-virtual versions to be used internally:
    ET_ReturnCode eGetFirstWordForm(CWordForm *& wordForm);
    ET_ReturnCode eGetNextWordForm(CWordForm *& wordForm);

    void SetDb(shared_ptr<CSqlite> spDb);

private:
    CParser();  // no use

    shared_ptr<CSqlite> m_spDb;
    unique_ptr<CParsingTree> m_spEndingsTree;
    vector<CWordForm *> m_vecWordForms;
    vector<CWordForm *>::iterator m_itCurrentWordForm;

    ET_ReturnCode eIrregularFormLookup(const CEString&);
    ET_ReturnCode eWholeWordLookup(const CEString&);
    ET_ReturnCode eFormLookup(const CEString&);
    ET_ReturnCode eRemoveFalsePositives();
    ET_ReturnCode eQueryDb(const CEString& sSelect, uint64_t& uiQueryHandle);

    void HandleDbException(CException& ex);
};

}   // namespace Hlib

#endif // C_PARSER_H_INCLUDED