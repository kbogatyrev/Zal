#pragma once
#include "Lexeme.h"
#include "WordForm.h"
#include "SqliteWrapper.h"
#include "Endings.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderBase
{
protected:
    CT_Lexeme * pco_Lexeme;
    CT_Sqlite * pco_Db;
    CT_Endings * pco_Endings;

    void v_ReportDbError();

    HRESULT h_GetCommonDeviation (int i_cdNum, St_CommonDeviation& st_data);

    void v_AssignStress (CComObject<CT_WordForm> * pco_wf, vector<int>& vec_iStressPos, bool b_isPrimary);

    bool b_MultiStress (const wstring& str_lemma, vector<int>& vec_iStressPos);

    HRESULT h_CloneWordForm (CComObject<CT_WordForm> * p_source, 
                             CComObject<CT_WordForm> *& p_clone);

private:
    CT_FormBuilderBase()
    {
        pco_Endings = NULL;
    }

public:
    CT_FormBuilderBase (CT_Lexeme * pco_lexeme) : pco_Lexeme (pco_lexeme), pco_Endings (NULL)
    {
        pco_Db = pco_lexeme->pco_Db;
    }

    virtual ~CT_FormBuilderBase()
    {
        //
        // Delete a member that may have been created by a subclass
        //
        if (pco_Endings)
        {
            delete pco_Endings;
            pco_Endings = NULL;
        }
    };

};
