#include "StdAfx.h"
#include "Globals.h"
#include "ExtString.h"
#include "ZalEntryParser.h"

using namespace std::tr1;

bool CT_ZalEntryParser::b_CheckSharp (wstring& str_descriptor, ST_Descriptor& st_entry)
{
    if (str_descriptor.length() <= 0)
    {
        return true;
    }
    wsmatch result;
    bool b_match = regex_match(str_descriptor, result, (const wregex)L"^([^#]*?),?\\s*\\#\\s*(\\d+)\\s*(.*)$");
    if (b_match == false && st_entry.i_Section <= 0)
    {
        st_entry.i_Section = 0;
        return true;
    }
    wstring str_number = result[2];
    if (str_number.length() == 0)
    {
        //ERROR_LOG(L"CheckSharp was started on " + str_descriptor + L" and failed to recognize the number.");
        return false;
    }
    st_entry.i_Section = _wtoi(str_number.c_str());
    //ERROR_LOG(L"CheckSharp was started on " + str_descriptor + L"; result is " + (wstring)result[1]);
    wstring str_rest = (wstring)result[3];
    if (str_rest.length() > 0)
    {
        str_descriptor = (wstring)result[1] + L" " + str_rest;
    }
    else
    {
        str_descriptor = (wstring)result[1];
    }
    return true;
}

bool CT_ZalEntryParser::b_CheckTriangle (wstring& str_descriptor, ST_Descriptor& st_entry)
{
    if (str_descriptor.length() <= 0)
    {
        return true;
    }
    wstring str_comment;
    wstring str_rest, str_left, str_middle, str_right, str_stem, str_end1, str_end2, str_nom, str_gen, str_dat,
        str_case1, str_case2, str_tense, str_1sg, str_1pl, str_2sg, str_2pl, str_3sg, str_3pl, str_num, str_wordform, str_wordform_slash,
        str_refl, str_m, str_f, str_n, str_pl, str_1sg_short, str_participle, str_hyphen, str_imper, str_tmp;
    wsmatch result;
    // @ comment can be followed by $, #, [, or %
    bool b_match = regex_match(str_descriptor, result, (const wregex)L"^(.*?)\\s*@\\s*([^\\s\\$\\#\\[\\%][^\\$\\#\\[\\%]+[^\\s])\\s*(([\\$\\#\\[\\%].*)?)$");
    if (b_match == false)
    {
        return true;
    }

    str_comment = result[2];
    str_rest = (wstring)result[1] + (wstring)result[3];
    //ERROR_LOG(L"CheckTriangle was started on " + str_comment + L"; str_rest = " + str_rest);

    // _сравн._ бойч<ее//б<ойче  --> _сравн._ бойч<ее, _сравн._ б<ойче
    b_DetalizeDoubleSlashes(str_comment);

    // _мн._ бр<ат/ья, -ьев, -ьям --> _мн._ бр<атья, бр<атьев, бр<атьям
    // OR
    // _буд._ возд<а/м, -шь, -ст, -д<им, -д<ите, -д<ут ->
    // -> _буд._ возд<ам, возд<ашь, возд<аст, воздад<им, воздад<ите, воздад<ут
    b_DetalizeSlashes(str_comment);
    b_DeleteDoubleStress(str_comment);

    if (map_Pos[st_entry.str_MainSymbol] == POS_NOUN)
    {
        // Replace strings like "_мн._ к<уры, кур, к<урам" with the full paradigm
        b_match = regex_match(str_comment, result, (const wregex)L"(.*)_мн\\._\\s*(\\(_затрудн._\\))?\\s*([<>\\w]+),\\s*([<>\\w]*),\\s*([<>\\w]*)(.*)");
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

        // Finally, if the string is OK, we put it to str_IrregularForms.
        b_match = regex_match(str_comment, (const wregex)L"(_(И|Р|Д|В|Т|П)\\.\\s*(ед|мн)\\._\\s*[<>\\w][<>\\-\\w]*([,;]\\s*|$))*");
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

        // Replace _В.=И._
        // TODO: Add the stress to the accusative form
        b_match = regex_match(str_comment, result, (const wregex)L"(.*)_В\\._?\\s*=\\s*_?И\\._(.*)");
        if (b_match == true)
        {
            str_comment = (wstring)result[1] + L"_В._ " + st_Head.str_Source + (wstring)result[2];
            //ERROR_LOG (L"--> " + str_comment);
        }

        // Finally, if the string is OK, we put it to str_IrregularForms.
        b_match = regex_match(str_comment, (const wregex)L"(_(И\\.|Р\\.|Д\\.|(В\\.(\\s*(одуш|неод)\\.)?)|Т\\.|П\\.)(\\s*(ед\\.|мн\\.))?_\\s*[<>\\-\\w]+([;,]\\s*|$))*");
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
        // Replace strings like "_кф_ нес<олон, несолон<а, нес<олоно, 
        // нес<олоны" with the detailed description
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

        // Finally, if the string is OK, we put it to str_IrregularForms.
        if (regex_match(str_comment, (const wregex)L"нет_"))
        {
            b_match = b_CheckDifficulties(str_comment, st_entry);
            if (!b_match)
            {
                return false;
            }
        }
        b_match = regex_match(str_comment, (const wregex)L"(_(сравн\\.|кф\\s*(мн\\.|м|ж|с))_\\s*[<>\\-\\w]+[;,]\\s*)*(_(сравн\\.|кф\\s*(мн\\.|м|ж|с))_\\s*[<>\\-\\w]+)");
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
        // _буд._ возд<ам, возд<ашь, возд<аст, воздад<им, воздад<ите, воздад<ут;
        b_match = regex_match(str_comment, result, (const wregex)L"(.*_)((буд\\.)|(наст\\.))_\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+)(.*)");
        if (b_match == true)
        {
            str_left = result[1];
            str_tense = result[2];
            str_1sg = result[5];
            str_2sg = result[6];
            str_3sg = result[7];
            str_1pl = result[8];
            str_2pl = result[9];
            str_3pl = result[10];
            str_right = result[11];
            str_comment = str_left + str_tense + L" 1 ед._ " + str_1sg + L", _" + 
                str_tense + L" 2 ед._ " + str_2sg + L", _" + 
                str_tense + L" 3 ед._ " + str_3sg + L", _" + 
                str_tense + L" 1 мн._ " + str_1pl + L", _" +  
                str_tense + L" 2 мн._ " + str_2pl + L", _" +
                str_tense + L" 3 мн._ " + str_3pl + str_right;
            //ERROR_LOG (L"--> " + str_comment);
        }

        // Expand
        // "_буд._ вгон<ю, вг<онит, вг<онят" OR "_буд._ взрев<у, взрев<ёт"
        b_match = regex_match(str_comment, result, (const wregex)L"(.*_)(буд\\.|наст\\.)_\\s*([<>\\w]+(у|ю)((сь)?)),\\s*([<>\\w]+)((,\\s*([<>\\w]+))?)(.*)");
        if (b_match == true)
        {
            str_left = result[1];
            str_tense = result[2];
            str_1sg = result[3];
            str_1sg_short = result[4];
            str_refl = result[5];
            str_3sg = result[7];
            wstring str_3pl_group = result[8];
            if (str_3pl_group.length() > 0)
            {
                str_3pl = result[10];
            }
            str_right = result[11];
            if (str_refl.length() > 0)
            {
                str_comment = str_left + str_tense + L" 1 ед._ " + str_1sg + L", _" + 
                    str_tense + L" 2 ед._ " + str_3sg.substr(0, str_3sg.length() - 3) + L"шься, _" + 
                    str_tense + L" 3 ед._ " + str_3sg + L", _" + 
                    str_tense + L" 1 мн._ " + str_3sg.substr(0, str_3sg.length() - 3) + L"мся, _" + 
                    str_tense + L" 2 мн._ " + str_3sg.substr(0, str_3sg.length() - 2) + L"есь, _" + str_tense + L" 3 мн._ ";
                if (str_3pl_group.length() > 0)
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
                if (str_3pl_group.length() > 0)
                {
                    str_comment += str_3pl;
                }
                else
                {
                    str_comment += str_3sg.substr(0, str_3sg.length() - 2) + str_1sg_short + L"т";
                }
            }
            str_comment += str_right;
            //ERROR_LOG (L"** CheckTriangle: FUTURE --> " + str_comment);
        }

        // Expand "_прош._ возд<ал, воздал<а, возд<ало, возд<али"
        b_match = regex_match(str_comment, result, (const wregex)L"(.*_)прош\\._\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+)(.*)");
        if (b_match == true)
        {
            str_left = result[1];
            str_m = result[2];
            str_f = result[3];
            str_n = result[4];
            str_pl = result[5];
            str_right = result[6];
            str_comment = str_left + L"прош. м_ " + str_m + L", _прош. ж_ " +
                str_f + L", _прош. с_ " + str_n + L", _прош. мн._ " + str_pl +
                str_right;
            //ERROR_LOG (L"--> " + str_comment);
        }

        // Expand "_прош._ в<ырос, в<ыросла, в<ыросший"
        // OR
        // "_прош._ взрев<ел, взрев<ела"
        b_match = regex_match(str_comment, result, (const wregex)L"(.*_)прош\\._\\s*([<>\\w]+),\\s*([<>\\w]+)((,\\s*([<>\\w]+ий(ся)?))?)(.*)");
        if (b_match == true)
        {
            str_left = result[1];
            str_m = result[2];
            str_f = result[3];
            wstring str_participle_group = result[4];
            str_participle = result[6];
            str_right = result[8];
            if (str_m.substr(str_m.length() - 2, 2) == L"ся")
            {
                str_n = str_f.substr(0, str_f.length() - 3) + L"ось";
                str_pl = str_f.substr(0, str_f.length() - 3) + L"ись";
            }
            else
            {
                str_n = str_f.substr(0, str_f.length() - 1) + L"о";
                str_pl = str_f.substr(0, str_f.length() - 1) + L"и";
            }
            str_comment = str_left + L"прош. м_ " + str_m + L", _прош. ж_ " +
                str_f + L", _прош. с_ " + str_n + L", _прош. мн._ " + str_pl;
            if (str_participle_group.length() > 0)
            {
                str_comment += L", _прич. прош._ " + str_participle + str_right;
            }
            //ERROR_LOG (L"--> " + str_comment);
        }
        //     _наст. 2 ед._ вопи<ешь, _3 ед._ -<ет --> 
        // --> _наст. 2 ед._ вопи<ешь, _3 ед._ вопи<ет
        b_match = regex_search(str_comment, (const wregex)L"\\w\\/[<>\\w].*\\s\\-[<>\\w]");
        if (b_match)
        {
            b_DetalizeSlashes(str_comment);
            b_DeleteDoubleStress(str_comment);
        }
        //     _наст. 2 ед._ вопи<ешь, _3 ед._ вопи<ет --> 
        // --> _наст. 2 ед._ вопи<ешь, _наст. 3 ед._ вопи<ет
        b_DetalizeCategories(str_comment);

        //     _прич. страд._ сд<анный (_кф_ сдан, сдан<а, сдан<о, сдан<ы) --> 
        // --> _прич. страд._ сд<анный, _прич. страд. кф м_ сдан, ...
        b_match = regex_match(str_comment, result, (const wregex)L"(.*?_(прич\\.\\s*(прош|страд|наст)\\.)_\\s*[<>\\w]+)[,;]?\\s*\\(_кф_\\s*([<>\\-\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+)\\)(.*)");
        if (b_match == true)
        {
            str_left = result[1];
            str_participle = result[2];
            str_m = result[4];
            str_f = result[5];
            str_n = result[6];
            str_pl = result[7];
            str_right = result[8];
            str_comment = str_left + L", _" + str_participle + L" кф м_ " + str_m +
                L", _" + str_participle + L" кф ж_ " + str_f + L", _" +
                str_participle + L" кф с_ " + str_n + L", _" +
                str_participle + L" кф мн._ " + str_pl + str_right;
            //ERROR_LOG (L"partcp --> " + str_comment);
        }

        // Finally, if the string is OK, we put it to str_IrregularForms.
        if (regex_match(str_comment, (const wregex)L"нет_"))
        {
            b_match = b_CheckDifficulties(str_comment, st_entry);
            if (!b_match)
            {
                return false;
            }
        }
        //ERROR_LOG (L"**CheckTriangle: Check correctness --> " + str_comment);
        b_match = regex_match(str_comment, (const wregex)L"(_(прич\\.\\s*(страд\\.(\\s*(прош|наст)\\.)?|прош\\.|наст\\.)(\\s*кф\\s*(мн\\.|ж|м|с))?(\\s*ед\\.)?|деепр\\.(\\s*(прош|наст)\\.)|повел\\.|прош\\.\\s*(мн\\.|м|ж|с)(\\s*ед\\.)?|(наст|буд)\\.\\s*[123]\\s*(ед|мн)\\.)_\\s*[<>\\w][<>\\-\\w]*([,;]\\s*|$))*");
        if (b_match == true)
        {
            st_entry.str_IrregularForms = str_comment;
            //ERROR_LOG (L"**CheckTriangle: OK --> " + str_comment);
            str_descriptor = str_rest;
            return true;
        }
    }

    if (1 == 1) //Otherwise ERROR_LOG won't work
    {
        ERROR_LOG (L"CheckTriangle couldn't recognize the text: " + str_comment);
    }
    return false;
}

bool CT_ZalEntryParser::b_DetalizeDoubleSlashes(wstring& str_source)
{
    // _сравн._ (_затрудн._) г<ульче//г<улче
    bool b_match = false;
    wsmatch result;
    wstring str_stem = L"", str_right = L"", str_left = L"";
    b_match = regex_match(str_source, result, 
        (const wregex)L"^(.*?)(_[^_]*_(\\s*\\(_затрудн\\.?_\\))?)\\s*([^\\s\\/]*)\\s*\\/\\/\\s*([^\\s\\/]*.*)$");
    if (b_match)
    {
        //ERROR_LOG(L"****b_DetalizeDoubleSlashes - begin: " + str_source);
        str_source = (wstring)result[1] +
            (wstring)result[2] + L" " + (wstring)result[4] + L", " +
            (wstring)result[2] + L" " + (wstring)result[5];
    }
    //ERROR_LOG(L"****b_DetalizeDoubleSlashes: " + str_source);
    return true;
}

bool CT_ZalEntryParser::b_DetalizeSlashes(wstring& str_source)
{
    // _мн._ бр<ат/ья, -ьев, -ьям --> _мн._ бр<ат--ья, бр<ат--ьев, бр<ат--ьям
    wsmatch result;
    wstring str_stem = L"", str_right = L"", str_left = L"", str_end1 = L"", str_end2 = L"", str_end3 = L"";

    // 1. (наст.|буд.)
    if (regex_match(str_source, result, 
        (const wregex)L"(.*?(_наст\\._|_буд\\._)\\s*)([\\w<>]+?)(у|ю|усь|юсь),\\s*\\-([\\w<>]+)(,\\s*\\-([\\w<>]+))?(.*)$"))
    {
        str_left = (wstring)result[1];
        str_right = (wstring)result[8];
        str_stem = (wstring)result[3];
        str_end1 = (wstring)result[4];
        str_end2 = (wstring)result[5];
        str_end3 = (wstring)result[7];
        str_source = str_left + str_stem + str_end1 + L", " + str_stem + L"--" + str_end2;
        if (str_end3.length() > 0)
        {
            //ERROR_LOG(L"****b_DetalizeSlashes: str_end3.length() > 0: " + str_source);
            str_source += L", " + str_stem + L"--" + str_end3;
        }
        str_source += str_right;
        //ERROR_LOG(L"****b_DetalizeSlashes: " + str_source + L"; str_end3 = " + str_end3);

        b_DeleteDoubleStress(str_source);
        b_DetalizeSlashes(str_source); // in case there are several such substrings
        return true;
    }
    // 2. With a slash: "_буд._ взъ<е/мся, -шься"
    else if (regex_match(str_source, result, 
        (const wregex)L"(.*?)([\\w<>]+)\\/([\\w<>]+)(.*)"))
    {
        str_left = (wstring)result[1] + (wstring)result[2] + L"--" + (wstring)result[3];
        str_right = (wstring)result[4];
        str_stem = (wstring)result[2];
        while (regex_match(str_right, result, 
            (const wregex)L"([^\\-\\/]*)\\-([\\w<>]+)(.*)"))
        {
            //ERROR_LOG(str_left + str_right);
            str_left += (wstring)result[1] + str_stem + L"--" + (wstring)result[2];
            str_right = (wstring)result[3];
            if (regex_search(str_right, (const wregex)L"^[^\\-]*\\s[<>\\w]+[,;]?\\s"))
            {
                // _буд._ зад<а/м, -шь, <...>; _повел._ зад<ай; _прош._ з<адал, -<а, -о, -и
                break;
            }
        }
        str_source = str_left + str_right;
        //ERROR_LOG(L"****b_DetalizeSlashes: " + str_source);

        b_DeleteDoubleStress(str_source);
        b_DetalizeSlashes(str_source); // in case there are several such substrings
        return true;
    }
    // 3. Without a slash: "_прош._ был, -<а, -о, -и" OR "кф ..."
    else if (regex_match(str_source, result, 
            (const wregex)L"(.*?_[^_\\s]*_)\\s*([\\w<>]+),\\s*\\-([\\w<>]+)(.*)"))
    {
        str_stem = (wstring)result[2];
        str_left = (wstring)result[1] + L" " + str_stem + L", " +
            str_stem + L"--" + (wstring)result[3];
        str_right = (wstring)result[4];
        while (regex_match(str_right, result, 
            (const wregex)L",\\s*\\-([\\w<>]+)(.*)"))
        {
            str_left += L", " + str_stem + L"--" +(wstring)result[1];
            str_right = (wstring)result[2];
        }
        str_source = str_left + str_right;
        //ERROR_LOG(L"****b_DetalizeSlashes: " + str_source);

        b_DeleteDoubleStress(str_source);
        b_DetalizeSlashes(str_source); // in case there are several such substrings
        return true;
    }
    return false;
}

bool CT_ZalEntryParser::b_DetalizeCategories (wstring& str_source)
{
    //     _наст. 2 ед._ вопи<ешь, _3 ед._ вопи<ет --> 
    // --> _наст. 2 ед._ вопи<ешь, _наст. 3 ед._ вопи<ет
    bool b_match = false;
    wsmatch result;
    wstring str_category = L"", str_right = L"", str_left = L"";
    b_match = regex_match(str_source, result, 
        (const wregex)L"(.*_)((наст)|(буд)\\.)(\\s*[123]\\s*((ед)|(мн))\\._[^_]*_)\\s*([123]\\s*((ед)|(мн))\\._)(.*)");
    if (b_match)
    {
        str_category = (wstring)result[2];
        str_left = (wstring)result[1] + str_category + (wstring)result[5] + str_category + L" " + (wstring)result[9];
        str_right = (wstring)result[13];
        while (regex_match(str_right, result, 
            (const wregex)L"([^_]*_)\\s*([123]\\s*((ед)|(мн))\\._)(.*)"))
        {
            //ERROR_LOG(str_left + str_right);
            str_left += (wstring)result[1] + str_category + L" " + (wstring)result[2];
            str_right = (wstring)result[6];
        }
        str_source = str_left + str_right;
        //ERROR_LOG(str_source);
        return true;
    }
    return false;
}

bool CT_ZalEntryParser::b_DeleteDoubleStress (wstring& str_source)
{
    bool b_changes_made = false;
    wsmatch ct_result;
    // 1. невел<ик--<о --> невелик<о
    while (regex_match(str_source, ct_result, (const wregex)L"(.*)<(\\w*)\\-\\-(\\w*<)(.*)"))
    {
        str_source = (wstring)ct_result[1] + (wstring)ct_result[2] + (wstring)ct_result[3] 
            + (wstring)ct_result[4];
        b_changes_made = true;
    }
    // 2. был--о --> б<ыло
    while (regex_match(str_source, ct_result, (const wregex)L"(.*?\\s)([бвгджзйклмнпрстфхцчшщъь]*)([аеёиоуыэюя][^\\-\\s<>]*)\\-\\-([^<]*[,;$].*)"))
    {
        str_source = (wstring)ct_result[1] + (wstring)ct_result[2] + L"<" + (wstring)ct_result[3] + (wstring)ct_result[4];
        b_changes_made = true;
    }
    // 3. был--<а --> был<а
    while (regex_match(str_source, ct_result, (const wregex)L"(.*?)\\-\\-(.*)"))
    {
        str_source = (wstring)ct_result[1] + (wstring)ct_result[2];
        b_changes_made = true;
    }
    return b_changes_made;
}

bool CT_ZalEntryParser::b_CheckColon (wstring& str_descriptor, ST_Descriptor& st_entry)
{
    if (str_descriptor.length() <= 0)
    {
        return true;
    }
    //ERROR_LOG(L"CheckColon: " + str_descriptor);
    bool b_match = false;
    wsmatch ct_result;

    // Looks for: ...:...
    // Except: (...:...)
    b_match = regex_search(str_descriptor, (const wregex)L"^[^\\(]*(\\([^\\)]*\\)[^\\(]*)*\\:");
    if (b_match == false)
    {
        return true;
    }

    // Handles:
    // ... :{...} ...
    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"^(.*?),?\\s*\\:\\s*\\{([^\\}]*)\\}\\s*(.*)$");
    if (b_match == true)
    {
        //ERROR_LOG(L"CheckColon, :(): " + str_descriptor);
        st_entry.str_Contexts = (wstring)ct_result[2];
        str_descriptor = (wstring)ct_result[1] + (wstring)ct_result[3];
        return true;
    }
    // ... : ... [;|%|#|$ ...]
    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"^([^\\(]*(\\([^\\)]*\\)[^\\(]*?)*),?\\s*\\:\\s*([^;%#\\$\\{\\}]*)(.*)$");
    if (b_match == true)
    {
        st_entry.str_Contexts = (wstring)ct_result[3];
        str_descriptor = (wstring)ct_result[1] + (wstring)ct_result[4];
        return true;
    }

    ERROR_LOG(L"CheckColon was started on " + str_descriptor + L" and couldn't recognize the string.");
    return false;
}

bool CT_ZalEntryParser::b_CheckPercent (wstring& str_descriptor, ST_Descriptor& st_entry)
{
    if (str_descriptor.length() <= 0)
    {
        return true;
    }
    bool b_match = false;
    wsmatch ct_result;
    b_match = regex_search(str_descriptor, (const wregex)L"\\%");
    //ERROR_LOG(L"CheckPercent: " + str_descriptor);
    if (b_match == false)
    {
        return true;
    }
    b_match = regex_match(str_descriptor, (const wregex)L"^([^\\%]*?),?\\s*\\%\\{([^\\}]*)\\}(.*)$");
    if (b_match == true)
    {
        //ERROR_LOG(L"CheckPercent, %(): " + str_descriptor);
        st_entry.str_RestrictedForms = (wstring)ct_result[2];
        str_descriptor = (wstring)ct_result[1] + (wstring)ct_result[3];
        return true;
    }
    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"^([^\\%]*?),?\\s*\\%\\s*([^#;\\:\\$]*)(.*)$");
    if (b_match == true)
    {
        st_entry.str_RestrictedForms = (wstring)ct_result[2];
        //ERROR_LOG(L"CheckPercent was started on " + str_descriptor + L", result is " + st_entry.str_RestrictedForms);
        str_descriptor = (wstring)ct_result[1] + (wstring)ct_result[3];
        return true;
    }
    ERROR_LOG(L"CheckPercent was started on " + str_descriptor + L" and couldn't recognize the string.");
    return false; // If there are parentheses, slashes, or something
}

bool CT_ZalEntryParser::b_CheckDifficulties (wstring& str_descriptor, ST_Descriptor& st_entry)
{
    // Handles cases:
    // _Р. мн. затрудн._
    // _пф нет_
    // etc.
    if (str_descriptor.length() <= 0)
    {
        return true;
    }
    bool b_match = false;
    wsmatch ct_result;
    wstring str_left, str_right, str_difficult, str_param1, str_param2;
    b_match = regex_search(str_descriptor, (const wregex)L"(затрудн\\.|нет)_");
    if (b_match == false)
    {
        return true;
    }

    // _Р. мн. затрудн._
    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"(.*?),?\\s*_([^_]+?)\\s*затрудн\\._(.*)");
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
            }
            else
            {
                st_entry.str_Difficulties = str_param1 + L", " + str_param2;
                //ERROR_LOG(L"CheckDifficulties was started on " + str_descriptor + L", result is " + st_entry.str_Difficulties);
                str_descriptor = str_left + str_right;
            }
        }
        else
        {
            st_entry.str_Difficulties = str_difficult;
            //ERROR_LOG(L"CheckDifficulties was started on " + str_descriptor + L", result is " + st_entry.str_Difficulties);
            str_descriptor = str_left + str_right;
        }
    }

    // _Р. мн._ ... (_затрудн._)
    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"(.*_([^_]*)_\\s*)\\(_затрудн\\._\\)(.*)");
    if (b_match == true)
    {
        str_left = ct_result[1];
        str_right = ct_result[3];
        st_entry.str_Difficulties = (wstring)ct_result[2];
        //ERROR_LOG(L"CheckDifficulties was started on " + str_descriptor + L", result is " + st_entry.str_Difficulties);
        str_descriptor = str_left + str_right;
    }

    // _пф нет_
    st_entry.str_Deficient = L"";
    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"(.*?)[;,]?\\s*_([^_]+?)\\s*нет_(.*)");
    while (b_match == true)
    {
        str_left = ct_result[1];
        str_right = ct_result[3];
        st_entry.str_Deficient += (wstring)ct_result[2] + L", ";
        //ERROR_LOG(L"CheckDifficulties was started on " + str_descriptor + L", result is " + st_entry.str_Difficulties);
        str_descriptor = str_left + str_right;
        b_match = regex_match(str_descriptor, ct_result, (const wregex)L"(.*?),?\\s*_([^_]+?)\\s*нет_(.*)");
    }

    // Finally, check if we've done everything we had to
    b_match = regex_search(str_descriptor, (const wregex)L"(затрудн\\.|нет)_");
    if (b_match == false)
    {
        return true;
    }
    ERROR_LOG(L"CheckDifficulties was started on " + str_descriptor + L" and couldn't recognize the string.");
    return false;
}

bool CT_ZalEntryParser::b_CheckInflectedParts (wstring& str_descriptor, ST_Descriptor& st_entry)
{
    // Handles cases:
    // ..., INFL=1
    // ..., INFL=2
    if (str_descriptor.length() <= 0)
    {
        return true;
    }
    bool b_match = false;
    wsmatch ct_result;
    wstring str_inflected;
    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"^(.*?),?\\s*INFL=1(.*)$");
    if (b_match == true)
    {
        str_descriptor = (wstring)ct_result[1] + (wstring)ct_result[2];
        st_entry.st_InflectionGroup.i_InflectedParts = 1;
    }
    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"^(.*?),?\\s*INFL=2(.*)$");
    if (b_match == true)
    {
        str_descriptor = (wstring)ct_result[1] + (wstring)ct_result[2];
        st_entry.st_InflectionGroup.i_InflectedParts = 2;
    }
    return true;
}
