#ifndef I_LEXEME_H_INCLUDED
#define I_LEXEME_H_INCLUDED

#include "Enums.h"
#include "LexemeProperties.h"
#include "IWordForm.h"
#include "IAspectPair.h"

namespace Hlib
{
    struct IWordForm;
    class CDictionary;

    struct ILexeme
    {
        virtual ~ILexeme() {};
        virtual __int64 llLexemeId() = 0;
        virtual __int64 llHeadwordId() = 0;
        virtual void SetDictionary(CDictionary *) = 0;
        virtual const StLexemeProperties& stGetProperties() = 0;
        virtual StLexemeProperties& stGetPropertiesForWriteAccess() = 0;
        virtual ET_Gender eInflectionTypeToGender() = 0;
        virtual ET_Gender eMainSymbolToGender() = 0;
        virtual ET_Animacy eInflectionTypeToAnimacy(bool) = 0;
        virtual ET_Animacy eMainSymbolToAnimacy() = 0;
        virtual ET_MainSymbol eMainSymbolToEnum(const CEString&) = 0;
        virtual CEString sGraphicStem() = 0;
        virtual void SetGraphicStem (const CEString&) = 0;
        virtual long long llSecondPartId() = 0;
        virtual void SetSecondPartId(long long) = 0;
        virtual bool bHasIrregularForms() = 0;
        virtual void SetHasIrregularForms(bool) = 0;
        virtual bool bHasSecondaryStress() = 0;
        virtual bool bHasFleetingVowel() = 0;
        virtual void SetHasFleetingVowel(bool) = 0;
        virtual bool bHasYoAlternation() = 0;
        virtual void SetHasYoAlternation(bool) = 0;
        virtual bool bHasOAlternation() = 0;
        virtual void SetHasOAlternation(bool) = 0;
        virtual CEString sSourceForm() = 0;
        virtual void SetSourceForm(const CEString&) = 0;
        virtual CEString sHeadwordComment() = 0;
        virtual void SetHeadwordComment(const CEString&) = 0;
        virtual CEString sHeadwordVariant() = 0;
        virtual void SetHeadwordVariant(const CEString&) = 0;
        virtual CEString sHeadwordVariantComment() = 0;
        virtual void SetHeadwordVariantComment(const CEString&) = 0;
        virtual CEString sPluralOf() = 0;
        virtual void SetPluralOf(const CEString&) = 0;
        virtual CEString sUsage() = 0;
        virtual void SetUsage(const CEString&) = 0;
        virtual CEString sSeeRef() = 0;
        virtual void SetSeeRef(const CEString&) = 0;
        virtual CEString sBackRef() = 0;
        virtual void SetBackRef(const CEString&) = 0;
        virtual bool bIsUnstressed() = 0;
        virtual void SetIsUnstressed(bool) = 0;
        virtual bool bIsVariant() = 0;
        virtual void SetIsVariant(bool) = 0;
        virtual CEString sMainSymbol() = 0;
        virtual void SetMainSymbol(const CEString&) = 0;
        virtual bool bIsPluralOf() = 0;
        virtual void SetIsPluralOf(bool) = 0;
        virtual bool bTransitive() = 0;
        virtual void SetTransitive(bool) = 0;
        virtual ET_Reflexivity eIsReflexive() = 0;
        virtual void SetIsReflexive(ET_Reflexivity) = 0;
        virtual CEString sMainSymbolPluralOf() = 0;
        virtual void SetMainSymbolPluralOf(const CEString&) = 0;
        virtual CEString sAltMainSymbol() = 0;
        virtual void SetAltMainSymbol(const CEString&) = 0;
        virtual ET_Aspect eAspect() = 0;
        virtual void SetAspect(ET_Aspect) = 0;
        virtual CEString sInflectionType() = 0;
        virtual void SetInflectionType(const CEString&) = 0;
        virtual ET_PartOfSpeech ePartOfSpeech() = 0;
        virtual void SetPartOfSpeech(ET_PartOfSpeech) = 0;
        virtual CEString sComment() = 0;
        virtual void SetComment(const CEString&) = 0;
        virtual CEString sAltMainSymbolComment() = 0;
        virtual void SetAltMainSymbolComment(const CEString&) = 0;
        virtual CEString sAltInflectionComment() = 0;
        virtual void SetAltInflectionComment(const CEString&) = 0;
        virtual CEString sVerbStemAlternation() = 0;
        virtual void SetVerbStemAlternation(const CEString&) = 0;
        virtual bool bPartPastPassZhd() = 0;
        virtual void SetPartPastPassZhd(bool) = 0;
        virtual int iSection() = 0;
        virtual void SetSection(int) = 0;
        virtual bool bNoComparative() = 0;
        virtual void SetNoComparative(bool) = 0;
        virtual bool bAssumedForms() = 0;
        virtual void SetAssumedForms(bool) = 0;
//        virtual bool bYoAlternation() = 0;
//        virtual void SetYoAlternation(bool) = 0;
//        virtual bool bOAlternation() = 0;
//        virtual void SetOAlternation(bool) = 0;
        virtual bool bSecondGenitive() = 0;
        virtual void SetSecondGenitive(bool) = 0;
        virtual bool bSecondPrepositional() = 0;
        virtual void SetSecondPrepositional(bool) = 0;
        virtual bool bSecondPrepositionalOptional() = 0;
        virtual void SetSecondPrepositionalOptional(bool) = 0;
        virtual CEString sP2Preposition() = 0;
        virtual void SetP2Preposition(const CEString&) = 0;
        virtual bool bHasAspectPair() = 0;
        virtual void SetHasAspectPair(bool) = 0;
        virtual bool bHasAltAspectPair() = 0;
        virtual int iAspectPairType() = 0;
        virtual void SetAspectPairType(int) = 0;
        virtual int iAltAspectPairType() = 0;
        virtual void SetAltAspectPairType(int) = 0;
        virtual void SetAspectPairData(const CEString&) = 0;
        virtual CEString sAspectPairData() = 0;
        virtual void SetAltAspectPairData(const CEString&) = 0;
        virtual CEString sAltAspectPairData() = 0;
        virtual CEString sAltAspectPairComment() = 0;
        virtual void SetAltAspectPairComment(const CEString&) = 0;
        virtual ET_ReturnCode eGetAspectPair(CEString&, int&) = 0;
//        virtual void SetAspectPair(CEString&, int&) = 0;
        virtual ET_ReturnCode eGetAltAspectPair(CEString&, int&) = 0;
//        virtual void SetAltAspectPair(CEString&, int&) = 0;
        virtual bool bHasIrregularVariants() = 0;
        virtual void SetHasIrregularVariants(bool) = 0;
        virtual bool bHasDifficultForms() = 0;
        virtual void SetHasDifficultForms(bool) = 0;
        virtual void AddDifficultForm(const CEString& sGramHash) = 0;
        virtual bool bHasMissingForms() = 0;
        virtual void SetHasMissingForms(bool) = 0;
        virtual void AddMissingForm(const CEString& sGramHash) = 0;
        virtual CEString sRestrictedContexts() = 0;
        virtual void SetRestrictedContexts(const CEString&) = 0;
        virtual CEString sContexts() = 0;
        virtual void SetContexts(const CEString&) = 0;
        virtual CEString sTrailingComment() = 0;
        virtual void SetTrailingComment(const CEString&) = 0;
        virtual long long llInflectionId() = 0;
        virtual void SetInflectionId(int) = 0;
        virtual bool bPrimaryInflectionGroup() = 0;
        virtual void SetPrimaryInflectionGroup(bool) = 0;
        virtual int iType() = 0;
        virtual void SetType(int) = 0;
        virtual ET_AccentType eAccentType1() = 0;
        virtual void SetAccentType1(ET_AccentType) = 0;
        virtual ET_AccentType eAccentType2() = 0;
        virtual void SetAccentType2(ET_AccentType) = 0;
        virtual bool bShortFormsRestricted() = 0;
        virtual void SetShortFormsRestricted(bool) = 0;
        virtual bool bPastParticipleRestricted() = 0;
        virtual void SetPastParticipleRestricted(bool) = 0;
        virtual bool bNoLongForms() = 0;
        virtual void SetNoLongForms(bool) = 0;
        virtual bool bShortFormsIncomplete() = 0;
        virtual void SetShortFormsIncomplete(bool) = 0;
        virtual bool bNoPassivePastParticiple() = 0;
        virtual void SetNoPassivePastParticiple(bool) = 0;
        virtual bool bFleetingVowel() = 0;
        virtual void SetFleetingVowel(bool) = 0;
        virtual int iStemAugment() = 0;
        virtual void SetStemAugment(int) = 0;
        virtual CEString s1SgStem() = 0;
        virtual void Set1SgStem(const CEString&) = 0;
        virtual CEString s3SgStem() = 0;
        virtual void Set3SgStem(const CEString&) = 0;
        virtual CEString sInfinitive() = 0;
        virtual void SetInfinitive(const CEString&) = 0;
        virtual CEString sInfStem() = 0;
        virtual void SetInfStem(const CEString&) = 0;
//        virtual int iInflectedParts() = 0;
//        virtual void SetInflectedParts(int) = 0;
        virtual bool bHasPresPassParticiple() = 0;
        virtual void SetHasPresPassParticiple(bool) = 0;
        virtual bool bSpryazhSm() = 0;
        virtual CEString sSpryazhSmRefSource() = 0;
        virtual ET_Subparadigm eSubparadigm() = 0;
        virtual void SetSubparadigm(ET_Subparadigm) = 0;    // currently only used with numerals

        virtual ET_ReturnCode eAddCommonDeviation(int iValue, bool bIsOptional) = 0;
        virtual void ClearCommonDeviations() = 0;
        virtual bool bFindCommonDeviation(int iNum, bool& bIsOptional) = 0;
        virtual bool bFindStandardAlternation(const CEString& sKey, CEString& sValue) = 0;
        virtual ET_ReturnCode eGetStemStressPositions(const CEString& sLemma, vector<int>& vecPosition) = 0;
        virtual ET_ReturnCode eGetAlternatingPreverb(const CEString& sVerbForm, CEString& sPreverb, bool& bVoicing) = 0;
        virtual CEString sHash() = 0;
        virtual CEString sParadigmHash() = 0;
        virtual ET_ReturnCode eWordFormFromHash(CEString sHash, int iAt, IWordForm *& pWf) = 0;
        virtual ET_ReturnCode eCreateWordForm(IWordForm *&) = 0;
        virtual ET_ReturnCode eRemoveWordForm(CEString sHash, int iAt) = 0;
        virtual ET_ReturnCode eRemoveWordForms(CEString sHash) = 0;
        virtual void AddWordForm(IWordForm* pWordForm) = 0;
        virtual bool bHasIrregularForm(CEString sGramHash) = 0;
        virtual bool bNoRegularForms(CEString sGramHash) = 0;
        virtual ET_ReturnCode eGetFirstWordForm(IWordForm *& pWf) = 0;
        virtual ET_ReturnCode eGetNextWordForm(IWordForm *& pWf) = 0;
        virtual ET_ReturnCode eGetFirstIrregularForm(CEString sHash, IWordForm *&, bool& bIsOptional) = 0;
        virtual ET_ReturnCode eGetFirstIrregularForm(IWordForm *&, bool& bIsOptional) = 0;
        virtual ET_ReturnCode eGetNextIrregularForm(IWordForm *&, bool& bIsOptional) = 0;
        virtual int iFormCount(CEString sHash) = 0;
        virtual bool bHasCommonDeviation(int iCd) = 0;
        virtual bool bDeviationOptional(int iCd) = 0;
        virtual ET_ReturnCode eFormExists(const CEString& hash) = 0;
        virtual ET_ReturnCode eSetFormExists(const CEString& hash, bool bExists) = 0;
        virtual ET_ReturnCode eIsFormDifficult(const CEString& sGramHash) = 0;
        virtual ET_ReturnCode eSetFormDifficult(const CEString& hash, bool bIsDifficult) = 0;
        virtual ET_ReturnCode eDifficultFormsHashes(vector<CEString>&) = 0;
        virtual ET_ReturnCode eIsFormAssumed(const CEString& sGramHash) = 0;
        virtual ET_ReturnCode eSetHasAssumedForms(bool bIsAssumed) = 0;
        virtual bool bIsMultistressedCompound() = 0;
        virtual ET_ReturnCode eGetSourceFormWithStress(CEString& sSourceForm, bool bIsVariant=false) = 0;
        virtual ET_ReturnCode eGetFirstStemStressPos(int& iPos) = 0;
        virtual ET_ReturnCode eGetNextStemStressPos(int& iPos) = 0;
        virtual ET_ReturnCode eGetFirstSecondaryStemStressPos(int& iPos) = 0;
        virtual ET_ReturnCode eGetNextSecondaryStemStressPos(int& iPos) = 0;
        virtual ET_ReturnCode eSetDb(const CEString& sDbPath) = 0;
        virtual ET_ReturnCode eGenerateParadigm() = 0;
        virtual ET_ReturnCode eSaveTestData() = 0;
        virtual ET_ReturnCode eClone(ILexeme *&) = 0;
//        virtual ET_ReturnCode eGetAspectPairs(IAspectPair *&) = 0;
        virtual ET_ReturnCode eCheckLexemeProperties() = 0;
        virtual ET_ReturnCode eDeleteIrregularForm(const CEString& sFormHash) = 0;
        virtual ET_ReturnCode eSaveIrregularForm(const CEString& sFormHash, IWordForm *&) = 0;
        virtual ET_ReturnCode eSaveIrregularForms(const CEString& sGramHash) = 0;
        virtual ET_ReturnCode eMakeGraphicStem() = 0;
        virtual ET_ReturnCode eMakeGraphicStem(const CEString& sSource, CEString& sGraphicStem) = 0;
        virtual ET_ReturnCode eGetErrorMsg(CEString& sErrorMsg) = 0;

    };      //  struct ILexeme

}       //  namespace Hlib

#endif  //  I_LEXEME_H_INCLUDED
