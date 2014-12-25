// Wrapper.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include <boost/python.hpp>
#include <boost/python/exception_translator.hpp>
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <exception> 

#include "IDictionary.h"
#include "Dictionary.h"
#include "ILexeme.h"
#include "Enums.h"
#include "EString.h"
#include "IWordForm.h"
using namespace boost::python;
using namespace Hlib;
using namespace std;

extern "C"
{
	ET_ReturnCode GetDictionary(IDictionary *& d);
}

wstring StrFromCES(CEString CES)
{
	wstring pWF = CES;
	return pWF;
}
CEString StrToCES(const wstring wStr)
{
	CEString sString(wStr.c_str());
	return sString;
}
struct Null_pointer : public exception
{
	char const* what() const throw() { return "Class instant is not initialized"; }
};
void translate_exception(Null_pointer const& e)
{
	PyErr_SetString(PyExc_RuntimeError, e.what());
}
class IWordFormWrap
{
public:
	IWordFormWrap() : pWF(NULL)
	{}
	ILexeme * pLexeme()
	{
		if (pWF == NULL){ throw Null_pointer(); }
		return pWF->pLexeme();
	}
	wstring sWordForm()
	{
		if (pWF == NULL){ throw Null_pointer(); }
		CEString CESWF = pWF->sWordForm();
		wstring wWF = CESWF;
		return wWF;
	}
	wstring sLemma()
	{
		if (pWF == NULL){ throw Null_pointer(); }
		CEString CESLemma = pWF->sLemma();
		wstring wLemma = CESLemma;
		return wLemma;
	}
	__int64 llLexemeId()
	{
		if (pWF == NULL){ throw Null_pointer(); }
		return pWF->llLexemeId();
	}
	ET_PartOfSpeech ePos()
	{
		if (pWF == NULL){ throw Null_pointer(); }
		return pWF->ePos();
	}
	ET_Case eCase()
	{
		if (pWF == NULL){ throw Null_pointer(); }
		return pWF->eCase();
	}
	ET_Number eNumber()
	{
		if (pWF == NULL){ throw Null_pointer(); }
		return pWF->eNumber();
	}
	ET_Subparadigm eSubparadigm()
	{
		if (pWF == NULL){ throw Null_pointer(); }
		return pWF->eSubparadigm();
	}
	ET_Gender eGender()
	{
		if (pWF == NULL){ throw Null_pointer(); }
		return pWF->eGender();
	}
	ET_Person ePerson()
	{
		if (pWF == NULL){ throw Null_pointer(); }
		return pWF->ePerson();
	}
	ET_Animacy eAnimacy()
	{
		if (pWF == NULL){ throw Null_pointer(); }
		return pWF->eAnimacy();
	}
	ET_Reflexive eReflexive()
	{
		if (pWF == NULL){ throw Null_pointer(); }
		return pWF->eReflexive();
	}
	ET_Aspect eAspect()
	{
		if (pWF == NULL){ throw Null_pointer(); }
		return pWF->eAspect();
	}
	ET_Status eStatus()
	{
		if (pWF == NULL){ throw Null_pointer(); }
		return pWF->eStatus();
	}
	bool bIrregular()
	{
		if (pWF == NULL){ throw Null_pointer(); }
		return pWF->bIrregular();
	}
	int iGramHash()
	{
		if (pWF == NULL){ throw Null_pointer(); }
		return pWF->iGramHash();
	}
	ET_ReturnCode eInitFromHash(int iHash)
	{
		if (pWF == NULL){ throw Null_pointer(); }
		return pWF->eInitFromHash(iHash);
	}
public:
	IWordForm * pWF;
};

class ILexemeWrap
{
public:
	ILexemeWrap() : pL(NULL)
	{}
	const StLexemeProperties& stGetProperties()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->stGetProperties();
	}
	StLexemeProperties& stGetPropertiesForWriteAccess()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->stGetPropertiesForWriteAccess();
	}
	ET_Gender eGender()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->eGender();
	}
	ET_Animacy eAnimacy()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->eAnimacy();
	}
	bool bHasIrregularForms()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bHasIrregularForms();
	}
	bool bHasSecondaryStress()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bHasSecondaryStress();
	}
	bool bHasFleetingVowel()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bHasFleetingVowel();
	}
	bool bHasYoAlternation()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bHasYoAlternation();
	}
	bool bHasOAlternation()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bHasOAlternation();
	}

	bool bIsUnstressed()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bIsUnstressed();
	}

	bool bIsPluralOf()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bIsPluralOf();
	}
	bool bTransitive()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bTransitive();
	}
	ET_Reflexive eIsReflexive()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->eIsReflexive();
	}

	ET_Aspect eAspect()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->eAspect();
	}

	ET_PartOfSpeech ePartOfSpeech()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->ePartOfSpeech();
	}

	bool bPartPastPassZhd()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bPartPastPassZhd();
	}
	int iSection()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->iSection();
	}
	bool bNoComparative()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bNoComparative();
	}
	bool bAssumedForms()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bAssumedForms();
	}
	bool bYoAlternation()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bYoAlternation();
	}
	bool bOAlternation()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bOAlternation();
	}
	bool bSecondGenitive()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bSecondGenitive();
	}
	bool bSecondLocative()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bSecondLocative();
	}
	bool bSecondLocativeOptional()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bSecondLocativeOptional();
	}

	bool bHasAspectPair()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bHasAspectPair();
	}
	int iAspectPairType()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->iAspectPairType();
	}

	bool bHasIrregularVariants()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bHasIrregularVariants();
	}
	bool bHasDeficiencies()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bHasDeficiencies();
	}

	int iInflectionId()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->iInflectionId();
	}
	bool bPrimaryInflectionGroup()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bPrimaryInflectionGroup();
	}
	int iType()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->iType();
	}
	ET_AccentType eAccentType1()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->eAccentType1();
	}
	ET_AccentType eAccentType2()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->eAccentType2();
	}
	bool bShortFormsRestricted()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bShortFormsRestricted();
	}
	bool bPastParticipleRestricted()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bPastParticipleRestricted();
	}
	bool bNoLongForms()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bNoLongForms();
	}
	bool bNoShortForms()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bNoShortForms();
	}
	bool bNoPastParticiple()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bNoPastParticiple();
	}
	bool bFleetingVowel()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bFleetingVowel();
	}
	int iStemAugment()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->iStemAugment();
	}

	list bFindCommonDeviation(int iNum)
	{
		if (pL == NULL){ throw Null_pointer(); }
		list res;
		bool b;

		res.append(pL->bFindCommonDeviation(iNum, b));
		res.append(b);
		return res;
	}
	list bFindStandardAlternation(wstring Key)
	{
		if (pL == NULL){ throw Null_pointer(); }
		list res;
		CEString sKey = StrToCES(Key);
		CEString sValue;
		res.append(pL->bFindStandardAlternation(sKey, sValue));
		res.append(StrFromCES(sValue));
		return res;
	}
	list eGetStemStressPositions(wstring Lemma)
	{
		if (pL == NULL){ throw Null_pointer(); }
		vector<int> vecPos;
		CEString sLemma = StrToCES(Lemma);
		list res;
		res.append(pL->eGetStemStressPositions(sLemma, vecPos));
		list lPos;
		vector<int>::iterator it;
		for (it = vecPos.begin(); it != vecPos.end(); ++it)
		{
			lPos.append(*it);
		}
		res.append(lPos);
		return res;
	}
	//ET_ReturnCode eGetAlternatingPreverb(const CEString& sVerbForm, CEString& sPreverb, bool& bVoicing)
	//{
	// if (pL == NULL){ throw Null_pointer(); }
	// return pL->eGetAlternatingPreverb(sVerbForm, sPreverb, bVoicing);
	//}

	//ET_ReturnCode eWordFormFromHash(int iHash, unsigned int uiAt, IWordForm *& pWf)
	//{
	//if (pL == NULL){ throw Null_pointer(); }
	// return pL->eWordFormFromHash(iHash, uiAt, pWf);
	//}
	bool bHasIrregularForm(int iGramHash)
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bHasIrregularForm(iGramHash);
	}
	bool bNoRegularForms(int iGramHash)
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bNoRegularForms(iGramHash);
	}
	ET_ReturnCode eGetFirstWordForm(IWordFormWrap& w)
	{
		if (pL == NULL){ throw Null_pointer(); }
		IWordForm * ptempWF = NULL;
		ET_ReturnCode rc = pL->eGetFirstWordForm(ptempWF);
		if (ptempWF == NULL) { return rc; }
		ptempWF->eClone(w.pWF);
		return rc;
	}
	ET_ReturnCode eGetNextWordForm(IWordFormWrap& w)
	{
		if (pL == NULL){ throw Null_pointer(); }
		IWordForm * ptempWF = NULL;
		ET_ReturnCode rc = pL->eGetNextWordForm(ptempWF);
		if (ptempWF == NULL) { return rc; }
		ptempWF->eClone(w.pWF);
		return rc;
	}
	list eGetFirstIrregularForm(int iHash, IWordFormWrap& w)
	{
		if (pL == NULL){ throw Null_pointer(); }
		bool b;
		ET_ReturnCode rc = pL->eGetFirstIrregularForm(iHash, w.pWF, b);
		list res;
		res.append(rc);
		res.append(b);
		return res;
	}
	list eGetNextIrregularForm(IWordFormWrap& w)
	{
		if (pL == NULL){ throw Null_pointer(); }
		bool b;
		ET_ReturnCode rc = pL->eGetNextIrregularForm(w.pWF, b);
		list res;
		res.append(rc);
		res.append(b);
		return res;
	}
	unsigned int uiFormCount(int iHash)
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->uiFormCount(iHash);
	}
	bool bHasCommonDeviation(int iCd)
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bHasCommonDeviation(iCd);
	}
	bool bDeviationOptional(int iCd)
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bDeviationOptional(iCd);
	}
	bool bIsFormMissing(int iGramHash)
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bIsFormMissing(iGramHash);
	}
	bool bIsMultistressedCompound()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->bIsMultistressedCompound();
	}
	list eGetFirstStemStressPos()
	{
		if (pL == NULL){ throw Null_pointer(); }
		list res;
		int iPos;
		res.append(pL->eGetFirstStemStressPos(iPos));
		res.append(iPos);
		return res;
	}
	list eGetNextStemStressPos()
	{
		if (pL == NULL){ throw Null_pointer(); }
		list res;
		int iPos;
		res.append(pL->eGetNextStemStressPos(iPos));
		res.append(iPos);
		return res;
	}
	ET_ReturnCode eSetDb(wstring path)
	{
		if (pL == NULL){ throw Null_pointer(); }
		return pL->eSetDb(StrToCES(path));
	}
	ET_ReturnCode eGenerateParadigm()
	{
		return pL->eGenerateParadigm();
	}
	wstring sGraphicStem()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->sGraphicStem());
	}
	wstring sSourceForm()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->sSourceForm());
	}
	wstring sHeadwordComment()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->sHeadwordComment());
	}
	wstring sMainSymbol()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->sMainSymbol());
	}
	wstring sMainSymbolPluralOf()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->sMainSymbolPluralOf());
	}
	wstring sAltMainSymbol()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->sAltMainSymbol());
	}
	wstring sInflectionType()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->sInflectionType());
	}
	wstring sComment()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->sComment());
	}
	wstring sAltMainSymbolComment()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->sAltMainSymbolComment());
	}
	wstring sAltInflectionComment()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->sAltInflectionComment());
	}
	wstring sVerbStemAlternation()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->sVerbStemAlternation());
	}
	wstring sLoc2Preposition()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->sLoc2Preposition());
	}
	wstring sAspectPairComment()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->sAspectPairComment());
	}
	wstring sQuestionableForms()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->sQuestionableForms());
	}
	wstring sContexts()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->sContexts());
	}
	wstring sTrailingComment()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->sTrailingComment());
	}
	wstring s1SgStem()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->s1SgStem());
	}
	wstring s3SgStem()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->s3SgStem());
	}
	wstring sInfinitive()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->sInfinitive());
	}
	wstring sInfStem()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->sInfStem());
	}
	wstring sHash()
	{
		if (pL == NULL){ throw Null_pointer(); }
		return StrFromCES(pL->sHash());
	}
	public:
		ILexeme * pL;
};
class IParserWrap
{
public:
	IParserWrap() : pP(NULL)
	{}
	~IParserWrap()
	{
		delete pP;
	}
	ET_ReturnCode eAnalyze(const wstring wText)
	{
		if (pP == NULL){ throw Null_pointer(); }
		return pP->eAnalyze(StrToCES(wText));
	}
	ET_ReturnCode eGetFirstWordForm(IWordFormWrap& w)
	{
		if (pP == NULL){ throw Null_pointer(); }
		IWordForm * ptempWF = NULL;
		ET_ReturnCode rc = pP->eGetFirstWordForm(ptempWF);
		if (ptempWF == NULL) { return rc; }
		ptempWF->eClone(w.pWF);
		return rc;
	}
	ET_ReturnCode eGetNextWordForm(IWordFormWrap& w)
	{
		if (pP == NULL){ throw Null_pointer(); }
		IWordForm * ptempWF = NULL;
		ET_ReturnCode rc = pP->eGetNextWordForm(ptempWF);
		if (ptempWF == NULL) { return rc; }
		ptempWF->eClone(w.pWF);
		return rc;
	}
public:
	IParser * pP;
};
class IDictionaryWrap
{public:
	IDictionaryWrap() : pD(NULL)
	{}

	ET_ReturnCode eSetDbPath(const wstring wDbPath)
	{
		if (pD == NULL){ throw Null_pointer(); }

		ET_ReturnCode rc = pD->eSetDbPath(StrToCES(wDbPath));
		return rc;
	}
	ET_ReturnCode eGetLexemeById(long Id)
	{
		if (pD == NULL){ throw Null_pointer(); }
		return pD->eGetLexemeById(Id);
	}
	ET_ReturnCode eGetLexemesByGraphicStem(const wstring wIF)
	{
		if (pD == NULL){ throw Null_pointer(); }
		return pD->eGetLexemesByGraphicStem(StrToCES(wIF));
	}
	ET_ReturnCode eGetLexemesByInitialForm(const wstring wIF)
	{
		if (pD == NULL){ throw Null_pointer(); }
		return pD->eGetLexemesByInitialForm(StrToCES(wIF));
	}
	ET_ReturnCode eGenerateAllForms()
	{
		if (pD == NULL){ throw Null_pointer(); }
		return pD->eGenerateAllForms();
	}
	ET_ReturnCode eGenerateFormsForSelectedLexemes()
	{
		if (pD == NULL){ throw Null_pointer(); }
		return pD->eGenerateFormsForSelectedLexemes();
	}
	list eCountLexemes()
	{
		if (pD == NULL){ throw Null_pointer(); }
		int iLexeme;
		ET_ReturnCode rc = pD->eCountLexemes(iLexeme);
		list res;
		res.append(rc);
		res.append(iLexeme);
		return res;
	}
	ET_ReturnCode eGetFirstLexeme(ILexemeWrap& l)
	{
		if (pD == NULL){ throw Null_pointer(); }
		ILexeme * ptempL = NULL;
		ET_ReturnCode rc = pD->eGetFirstLexeme(ptempL);
		if (ptempL == NULL) { return rc; }
		ptempL->eClone(l.pL);
		return rc;
	}
	ET_ReturnCode eGetNextLexeme(ILexemeWrap& l)
	{
		if (pD == NULL){ throw Null_pointer(); }
		ILexeme * ptempL = NULL;
		ET_ReturnCode rc = pD->eGetNextLexeme(ptempL);
		if (ptempL == NULL) { return rc; }
		ptempL->eClone(l.pL);
		return rc;
	}
	void Clear()
	{
		if (pD == NULL){ throw Null_pointer(); }
		return pD->Clear();
	}
	ET_ReturnCode eGetParser(IParserWrap & p)
	{
		if (pD == NULL){ throw Null_pointer(); }
		return pD->eGetParser(p.pP);

	}
public:
	IDictionary * pD;

};


IDictionaryWrap GetDictionaryWrap()
{
	IDictionaryWrap  IDict;
	GetDictionary(IDict.pD);
	return IDict;
}

//START: StLexemeProperties Functions
list vecSecondaryStressPosWrapped(StLexemeProperties stLP)
{
	list result;
	vector<int> v = stLP.vecSecondaryStressPos;
	vector<int>::iterator it;
	for (it = v.begin(); it != v.end(); ++it){
		result.append(*it);
	}
	return result;
}
list vecSourceStressPosWrapped(StLexemeProperties stLP)
{
	list result;
	vector<int> v = stLP.vecSourceStressPos;
	vector<int>::iterator it;
	for (it = v.begin(); it != v.end(); ++it){
		result.append(*it);
	}
	return result;
}
list vecHomonymsWrapped(StLexemeProperties stLP)
{
	list result;
	vector<int> v = stLP.vecHomonyms;
	vector<int>::iterator it;
	for (it = v.begin(); it != v.end(); ++it){
		result.append(*it);
	}
	return result;
}
dict mapCommonDeviationsWrapped(StLexemeProperties stLP)
{
	dict result;
	map<int, bool> m = stLP.mapCommonDeviations;
	map<int, bool>::iterator it;
	for (it = m.begin(); it != m.end(); ++it){
		result[it->first] = it->second;
	}
	return result;
}
//StLexemeProperties CEString functions
wstring sSourceFormWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.sSourceForm);
}
wstring sHeadwordCommentWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.sHeadwordComment);
}
wstring sGraphicStemWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.sGraphicStem);
}
wstring sMainSymbolWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.sMainSymbol);
}
wstring sMainSymbolPluralOfWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.sMainSymbolPluralOf);
}
wstring sAltMainSymbolWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.sAltMainSymbol);
}
wstring sInflectionTypeWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.sInflectionType);
}
wstring sCommentWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.sComment);
}
wstring sAltMainSymbolCommentWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.sAltMainSymbolComment);
}
wstring sAltInflectionCommentWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.sAltInflectionComment);
}
wstring sVerbStemAlternationWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.sVerbStemAlternation);
}
wstring sLoc2PrepositionWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.sLoc2Preposition);
}
wstring sAspectPairCommentWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.sAspectPairComment);
}
wstring sQuestionableFormsWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.sQuestionableForms);
}
wstring sContextsWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.sContexts);
}
wstring sTrailingCommentWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.sTrailingComment);
}
wstring sInfStemWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.sInfStem);
}
wstring s1SgStemWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.s1SgStem);
}
wstring s3SgStemWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.s3SgStem);
}
wstring sInfinitiveWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.sInfinitive);
}
void sInfinitiveWrapped_set(StLexemeProperties& stLP, wstring n)
{
	stLP.sInfinitive = StrToCES(n);
}
//END: StLexemeProperties
BOOST_PYTHON_MODULE(Wrapper)
{
	register_exception_translator<Null_pointer>(&translate_exception);
	class_<ILexemeWrap>("ILexeme")
		.def("stGetProperties", &ILexemeWrap::stGetProperties,return_value_policy<copy_const_reference>())
		.def("stGetPropertiesForWriteAccess", &ILexemeWrap::stGetPropertiesForWriteAccess, return_value_policy<copy_non_const_reference>())
		.def("eGender", &ILexemeWrap::eGender)
		.def("eAnimacy", &ILexemeWrap::eAnimacy)
		.def("sGraphicStem", &ILexemeWrap::sGraphicStem)
		.def("bHasIrregularForms", &ILexemeWrap::bHasIrregularForms)
		.def("bHasSecondaryStress", &ILexemeWrap::bHasSecondaryStress)
		.def("bHasFleetingVowel", &ILexemeWrap::bHasFleetingVowel)
		.def("bHasYoAlternation", &ILexemeWrap::bHasYoAlternation)
		.def("bHasOAlternation", &ILexemeWrap::bHasOAlternation)
		.def("sSourceForm", &ILexemeWrap::sSourceForm)
		.def("sHeadwordComment", &ILexemeWrap::sHeadwordComment)
		.def("bIsUnstressed", &ILexemeWrap::bIsUnstressed)
		.def("sMainSymbol", &ILexemeWrap::sMainSymbol)
		.def("bIsPluralOf", &ILexemeWrap::bIsPluralOf)
		.def("bTransitive", &ILexemeWrap::bTransitive)
		.def("eIsReflexive", &ILexemeWrap::eIsReflexive)
		.def("sMainSymbolPluralOf", &ILexemeWrap::sMainSymbolPluralOf)
		.def("sAltMainSymbol", &ILexemeWrap::sAltMainSymbol)
		.def("eAspect", &ILexemeWrap::eAspect)
		.def("sInflectionType", &ILexemeWrap::sInflectionType)
		.def("ePartOfSpeech", &ILexemeWrap::ePartOfSpeech)
		.def("sComment", &ILexemeWrap::sComment)
		.def("sAltMainSymbolComment", &ILexemeWrap::sAltMainSymbolComment)
		.def("sAltInflectionComment", &ILexemeWrap::sAltInflectionComment)
		.def("sVerbStemAlternation", &ILexemeWrap::sVerbStemAlternation)
		.def("bPartPastPassZhd", &ILexemeWrap::bPartPastPassZhd)
		.def("iSection", &ILexemeWrap::iSection)
		.def("bNoComparative", &ILexemeWrap::bNoComparative)
		.def("bAssumedForms", &ILexemeWrap::bAssumedForms)
		.def("bYoAlternation", &ILexemeWrap::bYoAlternation)
		.def("bOAlternation", &ILexemeWrap::bOAlternation)
		.def("bSecondGenitive", &ILexemeWrap::bSecondGenitive)
		.def("bSecondLocative", &ILexemeWrap::bSecondLocative)
		.def("bSecondLocativeOptional", &ILexemeWrap::bSecondLocativeOptional)
		.def("sLoc2Preposition", &ILexemeWrap::sLoc2Preposition)
		.def("bHasAspectPair", &ILexemeWrap::bHasAspectPair)
		.def("iAspectPairType", &ILexemeWrap::iAspectPairType)
		.def("sAspectPairComment", &ILexemeWrap::sAspectPairComment)
		.def("sQuestionableForms", &ILexemeWrap::sQuestionableForms)
		.def("bHasIrregularVariants", &ILexemeWrap::bHasIrregularVariants)
		.def("bHasDeficiencies", &ILexemeWrap::bHasDeficiencies)
		.def("sContexts", &ILexemeWrap::sContexts)
		.def("sTrailingComment", &ILexemeWrap::sTrailingComment)
		.def("iInflectionId", &ILexemeWrap::iInflectionId)
		.def("bPrimaryInflectionGroup", &ILexemeWrap::bPrimaryInflectionGroup)
		.def("iType", &ILexemeWrap::iType)
		.def("eAccentType1", &ILexemeWrap::eAccentType1)
		.def("eAccentType2", &ILexemeWrap::eAccentType2)
		.def("bShortFormsRestricted", &ILexemeWrap::bShortFormsRestricted)
		.def("bPastParticipleRestricted", &ILexemeWrap::bPastParticipleRestricted)
		.def("bNoLongForms", &ILexemeWrap::bNoLongForms)
		.def("bNoShortForms", &ILexemeWrap::bNoShortForms)
		.def("bNoPastParticiple", &ILexemeWrap::bNoPastParticiple)
		.def("bFleetingVowel", &ILexemeWrap::bFleetingVowel)
		.def("iStemAugment", &ILexemeWrap::iStemAugment)
		.def("s1SgStem", &ILexemeWrap::s1SgStem)
		.def("s3SgStem", &ILexemeWrap::s3SgStem)
		.def("sInfinitive", &ILexemeWrap::sInfinitive)
		.def("sInfStem", &ILexemeWrap::sInfStem)
		.def("bFindCommonDeviation", &ILexemeWrap::bFindCommonDeviation)
		.def("bFindStandardAlternation", &ILexemeWrap::bFindStandardAlternation)
		.def("eGetStemStressPositions", &ILexemeWrap::eGetStemStressPositions)
		//.def("eGetAlternatingPreverb", &ILexemeWrap::eGetAlternatingPreverb)
		.def("sHash", &ILexemeWrap::sHash)
		//.def("eWordFormFromHash", &ILexemeWrap::eWordFormFromHash)
		.def("bHasIrregularForm", &ILexemeWrap::bHasIrregularForm)
		.def("bNoRegularForms", &ILexemeWrap::bNoRegularForms)
		.def("eGetFirstWordForm", &ILexemeWrap::eGetFirstWordForm)
		.def("eGetNextWordForm", &ILexemeWrap::eGetNextWordForm)
		.def("eGetFirstIrregularForm", &ILexemeWrap::eGetFirstIrregularForm)
		.def("eGetNextIrregularForm", &ILexemeWrap::eGetNextIrregularForm)
		.def("uiFormCount", &ILexemeWrap::uiFormCount)
		.def("bHasCommonDeviation", &ILexemeWrap::bHasCommonDeviation)
		.def("bDeviationOptional", &ILexemeWrap::bDeviationOptional)
		.def("bIsFormMissing", &ILexemeWrap::bIsFormMissing)
		.def("bIsMultistressedCompound", &ILexemeWrap::bIsMultistressedCompound)
		.def("eGetFirstStemStressPos", &ILexemeWrap::eGetFirstStemStressPos)
		.def("eGetNextStemStressPos", &ILexemeWrap::eGetNextStemStressPos)
		.def("eSetDb", &ILexemeWrap::eSetDb)
		.def("eGenerateParadigm", &ILexemeWrap::eGenerateParadigm)
		;
	class_<IDictionaryWrap>("IDictionary")
		.def("eSetDbPath", &IDictionaryWrap::eSetDbPath)
		.def("eGetLexemeById", &IDictionaryWrap::eGetLexemeById)
		.def("eGetLexemesByGraphicStem", &IDictionaryWrap::eGetLexemesByGraphicStem)
		.def("eGetLexemesByInitialForm", &IDictionaryWrap::eGetLexemesByInitialForm)
		.def("eGenerateAllForms", &IDictionaryWrap::eGenerateAllForms)
		.def("eGenerateFormsForSelectedLexemes", &IDictionaryWrap::eGenerateFormsForSelectedLexemes)
		.def("eCountLexemes", &IDictionaryWrap::eCountLexemes)
		.def("eGetFirstLexeme", &IDictionaryWrap::eGetFirstLexeme)
		.def("eGetNextLexeme", &IDictionaryWrap::eGetNextLexeme)
		.def("Clear", &IDictionaryWrap::Clear)
		.def("eGetParser", &IDictionaryWrap::eGetParser)
		;
	class_<IParserWrap>("IParser")
		.def("eAnalyze", &IParserWrap::eAnalyze)
		.def("eGetFirstWordForm", &IParserWrap::eGetFirstWordForm)
		.def("eGetNextWordForm", &IParserWrap::eGetNextWordForm)
		;
	class_<IWordFormWrap>("IWordForm")
		.def("pLexeme", &IWordFormWrap::pLexeme, return_value_policy<manage_new_object>())
		.def("sWordForm", &IWordFormWrap::sWordForm)
		.def("sLemma", &IWordFormWrap::sLemma)
		.def("llLexemeId", &IWordFormWrap::llLexemeId)
		.def("ePos", &IWordFormWrap::ePos)
		.def("eCase", &IWordFormWrap::eCase)
		.def("eNumber", &IWordFormWrap::eNumber)
		.def("eSubparadigm", &IWordFormWrap::eSubparadigm)
		.def("eGender", &IWordFormWrap::eGender)
		.def("ePerson", &IWordFormWrap::ePerson)
		.def("eAnimacy", &IWordFormWrap::eAnimacy)
		.def("eReflexive", &IWordFormWrap::eReflexive)
		.def("eAspect", &IWordFormWrap::eAspect)
		.def("eStatus", &IWordFormWrap::eStatus)
		.def("bIrregular", &IWordFormWrap::bIrregular)
		.def("iGramHash", &IWordFormWrap::iGramHash)
		.def("eInitFromHash", &IWordFormWrap::eInitFromHash)
		;
	class_<CEString>("CEString")
		.def(init<const CEString&>(args("Source")))
		.def(init<const wchar_t *,
		const wchar_t *,
		const wchar_t *,
		const wchar_t *,
		const wchar_t *,
		const wchar_t *>(args("szSource","szBreaks","szTabs","szPunctuation","szEscape","szVowels")))
		;
	class_<StLexemeProperties>("StLexemeProperties")
		.def_readonly("iDbKey", &StLexemeProperties::iDbKey)
		.add_property("sSourceForm", sSourceFormWrapped)
		.add_property("sHeadwordComment", sHeadwordCommentWrapped)
		.add_property("vecSourceStressPos", vecSourceStressPosWrapped)
		.add_property("vecSecondaryStressPos", vecSecondaryStressPosWrapped)
		.def_readonly("bIsUnstressed", &StLexemeProperties::bIsUnstressed)
		.add_property("vecHomonyms", vecHomonymsWrapped)
		.add_property("sGraphicStem", sGraphicStemWrapped)
		.def_readonly("bIsVariant", &StLexemeProperties::bIsVariant)
		.add_property("sMainSymbol", sMainSymbolWrapped)
		.def_readonly("bIsPluralOf", &StLexemeProperties::bIsPluralOf)
		.def_readonly("bTransitive", &StLexemeProperties::bTransitive)
		.def_readonly("eReflexive", &StLexemeProperties::eReflexive)
		.add_property("sMainSymbolPluralOf", sMainSymbolPluralOfWrapped)
		.add_property("sAltMainSymbol", sAltMainSymbolWrapped)
		.def_readonly("eAspect", &StLexemeProperties::eAspect)
		.add_property("sInflectionType", sInflectionTypeWrapped)
		.def_readonly("ePartOfSpeech", &StLexemeProperties::ePartOfSpeech)
		.add_property("sComment", sCommentWrapped)
		.add_property("sAltMainSymbolComment", sAltMainSymbolCommentWrapped)
		.add_property("sAltInflectionComment", sAltInflectionCommentWrapped)
		.add_property("sVerbStemAlternation", sVerbStemAlternationWrapped)
		.def_readonly("bPartPastPassZhd", &StLexemeProperties::bPartPastPassZhd)
		.def_readonly("iSection", &StLexemeProperties::iSection)
		.def_readonly("bNoComparative", &StLexemeProperties::bNoComparative)
		.def_readonly("bAssumedForms", &StLexemeProperties::bAssumedForms)
		.def_readonly("bYoAlternation", &StLexemeProperties::bYoAlternation)
		.def_readonly("bOAlternation", &StLexemeProperties::bOAlternation)
		.def_readonly("bSecondGenitive", &StLexemeProperties::bSecondGenitive)
		.def_readonly("bSecondLocative", &StLexemeProperties::bSecondLocative)
		.def_readonly("bSecondLocativeOptional", &StLexemeProperties::bSecondLocativeOptional)
		.add_property("sLoc2Preposition", sLoc2PrepositionWrapped)
		.def_readonly("bHasAspectPair", &StLexemeProperties::bHasAspectPair)
		.def_readonly("iAspectPairType", &StLexemeProperties::iAspectPairType)
		.add_property("sAspectPairComment", sAspectPairCommentWrapped)
		.add_property("sQuestionableForms", sQuestionableFormsWrapped)
		.def_readonly("bHasIrregularForms", &StLexemeProperties::bHasIrregularForms)
		.def_readonly("bHasIrregularVariants", &StLexemeProperties::bHasIrregularVariants)
		.def_readonly("bHasDeficiencies", &StLexemeProperties::bHasDeficiencies)
		.add_property("sContexts", sContextsWrapped)
		.add_property("sTrailingComment", sTrailingCommentWrapped)
		.def_readonly("iInflectionId", &StLexemeProperties::iInflectionId)
		.def_readonly("bPrimaryInflectionGroup", &StLexemeProperties::bPrimaryInflectionGroup)
		.def_readonly("iType", &StLexemeProperties::iType)
		.def_readonly("eAccentType1", &StLexemeProperties::eAccentType1)
		.def_readonly("eAccentType2", &StLexemeProperties::eAccentType2)
		.def_readonly("bShortFormsRestricted", &StLexemeProperties::bShortFormsRestricted)
		.def_readonly("bPastParticipleRestricted", &StLexemeProperties::bPastParticipleRestricted)
		.def_readonly("bNoLongForms", &StLexemeProperties::bNoLongForms)
		.def_readonly("bNoShortForms", &StLexemeProperties::bNoShortForms)
		.def_readonly("bNoPastParticiple", &StLexemeProperties::bNoPastParticiple)
		.def_readonly("bFleetingVowel", &StLexemeProperties::bFleetingVowel)
		.def_readonly("iStemAugment", &StLexemeProperties::iStemAugment)
		.add_property("sInfStem", sInfStemWrapped)
		.add_property("s1SgStem", s1SgStemWrapped)
		.add_property("s3SgStem", s3SgStemWrapped)
		.add_property("sInfinitive", sInfinitiveWrapped, sInfinitiveWrapped_set)
		.add_property("mapCommonDeviations", mapCommonDeviationsWrapped)
		;

	enum_<ET_ReturnCode>("ET_ReturnCode")
		.value("H_NO_ERROR", H_NO_ERROR)
		.value("H_FALSE", H_FALSE)
		.value("H_TRUE", H_TRUE)
		.value("H_NO_MORE", H_NO_MORE)
		.value("H_ERROR_GENERAL", H_ERROR_GENERAL)
		.value("H_ERROR_UNEXPECTED", H_ERROR_UNEXPECTED)
		.value("H_ERROR_INVALID_ARG", H_ERROR_INVALID_ARG)
		.value("H_ERROR_DB", H_ERROR_DB)
		.value("H_EXCEPTION", H_EXCEPTION)
		.value("H_CTOR", H_CTOR)
		;
	enum_<ET_Ternary>("ET_Ternary")
		.value("TERNARY_FALSE", TERNARY_FALSE)
		.value("TERNARY_TRUE", TERNARY_TRUE)
		.value("TERNARY_BOTH", TERNARY_BOTH)
		;
	enum_<ET_PartOfSpeech>("ET_PartOfSpeech")
		.value("POS_UNDEFINED", POS_UNDEFINED)
		.value("POS_NOUN", POS_NOUN)
		.value("POS_VERB", POS_VERB)
		.value("POS_ADJ", POS_ADJ)
		.value("POS_PRONOUN", POS_PRONOUN)
		.value("POS_PRONOUN_ADJ", POS_PRONOUN_ADJ)
		.value("POS_PRONOUN_PREDIC", POS_PRONOUN_PREDIC)
		.value("POS_NUM", POS_NUM)
		.value("POS_NUM_ADJ", POS_NUM_ADJ)
		.value("POS_ADV", POS_ADV)
		.value("POS_COMPAR", POS_COMPAR)
		.value("POS_PREDIC", POS_PREDIC)
		.value("POS_PREP", POS_PREP)
		.value("POS_CONJ", POS_CONJ)
		.value("POS_PARTICLE", POS_PARTICLE)
		.value("POS_INTERJ", POS_INTERJ)
		.value("POS_PARENTH", POS_PARENTH)
		.value("POS_NULL", POS_NULL)
		.value("POS_COUNT", POS_COUNT)
		;
	enum_<ET_InflectionType>("ET_InflectionType")
		.value("INFLECTION_TYPE_UNDEFINED", INFLECTION_TYPE_UNDEFINED)
		.value("CONJUGATIONS_COUNT", CONJUGATIONS_COUNT)
		;
	enum_<ET_Case>("ET_Case")
		.value("CASE_UNDEFINED", CASE_UNDEFINED)
		.value("CASE_NOM", CASE_NOM)
		.value("CASE_ACC", CASE_ACC)
		.value("CASE_GEN", CASE_GEN)
		.value("CASE_PART", CASE_PART)
		.value("CASE_DAT", CASE_DAT)
		.value("CASE_INST", CASE_INST)
		.value("CASE_PREP", CASE_PREP)
		.value("CASE_LOC", CASE_LOC)
		.value("CASE_NUM", CASE_NUM)
		.value("CASE_COUNT", CASE_COUNT)
		;
	enum_<ET_Number>("ET_Number")
		.value("NUM_UNDEFINED", NUM_UNDEFINED)
		.value("NUM_SG", NUM_SG)
		.value("NUM_PL", NUM_PL)
		.value("NUM_COUNT", NUM_COUNT)
		;
	enum_<ET_Subparadigm>("ET_Subparadigm")
		.value("SUBPARADIGM_UNDEFINED", SUBPARADIGM_UNDEFINED)
		.value("SUBPARADIGM_NOUN", SUBPARADIGM_NOUN)
		.value("SUBPARADIGM_SHORT_ADJ", SUBPARADIGM_SHORT_ADJ)
		.value("SUBPARADIGM_LONG_ADJ", SUBPARADIGM_LONG_ADJ)
		.value("SUBPARADIGM_COMPARATIVE", SUBPARADIGM_COMPARATIVE)
		.value("SUBPARADIGM_INFINITIVE", SUBPARADIGM_INFINITIVE)
		.value("SUBPARADIGM_PRESENT_TENSE", SUBPARADIGM_PRESENT_TENSE)
		.value("SUBPARADIGM_PAST_TENSE", SUBPARADIGM_PAST_TENSE)
		.value("SUBPARADIGM_IMPERATIVE", SUBPARADIGM_IMPERATIVE)
		.value("SUBPARADIGM_ADVERBIAL_PRESENT", SUBPARADIGM_ADVERBIAL_PRESENT)
		.value("SUBPARADIGM_ADVERBIAL_PAST", SUBPARADIGM_ADVERBIAL_PAST)
		.value("SUBPARADIGM_PART_PRES_ACT", SUBPARADIGM_PART_PRES_ACT)
		.value("SUBPARADIGM_PART_PRES_PASS_LONG", SUBPARADIGM_PART_PRES_PASS_LONG)
		.value("SUBPARADIGM_PART_PRES_PASS_SHORT", SUBPARADIGM_PART_PRES_PASS_SHORT)
		.value("SUBPARADIGM_PART_PAST_ACT", SUBPARADIGM_PART_PAST_ACT)
		.value("SUBPARADIGM_PART_PAST_PASS_LONG", SUBPARADIGM_PART_PAST_PASS_LONG)
		.value("SUBPARADIGM_PART_PAST_PASS_SHORT", SUBPARADIGM_PART_PAST_PASS_SHORT)
		.value("SUBPARADIGM_COUNT", SUBPARADIGM_COUNT)
		;
	enum_<ET_Gender>("ET_Gender")
		.value("GENDER_UNDEFINED", GENDER_UNDEFINED)
		.value("GENDER_M", GENDER_M)
		.value("GENDER_F", GENDER_F)
		.value("GENDER_N", GENDER_N)
		.value("GENDER_COUNT", GENDER_COUNT)
		;
	enum_<ET_Aspect>("ET_Aspect")
		.value("ASPECT_UNDEFINED", ASPECT_UNDEFINED)
		.value("ASPECT_IMPERFECTIVE", ASPECT_IMPERFECTIVE)
		.value("ASPECT_PERFECTIVE", ASPECT_PERFECTIVE)
		.value("ASPECT_COUNT", ASPECT_COUNT)
		;
	enum_<ET_Person>("ET_Person")
		.value("PERSON_UNDEFINED", PERSON_UNDEFINED)
		.value("PERSON_1", PERSON_1)
		.value("PERSON_2", PERSON_2)
		.value("PERSON_3", PERSON_3)
		.value("PERSON_COUNT", PERSON_COUNT)
		;
	enum_<ET_Animacy>("ET_Animacy")
		.value("ANIM_UNDEFINED", ANIM_UNDEFINED)
		.value("ANIM_YES", ANIM_YES)
		.value("ANIM_NO", ANIM_NO)
		.value("ANIM_COUNT", ANIM_COUNT)
		;
	enum_<ET_Voice>("ET_Voice")
		.value("VOICE_UNDEFINED", VOICE_UNDEFINED)
		.value("VOICE_ACTIVE", VOICE_ACTIVE)
		.value("VOICE_PASSIVE", VOICE_PASSIVE)
		.value("VOICE_COUNT", VOICE_COUNT)
		;
	enum_<ET_Reflexive>("ET_Reflexive")
		.value("REFL_UNDEFINED", REFL_UNDEFINED)
		.value("REFL_YES", REFL_YES)
		.value("REFL_NO", REFL_NO)
		.value("REFL_COUNT", REFL_COUNT)
		;
	enum_<ET_AccentType>("ET_AccentType")
		.value("AT_UNDEFINED", AT_UNDEFINED)
		.value("AT_A", AT_A)
		.value("AT_A1", AT_A1)
		.value("AT_B", AT_B)
		.value("AT_B1", AT_B1)
		.value("AT_C", AT_C)
		.value("AT_C1", AT_C1)
		.value("AT_C2", AT_C2)
		.value("AT_D", AT_D)
		.value("AT_D1", AT_D1)
		.value("AT_E", AT_E)
		.value("AT_F", AT_F)
		.value("AT_F1", AT_F1)
		.value("AT_F2", AT_F2)
		.value("AT_COUNT", AT_COUNT)
		;
	enum_<ET_EndingClass>("ET_EndingClass")
		.value("ENDING_CLASS_UNDEFINED", ENDING_CLASS_UNDEFINED)
		.value("ENDING_CLASS_NOUN", ENDING_CLASS_NOUN)
		.value("ENDING_CLASS_ADJECTIVE", ENDING_CLASS_ADJECTIVE)
		.value("ENDING_CLASS_PRONOUN", ENDING_CLASS_PRONOUN)
		.value("ENDING_CLASS_SHORT_ADJECTIVE", ENDING_CLASS_SHORT_ADJECTIVE)
		.value("ENDING_CLASS_PRESENT_TENSE", ENDING_CLASS_PRESENT_TENSE)
		.value("ENDING_CLASS_PAST_TENSE", ENDING_CLASS_PAST_TENSE)
		.value("ENDING_CLASS_COMPARATIVE", ENDING_CLASS_COMPARATIVE)
		.value("ENDING_CLASS_COUNT", ENDING_CLASS_COUNT)
		;
	enum_<ET_StressLocation>("ET_StressLocation")
		.value("STRESS_LOCATION_UNDEFINED", STRESS_LOCATION_UNDEFINED)
		.value("STRESS_LOCATION_STEM", STRESS_LOCATION_STEM)
		.value("STRESS_LOCATION_ENDING", STRESS_LOCATION_ENDING)
		.value("STRESS_LOCATION_COUNT", STRESS_LOCATION_COUNT)
		;
	enum_<ET_StressType>("ET_StressType")
		.value("STRESS_TYPE_UNDEFINED", STRESS_TYPE_UNDEFINED)
		.value("STRESS_PRIMARY", STRESS_PRIMARY)
		.value("STRESS_SECONDARY", STRESS_SECONDARY)
		.value("STRESS_TYPE_COUNT", STRESS_TYPE_COUNT)
		;
	enum_<ET_StemAuslaut>("ET_StemAuslaut")
		.value("STEM_AUSLAUT_UNDEFINED", STEM_AUSLAUT_UNDEFINED)
		.value("STEM_AUSLAUT_SH", STEM_AUSLAUT_SH)
		.value("STEM_AUSLAUT_NOT_SH", STEM_AUSLAUT_NOT_SH)
		.value("STEM_AUSLAUT_COUNT", STEM_AUSLAUT_COUNT)
		;
	enum_<ET_Status>("ET_Status")
		.value("STATUS_UNDEFINED", STATUS_UNDEFINED)
		.value("STATUS_COMMON", STATUS_COMMON)
		.value("STATUS_OBSOLETE", STATUS_OBSOLETE)
		.value("STATUS_RARE", STATUS_RARE)
		.value("STATUS_QUESTIONABLE", STATUS_QUESTIONABLE)
		.value("STATUS_INCORRECT", STATUS_INCORRECT)
		.value("STATUS_COUNT", STATUS_COUNT)
		;
	enum_<ET_TestResult>("ET_TestResult")
		.value("TEST_RESULT_UNDEFINED", TEST_RESULT_UNDEFINED)
		.value("TEST_RESULT_OK", TEST_RESULT_OK)
		.value("TEST_RESULT_FAIL", TEST_RESULT_FAIL)
		.value("TEST_RESULT_INCOMPLETE", TEST_RESULT_INCOMPLETE)
		.value("TEST_RESULT_COUNT", TEST_RESULT_COUNT)
		;
	def("StrToCES", StrToCES);
	def("StrFromCES", StrFromCES);
	def("GetDictionary", GetDictionaryWrap);
}

int main()
{
	return 0;
}


