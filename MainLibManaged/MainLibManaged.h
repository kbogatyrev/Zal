// MainLibManaged.h

#pragma once

#include "IDictionary.h"
#include "ILexeme.h"
#include "IWordForm.h"
#include "Gramhasher.h"
#include "IParser.h"
#include "IVerifier.h"
#include "IAspectPair.h"
#include "..\Hlib-Windows\SqliteWrapper.h"
#include "EnumsManaged.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace Hlib;

namespace MainLibManaged 
{
    public delegate void DelegateProgress(int iPercentDone, bool bOperationComplete);

    ref class CLexemeManaged;
    ref class CParserManaged;
    ref class CVerifierManaged;

    public ref class CWordFormManaged
    {
        IWordForm * m_pWordForm;

    public:
        CWordFormManaged(IWordForm *);
        ~CWordFormManaged();

        CLexemeManaged^ Lexeme();
        String^ sWordForm();
        String^ sStem();
        __int64 llLexemeId();
        EM_PartOfSpeech ePos();
        EM_Case eCase();
        EM_Number eNumber();
        EM_Subparadigm eSubparadigm();
        EM_Gender eGender();
        EM_Person ePerson();
        EM_Animacy eAnimacy();
        EM_Reflexive eReflexive();
        EM_Aspect eAspect();
        EM_Status eStatus();
        bool bIrregular();      // came from the DB as opposed to being generated by the app
        EM_ReturnCode eGetFirstStressPos(int% iPos, EM_StressType% eType);
        EM_ReturnCode eGetNextStressPos(int% iPos, EM_StressType% eType);

        String^ sGramHash();
        EM_ReturnCode eInitFromHash(String^ sHash);
    };

    public ref class CLexemeManaged
    {
    public:
        ILexeme * m_pLexeme;

        CLexemeManaged();
        CLexemeManaged(ILexeme *);
        CLexemeManaged(String^ sGramHash);
        ~CLexemeManaged();

//        const StLexemeProperties& stGetProperties();
//        StLexemeProperties& stGetPropertiesForWriteAccess();
        __int64 llLexemeId();
        EM_Gender eGender();
//        EM_Animacy eAnimacy();
        String^ sGraphicStem();
        void sSetGraphicStem(String^);
        bool bHasIrregularForms();
        void SetHasIrregularForms(bool);
        bool bHasSecondaryStress();
        bool bHasFleetingVowel();
        void SetHasFleetingVowel(bool);
        bool bHasYoAlternation();
        void SetHasYoAlternation(bool);
        bool bHasOAlternation();
        void SetHasOAlternation(bool);
        String^ sSourceForm();
        void SetSourceForm(String^);
        bool bHasHomonyms();
        List<int>^ arrHomonyms();
        void SetHomonyms(List<int>^);
        String^ sHeadwordComment();
        void SetHeadwordComment(String^);
        String^ sHeadwordVariant();
        void SetHeadwordVariant(String^);
        String^ sHeadwordVariantComment();
        void SetHeadwordVariantComment(String^);
        String^ sPluralOf();
        void SetPluralOf(String^);
        String^ sUsage();
        void SetUsage(String^);
        String^ sSeeRef();
        void SetSeeRef(String^);
        String^ sBackRef();
        void SetBackRef(String^);
        bool bIsUnstressed();
        void SetIsUnstressed(bool);
        bool bIsVariant();
        void SetIsVariant(bool);
        String^ sMainSymbol();
        void SetMainSymbol(String^);
        bool bIsPluralOf();
        void SetIsPluralOf(bool);
        bool bTransitive();
        void SetTransitive(bool);
        EM_Reflexive eIsReflexive();
        void SetIsReflexive(EM_Reflexive);
        String^ sMainSymbolPluralOf();
        void SetMainSymbolPluralOf(String^);
        String^ sAltMainSymbol();
        void SetAltMainSymbol(String^);
        EM_Aspect eAspect();
        void SetAspect(EM_Aspect);
        String^ sInflectionType();
        void SetInflectionType(String^);
        EM_PartOfSpeech ePartOfSpeech();
        void SetPartOfSpeech(EM_PartOfSpeech);
        String^ sComment();
        void SetComment(String^);
        String^ sAltMainSymbolComment();
        void SetAltMainSymbolComment(String^);
        String^ sAltInflectionComment();
        void SetAltInflectionComment(String^);
        String^ sVerbStemAlternation();
        void SetVerbStemAlternation(String^);
        bool bPartPastPassZhd();
        void SetPartPastPassZhd(bool);
        int iSection();
        void SetSection(int);
        bool bNoComparative();
        void SetNoComparative(bool);
        bool bAssumedForms();
        void SetAssumedForms(bool);
//        bool bYoAlternation();
//        void SetYoAlternation(bool);
//        bool bOAlternation();
//        void SetHasOAlternation(bool);
        bool bSecondGenitive();
        void SetSecondGenitive(bool);
        bool bSecondLocative();
        void SetSecondLocative(bool);
        bool bSecondLocativeOptional();
        void SetSecondLocativeOptional(bool);
        String^ sLoc2Preposition();
        void SetLoc2Preposition(String^);
        bool bHasAspectPair();
        void SetHasAspectPair(bool);
        bool bHasAltAspectPair();
        int iAspectPairType();
        void SetAspectPairType(int);
        int iAltAspectPairType();
        void SetAltAspectPairType(int);
        String^ sAltAspectPairComment();
        void SetAltAspectPairComment(String^);
        EM_ReturnCode eGetAspectPair(String^% sAspectPair, int% iStressPos);
        EM_ReturnCode eGetAltAspectPair(String^% sAltAspectPair, int% iAltStressPos);
        String^ sQuestionableForms();
        void SetQuestionableForms(String^);
        bool bHasIrregularVariants();
        void SetHasIrregularVariants(bool);
        bool bHasDeficiencies();
        void SetHasDeficiencies(bool);
        String^ sRestrictedForms();
        void SetRestrictedForms(String^);
        String^ sContexts();
        void SetContexts(String^);
        String^ sTrailingComment();
        void SetTrailingComment(String^);
        int iInflectionId();
        void SetInflectionId(int);
        bool bPrimaryInflectionGroup();
        void SetPrimaryInflectionGroup(bool);
        int iType();
        void SetType(int);
        EM_AccentType eAccentType1();
        void SetAccentType1(EM_AccentType);
        EM_AccentType eAccentType2();
        void SetAccentType2(EM_AccentType);
        bool bShortFormsRestricted();
        void SetShortFormsRestricted(bool);
        bool bPastParticipleRestricted();
        void SetPastParticipleRestricted(bool);
        bool bNoLongForms();
        void SetNoLongForms(bool);
        bool bShortFormsIncomplete();
        void SetShortFormsIncomplete(bool);
        bool bNoPastParticiple();
        void SetNoPastParticiple(bool);
        bool bFleetingVowel();
        void SetFleetingVowel(bool);
        bool iStemAugment();
        void SetStemAugment(int);
        String^ s1SgStem();
        void Set1SgStem(String^);
        String^ s3SgStem();
        void Set3SgStem(String^);
        String^ sInfinitive();
        void SetInfinitive(String^);
        String^ sInfStem();
        void SetInfStem(String^);

        bool bFindCommonDeviation(int iNum, bool% bIsOptional);
        bool bFindStandardAlternation(String^ sKey, String^% sValue);
        EM_ReturnCode eGetStemStressPositions(String^ sStem, List<int>% listPositions);
        EM_ReturnCode eGetAlternatingPreverb(String^ sVerbForm, String^% sPreverb, bool% bVoicing);
        String^ sHash();
        EM_ReturnCode eWordFormFromHash(String^ sHash, int iAt, CWordFormManaged^% Wf);
        bool bHasIrregularForm(String^ sGramHash);
        bool bNoRegularForms(String^ sGramHash);
        EM_ReturnCode eGetFirstWordForm(CWordFormManaged^% wf);
        EM_ReturnCode eGetNextWordForm(CWordFormManaged^% wf);
        EM_ReturnCode eGetFirstIrregularForm(String^ sHash, CWordFormManaged^%, bool% bIsOptional);
        EM_ReturnCode eGetNextIrregularForm(CWordFormManaged^%, bool% bIsOptional);
        int iFormCount(String^ sHash);
        bool bHasCommonDeviation(int iCd);
        bool bDeviationOptional(int iCd);
        bool bIsFormMissing(String^ sGramHash);
        bool bIsMultistressedCompound();
        EM_ReturnCode eGetSourceFormWithStress(String^% sSourceForm, bool bIsVariant);
        EM_ReturnCode eGetFirstStemStressPos(int% iPos);
        EM_ReturnCode eGetNextStemStressPos(int% iPos);
        EM_ReturnCode eGetFirstSecondaryStemStressPos(int% iPos);
        EM_ReturnCode eGetNextSecondaryStemStressPos(int% iPos);

//        EM_ReturnCode eSetDb(String^ sDbPath);
        EM_ReturnCode eGenerateParadigm();
//        EM_ReturnCode eBuildAspectPair();
        EM_ReturnCode eSaveTestData();
        EM_ReturnCode eCheckLexemeProperties(); // for manual input/editing
        EM_ReturnCode eGetErrorMsg(String^% sErrorMsg);

    protected:
//        EM_ReturnCode eGetAspectPair(String^%, int%, bool bIsVariant);

    };  //  public ref class CLexemeManaged

    public ref class CDictionaryManaged
    {
        Hlib::IDictionary * m_pDictionary;  // we are using some non-itf methods

    public:
        CDictionaryManaged();
        ~CDictionaryManaged();

        EM_ReturnCode eSetDbPath(String^ sDbPath);
        String^ sGetDbPath();
        CLexemeManaged^ CreateLexeme();

        EM_ReturnCode eGetLexemeById(int Id);
        EM_ReturnCode eGetLexemesByHash(String^);
        EM_ReturnCode eGetLexemesByGraphicStem(String^);
        EM_ReturnCode eGetLexemesByInitialForm(String^);
        EM_ReturnCode eGenerateAllForms();
        EM_ReturnCode eGenerateFormsForSelectedLexemes();
        EM_ReturnCode eCountLexemes(Int64% iLexemes);

//        EM_ReturnCode eVerifyLexemeProperties(CLexemeManaged^);
//        EM_ReturnCode eSourceFormExists(CLexemeManaged^, bool%);

        EM_ReturnCode eSaveLexeme(CLexemeManaged^);

        EM_ReturnCode eGetFirstLexeme(CLexemeManaged^% pLexeme);
        EM_ReturnCode eGetNextLexeme(CLexemeManaged^% pLexeme);

//        EM_ReturnCode eAnalyze(String^ sText);

//        EM_ReturnCode eGetFirstWordForm(CWordFormManaged^% pWordFrom);
//        EM_ReturnCode eGetNextWordForm(CWordFormManaged^% pWordForm);

        void Clear();
        EM_ReturnCode Clear(CLexemeManaged^ pLexeme);

        EM_ReturnCode eGetParser(CParserManaged^%); 
        EM_ReturnCode eGetVerifier(CVerifierManaged^%);
        EM_ReturnCode eExportTestData(String^ sPath, DelegateProgress^);
        EM_ReturnCode eImportTestData(String^ sPath, DelegateProgress^);
    };

    public ref class CGramHasherManaged
    {
        Hlib::CGramHasher * m_pHasher;

    public:
        CGramHasherManaged();
        ~CGramHasherManaged();

        void SetPartOfSpeech(EM_PartOfSpeech);
        void SetSubparadigm(EM_Subparadigm);
        void SetCase(EM_Case);
        void SetNumber(EM_Number);
        void SetGender(EM_Gender);
        void SetPerson(EM_Person);
        void SetAnimacy(EM_Animacy);
        void SetReflexivity(EM_Reflexive);
        void SetAspect(EM_Aspect);
        String^ sHash();
    };

    public ref class CParserManaged
    {
        Hlib::IParser * m_pParser;

    public:
        CParserManaged(IParser *);
        ~CParserManaged();

        EM_ReturnCode eAnalyze(String^ sText);
        EM_ReturnCode eGetFirstWordForm(CWordFormManaged^% pWordFrom);
        EM_ReturnCode eGetNextWordForm(CWordFormManaged^% pWordFrom);
    };

    public ref class CVerifierManaged
    {
        Hlib::IVerifier * m_pVerifier;

    public:
        CVerifierManaged(IVerifier *);
        ~CVerifierManaged();

        EM_ReturnCode eVerify(String^ sLexemeHash);
        EM_TestResult eResult();

        int iCount();

        EM_ReturnCode eLoadStoredLexemes();
        EM_ReturnCode eDeleteStoredLexeme(String^);

        EM_ReturnCode eGetFirstLexemeData(String^% sLexemeHash, String^% sHeadword);
        EM_ReturnCode eGetNextLexemeData(String^% sLexemeHash, String^% sHeadword);
    };

}       //  namespace MainLibManaged
