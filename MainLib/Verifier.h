#pragma once
#include "resource.h"       // main symbols
#include "MainLib_i.h"

#include "VCUE_Collection.h"
#include "VCUE_Copy.h"

using namespace std;
using namespace std::tr1;

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// *** This was taken from ATLCollections Sample
// I'm not using some of the typedefs listed below 
// as they only make this convoluted code even more obscure.
// However, the MSFT's typedef names appear in comments. 
//                                                  -- kb

// We always need to provide the following information
//typedef std::vector<CComVariant> ContainerType;
//typedef VARIANT	ExposedType;
//typedef IEnumVARIANT EnumeratorInterface;

// Typically the copy policy can be calculated from the typedefs defined above:
//typedef VCUE::GenericCopy<ExposedType, ContainerType::value_type> CopyType;
// However, we may want to use a different class, as in this case:
//typedef VCUE::MapCopy<ContainerType, ExposedType>				CopyType;
	// (The advantage of MapCopy is that we don't need to provide implementations 
	//  of GenericCopy for all the different pairs of key and value types)

// Now we have all the information we need to fill in the template arguments on the implementation classes

/*
namespace TestResultsContainer
{
    typedef CComEnumOnSTL <IEnumVARIANT, 
                           &__uuidof(IEnumVARIANT), 
                           VARIANT,
                           VCUE::GenericCopy<VARIANT, 
                                             std::vector<CComVariant>::value_type>,
                           std::vector<CComVariant> > EnumeratorType;

    typedef VCUE::ICollectionOnSTLCopyImpl <IVerifier, 
                                            std::vector<CComVariant>, 
                                            VARIANT,
                                            VCUE::GenericCopy<VARIANT, 
                                                              std::vector<CComVariant>::value_type>,
                                            EnumeratorType> VectorCollection;

}   // namespace TestResultsContainer
*/

//using namespace TestResultsContainer;

class ATL_NO_VTABLE CT_Verifier :
    public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CT_Verifier, &CLSID_ZalVerifier>,
    public IVerifier,
    public IError
{
friend class CT_TestData;

public:
    CT_Verifier()
	{
	}

//    DECLARE_NO_REGISTRY()
    DECLARE_REGISTRY_RESOURCEID(IDR_VERIFIER)

BEGIN_COM_MAP(CT_Verifier)
    COM_INTERFACE_ENTRY(IVerifier)
    COM_INTERFACE_ENTRY(IError)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
        pco_Db = NULL;
        eo_Result = TEST_RESULT_UNDEFINED;
        
        return S_OK;
	}

	void FinalRelease()
    {}
//        delete pco_Db;
//	}

public:

// IError
    STDMETHOD (get_LastError) (BSTR * pbstr_description)
    {
        USES_CONVERSION;

        CT_Error * pco_error = CT_Error::pco_CreateInstance();
        if (!pco_error)
        {
            return E_POINTER;
        }

        CComBSTR sp_error (pco_error->str_GetLastError().c_str());
        *pbstr_description = sp_error.Detach();

        return S_OK;
    }

// IVerifier
    STDMETHOD (put_DbPath) (BSTR);

    STDMETHOD (put_Headword) (BSTR);
    STDMETHOD (get_Headword) (BSTR *);
    STDMETHOD (put_LexemeId) (ULONG);
    STDMETHOD (get_LexemeId) (ULONG *);
    STDMETHOD (Verify) (ULONG ul_lexemeId, ET_TestResult * pe_result);
    STDMETHOD (get_Result) (ET_TestResult *);

private:
    HRESULT h_LoadStoredForms();

private:
    ULONG ul_LexemeHash;
//    int i_DescriptorId;
    wstring str_Headword;
    CT_Sqlite * pco_Db;
    wstring str_DbPath;
    multimap<int, IWordForm *> mmap_StoredForms;
    ET_TestResult eo_Result;
};

OBJECT_ENTRY_AUTO(__uuidof(ZalVerifier), CT_Verifier)
