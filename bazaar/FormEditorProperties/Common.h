#ifndef FORM_EDITOR_PROPERTIES__COMMON_H
#define FORM_EDITOR_PROPERTIES__COMMON_H

#include <FormEditorCommon/FormEditorCommon.h>
#include <Esc/Esc.h>

///////////////////////////////////////////////////////////////////////////////////////////////
// For PropertyCaller automatic argument conversion
///////////////////////////////////////////////////////////////////////////////////////////////

class SProperty
{
public:
	SProperty(const Vector<Value>& args) { _args <<= args; }
	SProperty& operator=(const Value& v) { Return = v; return *this; }
	Value GetArg(int i) { if (i < 0 || i >= _args.GetCount()) return Value(); return _args[i]; }
	Value Return;
private:
	Vector<Value> _args;
};

template<typename P> void PropReturn(P p, SProperty& e) { }
template<> inline void PropReturn<EscValue>(EscValue p, SProperty& e) { e = p.ToString(); }
template<> inline void PropReturn<WString>(WString p, SProperty& e) { e = p.ToString(); }
template<> inline void PropReturn<String>(String p, SProperty& e) { e = p; }
template<> inline void PropReturn<double>(double p, SProperty& e) { e = p; }
template<> inline void PropReturn<dword>(dword p, SProperty& e) { e = (int)p; }
template<> inline void PropReturn<char*>(char* p, SProperty& e) { e = String(p); }
template<> inline void PropReturn<bool>(bool p, SProperty& e) { e = (int)p; }
template<> inline void PropReturn<long>(long p, SProperty& e) { e = (int)p; }
template<> inline void PropReturn<int>(int p, SProperty& e) { e = p; }
template<> inline void PropReturn<Size>(Size p, SProperty& e) { e = p; }
template<> inline void PropReturn<Rect>(Rect p, SProperty& e) { e = p; }
template<> inline void PropReturn<Point>(Point p, SProperty& e) { e = p; }
template<> inline void PropReturn<Color>(Color p, SProperty& e) { e = p; }

template<> inline void PropReturn<const EscValue&>(const EscValue& p, SProperty& e){ e = p.ToString();}
template<> inline void PropReturn<const WString&>(const WString& p, SProperty& e) { e = p.ToString(); }
template<> inline void PropReturn<const String&>(const String& p, SProperty& e) { e = p; }
template<> inline void PropReturn<const double&>(const double& p, SProperty& e) { e = p; }
template<> inline void PropReturn<const dword&>(const dword& p, SProperty& e) { e = (int)p; }
template<> inline void PropReturn<const char*>(const char* p, SProperty& e) { e = String(p); }
template<> inline void PropReturn<const bool&>(const bool& p, SProperty& e) { e = p; }
template<> inline void PropReturn<const long&>(const long& p, SProperty& e) { e = (int)p; }
template<> inline void PropReturn<const int&>(const int& p, SProperty& e) { e = p; }
template<> inline void PropReturn<const Size&>(const Size& p, SProperty& e) { e = p; }
template<> inline void PropReturn<const Rect&>(const Rect& p, SProperty& e) { e = p; }
template<> inline void PropReturn<const Point&>(const Point& p, SProperty& e) { e = p; }
template<> inline void PropReturn<const Color&>(const Color& p, SProperty& e) { e = p; }

class ValueConvert
{
private:
	String _Str;
	Value  _Val;

public:
	virtual ~ValueConvert() {}
	ValueConvert(const Value& v) { if (IsString(v)) _Str = v.ToString(); else _Val = v; }
	operator const char*() const { return (const char*)_Str; }
	String   ToString()    const { return _Str; }
	operator String()      const { return _Str; }
	operator Value()       const { return _Val; }
	operator dword()       const { return ScanInt(_Val.ToString()); }
	operator Color()       const { if (_Val.GetType() == COLOR_V) return _Val; return Color(); }
	operator Point()       const { if (_Val.GetType() == POINT_V) return _Val; return Point(); }
	operator Rect()        const { if (_Val.GetType() ==  RECT_V) return _Val; return  Rect(); }
	operator Size()        const { if (_Val.GetType() ==  SIZE_V) return _Val; return  Size(); }
	operator bool()        const { return ToBool(_Val); }
	operator int ()        const { return ScanInt(_Val.ToString()); }
	operator double()      const { return ScanDouble(_Val.ToString()); }
};

#endif
