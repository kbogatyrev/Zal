#include "stdafx.h"
#include "Analyzer.h"
#include "Dictionary.h"
#include "Lexeme.h"

using namespace std;
using namespace std::tr1;

HRESULT CT_Analyzer::put_DbPath(BSTR bstr_Path)
{
    USES_CONVERSION;

    if (pco_db)
    {
        delete pco_db;
    }

    str_DbPath = OLE2W(bstr_Path);
    pco_db = new CT_Sqlite(str_DbPath);

    if (pco_db == NULL)
    {
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CT_Analyzer::Analyze(BSTR bstr_Wordform)
{
    USES_CONVERSION;

    if (pco_db == NULL)
    {
        return E_FAIL;
    }
    m_coll.clear();

    vector<CComObject<CT_WordForm>*> vec_possible_wordforms;
    int i_wf_count = i_Analyze(OLE2W(bstr_Wordform), &vec_possible_wordforms);
    if (i_wf_count < 0)
    {
        return E_FAIL;
    }
    if (i_wf_count == 0)
    {
        return S_FALSE;
    }
    // Otherwise, i_wf_count > 0
    // Then, for every wordform found, put the data to the collection
    for (vector<CComObject<CT_WordForm>*>::iterator iter_wf = vec_possible_wordforms.begin();
        iter_wf != vec_possible_wordforms.end();
        iter_wf++)
    {
        m_coll.push_back(*iter_wf);
    }
    return S_OK;
} // Analyze(BSTR bstr_Wordform)

HRESULT CT_Analyzer::PrepareLexeme(__int64 ll_lexeme_id, BOOL b_Stress)
{
    if (ll_lexeme_id < 0)
    {
        return E_FAIL;
    }
    HRESULT h_r = PrepareLexemes(ll_lexeme_id, ll_lexeme_id, b_Stress);
    return h_r;
}

HRESULT CT_Analyzer::PrepareLexemes(__int64 ll_start, __int64 ll_end, BOOL b_Stress)
{
    USES_CONVERSION;
    
    if (ll_start < 0 || ll_end < 0 || pco_db == NULL)
    {
        return E_FAIL;
    }

    // TEST
    b_Stress = TRUE;

    wstring str_Stem;
    int i_MaxStems = 0;
    long l_Items = 0, l_Id = 0, l_Wordforms = 0;
    __int64 ll_lexeme_id;
    HRESULT h_r;
    wsmatch result;
    vector<CComObject<CT_WordForm>*> *pvec_Wordforms;
    wstring *arr_str_Paradigm, **parr_str_Stems, str_ending;
    parr_str_Stems = new wstring*;
    *parr_str_Stems = new wstring[1];
    bool b_match;

    // Create and initialize an IDictionary object to work with
    pvec_Wordforms = new vector<CComObject<CT_WordForm>*>;
    CComPtr<IDictionary> sp_dict = NULL;
    CComBSTR bstr_DbPath(str_DbPath.c_str());
    h_r = sp_dict.CoCreateInstance(__uuidof(ZalDictionary));
    if (h_r != S_OK)
    {
        return h_r;
    }
    sp_dict->put_DbPath(bstr_DbPath);

    pco_db->v_BeginTransaction();
    
    for (ll_lexeme_id = ll_start; 
        ll_lexeme_id <= ll_end; 
        ll_lexeme_id++) // for every lexeme within the range
    {
        int *arr_i_subtable_id, *arr_i_Hash;
        int i_wf = 0;
        CComPtr<ILexeme> sp_lex;
        pvec_Wordforms->clear();

        // Find lexeme in the db; generate the paradigm
        // (i_th element of arr_str_Paradigm has form arr_l_Hash[i])
        try
        {
            h_r = sp_dict->GetLexeme((LONG)ll_lexeme_id, &sp_lex);
            if (h_r != S_OK)
            {
                return h_r;
            }
            CComBSTR bstr_Lemma;
            sp_lex->get_InitialForm(&bstr_Lemma);
            ET_PartOfSpeech eo_pos = POS_UNDEFINED;
            h_r = sp_lex->get_PartOfSpeech(&eo_pos);
            // --------- Test ----------
            //if (eo_pos != POS_NOUN && eo_pos != POS_ADJ && eo_pos != POS_VERB)
            if (eo_pos != POS_NOUN)
            {
                continue;
            }
            // --------- /Test ----------
            if (eo_pos == POS_VERB)
            {
                i_MaxStems = 5;
            }
            else
            {
                i_MaxStems = 3;
            }
            h_r = sp_lex->GenerateWordForms();
            if (h_r != S_OK)
            {
                return h_r;
            }
            h_r = sp_lex->get_Count(&l_Wordforms);
            for (long l_Item = 1; l_Item <= l_Wordforms; l_Item++)
            {
                CComObject<CT_WordForm> *co_tmp_wf;
                h_r = CComObject<CT_WordForm>::CreateInstance(&co_tmp_wf);
                if (h_r != S_OK)
                {
                    return h_r;
                }
                CComVariant sp_disp = NULL;
                h_r = sp_lex->get_Item(l_Item, &sp_disp);
                if (h_r != S_OK)
                {
                    return h_r;
                }
                CComQIPtr<IWordForm> sp_wf = sp_disp.pdispVal;
                CComBSTR bstr_WordForm;
                sp_wf->get_Wordform(&bstr_WordForm);
                if (bstr_WordForm.Length() <= 0)
                {
                    continue; // We don't want to count empty strings as wordforms
                }
                co_tmp_wf->str_Lemma = OLE2W(bstr_Lemma);
                co_tmp_wf->str_WordForm = OLE2W(bstr_WordForm);

                sp_wf->get_PartOfSpeech(&(co_tmp_wf->eo_POS));
                sp_wf->get_Subparadigm(&(co_tmp_wf->eo_Subparadigm));
                sp_wf->get_Animacy(&(co_tmp_wf->eo_Animacy));
                sp_wf->get_Aspect(&(co_tmp_wf->eo_Aspect));
                sp_wf->get_Case(&(co_tmp_wf->eo_Case));
                sp_wf->get_Gender(&(co_tmp_wf->eo_Gender));
                sp_wf->get_IsReflexive(&(co_tmp_wf->eo_Reflexive));
                long l_LexemeId = 0, l_Stress = 0;
                sp_wf->get_LexemeId(&l_LexemeId);
                co_tmp_wf->ll_Lexeme_id = (__int64)l_LexemeId;
                sp_wf->get_Number(&(co_tmp_wf->eo_Number));
                sp_wf->get_Person(&(co_tmp_wf->eo_Person));
                if (b_Stress)
                    // TODO: What if there are several stresses?
                {
                    sp_wf->get_Stress(&l_Stress);
                    co_tmp_wf->map_Stress[l_Stress] = true;
                }

                pvec_Wordforms->push_back(co_tmp_wf);
                //co_tmp_wf->v_Init();
            }
        }
        catch (...)
        {
            return E_FAIL;
        }
        
        if (l_Wordforms <= 0 || pvec_Wordforms == NULL)
        {
            //return E_FAIL;
            continue;
        }
        
        arr_str_Paradigm = new wstring[pvec_Wordforms->size()];
        arr_i_Hash = new int[pvec_Wordforms->size()];
        for (vector<CComObject<CT_WordForm>*>::iterator iter_wf = pvec_Wordforms->begin();
            iter_wf != pvec_Wordforms->end();
            iter_wf++, i_wf++)
        {
            arr_i_Hash[i_wf] = (*iter_wf)->i_GramHash();

            bool b_StressFound = false;
            if (b_Stress)
            {
                // Find a stress
                // (works if there is only one main stress.)
                for (map<int, bool>::iterator iter_stress = (*iter_wf)->map_Stress.begin();
                    iter_stress != (*iter_wf)->map_Stress.end();
                    ++iter_stress)
                {
                    if ((*iter_stress).second == true)
                    {
                        arr_str_Paradigm[i_wf] = str_InsertStress((*iter_stress).first, (*iter_wf)->str_WordForm);
                        b_StressFound = true;
                        break;
                    }
                }
            }
            // Fill in arr_str_Paradigm (either with a stress, or without it)
            if (b_StressFound == false)
            {
                arr_str_Paradigm[i_wf] = (*iter_wf)->str_WordForm;
            }
        }
        
        // Create several gr_stems
        int i_Stems = i_LCP(arr_str_Paradigm, parr_str_Stems, pvec_Wordforms->size(), i_MaxStems);
        
        // Split every wordform into stem and ending,
        // then for every gr_stem create a table of endings usable with it
        CT_EndingsTable *arr_co_endings;
        arr_co_endings = new CT_EndingsTable[i_Stems];
        arr_i_subtable_id = new int[i_Stems];
        for (i_wf = 0; i_wf < l_Wordforms; i_wf++)
        {
            for (int i_s = 0; i_s < i_Stems; i_s++)
            {
                b_match = regex_match(arr_str_Paradigm[i_wf],
                    result, (const wregex)((*parr_str_Stems)[i_s] + L"(.*)"));
                if (b_match == true)
                {
                    str_ending = (wstring)result[1];
                    // TODO: Several stress positions
                    int i_StressPos = i_DeleteStress(str_ending);
                    if ((*parr_str_Stems)[i_s].substr((*parr_str_Stems)[i_s].length() - 1, 1) == L"<")
                    {
                        i_StressPos = 0;
                    }
                    // Write the ending and its grammatical meaning 
                    // to the temporary "endings table" no. i_s
                    arr_co_endings[i_s].i_AddEnding(arr_i_Hash[i_wf],
                        str_ending, i_StressPos);
                    break;
                }
            }
        }
        // Write the "endings tables" to the db if necessary;
        // get their subtable_id's
        for (int i_s = 0; i_s < i_Stems; i_s++)
        {
            arr_i_subtable_id[i_s] = arr_co_endings[i_s].i_db_Write(pco_db);
        }
        // Add the stems with links to the corresponding subtables to the db
        for (int i_s = 0; i_s < i_Stems; i_s++)
        {
            wstring str_Stem = (*parr_str_Stems)[i_s];
            int i_StressPos = i_DeleteStress(str_Stem);
            if ((unsigned int)i_StressPos >= str_Stem.length())
            {
                // This in fact means that the ending is stressed
                i_StressPos = -1;
            }
            pco_db->v_PrepareForInsert(L"stems_for_analysis", 4);
            pco_db->v_Bind(1, ll_lexeme_id);
            pco_db->v_Bind(2, str_Stem);
            pco_db->v_Bind(3, i_StressPos);
            pco_db->v_Bind(4, arr_i_subtable_id[i_s]);
            pco_db->v_InsertRow();
            pco_db->v_Finalize();
        }
    } // for (ll_lexeme_id = ll_start; ll_lexeme_id <= ll_end; ll_lexeme_id++)...
    pco_db->v_CommitTransaction();

    CoUninitialize();
    delete pvec_Wordforms;
    delete[] parr_str_Stems;
    return S_OK;
} // PrepareLexemes(__int64 ll_start, __int64 ll_end)

int CT_Analyzer::i_Analyze(wstring str_wordform, vector<CComObject<CT_WordForm>*>* pvec_possible_wordforms)
{
    // Be careful: it changes the input
    if (str_wordform.length() <= 0 || pco_db == NULL || pvec_possible_wordforms == NULL)
    {
        return -1;
    }
    wstring str_left, str_right;
    vector<int>* pvec_stems_id;
    pvec_possible_wordforms->clear();
    pvec_stems_id = new vector<int>;
    for (int i_left = str_wordform.length(); i_left >= 0; i_left--)
    {
        str_left = str_wordform.substr(0, i_left);
        str_right = str_wordform.substr(i_left, str_wordform.length() - i_left);
        pvec_stems_id->clear();
        i_LookUpStems(pvec_stems_id, str_left);
        if (pvec_stems_id->empty())
        {
            continue;
        }
        i_CheckEndings(pvec_possible_wordforms, pvec_stems_id, str_left, str_right);
    }
    return pvec_possible_wordforms->size();
}

int CT_Analyzer::i_LookUpStems(vector<int>* pvec_stems_id, wstring str_left)
{
    // Search the DB for graphic stems equal to str_left;
    // write their IDs to vec_stems_id
    if (pco_db == NULL || pvec_stems_id == NULL)
    {
        return -1;
    }
    wstring str_query = L"Select * from stems_for_analysis where stem = \"" + str_left + L"\"";
    int i_stem_id;
    pco_db->v_PrepareForSelect(str_query);
    while (pco_db->b_GetRow())
    {
        pco_db->v_GetData(0, i_stem_id);
        pvec_stems_id->push_back(i_stem_id);
    }
    return pvec_stems_id->size();
}

int CT_Analyzer::i_CheckEndings(vector<CComObject<CT_WordForm>*>* pvec_possible_wordforms, vector<int>* pvec_stems_id, wstring str_left, wstring str_right)
{
    if (pco_db == NULL || pvec_stems_id == NULL)
    {
        return -1;
    }
    static vector<int> vec_gram;
    wstring str_query, str_lemma;
    vector<int> vec_subtable_id;
    __int64 ll_lexeme_id;
    vector<__int64> vec_lexeme_id;
    vector<wstring> vec_lemma;
    int i_subtable_id;

    for (vector<int>::iterator iter_stems = pvec_stems_id->begin();
        iter_stems != pvec_stems_id->end(); iter_stems++)
    {
        // For each *iter_stems look up the endings table ID in DB, then in this table try to find
        // endings which are equal to str_right. For each ending found, write the parameters
        // to co_tmp_wf and then push_back co_tmp_wf to pvec_possible_wordforms:
        vec_gram.clear();
        str_query = L"Select * from stems_for_analysis where id = " + str_ToString<int>(*iter_stems);
        pco_db->v_PrepareForSelect(str_query);
        while (pco_db->b_GetRow())
        {
            pco_db->v_GetData(4, i_subtable_id);
            pco_db->v_GetData(1, ll_lexeme_id);
            vec_subtable_id.push_back(i_subtable_id);
            vec_lexeme_id.push_back(ll_lexeme_id);
        }
        if (vec_subtable_id.empty() || vec_lexeme_id.empty())
        {
            return -2;
        }
        str_query = L"Select * from headword as a0 where exists (select * from descriptor as a1 where id = " + str_ToString<__int64>(*(vec_lexeme_id.begin())) + L" and a0.id = a1.word_id)";
        pco_db->v_PrepareForSelect(str_query);
        while (pco_db->b_GetRow())
        {
            pco_db->v_GetData(1, str_lemma);
            vec_lemma.push_back(str_lemma);
        }
        str_query = L"Select * from endings_for_analysis where subtable_id = " + str_ToString<int>(*(vec_subtable_id.begin())) 
            + L" and ending = \"" + str_right + L"\"";
        pco_db->v_PrepareForSelect(str_query);
        while (pco_db->b_GetRow())
        {
            int i_hash;
            pco_db->v_GetData(3, i_hash);
            vec_gram.push_back(i_hash);
        }
        if (vec_lemma.empty() || vec_gram.empty())
        {
            return -2;
        }
        for (vector<int>::iterator iter_endings = vec_gram.begin();
            iter_endings != vec_gram.end(); iter_endings++)
        {
            CComObject<CT_WordForm>* co_tmp_wf;
            HRESULT h_r = CComObject<CT_WordForm>::CreateInstance(&co_tmp_wf);
            if (h_r != S_OK)
            {
                return -1;
            }
            co_tmp_wf->h_DecodeHash(*iter_endings);
            co_tmp_wf->ll_Lexeme_id = *(vec_lexeme_id.begin());
            co_tmp_wf->str_Lemma = *(vec_lemma.begin());
            co_tmp_wf->str_WordForm = str_left + str_right;
            pvec_possible_wordforms->push_back(co_tmp_wf);
        }
        vec_subtable_id.clear();
        vec_lexeme_id.clear();
        vec_lemma.clear(); // these 3 vectors are different for every stem found
    }
    vec_gram.clear();
    return 0;
}

wstring CT_Analyzer::str_InsertStress(int i_letter, wstring str_)
{
    if (i_letter >= 0 && i_letter < (int)(str_.length()))
    {
        return str_.substr(0, i_letter) + L"<" + str_.substr(i_letter, str_.length() - i_letter);
    }
    return str_;
}

int CT_Analyzer::i_DeleteStress(wstring& str_)
// Deletes the stress and returns the number of the letter it follows
{
    int i_StressPos = str_.find(L"<");
    if ((unsigned int)i_StressPos >= str_.length())
    {
        return -1;
    }
    str_ = str_.substr(0, i_StressPos) 
        + str_.substr(i_StressPos + 1, str_.length() - i_StressPos - 1);
    return i_StressPos;
}

int CT_Analyzer::i_LCP(wstring* str_words, wstring** pstr_pfx, int i_words, int i_pfx)
{
    // Divides the input into up to i_pfx groups and finds their longest common prefixes.
    // They may differ in length, but don't exceed the length of the shortest word.
    if (str_words == NULL || i_words <= 0 || i_pfx < 0)
    {
        return -1;
    }

    bool b_class_changed = false, b_try_again = false, *b_disabled;
    int i_len, i_minlen = 65536, i_numpfx = 1, i_numpfx_prev = 1, i_w_finished = 0, *i_pfxclass, *i_based_on;
    wstring *str_curpfx, *str_cursymb;
    wstring str_s;
    str_curpfx = new wstring[i_pfx];
    str_cursymb = new wstring[i_pfx];
    str_curpfx[0] = str_cursymb[0] = L"";
    i_pfxclass = new int[i_words]; // Stores current prefix class number of the i_th string or -1 if corresponding prefix is finished
    i_based_on = new int[i_words]; // Temporarily stores the number of class i_th class is based on
    b_disabled = new bool[i_words];
    memset(i_pfxclass, 0, i_words * sizeof(int)); // Initially, there is only one prefix class
    memset(i_based_on, 0, i_words * sizeof(int));
    memset(b_disabled, 0, i_words * sizeof(bool)); // All the words are enabled

    // Find minimal word length
    for (int i_ = 0; i_ < i_words; i_++)
    {
        i_len = str_words[i_].length();
        if (i_minlen > i_len)
        {
            i_minlen = i_len;
        }
    }

    // Find the prefixes
    for (int i_l = 0; i_l < i_minlen; i_l++)
    {
        for (int i_w = 0; i_w < i_words; i_w++)
        {
            if (b_disabled[i_w] == true)
            {
                continue; // The word is disabled
            }
            str_s = str_words[i_w].substr(i_l, 1); //current symbol of the current word
            if (str_cursymb[i_pfxclass[i_w]].length() == 0)
            {
                str_cursymb[i_pfxclass[i_w]] = str_s;
            }
            else
            {
                if (str_s != str_cursymb[i_pfxclass[i_w]])
                {
                    // Maybe we can just change its class to one of those recently introduced?
                    for (int i_ = i_pfxclass[i_w] + 1; i_ < i_numpfx; i_++)
                    {
                        if (str_cursymb[i_] == str_s && i_based_on[i_] == i_pfxclass[i_w])
                        {
                            i_pfxclass[i_w] = i_;
                            b_class_changed = true;
                            break;
                        }
                    }

                    if (b_class_changed == true)
                    {
                        b_class_changed = false;
                        continue;
                    }
                    //if not:
                    if (++i_numpfx > i_pfx)
                    {
                        //Finalize this prefix and initialize variables
                        for (int i_ = 0; i_ < i_words; i_++)
                        {
                            if (i_based_on[i_] == i_pfxclass[i_w])
                            {
                                b_disabled[i_] = true;
                                i_w_finished++; //One more word finished
                            }
                            i_pfxclass[i_] = i_based_on[i_];
                        }
                        for (int i_ = 0; i_ < i_pfx - 1; i_++)
                        {
                            str_cursymb[i_] = L"";
                        }
                        i_numpfx = i_numpfx_prev;
                        i_l--;
                        b_try_again = true; // Try again with the same symbol position, but more words disabled
                        break;
                    }
                    else
                    {
                        // Introduce a new prefix class
                        str_curpfx[i_numpfx - 1] = str_curpfx[i_pfxclass[i_w]];
                        str_cursymb[i_numpfx - 1] = str_s;
                        i_based_on[i_w] = i_pfxclass[i_w];
                        i_pfxclass[i_w] = i_numpfx - 1;
                    }
                }
            }
        }
        // Maybe we need to try the same position in the word again
        if (b_try_again == true)
        {
            b_try_again = false;
            if (i_w_finished == i_words)
            {
                break;
            }
            else
            {
                continue;
            }
        }
        // If not, add recently obtained symbols to the prefixes
        for (int i_ = 0; i_ < i_numpfx; i_++)
        {
            str_curpfx[i_] += str_cursymb[i_];
            str_cursymb[i_] = L"";
        }
        // Let recently formed classes become independent
        memcpy(i_based_on, i_pfxclass, i_words * sizeof(int));
        i_numpfx_prev = i_numpfx;
    }

    delete[] *pstr_pfx;
    *pstr_pfx = new wstring[i_numpfx_prev];
    for (int i_ = 0; i_ < i_numpfx_prev; i_++)
    {
        (*pstr_pfx)[i_] = str_curpfx[i_];
    }
    delete[] str_curpfx;
    delete[] str_cursymb;
    delete[] i_pfxclass;
    return i_numpfx_prev;
}
