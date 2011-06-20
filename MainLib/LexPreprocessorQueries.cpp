#include "stdafx.h"
#include "LexPreprocessor.h"
#include "Dictionary.h"
#include "Lexeme.h"

void CT_LexPreprocessor::v_LongStemsBySubtable(int i_subtable,
                                               int i_min_len,
                                               vector<CEString>& vec_stems,
                                               CEString& str_first_lemma)
{
    CEString str_query = L"Select * from stems_for_analysis where endings_link = "
            + str_ToString<int>(i_subtable);
    CEString str_stem;
    str_first_lemma = L"";

    pco_db->PrepareForSelect(str_query);
    while (pco_db->bGetRow())
    {
        pco_db->GetData(2, str_stem);
        if (str_stem.uiLength() > i_min_len)
        {
            vec_stems.push_back(str_stem);
            if (str_first_lemma.uiLength() <= 0)
            {
                pco_db->GetData(5, str_first_lemma);
            }
        }
    }
    pco_db->Finalize();
}

void CT_LexPreprocessor::v_InsertCommonSfx(CEString **parr_str_sfx,
                                           int i_sfx,
                                           int i_subtable,
                                           int i_lexeme_count,
                                           int iCutRight,
                                           CEString sLemmaEnding)
{
    CEString str_sfx = L"";
    for (int i_s = 0; i_s < i_sfx; ++i_s)
    {
        CEString str_cur_sfx = (*parr_str_sfx)[i_s];
//        reverse(str_cur_sfx.begin(), str_cur_sfx.end());
        str_cur_sfx.Reverse();
        str_sfx += str_cur_sfx;
        if (i_s < i_sfx - 1)
        {
            str_sfx += L"|";
        }
    }

    pco_db->PrepareForInsert(L"subtables_for_heuristics", 5);
    pco_db->Bind(1, i_subtable);
    pco_db->Bind(2, i_lexeme_count);
    pco_db->Bind(3, str_sfx);
    pco_db->Bind(4, iCutRight);
    pco_db->Bind(5, sLemmaEnding);
    pco_db->InsertRow();
    pco_db->Finalize();
}

void CT_LexPreprocessor::v_InsertStemsAndLinks(CEString **parr_str_stems,
                                               CEString sLemma,
                                               int *arr_i_subtable_id,
                                               int i_stems,
                                               __int64 llLexemeId)
{
    for (int i_s = 0; i_s < i_stems; i_s++)
    {
        wstring str_stem = (*parr_str_stems)[i_s];
        int i_StressPos = i_DeleteStress(str_stem);
        if (i_StressPos >= str_stem.length())
        {
            // This in fact means that the ending is stressed
            i_StressPos = -1;
        }
        pco_db->v_PrepareForInsert(L"stems_for_analysis", 5);
        pco_db->v_Bind(1, llLexemeId);
        pco_db->v_Bind(2, str_stem);
        pco_db->v_Bind(3, i_StressPos);
        pco_db->v_Bind(4, arr_i_subtable_id[i_s]);
        pco_db->v_Bind(5, sLemma);
        pco_db->v_InsertRow();
        pco_db->v_Finalize();
    }
}