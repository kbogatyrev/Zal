#pragma once
#include "resource.h"
#include "ConversionLib_i.h"

class ATL_NO_VTABLE CT_ZalSourceReader :
    public IConnectionPointContainerImpl<CT_ZalSourceReader>,
    public IConnectionPointImpl<CT_ZalSourceReader, &IID_IZalNotification, CComDynamicUnkArray>,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CT_ZalSourceReader, &CLSID_ZalSourceReader>,
    public IZalSourceReader
{
public:
	CT_ZalSourceReader()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ZALSOURCEREADER)


BEGIN_COM_MAP(CT_ZalSourceReader)
    COM_INTERFACE_ENTRY(IZalSourceReader)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CT_ZalSourceReader)
    CONNECTION_POINT_ENTRY(IID_IZalNotification)
END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void FinalRelease();

protected:
    HRESULT StatusUpdate (int i_progress);
    HRESULT StatusCheck (BOOL& b_cancel);

public:
    STDMETHOD (ProcessSourceFile) (BSTR bstr_sourcePath,
                                   BSTR bstr_dbPath,
                                   int i_maxEntries);

};

OBJECT_ENTRY_AUTO(__uuidof(ZalSourceReader), CT_ZalSourceReader)
