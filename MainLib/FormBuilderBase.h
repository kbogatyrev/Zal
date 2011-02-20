#pragma once
#include "Lexeme.h"
#include "WordForm.h"
#include "SqliteWrapper.h"
#include "Endings.h"

using namespace std;
using namespace std::tr1;

class CT_FormBuilderBase
{
public:
    CT_FormBuilderBase (CT_Lexeme * pco_lexeme, ET_Subparadigm eo_subparadigm) : 
       pco_Lexeme (pco_lexeme), 
       eo_Subparadigm (eo_subparadigm), 
       pco_Endings (NULL), 
       eo_Status (STATUS_COMMON)
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
    }

    void v_SetUsageStatus (ET_Status eo_status)
    {
        eo_Status = eo_status;
    }

protected:
    void v_ReportDbError();

    void v_AssignSecondaryStress (CComObject<CT_WordForm> * pco_wf);

    void v_AssignSecondaryStress (ST_IrregularForm& st_if);

    bool b_MultiStress (const wstring& str_lemma, vector<int>& vec_iStressPos);

    HRESULT h_CloneWordForm (const CComObject<CT_WordForm> * p_source, 
                             CComObject<CT_WordForm> *& p_clone);

    HRESULT h_CloneWordForm (const  CT_WordForm * co_source, 
                             CComObject<CT_WordForm> *& p_clone);

    HRESULT h_WordFormFromHash (int i_hash, int i_at, CT_WordForm *& pco_wf);

    HRESULT h_HandleIrregularForms (CComObject<CT_WordForm> * pco_wordForm, bool& b_isVariant);

    int i_NForms (int i_hash);

    bool b_HasCommonDeviation (int i_cd);

    bool b_DeviationOptional (int i_cd);

    HRESULT h_RetractStressToPreverb (CT_WordForm *, bool b_isOptional);

protected:
    CT_Lexeme * pco_Lexeme;
    ET_Subparadigm eo_Subparadigm;
    CT_Sqlite * pco_Db;
    CT_Endings * pco_Endings;
    ET_Status eo_Status;

private:
    CT_FormBuilderBase()
    {
        pco_Endings = NULL;
    }

};
