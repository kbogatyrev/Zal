#include "StdAfx.h"
#include "ExtString.h"
#include "ZalEntryParser.h"

using namespace std::tr1;

bool CT_ZalEntryParser::b_CheckTriangle (wstring& str_descriptor, ST_Descriptor& st_entry)
{
	wstring str_comment;
	wstring str_left, str_middle, str_right, str_stem, str_end1, str_end2, str_nom, str_gen, str_dat,
		str_case1, str_case2, str_tense, str_1sg, str_3sg, str_3pl, str_num, str_wordform, str_wordform_slash,
		str_refl, str_m, str_f, str_n, str_pl, str_1sg_short;
	wsmatch result;
	bool b_match = regex_match(str_descriptor, result, (const wregex)L"@\\s*([^\\s].+)");
	if (b_match == false)
	{
		return true;
	}
	str_comment = result[1];

	ERROR_LOG (str_comment); //Delete this

	if (map_Pos[st_entry.str_MainSymbol] == POS_NOUN)
	{
		// Replace strings like "_��._ �<��/��, -��, -���" with the full paradigm
		b_match = regex_match(str_comment, result, (const wregex)L"(.*)_��\\._\\s*(\\(_�������._\\))?\\s*([<>\\-\\w]+)/([<>\\w]+),\\s*\\-([<>\\w]*),\\s*\\-([<>\\w]*)(.*)"); //Works if * is greedy
		if (b_match == true)
		{
			str_left = result[1];
			str_stem = (wstring)result[3] + L"--";
			str_nom = result[4];
			str_gen = result[5];
			str_dat = result[6];
			str_right = result[7];
			str_middle = L"_�. ��._ " + str_stem + str_nom + L", _�. ��._ " + str_stem + str_gen + L", _�. ��._ " + str_stem + str_dat;
			// Accusative ending depends on animateness
			if (st_entry.str_MainSymbol == L"��" || st_entry.str_MainSymbol == L"��" || st_entry.str_MainSymbol == L"��-��")
			{
				str_middle += L", _�. ��._ " + str_stem + str_gen + L", _�. ��._ " + str_stem + str_dat + L"�" + L", _�. ��._ " + str_stem + str_dat.substr(0, str_dat.length() - 1) + L"�";
			}
			else
			{
				str_middle += L", _�. ��._ " + str_stem + str_nom + L", _�. ��._ " + str_stem + str_dat + L"�" + L", _�. ��._ " + str_stem + str_dat.substr(0, str_dat.length() - 1) + L"�";
			}
			b_match = b_DeleteDoubleStress(str_middle);
			
			str_comment = str_left + str_middle + str_right;
		}

		// Replace strings like "_��._ �<���, ���, �<����" with the full paradigm
		b_match = regex_match(str_comment, result, (const wregex)L"(.*)_��\\._\\s*(\\(_�������._\\))?\\s*([<>\\w]+),\\s*([<>\\w]*),\\s*([<>\\w]*)(.*)"); //Works if * is greedy
		if (b_match == true)
		{
			str_left = result[1];
			str_nom = result[3];
			str_gen = result[4];
			str_dat = result[5];
			str_right = result[6];
			str_middle = L"_�. ��._ " + str_nom + L", _�. ��._ " + str_gen + L", _�. ��._ " + str_dat;
			// Accusative ending depends on animateness
			if (st_entry.str_MainSymbol == L"��" || st_entry.str_MainSymbol == L"��" || st_entry.str_MainSymbol == L"��-��")
			{
				str_middle += L", _�. ��._ " + str_gen + L", _�. ��._ " + str_dat + L"�" + L", _�. ��._ " + str_dat.substr(0, str_dat.length() - 1) + L"�";
			}
			else
			{
				str_middle += L", _�. ��._ " + str_nom + L", _�. ��._ " + str_dat + L"�" + L", _�. ��._ " + str_dat.substr(0, str_dat.length() - 1) + L"�";
			}
			
			str_comment = str_left + str_middle + str_right;
		}

		// Replace strings like "_�., �. ��._ ������/<�" with the detailed description
		b_match = regex_match(str_comment, result, (const wregex)L"(.*)(�|�|�|�|�|�)\\.,\\s*(�|�|�|�|�|�)\\.\\s*((��\\.)|(��\\.))_\\s*([<>\\-\\w]*)(\\/?)([<>\\-\\w]*)(.*)"); //Works if * is greedy
		while (b_match == true)
		{
			str_left = result[1];
			str_case1 = result[2];
			str_case2 = result[3];
			str_num = result[4];
			str_wordform = (wstring)result[7] + (wstring)result[9];
			str_wordform_slash = (wstring)result[7] + (wstring)result[8] + (wstring)result[9];
			str_right = result[10];
			str_comment = str_left + str_case1 + L". " + str_num + L"_ " + str_wordform + L", _" + str_case2 + L". " + str_num + L"_ " + str_wordform_slash + str_right;
			b_match = regex_match(str_comment, result, (const wregex)L"(.*)(�|�|�|�|�|�)\\.,\\s*(�|�|�|�|�|�)\\.\\s*((��\\.)|(��\\.))_\\s*([<>\\-\\w]*)(.*)"); //Works if * is greedy
			ERROR_LOG (str_comment);
		}

		// Replace strings like "_�. ��._ ������/<�, _�. ��._ -<��" with the detailed description
		b_match = regex_match(str_comment, result, (const wregex)L"(.*)(_(�|�|�|�|�|�)\\.\\s*((��\\.)|(��\\.))_)\\s*([<>\\-\\w]*)\\/([<>\\-\\w]*),\\s*(_(�|�|�|�|�|�)\\.\\s*((��\\.)|(��\\.))_)\\s*\\-([<>\\w]*)(.*)"); //Works if * is greedy
		if (b_match == true)
		{
			while (b_match == true)
			{
				str_left = result[1];
				str_case1 = result[2];
				str_case2 = result[9];
				str_stem = result[7];
				str_end1 = result[8];
				str_end2 = result[14];
				str_right = result[15];
				str_middle = str_case1 + L" " + str_stem + L"--" + str_end1 + L", " + str_case2 + L" " + str_stem + L"/" + str_end2;
				str_comment = str_left + str_middle + str_right;
				b_match = regex_match(str_comment, result, (const wregex)L"(.*)(_(�|�|�|�|�|�)\\.\\s*((��\\.)|(��\\.))_)\\s*([<>\\-\\w]*)\\/([<>\\-\\w]*),\\s*(_(�|�|�|�|�|�)\\.\\s*((��\\.)|(��\\.))_)\\s*\\-([<>\\w]*)(.*)"); //Works if * is greedy
				ERROR_LOG (str_comment);
			}
			// The slash was moving rightwards, and now, when it's unneeded, we replace it with "--".
			b_match = regex_match(str_comment, result, (const wregex)L"(.*\\w)\\/([<>\\w].*)");
			if (b_match == true)
			{
				str_comment = (wstring)result[1] + L"--" + (wstring)result[2];
				ERROR_LOG (str_comment);
			}
			b_match = b_DeleteDoubleStress(str_comment);
		}

		// Finally, if the string is OK, we put it to str_Triangle.
		b_match = regex_match(str_comment, (const wregex)L"(_(�|�|�|�|�|�)\\.\\s*((��\\.)|(��\\.))_\\s*[<>\\-\\w]+,\\s*)*(_(�|�|�|�|�|�)\\.\\s*((��\\.)|(��\\.))_\\s*[<>\\-\\w]+)");
		if (b_match == true)
		{
			st_entry.str_Triangle = str_comment;
			ERROR_LOG (str_comment);
			return true;
		}
	}

	if (st_entry.str_MainSymbol == L"��" || st_entry.str_MainSymbol == L"����.")
	{
		// Replace strings like "_�., �., �._ ����<�����/�" with the detailed description
		b_match = regex_match(str_comment, result, (const wregex)L"(.*)(�|�|�|�|�|�)\\.,?\\s*(�|�|�|�|�|�)\\._\\s*([<>\\-\\w]*)(\\/?)([<>\\-\\w]*)(.*)"); //Works if * is greedy
		while (b_match == true)
		{
			str_left = result[1];
			str_case1 = result[2];
			str_case2 = result[3];
			str_wordform = (wstring)result[4] + (wstring)result[6];
			str_wordform_slash = (wstring)result[4] + (wstring)result[5] + (wstring)result[6];
			str_right = result[7];
			str_comment = str_left + str_case1 + L"._ " + str_wordform + L", _" + str_case2 + L"._ " + str_wordform_slash + str_right;
			b_match = regex_match(str_comment, result, (const wregex)L"(.*)(�|�|�|�|�|�)\\.,?\\s*(�|�|�|�|�|�)\\._\\s*([<>\\-\\w]*)(\\/?)([<>\\-\\w]*)(.*)"); //Works if * is greedy
			ERROR_LOG (str_comment);
		}

		// Replace strings like "_�._ ����<�����/�, _�._ -��" with the detailed description
		b_match = regex_match(str_comment, result, (const wregex)L"(.*)(_(�|�|�|�|�|�)\\._)\\s*([<>\\-\\w]*)\\/([<>\\-\\w]*),\\s*(_(�|�|�|�|�|�)\\._)\\s*\\-([<>\\w]*)(.*)"); //Works if * is greedy
		if (b_match == true)
		{
			while (b_match == true)
			{
				str_left = result[1];
				str_case1 = result[2];
				str_case2 = result[6];
				str_stem = result[4];
				str_end1 = result[5];
				str_end2 = result[8];
				str_right = result[9];
				str_middle = str_case1 + L" " + str_stem + L"--" + str_end1 + L", " + str_case2 + L" " + str_stem + L"/" + str_end2;
				str_comment = str_left + str_middle + str_right;
				b_match = regex_match(str_comment, result, (const wregex)L"(.*)(_(�|�|�|�|�|�)\\._)\\s*([<>\\-\\w]*)\\/([<>\\-\\w]*),\\s*(_(�|�|�|�|�|�)\\._)\\s*\\-([<>\\w]*)(.*)"); //Works if * is greedy
				ERROR_LOG (str_comment);
			}
			// The slash was moving rightwards, and now, when it's unneeded, we replace it with "--".
			b_match = regex_match(str_comment, result, (const wregex)L"(.*\\w)\\/([<>\\w].*)");
			if (b_match == true)
			{
				str_comment = (wstring)result[1] + L"--" + (wstring)result[2];
				ERROR_LOG (str_comment);
			}
			b_match = b_DeleteDoubleStress(str_comment);
		}
		// Replace _�.=�._
		b_match = regex_match(str_comment, result, (const wregex)L"(.*)_�\\._?\\s*\\=\\s*_?�\\._(.*)");
		if (b_match == true)
		{
			str_comment = (wstring)result[1] + L"_�._ " + st_Head.str_Source + (wstring)result[2];
			ERROR_LOG (str_comment);
		}

		// Finally, if the string is OK, we put it to str_Triangle.
		b_match = regex_match(str_comment, (const wregex)L"(_(�|�|�|�|�|�)\\._\\s*[<>\\-\\w]+,\\s*)*(_(�|�|�|�|�|�)\\._\\s*[<>\\-\\w]+)");
		if (b_match == true)
		{
			st_entry.str_Triangle = str_comment;
			ERROR_LOG (str_comment);
			return true;
		}
	}

	if (map_Pos[st_entry.str_MainSymbol] == POS_ADJ)
	{
		// Replace strings like "_��_ ���<����, �������<�, ���<�����, ���<�����" with the detailed description
		b_match = regex_match(str_comment, result, (const wregex)L"(.*)_��_\\s*([<>\\-\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+)(.*)");
		if (b_match == true)
		{
			str_left = result[1];
			str_m = result[2];
			str_f = result[3];
			str_n = result[4];
			str_pl = result[5];
			str_right = result[6];
			str_comment = str_left + L"_�� �_ " + str_m + L", _�� �_ " + str_f + L", _�� �_ " + str_n + L", _�� ��._ " + str_pl + str_right;
			ERROR_LOG (str_comment);
		}

		// Replace strings like "_��_ ��<������, -�, -�, -�" with the detailed description
		b_match = regex_match(str_comment, result, (const wregex)L"(.*)_��_\\s*([<>\\-\\w]+)\\/?([<>\\-\\w]*),\\s*\\-([<>\\w]+),\\s*\\-([<>\\w]+),\\s*\\-([<>\\w]+)(.*)");
		if (b_match == true)
		{
			str_left = result[1];
			str_stem = (wstring)result[2] + L"--";
			str_m = result[3];
			str_f = result[4];
			str_n = result[5];
			str_pl = result[6];
			str_right = result[7];
			str_middle = L"_�� �_ " + str_stem + str_m + L", _�� �_ " + str_stem + str_f + L", _�� �_ " + str_stem + str_n + L", _�� ��._ " + str_stem + str_pl;
			b_match = b_DeleteDoubleStress(str_middle);
			str_comment = str_left + str_middle + str_right;
			ERROR_LOG (str_comment);
		}

		// Finally, if the string is OK, we put it to str_Triangle.
		b_match = regex_match(str_comment, (const wregex)L"(_((�����\\.)|(��\\s*(�|�|�|��\\.)))_\\s*[<>\\-\\w]+,\\s*)*(_((�����\\.)|(��\\s*(�|�|�|��\\.)))_\\s*[<>\\-\\w]+)");
		if (b_match == true)
		{
			st_entry.str_Triangle = str_comment;
			ERROR_LOG (str_comment);
			return true;
		}
	}

	if (map_Pos[st_entry.str_MainSymbol] == POS_VERB)
	{
		// Replace strings like "_���._ ����<�, ��<����, ��<����" with the full present|future paradigm
		b_match = regex_match(str_comment, result, (const wregex)L"(.*_)((���\\.)|(����\\.))_\\s*([<>\\w]+(�|�)((��)?)),\\s*([<>\\w]+)(,\\s*([<>\\w]+))?(.*)");
		if (b_match == true)
		{
			str_left = result[1];
			str_tense = result[2];
			str_1sg = result[5];
			str_1sg_short = result[6];
			str_refl = result[7];
			str_3sg = result[9];
			str_3pl = result[11];
			str_right = result[12];
			if (str_refl.length() > 0)
			{
				str_comment = str_left + str_tense + L" 1 ��._ " + str_1sg + L", _" + 
					str_tense + L" 2 ��._ " + str_3sg.substr(0, str_3sg.length() - 1) + L"����, _" + 
					str_tense + L" 3 ��._ " + str_3sg + L", _" + 
					str_tense +	L" 1 ��._ " + str_3sg.substr(0, str_3sg.length() - 1) + L"���, _" + 
					str_tense + L" 2 ��._ " + str_3sg + L"���, _" + str_tense + L" 3 ��._ ";
				if (str_3pl.length() > 0)
				{
					str_comment += str_3pl;
				}
				else
				{
					str_comment += str_3sg.substr(0, str_3sg.length() - 4) + str_1sg_short + L"���"; // There are no such verbs with �/� in 3pl
				}
			}
			else
			{
				str_comment = str_left + str_tense + L" 1 ��._ " + str_1sg + L", _" + 
					str_tense + L" 2 ��._ " + str_3sg.substr(0, str_3sg.length() - 1) + L"��, _" + 
					str_tense + L" 3 ��._ " + str_3sg + L", _" + str_tense + L" 1 ��._ " + str_3sg.substr(0, str_3sg.length() - 1) + L"�, _" + 
					str_tense + L" 2 ��._ " + str_3sg + L"�, _" + str_tense + L" 3 ��._ ";
				if (str_3pl.length() > 0)
				{
					str_comment += str_3pl;
				}
				else
				{
					str_comment += str_3sg.substr(0, str_3sg.length() - 2) + str_1sg_short + L"�";
				}
			}

			str_comment += str_right;
			ERROR_LOG (str_comment);
		}
		// Replace strings like "_����._ ���<�, -<��" with the full present|future paradigm
		b_match = regex_match(str_comment, result, (const wregex)L"(.*_)((���\\.)|(����\\.))_\\s*([<>\\w]+)\\/?([<\\w]*?(�|�)((��)?)),\\s*\\-([<>\\w]*�)((��)?)(,\\s*\\-([<>\\w]*�)((��)?))?(.*)");
		if (b_match == true)
		{
			str_left = result[1];
			str_tense = result[2];
			str_stem = result[5];
			str_1sg = result[6];
			str_1sg_short = result[7];
			str_refl = result[8];
			str_3sg = result[10];
			str_3pl = result[14];
			str_right = result[17];
			if (str_refl.length() > 0)
			{
				str_comment = str_left + str_tense + L" 1 ��._ " + str_stem + str_1sg + L", _" + 
					str_tense + L" 2 ��._ " + str_stem + str_3sg.substr(0, str_3sg.length() - 1) + L"����, _" + 
					str_tense + L" 3 ��._ " + str_stem + str_3sg + L"��, _" +
					str_tense + L" 1 ��._ " + str_stem + str_3sg.substr(0, str_3sg.length() - 1) + L"���, _" + 
					str_tense + L" 2 ��._ " + str_stem + str_3sg + L"���, _" +
					str_tense + L" 3 ��._ " + str_stem;
				if (str_3pl.length() > 0)
				{
					str_comment += str_3pl + L"��";
				}
				else
				{
					str_comment += str_3sg.substr(0, str_3sg.length() - 2) + str_1sg_short + L"���";
				}
			}
			else
			{
				str_comment = str_left + str_tense + L" 1 ��._ " + str_stem + str_1sg + L", _" + 
					str_tense + L" 2 ��._ " + str_stem + str_3sg.substr(0, str_3sg.length() - 1) + L"��, _" + 
					str_tense + L" 3 ��._ " + str_stem + str_3sg + L", _" +
					str_tense + L" 1 ��._ " + str_stem + str_3sg.substr(0, str_3sg.length() - 1) + L"�, _" + 
					str_tense + L" 2 ��._ " + str_stem + str_3sg + L"�, _" +
					str_tense + L" 3 ��._ " + str_stem;
				if (str_3pl.length() > 0)
				{
					str_comment += str_3pl;
				}
				else
				{
					str_comment += str_3sg.substr(0, str_3sg.length() - 2) + str_1sg_short + L"�";
				}
			}
			str_comment += str_right;
			ERROR_LOG (str_comment);
		}
		// Finally, if the string is OK, we put it to str_Triangle.
		b_match = regex_match(str_comment, (const wregex)L"(_((����\\. �����\\.)|(�����\\.)|(�����\\.)|(((����\\.)|(���\\.))\\s*[123]\\s*((��\\.)|(��\\.))))_\\s*[<>\\-\\w]+,\\s*)*(_((����\\. �����\\.)|(�����\\.)|(�����\\.)|(((����\\.)|(���\\.))\\s*[123]\\s*((��\\.)|(��\\.))))_\\s*[<>\\-\\w]+)");
		if (b_match == true)
		{
			st_entry.str_Triangle = str_comment;
			ERROR_LOG (str_comment);
			return true;
		}
	}
	return true;
}

bool CT_ZalEntryParser::b_DeleteDoubleStress (wstring& str_source)
{
	// �����<��--<� --> �������--<�
	bool b_match = false;
	wsmatch ct_result;
	b_match = regex_match(str_source, ct_result, (const wregex)L"(.*)<(\\w*)\\-\\-(\\w*<)(.*)");
	while (b_match == true)
	{
		str_source = (wstring)ct_result[1] + (wstring)ct_result[2] + (wstring)ct_result[3] + (wstring)ct_result[4];
		b_match = regex_match(str_source, ct_result, (const wregex)L"(.*)<(\\w*)\\-\\-(\\w*<)(.*)");
	}
	// �������--<� --> �������<�
	b_match = regex_match(str_source, ct_result, (const wregex)L"(.*)\\-\\-(.*)");
	while (b_match == true)
	{
		str_source = (wstring)ct_result[1] + (wstring)ct_result[2];
		b_match = regex_match(str_source, ct_result, (const wregex)L"(.*)\\-\\-(.*)");
	}
	return b_match;
}
