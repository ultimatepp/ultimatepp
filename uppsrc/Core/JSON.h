Value  ParseJSON(CParser& p);
Value  ParseJSON(const char *s);

inline String AsJSON(int i)             { return IsNull(i) ? "null" : AsString(i); }
inline String AsJSON(double n)          { return IsNull(n) ? "null" : AsString(n); }
inline String AsJSON(bool b)            { return b ? "true" : "false"; }
inline String AsJSON(const String& s)   { return AsCString(s); }
inline String AsJSON(const WString& s)  { return AsCString(s.ToString()); }
inline String AsJSON(const char *s)     { return AsCString(s); }

String AsJSON(const Value& v, const String& indent, bool pretty);
String AsJSON(const Value& v, bool pretty = false);

class JsonArray;

class Json {
	String text;

	Json& CatRaw(const char *key, const String& val) {
		if(text.GetCount())
			text << ',';
		text << '\"' << key << "\":" << val;
		return *this;		
	}

public:
	String ToString() const                                     { return "{" + text + "}"; }
	String operator~() const                                    { return ToString(); }
	operator String() const                                     { return ToString(); }
	
	Json& operator()(const char *key, const Value& value)       { return CatRaw(key, AsJSON(value)); }
	Json& operator()(const char *key, int i)                    { return CatRaw(key, AsJSON(i)); }
	Json& operator()(const char *key, double n)                 { return CatRaw(key, AsJSON(n)); }
	Json& operator()(const char *key, bool b)                   { return CatRaw(key, AsJSON(b)); }
	Json& operator()(const char *key, const String& s)          { return CatRaw(key, AsJSON(s)); }
	Json& operator()(const char *key, const WString& s)         { return CatRaw(key, AsJSON(s)); }
	Json& operator()(const char *key, const char *s)            { return CatRaw(key, AsJSON(s)); }
	Json& operator()(const char *key, const Json& object)       { return CatRaw(key, ~object); }
	Json& operator()(const char *key, const JsonArray& array);
		
	Json() {}
	Json(const char *key, const Value& value)                   { CatRaw(key, AsJSON(value)); }
	Json(const char *key, int i)                                { CatRaw(key, AsJSON(i)); }
	Json(const char *key, double n)                             { CatRaw(key, AsJSON(n)); }
	Json(const char *key, bool b)                               { CatRaw(key, AsJSON(b)); }
	Json(const char *key, const String& s)                      { CatRaw(key, AsJSON(s)); }
	Json(const char *key, const WString& s)                     { CatRaw(key, AsJSON(s)); }
	Json(const char *key, const char *s)                        { CatRaw(key, AsJSON(s)); }
	Json(const char *key, const Json& object)                   { CatRaw(key, ~object); }
	Json(const char *key, const JsonArray& array)               { operator()(key, array); }
};

class JsonArray {
	String text;

	JsonArray& CatRaw(const String& val) {
		if(text.GetCount())
			text << ',';
		text << val;
		return *this;		
	}

public:
	String ToString() const                                     { return "[" + text + "]"; }
	String operator~() const                                    { return ToString(); }
	operator String() const                                     { return ToString(); }
	
	JsonArray& operator<<(const Value& value)                   { return CatRaw(AsJSON(value)); }
	JsonArray& operator<<(int i)                                { return CatRaw(AsJSON(i)); }
	JsonArray& operator<<(double n)                             { return CatRaw(AsJSON(n)); }
	JsonArray& operator<<(bool b)                               { return CatRaw(AsJSON(b)); }
	JsonArray& operator<<(const String& s)                      { return CatRaw(AsJSON(s)); }
	JsonArray& operator<<(const WString& s)                     { return CatRaw(AsJSON(s)); }
	JsonArray& operator<<(const char *s)                        { return CatRaw(AsJSON(s)); }
	JsonArray& operator<<(const Json& object)                   { return CatRaw(~object); }
	JsonArray& operator<<(const JsonArray& array)               { return CatRaw(~array); }
		
	JsonArray() {}
};

inline Json& Json::operator()(const char *key, const JsonArray& array)
{
	return CatRaw(key, array);
}

class JsonIO {
	const Value   *src;
	One<ValueMap>  map;
	Value          tgt;

public:
	bool IsLoading() const                       { return src; }
	bool IsStoring() const                       { return !src; }
	
	const Value& Get() const                     { ASSERT(IsLoading()); return *src; }
	void         Set(const Value& v)             { ASSERT(IsStoring() && !map); tgt = v; }
	void         Put(Value& v)                   { ASSERT(IsStoring()); if(map) v = *map; else v = tgt; }
	Value        GetResult() const               { ASSERT(IsStoring()); return map ? Value(*map) : tgt; }

	template <class T>
	JsonIO& operator()(const char *key, T& value);
	
	JsonIO(const Value& src) : src(&src)         {}
	JsonIO()                                     { src = NULL; }
};

struct JsonizeError : Exc {
	JsonizeError(const String& s) : Exc(s) {}
};

template <class T>
void Jsonize(JsonIO& io, T& var)
{
	var.Jsonize(io);
}

template <class T>
JsonIO& JsonIO::operator()(const char *key, T& value)
{
	if(IsLoading()) {
		const Value& v = (*src)[key];
		if(!v.IsVoid()) {
			JsonIO jio(v);
			Jsonize(jio, value);
		}
	}
	else {
		ASSERT(tgt.IsVoid());
		if(!map)
			map.Create();
		JsonIO jio;
		Jsonize(jio, value);
		if(jio.map)
			map->Add(key, *jio.map);
		else
			map->Add(key, jio.tgt);
	}
	return *this;
}

template <class T>
Value StoreAsJsonValue(const T& var)
{
	JsonIO io;
	Jsonize(io, const_cast<T&>(var));
	return io.GetResult();
}

template <class T>
void LoadFromJsonValue(T& var, const Value& x)
{
	JsonIO io(x);
	Jsonize(io, var);
}

template <class T>
String StoreAsJson(const T& var)
{
	return AsJSON(StoreAsJsonValue(var));
}

template <class T>
bool LoadFromJson(T& var, const char *json)
{
	try {
		LoadFromJsonValue(var, ParseJSON(json));
	}
	catch(CParser::Error) {
		return false;
	}
	catch(JsonizeError) {
		return false;
	}
	return true;
}

template<> void Jsonize(JsonIO& io, int& var);
template<> void Jsonize(JsonIO& io, int64& var);
template<> void Jsonize(JsonIO& io, double& var);
template<> void Jsonize(JsonIO& io, bool& var);
template<> void Jsonize(JsonIO& io, String& var);
template<> void Jsonize(JsonIO& io, WString& var);
template<> void Jsonize(JsonIO& io, Date& var);
template<> void Jsonize(JsonIO& io, Time& var);

template <class T, class V>
void JsonizeArray(JsonIO& io, T& array)
{
	if(io.IsLoading()) {
		const Value& va = io.Get();
		array.SetCount(va.GetCount());
		for(int i = 0; i < va.GetCount(); i++) {
			JsonIO jio(va[i]);
			Jsonize(jio, array[i]);
		}
	}
	else {
		Vector<Value> va;
		va.SetCount(array.GetCount());
		for(int i = 0; i < array.GetCount(); i++) {
			JsonIO jio;
			Jsonize(jio, array[i]);
			jio.Put(va[i]);
		}
		io.Set(ValueArray(va));
	}
}

template <class T>
void Jsonize(JsonIO& io, Vector<T>& var)
{
	JsonizeArray<Vector<T>, T>(io, var);
}

template <class T>
void Jsonize(JsonIO& io, Array<T>& var)
{
	JsonizeArray<Array<T>, T>(io, var);
}

template <class T, class K, class V>
void JsonizeMap(JsonIO& io, T& map, const char *keyid, const char *valueid)
{
	if(io.IsLoading()) {
		map.Clear();
		const Value& va = io.Get();
		map.Reserve(va.GetCount());
		for(int i = 0; i < va.GetCount(); i++) {
			K key;
			V value;
			LoadFromJsonValue(key, va[i][keyid]);
			LoadFromJsonValue(value, va[i][valueid]);	
			map.Add(key, value);
		}
	}
	else {
		Vector<Value> va;
		va.SetCount(map.GetCount());
		for(int i = 0; i < map.GetCount(); i++)
			if(!map.IsUnlinked(i)) {
				ValueMap item;
				item.Add(keyid, StoreAsJsonValue(map.GetKey(i)));
				item.Add(valueid, StoreAsJsonValue(map[i]));
				va[i] = item;
			}
		io.Set(ValueArray(va));
	}
}

template <class K, class V, class H>
void Jsonize(JsonIO& io, VectorMap<K, V, H>& map)
{
	JsonizeMap(io, map, "key", "value");
}

template <class K, class V, class H>
void Jsonize(JsonIO& io, ArrayMap<K, V, H>& map)
{
	JsonizeMap<ArrayMap<K, V, H>, K, V>(io, map, "key", "value");
}

template <class T, class V>
void JsonizeIndex(JsonIO& io, T& index)
{
	if(io.IsLoading()) {
		const Value& va = io.Get();
		index.Reserve(va.GetCount());
		for(int i = 0; i < va.GetCount(); i++) {
			V v;
			LoadFromJsonValue(v, va[i]);
			index.Add(v);
		}
	}
	else {
		Vector<Value> va;
		for(int i = 0; i < index.GetCount(); i++)
			if(!index.IsUnlinked(i))
				va.Add(StoreAsJsonValue(index[i]));
		io.Set(ValueArray(va));
	}
}

template <class T>
void Jsonize(JsonIO& io, Index<T>& var)
{
	JsonizeIndex<Index<T>, T>(io, var);
}

template <class T>
void Jsonize(JsonIO& io, ArrayIndex<T>& var)
{
	JsonizeIndex<ArrayIndex<T>, T>(io, var);
}
