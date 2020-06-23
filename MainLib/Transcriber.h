#ifndef C_TRANSCRIBER_H_INCLUDED
#define C_TRANSCRIBER_H_INCLUDED

#include <vector>
#include <map>

#include "Enums.h"
#include "EString.h"

namespace Hlib
{
    struct StCondition
    {
        ET_VowelStressRelation eStressContext;
        vector<ET_PHONEMIC_CONTEXT> vecLeftContexts;
        vector<wchar_t> vecLeftContextsChar;
        vector<ET_PHONEMIC_CONTEXT> vecRightContexts;
        vector<wchar_t> vecRightContextsChar;
    };
    
    class CRule
    {
        vector<CEString> m_vecInputs;

        ET_VowelStressRelation m_eStressContext;
        vector<ET_PHONEMIC_CONTEXT> m_vecLeftContexts;
        vector<wchar_t> m_vecLeftContextsChar;
        vector<ET_PHONEMIC_CONTEXT> m_vecRightContexts;
        vector<wchar_t> m_vecRightContextsChar;
        ET_Subparadigm m_eSubparadigm;
        ET_Gender m_eGender;
        ET_Number m_eNumber;
        ET_Case m_eCase;

        
    };
    
    class CTranscriber
    {
    public:
        enum class EC_CONTEXT_TYPE { LEFT, RIGHT };

    public:
        CTranscriber(const CEString& sDbPath);
        ET_ReturnCode eLoadTranscriptionRules();

    private:
        ET_ReturnCode eFormatInputs(const CEString&);
        ET_ReturnCode eFormatContexts(const CEString& sContext, EC_CONTEXT_TYPE eLeftOrRight);

    private:
        CEString m_sDbPath;
        vector<vector<CEString>> m_vecInputs;

    };

}   // namespace Hlib

#endif      //  C_TRANSCRIBER_H_INCLUDED