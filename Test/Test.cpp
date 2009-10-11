// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MainLib_i.h"

int _tmain(int argc, _TCHAR* argv[])
{
    HRESULT h_r = CoInitializeEx (NULL, COINIT_APARTMENTTHREADED);

    {
        CComPtr<IDictionary> sp_dict = NULL;
        h_r = sp_dict.CoCreateInstance (__uuidof (ZalDictionary));
//        h_r = sp_dict->get_Lexemes (L"божеств");
//        h_r = sp_dict->get_Lexemes (L"берёзк");
//		h_r = sp_dict->get_Lexemes (L"блинок");
		h_r = sp_dict->get_Lexemes (L"боёк");

        CComQIPtr<IZalCollection> sp_coll = sp_dict;
        long lItems = 0;
        h_r = sp_coll->get_Count (&lItems);

        CComVariant sp_disp = NULL;
        h_r = sp_coll->get_Item (1, &sp_disp);
		if (VT_DISPATCH == sp_disp.vt)
		{
			CComQIPtr<ILexeme> sp_lex = sp_disp.pdispVal;
			long lId = 0;
			h_r = sp_lex->get_LexemeId (&lId);
			CComBSTR sp_grStem;
			h_r = sp_lex->get_GraphicStem (&sp_grStem);
			h_r = sp_lex->get_WordForms();
			long lWordForms = 0;
			CComQIPtr<IZalCollection> sp_coll = sp_lex;
			h_r = sp_coll->get_Count (&lWordForms);
			for (long lItem = 1; lItem <= lWordForms; ++lItem)
			{
				CComVariant sp_disp = NULL;
				h_r = sp_coll->get_Item (lItem, &sp_disp);
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
				BOOL ui_shortForm = FALSE;
				h_r = sp_wf->get_IsShortForm (&ui_shortForm);
				ET_Gender eo_gender = GENDER_UNDEFINED;
				h_r = sp_wf->get_Gender (&eo_gender);
				ET_Tense eo_tense = TENSE_UNDEFINED;
				h_r = sp_wf->get_Tense (&eo_tense);
				ET_Person eo_person = PERSON_UNDEFINED;
				h_r = sp_wf->get_Person (&eo_person);
				BOOL ui_isReflexive = FALSE;
				h_r = sp_wf->get_IsReflexive (&ui_isReflexive);
				ET_Voice eo_voice = VOICE_UNDEFINED;
				h_r = sp_wf->get_Voice (&eo_voice);
				ET_SpecialForm eo_specialForm = SPECIAL_UNDEFINED;
				h_r = sp_wf->get_SpecialForm (&eo_specialForm);
int gogo = 0;
			}
		}
    }

    CoUninitialize();

	return 0;
}

