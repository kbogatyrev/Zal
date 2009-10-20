#include "Analyzer_Helpers.h"

using namespace std;
using namespace std::tr1;

class CT_Analyzer
{
public:
    int i_Analyze(CT_Sqlite* pco_dbHandle, wstring str_wordform);
    int i_LookUpStems(CT_Sqlite* pco_dbHandle, vector<int>* pvec_stems_id, wstring str_left);
    int i_CheckEndings(CT_Sqlite* pco_dbHandle, vector<CT_GramHelper>* pvec_possible_wordforms, vector<int>* pvec_stems_id, wstring str_left, wstring str_right);
    int i_WriteStemsAndEndings(CT_Sqlite* pco_dbHandle, __int64 ll_lexeme_id, int last_subtable_id);

private:
    int i_LCP(wstring* str_words, wstring* str_pfx, int i_words, int i_pfx);
    /*
    void v_LearnSubtableID_Endings(sqlite3_stmt *po_statement, void *v_subtable_id);
    void v_LearnSubtableID_Stems(sqlite3_stmt *po_statement, void *v_subtable_id);
    void v_LearnGram_Endings(sqlite3_stmt *po_statement, void *v_gram);   
    void v_LearnStemsId(sqlite3_stmt *po_statement, void *v_stems_id);
    */
};