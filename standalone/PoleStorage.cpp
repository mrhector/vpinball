#include "PoleStorage.h"
#include "PoleStream.h"

void PoleStorage::Init(string szName, string szPath)
{
   m_szName = szName;
   m_szPath = szPath;

   m_pStorage = new POLE::Storage(szName.c_str());
}

STDMETHODIMP PoleStorage::QueryInterface(REFIID iid, void **ppvObjOut) { return E_NOTIMPL; }

STDMETHODIMP_(ULONG) PoleStorage::AddRef()
{
   m_dwRef++;

   return m_dwRef;
}

STDMETHODIMP_(ULONG) PoleStorage::Release()
{
   m_dwRef--;

   if (m_dwRef == 0) {
      if (m_pStorage) {
         delete m_pStorage;
         m_pStorage = NULL;
       }

      delete this;
   }

   return m_dwRef;
}

STDMETHODIMP PoleStorage::CreateStream(LPCOLESTR pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStream **ppstm) { return E_NOTIMPL; }

STDMETHODIMP PoleStorage::OpenStream(LPCOLESTR pwcsName, void *reserved1, DWORD grfMode, DWORD reserved2, IStream **ppstm)
{
   char szName[1024];
   WideCharToMultiByte(CP_ACP, 0, pwcsName, -1, szName, sizeof(szName), NULL, NULL);

   string path = m_szPath;
   if (path.length() > 0)
      path.append("/");
   path.append(szName);

   if (m_pStorage->exists(path.c_str())) {
      PoleStream* stream = new PoleStream();
      stream->AddRef();

      stream->Init(m_pStorage, path.c_str());

      *ppstm = stream;

      return S_OK;
   }

   return E_FAIL;
}

STDMETHODIMP PoleStorage::CreateStorage(LPCOLESTR pwcsName, DWORD grfMode, DWORD dwStgFmt, DWORD reserved2, IStorage **ppstg) { return E_NOTIMPL; }

STDMETHODIMP PoleStorage::OpenStorage(LPCOLESTR pwcsName, IStorage *pstgPriority, DWORD grfMode, SNB snbExclude, DWORD reserved, IStorage **ppstg)
{
   PoleStorage* pStorage = NULL;

   char szName[1024];
   WideCharToMultiByte(CP_ACP, 0, pwcsName, -1, szName, sizeof(szName), NULL, NULL);

   if (!m_pStorage) {
      pStorage = new PoleStorage();
      pStorage->AddRef();

      pStorage->Init(szName, "");
   }
   else {
      string path = m_szPath;
      if (path.length() > 0)
         path.append("/");
      path.append(szName);

      if (m_pStorage->exists(path.c_str())) {
         pStorage = new PoleStorage();
         pStorage->AddRef();

         pStorage->Init(m_szName, path);
      }
   }

   if (pStorage) {
      *ppstg = pStorage;

      if (pStorage->m_pStorage->open())
         if (pStorage->m_pStorage->result() == POLE::Storage::Ok)
            return S_OK;
   }

   return E_FAIL;
}

STDMETHODIMP PoleStorage::CopyTo(DWORD ciidExclude, const IID *rgiidExclude, SNB snbExclude, IStorage *pstgDest) { return E_NOTIMPL; }
STDMETHODIMP PoleStorage::MoveElementTo(LPCOLESTR pwcsName, IStorage *pstgDest, LPCOLESTR pwcsNewName, DWORD grfFlags) { return E_NOTIMPL; }
STDMETHODIMP PoleStorage::Commit(DWORD grfCommitFlags) { return E_NOTIMPL; }
STDMETHODIMP PoleStorage::Revert() { return E_NOTIMPL; }
STDMETHODIMP PoleStorage::EnumElements(DWORD reserved1, void *reserved2, DWORD reserved3, IEnumSTATSTG **ppenum) { return E_NOTIMPL; }
STDMETHODIMP PoleStorage::DestroyElement(LPCOLESTR pwcsName) { return E_NOTIMPL; }
STDMETHODIMP PoleStorage::RenameElement(LPCOLESTR pwcsOldName, LPCOLESTR pwcsNewName) { return E_NOTIMPL; }
STDMETHODIMP PoleStorage::SetElementTimes(LPCOLESTR pwcsName, const FILETIME *pctime, const FILETIME *patime, const FILETIME *pmtime) { return E_NOTIMPL; }
STDMETHODIMP PoleStorage::SetClass(REFCLSID clsid) { return E_NOTIMPL; }
STDMETHODIMP PoleStorage::SetStateBits(DWORD grfStateBits, DWORD grfMask) { return E_NOTIMPL; }
STDMETHODIMP PoleStorage::Stat(STATSTG *pstatstg, DWORD grfStatFlag) { return E_NOTIMPL; }