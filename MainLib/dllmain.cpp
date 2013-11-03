// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
//#include "MainLib_i.h"
//#include "dllmain.h"

CMainLibModule _AtlModule;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}
 