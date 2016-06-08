#include	<Windows.h>
#include	<Shlwapi.h>
#include	"comlib.h"

/*============================================================================
 * class ComLib
 */
//----------------------------------------------------------------------------
// CTOR
ComLib::ComLib() :
    mhDll(NULL),
    mDllGetClassObject(NULL),
    mDllCanUnloadNow(NULL) {
}

//----------------------------------------------------------------------------
// DTOR
ComLib::~ComLib() {
    // force unloading
    unload(TRUE);
}

//----------------------------------------------------------------------------
// load: load a module from a certain path
HRESULT ComLib::load(LPOLESTR szDllName) {
    if (mhDll) {
        return S_FALSE;    // is already loaded
    }

    // Add a bit of securty:
    // 1) deny network paths
    if (PathIsNetworkPathW(szDllName)) {
        return E_INVALIDARG;
    }

    // 2) deny relative paths
    if (PathIsNetworkPathW(szDllName)) {
        return E_INVALIDARG;
    }

    // load library
    mhDll = ::CoLoadLibrary(szDllName, FALSE);
    if (!mhDll) {
        return HRESULT_FROM_WIN32(::GetLastError());
    }

    // get DllGetClassObject and DllCanUnloadNow
    mDllGetClassObject = (FnDllGetClassObject)::GetProcAddress(mhDll, "DllGetClassObject");
    HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());

    // DllCanUnloadNow is optional
    mDllCanUnloadNow = (FnDllCanUnloadNow)::GetProcAddress(mhDll, "DllCanUnloadNow");

    if (!mDllGetClassObject) {
        return hr;
    }

    return S_OK;
}

//----------------------------------------------------------------------------
// unload
HRESULT ComLib::unload(BOOL bForce) {
    if (!mhDll) {
        return S_OK;    // not loaded or already unloaded
    }

    // call DllCanUnloadNow
    HRESULT hr = this->can_unload_now();

    // if can NOT unload AND NOT force...
    if ( (S_OK != hr) && !bForce ) {
        return hr;    // ... don't close
    }

    // close: free and clean up
    mDllGetClassObject = NULL;
    mDllCanUnloadNow = NULL;
    ::CoFreeLibrary(mhDll);
    mhDll = NULL;
    return S_OK;
}

//----------------------------------------------------------------------------
// get_class_object
HRESULT ComLib::get_class_object(REFCLSID aClsid,
                                 REFIID aIid, LPVOID FAR* ppv) {
    return (mDllGetClassObject)
           ? mDllGetClassObject(aClsid, aIid, ppv)
           : E_UNEXPECTED;
}

//----------------------------------------------------------------------------
// can_unload_now
HRESULT ComLib::can_unload_now() {
    return (mDllCanUnloadNow)
           ? mDllCanUnloadNow()
           : S_OK; // default: yes, can unload
}
