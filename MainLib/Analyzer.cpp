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

    bstr_DbPath = bstr_Path;
    pco_db = new CT_Sqlite(OLE2W(bstr_Path));

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
    vector<CT_GramHelper> vec_possible_wordforms;
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
    // Then, for every wordform found, put the data to a CT_WordForm instance and copy it to the collection
    for (vector<CT_GramHelper>::iterator iter_wf = vec_possible_wordforms.begin();
        iter_wf != vec_possible_wordforms.end();
        iter_wf++)
    {
        CComObject<CT_WordForm> *sp_wf;
        HRESULT h_r = CComObject<CT_WordForm>::CreateInstance(&sp_wf);
        if (h_r != S_OK)
        {
            return h_r;
        }

        sp_wf->eo_AdjForm = (*iter_wf).eo_AdjForm;
        sp_wf->eo_Animacy = (*iter_wf).eo_Animacy;
        sp_wf->eo_Case = (*iter_wf).eo_Case;
        sp_wf->eo_Gender = (*iter_wf).eo_Gender;
        sp_wf->eo_Number = (*iter_wf).eo_Number;
        sp_wf->eo_Person = (*iter_wf).eo_Person;
        sp_wf->eo_POS = (*iter_wf).eo_POS;
        sp_wf->eo_Reflexive = (*iter_wf).eo_Reflexive;
//        sp_wf->eo_Special = (*iter_wf).eo_Special;
        sp_wf->eo_Tense = (*iter_wf).eo_Tense;
        sp_wf->eo_Voice = (*iter_wf).eo_Voice;
        sp_wf->str_Lemma = (*iter_wf).str_lemma;
        sp_wf->str_WordForm = (*iter_wf).str_wordform;
        sp_wf->ll_Lexeme_id = (*iter_wf).ll_lexeme_id;
        m_coll.push_back(sp_wf);
    }
    return S_OK;
}

HRESULT CT_Analyzer::PrepareLexeme(__int64 ll_lexeme_id)
{
    USES_CONVERSION;
    
    if (ll_lexeme_id < 0 || pco_db == NULL)
    {
        return -1;
    }

    wstring str_Stem;
    long l_Items = 0, l_Id = 0, l_Wordforms = 0;
    HRESULT h_r;
    CT_GramHelper co_tmp_wf;

    //wstring str_Stem = str_GrStem(ll_lexeme_id);
    //if (str_Stem == L"")
    //{
    wsmatch result;
    vector<CT_GramHelper> *pvec_Wordforms;
    wstring *pstr_Paradigm, *pstr_Stems, str_ending;
    int i_ = 0;
    int *pi_Gram, *pi_subtable_id;
    bool b_match;

    // Generate paradigm (i_th element of str_Paradigm has form i_Gram[i])
    pvec_Wordforms = new vector<CT_GramHelper>;
    /*
    //Commented to suppress a compilation-time error
    int i_Wordforms = i_GenerateParadigm(pco_dbHandle, ll_lexeme_id, pvec_Wordforms);
    */
    CComPtr<IDictionary> sp_dict = NULL;
    CComPtr<ILexeme> sp_lex;
    h_r = sp_dict.CoCreateInstance(__uuidof(ZalDictionary));
    if (h_r != S_OK)
    {
        return h_r;
    }
    sp_dict->put_DbPath(bstr_DbPath);
    h_r = sp_dict->GetLexeme((LONG)ll_lexeme_id, &sp_lex);
    if (h_r != S_OK)
    {
        return h_r;
    }
    CComBSTR sp_lemma;
    h_r = sp_lex->get_InitialForm(&sp_lemma);
    if (h_r != S_OK)
    {
        return h_r;
    }
    h_r = sp_lex->GenerateWordForms();
    if (h_r != S_OK)
    {
        return h_r;
    }
    h_r = sp_lex->get_Count(&l_Wordforms);
    co_tmp_wf.ll_lexeme_id = ll_lexeme_id;
    co_tmp_wf.str_lemma = OLE2W(sp_lemma);
    for (long l_Item = 1; l_Item <= l_Wordforms; l_Item++)
    {
        co_tmp_wf.GramClear();
        CComVariant sp_disp = NULL;
        h_r = sp_lex->get_Item(l_Item, &sp_disp);
        if (h_r != S_OK)
        {
            return h_r;
        }
        CComQIPtr<IWordForm> sp_wf = sp_disp.pdispVal;
        CComBSTR sp_bstrWf;
        h_r = sp_wf->get_Wordform (&sp_bstrWf);
        ET_PartOfSpeech eo_pos = POS_UNDEFINED;
        h_r = sp_wf->get_PartOfSpeech (&eo_pos);
        ET_AdjForm eo_adj_form = ADJ_FORM_UNDEFINED;
        h_r = sp_wf->get_AdjForm (&eo_adj_form);
        ET_Animacy eo_animacy = ANIM_UNDEFINED;
        h_r = sp_wf->get_Animacy (&eo_animacy);
        ET_Case eo_case = CASE_UNDEFINED;
        h_r = sp_wf->get_Case (&eo_case);
        ET_Number eo_num = NUM_UNDEFINED;
        h_r = sp_wf->get_Number (&eo_num);
        ET_Gender eo_gender = GENDER_UNDEFINED;
        h_r = sp_wf->get_Gender (&eo_gender);
        ET_Tense eo_tense = TENSE_UNDEFINED;
        h_r = sp_wf->get_Tense (&eo_tense);
        ET_Person eo_person = PERSON_UNDEFINED;
        h_r = sp_wf->get_Person (&eo_person);
        ET_Reflexive eo_reflexive = REFL_UNDEFINED;
        h_r = sp_wf->get_IsReflexive (&eo_reflexive);
        ET_Voice eo_voice = VOICE_UNDEFINED;
        h_r = sp_wf->get_Voice (&eo_voice);
        ET_SpecialForm eo_special_form = SPECIAL_UNDEFINED;
        //h_r = sp_wf->get_SpecialForm (&eo_special_form);

        co_tmp_wf.str_wordform = OLE2W(sp_bstrWf);
        co_tmp_wf.eo_AdjForm = eo_adj_form;
        co_tmp_wf.eo_Animacy = eo_animacy;
        co_tmp_wf.eo_Case = eo_case;
        co_tmp_wf.eo_Gender = eo_gender;
        co_tmp_wf.eo_Number = eo_num;
        co_tmp_wf.eo_Person = eo_person;
        co_tmp_wf.eo_POS = eo_pos;
        co_tmp_wf.eo_Reflexive = eo_reflexive;
        co_tmp_wf.eo_Special = eo_special_form;
        co_tmp_wf.eo_Tense = eo_tense;
        co_tmp_wf.eo_Voice = eo_voice;

        pvec_Wordforms->push_back(co_tmp_wf);
    }

    if (l_Wordforms <= 0 || pvec_Wordforms == NULL)
    {
        return E_FAIL;
    }

    pstr_Paradigm = new wstring[pvec_Wordforms->size()];
    pi_Gram = new int[pvec_Wordforms->size()];
    for (vector<CT_GramHelper>::iterator iter_wf = pvec_Wordforms->begin();
        iter_wf < pvec_Wordforms->end();
        iter_wf++, i_++)
    {
        pstr_Paradigm[i_] = (*iter_wf).str_wordform;
        pi_Gram[i_] = (*iter_wf).i_GramNumber();
    }
    // Create several gr_stems
    int i_Stems = i_LCP(pstr_Paradigm, pstr_Stems, (int)l_Wordforms, 3);
    // Split every wordform into stem and ending,
    // then for every gr_stem create a table of endings usable with it
    CT_EndingsTable *pco_endings;
    pco_endings = new CT_EndingsTable[i_Stems];
    pi_subtable_id = new int[i_Stems];
    for (long l_wf = 0; l_wf < l_Wordforms; l_wf++)
    {
        for (int i_s = 0; i_s < i_Stems; i_s++)
        {
            b_match = regex_match(pstr_Paradigm[l_wf], result, (const wregex)(pstr_Stems[i_s] + L"(.*)"));
            if (b_match == true)
            {
                str_ending = (wstring)result[1];
                // Write the ending and its grammatical meaning to the temporary "endings table" number i_s
                pco_endings[i_s].i_AddEnding(pi_Gram[l_wf], str_ending);
                break;
            }
        }
    }
    // Write the "endings tables" to the db if necessary;
    // get their subtable_id's
    for (int i_s = 0; i_s < i_Stems; i_s++)
    {
        pi_subtable_id[i_s] = pco_endings[i_s].i_db_Write(pco_db);
    }
    // Add the stems with links to the corresponding subtables to the db
    for (int i_s = 0; i_s < i_Stems; i_s++)
    {
        pco_db->v_PrepareForInsert(L"stems_for_analysis", 3);
        pco_db->v_Bind(1, ll_lexeme_id);
        pco_db->v_Bind(2, pstr_Stems[i_s]);
        pco_db->v_Bind(3, pi_subtable_id[i_s]);
        pco_db->v_Finalize();
    }
    //}
    CoUninitialize();
    delete[] pvec_Wordforms;
    delete[] pco_endings;
    delete[] pstr_Paradigm;
    delete[] pi_Gram;
    delete[] pi_subtable_id;
    return S_OK;
}

int CT_Analyzer::i_Analyze(wstring str_wordform, vector<CT_GramHelper>* pvec_possible_wordforms)
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
        if (pvec_stems_id->empty() == true)
        {
            continue;
        }
        i_CheckEndings(pvec_possible_wordforms, pvec_stems_id, str_left, str_right);
    }
    return pvec_possible_wordforms->size();
}

int CT_Analyzer::i_LookUpStems(vector<int>* pvec_stems_id, wstring str_left)
{
    // Search the DB for graphic stems equal to str_left; write their IDs to vec_stems_id
    if (pco_db == NULL || pvec_stems_id == NULL)
    {
        return -1;
    }
    wstring str_query = L"Select * from stems_for_analysis where stem = \"" + str_left + L"\"";
    S_Field<int> so_stems_id;
    so_stems_id.i_ColNumber = 0;
    so_stems_id.pvec_FieldData = pvec_stems_id;
    pco_db->v_Exec(str_query, v_GetTableField_int, &so_stems_id);
    so_stems_id.pvec_FieldData = NULL; // pvec_stems_id lives, so_stems_id dies
    return pvec_stems_id->size();
}

int CT_Analyzer::i_CheckEndings(vector<CT_GramHelper>* pvec_possible_wordforms, vector<int>* pvec_stems_id, wstring str_left, wstring str_right)
{
    if (pco_db == NULL || pvec_stems_id == NULL)
    {
        return -1;
    }
    CT_GramHelper co_tmp_wf;
    static vector<int> vec_gram;
    wstring str_query;
    vector<int> vec_subtable_id;
    vector<__int64> vec_lexeme_id;
    vector<wstring> vec_lemma;
    S_Field<int> so_subtable_id, so_gram;
    S_Field<__int64> so_lexeme_id;
    S_Field<wstring> so_lemma;
    so_subtable_id.i_ColNumber = 3; // in stems_for_analysis table
    so_lexeme_id.i_ColNumber = 1; // in stems_for_analysis table
    so_gram.i_ColNumber = 3; // in endings_for_analysis table
    so_lemma.i_ColNumber = 1; // in headword table
    so_subtable_id.pvec_FieldData = &vec_subtable_id;
    so_lexeme_id.pvec_FieldData = &vec_lexeme_id;
    so_gram.pvec_FieldData = &vec_gram;
    so_lemma.pvec_FieldData = &vec_lemma;

    for (vector<int>::iterator iter_stems = pvec_stems_id->begin();
        iter_stems != pvec_stems_id->end(); iter_stems++)
    {
        // For each *iter_stems look up the endings table ID in DB, then in this table try to find
        // endings which are equal to str_right. For each ending found, write the parameters
        // to co_tmp_wf and then push_back co_tmp_wf to pvec_possible_wordforms:
        vec_gram.clear();
        str_query = L"Select * from stems_for_analysis where id = " + str_ToString(*iter_stems);
        pco_db->v_Exec(str_query, v_GetTableField_int, &so_subtable_id);
        pco_db->v_Exec(str_query, v_GetTableField_int, &so_lexeme_id);
        str_query = L"Select * from headword where id = " + str_ToString(*(vec_lexeme_id.begin()));
        pco_db->v_Exec(str_query, v_GetTableField_str, &so_lemma);
        str_query = L"Select * from endings_for_analysis where subtable_id = " + str_ToString(*(vec_subtable_id.begin())) 
            + L" and ending = \"" + str_right + L"\"";
        pco_db->v_Exec(str_query, v_GetTableField_int, &so_gram);
        for (vector<int>::iterator iter_endings = vec_gram.begin();
            iter_endings != vec_gram.end(); iter_endings++)
        {
            co_tmp_wf.i_DecodeNumber(*iter_endings);
            co_tmp_wf.ll_lexeme_id = *(vec_lexeme_id.begin());
            co_tmp_wf.str_lemma = *(vec_lemma.begin());
            co_tmp_wf.str_wordform = str_left + str_right;
            pvec_possible_wordforms->push_back(co_tmp_wf);
            co_tmp_wf.Clear();
        }
        vec_subtable_id.clear();
        vec_lexeme_id.clear();
        vec_lemma.clear(); // these 3 vectors are different for every stem found
    }
    vec_gram.clear();
    so_gram.pvec_FieldData = NULL; // vec_gram lives, so_gram dies
    return 0;
}

int CT_Analyzer::i_LCP(wstring* str_words, wstring* str_pfx, int i_words, int i_pfx)
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
    
        delete[] str_pfx;
        str_pfx = new wstring[i_numpfx_prev];
        for (int i_ = 0; i_ < i_numpfx_prev; i_++)
        {
            str_pfx[i_] = str_curpfx[i_];
        }
	    delete[] str_curpfx;
	    delete[] str_cursymb;
	    delete[] i_pfxclass;
	    return i_numpfx_prev;
    }
    