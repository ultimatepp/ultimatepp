#ifndef _hdIE_ActiveX_Support_h_
#define _hdIE_ActiveX_Support_h_

NAMESPACE_UPP

bool BSTRSet(const String str, BSTR &bstr);
String BSTRGet(BSTR &bstr);
	
class OleStr {
public: 
	OleStr();
    ~OleStr();
    bool Set(const String str);
    operator VARIANT *() {return &var;};
private:    
    VARIANT var;
};

class CBSTR {
public:
	CBSTR() : bstr(0) {}
	bool Set(const String str);
	~CBSTR();
	operator BSTR *() {return &bstr;}
	operator BSTR ()  {return bstr;}
	String ToString() {
		WString ret((wchar *)bstr);
		return ret.ToString();
	}
private:
	BSTR bstr;
};

class AXClientSite : public IOleClientSite, public IOleInPlaceFrame, public IOleInPlaceSite {
public:
	AXClientSite()	{}
	AXClientSite(HWND hwnd):hwnd(hwnd){}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	virtual HRESULT STDMETHODCALLTYPE SaveObject();
	virtual HRESULT STDMETHODCALLTYPE GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker **ppmk);
	virtual HRESULT STDMETHODCALLTYPE GetContainer(IOleContainer **ppContainer);
	virtual HRESULT STDMETHODCALLTYPE ShowObject();
	virtual HRESULT STDMETHODCALLTYPE OnShowWindow(BOOL fShow);
	virtual HRESULT STDMETHODCALLTYPE RequestNewObjectLayout();
	virtual HRESULT STDMETHODCALLTYPE GetWindow(HWND *phwnd);
	virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode);
	virtual HRESULT STDMETHODCALLTYPE GetBorder(LPRECT lprectBorder);
	virtual HRESULT STDMETHODCALLTYPE RequestBorderSpace(LPCBORDERWIDTHS pborderwidths);
	virtual HRESULT STDMETHODCALLTYPE SetBorderSpace(LPCBORDERWIDTHS pborderwidths);
	virtual HRESULT STDMETHODCALLTYPE SetActiveObject(IOleInPlaceActiveObject *pActiveObject, LPCOLESTR pszObjName);
	virtual HRESULT STDMETHODCALLTYPE InsertMenus(HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths);
	virtual HRESULT STDMETHODCALLTYPE SetMenu(HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject);
	virtual HRESULT STDMETHODCALLTYPE RemoveMenus(HMENU hmenuShared);
	virtual HRESULT STDMETHODCALLTYPE SetStatusText(LPCOLESTR pszStatusText);
	virtual HRESULT STDMETHODCALLTYPE EnableModeless(BOOL fEnable);
	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator(LPMSG lpmsg, WORD wID);
	virtual HRESULT STDMETHODCALLTYPE CanInPlaceActivate();
	virtual HRESULT STDMETHODCALLTYPE OnInPlaceActivate();
	virtual HRESULT STDMETHODCALLTYPE OnUIActivate();
	virtual HRESULT STDMETHODCALLTYPE GetWindowContext(IOleInPlaceFrame **ppFrame, IOleInPlaceUIWindow **ppDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo);
	virtual HRESULT STDMETHODCALLTYPE Scroll(SIZE scrollExtant);
	virtual HRESULT STDMETHODCALLTYPE OnUIDeactivate(BOOL fUndoable);
	virtual HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate();
	virtual HRESULT STDMETHODCALLTYPE DiscardUndoState();
	virtual HRESULT STDMETHODCALLTYPE DeactivateAndUndo();
	virtual HRESULT STDMETHODCALLTYPE OnPosRectChange(LPCRECT lprcPosRect);

	HWND hwnd;
};

class AXStorage : public IStorage {
public:
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	virtual HRESULT STDMETHODCALLTYPE CreateStream(const OLECHAR *pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStream **ppstm);
	virtual HRESULT STDMETHODCALLTYPE OpenStream(const OLECHAR *pwcsName, void *reserved1, DWORD grfMode, DWORD reserved2, IStream **ppstm);
	virtual HRESULT STDMETHODCALLTYPE CreateStorage(const OLECHAR *pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStorage **ppstg);
	virtual HRESULT STDMETHODCALLTYPE OpenStorage(const OLECHAR *pwcsName, IStorage *pstgPriority, DWORD grfMode, SNB snbExclude, DWORD reserved, IStorage **ppstg);
	virtual HRESULT STDMETHODCALLTYPE CopyTo(DWORD ciidExclude, const IID *rgiidExclude, SNB snbExclude, IStorage *pstgDest);
	virtual HRESULT STDMETHODCALLTYPE MoveElementTo(const OLECHAR *pwcsName, IStorage *pstgDest, const OLECHAR *pwcsNewName, DWORD grfFlags);
	virtual HRESULT STDMETHODCALLTYPE Commit(DWORD grfCommitFlags);
	virtual HRESULT STDMETHODCALLTYPE Revert();
	virtual HRESULT STDMETHODCALLTYPE EnumElements(DWORD reserved1, void *reserved2, DWORD reserved3, IEnumSTATSTG **ppenum);
	virtual HRESULT STDMETHODCALLTYPE DestroyElement(const OLECHAR *pwcsName);
	virtual HRESULT STDMETHODCALLTYPE RenameElement(const OLECHAR *pwcsOldName, const OLECHAR *pwcsNewName);
	virtual HRESULT STDMETHODCALLTYPE SetElementTimes(const OLECHAR *pwcsName, const FILETIME *pctime, const FILETIME *patime, const FILETIME *pmtime);
	virtual HRESULT STDMETHODCALLTYPE SetClass(REFCLSID clsid);
	virtual HRESULT STDMETHODCALLTYPE SetStateBits(DWORD grfStateBits, DWORD grfMask);
	virtual HRESULT STDMETHODCALLTYPE Stat(STATSTG *pstatstg, DWORD grfStatFlag);
};

class DHCtrlActiveX : public DHCtrl {
public:
	DHCtrlActiveX(CLSID, const String, bool status = true);
	~DHCtrlActiveX(void);
	
	bool Attach(HWND hwnd);
	void *QueryInterface(const IID iid);
	bool IsLoaded() {return oleObj != NULL;}
	DHCtrlActiveX &SetStatus(bool _status);
	bool GetStatus() {return status;};
	
protected:
	void Detach();
	
private:
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void DoResize();
	
	AXClientSite pClientSite;
	
	CLSID clsid;
	String name;
	IOleObject *oleObj;	
	bool status;
};

END_UPP_NAMESPACE

#endif
