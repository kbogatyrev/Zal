#include "StdAfx.h"
#include "ExtString.h"
#include "ZalEntryParser.h"

void CT_ZalEntryParser::v_Init()
{
    wstring arr_strMainSymbol[] = { L"м", L"мо", L"ж", L"жо", L"с", L"со", L"мо-жо", L"мн.",
     L"мн. неод.", L"мн. одуш.", L"мн. _от_", L"п", L"мс", L"мс-п", L"числ.",
     L"числ.-п", L"св", L"нсв", L"св-нсв", L"межд.", L"н", L"предик.", L"предл.",
     L"част.", L"союз", L"вводн.", L"сравн." };

    map_Pos[L"м"] = POS_NOUN;
    map_Pos[L"мо"] = POS_NOUN;
    map_Pos[L"ж"] = POS_NOUN;
    map_Pos[L"жо"] = POS_NOUN;
    map_Pos[L"с"] = POS_NOUN;
    map_Pos[L"мо-жо"] = POS_NOUN;
    map_Pos[L"мн."] = POS_NOUN;
    map_Pos[L"мн. неод."] = POS_NOUN;
    map_Pos[L"мн. одуш."] = POS_NOUN;

    map_Pos[L"св"] = POS_VERB;
    map_Pos[L"нсв"] = POS_VERB;
    map_Pos[L"св-нсв"] = POS_VERB;

    map_Pos[L"п"] = POS_ADJ;

    map_Pos[L"мс"] = POS_PRONOUN;
    map_Pos[L"мс-п"] = POS_PRONOUN;

    map_Pos[L"н"] = POS_ADV;

    map_Pos[L"част."] = POS_PARTICLE;

    map_Pos[L"предл."] = POS_PREP;

    map_Pos[L"союз"] = POS_CONJ;

    map_Pos[L"вводн."] = POS_PARENTH;

    map_Pos[L"предик."] = POS_PREDIC;

    map_Pos[L"межд."] = POS_INTERJ;

    map_Pos[L"сравн."] = POS_COMPAR;

    wstring arr_strAccent[] = { L"а", L"в", L"с", L"D", L"е", L"F", L"а'", L"в'", L"с'", L"с''", L"D'",
                             L"F'", L"F''" };
    map_AccentType[L"а"] = AT_A;
    map_AccentType[L"в"] = AT_B;
    map_AccentType[L"с"] = AT_C;
    map_AccentType[L"D"] = AT_D;
    map_AccentType[L"е"] = AT_E;
    map_AccentType[L"F"] = AT_F;
    map_AccentType[L"а'"] = AT_A1;
    map_AccentType[L"в'"] = AT_B1;
    map_AccentType[L"с'"] = AT_C1;
    map_AccentType[L"с''"] = AT_C2;
    map_AccentType[L"D'"] = AT_D1;
    map_AccentType[L"F'"] = AT_F1;
    map_AccentType[L"F''"] = AT_F2;

    wstring str_cyrillicLowercase = L"-абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring str_vowels = L"аеёиоуыэюя";
//    unsigned char *walf= " ъ-эюёщфхцжшгчбзямудыйплквьсрнеитао";

    i_Offset = 0;

}   //  v_Init()

bool CT_ZalEntryParser::b_Parse()
{
    xstr_Entry.v_Reset();
    xstr_Entry = str_Source;
    xstr_Entry.v_SetBreakChars (L" \n");
    xstr_Entry.v_Trim();

    ST_Headword st_head;
    bool b_ = b_ExtractHead();
    if (!b_)
    {
        return false;
    }

    xstr_Entry.v_Reset();
    xstr_Entry.v_SetBreakChars (L"|");
    for (int i_at = 0; i_at < xstr_Entry.i_NFields(); ++i_at)
    {
        ST_Descriptor st_entry;
        wstring str_descriptor (xstr_Entry.str_GetField (i_at));

        bool b_ = b_CheckMnOt (str_descriptor, st_entry);
        if (!b_)
        {
            return false;
        }
        
        b_ = b_GetMainSymbol (str_descriptor, st_entry);
        if (!b_)
        {
            return false;
        }

        b_ = b_CheckMainSymbComment (str_descriptor, st_entry);
        if (!b_)
        {
            return false;
        }

        b_ = b_CheckInflectionType (str_descriptor, st_entry);
        if (!b_)
        {
            return false;
        }
//        if (st_entry.str_InflectionType.empty())
//        {
//            st_entry.str_InflectionType = st_entry.str_MainSymbol;
//        } 
        b_ = b_ParseInflectionGroup (str_descriptor, st_entry);
        if (!b_)
        {
            return false;
        }

        if (str_descriptor.find (L"//") == 0)
        {
            str_descriptor = str_descriptor.substr (2);
            b_ = b_ParseInflectionGroup (str_descriptor, st_entry);
            if (!b_)
            {
                return false;
            }
        }
        
        CT_ExtString xstr_descriptor (str_descriptor, L" ");
        xstr_descriptor.v_Trim();
        xstr_descriptor.v_Reset();
        bool b_match = xstr_descriptor.b_RegexMatch (L"^(\\(\\-[щсдтбгкнми]\\-\\))(.*)");
        if (b_match)
        {
            int i_ = xstr_descriptor.i_GetNumOfRegexMatches();
// TODO
int gogo = 0;
        }

        b_ = b_CheckCircledDigit (str_descriptor, st_entry);
        if (!b_)
        {
            return false;
        }

        if (L'-' == str_descriptor[0])
        {
            st_entry.b_Dash = true;
            str_descriptor = str_descriptor.substr (1);
        }

// TODO check for (@...)

        if (L',' == str_descriptor[0])
        {
// TODO Check for section mark + number
        }

        b_CheckSquareBrackets (str_descriptor, st_entry);
        if (!b_)
        {
            return false;
        }

        b_ = b_CheckHatchedCircle (str_descriptor, st_entry);
        if (!b_)
        {
            return false;
        }

        vec_Descriptors.push_back (st_entry);

    }   //  for (int i_at = 0; ...)

    return true;

}   //  i_Parse (...)

bool CT_ZalEntryParser::b_ExtractHead()
{
    CT_ExtString xstr_head (xstr_Entry.str_GetField (0));
    bool b_match = xstr_head.b_RegexMatch (L"^(?:([1-5])?(?:,([1-5]))*/)?([^ ]+)");
//    bool b_match = xstr_head.b_RegexMatch (L"^(([1-5])?(,([1-5]))*/)?([^ ]+)");

    wstring str_digit;
    for (int i_ = 0; i_ < xstr_head.i_NRegexMatches() - 1; ++i_)
    {
        str_digit = xstr_head.str_GetRegexMatch (i_);
        if (str_digit.length() != 1)
        {
            ATLASSERT(0);
            wstring str_msg (L"Bad homonym number: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }
        if (!iswdigit (str_digit[0]))
        {
            ATLASSERT(0);
            wstring str_msg (L"Homonym number is not numeric: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }
        st_Head.vec_IHomonyms.push_back (_wtoi (&str_digit[0]));
    }

    if (xstr_head.i_NRegexMatches() < 1)
    {
        ATLASSERT(0);
        wstring str_msg (L"No headword: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }
    st_Head.str_Source = xstr_head.str_GetRegexMatch (xstr_head.i_NRegexMatches() - 1);

    xstr_Entry = xstr_Entry.substr (xstr_Entry.i_GetFieldOffset (1));
//    b_match = xstr_Entry.b_RegexMatch (L"^(?:\\((.+?)\\)\\s)(.+)\n");
    b_match = xstr_Entry.b_RegexMatch (L"^(?:\\((.+?)\\)\\s)(.+)");
    if (b_match)
    {
        if (xstr_Entry.i_NRegexMatches() != 2)
        {
            ATLASSERT(0);
            wstring str_msg (L"Error matching parenthetical expression: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }
        st_Head.str_Comment = xstr_Entry.str_GetRegexMatch (0);
        xstr_Entry = CT_ExtString (xstr_Entry.str_GetRegexMatch (1));
    }

    return true;

}   //  b_ExtractHead()

bool CT_ZalEntryParser::b_CheckMnOt (wstring& str_source, ST_Descriptor& st_entry)
{
    st_entry.b_PluralOf = false;

    CT_ExtString xstr_source (str_source, L" /,");

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

    if (map_Pos.find (xstr_source.str_GetField (2)) != map_Pos.end())
    {
        str_source = str_source.substr (xstr_source.i_GetFieldOffset (2));
        return true;
    }

//    st_entry.str_MainSymbolPluralOf = xstr_source.str_GetField (2);
    st_Head.str_PluralOf = xstr_source.str_GetField (2);
    if (xstr_source.i_NFields() < 4)
    {
        ATLASSERT(0);
        wstring str_msg (L"Unterminated \"мн. _от_ <headword>\": ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    str_source = str_source.substr (xstr_source.i_GetFieldOffset (3));
    
    return true;

}   //  b_CheckMnOt (...)

bool CT_ZalEntryParser::b_GetMainSymbol(wstring& str_source, ST_Descriptor& st_entry)
{
    CT_ExtString xstr_descriptor (str_source, L" ");
    
    int i_nextField = 1;
    CT_ExtString xstr_mainSymb (xstr_descriptor.str_GetField (0), L"/,");
    if (L"мн." == xstr_mainSymb)
    {
        if (xstr_descriptor.i_NFields() > 1)
        {
            wstring str_next = xstr_descriptor.str_GetField (1);
            if ((L"неод." == str_next) || (L"одуш." == str_next))
            {
                xstr_mainSymb += L" " + str_next;
                ++i_nextField;
            }
        }
    }

    if (xstr_descriptor.i_NFields() < i_nextField + 1)
    {
        str_source.erase();
    }
    else
    {
        str_source = str_source.substr (xstr_descriptor.i_GetFieldOffset (i_nextField));
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

    return true;

}   //  b_GetMainSymbol (...)

bool CT_ZalEntryParser::b_CheckMainSymbComment (wstring& str_source, ST_Descriptor& st_entry)
{
    CT_ExtString xstr_descriptor (str_source);

    //   Extract descriptor comment
    //   Match:   <text><left parenth><italics><text><right parenth><text><\n>
    //   Capture: <left parenth><italics><text><right parenth>
    bool b_match = xstr_descriptor.b_RegexMatch (L".*(\\(_(.+?)(_?)\\)).*\n?");
    if (!b_match)
    {
        return true;
    }
        
    st_entry.str_Comment = xstr_descriptor.str_GetRegexMatch (1);
    int i_offset = xstr_descriptor.i_GetFieldOffset (0, CT_ExtString::ec_TokenRegexMatch);
    int i_length = xstr_descriptor.i_GetFieldLength (0, CT_ExtString::ec_TokenRegexMatch);
    wstring str_left = xstr_descriptor.substr (0, i_offset);
    wstring str_right = xstr_descriptor.substr (i_offset + i_length);
    xstr_descriptor = str_left + str_right;

    xstr_descriptor.v_Reset();
    xstr_descriptor.v_SetBreakChars (L" ");
    xstr_descriptor.v_Trim();

    if (xstr_descriptor.empty())
    {
        str_source.erase();
    }
    else
    {
        str_source = xstr_descriptor.c_str();
    }

    return true;

}   //     //  b_CheckDescriptorComment (...)

bool CT_ZalEntryParser::b_CheckInflectionType (wstring& str_source, ST_Descriptor& st_entry)
{
    CT_ExtString xstr_source (str_source);

    // Extract inflection type (if different from main symbol, 
    // e.g., б<абий п <мс 6*а>
    bool b_match = xstr_source.b_RegexMatch (L"(<(.+?)>).*");
    if (!b_match)
    {
        return true;
    }

    CT_ExtString xstr_descriptor (xstr_source.str_GetRegexMatch (1), L" ");
    st_entry.str_InflectionType = xstr_descriptor.str_GetField (0);
    if (map_Pos.find (st_entry.str_InflectionType) == map_Pos.end())
    {
        ATLASSERT(0);
        wstring str_msg (L"Unidentified main symbol: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    wstring str_left;
    if (xstr_descriptor.i_NFields() > 1)
    {
        str_left = xstr_descriptor.substr (xstr_descriptor.i_GetFieldOffset (1));
    }

    int i_rightOffset = xstr_source.i_GetFieldOffset (0, CT_ExtString::ec_TokenRegexMatch) +
                        xstr_source.i_GetFieldLength (0, CT_ExtString::ec_TokenRegexMatch);
    wstring str_right = str_source.substr (i_rightOffset);

    str_source = str_left + str_right;

    return true;

}   //  b_CheckAlternateInflection (...)

bool CT_ZalEntryParser::b_ParseInflectionGroup (wstring& str_descriptor, ST_Descriptor& st_entry)
{
    CT_ExtString xstr_descriptor (str_descriptor);
    bool b_match = xstr_descriptor.b_RegexMatch 
        (L"^(\\d{0,2})(°?)(\\*?)([авсDеF']{0,2})(.*)");
     //   < type > < circle > < asterisk > < accent type > < tail >
  
    if (!b_match)
    {
        return true;
    }
     
    wstring str_type = xstr_descriptor.str_GetRegexMatch (0);
    if (str_type.length() > 0)
    {
        st_entry.i_Type = _wtoi (str_type.c_str());
    }

    wstring str_circle = xstr_descriptor.str_GetRegexMatch (1);
    if (str_circle.length() > 0)
    {
        st_entry.b_Circle = true;
    }
    wstring str_asterisk = xstr_descriptor.str_GetRegexMatch (2);
    if (str_asterisk.length() > 0)
    {
        st_entry.b_Asterisk = true;
    }

    wstring str_accentType = xstr_descriptor.str_GetRegexMatch (3);
    if (str_accentType.length() > 0)
    {
        if (map_AccentType.find (str_accentType) != map_AccentType.end())
        {
            st_entry.e_AccentType1 = map_AccentType[str_accentType];
        }
        else
        {
            ATLASSERT(0);
            wstring str_msg (L"Unrecognized accent type: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }
    }

    if (xstr_descriptor.empty())
    {
        str_descriptor.erase();
        return true;
    }

//&&&& TODO accent type2 (after "/")

    // Extract and keep everything between matches, add the last match
    int i_start = 0;
    wstring str_keep;
    for (int i_ = 0; i_ < xstr_descriptor.i_GetNumOfRegexMatches(); ++i_)
    {
        CT_ExtString::ST_Token st_token;
        bool b_ = xstr_descriptor.b_GetField (i_, CT_ExtString::ec_TokenRegexMatch, st_token);
        if (!b_)
        {
            ATLASSERT(0);
            wstring str_msg (L"Unable to access token: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }

        if (st_token.i_Length > 0)
        {
            str_keep += xstr_descriptor.substr (i_start, st_token.i_Offset - i_start);
            i_start = st_token.i_Offset + st_token.i_Length;
        }
    }

    // We expect it to be empty??
    if (!str_keep.empty())
    {
        ATLASSERT(0);
    }

    if (xstr_descriptor.i_NRegexMatches() > 0)
    {
        str_keep += xstr_descriptor.str_GetRegexMatch (xstr_descriptor.i_NRegexMatches() - 1);
        str_descriptor = str_keep;
        xstr_descriptor = str_descriptor;
    }

    xstr_descriptor.v_SetBreakChars (L" ");
    xstr_descriptor.v_Trim();
    xstr_descriptor.v_Reset();

    b_match = xstr_descriptor.b_RegexMatch (L"(.*)([!\\?])(.*)");
    if (!b_match)
    {
        str_descriptor = xstr_descriptor.c_str();
        return true;
    }
     
    ATLASSERT (xstr_descriptor.i_GetFieldLength (0, CT_ExtString::ec_TokenRegexMatch) == 0);
    wstring str_x = xstr_descriptor.str_GetRegexMatch (1);
    if (str_x.length() > 0)
    {
        if (L"!" == str_x)
        {
            st_entry.b_X = true;
        }
        if (L"?" == str_x)
        {
            st_entry.b_BoxedX = true;
        }
        ATLASSERT (st_entry.b_X || st_entry.b_BoxedX);
    }

    xstr_descriptor = xstr_descriptor.str_GetRegexMatch (2);

    if (xstr_descriptor.empty())
    {
        str_descriptor.erase();
        return true;
    }

    xstr_descriptor.v_Reset();        
    b_match = xstr_descriptor.b_RegexMatch (L"(.*)(~)(.*)");
    if (!b_match)
    {
        str_descriptor = xstr_descriptor.c_str();
        return true;
    }

    ATLASSERT (xstr_descriptor.i_GetFieldLength (0, CT_ExtString::ec_TokenRegexMatch) == 0);
    wstring str_tilde = xstr_descriptor.str_GetRegexMatch (1);
    if (str_tilde.length() > 0)
    {
        ATLASSERT (L"~" == str_tilde);
        st_entry.b_Tilde = true;
    }

    str_descriptor = xstr_descriptor.str_GetRegexMatch (2);

    return true;

}   //  b_ParseInflectionGroup (...)

bool CT_ZalEntryParser::b_CheckCircledDigit (wstring& str_descriptor, ST_Descriptor& st_entry)
{
    bool b_match = true;
    CT_ExtString xstr_descriptor (str_descriptor);
    do
    {
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
            wstring str_digit = xstr_descriptor.str_GetField (1, CT_ExtString::ec_TokenRegexMatch);
            if (!iswdigit (str_digit[0]))
            {
                ATLASSERT(0);
                wstring str_msg (L"Digit expected: ");
                str_msg += str_Source;
                ERROR_LOG (str_msg);
                return false;
            }
            st_entry.vec_iNumbersInCircle.push_back (_wtoi (&str_digit[0]));
            xstr_descriptor = xstr_descriptor.substr (st_token.i_Length);
        }

    } while (b_match);

    xstr_descriptor.v_SetBreakChars (L" ");
    xstr_descriptor.v_Trim();
    str_descriptor = xstr_descriptor.c_str();

    return true;

}   //  b_CheckCircledDigit (...)

bool CT_ZalEntryParser::b_CheckSquareBrackets (wstring& str_descriptor, ST_Descriptor& st_entry)
{
// [//< restriction><headword><main symb, inflection>]
// also possible: [<restriction...] (without "//")...]
// Everything except for headowrd is optional

// Using regex may be tricky since we need to handle nested brackets

    int i_startOffset = str_descriptor.find (L"[");
    if (wstring::npos == i_startOffset)
    {
        return true;
    }

    ATLASSERT (0 == i_startOffset);

    int i_endOffset = str_descriptor.rfind (L"]");
    if (wstring::npos == i_endOffset)
    {
        ATLASSERT(0);
        wstring str_msg (L"No closing bracket: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }
/*
    CT_ExtString xstr_descriptor (str_descriptor);
    bool b_match = xstr_descriptor.b_RegexMatch (L"(.*)?(?:\\[{1,}?(_.+_)*(.+)\\])(.*)");
    if (!b_match)
    {
        return true;
    }
*/
//    ATLASSERT (xstr_descriptor.i_NRegexMatches() == 3);
//    ATLASSERT (xstr_descriptor.i_GetFieldLength (0, CT_ExtString::ec_TokenRegexMatch) == 0);

    wstring str_alternate = str_descriptor.substr (i_startOffset + 1, 
                                                   i_endOffset - i_startOffset - 1);
    CT_ExtString xstr_alternate (str_alternate);
    bool b_match = xstr_alternate.b_RegexMatch (L"^(//)?(_.+_)?(.+)");
    if (!b_match)
    {
        ATLASSERT(0);
        wstring str_msg (L"Unable to parse bracketed alternate: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    if (xstr_alternate.i_NRegexMatches() < 1)
    {
        ATLASSERT(0);
        wstring str_msg (L"Empty bracketed alternate: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    for (int i_ = 0; i_ < xstr_alternate.i_NFields (CT_ExtString::ec_TokenRegexMatch); ++i_)
    {
        wstring str_token = xstr_alternate.str_GetRegexMatch (i_);
        if (L"//" == str_token)
        {
            // do we care?
            continue;
        }
        if ((L'_' == str_token[0]) && (L'_' == str_token[str_token.length()-1]))
        {
            st_entry.str_UsageComment = str_token.substr (1, str_token.length()-2);
            continue;
        }

        CT_ExtString xstr_token (str_token, L" ");
        if (xstr_token.i_NFields() == 1)
        {
            st_Head.str_SeeAlso = xstr_token.str_GetField (0);
            continue;
        }

        CT_ZalEntryParser co_entry (str_token);
        bool b_ = co_entry.b_Parse();
        if (!b_)
        {
            return false;
        }
    }

    return true;

}   //  b_CheckSquareBrackets (...)

bool CT_ZalEntryParser::b_CheckHatchedCircle (wstring& str_source, ST_Descriptor& st_entry)
{
//    CT_ExtString xstr_descriptor (str_descriptor, L", ");
//    wstring str_nextToken = xstr_descriptor.str_GetField (0);

    return true;
}
