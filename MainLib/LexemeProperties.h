#ifndef LEXEMEPROPERTIES_H_INCLUDED
#define LEXEMEPROPERTIES_H_INCLUDED

#include <map>
#include "EString.h"

namespace Hlib
{
    struct StLexemeProperties
    {
        //
        // Stem data as acquired from the DB
        //
        StLexemeProperties() :
            llDescriptorId(-1),
            llHeadwordId(-1),
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
            bOAlternation(false),
            bSecondGenitive(false),
            bSecondLocative(false),
            bIsImpersonal(false),
            bIsIterative(false),
            bHasAspectPair(false),
            iAspectPairType(0),
            iAltAspectPairType(0),
            bHasIrregularForms(false),
            bHasIrregularVariants(false),
            bHasDeficiencies(false),
            bPrimaryInflectionGroup(false),
            iType(-1),
            eAccentType1(AT_UNDEFINED),
            eAccentType2(AT_UNDEFINED),
            bShortFormsRestricted(false),
            bPastParticipleRestricted(false),
            bNoLongForms(false),
            bShortFormsIncomplete(false),
            bNoPastParticiple(false),
            bFleetingVowel(false),
            iStemAugment(-1),
            bInflectedParts(false)
        {
                sSourceForm.SetVowels(g_szRusVowels);
                sGraphicStem.SetVowels(g_szRusVowels);
        }

        long long llDescriptorId;
        long long llHeadwordId;
        CEString sSourceForm;
        CEString sHeadwordComment;
        CEString sHeadwordVariant;
        CEString sHeadwordVariantComment;
        vector<int> vecSourceStressPos, vecSecondaryStressPos;
        vector<int> vecSourceVariantStressPos, vecSecondaryVariantStressPos;
        bool bIsUnstressed;
        vector<int> vecHomonyms;
        CEString sPluralOf;
        CEString sUsage;
        CEString sSeeRef;
        CEString sBackRef;
        CEString sGraphicStem;
        bool bIsVariant;
        CEString sMainSymbol;
        bool bIsPluralOf;
        bool bTransitive;
        ET_Reflexivity eReflexive;
        CEString sMainSymbolPluralOf;
        CEString sAltMainSymbol;
        ET_Aspect eAspect;
        CEString sInflectionType;
        CEString sPartOfSpeech;
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
        bool bIsImpersonal;
        bool bIsIterative;
        bool bHasAspectPair;
        int iAspectPairType;
        int iAltAspectPairType;
        CEString sAspectPairData;
        CEString sAltAspectPairData;
        CEString sAltAspectPairComment;
        CEString sQuestionableForms;
        bool bHasIrregularForms;
        CEString sIrregularFormsLeadComment;
        bool bHasIrregularVariants;    // true if any of irreg. forms alternates with regular
        bool bHasDeficiencies;
        CEString sRestrictedForms;
        CEString sContexts;
        CEString sCognate;
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
        bool bShortFormsIncomplete;
        bool bNoPastParticiple;
        bool bFleetingVowel;
        int iStemAugment;
        bool bInflectedParts;

        //
        // Verb stems & infinitive:
        //
        CEString sInfStem;
        CEString s1SgStem;
        CEString s3SgStem;

        CEString sInfinitive;

        map<int, bool> mapCommonDeviations;
    };

}       //  namespace Hlib

#endif  //  LEXEMEPROPERTIES_H_INCLUDED
