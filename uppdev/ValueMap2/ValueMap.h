#ifndef _ValueMap2_ValueMap_h_
#define _ValueMap2_ValueMap_h_

#include <Core/Core.h>

#include <Sql/Sql.h>

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
		
		ValueArray SweepedArray();
		ValueArray SweepedKeys();
		void Sweep();

		Index<Value> key;
		ValueArray   value;
		int          unlinked;
		
		Data() { unlinked = 0; }
	};
	struct NullData : Data {};
	Data *data;
	
	Data& Create();
	Data& Clone();
	
public:
	ValueMap();
	ValueMap(const ValueMap& v);
	~ValueMap();

	ValueMap& operator=(const ValueMap& v);

	operator Value() const;
	ValueMap(const Value& src);

	ValueMap(const Nuller&);
	bool IsNullInstance() const               { return data->IsNull(); }

	void Clear();

	void Put(const Value& key, const Value& value);
	void Put(Id id, const Value& value);
	void Remove(const Value& key);

	ValueArray GetKeys() const;
	ValueArray GetValues() const;

	operator ValueArray() const               { return GetValues(); }

	Value operator[](const Value& k) const;
	Value operator[](const Id& k) const       { return operator[](Value(k.ToString())); }

	unsigned GetHashValue() const             { return data->GetHashValue(); }
	void     Serialize(Stream& s);
	String   ToString() const                 { return data->AsString(); }

	bool operator==(const ValueMap& v) const;
	bool operator!=(const ValueMap& v) const  { return !operator==(v); }
};

inline bool IsValueMap(const Value& v) { return v.GetType() == VALUEMAP_V; }

#endif
