#ifndef I_DICTIONARY_H_INCLUDED
#define I_DICTIONARY_H_INCLUDED

#include "Enums.h"
#include "EString.h"
#include "Callbacks.h"
#include "ILexeme.h"
#include "IWordForm.h"
#include "IParser.h"
#include "IAnalytics.h"
#include "IVerifier.h"
#include "LexemeProperties.h"

namespace Hlib
{
    typedef void(__stdcall* PROGRESS_CALLBACK_CLR) (int iPercentDone, bool bOperationComplete);
    typedef void(__cdecl* PROGRESS_CALLBACK_PYTHON) (int iPercentDone, bool bOperationComplete, int iRecord, double dDuration);

    struct  ILexemeEnumerator
    {
        virtual ET_ReturnCode eReset() = 0;

        virtual ET_ReturnCode eGetFirstLexeme(ILexeme*& pLexemeItf) = 0;
        virtual ET_ReturnCode eGetNextLexeme(ILexeme*& pLexemeItf) = 0;
    };
 
    struct IDictionary
    {
        virtual ~IDictionary() {};
        virtual ET_ReturnCode eSetDbPath (const CEString& sDbPath) = 0;
        virtual CEString sGetDbPath() = 0;
        virtual ET_ReturnCode eCreateLexemeForEdit(ILexeme *&) = 0;
        virtual ET_ReturnCode eCopyLexemeForEdit(const ILexeme * pSource, ILexeme *& pCopy) = 0;
        virtual ET_ReturnCode eGetLexemeById(long long Id, ILexeme *&) = 0;
        virtual ET_ReturnCode eGetLexemesByHash (const CEString& sMd5) = 0;
        virtual ET_ReturnCode eGetLexemesByGraphicStem (const CEString&) = 0;
        virtual ET_ReturnCode eGetLexemesByInitialForm (const CEString&) = 0;
        virtual ET_ReturnCode eGenerateAllForms() = 0;
        virtual ET_ReturnCode eGenerateFormsForSelectedLexemes() = 0;
        virtual ET_ReturnCode eCountLexemes (int64_t& iLexemes) = 0;

        virtual int nLexemesFound() = 0;

        virtual void Clear() = 0;
        virtual ET_ReturnCode Clear(ILexeme *) = 0;

        virtual ET_ReturnCode eCreateLexemeEnumerator(ILexemeEnumerator*&) = 0;
        virtual void DeleteLexemeEnumerator(ILexemeEnumerator *) = 0;

        virtual ET_ReturnCode eGetParser(IParser *& p) = 0;
        virtual ET_ReturnCode eGetAnalytics(IAnalytics*& p) = 0;
        virtual ET_ReturnCode eGetVerifier(IVerifier *& pVerifier) = 0;
        virtual ET_ReturnCode eExportTestData(const CEString& sPath, PROGRESS_CALLBACK_CLR) = 0;
        virtual ET_ReturnCode eImportTestData(const CEString& sPath, PROGRESS_CALLBACK_CLR) = 0;

        virtual ET_ReturnCode ePopulateHashToDescriptorTable(PROGRESS_CALLBACK_CLR, PROGRESS_CALLBACK_PYTHON=nullptr) = 0;

        virtual ET_ReturnCode eSaveHeadword(ILexeme* pLexeme) = 0;
        virtual ET_ReturnCode eUpdateHeadword(ILexeme* pLexeme) = 0;
        virtual ET_ReturnCode eSaveHeadwordStress(ILexeme* pLexeme) = 0;
        virtual ET_ReturnCode eSaveHomonyms(ILexeme* pLexeme) = 0;
        virtual ET_ReturnCode eDeleteLexeme(ILexeme* pLexeme) = 0;
        virtual ET_ReturnCode eSaveAspectPairInfo(ILexeme * pLexeme) = 0;
        virtual ET_ReturnCode eSaveP2Info(ILexeme * pLexeme) = 0;
        virtual ET_ReturnCode eSaveDescriptorInfo(ILexeme * pLexeme) = 0;
        virtual ET_ReturnCode eSaveCommonDeviation(ILexeme * pLexeme) = 0;
        virtual ET_ReturnCode eSaveInflectionInfo(ILexeme * pLexeme) = 0;
//        virtual ET_ReturnCode eSaveMissingFormHashes(ILexeme* pLexeme) = 0;

//        virtual ET_ReturnCode eSaveLexeme(ILexeme * pL) = 0;
    };

}   // namespace Hlib

#endif  //  I_DICTIONARY_H_INCLUDED
