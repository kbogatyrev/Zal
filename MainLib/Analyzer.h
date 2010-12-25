#include "Analyzer_Helpers.h"
#include "Lexeme.h"
#include "WordForm.h"

using namespace std;
using namespace std::tr1;

#pragma once
#include "resource.h"       // main symbols
#include "MainLib_i.h"

#include "VCUE_Collection.h"
#include "VCUE_Copy.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

typedef CComEnumOnSTL <IEnumVARIANT, 
                       &__uuidof(IEnumVARIANT), 
                       VARIANT,
                       VCUE::GenericCopy<VARIANT, 
                                         std::vector<CComVariant>::value_type>,
                       std::vector<CComVariant> > EnumeratorType;

typedef VCUE::ICollectionOnSTLCopyImpl <IAnalyzer, 
                                        std::vector<CComVariant>, 
                                        VARIANT,
                                        VCUE::GenericCopy<VARIANT, 
                                                          std::vector<CComVariant>::value_type>,
                                        EnumeratorType> WordFormCollection;

class ATL_NO_VTABLE CT_Analyzer :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CT_Analyzer, &CLSID_ZalAnalyzer>,
//    public WordFormCollection,
    public IDispatchImpl<WordFormCollection, &IID_IAnalyzer, &LIBID_MainLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CT_Analyzer()
	{
        wstring arr_strMainSymbol[ ] = { L"м", L"мо", L"ж", L"жо", L"с", L"со", L"мо-жо", L"мн.",
            L"мн. неод.", L"мн. одуш.", L"мн. _от_", L"п", L"мс", L"мс-п", L"мс-предик.", L"числ.", L"числ.-п", 
            L"св", L"нсв", L"св-нсв", L"н", L"предл.", L"союз", L"предик.", L"вводн.", L"сравн.", 
            L"част.", L"межд.", L"NULL"  };
        
        for (ET_MainSymbol eo_ms = MS_START; eo_ms < MS_END; ++eo_ms)
        {
            map_MainSymbol[arr_strMainSymbol[eo_ms]] = eo_ms; 
        }
	}

    DECLARE_REGISTRY_RESOURCEID(IDR_ANALYZER)
    //DECLARE_NO_REGISTRY()


    BEGIN_COM_MAP(CT_Analyzer)
	    COM_INTERFACE_ENTRY(IAnalyzer)
//	    COM_INTERFACE_ENTRY(IError)
        COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
        pco_db = NULL;
        sp_dict = NULL;
		return S_OK;
	}

	void FinalRelease()
	{
        delete pco_db;
	}

public:

//  IAnalyze
    STDMETHOD (put_DbPath) (BSTR bstr_Path);
    STDMETHOD (PrepareLexeme) (__int64 ll_Lexeme_id, BOOL b_Stress);
    STDMETHOD (PrepareLexemes) (__int64 ll_First_Lexeme_id, __int64 ll_Last_Lexeme_id, BOOL b_Stress);
    STDMETHOD (Analyze) (BSTR bstr_Wordform);
//

    wstring str_DbPath;

    int i_Analyze(wstring str_wordform, vector<CT_Hasher>* pvec_possible_wordforms, BOOL b_guess);
    int i_LookUpStems(vector<int>* pvec_stems_id, wstring str_left, int i_StressPosStem);
    int i_CheckEndings(vector<CT_Hasher>* pvec_possible_wordforms, vector<int>* pvec_stems_id, wstring str_left, wstring str_right, int i_StressPosEnding);
    int i_ClassifyStems();
    HRESULT h_AddClassifyingCategories(CT_Hasher* pco_wf);

private:
    CT_Sqlite* pco_db;
    CComPtr<IDictionary> sp_dict;
    
    map<wstring, ET_MainSymbol> map_MainSymbol;
    
    wstring str_InsertStress(int i_letter, wstring str_);
    int i_DeleteStress(wstring& str_);
    void v_DeleteRepeats(vector<wstring>& vec_strings);
    HRESULT h_Hasher2Wordform (const wstring& str_wordform,
                                CT_Hasher co_from,
                                CComObject<CT_WordForm> *& pco_to);
    int i_LCP(wstring* str_words, wstring** str_pfx, int i_words, int i_pfx);

    // Helper functions for common DB queries
    // (Stored in AnalyzerQueries.cpp)
    int i_LastID(wstring str_TableName);
    void v_LongStemsBySubtable(int i_subtable, int i_min_len, vector<wstring>& vec_stems);
    void v_InsertCommonSfx(wstring **parr_str_sfx, int i_sfx, int i_subtable);
    void v_InsertStemsAndLinks(wstring **parr_str_stems, int *arr_i_subtable_id, int i_stems, __int64 ll_lexeme_id);
};

OBJECT_ENTRY_AUTO(__uuidof(ZalAnalyzer), CT_Analyzer)
