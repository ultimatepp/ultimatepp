Value  ParseJSON(CParser& p);
Value  ParseJSON(const char *s);

inline String AsJSON(int i)             { return IsNull(i) ? "null" : AsString(i); }
inline String AsJSON(double n)          { return IsNull(n) ? "null" : AsString(n); }
inline String AsJSON(float f)           { return IsNull(f) ? "null" : AsString(f); }
inline String AsJSON(bool b)            { return b ? "true" : "false"; }
inline String AsJSON(const String& s)   { return AsCString(s, INT_MAX, NULL, ASCSTRING_JSON); }
inline String AsJSON(const WString& s)  { return AsCString(s.ToString(), INT_MAX, NULL, ASCSTRING_JSON); }
inline String AsJSON(const char *s)     { return AsCString(s, INT_MAX, NULL, ASCSTRING_JSON); }
String AsJSON(Time tm);
String AsJSON(Date dt);

String AsJSON(const Value& v, const String& indent, bool pretty);
String AsJSON(const Value& v, bool pretty = false);

class JsonArray;

class Json {
	String text;

public:
	Json& CatRaw(const char *key, const String& val);

	String ToString() const                                     { return "{" + text + "}"; }
	String operator~() const                                    { return ToString(); }
	operator String() const                                     { return ToString(); }

	operator bool() const                                       { return text.GetCount(); }

	Json& operator()(const char *key, const Value& value)       { return CatRaw(key, AsJSON(value)); }
	Json& operator()(const char *key, int i)                    { return CatRaw(key, AsJSON(i)); }
	Json& operator()(const char *key, double n)                 { return CatRaw(key, AsJSON(n)); }
	Json& operator()(const char *key, float f)                  { return CatRaw(key, AsJSON(f)); }
	Json& operator()(const char *key, bool b)                   { return CatRaw(key, AsJSON(b)); }
	Json& operator()(const char *key, Date d)                   { return CatRaw(key, AsJSON(d)); }
	Json& operator()(const char *key, Time t)                   { return CatRaw(key, AsJSON(t)); }
	Json& operator()(const char *key, const String& s)          { return CatRaw(key, AsJSON(s)); }
	Json& operator()(const char *key, const WString& s)         { return CatRaw(key, AsJSON(s)); }
	Json& operator()(const char *key, const char *s)            { return CatRaw(key, AsJSON(s)); }
	Json& operator()(const char *key, const Json& object)       { return CatRaw(key, ~object); }
	Json& operator()(const char *key, const JsonArray& array);

	Json() {}
	Json(const char *key, const Value& value)                   { CatRaw(key, AsJSON(value)); }
	Json(const char *key, int i)                                { CatRaw(key, AsJSON(i)); }
	Json(const char *key, double n)                             { CatRaw(key, AsJSON(n)); }
	Json(const char *key, float f)                              { CatRaw(key, AsJSON(f)); }
	Json(const char *key, bool b)                               { CatRaw(key, AsJSON(b)); }
	Json(const char *key, Date d)                               { CatRaw(key, AsJSON(d)); }
	Json(const char *key, Time t)                               { CatRaw(key, AsJSON(t)); }
	Json(const char *key, const String& s)                      { CatRaw(key, AsJSON(s)); }
	Json(const char *key, const WString& s)                     { CatRaw(key, AsJSON(s)); }
	Json(const char *key, const char *s)                        { CatRaw(key, AsJSON(s)); }
	Json(const char *key, const Json& object)                   { CatRaw(key, ~object); }
	Json(const char *key, const JsonArray& array)               { operator()(key, array); }
};

class JsonArray {
	String text;

public:
	JsonArray& CatRaw(const String& val);

	String ToString() const                                     { return "[" + text + "]"; }
	String operator~() const                                    { return ToString(); }
	operator String() const                                     { return ToString(); }

	operator bool() const                                       { return text.GetCount(); }

	JsonArray& operator<<(const Value& value)                   { return CatRaw(AsJSON(value)); }
	JsonArray& operator<<(int i)                                { return CatRaw(AsJSON(i)); }
	JsonArray& operator<<(double n)                             { return CatRaw(AsJSON(n)); }
	JsonArray& operator<<(float f)                              { return CatRaw(AsJSON(f)); }
	JsonArray& operator<<(bool b)                               { return CatRaw(AsJSON(b)); }
	JsonArray& operator<<(Date d)                               { return CatRaw(AsJSON(d)); }
	JsonArray& operator<<(Time t)                               { return CatRaw(AsJSON(t)); }
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

	Value        Get(const char *key)            { ASSERT(IsLoading()); return (*src)[key]; }
	void         Set(const char *key, const Value& v);

	void         Put(Value& v)                   { ASSERT(IsStoring()); if(map) v = *map; else v = tgt; }
	Value        GetResult() const               { ASSERT(IsStoring()); return map ? Value(*map) : tgt; }

	template <class T>
	JsonIO& operator()(const char *key, T& value);

	template <class T>
	JsonIO& operator()(const char *key, T& value, const T& defvalue);

	template <class T>
	JsonIO& List(const char *key, const char *, T& var) { return operator()(key, var); }

	template <class T, class X>
	JsonIO& Var(const char *key, T& value, X item_jsonize);

	template <class T, class X>
	JsonIO& Array(const char *key, T& value, X item_jsonize, const char * = NULL);

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

template <class T, class X>
JsonIO& JsonIO::Var(const char *key, T& value, X jsonize)
{
	if(IsLoading()) {
		const Value& v = (*src)[key];
		if(!v.IsVoid()) {
			JsonIO jio(v);
			jsonize(jio, value);
		}
	}
	else {
		ASSERT(tgt.IsVoid());
		if(!map)
			map.Create();
		JsonIO jio;
		jsonize(jio, value);
		if(jio.map)
			map->Add(key, *jio.map);
		else
			map->Add(key, jio.tgt);
	}
	return *this;
}


template <class T, class X>
void JsonizeArray(JsonIO& io, T& array, X item_jsonize)
{
	if(io.IsLoading()) {
		const Value& va = io.Get();
		array.SetCount(va.GetCount());
		for(int i = 0; i < va.GetCount(); i++) {
			JsonIO jio(va[i]);
			item_jsonize(jio, array[i]);
		}
	}
	else {
		Vector<Value> va;
		va.SetCount(array.GetCount());
		for(int i = 0; i < array.GetCount(); i++) {
			JsonIO jio;
			item_jsonize(jio, array[i]);
			jio.Put(va[i]);
		}
		io.Set(ValueArray(pick(va)));
	}
}

template <class T, class X> JsonIO& JsonIO::Array(const char *key, T& value, X item_jsonize, const char *)
{
	if(IsLoading()) {
		const Value& v = (*src)[key];
		if(!v.IsVoid()) {
			JsonIO jio(v);
			JsonizeArray(jio, value, item_jsonize);
		}
	}
	else {
		ASSERT(tgt.IsVoid());
		if(!map)
			map.Create();
		JsonIO jio;
		JsonizeArray(jio, value, item_jsonize);
		if(jio.map)
			map->Add(key, *jio.map);
		else
			map->Add(key, jio.tgt);
	}
	return *this;
}

template <class T>
JsonIO& JsonIO::operator()(const char *key, T& value, const T& defvalue)
{
	if(IsLoading()) {
		const Value& v = (*src)[key];
		if(v.IsVoid())
			value = defvalue;
		else {
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
String StoreAsJson(const T& var, bool pretty = false)
{
	return AsJSON(StoreAsJsonValue(var), pretty);
}

template <class T>
bool LoadFromJson(T& var, const char *json)
{
	try {
		Value jv = ParseJSON(json);
		if(jv.IsError())
			return false;
		LoadFromJsonValue(var, jv);
	}
	catch(ValueTypeError) {
		return false;
	}
	catch(JsonizeError) {
		return false;
	}
	return true;
}

String sJsonFile(const char *file);

template <class T>
bool StoreAsJsonFile(const T& var, const char *file = NULL, bool pretty = false)
{
	return SaveFile(sJsonFile(file), StoreAsJson(var, pretty));;
}

template <class T>
bool LoadFromJsonFile(T& var, const char *file = NULL)
{
	return LoadFromJson(var, LoadFile(sJsonFile(file)));
}

template<> void Jsonize(JsonIO& io, int& var);
template<> void Jsonize(JsonIO& io, byte& var);
template<> void Jsonize(JsonIO& io, int16& var);
template<> void Jsonize(JsonIO& io, int64& var);
template<> void Jsonize(JsonIO& io, double& var);
template<> void Jsonize(JsonIO& io, float& var);
template<> void Jsonize(JsonIO& io, bool& var);
template<> void Jsonize(JsonIO& io, String& var);
template<> void Jsonize(JsonIO& io, WString& var);
template<> void Jsonize(JsonIO& io, Date& var);
template<> void Jsonize(JsonIO& io, Time& var);

template <class T>
void JsonizeArray(JsonIO& io, T& array)
{
	JsonizeArray(io, array, [](JsonIO& io, ValueTypeOf<T>& item) { Jsonize(io, item); });
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
			map.Add(key, pick(value));
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
		io.Set(ValueArray(pick(va)));
	}
}

template <class T, class K, class V>
void JsonizeSortedMap(JsonIO& io, T& map, const char *keyid, const char *valueid)
{
	if(io.IsLoading()) {
		map.Clear();
		const Value& va = io.Get();
		for(int i = 0; i < va.GetCount(); i++) {
			K key;
			V value;
			LoadFromJsonValue(key, va[i][keyid]);
			LoadFromJsonValue(value, va[i][valueid]);
			map.Add(key, pick(value));
		}
	}
	else {
		Vector<Value> va;
		va.SetCount(map.GetCount());
		for(int i = 0; i < map.GetCount(); i++) {
			ValueMap item;
			item.Add(keyid, StoreAsJsonValue(map.GetKey(i)));
			item.Add(valueid, StoreAsJsonValue(map[i]));
			va[i] = item;
		}
		io.Set(ValueArray(pick(va)));
	}
}

template <class T, class K, class V>
void JsonizeStringMap(JsonIO& io, T& map)
{
	if(io.IsLoading()) {
		map.Clear();
		const ValueMap& va = io.Get();
		map.Reserve(va.GetCount());
		for(int i = 0; i < va.GetCount(); i++) {
			V value;
			String key = va.GetKey(i);
			LoadFromJsonValue(key, va.GetKey(i));
			LoadFromJsonValue(value, va.GetValue(i));
			map.Add(key, pick(value));
		}
	}
	else {
		Index<Value>  index;
		Vector<Value> values;
		index.Reserve(map.GetCount());
		values.Reserve(map.GetCount());
		for (int i=0; i<map.GetCount(); ++i)
		{
			index.Add(StoreAsJsonValue(map.GetKey(i)));
			values.Add(StoreAsJsonValue(map[i]));
		}
		ValueMap vm(pick(index), pick(values));
		io.Set(vm);
	}
}

template <class K, class V>
void StringMap(JsonIO& io, VectorMap<K, V>& map)
{
	JsonizeStringMap<VectorMap<K, V>, K, V>(io, map);
}

template <class K, class V>
void StringMap(JsonIO& io, ArrayMap<K, V>& map)
{
	JsonizeStringMap<ArrayMap<K, V>, K, V>(io, map);
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
			index.Add(pick(v));
		}
	}
	else {
		Vector<Value> va;
		for(int i = 0; i < index.GetCount(); i++)
			if(!index.IsUnlinked(i))
				va.Add(StoreAsJsonValue(index[i]));
		io.Set(ValueArray(pick(va)));
	}
}

template <class T>
void JsonizeBySerialize(JsonIO& jio, T& x)
{
	String h;
	if(jio.IsStoring())
	   h = HexString(StoreAsString(x));
	jio("data", h);
	if(jio.IsLoading())
		try {
			LoadFromString(x, ScanHexString(h));
		}
		catch(LoadingError) {
			throw JsonizeError("jsonize by serialize error");
		}
}

template <class IZE>
struct LambdaIzeVar {
	IZE& ize;

	void Jsonize(JsonIO& io) { ize(io); }
	void Xmlize(XmlIO& io) { ize(io); }

	LambdaIzeVar(IZE& ize) : ize(ize) {}
};

template <class IO, class IZE>
void LambdaIze(IO& io, const char *id, IZE ize)
{
	LambdaIzeVar<IZE> var(ize);
	io(id, var);
}