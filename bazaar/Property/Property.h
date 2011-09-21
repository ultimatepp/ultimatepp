#ifndef _Property_Property_h
#define _Property_Property_h

#include <Core/Core.h>

NAMESPACE_UPP

template<class T>
struct Property
{
	typedef Callback1<const T&> S;
	typedef Callback1<T&> G;

	Property(const S& s, const G& g)
		: set(s), get(g) {}
	virtual ~Property() {}

	virtual const T& Set(const T& a) const { ASSERT(set); set(a); return a; }
	virtual T Get() const { ASSERT(get); T t; get(t); return t; }

	inline T operator= (const T& a) const { return Set(a); }
	inline operator T() const { return Get(); }

public:
	const S set;
	const G get;
};

typedef Property<Value> ValueProperty;

template<class T>
struct Accessor
{
	typedef Gate1<const T&> S;
	typedef Gate1<T&> G;

	Accessor(const S& s, const G& g)
		: set(s), get(g) {}
	virtual ~Accessor() {}

	virtual bool Set(const T& a) const { ASSERT(set); return set(a); }
	virtual bool Get(T& a) const { ASSERT(get); return get(a); }

	inline T operator= (const T& a) const { Set(a); return a; }
	inline operator T() const { T t; Get(t); return t; }

public:
	const S set;
	const G get;
};

typedef Accessor<Value> ValueAccessor;

///

/*
Accessor is used to offer set/get callbacks bound to an instance,
which is hidden inside the callbacks, providing instance independant calls.
they can be handled / grouped in a dictionary or passed to some algorithms, which dont care of instance.

PropKeeper<> is the interface used to declare hierarchy dependancies
and create an instance bound AccessorMap from the PropTyper registered Handlers.
corresponding PropTyper is set up with instance independant handlers in Init.
PropKeeper and PropTyper are all strongtyped.

to be able to setup an AccessorMap from base level, there is Props<BaseT>.
it uses the typeid to know 'where to start' to call into toplevel PropKeeper SetupAccessorMap
dynamic_cast is used to verify only. could be changed to ASSERT.

to be able to 'browse' the registered handlers from the Base class level,
typeid is used with Props<BaseT>. it uses another set of typename maps
to know the toplevel class name, 'from where to start' to look for the instance independant handlers.
it does not create or access any AccessorMap facilities.
*/

typedef ArrayMap<String, Accessor<Value> > AccessorMap;

template<class T>
class PropKeeper
{
public:
	static void Init();
	static void SetupAccessorMap(T& o, AccessorMap& am);
private:
	PropKeeper();
};

template<>
class PropKeeper<EmptyClass>
{
public:
	typedef EmptyClass C;

	static void Init() {}
	static void SetupAccessorMap(C& o, AccessorMap& am) {}	
};

template<class C> struct PropTyper 
{
	typedef bool (*SH)(const Value&, C&);
	typedef bool (*GH)(Value&, const C&);
	typedef Tuple2<SH, GH> TH;
	typedef ArrayMap<String, TH> HMap;
	static HMap& map2() { static HMap _; return _; }
};

//

#define DEC_PROPKEEPER(KLASS, BASE) \
template<> \
class PropKeeper<KLASS> : PropKeeper<BASE> \
{ \
public: \
	typedef PropKeeper<BASE> B; \
	typedef KLASS C; \
 \
	static void Init(); \
	static void SetupAccessorMap(C& o, AccessorMap& am); \
}; \


#define DEC_PROPERTIES(KLASS, BASE) \
DEC_PROPKEEPER(KLASS, BASE) \


#define PROPERTIES(KLASS, BASE) \
 \
void PropKeeper<KLASS>::SetupAccessorMap(C& o, AccessorMap& am) \
{ \
	PropKeeper<KLASS>::Init(); \
	const PropTyper<C>::HMap& m = PropTyper<C>::map2(); \
	for(int i = 0; i < m.GetCount(); i++) \
	{ \
		const PropTyper<C>::TH& t = m[i]; \
		am.Add(m.GetKey(i) \
			, new Accessor<Value>( \
				((t.a)?(callback1<const Value&, C&, C&>(t.a, o)):(Accessor<Value>::S::Empty())) \
				, ((t.b)?(callback1<Value&, const C&, const C&>(t.b, o)):(Accessor<Value>::G::Empty())) \
			) \
		); \
	} \
	B::SetupAccessorMap((B::C&)o, am); \
} \
 \
void PropKeeper<KLASS>::Init() \
{ \
	typedef KLASS _KLASS_; \
	ONCELOCK \
	{ \



#define EXT_PROPERTY(KLASS, name, fset, fget) \
{ \
	PropTyper<KLASS>::TH t = { &fset, &fget, }; \
	PropTyper<KLASS>::map2().Add(name, t); \
} \


#define EXT_PROPERTY_SET(KLASS, name, fset) \
{ \
	PropTyper<KLASS>::TH t = { &fset, NULL, }; \
	PropTyper<KLASS>::map2().Add(name, t); \
} \


#define EXT_PROPERTY_GET(KLASS, name, fget) \
{ \
	PropTyper<KLASS>::TH t = { NULL, &fget, }; \
	PropTyper<KLASS>::map2().Add(name, t); \
} \



#define END_PROPERTIES \
	B::Init(); \
	\
	} \
} \
 \

#define PROPERTY_SET(name, fset) \
	EXT_PROPERTY_SET(_KLASS_, name, fset) \


#define PROPERTY_GET(name, fset) \
	EXT_PROPERTY_GET(_KLASS_, name, fset) \


#define PROPERTY(name, fset, fget) \
	EXT_PROPERTY(_KLASS_, name, fset, fget) \



#define EXT_UNPROPERTY_SET(KLASS, name) \
	NEVER(); \


#define EXT_UNPROPERTY_GET(KLASS, name) \
	NEVER(); \


#define EXT_UNPROPERTY(KLASS, name) \
{ \
	PropTyper<KLASS>::map2().RemoveKey(name); \
} \


#define UNPROPERTY_SET(name) \
	EXT_UNPROPERTY_SET(_KLASS_, name) \


#define UNPROPERTY_GET(name) \
	EXT_UNPROPERTY_GET(_KLASS_, name) \


#define UNPROPERTY(name) \
	EXT_UNPROPERTY(_KLASS_, name) \


class GetAccessorMapI
{
public:
	virtual AccessorMap& GetAccessorMap() = 0;
};

template<class T, class C = GetAccessorMapI>
class WithAccessorMap : public C
{
public:
	WithAccessorMap() { PropKeeper<T>::SetupAccessorMap((T&)(*this), am); }
	virtual AccessorMap& GetAccessorMap() { return am; }
	AccessorMap am;
};

//

enum
{
	PROPSET = 1,
	PROPGET = 2,
};

template<class T>
struct Props
{
	typedef Gate2<T&, AccessorMap&> HandlerAM;
	static VectorMap<String, HandlerAM>& pmapam() { static VectorMap<String, HandlerAM> _; return _; }

	static bool SetupAccessorMap(T& c, AccessorMap& am)
	{
		int i = pmapam().Find(String(typeid(c).name()));
		if(i<0) i = pmapam().Find(String(typeid(T).name())); //fallback to base only
		if(i<0) return false;
		return pmapam()[i](c,am);
	}

	//

	typedef Gate4<T&, const String&, Value&, int> Handler;
	static VectorMap<String, Handler>& pmap() { static VectorMap<String, Handler> _; return _; }

	static bool GetSet(T& c, const String& p, Value& v, int f)
	{
		int i = pmap().Find(String(typeid(c).name()));
		if(i<0) i = pmap().Find(String(typeid(T).name()));
		if(i<0) return false;
		return pmap()[i](c,p,v,f);
	}
	static inline bool Set(T& c, const String& p, const Value& v) { return GetSet(c,p,const_cast<Value&>(v),PROPSET); }
	static inline bool Get(const T& c, const String& p, Value& v) { return GetSet(const_cast<T&>(c),p,v,PROPGET); }
};

void AddToValueArray(Value& v, const Vector<String>& k);

#define REGISTERSETAM(T, KLASS) Props<T>::pmapam().GetAdd(String(typeid(KLASS).name())) = callback(&SetupAccessorMap##KLASS)
#define REGISTERPROPS(T, KLASS) Props<T>::pmap().GetAdd(String(typeid(KLASS).name())) = callback(&GetSetProp##KLASS)

#define DEC_SETAM(T, KLASS) \
bool SetupAccessorMap##KLASS(T& c, AccessorMap& am) \


#define DEC_GETSETPROP(T, KLASS) \
bool GetSetProp##KLASS(T& c, const String& p, Value& v, int f) \


#define GETSETPROP_DUMMY(T, KLASS) \
DEC_GETSETPROP(T, KLASS) \
{ return false; } \


#define PROPS(T, KLASS, BASE) \
DEC_SETAM(T, KLASS); \
DEC_GETSETPROP(T, BASE); \
DEC_GETSETPROP(T, KLASS); \
INITBLOCK \
{ \
	RLOG("initblock: " << String(typeid(KLASS).name())); \
	REGISTERSETAM(T, KLASS); \
	REGISTERPROPS(T, KLASS); \
} \
DEC_SETAM(T, KLASS) \
{ \
	KLASS* pc = dynamic_cast<KLASS*>(&c); \
	if(!pc) return false; \
	KLASS& o = *pc; \
	PropKeeper<KLASS>::SetupAccessorMap(o, am); \
	return true; \
} \
DEC_GETSETPROP(T, KLASS) \
{ \
	KLASS* pc = dynamic_cast<KLASS*>(&c); \
	if(!pc) \
	{ \
__base:	return GetSetProp##BASE(c,p,v,f); \
	} \
	KLASS& o = *pc; \
	const PropTyper<KLASS>::HMap& m = PropTyper<KLASS>::map2(); \
	int i = m.Find(p); \
 \
	if(i<0) \
	{ \
		int f = 0; \
		if(p == "listset") f |= PROPSET; \
		else if(p == "listget") f |= PROPGET; \
		if(!f) goto __base; \
		Vector<String> vs; \
		for(int i = 0; i < m.GetCount(); i++) \
		{ \
			const PropTyper<KLASS>::TH& t = m[i]; \
			if((f & PROPSET) && (t.a)) vs << m.GetKey(i); \
			if((f & PROPGET) && (t.b)) vs << m.GetKey(i); \
		} \
		AddToValueArray(v, vs); \
		goto __base; \
	} \
	const PropTyper<KLASS>::TH& t = m[i]; \
	if((f & PROPSET) && (t.a)) return t.a(v, o); \
	else if((f & PROPGET) && (t.b)) return t.b(v, o); \
	goto __base; \
} \
 \


END_UPP_NAMESPACE

#endif

