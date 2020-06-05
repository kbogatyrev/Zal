#ifndef C_TRANSCRIBER_H_INCLUDED
#define C_TRANSCRIBER_H_INCLUDED

#include <vector>
#include <map>

#include "Enums.h"
#include "EString.h"

namespace Hlib
{
    class CTranscriber
    {
    public:
        CTranscriber(const CEString& sDbPath);
        ET_ReturnCode eLoadTranscriptionRules();

    private:
        CEString m_sDbPath;

        vector<CEString> m_vecInputs;

    };

}   // namespace Hlib

#endif      //  C_TRANSCRIBER_H_INCLUDED