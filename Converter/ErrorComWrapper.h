#pragma once
#include "resource.h"       // main symbols
#include "ConversionLib_i.h"

using namespace std;

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

class ATL_NO_VTABLE CT_ErrorComWrapper :
    public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CT_ErrorComWrapper, &CLSID_ZalError>,
    public IDispatchImpl<IError, &IID_IError, &LIBID_ZalConversionLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{

public:
    CT_ErrorComWrapper()
	{
	}

    DECLARE_NO_REGISTRY()
//    DECLARE_REGISTRY_RESOURCEID(IDR_ERROR)

BEGIN_COM_MAP(CT_ErrorComWrapper)
    COM_INTERFACE_ENTRY(IError)
    COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
        return S_OK;
	}

	void FinalRelease()
    {}

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
};

OBJECT_ENTRY_AUTO(__uuidof(ZalError), CT_ErrorComWrapper)
