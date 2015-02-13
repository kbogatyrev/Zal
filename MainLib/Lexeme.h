#ifndef C_LEXEME_H_INCLUDED
#define C_LEXEME_H_INCLUDED

#include "ILexeme.h"
#include "LexemeProperties.h"

using namespace std;
using namespace std::tr1;

namespace Hlib
{
    struct CWordForm;
    class CDictionary;

    enum ET_MainSymbol
    {
        MS_START,
        MS_M = MS_START,
        MS_MO,
        MS_ZH,
        MS_ZHO,
        MS_S,
        MS_SO,
        MS_MO_ZHO,
        MS_MN,
        MS_MN_NEOD,
        MS_MN_ODUSH,
        MS_MN_OT,
        MS_P,
        MS_MS,
        MS_MS_P,
        MS_CHISL,
        MS_CHISL_P,
        MS_SV,
        MS_NSV,
        MS_SV_NSV,
        MS_N,
        MS_PREDL,
        MS_SOJUZ,
        MS_PREDIK,
        MS_VVODN,
        MS_SRAVN,
        MS_CHAST,
        MS_MEZHD,
        MS_END,
        MS_UNDEFINED
    };

    static void operator++ (ET_MainSymbol& eMs)
    {
        eMs = (ET_MainSymbol)(eMs + 1);
    }

    struct StIrregularForm
    {
        CWordForm * pWordForm;
        bool bIsOptional;

        StIrregularForm(CWordForm * pWf, bool bOpt) : pWordForm(pWf), bIsOptional(bOpt)
        {}
    };

    class CLexeme : public ILexeme
    {

    public:
        CLexeme(CDictionary *);
        CLexeme(const CLexeme&);
        ~CLexeme();

        virtual const StLexemeProperties& stGetProperties()
        {
            return m_stProperties;
        }

        virtual StLexemeProperties& stGetPropertiesForWriteAccess()
        {
            return m_stProperties;
        }

        virtual ET_Gender eGender()
        {
            return m_mapMainSymbolToGender[m_stProperties.sInflectionType];
        }

        virtual ET_Animacy eAnimacy()
        {
            return m_mapMainSymbolToAnimacy[m_stProperties.sInflectionType];
        }

        virtual CEString sGraphicStem()
        {
            return m_stProperties.sGraphicStem;
        }

        virtual bool bHasIrregularForms()
        {
            return m_stProperties.bHasIrregularForms;
        }

        virtual bool bHasSecondaryStress()
        {
            return !m_stProperties.vecSecondaryStressPos.empty();
        }

        virtual bool bHasFleetingVowel()
        {
            return m_stProperties.bFleetingVowel;
        }

        virtual bool bHasYoAlternation()
        {
            return m_stProperties.bYoAlternation;
        }

        virtual bool bHasOAlternation()
        {
            return m_stProperties.bOAlternation;
        }

        virtual CEString sSourceForm()
        {
            return m_stProperties.sSourceForm;
        }

        virtual CEString sHeadwordComment()
        {
            return m_stProperties.sHeadwordComment;
        }

        virtual bool bIsUnstressed()
        {
            return m_stProperties.bIsUnstressed;
        }

        virtual bool bIsVariant()
        {
            return m_stProperties.bIsVariant;
        }

        virtual CEString sMainSymbol()
        {
            return m_stProperties.sMainSymbol;
        }

        virtual bool bIsPluralOf()
        {
            return m_stProperties.bIsPluralOf;
        }

        virtual bool bTransitive()
        {
            return m_stProperties.bTransitive;
        }

        virtual ET_Reflexive eIsReflexive()
        {
            return m_stProperties.eReflexive;
        }

        virtual CEString sMainSymbolPluralOf()
        {
            return m_stProperties.sMainSymbolPluralOf;
        }

        virtual CEString sAltMainSymbol()
        {
            return m_stProperties.sAltMainSymbol;
        }

        virtual ET_Aspect eAspect()
        {
            return m_stProperties.eAspect;
        }

        virtual CEString sInflectionType()
        {
            return m_stProperties.sInflectionType;
        }

        virtual ET_PartOfSpeech ePartOfSpeech()
        {
            return m_stProperties.ePartOfSpeech;
        }

        virtual CEString sComment()
        {
            return m_stProperties.sComment;
        }

        virtual CEString sAltMainSymbolComment()
        {
            return m_stProperties.sAltMainSymbolComment;
        }

        virtual CEString sAltInflectionComment()
        {
            return m_stProperties.sAltInflectionComment;
        }

        virtual CEString sVerbStemAlternation()
        {
            return m_stProperties.sVerbStemAlternation;
        }

        virtual bool bPartPastPassZhd()
        {
            return m_stProperties.bPartPastPassZhd;
        }

        virtual int iSection()
        {
            return m_stProperties.iSection;
        }

        virtual bool bNoComparative()
        {
            return m_stProperties.bNoComparative;
        }

        virtual bool bAssumedForms()
        {
            return m_stProperties.bAssumedForms;
        }

        virtual bool bYoAlternation()
        {
            return m_stProperties.bYoAlternation;
        }

        virtual bool bOAlternation()
        {
            return m_stProperties.bOAlternation;
        }

        virtual bool bSecondGenitive()
        {
            return m_stProperties.bSecondGenitive;
        }

        virtual bool bSecondLocative()
        {
            return m_stProperties.bSecondLocative;
        }

        virtual bool bSecondLocativeOptional()
        {
            return m_stProperties.bSecondLocativeOptional;
        }

        virtual CEString sLoc2Preposition()
        {
            return m_stProperties.sLoc2Preposition;
        }

        virtual bool bHasAspectPair()
        {
            return m_stProperties.bHasAspectPair;
        }

        virtual int iAspectPairType()
        {
            return m_stProperties.iAspectPairType;
        }

        virtual CEString sAspectPairComment()
        {
            return m_stProperties.sAspectPairComment;
        }

        virtual CEString sQuestionableForms()
        {
            return m_stProperties.sQuestionableForms;
        }

        virtual bool bHasIrregularVariants()
        {
            return m_stProperties.bHasIrregularVariants;    // true if any of irreg. forms alternates with regular
        }

        virtual bool bHasDeficiencies()
        {
            return m_stProperties.bHasDeficiencies;
        }

        virtual CEString sRestrictedForms()
        {
            return m_stProperties.sRestrictedForms;
        }

        virtual CEString sContexts()
        {
            return m_stProperties.sContexts;
        }

        virtual CEString sTrailingComment()
        {
            return m_stProperties.sTrailingComment;
        }

        // From inflection table:
        virtual int iInflectionId()
        {
            return m_stProperties.iInflectionId;
        }

        virtual bool bPrimaryInflectionGroup()
        {
            return m_stProperties.bPrimaryInflectionGroup;
        }

        virtual int iType()
        {
            return m_stProperties.iType;
        }

        virtual ET_AccentType eAccentType1()
        {
            return m_stProperties.eAccentType1;
        }

        virtual ET_AccentType eAccentType2()
        {
            return m_stProperties.eAccentType2;
        }

        virtual bool bShortFormsRestricted()
        {
            return m_stProperties.bShortFormsRestricted;
        }

        virtual bool bPastParticipleRestricted()
        {
            return m_stProperties.bPastParticipleRestricted;
        }

        virtual bool bNoLongForms()
        {
            return m_stProperties.bNoLongForms;
        }

        virtual bool bNoShortForms()
        {
            return m_stProperties.bNoShortForms;
        }

        virtual bool bNoPastParticiple()
        {
            return m_stProperties.bNoPastParticiple;
        }

        virtual bool bFleetingVowel()
        {
            return m_stProperties.bFleetingVowel;
        }

        virtual int iStemAugment()
        {
            return m_stProperties.iStemAugment;
        }

        virtual CEString s1SgStem()
        {
            return m_stProperties.s1SgStem;
        }

        virtual CEString s3SgStem()
        {
            return m_stProperties.s3SgStem;
        }

        virtual CEString sInfinitive()
        {
            return m_stProperties.sInfinitive;
        }

        virtual CEString sInfStem()
        {
            return m_stProperties.sInfStem;
        }

        virtual bool bFindCommonDeviation(int iNum, bool& bIsOptional);
        virtual bool bFindStandardAlternation(const CEString& sKey, CEString& sValue);

        virtual ET_ReturnCode eGetFirstStemStressPos(int& iPos);
        virtual ET_ReturnCode eGetNextStemStressPos(int& iPos);

        virtual ET_ReturnCode eGetAlternatingPreverb(const CEString& sVerbForm, CEString& sPreverb, bool& bVoicing);
        virtual CEString sHash();
        virtual ET_ReturnCode eWordFormFromHash(int iHash, unsigned int uiAt, IWordForm *& pWf);
        virtual bool bHasIrregularForm(int iGramHash);
        virtual bool bNoRegularForms(int iGramHash);

        ET_ReturnCode eGetFirstWordForm(IWordForm *& pWf);
        ET_ReturnCode eGetNextWordForm(IWordForm *& pWf);

        virtual ET_ReturnCode eGetFirstIrregularForm(int iHash, IWordForm *&, bool& bIsOptional);
        virtual ET_ReturnCode eGetFirstIrregularForm(IWordForm *&, bool& bIsOptional);
        virtual ET_ReturnCode eGetNextIrregularForm(IWordForm *&, bool& bIsOptional);

        ET_ReturnCode eGetFirstIrregularForm(int iHash, CWordForm *&, bool& bIsOptional);
        ET_ReturnCode eGetFirstIrregularForm(CWordForm *&, bool& bIsOptional);
        ET_ReturnCode eGetNextIrregularForm(CWordForm *&, bool& bIsOptional);

        virtual unsigned int uiFormCount(int iHash);
        virtual bool bHasCommonDeviation(int iCd);
        virtual bool bDeviationOptional(int iCd);
        virtual bool bIsFormMissing(int iGramHash);
        virtual bool bIsMultistressedCompound();
        virtual ET_ReturnCode eSetDb(const CEString& sDbPath);
        virtual ET_ReturnCode eGenerateParadigm();
        virtual ET_ReturnCode eSaveTestData();
        virtual ET_ReturnCode eClone(ILexeme *&);


        // Non-virtual public functions (not available to external consumers)
    public:
        CSqlite * pGetDb();
        ET_ReturnCode eWordFormFromHash(int iHash, unsigned int uiAt, CWordForm *& pWf);
        ET_ReturnCode eGetStemStressPositions(const CEString& sLemma, vector<int>& vecPosition);
        ET_ReturnCode eGetIrregularForms(int iHash, map<CWordForm *, bool>& mapResult);
        void AssignPrimaryStress(CWordForm * pWordForm);
        void AssignSecondaryStress(CWordForm * pWordForm);
        void AddWordForm(CWordForm * pWordForm);

        unsigned int uiTotalWordForms();
        ET_ReturnCode eGetWordForm(unsigned int uiAt, CWordForm *&);

    public:
        // Populate DB:
        ET_ReturnCode eSaveStemsToDb();
        ET_ReturnCode eAssignStemIds();
        ET_ReturnCode ePrepareDataInsert(unsigned int& uiStmtHandle);
        ET_ReturnCode eSaveDataToDb();

        // Helpers:
        ET_ReturnCode eLoadIrregularForms();
        ET_ReturnCode eLoadMissingForms();
        ET_ReturnCode eFormExists(CGramHasher& hasher);
        int iGramHashNSgMLong();

    private:
        void Init();

    private:
        CLexeme() {}

        //
        // Member variables
        //
        StLexemeProperties m_stProperties;
        multimap<int, CWordForm *> m_mmWordForms;        // gramm. hash --> wordform struct
        map<CEString, ET_MainSymbol> m_mapMainSymbol;
        map<CEString, ET_Gender> m_mapMainSymbolToGender;
        map<CEString, ET_Animacy> m_mapMainSymbolToAnimacy;
        map<CEString, CEString> m_mapStandardAlternations;
        multimap<int, int> m_mmEndingsHash;
        vector<CEString> m_vecAlternatingPreverbs, m_vecAlternatingPreverbsWithVoicing;
        CDictionary * m_pDictionary;

        // Word forms
        multimap<int, CWordForm *>::iterator m_itCurrentWordForm;

        // Irregular forms
        multimap<int, StIrregularForm> m_mmapIrregularForms;    // hash to irregular form struct
        typedef pair<multimap<int, StIrregularForm>::iterator,
            multimap<int, StIrregularForm>::iterator> Pair_itIrregularFormRange;
        Pair_itIrregularFormRange m_pairItIfRange;
        multimap<int, StIrregularForm>::iterator m_itCurrentIrregularForm;

        // Missing forms
        vector<int> m_vecMissingForms;

        vector<int>::iterator m_itCurrentStressPos;

    };  //  CLexeme

}   //  namespace Hlib

#endif  //  C_LEXEME_H_INCLUDED