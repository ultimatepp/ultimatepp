#ifndef _Gen_Gen_h
#define _Gen_Gen_h

#include <Core/Core.h>

NAMESPACE_UPP

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

//Min Max values
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

//copyable interface, implementing the Copy function, used i.e. by PolyDeepCopyNew
template<class T, class B = EmptyClass>
class Copyable : public B
{
public:
	virtual ~Copyable() {}
	virtual T* Copy() const                    { return PartialCopy(); }
	virtual T* PartialCopy() const             = 0;
};

//when exporting an iterface at base lavel which is implemented/derived at top
template<class T, class B = EmptyClass>
class TypeHook : public B
{
public:
	virtual const T& GetT() const              = 0;
	virtual T& GetT()                          = 0;
	
	operator const T&() const                  { return GetT(); }
	operator T&()                              { return GetT(); }
};

//copyable interface defining the common base class C, without implementing Copy
//makes possible to clone things and accessing some defined interface
template<class C, class B = EmptyClass>
class PolyCopyableHook : public PolyDeepCopyNew<PolyCopyableHook<C,B>, TypeHook<C, Copyable<PolyCopyableHook<C,B>, B> > >
{
};

// implements the base class accessors. 
//B is the derived type, i.e. EditInt, C is common base class, i.e. Ctrl
//forward another baseclass CB to extend CopyableC interface
template<class T, class B, class C, class CB = EmptyClass>
class PolyElement : public PolyDeepCopyNew<T, B>, public PolyCopyableHook<C, CB>
{
public:
	virtual const C& GetT() const              { return *this; }
	virtual C& GetT()                          { return *this; }
};

//by providing additional information about the common base class
//with template specialization can be defined what is copied
template<class B, class C, class CB = EmptyClass>
class PolyElementWrap : public PolyElement<PolyElementWrap<B,C,CB>,B,C,CB> 
{
public:
	virtual PolyElementWrap* PartialCopy() const { return new PolyElementWrap(); }
};

//declares a class Serializable for Stream
//maybe redundant because of global template for operator%(Stream& s, T& t)
//which calls t.Serialize()
//but to ensure implementation
template<class B = EmptyClass>
class Serializeable : public B
{
public:
	virtual void Serialize(Stream& s)          = 0;
};

template<class T>
void SerializeStore(Stream& s, const T& x)
{
	ASSERT(s.IsStoring());
	s % const_cast<T&>(x);
}

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

//most times it is a good idea to implement both
template<class B = EmptyClass>
class Persistable : public Xmlizeable<Serializeable<B> > {};

//a helper when using a class with XmlParser for DOM/SAX parsing
template<class B = EmptyClass>
class XmlParseable : public B
{
public:
	virtual String ToXML()                     = 0;
	virtual void   LoadXML(XmlParser& p)       = 0;
};

typedef void*(*GlobalInstancerType)();
VectorMap<Value, GlobalInstancerType>& GetGlobalInstancerMap();

//Instancer help class, for maps of Instancers. the static Map should
//somewhere be ONCELOCK initialized with the elements
//B is the common base class, T should be the derived class, to ensure
//proper pointer arithmetic
template<class B>
class Instancer
{
public:
	typedef B*(*InstancerType)();
	template<class T = B>
	class Typed
	{
	public:
		static B* GetInstance() { return new T(); }
		static InstancerType GetInstancer() { return &GetInstance; }
		static GlobalInstancerType GetGlobalInstancer() { return (GlobalInstancerType)&GetInstance; }
	};
	static VectorMap<Value, InstancerType>& Map() { static VectorMap<Value, InstancerType> map; return map; }
};

//in polymorph environment, type info is needed, i.e. when xmlizing/serializing elements
//to know later which one to instantiate..best used with Instancer
class Typer0
{
public:
	virtual ~Typer0() {}
	virtual String TypeOf() const = 0;
};

//ensures the access to Typer0 at base level
class Typer :  public virtual Typer0 
{
public:
//	virtual String TypeOf() const { return String(); } //makes ambiguity due to virtual
};

//topmost should derive from this, implementing the T::TypeOfS static
template<class T, class B = EmptyClass>
class TyperT : public B, public virtual Typer0
{
public:
	virtual String TypeOf() const { return T::TypeOfS(); }
};

//to declare a class beeing partaker in Instanciating.
//T is the interface / class that should be acessibl at base
template<class T>
class Instancing : public TypeHook<T, Typer> {};

//a visiting interface
template<class T, class B = EmptyClass>
class Visiting : public B
{
public:
	typedef Visiting<T,B> CLASSNAME;
	Visiting() : pt(NULL) {}

	virtual void Visit(T& t) { pt = &t; Reload(); }
	virtual void Reload() { }
	virtual void Clear() { pt = NULL; }

	bool IsVisiting() const { return pt; }
	bool IsEmpty() const { return !IsVisiting(); }
	
	T& Get() const { return *pt; }

protected:
	T* pt;
};

template<class T = double>
class Scaler
{
public:
	Scaler() : mn(0), mx(0) {}
	Scaler(const T& mn, const T& mx) : mn(mn), mx(mx) {}

	inline void Min(const T& t) { mn = t; }
	inline T Min() const { return mn; }
	inline void Max(const T& t) { mx = t; }
	inline T Max() const { return mx; }
	inline void MinMax(const T& _mn, const T& _mx) { mn = _mn; mx = _mx; }

	//scales local dimension value t to foreign dimensions d
	//returned in foreign dimension
	inline T To(const Scaler& d, const T& t) { return (t-mn)*(d.mx-d.mn)/(mx-mn)+d.mn; }
	//scales foreign dimension value t from foreign s to local dimension
	//return in local dimension
	inline T From(const Scaler& s, const T& t) { return (t-s.mn)*(mx-mn)/(s.mx-s.mn)+mn; }

	inline T operator() (const Scaler& s, const T& t) { return From(s, t); }
protected:
	T mn, mx;	
};

//some Link helpers

//returns count of linked elements
//not counting the reported l element, which most times is LinkOwner anyway.
template<class T>
int GetLinkCount(const T& l, bool f = true)
{
	int c = 0;
	const T *list = l.GetPtr(), *e = list;
	if(f) while((e = e->GetNext()) != list) ++c;
	else  while((e = e->GetPrev()) != list) ++c;
	return c;
}

template<class T>
inline void ChkLinkCount(const T& l) { ASSERT(GetLinkCount(l, true) == GetLinkCount(l, false)); }

template<class T>
inline void ChkLink(const T& l, bool f = true)
{
	if(f) ASSERT(l.GetNext()->GetPrev() == l.GetPtr());
	else  ASSERT(l.GetPrev()->GetNext() == l.GetPtr());
}

template<class T>
int ChkLinkDeep(const T& l, bool f = true)
{
	int c = 0;
	const T *list = l.GetPtr(), *e = list;
	do { ChkLink(*e); ++c; }
	while((e = e->GetNext()) != list);
	return --c;
}

END_UPP_NAMESPACE

#endif
