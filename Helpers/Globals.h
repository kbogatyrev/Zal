#pragma once

#include "StdAfx.h"
#include <Regex>

using namespace std;
using namespace std::tr1;

static wstring str_int2wstring (int i_input)
{
    wstringstream ss_tmp;
    ss_tmp << i_input;
    return ss_tmp.str();
}

static wstring str_CyrillicLowercase = L"-абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
static wstring str_Vowels = L"аеёиоуыэюя";

enum ET_PartOfSpeech 
{
    POS_UNDEFINED,
    POS_NOUN, 
    POS_VERB, 
    POS_ADJ, 
    POS_PRONOUN,
    POS_NUM,
    POS_ADV, 
    POS_COMPAR, 
    POS_PREDIC, 
    POS_PREP, 
    POS_CONJ, 
    POS_PARTICLE, 
    POS_INTERJ, 
    POS_PARENTH,     // ?
    POS_COUNT
};

static void operator++ (ET_PartOfSpeech& eo_pos) 
{
    eo_pos = (ET_PartOfSpeech)(eo_pos + 1);
}

enum ET_Case 
{
    CASE_UNDEFINED,
    CASE_NOM,
    CASE_ACC,
    CASE_GEN,
    CASE_PART,
    CASE_DAT,
    CASE_INST,
    CASE_PREP,
    CASE_LOC,
    CASE_NUM, // Счётная форма
    CASE_COUNT
};

enum ET_Number 
{
    NUM_UNDEFINED,
    NUM_SG,
    NUM_PL,
    NUM_COUNT
};

enum ET_AdjForm
{
    ADJ_FORM_UNDEFINED,
    ADJ_FORM_SHORT,
    ADJ_FORM_LONG,
    ADJ_COUNT_COUNT
};

enum ET_Gender 
{
    GENDER_UNDEFINED,
    GENDER_M,
    GENDER_F,
    GENDER_N,
    GENDER_COUNT
};

enum ET_Tense 
{
    TENSE_UNDEFINED,
    TENSE_FUTURE,
    TENSE_PRESENT,
    TENSE_PAST,
    TENSE_COUNT
};

enum ET_Person 
{
    PERSON_UNDEFINED,
    PERSON_1,
    PERSON_2,
    PERSON_3,
    PERSON_COUNT
};

enum ET_Animacy // for adjectives
{
    ANIM_UNDEFINED,
    ANIM_YES,
    ANIM_NO,
    ANIM_COUNT
};

enum ET_Voice
{
    VOICE_UNDEFINED,
    VOICE_ACTIVE,
    VOICE_PASSIVE,
    VOICE_COUNT
};

enum ET_Reflexive // -ся
{
    REFL_UNDEFINED,
    REFL_YES,
    REFL_NO,
    REFL_COUNT
};

enum ET_CommonDeviation
{
    COMMONDEV_NONE,
    COMMONDEV_1,
    COMMONDEV_2,
    COMMONDEV_BOTH,
    COMMONDEV_COUNT
};

enum ET_SpecialForm 
{
    SPECIAL_UNDEFINED,
    SPECIAL_NORMAL,
    SPECIAL_COMPARATIVE, // Should be avoided
    SPECIAL_INFINITIVE,
    SPECIAL_IMPERATIVE,
    SPECIAL_PARTICIPLE,
    SPECIAL_CONVERB,
    SPECIAL_COUNT
};

enum ET_AccentType
{
    AT_UNDEFINED,
    AT_A,
    AT_A1,
    AT_B,
    AT_B1,
    AT_C,
    AT_C1,
    AT_C2,
    AT_D,
    AT_D1,
    AT_E,
    AT_F,
    AT_F1,
    AT_F2,
    AT_COUNT
};
static void operator++ (ET_AccentType& eo_at) 
{
    eo_at = (ET_AccentType)(eo_at + 1);
}

class CT_Wordform // For ConversionLib only!
{
public:
    wstring str_wordform;
    wstring str_lemma;
    __int64 ll_lexeme_id;
    ET_PartOfSpeech eo_POS;
    ET_Case eo_Case;
    ET_Number eo_Number;
    ET_AdjForm eo_AdjForm;
    ET_Gender eo_Gender;
    ET_Tense eo_Tense;
    ET_Person eo_Person;
    ET_Animacy eo_Animacy;
    ET_Reflexive eo_Reflexive;
    ET_Voice eo_Voice;
    ET_SpecialForm eo_Special;

    CT_Wordform()
    {
        eo_POS = POS_UNDEFINED;
        eo_Case = CASE_UNDEFINED;
        eo_Number = NUM_UNDEFINED;
        eo_AdjForm = ADJ_FORM_UNDEFINED;
        eo_Gender = GENDER_UNDEFINED;
        eo_Tense = TENSE_UNDEFINED;
        eo_Person = PERSON_UNDEFINED;
        eo_Reflexive = REFL_UNDEFINED;
        eo_Voice = VOICE_UNDEFINED;
        eo_Animacy = ANIM_UNDEFINED;
        eo_Special = SPECIAL_UNDEFINED;
    }

    CT_Wordform(wstring str_wf)
    {
        eo_POS = POS_UNDEFINED;
        eo_Case = CASE_UNDEFINED;
        eo_Number = NUM_UNDEFINED;
        eo_AdjForm = ADJ_FORM_UNDEFINED;
        eo_Gender = GENDER_UNDEFINED;
        eo_Tense = TENSE_UNDEFINED;
        eo_Person = PERSON_UNDEFINED;
        eo_Reflexive = REFL_UNDEFINED;
        eo_Voice = VOICE_UNDEFINED;
        eo_Animacy = ANIM_UNDEFINED;
        eo_Special = SPECIAL_UNDEFINED;
        str_wordform = str_wf;
    }

    CT_Wordform(wstring str_wf, __int64 ll_id)
    {
        eo_POS = POS_UNDEFINED;
        eo_Case = CASE_UNDEFINED;
        eo_Number = NUM_UNDEFINED;
        eo_AdjForm = ADJ_FORM_UNDEFINED;
        eo_Gender = GENDER_UNDEFINED;
        eo_Tense = TENSE_UNDEFINED;
        eo_Person = PERSON_UNDEFINED;
        eo_Reflexive = REFL_UNDEFINED;
        eo_Voice = VOICE_UNDEFINED;
        eo_Animacy = ANIM_UNDEFINED;
        eo_Special = SPECIAL_UNDEFINED;
        str_wordform = str_wf;
        ll_lexeme_id = ll_id;
    }

    ~CT_Wordform() {}

    void GramClear()
    {
        eo_Case = CASE_UNDEFINED;
        eo_Number = NUM_UNDEFINED;
        eo_AdjForm = ADJ_FORM_UNDEFINED;
        eo_Gender = GENDER_UNDEFINED;
        eo_Tense = TENSE_UNDEFINED;
        eo_Person = PERSON_UNDEFINED;
        eo_Reflexive = REFL_UNDEFINED;
        eo_Voice = VOICE_UNDEFINED;
        eo_Animacy = ANIM_UNDEFINED;
        eo_Special = SPECIAL_UNDEFINED;   
        return;
    }

    void Clear()
    {
        GramClear();
        eo_POS = POS_UNDEFINED;
        str_wordform = L"";
        str_lemma = L"";
        ll_lexeme_id = -1;   
        return;
    }

    int i_GramNumber()
    {
        if (eo_POS == POS_UNDEFINED || eo_POS == POS_ADV || eo_POS == POS_PREP ||
            eo_POS == POS_CONJ || eo_POS == POS_PARTICLE || eo_POS == POS_COMPAR ||
            eo_POS == POS_INTERJ || eo_POS == POS_PARENTH)
        {
            return 0;
        }
        int i_Case;
        if (eo_POS == POS_NOUN) // 40 possibilities
        {
            return eo_Case * NUM_COUNT + eo_Number;
        }
        if (eo_POS == POS_ADJ || eo_POS == POS_NUM || eo_POS == POS_PRONOUN) // 264 possibilities
        {
            if (eo_Case != CASE_ACC || eo_Animacy != ANIM_YES)
            {
                i_Case = eo_Case;
            }
            else
            {
                i_Case = CASE_COUNT; // In fact, it means Accusative case of an animate adjective
            }
            return eo_AdjForm * (CASE_COUNT + 1) * NUM_COUNT * GENDER_COUNT
                    + i_Case * NUM_COUNT * GENDER_COUNT + eo_Number * GENDER_COUNT + eo_Gender;
        }
        if (eo_POS == POS_VERB)
        {
            if (eo_Special == SPECIAL_UNDEFINED)
            {
                return -1;
            }
            if (eo_Special == SPECIAL_NORMAL) // 144 possibilities
            {
                // It is assumed that GENDER_COUNT == PERSON_COUNT
                if (eo_Tense != TENSE_PAST)
                {
                    return eo_Tense * NUM_COUNT * PERSON_COUNT * REFL_COUNT
                            + eo_Number * PERSON_COUNT * REFL_COUNT + eo_Person * REFL_COUNT + eo_Reflexive;
                }
                else
                {
                    return TENSE_PAST * NUM_COUNT * GENDER_COUNT * REFL_COUNT 
                            + eo_Number * GENDER_COUNT * REFL_COUNT + eo_Gender * REFL_COUNT + eo_Reflexive;
                }
            }
            if (eo_Special == SPECIAL_IMPERATIVE) // 36 possibilities
            {
                return eo_Number * PERSON_COUNT * REFL_COUNT + eo_Person * REFL_COUNT + eo_Reflexive
                        + TENSE_COUNT * NUM_COUNT * GENDER_COUNT * REFL_COUNT;
            }
            if (eo_Special == SPECIAL_PARTICIPLE) // 4752 possibilities
            {
                if (eo_Case != CASE_ACC || eo_Animacy != ANIM_YES)
                {
                    i_Case = eo_Case;
                }
                else           
                {
                    i_Case = CASE_COUNT; // In fact, it means Accusative case of an animate participle
                }
                return eo_Tense * (CASE_COUNT + 1) * NUM_COUNT * GENDER_COUNT * VOICE_COUNT * REFL_COUNT + 
                        i_Case * NUM_COUNT * GENDER_COUNT * VOICE_COUNT * REFL_COUNT + 
                        eo_Number * GENDER_COUNT * VOICE_COUNT * REFL_COUNT + 
                        eo_Gender * VOICE_COUNT * REFL_COUNT + 
                        eo_Voice * REFL_COUNT + 
                        eo_Reflexive + 
                        TENSE_COUNT * NUM_COUNT * GENDER_COUNT * REFL_COUNT + NUM_COUNT * PERSON_COUNT * REFL_COUNT;
            }
            if (eo_Special == SPECIAL_CONVERB) // 12 possibilities
            {
                return eo_Tense * REFL_COUNT + eo_Reflexive 
                        + TENSE_COUNT * (CASE_COUNT + 1) * NUM_COUNT * GENDER_COUNT * VOICE_COUNT * REFL_COUNT
                        + TENSE_COUNT * NUM_COUNT * GENDER_COUNT * REFL_COUNT
                        + NUM_COUNT * PERSON_COUNT * REFL_COUNT;
            }
            if (eo_Special == SPECIAL_INFINITIVE) // 2 possibilities
            {
                return eo_Reflexive + TENSE_COUNT * REFL_COUNT 
                        + TENSE_COUNT * (CASE_COUNT + 1) * NUM_COUNT * GENDER_COUNT * VOICE_COUNT * REFL_COUNT
                        + TENSE_COUNT * NUM_COUNT * GENDER_COUNT * REFL_COUNT
                        + NUM_COUNT * PERSON_COUNT * REFL_COUNT;
            }
        }
        if (eo_Special == SPECIAL_COMPARATIVE) // 1 possibility
        {
            return REFL_COUNT + TENSE_COUNT * REFL_COUNT 
                    + TENSE_COUNT * (CASE_COUNT + 1) * NUM_COUNT * GENDER_COUNT * VOICE_COUNT * REFL_COUNT
                    + TENSE_COUNT * NUM_COUNT * GENDER_COUNT * REFL_COUNT
                    + NUM_COUNT * PERSON_COUNT * REFL_COUNT;
            //In original version, this used to give 4947;
        }
        return -1;   
    }   //  i_GramNumber()
       
    int i_DecodeNumber (int i_gram)
    {
        int i_gram_max = REFL_COUNT + TENSE_COUNT * REFL_COUNT 
                    + TENSE_COUNT * (CASE_COUNT + 1) * NUM_COUNT * GENDER_COUNT * VOICE_COUNT * REFL_COUNT
                    + TENSE_COUNT * NUM_COUNT * GENDER_COUNT * REFL_COUNT
                    + NUM_COUNT * PERSON_COUNT * REFL_COUNT;
        if (i_gram < 0 || i_gram > i_gram_max)
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
            eo_Number = (ET_Number)(i_gram % NUM_COUNT);
            eo_Case = (ET_Case)((i_gram - eo_Number) / NUM_COUNT);
            return 0;
        }
        if (eo_POS == POS_ADJ || eo_POS == POS_NUM || eo_POS == POS_PRONOUN) // 264 possibilities
        {
            eo_Gender = (ET_Gender)(i_gram % GENDER_COUNT);
            i_gram = (i_gram - eo_Gender) / GENDER_COUNT;
            eo_Number = (ET_Number)(i_gram % NUM_COUNT);
            i_gram = (i_gram - eo_Number) / NUM_COUNT;
            int i_tmp_case = i_gram % (CASE_COUNT + 1);
            if (i_tmp_case == CASE_COUNT)
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
            i_gram = (i_gram - i_tmp_case) / (CASE_COUNT + 1);
            eo_AdjForm = (ET_AdjForm)i_gram;   
        }
        if (eo_POS == POS_VERB)
        {
            if (i_gram < TENSE_COUNT * NUM_COUNT * GENDER_COUNT * REFL_COUNT) // 144 possibilities
            {
                eo_Special = SPECIAL_NORMAL;
                eo_Reflexive = (ET_Reflexive)(i_gram % REFL_COUNT);
                i_gram = (i_gram - eo_Reflexive) / REFL_COUNT;
                if (i_gram < TENSE_PAST * NUM_COUNT * GENDER_COUNT * REFL_COUNT)
                {
                    eo_Person = (ET_Person)(i_gram % PERSON_COUNT);
                    i_gram = (i_gram - eo_Person) / PERSON_COUNT;
                    eo_Number = (ET_Number)(i_gram % NUM_COUNT);
                    i_gram = (i_gram - eo_Number) / NUM_COUNT;
                    eo_Tense = (ET_Tense)i_gram;
                    return 0;
                }           
                else
                {
                    eo_Gender = (ET_Gender)(i_gram % GENDER_COUNT);
                    i_gram = (i_gram - eo_Gender) / GENDER_COUNT;
                    eo_Number = (ET_Number)(i_gram % NUM_COUNT);
                    eo_Tense = TENSE_PAST;
                    return 0;
                }
            }
           
            if (i_gram >= TENSE_COUNT * NUM_COUNT * GENDER_COUNT * REFL_COUNT
                && i_gram < TENSE_COUNT * NUM_COUNT * GENDER_COUNT * REFL_COUNT
                            + NUM_COUNT * PERSON_COUNT * REFL_COUNT) // 36 possibilities
            {
                i_gram -= TENSE_COUNT * NUM_COUNT * GENDER_COUNT * REFL_COUNT;
                eo_Special = SPECIAL_IMPERATIVE;
                eo_Reflexive = (ET_Reflexive)(i_gram % REFL_COUNT);
                i_gram = (i_gram - eo_Reflexive) / REFL_COUNT;
                eo_Person = (ET_Person)(i_gram % PERSON_COUNT);
                i_gram = (i_gram - eo_Person) / PERSON_COUNT;
                eo_Number = (ET_Number)i_gram;
                return 0;
            }
            if (i_gram >= TENSE_COUNT * NUM_COUNT * GENDER_COUNT * REFL_COUNT
                        + NUM_COUNT * PERSON_COUNT * REFL_COUNT
                    && i_gram < TENSE_COUNT * (CASE_COUNT + 1) * NUM_COUNT * GENDER_COUNT * VOICE_COUNT * REFL_COUNT
                        + TENSE_COUNT * NUM_COUNT * GENDER_COUNT * REFL_COUNT
                        + NUM_COUNT * PERSON_COUNT * REFL_COUNT) // 4752 possibilities
            {
                i_gram -= TENSE_COUNT * NUM_COUNT * GENDER_COUNT * REFL_COUNT
                            + NUM_COUNT * PERSON_COUNT * REFL_COUNT;
                eo_Special = SPECIAL_PARTICIPLE;
                eo_Reflexive = (ET_Reflexive)(i_gram % REFL_COUNT);
                i_gram = (i_gram - eo_Reflexive) / REFL_COUNT;
                eo_Voice = (ET_Voice)(i_gram % VOICE_COUNT);
                i_gram = (i_gram - eo_Voice) / VOICE_COUNT;
                eo_Gender = (ET_Gender)(i_gram % GENDER_COUNT);
                i_gram = (i_gram - eo_Gender) / GENDER_COUNT;
                eo_Number = (ET_Number)(i_gram % NUM_COUNT);
                i_gram = (i_gram - eo_Number) / NUM_COUNT;
                int i_tmp_case = i_gram % (CASE_COUNT + 1);
                if (i_tmp_case == CASE_COUNT)
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
                i_gram = (i_gram - i_tmp_case) / (CASE_COUNT + 1);
                eo_Tense = (ET_Tense)i_gram;
                return 0;
            }
            if (i_gram >= TENSE_COUNT * (CASE_COUNT + 1) * NUM_COUNT * GENDER_COUNT * VOICE_COUNT * REFL_COUNT
                        + TENSE_COUNT * NUM_COUNT * GENDER_COUNT * REFL_COUNT
                        + NUM_COUNT * PERSON_COUNT * REFL_COUNT
                    && i_gram < i_gram_max - REFL_COUNT) // 12 possibilities
            {
                i_gram -= TENSE_COUNT * (CASE_COUNT + 1) * NUM_COUNT * GENDER_COUNT * VOICE_COUNT * REFL_COUNT
                        + TENSE_COUNT * NUM_COUNT * GENDER_COUNT * REFL_COUNT
                        + NUM_COUNT * PERSON_COUNT * REFL_COUNT;
                eo_Special = SPECIAL_CONVERB;
                eo_Reflexive = (ET_Reflexive)(i_gram % REFL_COUNT);
                i_gram = (i_gram - eo_Reflexive) / REFL_COUNT;
                eo_Tense = (ET_Tense)i_gram;
                return 0;
            }
            if (i_gram >= i_gram_max - REFL_COUNT && i_gram < i_gram_max) // 2 possibilities
            {
                i_gram -= i_gram_max - REFL_COUNT;
                eo_Special = SPECIAL_INFINITIVE;
                eo_Reflexive = (ET_Reflexive)i_gram;
                return 0;
            }
        }
        if (i_gram == i_gram_max)
        {
            eo_Special = SPECIAL_COMPARATIVE;
            return 0;
        }
        
        return -1;
    }   //  int i_DecodeNumber (int i_gram)
    
    int i_DecodeString (wstring str_gram)
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
                eo_Person = PERSON_1;
            }
            if (str_parameter == L"2")
            {
                eo_Person = PERSON_2;
            }
            if (str_parameter == L"3")
            {
                eo_Person = PERSON_3;
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
                eo_AdjForm = ADJ_FORM_SHORT;
            }
            if (str_parameter == L"буд.")
            {
                eo_Tense = TENSE_FUTURE;
            }
            if (str_parameter == L"наст.")
            {
                eo_Tense = TENSE_PRESENT;
            }
            if (str_parameter == L"прош.")
            {
                eo_Tense = TENSE_PAST;
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
                eo_Gender = GENDER_M;
            }
            if (str_parameter == L"ж")
            {
                eo_Gender = GENDER_F;
            }
            if (str_parameter == L"с")
            {
                eo_Gender = GENDER_N;
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
            if (eo_AdjForm == ADJ_FORM_UNDEFINED)
            {
                eo_AdjForm = ADJ_FORM_LONG;
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
            if (eo_Special == SPECIAL_IMPERATIVE && eo_Person == PERSON_UNDEFINED)
            {
                eo_Person = PERSON_2;
            }
            if (eo_Special == SPECIAL_PARTICIPLE && eo_Case == CASE_ACC && eo_Animacy == ANIM_UNDEFINED)
            {
                eo_Animacy = ANIM_NO;
            }
            if (eo_Special == SPECIAL_PARTICIPLE && eo_Voice == VOICE_UNDEFINED)
            {
                eo_Voice = VOICE_ACTIVE;
            }
            b_match = regex_match(str_lemma, result, (const wregex)L".*(ся|сь)");
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
    }
};