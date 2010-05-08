#include "StdAfx.h"
#include "Globals.h"
#include "ExtString.h"
#include "ZalEntryParser.h"
//#include "Analysis.h"
//#include "Synthesis.h"

using namespace std::tr1;

bool CT_ZalEntryParser::b_CheckSharp (wstring& str_descriptor, ST_Descriptor& st_entry)
{
    wsmatch result;
    bool b_match = regex_match(str_descriptor, result, (const wregex)L",?\\s*\\#\\s*(\\d+)\\s*(.*)");
    if (b_match == false)
    {
        st_entry.i_Section = 0;
        return true;
    }
    wstring str_number = result[1];
    if (str_number.length() == 0)
    {
        //ERROR_LOG(L"CheckSharp was started on " + str_descriptor + L" and failed to recognize the number.");
        return false;
    }
    st_entry.i_Section = _wtoi(str_number.c_str());
    //ERROR_LOG(L"CheckSharp was started on " + str_descriptor + L"; result is " + (wstring)result[1]);
    str_descriptor = (wstring)result[2];
    return true;
}

bool CT_ZalEntryParser::b_CheckTriangle (wstring& str_descriptor, ST_Descriptor& st_entry)
{
    wstring str_comment;
    wstring str_rest, str_left, str_middle, str_right, str_stem, str_end1, str_end2, str_nom, str_gen, str_dat,
        str_case1, str_case2, str_tense, str_1sg, str_3sg, str_3pl, str_num, str_wordform, str_wordform_slash,
        str_refl, str_m, str_f, str_n, str_pl, str_1sg_short, str_participle, str_hyphen, str_imper, str_tmp;
    wsmatch result;
    // @ comment can be followed by $, #, or %
    bool b_match = regex_match(str_descriptor, result, (const wregex)L"\\s*@\\s*([^\\s\\$\\#\\%][^\\$\\#\\%]+[^\\s])\\s*(([\\$\\#\\%].*)?)");
    if (b_match == false)
    {
        return true;
    }

    str_comment = result[1];
    str_rest = result[2];
    //ERROR_LOG(L"CheckTriangle was started on " + str_comment + L"; str_rest = " + str_rest);

    if (map_Pos[st_entry.str_MainSymbol] == POS_NOUN)
    {
        // Replace strings like "_мн._ д<ет/ки, -ок, -кам" with the full paradigm
        b_match = regex_match(str_comment, result, (const wregex)L"(.*)_мн\\._\\s*(\\(_затрудн._\\))?\\s*([<>\\-\\w]+)/([<>\\w]+),\\s*\\-([<>\\w]*),\\s*\\-([<>\\w]*)(.*)"); //Works if * is greedy
        if (b_match == true)
        {
            str_left = result[1];
            str_stem = (wstring)result[3] + L"--";
            str_nom = result[4];
            str_gen = result[5];
            str_dat = result[6];
            str_right = result[7];
            str_middle = L"_И. мн._ " + str_stem + str_nom + L", _Р. мн._ " 
                + str_stem + str_gen + L", _Д. мн._ " + str_stem + str_dat;
            // Accusative ending depends on animacy
            if (st_entry.str_MainSymbol == L"мо" || st_entry.str_MainSymbol == L"жо" || st_entry.str_MainSymbol == L"мо-жо")
            {
                str_middle += L", _В. мн._ " + str_stem + str_gen + L", _Т. мн._ " 
                    + str_stem + str_dat + L"и" + L", _П. мн._ " + str_stem 
                    + str_dat.substr(0, str_dat.length() - 1) + L"х";
            }
            else
            {
                str_middle += L", _В. мн._ " + str_stem + str_nom + L", _Т. мн._ " 
                    + str_stem + str_dat + L"и" + L", _П. мн._ " + str_stem 
                    + str_dat.substr(0, str_dat.length() - 1) + L"х";
            }
            b_match = b_DeleteDoubleStress(str_middle);
            
            str_comment = str_left + str_middle + str_right;
        }

        // Replace strings like "_мн._ к<уры, кур, к<урам" with the full paradigm
        b_match = regex_match(str_comment, result, (const wregex)L"(.*)_мн\\._\\s*(\\(_затрудн._\\))?\\s*([<>\\w]+),\\s*([<>\\w]*),\\s*([<>\\w]*)(.*)"); //Works if * is greedy
        if (b_match == true)
        {
            str_left = result[1];
            str_nom = result[3];
            str_gen = result[4];
            str_dat = result[5];
            str_right = result[6];
            str_middle = L"_И. мн._ " + str_nom + L", _Р. мн._ " + str_gen + L", _Д. мн._ " + str_dat;
            // Accusative ending depends on animacy
            if (st_entry.str_MainSymbol == L"мо" || st_entry.str_MainSymbol == L"жо" || st_entry.str_MainSymbol == L"мо-жо")
            {
                str_middle += L", _В. мн._ " + str_gen + L", _Т. мн._ " + str_dat + L"и" 
                    + L", _П. мн._ " + str_dat.substr(0, str_dat.length() - 1) + L"х";
            }
            else
            {
                str_middle += L", _В. мн._ " + str_nom + L", _Т. мн._ " + str_dat + L"и" 
                    + L", _П. мн._ " + str_dat.substr(0, str_dat.length() - 1) + L"х";
            }
            
            str_comment = str_left + str_middle + str_right;
        }

        // Replace strings like "_Д., П. ед._ пареми/<и" with the detailed description
        b_match = regex_match(str_comment, result, (const wregex)L"(.*)(И|Р|Д|В|Т|П)\\.,\\s*(И|Р|Д|В|Т|П)\\.\\s*((ед\\.)|(мн\\.))_\\s*([<>\\-\\w]*)(\\/?)([<>\\-\\w]*)(.*)"); //Works if * is greedy
        while (b_match == true)
        {
            str_left = result[1];
            str_case1 = result[2];
            str_case2 = result[3];
            str_num = result[4];
            str_wordform = (wstring)result[7] + (wstring)result[9];
            str_wordform_slash = (wstring)result[7] + (wstring)result[8] + (wstring)result[9];
            str_right = result[10];
            str_comment = str_left + str_case1 + L". " + str_num + L"_ " + str_wordform + L", _" 
                + str_case2 + L". " + str_num + L"_ " + str_wordform_slash + str_right;
            b_match = regex_match(str_comment, result, (const wregex)L"(.*)(И|Р|Д|В|Т|П)\\.,\\s*(И|Р|Д|В|Т|П)\\.\\s*((ед\\.)|(мн\\.))_\\s*([<>\\-\\w]*)(.*)"); //Works if * is greedy
            //ERROR_LOG (L"--> " + str_comment);
        }

        // Replace strings like "_П. ед._ пареми/<и, _Т. ед._ -<ей" with the detailed description
        b_match = regex_match(str_comment, result, (const wregex)L"(.*)(_(И|Р|Д|В|Т|П)\\.\\s*((ед\\.)|(мн\\.))_)\\s*([<>\\-\\w]*)\\/([<>\\-\\w]*),\\s*(_(И|Р|Д|В|Т|П)\\.\\s*((ед\\.)|(мн\\.))_)\\s*\\-([<>\\w]*)(.*)"); //Works if * is greedy
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
                str_middle = str_case1 + L" " + str_stem + L"--" + str_end1 + L", " 
                    + str_case2 + L" " + str_stem + L"/" + str_end2;
                str_comment = str_left + str_middle + str_right;
                b_match = regex_match(str_comment, result, (const wregex)L"(.*)(_(И|Р|Д|В|Т|П)\\.\\s*((ед\\.)|(мн\\.))_)\\s*([<>\\-\\w]*)\\/([<>\\-\\w]*),\\s*(_(И|Р|Д|В|Т|П)\\.\\s*((ед\\.)|(мн\\.))_)\\s*\\-([<>\\w]*)(.*)"); //Works if * is greedy
                //ERROR_LOG (L"--> " + str_comment);
            }
            // The slash was moving rightwards, and now, when it's unneeded, we replace it with "--".
            b_match = regex_match(str_comment, result, (const wregex)L"(.*\\w)\\/([<>\\w].*)");
            if (b_match == true)
            {
                str_comment = (wstring)result[1] + L"--" + (wstring)result[2];
                //ERROR_LOG (L"--> " + str_comment);
            }
            b_match = b_DeleteDoubleStress(str_comment);
        }

        // Finally, if the string is OK, we put it to str_IrregularForms.
        b_match = regex_match(str_comment, (const wregex)L"(_(И|Р|Д|В|Т|П)\\.\\s*((ед\\.)|(мн\\.))_\\s*[<>\\-\\w]+,\\s*)*(_(И|Р|Д|В|Т|П)\\.\\s*((ед\\.)|(мн\\.))_\\s*[<>\\-\\w]+)");
        if (b_match == true)
        {
            st_entry.str_IrregularForms = str_comment;
            //ERROR_LOG (L"--> " + str_comment);
            str_descriptor = str_rest;
            return true;
        }
    }

    if (st_entry.str_MainSymbol == L"мс" || st_entry.str_MainSymbol == L"числ.")
    {
        // Replace strings like "_Р., Д., П._ двен<адцат/и" with the detailed description
        b_match = regex_match(str_comment, result, (const wregex)L"(.*)(И|Р|Д|В|Т|П)\\.,?\\s*(И|Р|Д|В|Т|П)\\._\\s*([<>\\-\\w]*)(\\/?)([<>\\-\\w]*)(.*)"); //Works if * is greedy
        while (b_match == true)
        {
            str_left = result[1];
            str_case1 = result[2];
            str_case2 = result[3];
            str_wordform = (wstring)result[4] + (wstring)result[6];
            str_wordform_slash = (wstring)result[4] + (wstring)result[5] + (wstring)result[6];
            str_right = result[7];
            str_comment = str_left + str_case1 + L"._ " + str_wordform + L", _" 
                + str_case2 + L"._ " + str_wordform_slash + str_right;
            b_match = regex_match(str_comment, result, (const wregex)L"(.*)(И|Р|Д|В|Т|П)\\.,?\\s*(И|Р|Д|В|Т|П)\\._\\s*([<>\\-\\w]*)(\\/?)([<>\\-\\w]*)(.*)"); //Works if * is greedy
            //ERROR_LOG (L"--> " + str_comment);
        }

        // Replace strings like "_П._ двен<адцат/и, _Т._ -ью" with the detailed description
        b_match = regex_match(str_comment, result, (const wregex)L"(.*)(_(И|Р|Д|В|Т|П)\\._)\\s*([<>\\-\\w]*)\\/([<>\\-\\w]*),\\s*(_(И|Р|Д|В|Т|П)\\._)\\s*\\-([<>\\w]*)(.*)"); //Works if * is greedy
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
                str_middle = str_case1 + L" " + str_stem + L"--" + str_end1 + L", " 
                    + str_case2 + L" " + str_stem + L"/" + str_end2;
                str_comment = str_left + str_middle + str_right;
                b_match = regex_match(str_comment, result, (const wregex)L"(.*)(_(И|Р|Д|В|Т|П)\\._)\\s*([<>\\-\\w]*)\\/([<>\\-\\w]*),\\s*(_(И|Р|Д|В|Т|П)\\._)\\s*\\-([<>\\w]*)(.*)"); //Works if * is greedy
                //ERROR_LOG (L"--> " + str_comment);
            }
            // The slash was moving rightwards, and now, when it's unneeded, we replace it with "--".
            b_match = regex_match(str_comment, result, (const wregex)L"(.*\\w)\\/([<>\\w].*)");
            if (b_match == true)
            {
                str_comment = (wstring)result[1] + L"--" + (wstring)result[2];
                //ERROR_LOG (L"--> " + str_comment);
            }
            b_match = b_DeleteDoubleStress(str_comment);
        }
        // Replace _В.=И._
        b_match = regex_match(str_comment, result, (const wregex)L"(.*)_В\\._?\\s*\\=\\s*_?И\\._(.*)");
        if (b_match == true)
        {
            str_comment = (wstring)result[1] + L"_В._ " + st_Head.str_Source + (wstring)result[2];
            //ERROR_LOG (L"--> " + str_comment);
        }

        // Finally, if the string is OK, we put it to str_IrregularForms.
        b_match = regex_match(str_comment, (const wregex)L"(_(И|Р|Д|В|Т|П)\\._\\s*[<>\\-\\w]+,\\s*)*(_(И|Р|Д|В|Т|П)\\._\\s*[<>\\-\\w]+)");
        if (b_match == true)
        {
            st_entry.str_IrregularForms = str_comment;
            //ERROR_LOG (L"--> " + str_comment);
            str_descriptor = str_rest;
            return true;
        }
    }

    if (map_Pos[st_entry.str_MainSymbol] == POS_ADJ)
    {
        // Replace strings like "_кф_ нес<олон, несолон<а, нес<олоно, нес<олоны" with the detailed description
        b_match = regex_match(str_comment, result, (const wregex)L"(.*)_кф_\\s*([<>\\-\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+)(.*)");
        if (b_match == true)
        {
            str_left = result[1];
            str_m = result[2];
            str_f = result[3];
            str_n = result[4];
            str_pl = result[5];
            str_right = result[6];
            str_comment = str_left + L"_кф м_ " + str_m + L", _кф ж_ " + str_f 
                + L", _кф с_ " + str_n + L", _кф мн._ " + str_pl + str_right;
            //ERROR_LOG (L"--> " + str_comment);
        }

        // Replace strings like "_кф_ сч<астлив, -а, -о, -ы" with the detailed description
        b_match = regex_match(str_comment, result, (const wregex)L"(.*)_кф_\\s*([<>\\-\\w]+)\\/?([<>\\-\\w]*),\\s*\\-([<>\\w]+),\\s*\\-([<>\\w]+),\\s*\\-([<>\\w]+)(.*)");
        if (b_match == true)
        {
            str_left = result[1];
            str_stem = (wstring)result[2] + L"--";
            str_m = result[3];
            str_f = result[4];
            str_n = result[5];
            str_pl = result[6];
            str_right = result[7];
            str_middle = L"_кф м_ " + str_stem + str_m + L", _кф ж_ " + str_stem 
                + str_f + L", _кф с_ " + str_stem + str_n + L", _кф мн._ " + str_stem + str_pl;
            b_match = b_DeleteDoubleStress(str_middle);
            str_comment = str_left + str_middle + str_right;
            //ERROR_LOG (L"--> " + str_comment);
        }

        // Finally, if the string is OK, we put it to str_IrregularForms.
        b_match = regex_match(str_comment, (const wregex)L"(_((сравн\\.)|(кф\\s*(м|ж|с|мн\\.)))_\\s*[<>\\-\\w]+,\\s*)*(_((сравн\\.)|(кф\\s*(м|ж|с|мн\\.)))_\\s*[<>\\-\\w]+)");
        if (b_match == true)
        {
            st_entry.str_IrregularForms = str_comment;
            //ERROR_LOG (L"--> " + str_comment);
            str_descriptor = str_rest;
            return true;
        }
    }

    if (map_Pos[st_entry.str_MainSymbol] == POS_VERB)
    {
        // Replace strings like "_буд._ вгон<ю, вг<онит, вг<онят" with the full present|future paradigm
        b_match = regex_match(str_comment, result, (const wregex)L"(.*_)((буд\\.)|(наст\\.))_\\s*([<>\\w]+(у|ю)((сь)?)),\\s*([<>\\w]+)(,\\s*([<>\\w]+))?(,\\s*_повел\\._([<>\\w]+))?(.*)");
        if (b_match == true)
        {
            str_left = result[1];
            str_tense = result[2];
            str_1sg = result[5];
            str_1sg_short = result[6];
            str_refl = result[7];
            str_3sg = result[9];
            str_3pl = result[11];
            str_imper = result[13];
            str_right = result[14];
            if (str_refl.length() > 0)
            {
                str_comment = str_left + str_tense + L" 1 ед._ " + str_1sg + L", _" + 
                    str_tense + L" 2 ед._ " + str_3sg.substr(0, str_3sg.length() - 3) + L"шься, _" + 
                    str_tense + L" 3 ед._ " + str_3sg + L", _" + 
                    str_tense +    L" 1 мн._ " + str_3sg.substr(0, str_3sg.length() - 3) + L"мся, _" + 
                    str_tense + L" 2 мн._ " + str_3sg.substr(0, str_3sg.length() - 2) + L"есь, _" + str_tense + L" 3 мн._ ";
                if (str_3pl.length() > 0)
                {
                    str_comment += str_3pl;
                }
                else
                {
                    str_comment += str_3sg.substr(0, str_3sg.length() - 4) + str_1sg_short + L"тся"; // There are no such verbs with а/я in 3pl
                }
            }
            else
            {
                str_comment = str_left + str_tense + L" 1 ед._ " + str_1sg + L", _" + 
                    str_tense + L" 2 ед._ " + str_3sg.substr(0, str_3sg.length() - 1) + L"шь, _" + 
                    str_tense + L" 3 ед._ " + str_3sg + L", _" + str_tense + L" 1 мн._ " + str_3sg.substr(0, str_3sg.length() - 1) + L"м, _" + 
                    str_tense + L" 2 мн._ " + str_3sg + L"е, _" + str_tense + L" 3 мн._ ";
                if (str_3pl.length() > 0)
                {
                    str_comment += str_3pl;
                }
                else
                {
                    str_comment += str_3sg.substr(0, str_3sg.length() - 2) + str_1sg_short + L"т";
                }
            }
            if (str_imper.length() > 0)
            {
                str_comment += L", _повел._ " + str_imper;
            }

            str_comment += str_right;
            //ERROR_LOG (L"--> " + str_comment);
        }
        // Replace strings like "_наст._ бер<у, -<ёт" with the full present|future paradigm
        b_match = regex_match(str_comment, result, (const wregex)L"(.*_)((буд\\.)|(наст\\.))_\\s*([<>\\w]+[\\w])\\/?([<\\w]*?(у|ю)((сь)?)),\\s*\\-([<>\\w]*т((ся)?))(,\\s*\\-([<>\\w]*т)((ся)?))?(,\\s*_повел\\._\\s*(\\-?)([<>\\w]+))?(.*)");
        if (b_match == true)
        {
            str_left = result[1];
            str_tense = result[2];
            str_stem = (wstring)result[5] + L"--";
            str_1sg = result[6];
            str_1sg_short = result[7];
            str_refl = result[8];
            str_3sg = result[10];
            str_3pl = result[14];
            str_hyphen = result[18];
            str_imper = result[19];
            str_right = result[20];
            if (str_refl.length() > 0)
            {
                //ERROR_LOG(L"----> str_3sg = " + str_3sg);
                str_comment = str_left + str_tense + L" 1 ед._ " + str_stem + str_1sg + L", _" + 
                    str_tense + L" 2 ед._ " + str_stem + str_3sg.substr(0, str_3sg.length() - 3) + L"шься, _" + 
                    str_tense + L" 3 ед._ " + str_stem + str_3sg + L", _" +
                    str_tense + L" 1 мн._ " + str_stem + str_3sg.substr(0, str_3sg.length() - 3) + L"мся, _" + 
                    str_tense + L" 2 мн._ " + str_stem + str_3sg.substr(0, str_3sg.length() - 2) + L"есь, _" +
                    str_tense + L" 3 мн._ " + str_stem;
                if (str_3pl.length() > 0)
                {
                    str_comment += str_3pl + L"ся";
                }
                else
                {
                    str_comment += str_3sg.substr(0, str_3sg.length() - 4) + str_1sg_short + L"тся";
                }
            }
            else
            {
                str_comment = str_left + str_tense + L" 1 ед._ " + str_stem + str_1sg + L", _" + 
                    str_tense + L" 2 ед._ " + str_stem + str_3sg.substr(0, str_3sg.length() - 1) + L"шь, _" + 
                    str_tense + L" 3 ед._ " + str_stem + str_3sg + L", _" +
                    str_tense + L" 1 мн._ " + str_stem + str_3sg.substr(0, str_3sg.length() - 1) + L"м, _" + 
                    str_tense + L" 2 мн._ " + str_stem + str_3sg + L"е, _" +
                    str_tense + L" 3 мн._ " + str_stem;
                if (str_3pl.length() > 0)
                {
                    str_comment += str_3pl;
                }
                else
                {
                    str_comment += str_3sg.substr(0, str_3sg.length() - 2) + str_1sg_short + L"т";
                }
            }
            if (str_imper.length() > 0)
            {
                str_comment += L", _повел._ ";
                if (str_hyphen.length() > 0)
                {
                    str_comment += str_stem;
                }
                str_comment += str_imper;
            }
            b_DeleteDoubleStress(str_comment);
            str_comment += str_right;
            //ERROR_LOG (L"--> " + str_comment);
        }
        // Replace strings like "_прош._ в<ырос, -ла, -ший" with the full past paradigm + active past participle
        b_match = regex_match(str_comment, result, (const wregex)L"(.*_)прош\\._\\s*([<>\\w]+)\\/?([<\\w]*),\\s*\\-([<>\\w]*а)((сь)?)(,\\s*(\\-?)([<>а-яА-Я]+))?(.*)");
        if (b_match == true)
        {
            str_left = result[1];
            str_stem = (wstring)result[2] + L"--";
            str_m = result[3];
            str_f = result[4];
            str_refl = result[5];
            str_hyphen = result[8];
            str_participle = result[9];
            str_right = result[10];
            str_comment = str_left + L"прош. м_ " + str_stem + str_m + L", _прош. ж_ " +
                str_stem + str_f + str_refl + L", _прош. с_ " + str_stem + str_f.substr(0, str_f.length() - 1) +
                L"о" + str_refl + L", _прош. мн._ " + str_stem + str_f.substr(0, str_f.length() - 1) +
                L"и" + str_refl;
            if (str_participle.length() > 0)
            {
                str_comment += L", _прич. прош._ ";
                if (str_hyphen.length() > 0)
                {
                    str_comment += str_stem;
                }
                str_comment += str_participle;
            }
            b_DeleteDoubleStress(str_comment);
            str_comment += str_right;
            //ERROR_LOG (L"--> " + str_comment);
        }
        // Finally, if the string is OK, we put it to str_IrregularForms.
        b_match = regex_match(str_comment, (const wregex)L"(_((прич\\.\\s*(страд\\.|прош\\.))|(деепр\\.)|(повел\\.)|(прош\\.\\s*(м|ж|с|мн\\.))|((наст\\.|буд\\.)\\s*[123]\\s*(ед\\.|мн\\.)))_\\s*[<>\\-\\w]+,\\s*)*(_((прич\\.\\s*(страд\\.|прош\\.))|(деепр\\.)|(повел\\.)|(прош\\.\\s*(м|ж|с|мн\\.))|((наст\\.|буд\\.)\\s*[123]\\s*(ед\\.|мн\\.)))_\\s*[<>\\-\\w]+)");
        if (b_match == true)
        {
            st_entry.str_IrregularForms = str_comment;
            //ERROR_LOG (L"--> " + str_comment);
            str_descriptor = str_rest;
            return true;
        }
    }

    if (1 == 1) //Otherwise ERROR_LOG won't work
    {
        ERROR_LOG (L"CheckTriangle couldn't recognize the text.");
    }
    return false;
}

bool CT_ZalEntryParser::b_DeleteDoubleStress (wstring& str_source)
{
    // невел<ик--<о --> невелик--<о
    bool b_match = false;
    wsmatch ct_result;
    b_match = regex_match(str_source, ct_result, (const wregex)L"(.*)<(\\w*)\\-\\-(\\w*<)(.*)");
    while (b_match == true)
    {
        str_source = (wstring)ct_result[1] + (wstring)ct_result[2] + (wstring)ct_result[3] 
        + (wstring)ct_result[4];
        b_match = regex_match(str_source, ct_result, (const wregex)L"(.*)<(\\w*)\\-\\-(\\w*<)(.*)");
    }
    // невелик--<о --> невелик<о
    b_match = regex_match(str_source, ct_result, (const wregex)L"(.*)\\-\\-(.*)");
    while (b_match == true)
    {
        str_source = (wstring)ct_result[1] + (wstring)ct_result[2];
        b_match = regex_match(str_source, ct_result, (const wregex)L"(.*)\\-\\-(.*)");
    }
    return b_match;
}

bool CT_ZalEntryParser::b_CheckColon (wstring& str_descriptor, ST_Descriptor& st_entry)
{
    bool b_match = false;
    wsmatch ct_result;
    // Look for: ...:...
    // Except: (_...:..._)
    b_match = regex_search(str_descriptor, (const wregex)L"(\\(\\_[^\\_]*\\_\\))*[^\\(]*\\:");
    if (b_match == false)
    {
        return true;
    }
    // Handles cases:
    // : ...            - a single context
    // : ... (...) ...  - a double contexts (in fact, two contexts)
    // : ... (_..._)    - a context with a comment (the comment is deleted)
    // : ... (чего-л.)  - a context with a specification (also deleted)
    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"[,\\s]*\\:\\s*(([\\s\\w\\-<>\\(\\)]+(\\_и\\s*т\\.\\s*(п|д)\\.)?[^\\s])\\s*((чего|кого|чему|кому)\\-л\\.)?\\s*(\\(\\_[\\w\\s\\.\\:\\-]+\\_\\))?)\\s*(([;%#\\$].*)?)");
    if (b_match == true)
    {
        st_entry.str_Contexts = (wstring)ct_result[2]; // Without "кому-л." etc. and comments in parentheses!
        str_descriptor = (wstring)ct_result[9];
        return true;
    }
    /*
    b_match = regex_match(str_descriptor, (const wregex)L"[,\\s]*\\:([^\\(\\)]*\\([^\\(\\)]*\\))*[^\\)]*");
    if (b_match == true)
    {
        ERROR_LOG(L"CheckColon was started on " + str_descriptor + L" and couldn't recognize the string.");
        return false;
    }
    return true;
    */
    ERROR_LOG(L"CheckColon was started on " + str_descriptor + L" and couldn't recognize the string.");
    return false;
}

bool CT_ZalEntryParser::b_CheckPercent (wstring& str_descriptor, ST_Descriptor& st_entry)
{
    bool b_match = false;
    wsmatch ct_result;
    b_match = regex_search(str_descriptor, (const wregex)L"\\%");
    if (b_match == false)
    {
        return true;
    }
    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"[,\\s]*\\%\\s*([<>\\w\\s]*)$");
    if (b_match == true)
    {
        st_entry.str_RestrictedForms = (wstring)ct_result[1];
        //ERROR_LOG(L"CheckPercent was started on " + str_descriptor + L", result is " + st_entry.str_RestrictedForms);
        str_descriptor = L"";
        return true;
    }
    ERROR_LOG(L"CheckPercent was started on " + str_descriptor + L" and couldn't recognize the string.");
    return false; // If there are parentheses, slashes, or something
}

bool CT_ZalEntryParser::b_CheckDifficulties (wstring& str_descriptor, ST_Descriptor& st_entry)
{
    bool b_match = false;
    wsmatch ct_result;
    wstring str_left, str_right, str_difficult, str_param1, str_param2;
    b_match = regex_search(str_descriptor, (const wregex)L"затрудн\\._");
    if (b_match == false)
    {
        return true;
    }

    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"(.*),\\s*_(.+)\\s+затрудн\\._(.*)");
    if (b_match == true)
    {
        str_left = ct_result[1];
        str_right = ct_result[3];
        str_difficult = ct_result[2];
        b_match = regex_match(str_difficult, ct_result, (const wregex)L"(.*)\\sи\\s(.*)");
        if (b_match == true)
        {
            str_param1 = ct_result[1];
            str_param2 = ct_result[2];
            b_match = regex_match(str_param1, ct_result, (const wregex)L"(.*)\\s(м|ж|с|ед\\.|мн\\.)");
            if (b_match == true)
            {
                st_entry.str_Difficulties = (wstring)ct_result[1] + L" " + (wstring)ct_result[2]
                + L", " + (wstring)ct_result[1] + str_param2;
                //ERROR_LOG(L"CheckDifficulties was started on " + str_descriptor + L", result is " + st_entry.str_Difficulties);
                str_descriptor = str_left + str_right;
                return true;
            }
            else
            {
                st_entry.str_Difficulties = str_param1 + L", " + str_param2;
                //ERROR_LOG(L"CheckDifficulties was started on " + str_descriptor + L", result is " + st_entry.str_Difficulties);
                str_descriptor = str_left + str_right;
                return true;
            }
        }
        else
        {
            st_entry.str_Difficulties = str_difficult;
            //ERROR_LOG(L"CheckDifficulties was started on " + str_descriptor + L", result is " + st_entry.str_Difficulties);
            str_descriptor = str_left + str_right;
            return true;
        }
    }

    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"(.*_([^_]*)_\\s*)\\(_затрудн\\._\\)(.*)");
    if (b_match == true)
    {
        str_left = ct_result[1];
        str_right = ct_result[3];
        st_entry.str_Difficulties = (wstring)ct_result[2];
        //ERROR_LOG(L"CheckDifficulties was started on " + str_descriptor + L", result is " + st_entry.str_Difficulties);
        str_descriptor = str_left + str_right;
        return true;
    }

    ERROR_LOG(L"CheckDifficulties was started on " + str_descriptor + L" and couldn't recognize the string.");
    return false;
}
