#include "PoleStream.h"

void PoleStream::Init(POLE::Storage* pStorage, const char* szPath) 
{
   m_pStream = new POLE::Stream(pStorage, szPath);
}

STDMETHODIMP PoleStream::QueryInterface(REFIID iid, void **ppvObjOut) { return E_NOTIMPL; }

STDMETHODIMP_(ULONG) PoleStream::AddRef()
{
   m_dwRef++;

   return m_dwRef;
}

STDMETHODIMP_(ULONG) PoleStream::Release()
{
   m_dwRef--;

   if (m_dwRef == 0) {
      if (m_pStream) {
         delete m_pStream;
         m_pStream = NULL;
      }
   
      delete this;
   }

   return m_dwRef;
}

STDMETHODIMP PoleStream::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition)
{
   if (dwOrigin == STREAM_SEEK_SET)
      m_pStream->seek(dlibMove.QuadPart);
   else if (dwOrigin == STREAM_SEEK_CUR)
      m_pStream->seek(m_pStream->tell() + dlibMove.QuadPart);

   return S_OK;
}

STDMETHODIMP PoleStream::SetSize(ULARGE_INTEGER libNewSize) { return E_NOTIMPL; }
STDMETHODIMP PoleStream::CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten) { return E_NOTIMPL; }
STDMETHODIMP PoleStream::Commit(DWORD grfCommitFlags) { return E_NOTIMPL; }
STDMETHODIMP PoleStream::Revert() { return E_NOTIMPL; }
STDMETHODIMP PoleStream::LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) { return E_NOTIMPL; }
STDMETHODIMP PoleStream::UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) { return E_NOTIMPL; }

STDMETHODIMP PoleStream::Stat(STATSTG *pstatstg, DWORD grfStatFlag)
{
   pstatstg->cbSize.LowPart = m_pStream->size();
   return S_OK;
}

STDMETHODIMP PoleStream::Clone(IStream **ppstm) { return E_NOTIMPL; }

STDMETHODIMP PoleStream::Read(void *pv, ULONG cb, ULONG *pcbRead)
{
   *pcbRead = m_pStream->read((unsigned char*)pv, cb);
   return S_OK;
}

STDMETHODIMP PoleStream::Write(const void *pv, ULONG cb, ULONG *pcbWritten) { return E_NOTIMPL; }