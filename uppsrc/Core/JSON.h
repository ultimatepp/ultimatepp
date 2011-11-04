Value  ParseJSON(CParser& p);
Value  ParseJSON(const char *s);

String AsJSON(const Value& v, const String& indent, bool pretty = false);
String AsJSON(const Value& v, bool pretty = false);
