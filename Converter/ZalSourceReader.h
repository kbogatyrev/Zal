#pragma once
#include "resource.h"
#include "ConversionLib_i.h"

class ATL_NO_VTABLE CT_ZalSourceReader :
    public IConnectionPointContainerImpl<CT_ZalSourceReader>,
    public IConnectionPointImpl<CT_ZalSourceReader, &IID_IZalNotification2, CComDynamicUnkArray>,
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
    CONNECTION_POINT_ENTRY(IID_IZalNotification2)
END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void FinalRelease();

protected:
    HRESULT StatusUpdate (int i_progress);
    HRESULT ShowCurrentWord (BSTR bstr_word);
    HRESULT StatusCheck (BOOL& b_cancel);

    bool b_CheckSemicolons (wstring& str_source, bool& b_suspiciousEntry);
    bool b_CheckSquareBrackets (const wstring& str_source, bool& b_suspiciousEntry);
    bool b_Show (const wstring& str_msg);

private:
    wstring str_TempFile;
    set<wstring> set_MainSymbols;

public:
    STDMETHOD (ConvertSourceFile) (BSTR bstr_sourcePath,
                                   BSTR bstr_dbPath,
                                   BSTR bstr_unprocessedPath,
                                   int i_maxEntries,
                                   BOOL b_endings);
    STDMETHOD (PreprocessSourceFile) (BSTR bstr_sourcePath, BSTR bstr_outputDir);
    STDMETHOD (SearchSourceFile) (BSTR bstr_sourcePath, BSTR bstr_searchString, BOOL b_regexSearch);
    STDMETHOD (SaveOutput) (BSTR bstr_path);

};

OBJECT_ENTRY_AUTO(__uuidof(ZalSourceReader), CT_ZalSourceReader)
