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
