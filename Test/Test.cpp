// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "IDictionary.h"
#include "ILexeme.h"
#include "IWordForm.h"
#include "Callbacks.h"

extern "C"
{
ET_ReturnCode GetDictionary(IDictionary *&);
}

int _tmain(int argc, _TCHAR* argv[])
{
    IDictionary * pD = NULL;
    ET_ReturnCode rc = GetDictionary(pD);

    rc = pD->eSetDbPath(L"C:\\git-repos\\Zal-Windows\\ZalData\\db1.db3");
    rc = pD->eGetLexemesByInitialForm(L"дом");

    ILexeme * pL = NULL;
    rc = pD->eGetFirstLexeme(pL);

    rc = pL->eGenerateParadigm();

    IWordForm * pW = NULL;
    rc = pL->eGetFirstWordForm(pW);

    while (H_NO_ERROR == rc)
    {
        rc = pL->eGetNextWordForm(pW);
        if (H_NO_ERROR == rc)
        {
            CEString sWf = pW->sWordForm();
            int i = 0;
        }
    }

    CProgressCallback progress; 

    rc = pD->eGenerateAllForms(progress);
 
    return 0;
}

