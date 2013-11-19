#ifndef I_DICTIONARY_H_INCLUDED
#define I_DICTIONARY_H_INCLUDED

#include "Enums.h"
#include "LexemeProperties.h"

struct IDictionary
{
    virtual ET_ReturnCode eSetDbPath (const CEString& sDbPath) = 0;
    virtual ET_ReturnCode eGetLexemeById (long Id) = 0;
    virtual ET_ReturnCode eGetLexemesByMd5 (const CEString& sMd5) = 0;
    virtual ET_ReturnCode eGetLexemesByGraphicStem (const CEString&) = 0;
    virtual ET_ReturnCode eGetLexemesByInitialForm (const CEString&) = 0;
    virtual ET_ReturnCode eGenerateAllForms(CProgressCallback& progress) = 0;
    virtual ET_ReturnCode eCountLexemes (int& iLexemes) = 0;

    virtual void Clear() = 0;
};

#endif  //  I_DICTIONARY_H_INCLUDED
