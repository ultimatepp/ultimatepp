#ifndef _src_dbase_dbRecordSet_h_
#define _src_dbase_dbRecordSet_h_

class dbRecordSet {
//class dbRecordSet : Moveable<dbRecordSet> {
	//Array<VectorMap<String, Value> > recordSet;
	VectorMap<int, VectorMap<String, Value> > recordSet;
	unsigned int ptr;
		
public:
	void				Reset();
	void				Add(int record, dbRecord &rec, Array<String> &orderField, Array<unsigned int> &ordStyle);

	bool				Next();
	bool				Previous();
	void				Rewind()																				{ ptr = 0; }
	void				Forward()																				{ ptr = recordSet.GetCount()-1; }
	int					GetIndex()																			{ return ptr; }
	void				SetIndex(int i)																	{ ptr = i; }	
	bool				GoTo(int record);
		
	String			GetName(int field)															{ return recordSet[ptr].GetKey(field); }
	Value&			GetValue(int field);
	Value				GetValue(const String &field);
	const Value	GetValue(const String &field) const;
	Value				GetValue(int record, const String &field);
	const Value	GetValue(int record, const String &field) const;
	Value&			GetValue(int record, int field); 
	
	int					GetKey()																				{ return recordSet.GetKey(ptr); }
	
	void				Serialize(Stream &s)														{ s % recordSet % ptr; return; }
	void				Fetch(const Value &v)														{ *this = ValueTo<dbRecordSet>(v); }
	
	Value     		operator()(const String &field)	              { return GetValue(field); }
	const Value		operator()(const String &field) const         { return GetValue(field); }
	Value					operator()(int record, const String &field)		{ return GetValue(record, field); }
	const Value		operator()(int record, const String &field)	const	{ return GetValue(record, field); }
	Value&				operator()(int record, int field)							{ return GetValue(record, field); }
	
	Value&       	operator[](int i)                     				{ return GetValue(i); }

	dbRecord		Get()																						{ dbRecord r; r.Set(recordSet[ptr]); return r; }

	unsigned	int	GetCount()																		{ return recordSet.GetCount(); }
	unsigned	int	GetWidth()																		{ return recordSet[0].GetCount(); }
	
	dbRecordSet();
	~dbRecordSet()																							{ Reset(); }
};

#endif
