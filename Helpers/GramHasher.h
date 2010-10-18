#pragma once

#include "stdafx.h"

class CT_GramHasher
{
    static const long MIN_NOUN = 0;
    static const long MIN_ADJ = 1000;
    static const long MIN_VERB = 50000;
    static const long MIN_UNCHANGEABLE = 100000;

    static enum et_Subtype
    {
        ec_SubtypeUndefined,
        ec_Participle,
        ec_Adverbial
    };

    static enum et_Tense
    {
        ec_TenseUndefined,
        ec_TensePresent,
        ec_TensePast,
        ec_TenseFuture
    };


public:
    string str_Lemma; // For reflexivity -- this is currently disabled

    ET_PartOfSpeech eo_POS;
    ET_Subparadigm eo_Subparadigm;
    ET_Case eo_Case;
    ET_Number eo_Number;
    ET_Gender eo_Gender;
    ET_Person eo_Person;
    ET_Animacy eo_Animacy;
    ET_Reflexive eo_Reflexive;
//    ET_Aspect eo_Aspect;

public:
    CT_GramHasher()
    {
        h_GramClear();
    }

    CT_GramHasher (int i_hash)
    {
        HRESULT h_r = S_OK;
        h_r = h_GramClear();
        if (S_OK == h_r)
        {
            h_r = h_DecodeHash (i_hash);
            if (S_OK != h_r)
            {
                throw CT_Exception (h_r, L"CT_GramHasher::i_Decode() failed.");
            }
        }
    }

    CT_GramHasher (ET_PartOfSpeech eo_pos, 
                   ET_Subparadigm eo_subparadigm, 
                   ET_Case eo_case, 
                   ET_Number eo_number, 
                   ET_Gender eo_gender,
                   ET_Person eo_person,
                   ET_Animacy eo_animacy,
                   ET_Reflexive eo_reflexive) : 
        eo_POS (eo_pos),
        eo_Subparadigm (eo_subparadigm),
        eo_Case (eo_case),
        eo_Number (eo_number),
        eo_Gender (eo_gender),
        eo_Person (eo_person),
        eo_Animacy (eo_animacy),
        eo_Reflexive (eo_reflexive) 
    {}

    CT_GramHasher (ET_Subparadigm eo_subparadigm, // For verbs
                   ET_Number eo_number, 
                   ET_Gender eo_gender, 
                   ET_Person eo_person,
                   ET_Animacy eo_animacy,
                   ET_Case eo_case,
                   ET_Reflexive eo_reflexive) :
        eo_POS (POS_VERB),
        eo_Subparadigm (eo_subparadigm),
        eo_Case (eo_case),
        eo_Number (eo_number),
        eo_Gender (eo_gender),
        eo_Person (eo_person),
        eo_Animacy (eo_animacy),
        eo_Reflexive (eo_reflexive)
    {}

        CT_GramHasher (ET_Number eo_number, // For nouns
                   ET_Gender eo_gender, 
                   ET_Animacy eo_animacy,
                   ET_Case eo_case) :
        eo_POS (POS_NOUN),
        eo_Subparadigm (SUBPARADIGM_NOUN),
        eo_Case (eo_case),
        eo_Number (eo_number),
        eo_Gender (eo_gender),
        eo_Person (PERSON_UNDEFINED),
        eo_Animacy (eo_animacy),
        eo_Reflexive (REFL_UNDEFINED)
    {}

        CT_GramHasher (ET_Subparadigm eo_subparadigm, // For adjectives
                       ET_Number eo_number, 
                       ET_Gender eo_gender, 
                       ET_Animacy eo_animacy,
                       ET_Case eo_case) :
        eo_POS (POS_ADJ),
        eo_Subparadigm (eo_subparadigm),
        eo_Case (eo_case),
        eo_Number (eo_number),
        eo_Gender (eo_gender),
        eo_Person (PERSON_UNDEFINED),
        eo_Animacy (eo_animacy),
        eo_Reflexive (REFL_UNDEFINED)
    {}

    HRESULT h_GramClear()
    {
        eo_POS = POS_UNDEFINED;
        eo_Subparadigm = SUBPARADIGM_UNDEFINED;
        eo_Case = CASE_UNDEFINED;
        eo_Number = NUM_UNDEFINED;
        eo_Gender = GENDER_UNDEFINED;
        eo_Person = PERSON_UNDEFINED;
        eo_Animacy = ANIM_UNDEFINED;
        eo_Reflexive = REFL_UNDEFINED;
        return S_OK;
    }

    int i_GramHash()
    {
        // Ranges:
        //          0-999: Nouns (incl. pronouns and numerals)
        //       1000-3999: Adjectives (incl. pronouns and numerals)
        //     50'000-99'999: Verbs
        //    100'000-infinity: all the rest (unchangeable Parts of Speech)

        int i_hash = 0;
        int i_shift = 0;

        if ((eo_POS == POS_NOUN && (eo_Subparadigm != SUBPARADIGM_NOUN && eo_Subparadigm != SUBPARADIGM_UNDEFINED))
            || ((eo_POS == POS_ADJ || eo_POS == POS_NUM || eo_POS == POS_PRONOUN) && 
               (eo_Subparadigm != SUBPARADIGM_SHORT_ADJ &&
                eo_Subparadigm != SUBPARADIGM_LONG_ADJ &&
                eo_Subparadigm != SUBPARADIGM_COMPARATIVE &&
                eo_Subparadigm != SUBPARADIGM_NOUN))
            || (eo_POS == POS_VERB && (eo_Subparadigm != SUBPARADIGM_UNDEFINED
                                && eo_Subparadigm != SUBPARADIGM_INFINITIVE
                                && eo_Subparadigm != SUBPARADIGM_PAST_TENSE 
                                && eo_Subparadigm != SUBPARADIGM_PRESENT_TENSE 
                                && eo_Subparadigm != SUBPARADIGM_IMPERATIVE 
                                && eo_Subparadigm != SUBPARADIGM_ADVERBIAL_PRESENT 
                                && eo_Subparadigm != SUBPARADIGM_ADVERBIAL_PAST 
                                && eo_Subparadigm != SUBPARADIGM_PART_PRES_ACT 
                                && eo_Subparadigm != SUBPARADIGM_PART_PRES_PASS_LONG
                                && eo_Subparadigm != SUBPARADIGM_PART_PRES_PASS_SHORT
                                && eo_Subparadigm != SUBPARADIGM_PART_PAST_ACT 
                                && eo_Subparadigm != SUBPARADIGM_PART_PAST_PASS_LONG
                                && eo_Subparadigm != SUBPARADIGM_PART_PAST_PASS_SHORT)))
        {
            return -1;
        }

        if (eo_POS == POS_UNDEFINED || eo_POS == POS_ADV || eo_POS == POS_PREP ||
            eo_POS == POS_CONJ || eo_POS == POS_PARTICLE || eo_POS == POS_COMPAR ||
            eo_POS == POS_INTERJ || eo_POS == POS_PARENTH)
        {
            i_hash = MIN_UNCHANGEABLE + (int)eo_POS;
            return i_hash;
        }
        int i_Case;
        if (eo_Subparadigm == SUBPARADIGM_NOUN) // Nouns, pronouns, and numerals
        {
            i_hash = MIN_NOUN + eo_POS * CASE_COUNT * NUM_COUNT + eo_Case * NUM_COUNT + eo_Number;
            return i_hash;
        }
        if (eo_POS == POS_ADJ || 
            ((eo_POS == POS_NUM || eo_POS == POS_PRONOUN) && 
             (eo_Subparadigm == SUBPARADIGM_LONG_ADJ || 
              eo_Subparadigm == SUBPARADIGM_SHORT_ADJ))) // 264 possibilities
        {
            if (eo_Case != CASE_ACC || eo_Animacy != ANIM_YES)
            {
                i_Case = (int)eo_Case;
            }
            else
            {
                i_Case = CASE_COUNT; // In fact, it means Accusative case of an animate adjective
            }
            i_shift = MIN_ADJ;
            i_hash = eo_POS * SUBPARADIGM_COUNT * (CASE_COUNT + 1) * NUM_COUNT * GENDER_COUNT +
                     eo_Subparadigm * (CASE_COUNT + 1) * NUM_COUNT * GENDER_COUNT +
                     i_Case * NUM_COUNT * GENDER_COUNT +
                     eo_Number * GENDER_COUNT +
                     eo_Gender;
            return i_hash;
        }
        if (eo_POS == POS_VERB)
        {
            if (eo_Subparadigm == SUBPARADIGM_PART_PRES_ACT 
                || eo_Subparadigm == SUBPARADIGM_PART_PRES_PASS_LONG 
                || eo_Subparadigm == SUBPARADIGM_PART_PRES_PASS_SHORT 
                || eo_Subparadigm == SUBPARADIGM_PART_PAST_ACT 
                || eo_Subparadigm == SUBPARADIGM_PART_PAST_PASS_LONG
                || eo_Subparadigm == SUBPARADIGM_PART_PAST_PASS_SHORT
                || eo_Subparadigm == SUBPARADIGM_PAST_TENSE)
            {
                if (eo_Case != CASE_ACC || eo_Animacy != ANIM_YES)
                {
                    i_Case = (int)eo_Case;
                }
                else           
                {
                    i_Case = CASE_COUNT; // In fact, it means Accusative case of an animate participle
                }
                i_hash = MIN_VERB +
                    eo_Subparadigm * (CASE_COUNT + 1) * NUM_COUNT * GENDER_COUNT * REFL_COUNT + 
                    i_Case * NUM_COUNT * GENDER_COUNT * REFL_COUNT + 
                    eo_Number * GENDER_COUNT * REFL_COUNT + 
                    eo_Gender * REFL_COUNT +  
                    eo_Reflexive;
                return i_hash;
            }
            else if (eo_Subparadigm == SUBPARADIGM_PRESENT_TENSE
                || eo_Subparadigm == SUBPARADIGM_IMPERATIVE)
            {
                i_hash = MIN_VERB +
                    eo_Subparadigm * (CASE_COUNT + 1) * NUM_COUNT * PERSON_COUNT * REFL_COUNT + 
                    0 * NUM_COUNT * PERSON_COUNT * REFL_COUNT + 
                    eo_Number * PERSON_COUNT * REFL_COUNT + 
                    eo_Person * REFL_COUNT +  
                    eo_Reflexive;
            }
            else if (eo_Subparadigm == SUBPARADIGM_ADVERBIAL_PRESENT 
                || eo_Subparadigm == SUBPARADIGM_ADVERBIAL_PAST
                || eo_Subparadigm == SUBPARADIGM_INFINITIVE
                || eo_Subparadigm == SUBPARADIGM_UNDEFINED)
            {
                i_hash = MIN_VERB +
                    eo_Subparadigm * (CASE_COUNT + 1) * NUM_COUNT * PERSON_COUNT * REFL_COUNT + 
                    0 * NUM_COUNT * PERSON_COUNT * REFL_COUNT + 
                    0 * PERSON_COUNT * REFL_COUNT + 
                    0 * REFL_COUNT +  
                    eo_Reflexive;
            }
            return i_hash;
        }
        
        if (eo_Subparadigm == SUBPARADIGM_COMPARATIVE) // 1 possibility
        {
            i_hash = MIN_VERB + SUBPARADIGM_COUNT * (CASE_COUNT + 1) * NUM_COUNT * PERSON_COUNT * REFL_COUNT;
            return i_hash;
        }
        return -1;
    } // get_GramHash

    HRESULT h_DecodeHash (int i_hash)
    {
        // Ranges:
        //          0-999: Nouns (incl. pronouns and numerals)
        //       1000-3999: Adjectives (incl. pronouns and numerals)
        //     50'000-99'999: Verbs
        //    100'000-infinity: all the rest (unchangeable Parts of Speech)

        int i_shift = 0;
        h_GramClear();
        if (i_hash >= MIN_NOUN && i_hash < MIN_ADJ)
        {
            i_hash -= MIN_NOUN;
            eo_Subparadigm = SUBPARADIGM_NOUN;
            eo_Number = (ET_Number)(i_hash % NUM_COUNT);
            i_hash = (i_hash - eo_Number) / NUM_COUNT;
            eo_Case = (ET_Case)(i_hash % CASE_COUNT);
            i_hash = (i_hash - eo_Case) / CASE_COUNT;
            eo_POS = (ET_PartOfSpeech)(i_hash % POS_COUNT);
            return S_OK;
        }
        else if (i_hash >= MIN_ADJ && i_hash < MIN_VERB)
        {
            i_hash -= MIN_ADJ;
            eo_Gender = (ET_Gender)(i_hash % GENDER_COUNT);
            i_hash = (i_hash - eo_Gender) / GENDER_COUNT;
            eo_Number = (ET_Number)(i_hash % NUM_COUNT);
            i_hash = (i_hash - eo_Number) / NUM_COUNT;
            int i_tmp_case = i_hash % (CASE_COUNT + 1);
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
            i_hash = (i_hash - i_tmp_case) / (CASE_COUNT + 1);
            eo_Subparadigm = (ET_Subparadigm)(i_hash % SUBPARADIGM_COUNT);
            i_hash = (i_hash - eo_Subparadigm) / SUBPARADIGM_COUNT;
            eo_POS = (ET_PartOfSpeech)(i_hash % POS_COUNT);
        }
        else if (i_hash >= MIN_VERB && i_hash < MIN_UNCHANGEABLE)
        {
            eo_POS = POS_VERB;
            i_hash -= MIN_VERB;
            eo_Subparadigm = (ET_Subparadigm)(i_hash / ((CASE_COUNT + 1) * NUM_COUNT * PERSON_COUNT * REFL_COUNT));
            if (eo_Subparadigm == SUBPARADIGM_PART_PRES_ACT 
                || eo_Subparadigm == SUBPARADIGM_PART_PRES_PASS_LONG 
                || eo_Subparadigm == SUBPARADIGM_PART_PRES_PASS_SHORT 
                || eo_Subparadigm == SUBPARADIGM_PART_PAST_ACT 
                || eo_Subparadigm == SUBPARADIGM_PART_PAST_PASS_LONG
                || eo_Subparadigm == SUBPARADIGM_PART_PAST_PASS_SHORT
                || eo_Subparadigm == SUBPARADIGM_PAST_TENSE)
            {
                eo_Reflexive = (ET_Reflexive)(i_hash % REFL_COUNT);
                i_hash = (i_hash - eo_Reflexive) / REFL_COUNT;
                eo_Gender = (ET_Gender)(i_hash % GENDER_COUNT);
                i_hash = (i_hash - eo_Gender) / GENDER_COUNT;
                eo_Number = (ET_Number)(i_hash % NUM_COUNT);
                i_hash = (i_hash - eo_Number) / NUM_COUNT;
                int i_tmp_case = i_hash % (CASE_COUNT + 1);
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
                return S_OK;
            }
            else if (eo_Subparadigm == SUBPARADIGM_PRESENT_TENSE
                || eo_Subparadigm == SUBPARADIGM_IMPERATIVE)
            {
                eo_Reflexive = (ET_Reflexive)(i_hash % REFL_COUNT);
                i_hash = (i_hash - eo_Reflexive) / REFL_COUNT;
                eo_Person = (ET_Person)(i_hash % PERSON_COUNT);
                i_hash = (i_hash - eo_Person) / PERSON_COUNT;
                eo_Number = (ET_Number)(i_hash % NUM_COUNT);
                return S_OK;
            }
            else if (eo_Subparadigm == SUBPARADIGM_ADVERBIAL_PRESENT 
                || eo_Subparadigm == SUBPARADIGM_ADVERBIAL_PAST
                || eo_Subparadigm == SUBPARADIGM_INFINITIVE
                || eo_Subparadigm == SUBPARADIGM_UNDEFINED)
            {
                eo_Reflexive = (ET_Reflexive)(i_hash % REFL_COUNT);
                return S_OK;
            }
        }
        else if (i_hash >= MIN_UNCHANGEABLE && i_hash < MIN_UNCHANGEABLE + POS_COUNT)
        {
            eo_POS = (ET_PartOfSpeech)(i_hash - MIN_UNCHANGEABLE);
            return S_OK;
        }

        return E_FAIL;
    }   // h_DecodeHash (long i_hash)

    int i_DecodeString (wstring str_gram)
    {
        // Partially depends on POS.
       
        //                           **** Warning! ****
        // 1. The list may be incomplete.
        // 2. Verb reflexivity is recognized only if str_lemma is present.
        // 3. If the description is deficient, subparadigm may rely on 
        //    aspect information.
        // 4. The user must make sure all unnecessary grammar information
        //    has been erased.

        if (str_gram.length() <= 0)
        {
            return E_INVALIDARG;
        }
        if (eo_POS == POS_UNDEFINED)
        {
            return E_FAIL;
        }

        ET_PartOfSpeech eo_POS_tmp = eo_POS;
        //h_GramClear();
        eo_POS = eo_POS_tmp;

        // Text parsing
        wsmatch result;
        wstring str_parameter;
        bool b_match;
        bool b_short = false;
        et_Subtype eo_subtype = ec_SubtypeUndefined;
        et_Tense eo_tense = ec_TenseUndefined;
        ET_Voice eo_voice = VOICE_UNDEFINED;

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
                eo_Case = CASE_LOC;
//                eo_Case = CASE_PREP;
            }
            if (str_parameter == L"Р2")       
            {
                eo_Case = CASE_PART;
            }
            if (str_parameter == L"П2")
            {
                eo_Case = CASE_LOC;          
            }
            if (str_parameter == L"кф" && eo_POS == POS_ADJ)
            {
                eo_Subparadigm = SUBPARADIGM_SHORT_ADJ;
            }
            if (str_parameter == L"пф" && eo_POS == POS_ADJ)
            {
                eo_Subparadigm = SUBPARADIGM_LONG_ADJ;
            }
            if (str_parameter == L"кф" && eo_POS == POS_VERB)
            {
                b_short = true;
            }
            if (str_parameter == L"буд.")
            {
                eo_tense = ec_TenseFuture;
            }
            if (str_parameter == L"наст.")
            {
                eo_tense = ec_TensePresent;
            }
            if (str_parameter == L"прош.")
            {
                eo_tense = ec_TensePast;
            }
            if (str_parameter == L"прич.")
            {
                eo_subtype = ec_Participle;
            }
            if (str_parameter == L"деепр.")
            {
                eo_subtype = ec_Adverbial;
            }
            if (str_parameter == L"повел.")
            {
                eo_Subparadigm = SUBPARADIGM_IMPERATIVE;
            }
            if (str_parameter == L"страд.")
            {
                eo_voice = VOICE_PASSIVE;
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
            if (str_parameter == L"неод.")
            {
                eo_Animacy = ANIM_NO;
            }
            if (str_parameter == L"сравн.")
            {
                eo_Subparadigm = SUBPARADIGM_COMPARATIVE;
            }
            if (str_parameter == L"инф.")
            {
                eo_Subparadigm = SUBPARADIGM_INFINITIVE;
            }
        }

        // Implications
        if (eo_POS == POS_NOUN)
        {
            eo_Subparadigm = SUBPARADIGM_NOUN;
        }
        if ((eo_POS == POS_NUM || eo_POS == POS_PRONOUN) &&
            eo_Subparadigm == SUBPARADIGM_UNDEFINED)
        {
            eo_Subparadigm = SUBPARADIGM_NOUN;
        }
        if (eo_POS == POS_ADJ)
        {
            if (eo_Subparadigm == SUBPARADIGM_UNDEFINED)
            {
                eo_Subparadigm = SUBPARADIGM_LONG_ADJ;
            }
            if (eo_Case == CASE_ACC && eo_Animacy == ANIM_UNDEFINED)
            {
                eo_Animacy = ANIM_NO;
            }
            if (GENDER_M == eo_Gender || 
                GENDER_F == eo_Gender || 
                GENDER_N == eo_Gender || 
                NUM_UNDEFINED == eo_Number)
            {
                eo_Number = NUM_SG;
            }
            if (NUM_PL == eo_Number)
            {
                eo_Gender = GENDER_UNDEFINED;
            }
        }
        if (eo_POS == POS_VERB)
        {
            // Convert several parameters to Subparadigm
            if (ec_SubtypeUndefined == eo_subtype &&
                (eo_tense == ec_TensePresent || eo_tense == ec_TenseFuture))
            {
                eo_Subparadigm = SUBPARADIGM_PRESENT_TENSE;
            }
            if (ec_SubtypeUndefined == eo_subtype && eo_tense == ec_TensePast)
            {
                eo_Subparadigm = SUBPARADIGM_PAST_TENSE;
            }
            if (ec_Participle == eo_subtype &&
                eo_voice == VOICE_PASSIVE &&
                eo_tense == ec_TenseUndefined)
            {
                eo_tense = ec_TensePast;
            }
            if (ec_Participle == eo_subtype &&
                eo_voice == VOICE_UNDEFINED)
            {
                eo_voice = VOICE_ACTIVE;
            }
            if (ec_Participle == eo_subtype &&
                (eo_tense == ec_TensePresent || eo_tense == ec_TenseFuture) &&
                eo_voice == VOICE_ACTIVE)
            {
                eo_Subparadigm = SUBPARADIGM_PART_PRES_ACT;
            }
            if (ec_Participle == eo_subtype && 
                (eo_tense == ec_TensePresent || eo_tense == ec_TenseFuture) &&
                eo_voice == VOICE_PASSIVE)
            {
                if (b_short)
                {
                    eo_Subparadigm = SUBPARADIGM_PART_PRES_PASS_SHORT;
                }
                else
                {
                    eo_Subparadigm = SUBPARADIGM_PART_PRES_PASS_LONG;
                }
            }
            if (ec_Participle == eo_subtype &&
                eo_tense == ec_TensePast &&
                eo_voice == VOICE_ACTIVE)
            {
                eo_Subparadigm = SUBPARADIGM_PART_PAST_ACT;
            }
            if (ec_Participle == eo_subtype &&
                eo_tense == ec_TensePast &&
                eo_voice == VOICE_PASSIVE)
            {
                if (b_short)
                {
                    eo_Subparadigm = SUBPARADIGM_PART_PAST_PASS_SHORT;
                }
                else
                {
                    eo_Subparadigm = SUBPARADIGM_PART_PAST_PASS_LONG;
                }
            }
            if (ec_Adverbial == eo_subtype &&
                (eo_tense == ec_TensePresent || eo_tense == ec_TenseFuture))
            {
                eo_Subparadigm = SUBPARADIGM_ADVERBIAL_PRESENT;
            }
            if (ec_Adverbial == eo_subtype && eo_tense == ec_TensePast)
            {
                eo_Subparadigm = SUBPARADIGM_ADVERBIAL_PAST;
            }

            // Other specifications
            if (SUBPARADIGM_IMPERATIVE == eo_subtype && eo_Person == PERSON_UNDEFINED)
            {
                eo_Person = PERSON_2;
                eo_Number = NUM_SG;     // PL is never given in the source
            }

            if (ec_Participle == eo_subtype &&
                eo_Case == CASE_ACC &&
                eo_Animacy == ANIM_UNDEFINED)
            {
                eo_Animacy = ANIM_NO;
            }
            /* // Temporarily blocked
            b_match = regex_match(str_Lemma, result, (const wregex)L".*(ся|сь)");
            if (b_match == true)
            {
                eo_Reflexive = REFL_YES;
            }
            else
            {
                eo_Reflexive = REFL_NO;
            }
            */

            if (eo_Subparadigm == SUBPARADIGM_PAST_TENSE ||
                ec_Participle == eo_subtype)
            {
                if ((GENDER_M == eo_Gender || GENDER_F == eo_Gender ||
                    GENDER_N == eo_Gender) &&
                    NUM_UNDEFINED == eo_Number)
                {
                    eo_Number = NUM_SG;
                }
                if (NUM_PL == eo_Number)
                {
                    eo_Gender = GENDER_UNDEFINED;
                }
            }
        }
       
        return S_OK;

    } // DecodeString (BSTR bstr_gram)

};
