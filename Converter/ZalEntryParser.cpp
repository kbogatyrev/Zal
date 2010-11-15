#include "StdAfx.h"
#include "ExtString.h"
#include "LexemeHasher.h"
#include "ZalEntryParser.h"

void CT_ZalEntryParser::v_Init()
{
    wstring arr_strMainSymbol[] = { L"м", L"мо", L"ж", L"жо", L"с", L"со", L"мо-жо", L"мн.",
     L"мн. неод.", L"мн. одуш.", L"мн. _от_", L"п", L"мс", L"мс-п", L"числ.", L"числ.-п", 
     L"св", L"нсв", L"св-нсв", L"н", L"предл.", L"союз", L"предик.", L"вводн.", L"сравн.", 
     L"част.", L"межд."  };

    for (ET_MainSymbol eo_ms = MS_START; eo_ms < MS_END; ++eo_ms)
    {
        map_MainSymbol[arr_strMainSymbol[eo_ms]] = eo_ms; 
    }

    for (int i_ms = 0; i_ms < (int)MS_END; ++i_ms)
    {
        wstring str_ms = arr_strMainSymbol[i_ms];
        ET_MainSymbol eo_ms = map_MainSymbol[str_ms];
        switch (eo_ms)
        {
            case MS_M:
            case MS_MO:
            case MS_ZH:
            case MS_ZHO:
            case MS_S:
            case MS_SO:
            case MS_MO_ZHO:
            case MS_MN:
            case MS_MN_NEOD:
            case MS_MN_ODUSH:
            case MS_MN_OT:
            {
                map_Pos[str_ms] = POS_NOUN;
                break;
            }

            case MS_SV:
            case MS_NSV:
            case MS_SV_NSV:
            {
                map_Pos[str_ms] = POS_VERB;
                break;
            }

            case MS_P:
            {
                map_Pos[str_ms] = POS_ADJ;
                break;
            }

            case MS_MS:
            case MS_MS_P:
            {
                map_Pos[str_ms] = POS_PRONOUN;
                break;
            }

            case MS_N:
            {
                map_Pos[str_ms] = POS_ADV;
                break;
            }

            case MS_CHISL:
            case MS_CHISL_P:
            {
                map_Pos[str_ms] = POS_NUM;
                break;
            }

            case MS_CHAST:
            {
                map_Pos[str_ms] = POS_PARTICLE;
                break;
            }

            case MS_PREDL:
            {
                map_Pos[str_ms] = POS_PREP;
                break;
            }

            case MS_SOJUZ:
            {
                map_Pos[str_ms] = POS_CONJ;
                break;
            }

            case MS_VVODN:
            {
                map_Pos[str_ms] = POS_PARENTH;
                break;
            }

            case MS_PREDIK:
            {
                map_Pos[str_ms] = POS_PREDIC;
                break;
            }

            case MS_MEZHD:
            {
                map_Pos[str_ms] = POS_INTERJ;
                break;
            }

            case MS_SRAVN:
            {
                map_Pos[str_ms] = POS_COMPAR;
                break;
            }

            default:
            {
                ATLASSERT(0);
                ERROR_LOG (L"Unable to initialize POS map.");
                return;
            }

        }    // switch

    }   //  for (int i_ms = 0; ... )

    wstring arr_strAccent[] = { L"а", L"а'", L"в", L"в'", L"с", L"с'", L"с''", L"D", L"D'", L"е", L"F", L"F'", L"F''" };

    for (ET_AccentType eo_at = AT_A; eo_at < AT_COUNT; ++eo_at)
    {
        map_AccentType[arr_strAccent[eo_at-1]] = eo_at;
    }

    b_CommaAfterMainSymbol = false;

}   //  v_Init()

bool CT_ZalEntryParser::b_Parse (const wstring& str_head, const wstring str_usageComment)
{
    wstring arr_strMainSymbol[] = { L"м", L"мо", L"ж", L"жо", L"с", L"со", L"мо-жо", L"мн.",
     L"мн. неод.", L"мн. одуш.", L"мн. _от_", L"п", L"мс", L"мс-п", L"числ.", L"числ.-п", 
     L"св", L"нсв", L"св-нсв", L"н", L"предл.", L"союз", L"предик.", L"вводн.", L"сравн.", 
     L"част.", L"межд."  };

    ST_Headword st_head;
    st_head.str_Usage = str_usageComment;

//    wstring str_entry (str_Source);
    CT_ExtString xstr_entry (str_Source);

    if (!str_head.empty())
    {
        ATLASSERT (b_SecondPass);
        st_head.str_Source = str_head;
    }
    else
    {
        bool b_ = b_ExtractHead (xstr_entry, st_head);
        if (!b_)
        {
            return false;
        }
    }

    // First, we split the entry into parts divided by semicolons.
    //
    // We should omit semicolons in parentheses or curly brackets.
    // Here, we take into account than parentheses can occur inside
    // curly brackets, whereas the converse doesn't hold.

    vector<wstring> vec_strParts;
//    wsmatch ct_result;
    wstring str_left;
    const wstring str_semicolonMatcher 
        (L"^(([^\\{]*?\\{[^\\}]*?\\})*?([^\\{\\(]*\\([^\\)]*?\\))*?[^\\{\\(;]*);(\\s*.+)");

    bool b_semicolon = xstr_entry.b_RegexMatch (str_semicolonMatcher);
    //    bool b_semicolon = regex_match(str_entry, ct_result, (const wregex)L"^(([^\\{]*?\\{[^\\}]*?\\})*?([^\\{\\(]*\\([^\\)]*?\\))*?[^\\{\\(;]*);(\\s*.+)");
    if (!b_semicolon)
    {
        vec_strParts.push_back (xstr_entry.c_str());
        xstr_entry.erase();
    }

    while (b_semicolon)
    {
//        str_left = ct_result[1];
//        str_entry = ct_result[4];
        str_left = xstr_entry.str_GetRegexMatch (0);
        xstr_entry = xstr_entry.str_GetRegexMatch (3);
        bool b_goodSemicolon = false;
//        bool b_match = regex_match(str_entry, ct_result, (const wregex)L"^\\s*([^\\s;:,\\/]*).*");
        bool b_match = xstr_entry.b_RegexMatch (L"^\\s*([^\\s;:,\\/]*).*");
        if (!b_match)
        {
            ATLASSERT(0);
            ERROR_LOG (L"No word after semicolon: " + xstr_entry);
            return false;
        }

        CT_ExtString xstr_firstWord = xstr_entry.str_GetRegexMatch (0);
//        wstring str_FirstWord = ct_result[1];
        for (int i_ms = 0; i_ms < (int)MS_END; ++i_ms)
        {
            if (xstr_firstWord == arr_strMainSymbol[i_ms])
            {
                b_goodSemicolon = true;
                break;
            }
        }

//        if (regex_match(str_FirstWord, (const wregex)L"^\\s*\\d"))
        if (xstr_firstWord.b_RegexMatch (L"^\\s*\\d"))
        {
            b_goodSemicolon = true;
        }
        while (!b_goodSemicolon)
        {
            // Then it's a semicolon with some other meaning,
            // so we have to continue our search
//            b_semicolon = regex_match(str_entry, ct_result, (const wregex)L"^(([^\\{]*?\\{[^\\}]*?\\})*?([^\\{\\(]*\\([^\\)]*?\\))*?[^\\{\\(;]*);(\\s*.+)");
            b_semicolon = xstr_entry.b_RegexMatch (str_semicolonMatcher);
            if (!b_semicolon)
            {
                // we reached the end of the string and haven't found a good semicolon
                str_left += L";" + xstr_entry;
                xstr_entry.erase();
                break;
            }

//            wstring str_middle = ct_result[1];
//            str_entry = ct_result[4];
//            str_left += L";" + str_middle;
            wstring str_middle = xstr_entry.str_GetRegexMatch (0);
            xstr_entry = xstr_entry.str_GetRegexMatch (3);
            str_left += L";" + str_middle;
//            b_match = regex_match(str_entry, ct_result, (const wregex)L"^\\s*([^\\s;:,\\/]*).*");
            b_match = xstr_entry.b_RegexMatch (L"^\\s*([^\\s;:,\\/]*).*");
            if (!b_match)
            {
                ERROR_LOG(L"No word after semicolon: " + xstr_entry);
                return false;
            }

//            str_FirstWord = ct_result[1];
            xstr_firstWord = xstr_entry.str_GetRegexMatch (0);
            for (int i_ms = 0; i_ms < (int)MS_END; ++i_ms)
            {
                if (arr_strMainSymbol[i_ms] == xstr_firstWord)
                {
                    b_goodSemicolon = true;
                    break;
                }
            }

            if (xstr_firstWord.b_RegexMatch (L"^\\s*\\d.*"))
//            if (regex_match(str_FirstWord, (const wregex)L"^\\s*\\d.*"))
            {
                b_goodSemicolon = true;
            }
        }
        // Here, we have yet another portion of the entry (str_left)
        // and the rest of it (str_entry).
        vec_strParts.push_back (str_left);
        if (xstr_entry.length() <= 0)
        {
            break;
        }

//        b_semicolon = regex_match(str_entry, ct_result, (const wregex)L"^(([^\\{]*?\\{[^\\}]*?\\})*?([^\\{\\(]*\\([^\\)]*?\\))*?[^\\{\\(;]*);(\\s*.+)");
        b_semicolon = xstr_entry.b_RegexMatch (str_semicolonMatcher);
    }

    if (xstr_entry.length() > 0)
    {
        vec_strParts.push_back (xstr_entry.c_str());
    }

    // Now, we can handle each part separately

    //for (int i_at = 0; !str_entry.empty(); ++i_at)
    for (vector<wstring>::iterator iter_parts = vec_strParts.begin();
         iter_parts != vec_strParts.end();
         ++iter_parts)
    {
        xstr_entry = *iter_parts;
//        CT_ExtString xstr_descriptor (str_entry, L" ");
        xstr_entry.v_SetBreakChars (L" ");
        xstr_entry.v_Trim();
//        xstr_descriptor.v_Trim();
//        if (L';' == xstr_descriptor[0])
        if (L';' == xstr_entry[0])
        {
            //if (i_at < 1)
            //{
                ATLASSERT(0);
                wstring str_msg (L"Unexpected \';\' at entry start: ");
                str_msg += str_Source;
                ERROR_LOG (str_msg);
                return false;
            //}
            //str_entry = xstr_descriptor.substr (1);
        }
//        else
//        {
//            str_entry = xstr_descriptor.c_str();
//        }

        ST_Descriptor st_entry;

        bool b_ = b_CheckMnOt (xstr_entry, st_head, st_entry);
        if (!b_)
        {
            return false;
        }
        
        b_ = b_GetMainSymbol (xstr_entry, st_entry);
        if (!b_)
        {
            return false;
        }

        b_ = b_CheckInflectionType (xstr_entry, st_entry);
        if (!b_)
        {
            return false;
        }

        b_ = b_ParseInflectionGroup (xstr_entry, st_head, st_entry);
        if (!b_)
        {
            return false;
        }

        if (xstr_entry.find (L"//") == 0)
        {
            xstr_entry = xstr_entry.substr (2);
            b_ = b_ParseInflectionGroup (xstr_entry, st_head, st_entry);
            if (!b_)
            {
                return false;
            }
        }
        
//        xstr_descriptor = str_entry;
//        xstr_descriptor.v_Reset();
        xstr_entry.v_Reset();
//        xstr_descriptor.v_SetBreakChars (L", ");
        xstr_entry.v_SetBreakChars (L" ");
//        xstr_descriptor.v_Trim();
        xstr_entry.v_Trim();
//        xstr_descriptor.v_Reset();
        xstr_entry.v_Reset();
//        bool b_match = xstr_descriptor.b_RegexMatch (L"^(?:\\(\\-([щсдтбгкнми]+)\\-\\))(.*)");
        bool b_match = xstr_entry.b_RegexMatch (L"^(?:\\(\\-([щсдтбгкнми]+)\\-\\))(.*)");
        if (b_match)
        {
//            st_entry.str_VerbAlternation = xstr_descriptor.str_GetRegexMatch (0);
            st_entry.str_VerbAlternation = xstr_entry.str_GetRegexMatch (0);
//            str_entry = xstr_descriptor.str_GetRegexMatch (1);
            xstr_entry = xstr_entry.str_GetRegexMatch (1);
        }
        else
        {
            b_match = 
                xstr_entry.b_RegexMatch (L"^(?:\\(([абвгдеёжзийклмнопрстуфхцчшщъыьэюя]+)\\-\\))(.*)");
//                xstr_descriptor.b_RegexMatch (L"^(?:\\(([абвгдеёжзийклмнопрстуфхцчшщъыьэюя]+)\\-\\))(.*)");
            if (b_match)
            {
                if ((L"св" != st_entry.str_MainSymbol &&
                     L"нсв" != st_entry.str_AltMainSymbol &&
                     L"св-нсв" != st_entry.str_MainSymbol))
                {
                    ATLASSERT(0);
                    wstring str_msg (L"This type of stem alternation is expected for verbs only: ");
                    str_msg += str_Source;
                    ERROR_LOG (str_msg);
                    return false;
                }
                if (14 != st_entry.st_InflectionGroup.i_Type)
                {
                    ATLASSERT(0);
                    wstring str_msg (L"This type of verb stem alternation is expected for type 14 only.");
                    str_msg += str_Source;
                    ERROR_LOG (str_msg);
                    return false;
                }
//                st_entry.str_VerbAlternation = xstr_descriptor.str_GetRegexMatch (0);
                st_entry.str_VerbAlternation = xstr_entry.str_GetRegexMatch (0);
//                str_entry = xstr_descriptor.str_GetRegexMatch (1);
                xstr_entry = xstr_entry.str_GetRegexMatch (1);
            }
            else
            {
//                if (xstr_descriptor.b_StartsWith (L"_прич. страд._ -жд-"))
                if (xstr_entry.b_StartsWith (L"_прич. страд._ -жд-"))
                {
                    st_entry.b_PastPartPassZhD = true;
                }
            }
        }

        // Here we have to repeat common deviations check since sometimes
        // a circled digit may appear after stem alternation 
//        b_ = b_CheckCircledDigits (str_entry, st_entry.st_InflectionGroup);
        b_ = b_CheckCircledDigits (xstr_entry, st_entry.st_InflectionGroup);
        if (!b_)
        {
            return false;
        }

//        b_ = b_CheckTilde (str_entry, st_entry);
        b_ = b_CheckTilde (xstr_entry, st_entry);
        if (!b_)
        {
            return false;
        }

        //if (L'-' == str_entry[0])
        //{
        //    st_entry.b_AssumedForms = true;
        //    CT_ExtString xstr_descriptor (str_entry.substr (1), L" ");
        //    xstr_descriptor.v_Trim();
        //    str_entry = xstr_descriptor.c_str();
        //}

        if (L'-' == xstr_entry[0])
        {
            st_entry.b_AssumedForms = true;
            xstr_entry.erase (0, 1);
            xstr_entry.v_SetBreakChars (L" ");
            xstr_entry.v_Trim();
        }

        b_ = b_CheckCognate (xstr_entry, st_entry);
        if (!b_)
        {
            return false;
        }

        b_ = b_CheckInflectedParts (xstr_entry, st_entry);
        if (!b_)
        {
            return false;
        }

        b_ = b_CheckVerbType (xstr_entry, st_entry);
        if (!b_)
        {
            return false;
        }

        b_ = b_CheckSharp (xstr_entry, st_entry);
        if (!b_)
        {
            return false;
        }

        b_ = b_CheckTriangle (st_head, xstr_entry, st_entry);
        if (!b_)
        {
            return false;
        }
        if (!st_entry.str_IrregularForms.empty())
        {
            st_entry.b_HasIrregularForms = true;
        }
        
		b_ = b_CheckDifficulties (xstr_entry, st_entry); // those outside the triangle group
        if (!b_)
        {
            return false;
        }

		b_ = b_CheckColon (xstr_entry, st_entry);
		if (!b_)
        {
            return false;
        }

        b_ = b_CheckComma (xstr_entry, st_entry);
		if (!b_)
        {
            return false;
        }

        b_ = b_CheckDollar (xstr_entry, st_entry);
		if (!b_)
        {
            return false;
        }

		b_ = b_CheckPercent (xstr_entry, st_entry);
		if (!b_)
        {
            return false;
        }

        b_ = b_CheckSquareBrackets (xstr_entry, st_head, st_entry);
        if (!b_)
        {
            return false;
        }

        if (!st_head.str_SeeAlso.empty())
        {
//&&&&
            wstring str_msg (L"WARNING: ignoring ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return true;
        }

        // Trim whitespaces at both ends
//        wsmatch ct_result;
//        if (regex_match(str_entry, ct_result, (const wregex)L"^\\s*(.*?)\\s*$"))
//        {
//            str_entry = ct_result[1];
//        }
        xstr_entry.v_Trim();

//        xstr_descriptor = str_entry;
//        b_ = xstr_descriptor.b_RegexMatch (L"\\(_(.+?)_\\)(.*)");
        b_ = xstr_entry.b_RegexMatch (L"\\(_(.+?)_\\)(.*)");
        if (b_)
        {
            ATLASSERT (xstr_entry.i_GetNumOfRegexMatches() > 0);
//            st_entry.str_TrailingComment = xstr_descriptor.str_GetRegexMatch (0);
            st_entry.str_TrailingComment = xstr_entry.str_GetRegexMatch (0);
//            if (xstr_descriptor.i_GetNumOfRegexMatches() > 1)
            if (xstr_entry.i_GetNumOfRegexMatches() > 1)
            {
//                str_entry = xstr_descriptor.str_GetRegexMatch (1);
                xstr_entry = xstr_entry.str_GetRegexMatch (1);
            }
            else
            {
                xstr_entry.erase();
            }
        }

        b_ = b_MakeGraphicStem (st_entry, st_head);
        vec_Descriptors.push_back (st_entry);

        if (xstr_entry.length() > 0)
        {
            ERROR_LOG(L"WARNING: Something still unparsed in the end: " + xstr_entry);
//            return false;
        }
    }   //  for (vector<wstring>::iterator iter_parts ...

    b_SaveToDb (st_head, vec_Descriptors);

    return true;

}   //  i_Parse (...)

//bool CT_ZalEntryParser::b_ExtractHead (wstring& str_source)
//bool CT_ZalEntryParser::b_ExtractHead (CT_ExtString& xstr_source)
bool CT_ZalEntryParser::b_ExtractHead (CT_ExtString& xstr_source, ST_Headword& st_head)
{
//    CT_ExtString xstr_source (str_source, L" ");
    xstr_source.v_SetBreakChars (L" ");
    if (xstr_source.i_NFields() < 1)
    {
        ATLASSERT(0);
        wstring str_msg (L"Too few fields: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    CT_ExtString xstr_head (xstr_source.str_GetField (0));
    bool b_match = xstr_head.b_RegexMatch (L"^(?:([1-5])?(?:,([1-5]))*/)?([^ ]+)");

    wstring str_digit;
    for (int i_ = 0; i_ < xstr_head.i_NRegexMatches() - 1; ++i_)
    {
        str_digit = xstr_head.str_GetRegexMatch (i_);
        if (str_digit.length() != 1)
        {
//            ATLASSERT(0);
//            wstring str_msg (L"Bad homonym number: ");
//            str_msg += str_Source;
//            ERROR_LOG (str_msg);
//            return false;
            continue;
        }
        if (!iswdigit (str_digit[0]))
        {
            ATLASSERT(0);
            wstring str_msg (L"Homonym number is not numeric: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }
        st_head.vec_IHomonyms.push_back (_wtoi (&str_digit[0]));
    }

    if (xstr_head.i_NRegexMatches() < 1)
    {
        ATLASSERT(0);
        wstring str_msg (L"No headword: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    CT_ExtString xstr_headword (xstr_head.str_GetRegexMatch (xstr_head.i_NRegexMatches()-1));
    st_head.str_RawSource = xstr_headword;        // slice

    bool b_ret = b_ExtractStress (xstr_headword, st_head, st_head.map_Stress);

    if (xstr_source.i_NFields() < 2)
    {
        return true;
    }

    xstr_source = xstr_source.substr (xstr_source.i_GetFieldOffset (1));
    b_match = xstr_source.b_RegexMatch (L"^(?:\\((.+?)\\)\\s)(.+)");
    if (b_match)
    {
        if (xstr_source.i_NRegexMatches() != 2)
        {
            ATLASSERT(0);
            wstring str_msg (L"Error matching parenthetical expression: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }
        st_head.str_Comment = xstr_source.str_GetRegexMatch (0);
        xstr_source = CT_ExtString (xstr_source.str_GetRegexMatch (1));
    }

//    str_source = xstr_source.c_str();

    return true;

}   //  b_ExtractHead()

//bool CT_ZalEntryParser::b_ExtractStress (const wstring& str_headWord, map<int, ET_StressType>& map_stress)
bool CT_ZalEntryParser::b_ExtractStress (CT_ExtString& xstr_headWord, 
                                         ST_Headword& st_head,
                                         map<int, ET_StressType>& map_stress)
{
//    CT_ExtString xstr_headWord (str_headWord, L"<>");
    xstr_headWord.v_SetBreakChars (L"<>");

    for (int i_textField = 0; i_textField < xstr_headWord.i_GetNumOfFields(); ++i_textField)
    {
        st_head.str_Source += xstr_headWord.str_GetField (i_textField);
    }

    int i_marksFound = 0;
    for (int i_breakField = 0; 
         i_breakField < xstr_headWord.i_GetNumOfFields (CT_ExtString::ec_TokenBreakChars); 
         ++i_breakField)
    {
        wstring str_stressMark = xstr_headWord.str_GetField (i_breakField, CT_ExtString::ec_TokenBreakChars);
        if (str_stressMark.length() != 1)
        {
            ATLASSERT(0);
            wstring str_msg (L"Error processing stress marks: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }

        int i_pos = xstr_headWord.i_GetFieldOffset (i_breakField, CT_ExtString::ec_TokenBreakChars);
        i_pos -= i_marksFound;
        ++i_marksFound;

        if (L"<" == str_stressMark)
        {
            map_stress[i_pos] = STRESS_PRIMARY;
        }
        else if (L">" == str_stressMark)
        {
            map_stress[i_pos] = STRESS_SECONDARY;
        }
        else
        {
            ATLASSERT(0);
            wstring str_msg (L"Error extracting stress marks: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }

    }   //  for (int i_breakField = 0; ...)

    return true;

//    if (xstr_source.i_NFields() < 2)
//    {
//        ATLASSERT(0);
//        wstring str_msg (L"Expected field is not found: ");
//        str_msg += str_Source;
//        ERROR_LOG (str_msg);
//        return false;
//    }

}   //  b_ExtractStress (...)

//bool CT_ZalEntryParser::b_CheckMnOt (wstring& str_source, ST_Descriptor& st_entry)
bool CT_ZalEntryParser::b_CheckMnOt (CT_ExtString& xstr_source, ST_Headword& st_head, ST_Descriptor& st_entry)
{
    st_entry.b_PluralOf = false;

//    CT_ExtString xstr_source (str_source, L" /,;:");
    xstr_source.v_SetBreakChars (L" /,;:");
    if (xstr_source.empty())
    {
        ATLASSERT(0);
        wstring str_msg (L"No main symbol: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    wstring str_main (xstr_source.str_GetField (0));
    if (map_Pos.find (str_main) == map_Pos.end())
    {
        ATLASSERT(0);
        wstring str_msg (L"Unknown main symbol: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    if (L"мн." != str_main)
    {
        return true;
    }

    if (xstr_source.i_NFields() < 2)
    {
        ATLASSERT(0);
        wstring str_msg (L"Unterminated \"мн.\": ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    wstring str_nextToken (xstr_source.str_GetField (1));        
    if (xstr_source.find (L"_от_") != xstr_source.i_GetFieldOffset (1))
    {
        return true;    // nothing to do
    }

    if (xstr_source.i_NFields() < 3)
    {
        ATLASSERT(0);
        wstring str_msg (L"Unterminated \"мн. _от_\": ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    st_entry.b_PluralOf = true;

    if (map_Pos.find (xstr_source.str_GetField(2)) != map_Pos.end())
    {
        int i_offset = xstr_source.i_GetFieldOffset(1) + xstr_source.i_GetFieldLength(1);
        xstr_source = xstr_source.substr (i_offset);

        return true;
    }

    st_head.str_PluralOf = xstr_source.str_GetField(2);
    if (xstr_source.i_NFields() < 4)
    {
        ATLASSERT(0);
        wstring str_msg (L"Unterminated \"мн. _от_ <headword>\": ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    int i_offset = xstr_source.i_GetFieldOffset (2) + xstr_source.i_GetFieldLength (2);
    xstr_source = xstr_source.substr (i_offset);
    
    return true;

}   //  b_CheckMnOt (...)

//bool CT_ZalEntryParser::b_GetMainSymbol(wstring& str_source, ST_Descriptor& st_entry)
bool CT_ZalEntryParser::b_GetMainSymbol(CT_ExtString& xstr_descriptor, ST_Descriptor& st_entry)
{
//    CT_ExtString xstr_descriptor (str_source, L" ;:");
    xstr_descriptor.v_SetBreakChars (L" ;:");
    
    int i_nextField = 1;
    CT_ExtString xstr_mainSymb (xstr_descriptor.str_GetField (0), L"/,;");
    if (L"мн." == xstr_mainSymb)
    {
        if (xstr_descriptor.i_NFields() > 1)
        {
            CT_ExtString xstr_next (xstr_descriptor.str_GetField (1), L"/,;");
            xstr_next.v_Trim();
            if ((L"неод." == xstr_next) || (L"одуш." == xstr_next))
            {
                xstr_mainSymb += L" " + xstr_next;
                ++i_nextField;
            }
        }
    }

    if ((L"св" == xstr_mainSymb) || (L"нсв" == xstr_mainSymb) || (L"св-нсв" == xstr_mainSymb))
    {
        if (xstr_descriptor.i_NFields() > 1)
        {
            CT_ExtString xstr_next (xstr_descriptor.str_GetField (1), L"/,;");
            xstr_next.v_Trim();
            if (L"нп" == xstr_next)
            {
                st_entry.b_Intransitive = true;
                ++i_nextField;
            }
        }
    }

    if (xstr_descriptor.i_NFields() < i_nextField + 1)
    {
        xstr_descriptor.erase();
    }
    else
    {
        int i_offset = xstr_descriptor.i_GetFieldOffset (i_nextField - 1) + 
                       xstr_descriptor.i_GetFieldLength (i_nextField - 1);
        xstr_descriptor = xstr_descriptor.substr (i_offset);
    }

    st_entry.str_MainSymbol = xstr_mainSymb.str_GetField (0);
    if (map_Pos.find (st_entry.str_MainSymbol) == map_Pos.end())
    {
        ATLASSERT(0);
        wstring str_msg (L"Unidentified main symbol: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }
    if (xstr_mainSymb.i_NFields() > 1)
    {
        CT_ExtString xstr_alternate (xstr_mainSymb.str_GetField (1));
        bool b_match = xstr_alternate.b_RegexMatch (L"_(.+?)_.*");
        if (b_match)
        {
            st_entry.str_AltMainSymbolComment = xstr_alternate.str_GetRegexMatch (0);
            int i_offset = xstr_alternate.i_GetFieldOffset (0, CT_ExtString::ec_TokenRegexMatch);
            int i_length = xstr_alternate.i_GetFieldLength (0, CT_ExtString::ec_TokenRegexMatch);
            wstring str_left = xstr_alternate.substr (0, i_offset);
            wstring str_right = xstr_alternate.substr (i_offset + i_length);
            xstr_alternate = str_left + str_right;

            xstr_alternate.v_Reset();
            xstr_alternate.v_SetBreakChars (L" ");
            xstr_alternate.v_Trim();
        }

        st_entry.str_AltMainSymbol = xstr_alternate.c_str();
        if (map_Pos.find (st_entry.str_AltMainSymbol) == map_Pos.end())
        {
            ATLASSERT(0);
            wstring str_msg (L"Unidentified alternate main symbol: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }
    }

    st_entry.e_PartOfSpeech = map_Pos[st_entry.str_MainSymbol];    // and alt. main symbol?
//    st_entry.e_MainSymbol = map_MainSymbol[st_entry.str_MainSymbol];
    st_entry.e_InflectionSymbol = map_MainSymbol[st_entry.str_MainSymbol];

    return true;

}   //  b_GetMainSymbol (...)

//bool CT_ZalEntryParser::b_CheckInflectionType (wstring& str_source, ST_Descriptor& st_entry)
bool CT_ZalEntryParser::b_CheckInflectionType (CT_ExtString& xstr_source, ST_Descriptor& st_entry)
{
//    CT_ExtString xstr_source (str_source, L" ");
    xstr_source.v_SetBreakChars (L" ");
    xstr_source.v_Trim();
//    str_source = xstr_source.c_str();

    st_entry.str_InflectionSymbol = st_entry.str_MainSymbol;  // default

    // Extract inflection type (if different from main symbol, 
    // e.g., б<абий п <мс 6*а>)
    bool b_match = xstr_source.b_RegexMatch (L"(<(.+?)>).*");
    if (!b_match)
    {
        return true;
    }

    CT_ExtString xstr_descriptor (xstr_source.str_GetRegexMatch (1), L" ,");
    CT_ExtString xstr_inflection (xstr_descriptor.str_GetField(0));
    xstr_inflection.v_SetBreakChars (L"/");
    st_entry.str_InflectionSymbol = xstr_inflection.str_GetField (0);
    if (xstr_inflection.i_NFields() < 1 || xstr_inflection.i_NFields() > 2)
    {    
        ATLASSERT (0);
        wstring str_msg (L"Error parsing inflection type : ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    if (map_Pos.find (st_entry.str_InflectionSymbol) == map_Pos.end())
    {
        ATLASSERT(0);
        wstring str_msg (L"Unidentified main symbol: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    if (xstr_inflection.i_NFields() > 1)
    {
        st_entry.str_AltInflectionSymbol = xstr_inflection.str_GetField (1);
    }

    st_entry.e_InflectionSymbol = map_MainSymbol[st_entry.str_InflectionSymbol];

    wstring str_left;
    if (xstr_descriptor.i_NFields() > 1)
    {
        str_left = xstr_descriptor.substr (xstr_descriptor.i_GetFieldOffset (1));
    }

    int i_rightOffset = xstr_source.i_GetFieldOffset (0, CT_ExtString::ec_TokenRegexMatch) +
                        xstr_source.i_GetFieldLength (0, CT_ExtString::ec_TokenRegexMatch);
    wstring str_right = xstr_source.substr (i_rightOffset);

    xstr_source = str_left + str_right;

    return true;

}   //  b_CheckInflectionType (...)

bool CT_ZalEntryParser::b_ParseInflectionGroup (CT_ExtString& xstr_descriptor, 
                                                const ST_Headword& st_head, 
                                                ST_Descriptor& st_entry)
{
//    CT_ExtString xstr_descriptor (str_descriptor, L" ");
    xstr_descriptor.v_SetBreakChars (L" ");
    xstr_descriptor.v_Trim();

    ST_InflectionGroup& st_inflectionGroup = st_entry.st_InflectionGroup.b_HasData 
        ? st_entry.st_AltInflectionGroup 
        : st_entry.st_InflectionGroup;

    bool b_match = xstr_descriptor.b_RegexMatch (L"^(_.+?_\\s*?)?(\\d{1,2}).*");
    if (b_match)
    {
        st_inflectionGroup.b_HasData = true;

        int i_length = (xstr_descriptor.str_GetRegexMatch(0)).length();
        if (i_length > 0)
        {
            st_inflectionGroup.str_Comment = xstr_descriptor.str_GetRegexMatch (0);     // rare
        }

        wstring str_type = xstr_descriptor.str_GetRegexMatch (1);
        if (str_type.length() < 1)
        {
            ATLASSERT(0);
            wstring str_msg (L"Error retrieving inflection type number: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }

        i_length += str_type.length();
        st_inflectionGroup.i_Type = _wtoi (str_type.c_str());
        xstr_descriptor = xstr_descriptor.substr (i_length);
    }
    if (xstr_descriptor.empty())
    {
        return true;
    }

    if (L'°' == xstr_descriptor[0])
    {
//        st_entry.b_StemAugment = true;
        if (POS_NOUN == map_Pos[st_entry.str_MainSymbol])
        {
            xstr_descriptor = xstr_descriptor.substr (1);
            if (3 == st_inflectionGroup.i_Type)
            {
                CT_ExtString xstr_headWord (st_head.str_Source);
                if (xstr_headWord.b_EndsWith (L"онок") || xstr_headWord.b_EndsWith (L"ёнок"))
                {
                    st_inflectionGroup.i_StemAugmentType = 1;
                }
                else
                {
                    if (xstr_headWord.b_EndsWith (L"оночек") || xstr_headWord.b_EndsWith (L"ёночек"))
                    {
                        st_inflectionGroup.i_StemAugmentType = 2;
                    }
                    else
                    {
                        ATLASSERT(0);
                        wstring str_msg (L"Unexpected source for a 3° noun: ");
                        str_msg += str_Source;
                        ERROR_LOG (str_msg);
                        return false;
                    }
                }
            }
        }
        else if (POS_VERB == map_Pos[st_entry.str_MainSymbol])
        {
            xstr_descriptor = xstr_descriptor.substr (1);
            st_inflectionGroup.i_StemAugmentType = 1;
        }
    }
    else
    {
        st_inflectionGroup.i_StemAugmentType = -1;
    }

    if (xstr_descriptor.empty())
    {
        return true;
    }

    if (L'*' == xstr_descriptor[0])
    {
        st_inflectionGroup.b_FleetingVowel = true;
        xstr_descriptor = xstr_descriptor.substr (1);
    }
    if (xstr_descriptor.empty())
    {
        return true;
    }

//    xstr_descriptor = str_descriptor;
    xstr_descriptor.v_Reset();
    b_match = xstr_descriptor.b_RegexMatch (L"^([авсDеF]'{0,2}).*");
    if (b_match)
    {
        wstring str_at1 = xstr_descriptor.str_GetRegexMatch (0);
        if (str_at1.length() < 1)
        {
            ATLASSERT(0);
            wstring str_msg (L"Error retrieving accent type 1: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }
        if (map_AccentType.find (str_at1) != map_AccentType.end())
        {
            st_inflectionGroup.e_AccentType1 = map_AccentType[str_at1];
        }
        else
        {
            ATLASSERT(0);
            wstring str_msg (L"Unrecognized accent type 1: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }
        xstr_descriptor = xstr_descriptor.substr (str_at1.length());
    }
    if (xstr_descriptor.empty())
    {
        return true;
    }
    
//    xstr_descriptor = str_descriptor;
    xstr_descriptor.v_Reset();
    b_match = xstr_descriptor.b_RegexMatch (L"^/([авсDеF]'{0,2}).*");
    if (b_match)
    {
        wstring str_at2 = xstr_descriptor.str_GetRegexMatch (0);
        if (str_at2.length() < 1)
        {
            ATLASSERT(0);
            wstring str_msg (L"Error retrieving accent type 2: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }
        if (map_AccentType.find (str_at2) != map_AccentType.end())
        {
            st_inflectionGroup.e_AccentType2 = map_AccentType[str_at2];
        }
        else
        {
            ATLASSERT(0);
            wstring str_msg (L"Unrecognized accent type 2: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }
        int i_offset = xstr_descriptor.i_GetFieldOffset (0, CT_ExtString::ec_TokenRegexMatch) +
                       xstr_descriptor.i_GetFieldLength (0, CT_ExtString::ec_TokenRegexMatch);
        xstr_descriptor = xstr_descriptor.substr (i_offset);
    }

    if (xstr_descriptor.empty())
    {
        return true;
    }

//    xstr_descriptor = str_descriptor;
    xstr_descriptor.v_Reset();
    b_match = xstr_descriptor.b_RegexMatch (L"(.*?)([!\\?])(.*)");
    if (b_match)
    {    
//        ATLASSERT (xstr_descriptor.i_GetFieldLength (0, CT_ExtString::ec_TokenRegexMatch) == 0);
        wstring str_x = xstr_descriptor.str_GetRegexMatch (1);
        if (str_x.length() > 0)
        {
            if (L"!" == str_x)
            {
                st_inflectionGroup.b_X = true;
            }
            if (L"?" == str_x)
            {
                st_inflectionGroup.b_BoxedX = true;
            }
            ATLASSERT (st_inflectionGroup.b_X || st_inflectionGroup.b_BoxedX);
        }
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0) + xstr_descriptor.str_GetRegexMatch (2);
    }
//    else
//    {
//        str_descriptor = xstr_descriptor.c_str();
//    }

    bool b_ = b_CheckCircledDigits (xstr_descriptor, st_inflectionGroup);
    if (!b_)
    {
        return false;
    }

    return true;

}   //  b_ParseInflectionGroup (...)

//bool CT_ZalEntryParser::b_CheckCircledDigits (wstring& str_descriptor, ST_InflectionGroup& st_entry)
bool CT_ZalEntryParser::b_CheckCircledDigits (CT_ExtString& xstr_descriptor, ST_InflectionGroup& st_entry)
{
    bool b_match = true;
//    CT_ExtString xstr_descriptor (str_descriptor);
    xstr_descriptor.v_SetBreakChars (L" ");
    xstr_descriptor.v_Trim();
    do
    {
        ST_CommonDeviation st_cd;
        b_match = xstr_descriptor.b_RegexMatch (L"^(\\[?(?:\"(\\d)\")\\]?).*");
        if (b_match)
        {
            CT_ExtString::ST_Token st_token;
            bool b_ = xstr_descriptor.b_GetField (0, 
                                                  CT_ExtString::ec_TokenRegexMatch, 
                                                  st_token);
            if (!b_)
            {
                return false;
            }

            if (L'[' == xstr_descriptor[st_token.i_Offset])
            {
                if (L']' != xstr_descriptor[st_token.i_Length - 1])
                {
                    ATLASSERT(0);
                    wstring str_msg (L"No closing bracket: ");
                    str_msg += str_Source;
                    ERROR_LOG (str_msg);
                    return false;
                }
                st_cd.b_Optional = true;
            }
            else
            {
                st_cd.b_Optional = false;
            }

            wstring str_digit = xstr_descriptor.str_GetField (1, CT_ExtString::ec_TokenRegexMatch);
            if (!iswdigit (str_digit[0]))
            {
                ATLASSERT(0);
                wstring str_msg (L"Digit expected: ");
                str_msg += str_Source;
                ERROR_LOG (str_msg);
                return false;
            }

            st_cd.i_Type = _wtoi (&str_digit[0]);
            if (1 > st_cd.i_Type || 9 < st_cd.i_Type)
            {
                ATLASSERT(0);
                wstring str_msg (L"Illegal circled number: ");
                str_msg += str_Source;
                ERROR_LOG (str_msg);
                return false;
            }

            st_entry.vec_stCommonDeviations.push_back (st_cd);
            xstr_descriptor = xstr_descriptor.substr (st_token.i_Length);
        }

    } while (b_match);

    xstr_descriptor.v_SetBreakChars (L" ");
    xstr_descriptor.v_Trim();
//    str_descriptor = xstr_descriptor.c_str();

    return true;

}   //  b_CheckCircledDigit (...)

//bool CT_ZalEntryParser::b_CheckTilde (wstring& str_descriptor, ST_Descriptor& st_entry)
bool CT_ZalEntryParser::b_CheckTilde (CT_ExtString& xstr_descriptor, ST_Descriptor& st_entry)
{
//    CT_ExtString xstr_descriptor (str_descriptor);
    bool b_match = xstr_descriptor.b_RegexMatch (L"(.*?)(~)(.*)");
    if (!b_match)
    {
//        str_descriptor = xstr_descriptor.c_str();
        return true;
    }

    ATLASSERT (xstr_descriptor.i_GetFieldLength (0, CT_ExtString::ec_TokenRegexMatch) == 0);
    wstring str_tilde = xstr_descriptor.str_GetRegexMatch (1);
    if (str_tilde.length() > 0)
    {
        ATLASSERT (L"~" == str_tilde);
        st_entry.b_NoComparative = true;
    }

    xstr_descriptor = xstr_descriptor.str_GetRegexMatch (2);

    return true;

}   //  b_CheckTilde (...)

bool CT_ZalEntryParser::b_CheckSquareBrackets (CT_ExtString& xstr_descriptor, 
                                               ST_Headword& st_head, 
                                               ST_Descriptor& st_entry)
{
// Using regex may be tricky since we need to handle nested brackets

    xstr_descriptor.v_Trim();
    int i_startOffset = xstr_descriptor.find_first_of (L";[");
    if (wstring::npos == i_startOffset)
    {
        return true;
    }

    //
    // Don't go beyond first ";"
    //
    if (L';' == xstr_descriptor[i_startOffset])
    {
        return true;
    }

    if (i_startOffset > 0)
    {
        ATLASSERT (0);
        wstring str_msg (L"Unidentifed sequence before opening bracket: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
//        return false;
    }

    int i_endOffset = xstr_descriptor.find_first_of (L"]");
    if (wstring::npos == i_endOffset)
    {
        ATLASSERT(0);
        wstring str_msg (L"No closing bracket: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    CT_ExtString xstr_source = xstr_descriptor.substr (i_startOffset + 1, 
                                                       i_endOffset - i_startOffset - 1);
    xstr_descriptor = xstr_descriptor.substr (i_endOffset+1);

//    CT_ExtString xstr_source (str_source);

    //  аб<ак м 3а [//аб<ака]
    bool b_match = xstr_source.b_RegexMatch (L"^//([абвгдеёжзийклмнопрстуфхцчшщъыьэюя<>]+)");
    if (b_match)
    {
        st_head.str_Variant = xstr_source.str_GetRegexMatch(0);
        st_entry.st_LookAhead = ST_LookAhead (st_head.str_Source, false);
        return true;
    }

    //  ак<анф м 1а [//ак<ант (_см._)]
    b_match = xstr_source.b_RegexMatch 
                (L"^//([абвгдеёжзийклмнопрстуфхцчшщъыьэюя<>]+)(?:\\s)(\\(_см._\\))");
    if (b_match)
    {
        st_head.str_SeeAlso = xstr_source.str_GetRegexMatch (0);
        st_entry.st_LookAhead = ST_LookAhead (st_head.str_Source, true);
        return true;
    }

    //  к<апсель м 2а [_проф._ м 2с"1"]
    //  б<ондарь мо 2а [//бонд<арь мо 2в] 
    //  врожд<ённый п 1*а"1" [//п 1*а/в"2", ё]
    b_match = xstr_source.b_RegexMatch 
        (L"^(//)?(_[абвгдеёжзийклмнопрстуфхцчшщъыьэюя\\s\\.\\-]+?_\\s)?([^\\s]+)(?:\\s)?([^\\s+])?(?:\\s)?(.+)?");
    if (!b_match)
    {
        return true;
    }
        
    int i_offset = 0;
    wstring str_head;
    wstring str_usageComment;
    bool b_mainSymbolFound = false;
    bool b_headWordFound = false;
    for (int i_at = 0; 
         !b_mainSymbolFound && (i_at < xstr_source.i_GetNumOfRegexMatches()); 
         ++i_at)
    {
        wstring str_captureGroup = xstr_source.str_GetRegexMatch (i_at);
        if (str_captureGroup.empty())
        {
            continue;
        }
        if (L"//" == str_captureGroup)
        {
            i_offset = xstr_source.i_GetFieldOffset (i_at, CT_ExtString::ec_TokenRegexMatch) + 
                xstr_source.i_GetFieldLength (i_at, CT_ExtString::ec_TokenRegexMatch);
            continue;
        }
        if (L'_' == str_captureGroup[0])
        {
            CT_ExtString xstr_comment (str_captureGroup, L"_ ");
            xstr_comment.v_Trim();
            str_usageComment = xstr_comment.c_str();
            i_offset = xstr_source.i_GetFieldOffset (i_at, CT_ExtString::ec_TokenRegexMatch) + 
                xstr_source.i_GetFieldLength (i_at, CT_ExtString::ec_TokenRegexMatch);
            continue;
        }

        if (map_Pos.find (str_captureGroup) != map_Pos.end())
        {
            b_mainSymbolFound = true;
        }
        else
        {
            if (!str_captureGroup.empty())
            {
                str_head = str_captureGroup;
                b_headWordFound = true; // additional tests here?
            }
        }
    }

    if (b_mainSymbolFound)
    {
        wstring str_entry (xstr_source.substr (i_offset));
        bool b_secondPass = true;
        CT_ZalEntryParser co_entry (str_entry, pco_Db, b_secondPass);
        if (!b_headWordFound)
        {
            str_head = st_head.str_Source;
        }
        else
        {
            str_head.clear();
        }
        bool b_parseResult = co_entry.b_Parse (str_head, str_usageComment);
        if (!b_parseResult)
        {
            wstring str_msg (L"*** Parsing error: \n");
            str_msg += str_entry;
            ERROR_LOG (str_msg.c_str());
            return false;
        }
    }
    else
    {
        ATLASSERT(0);
    }

    return true;

}   //  b_CheckSquareBrackets (...)

//bool CT_ZalEntryParser::b_CheckCognate (wstring& str_descriptor, ST_Descriptor& st_entry)
bool CT_ZalEntryParser::b_CheckCognate (CT_ExtString& xstr_descriptor, ST_Descriptor& st_entry)
{
    // Handles cases:
    // (_от_ ...)
    // (_женск. к_ ...)
    // (_уменьш. к_ ...)
    // (_увеличит. к_ ...)
    // (_ласкат. к_ ...)
    // (_сущ. к_ ...)
    // (_ср._ ...)
    // (_см. также_ ...)
    if (xstr_descriptor.length() <= 0)
    {
        return true;
    }

//    wsmatch ct_result;
//    wstring str_left, str_right, str_cognate;
//    b_match = regex_search(str_descriptor, (const wregex)L"\\(_(женск\\.\\s*к|уменьш\\.\\s*к|увеличит\\.\\s*к|ласкат\\.\\s*к|сущ\\.\\s*к|ср\\.|см\\.\\s*также|от)_");
    bool b_match = xstr_descriptor.b_RegexMatch 
        (L"\\(_(женск\\.\\s*к|уменьш\\.\\s*к|увеличит\\.\\s*к|ласкат\\.\\s*к|сущ\\.\\s*к|ср\\.|см\\.\\s*также|от)_");
    if (b_match == false)
    {
        return true;
    }

//    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"(.*?),?\\s*\\(_(женск\\.\\s*к|ср\\.|уменьш\\.\\s*к|увеличит\\.\\s*к|ласкат\\.\\s*к|сущ\\.\\s*к|см\\.\\s*также|от)_\\s*([^\\)]*)\\)(.*)");
    b_match = xstr_descriptor.b_RegexMatch
        (L"(.*?),?\\s*\\(_(женск\\.\\s*к|ср\\.|уменьш\\.\\s*к|увеличит\\.\\s*к|ласкат\\.\\s*к|сущ\\.\\s*к|см\\.\\s*также|от)_\\s*([^\\)]*)\\)(.*)");
    if (b_match == true)
    {
//        wstring str_left = ct_result[1];
//        wstring str_right = ct_result[4];
//        str_cognate = ct_result[3];
        wstring str_left = xstr_descriptor.str_GetRegexMatch (0);
        wstring str_right = xstr_descriptor.str_GetRegexMatch (3);
        xstr_descriptor = str_left + str_right;
        st_entry.str_Cognate = xstr_descriptor.str_GetRegexMatch (2);
        return true;
    }

    ERROR_LOG(L"CheckCognate was started on " + xstr_descriptor + L" and couldn't recognize the string.");
    return false;

}   //  b_CheckCognate (...)

//bool CT_ZalEntryParser::b_CheckVerbType (wstring& str_descriptor, ST_Descriptor& st_entry)
bool CT_ZalEntryParser::b_CheckVerbType (CT_ExtString& xstr_descriptor, ST_Descriptor& st_entry)
{
    // Handles cases:
    // ..., безл.
    // ..., многокр.
    if (xstr_descriptor.length() <= 0)
    {
        return true;
    }

//    CT_ExtString xstr_descriptor (str_descriptor);
    bool b_match = xstr_descriptor.b_RegexMatch (L"^(.*?),?\\s*безл\\.(.*)");
    if (b_match)
    {
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0) + xstr_descriptor.str_GetRegexMatch (1);
        st_entry.b_IsImpersonal = true;
    }
    b_match = xstr_descriptor.b_RegexMatch (L"^(.*?),?\\s*многокр\\.(.*)");
    if (b_match)
    {
        xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0) + xstr_descriptor.str_GetRegexMatch (1);
        st_entry.b_IsIterative = true;
    }

//    str_descriptor = xstr_descriptor;

    return true;

}   //  b_CheckVerbType (...)

bool CT_ZalEntryParser::b_CheckDollar (CT_ExtString& xstr_descriptor, ST_Descriptor& st_entry)
{
    st_entry.b_HasAspectPair = false;

    // The function looks for the aspect pair information.
    // If the number after $ is Roman, write it
    // to i_AspectPairType as negative, otherwise as positive.
    // Push all the numbers found to vec_i_AspectPairType
    // (there can be several, like in $II//I(-<а-)).
    // If there is also a comment in parentheses, push it to vec_str_AspectPairComment.

    if (map_Pos[st_entry.str_MainSymbol] != POS_VERB)
    {
        return true;
    }
    st_entry.vec_i_AspectPairType.clear();
    st_entry.vec_str_AspectPairComment.clear();
//    wstring str_rest, str_current, str_error_message, str_aspect_comment;
//    wsmatch result;
//    unsigned int i_previous_length = 0;
//    int i_step = 1;
//    bool b_match = regex_match(str_descriptor, result, (const wregex)L"^([^\\$]*)\\$\\s*([^\\]]*)(.*)$");
    bool b_match = xstr_descriptor.b_RegexMatch (L"^([^\\$]*)\\$\\s*([^\\]]*)(.*)$");
    if (b_match == false)
    {
        return true;
    }
  
//    str_rest = result[2];
    CT_ExtString xstr_rest = xstr_descriptor.str_GetRegexMatch (1);
//    str_descriptor = (wstring)result[1] + (wstring)result[3];
    xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0) + xstr_descriptor.str_GetRegexMatch (2);
//    str_error_message = L"CheckDollar couldn't recognize the text: " + str_rest;
    st_entry.b_HasAspectPair = true;
    //ERROR_LOG(L"---- CheckDollar - str_comment: " + str_comment + L"\n---- CheckDollar - str_descriptor: " + str_descriptor);
    // Several references can be divided by ; or //
    // (don't look for them inside parentheses)
    wstring str_matcher (L"\\s*([^\\/;\\(]+(\\([^\\)]*?\\)[^\\/;\\(]*)*)(;|\\/\\/)?(.*)?");
//    while (regex_match(str_rest, result, (const wregex)L"\\s*([^\\/;\\(]+(\\([^\\)]*?\\)[^\\/;\\(]*)*)(;|\\/\\/)?(.*)?"))
    CT_ExtString xstr_current;
    wstring str_aspectComment;
    while (xstr_rest.b_RegexMatch (str_matcher))
    {
//        str_current = result[1];
//        str_rest = result[4];
        xstr_current = xstr_rest.str_GetRegexMatch (0);
        xstr_rest = xstr_rest.str_GetRegexMatch (3);
        //ERROR_LOG(L"**** str_current: " + str_current);

        // 1. Roman numerals
//        if (regex_match(str_current, result,
//            (const wregex)L"(_\\s*н?св\\.?\\s*также\\s*_|_простореч\\._|_устар\\._)?\\s*(I+)\\s*(\\(([\\-<>\\w\\/]*|_[\\w<>\\-\\.\\s\\/,;]*_|[\\-<>\\w\\/]*,\\s*_[\\w<>\\-\\.\\s\\/,;]*_)\\))?\\s*"))
        wstring str_matcher1 (L"(_\\s*н?св\\.?\\s*также\\s*_|_простореч\\._|_устар\\._)?");
        wstring str_matcher2 (L"\\s*(I+)\\s*(\\(([\\-<>\\w\\/]*|_[\\w<>\\-\\.\\s\\/,;]*_|");        
        wstring str_matcher3 (L"[\\-<>\\w\\/]*,\\s*_[\\w<>\\-\\.\\s\\/,;]*_)\\))?\\s*");
        if (xstr_current.b_RegexMatch (str_matcher1 + str_matcher2 + str_matcher3))
        {
//            str_aspect_comment = (wstring)result[1];
            str_aspectComment = xstr_current.str_GetRegexMatch (0);
            if (str_aspectComment.length() <= 0)
            {
//                str_aspect_comment = (wstring)result[4];
                str_aspectComment = xstr_current.str_GetRegexMatch (3);
            }
            else
            {
//                str_aspect_comment += L" " + (wstring)result[4];
                str_aspectComment += L" " + xstr_current.str_GetRegexMatch (3);
            }

//            st_entry.vec_i_AspectPairType.push_back(-1 * ((int)((wstring)result[2]).length()));
            st_entry.vec_i_AspectPairType.push_back (-1 * ((int)(xstr_current.str_GetRegexMatch(1).length())));
            st_entry.vec_str_AspectPairComment.push_back (str_aspectComment);
            //ERROR_LOG(L"---- CheckDollar: Roman: " + str_ToString<int>(st_entry.vec_i_AspectPairType.at(0)) + L"; " + st_entry.vec_str_AspectPairComment.at(0));
        }        
        // 2. Arabic numerals
//        else if (regex_match(str_current, result,
//            (const wregex)L"(_\\s*н?св\\.?\\s*также\\s*_|_простореч\\._|_устар\\._)?\\s*(\\d+)\\s*(\\(([\\-<>\\w\\/]*|_[\\w<>\\-\\.\\s\\/,;]*_|[\\-<>\\w\\/]*,\\s*_[\\w<>\\-\\.\\s\\/,;]*_)\\))?\\s*"))
        else
        {
            str_matcher1 = L"(_\\s*н?св\\.?\\s*также\\s*_|_простореч\\._|_устар\\._)?";
            str_matcher2 = L"\\s*(\\d+)\\s*(\\(([\\-<>\\w\\/]*|_[\\w<>\\-\\.\\s\\/,;]*_|";
            str_matcher3 = L"[\\-<>\\w\\/]*,\\s*_[\\w<>\\-\\.\\s\\/,;]*_)\\))?\\s*";
            if (xstr_current.b_RegexMatch (str_matcher1 + str_matcher2 + str_matcher3))
            {
//            str_aspect_comment = (wstring)result[1];
                str_aspectComment = xstr_current.str_GetRegexMatch (0);
                if (str_aspectComment.length() <= 0)
                {
//                str_aspect_comment += L" " + (wstring)result[4];
                    str_aspectComment += L" " + xstr_current.str_GetRegexMatch (3);
                }
//            st_entry.vec_i_AspectPairType.push_back(_wtoi(((wstring)result[2]).c_str()));
                st_entry.vec_i_AspectPairType.push_back (_wtoi(xstr_current.str_GetRegexMatch (1).c_str()));
                st_entry.vec_str_AspectPairComment.push_back (str_aspectComment);
            //ERROR_LOG(L"---- CheckDollar: Arabic: " + str_ToString<int>(st_entry.vec_i_AspectPairType.at(0)) + L"; " + st_entry.vec_str_AspectPairComment.at(0));
            }
            else
            {
        // 3. No number at all
                str_matcher = L"([\\-<>\\w\\/]+(\\s*\\([^\\)]*\\))?)\\s*";
                if (xstr_current.b_RegexMatch (str_matcher))
//        if (regex_match(str_current, result, (const wregex)L"([\\-<>\\w\\/]+(\\s*\\([^\\)]*\\))?)\\s*"))
                {
                    st_entry.vec_i_AspectPairType.push_back (0);
//            st_entry.vec_str_AspectPairComment.push_back((wstring)result[1]);
                    st_entry.vec_str_AspectPairComment.push_back (xstr_current.str_GetRegexMatch (0));
            //ERROR_LOG(L"---- CheckDollar: No number: " + str_ToString<int>(st_entry.vec_i_AspectPairType.at(0)) + L"; " + st_entry.vec_str_AspectPairComment.at(0));
                }
                else
                {
//                    ERROR_LOG (str_error_message);
                    return false;
                }
            }
        }
    
    }
    
    return true;

}   //  b_CheckDollar (...)

bool CT_ZalEntryParser::b_CheckComma (CT_ExtString& xstr_descriptor, ST_Descriptor& st_entry)
{
    if (L',' != xstr_descriptor[0])
    {
        return true;
    }

//    CT_ExtString xstr_descriptor (str_descriptor, L", ");
    xstr_descriptor.v_SetBreakChars (L", ");
    if (xstr_descriptor.i_GetNumOfFields() < 1)
    {
        ATLASSERT(0);
        wstring str_msg (L"Continuation expected after comma: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    xstr_descriptor = xstr_descriptor.substr (1); // default value to be returned in 
                                                  // case we don't find anything interesting

    CT_ExtString xstr_afterComma = xstr_descriptor.str_GetField (0);
    bool b_ = xstr_afterComma.b_RegexMatch (L"#(\\d{1,2}).*");
    if (b_)
    {
        wstring str_sectionNum = xstr_afterComma.str_GetRegexMatch (0);
        st_entry.i_Section = _wtoi (str_sectionNum.c_str());
        xstr_descriptor = xstr_descriptor.substr (xstr_descriptor.i_GetFieldOffset (0) + 
                                                  xstr_descriptor.i_GetFieldLength (0));
    }

    if (L'ё' == xstr_afterComma[0])
    {
        st_entry.b_Yo = true;
        xstr_descriptor = xstr_descriptor.substr (xstr_descriptor.i_GetFieldOffset (0) + 
                                                  xstr_descriptor.i_GetFieldLength (0));
    }

    if (L'о' == xstr_afterComma[0])
    {
        st_entry.b_O = true;
        xstr_descriptor = xstr_descriptor.substr (xstr_descriptor.i_GetFieldOffset (0) + 
                                                  xstr_descriptor.i_GetFieldLength (0));
    }

    if (xstr_afterComma.b_StartsWith (L"Р2"))
    {
        st_entry.b_Gen2 = true;
        xstr_descriptor = xstr_descriptor.substr (xstr_descriptor.i_GetFieldOffset (0) + 
                                                  xstr_descriptor.i_GetFieldLength (0));
    }

    b_ = xstr_afterComma.b_RegexMatch 
            (L".?(П2\\s?(?:\\(([абвгдеёжзийклмнопрстуфхцчшщъыьэюя]{1,2})\\))?).*");
    if (b_)
    {
        if (xstr_afterComma.i_GetNumOfRegexMatches() > 1)
        {
            wstring str_preposition = xstr_afterComma.str_GetRegexMatch (1);
            if ((L"на" == str_preposition) || (L"в" == str_preposition))
            {
                st_entry.str_Loc2Preposition = str_preposition;
            }
            int i_offset = xstr_afterComma.i_GetFieldOffset (0);
            int i_length = xstr_afterComma.i_GetFieldLength (0);
            if (L'[' == xstr_afterComma[0])
            {
                int i_at = xstr_afterComma.find_first_of (L']', i_offset + 1);
                if (wstring::npos == i_at)
                {
                    ATLASSERT(0);
                    wstring str_msg (L"Right bracket missing: ");
                    str_msg += str_Source;
                    ERROR_LOG (str_msg);
                    return false;
                }
                st_entry.b_Loc2Optional = true;
                i_length = i_at + 1;
            }
            else
            {
                st_entry.b_Loc2 = true;
            }
            xstr_descriptor = xstr_descriptor.substr (xstr_descriptor.i_GetFieldOffset (0) +
                                                      i_length);
        }
        else
        {
            ATLASSERT(0);
            wstring str_msg (L"Unexpected preposition after П2: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }
    }

    return true;

}   //  b_CheckComma (...)

bool CT_ZalEntryParser::b_MakeGraphicStem (ST_Descriptor& st_entry, const ST_Headword& st_head)
{
    CT_ExtString xstr_source = st_head.str_Source;

    switch (st_entry.e_InflectionSymbol)
    {
        case MS_M:
        case MS_MO:
        case MS_ZH:
        case MS_ZHO:
        case MS_MO_ZHO:
        case MS_S:
        case MS_SO:
        case MS_MS:
        case MS_MS_P:
        {
            if (0 == st_entry.st_InflectionGroup.i_Type)
            {
                st_entry.str_GraphicStem = st_head.str_Source;
                break;
            }

            wstring str_endings (str_Vowels);
            str_endings += L'й';
            str_endings += L'ь';
            if (xstr_source.b_EndsWithOneOf (str_endings))
            {
                if (xstr_source.length() < 2)
                {
                    ATLASSERT(0);
                    wstring str_msg (L"Headword too short: ");
                    str_msg += str_Source;
                    ERROR_LOG (str_msg);
                    return false;
                }
                st_entry.str_GraphicStem = xstr_source.substr (0, xstr_source.length()-1);
            }
            else
            {
                st_entry.str_GraphicStem = st_head.str_Source;
            }
            break;
        }
        case MS_MN:
        case MS_MN_NEOD:
        case MS_MN_ODUSH:
        case MS_MN_OT:
        {
            // This is not mentioned in GS; needs verification.
            if (xstr_source.length() < 2)
            {
                ATLASSERT(0);
                wstring str_msg (L"Headword too short: ");
                str_msg += str_Source;
                ERROR_LOG (str_msg);
                return false;
            }
            st_entry.str_GraphicStem = xstr_source.substr (0, xstr_source.length()-1);
            break;
        }
        case MS_P:
        {
            unsigned int ui_charsToRemove = 0;
            if (xstr_source.b_EndsWith (L"ся"))
            {
                ui_charsToRemove = 4;
            }
            else
            {
                ui_charsToRemove = 2;
            }
            if (xstr_source.length() < ui_charsToRemove)
            {
                ATLASSERT(0);
                wstring str_msg (L"Headword too short: ");
                str_msg += str_Source;
                ERROR_LOG (str_msg);
                return false;
            }
            st_entry.str_GraphicStem = xstr_source.substr (0, xstr_source.length() - ui_charsToRemove);
            break;
        }
        case MS_SV:
        case MS_NSV:
        case MS_SV_NSV:
        {
            if (xstr_source.b_EndsWith (L"ти") || 
                xstr_source.b_EndsWith (L"ть") || 
                xstr_source.b_EndsWith (L"чь"))
            {
                st_entry.str_GraphicStem = xstr_source.substr (0, xstr_source.length() - 2);
            }
            else
            {
                if (xstr_source.b_EndsWith (L"тись") || 
                    xstr_source.b_EndsWith (L"ться") || 
                    xstr_source.b_EndsWith (L"чься"))
                {
                    st_entry.str_GraphicStem = xstr_source.substr (0, xstr_source.length() - 4);
                    st_entry.b_Reflexive = true;
                }
                else
                {
                    ATLASSERT(0);
                    wstring str_msg (L"Verb not recognized: ");
                    str_msg += str_Source;
                    ERROR_LOG (str_msg);
                    return false;
                }
            }
            break;
        }
        default:
        {
            st_entry.str_GraphicStem = st_head.str_Source;
        }
    }

    return true;

}   //  b_MakeGraphicStem()

bool CT_ZalEntryParser::b_SaveToDb (ST_Headword& st_head, vector<ST_Descriptor>& vec_descriptors)
{
    __int64 ll_wordId = 0;
    st_head.b_SaveToDb (pco_Db, ll_wordId);
    for (unsigned int ui_entry = 0; ui_entry < vec_descriptors.size(); ++ui_entry)
    {
        ST_Descriptor& st_entry = vec_descriptors[ui_entry];
        __int64 ll_descriptorId = -1;
        bool b_ret = st_entry.b_SaveToDb (pco_Db, ll_wordId, ll_descriptorId);
        if (!b_ret)
        {
            continue;
        }

        if (st_entry.st_LookAhead.b_Valid)
        {
            b_ret = b_SaveXReference (st_head, st_entry.st_LookAhead, ll_descriptorId);
        }

        b_ret = b_SaveLexemeHash (st_head, st_entry, ll_descriptorId);
    }

    return true;

}       //  b_SaveToDb (...)

bool CT_ZalEntryParser::b_SaveLexemeHash (ST_Headword& st_head, 
                                          ST_Descriptor& st_entry, 
                                          __int64 ll_descriptorId)
{
    st_LexemeHasher.str_SourceForm = st_head.str_Source;
    vector<int>::iterator it_homonym = st_head.vec_IHomonyms.begin();
    for (; it_homonym != st_head.vec_IHomonyms.end(); ++it_homonym)
    {
        st_LexemeHasher.str_SourceForm += str_ToString (*it_homonym);
    }
    st_LexemeHasher.map_Stress = st_head.map_Stress;
    st_LexemeHasher.str_MainSymbol = st_entry.str_MainSymbol;
    st_LexemeHasher.str_Comment = st_head.str_Comment;
    st_LexemeHasher.str_Comment += st_entry.str_Comment;
    st_LexemeHasher.str_Comment += st_entry.str_TrailingComment;

    if (st_entry.st_InflectionGroup.b_HasData)
    {
        st_LexemeHasher.i_InflectionType = st_entry.st_InflectionGroup.i_Type;
        st_LexemeHasher.i_AccentType1 = (int)st_entry.st_InflectionGroup.e_AccentType1;
        st_LexemeHasher.i_AccentType2 = (int)st_entry.st_InflectionGroup.e_AccentType2;
        st_LexemeHasher.b_SaveToDb (pco_Db, ll_descriptorId);
        if (st_entry.st_AltInflectionGroup.b_HasData)
        {
            st_LexemeHasher.i_InflectionType = st_entry.st_InflectionGroup.i_Type;
            st_LexemeHasher.i_AccentType1 = (int)st_entry.st_InflectionGroup.e_AccentType1;
            st_LexemeHasher.i_AccentType2 = (int)st_entry.st_InflectionGroup.e_AccentType2;
            st_LexemeHasher.b_SaveToDb (pco_Db, ll_descriptorId);
        }
    }
    else
    {
        st_LexemeHasher.b_SaveToDb (pco_Db, ll_descriptorId);
    }

    return true;

}   //  b_SaveLexemeHash (...)

bool CT_ZalEntryParser::b_SaveXReference (ST_Headword& st_head, ST_LookAhead& st_xRef, __int64 ll_descriptorId)
{
return true;
}   //  b_SaveXReferences (...)

