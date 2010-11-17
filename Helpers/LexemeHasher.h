#pragma once

#include "stdafx.h"
#include "SqliteWrapper.h"
#include "MD5.h"

struct ST_LexemeHasher
{
    map<int, ET_StressType> map_Stress;
    wstring str_SourceForm;
    wstring str_MainSymbol;
    int i_InflectionType;
    int i_AccentType1;
    int i_AccentType2;
    wstring str_Comment;

    wstring str_Hash()
    {
        wstring str_text (str_SourceForm);

        map<int, ET_StressType>::iterator it_stress = map_Stress.begin();
        for (; it_stress != map_Stress.end(); ++ it_stress)
        {
            unsigned char * pchr_stress = (unsigned char *)&(*it_stress).first;
            str_text += 32 + pchr_stress[0];
            pchr_stress = (unsigned char *)&(*it_stress).second;
            str_text += 32 + pchr_stress[0];
        }

        str_text += str_MainSymbol;

        unsigned char * pchr_at = (unsigned char *)&i_InflectionType;
        str_text += 32 + pchr_at[0];

        pchr_at = (unsigned char *)&i_AccentType1;
        str_text += 32 + pchr_at[0];

        pchr_at = (unsigned char *)&i_AccentType2;
        str_text += 32 + pchr_at[0];

        str_text += str_Comment;

//        unsigned int ui_hash = GenericHash::ui_hash ((unsigned char *)str_text.c_str(), 
//                                                     str_text.length()*sizeof (wchar_t), 
//                                                     0);
        CT_MD5 co_md5;
        return co_md5.str_Hash (str_text);

    }   // unsigned int str_Hash()

    bool b_SaveToDb (CT_Sqlite * pco_dbHandle, __int64 ll_descriptorId)
    {
        try
        {
            pco_dbHandle->v_PrepareForInsert (L"lexeme_hash_to_descriptor", 2);
            pco_dbHandle->v_Bind (1, str_Hash());
            pco_dbHandle->v_Bind (2, ll_descriptorId);
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

    }   //  bool b_SaveToDb (...)


};  //  struct ST_LexemeHasher
