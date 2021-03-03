#ifndef ANALYTICS_H
#define ANALYTICS_H

#include <set>
#include <vector>
#include <map>
#include <memory>

#include "Enums.h"
#include "EString.h"
#include "IAnalytics.h"
#include "WordForm.h"

namespace Hlib
{
    class CSqlite;
    class CParser;
    class CTranscriber;
//    struct CWordForm;
    class CEString;

    class CAnalytics : public IAnalytics
    {
    public:
        struct StWordParse
        {
            int iNumber;
            int iLineOffset;
            int iLength;
            int iPosInTactGroup;
            ET_WordStressType eStressType;
            long long llLineDbId;
            long long llWordInLIneDbId;
            long long llWordToWordFormId;
            CWordForm WordForm;

            StWordParse()
            {
                Reset();
            }

            void Reset()
            {
                int iNumber = -1;
                int iLineOffset = -1;
                int iLength = -1;
                int iPosInTactGroup = -1;
                long long llLineDbId = -1;
                eStressType = WORD_STRESS_TYPE_UNDEFINED;
                long long llWordInLIneDbId = -1;
                long long llWordToWordFormId = -1;
                CWordForm WordForm;
            }

            // Needed to be used with STL set
            bool operator < (const StWordParse& stRhs) const
            {
                return iNumber < stRhs.iNumber;
            }

        };      // StWordParse

        //  CREATE TABLE tact_group(id INTEGER PRIMARY KEY ASC, line_id INTEGER, first_word_position INTEGER, num_of_words INTEGER, 
        //  source TEXT, transcription TEXT, stressed_syllable INTEGER, reverse_stressed_syllable INTEGER, FOREIGN KEY(line_id) REFERENCES lines_in_text(id));
        struct StTactGroup
        {
            long long llLineId;
            int iFirstWordNum;
            int iMainWordPos;
            int iNumOfWords;
            int iNumOfSyllables;
            int iStressedSyllable;             // TODO: multiple stresses, dash-separated compounds etc
            int iReverseStressedSyllable;
            int iSecondaryStressedSyllable;
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
                iMainWordPos = -1;
                iNumOfWords = 0;
                iNumOfSyllables = -1;   
                iStressedSyllable = -1;
                iReverseStressedSyllable = -1;
                iSecondaryStressedSyllable = -1;
                sSource.Erase();
                sTranscription.Erase();
                vecWords.clear();
            }
        };      //  StTactGroup

    public:
        CAnalytics();
        CAnalytics(shared_ptr<CSqlite>, shared_ptr<CParser>);
        ~CAnalytics();

        virtual ET_ReturnCode eParseText(const CEString& sTextName, const CEString& sMetadata, const CEString& sText, long long& llParsedTextId);
        virtual void ClearResults();

    private:
        ET_ReturnCode eInit();
        ET_ReturnCode eParseMetadata(const CEString& sMetadata);
        ET_ReturnCode eRegisterText(const CEString& sTextName, const CEString sTextMetadata, const CEString& sText);
        ET_ReturnCode eParseWord(const CEString& sWord, const CEString& sLine, int iLine, int iNumInLine, int iWordsInLine, long long llLineDbKey);
        ET_ReturnCode eFindEquivalencies(CEString& sLine);
//        ET_ReturnCode eCountSyllables(StTactGroup&);
        ET_ReturnCode eGetStress(StTactGroup&);
        ET_ReturnCode eTranscribe(StTactGroup&);
        ET_ReturnCode eAssembleTactGroups(CEString& sLine);
        ET_ReturnCode eSaveLine(long long llTextId, int iLineNum, int iTextOffset, int iLength, int iNumOfWords, const CEString& sText, long long& llDbKey);
        ET_ReturnCode eSaveWord(long long llLineDbId, int iLine, int iWord, int iWordsInLine, int iLineOffset, int iSegmentLength, const CEString& sWord, long long& llWordDbKey);
        ET_ReturnCode eSaveWordParse(long long llSegmentId, long long llWordFormId, long long& llWordToWordFormId);
        ET_ReturnCode eSaveTactGroup(StTactGroup&);
        ET_ReturnCode eClearTextData(long long llText);
        bool bIsProclitic(const CWordForm&);
        bool bIsEnclitic(const CWordForm&);
        bool bArePhoneticallyIdentical(CWordForm& wf1, CWordForm& wf2);

    private:
        shared_ptr<CSqlite> m_pDb;
        shared_ptr<CParser> m_pParser;
        unique_ptr<CTranscriber> m_spTranscriber;
        CEString m_sTextName;
        CEString m_sTextTitle;
        CEString m_sTextMetaData;
        CEString m_sText;
        long long m_llTextDbId;

        multimap<int, StWordParse> m_mmapWordParses;
        multimap<int, vector<StWordParse>> m_mmapEquivalencies;  // word # --> phonetic invariant sets
        multimap<int, StTactGroup> m_mapTactGroups;   // 1st word # --> tact group

        vector<pair<CEString, CEString>> m_vecMetadataKeyValPairs;

    };      //  class CAnalytics

}   // namespace Hlib

#endif  //  ANALYTICS_H