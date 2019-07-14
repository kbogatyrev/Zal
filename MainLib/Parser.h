#ifndef C_PARSER_H_INCLUDED
#define C_PARSER_H_INCLUDED

#include "IParser.h"

namespace Hlib
{

class CSqlite;
class CParsingTree;
struct CWordForm;

struct StWordParse
{
    int iNumber;
    int iLineOffset;
    int iLength;
    int iPosInTactGroup;
    long long llLineDbId;
    long long llWordFormDbId;
    long long llWordToWordFormId;
    CWordForm* pWordForm;
};

//  CREATE TABLE tact_group(id INTEGER PRIMARY KEY ASC, line_id INTEGER, first_word_position INTEGER, num_of_words INTEGER, 
//  source TEXT, transcription TEXT, stressed_syllable INTEGER, reverse_stressed_syllable INTEGER, FOREIGN KEY(line_id) REFERENCES lines_in_text(id));
struct StTactGroup
{
    long long llLineId;
    int iFirstWordNum;
    int iNumOfWords;
    int iStressPos;
    int iReverseStressPos;
    int iSecondaryStressPos;
    CEString sSource;
    CEString sTranscription;
    vector<StWordParse> vecWords;

    StTactGroup()
    {
        Reset();
    }

    void Reset()
    {
        llLineId = -1;
        iFirstWordNum = -1;
        iNumOfWords = 0;
        iStressPos = -1;
        iReverseStressPos = -1;
        iSecondaryStressPos = -1;
        sSource.Erase();
        sTranscription.Erase();
        vecWords.clear();
    }
};

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
    ET_ReturnCode eSaveLine(long long llTextId, int iLineNum, int iTextOffset, int iLength, int iNumOfWords, const CEString& sText, long long& llDbKey);
    ET_ReturnCode eSaveWord(long long llLineDbId, int iLine, int iWord, int iLineOffset, int iSegmentLength, const CEString& sWord, long long& llWordDbKey);
    ET_ReturnCode eSaveWordParse(long long llSegmentId, long long llWordFormId, long long& llWordToWordFormId);
    ET_ReturnCode eSaveTactGroups(vector<StTactGroup>&);
    ET_ReturnCode eClearTextData(long long llText);
    bool bIsProclitic(CWordForm*);
    bool bIsEnclitic(CWordForm*);
    bool bArePhoneticallyIdentical(CWordForm* pWf1, CWordForm* pWf2);

    void HandleDbException(CException& ex);
};

}   // namespace Hlib

#endif // C_PARSER_H_INCLUDED