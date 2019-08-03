#ifndef I_ANALYTICS_H_INCLUDED
#define I_ANALYTICS_H_INCLUDED

#include "Enums.h"
#include "EString.h"

namespace Hlib
{
    struct IAnalytics
    {
        virtual ET_ReturnCode eParseText(const CEString& sName, const CEString& sMetaData, const CEString& sText) = 0;
        virtual void ClearResults() = 0;
    };

}   // namespace Hlib

#endif  //  I_ANALYTICS_H_INCLUDED
