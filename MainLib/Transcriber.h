#ifndef C_TRANSCRIBER_H_INCLUDED
#define C_TRANSCRIBER_H_INCLUDED

#include <vector>
#include <set>
#include <map>
#include <memory>
#include <variant>

#include "Enums.h"
#include "EString.h"
#include "SqliteWrapper.h"

namespace Hlib
{
    static CEString m_LowerCaseCyrillic(L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя");
    static CEString m_VoicedConsonants(L"бвдзжглмнрй");
    static CEString m_VoicelessConsonants(L"пфтсшкчщцх");
    static CEString m_PairedVoiced(L"бвдзжг");
    static CEString PairedVoicelessConsonants(L"пфтсшк");
    static CEString PairedHardSoftConsonants(L"бпвфдтзслнмркгх");

    // Input: a Cyrillic character, a sequence of characters, or two 
    // character sequences separated by "+"
    using PairInput = pair<CEString, CEString>;     // 1st element must exist, 2nd element is optional
    using PhonemicContextAtom = variant<ET_PhonemicContext, CEString>;
    using MorphemicContextAtom = variant<ET_MorphemicContext, CEString>;

    struct StRule
    {
        set<ET_VowelStressRelation> m_setStressContexts;
        set<vector<PhonemicContextAtom>> m_setLeftContexts;
        set<vector<PhonemicContextAtom>> m_setRightContexts;
        set<MorphemicContextAtom> m_setMorphemicContexts;
        set<ET_Subparadigm> m_setSubparadigms;
        set<ET_Gender> m_setGenders;
        set<ET_Number> m_setNumbers;
        set<ET_Case> m_setCases;  
        ET_RuleStrength m_eStrength;
        bool m_bIsVariant;
        CEString m_sComment;
    };

    struct StWord       // in tact group
    {
        long long llTactGroupId = -1;
        long long llLineId = -1;
        int iPosition = -1;     // pos in tact group
        int iMainWord = -1;
        int iNumOfWords = -1;
        CEString sSource;
        CEString sGramHash;
        // transcription here
        int iNumOfSyllables = -1;
        int iStressedSyllable = -1;
        int iReverseStressedSyllable = -1;
        int iSecondaryStressSyllable = -1;
    };
    
    class CTranscriber
    {
    public:
        enum class EC_CONTEXT_TYPE { LEFT, RIGHT };

    public:
        CTranscriber(CSqlite *);
        ET_ReturnCode eLoadTranscriptionRules();
        ET_ReturnCode eTranscribe();
        ET_ReturnCode eTranscribeTactGroup(vector<StWord>&);

    private:
        ET_ReturnCode eFormatInputs(CEString& sSource, PairInput& pairParsedInputs);
        ET_ReturnCode eSplitSource(CEString& sSource, vector<CEString>& vecOutput);
        ET_ReturnCode eParseContexts(CEString& sSource, set<vector<PhonemicContextAtom>>& setTarget);
        //        ET_ReturnCode eFormatContexts(const CEString& sContext, EC_CONTEXT_TYPE eLeftOrRight);
        //        ET_ReturnCode eSearchInSortedStrings(const vector<CEString>&, const CEString);


    private:
        map<PairInput, vector<StRule>> m_mapRules;
//        CEString m_sDbPath;
        CSqlite * m_pDb;

    private:
        struct StConsonant
        {
            ET_Place m_ePlace;
            ET_Manner m_eManner;
            ET_Voicedness m_eVoicedness;
            ET_Palatalization m_ePalatalization;
            ET_Nasality m_eNasality;

            StConsonant(ET_Place ePlace, ET_Manner eManner, ET_Voicedness eVoicedness, ET_Palatalization ePalatalization, ET_Nasality eNasality) :
                m_ePlace(ePlace), m_eManner(eManner), m_eVoicedness(eVoicedness), m_ePalatalization(ePalatalization), m_eNasality(eNasality)
            {}
        };

        struct StStressedVowel
        {
            ET_Height m_eHeight;
            ET_Frontness m_eFrontness;

            StStressedVowel(ET_Frontness eFrontness, ET_Height eHeight) :
                m_eFrontness(eFrontness), m_eHeight(eHeight)
            {}
        };

        struct StPretonicVowel
        {
            ET_Height m_eHeight;
            ET_Frontness m_eFrontness;

            StPretonicVowel(ET_Frontness eFrontness, ET_Height eHeight) :
                m_eFrontness(eFrontness), m_eHeight(eHeight)
            {}
        };

        struct StUnstressedVowel
        {
            ET_Height m_eHeight;
            ET_Frontness m_eFrontness;

            StUnstressedVowel(ET_Frontness eFrontness, ET_Height eHeight) :
                m_eFrontness(eFrontness), m_eHeight(eHeight)
            {}
        };

        struct StAcoustic
        {
            ET_AcousticFeatureValue m_eVocalicNonVocalic;
            ET_AcousticFeatureValue m_eConsonantalNonconsonantal;
            ET_AcousticFeatureValue m_eCompactDiffuse;
            ET_AcousticFeatureValue m_eGraveAcute;
            ET_AcousticFeatureValue m_eFlatPlain;
            ET_AcousticFeatureValue m_eNasalOral;
            ET_AcousticFeatureValue m_eTenseLax;
            ET_AcousticFeatureValue m_eContinuantInterrupted;
            ET_AcousticFeatureValue m_eStridentMellow;

            StAcoustic(ET_AcousticFeatureValue eVocalic, ET_AcousticFeatureValue eCons, ET_AcousticFeatureValue eCompDiff,
                ET_AcousticFeatureValue eGraveAcute, ET_AcousticFeatureValue eFlatPlain, ET_AcousticFeatureValue eNasalOral,
                ET_AcousticFeatureValue eTenseLax, ET_AcousticFeatureValue eContInt, ET_AcousticFeatureValue eStriMel) :
                m_eVocalicNonVocalic(eVocalic),
                m_eConsonantalNonconsonantal(eCons),
                m_eCompactDiffuse(eCompDiff),
                m_eGraveAcute(eGraveAcute),
                m_eFlatPlain(eFlatPlain),
                m_eNasalOral(eNasalOral),
                m_eTenseLax(eTenseLax),
                m_eContinuantInterrupted(eContInt),
                m_eStridentMellow(eStriMel)
            {}
        };

    private:
        map<CEString, ET_VowelStressRelation> m_mapStringToStressRelation =
        {
            { L"STRESSED", ET_VowelStressRelation::STRESSED },
            { L"PRETONIC", ET_VowelStressRelation::PRETONIC },
            { L"FIRST_PRETONIC", ET_VowelStressRelation::FIRST_PRETONIC },
            { L"OTHER_PRETONIC", ET_VowelStressRelation::OTHER_PRETONIC },
            { L"POSTTONIC", ET_VowelStressRelation::POSTTONIC },
            { L"POSTTONIC", ET_VowelStressRelation::VOWEL_STRESS_RELATION_COUNT }
        };

        map<CEString, ET_PhonemicContext> m_mapStringToContext =
        {
            { L"VOWEL", ET_PhonemicContext::VOWEL },
            { L"CONSONANT", ET_PhonemicContext::CONSONANT },
            { L"HARD_CONSONANT", ET_PhonemicContext::HARD_CONSONANT },
            { L"HARD_PAIRED_CONSONANT", ET_PhonemicContext::HARD_PAIRED_CONSONANT },
            { L"SOFT_CONSONANT", ET_PhonemicContext::SOFT_CONSONANT},
            { L"SOFT_CONSONANT_NO_CH_SHCH", ET_PhonemicContext::SOFT_CONSONANT_NO_CH_SHCH },
            { L"VOICELESS", ET_PhonemicContext::VOICELESS },
            { L"BOUNDARY_WORD", ET_PhonemicContext::BOUNDARY_WORD },
            { L"BOUNDARY_NOT_PROCLITIC", ET_PhonemicContext::BOUNDARY_NOT_PROCLITIC },
            { L"BOUNDARY_SYNTAGM", ET_PhonemicContext::BOUNDARY_SYNTAGM }
        };

        map<CEString, ET_Sounds> m_mapStringToTranscriptionSymbol =
        {
            { L"STRESSED_A", ET_Sounds::STRESSED_A },
            { L"STRESSED_O", ET_Sounds::STRESSED_O },
            { L"STRESSED_E", ET_Sounds::STRESSED_E },
            { L"STRESSED_I", ET_Sounds::STRESSED_I },
            { L"STRESSED_Y", ET_Sounds::STRESSED_Y },
            { L"STRESSED_U", ET_Sounds::STRESSED_U },
            { L"PRETONIC_A", ET_Sounds::PRETONIC_A },
            { L"PRETONIC_IE", ET_Sounds::PRETONIC_IE },
            { L"PRETONIC_Y", ET_Sounds::PRETONIC_Y },
            { L"PRETONIC_U", ET_Sounds::PRETONIC_U },
            { L"SCHWA", ET_Sounds::SCHWA },
            { L"UNSTRESSED_FRONT_VOWEL", ET_Sounds::UNSTRESSED_FRONT_VOWEL },
            { L"UNSTRESSED_I", ET_Sounds::UNSTRESSED_I },
            { L"UNSTRESSED_Y", ET_Sounds::UNSTRESSED_Y },
            { L"UNSTRESSED_U", ET_Sounds::UNSTRESSED_U },
            { L"P_HARD", ET_Sounds::P_HARD },
            { L"P_SOFT", ET_Sounds::P_SOFT },
            { L"B_HARD", ET_Sounds::B_HARD },
            { L"B_SOFT", ET_Sounds::B_SOFT },
            { L"M_HARD", ET_Sounds::M_HARD },
            { L"M_SOFT", ET_Sounds::M_SOFT },
            { L"F_HARD", ET_Sounds::F_HARD },
            { L"F_SOFT", ET_Sounds::F_SOFT },
            { L"V_HARD", ET_Sounds::V_HARD },
            { L"V_SOFT", ET_Sounds::V_SOFT },
            { L"N_HARD", ET_Sounds::N_HARD },
            { L"N_SOFT", ET_Sounds::N_SOFT },
            { L"T_HARD", ET_Sounds::T_HARD },
            { L"T_SOFT", ET_Sounds::T_SOFT },
            { L"D_HARD", ET_Sounds::D_HARD },
            { L"D_SOFT", ET_Sounds::D_SOFT },
            { L"TS_HARD", ET_Sounds::TS_HARD },
            { L"TS_SOFT", ET_Sounds::TS_SOFT },         // Цюрих, хуацяо
            { L"S_HARD", ET_Sounds::S_HARD },
            { L"S_SOFT", ET_Sounds::S_SOFT },
            { L"L_HARD", ET_Sounds::L_HARD },
            { L"L_SOFT", ET_Sounds::L_SOFT },
            { L"Z_HARD", ET_Sounds::Z_HARD },
            { L"Z_SOFT", ET_Sounds::Z_SOFT },
            { L"R_HARD", ET_Sounds::R_HARD },
            { L"R_SOFT", ET_Sounds::R_SOFT },
            { L"SH_HARD", ET_Sounds::SH_HARD },
            { L"SH_SOFT", ET_Sounds::SH_SOFT },
            { L"ZH_HARD", ET_Sounds::ZH_HARD },
            { L"ZH_SOFT", ET_Sounds::ZH_SOFT },
            { L"JOD", ET_Sounds::JOD },
            { L"K_HARD", ET_Sounds::K_HARD },
            { L"K_SOFT", ET_Sounds::K_SOFT },
            { L"X_HARD", ET_Sounds::X_HARD },
            { L"X_SOFT", ET_Sounds::X_SOFT },
            { L"G_FRICATIVE_HARD", ET_Sounds::G_FRICATIVE_HARD },
            { L"G_FRICATIVE_SOFT", ET_Sounds::G_FRICATIVE_SOFT }
        };

        map<CEString, ET_MorphemicContext> m_mapStringToMorphemicContext =
        {
            { L"ENDING", ET_MorphemicContext::ENDING },
            { L"ROOT", ET_MorphemicContext::ROOT },
            { L"ROOT_AUSLAUT", ET_MorphemicContext::ROOT_AUSLAUT }
        };

        map<CEString, ET_Subparadigm> m_mapStringToSubparadigm =
        {
            { L"SUBPARADIGM_NOUN", ET_Subparadigm::SUBPARADIGM_NOUN },
            { L"SUBPARADIGM_LONG_ADJ", ET_Subparadigm::SUBPARADIGM_LONG_ADJ },
            { L"SUBPARADIGM_SHORT_ADJ", ET_Subparadigm::SUBPARADIGM_SHORT_ADJ },
            { L"SUBPARADIGM_COMPARATIVE", ET_Subparadigm::SUBPARADIGM_COMPARATIVE },
            { L"SUBPARADIGM_PRONOUN", ET_Subparadigm::SUBPARADIGM_PRONOUN },
            { L"SUBPARADIGM_PRONOUN_ADJ", ET_Subparadigm::SUBPARADIGM_PRONOUN_ADJ },
            { L"SUBPARADIGM_INFINITIVE", ET_Subparadigm::SUBPARADIGM_INFINITIVE },
            { L"SUBPARADIGM_PRESENT_TENSE", ET_Subparadigm::SUBPARADIGM_PRESENT_TENSE },
            { L"SUBPARADIGM_PAST_TENSE", ET_Subparadigm::SUBPARADIGM_PAST_TENSE },
            { L"SUBPARADIGM_IMPERATIVE", ET_Subparadigm::SUBPARADIGM_IMPERATIVE },
            { L"SUBPARADIGM_ADVERBIAL_PRESENT", ET_Subparadigm::SUBPARADIGM_ADVERBIAL_PRESENT },
            { L"SUBPARADIGM_ADVERBIAL_PAST", ET_Subparadigm::SUBPARADIGM_ADVERBIAL_PAST },
            { L"SUBPARADIGM_PART_PRES_ACT", ET_Subparadigm::SUBPARADIGM_PART_PRES_ACT },
            { L"SUBPARADIGM_PART_PRES_PASS_LONG", ET_Subparadigm::SUBPARADIGM_PART_PRES_PASS_LONG },
            { L"SUBPARADIGM_PART_PRES_PASS_SHORT", ET_Subparadigm::SUBPARADIGM_PART_PRES_PASS_SHORT },
            { L"SUBPARADIGM_PART_PAST_ACT", ET_Subparadigm::SUBPARADIGM_PART_PAST_ACT },
            { L"SUBPARADIGM_PART_PAST_PASS_LONG", ET_Subparadigm::SUBPARADIGM_PART_PAST_PASS_LONG },
            { L"SUBPARADIGM_PART_PAST_PASS_SHORT", ET_Subparadigm::SUBPARADIGM_PART_PAST_PASS_SHORT },
            { L"SUBPARADIGM_NUM", ET_Subparadigm::SUBPARADIGM_NUM },
            { L"SUBPARADIGM_NUM_ADJ", ET_Subparadigm::SUBPARADIGM_NUM_ADJ },
            { L"SUBPARADIGM_ASPECT_PAIR", ET_Subparadigm::SUBPARADIGM_ASPECT_PAIR },
            { L"SUBPARADIGM_PARTICLE", ET_Subparadigm::SUBPARADIGM_PARTICLE },
            { L"SUBPARADIGM_PREPOSITION", ET_Subparadigm::SUBPARADIGM_PREPOSITION },
            { L"SUBPARADIGM_ADVERB", ET_Subparadigm::SUBPARADIGM_ADVERB },
            { L"SUBPARADIGM_CONJUNCTION", ET_Subparadigm::SUBPARADIGM_CONJUNCTION },
            { L"SUBPARADIGM_INTERJECTION", ET_Subparadigm::SUBPARADIGM_INTERJECTION },
            { L"SUBPARADIGM_PARENTHESIS", ET_Subparadigm::SUBPARADIGM_PARENTHESIS },
            { L"SUBPARADIGM_PREDICATIVE", ET_Subparadigm::SUBPARADIGM_PREDICATIVE }
        };

        map<CEString, ET_Gender> m_mapStringToGender =
        {
            { L"GENDER_M", ET_Gender::GENDER_M },
            { L"GENDER_F", ET_Gender::GENDER_F },
            { L"GENDER_N", ET_Gender::GENDER_N }
        };

        map<CEString, ET_Number> m_mapStringToNumber =
        {
            { L"NUM_SG", ET_Number::NUM_SG },
            { L"NUM_PL", ET_Number::NUM_PL }
        };

        map<CEString, ET_Case> m_mapStringToCase =
        {
            { L"CASE_NOM", ET_Case::CASE_NOM },
            { L"CASE_ACC", ET_Case::CASE_ACC },
            { L"CASE_GEN", ET_Case::CASE_GEN },
            { L"CASE_DAT", ET_Case::CASE_DAT },
            { L"CASE_INST", ET_Case::CASE_INST },
            { L"CASE_PREP", ET_Case::CASE_PREP },
            { L"CASE_LOC", ET_Case::CASE_LOC },
            { L"CASE_NUM", ET_Case::CASE_NUM }
        };

        map<CEString, ET_RuleStrength> m_mapStringToRuleStrength =
        {
            { L"RULE_STRENGTH_DEFAULT", ET_RuleStrength::RULE_STRENGTH_DEFAULT },
            { L"RULE_STRENGTH_VARIATION", ET_RuleStrength::RULE_STRENGTH_VARIATION },
            { L"RULE_STRENGTH_OPTIONAL", ET_RuleStrength::RULE_STRENGTH_OPTIONAL }
        };

        map<ET_Sounds, StStressedVowel> m_mapSoundToStressedVowel =
        {
            { STRESSED_A, StStressedVowel(ET_Frontness::FRONTNESS_CENTRAL, ET_Height::HEIGHT_LOW) },
            { STRESSED_O, StStressedVowel(ET_Frontness::FRONTNESS_BACK, ET_Height::HEIGHT_MID) },
            { STRESSED_E, StStressedVowel(ET_Frontness::FRONTNESS_FRONT, ET_Height::HEIGHT_MID) },
            { STRESSED_I, StStressedVowel(ET_Frontness::FRONTNESS_FRONT, ET_Height::HEIGHT_HIGH) },
            { STRESSED_Y, StStressedVowel(ET_Frontness::FRONTNESS_CENTRAL, ET_Height::HEIGHT_HIGH) },
            { STRESSED_U, StStressedVowel(ET_Frontness::FRONTNESS_BACK, ET_Height::HEIGHT_HIGH) }
        };

        map<ET_Sounds, StPretonicVowel> m_mapSoundToPretonicVowel =
        {

            { PRETONIC_A, StPretonicVowel(ET_Frontness::FRONTNESS_CENTRAL, ET_Height::HEIGHT_LOW) },
            { PRETONIC_IE, StPretonicVowel(ET_Frontness::FRONTNESS_FRONT, ET_Height::HEIGHT_HIGH) },
            { PRETONIC_Y, StPretonicVowel(ET_Frontness::FRONTNESS_CENTRAL, ET_Height::HEIGHT_HIGH) },
            { PRETONIC_U, StPretonicVowel(ET_Frontness::FRONTNESS_BACK, ET_Height::HEIGHT_HIGH) }
        };

        map<ET_Sounds, StUnstressedVowel> m_mapSoundToUnstressedVowel =
        {
            { SCHWA, StUnstressedVowel(ET_Frontness::FRONTNESS_CENTRAL, ET_Height::HEIGHT_MID) },
            { UNSTRESSED_I, StUnstressedVowel(ET_Frontness::FRONTNESS_FRONT, ET_Height::HEIGHT_HIGH) },
            { UNSTRESSED_Y, StUnstressedVowel(ET_Frontness::FRONTNESS_CENTRAL, ET_Height::HEIGHT_HIGH) },
            { UNSTRESSED_U, StUnstressedVowel(ET_Frontness::FRONTNESS_BACK, ET_Height::HEIGHT_HIGH) }
        };

        map<ET_Sounds, StConsonant> m_mapSoundToConsonant =
        {
            { P_HARD, StConsonant(ET_Place::PLACE_BILABIAL, ET_Manner::MANNER_STOP, ET_Voicedness::VOICEDNESS_VOICELESS, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NON_NASAL) },
            { P_SOFT, StConsonant(ET_Place::PLACE_BILABIAL, ET_Manner::MANNER_STOP, ET_Voicedness::VOICEDNESS_VOICELESS, ET_Palatalization::PALATALIZATION_SOFT, ET_Nasality::NASALITY_NON_NASAL) },
            { B_HARD, StConsonant(ET_Place::PLACE_BILABIAL, ET_Manner::MANNER_STOP, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NON_NASAL) },
            { B_SOFT, StConsonant(ET_Place::PLACE_BILABIAL, ET_Manner::MANNER_STOP, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NON_NASAL) },
            { M_HARD, StConsonant(ET_Place::PLACE_BILABIAL, ET_Manner::MANNER_STOP, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NASAL) },
            { M_SOFT, StConsonant(ET_Place::PLACE_BILABIAL, ET_Manner::MANNER_STOP, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_SOFT, ET_Nasality::NASALITY_NASAL) },
            { F_HARD, StConsonant(ET_Place::PLACE_LABIODENTAL, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICELESS, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NON_NASAL) },
            { F_SOFT, StConsonant(ET_Place::PLACE_LABIODENTAL, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICELESS, ET_Palatalization::PALATALIZATION_SOFT, ET_Nasality::NASALITY_NON_NASAL) },
            { V_HARD, StConsonant(ET_Place::PLACE_LABIODENTAL, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NON_NASAL) },
            { V_SOFT, StConsonant(ET_Place::PLACE_LABIODENTAL, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_SOFT, ET_Nasality::NASALITY_NON_NASAL) },
            { N_HARD, StConsonant(ET_Place::PLACE_DENTAL, ET_Manner::MANNER_STOP, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NASAL) },
            { N_SOFT, StConsonant(ET_Place::PLACE_DENTAL, ET_Manner::MANNER_STOP, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_SOFT, ET_Nasality::NASALITY_NASAL) },
            { T_HARD, StConsonant(ET_Place::PLACE_DENTAL, ET_Manner::MANNER_STOP, ET_Voicedness::VOICEDNESS_VOICELESS, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NON_NASAL) },
            { T_SOFT, StConsonant(ET_Place::PLACE_DENTAL, ET_Manner::MANNER_STOP, ET_Voicedness::VOICEDNESS_VOICELESS, ET_Palatalization::PALATALIZATION_SOFT, ET_Nasality::NASALITY_NON_NASAL) },
            { D_HARD, StConsonant(ET_Place::PLACE_DENTAL, ET_Manner::MANNER_STOP, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NON_NASAL) },
            { D_SOFT, StConsonant(ET_Place::PLACE_DENTAL, ET_Manner::MANNER_STOP, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_SOFT, ET_Nasality::NASALITY_NON_NASAL) },
            { TS_HARD, StConsonant(ET_Place::PLACE_DENTAL, ET_Manner::MANNER_STOP, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NON_NASAL) },
            { TS_SOFT, StConsonant(ET_Place::PLACE_DENTAL, ET_Manner::MANNER_STOP, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_SOFT, ET_Nasality::NASALITY_NON_NASAL) },
            { S_HARD, StConsonant(ET_Place::PLACE_DENTAL, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICELESS, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NON_NASAL) },
            { S_SOFT, StConsonant(ET_Place::PLACE_DENTAL, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICELESS, ET_Palatalization::PALATALIZATION_SOFT, ET_Nasality::NASALITY_NON_NASAL) },
            { Z_HARD, StConsonant(ET_Place::PLACE_DENTAL, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NON_NASAL) },
            { Z_SOFT, StConsonant(ET_Place::PLACE_DENTAL, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_SOFT, ET_Nasality::NASALITY_NON_NASAL) },
            { L_HARD, StConsonant(ET_Place::PLACE_DENTAL, ET_Manner::MANNER_APPROXIMANT, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NON_NASAL) },
            { L_SOFT, StConsonant(ET_Place::PLACE_DENTAL, ET_Manner::MANNER_APPROXIMANT, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_SOFT, ET_Nasality::NASALITY_NON_NASAL) },
            { R_HARD, StConsonant(ET_Place::PLACE_DENTAL, ET_Manner::MANNER_TRILL, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NON_NASAL) },                // post-alveolar?
            { R_SOFT, StConsonant(ET_Place::PLACE_DENTAL, ET_Manner::MANNER_TRILL, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_SOFT, ET_Nasality::NASALITY_NON_NASAL) },
            { SH_HARD, StConsonant(ET_Place::PLACE_ALVEOLAR, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICELESS, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NON_NASAL) },
            { SH_SOFT, StConsonant(ET_Place::PLACE_ALVEOLAR, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICELESS, ET_Palatalization::PALATALIZATION_SOFT, ET_Nasality::NASALITY_NON_NASAL) },
            { ZH_HARD, StConsonant(ET_Place::PLACE_ALVEOLAR, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NON_NASAL) },         // post-alveolar?
            { ZH_SOFT, StConsonant(ET_Place::PLACE_ALVEOLAR, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICELESS, ET_Palatalization::PALATALIZATION_SOFT, ET_Nasality::NASALITY_NON_NASAL) },      // palatal?
            { JOD, StConsonant(ET_Place::PLACE_PALATAL, ET_Manner::MANNER_APPROXIMANT, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_SOFT, ET_Nasality::NASALITY_NON_NASAL) },
            { K_HARD, StConsonant(ET_Place::PLACE_VELAR, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NON_NASAL) },         // post-alveolar?
            { K_SOFT, StConsonant(ET_Place::PLACE_VELAR, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICELESS, ET_Palatalization::PALATALIZATION_SOFT, ET_Nasality::NASALITY_NON_NASAL) },      // palatal?
            { G_HARD, StConsonant(ET_Place::PLACE_VELAR, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NON_NASAL) },         // post-alveolar?
            { G_SOFT, StConsonant(ET_Place::PLACE_VELAR, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICELESS, ET_Palatalization::PALATALIZATION_SOFT, ET_Nasality::NASALITY_NON_NASAL) },      // palatal?
            { X_HARD, StConsonant(ET_Place::PLACE_VELAR, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NON_NASAL) },         // post-alveolar?
            { X_SOFT, StConsonant(ET_Place::PLACE_VELAR, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICELESS, ET_Palatalization::PALATALIZATION_SOFT, ET_Nasality::NASALITY_NON_NASAL) },      // palatal?
            { G_FRICATIVE_HARD, StConsonant(ET_Place::PLACE_VELAR, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICED, ET_Palatalization::PALATALIZATION_HARD, ET_Nasality::NASALITY_NON_NASAL) },         // post-alveolar?
            { G_FRICATIVE_SOFT, StConsonant(ET_Place::PLACE_VELAR, ET_Manner::MANNER_FRICATIVE, ET_Voicedness::VOICEDNESS_VOICELESS, ET_Palatalization::PALATALIZATION_SOFT, ET_Nasality::NASALITY_NON_NASAL) },      // palatal?
        };

    };      //  class CTranscriber

}   // namespace Hlib

#endif      //  C_TRANSCRIBER_H_INCLUDED
