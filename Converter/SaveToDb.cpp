#include "StdAfx.h"
#include "ExtString.h"
#include "ZalEntryParser.h"
#include "GramHasher.h"

using namespace std::tr1;

bool b_ExtractStress (const wstring& str_in, wstring& str_out, map<int, ET_StressType>& map_stress)
{
    CT_ExtString xstr_in (str_in, L"<>");

    str_out = L"";

    for (int i_textField = 0; i_textField < xstr_in.i_GetNumOfFields(); ++i_textField)
    {
        str_out += xstr_in.str_GetField (i_textField);
    }

    int i_marksFound = 0;
    for (int i_breakField = 0; 
         i_breakField < xstr_in.i_GetNumOfFields (CT_ExtString::ec_TokenBreakChars); 
         ++i_breakField)
    {
        wstring str_stressMark = xstr_in.str_GetField (i_breakField, CT_ExtString::ec_TokenBreakChars);
        if (str_stressMark.length() != 1)
        {
            ATLASSERT(0);
            wstring str_msg (L"Error processing stress marks: ");
            str_msg += str_in;
            ERROR_LOG (str_msg);
            return false;
        }

        int i_pos = xstr_in.i_GetFieldOffset (i_breakField, CT_ExtString::ec_TokenBreakChars);
        i_pos -= i_marksFound;
        ++i_marksFound;

        if (L"<" == str_stressMark)
        {
            map_stress[i_pos] = STRESS_PRIMARY;
        }
        else if (L">" == str_stressMark)
        {
            map_stress[i_pos] = STRESS_SECONDARY;
        }
        else
        {
            ATLASSERT(0);
            wstring str_msg (L"Error extracting stress marks: ");
            str_msg += str_in;
            ERROR_LOG (str_msg);
            return false;
        }

    }   //  for (int i_breakField = 0; ...)

    return true;

//    if (xstr_source.i_NFields() < 2)
//    {
//        ATLASSERT(0);
//        wstring str_msg (L"Expected field is not found: ");
//        str_msg += str_Source;
//        ERROR_LOG (str_msg);
//        return false;
//    }

}   //  b_ExtractStress (...)

bool ST_Headword::b_SaveToDb (CT_Sqlite * pco_dbHandle, __int64& ll_headwordId)
{
    try
    {
        pco_dbHandle->v_PrepareForInsert (L"headword", 7);
        pco_dbHandle->v_Bind (1, str_Source);  // 0-based
        pco_dbHandle->v_Bind (2, str_PluralOf);
        pco_dbHandle->v_Bind (3, str_Comment);
        pco_dbHandle->v_Bind (4, str_Usage);
        pco_dbHandle->v_Bind (5, str_Variant);
        pco_dbHandle->v_Bind (6, str_SeeAlso);
        pco_dbHandle->v_Bind (7, str_Origination);
        pco_dbHandle->v_InsertRow();
        pco_dbHandle->v_Finalize();
        
        ll_headwordId = pco_dbHandle->ll_GetLastKey();

        //
        // Homonyms
        //
        for (unsigned int ui_homonym = 0; ui_homonym < vec_IHomonyms.size(); ++ui_homonym)
        {
            pco_dbHandle->v_PrepareForInsert (L"homonyms", 2);
            pco_dbHandle->v_Bind (1, ll_headwordId);  // 0-based
            pco_dbHandle->v_Bind (2, vec_IHomonyms[ui_homonym]);
            pco_dbHandle->v_InsertRow();
            pco_dbHandle->v_Finalize();
        }

        //
        // Stress
        //
        map<int, ET_StressType>::iterator it_stress = map_Stress.begin();
        for (; it_stress != map_Stress.end(); ++it_stress)
        {
            b_SaveStressData (pco_dbHandle, ll_headwordId, (*it_stress).first, (*it_stress).second);
        }
    }
    catch (...)
    {
        wstring str_msg;
        wstring str_error;
        try
        {
            pco_dbHandle->v_GetLastError (str_error);
            str_msg += wstring (L", error %d: ");
            str_msg += str_error;
        }
        catch (...)
        {
            str_msg = L"Apparent DB error ";
        }
    
        CString cs_msg;
        cs_msg.Format (str_msg.c_str(), pco_dbHandle->i_GetLastError());
        ERROR_LOG ((LPCTSTR)cs_msg);

        return false;
    }

    return true;

}   //  bool ST_Headword::b_SaveToDb (CT_Sqlite * pco_dbHandle)

bool ST_Headword::b_SaveStressData (CT_Sqlite * pco_dbHandle, 
                                    __int64 ll_headwordId, 
                                    int i_position, 
                                    ET_StressType eo_type)
{
    try
    {
        pco_dbHandle->v_PrepareForInsert (L"stress", 3);
        pco_dbHandle->v_Bind (1, ll_headwordId);  // 0-based
        pco_dbHandle->v_Bind (2, i_position);
        pco_dbHandle->v_Bind (3, (STRESS_PRIMARY == eo_type) ? true : false);    // is_primary
        pco_dbHandle->v_InsertRow();
        pco_dbHandle->v_Finalize();
    }
    catch (CT_Exception& co_exc)
    {
        wstring str_msg (co_exc.str_GetDescription());
        wstring str_error;
        try
        {
            pco_dbHandle->v_GetLastError (str_error);
            str_msg += wstring (L", error %d: ");
            str_msg += str_error;
        }
        catch (...)
        {
            str_msg = L"Apparent DB error ";
        }
    
        CString cs_msg;
        cs_msg.Format (str_msg.c_str(), pco_dbHandle->i_GetLastError());
        ERROR_LOG ((LPCTSTR)cs_msg);

        return false;
    }

    return true;

}   //  b_SaveStressData()

bool ST_Descriptor::b_SaveToDb (CT_Sqlite * pco_dbHandle, __int64 ll_wordId, __int64& ll_descriptorId)
{
    ll_descriptorId = -1;

    wsmatch result;
    static CT_GramHasher co_gram;
    try
    {
        pco_dbHandle->v_PrepareForInsert (L"descriptor", 31);
        pco_dbHandle->v_Bind (1, ll_wordId);
        pco_dbHandle->v_Bind (2, str_GraphicStem);
        pco_dbHandle->v_Bind (3, b_Variant);
        pco_dbHandle->v_Bind (4, str_MainSymbol);
        pco_dbHandle->v_Bind (5, e_PartOfSpeech);
        pco_dbHandle->v_Bind (6, b_PluralOf);
        pco_dbHandle->v_Bind (7, b_Intransitive);
        pco_dbHandle->v_Bind (8, b_Reflexive);
        pco_dbHandle->v_Bind (9, str_MainSymbolPluralOf);
        pco_dbHandle->v_Bind (10, str_AltMainSymbol);
        pco_dbHandle->v_Bind (11, str_InflectionSymbol);
        pco_dbHandle->v_Bind (12, str_Comment);
        pco_dbHandle->v_Bind (13, str_AltMainSymbolComment);
        pco_dbHandle->v_Bind (14, str_AltInflectionComment);
        pco_dbHandle->v_Bind (15, str_VerbAlternation);
        pco_dbHandle->v_Bind (16, b_PastPartPassZhD);
        pco_dbHandle->v_Bind (17, i_Section);
        pco_dbHandle->v_Bind (18, b_NoComparative);
        pco_dbHandle->v_Bind (19, b_AssumedForms);
        pco_dbHandle->v_Bind (20, b_Yo);
        pco_dbHandle->v_Bind (21, b_O);
        pco_dbHandle->v_Bind (22, b_Gen2);
        pco_dbHandle->v_Bind (23, b_IsImpersonal);
        pco_dbHandle->v_Bind (24, b_IsIterative);
        pco_dbHandle->v_Bind (25, b_HasAspectPair);
        //pco_dbHandle->v_Bind (26, i_AspectPairType);
        //pco_dbHandle->v_Bind (27, str_AspectPairComment);
        pco_dbHandle->v_Bind (26, str_Difficulties);
//        pco_dbHandle->v_Bind (27, str_IrregularForms);
        pco_dbHandle->v_Bind (27, b_HasIrregularForms);
        pco_dbHandle->v_Bind (28, str_RestrictedForms);
        pco_dbHandle->v_Bind (29, str_Contexts);
        pco_dbHandle->v_Bind (30, str_Cognate);
        pco_dbHandle->v_Bind (31, str_TrailingComment);
        pco_dbHandle->v_InsertRow();
        pco_dbHandle->v_Finalize();

        ll_descriptorId = pco_dbHandle->ll_GetLastKey();

        if (b_Loc2)
        {
            pco_dbHandle->v_PrepareForInsert (L"second_locative", 3);
            pco_dbHandle->v_Bind (1, ll_descriptorId);
            pco_dbHandle->v_Bind (2, b_Loc2Optional);
            pco_dbHandle->v_Bind (3, str_Loc2Preposition);
            pco_dbHandle->v_InsertRow();
            pco_dbHandle->v_Finalize();
        }

        if (b_HasAspectPair)
        {
            vector<wstring>::iterator iter_aspect_comment = vec_str_AspectPairComment.begin();
            for (vector<int>::iterator iter_aspect_type = vec_i_AspectPairType.begin();
                iter_aspect_type != vec_i_AspectPairType.end();
                ++iter_aspect_type, ++iter_aspect_comment)
            {
                pco_dbHandle->v_PrepareForInsert (L"aspect_pair", 3);
                pco_dbHandle->v_Bind (1, ll_descriptorId);
                pco_dbHandle->v_Bind (2, *iter_aspect_type);
                pco_dbHandle->v_Bind (3, *iter_aspect_comment);
                pco_dbHandle->v_InsertRow();
                pco_dbHandle->v_Finalize();
            }
        }

        while (str_IrregularForms.length()) // it deletes str_IrregularForms
        {
            bool b_match = regex_match(str_IrregularForms, result, 
                                        (const wregex)L"\\s*_([^_]+)_\\s*([^\\s,;]+),?;?(.*)");
            if (b_match == true)
            {
                wstring str_form = (wstring)result[2];
                map<int, ET_StressType> map_stress;
                bool b_ret = b_ExtractStress (str_form, str_form, map_stress);

                co_gram.h_GramClear();
                co_gram.eo_POS = e_PartOfSpeech;
                if (str_MainSymbol == L"св")
                {
                    co_gram.eo_Aspect = ASPECT_PERFECTIVE;
                }
                else if (str_MainSymbol == L"нсв")
                {
                    co_gram.eo_Aspect = ASPECT_IMPERFECTIVE;
                }
                //co_gram.str_Lemma = st_Head.str_Source;
                co_gram.i_DecodeString((wstring)result[1]);
// TODO can DecodeString handle this?
                if (POS_VERB == e_PartOfSpeech)
                {
                    co_gram.eo_Reflexive = b_Reflexive ? REFL_YES : REFL_NO;
                }
                pco_dbHandle->v_PrepareForInsert (L"irregular_forms", 4);
                pco_dbHandle->v_Bind (1, ll_descriptorId);
                pco_dbHandle->v_Bind (2, co_gram.i_GramHash());     // Morphosyntactic values code
                pco_dbHandle->v_Bind (3, str_form);                 // Wordform
                pco_dbHandle->v_Bind (4, false);                // isn't alternative to the regular form
                pco_dbHandle->v_InsertRow();
                pco_dbHandle->v_Finalize();

                b_SaveIrregularStressData (pco_dbHandle, 
                                           pco_dbHandle->ll_GetLastKey(), 
                                           map_stress);

                str_IrregularForms = (wstring)result[3];
            }
            else
            {
                if (str_IrregularForms.length() > 0)
                {
                    ERROR_LOG(L"INSERT>  str_IrregularForms is non-empty after extraction: " + str_IrregularForms);
                }
                break;
            }
        }

        while (str_AltIrregularForms.length()) // it deletes str_AltIrregularForms
        {
            bool b_match = regex_match(str_AltIrregularForms, result, 
                                        (const wregex)L"\\s*_([^_]+)_\\s*([^\\s,;]+),?;?(.*)");
            if (b_match == true)
            {
                wstring str_form = (wstring)result[2];
                map<int, ET_StressType> map_stress;
                bool b_ret = b_ExtractStress (str_form, str_form, map_stress);

                co_gram.h_GramClear();
                co_gram.eo_POS = e_PartOfSpeech;
                if (str_MainSymbol == L"св")
                {
                    co_gram.eo_Aspect = ASPECT_PERFECTIVE;
                }
                else if (str_MainSymbol == L"нсв")
                {
                    co_gram.eo_Aspect = ASPECT_IMPERFECTIVE;
                }
                //co_gram.str_Lemma = st_Head.str_Source;
                co_gram.i_DecodeString((wstring)result[1]);
// TODO can DecodeString handle this?
                if (POS_VERB == e_PartOfSpeech)
                {
                    co_gram.eo_Reflexive = b_Reflexive ? REFL_YES : REFL_NO;
                }
                pco_dbHandle->v_PrepareForInsert (L"irregular_forms", 4);
                pco_dbHandle->v_Bind (1, ll_descriptorId);
                pco_dbHandle->v_Bind (2, co_gram.i_GramHash());     // Morphosyntactic values code
                pco_dbHandle->v_Bind (3, str_form);                 // Wordform
                pco_dbHandle->v_Bind (4, true);                // is alternative to the regular form
                pco_dbHandle->v_InsertRow();
                pco_dbHandle->v_Finalize();

                b_SaveIrregularStressData (pco_dbHandle, 
                                           pco_dbHandle->ll_GetLastKey(), 
                                           map_stress);

                str_AltIrregularForms = (wstring)result[3];
            }
            else
            {
                if (str_AltIrregularForms.length() > 0)
                {
                    ERROR_LOG(L"INSERT>  str_AltIrregularForms is non-empty after extraction: " + str_IrregularForms);
                }
                break;
            }
        }

        while (str_Difficulties.length()) // it deletes str_Difficulties
        {
            bool b_match = regex_match(str_Difficulties, result, 
                                        (const wregex)L"\\s*([^,]+),?\\s*(.*)");
            if (b_match == true)
            {
                co_gram.h_GramClear();
                co_gram.eo_POS = e_PartOfSpeech;
                if (str_MainSymbol == L"св")
                {
                    co_gram.eo_Aspect = ASPECT_PERFECTIVE;
                }
                else if (str_MainSymbol == L"нсв")
                {
                    co_gram.eo_Aspect = ASPECT_IMPERFECTIVE;
                }
                if (POS_VERB == e_PartOfSpeech)
                {
                    co_gram.eo_Reflexive = b_Reflexive ? REFL_YES : REFL_NO;
                }
                co_gram.i_DecodeString((wstring)result[1]);
                pco_dbHandle->v_PrepareForInsert (L"difficulties", 3);
                pco_dbHandle->v_Bind (1, ll_descriptorId);
                pco_dbHandle->v_Bind (2, co_gram.i_GramHash());   // Morphosyntactic values code
                pco_dbHandle->v_Bind (3, 1);    // 1 = difficult
                pco_dbHandle->v_InsertRow();
                pco_dbHandle->v_Finalize();
                str_Difficulties = (wstring)result[2];
            }
            else
            {
                if (str_Difficulties.length() > 0)
                {
                    ERROR_LOG(L"INSERT>  str_Difficulties is non-empty after extraction: " + str_Difficulties);
                }
                break;
            }
        }

        while (str_Obsolete.length()) // it deletes str_Obsolete
        {
            bool b_match = regex_match(str_Obsolete, result, 
                                        (const wregex)L"\\s*([^,]+),?\\s*(.*)");
            if (b_match == true)
            {
                co_gram.h_GramClear();
                co_gram.eo_POS = e_PartOfSpeech;
                if (str_MainSymbol == L"св")
                {
                    co_gram.eo_Aspect = ASPECT_PERFECTIVE;
                }
                else if (str_MainSymbol == L"нсв")
                {
                    co_gram.eo_Aspect = ASPECT_IMPERFECTIVE;
                }
                if (POS_VERB == e_PartOfSpeech)
                {
                    co_gram.eo_Reflexive = b_Reflexive ? REFL_YES : REFL_NO;
                }
                co_gram.i_DecodeString((wstring)result[1]);
                pco_dbHandle->v_PrepareForInsert (L"difficulties", 3);
                pco_dbHandle->v_Bind (1, ll_descriptorId);
                pco_dbHandle->v_Bind (2, co_gram.i_GramHash());   // Morphosyntactic values code
                pco_dbHandle->v_Bind (3, 2);    // 2 = obsolete
                pco_dbHandle->v_InsertRow();
                pco_dbHandle->v_Finalize();
                str_Obsolete = (wstring)result[2];
            }
            else
            {
                if (str_Obsolete.length() > 0)
                {
                    ERROR_LOG(L"INSERT>  str_Obsolete is non-empty after extraction: " + str_Difficulties);
                }
                break;
            }
        }

        while (str_Deficient.length()) // it deletes str_Deficient
        {
            bool b_match = regex_match(str_Deficient, result, 
                                        (const wregex)L"\\s*([^,]+),?\\s*(.*)");
            if (b_match == true)
            {
                co_gram.h_GramClear();
                co_gram.eo_POS = e_PartOfSpeech;
                if (str_MainSymbol == L"св")
                {
                    co_gram.eo_Aspect = ASPECT_PERFECTIVE;
                }
                else if (str_MainSymbol == L"нсв")
                {
                    co_gram.eo_Aspect = ASPECT_IMPERFECTIVE;
                }
                if (POS_VERB == e_PartOfSpeech)
                {
                    co_gram.eo_Reflexive = b_Reflexive ? REFL_YES : REFL_NO;
                }
                co_gram.i_DecodeString((wstring)result[1]);
                pco_dbHandle->v_PrepareForInsert (L"nonexistent_forms", 2);
                pco_dbHandle->v_Bind (1, ll_descriptorId);
                pco_dbHandle->v_Bind (2, co_gram.i_GramHash());   // Morphosyntactic values code
                pco_dbHandle->v_InsertRow();
                pco_dbHandle->v_Finalize();
                str_Deficient = (wstring)result[2];
            }
            else
            {
                if (str_Difficulties.length() > 0)
                {
                    ERROR_LOG(L"INSERT>  str_Deficient is non-empty after extraction: " + str_Deficient);
                }
                break;
            }
        }

        if (st_InflectionGroup.b_HasData)
        {
            bool b_ = b_SaveInflectionGroup (pco_dbHandle, 
                                             ll_descriptorId, 
                                             st_InflectionGroup,
                                             true);
            if (st_AltInflectionGroup.b_HasData)
            {
                b_ = b_SaveInflectionGroup (pco_dbHandle, 
                                             ll_descriptorId, 
                                             st_AltInflectionGroup,
                                             false);
            }
            if (!b_)
            {
                ERROR_LOG (L"Error saving inflectiongroup.");
            }
        }
    }
    catch (CT_Exception& co_exc)
    {
        ATLASSERT(0);
        wstring str_msg (co_exc.str_GetDescription());
        wstring str_error;
        try
        {
            pco_dbHandle->v_GetLastError (str_error);
            str_msg += wstring (L", error %d: ");
            str_msg += str_error;
        }
        catch (...)
        {
            str_msg = L"Apparent DB error ";
        }
    
        CString cs_msg;
        cs_msg.Format (str_msg.c_str(), pco_dbHandle->i_GetLastError());
        ERROR_LOG ((LPCTSTR)cs_msg);

        return false;
    }

    return true;

}   // ST_Descriptor::b_SaveToDb()

bool ST_Descriptor::b_SaveInflectionGroup (CT_Sqlite * pco_dbHandle, 
                                           __int64 ll_descriptorId, 
                                           const ST_InflectionGroup& st_data, 
                                           bool b_isPrimary)
{
    try
    {
        bool b_dummy = false;

        pco_dbHandle->v_PrepareForInsert (L"inflection", 12);
        pco_dbHandle->v_Bind (1, ll_descriptorId);
        pco_dbHandle->v_Bind (2, b_isPrimary);
        pco_dbHandle->v_Bind (3, st_data.i_Type);
        pco_dbHandle->v_Bind (4, st_data.e_AccentType1);
        pco_dbHandle->v_Bind (5, st_data.e_AccentType2);

        if (POS_VERB == e_PartOfSpeech)
        {
            pco_dbHandle->v_Bind (6, b_dummy);          // short_form_restrictions N/A
            pco_dbHandle->v_Bind (7, st_data.b_X);      // past_part_restrictions
            pco_dbHandle->v_Bind (8, b_dummy);          // no_short_form N/A
            pco_dbHandle->v_Bind (9, st_data.b_BoxedX); // no_past_part
        }
        else
        {
            pco_dbHandle->v_Bind (6, st_data.b_X);      // short_form_restrictions 
            pco_dbHandle->v_Bind (7, b_dummy);          // past_part_restrictions N/A
            pco_dbHandle->v_Bind (8, st_data.b_BoxedX); // no_short_form
            pco_dbHandle->v_Bind (9, b_dummy);          // no_past_part N/A
        }

        pco_dbHandle->v_Bind (10, st_data.b_FleetingVowel);
        pco_dbHandle->v_Bind (11, st_data.i_StemAugmentType);
        pco_dbHandle->v_Bind (12, st_data.i_InflectedParts);
        pco_dbHandle->v_InsertRow();
        pco_dbHandle->v_Finalize();

        __int64 ll_igId = pco_dbHandle->ll_GetLastKey();
        for (unsigned int ui_cd = 0; ui_cd < st_data.vec_stCommonDeviations.size(); ++ui_cd)
        {
            pco_dbHandle->v_PrepareForInsert (L"common_deviation", 3);
            pco_dbHandle->v_Bind (1, ll_igId);
            pco_dbHandle->v_Bind (2, st_data.vec_stCommonDeviations[ui_cd].i_Type);
            pco_dbHandle->v_Bind (3, st_data.vec_stCommonDeviations[ui_cd].b_Optional);
            pco_dbHandle->v_InsertRow();
            pco_dbHandle->v_Finalize();
        }
    }
    catch (CT_Exception& co_exc)
    {
        wstring str_msg (co_exc.str_GetDescription());
        wstring str_error;
        try
        {
            pco_dbHandle->v_GetLastError (str_error);
            str_msg += wstring (L", error %d: ");
            str_msg += str_error;
        }
        catch (...)
        {
            str_msg = L"Apparent DB error ";
        }
    
        CString cs_msg;
        cs_msg.Format (str_msg.c_str(), pco_dbHandle->i_GetLastError());
        ERROR_LOG ((LPCTSTR)cs_msg);

        return false;
    }

    return true;

}   //  b_SaveInflectionGroup (...)

bool ST_Descriptor::b_SaveIrregularStressData (CT_Sqlite * pco_dbHandle, 
                                               __int64 ll_formId, 
                                               const map<int, ET_StressType>& map_stress) 
{
    try
    {
        map<int, ET_StressType>::const_iterator it_at = map_stress.begin();
        for (; it_at != map_stress.end(); ++it_at)
        {
            pco_dbHandle->v_PrepareForInsert (L"irregular_stress", 3);
            pco_dbHandle->v_Bind (1, ll_formId);
            pco_dbHandle->v_Bind (2, (*it_at).first);
            pco_dbHandle->v_Bind (3, (STRESS_PRIMARY == (*it_at).second) ? true : false);
            pco_dbHandle->v_InsertRow();
            pco_dbHandle->v_Finalize();
        }
    }
    catch (CT_Exception& co_exc)
    {
        wstring str_msg (co_exc.str_GetDescription());
        wstring str_error;
        try
        {
            pco_dbHandle->v_GetLastError (str_error);
            str_msg += wstring (L", error %d: ");
            str_msg += str_error;
        }
        catch (...)
        {
            str_msg = L"Apparent DB error ";
        }
    
        CString cs_msg;
        cs_msg.Format (str_msg.c_str(), pco_dbHandle->i_GetLastError());
        ERROR_LOG ((LPCTSTR)cs_msg);

        return false;
    }

    return true;

}   //  b_SaveIrregularStressData (...)
