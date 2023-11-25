#include "Core.h"

namespace Upp {

const Nuller Null;

#define LTIMING(x) // RTIMING(x)

ValueTypeError::ValueTypeError(const String& text, const Value& src, int target)
:	Exc(text), src(src), target(target) {}

hash_t Value::GetOtherHashValue() const {
	if(IsNull())
		return 0;
	byte st = data.GetSt();
	if(st == REF)
		return ptr()->GetHashValue();
	return svo[st]->GetHashValue(&data);
}

void Value::RefRelease()
{
	ASSERT(IsRef()); // Check that svo type is not registered as Ref
	ptr()->Release();
}

void Value::RefRetain()
{
	ptr()->Retain();
}

Value& Value::operator=(const Value& v) {
	if(this == &v) return *this;
	Value h = v; // Make copy a 'v' can be reference to ValueMap/Array contained element
	FreeRef();   // e.g. json = json["foo"]
	data = h.data;
	if(IsRef())
		ptr()->Retain();
	return *this;
}

void Value::SetLarge(const Value& v)
{
	if(v.IsRef()) {
		data.SetSmall(v.data);
		RefRetain();
	}
	else
		data.LSet(v.data);
}

dword Value::GetType() const
{
	if(IsString())
		return STRING_V;
	byte st = data.GetSt();
	return st == REF ? GetRefType() : st == VOIDV ? VOID_V : st;
}

bool Value::IsNull() const
{
	if(IsString())
		return data.GetCount() == 0;
	int st = data.GetSt();
	if(st == VOIDV)
		return true;
	if(st == REF)
		return ptr()->IsNull();
	return svo[st]->IsNull(&data);
}

bool Value::IsPolyEqual(const Value& v) const
{
	int st = data.GetSpecial();
	if(st == REF)
		return ptr()->IsPolyEqual(v);
	if(svo[st] && svo[st]->IsPolyEqual(&data, v))
		return true;
	return IsNull() && v.IsNull();
}

bool Value::operator==(const Value& v) const {
	if(IsString() && v.IsString())
		return data == v.data;
	if(GetType() != v.GetType()) {
		if(IsPolyEqual(v) || v.IsPolyEqual(*this))
			return true;
	}
	else {
		int st = data.GetSpecial();
		if(st == REF) {
			if(ptr()->IsEqual(v.ptr()))
				return true;
		}
		else
		if(st != VOIDV) {
			if(svo[st]->IsEqual(&data, &v.data))
				return true;
		}
	}
	return IsNull() && v.IsNull();
}

int Value::PolyCompare(const Value& v) const
{
	int st = data.GetSpecial();
	if(st == REF)
		return ptr()->PolyCompare(v);
	if(st != VOIDV)
		return svo[st]->PolyCompare(&data, v);
	return 0;
}

int Value::Compare2(const Value& v) const
{
	if(IsString() && v.IsString())
		return SgnCompare(data, v.data);
	dword stw = data.GetStW();
	if(stw == v.data.GetStW()) {
		if(stw == String::StW(INT64_V))
			return SgnCompare(GetSmallRaw<int64>(), v.GetSmallRaw<int64>());
		if(stw == String::StW(DATE_V))
			return SgnCompare(GetSmallRaw<Date>(), v.GetSmallRaw<Date>());
		if(stw == String::StW(TIME_V))
			return SgnCompare(GetSmallRaw<Time>(), v.GetSmallRaw<Time>());
	}
	bool a = IsNull();
	bool b = v.IsNull();
	if(a || b)
		return SgnCompare(b, a);
	int st = data.GetSpecial();
	if(GetType() == v.GetType()) {
		if(st == REF)
			return ptr()->Compare(v.ptr());
		if(st != VOIDV)
			return svo[st]->Compare(&data, &v.data);
	}
	if(st != VOIDV) {
		int q = PolyCompare(v);
		if(q) return q;
		return -v.PolyCompare(*this);
	}
	return 0;
}

bool Value::IsSame(const Value& b) const
{
	const Value& a = *this;
	if(a.Is<ValueMap>() && b.Is<ValueMap>())
		return ValueMap(a).IsSame(ValueMap(b));
	else
	if(a.Is<ValueArray>() && b.Is<ValueArray>()) {
		if(a.GetCount() != b.GetCount())
			return false;
		for(int i = 0; i < a.GetCount(); i++)
			if(!a[i].IsSame(b[i]))
				return false;
		return true;
	}
	else
		return a == b;
}

Value::Value(const WString& s) { InitRef(new RichValueRep<WString>(s), WSTRING_V); Magic(); }

Value::operator WString() const
{
	if(IsNull()) return Null;
	return GetType() == WSTRING_V ? To<WString>() : ((String)(*this)).ToWString();
}

Date Value::GetOtherDate() const
{
	if(IsNull()) return Null;
	return GetSmall<Time>();
}

Time Value::GetOtherTime() const
{
	if(IsNull()) return Null;
	return ToTime(GetSmall<Date>());
}

String Value::GetOtherString() const
{
	if(IsNull()) return Null;
	if(Is<String>())
		return To<String>();
	return To<WString>().ToString();
}

int Value::GetOtherInt() const
{
	if(IsNull()) return Null;
	return data.IsSpecial(BOOL_V) ? (int)GetSmall<bool>() :
	       data.IsSpecial(INT64_V) ? (int)GetSmall<int64>() :
	       data.IsSpecial(FLOAT_V) ? (int)GetSmall<float>() :
	       (int)GetSmall<double>();
}

int64 Value::GetOtherInt64() const
{
	if(IsNull()) return Null;
	return data.IsSpecial(BOOL_V) ? (int64)GetSmall<bool>() :
	       data.IsSpecial(INT_V) ? (int64)GetSmall<int>() :
	       data.IsSpecial(FLOAT_V) ? (int64)GetSmall<float>() :
	       (int64)GetSmall<double>();
}

double Value::GetOtherDouble() const
{
	if(IsNull()) return Null;
	return data.IsSpecial(BOOL_V) ? (double)GetSmall<bool>() :
	       data.IsSpecial(INT_V) ? (double)GetSmall<int>() :
	       data.IsSpecial(FLOAT_V) ? (double)GetSmall<float>() :
	       (double)GetSmall<int64>();
}

float Value::GetOtherFloat() const
{
	if(IsNull()) return Null;
	return data.IsSpecial(BOOL_V) ? (float)GetSmall<bool>() :
	       data.IsSpecial(INT_V) ? (float)GetSmall<int>() :
	       data.IsSpecial(DOUBLE_V) ? (float)GetSmall<double>() :
	       (float)GetSmall<int64>();
}

bool Value::GetOtherBool() const
{
	if(IsNull()) return Null;
	return data.IsSpecial(DOUBLE_V) ? (bool)GetSmall<double>() :
	       data.IsSpecial(INT_V) ? (bool)GetSmall<int>() :
	       data.IsSpecial(FLOAT_V) ? (float)GetSmall<float>() :
	       (bool)GetSmall<int64>();
}

VectorMap<dword, Value::Void *(*)()>& Value::Typemap()
{
	static VectorMap<dword, Value::Void *(*)()> x;
	return x;
}

Index<String>& Value::NameNdx()
{
	static Index<String> x;
	return x;
}

Index<dword>& Value::TypeNdx()
{
	static Index<dword> x;
	return x;
}

void Value::AddName(dword type, const char *name)
{
	NameNdx().Add(name);
	TypeNdx().Add(type);
}

int Value::GetType(const char *name)
{
	int q = NameNdx().Find(name);
	if(q < 0)
		return Null;
	return TypeNdx()[q];
}

String Value::GetName(dword type)
{
	int q = TypeNdx().Find(type);
	if(q < 0)
		return Null;
	return NameNdx()[q];
}

SVO_FN(s_String, String);
SVO_FN(s_int, int);
SVO_FN(s_double, double);
SVO_FN(s_float, float);
SVO_FN(s_int64, int64);
SVO_FN(s_bool, bool);
SVO_FN(s_date, Date);
SVO_FN(s_time, Time);

struct SvoVoidFn {
	static bool       IsNull(const void *p)                      { return true; }
	static void       Serialize(void *p, Stream& s)              {}
	static void       Xmlize(void *p, XmlIO& xio)               {}
	static void       Jsonize(void *p, JsonIO& jio)             {}
	static hash_t     GetHashValue(const void *p)                { return 0; }
	static bool       IsEqual(const void *p1, const void *p2)    { return true; }
	static bool       IsPolyEqual(const void *p, const Value& v) { return false; }
	static String     AsString(const void *p)                    { return String(); }
};

static Value::Sval s_void = {
	SvoVoidFn::IsNull, SvoVoidFn::Serialize,SvoVoidFn::Xmlize, SvoVoidFn::Jsonize,
	SvoVoidFn::GetHashValue, SvoVoidFn::IsEqual,
	SvoVoidFn::IsPolyEqual, SvoVoidFn::AsString
};

Value::Sval *Value::svo[256] = {
	&s_String, // STRING_V
	&s_int, // INT_V

	&s_double, //DOUBLE_V  = 2;
	&s_void, //VOIDV_V  = 3;
	&s_date, //DATE_V    = 4;
	&s_time, //TIME_V    = 5;

	NULL, //ERROR_V   = 6;

	NULL, //VALUE_V   = 7;

	NULL, //WSTRING_V = 8;

	NULL, //VALUEARRAY_V = 9;

	&s_int64, //INT64_V  = 10;
	&s_bool, //BOOL_V   = 11;

	NULL, //VALUEMAP_V   = 12;
	&s_float, //FLOAT_V = 13;
};

Value::Void *ValueArrayDataCreate()
{
	return new ValueArray::Data;
}

Value::Void *ValueMapDataCreate()
{
	return new ValueMap::Data;
}

void Value::RegisterStd()
{
	ONCELOCK {
		Value::Register<WString>("WString");
		Value::Register<Complex>("Complex");
		Value::Register(VALUEARRAY_V, ValueArrayDataCreate, "ValueArray");
		Value::Register(VALUEMAP_V, ValueMapDataCreate, "ValueMap");
		Value::AddName(STRING_V, "String");
		Value::AddName(INT_V, "int");
		Value::AddName(DOUBLE_V, "double");
		Value::AddName(FLOAT_V, "float");
		Value::AddName(VOID_V, "void");
		Value::AddName(DATE_V, "Date");
		Value::AddName(TIME_V, "Time");
		Value::AddName(INT64_V, "int64");
		Value::AddName(BOOL_V, "bool");
		Value::AddName(ERROR_V, "error");
	};
}

void ValueRegisterHelper()
{
	Value::RegisterStd();
}

INITBLOCK {
	ValueRegisterHelper();
}

void Value::Serialize(Stream& s) {
	RegisterStd();
	dword type;
	if(s.IsLoading()) {
		s / type;
		if(type >= 0x8000000)
			s.LoadError();
		Free();
		int st = type == VOID_V ? VOIDV : type == STRING_V ? STRING : type;
		if(st == STRING)
			s % data;
		else
		if(st >= 0 && st < 255 && svo[st]) {
			data.SetSpecial((byte)type);
			svo[st]->Serialize(&data, s);
		}
		else {
			typedef Void* (*vp)();
			vp *cr = Typemap().FindPtr(type);
			if(cr) {
				Void *p = (**cr)();
				p->Serialize(s);
				InitRef(p, type);
			}
			else {
				Free();
				data.SetSpecial(3);
				if(type != VOID_V && type != ERROR_V)
					s.LoadError();
			}
		}
	}
	else {
		type = GetType();
		ASSERT(type < 0x8000000); // only Values with assigned real type ID can be serialized
		s / type;
		int st = data.GetSpecial();
		ASSERT_(!type || type == ERROR_V || type == UNKNOWN_V || st == STRING ||
		        (IsRef() ? Typemap().Find(type) >= 0 : st < 255 && svo[st]),
		        GetName() + " is not registred for serialization");
		if(st == VOIDV)
			return;
		if(st == STRING)
			s % data;
		else
		if(IsRef())
			ptr()->Serialize(s);
		else
			svo[st]->Serialize(&data, s);
	}
}

static String s_binary("serialized_binary");

void Value::Xmlize(XmlIO& xio)
{
	RegisterStd();
	if(xio.IsStoring()) {
		if(IsError()) {
			Value v = Null;
			v.Xmlize(xio);
			return;
		}
		dword type = GetType();
		String name = GetName(type);
		if(name.GetCount() == 0) {
			xio.SetAttr("type", s_binary);
			String s = HexString(StoreAsString(*this));
			Upp::Xmlize(xio, s);
		}
		else {
			xio.SetAttr("type", name);
			int st = data.GetSpecial();
			ASSERT_(!type || type == ERROR_V || type == UNKNOWN_V || st == STRING ||
			        (IsRef() ? Typemap().Find(type) >= 0 : st < 255 && svo[st]),
			        GetName() + " is not registred for xmlize");
			if(st == VOIDV)
				return;
			if(st == STRING)
				Upp::Xmlize(xio, data);
			else
			if(IsRef())
				ptr()->Xmlize(xio);
			else
				svo[st]->Xmlize(&data, xio);
		}
	}
	else {
		String name = xio.GetAttr("type");
		if(Upp::IsNull(name))
			*this = Value();
		else
		if(name == s_binary) {
			String s;
			Upp::Xmlize(xio, s);
			try {
				LoadFromString(*this, ScanHexString(s));
			}
			catch(LoadingError) {
				throw XmlError("xmlize serialized_binary Error");
			}
		}
		else {
			int type = GetType(name);
			if(Upp::IsNull(type))
				throw XmlError("invalid Value type");
			Free();
			int st = (dword)type == VOID_V ? VOIDV : (dword)type == STRING_V ? STRING : type;
			if(st == STRING)
				Upp::Xmlize(xio, data);
			else
			if(st < 255 && svo[st]) {
				data.SetSpecial((byte)type);
				svo[st]->Xmlize(&data, xio);
			}
			else {
				typedef Void* (*vp)();
				vp *cr = Typemap().FindPtr(type);
				if(cr) {
					Void *p = (**cr)();
					p->Xmlize(xio);
					InitRef(p, type);
				}
				else
					throw XmlError("invalid Value type");
			}
		}
	}
}

void Value::Jsonize(JsonIO& jio)
{
	RegisterStd();
	if(jio.IsStoring()) {
		if(IsNull())
			jio.Set(Null);
		else {
			dword type = GetType();
			String name = GetName(type);
			if(name.GetCount() == 0) {
				String s = HexString(StoreAsString(*this));
				jio("type", s_binary)
				   ("value", s);
			}
			else {
				int st = data.GetSpecial();
				ASSERT_(!type || type == ERROR_V || type == UNKNOWN_V || st == STRING ||
				        (IsRef() ? Typemap().Find(type) >= 0 : st < 255 && svo[st]),
				        GetName() + " is not registred for jsonize");
				if(st == VOIDV)
					return;
				JsonIO hio;
				if(st == STRING) {
					String h = data;
					Upp::Jsonize(hio, h);
				}
				else {
					if(IsRef())
						ptr()->Jsonize(hio);
					else
						svo[st]->Jsonize(&data, hio);
				}
				ValueMap m;
				m.Add("type", name);
				m.Add("value", hio.GetResult());
				jio.Set(m);
			}
		}
	}
	else {
		Value g = jio.Get();
		if(g.IsNull())
			*this = Null;
		else {
			String name = g["type"];
			Value  val = g["value"];
			if(name == s_binary) {
				if(!Upp::IsString(val))
					throw JsonizeError("serialized_binary Error");
				String s = val;
				try {
					LoadFromString(*this, ScanHexString(s));
				}
				catch(LoadingError) {
					throw JsonizeError("serialized_binary Error");
				}
			}
			else {
				int type = GetType(name);
				if(Upp::IsNull(type))
					throw JsonizeError("invalid Value type");
				Free();
				int st = (dword)type == VOID_V ? VOIDV : (dword)type == STRING_V ? STRING : type;
				if(st == STRING) {
					if(!Upp::IsString(val))
						throw JsonizeError("serialized_binary Error");
					data = val;
				}
				else {
					JsonIO hio(val);
					if(st < 255 && svo[st]) {
						data.SetSpecial((byte)type);
						svo[st]->Jsonize(&data, hio);
					}
					else {
						typedef Void* (*vp)();
						vp *cr = Typemap().FindPtr(type);
						if(cr) {
							Void *p = (**cr)();
							p->Jsonize(hio);
							InitRef(p, type);
						}
						else
							throw JsonizeError("invalid Value type");
					}
				}
			}
		}
	}
}

void Value::Register(dword w, Void* (*c)(), const char *name) {
#ifdef flagCHECKINIT
	RLOG("Register valuetype " << w);
#endif
	AssertST(); // all needs to be registered at file level scope
	ASSERT(w != UNKNOWN_V);
	ASSERT(w < 0x8000000);
	CHECK(Typemap().GetAdd(w, c) == c);
	AddName(w, name);
}

String  Value::ToString() const {
	if(IsNull())
		return Null;
	if(IsString())
		return data;
	if(IsRef())
		return ptr()->AsString();
	int st = data.GetSpecial();
	return svo[st]->AsString(&data);
}

int Value::GetCount() const
{
	if(IsRef()) {
		dword t = GetRefType();
		if(t == VALUEARRAY_V)
			return ((ValueArray::Data *)ptr())->data.GetCount();
		if(t == VALUEMAP_V)
			return ((ValueMap::Data *)ptr())->value.GetCount();
	}
	return 0;
}

const Value& Value::operator[](int i) const
{
	if(IsRef()) {
		dword t = GetRefType();
		if(t == VALUEARRAY_V)
			return ((ValueArray::Data *)ptr())->data[i];
		if(t == VALUEMAP_V)
			return ((ValueMap::Data *)ptr())->value[i];
	}
	return ErrorValue();
}

const Vector<Value>& Value::GetVA() const
{
	if(IsRef()) {
		if(Is<ValueArray>())
			return ((ValueArray::Data *)ptr())->data;
		if(Is<ValueMap>())
			return ((ValueMap::Data *)ptr())->value.data->data;
	}
	return ValueArray::VoidData;
}

force_inline
Vector<Value>& Value::UnShareArray()
{
	ValueArray::Data *data = (ValueArray::Data *)ptr();
	if(data->GetRefCount() != 1) {
		ValueArray::Data *d = new ValueArray::Data;
		d->data = clone(data->data);
		data->Release();
		ptr() = d;
		data = d;
	}
	return data->data;
}

Value& Value::At(int i)
{
	if(IsNull())
		*this = ValueArray();
	ASSERT(i >= 0 && IsRef());
	dword t = GetRefType();
	if(t == VALUEMAP_V) {
		ValueArray& va = ValueMap::UnShare((ValueMap::Data*&)ptr()).value;
		ASSERT(i < va.GetCount());
		return va.At(i);
	}
	ASSERT(t == VALUEARRAY_V);
	return UnShareArray().At(i);
}

void Value::Add(const Value& s)
{
	if(IsNull()) {
		if(IsRef()) RefRelease();
		ValueArray::Data *d = new ValueArray::Data;
		d->data.Add(s);
		InitRef(d, VALUEARRAY_V);
		Magic();
		return;
	}
	ASSERT(IsRef() && GetRefType() == VALUEARRAY_V);
	UnShareArray().Add(s);
}

const Value& Value::operator[](const String& key) const
{
	if(IsRef() && GetRefType() == VALUEMAP_V)
		return ((ValueMap::Data *)ptr())->Get(key);
	return ErrorValue();
}

Value& Value::GetAdd(const Value& key)
{
	if(IsNull()) {
		if(IsRef()) RefRelease();
		ValueMap::Data *d = new ValueMap::Data;
		Value& h = d->GetAdd(key);
		InitRef(d, VALUEMAP_V);
		Magic();
		return h;
	}
	if(GetType() == VALUEARRAY_V) {
		ValueMap m = *this;
		*this = m;
	}
	ASSERT(GetType() == VALUEMAP_V);
	return ValueMap::UnShare((ValueMap::Data*&)ptr()).GetAdd(key);
}

Value& Value::operator()(const String& key)
{
	return GetAdd(key);
}

Value& Value::operator()(const char *key)
{
	return GetAdd(key);
}

Value& Value::operator()(const Id& key)
{
	return GetAdd(~key);
}

String Value::GetName() const
{
	if(IsRef()) {
		Void *p = ptr(); // supress CLANG warning
		return typeid(*p).name();
	}
	if(IsString())
		return "String";
	static Tuple<byte, const char *> tp[] = {
		{ (byte)INT_V, "int" },
		{ (byte)DOUBLE_V, "double" },
		{ (byte)FLOAT_V, "float" },
		{ (byte)VOIDV, "void" },
		{ (byte)DATE_V, "Date" },
		{ (byte)TIME_V, "Time" },
		{ (byte)INT64_V, "int64" },
		{ (byte)BOOL_V, "bool" },
	};
	Tuple<byte, const char *> *x = FindTuple(tp, __countof(tp), data.GetSpecial());
	return x ? String(x->b) : AsString(GetType());
}

class ValueErrorCls : public RichValueRep<String> {
public:
	virtual dword      GetType() const             { return ERROR_V; }
	virtual bool       IsNull() const              { return true; }
	virtual void       Serialize(Stream& s)        {}
	virtual String     AsString() const            { return "<error: \'" + v + "\'>"; }

	ValueErrorCls(const String& s) : RichValueRep<String>(s)  {}
};

Value ErrorValue(const String& s) {
	return Value(new ValueErrorCls(s), ERROR_V);
}

Value ErrorValue(const char *s) {
	return ErrorValue(String(s));
}

const Value& ErrorValue() {
	static Value v = ErrorValue(String());
	return v;
}

String GetErrorText(const Value& v) {
	ASSERT(IsError(v));
	return ((RichValueRep<String> *)v.GetVoidPtr())->Get();
}

}