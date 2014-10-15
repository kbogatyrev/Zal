#ifndef C_VERIFIER_H_INCLUDED
#define C_VERIFIER_H_INCLUDED

#include "IVerifier.h"
#include "ILexeme.h"
#include "IWordform.h"
#include "Dictionary.h"     // we need internal methods as well

using namespace std;

namespace Hlib
{
    class CVerifier : public IVerifier
    {
    public:
        CVerifier(CDictionary *);
        ~CVerifier();

    public:
        virtual ET_ReturnCode eSetHeadword(const CEString&);
        virtual CEString eGetHeadword();
        virtual void SetLexemeHash(const CEString&);
        virtual CEString sGetLexemeHash();
        virtual ET_ReturnCode eVerify(const CEString& sLexemeHash);
        virtual ET_TestResult eResult();

        virtual ET_ReturnCode eLoadStoredLexemes();
        virtual ET_ReturnCode eDeleteStoredLexeme(const CEString& sLexeme);

    private:
        CVerifier();        // no use
        ET_ReturnCode eLoadStoredForms();
        ET_ReturnCode eCheckLexeme(ILexeme&);
        bool bWordFormsMatch(IWordForm * pLhs, IWordForm * pRhs);
        ET_ReturnCode eGetStoredLexemeData(const CEString& sSelect);

    private:
        CEString m_sLexemeHash;
        CEString m_sHeadword;
        CDictionary * m_pDictionary;
        CSqlite * m_pDb;
        multimap<int, IWordForm *> m_mmapStoredForms;
        ET_TestResult m_eResult;
    };

}           // namespace

#endif  //  C_VERIFIER_H_INCLUDED