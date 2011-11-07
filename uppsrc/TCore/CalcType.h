NAMESPACE_UPP

class CalcTypeNameConvert
{
public:
	CalcTypeNameConvert(const char *name, String (*outfn)()) { Get().Add(name, outfn); }

	static VectorMap<String, String (*)()>& Get();
	static String                           Format(const char *raw_name);
};

#define RegisterCalcTypeName(type, ctname) \
static String COMBINE(ctn, MK__s)() { return ctname; } \
static CalcTypeNameConvert COMBINE(tn, MK__s)(typeid(type).name(), &COMBINE(ctn, MK__s));

#define RegisterStdCalcTypeName(type) \
static CalcTypeNameConvert COMBINE(tn, MK__s)(typeid(type).name(), &CalcType<type>::Describe);

template <class T>
struct CalcRawType
{
	static Value    ToValue(const T& t) { return RawToValue(t); }
	static const T& ValueTo(Value v)    { return RawValue<T>::Extract(v); }
	static bool     IsType(Value v)     { return IsTypeRaw<T>(v); }
	static String   Describe()          { return CalcTypeNameConvert::Format(typeid(T).name()); }
};

template <class T>
struct CalcRawNullType : CalcRawType<T>
{
	static bool     IsType(Value v)     { return v.IsVoid() || CalcRawType<T>::IsType(v); }
	static const T& ValueTo(Value v)    { if(v.IsVoid()) { static T t; return t; } return CalcRawType<T>::ValueTo(v); }
};

template <class T>
struct CalcCastType : CalcRawType<T>
{
	static Value    ToValue(const T& t)     { return t; }
	static T        ValueTo(Value v)        { return v; }
};

template <class T> struct CalcType : CalcRawType<T> {};

template <class T>
inline Value CalcTypeToValue(const T& value) { return CalcType<T>::ToValue(value); }

String CalcTypeDescribeInt();

template <class T>
struct CalcIntType
{
	static Value    ToValue(T t)            { return int(t); }
	static T        ValueTo(Value v)        { return (T)(int)v; }
	static bool     IsType(Value v)         { return IsNumber(v); }
	static String   Describe()              { return CalcTypeDescribeInt(); }
};

template <> struct CalcType<unsigned char>  : public CalcIntType<unsigned char>  {};
template <> struct CalcType<signed   char>  : public CalcIntType<signed   char>  {};
template <> struct CalcType<unsigned short> : public CalcIntType<unsigned short> {};
template <> struct CalcType<signed   short> : public CalcIntType<signed   short> {};
template <> struct CalcType<unsigned int>   : public CalcIntType<unsigned int>   {};
template <> struct CalcType<signed   int>   : public CalcIntType<signed   int>   {};
template <> struct CalcType<unsigned long>  : public CalcIntType<unsigned long>  {};
template <> struct CalcType<signed   long>  : public CalcIntType<signed   long>  {};

const char *CalcNanError();
const char *CalcInfError();

template <>
struct CalcType<double> : CalcCastType<double>
{
	static bool   IsType(Value v) { return IsNumber(v); }
	static String Describe();
	static Value  ToValue(double t);
};

template <>
struct CalcType<int64> : CalcCastType<int64>
{
	static bool   IsType(Value v) { return IsNumber(v); }
	static String Describe();
};

template <>
struct CalcType<String> : CalcCastType<String>
{
	static bool   IsType(Value v) { return IsNull(v) || IsString(v); }
	static String Describe();
};

template <>
struct CalcType<const String&> : CalcType<String> {};

template <>
struct CalcType<WString> : CalcCastType<WString>
{
	static bool   IsType(Value v) { return IsNull(v) || IsString(v); }
	static String Describe()             { return CalcType<String>::Describe(); }
};

template <>
struct CalcType<const WString&> : CalcType<WString> {};

template <>
struct CalcType<Date> : CalcCastType<Date>
{
	static bool   IsType(Value v) { return IsDateTime(v); }
	static String Describe();
};

template <>
struct CalcType<Time> : CalcCastType<Time>
{
	static bool   IsType(Value v) { return IsDateTime(v); }
	static String Describe();
};

template <>
struct CalcType<bool>
{
	static Value  ToValue(bool b)         { return b ? 1 : 0; }
	static bool   ValueTo(Value v);
	static bool   IsType(Value v)         { return IsNull(v) || IsNumber(v) || IsString(v); }
	static String Describe();
};

template <>
struct CalcType<Value>
{
	static Value    ToValue(Value v) { return v; }
	static Value    ValueTo(Value v) { return v; }
	static bool     IsType(Value v)  { return true; }
	static String   Describe();
};

template <>
struct CalcType<const Value&> : CalcType<Value> {};

template <>
struct CalcType<ValueArray>
{
	static Value       ToValue(const ValueArray& t)     { return t; }
	static ValueArray  ValueTo(Value v)                 { return IsValueArray(v) ? ValueArray(v) : ValueArray(); }
	static bool        IsType(Value v)                  { return IsNull(v) || IsValueArray(v); }
	static String      Describe();
};

template <>
struct CalcType<const ValueArray&> : CalcType<ValueArray> {};

template <>
struct CalcType<Nuller>
{
	static Value       ToValue(const Nuller&)            { return Value(); }
	static Nuller      ValueTo(Value v)                  { return Null; }
	static bool        IsType(Value v)                   { return IsNull(v); }
	static String      Describe();
};

template <class T>
struct CalcPtrType
{
	static Value    ToValue(T *t)           { return t ? PtrToValue(t) : Value(); }
	static T       *ValueTo(const Value& v) { return IsNull(v) ? NULL : PtrValue<T>::Extract(v); }
	static bool     IsType(const Value& v)  { return IsNull(v) || IsTypeRaw<T *>(v); }
	static String   Describe()              { return CalcTypeNameConvert::Format(typeid(T *).name()); }
};

#define CALC_PTR_TYPE(t) \
	template <> struct CalcType<t *> : public CalcPtrType<t> {}; \
	template <> struct CalcType<const t *> : public CalcPtrType<const t> {};

END_UPP_NAMESPACE
