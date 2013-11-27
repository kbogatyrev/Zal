// MainLib.cpp : Implementation of DLL Exports.
#include "stdafx.h"

#include "Enums.h"

#include "Dictionary.h"

extern "C"
{
ET_ReturnCode GetDictionary (IDictionary *& d)
{
    d = new CDictionary;
    if (NULL == d)
    {
        return H_ERROR_POINTER;
    }

    return H_NO_ERROR;
}
}