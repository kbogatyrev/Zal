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

        b_ = b_ParseInflectionGroup (str_entry, st_entry);
        if (!b_)
        {
            return false;
        }

        if (str_entry.find (L"//") == 0)
        {
            str_entry = str_entry.substr (2);
            b_ = b_ParseInflectionGroup (str_entry, st_entry);
            if (!b_)
            {
                return false;
            }
        }
        
        xstr_descriptor = str_entry;
        xstr_descriptor.v_Reset();
        xstr_descriptor.v_SetBreakChars (L", ");
        xstr_descriptor.v_Trim();
        xstr_descriptor.v_Reset();
        bool b_match = xstr_descriptor.b_RegexMatch (L"^(?:\\(\\-([щсдтбгкнми]+)\\-\\))(.*)");
        if (b_match)
        {
            st_entry.str_VerbAlternation = xstr_descriptor.str_GetRegexMatch (0);
            str_entry = xstr_descriptor.str_GetRegexMatch (1);
        }
        else
        {
            b_match = 
                xstr_descriptor.b_RegexMatch (L"^(?:\\(([абвгдеёжзийклмнопрстуфхцчшщъыьэюя]+)\\-\\))(.*)");
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
                st_entry.str_VerbAlternation = xstr_descriptor.str_GetRegexMatch (0);
                str_entry = xstr_descriptor.str_GetRegexMatch (1);
            }
            else
            {
                if (xstr_descriptor.b_StartsWith (L"_прич. страд._ -жд-"))
                {
                    st_entry.b_PastPartPassZhD = true;
                }
            }
        }

        // Here we have to repeat common deviations check since sometimes
        // a circled digit may appear after stem alternation 
        b_ = b_CheckCircledDigits (str_entry, st_entry.st_InflectionGroup);
        if (!b_)
        {
            return false;
        }

        b_ = b_CheckTilde (str_entry, st_entry);
        if (!b_)
        {
            return false;
        }

        if (L'-' == str_entry[0])
        {
            st_entry.b_AssumedForms = true;
            CT_ExtString xstr_descriptor (str_entry.substr (1), L" ");
            xstr_descriptor.v_Trim();
            str_entry = xstr_descriptor.c_str();
        }

        b_ = b_CheckCognate (str_entry, st_entry);
        if (!b_)
        {
            return false;
        }

        b_ = b_CheckInflectedParts (str_entry, st_entry);
        if (!b_)
        {
            return false;
        }

        b_ = b_CheckVerbType (str_entry, st_entry);
        if (!b_)
        {
            return false;
        }

        b_ = b_CheckSharp (str_entry, st_entry);
        if (!b_)
        {
            return false;
        }

		b_ = b_CheckDifficulties (str_entry, st_entry);
        if (!b_)
        {
            return false;
        }

        b_ = b_CheckTriangle (str_entry, st_entry);
        if (!b_)
        {
            return false;
        }
        if (!st_entry.str_IrregularForms.empty())
        {
            st_entry.b_HasIrregularForms = true;
        }

		b_ = b_CheckColon (str_entry, st_entry);
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

		b_ = b_CheckPercent (str_entry, st_entry);
		if (!b_)
        {
            return false;
        }

        b_ = b_CheckSquareBrackets (str_entry, st_entry);
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

        b_ = b_MakeGraphicStem (st_entry);
        vec_Descriptors.push_back (st_entry);

    }   //  for (int i_at = 0; ...)

    __int64 ll_wordId = 0;
    st_Head.b_SaveToDb (pco_Db, ll_wordId);
    for (unsigned int ui_entry = 0; ui_entry < vec_Descriptors.size(); ++ui_entry)
    {
        ST_Descriptor& st_entry = vec_Descriptors[ui_entry];
        __int64 ll_descriptorId = -1;
        bool b_ret = st_entry.b_SaveToDb (pco_Db, ll_wordId, ll_descriptorId);
        if (!b_ret)
        {
            continue;
        }

        st_LexemeHasher.str_SourceForm = st_Head.str_Source;
        vector<int>::iterator it_homonym = st_Head.vec_IHomonyms.begin();
        for (; it_homonym != st_Head.vec_IHomonyms.end(); ++it_homonym)
        {
            st_LexemeHasher.str_SourceForm += str_ToString (*it_homonym);
        }
        st_LexemeHasher.map_Stress = st_Head.map_Stress;
        st_LexemeHasher.str_MainSymbol = st_entry.str_MainSymbol;
        st_LexemeHasher.str_Comment = st_Head.str_Comment;
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
    }

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

    bool b_ret = b_ExtractStress (xstr_head.str_GetRegexMatch (xstr_head.i_NRegexMatches() - 1),
                                  st_Head.map_Stress);

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

bool CT_ZalEntryParser::b_ExtractStress (const wstring& str_headWord, map<int, ET_StressType>& map_stress)
{
    CT_ExtString xstr_headWord (str_headWord, L"<>");

    for (int i_textField = 0; i_textField < xstr_headWord.i_GetNumOfFields(); ++i_textField)
    {
        st_Head.str_Source += xstr_headWord.str_GetField (i_textField);
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

bool CT_ZalEntryParser::b_CheckMnOt (wstring& str_source, ST_Descriptor& st_entry)
{
    st_entry.b_PluralOf = false;

    CT_ExtString xstr_source (str_source, L" /,;:");

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
        int i_offset = xstr_source.i_GetFieldOffset (1) + xstr_source.i_GetFieldLength (1);
        str_source = str_source.substr (i_offset);

        return true;
    }

    st_Head.str_PluralOf = xstr_source.str_GetField (2);
    if (xstr_source.i_NFields() < 4)
    {
        ATLASSERT(0);
        wstring str_msg (L"Unterminated \"мн. _от_ <headword>\": ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    int i_offset = xstr_source.i_GetFieldOffset (2) + xstr_source.i_GetFieldLength (2);
    str_source = str_source.substr (i_offset);
    
    return true;

}   //  b_CheckMnOt (...)

bool CT_ZalEntryParser::b_GetMainSymbol(wstring& str_source, ST_Descriptor& st_entry)
{
    CT_ExtString xstr_descriptor (str_source, L" ;:");
    
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
        str_source.erase();
    }
    else
    {
        int i_offset = xstr_descriptor.i_GetFieldOffset (i_nextField - 1) + 
                       xstr_descriptor.i_GetFieldLength (i_nextField - 1);
        str_source = str_source.substr (i_offset);
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
    st_entry.e_MainSymbol = map_MainSymbol[st_entry.str_MainSymbol];
    st_entry.e_InflectionSymbol = map_MainSymbol[st_entry.str_MainSymbol];

    return true;

}   //  b_GetMainSymbol (...)

bool CT_ZalEntryParser::b_CheckInflectionType (wstring& str_source, ST_Descriptor& st_entry)
{
    CT_ExtString xstr_source (str_source, L" ");
    xstr_source.v_Trim();
    str_source = xstr_source.c_str();

    st_entry.str_InflectionSymbol = st_entry.str_MainSymbol;  // default

    // Extract inflection type (if different from main symbol, 
    // e.g., б<абий п <мс 6*а>)
    bool b_match = xstr_source.b_RegexMatch (L"(<(.+?)>).*");
    if (!b_match)
    {
        return true;
    }

    CT_ExtString xstr_descriptor (xstr_source.str_GetRegexMatch (1), L" ");
    st_entry.str_InflectionSymbol = xstr_descriptor.str_GetField (0);
    if (map_Pos.find (st_entry.str_InflectionSymbol) == map_Pos.end())
    {
        ATLASSERT(0);
        wstring str_msg (L"Unidentified main symbol: ");
        str_msg += str_Source;
        ERROR_LOG (str_msg);
        return false;
    }

    st_entry.e_InflectionSymbol = map_MainSymbol[st_entry.str_InflectionSymbol];

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

}   //  b_CheckInflectionType (...)

bool CT_ZalEntryParser::b_ParseInflectionGroup (wstring& str_descriptor, 
                                                ST_Descriptor& st_entry)
{
    CT_ExtString xstr_descriptor (str_descriptor, L" ");
    xstr_descriptor.v_Trim();

    ST_InflectionGroup& st_inflectionGroup = st_entry.st_InflectionGroup.b_HasData 
        ? st_entry.st_AltInflectionGroup 
        : st_entry.st_InflectionGroup;

    bool b_match = xstr_descriptor.b_RegexMatch (L"^(\\d{1,2}).*");
    if (b_match)
    {
        st_inflectionGroup.b_HasData = true;

        wstring str_type = xstr_descriptor.str_GetRegexMatch (0);
        if (str_type.length() < 1)
        {
            ATLASSERT(0);
            wstring str_msg (L"Error retrieving inflection type number: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }
        st_inflectionGroup.i_Type = _wtoi (str_type.c_str());
        str_descriptor = xstr_descriptor.substr (str_type.length());
    }
    if (str_descriptor.empty())
    {
        return true;
    }

    if (L'°' == str_descriptor[0])
    {
//        st_entry.b_StemAugment = true;
        if (POS_NOUN == map_Pos[st_entry.str_MainSymbol])
        {
            str_descriptor = str_descriptor.substr (1);
            if (3 == st_inflectionGroup.i_Type)
            {
                CT_ExtString xstr_headWord (st_Head.str_Source);
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
            str_descriptor = str_descriptor.substr (1);
            st_inflectionGroup.i_StemAugmentType = 1;
        }
    }
    else
    {
        st_inflectionGroup.i_StemAugmentType = -1;
    }

    if (str_descriptor.empty())
    {
        return true;
    }

    if (L'*' == str_descriptor[0])
    {
        st_inflectionGroup.b_FleetingVowel = true;
        str_descriptor = str_descriptor.substr (1);
    }
    if (str_descriptor.empty())
    {
        return true;
    }

    xstr_descriptor = str_descriptor;
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
        str_descriptor = str_descriptor.substr (str_at1.length());
    }
    if (str_descriptor.empty())
    {
        return true;
    }
    
    xstr_descriptor = str_descriptor;
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
        str_descriptor = str_descriptor.substr (i_offset);
    }

    if (str_descriptor.empty())
    {
        return true;
    }

    xstr_descriptor = str_descriptor;
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
        str_descriptor = xstr_descriptor.str_GetRegexMatch (0) + xstr_descriptor.str_GetRegexMatch (2);
    }
    else
    {
        str_descriptor = xstr_descriptor.c_str();
    }

    bool b_ = b_CheckCircledDigits (str_descriptor, st_inflectionGroup);
    if (!b_)
    {
        return false;
    }

    return true;

}   //  b_ParseInflectionGroup (...)

bool CT_ZalEntryParser::b_CheckCircledDigits (wstring& str_descriptor, ST_InflectionGroup& st_entry)
{
    bool b_match = true;
    CT_ExtString xstr_descriptor (str_descriptor);
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
    str_descriptor = xstr_descriptor.c_str();

    return true;

}   //  b_CheckCircledDigit (...)

bool CT_ZalEntryParser::b_CheckTilde (wstring& str_descriptor, ST_Descriptor& st_entry)
{
    CT_ExtString xstr_descriptor (str_descriptor);
    bool b_match = xstr_descriptor.b_RegexMatch (L"(.*?)(~)(.*)");
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
        st_entry.b_NoComparative = true;
    }

    str_descriptor = xstr_descriptor.str_GetRegexMatch (2);

    return true;
}

bool CT_ZalEntryParser::b_CheckSquareBrackets (wstring& str_descriptor, ST_Descriptor& st_entry)
{
// Using regex may be tricky since we need to handle nested brackets

    int i_startOffset = str_descriptor.find_first_of (L";[");
    if (wstring::npos == i_startOffset)
    {
        return true;
    }

    //
    // Don't go beyond first ";"
    //
    if (L';' == str_descriptor[i_startOffset])
    {
        return true;
    }

    ATLASSERT (0 == i_startOffset);

    int i_endOffset = str_descriptor.find_first_of (L"]");
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
    bool b_match = xstr_source.b_RegexMatch (L"^//([абвгдеёжзийклмнопрстуфхцчшщъыьэюя<>]+)");
    if (b_match)
    {
        st_Head.str_Variant = xstr_source.str_GetRegexMatch (0);
        return true;
    }

    //  ак<анф м 1а [//ак<ант (_см._)]
    b_match = xstr_source.b_RegexMatch 
                (L"^//([абвгдеёжзийклмнопрстуфхцчшщъыьэюя<>]+)(?:\\s)(\\(_см._\\))");
    if (b_match)
    {
        st_Head.str_SeeAlso = xstr_source.str_GetRegexMatch (0);
        return true;
    }

    //  к<апсель м 2а [_проф._ м 2с"1"]
    //  б<ондарь мо 2а [//бонд<арь мо 2в] 
    //  врожд<ённый п 1*а"1" [//п 1*а/в"2", ё]
    b_match = xstr_source.b_RegexMatch 
        (L"^(//)?(_[абвгдеёжзийклмнопрстуфхцчшщъыьэюя\\s\\.\\-]+_\\s)?([^\\s]+)(?:\\s)?([^\\s+])?(?:\\s)?(.+)?");
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
            CT_ZalEntryParser co_entry (str_entry, pco_Db, b_secondPass);
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

bool CT_ZalEntryParser::b_CheckCognate (wstring& str_descriptor, ST_Descriptor& st_entry)
{
    // Handles cases:
    // (_от_ ...)
    // (_женск. к_ ...)
    // (_уменьш. к_ ...)
    // (_ласкат. к_ ...)
    // (_сущ. к_ ...)
    // (_ср._ ...)
    // (_см. также_ ...)
    if (str_descriptor.length() <= 0)
    {
        return true;
    }
    bool b_match = false;
    wsmatch ct_result;
    wstring str_left, str_right, str_cognate;
    b_match = regex_search(str_descriptor, (const wregex)L"\\(_(женск\\.\\s*к|уменьш\\.\\s*к|ласкат\\.\\s*к|сущ\\.\\s*к|ср\\.|см\\.\\s*также|от)_");
    if (b_match == false)
    {
        return true;
    }

    b_match = regex_match(str_descriptor, ct_result, (const wregex)L"(.*?),?\\s*\\(_(женск\\.\\s*к|ср\\.|уменьш\\.\\s*к|ласкат\\.\\s*к|сущ\\.\\s*к|см\\.\\s*также|от)_\\s*([^\\)]*)\\)(.*)");
    if (b_match == true)
    {
        str_left = ct_result[1];
        str_right = ct_result[4];
        str_cognate = ct_result[3];
        str_descriptor = str_left + str_right;
        st_entry.str_Cognate = str_cognate;
        return true;
    }

    ERROR_LOG(L"CheckCognate was started on " + str_descriptor + L" and couldn't recognize the string.");
    return false;
}

bool CT_ZalEntryParser::b_CheckVerbType (wstring& str_descriptor, ST_Descriptor& st_entry)
{
    // Handles cases:
    // ..., безл.
    // ..., многокр.
    if (str_descriptor.length() <= 0)
    {
        return true;
    }
    bool b_match = false;
    wsmatch ct_result;
    b_match = regex_search(str_descriptor, (const wregex)L"^(.*?),?\\s*безл\\.(.*)");
    if (b_match == true)
    {
        str_descriptor = (wstring)ct_result[1] + (wstring)ct_result[2];
        st_entry.b_IsImpersonal = true;
    }
    b_match = regex_search(str_descriptor, (const wregex)L"^(.*?),?\\s*многокр\\.(.*)");
    if (b_match == true)
    {
        str_descriptor = (wstring)ct_result[1] + (wstring)ct_result[2];
        st_entry.b_IsIterative = true;
    }
    return true;
}

bool CT_ZalEntryParser::b_CheckDollar (wstring& str_descriptor, ST_Descriptor& st_entry)
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
    wstring str_comment, str_current;
    wsmatch result;
    unsigned int i_previous_length = 0;
    int i_step = 1;
    bool b_match = regex_match(str_descriptor, result, (const wregex)L".*\\$\\s*(.*)");
    if (b_match == false)
    {
        return true;
    }
    str_comment = result[1];
    st_entry.b_HasAspectPair = true;
    //ERROR_LOG(L"---- CheckDollar: " + str_comment);
    while (regex_match(str_comment, result, (const wregex)L"\\s*([^\\/;]+)([\\/;].*)?"))
    {
        str_current = result[1];
        str_comment = result[2];

        // 1. Roman numerals
        if (regex_match(str_current, result,
            (const wregex)L"((_\\s*н?св\\.?\\s*также\\s*_)?)\\s*(I+)\\s*((\\(([\\-<>\\w\\/]*|_[\\w<>\\-\\.]*_|[\\-<>\\w\\/]*,\\s*_[\\w<>\\-\\.]*_)\\))?)\\s*"))
        {
            st_entry.vec_i_AspectPairType.push_back(-1 * ((int)((wstring)result[3]).length()));
            st_entry.vec_str_AspectPairComment.push_back((wstring)result[1] + (wstring)result[4]);
            //ERROR_LOG(L"---- CheckDollar: Roman: " + str_ToString<int>(st_entry.vec_i_AspectPairType.at(0)) + L"; " + st_entry.vec_str_AspectPairComment.at(0));
        }
        // 2. Arabic numerals
        else if (regex_match(str_current, result,
            (const wregex)L"((_\\s*н?св\\.?\\s*также\\s*_)?)\\s*(\\d+)\\s*((\\(([\\-<>\\w\\/]*|_[\\w<>\\-\\.]*_|[\\-<>\\w\\/]*,\\s*_[\\w<>\\-\\.]*_)\\))?)\\s*"))
        {
            st_entry.vec_i_AspectPairType.push_back(_wtoi(((wstring)result[3]).c_str()));
            st_entry.vec_str_AspectPairComment.push_back((wstring)result[1] + (wstring)result[4]);
            //ERROR_LOG(L"---- CheckDollar: Arabic: " + str_ToString<int>(st_entry.vec_i_AspectPairType.at(0)) + L"; " + st_entry.vec_str_AspectPairComment.at(0));
        }
        // 3. No number at all
        else if (regex_match(str_current, result, (const wregex)L"([\\-<>\\w\\/]+)\\s*"))
        {
            st_entry.vec_i_AspectPairType.push_back(0);
            st_entry.vec_str_AspectPairComment.push_back((wstring)result[1]);
            //ERROR_LOG(L"---- CheckDollar: No number: " + str_ToString<int>(st_entry.vec_i_AspectPairType.at(0)) + L"; " + st_entry.vec_str_AspectPairComment.at(0));
        }
        else
        {
            //ERROR_LOG(L"CheckDollar couldn't recognize the text.");
            return false;
        }
    }
    str_descriptor = L"";
    return true;
}   //  b_CheckDollar (...)

bool CT_ZalEntryParser::b_CheckComma (wstring& str_descriptor, ST_Descriptor& st_entry)
{
    if (L',' != str_descriptor[0])
    {
        return true;
    }

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
        str_descriptor = xstr_descriptor.substr (xstr_descriptor.i_GetFieldOffset (0) + 
                                                 xstr_descriptor.i_GetFieldLength (0));
    }

    if (L'ё' == xstr_afterComma[0])
    {
        st_entry.b_Yo = true;
        str_descriptor = xstr_descriptor.substr (xstr_descriptor.i_GetFieldOffset (0) + 
                                                 xstr_descriptor.i_GetFieldLength (0));
    }

    if (L'о' == xstr_afterComma[0])
    {
        st_entry.b_O = true;
        str_descriptor = xstr_descriptor.substr (xstr_descriptor.i_GetFieldOffset (0) + 
                                                 xstr_descriptor.i_GetFieldLength (0));
    }

    if (xstr_afterComma.b_StartsWith (L"Р2"))
    {
        st_entry.b_Gen2 = true;
        str_descriptor = xstr_descriptor.substr (xstr_descriptor.i_GetFieldOffset (0) + 
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
            str_descriptor = xstr_descriptor.substr (xstr_descriptor.i_GetFieldOffset (0) +
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

bool CT_ZalEntryParser::b_MakeGraphicStem (ST_Descriptor& st_entry)
{
    CT_ExtString xstr_source = st_Head.str_Source;

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
                st_entry.str_GraphicStem = st_Head.str_Source;
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
                st_entry.str_GraphicStem = st_Head.str_Source;
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
            st_entry.str_GraphicStem = st_Head.str_Source;
        }
    }

    return true;

}   //  b_MakeGraphicStem()
