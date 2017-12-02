#ifndef I_ASPECTPAIR_H_INCLUDED
#define I_ASPECTPAIR_H_INCLUDED

#include "Enums.h"
#include "EString.h"
#include "Callbacks.h"
#include "ILexeme.h"
#include "IDictionary.h"
#include "LexemeProperties.h"

namespace Hlib
{
    struct IAspectPair
    {
        virtual ~IAspectPair() {};

        virtual ET_ReturnCode eGetFirstAspectPair(IWordForm *& pWordForm) = 0;
        virtual ET_ReturnCode eGetNextAspectPair(IWordForm *& pWordForm) = 0;
        virtual ET_ReturnCode eGetFirstAltAspectPair(IWordForm *& pWordForm) = 0;
        virtual ET_ReturnCode eGetNextAltAspectPair(IWordForm *& pWordForm) = 0;
    };

}   // namespace Hlib

#endif  //  I_ASPECTPAIR_H_INCLUDED
