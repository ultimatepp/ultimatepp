#ifndef _Gen_VTypes_h_
#define _Gen_VTypes_h_

#include <Core/Core.h>
#include <CtrlCore/CtrlCore.h>

NAMESPACE_UPP

const dword LOGPOS_V    = 97;
template<> inline dword ValueTypeNo(const Ctrl::LogPos*)    { return LOGPOS_V; }

template<> inline bool IsNull(const Ctrl::LogPos& l) { return l.x.IsEmpty() || l.y.IsEmpty(); }
template<> inline void SetNull(Ctrl::LogPos& l) { l.x = l.y = Ctrl::Logc(); }

inline Ctrl::LogPos   Nvl(Ctrl::LogPos a, Ctrl::LogPos b)             { return IsNull(a) ? b : a; }
inline Ctrl::LogPos   Nvl(Ctrl::LogPos a)                             { return Nvl(a, Ctrl::LogPos()); }

template<> inline unsigned ValueGetHashValue(const Ctrl::LogPos& l) {
	CombineHash ch;
	ch << l.x.GetAlign() << l.x.GetA() << l.x.GetB();
	ch << l.y.GetAlign() << l.y.GetA() << l.y.GetB();
	return ch;
}

inline bool operator==(const Value& v, const Ctrl::LogPos& x)   { return RichValue<Ctrl::LogPos>::Extract(v) == x; }
inline bool operator==(const Ctrl::LogPos& x, const Value& v)   { return RichValue<Ctrl::LogPos>::Extract(v) == x; }
inline bool operator!=(const Value& v, const Ctrl::LogPos& x)   { return RichValue<Ctrl::LogPos>::Extract(v) != x; }
inline bool operator!=(const Ctrl::LogPos& x, const Value& v)   { return RichValue<Ctrl::LogPos>::Extract(v) != x; }

template<> String AsString(const Ctrl::LogPos& pos);

template<> Stream& operator%(Stream& s, Ctrl::LogPos& pos);
template<> void Xmlize(XmlIO& xml, Ctrl::LogPos& pos);
template<> void Jsonize(JsonIO& jio, Ctrl::LogPos& pos);

END_UPP_NAMESPACE

#endif
