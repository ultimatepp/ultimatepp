#ifndef _Gen_Gen_h
#define _Gen_Gen_h

#include <Core/Core.h>

NAMESPACE_UPP

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
	virtual ~TypeHook() {}
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

//maybe this should go to Stream.h
template<class T>
void SerializeStore(Stream& s, const T& x)
{
	ASSERT(s.IsStoring());
	s % const_cast<T&>(x);
}

//declares a class Xmlizeable
//maybe redundant because of global template Xmlize(XmlIO& xml, T&t)
//which calls t.Xmlize()
//but to ensure implementations
template<class B = EmptyClass>
class Xmlizeable : public B
{
public:
	virtual void Xmlize(XmlIO& xml)             = 0;
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
typedef VectorMap<Value, GlobalInstancerType> GlobalInstancerMapType;
GlobalInstancerMapType& GetGlobalInstancerMap();

//Instancer help class, for maps of Instancers. the static Map should
//somewhere be ONCELOCK initialized with the elements
//B is the common base class, T should be the derived class, to ensure
//proper pointer arithmetic
template<class B>
class Instancer
{
public:
	typedef B*(*Type)();
	typedef VectorMap<Value, Type> MapType;

	template<class T = B>
	class Typed
	{
	public:
		static B* GetInstance() { return new T(); }
		static Type GetInstancer() { return &GetInstance; }
		static GlobalInstancerType GetGlobalInstancer() { return (GlobalInstancerType)&GetInstance; }
	};

	static B* GetInstance() { return Typed<B>::GetInstance(); }
	static MapType& Map() { static MapType _; return _; }
};

template<class T> inline String TypeOfS(T* = 0) { return String(typeid(T).name()); }
template<class T> inline String TypeOf(T* t = 0) { return TypeOfS<T>(t); }

//in polymorph environment, type info is needed, i.e. when xmlizing/serializing elements
//to know later which one to instantiate..best used with Instancer
class Typer
{
	friend class TyperRegistrar;
public:
	String TypeOf() const { ASSERT(tf); return (*tf)(); } //no virtual, dont permit override
	Typer() : tf(NULL) {}
private:
	String (*tf)();
};

class TyperRegistrar
{
public:
	static void RegisterType(Typer& t, String (*tf)()) { t.tf = tf; }	
};

template<class T, class B = EmptyClass>
class TyperT : public B, public TyperRegistrar
{
public:
	TyperT() { TyperRegistrar::RegisterType((T&)(*this), &Invoke); }
protected:
	static inline String Invoke() { return TypeOfS<T>((T*)NULL); }
};

//to declare a class beeing partaker in Instanciating.
//T is the interface / class that should be acessible at base
template<class T>
class Instance : public TypeHook<T, Typer> {};

template <class T>
class Shared : Moveable< Shared<T> > {
	mutable T  *ptr;
	Atomic     *rfc;

	void Retain() const { ASSERT(rfc); AtomicInc(*rfc); }
	void Release()      { ASSERT(rfc); if(AtomicDec(*rfc) == 0) { Free(); delete rfc; rfc = NULL; } }

	void        Free()                     { if(ptr && ptr != (T*)1) delete ptr; }
	void        Chk() const                { ASSERT(ptr != (T*)1); }
	void        ChkP() const               { Chk(); ASSERT(ptr); }

public:
	void        Attach(T *data)            { Free(); ptr = data; }
	T          *Detach() pick_             { ChkP(); T *t = ptr; ptr = NULL; return t; }
	T          *operator-() pick_          { return Detach(); }
	void        Clear()                    { Free(); ptr = NULL; }

	void        operator=(T *data)         { Attach(data); }
	void        operator=(const Shared<T>& d){ Release(); ptr = d.ptr; rfc = d.rfc; Retain(); }
	void        operator=(pick_ One<T>& d) { Attach(d.Detach()); }

	const T    *operator->() const         { ChkP(); return ptr; }
	T          *operator->()               { ChkP(); return ptr; }
	const T    *operator~() const          { Chk(); return ptr; }
	T          *operator~()                { Chk(); return ptr; }
	const T&    operator*() const          { ChkP(); return *ptr; }
	T&          operator*()                { ChkP(); return *ptr; }

	template <class TT>
	TT&         Create()                   { TT *q = new TT; Attach(q); return *q; }
	T&          Create()                   { T *q = new T; Attach(q); return *q; }

	bool        IsEmpty() const            { Chk(); return !ptr; }

	operator bool() const                  { return ptr; }

	Shared()                               { ptr = NULL; rfc = new Atomic(1); }
	Shared(T *newt)                        { ptr = newt; rfc = new Atomic(1); }
	Shared(const Shared<T>& p)             { ptr = p.ptr; rfc = p.rfc; Retain(); }
	~Shared()                              { Release(); }

	Shared(pick_ One<T>& p)                { ptr = p.Detach(); rfc = new Atomic(1); }
	Shared(const One<T>& p, int)           { ptr = DeepCopyNew(*p); rfc = new Atomic(1); }

	template<class TT> friend class GenerateShared;
private:
	Shared(T* p, Atomic* r)                { ptr = p; rfc = r; Retain(); }
};

template<class T> 
class GenerateShared //can access the internals of any Shared<T>
{
public:
	template<class TT>
	static Shared<T> FromShared(const Shared<TT>& p)
	{
		return Shared<T>((T*)p.ptr, p.rfc);
	}
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
	do { ChkLink(*e, f); ++c; }
	while((e = e->GetNext()) != list);
	return --c;
}

END_UPP_NAMESPACE

#endif
