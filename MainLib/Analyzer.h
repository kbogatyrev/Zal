#include "Analyzer_Helpers.h"
#include "Lexeme.h"
#include "WordForm.h"
//#include <unordered_map>

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

class ATL_NO_VTABLE CAnalyzer :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAnalyzer, &CLSID_ZalAnalyzer>,
//    public WordFormCollection,
    public IDispatchImpl<WordFormCollection, &IID_IAnalyzer, &LIBID_MainLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CAnalyzer()
	{}

    DECLARE_REGISTRY_RESOURCEID(IDR_ANALYZER)
    //DECLARE_NO_REGISTRY()


    BEGIN_COM_MAP(CAnalyzer)
	    COM_INTERFACE_ENTRY(IAnalyzer)
//	    COM_INTERFACE_ENTRY(IError)
        COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
        Init();
        m_pDb = NULL;
        arr_freq_endings = NULL;
		return S_OK;
	}

	void FinalRelease()
	{
        delete m_pDb;
        if (arr_freq_endings != NULL)
        {
            delete[] arr_freq_endings;
        }
	}

public:

//  IAnalyze
    STDMETHOD (LoadDb) (BSTR bstrPath);
    STDMETHOD (Analyze) (BSTR bstrWordform);
//

    CEString m_sDbPath;

    int iAnalyze(CEString str_wordform, vector<CHasher>& vecPossibleWordforms, BOOL bGuess);
    int iLookUpStems(vector<stStemLinks>& vec_stems,
                     CEString str_left,
                     int i_StressPosStem);
    int iCheckEndings(vector<CHasher>& vec_possible_wordforms,
                      vector<stStemLinks>& vec_stems,
                      CEString str_left,
                      CEString str_right,
                      int i_StressPosEnding);
    HRESULT hAddClassifyingCategories(CHasher* pco_wf);

private:
    CSqlite* m_pDb;
    
    map<CEString, ET_MainSymbol> m_mapMainSymbol;
    
    //unordered_multimap<wstring, stStemLinks> umap_freq_stems;
    multimap<CEString, stStemLinks> umap_freq_stems;
    //unordered_multimap<wstring, int> umap_endings2subtbl;
    multimap<CEString, int> umap_endings2subtbl;
    CEndingsTable* arr_freq_endings;
    
    void Init();
    void DeleteRepeats(vector<CEString>& vec_strings);
    HRESULT hHasher2Wordform (const CEString& str_wordform,
                              CHasher co_from,
                              CComObject<CWordForm> *& pco_to);
    bool bIsValidLemma(CEString str_wf);
    bool bContainsPlausibleVariants(vector<CHasher>& pvec_possible_wordforms);
    int iPlausibility(CHasher co_wf);
    void LeaveMostPlausible(vector<CHasher>& pvec_possible_wordforms);
};

OBJECT_ENTRY_AUTO(__uuidof(ZalAnalyzer), CAnalyzer)
