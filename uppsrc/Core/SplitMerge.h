Vector<String> Split(int maxcount, const char *s, const char * (*text_filter)(const char *), bool ignoreempty = true);
Vector<String> Split(int maxcount, const char *s, int (*filter)(int), bool ignoreempty = true);
Vector<String> Split(int maxcount, const char *s, int chr, bool ignoreempty = true);
Vector<String> Split(int maxcount, const char *s, const char *text, bool ignoreempty = true);
Vector<String> Split(const char *s, const char * (*text_filter)(const char *), bool ignoreempty = true);
Vector<String> Split(const char *s, int (*filter)(int), bool ignoreempty = true);
Vector<String> Split(const char *s, int chr, bool ignoreempty = true);
Vector<String> Split(const char *s, const char *text, bool ignoreempty = true);

Vector<WString> Split(int maxcount, const wchar *s, const wchar * (*text_filter)(const wchar *), bool ignoreempty = true);
Vector<WString> Split(int maxcount, const wchar *s, int (*filter)(int), bool ignoreempty = true);
Vector<WString> Split(int maxcount, const wchar *s, int chr, bool ignoreempty = true);
Vector<WString> Split(int maxcount, const wchar *s, const wchar *text, bool ignoreempty = true);
Vector<WString> Split(const wchar *s, const wchar * (*text_filter)(const wchar *), bool ignoreempty = true);
Vector<WString> Split(const wchar *s, int (*filter)(int), bool ignoreempty = true);
Vector<WString> Split(const wchar *s, int chr, bool ignoreempty = true);
Vector<WString> Split(const wchar *s, const wchar *text, bool ignoreempty = true);

String  Join(const Vector<String>& im, const String& delim, bool ignoreempty = false);
WString Join(const Vector<WString>& im, const WString& delim, bool ignoreempty = false);

//$-
#define E__NFValue(I)  String& COMBINE(p, I)
#define E__NFSValue(I) const String& COMBINE(p, I)

#define E__NFBody(I) \
bool SplitTo(const char *s, int delim, bool ignoreempty, __List##I(E__NFValue)); \
bool SplitTo(const char *s, int delim, __List##I(E__NFValue)); \
bool SplitTo(const char *s, int (*filter)(int), bool ignoreempty, __List##I(E__NFValue)); \
bool SplitTo(const char *s, int (*filter)(int), __List##I(E__NFValue)); \
bool SplitTo(const char *s, const char *delim, bool ignoreempty, __List##I(E__NFValue)); \
bool SplitTo(const char *s, const char *delim, __List##I(E__NFValue)); \
String Merge(const char *delim, __List##I(E__NFSValue)); \
void MergeWith(String& dest, const char *delim, __List##I(E__NFSValue)); \

__Expand12(E__NFBody)

#undef E__NFBody
#undef E__NFValue
#undef E__NFSValue

#define E__NFValue(I)  WString& COMBINE(p, I)
#define E__NFSValue(I) const WString& COMBINE(p, I)

#define E__NFBody(I) \
bool SplitTo(const wchar *s, int delim, bool ignoreempty, __List##I(E__NFValue)); \
bool SplitTo(const wchar *s, int delim, __List##I(E__NFValue)); \
bool SplitTo(const wchar *s, int (*filter)(int), bool ignoreempty, __List##I(E__NFValue)); \
bool SplitTo(const wchar *s, int (*filter)(int), __List##I(E__NFValue)); \
bool SplitTo(const wchar *s, const char *delim, bool ignoreempty, __List##I(E__NFValue)); \
bool SplitTo(const wchar *s, const char *delim, __List##I(E__NFValue)); \
WString Merge(const wchar *delim, __List##I(E__NFSValue)); \
void MergeWith(WString& dest, const wchar *delim, __List##I(E__NFSValue)); \

__Expand12(E__NFBody)

#undef E__NFBody
#undef E__NFValue
#undef E__NFSValue
//$+

//$ bool   SplitTo(const char *s, int delim, bool ignoreempty, String& p1...);
//$ bool   SplitTo(const char *s, int delim, String& p1...);
//$ bool   SplitTo(const char *s, int (*filter)(int), String& p1...);
//$ bool   SplitTo(const char *s, int (*filter)(int), String& p1...);
//$ bool   SplitTo(const char *s, const char *delim, bool ignoreempty, String& p1...);
//$ bool   SplitTo(const char *s, const char *delim, String& p1...);

//$ bool   SplitTo(const wchar *s, int delim, bool ignoreempty, WString& p1...);
//$ bool   SplitTo(const wchar *s, int delim, WString& p1...);
//$ bool   SplitTo(const wchar *s, int (*filter)(int), WString& p1...);
//$ bool   SplitTo(const wchar *s, int (*filter)(int), WString& p1...);
//$ bool   SplitTo(const wchar *s, const wchar *delim, bool ignoreempty, WString& p1...);
//$ bool   SplitTo(const wchar *s, const wchar *delim, WString& p1...);

//$ String Merge(const char *delim, String& p1...);
//$ WString Merge(const wchar *delim, WString& p1...);

//$ void   MergeWith(String& dest, const char *delim, String& p1...);
//$ void   MergeWith(WString& dest, const wchar *delim, WString& p1...);
