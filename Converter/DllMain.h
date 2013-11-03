// dllmain.h : Declaration of module class.

class CConversionLibModule : public CAtlDllModuleT< CConversionLibModule >
{
public :
    DECLARE_LIBID(LIBID_ZalConversionLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_CONVERSIONLIB, "{97F389FC-5341-49DE-BFBD-E4C9E174F501}")
};

extern class CConversionLibModule _AtlModule;
