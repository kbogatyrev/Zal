// Dictionary.h : Declaration of the CDictionary

#pragma once

#include "resource.h"       // main symbols
#include "MainLib_i.h"

#include "VCUE_Collection.h"
#include "VCUE_Copy.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

namespace TestDataVector
{
// We always need to provide the following information
//typedef std::vector<CComVariant> ContainerType;
//typedef VARIANT	ExposedType;
//typedef IEnumVARIANT EnumeratorInterface;
//typedef IZalCollection CollectionInterface;
//typedef IDictionary DCollectionInterface;

// Typically the copy policy can be calculated from the typedefs defined above:
//typedef VCUE::GenericCopy<ExposedType, ContainerType::value_type> CopyType;

// However, we may want to use a different class, as in this case:
// typedef VCUE::MapCopy<ContainerType, ExposedType>				CopyType;
	// (The advantage of MapCopy is that we don't need to provide implementations 
	//  of GenericCopy for all the different pairs of key and value types)

// Now we have all the information we need to fill in the template arguments on the implementation classes
typedef CComEnumOnSTL <IEnumVARIANT, 
                       &__uuidof(IEnumVARIANT), 
                       VARIANT,
                       VCUE::GenericCopy<VARIANT, 
                                         std::vector<CComVariant>::value_type>,
                       std::vector<CComVariant> > EnumeratorType;

typedef VCUE::ICollectionOnSTLCopyImpl <ILexemeCollection, 
                                        std::vector<CComVariant>, 
                                        VARIANT,
                                        VCUE::GenericCopy<VARIANT, 
                                                          std::vector<CComVariant>::value_type>,
                                        EnumeratorType> TestDataCollection;
}

using namespace TestDataVector;

class ATL_NO_VTABLE CTestData :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTestData, &CLSID_ZalStoredLexemeData>,
    public IDispatchImpl<TestDataCollection, &IID_ILexemeCollection, &LIBID_MainLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
    CTestData()
	{
	}

    DECLARE_REGISTRY_RESOURCEID(IDR_TESTDATA)

    BEGIN_COM_MAP(CTestData)
        COM_INTERFACE_ENTRY(ILexemeCollection)
	    COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

    DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
        m_pDb = NULL;
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
    // ILexemeCollection
    STDMETHOD (put_DbPath) (BSTR bstr_path);
    STDMETHOD (LoadStoredLexemes) ();
    STDMETHOD (DeleteStoredLexeme) (BSTR bstrLexeme);

private:
    HRESULT hGetStoredLexemeData (const CEString& sSelect);

private:
    CEString m_sDbPath;
    CSqlite * m_pDb;

};

OBJECT_ENTRY_AUTO(__uuidof(ZalStoredLexemeData), CTestData)
