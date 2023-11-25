class XmlIO;

template <class T>
void XmlAttrLoad(T& var, const String& text)
{
	var.XmlAttrLoad(text);
}

template <class T>
String XmlAttrStore(const T& var)
{
	return var.XmlAttrStore();
}

class XmlIO {
	XmlNode& node;
	bool     loading;
	Value    userdata;

public:
	bool IsLoading() const            { return loading; }
	bool IsStoring() const            { return !loading; }

	XmlNode& Node()                   { return node; }
	const XmlNode& Node() const       { return node; }

	XmlNode *operator->()             { return &node; }

	String GetAttr(const char *id)                    { return node.Attr(id); }
	void   SetAttr(const char *id, const String& val) { node.SetAttr(id, val); }

	template <class T> XmlIO operator()(const char *tag, T& var);
	template <class T> XmlIO List(const char *tag, const char *itemtag, T& var);
	template <class T, class X> XmlIO Var(const char *tag, T& var, X var_xmlize);
	template <class T, class X> XmlIO Array(const char *tag, T& var, X item_xmlize, const char *itemtag = "item");

	template <class T, class D> XmlIO operator()(const char *tag, T& var, const D& def);
	template <class T, class D> XmlIO List(const char *tag, const char *itemtag, T& var, const D& def);

	template <class T> XmlIO Attr(const char *id, T& var) {
		if(IsLoading())
			XmlAttrLoad(var, node.Attr(id));
		else
			node.SetAttr(id, XmlAttrStore(var));
		return *this;
	}

	template <class T, class D> XmlIO Attr(const char *id, T& var, const D& def) {
		if(IsLoading())
		    if(IsNull(node.Attr(id)))
				var = def;
		    else
				XmlAttrLoad(var, node.Attr(id));
		else
		if(var != def)
			node.SetAttr(id, XmlAttrStore(var));
		return *this;
	}

	XmlIO At(int i)                                    { XmlIO m(node.At(i), IsLoading(), userdata); return m; }
	XmlIO Add()                                        { XmlIO m(node.Add(), IsLoading(), userdata); return m; }
	XmlIO Add(const char *id)                          { XmlIO m(node.Add(id), IsLoading(), userdata); return m; }
	XmlIO GetAdd(const char *id)                       { XmlIO m(node.GetAdd(id), IsLoading(), userdata); return m; }

	void  SetUserData(const Value& v)                  { userdata = v; }
	Value GetUserData() const                          { return userdata; }

	XmlIO(XmlNode& xml, bool loading, const Value& userdata) : node(xml), loading(loading), userdata(userdata) {}
	XmlIO(XmlNode& xml, bool loading) : node(xml), loading(loading) {}
	XmlIO(XmlIO xml, const char *tag) : node(xml.node.GetAdd(tag)), loading(xml.loading), userdata(xml.userdata) {}
};

template<> inline void XmlAttrLoad(String& var, const String& text) { var = text; }
template<> inline String XmlAttrStore(const String& var)            { return var; }

template<> void XmlAttrLoad(WString& var, const String& text);
template<> String XmlAttrStore(const WString& var);
template<> void XmlAttrLoad(int& var, const String& text);
template<> String XmlAttrStore(const int& var);
template<> void XmlAttrLoad(dword& var, const String& text);
template<> String XmlAttrStore(const dword& var);
template<> void XmlAttrLoad(double& var, const String& text);
template<> String XmlAttrStore(const double& var);
template<> void XmlAttrLoad(float& var, const String& text);
template<> String XmlAttrStore(const float& var);
template<> void XmlAttrLoad(bool& var, const String& text);
template<> String XmlAttrStore(const bool& var);
template <> void XmlAttrLoad(int16& var, const String& text);
template <> String XmlAttrStore(const int16& var);
template <> void XmlAttrLoad(int64& var, const String& text);
template <> String XmlAttrStore(const int64& var);
template <> void XmlAttrLoad(byte& var, const String& text);
template <> String XmlAttrStore(const byte& var);
template <> void XmlAttrLoad(Date& var, const String& text);
template <> String XmlAttrStore(const Date& var);
template <> void XmlAttrLoad(Time& var, const String& text);
template <> String XmlAttrStore(const Time& var);

template<> void Xmlize(XmlIO& xml, String& var);
template<> void Xmlize(XmlIO& xml, WString& var);
template<> void Xmlize(XmlIO& xml, int& var);
template<> void Xmlize(XmlIO& xml, dword& var);
template<> void Xmlize(XmlIO& xml, double& var);
template<> void Xmlize(XmlIO& xml, float& var);
template<> void Xmlize(XmlIO& xml, bool& var);
template<> void Xmlize(XmlIO& xml, Date& var);
template<> void Xmlize(XmlIO& xml, Time& var);
template<> void Xmlize(XmlIO& xml, int16& var);
template<> void Xmlize(XmlIO& xml, int64& var);
template<> void Xmlize(XmlIO& xml, byte& var);

void XmlizeLangAttr(XmlIO& xml, int& lang, const char *id = "lang");
void XmlizeLang(XmlIO& xml, const char *tag, int& lang, const char *id = "id");

template <class T>
void Xmlize(XmlIO& xml, T& var)
{
	var.Xmlize(xml);
}

template <class T>
void Xmlize(XmlIO& xml, const char* itemtag, T& var)
{
	var.Xmlize(xml, itemtag);
}

template <class T, class X>
void XmlizeContainer(XmlIO& xml, const char *tag, T& data, X item_xmlize);

template<class T>
void XmlizeContainer(XmlIO& xml, const char *tag, T& data);

template<class K, class V, class T>
void XmlizeMap(XmlIO& xml, const char *keytag, const char *valuetag, T& data);

template<class K, class V, class T>
void XmlizeSortedMap(XmlIO& xml, const char *keytag, const char *valuetag, T& data);

template<class K, class T>
void XmlizeIndex(XmlIO& xml, const char *keytag, T& data);

template<class T>
void XmlizeStore(XmlIO& xml, const T& data)
{
	ASSERT(xml.IsStoring());
	Xmlize(xml, const_cast<T&>(data));
}

template <class T>
struct ParamHelper__ {
	T&   data;
	void Invoke(XmlIO xml) {
		Xmlize(xml, data);
	}

	ParamHelper__(T& data) : data(data) {}
};

String DoStoreAsXML(Event<XmlIO> xmlize, const char *name);
bool   DoLoadFromXML(Event<XmlIO> xmlize, const String& xml);
bool   DoTryLoadFromXML(Event<XmlIO> xmlize, const String& xml);

template <class T>
String StoreAsXML(const T& data, const char *name = NULL)
{
	ParamHelper__<T> p(const_cast<T &>(data));
	return DoStoreAsXML([&](XmlIO io) { Xmlize(io, const_cast<T &>(data)); }, name);
}

template <class T>
bool LoadFromXML(T& data, const String& xml)
{
	ParamHelper__<T> p(data);
	return DoLoadFromXML(callback(&p, &ParamHelper__<T>::Invoke), xml);
}

template <class T>
bool TryLoadFromXML(T& data, const String& xml)
{
	ParamHelper__<T> p(data);
	return DoTryLoadFromXML(callback(&p, &ParamHelper__<T>::Invoke), xml);
}

bool StoreAsXMLFile(Event<XmlIO> xmlize, const char *name = NULL, const char *file = NULL);
bool LoadFromXMLFile(Event<XmlIO> xmlize, const char *file = NULL);
bool TryLoadFromXMLFile(Event<XmlIO> xmlize, const char *file = NULL);

template <class T>
bool StoreAsXMLFile(T& data, const char *name = NULL, const char *file = NULL)
{
	ParamHelper__<T> p(data);
	return StoreAsXMLFile(callback(&p, &ParamHelper__<T>::Invoke), name, file);
}

template <class T>
bool LoadFromXMLFile(T& data, const char *file = NULL)
{
	ParamHelper__<T> p(data);
	return LoadFromXMLFile(callback(&p, &ParamHelper__<T>::Invoke), file);
}

template <class T>
bool TryLoadFromXMLFile(T& data, const char *file = NULL)
{
	ParamHelper__<T> p(data);
	return TryLoadFromXMLFile(callback(&p, &ParamHelper__<T>::Invoke), file);
}

template <class T>
void XmlizeBySerialize(XmlIO& xio, T& x)
{
	String h;
	if(xio.IsStoring())
		h = HexString(StoreAsString(x));
	xio.Attr("data", h);
	if(xio.IsLoading())
		try {
			LoadFromString(x, ScanHexString(h));
		}
		catch(LoadingError) {
			throw XmlError("xmlize by serialize error");
		}
}

void  StoreJsonValue(XmlIO& xio, const Value& v);
Value LoadJsonValue(const XmlNode& n);

template <class T>
void XmlizeByJsonize(XmlIO& xio, T& x)
{
	if(xio.IsStoring())
		StoreJsonValue(xio, StoreAsJsonValue(x));
	else {
		try {
			LoadFromJsonValue(x, LoadJsonValue(xio.Node()));
		}
		catch(JsonizeError e) {
			throw XmlError("xmlize by jsonize error: " + e);
		}
	}
}

#include "Xmlize.hpp"