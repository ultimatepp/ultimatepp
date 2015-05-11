#include "Core.h"

NAMESPACE_UPP

static String sAsString(const Vector<Value>& v);

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
	return data.IsEmpty();
}

void ValueArray::Data::Serialize(Stream& s)
{
#if !defined(_MSC_VER) || _MSC_VER > 1310
	s % data;
#else
	throw 0;
#endif
}

void ValueArray::Data::Jsonize(JsonIO& jio)
{
#if !defined(_MSC_VER) || _MSC_VER > 1310
	Upp::Jsonize(jio, data);
#else
	throw 0;
#endif
}

void ValueArray::Data::Xmlize(XmlIO& io)
{
#if !defined(_MSC_VER) || _MSC_VER > 1310
	Upp::Xmlize(io, data);
#else
	throw 0;
#endif
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

int ValueArray::Data::Compare(const Value::Void *p)
{
	return CompareArray(data, ((Data *)p)->data);
}

bool ValueArray::operator==(const ValueArray& v) const
{
	return sCmp(data->data, v.data->data);
}

int ValueArray::Compare(const ValueArray& b) const
{
	return CompareArray(data->data, b.data->data);
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

ValueArray::ValueArray(Vector<Value> rval_ v)
{
	Create() = pick(v);
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
			if(src.GetType() != VALUEARRAY_V)
				throw ValueTypeError(String().Cat() << "Invalid value conversion: "
			                         << src.GetTypeName() << " -> ValueArray",
			                         src, VALUEARRAY_V);
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

void ValueArray::Jsonize(JsonIO& jio)
{
	if(jio.IsLoading()) {
		data->Release();
		Create();
	}
	data->Jsonize(jio);
}

void ValueArray::Xmlize(XmlIO& xio)
{
	if(xio.IsLoading()) {
		data->Release();
		Create();
	}
	data->Xmlize(xio);
}

String ValueArray::ToString() const
{
	return sAsString(Get());
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
#if !defined(_MSC_VER) || _MSC_VER > 1310
	Clone().SetCount(n);
#else
	throw 0;
#endif
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
#if !defined(_MSC_VER) || _MSC_VER > 1310
	ASSERT(i >= 0);
	Clone().At(i) = v;
#else
	throw 0;
#endif
}

Value& ValueArray::At(int i)
{
	return Clone().At(i);
}

void ValueArray::Remove(int i, int count)
{
	Clone().Remove(i, count);
}

void ValueArray::Remove(const Vector<int>& ii)
{
	Clone().Remove(ii);
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

Vector<Value> ValueArray::Pick()
{
	Vector<Value>& x = Clone();
	Vector<Value> r = pick(x);
	x.Clear();
	return r;
}

template<>
String AsString(const ValueArray& v) {
	return sAsString(v.Get());
}

bool ValueMap::Data::IsNull() const {
	return this == &Single<ValueMap::NullData>();
}

void ValueMap::Data::Serialize(Stream& s) {
#if !defined(_MSC_VER) || _MSC_VER > 1310
	s % key % value;
#else
	throw 0;
#endif
}

void ValueMap::Data::Xmlize(XmlIO& xio)
{
#if !defined(_MSC_VER) || _MSC_VER > 1310
	Upp::Xmlize(xio, key);
	Upp::Xmlize(xio, value);
#else
	throw 0;
#endif
}

void ValueMap::Data::Jsonize(JsonIO& jio)
{
	if(jio.IsStoring()) {
		ValueArray va;
		int n = min(value.GetCount(), key.GetCount());
		for(int i = 0; i < n; i++) {
			ValueMap m;
			m.Add("key", StoreAsJsonValue(key[i]));
			m.Add("value", StoreAsJsonValue(value[i]));
			va.Add(m);
		}
		jio.Set(va);
	}
	else {
		Value va = jio.Get();
		key.Clear();
		value.Clear();
		for(int i = 0; i < va.GetCount(); i++) {
			Value k, v;
			LoadFromJsonValue(k, va[i]["key"]);
			LoadFromJsonValue(v, va[i]["value"]);
			key.Add(k);
			value.Add(v);
		}
	}
}

unsigned ValueMap::Data::GetHashValue() const {
	CombineHash w(key.GetCount());
	for(int i = 0; i < key.GetCount(); i++)
		w.Put(key[i].GetHashValue());
	w.Put(value.GetHashValue());
	return w;
}

static bool sIsEqual(const Index<Value>& a, const Index<Value>& b)
{
	if(&a == &b) return true;
	if(a.GetCount() != b.GetCount()) return false;
	for(int i = 0; i < a.GetCount(); i++) {
		if(a[i] != b[i]) return false;
	}
	return true;
}

bool ValueMap::Data::IsEqual(const Value::Void *p)
{
	return sIsEqual(((Data *)p)->key, key) && ((Data *)p)->value == value;
}

bool ValueMap::operator==(const ValueMap& v) const
{
	return sIsEqual(data->key, v.data->key) && data->value == v.data->value;
}

int  ValueMap::Data::Compare(const Value::Void *p)
{
	Data *b = (Data *)p;
	int n = min(key.GetCount(), b->key.GetCount());
	for(int i = 0; i < n; i++) {
		int q = SgnCompare(key[i], b->key[i]);
		if(q)
			return q;
		q = SgnCompare(value[i], b->value[i]);
		if(q)
			return q;
	}
	return SgnCompare(key.GetCount(), b->key.GetCount());
}

int ValueMap::Compare(const ValueMap& b) const
{
	return data->Compare((Value::Void *)b.data);
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

ValueMap::Data& ValueMap::Clone(Data *&ptr)
{
	if(ptr->GetRefCount() != 1) {
		Data *d = new Data;
		d->key <<= ptr->key;
		d->value = ptr->value;
		ptr->Release();
		ptr = d;
	}
	return *ptr;
}

ValueMap::Data& ValueMap::Clone() {
	return Clone(data);
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

ValueMap::ValueMap(Index<Value> rval_ k, Vector<Value> rval_ v)
{
	Data& d = Create();
	d.key = pick(k);
	d.value = ValueArray(pick(v));
}

ValueMap::ValueMap(VectorMap<Value, Value> rval_ m)
{
	Data& d = Create();
	d.key = m.PickKeys();
	d.value = ValueArray(m.PickValues());
}

ValueMap::ValueMap(const Index<Value>& k, const Vector<Value>& v, int deep)
{
	Data& d = Create();
	d.key <<= k;
	d.value = ValueArray(v, 0);
}

ValueMap::ValueMap(const VectorMap<Value, Value>& m, int deep)
{
	Data& d = Create();
	d.key = clone(m.GetKeys());
	d.value = ValueArray(m.GetValues(), 0);
}

VectorMap<Value, Value> ValueMap::Pick()
{
	Data& d = Clone();
	VectorMap<Value, Value> m(d.key.PickKeys(), d.value.Pick());
	d.key.Clear();
	return m;
}

ValueMap::operator Value() const {
	data->Retain();
	return Value(data);
}

Value& ValueMap::Data::GetAdd(const Value& k)
{
	int i = key.Find(k);
	if(i < 0) {
		i = value.GetCount();
		key.Add(k);
	}
	return value.At(i);
}

Value& ValueMap::Data::At(int i)
{
	ASSERT(i < value.GetCount());
	return value.At(i);
}

Value& ValueMap::GetAdd(const Value& key)
{
	return Clone().GetAdd(key);
}

Value& ValueMap::At(int i)
{
	return Clone().At(i);
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
			if(src.GetType() != VALUEMAP_V)
				throw ValueTypeError(String().Cat() << "Invalid value conversion: "
			                         << src.GetTypeName() << " -> ValueMap",
			                         src, VALUEMAP_V);
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

void ValueMap::Jsonize(JsonIO& jio)
{
	if(jio.IsLoading()) {
		data->Release();
		Create();
	}
	data->Jsonize(jio);
}

void ValueMap::Xmlize(XmlIO& xio)
{
	if(xio.IsLoading()) {
		data->Release();
		Create();
	}
	data->Xmlize(xio);
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

int ValueMap::RemoveKey(const Value& key)
{
	Data& d = Clone();
	Vector<int> rk;
	int q = d.key.Find(key);
	while(q >= 0) {
		rk.Add(q);
		q = d.key.FindNext(q);
	}
	if(rk.GetCount()) {
		Sort(rk);
		d.key.Remove(rk);
		d.value.Remove(rk);
	}
	return rk.GetCount();
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
	return a.Compare(b);
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

void Complex::Xmlize(XmlIO& xio)
{
	double r, i;
	r = real(); i = imag();
	xio.Attr("real", r).Attr("imag", i);
	*this = C(r, i);
}

void Complex::Jsonize(JsonIO& jio)
{
	double r, i;
	r = real(); i = imag();
	jio("real", r)("imag", i);
	*this = C(r, i);
}

void Complex::Serialize(Stream& s)
{
	double r, i;
	r = real(); i = imag();
	s % r % i;
	*this = C(r, i);
}

template <class T>
static void sReg(const char *name)
{
	if(FitsSvoValue<T>())
		Value::SvoRegister<T>(name);
	else
		Value::Register<T>(name);
}

INITBLOCK
{
	sReg<Point>("Point");
	sReg<Point64>("Point64");
	sReg<Pointf>("Pointf");
	sReg<Size>("Size");
	sReg<Size64>("Size64");
	sReg<Sizef>("Sizef");
	Value::Register<Rect>("Rect");
	Value::Register<Rect64>("Rect64");
	Value::Register<Rectf>("Rectf");
}

END_UPP_NAMESPACE

