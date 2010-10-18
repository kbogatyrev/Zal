#include "StdAfx.h"
#include "Globals.h"
#include "ExtString.h"
#include "ZalEntryParser.h"

using namespace std::tr1;

bool CT_ZalEntryParser::b_CheckSharp (CT_ExtString& xstr_descriptor, ST_Descriptor& st_entry)
{
    if (xstr_descriptor.length() <= 0)
    {
        return true;
    }
//    wsmatch result;
//    bool b_match = regex_match(str_descriptor, result, (const wregex)L"^([^#]*?),?\\s*\\#\\s*(\\d+)\\s*(.*)$");
    bool b_match = xstr_descriptor.b_RegexMatch (L"^([^#]*?),?\\s*\\#\\s*(\\d+)\\s*(.*)$");
    if (!b_match && st_entry.i_Section <= 0)
    {
        st_entry.i_Section = 0;
        return true;
    }

//    wstring str_number = result[2];
    wstring str_number = xstr_descriptor.str_GetRegexMatch (1);
    if (str_number.length() == 0)
    {
        return false;
    }

    st_entry.i_Section = _wtoi(str_number.c_str());
//    wstring str_rest = (wstring)result[3];
//    if (str_rest.length() > 0)
    if (xstr_descriptor.str_GetRegexMatch(2).length() > 0)
    {
//        str_descriptor = (wstring)result[1] + L" " + str_rest;
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch(0) + L" " + xstr_descriptor.str_GetRegexMatch (2);
    }
    else
    {
//        str_descriptor = (wstring)result[1];
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0);
    }
    return true;

}   //  b_CheckSharp (...)

//bool CT_ZalEntryParser::b_CheckTriangle (wstring& str_descriptor, ST_Descriptor& st_entry, bool b_alternative)
bool CT_ZalEntryParser::b_CheckTriangle (CT_ExtString& xstr_descriptor, ST_Descriptor& st_entry, bool b_alternative)
{
    if (xstr_descriptor.length() <= 0)
    {
        return true;
    }
//    wstring str_comment;
//    wstring str_rest, str_left, str_middle, str_right, str_stem, str_end1, str_end2, str_nom, str_gen, str_dat,
//        str_case1, str_case2, str_tense, str_1sg, str_1pl, str_2sg, str_2pl, str_3sg, str_3pl, str_num, str_wordform, str_wordform_slash,
//        str_refl, str_m, str_f, str_n, str_pl, str_1sg_short, str_participle, str_hyphen, str_imper, str_tmp;
//    wsmatch result;
    // @ comment can be followed by $, #, [, or %
//    bool b_match = regex_match(str_descriptor, result, (const wregex)L"^(.*?)\\s*@\\s*([^\\s\\$\\#\\[\\%][^\\$\\#\\[\\%]+[^\\s])\\s*(([\\$\\#\\[\\%].*)?)$");
    bool b_match = xstr_descriptor.b_RegexMatch 
        (L"^(.*?)\\s*@\\s*([^\\s\\$\\#\\[\\%][^\\$\\#\\[\\%]+[^\\s])\\s*(([\\$\\#\\[\\%].*)?)$");
    if (!b_match)
    {
        return true;
    }

//    str_comment = result[2];
//    str_rest = (wstring)result[1] + (wstring)result[3];
    CT_ExtString xstr_comment = xstr_descriptor.str_GetRegexMatch (1);
    wstring str_rest = xstr_descriptor.str_GetRegexMatch (0) + xstr_descriptor.str_GetRegexMatch (2);

    // _сравн._ бойч<ее//б<ойче  --> _сравн._ бойч<ее, _сравн._ б<ойче
    b_DetalizeDoubleSlashes (xstr_comment);

    // _мн._ бр<ат/ья, -ьев, -ьям --> _мн._ бр<атья, бр<атьев, бр<атьям
    // OR
    // _буд._ возд<а/м, -шь, -ст, -д<им, -д<ите, -д<ут ->
    // -> _буд._ возд<ам, возд<ашь, возд<аст, воздад<им, воздад<ите, воздад<ут
    b_DetalizeSlashes (xstr_comment);
    b_DeleteDoubleStress (xstr_comment);

    // _буд. 1 ед._ (_устар._) застон<аю
    b_CheckDifficulties (xstr_comment, st_entry);

    // ALTPAR or POETPAR marks handling
//    b_match = regex_match(str_comment, result, (const wregex)L"^(.*?)[;,]?\\s*ALTPAR\\{([^\\}]+)\\}(.*)$");
    b_match = xstr_comment.b_RegexMatch (L"^(.*?)[;,]?\\s*ALTPAR\\{([^\\}]+)\\}(.*)$");
    if (b_match)
    {
//        wstring str_new_descriptor = L"@" + (wstring)result[2];
//        str_comment = (wstring)result[1] + (wstring)result[3];
        xstr_comment = xstr_comment.str_GetRegexMatch (0) + xstr_comment.str_GetRegexMatch (2);
//        bool b_AltProcessed = b_CheckTriangle(str_new_descriptor, st_entry, true);
        bool b_AltProcessed = b_CheckTriangle (CT_ExtString (L"@" + xstr_comment.str_GetRegexMatch(1)), st_entry, true);
        //ERROR_LOG(L"ALTPAR: " + str_new_descriptor + L";str_comment: " + str_comment);
        if (!b_AltProcessed)
        {
            return false;
        }

//        if (regex_match(str_comment, (const wregex)L"^\\s*$"))
        if (xstr_comment.b_RegexMatch (L"^\\s*$"))
        {
            if (!b_alternative)
            {
                xstr_descriptor = str_rest;
            }
            return true;
        }
    }

    if (map_Pos[st_entry.str_MainSymbol] == POS_NOUN)
    {
        // Replace strings like "_мн._ к<уры, кур, к<урам" with the full paradigm
//        b_match = regex_match(str_comment, result, (const wregex)L"(.*)_мн\\._\\s*(\\(_затрудн._\\))?\\s*([<>\\w]+),\\s*([<>\\w]*),\\s*([<>\\w]*)(.*)");
        b_match = xstr_comment.b_RegexMatch 
            (L"(.*)_мн\\._\\s*(\\(_затрудн._\\))?\\s*([<>\\w]+),\\s*([<>\\w]*),\\s*([<>\\w]*)(.*)");
        if (b_match)
        {
//            wstring str_left = result[1];
//            wstring str_nom = result[3];
//            wstring str_gen = result[4];
//            wstring str_dat = result[5];
//            wstring str_right = result[6];
//            wstring str_middle = L"_И. мн._ " + str_nom + L", _Р. мн._ " + str_gen + L", _Д. мн._ " + str_dat;
            wstring str_left = xstr_comment.str_GetRegexMatch (0);
            wstring str_nom = xstr_comment.str_GetRegexMatch (2);
            wstring str_gen = xstr_comment.str_GetRegexMatch (3);
            wstring str_dat = xstr_comment.str_GetRegexMatch (4);
            wstring str_right = xstr_comment.str_GetRegexMatch (5);
            wstring str_middle = L"_И. мн._ " + str_nom + L", _Р. мн._ " + str_gen + L", _Д. мн._ " + str_dat;

            // Accusative ending depends on animacy
            if (st_entry.str_MainSymbol == L"мо" || 
                st_entry.str_MainSymbol == L"жо" || 
                st_entry.str_MainSymbol == L"мо-жо")
            {
                str_middle += L", _В. мн._ " + str_gen + L", _Т. мн._ " + str_dat + L"и" 
                    + L", _П. мн._ " + str_dat.substr (0, str_dat.length() - 1) + L"х";
            }
            else
            {
                str_middle += L", _В. мн._ " + str_nom + L", _Т. мн._ " + str_dat + L"и" 
                    + L", _П. мн._ " + str_dat.substr (0, str_dat.length() - 1) + L"х";
            }

            xstr_comment = str_left + str_middle + str_right;
        }

        // Replace strings like "_Д., П. ед._ пареми/<и" with the detailed description
//        b_match = regex_match(str_comment, result, (const wregex)L"(.*)(И|Р|Д|В|Т|П)\\.,\\s*(И|Р|Д|В|Т|П)\\.\\s*((ед\\.)|(мн\\.))_\\s*([<>\\-\\w]*)(\\/?)([<>\\-\\w]*)(.*)"); //Works if * is greedy
        wstring str_regexBase 
            (L"(.*)(И|Р|Д|В|Т|П)\\.,\\s*(И|Р|Д|В|Т|П)\\.\\s*((ед\\.)|(мн\\.))_\\s*([<>\\-\\w]*)");
        b_match = xstr_comment.b_RegexMatch (str_regexBase + L"(\\/?)([<>\\-\\w]*)(.*)"); 
        while (b_match)
        {
//            str_left = result[1];
//            str_case1 = result[2];
//            str_case2 = result[3];
//            str_num = result[4];
//            str_wordform = (wstring)result[7] + (wstring)result[9];
//            str_wordform_slash = (wstring)result[7] + (wstring)result[8] + (wstring)result[9];
//            str_right = result[10];
//            str_comment = str_left + str_case1 + L". " + str_num + L"_ " + str_wordform + L", _" 
//                + str_case2 + L". " + str_num + L"_ " + str_wordform_slash + str_right;

            wstring str_left = xstr_comment.str_GetRegexMatch(0);
            wstring str_case1 = xstr_comment.str_GetRegexMatch(1);
            wstring str_case2 = xstr_comment.str_GetRegexMatch(2);
            wstring str_num = xstr_comment.str_GetRegexMatch(3);
            wstring str_wordForm = xstr_comment.str_GetRegexMatch(6) + 
                                   xstr_comment.str_GetRegexMatch(8);
            wstring str_wordFormSlash = xstr_comment.str_GetRegexMatch(6) + 
                                   xstr_comment.str_GetRegexMatch(7) + 
                                   xstr_comment.str_GetRegexMatch(8);
            wstring str_right = xstr_comment.str_GetRegexMatch(9);
            xstr_comment = str_left + str_case1 + L". " + str_num + L"_ " + str_wordForm + L", _" 
                + str_case2 + L". " + str_num + L"_ " + str_wordFormSlash + str_right;

//            b_match = regex_match(str_comment, result, (const wregex)L"(.*)(И|Р|Д|В|Т|П)\\.,\\s*(И|Р|Д|В|Т|П)\\.\\s*((ед\\.)|(мн\\.))_\\s*([<>\\-\\w]*)(.*)"); //Works if * is greedy
            b_match = xstr_comment.b_RegexMatch (str_regexBase + L"(.*)");

        }   //  while (b_match)

        // Finally, if the string is OK, we put it to str_IrregularForms.
//        b_match = regex_match(str_comment, (const wregex)L"(_(И|Р|Д|В|Т|П)\\.\\s*(ед|мн)\\._\\s*[<>\\w][<>\\-\\w]*([,;]\\s*|$))*");
        b_match = xstr_comment.b_RegexMatch (L"(_(И|Р|Д|В|Т|П)\\.\\s*(ед|мн)\\._\\s*[<>\\w][<>\\-\\w]*([,;]\\s*|$))*");
        if (b_match)
        {
            if (b_alternative)
            {
                st_entry.str_AltIrregularForms = xstr_comment;  // slice...
            }
            else
            {
                st_entry.str_IrregularForms = xstr_comment;
                xstr_descriptor = str_rest;
            }
            //ERROR_LOG (L"--> " + str_comment);
            return true;
        }
    }       //  if (map_Pos[st_entry.str_MainSymbol] == POS_NOUN)

    if (st_entry.str_MainSymbol == L"мс" || st_entry.str_MainSymbol == L"числ.")
    {
        // Replace strings like "_Р., Д., П._ двен<адцат/и" with the detailed description
//        b_match = regex_match(str_comment, result, (const wregex)L"(.*)(И|Р|Д|В|Т|П)\\.,?\\s*(И|Р|Д|В|Т|П)\\._\\s*([<>\\-\\w]*)(\\/?)([<>\\-\\w]*)(.*)"); //Works if * is greedy
        wstring str_matcher (L"(.*)(И|Р|Д|В|Т|П)\\.,?\\s*(И|Р|Д|В|Т|П)\\._\\s*([<>\\-\\w]*)(\\/?)([<>\\-\\w]*)(.*)");
//            (L"(.*)(И|Р|Д|В|Т|П)\\.,?\\s*(И|Р|Д|В|Т|П)\\._\\s*([<>\\-\\w]*)(\\/?)([<>\\-\\w]*)(.*)"); 
        while (xstr_comment.b_RegexMatch (str_matcher))
        {
//            str_left = result[1];
//            str_case1 = result[2];
//            str_case2 = result[3];
//            str_wordform = (wstring)result[4] + (wstring)result[6];
//            str_wordform_slash = (wstring)result[4] + (wstring)result[5] + (wstring)result[6];
//            str_right = result[7];
//            str_comment = str_left + str_case1 + L"._ " + str_wordform + L", _" 
//                + str_case2 + L"._ " + str_wordform_slash + str_right;
//            b_match = regex_match(str_comment, result, (const wregex)L"(.*)(И|Р|Д|В|Т|П)\\.,?\\s*(И|Р|Д|В|Т|П)\\._\\s*([<>\\-\\w]*)(\\/?)([<>\\-\\w]*)(.*)"); //Works if * is greedy
            wstring str_left = xstr_comment.str_GetRegexMatch(0);
            wstring str_case1 = xstr_comment.str_GetRegexMatch(1);
            wstring str_case2 = xstr_comment.str_GetRegexMatch(2);
            wstring str_wordForm = xstr_comment.str_GetRegexMatch(3) + xstr_comment.str_GetRegexMatch(5);
            wstring str_wordFormSlash = xstr_comment.str_GetRegexMatch(3) + 
                                        xstr_comment.str_GetRegexMatch(4) + 
                                        xstr_comment.str_GetRegexMatch(5);
            wstring str_right = xstr_comment.str_GetRegexMatch(6);
            xstr_comment = str_left + str_case1 + L"._ " + str_wordForm + L", _" + str_case2 + L"._ " + 
                str_wordFormSlash + str_right;            
        }

        // Replace _В.=И._
        // TODO: Add the stress to the accusative form
//        b_match = regex_match(str_comment, result, (const wregex)L"(.*)_В\\._?\\s*=\\s*_?И\\._(.*)");
        b_match = xstr_comment.b_RegexMatch (L"(.*)_В\\._?\\s*=\\s*_?И\\._(.*)");
        if (b_match)
        {
//            str_comment = (wstring)result[1] + L"_В._ " + st_Head.str_Source + (wstring)result[2];
            xstr_comment = xstr_comment.str_GetRegexMatch (0) + L"_В._ " + st_Head.str_RawSource + xstr_comment.str_GetRegexMatch (1);
            //ERROR_LOG (L"--> " + str_comment);
        }

        // Finally, put the string to str_IrregularForms.
        // (We don't check it because it's easier to check all
        // relevant dictionary entries manually.)
        //b_match = regex_match(str_comment, (const wregex)L"(_(И\\.|Р\\.|Д\\.|(В\\.(\\s*(одуш|неод)\\.)?)|Т\\.|П\\.)(\\s*(ед\\.|мн\\.))?_\\s*[<>\\-\\w]+([;,]\\s*|$))*");
        if (b_alternative)
        {
            st_entry.str_AltIrregularForms = xstr_comment;  // slice
        }
        else
        {
            st_entry.str_IrregularForms = xstr_comment;      // slice
            xstr_descriptor = str_rest;
        }

        return true;

    }   //  if (st_entry.str_MainSymbol == L"мс" || st_entry.str_MainSymbol == L"числ.")

    if (map_Pos[st_entry.str_MainSymbol] == POS_ADJ)
    {
        // Replace strings like "_кф_ нес<олон, несолон<а, нес<олоно, 
        // нес<олоны" with the detailed description
//        b_match = regex_match(str_comment, result, (const wregex)L"(.*)_кф_\\s*([<>\\-\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+)(.*)");
        b_match = xstr_comment.b_RegexMatch (L"(.*)_кф_\\s*([<>\\-\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+)(.*)");
        if (b_match)
        {
//            str_left = result[1];
//            str_m = result[2];
//            str_f = result[3];
//            str_n = result[4];
//            str_pl = result[5];
//            str_right = result[6];
//            str_comment = str_left + L"_кф м_ " + str_m + L", _кф ж_ " + str_f 
//                + L", _кф с_ " + str_n + L", _кф мн._ " + str_pl + str_right;
            wstring str_left = xstr_comment.str_GetRegexMatch (0);
            wstring str_m = xstr_comment.str_GetRegexMatch (1);
            wstring str_f = xstr_comment.str_GetRegexMatch (2);
            wstring str_n = xstr_comment.str_GetRegexMatch (3);
            wstring str_pl = xstr_comment.str_GetRegexMatch (4);
            wstring str_right = xstr_comment.str_GetRegexMatch (5);
            xstr_comment = str_left + L"_кф м_ " + str_m + L", _кф ж_ " + str_f 
                + L", _кф с_ " + str_n + L", _кф мн._ " + str_pl + str_right;
        }

        // Finally, if the string is OK, we put it to str_IrregularForms.
//        if (regex_match(str_comment, (const wregex)L"нет_"))
        if (wstring::npos != xstr_comment.find (L"нет_"))
//        if (regex_match(str_comment, (const wregex)L"нет_"))
        {
            b_match = b_CheckDifficulties (xstr_comment, st_entry);
            if (!b_match)
            {
                return false;
            }
        }
//        b_match = regex_match(str_comment, (const wregex)L"(_(сравн\\.|кф\\s*(мн\\.|м|ж|с))_\\s*[<>\\-\\w]+[;,]\\s*)*(_(сравн\\.|кф\\s*(мн\\.|м|ж|с))_\\s*[<>\\-\\w]+)");
        b_match = xstr_comment.b_RegexMatch 
            (L"(_(сравн\\.|кф\\s*(мн\\.|м|ж|с))_\\s*[<>\\-\\w]+[;,]\\s*)*(_(сравн\\.|кф\\s*(мн\\.|м|ж|с))_\\s*[<>\\-\\w]+)");
        if (b_match)
        {
            if (b_alternative)
            {
                st_entry.str_AltIrregularForms = xstr_comment;  // slice
            }
            else
            {
                st_entry.str_IrregularForms = xstr_comment;     // slice
                xstr_descriptor = str_rest;
            }
            return true;
        }
    }       //  if (map_Pos[st_entry.str_MainSymbol] == POS_ADJ)

    if (map_Pos[st_entry.str_MainSymbol] == POS_VERB)
    {
        // _буд._ возд<ам, возд<ашь, возд<аст, воздад<им, воздад<ите, воздад<ут;
//        b_match = regex_match (str_comment, 
//                               result, 
//                               (const wregex)L"(.*_)((буд\\.)|(наст\\.))_\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+)(.*)");
        b_match = xstr_comment.b_RegexMatch 
            (L"(.*_)((буд\\.)|(наст\\.))_\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+)(.*)");
        if (b_match)
        {
//            str_left = result[1];
//            str_tense = result[2];
//            str_1sg = result[5];
//            str_2sg = result[6];
//            str_3sg = result[7];
//            str_1pl = result[8];
//            str_2pl = result[9];
//            str_3pl = result[10];
//            str_right = result[11];
//            str_comment = str_left + str_tense + L" 1 ед._ " + str_1sg + L", _" + 
//                str_tense + L" 2 ед._ " + str_2sg + L", _" + 
//                str_tense + L" 3 ед._ " + str_3sg + L", _" + 
//                str_tense + L" 1 мн._ " + str_1pl + L", _" +  
//                str_tense + L" 2 мн._ " + str_2pl + L", _" +
//                str_tense + L" 3 мн._ " + str_3pl + str_right;
            wstring str_left = xstr_comment.str_GetRegexMatch (0);
            wstring str_tense = xstr_comment.str_GetRegexMatch (1);
            wstring str_1sg = xstr_comment.str_GetRegexMatch (4);
            wstring str_2sg = xstr_comment.str_GetRegexMatch (5);
            wstring str_3sg = xstr_comment.str_GetRegexMatch (6);
            wstring str_1pl = xstr_comment.str_GetRegexMatch (7);
            wstring str_2pl = xstr_comment.str_GetRegexMatch (8);
            wstring str_3pl = xstr_comment.str_GetRegexMatch (9);
            wstring str_right = xstr_comment.str_GetRegexMatch (10);
            xstr_comment = str_left + str_tense + L" 1 ед._ " + str_1sg + L", _" + 
                str_tense + L" 2 ед._ " + str_2sg + L", _" + 
                str_tense + L" 3 ед._ " + str_3sg + L", _" + 
                str_tense + L" 1 мн._ " + str_1pl + L", _" +  
                str_tense + L" 2 мн._ " + str_2pl + L", _" +
                str_tense + L" 3 мн._ " + str_3pl + str_right;
        }

        // Expand
        // "_буд._ вгон<ю, вг<онит, вг<онят" OR "_буд._ взрев<у, взрев<ёт"
//        b_match = regex_match (str_comment, 
//                               result, 
//                               (const wregex)L"(.*_)(буд\\.|наст\\.)_\\s*([<>\\w]+(у|ю)((сь)?)),\\s*([<>\\w]+)((,\\s*([<>\\w]+))?)(.*)");
        b_match = xstr_comment.b_RegexMatch 
            (L"(.*_)(буд\\.|наст\\.)_\\s*([<>\\w]+(у|ю)((сь)?)),\\s*([<>\\w]+)((,\\s*([<>\\w]+))?)(.*)");
        if (b_match)
        {
//            str_left = result[1];
//            str_tense = result[2];
//            str_1sg = result[3];
//            str_1sg_short = result[4];
//            str_refl = result[5];
//            str_3sg = result[7];
//            wstring str_3pl_group = result[8];
            wstring str_left = xstr_comment.str_GetRegexMatch (0);
            wstring str_tense = xstr_comment.str_GetRegexMatch (1);
            wstring str_1sg = xstr_comment.str_GetRegexMatch (2);
            wstring str_1sgShort = xstr_comment.str_GetRegexMatch (3);
            wstring str_refl = xstr_comment.str_GetRegexMatch (4);
            wstring str_3sg = xstr_comment.str_GetRegexMatch (6);
            wstring str_3plGroup = xstr_comment.str_GetRegexMatch (7);

            wstring str_3pl;
            if (str_3plGroup.length() > 0)
            {
//                str_3pl = result[10];
                str_3pl = xstr_comment.str_GetRegexMatch (9);
            }

//            str_right = result[11];
            wstring str_right = xstr_comment.str_GetRegexMatch (10);
            if (str_refl.length() > 0)
            {
                xstr_comment = str_left + str_tense + L" 1 ед._ " + str_1sg + L", _" + 
                    str_tense + L" 2 ед._ " + str_3sg.substr (0, str_3sg.length() - 3) + L"шься, _" + 
                    str_tense + L" 3 ед._ " + str_3sg + L", _" + 
                    str_tense + L" 1 мн._ " + str_3sg.substr (0, str_3sg.length() - 3) + L"мся, _" + 
                    str_tense + L" 2 мн._ " + str_3sg.substr (0, str_3sg.length() - 2) + L"есь, _" + 
                    str_tense + L" 3 мн._ ";
                if (str_3plGroup.length() > 0)
                {
                    xstr_comment += str_3pl;
                }
                else
                {
                    xstr_comment += str_3sg.substr (0, str_3sg.length() - 4) + str_1sgShort + L"тся"; 
                                                                // There are no such verbs with а/я in 3pl
                }
            }
            else
            {
                xstr_comment = str_left + str_tense + L" 1 ед._ " + str_1sg + L", _" + 
                    str_tense + L" 2 ед._ " + str_3sg.substr (0, str_3sg.length() - 1) + L"шь, _" + 
                    str_tense + L" 3 ед._ " + str_3sg + L", _" + str_tense + L" 1 мн._ " + 
                    str_3sg.substr (0, str_3sg.length() - 1) + L"м, _" + 
                    str_tense + L" 2 мн._ " + str_3sg + L"е, _" + str_tense + L" 3 мн._ ";
                if (str_3plGroup.length() > 0)
                {
                    xstr_comment += str_3pl;
                }
                else
                {
                    xstr_comment += str_3sg.substr(0, str_3sg.length() - 2) + str_1sgShort + L"т";
                }
            }
            xstr_comment += str_right;
        
        }   //  if (b_match)

        // Expand
        // "_повел._ бег<и"
        CT_ExtString xstr_source (xstr_comment);
        b_match = xstr_source.b_RegexMatch (L"(.*_)(повел\\.)_\\s*([<>\\w]+(ь|и)(ся|сь)?)(.*)");
        wstring str_mood;
        wstring str_ending;

//        b_match = regex_match (str_comment, 
//                               result, 
//                               (const wregex)L"(.*_)(повел\\.)_\\s*([<>\\w]+(ь|и)(ся|сь)?))?)(.*)");
        if (b_match)
        {
            ATLASSERT (xstr_source.i_GetNumOfRegexMatches() > 0);
            wstring str_left = xstr_source.str_GetRegexMatch (0);
            wstring str_mood = xstr_source.str_GetRegexMatch (1);
            wstring str_ending = xstr_source.str_GetRegexMatch (2);
            wstring str_refl = xstr_source.str_GetRegexMatch (3);
            wstring str_right = xstr_source.str_GetRegexMatch (4);

//            str_left = result[1];
//            str_mood = result[2];
//            str_ending = result[3];
//            str_refl = result[4];
//            str_right = result[5];
            if (str_refl.length() > 0)
            {
int gogo = -1;
            }
            else
            {
int gogo = -2;
            }
            xstr_comment += str_right;
            //ERROR_LOG (L"** CheckTriangle: FUTURE --> " + str_comment);
        }

        // Expand "_прош._ возд<ал, воздал<а, возд<ало, возд<али"
//        b_match = regex_match (str_comment, 
//                               result, 
//                               (const wregex)L"(.*_)прош\\._\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+)(.*)");
        b_match = xstr_comment.b_RegexMatch 
            (L"(.*_)прош\\._\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+)(.*)");
        if (b_match)
        {
//            str_left = result[1];
//            str_m = result[2];
//            str_f = result[3];
//            str_n = result[4];
//            str_pl = result[5];
//            str_right = result[6];
            wstring str_left = xstr_comment.str_GetRegexMatch(0);
            wstring str_m = xstr_comment.str_GetRegexMatch(1);
            wstring str_f = xstr_comment.str_GetRegexMatch(2);
            wstring str_n = xstr_comment.str_GetRegexMatch(3);
            wstring str_pl = xstr_comment.str_GetRegexMatch(4);
            wstring str_right = xstr_comment.str_GetRegexMatch(5);
            xstr_comment = str_left + L"прош. м_ " + str_m + L", _прош. ж_ " +
                str_f + L", _прош. с_ " + str_n + L", _прош. мн._ " + str_pl +
                str_right;
        }

        // Expand "_прош._ в<ырос, в<ыросла, в<ыросший"
        // OR
        // "_прош._ взрев<ел, взрев<ела"
//        b_match = regex_match (str_comment, 
//                               result, 
//                               (const wregex)L"(.*_)прош\\._\\s*([<>\\w]+),\\s*([<>\\w]+)((,\\s*([<>\\w]+ий(ся)?))?)(.*)");
        b_match = xstr_comment.b_RegexMatch 
            (L"(.*_)прош\\._\\s*([<>\\w]+),\\s*([<>\\w]+)((,\\s*([<>\\w]+ий(ся)?))?)(.*)");
        if (b_match)
        {
//            str_left = result[1];
//            str_m = result[2];
//            str_f = result[3];
//            wstring str_participle_group = result[4];
//            str_participle = result[6];
//            str_right = result[8];
            wstring str_left = xstr_comment.str_GetRegexMatch (0);
            wstring str_m = xstr_comment.str_GetRegexMatch (1);
            wstring str_f = xstr_comment.str_GetRegexMatch (2);
            wstring str_participleGroup = xstr_comment.str_GetRegexMatch (3);
            wstring str_participle = xstr_comment.str_GetRegexMatch (5);
            wstring str_right = xstr_comment.str_GetRegexMatch (7);

            wstring str_n;
            wstring str_pl;
            if (str_m.substr (str_m.length() - 2, 2) == L"ся")
            {
                str_n = str_f.substr (0, str_f.length() - 3) + L"ось";
                str_pl = str_f.substr (0, str_f.length() - 3) + L"ись";
            }
            else
            {
                str_n = str_f.substr (0, str_f.length() - 1) + L"о";
                str_pl = str_f.substr (0, str_f.length() - 1) + L"и";
            }
            xstr_comment = str_left + L"прош. м_ " + str_m + L", _прош. ж_ " +
                str_f + L", _прош. с_ " + str_n + L", _прош. мн._ " + str_pl;
            if (str_participleGroup.length() > 0)
            {
                xstr_comment += L", _прич. прош._ " + str_participle + str_right;
            }
        }

        //     _наст. 2 ед._ вопи<ешь, _3 ед._ -<ет --> 
        // --> _наст. 2 ед._ вопи<ешь, _3 ед._ вопи<ет
//        b_match = regex_search(str_comment, (const wregex)L"\\w\\/[<>\\w].*\\s\\-[<>\\w]");
        b_match = xstr_comment.b_RegexMatch (L"\\w\\/[<>\\w].*\\s\\-[<>\\w]");
        if (b_match)
        {
            b_DetalizeSlashes (xstr_comment);
            b_DeleteDoubleStress (xstr_comment);
        }
        //     _наст. 2 ед._ вопи<ешь, _3 ед._ вопи<ет --> 
        // --> _наст. 2 ед._ вопи<ешь, _наст. 3 ед._ вопи<ет
        b_DetalizeCategories (xstr_comment);

        //     _прич. страд._ сд<анный (_кф_ сдан, сдан<а, сдан<о, сдан<ы) --> 
        // --> _прич. страд._ сд<анный, _прич. страд. кф м_ сдан, ...
//        b_match = regex_match(str_comment, result, (const wregex)L"(.*?_(прич\\.\\s*(прош|страд|наст)\\.)_\\s*[<>\\w]+)[,;]?\\s*\\(_кф_\\s*([<>\\-\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+)\\)(.*)");
        b_match = xstr_comment.b_RegexMatch 
            (L"(.*?_(прич\\.\\s*(прош|страд|наст)\\.)_\\s*[<>\\w]+)[,;]?\\s*\\(_кф_\\s*([<>\\-\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+)\\)(.*)");
        if (b_match)
        {
//            str_left = result[1];
//            str_participle = result[2];
//            str_m = result[4];
//            str_f = result[5];
//            str_n = result[6];
//            str_pl = result[7];
//            str_right = result[8];
            wstring str_left = xstr_comment.str_GetRegexMatch (0);
            wstring str_participle = xstr_comment.str_GetRegexMatch (1);
            wstring str_m = xstr_comment.str_GetRegexMatch (3);
            wstring str_f = xstr_comment.str_GetRegexMatch (4);
            wstring str_n = xstr_comment.str_GetRegexMatch (5);
            wstring str_pl = xstr_comment.str_GetRegexMatch (6);
            wstring str_right = xstr_comment.str_GetRegexMatch (7);
            xstr_comment = str_left + L", _" + str_participle + L" кф м_ " + str_m +
                L", _" + str_participle + L" кф ж_ " + str_f + L", _" +
                str_participle + L" кф с_ " + str_n + L", _" +
                str_participle + L" кф мн._ " + str_pl + str_right;
        }

        // Finally, if the string is OK, we put it to str_IrregularForms.
//        if (regex_match(str_comment, (const wregex)L"нет_"))
        if (wstring::npos != xstr_comment.find (L"нет_"))
        {
            b_match = b_CheckDifficulties (xstr_comment, st_entry);
            if (!b_match)
            {
                return false;
            }
        }

        //        b_match = regex_match(str_comment, (const wregex)L"(_(прич\\.\\s*(страд\\.(\\s*(прош|наст)\\.)?|прош\\.(\\s*(мн\\.|м|ж|с))?|наст\\.)(\\s*кф\\s*(мн\\.|ж|м|с))?(\\s*ед\\.)?|деепр\\.(\\s*(прош|наст)\\.)|повел\\.|прош\\.\\s*(мн\\.|м|ж|с)(\\s*ед\\.)?|(наст|буд)\\.\\s*[123]\\s*(ед|мн)\\.|инф\\.)_\\s*[<>\\w][<>\\-\\w]*([,;]\\s*|$))*");
        b_match = xstr_comment.b_RegexMatch 
            (L"(_(прич\\.\\s*(страд\\.(\\s*(прош|наст)\\.)?|прош\\.(\\s*(мн\\.|м|ж|с))?|наст\\.)(\\s*кф\\s*(мн\\.|ж|м|с))?(\\s*ед\\.)?|деепр\\.(\\s*(прош|наст)\\.)|повел\\.|прош\\.\\s*(мн\\.|м|ж|с)(\\s*ед\\.)?|(наст|буд)\\.\\s*[123]\\s*(ед|мн)\\.|инф\\.)_\\s*[<>\\w][<>\\-\\w]*([,;]\\s*|$))*");
        if (b_match)
        {
            if (b_alternative)
            {
                st_entry.str_AltIrregularForms = xstr_comment;  // slice
            }
            else
            {
                st_entry.str_IrregularForms = xstr_comment;
                xstr_descriptor = str_rest;
            }

            return true;
        }
    }       //  if (map_Pos[st_entry.str_MainSymbol] == POS_VERB)

//    if (1 == 1) //Otherwise ERROR_LOG won't work
//    {
        ERROR_LOG (L"CheckTriangle couldn't recognize the text: " + xstr_comment);
//    }
    return false;

}   //  b_CheckTriangle (...)

bool CT_ZalEntryParser::b_DetalizeDoubleSlashes (CT_ExtString& xstr_source)
{
    // _сравн._ (_затрудн._) г<ульче//г<улче
//    bool b_match = false;
    wsmatch result;
    wstring str_stem = L"", str_right = L"", str_left = L"";
//    b_match = regex_match(str_source, result, 
//        (const wregex)L"^(.*?)(_[^_]*_(\\s*\\(_затрудн\\.?_\\))?)\\s*([^\\s\\/]*)\\s*\\/\\/\\s*([^\\s\\/_]+.*)$");
    bool b_match = xstr_source.b_RegexMatch 
        (L"^(.*?)(_[^_]*_(\\s*\\(_затрудн\\.?_\\))?)\\s*([^\\s\\/]*)\\s*\\/\\/\\s*([^\\s\\/_]+.*)$");
    if (b_match)
    {
//        str_source = (wstring)result[1] +
//            (wstring)result[2] + L" " + (wstring)result[4] + L", " +
//            (wstring)result[2] + L" " + (wstring)result[5];
        xstr_source = xstr_source.str_GetRegexMatch(0) +
            xstr_source.str_GetRegexMatch(1) + L" " + xstr_source.str_GetRegexMatch(3) + L", " +
            xstr_source.str_GetRegexMatch(1) + L" " + xstr_source.str_GetRegexMatch(4);
    }
    // _сравн._ г<ульче//_устар._ г<улче
//    b_match = regex_match(str_source, result, 
//        (const wregex)L"^(.*?)(_[^_]*_)\\s*([^\\s\\/]*)\\s*\\/\\/\\s*(_устар\\._|_затрудн\\._)\\s*([^\\s\\/]+.*)$");
    b_match = xstr_source.b_RegexMatch
        (L"^(.*?)(_[^_]*_)\\s*([^\\s\\/]*)\\s*\\/\\/\\s*(_устар\\._|_затрудн\\._)\\s*([^\\s\\/]+.*)$");
    if (b_match)
    {
        //ERROR_LOG(L"****b_DetalizeDoubleSlashes - begin: " + str_source);
//        str_source = (wstring)result[1] +
//            (wstring)result[2] + L" " + (wstring)result[3] + L", " +
//            (wstring)result[2] + L" (" + (wstring)result[4] + L") " +
//            (wstring)result[5];
        xstr_source = (wstring)result[1] +
            (wstring)result[2] + L" " + (wstring)result[3] + L", " +
            (wstring)result[2] + L" (" + (wstring)result[4] + L") " +
            (wstring)result[5];
    }
    //ERROR_LOG(L"****b_DetalizeDoubleSlashes: " + str_source);
    return true;

}   //  b_DetalizeDoubleSlashes (...)

bool CT_ZalEntryParser::b_DetalizeSlashes (CT_ExtString& xstr_source)
{
    // _мн._ бр<ат/ья, -ьев, -ьям --> _мн._ бр<ат--ья, бр<ат--ьев, бр<ат--ьям
//    wsmatch result;
//    wstring str_stem = L"", str_right = L"", str_left = L"", str_end1 = L"", str_end2 = L"", str_end3 = L"";

    // 1. (наст.|буд.)
//    if (regex_match(str_source, result, 
//        (const wregex)L"(.*?(_наст\\._|_буд\\._)\\s*)([\\w<>]+?)(у|ю|усь|юсь),\\s*\\-([\\w<>]+)(,\\s*\\-([\\w<>]+))?(.*)$"))
    if (xstr_source.b_RegexMatch 
        (L"(.*?(_наст\\._|_буд\\._)\\s*)([\\w<>]+?)(у|ю|усь|юсь),\\s*\\-([\\w<>]+)(,\\s*\\-([\\w<>]+))?(.*)$"))
    {
//        str_left = (wstring)result[1];
//        str_right = (wstring)result[8];
//        str_stem = (wstring)result[3];
//        str_end1 = (wstring)result[4];
//        str_end2 = (wstring)result[5];
//        str_end3 = (wstring)result[7];
        wstring str_left = xstr_source.str_GetRegexMatch (0);
        wstring str_right = xstr_source.str_GetRegexMatch (7);
        wstring str_stem = xstr_source.str_GetRegexMatch (2);
        wstring str_end1 = xstr_source.str_GetRegexMatch (3);
        wstring str_end2 = xstr_source.str_GetRegexMatch (4);
        wstring str_end3 = xstr_source.str_GetRegexMatch (6);
        xstr_source = str_left + str_stem + str_end1 + L", " + str_stem + L"--" + str_end2;
        if (str_end3.length() > 0)
        {
            xstr_source += L", " + str_stem + L"--" + str_end3;
        }
        xstr_source += str_right;

        b_DeleteDoubleStress (xstr_source);
        b_DetalizeSlashes (xstr_source); // in case there are several such substrings
        
        return true;
    }
    // 2. With a slash: "_буд._ взъ<е/мся, -шься"
//    else if (regex_match(str_source, result, 
//        (const wregex)L"(.*?)([\\w<>]+)\\/([\\w<>]+)(.*)"))
    else if (xstr_source.b_RegexMatch (L"(.*?)([\\w<>]+)\\/([\\w<>]+)(.*)"))
    {
//        str_left = (wstring)result[1] + (wstring)result[2] + L"--" + (wstring)result[3];
//        str_right = (wstring)result[4];
//        str_stem = (wstring)result[2];
        wstring str_left = xstr_source.str_GetRegexMatch(0) + 
                           xstr_source.str_GetRegexMatch(1) + 
                           L"--" + 
                           xstr_source.str_GetRegexMatch(2);
//        str_right = (wstring)result[4];
//        str_stem = (wstring)result[2];
        CT_ExtString xstr_right = xstr_source.str_GetRegexMatch(3);
        wstring str_stem = xstr_source.str_GetRegexMatch(1);

//        while (regex_match(str_right, result, 
//            (const wregex)L"([^\\-\\/]*)\\-([\\w<>]+)(.*)"))
        while (xstr_right.b_RegexMatch (L"([^\\-\\/]*)\\-([\\w<>]+)(.*)"))
        {
//            str_left += (wstring)result[1] + str_stem + L"--" + (wstring)result[2];
//            str_right = (wstring)result[3];
//            if (regex_search(str_right, (const wregex)L"^[^\\-]*\\s[<>\\w]+[,;]?\\s"))
//            {
//                // _буд._ зад<а/м, -шь, <...>; _повел._ зад<ай; _прош._ з<адал, -<а, -о, -и
//                break;
//            }
            str_left += xstr_right.str_GetRegexMatch(0) + str_stem + L"--" + xstr_right.str_GetRegexMatch(1);
            xstr_right = xstr_right.str_GetRegexMatch(2);
//            if (regex_search(str_right, (const wregex)L"^[^\\-]*\\s[<>\\w]+[,;]?\\s"))
//            {
//                // _буд._ зад<а/м, -шь, <...>; _повел._ зад<ай; _прош._ з<адал, -<а, -о, -и
//                break;
//            }
            if (xstr_right.b_RegexMatch (L"^[^\\-]*\\s[<>\\w]+[,;]?\\s"))
            {
                // _буд._ зад<а/м, -шь, <...>; _повел._ зад<ай; _прош._ з<адал, -<а, -о, -и
                break;
            }
        }
        xstr_source = str_left + xstr_right;

        b_DeleteDoubleStress (xstr_source);
        b_DetalizeSlashes (xstr_source); // in case there are several such substrings
        
        return true;
    }
    // 3. Without a slash: "_прош._ был, -<а, -о, -и" OR "кф ..."
//    else if (regex_match(str_source, result, 
//            (const wregex)L"(.*?_[^_\\s]*_)\\s*([\\w<>]+),\\s*\\-([\\w<>]+)(.*)"))
    else if (xstr_source.b_RegexMatch (L"(.*?_[^_\\s]*_)\\s*([\\w<>]+),\\s*\\-([\\w<>]+)(.*)"))
    {
//        str_stem = (wstring)result[2];
//        str_left = (wstring)result[1] + L" " + str_stem + L", " +
//            str_stem + L"--" + (wstring)result[3];
//        str_right = (wstring)result[4];
        wstring str_stem = xstr_source.str_GetRegexMatch(1);
        wstring str_left = xstr_source.str_GetRegexMatch(0) + L" " + str_stem + L", " +
            str_stem + L"--" + xstr_source.str_GetRegexMatch(2);
        CT_ExtString xstr_right = xstr_source.str_GetRegexMatch(3);
//        while (regex_match(str_right, result, 
//            (const wregex)L",\\s*\\-([\\w<>]+)(.*)"))
//        {
//            str_left += L", " + str_stem + L"--" +(wstring)result[1];
//            str_right = (wstring)result[2];
//        }
        xstr_source = str_left + xstr_right;
        while (xstr_right.b_RegexMatch (L",\\s*\\-([\\w<>]+)(.*)"))
        {
            str_left += L", " + str_stem + L"--" + xstr_right.str_GetRegexMatch(0);
            xstr_right = xstr_right.str_GetRegexMatch(1);
        }
        xstr_source = str_left + xstr_right;

        b_DeleteDoubleStress (xstr_source);
        b_DetalizeSlashes (xstr_source); // in case there are several such substrings
        
        return true;
    }
    
    return false;

}   //  b_DetalizeSlashes (...)

bool CT_ZalEntryParser::b_DetalizeCategories (CT_ExtString& xstr_source)
{
    //     _наст. 2 ед._ вопи<ешь, _3 ед._ вопи<ет --> 
    // --> _наст. 2 ед._ вопи<ешь, _наст. 3 ед._ вопи<ет
//    bool b_match = false;
//    wsmatch result;
//    wstring str_category = L"", str_right = L"", str_left = L"";
//    b_match = regex_match(str_source, result, 
//        (const wregex)L"(.*_)((наст)|(буд)\\.)(\\s*[123]\\s*((ед)|(мн))\\._[^_]*_)\\s*([123]\\s*((ед)|(мн))\\._)(.*)");
    bool b_match = xstr_source.b_RegexMatch 
        (L"(.*_)((наст)|(буд)\\.)(\\s*[123]\\s*((ед)|(мн))\\._[^_]*_)\\s*([123]\\s*((ед)|(мн))\\._)(.*)");
    if (b_match)
    {
//        str_category = (wstring)result[2];
//        str_left = (wstring)result[1] + str_category + (wstring)result[5] + str_category + L" " + (wstring)result[9];
//        str_right = (wstring)result[13];
//        while (regex_match(str_right, result, 
//            (const wregex)L"([^_]*_)\\s*([123]\\s*((ед)|(мн))\\._)(.*)"))
//        {
            //ERROR_LOG(str_left + str_right);
//            str_left += (wstring)result[1] + str_category + L" " + (wstring)result[2];
//            str_right = (wstring)result[6];
//        }
//        str_source = str_left + str_right;

        wstring str_category = xstr_source.str_GetRegexMatch(1);
        wstring str_left = xstr_source.str_GetRegexMatch(0) + str_category + xstr_source.str_GetRegexMatch(4) +
            str_category + L" " + xstr_source.str_GetRegexMatch(8);
        CT_ExtString xstr_right = xstr_source.str_GetRegexMatch(12);
//        while (regex_match(str_right, result, 
//            (const wregex)L"([^_]*_)\\s*([123]\\s*((ед)|(мн))\\._)(.*)"))
//        {
            //ERROR_LOG(str_left + str_right);
//            str_left += (wstring)result[1] + str_category + L" " + (wstring)result[2];
//            str_right = (wstring)result[6];
//        }
//        str_source = str_left + str_right;
        while (xstr_right.b_RegexMatch (L"([^_]*_)\\s*([123]\\s*((ед)|(мн))\\._)(.*)"))
        {
            str_left += xstr_right.str_GetRegexMatch(0) + str_category + L" " + xstr_right.str_GetRegexMatch(1);
            xstr_right = xstr_right.str_GetRegexMatch(5);
        }
        xstr_source = str_left + xstr_right;

        return true;
    }
    
    return false;

}   //  b_DetalizeCategories (...)

bool CT_ZalEntryParser::b_DeleteDoubleStress (CT_ExtString& xstr_source)
{
    bool b_changes_made = false;
//    wsmatch ct_result;

    // 1. невел<ик--<о --> невелик<о
//    while (regex_match(str_source, ct_result, (const wregex)L"(.*)<(\\w*)\\-\\-(\\w*<)(.*)"))
//    {
//        str_source = (wstring)ct_result[1] + (wstring)ct_result[2] + (wstring)ct_result[3] 
//            + (wstring)ct_result[4];
//        b_changes_made = true;
//    }
    while (xstr_source.b_RegexMatch (L"(.*)<(\\w*)\\-\\-(\\w*<)(.*)"))
    {
        xstr_source = xstr_source.str_GetRegexMatch(0) + xstr_source.str_GetRegexMatch(1) + 
            xstr_source.str_GetRegexMatch(2) + xstr_source.str_GetRegexMatch(3);
        b_changes_made = true;
    }

    // 2. был--о --> б<ыло
//    while (regex_match(str_source, ct_result, (const wregex)L"(.*?\\s)([бвгджзйклмнпрстфхцчшщъь]*)([аеёиоуыэюя][^\\-\\s<>]*)\\-\\-([^<]*[,;$].*)"))
//    {
//        str_source = (wstring)ct_result[1] + (wstring)ct_result[2] + L"<" + (wstring)ct_result[3] + (wstring)ct_result[4];
//        b_changes_made = true;
//    }
    while (xstr_source.b_RegexMatch 
        (L"(.*?\\s)([бвгджзйклмнпрстфхцчшщъь]*)([аеёиоуыэюя][^\\-\\s<>]*)\\-\\-([^<]*[,;$].*)"))
    {
        xstr_source = xstr_source.str_GetRegexMatch(0) + xstr_source.str_GetRegexMatch(1) + L"<" + 
            xstr_source.str_GetRegexMatch (2) + xstr_source.str_GetRegexMatch (3);
        b_changes_made = true;
    }

    // 3. был--<а --> был<а
//    while (regex_match(str_source, ct_result, (const wregex)L"(.*?)\\-\\-(.*)"))
//    {
//        str_source = (wstring)ct_result[1] + (wstring)ct_result[2];
//        b_changes_made = true;
//    }
    while (xstr_source.b_RegexMatch (L"(.*?)\\-\\-(.*)"))
    {
        xstr_source = xstr_source.str_GetRegexMatch(0) + xstr_source.str_GetRegexMatch(1);
        b_changes_made = true;
    }

    return b_changes_made;

}   //  b_DeleteDoubleStress (...)

bool CT_ZalEntryParser::b_CheckColon (CT_ExtString& xstr_descriptor, ST_Descriptor& st_entry)
{
    if (xstr_descriptor.length() <= 0)
    {
        return true;
    }

    bool b_match = false;
//    wsmatch ct_result;

    // Looks for: ...:...
    // Except: (...:...)
//    b_match = regex_search(str_descriptor, (const wregex)L"^[^\\(]*(\\([^\\)]*\\)[^\\(]*)*\\:");
    b_match = xstr_descriptor.b_RegexMatch (L"^[^\\(]*(\\([^\\)]*\\)[^\\(]*)*\\:");
    if (!b_match)
    {
        return true;
    }

    // Handles:
    // ... :{...} ...
//    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"^(.*?),?\\s*\\:\\s*\\{([^\\}]*)\\}\\s*(.*)$");
//    if (b_match == true)
//    {
//        //ERROR_LOG(L"CheckColon, :(): " + str_descriptor);
//        st_entry.str_Contexts = (wstring)ct_result[2];
//        str_descriptor = (wstring)ct_result[1] + (wstring)ct_result[3];
//        return true;
//    }
    b_match = xstr_descriptor.b_RegexMatch (L"^(.*?),?\\s*\\:\\s*\\{([^\\}]*)\\}\\s*(.*)$");
    if (b_match)
    {
        st_entry.str_Contexts = xstr_descriptor.str_GetRegexMatch(1);
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch(0) + xstr_descriptor.str_GetRegexMatch(2);
        return true;
    }

    // ... : ... [;|%|#|$ ...]
//    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"^([^\\(]*(\\([^\\)]*\\)[^\\(]*?)*),?\\s*\\:\\s*([^;%#\\$\\{\\}]*)(.*)$");
//    if (b_match == true)
//    {
//        st_entry.str_Contexts = (wstring)ct_result[3];
//        str_descriptor = (wstring)ct_result[1] + (wstring)ct_result[4];
//        return true;
//    }
    b_match = xstr_descriptor.b_RegexMatch 
        (L"^([^\\(]*(\\([^\\)]*\\)[^\\(]*?)*),?\\s*\\:\\s*([^;%#\\$\\{\\}]*)(.*)$");
    if (b_match)
    {
        st_entry.str_Contexts = xstr_descriptor.str_GetRegexMatch (2);
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch(0) + xstr_descriptor.str_GetRegexMatch(3);
        return true;
    }

    ERROR_LOG(L"CheckColon was started on " + xstr_descriptor + L" and couldn't recognize the string.");

    return false;

}   //  b_CheckColon (...)

bool CT_ZalEntryParser::b_CheckPercent (CT_ExtString& xstr_descriptor, ST_Descriptor& st_entry)
{
    if (xstr_descriptor.length() <= 0)
    {
        return true;
    }
//    bool b_match = false;
//    wsmatch ct_result;
//    b_match = regex_search(str_descriptor, (const wregex)L"\\%");
    //ERROR_LOG(L"CheckPercent: " + str_descriptor);
    if (wstring::npos == xstr_descriptor.find (L'%'))
    {
        return true;
    }
//    if (b_match == false)
//    {
//        return true;
//    }
//    b_match = regex_match(str_descriptor, (const wregex)L"^([^\\%]*?),?\\s*\\%\\{([^\\}]*)\\}(.*)$");
    bool b_match = xstr_descriptor.b_RegexMatch (L"^([^\\%]*?),?\\s*\\%\\{([^\\}]*)\\}(.*)$");
    if (b_match)
    {
        //ERROR_LOG(L"CheckPercent, %(): " + str_descriptor);
//        st_entry.str_RestrictedForms = (wstring)ct_result[2];
        st_entry.str_RestrictedForms = xstr_descriptor.str_GetRegexMatch (1);
//        str_descriptor = (wstring)ct_result[1] + (wstring)ct_result[3];
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0) + xstr_descriptor.str_GetRegexMatch (2);
        return true;
    }

    //b_match = regex_match(str_descriptor, ct_result, (const wregex)L"^([^\\%]*?),?\\s*\\%\\s*([^#;\\:\\$]*)(.*)$");
    b_match = xstr_descriptor.b_RegexMatch (L"^([^\\%]*?),?\\s*\\%\\s*([^#;\\:\\$]*)(.*)$");
    if (b_match)
    {
//        st_entry.str_RestrictedForms = (wstring)ct_result[2];
        st_entry.str_RestrictedForms = xstr_descriptor.str_GetRegexMatch (1);
        //ERROR_LOG(L"CheckPercent was started on " + str_descriptor + L", result is " + st_entry.str_RestrictedForms);
//        str_descriptor = (wstring)ct_result[1] + (wstring)ct_result[3];
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0) + xstr_descriptor.str_GetRegexMatch (2);
        return true;
    }

    ERROR_LOG (L"CheckPercent was started on " + xstr_descriptor + L" and couldn't recognize the string.");

    return false; // If there are parentheses, slashes, or something

}   //  b_CheckPercent (...)

bool CT_ZalEntryParser::b_CheckDifficulties (CT_ExtString& xstr_descriptor, ST_Descriptor& st_entry)
{
    // Handles cases:
    // _Р. мн. затрудн._
    // _пф нет_
    // etc.
    if (xstr_descriptor.length() <= 0)
    {
        return true;
    }
    
//    bool b_match = false;
//    wsmatch ct_result;
//    wstring str_left, str_right, str_difficult, str_param1, str_param2;
//    b_match = regex_search(str_descriptor, (const wregex)L"(затрудн\\.|нет|\\(_устар\\.)_");
//    if (b_match == false)
//    {
//        return true;
//    }

    bool b_match = xstr_descriptor.b_RegexMatch (L"(затрудн\\.|нет|\\(_устар\\.)_");
    if (!b_match)
    {
        return true;
    }

    // _Р. мн. затрудн._, etc
//    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"(.*?),?\\s*_([^_]+?)\\s*затрудн\\._(.*)");
    b_match = xstr_descriptor.b_RegexMatch (L"(.*?),?\\s*_([^_]+?)\\s*затрудн\\._(.*)");
    while (b_match)
    {
//        str_left = ct_result[1];
//        str_right = ct_result[3];
//        str_difficult = ct_result[2];
//        b_match = regex_match(str_difficult, ct_result, (const wregex)L"(.*)\\sи\\s(.*)");
        wstring str_left = xstr_descriptor.str_GetRegexMatch(0);
        wstring str_right = xstr_descriptor.str_GetRegexMatch(2);
        CT_ExtString xstr_target (xstr_descriptor.str_GetRegexMatch(1));
        b_match = xstr_target.b_RegexMatch (L"(.*)\\sи\\s(.*)");
        if (b_match)
        {
//            str_param1 = ct_result[1];
//            str_param2 = ct_result[2];
            CT_ExtString xstr_gramm (xstr_target.str_GetRegexMatch (0));
//            b_match = regex_match(str_param1, ct_result, (const wregex)L"(.*)\\s(м|ж|с|ед\\.|мн\\.)");
            b_match = xstr_gramm.b_RegexMatch (L"(.*)\\s(м|ж|с|ед\\.|мн\\.)");
            if (b_match)
            {
//                st_entry.str_Difficulties += (wstring)ct_result[1] + L" " + (wstring)ct_result[2]
//                + L", " + (wstring)ct_result[1] +L" " + str_param2 + L", ";
                st_entry.str_Difficulties += xstr_gramm.str_GetRegexMatch(0) + L" " +
                    xstr_gramm.str_GetRegexMatch(1) + L", " + 
                    xstr_target.str_GetRegexMatch(1) + L", ";
//                str_descriptor = str_left + str_right;
                xstr_descriptor = str_left + str_right;
            }
            else
            {
                st_entry.str_Difficulties += xstr_gramm.str_GetRegexMatch(0) + L", " + 
                    xstr_gramm.str_GetRegexMatch(1) + L",";
                xstr_descriptor = str_left + str_right;
            }
        }
        else
        {
            st_entry.str_Difficulties += xstr_target + L",";
//            str_descriptor = str_left + str_right;
            xstr_descriptor = xstr_descriptor.str_GetRegexMatch(0) + xstr_descriptor.str_GetRegexMatch(2);
        }

//        b_match = regex_match(str_descriptor, ct_result, (const wregex)L"(.*?),?\\s*_([^_]+?)\\s*затрудн\\._(.*)");
        b_match = xstr_descriptor.b_RegexMatch (L"(.*?),?\\s*_([^_]+?)\\s*затрудн\\._(.*)");
    
    }   //  while (b_match)

    // _Р. мн._ (_затрудн._) ...
//    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"(.*_([^_]*)_\\s*)\\(_затрудн\\._\\)(.*)");
    wstring str_match (L"(.*_([^_]*)_\\s*)\\(_затрудн\\._\\)(.*)");
    b_match = xstr_descriptor.b_RegexMatch (str_match);
    while (b_match)
    {
//        str_left = ct_result[1];
//        str_right = ct_result[3];
//        st_entry.str_Difficulties += (wstring)ct_result[2] + L", ";
        st_entry.str_Difficulties += xstr_descriptor.str_GetRegexMatch (1) + L", ";
//        str_descriptor = str_left + str_right;
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0) + xstr_descriptor.str_GetRegexMatch (2);
//        b_match = regex_match(str_descriptor, ct_result, (const wregex)L"(.*_([^_]*)_\\s*)\\(_затрудн\\._\\)(.*)");
        b_match = xstr_descriptor.b_RegexMatch (str_match);
    }

    // _Р. мн._ (_устар._) ...
//    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"(.*_([^_]*)_\\s*)\\(_устар\\._\\)(.*)");
    str_match = L"(.*_([^_]*)_\\s*)\\(_устар\\._\\)(.*)";
    b_match = xstr_descriptor.b_RegexMatch (str_match);
    while (b_match)
    {
//        str_left = ct_result[1];
//        str_right = ct_result[3];
//        st_entry.str_Obsolete += (wstring)ct_result[2] + L", ";
        st_entry.str_Obsolete += xstr_descriptor.str_GetRegexMatch (1) + L", ";
//        str_descriptor = str_left + str_right;
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0) + xstr_descriptor.str_GetRegexMatch (2);
        xstr_descriptor.b_RegexMatch (str_match);
//        b_match = regex_match(str_descriptor, ct_result, (const wregex)L"(.*_([^_]*)_\\s*)\\(_устар\\._\\)(.*)");

    }

    // _пф нет_
    //st_entry.str_Deficient = L"";
    str_match = L"(.*?)[;,]?\\s*_([^_]+?)\\s*нет_(.*)";
//    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"(.*?)[;,]?\\s*_([^_]+?)\\s*нет_(.*)")
    b_match = xstr_descriptor.b_RegexMatch (str_match);
    while (b_match)
    {
//        str_left = ct_result[1];
//        str_right = ct_result[3];
//        st_entry.str_Deficient += (wstring)ct_result[2] + L", ";
        st_entry.str_Deficient += xstr_descriptor.str_GetRegexMatch (1) + L", ";
//        str_descriptor = str_left + str_right;
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0) + xstr_descriptor.str_GetRegexMatch (2);
//        b_match = regex_match(str_descriptor, ct_result, (const wregex)L"(.*?)[;,]?\\s*_([^_]+?)\\s*нет_(.*)");
        b_match = xstr_descriptor.b_RegexMatch (str_match);
    }

    // Finally, check if we've done everything we had to
//    b_match = regex_search(str_descriptor, (const wregex)L"(затрудн\\.|нет|\\(_устар\\.)_");
    // ^^^^^^^^^^^^^^^^^^ ---- search or match? kb
    //if (b_match == false)
    //{
    //    return true;
    //}
    return !xstr_descriptor.b_RegexMatch (L"(затрудн\\.|нет|\\(_устар\\.)_");

}   // b_CheckDifficulties (...)

//bool CT_ZalEntryParser::b_CheckInflectedParts (wstring& str_descriptor, ST_Descriptor& st_entry)
bool CT_ZalEntryParser::b_CheckInflectedParts (CT_ExtString& xstr_descriptor, ST_Descriptor& st_entry)
{
    // Handles cases:
    // ..., INFL=1
    // ..., INFL=2
    if (xstr_descriptor.length() <= 0)
    {
        return true;
    }
    
//    bool b_match = false;
//    wsmatch ct_result;
//    wstring str_inflected;
//    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"^(.*?),?\\s*INFL=1(.*)$");
    bool b_match = xstr_descriptor.b_RegexMatch (L"^(.*?),?\\s*INFL=1(.*)$");
    if (b_match)
    {
//        str_descriptor = (wstring)ct_result[1] + (wstring)ct_result[2];
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0) + xstr_descriptor.str_GetRegexMatch (1);
        st_entry.st_InflectionGroup.i_InflectedParts = 1;
    }

//    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"^(.*?),?\\s*INFL=2(.*)$");
    b_match = xstr_descriptor.b_RegexMatch (L"^(.*?),?\\s*INFL=2(.*)$");
    if (b_match)
    {
//        str_descriptor = (wstring)ct_result[1] + (wstring)ct_result[2];
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0) + xstr_descriptor.str_GetRegexMatch (1);
        st_entry.st_InflectionGroup.i_InflectedParts = 2;
    }
    return true;

}   //  b_CheckInflectedParts (...)
