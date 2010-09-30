#ifndef _Gen_Gen_h
#define _Gen_Gen_h

#include <Core/Core.h>

using namespace Upp;

//some usefull interfaces

//the enable option
template<class B = EmptyClass>
class EnableOption : public B
{
public:
	EnableOption() : enabled(true) {}
	virtual void Enable(bool e)                { enabled = e; }
	inline void Disable()                      { return Enable(false); }
	virtual bool IsEnabled() const             { return enabled; }
protected:
	bool enabled;	
};

//notificator interface
template<class B = EmptyClass>
class Observable : public B
{
public:	
	Callback  operator<<=(Callback action)     { WhenAction = action; return action; }
	Callback& operator<<(Callback action)      { return WhenAction << action; }
	void Action()                              { WhenAction(); }
public:
	Callback WhenAction;
};

//get set data interface only with some helpers
template<class T, class B = EmptyClass>
class GetSetData : public B
{
public:
	virtual ~GetSetData() {} //might be used as base interface
	virtual T    GetData() const               = 0;
	virtual void SetData(const T& _v)          = 0;
	virtual T& GetRef()                        = 0;  
	virtual const T& GetRef() const            = 0;  
	T            operator~() const             { return GetData(); }
	const T&     operator<<=(const T& v)       { SetData(v); return v; }
};

//get set data interface only with some helpers
template<class T, class B = EmptyClass>
class GetSetDataMulti : public B
{
public:
	virtual ~GetSetDataMulti() {} //might be used as base interface
	virtual T    GetDataMulti(int i) const     = 0;
	virtual void SetDataMulti(int i, const T& _v) = 0;
	virtual T& GetRefMulti(int i)              = 0;
	virtual const T& GetRefMulti(int i) const  = 0;
	T            operator~() const             { return GetDataMulti(0); }
	const T&     operator<<=(const T& v)       { SetDataMulti(0, v); return v; }
};

template <class T, class B = EmptyClass>
class MinMaxOption : public B
{
public:
	MinMaxOption() : nn(false) {}
	MinMaxOption(const T & min, const T& max) : min(min), max(max), nn(false) {}
	virtual MinMaxOption& SetMin(const T& _min){ min = _min; return *this; }
	inline MinMaxOption& Min(const T& _min)    { return SetMin(_min); }
	T GetMin() const                           { return min; }
	inline T Min() const                       { return GetMin(); }
	virtual MinMaxOption& SetMax(const T& _max){ max = _max; return *this; }
	inline MinMaxOption& Max(const T& _max)    { return SetMax(_max); }
	T GetMax() const                           { return max; }
	inline T Max() const                       { return GetMax(); }
	virtual MinMaxOption& NotNull(bool _nn = true) { nn = _nn; return *this; }
	T IsNotNull() const                        { return nn; }
protected:
	T min, max;
	bool nn;
};

//copyable interface, implementing the Copy function, used by PolyDeepCopyNew

template<class T, class B = EmptyClass>
class Copyable : public B
{
public:
	virtual ~Copyable() {}
	virtual T* Copy() const                    { return PartialCopy(); }
	virtual T* PartialCopy() const             = 0; //{ NEVER(); return NULL; }
};

#if 1

//copyable interface defining the common base class C, without implementing Copy

template<class C, class B = EmptyClass>
class CopyableC : public Copyable<CopyableC<C,B>, B>
{
public:
	virtual const C& GetC() const              = 0;
	virtual C& GetC()                          = 0;
	
	operator const C&() const                  { return GetC(); }
	operator C&()                              { return GetC(); }
};

//provides the implementation of Copy, which is used by PolyDeepNew
//and implements the base class accessors. 
//T is the derived type, i.e. EditInt, C is common base class, i.e. Ctrl
//forward another baseclass CB to extend CopyableC interface
template<class T, class B, class C, class CB = EmptyClass>
class PolyCopyableC : public PolyDeepCopyNew<T, B>, public CopyableC<C, CB>
{
public:
	//works
	//virtual PolyCopyableC* PartialCopy() const  { return new T(); }
	
	//doesnt work, signature differences somehow..T is not yet part of derive hiearchy at this point..
	//so compiler does not recognize it legitime override. thus it moves to derived
	//virtual T* PartialCopy() const  { return new T(); }

	using CopyableC<C,CB>::Copy;
	using CopyableC<C,CB>::PartialCopy;

	virtual const C& GetC() const              { return *this; }
	virtual C& GetC()                          { return *this; }
};
//USE:
//PolyCopyingC<EditInt, Ctrl> a;
//CopyableC<Ctrl>* p = a.Copy();
//p->GetC().SizePos();

//THIS ONE WORKS
//by providing additional information about the common base class
//with template specialization can be defined what is copied

template<class B, class C, class CB = EmptyClass>
class PolyCopyingC : public PolyCopyableC<PolyCopyingC<B,C,CB>,B,C,CB> 
{
public:
	virtual PolyCopyingC* PartialCopy() const  { return new PolyCopyingC(); }
};

#endif

//declares a class Serializable for Stream
//maybe redundant because of global template for operator%(Stream%s, T&t)
//which calls t.Serialize()
template<class B = EmptyClass>
class Serializeable : public B
{
public:
	virtual void Serialize(Stream& s)          = 0;
};

//declares a class Xmlizeable
//maybe redundant because of global template Xmlize(XmlIO xml, T&t)
//which calls t.Xmlize()
//but to ensure implementations
template<class B = EmptyClass>
class Xmlizeable : public B
{
public:
	virtual void Xmlize(XmlIO xml)             = 0;
};

//a helper when using a class with XmlParser for DOM/SAX parsing
template<class B = EmptyClass>
class XmlParseable : public B
{
public:
	virtual String ToXML()                     = 0;
	virtual void   LoadXML(XmlParser& p)       = 0;
};

#endif
