#ifndef _Gen_Gen_h
#define _Gen_Gen_h

#include <Core/Core.h>

using namespace Upp;

//some usefull interfaces

//the enable option
template<class B = EmptyClass>
class EnableOption
	: public B
{
public:
	typedef EnableOption<B> CLASSNAME;
	EnableOption() : enabled(true) {}
	virtual ~EnableOption() {}
	
	virtual void Enable(bool e) { enabled = e; }
	inline void Disable() { return Enable(false); }
	virtual bool IsEnabled() const { return enabled; }
	
protected:
	bool enabled;	
};

//notificator interface
template<class B = EmptyClass>
class Observable
	: public B
{
public:	
	typedef Observable<B> CLASSNAME;
	Observable() {}
	~Observable() {}

	Callback     operator<<=(Callback action)  { WhenAction = action; return action; }
	Callback&    operator<<(Callback action)   { return WhenAction << action; }

	void Action() { WhenAction(); }
	Callback WhenAction;
};

//get set data interface only with some helpers
template<class T, class B = EmptyClass>
class GetSetData
	: public B
{
public:
	typedef GetSetData<T,B> CLASSNAME;
	GetSetData() {}
	virtual ~GetSetData() {}

	virtual T    GetData() const               = 0;
	virtual void SetData(const T& _v)          = 0;

	T            operator~() const             { return GetData(); }
	const T&     operator<<=(const T& v)       { SetData(v); return v; }
};

template <class T, class B = EmptyClass>
class MinMaxOption
	: public B
{
public:
	typedef MinMaxOption<T,B> CLASSNAME;
	MinMaxOption() : nn(false) {}
	MinMaxOption(const T & min, const T& max) : min(min), max(max), nn(false) {}
	virtual ~MinMaxOption() {}

	MinMaxOption& Min(const T& _min)             { min = _min; return *this; }
	T GetMin() const { return min; }
	MinMaxOption& Max(const T& _max)             { max = _max; return *this; }
	T GetMax() const { return max; }
	MinMaxOption& NotNull(bool _nn = true)        { nn = _nn; return *this; }
	T IsNotNull() const { return nn; }
protected:
	T min, max;
	bool nn;
};

//extends the cloneable behaviour, needs to be implemented the hierarchy up
//this is only a first idea, maybe not use it yet, has got drawbacks
template<class T, class B = EmptyClass>
class Cloneable
//	: public B //every derive needs to use public Cloneable<OwnClassType>, so avoid multiple derives from EmptyClass
{
public:
	typedef Cloneable<T,B> CLASSNAME;
	Cloneable() {}
	virtual ~Cloneable() {}

	virtual T* Clone() const { return DeepCopyNew<T>(*(T*)this); }
	virtual T* PartialClone() const { return new T(); }
};

template<class T>
T* Clone(const T& c) { return c.Clone(); }

template<class T>
T* PartialClone(const T& c) { return c.PartialClone(); }

//declares a class Serializable for Stream
//maybe redundant because of global template for operator%(Stream%s, T&t)
//which calls t.Serialize()
template<class B = EmptyClass>
class Serializeable
	: public B
{
public:
	typedef Serializeable<B> CLASSNAME;
	Serializeable() {}
	virtual ~Serializeable() {}

	virtual void Serialize(Stream& s) = 0;
};

//declares a class Xmlizeable
//maybe redundant because of global template Xmlize(XmlIO xml, T&t)
//which calls t.Xmlize()
template<class B = EmptyClass>
class Xmlizeable
	: public B
{
public:
	typedef Xmlizeable<B> CLASSNAME;
	Xmlizeable() {}
	virtual ~Xmlizeable() {}

	virtual void Xmlize(XmlIO xml) = 0;
};

//a helper when using a class with XmlParser for DOM/SAX parsing
template<class B = EmptyClass>
class XmlParseable
	: public B
{
public:
	typedef XmlParseable<B> CLASSNAME;
	XmlParseable() {}
	virtual ~XmlParseable() {}

	virtual String ToXML() = 0;
	virtual void   LoadXML(XmlParser& p) = 0;
};

#endif
