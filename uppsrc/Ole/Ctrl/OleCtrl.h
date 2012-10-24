#ifndef _Ole_Ctrl_OleCtrl_h_
#define _Ole_Ctrl_OleCtrl_h_

#include <CtrlCore/CtrlCore.h>
#include <Ole/Ole.h>

NAMESPACE_UPP

#define OCXLOG RLOG    // redefine to RLOG if you want logs in retail versions

#ifdef _DEBUG
#define LOG_SYSOCXS 1 // 1 = log system calls (DllCanUnloadNow, DllGetClassObject)
#define LOG_METHODS 1 // 1 = log method calls
#define LOG_CREATES 1 // 1 = log instance creations & destructions
#define LOG_QUERIES 2 // 1 = log failed QueryInterface's
//                    // 2 = log all QueryInterface's
//                    // 3 = dump interface map whenever Query fails
#define LOG_PERSIST 1 // 1 = log saves / loads
#define LOG_RESULTS 2 // 1 = log error results
//                    // 2 = log all results
#define LOG_ADDREFS 1 // 1 = log AddRef's / Releases
#define LOG_INVOKES 1 // 1 = log IDispatch::Invoke
#else
#define LOG_SYSOCXS 1
#define LOG_METHODS 0
#define LOG_CREATES 0
#define LOG_QUERIES 0
#define LOG_PERSIST 0
#define LOG_RESULTS 0
#define LOG_ADDREFS 0
#define LOG_INVOKES 0
#endif

#if LOG_SYSOCXS >= 1
#define LOGSYSOCX(x) OCXLOG(x)
#else
#define LOGSYSOCX(x)
#endif

#if LOG_METHODS >= 1
#define LOGMETHOD(x) OCXLOG(x)
#else
#define LOGMETHOD(x)
#endif

#if LOG_CREATES >= 1
#define LOGCREATE(x) OCXLOG(x)
#else
#define LOGCREATE(x)
#endif

#if LOG_QUERIES >= 1
#define LOGQUERY(x) OCXLOG(x)
#else
#define LOGQUERY(x)
#endif

#if LOG_PERSIST >= 1
#define LOGPERSIST(x) OCXLOG(x)
#else
#define LOGPERSIST(x)
#endif

#if LOG_RESULTS >= 2
#define LOGRESULT(x) LogResult((x))
#elif LOG_RESULTS >= 1
#define LOGRESULT(x) LogError((x))
#else
#define LOGRESULT(x) (x)
#endif

#if LOG_INVOKES >= 1
#define LOGINVOKE(x) OCXLOG(x)
#else
#define LOGINVOKE(x)
#endif

typedef void (*InitProc)();
InitProc& LateInitProc();
InitProc& LateExitProc();

void DoLateInit();
void DoLateExit();

#define OCX_APP_MAIN \
void _DllMainAppInit(); \
\
static void _DllMainLateExit() \
{ \
	RLOGBLOCK("_DllMainLateExit"); \
	Ctrl::ExitWin32(); \
} \
\
static void _DllMainLateInit() \
{ \
	RLOGBLOCK("_DllMainLateInit"); \
	RLOG("Ctrl::InitWin32"); \
	Ctrl::InitWin32(AppGetHandle()); \
	RLOG("AppInitEnvironment"); \
	AppInitEnvironment__(); \
	RLOG("DllMainAppInit"); \
	_DllMainAppInit(); \
	LateExitProc() = &_DllMainLateExit; \
} \
\
BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpReserved) \
{ \
	if(fdwReason == DLL_PROCESS_ATTACH) { \
		AppSetHandle(hinstDll); \
	} \
	RLOG("DllMain(" << FormatIntHex(hinstDll) << ", reason = " << (int)fdwReason << ")"); \
	if(fdwReason == DLL_PROCESS_ATTACH) { \
		LateInitProc() = &_DllMainLateInit; \
	} \
	else if(fdwReason == DLL_PROCESS_DETACH) { \
		DoLateExit(); \
	} \
	RLOG("//DllMain(" << FormatIntHex(hinstDll) << ", reason = " << (int)fdwReason << ")"); \
	return true; \
} \
\
void _DllMainAppInit()

/*
_variant_t ValueToVariant(const Value& v);
Value      DispatchToValue(IDispatch *disp);
Value      UnknownToValue(IUnknown *unk);
void       ReturnVariant(VARIANT *var, const Value& v);
HRESULT    CheckReturnString(BSTR *bstr, const String& s);
Color      PackColor(long rgb);
long       UnpackColor(Color c);
HRESULT    CheckReturnColor(long *ptr, Color c);
*/

#define std_method      HRESULT STDMETHODCALLTYPE
#define void_method     void    STDMETHODCALLTYPE

class OcxTypeInfo;

typedef VectorMap<Guid, IUnknown *> InterfaceMap;

inline void AddInterfaceRaw(InterfaceMap& map, const Guid& guid, IUnknown *iface)
{
	map.Add(guid, iface);
}

template <class T>
inline void AddInterface(InterfaceMap& map, const Guid& guid, T *iface)
{
	AddInterfaceRaw(map, guid, iface);
}

#define PARENT_INTERFACE(clss, base) \
template <> \
inline void AddInterface(InterfaceMap& map, const Guid& guid, clss *iface) \
{ \
	AddInterfaceRaw(map, guid, iface); \
	AddInterface<base>(map, __uuidof(base), iface); \
}

PARENT_INTERFACE(IProvideClassInfo2, IProvideClassInfo)
PARENT_INTERFACE(IViewObject2, IViewObject)
PARENT_INTERFACE(IViewObjectEx, IViewObject2)
PARENT_INTERFACE(IOleInPlaceObject, IOleWindow)
PARENT_INTERFACE(IOleInPlaceObjectWindowless, IOleInPlaceObject)
PARENT_INTERFACE(IOleInPlaceActiveObject, IOleWindow)
PARENT_INTERFACE(IPersistStorage, IPersist)
PARENT_INTERFACE(IPersistStream, IPersist)
PARENT_INTERFACE(IPersistFile, IPersist)
PARENT_INTERFACE(IClassFactory2, IClassFactory)

class OcxObject
{
	friend class OcxTypeInfo;

public:
	OcxObject();

	template <class T>
	void               AddInterface(const Guid& guid, T *iface) { UPP::AddInterface<T>(interface_map, guid, iface); }
	template <class T>
	void               AddInterface(T *iface)                   { UPP::AddInterface<T>(interface_map, __uuidof(T), iface); }

	IUnknown          *OuterUnknown()                           { return outer_unknown ? outer_unknown : &inner_unknown; }
	IUnknown          *InnerUnknown()                           { return &inner_unknown; }

	HRESULT            InternalQueryInterface(const GUID& iid, void **ppv);
	HRESULT            ExternalQueryInterface(const GUID& iid, void **ppv);

	static HRESULT     RawGetTypeInfo(IRef<ITypeInfo>& dispatch_info, unsigned tinfo, LCID lcid, ITypeInfo **ppinfo);
	static HRESULT     RawGetIDsOfNames(IRef<ITypeInfo>& dispatch_info, REFIID riid, OLECHAR **names, unsigned cnames, LCID lcid, DISPID *dispid);
	static HRESULT     RawInvoke(IRef<ITypeInfo>& dispatch_info, IDispatch *disp, DISPID dispid, word flags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *excep, unsigned *arg_err);

	static const char *GetObjectName()   { return ""; }          // programmatic object name (synthetic = <ocx name>.<class name>)
	static const char *GetObjectHelp()   { return ""; }          // user-visible object name (synthetic = <class name> (CONTROL ? " Control " | " Object ") <version>
//	static GUID        GetDispatchGUID() { return __uuidof(0); } // default ingoing dispatch interface
	static GUID        GetEventGUID()    { return __uuidof(0); } // default outgoing dispatch interface

	enum
	{
		VERSION = 0,
		CONTROL = 0, // override with 1 to make a control class
	};

	virtual int        InternalAddRef() = 0;
	virtual int        InternalRelease() = 0;

	int                ExternalAddRef();
	int                ExternalRelease();

protected:
	class OcxInnerUnknown : public IUnknown {
	public:
		OcxInnerUnknown() : owner(0) {}
		ULONG STDMETHODCALLTYPE AddRef()                  { return owner->InternalAddRef(); }
		ULONG STDMETHODCALLTYPE Release()                 { return owner->InternalRelease(); }
		STDMETHOD(QueryInterface)(REFIID iid, void **ppv) { return owner->InternalQueryInterface(iid, ppv); }

	public:
		OcxObject *owner;
	};

	OcxTypeInfo       *ocx_info;
	IUnknown          *outer_unknown;
	InterfaceMap       interface_map;
	OcxInnerUnknown    inner_unknown;
};

template <class I>
class Interface : public I, virtual public OcxObject
{
public:
	Interface() { AddInterface<I>(this); }
};

template <class I>
class DispatchInterface : public Interface<I>
{
public:
	DispatchInterface() { AddInterface(__uuidof(I), this); AddInterface<IDispatch>(this); }
//	static GUID GetDispatchGUID() { return __uuidof(I); }

	// IDispatch

	STDMETHOD(GetTypeInfoCount)(unsigned *pctinfo);
	STDMETHOD(GetTypeInfo)(unsigned tinfo, LCID lcid, ITypeInfo **ppinfo);
	STDMETHOD(GetIDsOfNames)(REFIID riid, OLECHAR **names, unsigned cnames, LCID lcid, DISPID *dispid);
	STDMETHOD(Invoke)(DISPID dispid, REFIID riid, LCID lcid, word flags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *excep, unsigned *arg_err);

protected:
	void            LoadTypeInfo() { ocx_info->GetDispatchTypeInfo(dispatch_info, __uuidof(I)); }

protected:
	IRef<ITypeInfo> dispatch_info;
};

template <class I>
STDMETHODIMP DispatchInterface<I>::GetTypeInfoCount(unsigned *pctinfo)
{
	if(!pctinfo) return E_POINTER;
	*pctinfo = 1;
	return S_OK;
}

template <class I>
STDMETHODIMP DispatchInterface<I>::GetTypeInfo(unsigned tinfo, LCID lcid, ITypeInfo **ppinfo)
{
	if(!ppinfo) return E_POINTER;
	LoadTypeInfo();
	if(*ppinfo = ~dispatch_info) dispatch_info->AddRef();
	return S_OK;
}

template <class I>
STDMETHODIMP DispatchInterface<I>::GetIDsOfNames(REFIID riid, OLECHAR **names, unsigned cnames, LCID lcid, DISPID *dispid)
{
	LoadTypeInfo();
	return RawGetIDsOfNames(dispatch_info, riid, names, cnames, lcid, dispid);
}

template <class I>
STDMETHODIMP DispatchInterface<I>::Invoke(DISPID dispid, REFIID riid, LCID lcid, word flags, DISPPARAMS *params, VARIANT *result, EXCEPINFO *excep, unsigned *arg_err)
{
	LoadTypeInfo();
	return RawInvoke(dispatch_info, static_cast<I *>(this), dispid, flags, params, result, excep, arg_err);
}

class OcxProvideClassInfo : public Interface<IProvideClassInfo2>
{
public:
	// IProvideClassInfo

	STDMETHOD(GetClassInfo)(ITypeInfo **info);

	// IProvideClassInfo2

	STDMETHOD(GetGUID)(dword guidkind, GUID *guid);
};

template <class T>
class OcxObjectWrapper : public T
{
public:
	OcxObjectWrapper() : refcount(0) {}
	OcxObjectWrapper(OcxTypeInfo& _ocx_info) : refcount(0) { ocx_info = &_ocx_info; _ocx_info.object_count++; }

	static OcxObject     *New()                       { return new OcxObjectWrapper<T>(); }
	static OcxObject     *New(OcxTypeInfo& _ocx_info) { return new OcxObjectWrapper<T>(_ocx_info); }

	virtual ULONG STDMETHODCALLTYPE AddRef()          { return ExternalAddRef(); }
	virtual ULONG STDMETHODCALLTYPE Release()         { return ExternalRelease(); }
	STDMETHOD(QueryInterface)(REFIID iid, void **ppv) { return ExternalQueryInterface(iid, ppv); }

	static String         GetName();

	virtual int           InternalAddRef();
	virtual int           InternalRelease();

private:
	void                  Destroy();

private:
	ULONG                 refcount;
};

template <class T>
int OcxObjectWrapper<T>::InternalAddRef()
{
	++refcount;
#if LOG_ADDREFS
	RLOG(GetTypeName(typeid(*this)) << "::InternalAddRef(" << refcount << ")");
#endif
	return refcount;
}

template <class T>
int OcxObjectWrapper<T>::InternalRelease()
{
	int u = --refcount;
#if LOG_ADDREFS
	RLOG(GetTypeName(typeid(*this)) << "::InternalRelease(" << refcount << ")");
#endif
	if(!u)
		Destroy();
	return u;
}

template <class T>
String OcxObjectWrapper<T>::GetName()
{
	if(*T::GetObjectName())
		return T::GetObjectName();
	return GetTypeName(typeid(T));
}

template <class T>
void OcxObjectWrapper<T>::Destroy()
{
	if(ocx_info)
	{
		ocx_info->object_count--;
		LOGCREATE("\tdelete " << GetTypeName(typeid(T)) << ": "
			<< (int)ocx_info->object_count << " instances left in system");
	}
	else
		LOGCREATE("\tdelete " << GetTypeName(typeid(T)));
	delete this;
}

class OcxTypeInfo : public Interface<IClassFactory2>
{
	friend class OcxTypeLib;

public:
	typedef OcxObject *(*New)(OcxTypeInfo& entry);

	OcxTypeInfo(const GUID& coclass_guid, /*const GUID& dispatch_guid,*/ const GUID& event_guid,
		New new_fn, String name, const char* help = "", int ver = 0,
		bool is_control = false);

	bool            IsControl() const       { return is_control; }
	bool            CanUnload() const;

	const Guid&     GetCoClassGUID() const  { return coclass_guid; }
//	const Guid&     GetDispatchGUID() const { return dispatch_guid; }
	const Guid&     GetEventGUID() const    { return event_guid; }

	String          GetName() const         { return name; }

//	IRef<ITypeInfo> GetDispatchTypeInfo();
	void            GetDispatchTypeInfo(IRef<ITypeInfo>& dest, Guid dispatch_iid);
	IRef<ITypeInfo> GetCoClassTypeInfo();

	// IUnknown

	virtual int     InternalAddRef()              { return IncRef(); }
	virtual int     InternalRelease()             { return DecRef(); }
	virtual ULONG   STDMETHODCALLTYPE AddRef()    { return IncRef(); }
	virtual ULONG   STDMETHODCALLTYPE Release()   { return DecRef(); }
	STDMETHOD(QueryInterface)(REFIID iid, void **ppv) { return ExternalQueryInterface(iid, ppv); }

	// IClassFactory

	STDMETHOD(CreateInstance)(IUnknown *outer, REFIID iid, void **object);
	STDMETHOD(LockServer)(BOOL lock);

	STDMETHOD(GetLicInfo)(/* [out] */ LICINFO *pLicInfo);
	STDMETHOD(RequestLicKey)(/* [in] */ DWORD dwReserved, /* [out] */ BSTR *pBstrKey);

	STDMETHOD(CreateInstanceLic)(
			/* [in] */ IUnknown *pUnkOuter,
			/* [in] */ IUnknown *pUnkReserved,
			/* [in] */ REFIID riid,
			/* [in] */ BSTR bstrKey,
			/* [iid_is][out] */ PVOID *ppvObj);

public:
	long            object_count;  // managed object count
	IRef<ITypeInfo> coclass_info;  // coclass type info
//	IRef<ITypeInfo> dispatch_info; // ingoing dispatch interface type info
	CriticalSection critical;      // critical section used for initializations

private:
	int             IncRef();
	int             DecRef();

private:
	Guid            coclass_guid;  // coclass GUID
	Guid            dispatch_guid; // GUID of default ingoing dispatch interface
	Guid            event_guid;    // GUID of default outgoing dispatch interface
	New             new_fn;        // constructor
	String          name;          // programmatic object name - <ocx name>.<class name>.<version> by default
	const char     *help;          // user-visible object name - <class name> (" Control " | " Object ") <version> by default
	int             ver;           // 0 = use main library version
	bool            is_control;    // false = generic object, true = insertable control

	long            refcount;      // AddRef/Release counter
	Atomic          lock_count;    // LockServer lock count

#ifndef _USRDLL
	dword           registration;  // for CoRevokeClassObject
#endif
};

class OcxTypeLib
{
public:
	static OcxTypeLib&     Get();

	OcxTypeLib&            Name(String name)           { lib_name = name; return *this; }
	String                 GetName() const             { return lib_name; }
	OcxTypeLib&            DisplayName(String dn)      { display_name = dn; return *this; }
	String                 GetDisplayName() const      { return display_name; }
	String                 GetLibName() const;
	OcxTypeLib&            UsesLibrary(String name)    { uses_libraries.GetAdd(name); return *this; }

	OcxTypeLib&            Version(int _ver)           { lib_ver  = _ver; return *this; }
	int                    GetVersion() const          { return lib_ver; }

	HRESULT                Register();
	HRESULT                Unregister();
	HRESULT                GetFactory(REFCLSID clsid, REFIID iid, void **ppv);
	bool                   CanUnload() const;
#ifndef _USRDLL
	HRESULT                RegisterObjects(); // call CoRegisterClassObject for each object
	void                   RevokeObjects();   // call CoRevokeClassObject
#endif

	void                   Add(OcxTypeInfo& entry)     { objects.FindAdd(entry.coclass_guid, &entry); }
	void                   AddInit(void (*fn)())       { ocx_init.Add(fn); }

	IRef<ITypeInfo>        GetTypeInfo(const Guid& guid);

	IRef<ITypeLib>&        GetTypeLib();
	IRef<ITypeLib>&        operator -> ()              { return GetTypeLib(); }

public:
	IRef<ITypeLib>         typelib;
	CriticalSection        critical;

private:
	OcxTypeLib();

private:
	VectorMap<Guid, OcxTypeInfo *> objects;
	Vector<void (*)()>     ocx_init;
	String                 lib_name;
	String                 display_name;
	int                    lib_ver;
	VectorMap<String, IRef<ITypeLib> > uses_libraries;
};

struct OcxInit { OcxInit(void (*fn)()) { OcxTypeLib::Get().AddInit(fn); } };
#define OCX_INIT(fn) static OcxInit MK__s = fn;

#define OCX_OBJECT(type) \
	static OcxTypeInfo MK__s( \
		__uuidof(type),                    /* coclass GUID */ \
		/*type::GetDispatchGUID(),*/       /* default ingoing dispatch interface GUID */ \
		type::GetEventGUID(),              /* default outgoing dispatch interface GUID */ \
		OcxObjectWrapper<type>::New,       /* constructor */ \
		OcxObjectWrapper<type>::GetName(), /* name */ \
		type::GetObjectHelp(),             /* help */ \
		type::VERSION,                     /* version */ \
		type::CONTROL);                    /* control object */

#define OCX_NEWOBJECT(type) \
	OCX_OBJECT(type) \
	static IRef<type> COMBINE(OcxNew_, type)() { return new OcxObjectWrapper<type>(MK__s); }

#ifndef _USRDLL
bool ExeRegisterServer();
bool ExeUnregisterServer();
void ExeRunServer();
#endif

Size ToHiMetric(Size pixel_size);
Size FromHiMetric(Size himetric_size);
Rect GetWindow(HDC hdc);
Rect GetViewport(HDC hdc);

LPOLESTR AllocString(String s);

template <class T>
class SinkMap : public VectorMap<dword, T>
{
public:
	typedef VectorMap<dword, T> Base;
	SinkMap() : next_sink_id(0) {}
	dword Add(const T& object) { Base::Add(++next_sink_id, object); return next_sink_id; }
	bool  Remove(dword id)     { int i = Find(id); return (i >= 0 ? (Base::Remove(i), true) : false); }

private:
	dword next_sink_id;
};

//template OleType<IAdviseSinkPtr>;

class OcxControl : public Ctrl,
	public Interface<IOleInPlaceActiveObject>,
	public Interface<IOleInPlaceObject>,
	public Interface<IViewObjectEx>,
	public Interface<IDataObject>,
	public Interface<IPersistStorage>,
	public Interface<IPersistStreamInit>,
	public Interface<IOleObject>,
	public Interface<IOleControl>,
	public Interface<IPointerInactive>,
	public OcxProvideClassInfo
{
public:
	OcxControl();
	virtual ~OcxControl();

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	bool            Open(HWND parent);
	virtual void    Serialize(Stream& stream);
	void            SerializePos(Stream& stream);

	virtual bool    IsOcxChild();

	using Ctrl::GetRect;
	using Ctrl::SetData;
	using Ctrl::GetData;

	// IOleObject

	STDMETHOD(SetClientSite)(IOleClientSite *site);
	STDMETHOD(GetClientSite)(IOleClientSite **res);
	STDMETHOD(SetHostNames)(LPCOLESTR app_name, LPCOLESTR doc_name);
	STDMETHOD(Close)(dword save_option);
	STDMETHOD(SetMoniker)(dword which, IMoniker *moniker);
	STDMETHOD(GetMoniker)(dword assign, dword which, IMoniker **moniker);
	STDMETHOD(InitFromData)(IDataObject *data, BOOL creation, dword);
	STDMETHOD(GetClipboardData)(dword, IDataObject **data);
	STDMETHOD(DoVerb)(long verb, MSG *msg, IOleClientSite *active_site, long index, HWND hwnd, const RECT *rc);
	STDMETHOD(EnumVerbs)(IEnumOLEVERB **verbs);
	STDMETHOD(Update)();
	STDMETHOD(IsUpToDate)();
	STDMETHOD(GetUserClassID)(GUID *guid);
	STDMETHOD(GetUserType)(dword form, LPOLESTR *type);
	STDMETHOD(SetExtent)(dword aspect, SIZEL *size);
	STDMETHOD(GetExtent)(dword aspect, SIZEL *size);
	STDMETHOD(Advise)(IAdviseSink *sink, dword *connection);
	STDMETHOD(Unadvise)(dword connection);
	STDMETHOD(EnumAdvise)(IEnumSTATDATA **sink_enum);
	STDMETHOD(GetMiscStatus)(dword aspect, dword *status);
	STDMETHOD(SetColorScheme)(LOGPALETTE *logpal);

	// IPersistStorage

	STDMETHOD(GetClassID)(GUID *guid);
	STDMETHOD(IsDirty)(); // IPersistStreamInit as well
	STDMETHOD(InitNew)(IStorage *stg);
	STDMETHOD(Load)(IStorage *stg);
	STDMETHOD(Save)(IStorage *stg, BOOL same_as_load);
	STDMETHOD(SaveCompleted)(IStorage *stg);
	STDMETHOD(HandsOffStorage)();

	// IPersistStreamInit

	STDMETHOD(Load)(IStream *stream);
	STDMETHOD(Save)(IStream *stream, BOOL clear_dirty);
	STDMETHOD(GetSizeMax)(ULARGE_INTEGER *size);
	STDMETHOD(InitNew)();

	// IDataObject

	STDMETHOD(GetData)(FORMATETC *format, STGMEDIUM *medium);
	STDMETHOD(GetDataHere)(FORMATETC *format, STGMEDIUM *medium);
	STDMETHOD(QueryGetData)(FORMATETC *format);
	STDMETHOD(GetCanonicalFormatEtc)(FORMATETC *in, FORMATETC *out);
	STDMETHOD(SetData)(FORMATETC *format, STGMEDIUM *medium, BOOL free);
	STDMETHOD(EnumFormatEtc)(dword dir, IEnumFORMATETC **enumerator);
	STDMETHOD(DAdvise)(FORMATETC *format, dword advf, IAdviseSink *sink, dword *connection);
	STDMETHOD(DUnadvise)(dword connection);
	STDMETHOD(EnumDAdvise)(IEnumSTATDATA **enumerator);

	// IViewObject

	STDMETHOD(Draw)(dword aspect, long index, void *aspectinfo,
		DVTARGETDEVICE *device, HDC target, HDC draw,
		const RECTL *bounds, const RECTL *wbounds,
		BOOL (STDMETHODCALLTYPE *progress)(dword arg), dword arg);
	STDMETHOD(GetColorSet)(dword aspect, long index, void *aspectinfo,
		DVTARGETDEVICE *device, HDC target, LOGPALETTE **palette);
	STDMETHOD(Freeze)(dword aspect, long index, void *aspectinfo,
		dword *freeze_key);
	STDMETHOD(Unfreeze)(dword freeze_key);
	STDMETHOD(SetAdvise)(dword aspect, dword advf, IAdviseSink *sink);
	STDMETHOD(GetAdvise)(dword *aspect, dword *advf, IAdviseSink **sink);

	// IViewObject2

	STDMETHOD(GetExtent)(dword aspect, long index, DVTARGETDEVICE *device, SIZEL *size);

	// IViewObjectEx

	STDMETHOD(GetRect)(dword aspect, RECTL *rc);
	STDMETHOD(GetViewStatus)(dword *status);
	STDMETHOD(QueryHitPoint)(dword aspect, const RECT *bounds, POINT pos, long close_hint, dword *result);
	STDMETHOD(QueryHitRect)(dword aspect, const RECT *bounds, const RECT *, long close_hint, dword *result);
	STDMETHOD(GetNaturalExtent)(dword aspect, long index, DVTARGETDEVICE *ptd, HDC tdc, DVEXTENTINFO *extinfo, SIZEL *psize);

	// IOleWindow

	STDMETHOD(GetWindow)(HWND *hwnd);
	STDMETHOD(ContextSensitiveHelp)(BOOL enter_mode);

	// IOleInPlaceObject

	STDMETHOD(InPlaceDeactivate)();
	STDMETHOD(UIDeactivate)();
	STDMETHOD(SetObjectRects)(const RECT *pos, const RECT *clip);
	STDMETHOD(ReactivateAndUndo)();

	// IOleInPlaceActiveObject

	STDMETHOD(TranslateAccelerator)(MSG *msg);
	STDMETHOD(OnFrameWindowActivate)(BOOL activate);
	STDMETHOD(OnDocWindowActivate)(BOOL activate);
	STDMETHOD(ResizeBorder)(const RECT *border, IOleInPlaceUIWindow *ui, BOOL frame);
	STDMETHOD(EnableModeless)(BOOL enable);

	// IOleControl

	STDMETHOD(GetControlInfo)(CONTROLINFO *cinfo);
	STDMETHOD(OnMnemonic)(MSG *msg);
	STDMETHOD(OnAmbientPropertyChange)(DISPID dispid);
	STDMETHOD(FreezeEvents)(BOOL freeze);

	// IPointerInactive

	STDMETHOD(GetActivationPolicy)(DWORD *pdwPolicy);
	STDMETHOD(OnInactiveMouseMove)(LPCRECT pRectBounds, LONG x, LONG y, DWORD grfKeyState);
	STDMETHOD(OnInactiveSetCursor)(LPCRECT pRectBounds, LONG x, LONG y, DWORD dwMouseMsg, BOOL fSetAlways);

	// default verb

	virtual HRESULT DoVerbPrimary();
	virtual HRESULT DoVerbShow();
	virtual HRESULT DoVerbInPlaceActivate();
	virtual HRESULT DoVerbUIActivate();
	virtual HRESULT DoVerbOpen();
	virtual HRESULT DoVerbHide();
	virtual HRESULT DoVerbDiscardUndo();
	virtual HRESULT DoVerbProperties();

	// various helpers

	bool            IsClosed() const            { return status == CLOSED; }
//	bool            IsForged() const            { return status == FORGED; }
	bool            IsActive() const            { return status >= ACTIVE; }
	bool            IsUIActive() const          { return status == UIACTIVE; }

	void            RefreshSink();

	enum { CONTROL = 1 };

protected:
	void            UpdateControlRect();
	HRESULT         ShowControl(bool ui_activate);
	void            SetActiveObject(bool set);

	void            TimerThread();

protected:
	static const WCHAR data_stream_name[]; // default data stream

	enum STATE { CLOSED, /* FORGED, */ ACTIVE, UIACTIVE };

//	Thread                       timer_thread;
	IRef<IOleClientSite>         client_site;
	IRef<IOleInPlaceSite>        in_place_site;
	SinkMap< IRef<IAdviseSink> > advise_sinks;
	dword                        view_sink_aspect;
	dword                        view_sink_advf;
	IRef<IAdviseSink>            view_sink;
	Rect                         ctrl_rect;
	Size                         extent_size;
	STATE                        status;
//	WPARAM                       ncmm_wParam;
//	LPARAM                       ncmm_lParam;
	bool                         stream_inited;
	bool                         timer_shutdown;
};

class OcxRunnableControl : public OcxControl, public Interface<IRunnableObject>
{
public:
	OcxRunnableControl() {}

	// IRunnableObject

	STDMETHOD(GetRunningClass)(GUID *clsid);
	STDMETHOD(Run)(LPBC ctx);
	BOOL STDMETHODCALLTYPE IsRunning();
	STDMETHOD(LockRunning)(BOOL lock, BOOL last_unlock_closes);
	STDMETHOD(SetContainedObject)(BOOL contained);
};

/*
struct CtrlHook : public Ctrl
{
public:
	void _CreateChild() { Ctrl::CreateChild(); }
};
*/

class OcxConnectionPoint : public Interface<IConnectionPoint>
{
public:
	OcxConnectionPoint() {}

	void SetEventGUID(const Guid& g)                   { guid = g; }
	void SetContainer(IConnectionPointContainer *cont) { container = cont; }
	void FireEvent(int method_id, const Vector<Value>& values);

	STDMETHOD(GetConnectionInterface)(IID *iid);
	STDMETHOD(GetConnectionPointContainer)(IConnectionPointContainer **ppCPC);
	STDMETHOD(Advise)(IUnknown *ptr, DWORD *cookie);
	STDMETHOD(Unadvise)(DWORD cookie);
	STDMETHOD(EnumConnections)(IEnumConnections **enum_conn);

	class Enumerator : public Interface<IEnumConnections>
	{
	public:
		Enumerator() {}

		void            Attach(OcxConnectionPoint *pt) { point = IRef<OcxConnectionPoint>(pt); index = 0; }

		STDMETHOD(Next)(unsigned long count, CONNECTDATA *data, unsigned long *fetched);
		STDMETHOD(Skip)(unsigned long connections);
		STDMETHOD(Reset)();
		STDMETHOD(Clone)(IEnumConnections **enum_conn);

	private:
		IRef<OcxConnectionPoint> point;
		int                          index;
	};

	friend class Enumerator;

public:
	Guid                               guid;

protected:
	IConnectionPointContainer         *container;
	VectorMap<dword, IRef<IDispatch> > conn_map;
	Vector<dword>                      free_id;
};

class OcxConnectionPointContainer : public Interface<IConnectionPointContainer>
{
public:
	OcxConnectionPointContainer();

	void AddConnectionPoint(OcxConnectionPoint *conn) { point_map.GetAdd(conn -> guid) = conn; conn -> SetContainer(this); }

	STDMETHOD(EnumConnectionPoints)(IEnumConnectionPoints **ppEnum);
	STDMETHOD(FindConnectionPoint)(REFIID riid, IConnectionPoint **ppCP);

private:
	class Enumerator : public Interface<IEnumConnectionPoints>
	{
	public:
		Enumerator() {}

		void            Attach(OcxConnectionPointContainer *pt) { point = pt; index = 0; }

		STDMETHOD(Next)(unsigned long count, IConnectionPoint **rgpcn, unsigned long *fetched);
		STDMETHOD(Skip)(unsigned long connections);
		STDMETHOD(Reset)();
		STDMETHOD(Clone)(IEnumConnectionPoints **enum_conn);

	private:
		IRef<OcxConnectionPointContainer> point;
		int                               index;
	};

	friend class Enumerator;

private:
	VectorMap<Guid, IConnectionPoint *> point_map;
};

END_UPP_NAMESPACE

#endif
