// Dictionary.h : Declaration of the CDictionary

#pragma once
#include "resource.h"       // main symbols
#include "MainLib_i.h"

#include "VCUE_Collection.h"
#include "VCUE_Copy.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

namespace LexemeVector
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

typedef VCUE::ICollectionOnSTLCopyImpl <IDictionary, 
                                        std::vector<CComVariant>, 
                                        VARIANT,
                                        VCUE::GenericCopy<VARIANT, 
                                                          std::vector<CComVariant>::value_type>,
                                        EnumeratorType> LexemeCollection;
}

using namespace LexemeVector;

class ATL_NO_VTABLE CT_Dictionary :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CT_Dictionary, &CLSID_ZalDictionary>,
    public IError,
    public LexemeCollection
//    public IDispatchImpl<LexemeCollection, &IID_IDictionary, &LIBID_MainLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
friend class CT_Verifier;

public:
	CT_Dictionary()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DICTIONARY)


BEGIN_COM_MAP(CT_Dictionary)
	COM_INTERFACE_ENTRY(IDictionary)
	COM_INTERFACE_ENTRY(IError)
//    COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
        pco_Db = NULL;
		return S_OK;
	}

	void FinalRelease()
	{
        delete pco_Db;
	}

public:
    STDMETHOD (put_DbPath) (BSTR bstr_dbPath);
    STDMETHOD (GetLexeme) (LONG Id, ILexeme ** pp_lexeme);
    STDMETHOD (GetLexemesByGraphicStem) (BSTR bstr_key);
    STDMETHOD (GetLexemesByInitialForm) (BSTR bstr_key);

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

private:
    wstring str_DbPath;
    CT_Sqlite * pco_Db;

    HRESULT h_GetData (const wstring& str_select);
    HRESULT h_ReadFromDb (__int64 ll_lexemeId);
    HRESULT h_ReadFromDb (const CT_ExtString& str_stem);
};

OBJECT_ENTRY_AUTO(__uuidof(ZalDictionary), CT_Dictionary)
