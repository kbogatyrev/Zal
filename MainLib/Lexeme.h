#pragma once

#include "WordForm.h"
#include "FormDescriptor.h"

using namespace std;
using namespace std::tr1;

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

class CLexeme
{
friend class CDictionary;
friend class CFormBuilderBase;
friend class CFormBuilderBaseDecl;
friend class CFormBuilderBaseConj;
friend class CFormBuilderNouns;
friend class CFormBuilderLongAdj;
friend class CFormBuilderShortAdj;
friend class CFormBuilderPronounAdj;
friend class CFormBuilderPersonal;
friend class CFormBuilderPast;
friend class CFormBuilderImperative;
friend class CFormBuilderNonFinite;
friend class CFormBuilderComparative;
friend struct CWordForm;
friend class CAnalyzer;

public:
    CLexeme();
    ~CLexeme();

    void Init();

    ET_Gender eGetGender()
    {
        return m_mapMainSymbolToGender[m_sInflectionType];
    }

    ET_Animacy eGetAnimacy()
    {
        return m_mapMainSymbolToAnimacy[m_sInflectionType];
    }

    CEString sGraphicStem()
    {
        return m_sGraphicStem;
    }

    bool bHasIrregularForms()
    {
        return m_bHasIrregularForms;
    }

    void SetDb (const CEString& str_dbPath);
    void AddWordForm (CWordForm * pWordForm);
	void WordFormFromHash (int iHash, unsigned int uiAt, CWordForm *& pWf);
    bool bHasIrregularForm (int iGramHash);
    bool bNoRegularForms (int iGramHash);
    void GetIrregularForms(int iHash, map<CWordForm *, bool>& mapResult);
    unsigned int uiFormCount (int iHash);
    bool bHasCommonDeviation (int iCd);
    bool bDeviationOptional (int iCd);

private:
    multimap<int, CWordForm *> m_mmWordForms;
    map<CEString, ET_MainSymbol> m_mapMainSymbol;
    map<CEString, ET_Gender> m_mapMainSymbolToGender;
    map<CEString, ET_Animacy> m_mapMainSymbolToAnimacy;
    map<CEString, CEString> m_mapStandardAlternations;
    multimap<int, int> m_mmEndingsHash;
    vector<CEString> m_vecAlternatingPreverbs, m_vecAlternatingPreverbsWithVoicing;
    CEString sDbPath;
    CSqlite * m_pDb;

    //
    // Stem data as acquired from the DB
    //
    int m_iDbKey;
    CEString m_sSourceForm;
    CEString m_sHeadwordComment;
    vector<int> m_vecSourceStressPos, m_vecSecondaryStressPos;
    bool m_bIsUnstressed;
    vector<int> m_vecHomonyms;
    CEString m_sGraphicStem;
    bool m_bIsVariant;
    CEString m_sMainSymbol;
    bool m_bIsPluralOf;
    bool m_bTransitive;
    ET_Reflexive m_eReflexive;
    CEString m_sMainSymbolPluralOf;
    CEString m_sAltMainSymbol;
    ET_Aspect m_eAspect;
    CEString m_sInflectionType;
    ET_PartOfSpeech m_ePartOfSpeech;
    CEString m_sComment;
    CEString m_sAltMainSymbolComment;
    CEString m_sAltInflectionComment;
    CEString m_sVerbStemAlternation;
    bool m_bPartPastPassZhd;
    int m_iSection;
    bool m_bNoComparative;
    bool m_bAssumedForms;
    bool m_bYoAlternation;
    bool m_bOAlternation;
    bool m_bSecondGenitive;
    bool m_bSecondLocative;
    bool m_bSecondLocativeOptional;
    CEString m_sLoc2Preposition;
    bool m_bHasAspectPair;
    int m_iAspectPairType;
    CEString m_sAspectPairComment;
    CEString m_sQuestionableForms;
    bool m_bHasIrregularForms;
    bool m_bHasIrregularVariants;    // true if any of irreg. forms alternates with regular
    bool m_bHasDeficiencies;
    CEString m_sRestrictedFroms;
    CEString m_sContexts;
    CEString m_sTrailingComment;

    // From inflection table:
    int m_iInflectionId;
    bool m_bPrimaryInflectionGroup;
    int m_iType;
    ET_AccentType m_eAccentType1;
    ET_AccentType m_eAccentType2;
    bool m_bShortFormsRestricted;
    bool m_bPastParticipleRestricted;
    bool m_bNoLongForms;
    bool m_bNoShortForms;
    bool m_bNoPastParticiple;
    bool m_bFleetingVowel;
    int m_iStemAugment;

    map<int, bool> m_mapCommonDeviations;

	//
	// Regular forms
	//
	multimap<int, CWordForm *> m_mmapWordForms;

    //
    // Irregular forms
    //
//    multimap<int, StIrregularForm> m_mmapIrregularForms; 
//    typedef pair<multimap<int, StIrregularForm>::iterator, 
//                 multimap<int, StIrregularForm>::iterator> Pair_itIrregularFormRange;
//    Pair_itIrregularFormRange pairItIfRange;
//    multimap<int, StIrregularForm>::iterator m_itCurrentIrregularForm;
    multimap<int, StIrregularForm> m_mmapIrregularForms;    // hash to irregular form struct
    typedef pair<multimap<int, StIrregularForm>::iterator, 
                 multimap<int, StIrregularForm>::iterator> Pair_itIrregularFormRange;
    Pair_itIrregularFormRange m_pairItIfRange;
    multimap<int, StIrregularForm>::iterator m_itCurrentIrregularForm;

    //
    // Missing forms
    //
    vector<int> m_vecMissingForms;

    //
    // Verb stems:
    //
    CEString m_sInfStem;
    CEString m_sInfinitive;
    CEString m_s1SgStem;
    CEString m_s3SgStem;

    //
    // Helpers:
    //
    HRESULT hGetIrregularForms();

    HRESULT hGetMissingForms();
    HRESULT hGenerateParadigm();
    CEString sHash();

    HRESULT hFormExists (CGramHasher& hasher);

    //	HRESULT hAddIrregularForm (int iHash, const StIrregularForm& stIf);
    HRESULT hGetFirstIrregularForm (int iHash, CWordForm *&, bool& bIsOptional);
    HRESULT hGetNextIrregularForm (CWordForm *&, bool& bIsOptional);

};  //  CLexeme
