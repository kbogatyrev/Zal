#pragma once
#include "resource.h"
#include "ConversionLib_i.h"

class ATL_NO_VTABLE CZalSourceReader :
    public IConnectionPointContainerImpl<CZalSourceReader>,
    public IConnectionPointImpl<CZalSourceReader, &IID_IZalNotification2, CComDynamicUnkArray>,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CZalSourceReader, &CLSID_ZalSourceReader>,
    public IZalSourceReader
{
public:
	CZalSourceReader()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ZALSOURCEREADER)


BEGIN_COM_MAP(CZalSourceReader)
    COM_INTERFACE_ENTRY(IZalSourceReader)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(CZalSourceReader)
    CONNECTION_POINT_ENTRY(IID_IZalNotification2)
END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void FinalRelease();

protected:
    HRESULT StatusUpdate (int iProgress);
    HRESULT ShowCurrentWord (BSTR bstrWord);
    HRESULT StatusCheck (BOOL& bCancel);

    bool bCheckSemicolons (CEString& sSource, bool& bSuspiciousEntry);
    bool bCheckSquareBrackets (const CEString& sSource, bool& bSuspiciousEntry);
    bool bShow (const CEString& sMsg);

private:
    CEString m_sTempFile;
    set<CEString> m_setMainSymbols;

public:
    STDMETHOD (ConvertSourceFile) (BSTR bstrSourcePath,
                                   BSTR bstrDbPath,
                                   BSTR bstrUnprocessedPath,
                                   int iMaxEntries,
                                   BOOL bEndings);
    STDMETHOD (PreprocessSourceFile) (BSTR bstrSourcePath, BSTR bstrOutputDir);
    STDMETHOD (SearchSourceFile) (BSTR bstrSourcePath, BSTR bstrSearchString, BOOL bRegexSearch);
    STDMETHOD (SaveOutput) (BSTR bstrPath);

};

OBJECT_ENTRY_AUTO(__uuidof(ZalSourceReader), CZalSourceReader)
