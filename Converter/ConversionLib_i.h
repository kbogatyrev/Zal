

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Mon Nov 14 07:27:34 2016
 */
/* Compiler settings for .\ConversionLib.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ConversionLib_i_h__
#define __ConversionLib_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IError_FWD_DEFINED__
#define __IError_FWD_DEFINED__
typedef interface IError IError;
#endif 	/* __IError_FWD_DEFINED__ */


#ifndef __IZalSourceReader_FWD_DEFINED__
#define __IZalSourceReader_FWD_DEFINED__
typedef interface IZalSourceReader IZalSourceReader;
#endif 	/* __IZalSourceReader_FWD_DEFINED__ */


#ifndef __IZalNotification2_FWD_DEFINED__
#define __IZalNotification2_FWD_DEFINED__
typedef interface IZalNotification2 IZalNotification2;
#endif 	/* __IZalNotification2_FWD_DEFINED__ */


#ifndef __ZalError_FWD_DEFINED__
#define __ZalError_FWD_DEFINED__

#ifdef __cplusplus
typedef class ZalError ZalError;
#else
typedef struct ZalError ZalError;
#endif /* __cplusplus */

#endif 	/* __ZalError_FWD_DEFINED__ */


#ifndef __ZalSourceReader_FWD_DEFINED__
#define __ZalSourceReader_FWD_DEFINED__

#ifdef __cplusplus
typedef class ZalSourceReader ZalSourceReader;
#else
typedef struct ZalSourceReader ZalSourceReader;
#endif /* __cplusplus */

#endif 	/* __ZalSourceReader_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IError_INTERFACE_DEFINED__
#define __IError_INTERFACE_DEFINED__

/* interface IError */
/* [unique][helpstring][nonextensible][uuid][object] */ 


EXTERN_C const IID IID_IError;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A88EFFF9-8C28-421E-B101-8ECA30920860")
    IError : public IDispatch
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LastError( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IErrorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IError * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IError * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IError * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IError * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IError * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IError * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IError * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LastError )( 
            IError * This,
            /* [retval][out] */ BSTR *pVal);
        
        END_INTERFACE
    } IErrorVtbl;

    interface IError
    {
        CONST_VTBL struct IErrorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IError_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IError_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IError_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IError_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IError_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IError_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IError_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IError_get_LastError(This,pVal)	\
    ( (This)->lpVtbl -> get_LastError(This,pVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IError_INTERFACE_DEFINED__ */


#ifndef __IZalSourceReader_INTERFACE_DEFINED__
#define __IZalSourceReader_INTERFACE_DEFINED__

/* interface IZalSourceReader */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IZalSourceReader;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("33D63849-08AD-4261-B09D-1A60217ED40A")
    IZalSourceReader : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE PreprocessSourceFile( 
            /* [in] */ BSTR bstr_sourcePath,
            /* [in] */ BSTR bstr_outputDir) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ConvertSourceFile( 
            /* [in] */ BSTR bstr_sourcePath,
            /* [in] */ BSTR bstr_dbPath,
            /* [in] */ BSTR bstr_unprocessedPath,
            /* [in] */ int i_maxEntries,
            /* [in] */ BOOL b_Endings) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SearchSourceFile( 
            /* [in] */ BSTR bstr_sourcePath,
            /* [in] */ BSTR bstr_searchString,
            /* [in] */ BOOL b_regexSearch) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SaveOutput( 
            /* [in] */ BSTR bstr_path) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IZalSourceReaderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IZalSourceReader * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IZalSourceReader * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IZalSourceReader * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *PreprocessSourceFile )( 
            IZalSourceReader * This,
            /* [in] */ BSTR bstr_sourcePath,
            /* [in] */ BSTR bstr_outputDir);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ConvertSourceFile )( 
            IZalSourceReader * This,
            /* [in] */ BSTR bstr_sourcePath,
            /* [in] */ BSTR bstr_dbPath,
            /* [in] */ BSTR bstr_unprocessedPath,
            /* [in] */ int i_maxEntries,
            /* [in] */ BOOL b_Endings);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SearchSourceFile )( 
            IZalSourceReader * This,
            /* [in] */ BSTR bstr_sourcePath,
            /* [in] */ BSTR bstr_searchString,
            /* [in] */ BOOL b_regexSearch);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SaveOutput )( 
            IZalSourceReader * This,
            /* [in] */ BSTR bstr_path);
        
        END_INTERFACE
    } IZalSourceReaderVtbl;

    interface IZalSourceReader
    {
        CONST_VTBL struct IZalSourceReaderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IZalSourceReader_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IZalSourceReader_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IZalSourceReader_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IZalSourceReader_PreprocessSourceFile(This,bstr_sourcePath,bstr_outputDir)	\
    ( (This)->lpVtbl -> PreprocessSourceFile(This,bstr_sourcePath,bstr_outputDir) ) 

#define IZalSourceReader_ConvertSourceFile(This,bstr_sourcePath,bstr_dbPath,bstr_unprocessedPath,i_maxEntries,b_Endings)	\
    ( (This)->lpVtbl -> ConvertSourceFile(This,bstr_sourcePath,bstr_dbPath,bstr_unprocessedPath,i_maxEntries,b_Endings) ) 

#define IZalSourceReader_SearchSourceFile(This,bstr_sourcePath,bstr_searchString,b_regexSearch)	\
    ( (This)->lpVtbl -> SearchSourceFile(This,bstr_sourcePath,bstr_searchString,b_regexSearch) ) 

#define IZalSourceReader_SaveOutput(This,bstr_path)	\
    ( (This)->lpVtbl -> SaveOutput(This,bstr_path) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IZalSourceReader_INTERFACE_DEFINED__ */


#ifndef __IZalNotification2_INTERFACE_DEFINED__
#define __IZalNotification2_INTERFACE_DEFINED__

/* interface IZalNotification2 */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IZalNotification2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DD2CCF34-A642-495d-9DA9-7B9666AC8D90")
    IZalNotification2 : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ProgressNotification( 
            /* [in] */ int i_progress) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ShowCurrentWord( 
            /* [in] */ BSTR bstr_word) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE StatusCheck( 
            /* [out] */ BOOL *b_cancel) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IZalNotification2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IZalNotification2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IZalNotification2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IZalNotification2 * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ProgressNotification )( 
            IZalNotification2 * This,
            /* [in] */ int i_progress);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ShowCurrentWord )( 
            IZalNotification2 * This,
            /* [in] */ BSTR bstr_word);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *StatusCheck )( 
            IZalNotification2 * This,
            /* [out] */ BOOL *b_cancel);
        
        END_INTERFACE
    } IZalNotification2Vtbl;

    interface IZalNotification2
    {
        CONST_VTBL struct IZalNotification2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IZalNotification2_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IZalNotification2_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IZalNotification2_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IZalNotification2_ProgressNotification(This,i_progress)	\
    ( (This)->lpVtbl -> ProgressNotification(This,i_progress) ) 

#define IZalNotification2_ShowCurrentWord(This,bstr_word)	\
    ( (This)->lpVtbl -> ShowCurrentWord(This,bstr_word) ) 

#define IZalNotification2_StatusCheck(This,b_cancel)	\
    ( (This)->lpVtbl -> StatusCheck(This,b_cancel) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IZalNotification2_INTERFACE_DEFINED__ */



#ifndef __ZalConversionLib_LIBRARY_DEFINED__
#define __ZalConversionLib_LIBRARY_DEFINED__

/* library ZalConversionLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ZalConversionLib;

EXTERN_C const CLSID CLSID_ZalError;

#ifdef __cplusplus

class DECLSPEC_UUID("0BB9FE60-72F9-43ab-AABA-970D1C0F79B5")
ZalError;
#endif

EXTERN_C const CLSID CLSID_ZalSourceReader;

#ifdef __cplusplus

class DECLSPEC_UUID("7EC2AD6C-8885-46cb-B72A-8F2087F24CE4")
ZalSourceReader;
#endif
#endif /* __ZalConversionLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


