#ifdef _WIN32

#include <CtrlLib/CtrlLib.h>
#include "ActiveX.h"

NAMESPACE_UPP

bool BSTRSet(const String str, BSTR &bstr) {
	wchar_t *buffer;
	DWORD size = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
	if (!(buffer = (wchar_t *)GlobalAlloc(GMEM_FIXED, sizeof(wchar_t) * size)))
		return false;

	MultiByteToWideChar(CP_UTF8, 0, str, -1, buffer, size);
	bstr = SysAllocString(buffer);
	GlobalFree(buffer);
	if (!bstr)
		return false;
	return true;
}

String BSTRGet(BSTR &bstr) {
	if (!bstr)
		return Null;
	
	char *buffer;
	DWORD size = SysStringLen(bstr);
	if (!(buffer = (char *)GlobalAlloc(GMEM_FIXED, sizeof(wchar_t) * size)))
		return Null;
	
	size_t i = wcstombs(buffer, bstr, size);
	buffer[i] = 0;
	
	String ret = buffer;
	GlobalFree(buffer);
	return ret;
}

OleStr::OleStr() {
    VariantInit(&var);
    VariantClear(&var); 	
}

OleStr::~OleStr() {
  	SysFreeString(var.bstrVal);
}

bool OleStr::Set(const String str) {
	var.vt = VT_BSTR;
	return BSTRSet(str, var.bstrVal);
}

CBSTR::~CBSTR() {
	SysFreeString(bstr);
}

bool CBSTR::Set(const String str) {
	return BSTRSet(str, bstr);
}

DHCtrlActiveX::DHCtrlActiveX(CLSID clsid, const String name, bool status) : 
						oleObj(0), pClientSite(0), clsid(clsid), name(name), status(status) {}

DHCtrlActiveX::~DHCtrlActiveX(void) {
	Detach();
}

DHCtrlActiveX &DHCtrlActiveX::SetStatus(bool _status) {
	status = _status; 
	if (status) {
		Attach(GetHWND());
		EnableWindow(GetHWND(), true);
		BackPaint(NOBACKPAINT);
	} else {
		Detach();
		BackPaint(FULLBACKPAINT);
	}
	return *this;
};
          
LRESULT DHCtrlActiveX::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
	if (status) {
		switch (message) {
		case WM_SIZE: 	DoResize();
						break;
		case WM_CREATE:	Attach(GetHWND());
						EnableWindow(GetHWND(), true);
						break;
		}
	}
	return DHCtrl::WindowProc(message, wParam, lParam);
}

void *DHCtrlActiveX::QueryInterface(const IID iid) {
	void *ret;
	if (!oleObj || !status)
		return NULL;
	if (S_OK != oleObj->QueryInterface(iid, &ret)) 
		return NULL;
	return ret;
}
	
void DHCtrlActiveX::Detach() {
	if(!pClientSite.hwnd) 
		return;
	if (!oleObj)
		return;
	
	oleObj->Close(OLECLOSE_NOSAVE);
	oleObj->Release();
	oleObj = 0;
}

void DHCtrlActiveX::DoResize() {
	if(!pClientSite.hwnd) 
		return;
	if (!oleObj || !status)
		return;
	
	RECT rect;
	GetClientRect(pClientSite.hwnd, &rect);

	IOleInPlaceObject *inplace = (IOleInPlaceObject *)QueryInterface(IID_IOleInPlaceObject);
	if (inplace) {
		inplace->SetObjectRects(&rect, &rect);
		inplace->Release();
	}
}

bool DHCtrlActiveX::Attach(HWND hwnd) {
	if (hwnd == (HWND)-1 || hwnd == 0 || !status)
		return false;
	if(pClientSite.hwnd && pClientSite.hwnd != hwnd) 
		Detach();
	if(!hwnd) 
		return false;

	RECT rect;
	AXStorage storage;
	pClientSite.hwnd = hwnd;
 
	if (OleCreate(clsid, IID_IOleObject, OLERENDER_DRAW, 0, (IOleClientSite *)&pClientSite, &storage, (void**)&oleObj)) 
		return false;
	
	oleObj->SetHostNames(name.ToWString(), 0);
	GetClientRect(hwnd, &rect);
	if (!OleSetContainedObject((IUnknown *)oleObj, TRUE)) {
		if (!oleObj->DoVerb(OLEIVERB_INPLACEACTIVATE, NULL, (IOleClientSite *)&pClientSite, -1, hwnd, &rect))
			return true;
	}
	Detach();
	return false;
}

HRESULT STDMETHODCALLTYPE AXStorage::QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject) {
	throw Exc(t_("Function not implemented"));
}

ULONG STDMETHODCALLTYPE AXStorage::AddRef( void) {
	return 1;
}

ULONG STDMETHODCALLTYPE AXStorage::Release( void) {
	return 0;
}

HRESULT STDMETHODCALLTYPE AXStorage::CreateStream(const OLECHAR *pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStream **ppstm) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXStorage::OpenStream(const OLECHAR *pwcsName, void *reserved1, DWORD grfMode, DWORD reserved2, IStream **ppstm) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXStorage::CreateStorage(const OLECHAR *pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStorage **ppstg) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXStorage::OpenStorage(const OLECHAR *pwcsName, IStorage *pstgPriority, DWORD grfMode, SNB snbExclude, DWORD reserved, IStorage **ppstg) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXStorage::CopyTo(DWORD ciidExclude, const IID *rgiidExclude, SNB snbExclude, IStorage *pstgDest) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXStorage::MoveElementTo(const OLECHAR *pwcsName, IStorage *pstgDest, const OLECHAR *pwcsNewName, DWORD grfFlags) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXStorage::Commit(DWORD grfCommitFlags) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXStorage::Revert() {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXStorage::EnumElements(DWORD reserved1, void *reserved2, DWORD reserved3, IEnumSTATSTG **ppenum) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXStorage::DestroyElement(const OLECHAR *pwcsName) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXStorage::RenameElement(const OLECHAR *pwcsOldName, const OLECHAR *pwcsNewName) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXStorage::SetElementTimes(const OLECHAR *pwcsName, const FILETIME *pctime, const FILETIME *patime, const FILETIME *pmtime) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXStorage::SetClass(REFCLSID clsid) {
	return(S_OK);
}

HRESULT STDMETHODCALLTYPE AXStorage::SetStateBits(DWORD grfStateBits, DWORD grfMask) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXStorage::Stat(STATSTG *pstatstg, DWORD grfStatFlag) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXClientSite::QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject) {
	if (!memcmp(&riid, &IID_IUnknown, sizeof(GUID))) 
		*ppvObject = reinterpret_cast<IUnknown *> (this);
	else if (!memcmp(&riid, &IID_IOleClientSite, sizeof(GUID)))
		*ppvObject = (IOleClientSite *)this;
	else if (!memcmp(&riid, &IID_IOleInPlaceSite, sizeof(GUID)))
		*ppvObject = (IOleInPlaceSite *)this;
	else {
		*ppvObject = 0;
		return(E_NOINTERFACE);
	}
	return S_OK;
}

ULONG STDMETHODCALLTYPE AXClientSite::AddRef() {
	return 1;
}

ULONG STDMETHODCALLTYPE AXClientSite::Release() {
	return 0;
}

HRESULT STDMETHODCALLTYPE AXClientSite::SaveObject() {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXClientSite::GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker **ppmk) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXClientSite::GetContainer(IOleContainer **ppContainer) {
	*ppContainer = 0;
	return E_NOINTERFACE;
}

HRESULT STDMETHODCALLTYPE AXClientSite::ShowObject() {
	return NOERROR;
}

HRESULT STDMETHODCALLTYPE AXClientSite::OnShowWindow(BOOL fShow) {
	return NOERROR;
}

HRESULT STDMETHODCALLTYPE AXClientSite::RequestNewObjectLayout() {
	throw Exc(t_("Function not implemented"));
}


HRESULT STDMETHODCALLTYPE AXClientSite::GetWindow(HWND *phwnd) {
	*phwnd = hwnd;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE AXClientSite::ContextSensitiveHelp(BOOL fEnterMode) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXClientSite::GetBorder(LPRECT lprectBorder) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXClientSite::RequestBorderSpace(LPCBORDERWIDTHS pborderwidths) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXClientSite::SetBorderSpace(LPCBORDERWIDTHS pborderwidths) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXClientSite::SetActiveObject(IOleInPlaceActiveObject *pActiveObject, LPCOLESTR pszObjName) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE AXClientSite::InsertMenus(HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXClientSite::SetMenu(HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE AXClientSite::RemoveMenus(HMENU hmenuShared) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXClientSite::SetStatusText(LPCOLESTR pszStatusText) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE AXClientSite::EnableModeless(BOOL fEnable) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE AXClientSite::TranslateAccelerator(LPMSG lpmsg, WORD wID) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXClientSite::CanInPlaceActivate() {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE AXClientSite::OnInPlaceActivate() {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE AXClientSite::OnUIActivate() {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE AXClientSite::GetWindowContext(IOleInPlaceFrame **ppFrame, IOleInPlaceUIWindow **ppDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo) {
	*ppFrame = (IOleInPlaceFrame *)this; 

	*ppDoc = 0;

	lpFrameInfo->fMDIApp = FALSE;
	lpFrameInfo->hwndFrame = hwnd;  
	lpFrameInfo->haccel = 0;
	lpFrameInfo->cAccelEntries = 0;

	GetClientRect(lpFrameInfo->hwndFrame, lprcPosRect);
	GetClientRect(lpFrameInfo->hwndFrame, lprcClipRect);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE AXClientSite::Scroll(SIZE scrollExtant) {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXClientSite::OnUIDeactivate(BOOL fUndoable) {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE AXClientSite::OnInPlaceDeactivate() {
	return S_OK;
}

HRESULT STDMETHODCALLTYPE AXClientSite::DiscardUndoState() {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXClientSite::DeactivateAndUndo() {
	throw Exc(t_("Function not implemented"));
}

HRESULT STDMETHODCALLTYPE AXClientSite::OnPosRectChange(LPCRECT lprcPosRect) {
	return S_OK;
}

END_UPP_NAMESPACE

#endif
