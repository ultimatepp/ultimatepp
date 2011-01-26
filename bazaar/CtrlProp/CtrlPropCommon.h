#ifndef _CtrlProp_CtrlPropCommon_h
#define _CtrlProp_CtrlPropCommon_h

#include <CtrlCore/CtrlCore.h>
using namespace Upp;

enum
{
	PROPGET = 0,
	PROPSET = 1,
};

template<class C> struct GetTyper{
	typedef bool (*H)(const C&, Value&);
	typedef Gate2<const C&, Value&> Handler;
	static ArrayMap<String, Handler>& map() { static ArrayMap<String, Handler> _; return _; }
};
template<class C> struct SetTyper{
	typedef bool (*H)(C&, const Value&);
	typedef Gate2<C&, const Value&> Handler;
	static ArrayMap<String, Handler>& map() { static ArrayMap<String, Handler> _; return _; }
};

typedef Gate4<Ctrl&, const String&, Value&, int> ParamHandler;
VectorMap<String, ParamHandler>& pmap();
#define REGISTERPROPS(CTRL) pmap().GetAdd(String(typeid(CTRL).name())) = callback(&GetSetProp##CTRL)

void AddToValueArray(Value& v, const Vector<String>& k);
bool GetSetPropRecurseDone(Ctrl& c, const String& p, Value& v, int f);

#define DEC_CTRL_PROPERTIES(CTRL) \
bool GetSetProp##CTRL(Ctrl& c, const String& p, Value& v, int f) \


#define CTRL_PROPERTIES(CTRL, BASE) \
DEC_CTRL_PROPERTIES(CTRL); \
DEC_CTRL_PROPERTIES(BASE); \
INITBLOCK { REGISTERPROPS(CTRL); } \
DEC_CTRL_PROPERTIES(CTRL) \
{ \
	typedef CTRL _CTRL_; \
	if(0) { __base: return GetSetProp##BASE(c,p,v,f); } \
	ONCELOCK { \


#define END_CTRL_PROPERTIES \
	} \
	_CTRL_* pc = dynamic_cast<_CTRL_*>(&c); \
	if(!pc) return false; \
	_CTRL_& o = *pc; \
 \
	ArrayMap<String, SetTyper<_CTRL_>::Handler>& s = SetTyper<_CTRL_>::map(); \
	ArrayMap<String, GetTyper<_CTRL_>::Handler>& g = GetTyper<_CTRL_>::map(); \
	if(f&PROPSET) \
	{ \
		int i = s.Find(p); if(i>=0) return s[i](o, v); \
	} \
	else \
	{ \
		int i = g.Find(p); if(i>=0) return g[i](o, v); \
		if(p == "listset") AddToValueArray(v, s.GetKeys()); \
		else if(p == "listget") AddToValueArray(v, g.GetKeys()); \
	} \
	goto __base; \
} \


#define EXT_PROPERTY_SET(CTRL, name, fset) \
		SetTyper<CTRL>::map().GetAdd(name) = callback((SetTyper<CTRL>::H)&fset); \


#define EXT_PROPERTY_GET(CTRL, name, fget) \
		GetTyper<CTRL>::map().GetAdd(name) = callback((GetTyper<CTRL>::H)&fget); \


#define EXT_PROPERTY(CTRL, name, fset, fget) \
	EXT_PROPERTY_SET(CTRL, name, fset) \
	EXT_PROPERTY_GET(CTRL, name, fget) \


#define PROPERTY_SET(name, fset) \
	EXT_PROPERTY_SET(_CTRL_, name, fset) \


#define PROPERTY_GET(name, fset) \
	EXT_PROPERTY_GET(_CTRL_, name, fset) \


#define PROPERTY(name, fset, fget) \
	EXT_PROPERTY_SET(_CTRL_, name, fset) \
	EXT_PROPERTY_GET(_CTRL_, name, fget) \



#define EXT_UNPROPERTY_SET(CTRL, name) \
		SetTyper<CTRL>::map().RemoveKey(name); \

#define EXT_UNPROPERTY_GET(CTRL, name) \
		GetTyper<CTRL>::map().RemoveKey(name); \

#define EXT_UNPROPERTY(CTRL, name) \
	EXT_UNPROPERTY_SET(CTRL, name) \
	EXT_UNPROPERTY_GET(CTRL, name) \


#define UNPROPERTY_SET(name, fset) \
	EXT_UNPROPERTY_SET(_CTRL_, name, fset) \


#define UNPROPERTY_GET(name, fset) \
	EXT_UNPROPERTY_GET(_CTRL_, name, fset) \


#define UNPROPERTY(name, fset, fget) \
	EXT_UNPROPERTY_SET(_CTRL_, name, fset) \
	EXT_UNPROPERTY_GET(_CTRL_, name, fget) \


bool Property(Ctrl& c, const String& p, Value& v, int f);
inline bool SetProperty(Ctrl& c, const String& p, const Value& v) { return Property(c,p,const_cast<Value&>(v),PROPSET); }
inline bool GetProperty(const Ctrl& c, const String& p, Value& v) { return Property(const_cast<Ctrl&>(c),p,v,PROPGET); }

#endif

