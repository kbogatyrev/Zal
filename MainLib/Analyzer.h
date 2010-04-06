#include "Analyzer_Helpers.h"
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
                                        EnumeratorType> WordformCollection;

class ATL_NO_VTABLE CT_Analyzer :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CT_Analyzer, &CLSID_ZalAnalyzer>,
    public IDispatchImpl<WordformCollection, &IID_IAnalyzer, &LIBID_MainLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CT_Analyzer()
	{
	}

    DECLARE_REGISTRY_RESOURCEID(IDR_ANALYZER)
    //DECLARE_NO_REGISTRY()


    BEGIN_COM_MAP(CT_Analyzer)
	    COM_INTERFACE_ENTRY(IAnalyzer)
        COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
        pco_db = NULL;
		return S_OK;
	}

	void FinalRelease()
	{
        delete pco_db;
	}

public:
    STDMETHOD (put_DbPath) (BSTR bstr_Path);
    STDMETHOD (PrepareLexeme) (__int64 ll_Lexeme_id, BOOL b_Stress);
    STDMETHOD (PrepareLexemes) (__int64 ll_First_Lexeme_id, __int64 ll_Last_Lexeme_id, BOOL b_Stress);
    STDMETHOD (Analyze) (BSTR bstr_Wordform);

    wstring str_DbPath;

    int i_Analyze(wstring str_wordform, vector<CComObject<CT_WordForm>*>* pvec_possible_wordforms);
    int i_LookUpStems(vector<int>* pvec_stems_id, wstring str_left);
    int i_CheckEndings(vector<CComObject<CT_WordForm>*>* pvec_possible_wordforms, vector<int>* pvec_stems_id, wstring str_left, wstring str_right);

private:
    CT_Sqlite* pco_db;
    wstring str_InsertStress(int i_letter, wstring str_);
    int i_LCP(wstring* str_words, wstring** str_pfx, int i_words, int i_pfx);
};

OBJECT_ENTRY_AUTO(__uuidof(ZalAnalyzer), CT_Analyzer)
