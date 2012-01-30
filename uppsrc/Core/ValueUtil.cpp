#include "Core.h"

#ifdef SVO_VALUE

NAMESPACE_UPP

#define LTIMING(x) // RTIMING(x)

struct Ref::ValueRef : public RefManager {
	virtual int   GetType()                            { return VALUE_V; }
	virtual Value GetValue(const void *ptr)            { return *(Value *) ptr; }
	virtual bool  IsNull(const void *ptr)              { return UPP::IsNull(*(Value *) ptr); }
	virtual void  SetValue(void *ptr, const Value& v)  { *(Value *) ptr = v; }
	virtual void  SetNull(void *ptr)                   { *(Value *) ptr = Null; }
};

Ref::Ref(String& s)  { ptr = &s; m = &Single< StdRef<String> >(); }
Ref::Ref(WString& s) { ptr = &s; m = &Single< StdRef<WString> >(); }
Ref::Ref(int& i)     { ptr = &i; m = &Single< StdRef<int> >(); }
Ref::Ref(int64& i)   { ptr = &i; m = &Single< StdRef<int64> >(); }
Ref::Ref(double& d)  { ptr = &d; m = &Single< StdRef<double> >(); }
Ref::Ref(bool& b)    { ptr = &b; m = &Single< StdRef<bool> >(); }
Ref::Ref(Date& d)    { ptr = &d; m = &Single< StdRef<Date> >(); }
Ref::Ref(Time& t)    { ptr = &t; m = &Single< StdRef<Time> >(); }
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
	s << "[";
	for(int i = 0; i < v.GetCount(); i++) {
		if(i) s << ", ";
		s << v[i];
	}
	s << "]";
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

void ValueArray::Init0()
{
	data = &Single<NullData>();
	data->Retain();
}

ValueArray::ValueArray(const ValueArray& v) {
	data = v.data;
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
	if(s.IsLoading()) {
		data->Release();
		Create();
	}
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
	Clone().At(i) = v;
}

void ValueArray::Remove(int i, int count)
{
	Clone().Remove(i, count);
}

void ValueArray::Insert(int i, const ValueArray& va)
{
	if(va.data == data) {
		ValueArray va2 = va;
		Clone().Insert(i, va2.Get());
	}
	else
		Clone().Insert(i, va.Get());
}

const Value& ValueArray::Get(int i) const {
	ASSERT(i >= 0 && i < GetCount());
	return data->data[i];
}

Value ValueArray::GetAndClear(int i)
{
	ASSERT(i >= 0 && i < GetCount());
	Vector<Value>& x = Clone();
	Value v = x[i];
	x[i] = Value();
	return v;
}

template<>
String AsString(const ValueArray& v) {
	return sAsString(v.Get());
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
		s << key[i] << ": " << value[i];
	}
	s << " }";
	return s;
}

ValueMap::Data& ValueMap::Create()
{
	data = new Data;
	return *data;
}

const Value& ValueMap::Data::Get(const Value& k) const
{
	int q = key.Find(k);
	return q >= 0 ? value[q] : ErrorValue();
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

ValueMap::ValueMap(pick_ Index<Value>& k, pick_ Vector<Value>& v)
{
	Data& d = Create();
	d.key = k;
	d.value = ValueArray(v);
}

ValueMap::ValueMap(const Index<Value>& k, const Vector<Value>& v, int deep)
{
	Data& d = Create();
	d.key <<= k;
	Vector<Value> _v(v, 0);
	d.value = ValueArray(_v);
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

void ValueMap::Set(const Value& key, const Value& value)
{
	Data& d = Clone();
	int i = d.key.Find(key);
	if(i >= 0)
		d.value.Set(i, value);
	else {
		d.key.Add(key);
		d.value.Add(value);
	}
}

void ValueMap::SetAt(int i, const Value& v) {
	Clone().value.Set(i, v);
}

void ValueMap::SetKey(int i, const Value& k) {
	Clone().key.Set(i, k);
}

void ValueMap::Remove(int i)
{
	Data& d = Clone();
	d.key.Remove(i);
	d.value.Remove(i);
}

const Value& ValueMap::operator[](const Value& key) const
{
	return data->Get(key);
}

Value ValueMap::GetAndClear(const Value& key)
{
	Data& d = Clone();
	int q = d.key.Find(key);
	return q < 0 ? ErrorValue() : d.value.GetAndClear(q);
}

// ----------------------------------

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
	return StdValueCompare(a, b, GetCurrentLanguage());
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

bool StdValueOrder::operator()(const Value& a, const Value& b) const
{
	return StdValueCompare(a, b, language) < 0;
}

bool FnValueOrder::operator()(const Value& a, const Value& b) const
{
	return (*fn)(a, b) < 0;
}

bool StdValuePairOrder::operator()(const Value& k1, const Value& v1, const Value& k2, const Value& v2) const
{
	int q = StdValueCompare(k1, k2, language);
	if(q) return q < 0;
	return StdValueCompare(v1, v2, language);
}

bool FnValuePairOrder::operator()(const Value& keya, const Value& valuea, const Value& keyb, const Value& valueb) const
{
	return (*fn)(keya, valuea, keyb, valueb) < 0;
}

END_UPP_NAMESPACE

#endif
