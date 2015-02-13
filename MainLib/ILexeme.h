#ifndef I_LEXEME_H_INCLUDED
#define I_LEXEME_H_INCLUDED

#include "Enums.h"
#include "LexemeProperties.h"

namespace Hlib
{
    struct IWordForm;

    struct ILexeme
    {
        virtual ~ILexeme() {};
        virtual const StLexemeProperties& stGetProperties() = 0;
        virtual StLexemeProperties& stGetPropertiesForWriteAccess() = 0;
        virtual ET_Gender eGender() = 0;
        virtual ET_Animacy eAnimacy() = 0;
        virtual CEString sGraphicStem() = 0;
        virtual bool bHasIrregularForms() = 0;
        virtual bool bHasSecondaryStress() = 0;
        virtual bool bHasFleetingVowel() = 0;
        virtual bool bHasYoAlternation() = 0;
        virtual bool bHasOAlternation() = 0;
        virtual CEString sSourceForm() = 0;
        //    virtual CSqlite * pGetDb() = 0;
        virtual CEString sHeadwordComment() = 0;
        virtual bool bIsUnstressed() = 0;
        virtual bool bIsVariant() = 0;
        virtual CEString sMainSymbol() = 0;
        virtual bool bIsPluralOf() = 0;
        virtual bool bTransitive() = 0;
        virtual ET_Reflexive eIsReflexive() = 0;
        virtual CEString sMainSymbolPluralOf() = 0;
        virtual CEString sAltMainSymbol() = 0;
        virtual ET_Aspect eAspect() = 0;
        virtual CEString sInflectionType() = 0;
        virtual ET_PartOfSpeech ePartOfSpeech() = 0;
        virtual CEString sComment() = 0;
        virtual CEString sAltMainSymbolComment() = 0;
        virtual CEString sAltInflectionComment() = 0;
        virtual CEString sVerbStemAlternation() = 0;
        virtual bool bPartPastPassZhd() = 0;
        virtual int iSection() = 0;
        virtual bool bNoComparative() = 0;
        virtual bool bAssumedForms() = 0;
        virtual bool bYoAlternation() = 0;
        virtual bool bOAlternation() = 0;
        virtual bool bSecondGenitive() = 0;
        virtual bool bSecondLocative() = 0;
        virtual bool bSecondLocativeOptional() = 0;
        virtual CEString sLoc2Preposition() = 0;
        virtual bool bHasAspectPair() = 0;
        virtual int iAspectPairType() = 0;
        virtual CEString sAspectPairComment() = 0;
        virtual CEString sQuestionableForms() = 0;
        virtual bool bHasIrregularVariants() = 0;
        virtual bool bHasDeficiencies() = 0;
        virtual CEString sRestrictedForms() = 0;
        virtual CEString sContexts() = 0;
        virtual CEString sTrailingComment() = 0;
        virtual int iInflectionId() = 0;
        virtual bool bPrimaryInflectionGroup() = 0;
        virtual int iType() = 0;
        virtual ET_AccentType eAccentType1() = 0;
        virtual ET_AccentType eAccentType2() = 0;
        bool virtual bShortFormsRestricted() = 0;
        virtual bool bPastParticipleRestricted() = 0;
        virtual bool bNoLongForms() = 0;
        virtual bool bNoShortForms() = 0;
        virtual bool bNoPastParticiple() = 0;
        virtual bool bFleetingVowel() = 0;
        virtual int iStemAugment() = 0;
        virtual CEString s1SgStem() = 0;
        virtual CEString s3SgStem() = 0;
        virtual CEString sInfinitive() = 0;
        virtual CEString sInfStem() = 0;
        virtual bool bFindCommonDeviation(int iNum, bool& bIsOptional) = 0;
        virtual bool bFindStandardAlternation(const CEString& sKey, CEString& sValue) = 0;
        virtual ET_ReturnCode eGetStemStressPositions(const CEString& sLemma, vector<int>& vecPosition) = 0;
        virtual ET_ReturnCode eGetAlternatingPreverb(const CEString& sVerbForm, CEString& sPreverb, bool& bVoicing) = 0;
        virtual CEString sHash() = 0;
        virtual ET_ReturnCode eWordFormFromHash(int iHash, unsigned int uiAt, IWordForm *& pWf) = 0;
        virtual bool bHasIrregularForm(int iGramHash) = 0;
        virtual bool bNoRegularForms(int iGramHash) = 0;
        virtual ET_ReturnCode eGetFirstWordForm(IWordForm *& pWf) = 0;
        virtual ET_ReturnCode eGetNextWordForm(IWordForm *& pWf) = 0;
        virtual ET_ReturnCode eGetFirstIrregularForm(int iHash, IWordForm *&, bool& bIsOptional) = 0;
        virtual ET_ReturnCode eGetFirstIrregularForm(IWordForm *&, bool& bIsOptional) = 0;
        virtual ET_ReturnCode eGetNextIrregularForm(IWordForm *&, bool& bIsOptional) = 0;
        virtual unsigned int uiFormCount(int iHash) = 0;
        virtual bool bHasCommonDeviation(int iCd) = 0;
        virtual bool bDeviationOptional(int iCd) = 0;
        virtual bool bIsFormMissing(int iGramHash) = 0;
        virtual bool bIsMultistressedCompound() = 0;
        virtual ET_ReturnCode eGetFirstStemStressPos(int& iPos) = 0;
        virtual ET_ReturnCode eGetNextStemStressPos(int& iPos) = 0;
        virtual ET_ReturnCode eSetDb(const CEString& sDbPath) = 0;
        virtual ET_ReturnCode eGenerateParadigm() = 0;
        virtual ET_ReturnCode eSaveTestData() = 0;
        virtual ET_ReturnCode eClone(ILexeme *&) = 0;

    };      //  struct ILexeme

}       //  namespace Hlib

#endif  //  I_LEXEME_H_INCLUDED
