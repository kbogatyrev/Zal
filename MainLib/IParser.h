#ifndef I_PARSER_H_INCLUDED
#define I_PARSER_H_INCLUDED

#include "Enums.h"
#include "IWordForm.h"

namespace Hlib
{
    struct IParser
    {
        virtual ~IParser() {};
        virtual ET_ReturnCode eAnalyze(const CEString& sText) = 0;
        virtual ET_ReturnCode eGetFirstWordForm(IWordForm *& pWordForm) = 0;
        virtual ET_ReturnCode eGetNextWordForm(IWordForm *& pWordForm) = 0;
        virtual void ClearResults() = 0;
    };

}   // namespace Hlib

#endif  //  I_PARSER_H_INCLUDED
