/*** Autogenerated by WIDL 8.2 from dlls/vbscript/vbsglobal.idl - Do not edit ***/

#ifdef _WIN32
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif
#include <rpc.h>
#include <rpcndr.h>
#endif

#ifndef COM_NO_WINDOWS_H
#include <windows.h>
#include <ole2.h>
#endif

#ifndef __vbsglobal_h__
#define __vbsglobal_h__

#ifndef __WIDL_INLINE
#if defined(__cplusplus) || defined(_MSC_VER)
#define __WIDL_INLINE inline
#elif defined(__GNUC__)
#define __WIDL_INLINE __inline__
#endif
#endif

/* Forward declarations */

#ifndef __GlobalObj_FWD_DEFINED__
#define __GlobalObj_FWD_DEFINED__
typedef interface GlobalObj GlobalObj;
#ifdef __cplusplus
interface GlobalObj;
#endif /* __cplusplus */
#endif

#ifndef __ErrObj_FWD_DEFINED__
#define __ErrObj_FWD_DEFINED__
typedef interface ErrObj ErrObj;
#ifdef __cplusplus
interface ErrObj;
#endif /* __cplusplus */
#endif

/* Headers for imported files */

#include <oaidl.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __VBScript_Global_LIBRARY_DEFINED__
#define __VBScript_Global_LIBRARY_DEFINED__

DEFINE_GUID(LIBID_VBScript_Global, 0x3eef9758, 0x35fc, 0x11d1, 0x8c,0xe4, 0x00,0xc0,0x4f,0xc2,0xb1,0x85);

/*****************************************************************************
 * GlobalObj dispinterface
 */
#ifndef __GlobalObj_DISPINTERFACE_DEFINED__
#define __GlobalObj_DISPINTERFACE_DEFINED__

DEFINE_GUID(DIID_GlobalObj, 0x3eef9758, 0x35fc, 0x11d1, 0x8c,0xe4, 0x00,0xc0,0x4f,0xc2,0xb1,0x86);
#if defined(__cplusplus) && !defined(CINTERFACE)
MIDL_INTERFACE("3eef9758-35fc-11d1-8ce4-00c04fc2b186")
GlobalObj : public IDispatch
{
};
#ifdef __CRT_UUID_DECL
__CRT_UUID_DECL(GlobalObj, 0x3eef9758, 0x35fc, 0x11d1, 0x8c,0xe4, 0x00,0xc0,0x4f,0xc2,0xb1,0x86)
#endif
#else
typedef struct GlobalObjVtbl {
    BEGIN_INTERFACE

    /*** IUnknown methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(
        GlobalObj *This,
        REFIID riid,
        void **ppvObject);

    ULONG (STDMETHODCALLTYPE *AddRef)(
        GlobalObj *This);

    ULONG (STDMETHODCALLTYPE *Release)(
        GlobalObj *This);

    /*** IDispatch methods ***/
    HRESULT (STDMETHODCALLTYPE *GetTypeInfoCount)(
        GlobalObj *This,
        UINT *pctinfo);

    HRESULT (STDMETHODCALLTYPE *GetTypeInfo)(
        GlobalObj *This,
        UINT iTInfo,
        LCID lcid,
        ITypeInfo **ppTInfo);

    HRESULT (STDMETHODCALLTYPE *GetIDsOfNames)(
        GlobalObj *This,
        REFIID riid,
        LPOLESTR *rgszNames,
        UINT cNames,
        LCID lcid,
        DISPID *rgDispId);

    HRESULT (STDMETHODCALLTYPE *Invoke)(
        GlobalObj *This,
        DISPID dispIdMember,
        REFIID riid,
        LCID lcid,
        WORD wFlags,
        DISPPARAMS *pDispParams,
        VARIANT *pVarResult,
        EXCEPINFO *pExcepInfo,
        UINT *puArgErr);

    END_INTERFACE
} GlobalObjVtbl;

interface GlobalObj {
    CONST_VTBL GlobalObjVtbl* lpVtbl;
};

#ifdef COBJMACROS
#ifndef WIDL_C_INLINE_WRAPPERS
/*** IUnknown methods ***/
#define GlobalObj_QueryInterface(This,riid,ppvObject) (This)->lpVtbl->QueryInterface(This,riid,ppvObject)
#define GlobalObj_AddRef(This) (This)->lpVtbl->AddRef(This)
#define GlobalObj_Release(This) (This)->lpVtbl->Release(This)
/*** IDispatch methods ***/
#define GlobalObj_GetTypeInfoCount(This,pctinfo) (This)->lpVtbl->GetTypeInfoCount(This,pctinfo)
#define GlobalObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo) (This)->lpVtbl->GetTypeInfo(This,iTInfo,lcid,ppTInfo)
#define GlobalObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) (This)->lpVtbl->GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)
#define GlobalObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) (This)->lpVtbl->Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)
#else
/*** IUnknown methods ***/
static __WIDL_INLINE HRESULT GlobalObj_QueryInterface(GlobalObj* This,REFIID riid,void **ppvObject) {
    return This->lpVtbl->QueryInterface(This,riid,ppvObject);
}
static __WIDL_INLINE ULONG GlobalObj_AddRef(GlobalObj* This) {
    return This->lpVtbl->AddRef(This);
}
static __WIDL_INLINE ULONG GlobalObj_Release(GlobalObj* This) {
    return This->lpVtbl->Release(This);
}
/*** IDispatch methods ***/
static __WIDL_INLINE HRESULT GlobalObj_GetTypeInfoCount(GlobalObj* This,UINT *pctinfo) {
    return This->lpVtbl->GetTypeInfoCount(This,pctinfo);
}
static __WIDL_INLINE HRESULT GlobalObj_GetTypeInfo(GlobalObj* This,UINT iTInfo,LCID lcid,ITypeInfo **ppTInfo) {
    return This->lpVtbl->GetTypeInfo(This,iTInfo,lcid,ppTInfo);
}
static __WIDL_INLINE HRESULT GlobalObj_GetIDsOfNames(GlobalObj* This,REFIID riid,LPOLESTR *rgszNames,UINT cNames,LCID lcid,DISPID *rgDispId) {
    return This->lpVtbl->GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId);
}
static __WIDL_INLINE HRESULT GlobalObj_Invoke(GlobalObj* This,DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,DISPPARAMS *pDispParams,VARIANT *pVarResult,EXCEPINFO *pExcepInfo,UINT *puArgErr) {
    return This->lpVtbl->Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr);
}
#endif
#endif

#endif

#endif  /* __GlobalObj_DISPINTERFACE_DEFINED__ */

/*****************************************************************************
 * ErrObj dispinterface
 */
#ifndef __ErrObj_DISPINTERFACE_DEFINED__
#define __ErrObj_DISPINTERFACE_DEFINED__

DEFINE_GUID(DIID_ErrObj, 0x3eef9758, 0x35fc, 0x11d1, 0x8c,0xe4, 0x00,0xc0,0x4f,0xc2,0xb1,0x87);
#if defined(__cplusplus) && !defined(CINTERFACE)
MIDL_INTERFACE("3eef9758-35fc-11d1-8ce4-00c04fc2b187")
ErrObj : public IDispatch
{
};
#ifdef __CRT_UUID_DECL
__CRT_UUID_DECL(ErrObj, 0x3eef9758, 0x35fc, 0x11d1, 0x8c,0xe4, 0x00,0xc0,0x4f,0xc2,0xb1,0x87)
#endif
#else
typedef struct ErrObjVtbl {
    BEGIN_INTERFACE

    /*** IUnknown methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(
        ErrObj *This,
        REFIID riid,
        void **ppvObject);

    ULONG (STDMETHODCALLTYPE *AddRef)(
        ErrObj *This);

    ULONG (STDMETHODCALLTYPE *Release)(
        ErrObj *This);

    /*** IDispatch methods ***/
    HRESULT (STDMETHODCALLTYPE *GetTypeInfoCount)(
        ErrObj *This,
        UINT *pctinfo);

    HRESULT (STDMETHODCALLTYPE *GetTypeInfo)(
        ErrObj *This,
        UINT iTInfo,
        LCID lcid,
        ITypeInfo **ppTInfo);

    HRESULT (STDMETHODCALLTYPE *GetIDsOfNames)(
        ErrObj *This,
        REFIID riid,
        LPOLESTR *rgszNames,
        UINT cNames,
        LCID lcid,
        DISPID *rgDispId);

    HRESULT (STDMETHODCALLTYPE *Invoke)(
        ErrObj *This,
        DISPID dispIdMember,
        REFIID riid,
        LCID lcid,
        WORD wFlags,
        DISPPARAMS *pDispParams,
        VARIANT *pVarResult,
        EXCEPINFO *pExcepInfo,
        UINT *puArgErr);

    END_INTERFACE
} ErrObjVtbl;

interface ErrObj {
    CONST_VTBL ErrObjVtbl* lpVtbl;
};

#ifdef COBJMACROS
#ifndef WIDL_C_INLINE_WRAPPERS
/*** IUnknown methods ***/
#define ErrObj_QueryInterface(This,riid,ppvObject) (This)->lpVtbl->QueryInterface(This,riid,ppvObject)
#define ErrObj_AddRef(This) (This)->lpVtbl->AddRef(This)
#define ErrObj_Release(This) (This)->lpVtbl->Release(This)
/*** IDispatch methods ***/
#define ErrObj_GetTypeInfoCount(This,pctinfo) (This)->lpVtbl->GetTypeInfoCount(This,pctinfo)
#define ErrObj_GetTypeInfo(This,iTInfo,lcid,ppTInfo) (This)->lpVtbl->GetTypeInfo(This,iTInfo,lcid,ppTInfo)
#define ErrObj_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) (This)->lpVtbl->GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)
#define ErrObj_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) (This)->lpVtbl->Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)
#else
/*** IUnknown methods ***/
static __WIDL_INLINE HRESULT ErrObj_QueryInterface(ErrObj* This,REFIID riid,void **ppvObject) {
    return This->lpVtbl->QueryInterface(This,riid,ppvObject);
}
static __WIDL_INLINE ULONG ErrObj_AddRef(ErrObj* This) {
    return This->lpVtbl->AddRef(This);
}
static __WIDL_INLINE ULONG ErrObj_Release(ErrObj* This) {
    return This->lpVtbl->Release(This);
}
/*** IDispatch methods ***/
static __WIDL_INLINE HRESULT ErrObj_GetTypeInfoCount(ErrObj* This,UINT *pctinfo) {
    return This->lpVtbl->GetTypeInfoCount(This,pctinfo);
}
static __WIDL_INLINE HRESULT ErrObj_GetTypeInfo(ErrObj* This,UINT iTInfo,LCID lcid,ITypeInfo **ppTInfo) {
    return This->lpVtbl->GetTypeInfo(This,iTInfo,lcid,ppTInfo);
}
static __WIDL_INLINE HRESULT ErrObj_GetIDsOfNames(ErrObj* This,REFIID riid,LPOLESTR *rgszNames,UINT cNames,LCID lcid,DISPID *rgDispId) {
    return This->lpVtbl->GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId);
}
static __WIDL_INLINE HRESULT ErrObj_Invoke(ErrObj* This,DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,DISPPARAMS *pDispParams,VARIANT *pVarResult,EXCEPINFO *pExcepInfo,UINT *puArgErr) {
    return This->lpVtbl->Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr);
}
#endif
#endif

#endif

#endif  /* __ErrObj_DISPINTERFACE_DEFINED__ */

#endif /* __VBScript_Global_LIBRARY_DEFINED__ */
/* Begin additional prototypes for all interfaces */


/* End additional prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __vbsglobal_h__ */
