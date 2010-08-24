/**
*/

#pragma once

#include "StdAfx.h"

using namespace std;

class CT_ProgressCallback
{
//private:

public:
    CT_ProgressCallback()
    {
    }

    ~CT_ProgressCallback()
    {
    }
    
    virtual int operator()(int i_percentDone) const 
    { 
        return i_percentDone; 
    }

};
