#include "Core.h"

NAMESPACE_UPP

template <class S, class Char, class F>
Vector<S> SplitGeneric(int maxcount, const F& delim, const Char *s, bool ignoreempty = true)
{
	Vector<S> r;
	r.Reserve(min(maxcount, 8));
	const Char *t = s;
	while(*t && r.GetCount() < maxcount) {
		const Char *q = delim(t);
		if(q) {
			if(!ignoreempty || t > s)
				r.Add().Set(s, int(t - s)); // This is faster than r.Add(String(s, t))...
			t = s = q;
		}
		else
			t++;
	}
	if((!ignoreempty || t > s) && r.GetCount() < maxcount)
		r.Add().Set(s, int(t - s));
	return r;
}

Vector<String> Split(int maxcount, const char *s, const char * (*text_filter)(const char *), bool ignoreempty)
{
	return SplitGeneric<String>(maxcount, text_filter, s, ignoreempty);
}

struct SplitDelimFilter__ {
	int (*filter)(int);
	const char *operator()(const char *s) const { return (*filter)((byte)*s) ? s + 1 : NULL; }
};

Vector<String> Split(int maxcount, const char *s, int (*filter)(int), bool ignoreempty)
{
	SplitDelimFilter__ delim;
	delim.filter = filter;
	return SplitGeneric<String>(maxcount, delim, s, ignoreempty);
}

struct SplitDelimChar__ {
	int chr;
	const char *operator()(const char *s) const { return *s == chr ? s + 1 : NULL; }
};

Vector<String> Split(int maxcount, const char *s, int chr, bool ignoreempty)
{
	SplitDelimChar__ delim;
	delim.chr = chr;
	return SplitGeneric<String>(maxcount, delim, s, ignoreempty);
}

struct SplitDelimText__ {
	const char *ds;
	int l;
	const char *operator()(const char *s) const { return strncmp(s, ds, l) == 0 ? s + l : NULL; }
};

Vector<String> Split(int maxcount, const char *s, const char *text, bool ignoreempty)
{
	SplitDelimText__ delim;
	delim.ds = text;
	delim.l = (int)strlen(text);
	return delim.l ? SplitGeneric<String>(maxcount, delim, s, ignoreempty) : Vector<String>();
}

Vector<String> Split(const char *s, const char * (*text_filter)(const char *), bool ignoreempty)
{
	return Split(INT_MAX, s, text_filter, ignoreempty);
}

Vector<String> Split(const char *s, int (*filter)(int), bool ignoreempty)
{
	return Split(INT_MAX, s, filter, ignoreempty);
}

Vector<String> Split(const char *s, int chr, bool ignoreempty)
{
	return Split(INT_MAX, s, chr, ignoreempty);
}

Vector<String> Split(const char *s, const char *text, bool ignoreempty)
{
	return Split(INT_MAX, s, text, ignoreempty);
}

Vector<WString> Split(int maxcount, const wchar *s, const wchar * (*text_filter)(const wchar *), bool ignoreempty)
{
	return SplitGeneric<WString>(maxcount, text_filter, s, ignoreempty);
}

struct SplitDelimWFilter__ {
	int (*filter)(int);
	const wchar *operator()(const wchar *s) const { return (*filter)((byte)*s) ? s + 1 : NULL; }
};

Vector<WString> Split(int maxcount, const wchar *s, int (*filter)(int), bool ignoreempty)
{
	SplitDelimWFilter__ delim;
	delim.filter = filter;
	return SplitGeneric<WString>(maxcount, delim, s, ignoreempty);
}

struct SplitDelimWChar__ {
	int chr;
	const wchar *operator()(const wchar *s) const { return *s == chr ? s + 1 : NULL; }
};

Vector<WString> Split(int maxcount, const wchar *s, int chr, bool ignoreempty)
{
	SplitDelimWChar__ delim;
	delim.chr = chr;
	return SplitGeneric<WString>(maxcount, delim, s, ignoreempty);
}

int w_strncmp(const wchar *s, const wchar *t, int n)
{
	while(*s && *t && n > 0) {
		int q = (int)(uint16)*s - (int)(uint16)*t;
		if(q)
			return q;
		s++;
		t++;
		n--;
	}
	return 0;
}

struct SplitDelimWText {
	const wchar *ds;
	int l;
	const wchar *operator()(const wchar *s) const { return w_strncmp(s, ds, l) == 0 ? s + l : NULL; }
};

Vector<WString> Split(int maxcount, const wchar *s, const wchar *text, bool ignoreempty)
{
	SplitDelimWText delim;
	delim.ds = text;
	delim.l = wstrlen(text);
	return delim.l ? SplitGeneric<WString>(maxcount, delim, s, ignoreempty) : Vector<WString>();
}

Vector<WString> Split(const wchar *s, int (*filter)(int), bool ignoreempty)
{
	return Split(INT_MAX, s, filter, ignoreempty);
}

Vector<WString> Split(const wchar *s, int chr, bool ignoreempty)
{
	return Split(INT_MAX, s, chr, ignoreempty);
}

Vector<WString> Split(const wchar *s, const wchar *text, bool ignoreempty)
{
	return Split(INT_MAX, s, text, ignoreempty);
}

Vector<WString> Split(const wchar *s, const wchar * (*text_filter)(const wchar *), bool ignoreempty)
{
	return SplitGeneric<WString>(INT_MAX, text_filter, s, ignoreempty);
}

template <class B, class T>
T Join_(const Vector<T>& im, const T& delim, bool ignoreempty) {
	B r;
	bool next = false;
	for(int i = 0; i < im.GetCount(); i++)
		if(!ignoreempty || im[i].GetCount()) {
			if(next)
				r.Cat(delim);
			r.Cat(im[i]);
			next = true;
		}
	return r;
}

String Join(const Vector<String>& im, const String& delim, bool ignoreempty) {
	return Join_<StringBuffer, String>(im, delim, ignoreempty);
}

WString Join(const Vector<WString>& im, const WString& delim, bool ignoreempty) {
	return Join_<WStringBuffer, WString>(im, delim, ignoreempty);
}

static void sMergeWith(String& dest, const char *delim, const String& s)
{
	if(s.GetLength()) {
		if(dest.GetCount())
			dest.Cat(delim);
		dest.Cat(s);
	}
}

//$-
#define E__TL(I)       typename COMBINE(T, I)
#define E__NFIf(I)     COMBINE(p, I) = r[I - 1]
#define E__NFValue(I)  String& COMBINE(p, I)
#define E__PI(I)       COMBINE(p, I)
#define E__NFSValue(I) const String& COMBINE(p, I)
#define E__Merge(I)    sMergeWith(dest, delim, COMBINE(p, I))

#define E__NFBody(I) \
bool SplitTo(const char *s, int chr, bool ignoreempty, __List##I(E__NFValue)) \
{ \
	Vector<String> r = Split(I, s, chr, ignoreempty); \
	if(r.GetCount() < I) return false; \
	__List##I(E__NFIf); \
	return true; \
} \
bool SplitTo(const char *s, int chr, __List##I(E__NFValue)) \
{ \
	return SplitTo(s, chr, true, __List##I(E__PI)); \
} \
bool SplitTo(const char *s, int (*filter)(int), bool ignoreempty, __List##I(E__NFValue)) \
{ \
	Vector<String> r = Split(I, s, filter, ignoreempty); \
	if(r.GetCount() < I) return false; \
	__List##I(E__NFIf); \
	return true; \
} \
bool SplitTo(const char *s, int (*filter)(int), __List##I(E__NFValue)) \
{ \
	return SplitTo(s, filter, true, __List##I(E__PI)); \
} \
bool SplitTo(const char *s, const char *text, bool ignoreempty, __List##I(E__NFValue)) \
{ \
	Vector<String> r = Split(I, s, text, ignoreempty); \
	if(r.GetCount() < I) return false; \
	__List##I(E__NFIf); \
	return true; \
} \
bool SplitTo(const char *s, const char *text, __List##I(E__NFValue)) \
{ \
	return SplitTo(s, text, true, __List##I(E__PI)); \
} \
String Merge(const char *delim, __List##I(E__NFSValue)) \
{ \
	String dest; \
	__List##I(E__Merge); \
	return dest; \
} \
void MergeWith(String& dest, const char *delim, __List##I(E__NFSValue)) \
{ \
	__List##I(E__Merge); \
} \

__Expand12(E__NFBody)

#undef E__TL
#undef E__NFIf
#undef E__NFValue
#undef E__NFSValue
#undef E__PI
#undef E__NFBody

static void sMergeWith(WString& dest, const wchar *delim, const WString& s)
{
	if(s.GetLength()) {
		if(dest.GetCount())
			dest.Cat(delim);
		dest.Cat(s);
	}
}

#define E__TL(I)       typename COMBINE(T, I)
#define E__NFIf(I)     COMBINE(p, I) = r[I - 1]
#define E__NFValue(I)  WString& COMBINE(p, I)
#define E__PI(I)       COMBINE(p, I)
#define E__NFSValue(I) const WString& COMBINE(p, I)
#define E__Merge(I)    sMergeWith(dest, delim, COMBINE(p, I))

#define E__NFBody(I) \
bool SplitTo(const wchar *s, int chr, bool ignoreempty, __List##I(E__NFValue)) \
{ \
	Vector<WString> r = Split(I, s, chr, ignoreempty); \
	if(r.GetCount() < I) return false; \
	__List##I(E__NFIf); \
	return true; \
} \
bool SplitTo(const wchar *s, int chr, __List##I(E__NFValue)) \
{ \
	return SplitTo(s, chr, true, __List##I(E__PI)); \
} \
bool SplitTo(const wchar *s, int (*filter)(int), bool ignoreempty, __List##I(E__NFValue)) \
{ \
	Vector<WString> r = Split(I, s, filter, ignoreempty); \
	if(r.GetCount() < I) return false; \
	__List##I(E__NFIf); \
	return true; \
} \
bool SplitTo(const wchar *s, int (*filter)(int), __List##I(E__NFValue)) \
{ \
	return SplitTo(s, filter, true, __List##I(E__PI)); \
} \
bool SplitTo(const wchar *s, const wchar *text, bool ignoreempty, __List##I(E__NFValue)) \
{ \
	Vector<WString> r = Split(I, s, text, ignoreempty); \
	if(r.GetCount() < I) return false; \
	__List##I(E__NFIf); \
	return true; \
} \
bool SplitTo(const wchar *s, const wchar *text, __List##I(E__NFValue)) \
{ \
	return SplitTo(s, text, true, __List##I(E__PI)); \
} \
WString Merge(const wchar *delim, __List##I(E__NFSValue)) \
{ \
	WString dest; \
	__List##I(E__Merge); \
	return dest; \
} \
void MergeWith(WString& dest, const wchar *delim, __List##I(E__NFSValue)) \
{ \
	__List##I(E__Merge); \
} \

__Expand12(E__NFBody)

#undef E__TL
#undef E__NFIf
#undef E__NFValue
#undef E__NFSValue
#undef E__PI
#undef E__NFBody

END_UPP_NAMESPACE
