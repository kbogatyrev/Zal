#include "Analyzer_Helpers.h"
#include "Lexeme.h"
#include "WordForm.h"
#include <unordered_map>

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
        v_Init();
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
        arr_freq_endings = NULL;
		return S_OK;
	}

	void FinalRelease()
	{
        delete pco_db;
        if (arr_freq_endings != NULL)
        {
            delete[] arr_freq_endings;
        }
	}

public:

//  IAnalyze
    STDMETHOD (put_DbPath) (BSTR bstr_Path);
    STDMETHOD (Analyze) (BSTR bstr_Wordform);
//

    wstring str_DbPath;

    int i_Analyze(wstring str_wordform, vector<CT_Hasher>* pvec_possible_wordforms, BOOL b_guess);
    int i_LookUpStems(vector<struct_stem_links>* pvec_stems,
                      wstring str_left,
                      int i_StressPosStem);
    int i_CheckEndings(vector<CT_Hasher>* pvec_possible_wordforms,
                       vector<struct_stem_links>* pvec_stems,
                       wstring str_left,
                       wstring str_right,
                       int i_StressPosEnding);
    HRESULT h_AddClassifyingCategories(CT_Hasher* pco_wf);

private:
    CT_Sqlite* pco_db;
    
    map<wstring, ET_MainSymbol> map_MainSymbol;
    
    unordered_multimap<wstring, struct_stem_links> umap_freq_stems;
    unordered_multimap<wstring, int> umap_endings2subtbl;
    CT_EndingsTable* arr_freq_endings;
    
    void v_Init();
    void v_DeleteRepeats(vector<wstring>& vec_strings);
    HRESULT h_Hasher2Wordform (const wstring& str_wordform,
                               CT_Hasher co_from,
                               CComObject<CT_WordForm> *& pco_to);
    bool b_IsValidLemma(wstring str_wf);
    bool b_ContainsPlausibleVariants(vector<CT_Hasher>* pvec_possible_wordforms);
    int i_Plausibility(CT_Hasher co_wf);
    void v_LeaveMostPlausible(vector<CT_Hasher>* pvec_possible_wordforms);
};

OBJECT_ENTRY_AUTO(__uuidof(ZalAnalyzer), CT_Analyzer)
