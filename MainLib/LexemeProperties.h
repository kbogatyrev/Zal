#ifndef LEXEMEPROPERTIES_H_INCLUDED
#define LEXEMEPROPERTIES_H_INCLUDED

struct StLexemeProperties
{
    //
    // Stem data as acquired from the DB
    //
    StLexemeProperties() :
        iDbKey(-1),
        bIsUnstressed(false),
        bIsVariant(false),
        bIsPluralOf(false),
        bTransitive(false),
        eReflexive(REFL_UNDEFINED),
        ePartOfSpeech(POS_UNDEFINED),
        iSection(-1),
        bPartPastPassZhd(false),
        bNoComparative(false),
        bAssumedForms(false),
        bYoAlternation(false),
        bSecondGenitive(false),
        bHasAspectPair(false),
        iAspectPairType(-1),
        bHasIrregularForms(false),
        bHasIrregularVariants(false),
        bPrimaryInflectionGroup(false),
        iType(-1),
        eAccentType1(AT_UNDEFINED),
        eAccentType2(AT_UNDEFINED),
        bShortFormsRestricted(false),
        bPastParticipleRestricted(false),
        bNoLongForms(false),
        bNoShortForms(false),
        bNoPastParticiple(false),
        bFleetingVowel(false),
        iStemAugment(-1)
        {
            sSourceForm.SetVowels (g_szVowels);
            sGraphicStem.SetVowels (g_szVowels);
        }

    int iDbKey;
    CEString sSourceForm;
    CEString sHeadwordComment;
    vector<int> vecSourceStressPos, vecSecondaryStressPos;
    bool bIsUnstressed;
    vector<int> vecHomonyms;
    CEString sGraphicStem;
    bool bIsVariant;
    CEString sMainSymbol;
    bool bIsPluralOf;
    bool bTransitive;
    ET_Reflexive eReflexive;
    CEString sMainSymbolPluralOf;
    CEString sAltMainSymbol;
    ET_Aspect eAspect;
    CEString sInflectionType;
    ET_PartOfSpeech ePartOfSpeech;
    CEString sComment;
    CEString sAltMainSymbolComment;
    CEString sAltInflectionComment;
    CEString sVerbStemAlternation;
    bool bPartPastPassZhd;
    int iSection;
    bool bNoComparative;
    bool bAssumedForms;
    bool bYoAlternation;
    bool bOAlternation;
    bool bSecondGenitive;
    bool bSecondLocative;
    bool bSecondLocativeOptional;
    CEString sLoc2Preposition;
    bool bHasAspectPair;
    int iAspectPairType;
    CEString sAspectPairComment;
    CEString sQuestionableForms;
    bool bHasIrregularForms;
    bool bHasIrregularVariants;    // true if any of irreg. forms alternates with regular
    bool bHasDeficiencies;
    CEString sRestrictedFroms;
    CEString sContexts;
    CEString sTrailingComment;

    // From inflection table:
    int iInflectionId;
    bool bPrimaryInflectionGroup;
    int iType;
    ET_AccentType eAccentType1;
    ET_AccentType eAccentType2;
    bool bShortFormsRestricted;
    bool bPastParticipleRestricted;
    bool bNoLongForms;
    bool bNoShortForms;
    bool bNoPastParticiple;
    bool bFleetingVowel;
    int iStemAugment;

    //
    // Verb stems & infinitive:
    //
    CEString sInfStem;
    CEString s1SgStem;
    CEString s3SgStem;

    CEString sInfinitive;

    map<int, bool> mapCommonDeviations;
};

#endif  //  LEXEMEPROPERTIES_H_INCLUDED
