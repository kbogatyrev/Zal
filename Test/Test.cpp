// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "MainLib_i.h"

int _tmain(int argc, _TCHAR* argv[])
{
    USES_CONVERSION;

    HRESULT h_r = CoInitializeEx (NULL, COINIT_APARTMENTTHREADED);

    {
        CComPtr<IDictionary> sp_dict = NULL;
        h_r = sp_dict.CoCreateInstance (__uuidof (ZalDictionary));

        sp_dict->put_DbPath (L"D:\\Dev\\Zal\\ZalData\\ZalTest.db");
		h_r = sp_dict->GetLexemesByGraphicStem (L"абиссинск");

        long lItems = 0;
        h_r = sp_dict->get_Count (&lItems);

        long l_Id = 0;
        CComVariant sp_disp = NULL;
        h_r = sp_dict->get_Item (1, &sp_disp);
		if (VT_DISPATCH == sp_disp.vt)
		{
			CComQIPtr<ILexeme> sp_lex = sp_disp.pdispVal;
			h_r = sp_lex->get_LexemeId (&l_Id);
			CComBSTR sp_grStem;
			h_r = sp_lex->get_GraphicStem (&sp_grStem);
			h_r = sp_lex->GenerateWordForms();
			long lWordForms = 0;
			h_r = sp_lex->get_Count (&lWordForms);
			for (long lItem = 1; lItem <= lWordForms; ++lItem)
			{
				h_r = sp_lex->get_Item (lItem, &sp_disp);
				CComQIPtr<IWordForm> sp_wf = sp_disp.pdispVal;

				CComBSTR sp_bstrWf;
				h_r = sp_wf->get_Wordform (&sp_bstrWf);
				CComBSTR sp_bstrLemma;
				h_r = sp_wf->get_Lemma (&sp_bstrLemma);
				ET_PartOfSpeech eo_pos = POS_UNDEFINED;
				h_r = sp_wf->get_PartOfSpeech (&eo_pos);
				ET_Case eo_case = CASE_UNDEFINED;
				h_r = sp_wf->get_Case (&eo_case);
				ET_Number eo_num = NUM_UNDEFINED;
				h_r = sp_wf->get_Number (&eo_num);
				ET_Gender eo_gender = GENDER_UNDEFINED;
				h_r = sp_wf->get_Gender (&eo_gender);
				ET_Tense eo_tense = TENSE_UNDEFINED;
				h_r = sp_wf->get_Tense (&eo_tense);
				ET_Person eo_person = PERSON_UNDEFINED;
				h_r = sp_wf->get_Person (&eo_person);
				ET_Voice eo_voice = VOICE_UNDEFINED;
				h_r = sp_wf->get_Voice (&eo_voice);
			}
		}

    //////

        CComPtr<ILexeme> sp_lex;
        h_r = sp_dict->GetLexeme (l_Id, &sp_lex);
        CComBSTR sp_grStem;
		h_r = sp_lex->get_GraphicStem (&sp_grStem);
    }

    CoUninitialize();

	return 0;
}

