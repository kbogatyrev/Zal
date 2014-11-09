#ifndef I_VERIFIER_H_INCLUDED
#define I_VERIFIER_H_INCLUDED

#include "Enums.h"
#include "EString.h"
#include "Callbacks.h"
#include "ILexeme.h"
#include "LexemeProperties.h"

namespace Hlib
{
    struct IVerifier
    {
        virtual ET_ReturnCode eVerify(const CEString& sLexemeHash) = 0;
        virtual ET_TestResult eResult() = 0;
        virtual int iCount() = 0;

        virtual ET_ReturnCode eLoadStoredLexemes() = 0;
        virtual ET_ReturnCode eDeleteStoredLexeme(const CEString& sLexemeHash) = 0;

        virtual ET_ReturnCode eGetFirstLexemeData(CEString& sLexemeHash, CEString& sHeadword) = 0;
        virtual ET_ReturnCode eGetNextLexemeData(CEString& sLexemeHash, CEString& sHeadword) = 0;

        virtual ET_ReturnCode eGetFirstWordForm(IWordForm *& pWordForm) = 0;
        virtual ET_ReturnCode eGetNextWordForm(IWordForm *& pWordForm) = 0;
    };

}   // namespace Hlib

#endif  //  I_VERIFIER_H_INCLUDED
