#include "stdafx.h"
#include "Analyzer.h"

using namespace std;
using namespace std::tr1;

int CT_Analyzer::i_Analyze(CT_Sqlite* pco_dbHandle, wstring str_wordform)
    {
        if (str_wordform.length() <= 0 || pco_dbHandle == NULL)
        {
            return -1;
        }
        wstring str_left, str_right;
        vector<int>* pvec_stems_id;
        vector<CT_GramHelper>* pvec_possible_wordforms;
        pvec_possible_wordforms = new vector<CT_GramHelper>;
        pvec_stems_id = new vector<int>;
        for (int i_left = str_wordform.length(); i_left >= 0; i_left--)
        {
            str_left = str_wordform.substr(0, i_left);
            str_right = str_wordform.substr(i_left, str_wordform.length() - i_left);
            pvec_stems_id->clear();
            i_LookUpStems(pco_dbHandle, pvec_stems_id, str_left);
            if (pvec_stems_id->empty() == true)
            {
                continue;
            }
            i_CheckEndings(pco_dbHandle, pvec_possible_wordforms, pvec_stems_id, str_left, str_right);
        }
        if (pvec_possible_wordforms->empty() == true)
        {
            return 1; // No words found
        }
        return 0;
    }

    int CT_Analyzer::i_LookUpStems(CT_Sqlite* pco_dbHandle, vector<int>* pvec_stems_id, wstring str_left)
    {
        // Search the DB for graphic stems equal to str_left; write their IDs to vec_stems_id
        if (pco_dbHandle == NULL || pvec_stems_id == NULL)
        {
            return -1;
        }
        wstring str_query = L"Select * from GrStems where stem = \"" + str_left + L"\"";
        S_Field<int> so_stems_id;
        so_stems_id.i_ColNumber = 0;
        so_stems_id.pvec_FieldData = pvec_stems_id;
        pco_dbHandle->v_Exec(str_query, v_GetTableField_int, &so_stems_id);
        so_stems_id.pvec_FieldData = NULL; // pvec_stems_id lives, so_stems_id dies
        return pvec_stems_id->size();
    }

    int CT_Analyzer::i_CheckEndings(CT_Sqlite* pco_dbHandle, vector<CT_GramHelper>* pvec_possible_wordforms, vector<int>* pvec_stems_id, wstring str_left, wstring str_right)
    {
        CT_GramHelper co_tmp_wf;
        static vector<int> vec_gram;
        wstring str_query;
        vector<int> vec_subtable_id, vec_lexeme_id;
        vector<wstring> vec_lemma;

        S_Field<int> so_subtable_id, so_lexeme_id, so_gram;
        S_Field<wstring> so_lemma;
        so_subtable_id.i_ColNumber = 3; // in Stems table
        so_lexeme_id.i_ColNumber = 2; // in Stems table
        so_gram.i_ColNumber = 3; // in Endings table
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
            str_query = L"Select * from stems where id = " + str_int2wstring(*iter_stems);
            pco_dbHandle->v_Exec(str_query, v_GetTableField_int, &so_subtable_id);
            pco_dbHandle->v_Exec(str_query, v_GetTableField_int, &so_lexeme_id);
            str_query = L"Select * from headword where id = " + str_int2wstring(*vec_lexeme_id.begin());
            pco_dbHandle->v_Exec(str_query, v_GetTableField_str, &so_lemma);
            str_query = L"Select * from endings where subtable_id = " + str_int2wstring(*vec_subtable_id.begin()) 
                        + L" and ending = \"" + str_right + L"\"";
            pco_dbHandle->v_Exec(str_query, v_GetTableField_int, &so_gram);
            for (vector<int>::iterator iter_endings = vec_gram.begin();
                iter_endings != vec_gram.end(); iter_endings++)
            {
                co_tmp_wf.i_DecodeNumber(*iter_endings);
                co_tmp_wf.ll_lexeme_id = (__int64)(*vec_lexeme_id.begin()); // TODO: get rid of this conversion
                co_tmp_wf.str_lemma = (*vec_lemma.begin());
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

    int CT_Analyzer::i_WriteStemsAndEndings(CT_Sqlite* pco_dbHandle, __int64 ll_lexeme_id, int last_subtable_id)
    {
        if (ll_lexeme_id < 0)
        {
            return -1;
        }
    
        wstring str_Stem;
        //wstring str_Stem = str_GrStem(ll_lexeme_id);
        //if (str_Stem == L"")
        //{
        wsmatch result;
        wstring *str_Paradigm, *str_Stems, str_ending;
        int *i_Gram;
        bool b_match;
        // Generate paradigm (i_th element of str_Paradigm has form i_Gram[i])

        int i_Wordforms;
        /*
        //Commented to suppress a compilation-time error
        int i_Wordforms = i_GenerateParadigm(pco_dbHandle, ll_lexeme_id, str_Paradigm, i_Gram);
        */

        if (i_Wordforms <= 0)
        {
            return 1;
        }
        // Create several gr_stems
        int i_Stems = i_LCP(str_Paradigm, str_Stems, i_Wordforms, 3);
        // Split every wordform into stem and ending
        for (int i_wf = 0; i_wf < i_Wordforms; i_wf++)
        {
            for (int i_s = 0; i_s < i_Stems; i_s++)
            {
                b_match = regex_match(str_Paradigm[i_wf], result, (const wregex)(str_Stems[i_s] + L"(.*)"));
                if (b_match == true)
                {
                    str_ending = (wstring)result[1];
                    // Write the ending, its grammatical meaning and i_s to the temporary "endings table"
                    break;
                }
            }
        }
        // Check whether the temporary "endings tables" coincide with the existing real tables
        //}
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
    

    /*
    void v_LearnSubtableID_Endings(sqlite3_stmt *po_statement, void *v_subtable_id)
    {
        if (v_subtable_id == NULL)
        {
            return;
        }
        vector<int> *pvec_subtable_id = (vector<int> *)v_subtable_id;
        pvec_subtable_id->push_back(sqlite3_column_int(po_statement, 1)); // Provided the subtable_id column is #1.
        ERROR_LOG(L"*** v_LearnSubtableID_Endings works.");
        return;
    }
    
    void v_LearnSubtableID_Stems(sqlite3_stmt *po_statement, void *v_subtable_id)
    {
        if (v_subtable_id == NULL)
        {
            return;
        }
        vector<int> *pvec_subtable_id = (vector<int> *)v_subtable_id;
        pvec_subtable_id->push_back(sqlite3_column_int(po_statement, 3)); // Provided the subtable_id column in the stems table is #3.
        ERROR_LOG(L"*** v_LearnSubtableID_Stems works.");
        return;
    }
    
    void v_LearnGram_Endings(sqlite3_stmt *po_statement, void *v_gram)
    {
        if (v_gram == NULL)
        {
            return;
        }
        vector<int> *pvec_gram = (vector<int> *)v_gram;
        pvec_gram->push_back(sqlite3_column_int(po_statement, 2)); // Provided the gram column is #3.
        ERROR_LOG(L"*** v_LearnGram_Endings works.");
        return;
    }
    
    void v_LearnStemsId(sqlite3_stmt *po_statement, void *v_stems_id)
    {
        if (v_stems_id == NULL)
        {
            return;
        }
        vector<int>* pvec_stems_id = (vector<int>*)v_stems_id;
        pvec_stems_id->push_back(sqlite3_column_int(po_statement, 0));
        return;
    }
    */