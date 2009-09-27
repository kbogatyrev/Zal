#include "StdAfx.h"
#include "WordForm.h"

HRESULT CT_WordForm::get_Wordform (BSTR * pbstr_wordForm)
{
    if (str_WordForm.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_WordForm.c_str());
    *pbstr_wordForm = sp_bstr.Detach();

    return S_OK;
}

HRESULT CT_WordForm::get_Lemma (BSTR * pbstr_lemma)
{
    if (str_Lemma.empty())
    {
        return S_FALSE;
    }
    CComBSTR sp_bstr (str_Lemma.c_str());
    *pbstr_lemma = sp_bstr.Detach();

    return S_OK;
}

HRESULT CT_WordForm::get_LexemeId (LONG *)
{
    return E_NOTIMPL;
}

HRESULT CT_WordForm::get_PartOfSpeech (ET_PartOfSpeech * pe_partOfSpeech)
{
    *pe_partOfSpeech = (ET_PartOfSpeech)eo_POS;
    return S_OK;
}

HRESULT CT_WordForm::get_Case (ET_Case * pe_case)
{
    *pe_case = (ET_Case)eo_Case;
    return S_OK;
}

HRESULT CT_WordForm::get_Number (ET_Number * pe_number)
{
    *pe_number = (ET_Number)eo_Number;
    return S_OK;
}

HRESULT CT_WordForm::get_IsShortForm (BOOL * pb_shortForm)
{
    *pb_shortForm = b_ShortForm ? TRUE : FALSE;
    return S_OK;
}

HRESULT CT_WordForm::get_Gender (ET_Gender * pe_gender)
{
    *pe_gender = (ET_Gender)eo_Gender;
    return S_OK;
}

HRESULT CT_WordForm::get_Tense (ET_Tense * pe_tense)
{
    *pe_tense = (ET_Tense)eo_Tense;
    return S_OK;
}

HRESULT CT_WordForm::get_Person (ET_Person * pe_person)
{
    *pe_person = (ET_Person)eo_Person;
    return S_OK;
}

HRESULT CT_WordForm::get_IsReflexive (BOOL * pb_reflexive)
{
    *pb_reflexive = b_Reflexive ? TRUE : FALSE;
    return S_OK;
}

HRESULT CT_WordForm::get_Voice (ET_Voice * pe_voice)
{
    *pe_voice = (ET_Voice)eo_Voice;
    return S_OK;
}

HRESULT CT_WordForm::get_SpecialForm (ET_SpecialForm * pe_specialForm)
{
    *pe_specialForm = (ET_SpecialForm)eo_Special;
    return S_OK;
}

//
//
//
int CT_WordForm::i_GramNumber()
{
    if (eo_POS == POS_UNDEFINED || eo_POS == POS_ADV || eo_POS == POS_PREP ||
        eo_POS == POS_CONJ || eo_POS == POS_PARTICLE || eo_POS == POS_COMPAR ||
        eo_POS == POS_INTERJ || eo_POS == POS_PARENTH)
    {
        return 0;
    }
    if (eo_POS == POS_NOUN) // 40 possibilities
    {
        return eo_Case * 3 + eo_Number;
    }
    if (eo_POS == POS_ADJ || eo_POS == POS_NUM || eo_POS == POS_PRONOUN) // 264 possibilities
    {
        if (eo_Case != CASE_ACC || eo_Animacy != ANIM_YES)
        {
            return eo_AdjForm * 132 + eo_Case * 12 + eo_Number * 4 + eo_Gender;
        }
        else
        {
            return eo_AdjForm * 132 + 10 * 12 + eo_Number * 4 + eo_Gender;
        }
    }
    if (eo_POS == POS_VERB)
    {
        if (eo_Special == SPECIAL_UNDEFINED)
        {
            return -1;
        }
        if (eo_Special == SPECIAL_NORMAL) // 144 possibilities
        {
            if (eo_Tense != T_PAST)
            {
                return eo_Tense * 36 + eo_Number * 12 + eo_Person * 3 + eo_Reflexive;
            }
            else
            {
                return 3 * 36 + eo_Number * 12 + eo_Gender * 3 + eo_Reflexive;
            }
        }
        if (eo_Special == SPECIAL_IMPERATIVE) // 36 possibilities
        {
            return eo_Number * 12 + eo_Person * 3 + eo_Reflexive + 144;
        }
        if (eo_Special == SPECIAL_PARTICIPLE) // 4752 possibilities
        {
            if (eo_Case != CASE_ACC || eo_Animacy != ANIM_YES)
            {
                return eo_Tense * 1188 + 
                         eo_Case * 108 + 
                         eo_Number * 36 + 
                         eo_Gender * 9 + 
                         eo_Voice * 3 + 
                         eo_Reflexive + 
                         180;
            }
            else
            {
                return eo_Tense * 1188 + 
                         10 * 108 + 
                         eo_Number * 36 + 
                         eo_Gender * 9 + 
                         eo_Voice * 3 + 
                         eo_Reflexive + 180;
            }
        }
        if (eo_Special == SPECIAL_CONVERB) // 12 possibilities
        {
            return eo_Tense * 3 + eo_Reflexive + 4932;
        }
        if (eo_Special == SPECIAL_INFINITIVE) // 2 possibilities
        {
            return eo_Reflexive + 4944;
        }
    }
    if (eo_Special == SPECIAL_COMPARATIVE) // 1 possibility
    {
        return 4946;
    }

    return -1;

}   //  i_GramNumber()

int CT_WordForm::i_DecodeNumber (int i_gram)
{
    if (i_gram < 0 || i_gram > 4946)
    {
        return -1;
    }
    GramClear();
    if (eo_POS == POS_UNDEFINED || eo_POS == POS_ADV || eo_POS == POS_PREP ||
        eo_POS == POS_CONJ || eo_POS == POS_PARTICLE || eo_POS == POS_COMPAR ||
        eo_POS == POS_INTERJ || eo_POS == POS_PARENTH)
    {
        return 0;
    }
    if (eo_POS == POS_NOUN) // 40 possibilities
    {
        eo_Number = (ET_Number)(i_gram % 3);
        eo_Case = (ET_Case)((i_gram - eo_Number) / 3);
        return 0;
    }
    if (eo_POS == POS_ADJ || eo_POS == POS_NUM || eo_POS == POS_PRONOUN) // 264 possibilities
    {
        eo_Gender = (ET_Gender)(i_gram % 4);
        i_gram = (i_gram - eo_Gender) / 4;
        eo_Number = (ET_Number)(i_gram % 3);
        i_gram = (i_gram - eo_Number) / 3;
        int i_tmp_case = i_gram % 11;
        if (i_tmp_case == 10)
        {
            eo_Case = CASE_ACC;
            eo_Animacy = ANIM_YES;
        }
        else
        {
            eo_Case = (ET_Case)i_tmp_case;
            if (eo_Case != CASE_UNDEFINED)
            {
                eo_Animacy = ANIM_NO;
            }
        }
        i_gram = (i_gram - i_tmp_case) / 11;
        eo_AdjForm = (ET_AdjForm)i_gram;
    }
    if (eo_POS == POS_VERB)
    {
        if (i_gram < 144) // 144 possibilities
        {
            eo_Special = SPECIAL_NORMAL;
            eo_Reflexive = (ET_Reflexive)(i_gram % 3);
            i_gram = (i_gram - eo_Reflexive) / 3;
            if (i_gram < 108)
            {
                eo_Person = (ET_Person)(i_gram % 4);
                i_gram = (i_gram - eo_Person) / 4;
                eo_Number = (ET_Number)(i_gram % 3);
                i_gram = (i_gram - eo_Number) / 3;
                eo_Tense = (ET_Tense)i_gram;
                return 0;
            }
            else
            {
                eo_Gender = (ET_Gender)(i_gram % 4);
                i_gram = (i_gram - eo_Gender) / 4;
                eo_Number = (ET_Number)(i_gram % 3);
                eo_Tense = (ET_Tense)3;
                return 0;
            }
        }
        if (i_gram >= 144 && i_gram < 180) // 36 possibilities
        {
            i_gram -=144;
            eo_Special = SPECIAL_IMPERATIVE;
            eo_Reflexive = (ET_Reflexive)(i_gram % 3);
            i_gram = (i_gram - eo_Reflexive) / 3;
            eo_Person = (ET_Person)(i_gram % 4);
            i_gram = (i_gram - eo_Person) / 4;
            eo_Number = (ET_Number)i_gram;
            return 0;
        }
        if (eo_Special == SPECIAL_PARTICIPLE) // 4752 possibilities
        {
            i_gram -= 180;
            eo_Special = SPECIAL_PARTICIPLE;
            eo_Reflexive = (ET_Reflexive)(i_gram % 3);
            i_gram = (i_gram - eo_Reflexive) / 3;
            eo_Voice = (ET_Voice)(i_gram % 3);
            i_gram = (i_gram - eo_Voice) / 3;
            eo_Gender = (ET_Gender)(i_gram % 4);
            i_gram = (i_gram - eo_Gender) / 4;
            eo_Number = (ET_Number)(i_gram % 3);
            i_gram = (i_gram - eo_Number) / 3;
            int i_tmp_case = i_gram % 11;
            if (i_tmp_case == 10)
            {
                eo_Case = CASE_ACC;
                eo_Animacy = ANIM_YES;
            }
            else
            {
                eo_Case = (ET_Case)i_tmp_case;
                if (eo_Case != CASE_UNDEFINED)
                {
                    eo_Animacy = ANIM_NO;
                }
            }
            i_gram = (i_gram - i_tmp_case) / 11;
            eo_Tense = (ET_Tense)i_gram;
            return 0;
        }
        if (i_gram >= 4932 && i_gram < 4944) // 12 possibilities
        {
            i_gram -= 4932;
            eo_Special = SPECIAL_CONVERB;
            eo_Reflexive = (ET_Reflexive)(i_gram % 3);
            i_gram = (i_gram - eo_Reflexive) / 3;
            eo_Tense = (ET_Tense)i_gram;
            return 0;
        }
        if (i_gram >= 4944 && i_gram < 4946) // 2 possibilities
        {
            i_gram -= 4944;
            eo_Special = SPECIAL_INFINITIVE;
            eo_Reflexive = (ET_Reflexive)i_gram;
            return 0;
        }
    }
    if (i_gram == 4946)
    {
        eo_Special = SPECIAL_COMPARATIVE;
        return 0;
    }

    return -1;

}   //  int i_DecodeNumber (int i_gram)

int CT_WordForm::i_DecodeString (wstring str_gram)
{
    // Partially depends on POS.

    // Warning!
    // 1. The list may be incomplete.
    // 2. Verb reflexivity is recognized only if str_lemma is present.

    GramClear();
    if (str_gram.length() == 0)
    {
        return 0;
    }
    wsmatch result;
    wstring str_parameter;
    bool b_match;
    while (str_gram.length() > 0)
    {
        b_match = regex_match(str_gram, result, (const wregex)L"\\s*([^\\s\\.]*\\.?)\\s*(.*)");
        str_parameter = result[1];
        str_gram = result[2];
        if (str_parameter == L"1")
        {
            eo_Person = P_1;
        }
        if (str_parameter == L"2")
        {
            eo_Person = P_2;
        }
        if (str_parameter == L"3")
        {
            eo_Person = P_3;
        }
        if (str_parameter == L"ед.")
        {
            eo_Number = NUM_SG;
        }
        if (str_parameter == L"мн.")
        {
            eo_Number = NUM_PL;
        }
        if (str_parameter == L"И.")
        {
            eo_Case = CASE_NOM;
        }
        if (str_parameter == L"Р.")
        {
            eo_Case = CASE_GEN;
        }
        if (str_parameter == L"Д.")
        {
            eo_Case = CASE_DAT;
        }
        if (str_parameter == L"В.")
        {
            eo_Case = CASE_ACC;
        }
        if (str_parameter == L"Т.")
        {
            eo_Case = CASE_INST;
        }
        if (str_parameter == L"П.")
        {
            eo_Case = CASE_PREP;
        }
        if (str_parameter == L"Р2")
        {
            eo_Case = CASE_PART;
        }
        if (str_parameter == L"П2")
        {
            eo_Case = CASE_LOC;
        }
        if (str_parameter == L"кф")
        {
            eo_AdjForm = FORM_SHORT;
        }
        if (str_parameter == L"буд.")
        {
            eo_Tense = T_FUTURE;
        }
        if (str_parameter == L"наст.")
        {
            eo_Tense = T_PRESENT;
        }
        if (str_parameter == L"прош.")
        {
            eo_Tense = T_PAST;
        }
        if (str_parameter == L"прич.")
        {
            eo_Special = SPECIAL_PARTICIPLE;
        }
        if (str_parameter == L"деепр.")
        {
            eo_Special = SPECIAL_CONVERB;
        }
        if (str_parameter == L"повел.")
        {
            eo_Special = SPECIAL_IMPERATIVE;
        }
        if (str_parameter == L"страд.")
        {
            eo_Voice = VOICE_PASSIVE;
        }
        if (str_parameter == L"м")
        {
            eo_Gender = G_M;
        }
        if (str_parameter == L"ж")
        {
            eo_Gender = G_F;
        }
        if (str_parameter == L"с")
        {
            eo_Gender = G_N;
        }
        if (str_parameter == L"одуш.")
        {
            eo_Animacy = ANIM_YES;
        }
        if (str_parameter == L"сравн.")
        {
            eo_Special = SPECIAL_COMPARATIVE;
        }
    }
    if (eo_POS == POS_ADV)
    {
        if (eo_AdjForm == FORM_UNDEFINED)
        {
            eo_AdjForm = FORM_LONG;
        }
        if (eo_Case == CASE_ACC && eo_Animacy == ANIM_UNDEFINED)
        {
            eo_Animacy = ANIM_NO;
        }
    }
    if (eo_POS == POS_VERB)
    {
        if (eo_Special == SPECIAL_UNDEFINED)
        {
            eo_Special = SPECIAL_NORMAL;
        }
        if (eo_Special == SPECIAL_IMPERATIVE && eo_Person == P_UNDEFINED)
        {
            eo_Person = P_2;
        }
        if (eo_Special == SPECIAL_PARTICIPLE && eo_Case == CASE_ACC && eo_Animacy == ANIM_UNDEFINED)
        {
            eo_Animacy = ANIM_NO;
        }
        if (eo_Special == SPECIAL_PARTICIPLE && eo_Voice == VOICE_UNDEFINED)
        {
            eo_Voice = VOICE_ACTIVE;
        }
        b_match = regex_match (str_Lemma, result, (const wregex)L".*(ся|сь)");
        if (b_match == true)
        {
            eo_Reflexive = REFL_YES;
        }
        else
        {
            eo_Reflexive = REFL_NO;
        }
    }

    return 0;

}   //  i_DecodeString (wstring str_gram)
