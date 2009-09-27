#pragma once

#include "stdafx.h"

class CT_Exception
{

public:
    enum ET_Exception;

protected:
    int i_ErrorCode_;
    std::wstring str_Description_;

public:
//
// C-tors and d-tors
//
public:
    CT_Exception() 
    {
        i_ErrorCode_ = -1;
    };

    CT_Exception (int i_errorCode, const std::wstring& str_description)
        : i_ErrorCode_ (i_errorCode), str_Description_ (str_Description_)
    {}

    virtual ~CT_Exception() {};

public:
    int i_GetErrorCode()
    {
        return i_ErrorCode_;
    }
    wstring str_GetDescription()
    {
        return str_Description_;
    }

};
