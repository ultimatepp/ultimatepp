#ifndef _src_dbase_dbRecord_h_
#define _src_dbase_dbRecord_h_

class dbRecord {
protected:
	VectorMap<String, Value> record;

public:
	void 				Reset()		 																					{ record.Clear(); return; }

	//void				Add(String name, Value value)		 										{ record.Add(name, value); }
	void				Add(const String& name, const Value &value)					{ record.Add(name, value); }
	int					GetCount()																					{ return record.GetCount(); }
	
	String					GetName(int field)															{ return record.GetKey(field); }
	Value&					GetValue(int field)															{ return record[field]; }
	const Value&		GetValue(int field) const												{ return record[field]; }
	Value&					GetValue(const String &field)										{ int q = record.Find(field); return GetValue(q); }
	VectorMap<String, Value> Get()																	{ return record; }
	
	Value&      	operator()(const String &field);

	const Value& 	operator[](int i) const							              { return record[i]; }
	Value&       	operator[](int i)                     						{ return record[i]; }

	void Set(VectorMap<String, Value> &v)														{ record = v; return; }
	void Set(const VectorMap<String, Value> &v)											{ record = v; return; }
	
	dbRecord()																											{ record.Clear(); }
	~dbRecord()																											{ Reset(); }
};

#endif
