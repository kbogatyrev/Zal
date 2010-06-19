#include "stdafx.h"
#include "Analyzer_Helpers.h"

CT_EndingsTable::CT_EndingsTable()
{
    pvec_Endings = new vector<wstring>;
    pvec_Gram = new vector<int>;
    pvec_Stress = new vector<int>;
}

CT_EndingsTable::~CT_EndingsTable()
{
    Clear();
    delete pvec_Endings;
    delete pvec_Gram;
    delete pvec_Stress;
}

void CT_EndingsTable::Clear()
{
    pvec_Endings->clear();
    pvec_Gram->clear();
    str_Stem = L"";
}

int CT_EndingsTable::i_AddEnding(int i_Hash, wstring str_Ending, int i_StressPos)
{
    pvec_Endings->push_back(str_Ending);
    pvec_Gram->push_back(i_Hash);
    pvec_Stress->push_back(i_StressPos);
    return 0;
}

int CT_EndingsTable::i_AddEnding(long l_Hash, wstring str_Ending, int i_StressPos)
{
    // TODO: replace int with long in the whole class
    pvec_Endings->push_back(str_Ending);
    pvec_Gram->push_back((int)l_Hash);
    pvec_Stress->push_back(i_StressPos);
    return 0;
}

vector<int> CT_EndingsTable::vec_db_id(CT_Sqlite* pco_dbHandle)
// Tries to find the table in the db, returns -1 if there's no such table.
// If everything goes well, the output vector will contain just 1 element. But who knows what can happen.
{
    vector<int> vec_subtable_id, vec_entries_count, vec_result;
    if (pco_dbHandle == NULL || pvec_Endings->empty() == true
        || pvec_Gram->empty() == true || pvec_Endings->size() != pvec_Gram->size())
    {
        return vec_result;
    }
    wstring str_query, str_count, str_gram, str_stress;
    int i_entries_count, i_subtable_id;

    vector<wstring>::iterator iter_endings = pvec_Endings->begin();
    vector<int>::iterator iter_gram = pvec_Gram->begin();
    vector<int>::iterator iter_stress = pvec_Stress->begin();
    str_gram = str_ToString<int>(*iter_gram);
    str_stress = str_ToString<int>(*iter_stress);
    str_query += L"Select * from endings_for_analysis as a0 where ending = \"" 
        + *iter_endings + L"\" and gram_hash = " + str_gram 
        + L" and stress_pos = " + str_stress;
    ++iter_gram;
    ++iter_endings;
    ++iter_stress;
    if (pvec_Endings->size() > 1)
    {
        for (int i_ = 1;
            iter_endings != pvec_Endings->end();
            ++iter_endings, ++iter_gram, ++iter_stress, ++i_)
        {
            str_count = str_ToString<int>(i_);
            str_gram = str_ToString<int>(*iter_gram);
            str_stress = str_ToString<int>(*iter_stress);
            str_query += L" and exists (select * from endings_for_analysis as a" 
                + str_count + L" where ending = \"" + *iter_endings 
                + L"\" and gram_hash = " + str_gram 
                + L" and stress_pos = " + str_stress
                + L" and a" + str_count + L".subtable_id = a0.subtable_id)";
        }
    }
    pco_dbHandle->v_PrepareForSelect(str_query);
    while (pco_dbHandle->b_GetRow())
    {
        pco_dbHandle->v_GetData(1, i_subtable_id);
        vec_subtable_id.push_back(i_subtable_id);
    }
    pco_dbHandle->v_Finalize();

    // Now that we've learned the ids of possible subtables we must ensure they 
    // don't contain any entries except those specified by the input vectors.
    for (vector<int>::iterator iter_subtable_id = vec_subtable_id.begin();
        iter_subtable_id != vec_subtable_id.end();
        ++iter_subtable_id)
    {
        str_query = L"Select entries_count from endings_meta where subtable_id = " 
            + str_ToString<int>(*iter_subtable_id);
        pco_dbHandle->v_PrepareForSelect(str_query);
        while (pco_dbHandle->b_GetRow())
        {
            pco_dbHandle->v_GetData(0, i_entries_count);
            vec_entries_count.push_back(i_entries_count);
        }
        pco_dbHandle->v_Finalize();
        if (vec_entries_count.empty() == false
            && *(vec_entries_count.begin()) == pvec_Gram->size())
        {
            vec_result.push_back(*iter_subtable_id);
            break;
        }
        vec_entries_count.clear();
    }
    return vec_result;
}

int CT_EndingsTable::i_db_Write(CT_Sqlite* pco_dbHandle)
// If a table already exists, return its subtable_id.
// If not, write the whole table to the db, then return its subtable_id.
// Return -1 or -2 on error.
{
    if (pco_dbHandle == NULL)
    {
        return -1;
    }
    int i_inserted = 0, i_subtable_id, i_search_result;
    vector<int> vec_search_result;
    vec_search_result = vec_db_id(pco_dbHandle);
    if (!vec_search_result.empty())
    {
        return *(vec_search_result.begin());
        // TODO: What if it contains more than one element?
    }

    // If we've reached this mark, it means that there's no such table in the db yet.
    // We should get the number of the last table in the db and save our table there.
    vector<wstring>::iterator iter_Ending;
    vector<int>::iterator iter_Gram;
    vector<int>::iterator iter_Stress;
    wstring str_query = L"Select * from endings_meta as a0 where not exists (select * from endings_meta as a1 where a1.id > a0.id)";
    pco_dbHandle->v_PrepareForSelect(str_query);
    while (pco_dbHandle->b_GetRow())
    {
        pco_dbHandle->v_GetData(0, i_search_result);
        vec_search_result.push_back(i_search_result);
    }
    pco_dbHandle->v_Finalize();
    i_subtable_id = *(vec_search_result.begin()) + 1;

    for (iter_Ending = pvec_Endings->begin(),
            iter_Gram = pvec_Gram->begin(),
            iter_Stress = pvec_Stress->begin();
        (iter_Ending != pvec_Endings->end()) &&
            (iter_Gram != pvec_Gram->end()) &&
            (iter_Stress != pvec_Stress->end());
        ++iter_Ending, ++iter_Gram, ++iter_Stress, ++i_inserted)
    {
        // For each tuple <ending, grammatical parameters, stress position>,
        // insert it into the endings table.
        pco_dbHandle->v_PrepareForInsert(L"endings_for_analysis", 4);
        pco_dbHandle->v_Bind(1, i_subtable_id);  // 0-based
        pco_dbHandle->v_Bind(2, *iter_Ending);
        pco_dbHandle->v_Bind(3, *iter_Gram);
        pco_dbHandle->v_Bind(4, *iter_Stress);
        pco_dbHandle->v_InsertRow();
        pco_dbHandle->v_Finalize();
    }
    // Now, write auxiliary information to endings_meta
    pco_dbHandle->v_PrepareForInsert(L"endings_meta", 2);
    pco_dbHandle->v_Bind(1, i_subtable_id);
    pco_dbHandle->v_Bind(2, i_inserted);
    pco_dbHandle->v_InsertRow();
    pco_dbHandle->v_Finalize();
    return i_subtable_id;
}


template <typename WF>
CT_Wordform_Iterator<WF>::CT_Wordform_Iterator()
{
}

template <typename WF>
CT_Wordform_Iterator<WF>::~CT_Wordform_Iterator()
{
}

template <typename WF>
int CT_Wordform_Iterator<WF>::i_Initialize(ET_PartOfSpeech eo_in_POS)
{
    eo_POS = eo_in_POS;
    b_end = false;
    co_wf.eo_POS = eo_POS;
    co_wf.eo_Case = CASE_UNDEFINED;
    co_wf.eo_Number = NUM_UNDEFINED;
    co_wf.eo_AdjForm = ADJ_FORM_UNDEFINED;
    co_wf.eo_Gender = GENDER_UNDEFINED;
    co_wf.eo_Tense = TENSE_UNDEFINED;
    co_wf.eo_Person = PERSON_UNDEFINED;
    co_wf.eo_Reflexive = REFL_UNDEFINED;
    co_wf.eo_Voice = VOICE_UNDEFINED;
    co_wf.eo_Animacy = ANIM_UNDEFINED;
    //co_wf.eo_Special = SPECIAL_UNDEFINED;  

    if (eo_POS == POS_NOUN)
    {
        co_wf.eo_Case++;
        co_wf.eo_Number++;
    }
    if (eo_POS == POS_ADJ)
    {
        co_wf.eo_AdjForm = ADJ_FORM_LONG;
        co_wf.eo_Case++;
        co_wf.eo_Number++;
    }
    if (eo_POS == POS_VERB)
    {
        co_wf.eo_Special = SPECIAL_NORMAL;
        co_wf.eo_Tense = TENSE_PRESENT;
        co_wf.eo_Person++;
        co_wf.eo_Number++;
    }
    if (eo_POS == POS_ADV ||
        eo_POS == POS_PREP ||
        eo_POS == POS_CONJ || 
        eo_POS == POS_PARTICLE ||
        eo_POS == POS_INTERJ || 
        eo_POS == POS_PARENTH)
    {
        b_end = true;
    }
    return 0;
}


template <typename WF>
WF CT_Wordform_Iterator<WF>::get_Wordform()
{
    return co_wf;
}

template <typename WF>
CT_Wordform_Iterator<WF> CT_Wordform_Iterator<WF>::operator++()
{
    if (eo_POS == POS_NOUN)
    {
        co_wf.eo_Case++;
        if (co_wf.eo_Case == CASE_COUNT)
        {
            co_wf.eo_Number++;
            if (co_wf.eo_Number == NUM_COUNT)
            {
                b_end = true;
                return this;
            }
            co_wf.eo_Case = CASE_UNDEFINED++;
        }
    }

    if (eo_POS == POS_ADJ) // Including short forms with cases
    {
        if (co_wf.eo_Case == CASE_ACC && co_wf.eo_Animacy == ANIM_NO)
        {
            co_wf.eo_Animacy = ANIM_UNDEFINED;
        }
        co_wf.eo_Case++;
        if (co_wf.eo_Case == CASE_COUNT)
        {
            if (co_wf.eo_Number == NUM_SG)
            {
                co_wf.eo_Gender++;
                if (co_wf.eo_Gender == GENDER_COUNT)
                {
                    co_wf.eo_Gender = GENDER_UNDEFINED;
                    co_wf.eo_Number = NUM_PL;
                }
            }
            else
            {
                co_wf.AdjForm++;
                if (co_wf.AdjForm == ADJ_FORM_COUNT)
                {
                    b_end = true;
                    return this;
                }
                co_wf.eo_Number = NUM_SG;
                co_wf.eo_Gender = GENDER_UNDEFINED++;
            }
            co_wf.eo_Case = CASE_UNDEFINED++;
        }

        // TODO: Verbs and so on
    }
    return this;
}
