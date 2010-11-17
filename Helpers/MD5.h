#pragma once

#include "StdAfx.h"
#include <wincrypt.h>

using namespace std;

class CT_MD5
{

private:
    HCRYPTPROV h_CryptProv;
    HCRYPTHASH h_Hash;

public:
    CT_MD5()
    {
        h_CryptProv = NULL;
        h_Hash = NULL;
        v_Init();
    }

    ~CT_MD5()
    {
        v_Null();
    }

    void v_Init()
    {
        BOOL ui_ret = CryptAcquireContext (&h_CryptProv, NULL, NULL, PROV_RSA_FULL, 0);
        if (!ui_ret)
        {
            throw CT_Exception (-1, L"CT_Hash::v_Init(): CryptAcquireContext() failed.");
        }

        ui_ret = CryptCreateHash (h_CryptProv, CALG_MD5, 0, 0, &h_Hash);
        if (!ui_ret)
        {
            throw CT_Exception (-1, L"CT_Hash::v_Init(): CryptCreateHash() failed.");
        }

    }   //  v_Init()

    wstring str_Hash (const wstring& str_source)
    {
        BOOL ui_ret = CryptHashData (h_Hash, 
                                     (BYTE *)str_source.c_str(), 
                                     str_source.length()* sizeof(wchar_t), 0);
        if (!ui_ret)
        {
            throw CT_Exception (-1, L"CT_Hash::b_Hash():CryptHashData() failed.");
        }

        BYTE byte_hash[16];
        DWORD dw_length = 16;
        ui_ret = CryptGetHashParam (h_Hash, HP_HASHVAL, byte_hash, &dw_length, 0);
        if (!ui_ret)
        {
            throw CT_Exception (-1, L"CT_Hash::b_Hash(): CryptGetHashParam() failed.");
        }

        wstringstream io_;
        io_.fill ('0');
        for (int i_byte = 0; i_byte < 16; ++i_byte)
        {
            io_.width (2);
	        io_ << hex << byte_hash[i_byte];
        }

        return io_.str();

    }   // str_Hash()

    void v_Null()
    {
        if (h_Hash)
        {
            CryptDestroyHash (h_Hash);
            h_Hash = NULL;
        }
        if (h_CryptProv) 
        {
            CryptReleaseContext (h_CryptProv, 0);
            h_CryptProv = NULL;
        }
    
    }   //  v_Null()

};
