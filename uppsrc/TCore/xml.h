#ifndef _webmap_i_xml_h_
#define _webmap_i_xml_h_

String        ToXml(const char *s);

class XmlTag;

class Xmls : public String, Moveable<Xmls> {
public:
	Xmls(const char *s) : String(s) {}
	Xmls(const String& s) : String(s) {}
	Xmls(const Nuller& = Null) {}

	bool  IsNullInstance() const     { return IsEmpty(); }

	Xmls& Cat()                      { return *this; }

	Xmls& Text(const char *s)        { String::Cat(ToXml(s)); return *this; }
	Xmls& Quote(const char *text)    { Cat('\"'); Cat(ToXml(text)); Cat('\"'); return *this; }

	Xmls& Attr(const char *attr);
	Xmls& Attr(const char *attr, const char *s);
	Xmls& Attr(const char *attr, String s);
	Xmls& Attr(const char *attr, int i);
	Xmls& Attr(const char *attr, double d);
	Xmls& Attr(const char *attr, Date date);
	Xmls& Attr(const char *attr, Value v);

	Xmls& Cat(const XmlTag& tag);
	Xmls& Cat(String s)              { String::Cat(s); return *this; }
	Xmls& Cat(const char *s)         { String::Cat(s); return *this; }
	Xmls& Cat(char c)                { String::Cat(c); return *this; }
	Xmls& Cat(const char *s, int n)  { String::Cat(s, n); return *this; }
};

class XmlTag : Moveable<XmlTag> {
public:
	XmlTag(const char *s);
	XmlTag() {}

	XmlTag&       Text(const char *s)                        { tag.Text(s); return *this; }
	XmlTag&       Quote(const char *s)                       { tag.Quote(s); return *this; }

	XmlTag&       Attr(const char *attr)                     { tag.Attr(attr); return *this; }
	XmlTag&       Attr(const char *attr, const char *v)      { tag.Attr(attr, v); return *this; }
	XmlTag&       Attr(const char *attr, String v)           { tag.Attr(attr, v); return *this; }
	XmlTag&       Attr(const char *attr, int v)              { tag.Attr(attr, v); return *this; }
	XmlTag&       Attr(const char *attr, double v)           { tag.Attr(attr, v); return *this; }
	XmlTag&       Attr(const char *attr, Date v)             { tag.Attr(attr, v); return *this; }
	XmlTag&       Attr(const char *attr, Value v)            { tag.Attr(attr, v); return *this; }

	void          Combine(const XmlTag& tag, bool linebreak);

	Xmls          ApplyTo(const char *s, bool lf = false) const;
	Xmls          ApplyTo(String s, bool lf = false) const   { return ApplyTo(~s, lf); }

	XmlTag        operator()() const                         { return *this; }
	Xmls          operator~() const                          { return ApplyTo(""); }
	operator      Xmls() const                               { return ApplyTo(""); }
	String        ToString() const                           { return ApplyTo(""); }

	const Xmls&   Tag() const                                { return tag; }
	Xmls&         Tag()                                      { return tag; }

	const String& End() const                                { return end; }
	String&       End()                                      { return end; }

	bool          IsEmpty() const                            { return tag.IsEmpty(); }

protected:
	Xmls          tag;
	String        end;
};

Xmls           Xml10(const char *encoding);
inline Xmls    Xml10(byte charset = CHARSET_DEFAULT)               { return Xml10(MIMECharsetName(charset)); }
Xmls           XmlComment(const char *text);

inline Xmls&   operator << (Xmls& s, const XmlTag& tag)            { return s.Cat(tag); }
inline Xmls&   operator << (Xmls& s, const Xmls& v)                { return s.Cat(v); }
inline Xmls&   operator << (Xmls& s, String v)                     { return s.Cat(v); }
inline Xmls&   operator << (Xmls& s, const char *v)                { return s.Cat(v); }

inline Xmls    operator + (const XmlTag& tag1, const XmlTag& tag2) { Xmls r(tag1); r << tag2; return r; }
inline Xmls    operator + (const String& s, const XmlTag& tag)     { Xmls r(s); r << tag; return r; }
inline Xmls    operator + (const XmlTag& tag, const String& s)     { Xmls r(tag); r << s; return r; }
inline Xmls    operator + (const char *s, const XmlTag& tag)       { Xmls r(s); r << tag; return r; }
inline Xmls    operator + (const XmlTag& tag, const char *s)       { Xmls r(tag); r << s; return r; }

inline XmlTag  operator /  (const XmlTag& t1, const XmlTag& t2)    { XmlTag r(t1); r.Combine(t2, false); return r; }
inline XmlTag& operator /= (XmlTag& tag, const XmlTag& s)          { tag.Combine(s, false); return tag; }

inline Xmls    operator / (const XmlTag& tag, String s)            { return tag.ApplyTo(s, false); }
inline Xmls    operator / (const XmlTag& tag, const char *s)       { return tag.ApplyTo(s, false); }

inline XmlTag  operator %  (const XmlTag& t1, const XmlTag& t2)    { XmlTag r(t1); r.Combine(t2, true); return r; }
inline XmlTag& operator %= (XmlTag& tag, const XmlTag& s)          { tag.Combine(s, true); return tag; }

inline Xmls    operator % (const XmlTag& tag, String s)            { return tag.ApplyTo(s, true); }
inline Xmls    operator % (const XmlTag& tag, const char *s)       { return tag.ApplyTo(s, true); }

#endif
