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
    map_Pos[L"со"] = POS_NOUN;
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

//    wstring str_cyrillicLowercase = L"-абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
//    wstring str_vowels = L"аеёиоуыэюя";
//    unsigned char *walf= " ъ-эюёщфхцжшгчбзямудыйплквьсрнеитао";

    b_CommaAfterMainSymbol = false;

}   //  v_Init()

bool CT_ZalEntryParser::b_Parse (const wstring& str_head, const wstring str_usageComment)
{
    st_Head.str_Usage = str_usageComment;

    wstring str_entry (str_Source);

    if (!str_head.empty())
    {
        ATLASSERT (b_SecondPass);
        st_Head.str_Source = str_head;
    }
    else
    {
        bool b_ = b_ExtractHead (str_entry);
        if (!b_)
        {
            return false;
        }
    }

    for (int i_at = 0; !str_entry.empty(); ++i_at)
    {
        CT_ExtString xstr_descriptor (str_entry, L" ");
        xstr_descriptor.v_Trim();
        if (L';' == xstr_descriptor[0])
        {
            if (i_at < 1)
            {
                ATLASSERT(0);
                wstring str_msg (L"Unexpected \';\' at entry start: ");
                str_msg += str_Source;
                ERROR_LOG (str_msg);
                return false;
            }
            str_entry = xstr_descriptor.substr (1);
        }
        else
        {
            str_entry = xstr_descriptor.c_str();
        }

        ST_Descriptor st_entry;

        bool b_ = b_CheckMnOt (str_entry, st_entry);
        if (!b_)
        {
            return false;
        }
        
        b_ = b_GetMainSymbol (str_entry, st_entry);
        if (!b_)
        {
            return false;
        }

        b_ = b_CheckInflectionType (str_entry, st_entry);
        if (!b_)
        {
            return false;
        }

        b_ = b_ParseInflectionGroup (str_entry, st_entry.st_InflectionGroup);
        if (!b_)
        {
            return false;
        }

        if (str_entry.find (L"//") == 0)
        {
            str_entry = str_entry.substr (2);
            b_ = b_ParseInflectionGroup (str_entry, st_entry.st_AltInflectionGroup);
            if (!b_)
            {
                return false;
            }
        }
        
        xstr_descriptor = str_entry;
        xstr_descriptor.v_Reset();
        xstr_descriptor.v_SetBreakChars (L" ");
        xstr_descriptor.v_Trim();
        xstr_descriptor.v_Reset();
        bool b_match = xstr_descriptor.b_RegexMatch (L"^(?:\\(\\-([щсдтбгкнми])\\-\\))(.*)");
        if (b_match)
        {
            st_entry.str_VerbAlternation = xstr_descriptor.str_GetRegexMatch (0);
            str_entry = xstr_descriptor.str_GetRegexMatch (1);
        }

        //b_ = b_CheckCircledDigit (str_entry, st_entry);
        //if (!b_)
        //{
        //    return false;
        //}

        b_ = b_CheckTilde (str_entry, st_entry);
        if (!b_)
        {
            return false;
        }

        if (L'-' == str_entry[0])
        {
            st_entry.b_Dash = true;
            CT_ExtString xstr_descriptor (str_entry.substr (1), L" ");
            xstr_descriptor.v_Trim();
            str_entry = xstr_descriptor.c_str();
        }

        b_ = b_CheckSharp (str_entry, st_entry);
        if (!b_)
        {
            return false;
        }

        b_ = b_CheckTriangle (str_entry, st_entry);
        if (!b_)
        {
            return false;
        }

		b_ = b_CheckColon (str_entry, st_entry);
		if (!b_)
        {
            return false;
        }

        b_CheckSquareBrackets (str_entry, st_entry);
        if (!b_)
        {
            return false;
        }

        b_ = b_CheckComma (str_entry, st_entry);
		if (!b_)
        {
            return false;
        }

		b_ = b_CheckDollar (str_entry, st_entry);
		if (!b_)
        {
            return false;
        }

        xstr_descriptor = str_entry;
        b_ = xstr_descriptor.b_RegexMatch (L"(?:\\s)*\\(_(.+?)_\\)(.*)");
        if (b_)
        {
            ATLASSERT (xstr_descriptor.i_GetNumOfRegexMatches() > 0);
            st_entry.str_TrailingComment = xstr_descriptor.str_GetRegexMatch (0);
            if (xstr_descriptor.i_GetNumOfRegexMatches() > 1)
            {
                str_entry = xstr_descriptor.str_GetRegexMatch (1);
            }
            else
            {
                str_entry.erase();
            }
        }

        vec_Descriptors.push_back (st_entry);

    }   //  for (int i_at = 0; ...)

    return true;

}   //  i_Parse (...)

bool CT_ZalEntryParser::b_ExtractHead (wstring& str_source)
{
    CT_ExtString xstr_source (str_source, L" ");
    CT_ExtString xstr_head (xstr_source.str_GetField (0));
    bool b_match = xstr_head.b_RegexMatch (L"^(?:([1-5])?(?:,([1-5]))*/)?([^ ]+)");

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
        st_Head.str_Comment = xstr_source.str_GetRegexMatch (0);
        xstr_source = CT_ExtString (xstr_source.str_GetRegexMatch (1));
    }

    str_source = xstr_source.c_str();

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

bool CT_ZalEntryParser::b_CheckInflectionType (wstring& str_source, ST_Descriptor& st_entry)
{
    CT_ExtString xstr_source (str_source);

    // Extract inflection type (if different from main symbol, 
    // e.g., б<абий п <мс 6*а>)
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

bool CT_ZalEntryParser::b_ParseInflectionGroup (wstring& str_descriptor, 
                                                ST_InflectionGroup& st_entry)
{
    CT_ExtString xstr_descriptor (str_descriptor);
    bool b_match = xstr_descriptor.b_RegexMatch 
        (L"^(\\d{0,2})(°?)(\\*?)([авсDеF]'?(/[авсDеF]'?)?)(.*)");
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

    CT_ExtString xstr_accentType (xstr_descriptor.str_GetRegexMatch (3), L"/");
    if (xstr_accentType.length() > 0)
    {
        int i_nFields = xstr_accentType.i_GetNumOfFields();
        if ((i_nFields > 2) || (i_nFields < 1))
        {
            ATLASSERT(0);
            wstring str_msg (L"Bad number of accent type fields: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }
        
        wstring str_ap1 = xstr_accentType.str_GetField (0);
        if (map_AccentType.find (str_ap1) != map_AccentType.end())
        {
            st_entry.e_AccentType1 = map_AccentType[str_ap1];
        }
        else
        {
            ATLASSERT(0);
            wstring str_msg (L"Unrecognized accent type: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }

        if (i_nFields > 1)
        {
            wstring str_ap2 = xstr_accentType.str_GetField (1);
            if (map_AccentType.find (str_ap2) != map_AccentType.end())
            {
                st_entry.e_AccentType2 = map_AccentType[str_ap2];
            }
            else
            {
                ATLASSERT(0);
                wstring str_msg (L"Unrecognized alterane accent type: ");
                str_msg += str_Source;
                ERROR_LOG (str_msg);
                return false;
            }
        }
    }

    if (xstr_descriptor.empty())
    {
        str_descriptor.erase();
        return true;
    }

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
    if (b_match)
    {    
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
        str_descriptor = xstr_descriptor.str_GetRegexMatch (2);
    }
    else
    {
        str_descriptor = xstr_descriptor.c_str();
    }

    bool b_ = b_CheckCircledDigit (str_descriptor, st_entry);
    if (!b_)
    {
        return false;
    }

    return true;

}   //  b_ParseInflectionGroup (...)

bool CT_ZalEntryParser::b_CheckCircledDigit (wstring& str_descriptor, ST_InflectionGroup& st_entry)
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

bool CT_ZalEntryParser::b_CheckTilde (wstring& str_descriptor, ST_Descriptor& st_entry)
{
    CT_ExtString xstr_descriptor (str_descriptor);        
    bool b_match = xstr_descriptor.b_RegexMatch (L"(.*)(~)(.*)");
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
}

bool CT_ZalEntryParser::b_CheckSquareBrackets (wstring& str_descriptor, ST_Descriptor& st_entry)
{
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

    wstring str_source = str_descriptor.substr (i_startOffset + 1, 
                                                i_endOffset - i_startOffset - 1);

    str_descriptor = str_descriptor.substr (i_endOffset+1);

    CT_ExtString xstr_source (str_source);

    //  аб<ак м 3а [//аб<ака]
    bool b_match = xstr_source.b_RegexMatch (L"^//([а-я<>]+)");
    if (b_match)
    {
        st_Head.str_Variant = xstr_source.str_GetRegexMatch (0);
        return true;
    }

    //  ак<анф м 1а [//ак<ант (_см._)]
    b_match = xstr_source.b_RegexMatch (L"^//([а-я<>]+)(?:\\s)(\\(_см._\\))");
    if (b_match)
    {
        st_Head.str_SeeAlso = xstr_source.str_GetRegexMatch (0);
        return true;
    }

    //  к<апсель м 2а [_проф._ м 2с"1"]
    //  б<ондарь мо 2а [//бонд<арь мо 2в] 
    //  врожд<ённый п 1*а"1" [//п 1*а/в"2", ё]
    b_match = xstr_source.b_RegexMatch 
        (L"^(//)?(_[а-я\\s\\.\\-]+_\\s)?([^\\s]+)(?:\\s)?([^\\s+])?(?:\\s)?(.+)?");
    if (b_match)
    {
        int i_offset = 0;
        wstring str_usageComment;
        bool b_mainSymbolFound = false;
        bool b_headWordFound = false;
        for (int i_at = 0; 
             !b_mainSymbolFound && (i_at < xstr_source.i_GetNumOfRegexMatches()); 
             ++i_at)
        {
            wstring str_captureGroup = xstr_source.str_GetRegexMatch (i_at);
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
                b_headWordFound = true; // additional tests here?
            }
        }

        if (b_mainSymbolFound)
        {
            wstring str_entry (str_source.substr (i_offset));
            bool b_secondPass = true;
            CT_ZalEntryParser co_entry (str_entry, b_secondPass);
            wstring str_head;
            if (!b_headWordFound)
            {
                str_head = st_Head.str_Source;
            }
            bool b_parseResult = co_entry.b_Parse (str_head, str_usageComment);
            if (!b_parseResult)
            {
                wstring str_msg(L"*** Parsing error: \n");
                str_msg += str_entry;
                ERROR_LOG (str_msg.c_str());
                return false;
            }
        }
        else
        {
            // TODO: unable to parse
            return false;
        }
    }

    return true;

}   //  b_CheckSquareBrackets (...)

bool CT_ZalEntryParser::b_CheckComma (wstring& str_descriptor, ST_Descriptor& st_entry)
{
    if (L',' == str_descriptor[0])
    {
        CT_ExtString xstr_descriptor (str_descriptor, L", ");
        if (xstr_descriptor.i_GetNumOfFields() < 1)
        {
            ATLASSERT(0);
            wstring str_msg (L"Continuation expected after comma: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }

        str_descriptor = str_descriptor.substr (1); // default value to be returned in 
                                                    // case we don't find anything interesting

        CT_ExtString xstr_afterComma = xstr_descriptor.str_GetField (0);
        bool b_ = xstr_afterComma.b_RegexMatch (L"#(\\d{1,2}).*");
        if (b_)
        {
            wstring str_sectionNum = xstr_afterComma.str_GetRegexMatch (0);
            st_entry.i_Section = _wtoi (str_sectionNum.c_str());
            str_descriptor = str_descriptor.substr (xstr_descriptor.i_GetFieldOffset (0) + 
                                                    xstr_descriptor.i_GetFieldLength (0));
        }

        if (L'ё' == xstr_afterComma[0])
        {
            st_entry.b_Yo = true;
            str_descriptor = str_descriptor.substr (xstr_descriptor.i_GetFieldOffset (0) + 
                                                    xstr_descriptor.i_GetFieldLength (0));
        }

        if (L'о' == xstr_afterComma[0])
        {
            st_entry.b_O = true;
            str_descriptor = str_descriptor.substr (xstr_descriptor.i_GetFieldOffset (0) + 
                                                    xstr_descriptor.i_GetFieldLength (0));
        }

        if (L"Р2" == xstr_afterComma)
        {
            st_entry.b_Gen2 = true;
            str_descriptor = str_descriptor.substr (xstr_descriptor.i_GetFieldOffset (0) + 
                                                    xstr_descriptor.i_GetFieldLength (0));
        }

        b_ = xstr_afterComma.b_RegexMatch (L".?(П2\\s?(?:\\(([а-я]{1,2})\\))?).*");
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
                        wstring str_msg (L"Righ bracket missing: ");
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
                str_descriptor = str_descriptor.substr (xstr_descriptor.i_GetFieldOffset (0) +
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
    }

    return true;

}   //  b_CheckComma (...)
