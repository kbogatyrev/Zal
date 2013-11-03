// dllmain.h : Declaration of module class.

class CMainLibModule : public CAtlDllModuleT< CMainLibModule >
{
public :
    DECLARE_LIBID(LIBID_MainLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MAINLIB, "{C3BBBADA-274A-4BF4-9016-58733C4ECFB6}")
};

extern class CMainLibModule _AtlModule;
