
struct IdConst {
	const char *text;
	mutable int ndx;
};

class Value;

class Id : Moveable<Id> {
protected:
	int ndx;
	static Index<String>& Ids();
	void           Set(const String& s);

public:
	int            AsNdx() const              { return ndx; }
	String         ToString() const;
	dword          GetHashValue() const       { return ndx; }
	bool           IsNull() const             { return ndx == 0; }

	void           Serialize(Stream& s);

	operator       String() const             { return ToString(); }
	String         operator~() const          { return ToString(); }
	bool           operator==(Id b) const     { return ndx == b.ndx; }
	bool           operator!=(Id b) const     { return ndx != b.ndx; }

	static Id            Find(const String& s);
	static const String& AsString(int n);

	operator bool() const                     { return ndx; }

	Id()                                      { ndx = 0; }
	Id(const String& s)                       { Set(s); }
	Id(const char *s)                         { Set(s); }
	explicit Id(int n)                        { ASSERT(n >= 0 && n < Ids().GetCount()); ndx = n; }
	Id(const IdConst& cnst);
};

const int    INT_NULL           =    INT_MIN;
const int64  INT64_NULL         =    INT64_MIN;

const double DOUBLE_NULL        =    -1.0E+308;
const double DOUBLE_NULL_LIM    =    -1.0E+307;

class Nuller {
public:
	operator int() const                { return INT_NULL; }
	operator int64() const              { return INT64_NULL; }
	operator double() const             { return DOUBLE_NULL; }
	operator bool() const               { return false; }

	Nuller() {}
};

#ifdef flagSO
static const Nuller Null;
#else
extern const Nuller Null;
#endif

template<> inline bool  IsNull(const int& i)     { return i == INT_NULL; }
template<> inline bool  IsNull(const int64& i)   { return i == INT64_NULL; }
template<> inline bool  IsNull(const double& r)  { return r < DOUBLE_NULL_LIM; }
template<> inline bool  IsNull(const bool& r  )  { return false; }
template<> inline bool  IsNull(const Date& d)    { return d.year == -32768; }
template<> inline bool  IsNull(const Time& t)    { return t.year == -32768; }

inline const String&  Nvl(const String& a, const String& b)    { return IsNull(a) ? b : a; }
inline int            Nvl(int a, int b)                        { return IsNull(a) ? b : a; }
inline int64          Nvl(int64 a, int64 b)                    { return IsNull(a) ? b : a; }
inline double         Nvl(double a, double b)                  { return IsNull(a) ? b : a; }
inline Date           Nvl(Date a, Date b)                      { return IsNull(a) ? b : a; }
inline Time           Nvl(Time a, Time b)                      { return IsNull(a) ? b : a; }

inline int            Nvl(int a)                               { return Nvl(a, 0); }
inline int64          Nvl(int64 a)                             { return Nvl(a, (int64)0); }
inline double         Nvl(double a)                            { return Nvl(a, 0.0); }

const dword VOID_V    = 0;

const dword INT_V     = 1;
const dword DOUBLE_V  = 2;
const dword STRING_V  = 3;
const dword DATE_V    = 4;
const dword TIME_V    = 5;

const dword ERROR_V   = 6;

const dword VALUE_V   = 7;

const dword WSTRING_V = 8;

const dword VALUEARRAY_V = 9;

const dword INT64_V  = 10;
const dword BOOL_V   = 11;

const dword VALUEMAP_V   = 12;

const dword UNKNOWN_V = (dword)0xffffffff;

class Value : Moveable<Value> {
public:
	class Void {
	protected:
		Atomic  refcount;

	public:
		void               Retain()                    { AtomicInc(refcount); }
		void               Release()                   { if(AtomicDec(refcount) == 0) delete this; }

		virtual dword      GetType() const             { return VOID_V; }
		virtual bool       IsNull() const              { return true; }
		virtual void       Serialize(Stream& s)        {}
		virtual unsigned   GetHashValue() const        { return 0; }
		virtual bool       IsEqual(const Void *p)      { return false; }
		virtual bool       IsPolyEqual(const Value& v) { return false; }
		virtual String     AsString() const            { return ""; }

		Void()                                         { AtomicWrite(refcount, 1); }
		virtual ~Void()                                {}

		friend class Value;
	};

protected:
	static VectorMap<dword, Void* (*)(Stream& s) >& Typemap();

	Void    *ptr;

	void     SetVoidVal();

public:
	static  void Register(dword w, Void* (*c)(Stream& s)) init_;

	dword    GetType() const         { return ptr->GetType(); }
	bool     IsError() const         { return GetType() == ERROR_V; }
	bool     IsVoid() const          { return GetType() == VOID_V || IsError(); }
	bool     IsNull() const          { return ptr->IsNull(); }

	template <class T>
	bool     Is() const;

	operator String() const;
	operator WString() const;
	operator Date() const;
	operator Time() const;
	operator double() const;
	operator int() const;
	operator int64() const;
	operator bool() const;

	Value(const String& s);
	Value(const WString& s);
	Value(const char *s);
	Value(int i);
	Value(int64 i);
	Value(double d);
	Value(bool b);
	Value(Date d);
	Value(Time t);
	Value(const Nuller&);

	bool operator==(const Value& v) const;
	bool operator!=(const Value& v) const { return !operator==(v); }

	String ToString() const;

	void  Serialize(Stream& s);

	unsigned GetHashValue() const;

	Value& operator=(const Value& v);
	Value(const Value& v);

	Value();
	~Value();

	Value(Void *p)                        { ptr = p; }
	const Void *GetVoidPtr() const        { return ptr; }
};

#define VALUE_COMPARE(T) \
inline bool operator==(const Value& v, T x)   { return (T)v == x; } \
inline bool operator==(T x, const Value& v)   { return (T)v == x; } \
inline bool operator!=(const Value& v, T x)   { return (T)v != x; } \
inline bool operator!=(T x, const Value& v)   { return (T)v != x; } \

VALUE_COMPARE(int)
VALUE_COMPARE(int64)
VALUE_COMPARE(double)
VALUE_COMPARE(bool)
VALUE_COMPARE(Date)
VALUE_COMPARE(Time)
VALUE_COMPARE(String)
VALUE_COMPARE(WString)

inline bool operator==(const Value& v, const char *x)   { return (String)v == x; }
inline bool operator==(const char *x, const Value& v)   { return (String)v == x; }
inline bool operator!=(const Value& v, const char *x)   { return (String)v != x; }
inline bool operator!=(const char *x, const Value& v)   { return (String)v != x; }

inline bool operator==(const Value& v, const wchar *x)  { return (WString)v == x; }
inline bool operator==(const wchar *x, const Value& v)  { return (WString)v == x; }
inline bool operator!=(const Value& v, const wchar *x)  { return (WString)v != x; }
inline bool operator!=(const wchar *x, const Value& v)  { return (WString)v != x; }

inline bool IsVoid(const Value& v)     { return v.GetType() == VOID_V; }
inline bool IsError(const Value& v)    { return v.GetType() == ERROR_V; }
inline bool IsString(const Value& v)   { return v.GetType() == STRING_V || v.GetType() == WSTRING_V; }
inline bool IsNumber(const Value& v)   { return v.GetType() == DOUBLE_V || v.GetType() == INT_V
                                                || v.GetType() == INT64_V || v.GetType() == BOOL_V; }
inline bool IsDateTime(const Value& v) { return v.GetType() == DATE_V || v.GetType() == TIME_V; }

int StdValueCompare(const Value& a, const Value& b, int language);
int StdValueCompare(const Value& a, const Value& b);

int StdValueCompareDesc(const Value& a, const Value& b, int language);
int StdValueCompareDesc(const Value& a, const Value& b);

struct ValueOrder {
	virtual bool operator()(const Value& a, const Value& b) const = 0;
	virtual ~ValueOrder() {}
};

struct StdValueOrder : ValueOrder {
	int language;

	virtual bool operator()(const Value& a, const Value& b) const;

	StdValueOrder(int l = -1);
	virtual ~StdValueOrder();
};

struct FnValueOrder : ValueOrder {
	int (*fn)(const Value& a, const Value& b);

	virtual bool operator()(const Value& a, const Value& b) const;

	FnValueOrder(int (*fn)(const Value& a, const Value& b));
	virtual ~FnValueOrder();
};

template <class T>
inline dword ValueTypeNo(const T&)       { return StaticTypeNo<T>() + 0x8000000;; }

template<> inline dword ValueTypeNo(const int&)     { return INT_V; }
template<> inline dword ValueTypeNo(const int64&)   { return INT64_V; }
template<> inline dword ValueTypeNo(const double&)  { return DOUBLE_V; }
template<> inline dword ValueTypeNo(const bool&)    { return BOOL_V; }
template<> inline dword ValueTypeNo(const String&)  { return STRING_V; }
template<> inline dword ValueTypeNo(const WString&) { return WSTRING_V; }
template<> inline dword ValueTypeNo(const Date&)    { return DATE_V; }
template<> inline dword ValueTypeNo(const Time&)    { return TIME_V; }

template <class T, dword type, class B = EmptyClass>
class AssignValueTypeNo : public B {
public:
	friend dword ValueTypeNo(const T&)                  { return type; }

	void operator=(const AssignValueTypeNo&) {} // MSC 6.0 empty base class bug fix
};

template <class T>
bool IsType(const Value& x, T* = 0)           { return ValueTypeNo(*(T *)NULL) == x.GetType(); }

template <class T>
inline bool Value::Is() const
{
	return IsType<T>(*this);
}

template <class T>
class RawValueRep : public Value::Void {
protected:
	T v;

public:
	virtual dword GetType() const             { return ValueTypeNo(v); }
	virtual bool  IsNull() const              { return false; }

	const T& Get() const                      { return v; }
	RawValueRep(const T& v) : v(v)            {}
	RawValueRep()                             {}
	static const RawValueRep *Cast(const Value::Void *p) {
		ASSERT_(dynamic_cast<const RawValueRep *>(p),
		        String().Cat() << "Invalid value conversion: "
		                       << typeid(*p).name() << " -> " << typeid(T).name());
		return (const RawValueRep *) p;
	}
};

template <class T>
bool IsTypeRaw(const Value& value, T * = 0) {
	return !IsVoid(value) && dynamic_cast<const RawValueRep<T> *>(value.GetVoidPtr());
}

template <class T>
class RawValue : public Value {
protected:
	typedef RawValueRep<T> Rep;

	RawValue(Rep *rep) : Value(rep) {}

public:
	RawValue(const T& x) : Value(new Rep(x)) {}
	static const T& Extract(const Value& v) {
		return Rep::Cast(v.GetVoidPtr())->Get();
	}
	static const T& Extract(const Value& v, const T& dflt) {
		return IsTypeRaw<T>(v) ? Rep::Cast(v.GetVoidPtr())->Get() : dflt;
	}
};

template <class T>
class RawPickValueRep : public RawValueRep<T> {
public:
	RawPickValueRep(pick_ T& _v)      { this->v = _v; }
	RawPickValueRep(const T& _v, int) { this->v <<= _v; }
	RawPickValueRep()                 {}
};

template <class T>
class RawPickValue : public RawValue<T> {
protected:
	typedef RawPickValueRep<T> PickRep;

public:
	RawPickValue(pick_ T& x) : RawValue<T>(new PickRep(x)) {}
	RawPickValue(const T& x, int) : RawValue<T>(new PickRep(x, 0)) {}
};

template <class T>
class RawValueCreateRep : public RawValueRep<T> {
public:
	T& Get()                        { return this->v; }
};

template <class T>
class RawValueCreate : public RawValue<T> {
protected:
	typedef RawValueCreateRep<T> Rep;

public:
	RawValueCreate() : RawValue<T>(new Rep()) {}
	T& Get()                                     { return ((Rep *)this->GetVoidPtr())->Get(); }
};

template <class T>
inline Value RawToValue(const T& data)           { return RawValue<T>(data); }

template <class T>
inline Value RawPickToValue(pick_ T& data)       { return RawPickValue<T>(data); }

template <class T>
inline Value RawDeepToValue(const T& data)       { return RawPickValue<T>(data, 0); }

template <class T>
inline T& CreateRawValue(Value& v) {
	RawValueCreate<T> x;
	v = x;
	return x.Get();
}

template <class T>
inline const T& ValueTo(const Value& v, T * = 0)       { return RawValue<T>::Extract(v); }

template <class T>
inline const T& ValueTo(const Value& v, const T& dflt) { return RawValue<T>::Extract(v, dflt); }

template <class T>
inline bool IsPolyEqual(const T& x, const Value& v) {
	return false;
}

template <class T>
inline unsigned ValueGetHashValue(const T& x) {
	return UPP::GetHashValue(x);
}

inline bool IsPolyEqual(const bool& x, const Value& v) {
	return v.GetType() == DOUBLE_V && int(x) == double(v)
	    || v.GetType() == INT64_V && int(x) == int64(v)
	    || v.GetType() == INT_V && int(x) == int(v);
}

inline bool IsPolyEqual(const int& x, const Value& v) {
	return v.GetType() == DOUBLE_V && x == double(v)
	    || v.GetType() == INT64_V && x == int64(v);
}

inline bool IsPolyEqual(const int64& x, const Value& v) {
	return v.GetType() == DOUBLE_V && double(x) == double(v);
}

inline bool IsPolyEqual(const Date& x, const Value& v) {
	return v.GetType() == TIME_V && ToTime(x) == Time(v);
}

inline bool IsPolyEqual(const WString& x, const Value& v) {
	return v.GetType() == STRING_V && WString(v) == x;
}

inline unsigned ValueGetHashValue(const bool& x) {
	return UPP::GetHashValue((double)x);
}

inline unsigned ValueGetHashValue(const int& x) {
	return UPP::GetHashValue((double)x);
}

inline unsigned ValueGetHashValue(const int64& x) {
	return UPP::GetHashValue((double)x);
}

inline unsigned ValueGetHashValue(const Date& x) {
	return UPP::GetHashValue(ToTime(x));
}

inline unsigned ValueGetHashValue(const String& x) { // Improve by specialized routines !!!
	return UPP::GetHashValue((WString)x);
}

template <class T>
class RichValueRep : public RawValueRep<T> {
public:
	virtual bool       IsNull() const                { return UPP::IsNull(this->v); }
	virtual void       Serialize(Stream& s)          { s % this->v; }
	virtual unsigned   GetHashValue() const          { return UPP::ValueGetHashValue(this->v); }
	virtual bool       IsEqual(const Value::Void *p) { return Cast(p)->Get() == this->v; }
	virtual bool       IsPolyEqual(const Value& b)   { return UPP::IsPolyEqual(this->v, b); }
	virtual String     AsString() const              { return UPP::AsString(this->v); }

	RichValueRep(const T& v) : RawValueRep<T>(v)     {}
	RichValueRep(Stream& s)                          { Serialize(s); }

	static Value::Void *Create(Stream& s)            { return new RichValueRep(s); }
	static const RichValueRep *Cast(const Value::Void *p) {
		ASSERT_(dynamic_cast<const RichValueRep *>(p),
		        String().Cat() << "Invalid value conversion: "
		                       << typeid(*p).name() << " -> " << typeid(T).name());
		return (const RichValueRep *) p;
	}
};

template <class T>
class RichValue : public Value {
protected:
	typedef RichValueRep<T> Rep;

	static const T& GetNull() {
		static T *q;
		ONCELOCK {
			static T x(Null);
			q = &x;
		}
		return *q;
	}

public:
	RichValue(const T& x) : Value(new Rep(x)) {}
	const T& Get(const Value& v) {
		if(IsNull(v)) return GetNull();
		return Rep::Cast(v.GetVoidPtr())->Get();
	}
	static void Register() init_ {
		Value::Register(ValueTypeNo(*(T *)NULL), Rep::Create);
	}
	static const T& Extract(const Value& v) {
		if(v.IsNull()) return GetNull();
		return Rep::Cast(v.GetVoidPtr())->Get();
	}
};

template <class T>
inline Value RichToValue(const T& data)                   { return RichValue<T>(data); }

Value ErrorValue(const char *s);
Value ErrorValue(const String& s);
Value ErrorValue();

String GetErrorText(const Value& v);

inline bool          IsNull(const Value& v)               { return v.IsNull(); }
inline const Value&  Nvl(const Value& a, const Value& b)  { return IsNull(a) ? b : a; }

Value Scan(dword stdtype, const String& text);

// ----------------------- Ref

struct RefManager {
	virtual int   GetType() = 0;
	virtual Value GetValue(const void *)            { return Null; }
	virtual bool  IsNull(const void *)              { return false; }
	virtual void  SetValue(void *, const Value& v)  { NEVER(); }
	virtual void  SetNull(void *)                   { NEVER(); }
	virtual ~RefManager() {}
};


template <class T>
struct RawRef : public RefManager {
	virtual void  SetValue(void *p, const Value& v) { *(T *) p = RawValue<T>::Extract(v); }
	virtual Value GetValue(const void *p)           { return RawValue<T>(*(const T *) p); }
	virtual int   GetType()                         { return ValueTypeNo(*(T *)NULL); }
	virtual ~RawRef() {}
};

template <class T>
struct RichRef : public RawRef<T> {
	virtual Value GetValue(const void *p)           { return RichValue<T>(*(T *) p); }
	virtual bool  IsNull(const void *p)             { return UPP::IsNull(*(T *) p); }
	virtual void  SetValue(void *p, const Value& v) { *(T *) p = T(v); }
	virtual void  SetNull(void *p)                  { *(T *) p = T(Null); }
};

class Ref : Moveable<Ref> {
protected:
	RefManager *m;
	void       *ptr;
	struct      ValueRef;

public:
	dword    GetType() const           { return m ? m->GetType() : VOID_V; }
	operator Value() const             { return m ? m->GetValue(ptr) : Value(); }
	bool     IsNull() const            { return m ? m->IsNull(ptr) : true; }

	void    *GetVoidPtr() const        { return ptr; }

	void     SetNull()                 { if(m) m->SetNull(ptr); }
	void     SetValue(const Value& v)  { ASSERT(m); m->SetValue(ptr, v); }

	Ref&     operator=(const Value& v) { SetValue(v); return *this; }

	Ref(String& s);
	Ref(WString& s);
	Ref(int& i);
	Ref(int64& i);
	Ref(double& d);
	Ref(Date& d);
	Ref(Time& t);
	Ref(Value& v);
	Ref(void *_ptr, RefManager *_m)    { ptr = _ptr, m = _m; }
	Ref()                              { ptr = m = NULL; }
};

template <class T>
T& GetRef(Ref r, T *x = NULL) {
	ASSERT(ValueTypeNo(*x) == r.GetType());
	return *(T *) r.GetVoidPtr();
}

inline String&  RefString(Ref f)  { return GetRef<String>(f); }
inline WString& RefWString(Ref f) { return GetRef<WString>(f); }
inline int&     RefInt(Ref f)     { return GetRef<int>(f); }
inline int64&   RefInt64(Ref f)   { return GetRef<int64>(f); }
inline double&  RefDouble(Ref f)  { return GetRef<double>(f); }
inline Date&    RefDate(Ref f)    { return GetRef<Date>(f); }
inline Time&    RefTime(Ref f)    { return GetRef<Time>(f); }
inline Value&   RefValue(Ref f)   { ASSERT(f.GetType() == VALUE_V);
                                    return *(Value *)f.GetVoidPtr(); }

template <class T>
Ref RawAsRef(T& x) {
	return Ref(&x, &Single< RawRef<T> >());
}

template <class T>
Ref RichAsRef(T& x) {
	return Ref(&x, &Single< RichRef<T> >());
}

#define E__Value(I)   Value p##I
#define E__Ref(I)     Ref p##I

// ---------------------- Value Array

class ValueArray : AssignValueTypeNo<ValueArray, VALUEARRAY_V, Moveable<ValueArray> > {
	struct Data : Value::Void {
		virtual dword      GetType() const             { return VALUEARRAY_V; }
		virtual bool       IsNull() const;
		virtual void       Serialize(Stream& s);
		virtual unsigned   GetHashValue() const;
		virtual bool       IsEqual(const Value::Void *p);
		virtual String     AsString() const;

		int GetRefCount() const     { return AtomicRead(refcount); }

		Vector<Value> data;
	};
	struct NullData : Data {};
	Data *data;

	Vector<Value>& Create();
	Vector<Value>& Clone();

public:
	ValueArray();
	ValueArray(const ValueArray& v);
	explicit ValueArray(pick_ Vector<Value>& values);
	explicit ValueArray(const Vector<Value>& values, int deep);
	~ValueArray();

	ValueArray& operator=(const ValueArray& v);

	operator Value() const;
	ValueArray(const Value& src);

	ValueArray(const Nuller&);
	bool IsNull() const                       { return data->IsNull(); }

	void Clear();
	void SetCount(int n);
	void SetCount(int n, const Value& v);
	int  GetCount() const                     { return data->data.GetCount(); }
	bool IsEmpty() const                      { return data->data.IsEmpty(); }

	void Add(const Value& v);
	void Set(int i, const Value& v);
	void Remove(int i);
	const Value& Get(int i) const;
	const Vector<Value>& Get() const          { return data->data; }

	const Value& operator[](int i) const      { return Get(i); }

	unsigned GetHashValue() const             { return data->GetHashValue(); }
	void  Serialize(Stream& s);

	bool operator==(const ValueArray& v) const;
	bool operator!=(const ValueArray& v) const  { return !operator==(v); }
};

template<> inline unsigned GetHashValue(const ValueArray& v) { return v.GetHashValue(); }
template<> inline bool     IsNull(const ValueArray& v)       { return v.IsNull(); }

template<>
String AsString(const ValueArray& v);

class ValueMap : AssignValueTypeNo<ValueMap, VALUEMAP_V, Moveable<ValueMap> >{
	struct Data : Value::Void {
		virtual dword      GetType() const             { return VALUEMAP_V; }
		virtual bool       IsNull() const;
		virtual void       Serialize(Stream& s);
		virtual unsigned   GetHashValue() const;
		virtual bool       IsEqual(const Value::Void *p);
		virtual String     AsString() const;

		int GetRefCount() const     { return AtomicRead(refcount); }

		Index<Value> key;
		ValueArray   value;
	};

	struct NullData : Data {};
	Data *data;

	Data& Create();
	Data& Clone();
	void  Init0();

public:
	ValueMap()                               { Init0(); }
	ValueMap(const ValueMap& v);
	~ValueMap();

	ValueMap& operator=(const ValueMap& v);

	operator Value() const;
	ValueMap(const Value& src);

	ValueMap(const Nuller&)                  { Init0(); }
	bool IsNullInstance() const              { return data->IsNull(); }

	void Clear();

	void Add(const Value& key, const Value& value);
	void Add(const char *s, const Value& value) { Add(Value(s), value); }
	void Add(Id id, const Value& value)       { Add(Value(id.ToString()), value); }

	const Index<Value>& GetKeys() const       { return data->key; }
	ValueArray GetValues() const              { return data->value; }

	operator ValueArray() const               { return GetValues(); }

	Value operator[](const Value& k) const;
	Value operator[](const char *s) const     { return operator[](Value(s)); }
	Value operator[](const Id& k) const       { return operator[](Value(k.ToString())); }

	unsigned GetHashValue() const             { return data->GetHashValue(); }
	void     Serialize(Stream& s);
	String   ToString() const                 { return data->AsString(); }

	bool operator==(const ValueMap& v) const;
	bool operator!=(const ValueMap& v) const  { return !operator==(v); }
};

inline bool IsValueArray(const Value& v) { return v.GetType() == VALUEARRAY_V || v.GetType() == VALUEMAP_V; }
inline bool IsValueMap(const Value& v)   { return IsValueArray(v); }


class ValueGen {
public:
	virtual Value  Get() = 0;
	Value operator++()                           { return Get(); }
	virtual ~ValueGen() {}
};
