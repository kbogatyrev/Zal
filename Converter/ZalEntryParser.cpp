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

    map_Pos[L"св"] = POS_VERB;
    map_Pos[L"нсв"] = POS_VERB;
    map_Pos[L"св-нсв"] = POS_VERB;

    map_Pos[L"п"] = POS_ADJ;
	
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

    wstring str_cyrillicLowercase = L"-абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    wstring str_vowels = L"аеёиоуыэюя";
//    unsigned char *walf= " ъ-эюёщфхцжшгчбзямудыйплквьсрнеитао";

    i_Offset = 0;

    i_Type = -1;
    b_PluralOf = false;

    b_Cross = false;
	b_BoxCross = false;
	b_Tilde = false;
	b_Asterisc = false;
	b_Circle = false;
	b_Minus = false;
	b_Loc = false;
	b_Part = false;
	b_Yo = false;
	b_O = false;

    i_Deviation = -1;
	i_Ref = -1;

}   //  v_Init()

bool CT_ZalEntryParser::b_Parse()
{
    xstr_Entry = str_Source;
    bool b_match = xstr_Entry.b_RegexMatch 
        (L"^[\\w\\s\\[\\]{}\\.,;:\"\'\\?~@#\\$\\%\\^\\&\\*\\(\\)-=\\+]+\n$");
    if (!b_match)
    {
        ATLASSERT(0);
        wstring str_msg (L"Illegal characters: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    xstr_Entry.v_SetBreakChars (L" \n");
    xstr_Entry.v_Trim();

    bool b_ = b_ExtractHead();
    if (!b_)
    {
        return false;
    }

    xstr_Entry.v_Reset();
    xstr_Entry.v_SetBreakChars (L"|");
    for (int i_at = 0; i_at < xstr_Entry.i_NFields(); ++i_at)
    {
        wstring str_descriptor (xstr_Entry.str_GetField (i_at));
        bool b_ = b_CheckMnOt (str_descriptor);
        if (!b_)
        {
            return false;
        }
        
        b_ = b_GetMainSymbol (str_descriptor);
        if (!b_)
        {
            return false;
        }

        b_ = b_CheckDescriptorComment (str_descriptor);
        if (!b_)
        {
            return false;
        }

        b_ = b_CheckAlternateInflection (str_descriptor);
        if (!b_)
        {
            return false;
        }

        if (str_InflectionType.empty())
        {
            str_InflectionType = str_MainSymbol;
        }
 


/*
        if (map_Pos.find (str_MainSymbol) != map_Pos.end())
        {
            e_PartOfSpeech = map_Pos[str_MainSymbol];
        }
        else
        {
            wstring str_msg (L"Unidentified main symbol: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }

        if (!str_MainSymbolVariant.empty())
        {
            if (map_Pos.find (str_MainSymbolVariant) != map_Pos.end())
            {
//&&&&                e_PartOfSpeech = map_Pos[str_MainSymbolVariant];
            }
            else
            {
                wstring str_msg (L"Unidentified main symbol variant: ");

                str_msg += str_Source;
                ERROR_LOG (str_msg);
                return false;
            }
        }

        if (!iswdigit (xstr_descriptor[0]))
        {
            ATLASSERT(0);
            wstring str_msg (L"Descriptor must begin with a digit: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }

        i_Type = _wtoi (&xstr_descriptor[0]);

int iiii = 0;
*/
    }

	return true;

}   //  i_Parse (...)

bool CT_ZalEntryParser::b_ExtractHead()
{
    CT_ExtString xstr_head (xstr_Entry.str_GetField (0));
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
         vec_iHomonyms.push_back (_wtoi (&str_digit[0]));
    }

    if (xstr_head.i_NRegexMatches() < 1)
    {
        ATLASSERT(0);
        wstring str_msg (L"No headword: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }
    str_Headword = xstr_head.str_GetRegexMatch (xstr_head.i_NRegexMatches() - 1);

    xstr_Entry = str_Source.substr (xstr_Entry.i_GetFieldOffset (1));
    b_match = xstr_Entry.b_RegexMatch (L"^(?:\\((.+?)\\)\\s)(.+)\n");
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
        str_HeadComment = xstr_Entry.str_GetRegexMatch (0);
        xstr_Entry = CT_ExtString (xstr_Entry.str_GetRegexMatch (1));
    }

    return true;

}   //  b_ExtractHead()

bool CT_ZalEntryParser::b_CheckMnOt (wstring& str_source)
{
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
    if (map_Pos.find (str_main) != map_Pos.end())
    {
        ATLASSERT(0);
        wstring str_msg (L"Unknown main symbol: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    if (L"мн." == str_main)
    {
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

        b_PluralOf = true;
        if (xstr_source.i_NFields() < 3)
        {
            ATLASSERT(0);
            wstring str_msg (L"Unterminated \"мн. _от_\": ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }

        if (map_Pos.find (xstr_source.str_GetField (2)) != map_Pos.end())
        {
            str_source = str_source.substr (xstr_source.i_GetFieldOffset (2));
            return true;
        }

        str_HeadWordPluralOf = xstr_source.str_GetField (2);
        if (xstr_source.i_NFields() < 4)
        {
            ATLASSERT(0);
            wstring str_msg (L"Unterminated \"мн. _от_ <headword>\": ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }

        str_source = str_source.substr (xstr_source.i_GetFieldOffset (3));

/*
                if (map_Pos.find (xstr_source.str_GetField (3)) != map_Pos.end())
                {
//                    str_MainSymbolPluralOf = xstr_source.str_GetField (3);
                    str_source = str_source.substr (xstr_source.i_GetFieldOffset (3));
                }
                else
                {
                    wstring str_msg (L"Unidentified main symbol: ");
                    str_msg += str_Source;
                    ERROR_LOG (str_msg);
                    return false;
                }
*/
        }
        
/*
        if (xstr_source.find (L"неод.") == xstr_source.i_GetFieldOffset (1))
        {
            str_MainSymbol = L"мн. неод.";
            xstr_source = xstr_source.substr (xstr_source.i_GetFieldOffset (1));
        }

        if (xstr_source.find (L"одуш.") == xstr_source.i_GetFieldOffset (1))
        {
            str_MainSymbol = L"мн. одуш.";
        }

    }   //     if (L"мн." == str_main)
    else
    {
        str_MainSymbol = str_main;
        xstr_source = xstr_source.substr (xstr_source.i_GetFieldOffset (1));
    }
*/
    return true;
}

bool CT_ZalEntryParser::b_GetMainSymbol (wstring& str_source)
{
//    CT_ExtString xstr_descriptor (str_source, L" \n");
    CT_ExtString xstr_descriptor (str_source, L" ");
    
    CT_ExtString xstr_mainSymb (xstr_descriptor.str_GetField (0), L"/,");
    str_MainSymbol = xstr_mainSymb.str_GetField (0);
    if (map_Pos.find (str_MainSymbol) == map_Pos.end())
    {
        ATLASSERT(0);
        wstring str_msg (L"Unidentified main symbol: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }
    if (xstr_mainSymb.i_NFields() > 1)
    {
        CT_ExtString xstr_variant (xstr_mainSymb.str_GetField (1));
        bool b_match = xstr_variant.b_RegexMatch (L"_(.+?)_.*");
        if (b_match)
        {
            str_MainSymbolVariantComment = xstr_variant.str_GetRegexMatch (0);
            int i_offset = xstr_variant.i_GetFieldOffset (0, CT_ExtString::ec_TokenRegexMatch);
            int i_length = xstr_variant.i_GetFieldLength (0, CT_ExtString::ec_TokenRegexMatch);
            wstring str_left = xstr_variant.substr (0, i_offset);
            wstring str_right = xstr_variant.substr (i_offset + i_length);
            xstr_variant = str_left + str_right;

            xstr_variant.v_Reset();
            xstr_variant.v_SetBreakChars (L" ");
            xstr_variant.v_Trim();
        }

        str_MainSymbolVariant = xstr_variant.c_str();
        if (map_Pos.find (str_MainSymbolVariant) == map_Pos.end())
        {
            ATLASSERT(0);
            wstring str_msg (L"Unidentified main symbol variant: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }
    }

    if (xstr_descriptor.i_NFields() < 2)
    {
        str_source.erase();
        return true;
    }

    str_source = str_source.substr (xstr_descriptor.i_GetFieldOffset (1));

    return true;

}   //  b_GetMainSymbol (...)

bool CT_ZalEntryParser::b_CheckDescriptorComment (wstring& str_source)
{
    CT_ExtString xstr_descriptor (str_source);

    //   Extract descriptor comment
    //   Match:   <text><left parenth><italics><text><right parenth><text><\n>
    //   Capture: <left parenth><italics><text><right parenth>
    bool b_match = xstr_descriptor.b_RegexMatch (L".*(\\(_(.+?)\\)).*\n?");
    if (!b_match)
    {
        return true;
    }
        
    str_DescriptorComment = xstr_descriptor.str_GetRegexMatch (1);
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

bool CT_ZalEntryParser::b_CheckAlternateInflection (wstring& str_source)
{
    CT_ExtString xstr_descriptor (str_source);

    // Extract inflection type (if different from main symbol, 
    // e.g., б<абий п <мс 6*а>
    bool b_match = xstr_descriptor.b_RegexMatch (L"<(.+?)>.*");
    if (!b_match)
    {
        return true;
    }
        
    xstr_descriptor = xstr_descriptor.str_GetRegexMatch (0);
    str_InflectionType = xstr_descriptor.str_GetField (0);
    int i_offset = xstr_descriptor.i_GetFieldOffset (0, CT_ExtString::ec_TokenRegexMatch);
    int i_length = xstr_descriptor.i_GetFieldLength (0, CT_ExtString::ec_TokenRegexMatch);
    wstring str_left = xstr_descriptor.substr (0, i_offset);
    wstring str_right = xstr_descriptor.substr (i_offset + i_length);
    xstr_descriptor = str_left + str_right;

    if (map_Pos.find (str_MainSymbol) == map_Pos.end())
    {
        ATLASSERT(0);
        wstring str_msg (L"Unidentified main symbol: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    str_source = xstr_descriptor.c_str();

    return true;
}
