#ifndef C_LEXEME_ENUMERATOR_H_INCLUDED
#define C_LEXEME_ENUMERATOR_H_INCLUDED

#include "IDictionary.h"
#include "LexemeEnumerator.h"

namespace Hlib
{
    class CLexemeEnumerator : public ILexemeEnumerator
    {
    public:
        CLexemeEnumerator() = delete;
        CLexemeEnumerator(CDictionary* dict) : m_pDictionary(dict) {}

        virtual ET_ReturnCode eReset();

        virtual ET_ReturnCode eGetFirstLexeme(ILexeme*& pLexemeItf);
        virtual ET_ReturnCode eGetNextLexeme(ILexeme*& pLexemeItf);

        ET_ReturnCode eGetFirstLexeme(CLexeme*& pLexeme);
        ET_ReturnCode eGetNextLexeme(CLexeme*& pLexeme);

    private:
        vector<CLexeme*>::iterator m_itCurrentLexeme;
        CDictionary* m_pDictionary;
    };

}

#endif  // C_LEXEME_ENUMERATOR_INCLUDED