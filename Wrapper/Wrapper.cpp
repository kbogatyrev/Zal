// Wrapper.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include <boost/python.hpp>
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include "IDictionary.h"
#include "Dictionary.h"
#include "ILexeme.h"
#include "Enums.h"
#include "EString.h"
#include "IWordForm.h"
using namespace boost::python;
using namespace Hlib;
using namespace std;

struct ILexemeWrap : ILexeme, wrapper<ILexeme>
{
	const StLexemeProperties& stGetProperties()
	{
		return this->get_override("stGetProperties")();
	}
	StLexemeProperties& stGetPropertiesForWriteAccess()
	{
		return call<StLexemeProperties&>(get_override("stGetPropertiesForWriteAccess").ptr());
	}
	ET_Gender eGender()
	{
		return this->get_override("eGender")();
	}
	ET_Animacy eAnimacy()
	{
		return this->get_override("eAnimacy")();
	}
	CEString sGraphicStem()
	{
		return this->get_override("sGraphicStem")();
	}
	bool bHasIrregularForms()
	{
		return this->get_override("bHasIrregularForms")();
	}
	bool bHasSecondaryStress()
	{
		return this->get_override("bHasSecondaryStress")();
	}
	bool bHasFleetingVowel()
	{
		return this->get_override("bHasFleetingVowel")();
	}
	bool bHasYoAlternation()
	{
		return this->get_override("bHasYoAlternation")();
	}
	bool bHasOAlternation()
	{
		return this->get_override("bHasOAlternation")();
	}
	CEString sSourceForm()
	{
		return this->get_override("sSourceForm")();
	}
	CEString sHeadwordComment()
	{
		return this->get_override("sHeadwordComment")();
	}
	bool bIsUnstressed()
	{
		return this->get_override("bIsUnstressed")();
	}
	bool IsVariant()
	{
		return this->get_override("IsVariant")();
	}
	CEString sMainSymbol()
	{
		return this->get_override("sMainSymbol")();
	}
	bool bIsPluralOf()
	{
		return this->get_override("bIsPluralOf")();
	}
	bool bTransitive()
	{
		return this->get_override("bTransitive")();
	}
	ET_Reflexive eIsReflexive()
	{
		return this->get_override("eIsReflexive")();
	}
	CEString sMainSymbolPluralOf()
	{
		return this->get_override("sMainSymbolPluralOf")();
	}
	CEString sAltMainSymbol()
	{
		return this->get_override("sAltMainSymbol")();
	}
	ET_Aspect eAspect()
	{
		return this->get_override("eAspect")();
	}
	CEString sInflectionType()
	{
		return this->get_override("sInflectionType")();
	}
	ET_PartOfSpeech ePartOfSpeech()
	{
		return this->get_override("ePartOfSpeech")();
	}
	CEString sComment()
	{
		return this->get_override("sComment")();
	}
	CEString sAltMainSymbolComment()
	{
		return this->get_override("sAltMainSymbolComment")();
	}
	CEString sAltInflectionComment()
	{
		return this->get_override("sAltInflectionComment")();
	}
	CEString sVerbStemAlternation()
	{
		return this->get_override("sVerbStemAlternation")();
	}
	bool bPartPastPassZhd()
	{
		return this->get_override("bPartPastPassZhd")();
	}
	int iSection()
	{
		return this->get_override("iSection")();
	}
	bool bNoComparative()
	{
		return this->get_override("bNoComparative")();
	}
	bool bAssumedForms()
	{
		return this->get_override("bAssumedForms")();
	}
	bool bYoAlternation()
	{
		return this->get_override("bYoAlternation")();
	}
	bool bOAlternation()
	{
		return this->get_override("bOAlternation")();
	}
	bool bSecondGenitive()
	{
		return this->get_override("bSecondGenitive")();
	}
	bool bSecondLocative()
	{
		return this->get_override("bSecondLocative")();
	}
	bool bSecondLocativeOptional()
	{
		return this->get_override("bSecondLocativeOptional")();
	}
	CEString sLoc2Preposition()
	{
		return this->get_override("sLoc2Preposition")();
	}
	bool bHasAspectPair()
	{
		return this->get_override("bHasAspectPair")();
	}
	int iAspectPairType()
	{
		return this->get_override("iAspectPairType")();
	}
	CEString sAspectPairComment()
	{
		return this->get_override("sAspectPairComment")();
	}
	CEString sQuestionableForms()
	{
		return this->get_override("sQuestionableForms")();
	}
	bool bHasIrregularVariants()
	{
		return this->get_override("bHasIrregularVariants")();
	}
	bool bHasDeficiencies()
	{
		return this->get_override("bHasDeficiencies")();
	}
	CEString sRestrictedFroms()
	{
		return this->get_override("sRestrictedFroms")();
	}
	CEString sContexts()
	{
		return this->get_override("sContexts")();
	}
	CEString sTrailingComment()
	{
		return this->get_override("sTrailingComment")();
	}
	int iInflectionId()
	{
		return this->get_override("iInflectionId")();
	}
	bool bPrimaryInflectionGroup()
	{
		return this->get_override("bPrimaryInflectionGroup")();
	}
	int iType()
	{
		return this->get_override("iType")();
	}
	ET_AccentType eAccentType1()
	{
		return this->get_override("eAccentType1")();
	}
	ET_AccentType eAccentType2()
	{
		return this->get_override("eAccentType2")();
	}
	bool  bShortFormsRestricted()
	{
		return this->get_override("bShortFormsRestricted")();
	}
	bool bPastParticipleRestricted()
	{
		return this->get_override("bPastParticipleRestricted")();
	}
	bool bNoLongForms()
	{
		return this->get_override("bNoLongForms")();
	}
	bool bNoShortForms()
	{
		return this->get_override("bNoShortForms")();
	}
	bool bNoPastParticiple()
	{
		return this->get_override("bNoPastParticiple")();
	}
	bool bFleetingVowel()
	{
		return this->get_override("bFleetingVowel")();
	}
	int iStemAugment()
	{
		return this->get_override("iStemAugment")();
	}
	CEString s1SgStem()
	{
		return this->get_override("s1SgStem")();
	}
	CEString s3SgStem()
	{
		return this->get_override("s3SgStem")();
	}
	CEString sInfinitive()
	{
		return this->get_override("sInfinitive")();
	}
	CEString sInfStem()
	{
		return this->get_override("sInfStem")();
	}
	bool bFindCommonDeviation(int iNum, bool& bIsOptional)
	{
		return this->get_override("bFindCommonDeviation")(iNum, bIsOptional);
	}
	bool bFindStandardAlternation(const CEString& sKey, CEString& sValue)
	{
		return this->get_override("bFindStandardAlternation")(sKey, sValue);
	}
	ET_ReturnCode eGetStemStressPositions(CEString& sLemma, vector<int>& vecPosition)
	{
		return this->get_override("eGetStemStressPositions")(sLemma, vecPosition);
	}
	ET_ReturnCode eGetAlternatingPreverb(const CEString& sVerbForm, CEString& sPreverb, bool& bVoicing)
	{
		return this->get_override("eGetAlternatingPreverb")(sVerbForm, sPreverb, bVoicing);
	}
	CEString sHash()
	{
		return this->get_override("sHash")();
	}
	ET_ReturnCode eWordFormFromHash(int iHash, unsigned int uiAt, IWordForm *& pWf)
	{
		return this->get_override("eWordFormFromHash")(iHash, uiAt, pWf);
	}
	bool bHasIrregularForm(int iGramHash)
	{
		return this->get_override("bHasIrregularForm")(iGramHash);
	}
	bool bNoRegularForms(int iGramHash)
	{
		return this->get_override("bNoRegularForms")(iGramHash);
	}
	ET_ReturnCode eGetFirstWordForm(IWordForm *& pWf)
	{
		return this->get_override("eGetFirstWordForm")(pWf);
	}
	ET_ReturnCode eGetNextWordForm(IWordForm *& pWf)
	{
		return this->get_override("eGetNextWordForm")(pWf);
	}
	ET_ReturnCode eGetFirstIrregularForm(int iHash, IWordForm *& pIWordForm, bool& bIsOptional)
	{
		return this->get_override("eGetFirstIrregularForm")(iHash, pIWordForm, bIsOptional);
	}
	ET_ReturnCode eGetNextIrregularForm(IWordForm *& pIWordForm, bool& bIsOptional)
	{
		return this->get_override("eGetNextIrregularForm")(pIWordForm, bIsOptional);
	}
	unsigned int uiFormCount(int iHash)
	{
		return this->get_override("uiFormCount")(iHash);
	}
	bool bHasCommonDeviation(int iCd)
	{
		return this->get_override("bHasCommonDeviation")(iCd);
	}
	bool bDeviationOptional(int iCd)
	{
		return this->get_override("bDeviationOptional")(iCd);
	}
	bool bIsFormMissing(int iGramHash)
	{
		return this->get_override("bIsFormMissing")(iGramHash);
	}
	bool bIsMultistressedCompound()
	{
		return this->get_override("bIsMultistressedCompound")();
	}
	ET_ReturnCode eGetFirstStemStressPos(int& iPos)
	{
		return this->get_override("eGetFirstStemStressPos")(iPos);
	}
	ET_ReturnCode eGetNextStemStressPos(int& iPos)
	{
		return this->get_override("eGetNextStemStressPos")(iPos);
	}
	ET_ReturnCode SetDb(const CEString& sDbPath)
	{
		return this->get_override("SetDb")(sDbPath);
	}
	ET_ReturnCode eGenerateParadigm()
	{
		return this->get_override("eGenerateParadigm")();
	}
	bool bSaveToDb(){
		return this->get_override("bSaveToDb")();
	}


};
struct IDictionaryWrap : IDictionary, wrapper<IDictionary>
{
	  ET_ReturnCode eSetDbPath(const CEString& sDbPath)
        {	
		        return this->get_override("eSetDbPath")(sDbPath);
	}

	ET_ReturnCode eGetLexemeById(long Id)
	{
		return this->get_override("eGetLexemeById")(Id);
	}
	ET_ReturnCode eGetLexemesByMd5(const CEString& sMd5)
	{
		return this->get_override("eGetLexemesByMd5")(sMd5);
	}
	ET_ReturnCode eGetLexemesByGraphicStem(const CEString&)
	{
		return this->get_override("eGetLexemesByGraphicStem")();
	}
	ET_ReturnCode eGetLexemesByInitialForm(const CEString&)
	{
		return this->get_override("eGetLexemesByInitialForm")();
	}
	ET_ReturnCode eGenerateAllForms(CProgressCallback& progress)
	{
		return this->get_override("eGenerateAllForms")(progress);
	}
	ET_ReturnCode eCountLexemes(int& iLexemes)
	{
		return this->get_override("eCountLexemes")(iLexemes);
	}
	ET_ReturnCode eGetFirstLexeme(ILexeme *& pLexeme)
	{
		return this->get_override("eGetFirstLexeme")(pLexeme);
	}
	ET_ReturnCode eGetNextLexeme(ILexeme *& pLexeme)
	{
		return this->get_override("eGetNextLexeme")(pLexeme);
	}
	void Clear(){
		this->get_override("Clear")();
	}

};
struct IWordFormWrap : IWordForm, wrapper<IWordForm>
{
	ILexeme * pLexeme()
	{
		return this->get_override("pLexeme")();
	}
	CEString sWordForm()
	{
		return this->get_override("sWordForm")();
	}
	CEString sLemma()
	{
		return this->get_override("sLemma")();
	}
	__int64 llLexemeId()
	{
		return this->get_override("llLexemeId")();
	}
	ET_PartOfSpeech ePos()
	{
		return this->get_override("ePos")();
	}
	ET_Case eCase()
	{
		return this->get_override("eCase")();
	}
	ET_Number eNumber()
	{
		return this->get_override("eNumber")();
	}
	ET_Subparadigm eSubparadigm()
	{
		return this->get_override("eSubparadigm")();
	}
	ET_Gender eGender()
	{
		return this->get_override("eGender")();
	}
	ET_Person ePerson()
	{
		return this->get_override("ePerson")();
	}
	ET_Animacy eAnimacy()
	{
		return this->get_override("eAnimacy")();
	}
	ET_Reflexive eReflexive()
	{
		return this->get_override("eReflexive")();
	}
	ET_Aspect eAspect()
	{
		return this->get_override("eAspect")();
	}
	ET_Status eStatus()
	{
		return this->get_override("eStatus")();
	}
	bool bIrregular()
	{
		return this->get_override("bIrregular")();
	}
	int iGramHash()
	{
		return this->get_override("iGramHash")();
	}
	ET_ReturnCode eInitFromHash(int iHash){
		return this->get_override("eInitFromHash")(iHash);
	}
};
class WFP
{
public:
	WFP() : pWF(NULL)
	{}
	~WFP()
	{
		delete pWF;
	}
	IWordForm * get()
	{
		return pWF;
	}
	IWordForm * pWF;
};
list eGetNextIrregularFormWrapped(ILexeme* pL, WFP& w)
{
	bool b;
	ET_ReturnCode rc = pL->eGetNextIrregularForm(w.pWF, b);
	list res;
	res.append(rc);
	res.append(b);
	return res;
}
extern "C"
{
	ET_ReturnCode GetDictionary(IDictionary *& d);
}
IDictionary* GetDictionaryWrap()
{
	IDictionary * pD = NULL;
	GetDictionary(pD);
	return pD;
}
CEString StrToCES(const wstring wDbPath)
{
	const wchar_t* wpDbPath = wDbPath.c_str();
	CEString sDbPath(wpDbPath);
	return sDbPath;
}
ILexeme* CreateNewLexeme()
{
	ILexeme * pL = NULL;
	return pL;
}
ET_ReturnCode eSetDbPathWrapped(IDictionary* pD, const wstring wDbPath)
{
	
	ET_ReturnCode rc = pD->eSetDbPath(StrToCES(wDbPath));
	return rc;
}
list eCountLexemesWrapped(IDictionary* pD)
{
	int iLexeme;
	ET_ReturnCode rc = pD->eCountLexemes(iLexeme);
	list res;
	res.append(rc);
	res.append(iLexeme);
	return res;
}
ET_ReturnCode eGetLexemesByInitialFormWrapped(IDictionary* pD, const wstring wIF)
{
	ET_ReturnCode rc = pD->eGetLexemesByInitialForm(StrToCES(wIF));
	return rc;
}
pair<ET_ReturnCode, ILexeme*> eGetFirstLexemeWrapped(IDictionary* pD)
{
	ILexeme * pL = NULL;
	ET_ReturnCode rc = pD->eGetFirstLexeme(pL);
	pair< ET_ReturnCode, ILexeme*> res(rc, pL);
	return res;
}
pair<ET_ReturnCode, ILexeme*> eGetNextLexemeWrapped(IDictionary* pD)
{
	ILexeme * pL = NULL;
	ET_ReturnCode rc = pD->eGetNextLexeme(pL);
	pair< ET_ReturnCode, ILexeme*> res(rc, pL);
	return res;
}
ILexeme* second_from_pair_IL(pair<ET_ReturnCode, ILexeme*> IL_pair)
{
	ILexeme * pL = NULL;
	pL = IL_pair.second;
	return pL;
}
IWordForm* second_from_pair_WF(pair<ET_ReturnCode, IWordForm*> IW_pair)
{
	IWordForm * pW = NULL;
	pW = IW_pair.second;
	return pW;
}
wstring sWordFormWrapped(IWordForm* pW)
{
	CEString sWF= pW->sWordForm();
	wstring pWF = sWF;
	return pWF;
}
wstring StrFromCES(CEString CES)
{
	wstring pWF = CES;
	return pWF;
}
pair<ET_ReturnCode, IWordForm*> eGetFirstWordFormWrapped(ILexeme* pL)
{
	IWordForm * pW = NULL;
	ET_ReturnCode rc = pL->eGetFirstWordForm(pW);
	pair<ET_ReturnCode, IWordForm*> res(rc, pW);
	return res;
}
pair<ET_ReturnCode, IWordForm*> eGetNextWordFormWrapped(ILexeme* pL)
{
	IWordForm * pW = NULL;
	ET_ReturnCode rc = pL->eGetNextWordForm(pW);
	pair<ET_ReturnCode, IWordForm*> res(rc, pW);
	return res;
}
ET_ReturnCode  eGenerateAllFormsWrapped(IDictionary* pD)
{
	CProgressCallback progress;
	ET_ReturnCode rc = pD->eGenerateAllForms(progress);
	return rc;
}
list vecSecondaryStressPos_wrapped(StLexemeProperties stLP)
{
	list result;
	vector<int> v = stLP.vecSecondaryStressPos;
	vector<int>::iterator it;
	for (it = v.begin(); it != v.end(); ++it){
		result.append(*it);
	}
	return result;
}
list vecSourceStressPos_wrapped(StLexemeProperties stLP)
{
	list result;
	vector<int> v = stLP.vecSourceStressPos;
	vector<int>::iterator it;
	for (it = v.begin(); it != v.end(); ++it){
		result.append(*it);
	}
	return result;
}
list vecHomonyms_wrapped(StLexemeProperties stLP)
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
wstring sRestrictedFromsWrapped(StLexemeProperties stLP)
{
	return StrFromCES(stLP.sRestrictedFroms);
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
void  sInfinitiveWrapped_set(StLexemeProperties& stLP, wstring n)
{
	stLP.sInfinitive = StrToCES(n);
}
BOOST_PYTHON_MODULE(Wrapper)
{
	class_<ILexemeWrap, boost::noncopyable>("ILexeme")
		.def("stGetProperties", pure_virtual(&ILexeme::stGetProperties), return_value_policy<copy_const_reference>())
		.def("stGetPropertiesForWriteAccess", pure_virtual(&ILexeme::stGetPropertiesForWriteAccess), return_value_policy<copy_non_const_reference>())
		.def("eGender", pure_virtual(&ILexeme::eGender))
		.def("eAnimacy", pure_virtual(&ILexeme::eAnimacy))
		.def("sGraphicStem", pure_virtual(&ILexeme::sGraphicStem))
		.def("bHasIrregularForms", pure_virtual(&ILexeme::bHasIrregularForms))
		.def("bHasSecondaryStress", pure_virtual(&ILexeme::bHasSecondaryStress))
		.def("bHasFleetingVowel", pure_virtual(&ILexeme::bHasFleetingVowel))
		.def("bHasYoAlternation", pure_virtual(&ILexeme::bHasYoAlternation))
		.def("bHasOAlternation", pure_virtual(&ILexeme::bHasOAlternation))
		.def("sSourceForm", pure_virtual(&ILexeme::sSourceForm))
		.def("sHeadwordComment", pure_virtual(&ILexeme::sHeadwordComment))
		.def("bIsUnstressed", pure_virtual(&ILexeme::bIsUnstressed))
		.def("IsVariant", pure_virtual(&ILexeme::IsVariant))
		.def("sMainSymbol", pure_virtual(&ILexeme::sMainSymbol))
		.def("bIsPluralOf", pure_virtual(&ILexeme::bIsPluralOf))
		.def("bTransitive", pure_virtual(&ILexeme::bTransitive))
		.def("eIsReflexive", pure_virtual(&ILexeme::eIsReflexive))
		.def("sMainSymbolPluralOf", pure_virtual(&ILexeme::sMainSymbolPluralOf))
		.def("sAltMainSymbol", pure_virtual(&ILexeme::sAltMainSymbol))
		.def("eAspect", pure_virtual(&ILexeme::eAspect))
		.def("sInflectionType", pure_virtual(&ILexeme::sInflectionType))
		.def("ePartOfSpeech", pure_virtual(&ILexeme::ePartOfSpeech))
		.def("sComment", pure_virtual(&ILexeme::sComment))
		.def("sAltMainSymbolComment", pure_virtual(&ILexeme::sAltMainSymbolComment))
		.def("sAltInflectionComment", pure_virtual(&ILexeme::sAltInflectionComment))
		.def("sVerbStemAlternation", pure_virtual(&ILexeme::sVerbStemAlternation))
		.def("bPartPastPassZhd", pure_virtual(&ILexeme::bPartPastPassZhd))
		.def("iSection", pure_virtual(&ILexeme::iSection))
		.def("bNoComparative", pure_virtual(&ILexeme::bNoComparative))
		.def("bAssumedForms", pure_virtual(&ILexeme::bAssumedForms))
		.def("bYoAlternation", pure_virtual(&ILexeme::bYoAlternation))
		.def("bOAlternation", pure_virtual(&ILexeme::bOAlternation))
		.def("bSecondGenitive", pure_virtual(&ILexeme::bSecondGenitive))
		.def("bSecondLocative", pure_virtual(&ILexeme::bSecondLocative))
		.def("bSecondLocativeOptional", pure_virtual(&ILexeme::bSecondLocativeOptional))
		.def("sLoc2Preposition", pure_virtual(&ILexeme::sLoc2Preposition))
		.def("bHasAspectPair", pure_virtual(&ILexeme::bHasAspectPair))
		.def("iAspectPairType", pure_virtual(&ILexeme::iAspectPairType))
		.def("sAspectPairComment", pure_virtual(&ILexeme::sAspectPairComment))
		.def("sQuestionableForms", pure_virtual(&ILexeme::sQuestionableForms))
		.def("bHasIrregularVariants", pure_virtual(&ILexeme::bHasIrregularVariants))
		.def("bHasDeficiencies", pure_virtual(&ILexeme::bHasDeficiencies))
		.def("sRestrictedFroms", pure_virtual(&ILexeme::sRestrictedFroms))
		.def("sContexts", pure_virtual(&ILexeme::sContexts))
		.def("sTrailingComment", pure_virtual(&ILexeme::sTrailingComment))
		.def("iInflectionId", pure_virtual(&ILexeme::iInflectionId))
		.def("bPrimaryInflectionGroup", pure_virtual(&ILexeme::bPrimaryInflectionGroup))
		.def("iType", pure_virtual(&ILexeme::iType))
		.def("eAccentType1", pure_virtual(&ILexeme::eAccentType1))
		.def("eAccentType2", pure_virtual(&ILexeme::eAccentType2))
		.def("bShortFormsRestricted", pure_virtual(&ILexeme::bShortFormsRestricted))
		.def("bPastParticipleRestricted", pure_virtual(&ILexeme::bPastParticipleRestricted))
		.def("bNoLongForms", pure_virtual(&ILexeme::bNoLongForms))
		.def("bNoShortForms", pure_virtual(&ILexeme::bNoShortForms))
		.def("bNoPastParticiple", pure_virtual(&ILexeme::bNoPastParticiple))
		.def("bFleetingVowel", pure_virtual(&ILexeme::bFleetingVowel))
		.def("iStemAugment", pure_virtual(&ILexeme::iStemAugment))
		.def("s1SgStem", pure_virtual(&ILexeme::s1SgStem))
		.def("s3SgStem", pure_virtual(&ILexeme::s3SgStem))
		.def("sInfinitive", pure_virtual(&ILexeme::sInfinitive))
		.def("sInfStem", pure_virtual(&ILexeme::sInfStem))
		.def("bFindCommonDeviation", pure_virtual(&ILexeme::bFindCommonDeviation))
		.def("bFindStandardAlternation", pure_virtual(&ILexeme::bFindStandardAlternation))
		.def("eGetStemStressPositions", pure_virtual(&ILexeme::eGetStemStressPositions))
		.def("eGetAlternatingPreverb", pure_virtual(&ILexeme::eGetAlternatingPreverb))
		.def("sHash", pure_virtual(&ILexeme::sHash))
		.def("eWordFormFromHash", pure_virtual(&ILexeme::eWordFormFromHash))
		.def("bHasIrregularForm", pure_virtual(&ILexeme::bHasIrregularForm))
		.def("bNoRegularForms", pure_virtual(&ILexeme::bNoRegularForms))
		.def("eGetFirstIrregularForm", pure_virtual(&ILexeme::eGetFirstIrregularForm))
		.def("eGetNextIrregularForm", eGetNextIrregularFormWrapped)
		.def("uiFormCount", pure_virtual(&ILexeme::uiFormCount))
		.def("bHasCommonDeviation", pure_virtual(&ILexeme::bHasCommonDeviation))
		.def("bDeviationOptional", pure_virtual(&ILexeme::bDeviationOptional))
		.def("bIsFormMissing", pure_virtual(&ILexeme::bIsFormMissing))
		.def("bIsMultistressedCompound", pure_virtual(&ILexeme::bIsMultistressedCompound))
		.def("eGetFirstStemStressPos", pure_virtual(&ILexeme::eGetFirstStemStressPos))
		.def("eGetNextStemStressPos", pure_virtual(&ILexeme::eGetNextStemStressPos))
		.def("SetDb", pure_virtual(&ILexeme::SetDb))
		.def("eGenerateParadigm", pure_virtual(&ILexeme::eGenerateParadigm))
		.def("bSaveToDb", pure_virtual(&ILexeme::bSaveToDb))
		.def("eGetFirstWordForm", eGetFirstWordFormWrapped)
	.def("eGetNextWordForm", eGetNextWordFormWrapped)
		;
	class_<IDictionaryWrap, boost::noncopyable>("IDictionary")
		.def("eSetDbPath", eSetDbPathWrapped)
		.def("eCountLexemes", eCountLexemesWrapped)
		.def("eGetLexemeById", pure_virtual(&IDictionary::eGetLexemeById))
		.def("eGetLexemesByMd5", pure_virtual(&IDictionary::eGetLexemesByMd5))
		.def("eGetLexemesByGraphicStem", pure_virtual(&IDictionary::eGetLexemesByGraphicStem))
		.def("eGetLexemesByInitialForm", eGetLexemesByInitialFormWrapped)
		.def("eGenerateAllForms", pure_virtual(&IDictionary::eGenerateAllForms))
		.def("Clear", pure_virtual(&IDictionary::Clear))
		.def("eGetFirstLexeme", eGetFirstLexemeWrapped)
		.def("eGetNextLexeme", eGetNextLexemeWrapped)
		;
	class_<WFP>("WFP")
		.def("get", &WFP::get, return_value_policy<manage_new_object>())
		;
	class_<IWordFormWrap, boost::noncopyable>("IWordForm")
		.def("pLexeme", pure_virtual(&IWordForm::pLexeme), return_value_policy<manage_new_object>())
		.def("sLemma", pure_virtual(&IWordForm::sLemma))
		.def("llLexemeId", pure_virtual(&IWordForm::llLexemeId))
		.def("ePos", pure_virtual(&IWordForm::ePos))
		.def("eCase", pure_virtual(&IWordForm::eCase))
		.def("eNumber", pure_virtual(&IWordForm::eNumber))
		.def("eSubparadigm", pure_virtual(&IWordForm::eSubparadigm))
		.def("eGender", pure_virtual(&IWordForm::eGender))
		.def("ePerson", pure_virtual(&IWordForm::ePerson))
		.def("eAnimacy", pure_virtual(&IWordForm::eAnimacy))
		.def("eReflexive", pure_virtual(&IWordForm::eReflexive))
		.def("eAspect", pure_virtual(&IWordForm::eAspect))
		.def("eStatus", pure_virtual(&IWordForm::eStatus))
		.def("bIrregular", pure_virtual(&IWordForm::bIrregular))
		.def("iGramHash", pure_virtual(&IWordForm::iGramHash))
		.def("eInitFromHash", pure_virtual(&IWordForm::eInitFromHash))
		.def("sWordForm", sWordFormWrapped)
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
		.add_property("vecSourceStressPos", vecSourceStressPos_wrapped)
		.add_property("vecSecondaryStressPos", vecSecondaryStressPos_wrapped)
		.def_readonly("bIsUnstressed", &StLexemeProperties::bIsUnstressed)
		.add_property("vecHomonyms", vecHomonyms_wrapped)
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
		.add_property("sRestrictedFroms", sRestrictedFromsWrapped)
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
	class_<std::pair<ET_ReturnCode, int> >("Int_Pair")
		.def(init<ET_ReturnCode, int>())
		.def_readwrite("first", &std::pair<ET_ReturnCode, int>::first)
		.def_readwrite("second", &std::pair<ET_ReturnCode, int>::second)
		;
	class_<std::pair<ET_ReturnCode, ILexeme*> >("ILexeme_Pair")
		.def(init<ET_ReturnCode, ILexeme*>())
		.def_readwrite("first", &std::pair<ET_ReturnCode, ILexeme*>::first)
		.def_readwrite("second", &std::pair<ET_ReturnCode, ILexeme*>::second)
		;
	class_<std::pair<ET_ReturnCode, IWordForm*> >("IWordForm_Pair")
		.def(init<ET_ReturnCode, IWordForm*>())
		.def_readwrite("first", &std::pair<ET_ReturnCode, IWordForm*>::first)
		.def_readwrite("second", &std::pair<ET_ReturnCode, IWordForm*>::second)
		;
	def("GetDictionary", GetDictionaryWrap, return_value_policy<manage_new_object>());
	def("CreateNewLexeme", CreateNewLexeme, return_value_policy<manage_new_object>());
	def("second_from_pair_IL", second_from_pair_IL, return_value_policy<manage_new_object>());
	def("second_from_pair_WF", second_from_pair_WF, return_value_policy<manage_new_object>());
	def("StrToCES", StrToCES);
	def("StrFromCES", StrFromCES);
}

int main()
{
	return 0;
}


