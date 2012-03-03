#include "Core.h"

#ifdef SVO_VALUE

NAMESPACE_UPP

#ifndef flagSO
const Nuller Null;
#endif

#define LTIMING(x) // RTIMING(x)

unsigned Value::GetOtherHashValue() const {
	if(IsNull())
		return 0;
	byte st = data.GetSt();
	if(st == REF)
		return ptr()->GetHashValue();
	return svo[st]->GetHashValue(&data);
}

void Value::RefRelease()
{
	ASSERT(ptr()->GetType() >= 255 || !svo[ptr()->GetType()]); // Check that svo type is not registered as Ref
	ptr()->Release();
}

void Value::RefRetain()
{
	ptr()->Retain();
}

Value& Value::operator=(const Value& v) {
	if(this == &v) return *this;
	Free();
	data = v.data;
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
	return st == REF ? ptr()->GetType() : st == VOIDV ? VOID_V : st;
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
	if(GetType() != v.GetType())
		return IsPolyEqual(v) || v.IsPolyEqual(*this);
	int st = data.GetSpecial();
	if(st == REF)
		return ptr()->IsEqual(v.ptr());
	if(st == VOIDV)
		return v.IsVoid();
	return svo[st]->IsEqual(&data, &v.data);
}

Value::Value(const WString& s) { InitRef(new RichValueRep<WString>(s)); }

Value::operator WString() const
{
	if(IsNull()) return Null;
	return GetType() == WSTRING_V ? RichValue<WString>::Extract(*this)
		                          : ((String)(*this)).ToWString();//!!!
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
	return RichValue<WString>::Extract(*this).ToString();
}

int Value::GetOtherInt() const
{
	if(IsNull()) return Null;
	return data.IsSpecial(BOOL_V) ? (int)GetSmall<bool>() :
	       data.IsSpecial(INT64_V) ? (int)GetSmall<int64>() :
	       (int)GetSmall<double>();
}

int64 Value::GetOtherInt64() const
{
	if(IsNull()) return Null;
	return data.IsSpecial(BOOL_V) ? (int64)GetSmall<bool>() :
	       data.IsSpecial(INT_V) ? (int64)GetSmall<int>() :
	       (int64)GetSmall<double>();
}

double Value::GetOtherDouble() const
{
	if(IsNull()) return Null;
	return data.IsSpecial(BOOL_V) ? (double)GetSmall<bool>() :
	       data.IsSpecial(INT_V) ? (double)GetSmall<int>() :
	       (double)GetSmall<int64>();
}

bool Value::GetOtherBool() const
{
	if(IsNull()) return Null;
	return data.IsSpecial(DOUBLE_V) ? (bool)GetSmall<double>() :
	       data.IsSpecial(INT_V) ? (bool)GetSmall<int>() :
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
SVO_FN(s_int64, int64);
SVO_FN(s_bool, bool);
SVO_FN(s_date, Date);
SVO_FN(s_time, Time);

struct SvoVoidFn {
	static bool       IsNull(const void *p)                      { return true; }
	static void       Serialize(void *p, Stream& s)              {}
	static void       Xmlize(void *p, XmlIO& xio)               {}
	static void       Jsonize(void *p, JsonIO& jio)             {}
	static unsigned   GetHashValue(const void *p)                { return 0; }
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
		Value::AddName(VOID_V, "void");
		Value::AddName(DATE_V, "date");
		Value::AddName(TIME_V, "time");
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
		Free();
		int st = type == VOID_V ? VOIDV : type == STRING_V ? STRING : type;
		if(st == STRING)
			s % data;
		else
		if(st < 255 && svo[st]) {
			data.SetSpecial((byte)type);
			svo[st]->Serialize(&data, s);
		}
		else {
			typedef Void* (*vp)();
			vp *cr = Typemap().FindPtr(type);
			if(cr) {
				Void *p = (**cr)();
				p->Serialize(s);
				InitRef(p);
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
		s / type;
		int st = data.GetSpecial();
		ASSERT_(!type || type == ERROR_V || type == UNKNOWN_V || st == STRING ||
		        (IsRef() ? Typemap().Find(type) >= 0 : st < 255 && svo[st]),
		        AsString(type) + " is not registred for serialization");
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
			        AsString(type) + " is not registred for serialization");
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
		if(name == s_binary) {
			String s;
			Upp::Xmlize(xio, s);
			try {
				LoadFromString(*this, ScanHexString(s));
			}
			catch(LoadingError) {
				throw XmlError("serialized_binary Error");
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
					InitRef(p);
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
			        AsString(type) + " is not registred for serialization");
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
	else {
		Value g = jio.Get();
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
						InitRef(p);
					}
					else
						throw JsonizeError("invalid Value type");
				}
			}
		}
	}
}

void Value::Register(dword w, Void* (*c)(), const char *name) init_ {
#ifdef flagCHECKINIT
	RLOG("Register valuetype " << w);
#endif
	AssertST();
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
		dword t = ptr()->GetType();
		if(t == VALUEARRAY_V)
			return ((ValueArray::Data *)ptr())->data.GetCount();
		if(t == VALUEMAP_V)
			return ((ValueMap::Data *)ptr())->value.GetCount();
	}
	return ErrorValue();
}

const Value& Value::operator[](int i) const
{
	if(IsRef()) {
		dword t = ptr()->GetType();
		if(t == VALUEARRAY_V)
			return ((ValueArray::Data *)ptr())->data[i];
		if(t == VALUEMAP_V)
			return ((ValueMap::Data *)ptr())->value[i];
	}
	return ErrorValue();
}

const Value& Value::operator[](const String& key) const
{
	if(IsRef() && ptr()->GetType() == VALUEMAP_V)
		return ((ValueMap::Data *)ptr())->Get(key);
	return ErrorValue();	
}

String Value::GetName() const
{
	if(IsRef())
		return typeid(*ptr()).name();
	if(IsString())
		return "String";
	static Tuple2<byte, const char *> tp[] = {
		{ INT_V, "int" },
		{ DOUBLE_V, "double" },
		{ VOIDV, "void" },
		{ DATE_V, "Date" },
		{ TIME_V, "Time" },
		{ INT64_V, "int64" },
		{ BOOL_V, "bool" },
	};
	Tuple2<byte, const char *> *x = FindTuple(tp, __countof(tp), data.GetSpecial());
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
	return Value(new ValueErrorCls(s));
}

Value ErrorValue(const char *s) {
	return ErrorValue(String(s));
}

const Value& ErrorValue() {
	static Value *p;
	ONCELOCK {
		static Value v = ErrorValue(String());
		p = &v;
	}
	return *p;
}

String GetErrorText(const Value& v) {
	ASSERT(IsError(v));
	return ((RichValueRep<String> *)v.GetVoidPtr())->Get();
}

END_UPP_NAMESPACE

#endif