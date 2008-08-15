#ifndef _ValueMap_ValueMap_h_
#define _ValueMap_ValueMap_h_
#include <Core/Core.h>

using namespace Upp;

const int VALUEMAP_V = 512;

class ValueMap : AssignValueTypeNo<ValueMap, VALUEMAP_V, Moveable<ValueMap> >{
	struct Data : Value::Void {
		virtual dword      GetType() const             { return VALUEMAP_V; }
		virtual bool       IsNull() const;
		virtual void       Serialize(Stream& s);
		virtual unsigned   GetHashValue() const;
		virtual bool       IsEqual(const Value::Void *p);
		virtual String     AsString() const;

		int GetRefCount() const     { return AtomicRead(refcount); }

		VectorMap<Value, Value> data;
	};
	struct NullData : Data {};
	Data *data;
	
	VectorMap<Value, Value>& Create();
	VectorMap<Value, Value>& Clone();
	
public:
	ValueMap();
	ValueMap(const ValueMap& v);
	explicit ValueMap(pick_ VectorMap<Value, Value>& values);
	explicit ValueMap(const VectorMap<Value, Value>& values, int deep);
	~ValueMap();

	ValueMap& operator=(const ValueMap& v);

	operator Value() const;
	ValueMap(const Value& src);

	ValueMap(const Nuller&);
	bool IsNull() const                       { return data->IsNull(); }

	void Clear();

	int  GetCount() const                     { return data->data.GetCount(); }
	bool IsEmpty() const                      { return data->data.IsEmpty(); }

	void Add(const Value& k, const Value& v);
	void Set(const Value& k, const Value& v);
//	void Remove(int i);
//	void Remove(const Value& k);
	const Value& Get(int i) const;
	const Value& GetKey(int i) const;
	const Value& Get(const Value& k) const				{ return data->data.Get(k); }
	const VectorMap<Value, Value>& Get() const          { return data->data; }

	const Value& operator[](int i) const      			{ return Get(i); }
	const Value& operator[](const Value& k) const		{ return Get(k); }

	unsigned GetHashValue() const             { return data->GetHashValue(); }
	void  Serialize(Stream& s);

	bool operator==(const ValueMap& v) const;
	bool operator!=(const ValueMap& v) const  { return !operator==(v); }
};

template<> inline unsigned GetHashValue(const ValueMap& v) { return v.GetHashValue(); }
template<> inline bool     IsNull(const ValueMap& v)       { return v.IsNull(); }

template<>
String AsString(const ValueMap& v);

inline bool IsValueMap(const Value& v) { return v.GetType() == VALUEMAP_V; }

#endif
