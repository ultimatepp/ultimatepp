inline
Value::Value(const Value& v)
:	data(String::SPECIAL)
{
	if(v.IsRef() || v.data.IsLarge())
		SetLarge(v);
	else
		data.SetSmall(v.data);
 	Magic(); 
}

template<>
inline bool IsPolyEqual(const bool& x, const Value& v) {
	return v.Is<double>() && int(x) == double(v)
	    || v.Is<int64>() && int(x) == int64(v)
	    || v.Is<int>() && int(x) == int(v);
}

template<>
inline bool IsPolyEqual(const int& x, const Value& v) {
	return v.Is<double>() && x == double(v)
	    || v.Is<int64>() && x == int64(v);
}

template<>
inline bool IsPolyEqual(const int64& x, const Value& v) {
	return v.Is<double>() && double(x) == double(v);
}

template<>
inline bool IsPolyEqual(const Date& x, const Value& v) {
	return v.Is<Time>() && ToTime(x) == Time(v);
}

template<>
inline bool IsPolyEqual(const WString& x, const Value& v) {
	return v.GetType() == STRING_V && WString(v) == x;
}

template<class T>
inline int  PolyCompare(const T& a, const Value& b)
{
	return a.PolyCompare(b);
}

template<>
inline int PolyCompare(const WString& x, const Value& v) {
	return IsString(v) && SgnCompare(x, WString(v));
}

template<>
inline int PolyCompare(const String& x, const Value& v) {
	return IsString(v) && SgnCompare((WString)x, (WString)v);
}

template<>
inline int PolyCompare(const bool& x, const Value& v) {
	return v.Is<int64>() ? SgnCompare((int64)x, (int64)v)
	     : IsNumber(v) ? SgnCompare((double)x, (double)v)
	     : 0;
}

template<>
inline int PolyCompare(const int& x, const Value& v) {
	return v.Is<int64>() ? SgnCompare((int64)x, (int64)v)
	     : IsNumber(v) ? SgnCompare((double)x, (double)v)
	     : 0;
}

template<>
inline int PolyCompare(const int64& x, const Value& v) {
	return v.Is<double>() ? SgnCompare((double)x, (double)v)
	     : IsNumber(v) ? SgnCompare((int64)x, (int64)v)
	     : 0;
}

inline int PolyCompare(const double& x, const Value& v) {
	return IsNumber(v) ? SgnCompare((double)x, (double)v) : 0;
}

template<>
inline int PolyCompare(const Date& x, const Value& v) {
	return v.Is<Time>() && SgnCompare(ToTime(x), Time(v));
}

template<>
inline int PolyCompare(const Time& x, const Value& v) {
	return v.Is<Date>() && SgnCompare(ToTime(x), Time(v));
}

template<>
inline unsigned ValueGetHashValue(const bool& x) {
	return UPP::GetHashValue((int64)x);
}

template<>
inline unsigned ValueGetHashValue(const int& x) {
	return UPP::GetHashValue((int64)x);
}

template<>
inline unsigned ValueGetHashValue(const double& x) {
	if(x >= INT64_MIN && x <= INT64_MAX && (int64)x == x)
		return UPP::GetHashValue((int64)x);
	return UPP::GetHashValue(x);
}

template<>
inline unsigned ValueGetHashValue(const Date& x) {
	return UPP::GetHashValue(ToTime(x));
}

template<>
inline unsigned ValueGetHashValue(const WString& x) {
	return UPP::GetHashValue(x.ToString());
}

template <class T>
class RawValueRep : public Value::Void {
public:
	virtual dword GetType() const             { return GetValueTypeNo<T>(); }
	virtual bool  IsNull() const              { return false; }

	T v;

	enum VPICK { PICK };
	enum VDEEP { DEEP };	
	
	const T& Get() const                      { return v; }
	T&       Get()                            { return v; }
	
	RawValueRep(const T& v) : v(v)             {}
	RawValueRep(T rval_ v, VPICK) : v(pick(v)) {}
	RawValueRep(const T& v, VDEEP) : v(v, 1)   {}
	RawValueRep()                              {}
};

template <class T>
class RichValueRep : public RawValueRep<T> {
public:
	virtual bool       IsNull() const                { return UPP::IsNull(this->v); }
	virtual void       Serialize(Stream& s)          { s % this->v; }
	virtual void       Xmlize(XmlIO& xio)            { Upp::Xmlize(xio, this->v); }
	virtual void       Jsonize(JsonIO& jio)          { Upp::Jsonize(jio, this->v); }
	virtual unsigned   GetHashValue() const          { return UPP::ValueGetHashValue(this->v); }
	virtual bool       IsEqual(const Value::Void *p) { ASSERT(dynamic_cast<const RawValueRep<T> *>(p));
	                                                   return static_cast<const RawValueRep<T> *>(p)->Get() == this->v; }
	virtual bool       IsPolyEqual(const Value& b)   { return UPP::IsPolyEqual(this->v, b); }
	virtual String     AsString() const              { return UPP::AsString(this->v); }
	virtual int        Compare(const Value::Void *p) { ASSERT(dynamic_cast<const RawValueRep<T> *>(p));
	                                                   return SgnCompare(this->v, static_cast<const RawValueRep<T> *>(p)->Get()); }
	virtual int        PolyCompare(const Value& b)   { return Upp::PolyCompare(this->v, b); }

	RichValueRep(const T& v) : RawValueRep<T>(v)     {}
	RichValueRep()                                   {}

	static Value::Void *Create()                     { return new RichValueRep; }
};

template <class T>
struct SvoFn {
	static bool       IsNull(const void *p)                      { return UPP::IsNull(*(T *)p); }
	static void       Serialize(void *p, Stream& s)              { s % *(T*)p; }
	static void       Xmlize(void *p, XmlIO& xio)                { Upp::Xmlize(xio, *(T*)p); }
	static void       Jsonize(void *p, JsonIO& jio)              { Upp::Jsonize(jio, *(T*)p); }
	static unsigned   GetHashValue(const void *p)                { return UPP::ValueGetHashValue(*(T*)p); }
	static bool       IsEqual(const void *p1, const void *p2)    { return *(T*)p1 == *(T*)p2; }
	static bool       IsPolyEqual(const void *p, const Value& v) { return UPP::IsPolyEqual(*(T*)p, v); }
	static String     AsString(const void *p)                    { return UPP::AsString(*(T*)p); }
	static int        Compare(const void *p1, const void *p2)    { return SgnCompare(*(T*)p1, *(T*)p2); }
	static int        PolyCompare(const void *p1, const Value& p2) { return UPP::PolyCompare(*(T*)p1, p2); }
};

#define SVO_FN(id, T) \
	static Value::Sval id = { \
		SvoFn<T>::IsNull, SvoFn<T>::Serialize, SvoFn<T>::Xmlize, SvoFn<T>::Jsonize, \
		SvoFn<T>::GetHashValue, SvoFn<T>::IsEqual, \
		SvoFn<T>::IsPolyEqual, SvoFn<T>::AsString, \
		SvoFn<T>::Compare, SvoFn<T>::PolyCompare \
	};

template <class T>
void Value::InitSmall(const T& init)
{
	ASSERT(sizeof(T) <= 8);
	SVO_FN(sval, T)
	int typeno = GetValueTypeNo<T>();
	ASSERT(typeno >= 0 && typeno < 256);
	svo[typeno] = &sval;
	data.SetSpecial(typeno);
	new(&data) T(init);
}

template <class T>
inline T& Value::GetSmallRaw() const
{
	return *(T*)&data; // Silence compiler warning
}

template <class T>
T& Value::GetSmall() const
{
	dword t = GetValueTypeNo<T>();
	ASSERT(t < 255);
	if(Is((byte)t))
		return GetSmallRaw<T>();
	throw ValueTypeError(String().Cat() << "Invalid value conversion: "
	                     << GetName() << " -> " << typeid(T).name(),
	                     *this, t);
	return *(T*)&data; // Silence compiler warning
}

template <class T>
void Value::SvoRegister(const char *name)
{
	dword t = GetValueTypeNo<T>();
	ASSERT(t < 255);
	SVO_FN(sval, T)
	svo[t] = &sval;
	AddName(t, name);
}

template <class T>
inline const T& Value::To() const
{
	dword t = GetValueTypeNo<T>();
	ASSERT(t != VALUEARRAY_V && t != VALUEMAP_V);
#ifndef _DEBUG
	if(t == VALUEARRAY_V) {
		ASSERT(ptr()->GetType() == VALUEARRAY_V);
		return *(T*)this; // Illegal, but works -> better than crash in release mode
	}
	if(t == VALUEMAP_V) {
		ASSERT(ptr()->GetType() == VALUEMAP_V);
		return *(T*)this; // Illegal, but works -> better than crash in release mode
	}
#endif
	if(t == STRING_V) {
		ASSERT(IsString());
		return *reinterpret_cast<const T*>(&data); // Only active when T is String
	}
	if(IsRef()) {
		const RawValueRep<T> *x = dynamic_cast<const RawValueRep<T>*>(ptr());
		if(x)
			return x->Get();
	}
	else {
		ASSERT(t < 255);
		if(Is((byte)t))
			return GetSmallRaw<T>();
	}
	throw ValueTypeError(String().Cat() << "Invalid value conversion: "
	                     << GetName() << " -> " << typeid(T).name(),
	                     *this, t);
}

template <class T>
inline bool Value::Is() const
{
	dword t = GetValueTypeNo<T>();
	if(t > 0x80000000)
		return IsRef() && dynamic_cast<const RawValueRep<T> *>(ptr());
	if(t == STRING_V)
		return IsString();
	if(t == VOID_V)
		return IsVoid();
	if(t == INT_V || t == INT64_V || t == DOUBLE_V || t == BOOL_V ||
	   t == DATE_V || t == TIME_V)
	   	return Is((byte)t);
	return t < 255 && Is((byte)t) || IsRef() && ptr()->GetType() == t;
}

template <class T>
const T& GetStaticNull()
{
	static T *q;
	ONCELOCK {
		static T x;
		SetNull(x);
		q = &x;
	}
	return *q;
}

template <class T>
inline const T& Value::Get() const
{
#ifndef _DEBUG
	dword t = GetValueTypeNo<T>();
	if(t == VALUEARRAY_V) {
		ASSERT(ptr()->GetType() == VALUEARRAY_V);
		return *(T*)this; // Illegal, but works -> better than crash in release mode
	}
	if(t == VALUEMAP_V) {
		ASSERT(ptr()->GetType() == VALUEMAP_V);
		return *(T*)this; // Illegal, but works -> better than crash in release mode
	}
#endif
	if(IsNull())
		return GetStaticNull<T>();
	return To<T>();
}

template <class T>
Value::Value(const T& x, VSMALL)
{
	InitSmall(x);
}

inline
unsigned Value::GetHashValue() const
{
	return IsString() ? data.GetCount() ? data.GetHashValue() : 0
	                  : GetOtherHashValue();
}

template <class T>
void Value::Register(const char *name)
{
	dword t = GetValueTypeNo<T>();
	Value::Register(t, RichValueRep<T>::Create, name);
}

inline
const Value& Value::operator[](const char *key) const
{
	return operator[](String(key));
}

inline
const Value& Value::operator[](const Id& key) const
{
	return operator[](~key);
}

template <class T>
inline Value RawToValue(const T& data)
{
	return Value(new RawValueRep<T>(data));
}

template <class T>
inline Value RawPickToValue(T rval_ data)
{
	typedef RawValueRep<T> R;
	return Value(new R(pick(data), R::PICK));
}

template <class T>
inline Value RawDeepToValue(const T& data)
{
	typedef RawValueRep<T> R;
	return Value(new R(data, R::DEEP));
}

template <class T>
inline T& CreateRawValue(Value& v) {
	typedef RawValueRep<T> R;
	R *r = new R;
	v = Value(r);
	return r->Get();	
}

template <class T>
inline Value RichToValue(const T& data)
{
	return Value(new RichValueRep<T>(data));
}

template <class T> // Deprecated, use Value::Is
bool IsTypeRaw(const Value& value, T * = 0)                { return value.Is<T>(); }

template <class T> // deprecated, use Value::Is
bool IsType(const Value& x, T* = 0)                        { return x.Is<T>(); }

template <class T>
struct RawValue : public Value { // Deprecated, use RawToValue and Value::To
	RawValue(const T& x) : Value(RawToValue(x))            {}
	static const T& Extract(const Value& v)                { return v.To<T>(); }
	static const T& Extract(const Value& v, const T& dflt) { return v.Is<T>() ? v.To<T>() : dflt; }
};

template <class T>
struct RichValue : public Value { // Deprecated, use RichToValue and Value::To
public:
	RichValue(const T& x) : Value(RichToValue<T>(x))       {}
	static void Register() init_                           { Value::Register<T>(); }
	static const T& Extract(const Value& v)                { return v.Get<T>(); }
};

template <class T> // Deprecated, use Value::To
inline const T& ValueTo(const Value& v)                    { return v.To<T>(); }

template <class T> // Deprecated, use Value::To
inline const T& ValueTo(const Value& v, const T& dflt)     { return v.Is<T>() ? v.To<T>() : dflt; }


template <class T> // Deprecated (?)
struct RawRef : public RefManager {
	virtual void  SetValue(void *p, const Value& v)       { *(T *) p = RawValue<T>::Extract(v); }
	virtual Value GetValue(const void *p)                 { return RawValue<T>(*(const T *) p); }
	virtual int   GetType()                               { return GetValueTypeNo<T>(); }
	virtual ~RawRef() {}
};

template <class T>
Ref RawAsRef(T& x) { // Deprecated (?)
	return Ref(&x, &Single< RawRef<T> >());
}

template <class T> // Deprecated
struct RichRef : public RawRef<T> {
	virtual Value GetValue(const void *p)                 { return RichValue<T>(*(T *) p); }
	virtual bool  IsNull(const void *p)                   { return UPP::IsNull(*(T *) p); }
	virtual void  SetValue(void *p, const Value& v)       { *(T *) p = T(v); }
	virtual void  SetNull(void *p)                        { UPP::SetNull(*(T *)p); }
};

template <class T> // Deprecated
Ref RichAsRef(T& x) {
	return Ref(&x, &Single< RichRef<T> >());
}
