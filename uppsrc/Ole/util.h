#if defined(PLATFORM_WIN32) && defined(COMPILER_MSC)

NAMESPACE_UPP

#ifdef COMPILER_MSC
typedef __int64 longlong_t;
#define LL_(x) COMBINE(x, i64)
#else
typedef long long longlong_t;
#define LL_(x) COMBINE(x, LL)
#endif//COMPILER

inline void           OleInit() { CoInitialize(NULL); }

inline const wchar_t *WcharT(const wchar *w)   { return (const wchar_t *)w; }
inline wchar_t       *WcharT(wchar *w)         { return (wchar_t *)w; }
inline const wchar   *TWchar(const wchar_t *w) { return (const wchar *)w; }
inline wchar         *TWchar(wchar_t *w)       { return (wchar *)w; }

template <class T>
class OleBuffer
{
public:
	OleBuffer(T *ptr = 0) : ptr(ptr) {}
	~OleBuffer()                { Clear(); }

	void     Clear()            { if(ptr) { CoTaskMemFree(ptr); ptr = 0; } }
	T      **Set()              { Clear(); return &ptr; }

	operator T * ()             { return ptr; }
	operator const T * () const { return ptr; }

private:
	T       *ptr;
};

unsigned              GetHashValue(const GUID& guid);
String                Format(const GUID& guid);
String                CFormat(const GUID& guid);
inline void           Serialize(Stream& stream, GUID& guid) { stream.SerializeRaw((byte *)&guid, sizeof(GUID)); }
String                GetInterfaceName(const GUID& guid);
String                GetCoClassName(const GUID& guid);
String                GetDisplayName(const GUID& guid);
template <> bool      IsNull(const GUID& guid);
GUID                  GetCoClassGUID(const char *name, bool prog_id = true);

class Guid : public ValueType<Guid, 70, Moveable<Guid> >
{
public:
	Guid(const Nuller& = Null)               { Clear(); }
	Guid(const char *text, bool prid = true) { guid = GetCoClassGUID(text, prid); }
	Guid(const GUID& guid_) : guid(guid_) {}
	Guid(const Guid& guid_) : guid(guid_.guid) {}
	Guid(IDispatchPtr& dispatch); // dynamic GUID of a dispatch object
	Guid(Value v)                            { *this = ValueTo<Guid>(v); }

	operator Value () const                  { return RichValue<Guid>(*this); }
	operator const GUID& () const            { return guid; }
	operator GUID& ()                        { return guid; }
	const GUID& operator ~() const           { return guid; }

	Guid&    operator = (const GUID& _guid)  { guid = _guid; return *this; }

	bool     IsNullInstance() const          { return UPP::IsNull(guid); }
	bool     IsEmpty() const                 { return IsNull(guid); }
	void     Clear()                         { Zero(guid); }

	operator String () const                 { return UPP::Format(guid); }
	String   CFormat() const                 { return UPP::CFormat(guid); }

	void     Serialize(Stream& stream)       { UPP::Serialize(stream, guid); }

private:
	GUID     guid;
};

template <>
inline unsigned GetHashValue(const Guid& guid) { return GetHashValue(~guid); }
template <>
inline String   AsString(const GUID& guid) { return Format(guid); }
template <>
inline String   AsString(const Guid& guid) { return Format(~guid); }

inline bool     operator == (const Guid& a, const Guid& b) { return ~a == ~b; }
inline bool     operator != (const Guid& a, const Guid& b) { return ~a != ~b; }

class OleExc : public Exc
{
public:
	OleExc(HRESULT hresult);
	OleExc(HRESULT hresult, const char *text);

public:
	HRESULT hresult;
};

HRESULT    LogResult(HRESULT hr);
HRESULT    LogError(HRESULT hr);

inline void OleVerify(HRESULT hr) { if(FAILED(hr)) throw OleExc(hr); }
inline void OleVerify(HRESULT hr, const char *text) { if(FAILED(hr)) throw OleExc(hr, text); }

#define OLE_VERIFY(c) OleVerify(c, #c)

class OleVariant : public VARIANT
{
public:
	OleVariant(int vtype = VT_EMPTY) { VariantInit(this); vt = vtype; }
	OleVariant(const OleVariant& o) { VariantInit(this); VariantCopy(this, const_cast<OleVariant *>(&o)); }
	~OleVariant() { VariantClear(this); }
	OleVariant(const VARIANT& var) { VariantInit(this); VariantCopy(this, const_cast<VARIANT *>(&var)); }

	OleVariant& operator = (const OleVariant& var) { if(this != &var) { VariantClear(this); VariantCopy(this, const_cast<VARIANT *>((VARIANT *)&var)); } return *this; }

	VARIANT operator ~ () const { VARIANT v; VariantInit(&v); VariantCopy(&v, const_cast<OleVariant *>(this)); return v; }
};

Value                 AsValue(const VARIANT& variant);
OleVariant            AsVariant(Value value);

//inline ULARGE_INTEGER AsULarge(dword value)   { ULARGE_INTEGER i; i.QuadPart = value; return i; }
//inline LARGE_INTEGER  AsLarge(int value)      { LARGE_INTEGER i; i.QuadPart = value; return i; }
inline ULARGE_INTEGER AsULarge(int64 value) { ULARGE_INTEGER i; i.QuadPart = value; return i; }
inline LARGE_INTEGER  AsLarge(int64 value)  { LARGE_INTEGER i; i.QuadPart = value; return i; }

double                GetCurrency(const win32_CY_& currency);
inline DATE           ToDATE(Date date)     { return date - Date(1899, 12, 30); }
inline DATE           ToDATE(Time time)     { return (time - Time(1899, 12, 30)) / 86400.0; }
inline Time           FromDATE(DATE date)   { return Time(1899, 12, 30) + (int64)floor(date * 86400.0 + 0.5); }

OleVariant            ValueToVariant(Value v);
void                  ReturnVariant(VARIANT *var, Value v);

inline WString        BSTRToWString(BSTR bstr)    { return WString((wchar *)bstr, SysStringLen(bstr)); }
inline BSTR           WStringToBSTR(WString w)    { return SysAllocStringLen((BSTR)~w, w.GetLength()); }
String                BSTRToString(BSTR bstr);
BSTR                  StringToBSTR(String s);

void                  ReturnString(BSTR *dest, String s);
HRESULT               CheckReturnString(BSTR *bstr, String s);
void                  ReturnWString(BSTR *dest, WString s);
HRESULT               CheckReturnWString(BSTR *bstr, WString s);

ValueArray            SAFEARRAYToValueArray(SAFEARRAY *array);
Vector<WString>       SAFEARRAYToWStringVector(SAFEARRAY *array);
SAFEARRAY            *ToSAFEARRAY(const ValueArray& varray);
SAFEARRAY            *ToSAFEARRAY(const Vector<WString>& varray);
void                  ReturnSAFEARRAY(SAFEARRAY *dest, const ValueArray& array);
void                  ReturnSAFEARRAY(SAFEARRAY *dest, const Vector<WString>& array);
HRESULT               CheckReturnSAFEARRAY(SAFEARRAY *dest, const ValueArray& array);
HRESULT               CheckReturnSAFEARRAY(SAFEARRAY *dest, const Vector<WString>& array);

class OleSafeArray {
public:
	OleSafeArray() : array(NULL)            {}
	OleSafeArray(const ValueArray& va)      { array = ToSAFEARRAY(va); }
	OleSafeArray(const Vector<WString>& vs) { array = ToSAFEARRAY(vs); }
	OleSafeArray(const OleSafeArray& a)     { array = NULL; if(a.array) OleVerify(SafeArrayCopy(a.array, &array)); }
	~OleSafeArray()                         { if(array) SafeArrayDestroy(array); }
	
	operator ValueArray () const            { return SAFEARRAYToValueArray(array); }
	operator Vector<WString> () const       { return SAFEARRAYToWStringVector(array); }
	SAFEARRAY *operator ~ () const          { return array; }
	
private:
	SAFEARRAY *array;
};

class OleBstr
{
public:
	OleBstr(const Nuller& = Null) : bstr(NULL) {}
	OleBstr(WString w) : bstr(WStringToBSTR(w)) {}
	OleBstr(String s) : bstr(StringToBSTR(s)) {}
	~OleBstr() { if(bstr) SysFreeString(bstr); }

	void     Clear()             { if(bstr) { SysFreeString(bstr); bstr = NULL; } }
	BSTR    *Set()               { Clear(); return &bstr; }

	operator WString () const    { return BSTRToWString(bstr); }
	operator String () const     { return BSTRToString(bstr); }
	String   ToString() const    { return BSTRToString(bstr); }
//	operator BSTR () const       { return bstr; }
	BSTR     operator ~ () const { return bstr; }
	int      GetLength() const   { return SysStringLen(bstr); }

private:
	BSTR     bstr;
};

inline const RECTL&   ToRectL(const Rect& rc) { return reinterpret_cast<const RECTL&>(rc); }
inline const Rect&    ToRect(const RECTL& rc) { return reinterpret_cast<const Rect&>(rc); }

Color                 PackColor(long rgb);
long                  UnpackColor(Color c);
HRESULT               CheckReturnColor(long *ptr, Color c);

template <class T>
class IRefBase : Moveable< IRefBase<T> >
{
public:
	IRefBase(T *ptr = NULL) : ptr(ptr)             { if(ptr) ptr->AddRef(); }
	IRefBase(const IRefBase& i)                    { if(ptr = i.ptr) ptr->AddRef(); }
	~IRefBase()                                    { if(ptr) ptr->Release(); }

	static GUID GetIID()                           { return __uuidof(T); }

	HRESULT     Create(const GUID& clsid, dword flags = CLSCTX_INPROC_SERVER)
	{ return CoCreateInstance(clsid, NULL, flags, GetIID(), (void **)Set()); }

	void        Clear()                            { if(ptr) { ptr->Release(); ptr = 0; } }

	T          *operator->() const               { ASSERT(ptr); return ptr; }
	T&          operator * () const                { ASSERT(ptr); return *ptr; }
	T          *operator ~ () const                { return ptr; }

	T         **Set()                              { Clear(); return &ptr; }
	IUnknown  **SetUnk()                           { Clear(); return (IUnknown **)&ptr; }
	void      **SetVoid()                          { Clear(); return (void **)&ptr; }
	void        Set(T *p)                          { T *old = ptr; if(ptr = p) ptr->AddRef(); if(old) old->Release(); }

	bool        operator !() const                 { return !ptr; }

	T          *AddRef()                           { if(ptr) ptr->AddRef(); return ptr; }
	T          *Detach()                           { T *out = ptr; ptr = 0; return out; }
	T          *operator - ()                      { return Detach(); }

protected:
	mutable T  *ptr;
};

template <class T>
class IRef : public Moveable< IRef<T>, IRefBase<T> >
{
public:
	IRef()                                           {}
	IRef(IUnknown *u)                                { if(u) OleVerify(u->QueryInterface(__uuidof(T), (void **)&ptr)); }
	IRef(T *t)                                       { if(ptr = t) ptr->AddRef(); }
	IRef(const GUID& clsid, dword flags = CLSCTX_INPROC_SERVER) { OleVerify(Create(clsid, flags)); }
	IRef(const IRef<T>& x) : Moveable< IRef<T>, IRefBase<T> >(x) {}
	explicit IRef(const VARIANT& v)
	{
		ptr = NULL;
		if(v.vt == VT_DISPATCH)
			OleVerify(v.pdispVal->QueryInterface(__uuidof(T), (void **)&ptr));
		else if(v.vt == VT_UNKNOWN)
			OleVerify(v.punkVal->QueryInterface(__uuidof(T), (void **)&ptr));
		else
		{
			ASSERT(v.vt == VT_EMPTY || v.vt == VT_NULL);
		}
	}

	IRef&       operator = (const IRef<T>& x)        { Set(x.ptr); return *this; }
};

template <>
class IRef<IUnknown> : public IRefBase<IUnknown>
{
public:
	IRef()                                           {}
	IRef(IUnknown *u) : IRefBase<IUnknown>(u)        {}
	IRef(const GUID& clsid, dword flags = CLSCTX_INPROC_SERVER) { OleVerify(Create(clsid, flags)); }
	IRef(const IRef<IUnknown>& x) : IRefBase<IUnknown>(x.ptr) {}

	IRef&       operator = (const IRef<IUnknown>& x) { Set(x.ptr); return *this; }
};

Value                 DispatchToValue(IDispatch *disp);
Value                 UnknownToValue(IUnknown *unk);

template <class I>
OleVariant DispVar(I& iface)
{
	IRef<IDispatch> disp = iface;
	OleVariant out(VT_DISPATCH);
	if(!!disp) disp->AddRef();
	out.pdispVal = ~disp;
	return out;
}

template <class T>
struct OleType : Moveable< OleType<T> >
{
	friend T   *Ptr(T& t)                           { return reinterpret_cast<T *>(&reinterpret_cast<char &>(t)); }
	friend T&   Deref(T *t)                         { return reinterpret_cast<T&>(*reinterpret_cast<char *>(t)); }
	friend void Destroy(T& t)                       { t.~T(); }
	friend T&   Construct(T& t)                     { return Deref(new(Ptr(t)) T); }
	friend void CopyConstruct(T& t, T& x)           { new(Ptr(t)) T(x); }
	friend void DeepCopyConstruct(T& t, const T& x) { new(Ptr(t)) T(x); }
};

template <class T, class U>
inline HRESULT QueryInterface(T& src, U& dest, const GUID& guid)
{
	return src->QueryInterface(guid, (void **)dest.Set());
}

template <class T, class U>
inline HRESULT QueryInterface(T& src, U& dest)
{
	return QueryInterface(src, dest, dest.GetIID());
}

class OleStream : public BlockStream
{
public:
	OleStream(IUnknown *stream = 0);

	bool           Open(IUnknown *stream);
	virtual  void  Close();
	virtual  bool  IsOpen() const { return istream; }

protected:
	virtual  void  SetStreamSize(dword size);
	virtual  dword Read(int64 at, void *ptr, dword size);
	virtual  void  Write(int64 at, const void *data, dword size);

	void           RawSeek(int64 pos);

protected:
	IStreamPtr     istream;
	int64          current_offset;
};

END_UPP_NAMESPACE

#endif
