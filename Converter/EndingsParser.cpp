#include "StdAfx.h"
#include "EndingsParser.h"

void CT_EndingsParser::v_Init()
{
    e_Class = ECLASS_UNDEFINED;
    e_Gender = GENDER_UNDEFINED;
    i_InflectionType = -1;
    e_Case = CASE_UNDEFINED;
    i_Person = -1;
    e_Animacy = ANIM_UNDEFINED;
    e_Stress = ESTRESS_UNDEFINED;
    i_Conjugation = -1;
    b_StemAugment = false;
    i_CommonDeviation = 0;

}   //  v_Init()

bool CT_EndingsParser::b_Parse()
{
    v_Init();

    wchar_t chr_class = str_Source[0];
    switch (chr_class)
    {
        case L'N':
        {
            e_Class = ECLASS_NOUN;
            break;
        }
        case L'A':
        {
            e_Class = ECLASS_ADJECTIVE;
            break;
        }
        case L'P':
        {
            e_Class = ECLASS_PRONOUN;
            break;
        }
        case L'H':
        {
            e_Class = ECLASS_SHORT_ADJECTIVE;
            break;
        }
        case L'R':
        {
            e_Class = ECLASS_PRESENT_TENSE;
            break;
        }
        case L'S':
        {
            e_Class = ECLASS_PAST_TENSE;
            break;
        }
        default:
        {
            ATLASSERT(0);
            wstring str_msg (L"Unexpected character in class field: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }

    }   // switch ...

    xstr_Ending = str_Source.substr (1, 7);
    xstr_Ending.v_Trim();

    wchar_t chr_gender = str_Source[8];
    switch (chr_gender)
    {
        case L'm':
        {
            e_Gender = GENDER_M;
            break;
        }
        case L'n':
        {
            e_Gender = GENDER_N;
            break;
        }
        case L'f':
        {
            e_Gender = GENDER_F;
            break;
        }
        default:
        {
            e_Gender = GENDER_UNDEFINED;
            if (L' ' != chr_gender)
            {
                ATLASSERT(0);
                wstring str_msg (L"Unexpected character in gender field: ");
                str_msg += str_Source;
                ERROR_LOG (str_msg);
                return false;
            }
        }

    }   // switch ...

    if ((ECLASS_PRESENT_TENSE == e_Class) || (ECLASS_PAST_TENSE ==  e_Class))
    {
//        i_Conjugation = _wtoi (&str_Source[10]);
        i_InflectionType = _wtoi (&str_Source[10]);
    }
    else
    {
        i_InflectionType = _wtoi (&str_Source[9]);
        if (L'A' == str_Source[10])
        {
            e_Animacy = ANIM_YES;
        }
        else
        {
//            i_InflectionType = _wtoi (&str_Source[9]);
            // TODO: check value

            if (L'U' == str_Source[10])
            {
                e_Animacy = ANIM_NO;
            }
            else if (L' ' != str_Source[10])
            {
                ATLASSERT(0);
                wstring str_msg (L"Unexpected character in animacy field: ");
                str_msg += str_Source;
                ERROR_LOG (str_msg);
                return false;
            }
        }
    }

    if ((L'S' == str_Source[11]) || (L'A' == str_Source[11]))
    {
        e_Stress = ESTRESS_STRESSED;
    }
    else
    {
        if (L'U' == str_Source[11])
        {
            e_Stress = ESTRESS_UNSTRESSED;
        }
        else if (L' ' != str_Source[11])
        {
            ATLASSERT(0);
            wstring str_msg (L"Unexpected character in stress field: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }
    }

    if (L'S' == str_Source[12])
    {
        b_Plural = false;
    }
    else
    {
        if (L'P' == str_Source[12])
        {
            b_Plural = true;
        }
        else
        {
            ATLASSERT(0);
            wstring str_msg (L"Unexpected character in number field: ");
            str_msg += str_Source;
            ERROR_LOG (str_msg);
            return false;
        }
    }

    if (str_Source.length() >= 14)
    {
        if ((ECLASS_PRESENT_TENSE == e_Class) || (ECLASS_PAST_TENSE ==  e_Class))
        {
            i_Person = _wtoi (&str_Source[13]);
            if ((1 > i_Person) && (3 < i_Person))
            {
                ATLASSERT(0);
                wstring str_msg (L"Unexpected digit in person field: ");
                str_msg += str_Source;
                ERROR_LOG (str_msg);
                return false;
            }
        }
        else
        {
            wchar_t chr_case = str_Source[13];
            switch (chr_case)
            {
                case L'N':
                {
                    e_Case = CASE_NOM;
                    break;
                }
                case L'G':
                {
                    e_Case = CASE_GEN;
                    break;
                }
                case L'D':
                {
                    e_Case = CASE_DAT;
                    break;
                }
                case L'A':
                {
                    e_Case = CASE_ACC;
                    break;
                }
                case L'I':
                {
                    e_Case = CASE_INST;
                    break;
                }
                case L'L':
                {
                    e_Case = CASE_LOC;
                    break;
                }
                default:
                {
                    e_Case = CASE_UNDEFINED;
                    if (L' ' != chr_case)
                    {
                        ATLASSERT(0);
                        wstring str_msg (L"Unexpected character in case field: ");
                        str_msg += str_Source;
                        ERROR_LOG (str_msg);
                        return false;
                    }
                }
            }   // switch ...
        }
    }   // if (str_Source.length() >= 14)

    if (str_Source.length() >= 15)
    {
        wchar_t chr_ = str_Source[14];
        switch (chr_)
        {
            case L'°':
            {
                b_StemAugment = true;
                break;
            }
            case L'1':
            {
                i_CommonDeviation = 1;
                break;
            }
            case L'2':
            {
                i_CommonDeviation = 2;
                break;
            }
            default:
            {
                if (L' ' != chr_)
                {
                    ATLASSERT(0);
                    wstring str_msg (L"Unexpected character in circle field: ");
                    str_msg += str_Source;
                    ERROR_LOG (str_msg);
                    return false;
                }
            }
        }   // switch ...
    }

    bool b_ = b_SaveToDb();

    return true;

}   // b_Parse()

bool CT_EndingsParser::b_SaveToDb()
{
    try
    {
        pco_Db->v_PrepareForInsert (L"endings", 11);
        pco_Db->v_Bind (1, xstr_Ending);  // 0-based
        pco_Db->v_Bind (2, e_Class);
        pco_Db->v_Bind (3, i_InflectionType);
        pco_Db->v_Bind (4, e_Case);
        pco_Db->v_Bind (5, i_Person);
        pco_Db->v_Bind (6, e_Gender);
        pco_Db->v_Bind (7, e_Animacy);
        pco_Db->v_Bind (8, e_Stress);
        pco_Db->v_Bind (9, b_Plural);
        pco_Db->v_Bind (10, b_StemAugment);
        pco_Db->v_Bind (11, i_CommonDeviation);

        pco_Db->v_InsertRow();
        pco_Db->v_Finalize();        
    }
    catch (...)
    {
        wstring str_msg;
        wstring str_error;
        try
        {
            pco_Db->v_GetLastError (str_error);
            str_msg += wstring (L", error %d: ");
            str_msg += str_error;
        }
        catch (...)
        {
            str_msg = L"Apparent DB error ";
        }
    
        CString cs_msg;
        cs_msg.Format (str_msg.c_str(), pco_Db->i_GetLastError());
        ERROR_LOG ((LPCTSTR)cs_msg);

        return false;
    }

     return true;

}   //  b_SaveToDb()
