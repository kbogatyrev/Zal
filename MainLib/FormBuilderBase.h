#pragma once
#include "Lexeme.h"
#include "WordForm.h"
#include "SqliteWrapper.h"
#include "Endings.h"

using namespace std;
using namespace std::tr1;

class CFormBuilderBase
{
public:
    CFormBuilderBase (CLexeme * pLexeme, ET_Subparadigm eSubparadigm);

    virtual ~CFormBuilderBase()
    {
        //
        // Delete a member that may have been created by a subclass
        //
        if (m_pEndings)
        {
            delete m_pEndings;
        }
    }

    void SetUsageStatus (ET_Status eStatus)
    {
        m_eStatus = eStatus;
    }

protected:
    void ReportDbError();
    void CloneWordForm (const CWordForm * pSource, CWordForm *& pClone);

protected:
    CLexeme * m_pLexeme;
    ET_Subparadigm m_eSubparadigm;
    CEndings * m_pEndings;
    ET_Status m_eStatus;

private:
    CFormBuilderBase()
    {
        m_pEndings = NULL;
    }

};
