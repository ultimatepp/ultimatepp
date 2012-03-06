#ifdef SVO_VALUE

class   Id;
class   ValueArray;
class   ValueMap;
class   XmlIO;
class   JsonIO;

class   Ref;
struct  ValueTypeRef;

template <class T>
void Jsonize(JsonIO& io, T& var);

template <class T>
void Xmlize(XmlIO& xml, T& var);

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

template <class T>
inline dword ValueTypeNo(const T *)                 { return StaticTypeNo<T>() + 0x8000000;; }

template<> inline dword ValueTypeNo(const int*)     { return INT_V; }
template<> inline dword ValueTypeNo(const int64*)   { return INT64_V; }
template<> inline dword ValueTypeNo(const double*)  { return DOUBLE_V; }
template<> inline dword ValueTypeNo(const bool*)    { return BOOL_V; }
template<> inline dword ValueTypeNo(const String*)  { return STRING_V; }
template<> inline dword ValueTypeNo(const WString*) { return WSTRING_V; }
template<> inline dword ValueTypeNo(const Date*)    { return DATE_V; }
template<> inline dword ValueTypeNo(const Time*)    { return TIME_V; }

template <class T, dword type, class B = EmptyClass>
class ValueType : public B {
public:
	friend dword ValueTypeNo(const T*)              { return type; }
	
	bool     IsNullInstance() const                 { return false; }
	void     Serialize(Stream& s)                   { NEVER(); }
	void     Xmlize(XmlIO& xio)                     { NEVER(); }
	void     Jsonize(JsonIO& jio)                   { NEVER(); }
	unsigned GetHashValue() const                   { return 0; }
	bool     operator==(const T&) const             { NEVER(); return false; }
	String   ToString() const                       { return typeid(T).name(); }
	
	operator ValueTypeRef();
};

template <class T, dword type, class B = EmptyClass> // Backward compatiblity
class AssignValueTypeNo : public ValueType<T, type, B> {};

template <class T>
dword GetValueTypeNo() { return ValueTypeNo((T*)NULL); }

class Value : Moveable<Value> {
public:
	class Void {
	protected:
		Atomic  refcount;

	public:
		void               Retain()                    { AtomicInc(refcount); }
		void               Release()                   { if(AtomicDec(refcount) == 0) delete this; }
		int                GetRefCount() const         { return AtomicRead(refcount); }

		virtual dword      GetType() const             { return VOID_V; }
		virtual bool       IsNull() const              { return true; }
		virtual void       Serialize(Stream& s)        {}
		virtual void       Xmlize(XmlIO& xio)          {}
		virtual void       Jsonize(JsonIO& jio)        {}
		virtual unsigned   GetHashValue() const        { return 0; }
		virtual bool       IsEqual(const Void *p)      { return false; }
		virtual bool       IsPolyEqual(const Value& v) { return false; }
		virtual String     AsString() const            { return ""; }

		Void()                                         { refcount = 1; }
		virtual ~Void()                                {}

		friend class Value;
	};

	struct Sval {
		bool       (*IsNull)(const void *p);
		void       (*Serialize)(void *p, Stream& s);
		void       (*Xmlize)(void *p, XmlIO& xio);
		void       (*Jsonize)(void *p, JsonIO& jio);
		unsigned   (*GetHashValue)(const void *p);
		bool       (*IsEqual)(const void *p1, const void *p2);
		bool       (*IsPolyEqual)(const void *p, const Value& v);
		String     (*AsString)(const void *p);
	};
	
protected:
	enum { STRING = 0, REF = 255, VOIDV = 3 };

	static VectorMap<dword, Void* (*)()>& Typemap();
	static Sval *svo[256];
	static Index<String>& NameNdx();
	static Index<dword>&  TypeNdx();
	
	static void   AddName(dword type, const char *name);
	static int    GetType(const char *name);
	static String GetName(dword type);
	static void   RegisterStd();

	friend void ValueRegisterHelper();

	String   data;
	Void    *&ptr()                  { ASSERT(IsRef()); return *(Void **)&data; }
	Void     *ptr() const            { ASSERT(IsRef()); return *(Void **)&data; }
	
	bool     IsString() const        { return !data.IsSpecial(); }
	bool     Is(byte v) const        { return data.IsSpecial(v); }
	bool     IsRef() const           { return Is(REF); }
	void     InitRef(Void *p)        { data.SetSpecial(REF); ptr() = p; }
	void     RefRelease();
	void     RefRetain();
	void     Free()                  { if(IsRef()) RefRelease(); }
	void     SetLarge(const Value& v);

	template <class T>
	void     InitSmall(const T& init);
	template <class T>
	T&       GetSmall() const;
	
	int      GetOtherInt() const;
	int64    GetOtherInt64() const;
	double   GetOtherDouble() const;
	bool     GetOtherBool() const;
	Date     GetOtherDate() const;
	Time     GetOtherTime() const;
	String   GetOtherString() const;
	unsigned GetOtherHashValue() const;

	bool     IsPolyEqual(const Value& v) const;
	
	enum VSMALL { SMALL };

	template <class T>
	Value(const T& value, VSMALL);
	
	template <class T> friend Value SvoToValue(const T& x);

	String  GetName() const;

public:
	static  void Register(dword w, Void* (*c)(), const char *name = NULL) init_; // Direct use deprecated

	template <class T>
	static  void Register(const char *name = NULL);
	template <class T>
	static  void SvoRegister(const char *name = NULL);
	
	dword    GetType() const;
	bool     IsError() const         { return GetType() == ERROR_V; }
	bool     IsVoid() const          { return Is(VOIDV) || IsError(); }
	bool     IsNull() const;

	template <class T>
	bool     Is() const;
	template <class T>
	const T& To() const;
	template <class T>
	const T& Get() const;

	operator String() const          { return IsString() ? data : GetOtherString(); }
	operator WString() const;
	operator Date() const            { return Is(DATE_V) ? GetSmall<Date>() : GetOtherDate(); }
	operator Time() const            { return Is(TIME_V) ? GetSmall<Time>() : GetOtherTime(); }
	operator double() const          { return Is(DOUBLE_V) ? GetSmall<double>() : GetOtherDouble(); }
	operator int() const             { return Is(INT_V) ? GetSmall<int>() : GetOtherInt(); }
	operator int64() const           { return Is(INT64_V) ? GetSmall<int64>() : GetOtherInt64(); }
	operator bool() const            { return Is(BOOL_V) ? GetSmall<bool>() : GetOtherBool(); }

	Value(const String& s) : data(s) {}
	Value(const WString& s);
	Value(const char *s) : data(s)   {}
	Value(int i)                     : data(i, INT_V, String::SPECIAL) {}
	Value(int64 i)                   : data(i, INT64_V, String::SPECIAL) {}
	Value(double d)                  : data(d, DOUBLE_V, String::SPECIAL) {}
	Value(bool b)                    : data(b, BOOL_V, String::SPECIAL) {}
	Value(Date d)                    : data(d, DATE_V, String::SPECIAL) {}
	Value(Time t)                    : data(t, TIME_V, String::SPECIAL) {}
	Value(const Nuller&)             : data((int)Null, INT_V, String::SPECIAL) {}

	bool operator==(const Value& v) const;
	bool operator!=(const Value& v) const { return !operator==(v); }

	String ToString() const;

	void  Serialize(Stream& s);
	void  Xmlize(XmlIO& xio);
	void  Jsonize(JsonIO& jio);

	unsigned GetHashValue() const;

	Value& operator=(const Value& v);
	Value(const Value& v);
	
	int   GetCount() const;
	const Value& operator[](int i) const;
	const Value& operator[](const String& key) const;
	const Value& operator[](const char *key) const;
	const Value& operator[](const Id& key) const;

	Value()                               : data((int)Null, VOIDV, String::SPECIAL) {}
	~Value()                              { if(IsRef()) RefRelease(); }

	Value(Void *p)                        { InitRef(p); }
	const Void *GetVoidPtr() const        { ASSERT(IsRef()); return ptr(); }

	friend void Swap(Value& a, Value& b)  { Swap(a.data, b.data); }
};

template <class T> bool  FitsSvoValue()                    { return sizeof(T) <= 8; }
template <class T> Value SvoToValue(const T& x)            { return Value(x, Value::SMALL); }

template <class T> Value RichToValue(const T& data);

template <class T> Value RawToValue(const T& data);
template <class T> Value RawPickToValue(pick_ T& data);
template <class T> Value RawDeepToValue(const T& data);
template <class T> T&    CreateRawValue(Value& v);

Value        ErrorValue(const char *s);
Value        ErrorValue(const String& s);
const Value& ErrorValue();

template <class T>
inline bool IsPolyEqual(const T& x, const Value& v) {
	return false;
}

template <class T>
inline unsigned ValueGetHashValue(const T& x) {
	return UPP::GetHashValue(x);
}

#define VALUE_COMPARE_V(T, VT) \
inline bool operator==(const Value& v, T x)   { return (VT)v == x; } \
inline bool operator==(T x, const Value& v)   { return (VT)v == x; } \
inline bool operator!=(const Value& v, T x)   { return (VT)v != x; } \
inline bool operator!=(T x, const Value& v)   { return (VT)v != x; } \

#define VALUE_COMPARE(T) VALUE_COMPARE_V(T, T)

VALUE_COMPARE(int)
VALUE_COMPARE(int64)
VALUE_COMPARE(double)
VALUE_COMPARE(bool)
VALUE_COMPARE(Date)
VALUE_COMPARE(Time)
VALUE_COMPARE(String)
VALUE_COMPARE(WString)
VALUE_COMPARE_V(const char *, String)
VALUE_COMPARE_V(const wchar *, WString)

inline bool IsVoidValueTypeNo(int q)     { return (dword)q == VOID_V; }
inline bool IsErrorValueTypeNo(int q)    { return (dword)q == ERROR_V; }
inline bool IsStringValueTypeNo(int q)   { return (dword)q == STRING_V || (dword)q == WSTRING_V; }

inline bool IsIntegerValueTypeNo(int q)  { return (dword)q == INT_V || (dword)q == INT64_V || (dword)q == BOOL_V; }
inline bool IsFloatValueTypeNo(int q)    { return (dword)q == DOUBLE_V; }

inline bool IsNumberValueTypeNo(int q)   { return IsIntegerValueTypeNo(q) || IsFloatValueTypeNo(q); }
inline bool IsDateTimeValueTypeNo(int q) { return (dword)q == DATE_V || (dword)q == TIME_V; }

inline bool IsVoid(const Value& v)       { return v.IsVoid(); }
inline bool IsError(const Value& v)      { return v.IsError(); }
inline bool IsString(const Value& v)     { return v.Is<String>() || v.Is<WString>(); }
inline bool IsNumber(const Value& v)     { return v.Is<double>() || v.Is<int>() || v.Is<int64>() || v.Is<bool>(); }
inline bool IsDateTime(const Value& v)   { return v.Is<Date>() || v.Is<Time>(); }
inline bool IsValueArray(const Value& v) { return v.GetType() == VALUEARRAY_V || v.GetType() == VALUEMAP_V; }
inline bool IsValueMap(const Value& v)   { return IsValueArray(v); }

String       GetErrorText(const Value& v);

inline bool          IsNull(const Value& v)               { return v.IsNull(); }
inline const Value&  Nvl(const Value& a, const Value& b)  { return IsNull(a) ? b : a; }

#endif