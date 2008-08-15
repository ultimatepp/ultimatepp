#include "ValueMap.h"

bool ValueMap::Data::IsNull() const {
	return this == &Single<ValueMap::NullData>();
}

void ValueMap::Data::Serialize(Stream& s) {
	s % data;
}

unsigned ValueMap::Data::GetHashValue() const {
	CombineHash w(data.GetCount());
	for(int i = 0; i < data.GetCount(); i++)
		w.Put(data[i].GetHashValue());
	return w;
}

static bool sCmp(const VectorMap<Value, Value>& a, const VectorMap<Value, Value>& b) {
	if(&a == &b) return true;
	if(a.GetCount() != b.GetCount()) return false;
	for(int i = 0; i < a.GetCount(); i++) {
		if(a[i] != b[i]) return false;
		if(a.GetKey(i) != b.GetKey(i)) return false;
	}
	return true;
}

bool ValueMap::Data::IsEqual(const Value::Void *p) {
	return sCmp(((Data *)p)->data, data);
}

bool ValueMap::operator==(const ValueMap& v) const {
	return sCmp(data->data, v.data->data);
}

static String sAsString(const VectorMap<Value, Value>& v) {
	String s;
	s << "{ ";
	for(int i = 0; i < v.GetCount(); i++) {
		if(i) s << ", ";
		s <<"("<<v.GetKey(i)<<", "<< v[i]<<")";
	}
	s << " }";
	return s;
}

String ValueMap::Data::AsString() const
{
	return sAsString(data);
}

VectorMap<Value, Value>& ValueMap::Create()
{
	data = new Data;
	return data->data;
}

VectorMap<Value, Value>& ValueMap::Clone() {
	if(data->GetRefCount() != 1) {
		Data *d = new Data;
		d->data <<= data->data;
		data->Release();
		data = d;
	}
	return data->data;
}

ValueMap::ValueMap() {
	data = &Single<NullData>();
	data->Retain();
}

ValueMap::ValueMap(const ValueMap& v) {
	data = v.data;
	data->Retain();
}

ValueMap::ValueMap(const Nuller&) {
	data = &Single<NullData>();
	data->Retain();
}

ValueMap::ValueMap(pick_ VectorMap<Value, Value>& v)
{
	Create() = v;
}

ValueMap::ValueMap(const VectorMap<Value, Value>& v, int deep)
{
	Create() <<= v;
}

ValueMap::operator Value() const {
	data->Retain();
	return Value(data);
}

ValueMap::ValueMap(const Value& src)
{
	if(!::IsNull(src)) {
		ASSERT(dynamic_cast<const ValueMap::Data *>(src.GetVoidPtr()));
		data = (ValueMap::Data *)src.GetVoidPtr();
	}
	else
		data = &Single<NullData>();
	data->Retain();
}

void ValueMap::Serialize(Stream& s) {
	if(s.IsStoring())
		Clone();
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
	Clone().Clear();
}

void ValueMap::Add(const Value& k, const Value& v) {
	Clone().Add(k, v);
}

void ValueMap::Set(const Value& k, const Value& v) {
	Clone();
	data->data[data->data.Find(k)] = v;
}

/*
void ValueMap::Remove(int i) {
	Clone().Remove(i);
}

void ValueMap::Remove(const Value& k) {
	Clone().Remove(k);
}
*/

const Value& ValueMap::Get(int i) const {
	ASSERT(i >= 0 && i < GetCount());
	return data->data[i];
}
const Value& ValueMap::GetKey(int i) const {
	ASSERT(i >= 0 && i < GetCount());
	return data->data.GetKey(i);
};

template<>
String AsString(const ValueMap& v) {
	return sAsString(v.Get());
}

RichValue<ValueMap>::Registrator ValueMapRegistrator;
