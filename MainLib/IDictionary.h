#ifndef I_DICTIONARY_H_INCLUDED
#define I_DICTIONARY_H_INCLUDED

#include "Enums.h"
#include "EString.h"
#include "Callbacks.h"
#include "ILexeme.h"
#include "IWordForm.h"
#include "IParser.h"
#include "IVerifier.h"
#include "LexemeProperties.h"

namespace Hlib
{
    // Progress delegate invokedfrom C#/CLR
    typedef void(__stdcall *PROGRESS_CALLBACK_CLR) (int iPercentDone, bool bOperationComplete);

    struct IDictionary
    {
        virtual ~IDictionary() {};
        virtual ET_ReturnCode eSetDbPath (const CEString& sDbPath) = 0;
        virtual CEString sGetDbPath() = 0;
        virtual ET_ReturnCode eCreateLexemeForEdit(ILexeme *&) = 0;
        virtual ET_ReturnCode eCopyLexemeForEdit(const ILexeme * pSource, ILexeme *& pCopy) = 0;
        virtual ET_ReturnCode eGetLexemeById(int Id) = 0;
        virtual ET_ReturnCode eGetLexemesByHash (const CEString& sMd5) = 0;
        virtual ET_ReturnCode eGetLexemesByGraphicStem (const CEString&) = 0;
        virtual ET_ReturnCode eGetLexemesByInitialForm (const CEString&) = 0;
        virtual ET_ReturnCode eGenerateAllForms () = 0;
        virtual ET_ReturnCode eGenerateFormsForSelectedLexemes() = 0;
        virtual ET_ReturnCode eCountLexemes (int64_t& iLexemes) = 0;

        virtual ET_ReturnCode eGetFirstLexeme (ILexeme *& pLexeme) = 0;
        virtual ET_ReturnCode eGetNextLexeme (ILexeme *& pLexeme) = 0;

        virtual void Clear() = 0;
        virtual ET_ReturnCode Clear(ILexeme *) = 0;

        virtual ET_ReturnCode eGetParser(IParser *& p) = 0;
        virtual ET_ReturnCode eGetVerifier(IVerifier *& pVerifier) = 0;
        virtual ET_ReturnCode eExportTestData(const CEString& sPath, PROGRESS_CALLBACK_CLR) = 0;
        virtual ET_ReturnCode eImportTestData(const CEString& sPath, PROGRESS_CALLBACK_CLR) = 0;

        virtual ET_ReturnCode eSaveLexeme(ILexeme * pL) = 0;
    };

}   // namespace Hlib

#endif  //  I_DICTIONARY_H_INCLUDED
