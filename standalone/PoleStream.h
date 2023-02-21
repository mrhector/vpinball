#if defined(__clang__)
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#pragma clang diagnostic ignored "-Wmissing-exception-spec"
#endif

#include "objidl.h"
#include "pole.h"

class PoleStream : public IStream {
public:
   void Init(POLE::Storage* pStorage, const char* szPath);

   HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
   ULONG STDMETHODCALLTYPE AddRef();
   ULONG STDMETHODCALLTYPE Release();

   STDMETHOD(Seek)(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition);
   STDMETHOD(SetSize)(ULARGE_INTEGER libNewSize);
   STDMETHOD(CopyTo)(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten);
   STDMETHOD(Commit)(DWORD grfCommitFlags);
   STDMETHOD(Revert)();
   STDMETHOD(LockRegion)(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
   STDMETHOD(UnlockRegion)(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
   STDMETHOD(Stat)(STATSTG *pstatstg, DWORD grfStatFlag);
   STDMETHOD(Clone)(IStream **ppstm);
   STDMETHOD(Read)(void *pv, ULONG cb, ULONG *pcbRead);
   STDMETHOD(Write)(const void *pv, ULONG cb, ULONG *pcbWritten);

private:
   POLE::Stream* m_pStream = NULL;

   ULONG m_dwRef = 0;
};