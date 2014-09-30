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
        virtual ET_ReturnCode eSetHeadWord (const CEString&) = 0;
        virtual ET_ReturnCode eSetHeadword(const CEString&) = 0;
        virtual CEString eGetHeadword() = 0;
        virtual void SetLexemeHash(const CEString&) = 0;
        virtual CEString eGetLexemeHash() = 0;
        virtual ET_ReturnCode eVerify(const CEString& sLexemeHash) = 0;
        virtual ET_TestResult eResult() = 0;

        virtual ET_ReturnCode eLoadStoredLexemes() = 0;
        virtual ET_ReturnCode eDeleteStoredLexeme(const CEString& sLexeme) = 0;
    };

}   // namespace Hlib

#endif  //  I_VERIFIER_H_INCLUDED
