#pragma once

#include "stdafx.h"
#include "SqliteWrapper.h"
#include "MD5.h"

struct StLexemeHasher
{
    map<int, ET_StressType> mapStress;
    CEString sSourceForm;
    CEString sMainSymbol;
    int iInflectionType;
    int iAccentType1;
    int iAccentType2;
    CEString sComment;

    CEString sHash()
    {
        CEString sText (sSourceForm);

        map<int, ET_StressType>::iterator itStress = mapStress.begin();
        for (; itStress != mapStress.end(); ++ itStress)
        {
            unsigned char * pchrStress = (unsigned char *)&(*itStress).first;
            sText += 32 + pchrStress[0];
            pchrStress = (unsigned char *)&(*itStress).second;
            sText += 32 + pchrStress[0];
        }

        sText += sMainSymbol;

        unsigned char * pchrAt = (unsigned char *)&iInflectionType;
        sText += 32 + pchrAt[0];

        pchrAt = (unsigned char *)&iAccentType1;
        sText += 32 + pchrAt[0];

        pchrAt = (unsigned char *)&iAccentType2;
        sText += 32 + pchrAt[0];

        sText += sComment;

//        unsigned int ui_hash = GenericHash::ui_hash ((unsigned char *)sText.c_str(), 
//                                                     sText.length()*sizeof (wchar_t), 
//                                                     0);
        CMD5 md5;
        return md5.sHash (sText);

    }   // unsigned int sHash()

    bool bSaveToDb (CSqlite * pDbHandle, __int64 llDescriptorId)
    {
        try
        {
            pDbHandle->PrepareForInsert (L"lexeme_hash_to_descriptor", 2);
            pDbHandle->Bind (1, sHash());
            pDbHandle->Bind (2, llDescriptorId);
            pDbHandle->InsertRow();
            pDbHandle->Finalize();
        }
        catch (CException& exc)
        {
            CEString sMsg (exc.szGetDescription());
            CEString sError;
            try
            {
                pDbHandle->GetLastError (sError);
                sMsg += CEString (L", error %d: ");
                sMsg += sError;
            }
            catch (...)
            {
                sMsg = L"Apparent DB error ";
            }
        
            CString csMsg;
            csMsg.Format (sMsg, pDbHandle->iGetLastError());
            ERROR_LOG ((LPCTSTR)csMsg);

            return false;
        }

        return true;

    }   //  bool bSaveToDb (...)


};  //  struct StLexemeHasher
