#ifndef __tweb_util__
#define __tweb_util__

String        FormatIP(dword _ip);
String        UrlEncode(const String& s, const char *specials);
bool          IsSameTextFile(const char *p, const char *q);
String        StringSample(const char *s, int limit);
String        GetRandomIdent(int length);
String        OtpEncode(const String& password, const String& otp_key);
String        EncryptString(const String& password, const String& otp_key);
String        BinHexEncode(const char *b, const char *e);
inline String BinHexEncode(const String& data)    { return BinHexEncode(data.Begin(), data.End()); }
String        BinhexEncode(const char *b, const char *e);
inline String BinhexEncode(const String& data)    { return BinhexEncode(data.Begin(), data.End()); }
String        BinHexDecode(const char *b, const char *e);
inline String BinHexDecode(const String& data)    { return BinHexDecode(data.Begin(), data.End()); }
String        ASCII85Encode(const byte *p, int length);
inline String ASCII85Encode(const String& s)      { return ASCII85Encode(s, s.GetLength()); }
String        ASCII85Decode(const byte *p, int length);
inline String ASCII85Decode(const String& s)      { return ASCII85Decode(s, s.GetLength()); }
dword         AddCRC(dword crc, const byte *data, int count);
inline dword  AddCRC(dword crc, const String& s)  { return AddCRC(crc, s, s.GetLength()); }
inline dword  GetCRC(const byte *data, int count) { return AddCRC(0x80000000, data, count); }
inline dword  GetCRC(const String& s)             { return AddCRC(0x80000000, s, s.GetLength()); }
int           LocateLine(String old_file, int old_line, String new_file);
void          AppVersion(const char *ver);
String        MD5Digest(const char *text, int length);
inline String MD5Digest(String s)                 { return MD5Digest(s.Begin(), s.GetLength()); }

#define WID(s) static const String COMBINE(WID_, s)(#s);

class RefBase
{
public:
	RefBase()
	{
		refcount = 0;
#ifdef REF_DEBUG
		allocindex = ++nextindex;
#endif//REF_DEBUG
	}

	RefBase(const RefBase& rb)
	{
		refcount = 0;
#ifdef REF_DEBUG
		allocindex = ++nextindex;
#endif//REF_DEBUG
	}

	virtual ~RefBase()
	{
		ASSERT(refcount == 0);
	}

	void           AddRef() const      { if(this) AtomicInc(refcount); }
	int            GetRefCount() const { return AtomicXAdd(refcount, 0); }
	void           Release() const     { if(this && !AtomicDec(refcount)) delete this; }
#ifdef REF_DEBUG
	int            GetAllocIndex() const { return allocindex; }
#endif//REF_DEBUG

private:
	mutable Atomic refcount;
#ifdef REF_DEBUG
	int            allocindex;
	static int     nextindex;
#endif//REF_DEBUG

private:
	RefBase&    operator = (const RefBase& rb) { NEVER(); return *this; }
};

template <class T>
class RefCon : Moveable< RefCon<T> >
{
public:
	RefCon(const Nuller& = Null) : t(0) {}
	RefCon(const T *t);
	RefCon(const RefCon<T>& rp);
	~RefCon();

	void            Clear()                                { if(t) { t->Release(); t = NULL; } }
	bool            IsNullInstance() const                 { return !t; }
	dword           GetHashValue() const                   { return UPP::GetHashValue((unsigned)(uintptr_t)t); }

	RefCon<T>&      operator = (const RefCon<T>& rp);

	bool            operator ! () const                    { return !t; }
	const T        *Get() const                            { return t; }
	const T        *operator ~ () const                    { return t; }
	const T        *operator -> () const                   { ASSERT(t); return t; }
	const T&        operator * () const                    { ASSERT(t); return *t; }

	String          ToString() const                       { return t ? AsString(*t) : String("NULL"); }

	friend bool     operator == (RefCon<T> a, RefCon<T> b) { return a.t == b.t; }
	friend bool     operator != (RefCon<T> a, RefCon<T> b) { return a.t != b.t; }

protected:
	const T        *t;
};

template <class T>
RefCon<T>::RefCon(const T *t)
: t(t)
{
	t->AddRef();
#ifdef REF_DEBUG
	if(t && t->GetRefCount() == 1)
		RefMemStat::App().Add(typeid(*t).name(), t->GetAllocIndex());
#endif//REF_DEBUG
}

template <class T>
RefCon<T>::RefCon(const RefCon<T>& rp)
: t(rp.t)
{ t->AddRef(); }

template <class T>
RefCon<T>::~RefCon()
{
#ifdef REF_DEBUG
	if(t && t->GetRefCount() == 1)
		RefMemStat::App().Remove(typeid(*t).name(), t->GetAllocIndex());
#endif//REF_DEBUG
	t->Release();
}

template <class T>
RefCon<T>& RefCon<T>::operator = (const RefCon<T>& rp)
{
	const T *old = t;
	t = rp.t;
	t->AddRef();
#ifdef REF_DEBUG
	if(old && old->GetRefCount() == 1)
		RefMemStat::App().Remove(typeid(*old).name(), old->GetAllocIndex());
#endif//REF_DEBUG
	old->Release();
	return *this;
}

template <class T>
class RefPtr : public RefCon<T>, public Moveable< RefPtr<T> >
{
public:
	RefPtr(const Nuller& = Null)                 {}
	RefPtr(T *t) : RefCon<T>(t)                  {}
	RefPtr(const RefPtr<T>& rp) : RefCon<T>(rp)  {}

	RefPtr<T>&  operator = (const RefPtr<T>& rp) { RefCon<T>::operator = (rp); return *this; }

	T          *Get() const                      { return const_cast<T *>(this->t); }
	T          *operator ~ () const              { return Get(); }
	T          *operator -> () const             { ASSERT(this->t); return Get(); }
	T&          operator * () const              { ASSERT(this->t); return *Get(); }
};

template <class T>
class RefValueRep : public RawValueRep<T>
{
public:
	RefValueRep(T v) : RawValueRep<T>(v) {}

	virtual bool       IsNull() const                { return this->v == 0; }
	virtual unsigned   GetHashValue() const          { return (unsigned)~this->v; }
	virtual bool       IsEqual(const Value::Void *p);
	virtual String     AsString()                    { return !!this->v ? UPP::AsString(*this->v) : String(Null); }

	static const RawValueRep<T> *Cast(const Value::Void *p)
	{ ASSERT(dynamic_cast<const RawValueRep<T> *>(p)); return (const RawValueRep<T> *)p; }
};

template <class T>
bool RefValueRep<T>::IsEqual(const Value::Void *p)
{
	const RawValueRep<T> *cast = dynamic_cast<const RawValueRep<T> *>(p);
	return cast && cast->Get() == this->v;
}

template <class T>
class RefPCValue : public Value
{
protected:
	typedef RefValueRep<T> Rep;

public:
	RefPCValue(T x) : Value(new Rep(x)) {}
	static T Get(const Value& v)       { return UPP::IsNull(v) ? 0 : Rep::Cast(v.GetVoidPtr())->Get(); }
	static T Extract(const Value& v)   { return UPP::IsNull(v) ? 0 : Rep::Cast(v.GetVoidPtr())->Get(); }
};

template <class T>
inline Value RefConToValue(RefCon<T> p)        { return RefPCValue< RefCon<T> >(p); }

template <class T>
inline Value RefPtrToValue(RefPtr<T> p)        { return RefPCValue< RefPtr<T> >(p); }

class HttpQuery
{
private:
	class Data : public RefBase
	{
	public:
		Data() {}
		Data(const Data& d) : map(d.map, 0) {}

		VectorMap<String, String> map;
	};
	
	bool case_sensitive;

public:
	HttpQuery(const Nuller& = Null) : data(Empty())    { case_sensitive = false; }
	explicit HttpQuery(String url) { data = Empty(); case_sensitive = false; SetURL(url); case_sensitive = false; }

	void                   CaseSensitive(bool b = true) { case_sensitive = b; }

	void                   Serialize(Stream& stream);

	String                 GetHidden() const;
	String                 GetQuery(bool empty = false) const;
	String                 GetQuery(HttpQuery patch, bool empty = false) const;

	void                   Clear()                                            { data = Empty(); }

	bool                   IsEmpty() const                                    { return data -> map.IsEmpty(); }
	bool                   IsEmpty(String key) const;

	int                    GetCount() const                                   { return data -> map.GetCount(); }
	int                    Find(String key) const                             { return data -> map.Find(key); }
	String                 GetKey(int i) const                                { return data -> map.GetKey(i); }
	String                 GetValue(int i) const                              { return data -> map[i]; }
	bool                   IsInternal(int i) const;
	
	String                 operator [] (int i) const                          { return GetValue(i); }
	String                 operator [] (const String& key) const              { return GetString(key); }

	void                   Get(const String& key, Ref p1) const;
	HttpQuery&             SetValue(const String& key, const Value& v);
	HttpQuery&             Set(const String& key, const String& value);
	HttpQuery&             SetRaw(const String& key, const String& value);

	bool                   GetBool(const String& key) const;
	bool                   GetBool(const String& key, bool dflt) const;
	HttpQuery&             SetBool(const String& key, bool b);

	int                    GetInt(const String& key) const;
	int                    GetInt(const String& key, int min, int max, int dflt = 0) const;
	HttpQuery&             SetInt(const String& key, int i);

	double                 GetDouble(const String& key) const;
	double                 GetDouble(const String& key, double min, double max, double dflt = Null) const;
	HttpQuery&             SetDouble(const String& key, double f);

	String                 GetString(const String& key) const;
	String                 GetString(const String& key, const String& dflt) const;
	HttpQuery&             SetString(const String& key, const String& s)      { return Set(key, s); }

	Date                   GetDate(const String& key) const;
	Date                   GetDate(const String& key, Date dflt) const        { return Nvl(GetDate(key), dflt); }
	HttpQuery&             SetDate(const String& key, Date d);

	Time                   GetTime(const String& key) const;
	Time                   GetTime(const String& key, Time dflt) const        { return Nvl(GetTime(key), dflt); }
	HttpQuery&             SetTime(const String& key, Time t);

	Color                  GetColor(const String& key) const;
	Color                  GetColor(const String& key, Color dflt) const      { return Nvl(GetColor(key), dflt); }
	HttpQuery&             SetColor(const String& key, Color c);

	HttpQuery&             Set(HttpQuery query);
	HttpQuery&             SetURL(const String& url);

	HttpQuery&             Remove(const String& key);
	HttpQuery&             Remove(const Vector<String>& keys);
	HttpQuery&             Remove(const Vector<Id>& keys);

	String                 ToString() const;

private:
	static RefPtr<Data>    Empty();
	void                   Clone() { if(data -> GetRefCount() > 1) data = new Data(*data); }

private:
	RefPtr<Data>           data;
};

#endif//__tweb_util__
