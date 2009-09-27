#include "StdAfx.h"
#include "ExtString.h"
#include "ZalEntryParser.h"
//#include "EndingsParser.h"
//#include "Analysis.h"
//#include "Synthesis.h"

using namespace std::tr1;

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
        if (i_PrimaryStress >= 0)
        {
            b_SaveStressData (pco_dbHandle, ll_headwordId, i_PrimaryStress, true);
        }
        if (i_PrimaryStress2 >= 0)
        {
            b_SaveStressData (pco_dbHandle, ll_headwordId, i_PrimaryStress, true);
        }
        if (i_SecondaryStress >= 0)
        {
            b_SaveStressData (pco_dbHandle, ll_headwordId, i_PrimaryStress, false);
        }
        if (i_SecondaryStress2 >= 0)
        {
            b_SaveStressData (pco_dbHandle, ll_headwordId, i_PrimaryStress, false);
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
                                    bool b_isPrimary)
{
    try
    {
        pco_dbHandle->v_PrepareForInsert (L"stress", 3);
        pco_dbHandle->v_Bind (1, ll_headwordId);  // 0-based
        pco_dbHandle->v_Bind (2, i_position);
        pco_dbHandle->v_Bind (3, b_isPrimary);    // is_primary
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

bool ST_Descriptor::b_SaveToDb (CT_Sqlite * pco_dbHandle, 
                                __int64 ll_wordId)
{
    wsmatch result;
    static CT_Wordform co_gram;
    try
    {
        pco_dbHandle->v_PrepareForInsert (L"descriptor", 25);
        pco_dbHandle->v_Bind (1, ll_wordId);
        pco_dbHandle->v_Bind (2, str_GraphicStem);
        pco_dbHandle->v_Bind (3, b_Variant);
        pco_dbHandle->v_Bind (4, str_MainSymbol);
        pco_dbHandle->v_Bind (5, b_PluralOf);
        pco_dbHandle->v_Bind (6, b_Intransitive);
        pco_dbHandle->v_Bind (7, str_MainSymbolPluralOf);
        pco_dbHandle->v_Bind (8, str_AltMainSymbol);
        pco_dbHandle->v_Bind (9, str_InflectionType);
        pco_dbHandle->v_Bind (10, e_PartOfSpeech);
        pco_dbHandle->v_Bind (11, str_Comment);
        pco_dbHandle->v_Bind (12, str_AltMainSymbolComment);
        pco_dbHandle->v_Bind (13, str_AltInflectionComment);
        pco_dbHandle->v_Bind (14, str_VerbAlternation);
        pco_dbHandle->v_Bind (15, i_Section);
        pco_dbHandle->v_Bind (16, b_NoComparative);
        pco_dbHandle->v_Bind (17, b_AssumedForms);
        pco_dbHandle->v_Bind (18, b_Yo);
        pco_dbHandle->v_Bind (19, b_O);
        pco_dbHandle->v_Bind (20, b_Gen2);
        pco_dbHandle->v_Bind (21, str_Difficulties);
        pco_dbHandle->v_Bind (22, str_IrregularForms);
        pco_dbHandle->v_Bind (23, str_RestrictedForms);
        pco_dbHandle->v_Bind (24, str_Contexts);
        pco_dbHandle->v_Bind (25, str_TrailingComment);
        pco_dbHandle->v_InsertRow();
        pco_dbHandle->v_Finalize();

        __int64 ll_descriptorId = pco_dbHandle->ll_GetLastKey();

        if (b_Loc2)
        {
            pco_dbHandle->v_PrepareForInsert (L"second_locative", 3);
            pco_dbHandle->v_Bind (1, ll_descriptorId);
            pco_dbHandle->v_Bind (2, b_Loc2Optional);
            pco_dbHandle->v_Bind (3, str_Loc2Preposition);
            pco_dbHandle->v_InsertRow();
            pco_dbHandle->v_Finalize();
        }

        while (str_IrregularForms.length()) // it deletes str_IrregularForms
        {
            bool b_match = regex_match(str_IrregularForms, result, 
                                        (const wregex)L"\\s*_([^_]+)_\\s*([^\\s,]+),?(.*)");
            if (b_match == true)
            {
                co_gram.GramClear();
                co_gram.eo_POS = e_PartOfSpeech;
                co_gram.i_DecodeString((wstring)result[1]);
                pco_dbHandle->v_PrepareForInsert (L"irregular_forms", 3);
                pco_dbHandle->v_Bind (1, ll_descriptorId);
                pco_dbHandle->v_Bind (2, co_gram.i_GramNumber());   // Morphosyntactic values code
                pco_dbHandle->v_Bind (3, (wstring)result[2]);       // Wordform
                pco_dbHandle->v_InsertRow();
                pco_dbHandle->v_Finalize();
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

        while (str_Difficulties.length()) // it deletes str_Difficulties
        {
            bool b_match = regex_match(str_Difficulties, result, 
                                        (const wregex)L"\\s*([^,]+),?(.*)");
            if (b_match == true)
            {
                co_gram.GramClear();
                co_gram.eo_POS = e_PartOfSpeech;
                co_gram.i_DecodeString((wstring)result[1]);
                pco_dbHandle->v_PrepareForInsert (L"difficulties", 2);
                pco_dbHandle->v_Bind (1, ll_descriptorId);
                pco_dbHandle->v_Bind (2, co_gram.i_GramNumber());   // Morphosyntactic values code
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
        pco_dbHandle->v_Bind (11, st_data.b_StemAugment);
        pco_dbHandle->v_Bind (12, st_data.e_CommonDeviation);
        pco_dbHandle->v_InsertRow();
        pco_dbHandle->v_Finalize();

/*
        __int64 ll_igId = pco_dbHandle->ll_GetLastKey();
        for (unsigned int ui_cd = 0; ui_cd < st_data.vec_iCommonDeviations.size(); ++ui_cd)
        {
            pco_dbHandle->v_PrepareForInsert (L"common_deviation", 2);
            pco_dbHandle->v_Bind (1, ll_igId);
            pco_dbHandle->v_Bind (2, st_data.vec_iCommonDeviations[ui_cd]);
            pco_dbHandle->v_InsertRow();
            pco_dbHandle->v_Finalize();
        }
*/
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
}


