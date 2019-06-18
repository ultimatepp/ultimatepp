Value Scan(dword stdtype, const String& text, const Value& def = Null, bool *hastime = NULL);

inline const String&  Nvl(const String& a, const String& b)    { return IsNull(a) ? b : a; }
inline int            Nvl(int a, int b)                        { return IsNull(a) ? b : a; }
inline int64          Nvl(int64 a, int64 b)                    { return IsNull(a) ? b : a; }
inline double         Nvl(double a, double b)                  { return IsNull(a) ? b : a; }
inline Date           Nvl(Date a, Date b)                      { return IsNull(a) ? b : a; }
inline Time           Nvl(Time a, Time b)                      { return IsNull(a) ? b : a; }

inline int            Nvl(int a)                               { return Nvl(a, 0); }
inline int64          Nvl(int64 a)                             { return Nvl(a, (int64)0); }
inline double         Nvl(double a)                            { return Nvl(a, 0.0); }

template <class T>
inline T              Nvl(T a, T b, T c)                       { return Nvl(Nvl(a, b), c); }

template <class T>
inline T              Nvl(T a, T b, T c, T d)                  { return Nvl(Nvl(a, b), c, d); }

template <class T>
inline T              Nvl(T a, T b, T c, T d, T e)             { return Nvl(Nvl(a, b), c, d, e); }

int StdValueCompare(const Value& a, const Value& b, const LanguageInfo& f);
int StdValueCompare(const Value& a, const Value& b, int language);
int StdValueCompare(const Value& a, const Value& b);

int StdValueCompareDesc(const Value& a, const Value& b, const LanguageInfo& f);
int StdValueCompareDesc(const Value& a, const Value& b, int language);
int StdValueCompareDesc(const Value& a, const Value& b);

struct ValueOrder {
	virtual bool operator()(const Value& a, const Value& b) const = 0;
	virtual ~ValueOrder() {}
};

struct StdValueOrder : ValueOrder {
	int language;

	virtual bool operator()(const Value& a, const Value& b) const { return StdValueCompare(a, b, language) < 0; }

	StdValueOrder(int l = -1) : language(l) {}
};

struct FnValueOrder : ValueOrder {
	int (*fn)(const Value& a, const Value& b);

	virtual bool operator()(const Value& a, const Value& b) const { return (*fn)(a, b) < 0; }

	FnValueOrder(int (*fn)(const Value& a, const Value& b)) : fn(fn) {}
};

struct ValuePairOrder {
	virtual bool operator()(const Value& keya, const Value& valuea, const Value& keyb, const Value& valueb) const = 0;
	virtual ~ValuePairOrder() {}
};

struct StdValuePairOrder : ValuePairOrder {
	int language;

	virtual bool operator()(const Value& keya, const Value& valuea, const Value& keyb, const Value& valueb) const;

	StdValuePairOrder(int l = -1);
};

struct FnValuePairOrder : ValuePairOrder {
	int (*fn)(const Value& k1, const Value& v1, const Value& k2, const Value& v2);

	virtual bool operator()(const Value& keya, const Value& valuea, const Value& keyb, const Value& valueb) const;

	FnValuePairOrder(int (*fn)(const Value& k1, const Value& v1, const Value& k2, const Value& v2)) : fn(fn) {}
};

int CompareStrings(const Value& a, const Value& b, const LanguageInfo& f); // used by StdCompareValue

class Id : Moveable<Id> {
	String id;

public:
	const String&  ToString() const              { return id; }
	dword          GetHashValue() const          { return UPP::GetHashValue(id); }
	bool           IsNull() const                { return UPP::IsNull(id); }

	operator const String&() const               { return ToString(); }
	const String&  operator~() const             { return ToString(); }
	bool           operator==(const Id& b) const { return id == b.id; }
	bool           operator!=(const Id& b) const { return id != b.id; }

	operator bool() const                        { return id.GetCount(); }

	Id()                                         {}
	Id(const String& s)                          { id = s; }
	Id(const char *s)                            { id = s; }
};

struct RefManager {
	virtual int   GetType() = 0;
	virtual Value GetValue(const void *)            { return Null; }
	virtual bool  IsNull(const void *)              { return false; }
	virtual void  SetValue(void *, const Value& v)  { NEVER(); }
	virtual void  SetNull(void *)                   { NEVER(); }
	virtual ~RefManager() {}
};

template <class T>
struct StdRef : public RefManager {
	virtual void  SetValue(void *p, const Value& v) { *(T *) p = (T)v; }
	virtual Value GetValue(const void *p)           { return *(const T *) p; }
	virtual int   GetType()                         { return GetValueTypeNo<T>(); }
	virtual bool  IsNull(const void *p)             { return UPP::IsNull(*(T *) p); }
	virtual void  SetNull(void *p)                  { UPP::SetNull(*(T *)p); }
	virtual ~StdRef() {}
};

class Ref : Moveable<Ref> {
protected:
	RefManager *m;
	void       *ptr;
	struct      ValueRef;

public:
	dword    GetType() const           { return m ? m->GetType() : VOID_V; }
	bool     IsNull() const            { return m ? m->IsNull(ptr) : true; }

	void       *GetVoidPtr() const     { return ptr; }
	RefManager *GetManager() const     { return m; }

	template <class T>
	bool     Is() const                { return GetType() == GetValueTypeNo<T>(); } // VALUE_V!!!
	template <class T>
	T&       Get() const               { ASSERT(GetValueTypeNo<T>() == GetType()); return *(T *)GetVoidPtr(); }

	void     SetNull()                 { if(m) m->SetNull(ptr); }
	Value    GetValue() const          { return m ? m->GetValue(ptr) : Value(); }
	void     SetValue(const Value& v)  { ASSERT(m); m->SetValue(ptr, v); }

	operator Value() const             { return GetValue(); }
	Value    operator~() const         { return GetValue(); }
	Ref&     operator=(const Value& v) { SetValue(v); return *this; }

	Ref(String& s);
	Ref(WString& s);
	Ref(int& i);
	Ref(int64& i);
	Ref(double& d);
	Ref(bool& b);
	Ref(Date& d);
	Ref(Time& t);
	Ref(Value& v);
	Ref(void *_ptr, RefManager *_m)    { ptr = _ptr, m = _m; }
	Ref(const ValueTypeRef& r);
	Ref()                              { ptr = m = NULL; }
};

template <class T>
T& GetRef(Ref r, T *x = NULL) {
	ASSERT(GetValueTypeNo<T>() == r.GetType());
	return *(T *) r.GetVoidPtr();
}

inline String&  RefString(Ref f)  { return GetRef<String>(f); }
inline WString& RefWString(Ref f) { return GetRef<WString>(f); }
inline int&     RefInt(Ref f)     { return GetRef<int>(f); }
inline int64&   RefInt64(Ref f)   { return GetRef<int64>(f); }
inline double&  RefDouble(Ref f)  { return GetRef<double>(f); }
inline bool&    RefBool(Ref f)    { return GetRef<bool>(f); }
inline Date&    RefDate(Ref f)    { return GetRef<Date>(f); }
inline Time&    RefTime(Ref f)    { return GetRef<Time>(f); }
inline Value&   RefValue(Ref f)   { ASSERT(f.GetType() == VALUE_V);
                                    return *(Value *)f.GetVoidPtr(); }

template <class T>
Ref AsRef(T& x) {
	return Ref(&x, &Single< StdRef<T> >());
}

struct ValueTypeRef {
	RefManager *m;
	void       *ptr;
};

inline
Ref::Ref(const ValueTypeRef& r)
{
	ptr = r.ptr;
	m = r.m;
}

template <class T, dword type, class B>
ValueType<T, type, B>::operator ValueTypeRef()
{
	ValueTypeRef h;
	h.ptr = this;
	h.m = &Single< StdRef<T> >();
	return h;
}

#define E__Value(I)   Value p##I
#define E__Ref(I)     Ref p##I

// ---------------------- Value Array

class ValueArray : public ValueType<ValueArray, VALUEARRAY_V, Moveable<ValueArray> > {
	struct Data : Value::Void {
		virtual dword        GetType() const             { return VALUEARRAY_V; }
		virtual bool         IsNull() const;
		virtual void         Serialize(Stream& s);
		virtual void         Xmlize(XmlIO& xio);
		virtual void         Jsonize(JsonIO& jio);
		virtual unsigned     GetHashValue() const;
		virtual bool         IsEqual(const Value::Void *p);
		virtual String       AsString() const;
		virtual int          Compare(const Value::Void *p);

		int GetRefCount() const     { return refcount; }
		Vector<Value>& Clone();

		Vector<Value> data;
	};
	struct NullData : Data {};
	Data *data;
	
	static Vector<Value> VoidData;

	Vector<Value>& Create();
	Vector<Value>& Clone();
	void  Init0();

	friend Value::Void *ValueArrayDataCreate();
	friend class Value;
	friend class ValueMap;

public:
	ValueArray()                              { Init0(); }
	ValueArray(const ValueArray& v);
	ValueArray(ValueArray&& v);
	ValueArray(Vector<Value>&& values);
	explicit ValueArray(const Vector<Value>& values, int deep);
	ValueArray(std::initializer_list<Value> init)    { Init0(); for(const auto& i : init) { Add(i); }}
	~ValueArray();

	ValueArray& operator=(const ValueArray& v);
	ValueArray& operator=(Vector<Value>&& values)  { *this = ValueArray(pick(values)); return *this; }

	operator Value() const;
	ValueArray(const Value& src);

	ValueArray(const Nuller&)                 { Init0(); }
	bool IsNullInstance() const               { return IsEmpty(); }
	
	void Clear();
	void SetCount(int n);
	void SetCount(int n, const Value& v);
	int  GetCount() const                     { return data->data.GetCount(); }
	bool IsEmpty() const                      { return data->data.IsEmpty(); }

	void Add(const Value& v);
	ValueArray& operator<<(const Value& v)    { Add(v); return *this; }
	void Set(int i, const Value& v);
	const Value& Get(int i) const;
	Value GetAndClear(int i);
	const Vector<Value>& Get() const          { return data ? data->data : VoidData; }
	Vector<Value> Pick();

	void Remove(int i, int count = 1);
	void Remove(const Vector<int>& ii);
	void Insert(int i, const ValueArray& va);
	void Append(const ValueArray& va)         { Insert(GetCount(), va); }

	const Value& operator[](int i) const      { return Get(i); }
	
	Value& At(int i);

	unsigned GetHashValue() const             { return data->GetHashValue(); }
	void     Serialize(Stream& s);
	void     Jsonize(JsonIO& jio);
	void     Xmlize(XmlIO& xio);
	String   ToString() const;

	bool     operator==(const ValueArray& v) const;
	bool     operator!=(const ValueArray& v) const  { return !operator==(v); }
	
	int      Compare(const ValueArray& b) const;
	bool     operator<=(const ValueArray& x) const { return Compare(x) <= 0; }
	bool     operator>=(const ValueArray& x) const { return Compare(x) >= 0; }
	bool     operator<(const ValueArray& x) const  { return Compare(x) < 0; }
	bool     operator>(const ValueArray& x) const  { return Compare(x) > 0; }

	typedef ConstIteratorOf<Vector<Value>> const_iterator;

#ifdef DEPRECATED
	typedef Value          value_type;
	typedef const_iterator ConstIterator;
	typedef const Value&   const_reference;
	typedef int            size_type;
	typedef int            difference_type;
#endif

	const_iterator         begin() const                      { return Get().begin(); }
	const_iterator         end() const                        { return Get().end(); }
};

template<>
String AsString(const ValueArray& v);

class ValueMap : public ValueType<ValueMap, VALUEMAP_V, Moveable<ValueMap> >{
	struct Data : Value::Void {
		virtual dword      GetType() const             { return VALUEMAP_V; }
		virtual bool       IsNull() const;
		virtual void       Serialize(Stream& s);
		virtual void       Xmlize(XmlIO& xio);
		virtual void       Jsonize(JsonIO& jio);
		virtual unsigned   GetHashValue() const;
		virtual bool       IsEqual(const Value::Void *p);
		virtual String     AsString() const;
		virtual int        Compare(const Value::Void *p);

		const Value& Get(const Value& k) const {
			int q = key.Find(k);
			return q >= 0 ? value[q] : ErrorValue();
		}
		Value& GetAdd(const Value& k) {
			int i = key.Find(k);
			if(i < 0) {
				i = value.GetCount();
				key.Add(k);
			}
			return value.At(i);
		}
		Value& At(int i) {
			ASSERT(i < value.GetCount());
			return value.At(i);
		}


		Index<Value> key;
		ValueArray   value;
	};

	struct NullData : Data {};
	Data *data;

	Data& Create();
	static void Clone(Data *&ptr);
	force_inline
	static ValueMap::Data& UnShare(ValueMap::Data *&ptr) { if(ptr->GetRefCount() != 1) Clone(ptr); return *ptr; }
	Data& UnShare() { return UnShare(data); }
	void  Init0();
	void  FromArray(const ValueArray& va);

	friend Value::Void *ValueMapDataCreate();
	friend class Value;

public:
	ValueMap()                                      { Init0(); }
	ValueMap(const ValueMap& v);
	ValueMap(const ValueArray& va)                  { FromArray(va); }
	ValueMap(Index<Value>&& k, Vector<Value>&& v);
	ValueMap(VectorMap<Value, Value>&& m);
	ValueMap(const Index<Value>& k, const Vector<Value>& v, int deep);
	ValueMap(const VectorMap<Value, Value>& m, int deep);
	ValueMap(std::initializer_list<std::pair<Value, Value>> init) { Init0(); for(const auto& i : init) { Add(i.first, i.second); }}
	~ValueMap();

	ValueMap& operator=(const ValueMap& v);
	ValueMap& operator=(VectorMap<Value, Value>&& m) { *this = ValueMap(pick(m)); return *this; }

	operator Value() const;
	ValueMap(const Value& src);

	ValueMap(const Nuller&)                         { Init0(); }
	bool IsNullInstance() const                     { return IsEmpty(); }

	void Clear();
	int  GetCount() const                           { return data->value.GetCount(); }
	bool IsEmpty() const                            { return data->value.IsEmpty(); }
	const Value& GetKey(int i) const                { return data->key[i]; }
	const Value& GetValue(int i) const              { return data->value[i]; }
	int  Find(const Value& key) const               { return data ? data->key.Find(key) : -1; }
	int  FindNext(int ii) const                     { return data ? data->key.FindNext(ii) : -1; }

	void Add(const Value& key, const Value& value);
	void Add(const String& key, const Value& value) { Add(Value(key), value); }
	void Add(const char *key, const Value& value)   { Add(Value(key), value); }
	void Add(int key, const Value& value)           { Add(Value(key), value); }
	void Add(Id key, const Value& value)            { Add(Value(key.ToString()), value); }
	
	ValueMap& operator()(const Value& key, const Value& value)  { Add(key, value); return *this; }
	ValueMap& operator()(const String& key, const Value& value) { Add(Value(key), value); return *this; }
	ValueMap& operator()(const char *key, const Value& value)   { Add(Value(key), value); return *this; }
	ValueMap& operator()(int key, const Value& value)           { Add(Value(key), value); return *this; }
	ValueMap& operator()(Id key, const Value& value)            { Add(Value(key.ToString()), value); return *this; }

	void Set(const Value& key, const Value& value);
	void Set(const String& key, const Value& value) { Set(Value(key), value); }
	void Set(const char *key, const Value& value)   { Set(Value(key), value); }
	void Set(int key, const Value& value)           { Set(Value(key), value); }
	void Set(Id key, const Value& value)            { Set(Value(key.ToString()), value); }

	void SetAt(int i, const Value& v);
	void SetKey(int i, const Value& key);
	void SetKey(int i, const String& key)           { SetKey(i, Value(key)); }
	void SetKey(int i, const char* key)             { SetKey(i, Value(key)); }
	void SetKey(int i, int key)                     { SetKey(i, Value(key)); }
	void SetKey(int i, Id key)                      { SetKey(i, Value(key.ToString())); }

	int  RemoveKey(const Value& key);
	int  RemoveKey(const String& key)               { return RemoveKey(Value(key)); }
	int  RemoveKey(const char* key)                 { return RemoveKey(Value(key)); }
	int  RemoveKey(int key)                         { return RemoveKey(Value(key)); }
	int  RemoveKey(Id key)                          { return RemoveKey(Value(key.ToString())); }
	void Remove(int i);

	const Index<Value>& GetKeys() const             { return data->key; }
	ValueArray GetValues() const                    { return data->value; }

	operator ValueArray() const                     { return GetValues(); }
	
	VectorMap<Value, Value> Pick();

	const Value& operator[](const Value& key) const  { return data->Get(key); }
	const Value& operator[](const String& key) const { return operator[](Value(key)); }
	const Value& operator[](const char *key) const   { return operator[](Value(key)); }
	const Value& operator[](const int key) const     { return operator[](Value(key)); }
	const Value& operator[](const Id& key) const     { return operator[](Value(key.ToString())); }

	Value& GetAdd(const Value& key)                  { return UnShare().GetAdd(key); }
	Value& operator()(const Value& key)              { return GetAdd(key); }
	Value& operator()(const String& key)             { return operator()(Value(key)); }
	Value& operator()(const char *key)               { return operator()(Value(key)); }
	Value& operator()(const int key)                 { return operator()(Value(key)); }
	Value& operator()(const Id& key)                 { return operator()(Value(key.ToString())); }
	Value& At(int i)                                 { return UnShare().At(i); }
	
	Value GetAndClear(const Value& key);

	unsigned GetHashValue() const                   { return data->GetHashValue(); }
	void     Serialize(Stream& s);
	void     Jsonize(JsonIO& jio);
	void     Xmlize(XmlIO& xio);
	String   ToString() const                       { return data->AsString(); }

	bool operator==(const ValueMap& v) const;
	bool operator!=(const ValueMap& v) const        { return !operator==(v); }

	int      Compare(const ValueMap& b) const;
	bool     operator<=(const ValueMap& x) const    { return Compare(x) <= 0; }
	bool     operator>=(const ValueMap& x) const    { return Compare(x) >= 0; }
	bool     operator<(const ValueMap& x) const     { return Compare(x) < 0; }
	bool     operator>(const ValueMap& x) const     { return Compare(x) > 0; }

	bool     IsSame(const ValueMap& b) const;
};

class ValueGen {
public:
	virtual Value  Get() = 0;
	Value operator++()  { return Get(); }
	virtual ~ValueGen() {}
};


#include "Value.hpp"
#include "ValueUtil.hpp"