#include "Core.h"

NAMESPACE_UPP

#ifndef flagSO
const Nuller Null;
#endif

#define LTIMING(x) // RTIMING(x)

unsigned Value::GetHashValue() const {
	return IsNull() ? 0 : ptr->GetHashValue();
}

Value& Value::operator=(const Value& v) {
	if(this == &v) return *this;
	ptr->Release();
	ptr = v.ptr;
	ptr->Retain();
	return *this;
}

Value::Value(const Value& v) {
	ptr = v.ptr;
	ptr->Retain();
}

void Value::SetVoidVal()
{
	ptr = &Single<Void>();
	ptr->Retain();
}

Value::Value() {
	SetVoidVal();
}

Value::~Value() {
	ptr->Release();
}

bool Value::operator==(const Value& v) const {
	if(ptr == v.ptr) return true;
	bool an = IsNull();
	bool bn = v.IsNull();
	if(an || bn) return an && bn;
	if(GetType() == v.GetType())
		return ptr->IsEqual(v.ptr);
	return ptr->IsPolyEqual(v) || v.ptr->IsPolyEqual(*this);
}

Value::Value(const String& s)  { ptr = new RichValueRep<String>(s); }
Value::Value(const WString& s) { ptr = new RichValueRep<WString>(s); }
Value::Value(const char *s)    { ptr = new RichValueRep<String>(s); }
Value::Value(int i)            { ptr = new RichValueRep<int>(i); }
Value::Value(int64 i)          { ptr = new RichValueRep<int64>(i); }
Value::Value(double d)         { ptr = new RichValueRep<double>(d); }
Value::Value(bool b)           { ptr = new RichValueRep<bool>(b); }
Value::Value(Date d)           { ptr = new RichValueRep<Date>(d); }
Value::Value(Time t)           { ptr = new RichValueRep<Time>(t); }
Value::Value(const Nuller&)    { ptr = new RichValueRep<int>(Null); }

Value::operator String() const
{
	if(IsNull()) return Null;
	return GetType() == WSTRING_V ? RichValue<WString>::Extract(*this).ToString()
		                          : RichValue<String>::Extract(*this);
}

Value::operator WString() const
{
	if(IsNull()) return Null;
	return GetType() == WSTRING_V ? RichValue<WString>::Extract(*this)
		                          : RichValue<String>::Extract(*this).ToWString();
}

Value::operator Date() const
{
	if(IsNull()) return Null;
	return GetType() == DATE_V ? RichValue<Date>::Extract(*this)
		                       : Date(RichValue<Time>::Extract(*this));
}

Value::operator Time() const
{
	if(IsNull()) return Null;
	return GetType() == DATE_V ? ToTime(RichValue<Date>::Extract(*this))
		                       : RichValue<Time>::Extract(*this);
}

Value::operator double() const
{
	if(IsNull()) return Null;
	return GetType() == INT_V   ? double(RichValue<int>::Extract(*this))
	     : GetType() == BOOL_V ? double(RichValue<bool>::Extract(*this))
	     : GetType() == INT64_V ? double(RichValue<int64>::Extract(*this))
		                        : RichValue<double>::Extract(*this);
}

Value::operator int() const
{
	if(IsNull()) return Null;
	return GetType() == INT_V   ? RichValue<int>::Extract(*this)
	     : GetType() == BOOL_V ? int(RichValue<bool>::Extract(*this))
	     : GetType() == INT64_V ? int(RichValue<int64>::Extract(*this))
		                        : int(RichValue<double>::Extract(*this));
}

Value::operator int64() const
{
	if(IsNull()) return Null;
	return GetType() == INT_V   ? int64(RichValue<int>::Extract(*this))
	     : GetType() == BOOL_V ? int64(RichValue<bool>::Extract(*this))
	     : GetType() == INT64_V ? RichValue<int64>::Extract(*this)
		                        : int64(RichValue<double>::Extract(*this));
}

Value::operator bool() const
{
	if(IsNull()) return false;
	return operator int();
}

VectorMap<dword, Value::Void *(*)(Stream& s)>& Value::Typemap()
{
	static VectorMap<dword, Value::Void *(*)(Stream& s)> x;
	return x;
}

static void sRegisterStd()
{
	ONCELOCK {
		RichValue<int>::Register();
		RichValue<int64>::Register();
		RichValue<double>::Register();
		RichValue<String>::Register();
		RichValue<WString>::Register();
		RichValue<Date>::Register();
		RichValue<Time>::Register();
	};
}

INITBLOCK {
	sRegisterStd();
}


void Value::Serialize(Stream& s) {
	sRegisterStd();
	dword type;
	if(s.IsLoading()) {
		s / type;
		ptr->Release();
		typedef Void* (*vp)(Stream& s);
		vp *cr = Typemap().FindPtr(type);
		if(cr)
			ptr = (**cr)(s);
		else {
			SetVoidVal();
			if(type != VOID_V && type != ERROR_V)
				s.LoadError();
		}
	}
	else {
		type = GetType();
		s / type;
		ptr->Serialize(s);
	}
}

void Value::Register(dword w, Void* (*c)(Stream& s)) init_ {
#ifdef flagCHECKINIT
	RLOG("Register valuetype " << w);
#endif
	AssertST();
	ASSERT(w != UNKNOWN_V);
	ASSERT(w < 0x8000000);
	CHECK(Typemap().GetAdd(w, c) == c);
}

String  Value::ToString() const {
	return ptr->AsString();
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

Value ErrorValue() {
	return ErrorValue(String());
}

String GetErrorText(const Value& v) {
	ASSERT(IsError(v));
	return ((RichValueRep<String> *)v.GetVoidPtr())->Get();
}

// ----------------------------------

struct Ref::ValueRef : public RefManager {
	virtual int   GetType()                            { return VALUE_V; }
	virtual Value GetValue(const void *ptr)            { return *(Value *) ptr; }
	virtual bool  IsNull(const void *ptr)              { return UPP::IsNull(*(Value *) ptr); }
	virtual void  SetValue(void *ptr, const Value& v)  { *(Value *) ptr = v; }
	virtual void  SetNull(void *ptr)                   { *(Value *) ptr = Null; }
};


Ref::Ref(String& s)  { ptr = &s; m = &Single< RichRef<String> >(); }
Ref::Ref(WString& s) { ptr = &s; m = &Single< RichRef<WString> >(); }
Ref::Ref(int& i)     { ptr = &i; m = &Single< RichRef<int> >(); }
Ref::Ref(int64& i)   { ptr = &i; m = &Single< RichRef<int64> >(); }
Ref::Ref(double& d)  { ptr = &d; m = &Single< RichRef<double> >(); }
Ref::Ref(Date& d)    { ptr = &d; m = &Single< RichRef<Date> >(); }
Ref::Ref(Time& t)    { ptr = &t; m = &Single< RichRef<Time> >(); }

Ref::Ref(Value& v)   { ptr = &v; m = &Single< ValueRef >(); }

// ----------------------------------

bool ValueArray::Data::IsNull() const
{
	return this == &Single<ValueArray::NullData>();
}

void ValueArray::Data::Serialize(Stream& s)
{
	s % data;
}

unsigned ValueArray::Data::GetHashValue() const
{
	CombineHash w(data.GetCount());
	for(int i = 0; i < data.GetCount(); i++)
		w.Put(data[i].GetHashValue());
	return w;
}

static bool sCmp(const Vector<Value>& a, const Vector<Value>& b)
{
	if(&a == &b) return true;
	if(a.GetCount() != b.GetCount()) return false;
	for(int i = 0; i < a.GetCount(); i++)
		if(a[i] != b[i]) return false;
	return true;
}

bool ValueArray::Data::IsEqual(const Value::Void *p)
{
	return sCmp(((Data *)p)->data, data);
}

bool ValueArray::operator==(const ValueArray& v) const
{
	return sCmp(data->data, v.data->data);
}

static String sAsString(const Vector<Value>& v)
{
	String s;
	s << "{ ";
	for(int i = 0; i < v.GetCount(); i++) {
		if(i) s << ", ";
		s << v[i];
	}
	s << " }";
	return s;
}

String ValueArray::Data::AsString() const
{
	return sAsString(data);
}

Vector<Value>& ValueArray::Create()
{
	data = new Data;
	return data->data;
}

Vector<Value>& ValueArray::Clone() {
	if(data->GetRefCount() != 1) {
		Data *d = new Data;
		d->data <<= data->data;
		data->Release();
		data = d;
	}
	return data->data;
}

ValueArray::ValueArray() {
	data = &Single<NullData>();
	data->Retain();
}

ValueArray::ValueArray(const ValueArray& v) {
	data = v.data;
	data->Retain();
}

ValueArray::ValueArray(const Nuller&) {
	data = &Single<NullData>();
	data->Retain();
}

ValueArray::ValueArray(pick_ Vector<Value>& v)
{
	Create() = v;
}

ValueArray::ValueArray(const Vector<Value>& v, int deep)
{
	Create() <<= v;
}

ValueArray::operator Value() const {
	data->Retain();
	return Value(data);
}

ValueArray::ValueArray(const Value& src)
{
	if(!UPP::IsNull(src)) {
		if(IsType<ValueMap>(src)) {
			ValueArray v = ValueMap(src);
			data = v.data;
			data->Retain();
			return;
		}
		else {
			ASSERT(dynamic_cast<const ValueArray::Data *>(src.GetVoidPtr()));
			data = (ValueArray::Data *)src.GetVoidPtr();
		}
	}
	else
		data = &Single<NullData>();
	data->Retain();
}

void ValueArray::Serialize(Stream& s) {
	if(s.IsStoring())
		Clone();
	data->Serialize(s);
}

ValueArray::~ValueArray() {
	ASSERT(data->GetRefCount() > 0);
	data->Release();
}

ValueArray& ValueArray::operator=(const ValueArray& v) {
	v.data->Retain();
	data->Release();
	data = v.data;
	return *this;
}

void ValueArray::SetCount(int n) {
	Clone().SetCount(n);
}

void ValueArray::SetCount(int n, const Value& v)
{
	Clone().SetCount(n, v);
}

void ValueArray::Clear() {
	Clone().Clear();
}

void ValueArray::Add(const Value& v) {
	Clone().Add(v);
}

void ValueArray::Set(int i, const Value& v) {
	ASSERT(i >= 0);
	Clone().DoIndex(i) = v;
}

void ValueArray::Remove(int i)
{
	Clone().Remove(i);
}

const Value& ValueArray::Get(int i) const {
	ASSERT(i >= 0 && i < GetCount());
	return data->data[i];
}

template<>
String AsString(const ValueArray& v) {
	return sAsString(v.Get());
}

INITBLOCK {
	RichValue<ValueArray>::Register();
}

bool ValueMap::Data::IsNull() const {
	return this == &Single<ValueMap::NullData>();
}

void ValueMap::Data::Serialize(Stream& s) {
	s % key % value;
}

unsigned ValueMap::Data::GetHashValue() const {
	CombineHash w(key.GetCount());
	for(int i = 0; i < key.GetCount(); i++)
		w.Put(key[i].GetHashValue());
	w.Put(value.GetHashValue());
	return w;
}

static bool sIsEqual(const Index<Value>& a, const Index<Value>& b) {
	if(&a == &b) return true;
	if(a.GetCount() != b.GetCount()) return false;
	for(int i = 0; i < a.GetCount(); i++) {
		if(a[i] != b[i]) return false;
	}
	return true;
}

bool ValueMap::Data::IsEqual(const Value::Void *p) {
	return sIsEqual(((Data *)p)->key, key) && ((Data *)p)->value == value;
}

bool ValueMap::operator==(const ValueMap& v) const {
	return sIsEqual(data->key, v.data->key) && data->value == v.data->value;
}

String ValueMap::Data::AsString() const
{
	String s;
	s << "{ ";
	for(int i = 0; i < key.GetCount(); i++) {
		if(i) s << ", ";
		s << "(" << key[i] << ", " << value[i] << ")";
	}
	s << " }";
	return s;
}

ValueMap::Data& ValueMap::Create()
{
	data = new Data;
	return *data;
}

ValueMap::Data& ValueMap::Clone() {
	if(data->GetRefCount() != 1) {
		Data *d = new Data;
		d->key <<= data->key;
		d->value = data->value;
		data->Release();
		data = d;
	}
	return *data;
}

void ValueMap::Init0()
{
	data = &Single<NullData>();
	data->Retain();
}

ValueMap::ValueMap(const ValueMap& v)
{
	data = v.data;
	data->Retain();
}

ValueMap::operator Value() const {
	data->Retain();
	return Value(data);
}

ValueMap::ValueMap(const Value& src)
{
	if(!IsNull(src)) {
		if(IsType<ValueArray>(src)) {
			ValueArray va = src;
			Init0();
			for(int i = 0; i < va.GetCount(); i++)
				Add(i, va[i]);
			return;
		}
		else {
			ASSERT(dynamic_cast<const ValueMap::Data *>(src.GetVoidPtr()));
			data = (ValueMap::Data *)src.GetVoidPtr();
		}
	}
	else
		data = &Single<NullData>();
	data->Retain();
}

void ValueMap::Serialize(Stream& s) {
	if(s.IsLoading()) {
		data->Release();
		Create();
	}
	data->Serialize(s);
}

ValueMap::~ValueMap() {
	ASSERT(data->GetRefCount() > 0);
	data->Release();
}

ValueMap& ValueMap::operator=(const ValueMap& v) {
	v.data->Retain();
	data->Release();
	data = v.data;
	return *this;
}

void ValueMap::Clear() {
	data->Release();
	Init0();
}

void ValueMap::Add(const Value& key, const Value& value) {
	Data& d = Clone();
	d.key.Add(key);
	d.value.Add(value);
}

Value ValueMap::operator[](const Value& key) const
{
	int q = data->key.Find(key);
	return q >= 0 ? data->value[q] : ErrorValue();
}

INITBLOCK {
	RichValue<Color>::Register();
}

// ----------------------------------

struct IdList : public Index<String> {
	IdList()  { Add(String()); }
};

Index<String>& Id::Ids() {
	return Single<IdList>();
}

static StaticCriticalSection s_ids;

void Id::Set(const String& s) {
	CriticalSection::Lock __(s_ids);
	ndx = Ids().FindAdd(s);
}

String Id::ToString() const {
	CriticalSection::Lock __(s_ids);
	return Ids()[ndx];
}

const String& Id::AsString(int n) {
	CriticalSection::Lock __(s_ids);
	return Ids()[n];
}

Id Id::Find(const String& s) {
	CriticalSection::Lock __(s_ids);
	int i = Ids().Find(s);
	if(i < 0) return Id();
	return Id(i);
}

Id::Id(const IdConst& cnst) {
	if(cnst.ndx > 0) {
		ASSERT(cnst.ndx >= 0 && cnst.ndx < Ids().GetCount());
		ndx = cnst.ndx;
	}
	else {
		Set(cnst.text);
		cnst.ndx = ndx;
	}
}

int StdValueCompare(const Value& a, const Value& b, int language)
{
	LTIMING("StdValueCompare");

	bool na = IsNull(a), nb = IsNull(b);
	if(na || nb)
		return !na ? 1 : !nb ? -1 : 0;
	dword ta = a.GetType(), tb = b.GetType();
	if((ta == INT_V || ta == BOOL_V) && (tb == INT_V || tb == BOOL_V))
		return cmp<int>(a, b);
	if((ta == BOOL_V || ta == INT_V || ta == INT64_V || ta == DOUBLE_V)
	&& (tb == BOOL_V || tb == INT_V || tb == INT64_V || tb == DOUBLE_V))
		return cmp<double>(a, b);
	if(ta == DATE_V && tb == DATE_V)
		return cmp<Date>(a, b);
	if((ta == DATE_V || ta == TIME_V) && (tb == DATE_V || tb == TIME_V))
		return cmp<Time>(a, b);
	if((ta == STRING_V || ta == WSTRING_V) && (tb == STRING_V || tb == WSTRING_V))
		return GetLanguageInfo(language).Compare(WString(a), WString(b));
	return cmp<int>(ta, tb);
}

int StdValueCompare(const Value& a, const Value& b)
{
	return StdValueCompare(a, b, 0);
}

int StdValueCompareDesc(const Value& a, const Value& b, int language)
{
	return -StdValueCompare(a, b, language);
}

int StdValueCompareDesc(const Value& a, const Value& b)
{
	return -StdValueCompare(a, b);
}

StdValueOrder::StdValueOrder(int l) : language(l) {}
StdValueOrder::~StdValueOrder() {}

bool StdValueOrder::operator()(const Value& a, const Value& b) const
{
	return StdValueCompare(a, b, language) < 0;
}

FnValueOrder::FnValueOrder(int (*fn)(const Value& a, const Value& b)) : fn(fn) {}
FnValueOrder::~FnValueOrder() {}

bool FnValueOrder::operator()(const Value& a, const Value& b) const
{
	return (*fn)(a, b) < 0;
}

END_UPP_NAMESPACE
