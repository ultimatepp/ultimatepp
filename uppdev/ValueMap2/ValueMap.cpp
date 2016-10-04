#include "ValueMap.h"

bool ValueMap::Data::IsNull() const {
	return this == &Single<ValueMap::NullData>();
}

void ValueMap::Data::Serialize(Stream& s) {
	s % key % value % unlinked;
}

unsigned ValueMap::Data::GetHashValue() const {
	CombineHash w(key.GetCount());
	for(int i = 0; i < key.GetCount(); i++)
		w.Put(key[i].GetHashValue());
	w.Put(value.GetHashValue());
	return w;
}

static bool sCmp(const Index<Value>& a, const Index<Value>& b) {
	if(&a == &b) return true;
	if(a.GetCount() != b.GetCount()) return false;
	for(int i = 0; i < a.GetCount(); i++) {
		if(a[i] != b[i]) return false;
	}
	return true;
}

bool ValueMap::Data::IsEqual(const Value::Void *p) {
	return sCmp(((Data *)p)->key, key) && ((Data *)p)->value == value;
}

bool ValueMap::operator==(const ValueMap& v) const {
	return sCmp(data->key, v.data->key) && data->value == v.data->value;
}

String ValueMap::Data::AsString() const
{
	String s;
	s << "{ ";
	bool was = false;
	for(int i = 0; i < key.GetCount(); i++) {
		if(!key.IsUnlinked(i)) {
			if(was) s << ", ";
			was = true;
			s << "(" << key[i] << ", " << value[i] << ")";
		}
	}
	s << " }";
	return s;
}

ValueArray ValueMap::Data::SweepedKeys()
{
	ValueArray va;
	for(int i = 0; i < key.GetCount(); i++)
		if(!key.IsUnlinked(i))
			va.Add(key[i]);
	return va;
}

ValueArray ValueMap::Data::SweepedArray()
{
	if(unlinked == 0)
		return value;
	ValueArray va;
	for(int i = 0; i < key.GetCount(); i++)
		if(!key.IsUnlinked(i))
			va.Add(value[i]);
	return va;
}


void ValueMap::Data::Sweep()
{
	value = SweepedArray();
	key.Sweep();
	unlinked = 0;
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
		d->unlinked = data->unlinked;
		data->Release();
		data = d;
	}
	return *data;
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
	Data& d = Clone();
	d.key.Clear();
	d.value.Clear();
}

void ValueMap::Put(const Value& key, const Value& value) {
	Data& d = Clone();
	int q = d.key.Find(key);
	if(d.unlinked > (d.key.GetCount() >> 1))
		d.Sweep();
	d.key.Add(key);
	d.value.Add(value);
}

void ValueMap::Remove(const Value& key) {
	Data& d = Clone();
	int q = d.key.Find(key);
	if(q >= 0) {
		d.key.Unlink(q);
		d.unlinked++;
	}
}

Value ValueMap::operator[](const Value& key) const
{
	int q = data->key.Find(key);
	return q >= 0 ? data->value[q] : ErrorValue();
}

ValueArray ValueMap::GetKeys() const
{
	return data->SweepedKeys();
}

ValueArray ValueMap::GetValues() const
{
	return data->SweepedArray();
}

//RichValue<ValueMap>::Registrator ValueMapRegistrator;
