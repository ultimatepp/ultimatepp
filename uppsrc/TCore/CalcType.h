NAMESPACE_UPP

class CalcTypeNameConvert
{
public:
	CalcTypeNameConvert(const type_info& tinfo, String (*outfn)()) { Get().Add(tinfo.name(), outfn); }

	static VectorMap<String, String (*)()>& Get();
	static String                           Format(const char *raw_name);
};

#define RegisterCalcTypeName(type, name) \
static String COMBINE(ctn, MK__s)() { return name; } \
static CalcTypeNameConvert COMBINE(tn, MK__s)(typeid(type), &COMBINE(ctn, MK__s));

#define RegisterStdCalcTypeName(type) \
static CalcTypeNameConvert COMBINE(tn, MK__s)(typeid(type), &CalcType<type>::Describe);

template <class T>
struct CalcRawType
{
	static Value    ToValue(const T& t) { return RawToValue(t); }
	static const T& ValueTo(Value v)    { return RawValue<T>::Extract(v); }
	static bool     IsType(Value v)     { return IsTypeRaw<T>(v); }
	static String   Describe()          { return CalcTypeNameConvert::Format(typeid(T).name()); }
};

template <class T>
struct CalcRawNullType : public CalcRawType<T>
{
	static bool     IsType(Value v)     { return v.IsVoid() || CalcRawType<T>::IsType(v); }
	static const T& ValueTo(Value v)    { if(v.IsVoid()) { static T t; return t; } return CalcRawType<T>::ValueTo(v); }
};

template <class T>
struct CalcCastType : public CalcRawType<T>
{
	static Value    ToValue(const T& t)     { return t; }
	static T        ValueTo(Value v)        { return v; }
};

template <class T> struct CalcType : public CalcRawType<T> {};

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
struct CalcType<double> : public CalcCastType<double>
{
	static bool   IsType(Value v) { return IsNumber(v); }
	static String Describe();
	static Value  ToValue(double t);
};

template <>
struct CalcType<String> : public CalcCastType<String>
{
	static bool   IsType(Value v) { return IsNull(v) || IsString(v); }
	static String Describe();
};

template <>
struct CalcType<WString> : public CalcCastType<WString>
{
	static bool   IsType(Value v) { return IsNull(v) || IsString(v); }
	static String Describe()             { return CalcType<String>::Describe(); }
};

template <>
struct CalcType<Date> : public CalcCastType<Date>
{
	static bool   IsType(Value v) { return IsDateTime(v); }
	static String Describe();
};

template <>
struct CalcType<Time> : public CalcCastType<Time>
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
struct CalcType<ValueArray>
{
	static Value       ToValue(const ValueArray& t)     { return t; }
	static ValueArray  ValueTo(Value v)                 { return IsValueArray(v) ? ValueArray(v) : ValueArray(); }
	static bool        IsType(Value v)                  { return IsNull(v) || IsValueArray(v); }
	static String      Describe();
};

template <>
struct CalcType<Nuller>
{
	static Value       ToValue(const Nuller&)            { return Value(); }
	static Nuller      ValueTo(Value v)                  { return Null; }
	static bool        IsType(Value v)                   { return IsNull(v); }
	static String      Describe();
};

END_UPP_NAMESPACE
