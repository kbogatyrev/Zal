// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "ConversionLib_i.h"
#include "dllmain.h"

CConversionLibModule _AtlModule;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}
