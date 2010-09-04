#pragma once
#include "resource.h"       // main symbols
#include "MainLib_i.h"

using namespace std;
//using namespace std::tr1;

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

class CT_StatusUpdate : public CT_ProgressCallback
{
    friend class CT_SqliteComWrapper;

    virtual int operator()(int i_percentDone) const;

private:
    CT_SqliteComWrapper * pco_Parent;
};

class ATL_NO_VTABLE CT_SqliteComWrapper :
    public IConnectionPointContainerImpl<CT_SqliteComWrapper>,
    public IConnectionPointImpl<CT_SqliteComWrapper, &IID_IZalNotification, CComDynamicUnkArray>,
    public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CT_SqliteComWrapper, &CLSID_ZalSqliteWrapper>,
    public IDispatchImpl<ISqliteWrapper, &IID_ISqliteWrapper, &LIBID_MainLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{

public:
    CT_SqliteComWrapper()
	{
	}

    DECLARE_REGISTRY_RESOURCEID(IDR_SQLITEWRAPPER)

    BEGIN_COM_MAP(CT_SqliteComWrapper)
        COM_INTERFACE_ENTRY(ISqliteWrapper)
        COM_INTERFACE_ENTRY(IDispatch)
        COM_INTERFACE_ENTRY(IConnectionPointContainer)
    END_COM_MAP()

    BEGIN_CONNECTION_POINT_MAP(CT_SqliteComWrapper)
        CONNECTION_POINT_ENTRY (IID_IZalNotification)
    END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
        pco_Db = NULL;
        return S_OK;
	}

	void FinalRelease()
    {}

public:
// ISqliteWrapper
    STDMETHOD (put_DbPath) (BSTR bstr_dbPath);
    STDMETHOD (TableExists) (BSTR bstr_table, BOOL * b_exists);
    STDMETHOD (Export) (BSTR bstr_path, SAFEARRAY * sarr_names);
    STDMETHOD (Import) (BSTR bstr_path);
    STDMETHOD (StatusUpdate) (int i_progress);

private:
    wstring str_DbPath;
    CT_Sqlite * pco_Db;
};

OBJECT_ENTRY_AUTO(__uuidof(ZalSqliteWrapper), CT_SqliteComWrapper)




