using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestUI
{
    public enum ET_Ternary
    {
        TERNARY_FALSE,
        TERNARY_TRUE,
        TERNARY_BOTH
    }

    public enum ET_ReturnCode
    {
        H_NO_ERROR,
        H_FALSE,
        H_TRUE,
        H_NO_MORE,
        H_ERROR_GENERAL,
        H_ERROR_UNEXPECTED,
        H_ERROR_POINTER,
        H_ERROR_INVALID_ARG,
        H_ERROR_DB,
        H_EXCEPTION,            // an exception was caught and handled upstream
        H_CTOR,                 // exception thrown from a ctor
        H_NOTIMPL,              // interface method not implemented (but may be in the future)
        H_UNAVAILABLE           // interface method not implemented (and never will be) for this consumer
    }

    public enum ET_PartOfSpeech
    {
        POS_UNDEFINED,
        POS_NOUN,
        POS_VERB,
        POS_ADJ,
        POS_PRONOUN,
        POS_PRONOUN_ADJ,
        POS_PRONOUN_PREDIC,
        POS_NUM,
        POS_NUM_ADJ,
        POS_ADV,
        POS_COMPAR,
        POS_PREDIC,
        POS_PREP,
        POS_CONJ,
        POS_PARTICLE,
        POS_INTERJ,
        POS_PARENTH,
        POS_NULL,
        POS_COUNT
    }

    public enum ET_InflectionType
    {
        INFLECTION_TYPE_UNDEFINED = -1,
        CONJUGATIONS_COUNT = 16
    }

    public enum ET_Case
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
        CASE_NUM,
        CASE_COUNT
    }

    public enum ET_Number
    {
        NUM_UNDEFINED,
        NUM_SG,
        NUM_PL,
        NUM_COUNT

    }

    public enum ET_Subparadigm
    {
        SUBPARADIGM_UNDEFINED,
        SUBPARADIGM_NOUN,
        SUBPARADIGM_SHORT_ADJ,
        SUBPARADIGM_LONG_ADJ,
        SUBPARADIGM_COMPARATIVE,
        SUBPARADIGM_INFINITIVE,
        SUBPARADIGM_PRESENT_TENSE,
        SUBPARADIGM_PAST_TENSE,
        SUBPARADIGM_IMPERATIVE,
        SUBPARADIGM_ADVERBIAL_PRESENT,
        SUBPARADIGM_ADVERBIAL_PAST,
        SUBPARADIGM_PART_PRES_ACT,
        SUBPARADIGM_PART_PRES_PASS_LONG,
        SUBPARADIGM_PART_PRES_PASS_SHORT,
        SUBPARADIGM_PART_PAST_ACT,
        SUBPARADIGM_PART_PAST_PASS_LONG,
        SUBPARADIGM_PART_PAST_PASS_SHORT,
        SUBPARADIGM_COUNT
    }

    public enum ET_Gender
    {
        GENDER_UNDEFINED,
        GENDER_M,
        GENDER_F,
        GENDER_N,
        GENDER_COUNT
    }

    public enum ET_Aspect
    {
        ASPECT_UNDEFINED,
        ASPECT_IMPERFECTIVE,
        ASPECT_PERFECTIVE,
        ASPECT_COUNT

    }

    public enum ET_Person
    {
        PERSON_UNDEFINED,
        PERSON_1,
        PERSON_2,
        PERSON_3,
        PERSON_COUNT
    }

    public enum ET_Animacy
    {
        ANIM_UNDEFINED,
        ANIM_YES,
        ANIM_NO,
        ANIM_COUNT
    }

    public enum ET_Voice
    {
        VOICE_UNDEFINED,
        VOICE_ACTIVE,
        VOICE_PASSIVE,
        VOICE_COUNT
    }

    public enum ET_Reflexive
    {
        REFL_UNDEFINED,
        REFL_YES,
        REFL_NO,
        REFL_COUNT

    }

    public enum ET_AccentType
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
    }

    public enum ET_EndingClass
    {
        ENDING_CLASS_UNDEFINED,
        ENDING_CLASS_NOUN,
        ENDING_CLASS_ADJECTIVE,
        ENDING_CLASS_PRONOUN,
        ENDING_CLASS_SHORT_ADJECTIVE,
        ENDING_CLASS_PRESENT_TENSE,
        ENDING_CLASS_PAST_TENSE,
        ENDING_CLASS_COMPARATIVE,
        ENDING_CLASS_COUNT
    }

    enum ET_StressLocation
    {
        STRESS_LOCATION_UNDEFINED,
        STRESS_LOCATION_STEM,
        STRESS_LOCATION_ENDING,
        STRESS_LOCATION_COUNT
    }

    public enum ET_StressType
    {
        STRESS_TYPE_UNDEFINED,
        STRESS_PRIMARY,
        STRESS_SECONDARY,
        STRESS_TYPE_COUNT
    }

    public enum ET_StemAuslaut
    {
        STEM_AUSLAUT_UNDEFINED,
        STEM_AUSLAUT_SH,
        STEM_AUSLAUT_NOT_SH,
        STEM_AUSLAUT_COUNT
    }

    public enum ET_Status
    {
        STATUS_UNDEFINED,
        STATUS_COMMON,
        STATUS_OBSOLETE,
        STATUS_RARE,
        STATUS_QUESTIONABLE,
        STATUS_INCORRECT,
        STATUS_COUNT
    }

    public enum ET_TestResult
    {
        TEST_RESULT_UNDEFINED,
        TEST_RESULT_OK,
        TEST_RESULT_FAIL,
        TEST_RESULT_INCOMPLETE,
        TEST_RESULT_COUNT
    }

}   // namespace Hlib
