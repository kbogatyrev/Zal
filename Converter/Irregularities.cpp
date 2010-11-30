#include "StdAfx.h"
#include "Globals.h"
#include "ExtString.h"
#include "ZalEntryParser.h"

bool CT_ZalEntryParser::b_CheckSharp (CT_ExtString& xstr_descriptor, ST_Descriptor& st_entry)
{
    if (xstr_descriptor.length() <= 0)
    {
        return true;
    }
    bool b_match = xstr_descriptor.b_RegexMatch (L"^([^#]*?),?\\s*\\#\\s*(\\d+)\\s*(.*)$");
    if (!b_match && st_entry.i_Section <= 0)
    {
        st_entry.i_Section = 0;
        return true;
    }

    wstring str_number = xstr_descriptor.str_GetRegexMatch (1);
    if (str_number.length() == 0)
    {
        return false;
    }

    st_entry.i_Section = _wtoi(str_number.c_str());
    wstring str_rest = xstr_descriptor.str_GetRegexMatch(2);
    if (str_rest.length() > 0)
    {
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch(0) + L" " + str_rest;
    }
    else
    {
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch(0);
    }
    return true;

}   //  b_CheckSharp (...)

bool CT_ZalEntryParser::b_CheckTriangle (const ST_Headword& st_head, 
                                         CT_ExtString& xstr_descriptor, 
                                         ST_Descriptor& st_entry, 
                                         bool b_alternative)
{
    if (xstr_descriptor.length() <= 0)
    {
        return true;
    }
    // @ comment can be followed by $, #, [, or %
    bool b_match = xstr_descriptor.b_RegexMatch 
        (L"^(.*?)\\s*@\\s*([^\\s\\$\\#\\[\\%][^\\$\\#\\[\\%]+[^\\s])\\s*(([\\$\\#\\[\\%].*)?)$");
    if (!b_match)
    {
        return true;
    }

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
    b_match = xstr_comment.b_RegexMatch (L"^(.*?)[;,]?\\s*(ALTPAR|POETPAR)\\{([^\\}]+)\\}(.*)$");
    if (b_match)
    {
        CT_ExtString xstr_alt (L"@" + xstr_comment.str_GetRegexMatch(2));
        xstr_comment = xstr_comment.str_GetRegexMatch (0) + xstr_comment.str_GetRegexMatch (3);
        bool b_AltProcessed = b_CheckTriangle (st_head, xstr_alt, st_entry, true);
        //ERROR_LOG(L"ALTPAR: " + str_new_descriptor + L";str_comment: " + str_comment);
        if (!b_AltProcessed)
        {
            return false;
        }

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
        wstring str_regex = L"(.*)_мн\\._\\s*(\\(_затрудн._\\))?\\s*([<>\\w]+),\\s*([<>\\w]*),\\s*([<>\\w]*)(.*)";
        b_match = xstr_comment.b_RegexMatch(str_regex);
        while (b_match)
        {
            wstring str_left = xstr_comment.str_GetRegexMatch (0);
            wstring str_nom = xstr_comment.str_GetRegexMatch (2);
            wstring str_gen = xstr_comment.str_GetRegexMatch (3);
            wstring str_dat = xstr_comment.str_GetRegexMatch (4);
            wstring str_right = xstr_comment.str_GetRegexMatch (5);
            wstring str_middle = L"_И. мн._ " + str_nom + 
                                 L", _Р. мн._ " + str_gen + 
                                 L", _Д. мн._ " + str_dat;

            // Accusative ending depends on animacy
            if (st_entry.str_MainSymbol == L"мо" || 
                st_entry.str_MainSymbol == L"жо" || 
                st_entry.str_MainSymbol == L"мо-жо")
            {
                str_middle += L", _В. мн._ " + str_gen +
                              L", _Т. мн._ " + str_dat + L"и" +
                              L", _П. мн._ " + str_dat.substr(0, str_dat.length() - 1) + L"х";
            }
            else
            {
                str_middle += L", _В. мн._ " + str_nom + 
                              L", _Т. мн._ " + str_dat + L"и" +
                              L", _П. мн._ " + str_dat.substr(0, str_dat.length() - 1) + L"х";
            }
            xstr_comment = str_left + str_middle + str_right;
            b_match = xstr_comment.b_RegexMatch(str_regex);
        }

        // Replace strings like "_Д., П. ед._ пареми/<и" with the detailed description
        wstring str_regexBase 
            (L"(.*)(И|Р2?|Д|В|Т|П2?)\\.?,\\s*(И|Р2?|Д|В|Т|П2?)\\.?\\s*((ед\\.)|(мн\\.))_\\s*([<>\\-\\w]*)");
        b_match = xstr_comment.b_RegexMatch (str_regexBase + L"(\\/?)([<>\\-\\w]*)(.*)"); 
        while (b_match)
        {
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
            xstr_comment = str_left + str_case1 + L". " + str_num + L"_ " +
                           str_wordForm + L", _" + str_case2 + L". " +
                           str_num + L"_ " + str_wordFormSlash + str_right;

            b_match = xstr_comment.b_RegexMatch (str_regexBase + L"(.*)");
        }   //  while (b_match)

        // Finally, if the string is OK, we put it to str_IrregularForms.
        b_match = xstr_comment.b_RegexMatch (L"(_(И|Р2?|Д|В|Т|П2?)\\.?\\s*(ед|мн)\\._\\s*[<>\\w][<>\\-\\w]*([,;]\\s*|$))*");
        if (b_match)
        {
            if (b_alternative)
            {
                st_entry.xstr_AltIrregularForms = xstr_comment;  // slice...
            }
            else
            {
                st_entry.xstr_IrregularForms = xstr_comment;
                xstr_descriptor = str_rest;
            }
            //ERROR_LOG (L"--> " + str_comment);
            return true;
        }
    }       //  if (map_Pos[st_entry.str_MainSymbol] == POS_NOUN)

    if (map_Pos[st_entry.str_MainSymbol] == POS_PRONOUN || 
        map_Pos[st_entry.str_MainSymbol] == POS_NUM)
    {
        // Replace strings like "_Р., Д., П._ двен<адцат/и" with the detailed description
        wstring str_matcher (L"(.*)(И|Р|Д|В|Т|П)\\.,?\\s*(И|Р|Д|В|Т|П)\\._\\s*([<>\\-\\w]*)(\\/?)([<>\\-\\w]*)(.*)");
        while (xstr_comment.b_RegexMatch (str_matcher))
        {
            wstring str_left = xstr_comment.str_GetRegexMatch(0);
            wstring str_case1 = xstr_comment.str_GetRegexMatch(1);
            wstring str_case2 = xstr_comment.str_GetRegexMatch(2);
            wstring str_wordForm = xstr_comment.str_GetRegexMatch(3) + xstr_comment.str_GetRegexMatch(5);
            wstring str_wordFormSlash = xstr_comment.str_GetRegexMatch(3) + 
                                        xstr_comment.str_GetRegexMatch(4) + 
                                        xstr_comment.str_GetRegexMatch(5);
            wstring str_right = xstr_comment.str_GetRegexMatch(6);
            xstr_comment = str_left + str_case1 + L"._ " + str_wordForm + 
                           L", _" + str_case2 + L"._ " + str_wordFormSlash + str_right;
            b_match = xstr_comment.b_RegexMatch(str_matcher);
        }

        // Replace _В.=И._
        // TODO: Add the stress to the accusative form
        b_match = xstr_comment.b_RegexMatch (L"(.*)_В\\._?\\s*=\\s*_?И\\._(.*)");
        if (b_match)
        {
            xstr_comment = xstr_comment.str_GetRegexMatch (0) + L"_В._ " +
                           st_head.str_RawSource + xstr_comment.str_GetRegexMatch (1);
            //ERROR_LOG (L"--> " + str_comment);
        }

        // Finally, put the string to str_IrregularForms.
        // (We don't check it because it's easier to check all
        // relevant dictionary entries manually.)
        if (b_alternative)
        {
            st_entry.xstr_AltIrregularForms = xstr_comment;  // slice
        }
        else
        {
            st_entry.xstr_IrregularForms = xstr_comment;      // slice
            xstr_descriptor = str_rest;
        }

        return true;

    }   //  POS_PRONOUN, POS_NUM

    if (map_Pos[st_entry.str_MainSymbol] == POS_ADJ)
    {
        // Replace strings like "_кф_ нес<олон, несолон<а, нес<олоно, 
        // нес<олоны" with the detailed description
        b_match = xstr_comment.b_RegexMatch (L"(.*)_кф_\\s*([<>\\-\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+)(.*)");
        if (b_match)
        {
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
        {
            b_match = b_CheckDifficulties (xstr_comment, st_entry);
            if (!b_match)
            {
                return false;
            }
        }
        b_match = xstr_comment.b_RegexMatch 
            (L"(_(сравн\\.|кф\\s*(мн\\.|м|ж|с))_\\s*[<>\\-\\w]+[;,]\\s*)*(_(сравн\\.|кф\\s*(мн\\.|м|ж|с))_\\s*[<>\\-\\w]+)");
        if (b_match)
        {
            if (b_alternative)
            {
                st_entry.xstr_AltIrregularForms = xstr_comment;  // slice
            }
            else
            {
                st_entry.xstr_IrregularForms = xstr_comment;     // slice
                xstr_descriptor = str_rest;
            }
            return true;
        }
    }       //  if (map_Pos[st_entry.str_MainSymbol] == POS_ADJ)

    if (map_Pos[st_entry.str_MainSymbol] == POS_VERB)
    {
        // _буд._ возд<ам, возд<ашь, возд<аст, воздад<им, воздад<ите, воздад<ут
        b_match = xstr_comment.b_RegexMatch 
            (L"(.*_)((буд\\.)|(наст\\.))_\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+)(.*)");
        if (b_match)
        {
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
        b_match = xstr_comment.b_RegexMatch 
            (L"(.*_)(буд\\.|наст\\.)_\\s*([<>\\w]+(у|ю)((сь)?)),\\s*([<>\\w]+)((,\\s*([<>\\w]+))?)(.*)");
        if (b_match)
        {
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
                str_3pl = xstr_comment.str_GetRegexMatch (9);
            }

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
        b_match = xstr_source.b_RegexMatch (L"(.*_)(повел\\.)_\\s*([<>\\w]+?(ь|и)(ся|сь)?)([\\s,;\\}\\]].*)");
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
        }

        // Expand "_прош._ возд<ал, воздал<а, возд<ало, возд<али"
        b_match = xstr_comment.b_RegexMatch 
            (L"(.*_)прош\\._\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+)(.*)");
        if (b_match)
        {
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
        b_match = xstr_comment.b_RegexMatch 
            (L"(.*_)прош\\._\\s*([<>\\w]+),\\s*([<>\\w]+)((,\\s*([<>\\w]+ий(ся)?))?)(.*)");
        if (b_match)
        {
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
        b_match = xstr_comment.b_RegexSearch (L"\\w\\/[<>\\w].*\\s\\-[<>\\w]");
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
        b_match = xstr_comment.b_RegexMatch 
            (L"(.*?_(прич\\.\\s*(прош|страд|наст)\\.)_\\s*[<>\\w]+)[,;]?\\s*\\(_кф_\\s*([<>\\-\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+),\\s*([<>\\w]+)\\)(.*)");
        if (b_match)
        {
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

        //b_match = xstr_comment.b_RegexMatch 
        //    (L"(_(прич\\.\\s*(страд\\.(\\s*(прош|наст)\\.)?|прош\\.(\\s*(мн\\.|м|ж|с))?|наст\\.)(\\s*кф\\s*(мн\\.|ж|м|с))?(\\s*ед\\.)?|деепр\\.(\\s*(прош|наст)\\.)|повел\\.|прош\\.\\s*(мн\\.|м|ж|с)(\\s*ед\\.)?|(наст|буд)\\.\\s*[123]\\s*(ед|мн)\\.|инф\\.)_\\s*[<>\\w][<>\\-\\w]*([,;]\\s*|$))*");
        //if (b_match)
        //{
            if (b_alternative)
            {
                st_entry.xstr_AltIrregularForms = xstr_comment;  // slice
            }
            else
            {
                st_entry.xstr_IrregularForms = xstr_comment;
                xstr_descriptor = str_rest;
            }

            return true;
        //}
    }       //  if (map_Pos[st_entry.str_MainSymbol] == POS_VERB)

    ERROR_LOG (L"CheckTriangle couldn't recognize the text: " + xstr_comment);
    return false;

}   //  b_CheckTriangle (...)

bool CT_ZalEntryParser::b_DetalizeDoubleSlashes (CT_ExtString& xstr_source)
{
    // _сравн._ (_затрудн._) г<ульче//г<улче
    wstring str_stem = L"", str_right = L"", str_left = L"";
    wstring str_matcher = L"^(.*?)(_[^_]*_(\\s*\\(_затрудн\\.?_\\))?)\\s*([^\\s\\/]*)\\s*\\/\\/\\s*([^\\s\\/_]+.*)$";
    bool b_match = xstr_source.b_RegexMatch(str_matcher);
    while (b_match)
    {
        xstr_source = xstr_source.str_GetRegexMatch(0) +
            xstr_source.str_GetRegexMatch(1) + L" " + xstr_source.str_GetRegexMatch(3) + L", " +
            xstr_source.str_GetRegexMatch(1) + L" " + xstr_source.str_GetRegexMatch(4);
        b_match = xstr_source.b_RegexMatch(str_matcher);
    }
    // _сравн._ г<ульче//_устар._ г<улче
    str_matcher = L"^(.*?)(_[^_]*_)\\s*([^\\s\\/]*)\\s*\\/\\/\\s*(_устар\\._|_затрудн\\._)\\s*([^\\s\\/]+.*)$";
    b_match = xstr_source.b_RegexMatch(str_matcher);
    while (b_match)
    {
        //ERROR_LOG(L"****b_DetalizeDoubleSlashes - begin: " + str_source);
        xstr_source = xstr_source.str_GetRegexMatch(0) +
            xstr_source.str_GetRegexMatch(1) + L" " + xstr_source.str_GetRegexMatch(2) + L", " +
            xstr_source.str_GetRegexMatch(1) + L" (" + xstr_source.str_GetRegexMatch(3) + L") " +
            xstr_source.str_GetRegexMatch(4);
        b_match = xstr_source.b_RegexMatch(str_matcher);
    }
    //ERROR_LOG(L"****b_DetalizeDoubleSlashes: " + str_source);
    return true;
}   //  b_DetalizeDoubleSlashes (...)

bool CT_ZalEntryParser::b_DetalizeSlashes (CT_ExtString& xstr_source)
{
    // _мн._ бр<ат/ья, -ьев, -ьям --> _мн._ бр<ат--ья, бр<ат--ьев, бр<ат--ьям
    // 1. (наст.|буд.)
    if (xstr_source.b_RegexMatch 
        (L"(.*?(_наст\\._|_буд\\._)\\s*)([\\w<>]+?)(у|ю|усь|юсь),\\s*\\-([\\w<>]+)(,\\s*\\-([\\w<>]+))?(.*)$"))
    {
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
    else if (xstr_source.b_RegexMatch (L"(.*?)([\\w<>\\-]+)\\/([\\w<>]+)(.*)"))
    {
        wstring str_left = xstr_source.str_GetRegexMatch(0) + 
                           xstr_source.str_GetRegexMatch(1) + 
                           L"--" + 
                           xstr_source.str_GetRegexMatch(2);
        CT_ExtString xstr_right = xstr_source.str_GetRegexMatch(3);
        wstring str_stem = xstr_source.str_GetRegexMatch(1);

        while (xstr_right.b_RegexMatch (L"([^\\-\\/]*)\\-([\\w<>]+)(.*)"))
        {
            str_left += xstr_right.str_GetRegexMatch(0) + str_stem + L"--" + xstr_right.str_GetRegexMatch(1);
            xstr_right = xstr_right.str_GetRegexMatch(2);
            if (xstr_right.b_RegexSearch(L"^[^\\-]*\\s[<>\\w]+[,;]?\\s"))
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
    else if (xstr_source.b_RegexMatch (L"(.*?_[^_\\s]*_)\\s*([\\w<>]+),\\s*\\-([\\w<>]+)(.*)"))
    {
        wstring str_stem = xstr_source.str_GetRegexMatch(1);
        wstring str_left = xstr_source.str_GetRegexMatch(0) + L" " + str_stem + L", " +
            str_stem + L"--" + xstr_source.str_GetRegexMatch(2);
        CT_ExtString xstr_right = xstr_source.str_GetRegexMatch(3);
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
    bool b_match = xstr_source.b_RegexMatch 
        (L"(.*_)((наст)|(буд)\\.)(\\s*[123]\\s*((ед)|(мн))\\._[^_]*_)\\s*([123]\\s*((ед)|(мн))\\._)(.*)");
    if (b_match)
    {
        wstring str_category = xstr_source.str_GetRegexMatch(1);
        wstring str_left = xstr_source.str_GetRegexMatch(0) + str_category + xstr_source.str_GetRegexMatch(4) +
            str_category + L" " + xstr_source.str_GetRegexMatch(8);
        CT_ExtString xstr_right = xstr_source.str_GetRegexMatch(12);
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

    // 1. невел<ик--<о --> невелик<о
    while (xstr_source.b_RegexMatch (L"(.*)<(\\w*)\\-\\-(\\w*<)(.*)"))
    {
        xstr_source = xstr_source.str_GetRegexMatch(0) + xstr_source.str_GetRegexMatch(1) + 
            xstr_source.str_GetRegexMatch(2) + xstr_source.str_GetRegexMatch(3);
        b_changes_made = true;
    }

    // 2. был--о --> б<ыло
    while (xstr_source.b_RegexMatch 
        (L"(.*?\\s)([бвгджзйклмнпрстфхцчшщъь]*)([аеёиоуыэюя][^\\-\\s<>]*)\\-\\-([^<]*[,;$].*)"))
    {
        xstr_source = xstr_source.str_GetRegexMatch(0) + xstr_source.str_GetRegexMatch(1) + L"<" + 
            xstr_source.str_GetRegexMatch (2) + xstr_source.str_GetRegexMatch (3);
        b_changes_made = true;
    }

    // 3. был--<а --> был<а
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
    b_match = xstr_descriptor.b_RegexSearch (L"^[^\\(]*(\\([^\\)]*\\)[^\\(]*)*\\:");
    if (!b_match)
    {
        return true;
    }

    // Handles:
    // ... :{...} ...
    b_match = xstr_descriptor.b_RegexMatch (L"^(.*?),?\\s*\\:\\s*\\{([^\\}]*)\\}\\s*(.*)$");
    if (b_match)
    {
        st_entry.str_Contexts = xstr_descriptor.str_GetRegexMatch(1);
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch(0) + xstr_descriptor.str_GetRegexMatch(2);
        return true;
    }

    // ... : ... [;|%|#|$ ...]
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
    //ERROR_LOG(L"CheckPercent: " + str_descriptor);
    if (wstring::npos == xstr_descriptor.find (L'%'))
    {
        return true;
    }
    bool b_match = xstr_descriptor.b_RegexMatch (L"^([^\\%]*?),?\\s*\\%\\{([^\\}]*)\\}(.*)$");
    if (b_match)
    {
        //ERROR_LOG(L"CheckPercent, %(): " + str_descriptor);
        st_entry.str_RestrictedForms = xstr_descriptor.str_GetRegexMatch (1);
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0) + xstr_descriptor.str_GetRegexMatch (2);
        return true;
    }

    b_match = xstr_descriptor.b_RegexMatch (L"^([^\\%]*?),?\\s*\\%\\s*([^#;\\:\\$]*)(.*)$");
    if (b_match)
    {
        st_entry.str_RestrictedForms = xstr_descriptor.str_GetRegexMatch (1);
        //ERROR_LOG(L"CheckPercent was started on " + str_descriptor + L", result is " + st_entry.str_RestrictedForms);
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

    bool b_match = xstr_descriptor.b_RegexSearch (L"[^\\$]*(затрудн\\.|нет|устар\\.|простореч\\.)_");
    if (!b_match)
    {
        return true;
    }

    // _Р. мн. затрудн._, etc
    wstring str_match = L"(.*?)[;,]?\\s*_([^_]+?)\\s*затрудн\\._(.*)";
    b_match = xstr_descriptor.b_RegexMatch (str_match);
    while (b_match)
    {
        wstring str_left = xstr_descriptor.str_GetRegexMatch(0);
        wstring str_right = xstr_descriptor.str_GetRegexMatch(2);
        CT_ExtString xstr_target (xstr_descriptor.str_GetRegexMatch(1));
        b_match = xstr_target.b_RegexMatch (L"(.*)\\sи\\s(.*)");
        if (b_match)
        {
            CT_ExtString xstr_gramm (xstr_target.str_GetRegexMatch (0));
            b_match = xstr_gramm.b_RegexMatch (L"(.*)\\s(м|ж|с|ед\\.|мн\\.)");
            if (b_match)
            {
                st_entry.xstr_Difficulties += xstr_gramm.str_GetRegexMatch(0) + L" " +
                    xstr_gramm.str_GetRegexMatch(1) + L", " + 
                    xstr_target.str_GetRegexMatch(1) + L", ";
                xstr_descriptor = str_left + str_right;
            }
            else
            {
                st_entry.xstr_Difficulties += xstr_gramm.str_GetRegexMatch(0) + L", " + 
                    xstr_gramm.str_GetRegexMatch(1) + L",";
                xstr_descriptor = str_left + str_right;
            }
        }
        else
        {
            st_entry.xstr_Difficulties += xstr_target + L",";
            xstr_descriptor = str_left + str_right;
        }

        b_match = xstr_descriptor.b_RegexMatch (str_match);  
    }   //  while (b_match)
    
    // _Р. мн. устар._
    str_match = L"(.*?)[;,]?\\s*_([^_]+?)\\s*устар\\._(.*)";
    b_match = xstr_descriptor.b_RegexMatch (str_match);
    while (b_match)
    {
        wstring str_left = xstr_descriptor.str_GetRegexMatch(0);
        wstring str_right = xstr_descriptor.str_GetRegexMatch(2);
        st_entry.xstr_Obsolete += xstr_descriptor.str_GetRegexMatch(1) + L",";
        xstr_descriptor = str_left + str_right;
        b_match = xstr_descriptor.b_RegexMatch (str_match);  
    }

    // _Р. мн._ (_затрудн._) ...
    // We should also memorize the form itself because there can be several
    // different forms with the same grammatical parameters, only one of which
    // being "difficult"
    str_match = L"(.*_([^_]*)_\\s*)\\(_затрудн\\._\\)\\s*([\\w<>\\-]+)(.*)";
    b_match = xstr_descriptor.b_RegexMatch (str_match);
    while (b_match)
    {
        st_entry.xstr_Difficulties += xstr_descriptor.str_GetRegexMatch (1) + L" (" +
                                     xstr_descriptor.str_GetRegexMatch (2) + L"), ";
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0) +
                          xstr_descriptor.str_GetRegexMatch (2) +
                          xstr_descriptor.str_GetRegexMatch (3);
        b_match = xstr_descriptor.b_RegexMatch (str_match);
    }

    // _Р. мн._ (_устар._) ...
    // We should also memorize the form itself because there can be several
    // different forms with the same grammatical parameters, only one of which
    // being obsolete
    str_match = L"(.*_([^_]*)_\\s*)\\(_устар\\._\\)\\s*([\\w<>\\-]+)(.*)";
    b_match = xstr_descriptor.b_RegexMatch (str_match);
    while (b_match)
    {
        st_entry.xstr_Obsolete += xstr_descriptor.str_GetRegexMatch (1) + L" (" +
                                  xstr_descriptor.str_GetRegexMatch (2) + L"), ";
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0) +
                          xstr_descriptor.str_GetRegexMatch (2) +
                          xstr_descriptor.str_GetRegexMatch (3);
        b_match = xstr_descriptor.b_RegexMatch (str_match);
    }

    // _повел._ (_простореч._) ...
    // We should also memorize the form itself because there can be several
    // different forms with the same grammatical parameters, only one of which
    // is informal
    str_match = L"([^\\$]*_([^_]*)_\\s*)\\(_простореч\\._\\)\\s*([\\w<>\\-]+)(.*)";
    b_match = xstr_descriptor.b_RegexMatch (str_match);
    while (b_match)
    {
        st_entry.xstr_Colloquial += xstr_descriptor.str_GetRegexMatch (1) + L" (" +
                                    xstr_descriptor.str_GetRegexMatch (2) + L"), ";
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0) +
                          xstr_descriptor.str_GetRegexMatch (2) +
                          xstr_descriptor.str_GetRegexMatch (3);
        b_match = xstr_descriptor.b_RegexMatch (str_match);
    }

    // _пф нет_
    //st_entry.str_Deficient = L"";
    str_match = L"(.*?)[;,]?\\s*_([^_]+?)\\s*нет_(.*)";
    b_match = xstr_descriptor.b_RegexMatch (str_match);
    while (b_match)
    {
        st_entry.xstr_Deficient += xstr_descriptor.str_GetRegexMatch (1) + L", ";
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0) + xstr_descriptor.str_GetRegexMatch (2);
        b_match = xstr_descriptor.b_RegexMatch (str_match);
    }

    // Finally, check if we've done everything we had to
    return !xstr_descriptor.b_RegexSearch (L"(затрудн\\.|[\\s_]нет|\\(_устар\\.)_|^[^\\$]*_простореч\\._");

}   // b_CheckDifficulties (...)

bool CT_ZalEntryParser::b_CheckInflectedParts (CT_ExtString& xstr_descriptor, ST_Descriptor& st_entry)
{
    // Handles cases:
    // ..., INFL=1
    // ..., INFL=2
    if (xstr_descriptor.length() <= 0)
    {
        return true;
    }
    
    bool b_match = xstr_descriptor.b_RegexMatch (L"^(.*?),?\\s*INFL=1(.*)$");
    if (b_match)
    {
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0) + xstr_descriptor.str_GetRegexMatch (1);
        st_entry.st_InflectionGroup.i_InflectedParts = 1;
    }

    b_match = xstr_descriptor.b_RegexMatch (L"^(.*?),?\\s*INFL=2(.*)$");
    if (b_match)
    {
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0) + xstr_descriptor.str_GetRegexMatch (1);
        st_entry.st_InflectionGroup.i_InflectedParts = 2;
    }
    return true;

}   //  b_CheckInflectedParts (...)
