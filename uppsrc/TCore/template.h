NAMESPACE_UPP

/* TRC 2/12/2003: obsoleted by Fidler's callback1
template <class OBJECT_, class METHOD_, class A, class T>
struct Callback1MethodActionArg : public Callback1Action<A> {
	OBJECT_  *object;
	METHOD_   method;
	T         arg;
	void    Execute(A a) { (object->*method)(a, arg); }
	bool    IsEqual(const Callback1Action<A> *other) const {
		const Callback1MethodActionArg<OBJECT_, METHOD_, A, T> *q
		       = dynamic_cast<const Callback1MethodActionArg<OBJECT_, METHOD_, A, T> *>(other);
		return q && q->object == object && q->method == method && q->arg == arg;
	}
	unsigned GetHashValue() const {
		return (unsigned)object ^ brutal_cast<unsigned>(method) ^ ::GetHashValue(arg);
	}
	Callback1MethodActionArg(OBJECT_ *object, METHOD_ method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class Object, class R, class O, class A, class B, class T>
Callback1<A> callback2(Object *object, R (O::*method)(A, B), T arg) {
	return Callback1<A>(new Callback1MethodActionArg<Object, R (O::*)(A, B), A, T>
	                    (object, method, arg));
}

template <class Object, class R, class O, class A, class B, class T>
Callback1<A> callback2(const Object *object, R (O::*method)(A, B) const, T arg) {
	return Callback1<A>(new Callback1MethodActionArg<Object, R (O::*)(A, B) const, A, T>
	                    (object, method, arg));
}
*/

/*
template <class X, class A, class T>
struct Callback1ActionCallArg : public Callback1Action<A> {
	X         x;
	T         arg;
	void    Execute(A a) { x(a, arg); }
	bool    IsEqual(const Callback1Action<A> *other) const {
		const Callback1ActionCallArg<X, A, T> *q = dynamic_cast<const Callback1ActionCallArg<X, A, T> *>(other);
		return q && q->x == x && q->arg == arg;
	}
	unsigned GetHashValue() const {
		return ::GetHashValue(x) ^ ::GetHashValue(arg);
	}
	Callback1ActionCallArg(X x, T arg) : x(x), arg(arg) {}
};

template <class R, class A, class B, class T>
Callback1<A> callback2(R (*fn)(A, B), T arg) {
	return Callback1<A>(new Callback1ActionCallArg<R (*)(A, B), A, T>(fn, arg));
}
*/

//#define THISBACK2(m, a) callback2(this, &CLASSNAME::m, a)


template <class T>
void SerializeRaw(Stream& stream, T& object)
{
	stream.SerializeRaw((byte *)&object, sizeof(object));
}

template <class T>
void SerializeEnum(Stream& stream, T& object)
{
	int o = object;
	stream / o;
	object = T(o);
}

template <class T>
static bool SerializeIn(T& object, String data)
{
	StringStream stream(data);
	stream % object;
	return !stream.IsError();
}

template <class T>
static String SerializeOut(const T& object)
{
	StringStream stream;
	stream % const_cast<T&>(object);
	return stream;
}

template <class T>
class PtrValueRep : public RawValueRep<T *>
{
public:
	PtrValueRep(T *v) : RawValueRep<T *>(v)          {}

	virtual bool       IsNull() const                { return this->v == 0; }
	virtual unsigned   GetHashValue() const          { return (unsigned)this->v; }
	virtual bool       IsEqual(const Value::Void *p);
	virtual String     AsString()                    { return FormatIntHex((int)this->v); }

	static const RawValueRep<T *> *Cast(const Value::Void *p)
	{ ASSERT(dynamic_cast<const RawValueRep<T *> *>(p)); return (const RawValueRep<T *> *)p; }
};

template <class T>
bool PtrValueRep<T>::IsEqual(const Value::Void *p)
{
	const RawValueRep<T *> *cast = dynamic_cast<const RawValueRep<T *> *>(p);
	return cast && cast->Get() == this->v;
}

template <class T>
class PtrValue : public Value
{
protected:
	typedef PtrValueRep<T> Rep;

public:
	PtrValue(T *x) : Value(new Rep(x)) {}
	T        *Get(const Value& v)       { return UPP::IsNull(v) ? 0 : Rep::Cast(v.GetVoidPtr())->Get(); }
	static T *Extract(const Value& v)   { return UPP::IsNull(v) ? 0 : Rep::Cast(v.GetVoidPtr())->Get(); }
};

template <class T>
inline Value PtrToValue(T *p)           { return PtrValue<T>(p); }

#ifdef COMPILER_GCC
typedef std::type_info type_info;
#endif

template <class T, class I = int>
class TypeMap : Moveable< TypeMap<T, I> > {
public:
	One<T>                    CreateObject(I id) const;
	I                         QueryObject(const T *object) const;

	One<T>                    Copy(const One<T>& object) const;
	void                      Serialize(Stream& stream, One<T>& object) const;
	void                      Serialize(Stream& stream, Array<T>& array) const;
//	template<> friend Stream&        operator % (Stream& stream, One<T>& object) { TypeMap<T, I>::Serialize(stream, object); return stream; }

	One<T>                    SerializeIn(const String& string) const;
	String                    SerializeOut(const One<T>& object) const;

	typedef One<T>            (*NewFunc)();

	void                      Register(NewFunc fn, const type_info *ti, I id);

	template <class ST>
	struct NewType {
		static One<T> New() { return new ST; }
	};

	template <class ST>
	void                      RegisterType(I id) { Register(&NewType<ST>::New, &typeid(ST), id); }

public:
	Index<I>                  id_index;
	Vector< One<T> (*)()>     factories;
	Index<const type_info *, PtrHash> type_index;
//	static Index<I>&                 GetIDIndex()   { static Index<I> index; return index; }
//	static Vector<One<T> (*)()>&     GetFactories() { static Vector<One<T> (*)()> factories; return factories; }
//	static Index<const type_info *, PtrHash>& GetTypeIndex() { static Index<const type_info *, PtrHash> index; return index; }
};

template <class T, class I>
One<T> TypeMap<T, I>::CreateObject(I id) const
{
	if(IsNull(id))
		return 0;
	int f = id_index.Find(id);
	if(f < 0)
		return 0;
	return factories[f]();
}

template <class T, class I>
I TypeMap<T, I>::QueryObject(const T *object) const
{
	if(!object)
		return Null;
	int i = type_index.Find(&typeid(*object));
	return i >= 0 ? id_index[i] : I(Null);
}

template <class T, class I>
One<T> TypeMap<T, I>::Copy(const One<T>& object) const
{
	if(!object)
		return NULL;
	return this->SerializeIn(SerializeOut(object));
}

template <class T, class I>
One<T> TypeMap<T, I>::SerializeIn(const String& string) const
{
	One<T> object;
	StringStream strm(string);
	Serialize(strm, object);
	return object;
}

template <class T, class I>
String TypeMap<T, I>::SerializeOut(const One<T>& object) const
{
	StringStream ss;
	Serialize(ss, const_cast<One<T>&>(object));
	return ss;
}

template <class T, class I>
void TypeMap<T, I>::Serialize(Stream& stream, One<T>& object) const
{
	I ID;
	if(stream.IsStoring())
		ID = QueryObject(~object);
	stream / ID;
	if(stream.IsLoading()) {
		object = CreateObject(ID);
		if(object)
			stream % *object;
		else if(!IsNull(ID))
			stream.SetError();
	}
	else if(object)
		stream % *object;
}

template <class T, class I>
void TypeMap<T, I>::Serialize(Stream& stream, Array<T>& array) const
{
	int c = array.GetCount();
	stream % c;
	if(stream.IsStoring()) {
		for(int i = 0; i < c; i++) {
			I ID = QueryObject(&array[i]);
			stream / ID;
			stream % array[i];
		}
	}
	else {
		array.SetCount(c);
		for(int i = 0; i < c; i++) {
			I ID;
			stream / ID;
			One<T> object = CreateObject(ID);
			if(object) {
				array.Add(-object);
				stream % array.Top();
			}
			else {
				stream.SetError();
				return;
			}
		}
	}
}

template <class T, class I>
void TypeMap<T, I>::Register(NewFunc fn, const type_info *ti, I id)
{
	int tx = type_index.Find(ti);
	if(tx >= 0)
		ASSERT((id_index[tx] == id));
	else {
		ASSERT((id_index.Find(id) < 0));
		type_index.Add(ti);
		id_index.Add(id);
		factories.Add() = fn;
	}
}

template <class T>
void StreamOne(Stream& stream, One<T>& one)
{
	bool full = one;
	stream % full;
	if(stream.IsLoading())
		if(full)
			one = new T;
		else
			one = NULL;
	if(one)
		stream % *one;
}

template <class T>
One<T> StreamCopyOne(const One<T>& one)
{
	if(!one)
		return NULL;
	String aux = SerializeOut(*one);
	One<T> out = new T;
	SerializeIn(*out, aux);
	return out;
}

template <class B>
struct NewableType {
	typedef B *(*Factory)();

	typedef VectorMap<const type_info *, Factory, PtrHash> FactoryMap;

	static FactoryMap& Factories();

	static B          *New(const type_info& t); // throws on failure
	static B          *NewNull(const type_info& t); // may return 0
	static Factory     GetFactory(const type_info& t);
};

template <class B>
typename NewableType<B>::FactoryMap& NewableType<B>::Factories()
{
	static FactoryMap *pmap = 0;
	if(!pmap) // a nasty trick patching multiple construction compiler error
	{
		static FactoryMap map;
		pmap = &map;
	}
	return *pmap;
}

template <class B>
B *NewableType<B>::New(const type_info& t)
{
	B *ptr = NewNull(t);
	if(ptr == NULL)
		throw Exc(NFormat("Objekt '%s' není registrován.", t.name()));
	return ptr;
}

template <class B>
B *NewableType<B>::NewNull(const type_info& t)
{
	Factory factory = GetFactory(t);
	return factory ? factory() : 0;
}

template <class B>
typename NewableType<B>::Factory NewableType<B>::GetFactory(const type_info& t)
{
	return Factories().Get(&t, 0);
}

template <class B, class C>
struct RegisterNewableType : NewableType<B>
{
	RegisterNewableType() { typename NewableType<B>::Factory f = CreateObject; this->Factories().FindAdd(&typeid(C), f); }

private:
	static B *CreateObject() { return new C; }
};

//////////////////////////////////////////////////////////////////////
// RefMemStat: refcounted object memory statistics.

#ifdef flagREFDEBUG
#define REF_DEBUG
#endif//flagREFDEBUG

class RefMemStat : DeepCopyOption<RefMemStat> {
public:
	RefMemStat() {}
	RefMemStat(const RefMemStat& rms, int deep) : alloc_map(rms.alloc_map, 0) {}

	static RefMemStat&             App();

	void                           Add(const String& name, int allocindex);
	void                           Remove(const String& name, int allocindex);

	String                         Format(bool index = false) const;
	String                         FormatDelta(const RefMemStat& old, bool index = false) const;

private:
	VectorMap<String, Index<int> > alloc_map;
};

//////////////////////////////////////////////////////////////////////
// RefBase: base for refcounted objects.

/* TRC 2/12/2003: moved to Web/util
template <class T>
inline Value RefConToValue(RefCon<T> p)        { return StrongValue< RefCon<T> >(p); }

template <class T>
inline Value RefPtrToValue(RefPtr<T> p)        { return StrongValue< RefPtr<T> >(p); }
*/

class WeakBase : public Link<WeakBase>
{
public:
	virtual ~WeakBase() { DbgChk(); }
	virtual void Clear() = 0;

#ifdef _DEBUG
	void         DbgChk() const { Chk(); }
#else
	void         DbgChk() const {}
#endif

	void         Chk() const;
};

template <class T>
class UPP::RefCon;

template <class T>
class UPP::RefPtr;

class WeakRef;

template <class T>
class WeakCon : public WeakBase
{
	friend class WeakRef;
	friend class UPP::RefCon<T>;

public:
	WeakCon() : ptr(0) {}
	WeakCon(const WeakCon<T>& p)                         { p.Chk(); if(ptr = p.ptr) LinkAfter(ptr); Chk(); }
	WeakCon(const T *p)                                  { if(ptr = const_cast<T *>(p)) LinkAfter(ptr); Chk(); }
	WeakCon(const RefCon<T>& p)                          { if(ptr = const_cast<T *>(~p)) LinkAfter(ptr); Chk(); }
	~WeakCon()                                           { Chk(); }

	operator       RefCon<T> () const                    { return RefCon<T>(ptr); }

	WeakCon<T>&    operator = (const T *p)               { Chk(); Unlink(); if(ptr = const_cast<T *>(p)) LinkAfter(ptr); Chk(); return *this; }
	WeakCon<T>&    operator = (const WeakCon<T>& p)      { Chk(); Unlink(); if(ptr = p.ptr) LinkAfter(ptr); Chk(); return *this; }
	WeakCon<T>&    operator = (const RefCon<T>& p)       { Chk(); Unlink(); if(ptr = p.ptr) LinkAfter(ptr); Chk(); return *this; }

	virtual void   Clear()                               { Chk(); Unlink(); ptr = 0; }
	bool           IsEmpty() const                       { return ptr == NULL; }
	bool           operator ! () const                   { return IsEmpty(); }

	const T       *operator ~ () const                   { return ptr; }
	const T&       operator * () const                   { ASSERT(ptr); return *ptr; }
	const T       *operator -> () const                  { ASSERT(ptr); return ptr; }

	friend bool     operator == (const WeakCon<T>& a, const WeakCon<T>& b) { return a.ptr == b.ptr; }
	friend bool     operator != (const WeakCon<T>& a, const WeakCon<T>& b) { return a.ptr != b.ptr; }
	friend unsigned GetHashValue(const WeakCon<T>& a)                      { return UPP::GetHashValue(a.ptr); }

protected:
	T              *ptr;
};

template <class T>
class WeakPtr : public WeakCon<T>
{
public:
	WeakPtr() {}
	WeakPtr(const WeakPtr<T>& p) : WeakCon<T>(p)         {}
	WeakPtr(T *p) : WeakCon<T>(p)                        {}
	WeakPtr(const RefPtr<T>& p) : WeakCon<T>(p)          {}

	operator       RefPtr<T> () const                    { return RefPtr<T>(const_cast<T *>(this->ptr)); }

	WeakPtr<T>&    operator = (T *p)                     { this->Chk(); this->Unlink(); if(this->ptr = p) LinkAfter(this->ptr); this->Chk(); return *this; }
	WeakPtr<T>&    operator = (const WeakPtr<T>& p)      { this->Chk(); this->Unlink(); if(this->ptr = p.ptr) LinkAfter(this->ptr); this->Chk(); return *this; }
	WeakPtr<T>&    operator = (const RefPtr<T>& p)       { this->Chk(); this->Unlink(); if(this->ptr = ~p) LinkAfter(this->ptr); this->Chk(); return *this; }

	T             *operator ~ () const                   { return const_cast<T *>(this->ptr); }
	T&             operator * () const                   { ASSERT(this->ptr); return *const_cast<T *>(this->ptr); }
	T             *operator -> () const                  { ASSERT(this->ptr); return const_cast<T *>(this->ptr); }
};

class WeakRef : public Link<WeakBase>
{
public:
	WeakRef() : refcount(0) {}
	virtual ~WeakRef()                  { ASSERT(refcount == 0); while(GetNext() != GetPtr()) GetNext()->Clear(); }

	int             GetRefCount() const { return AtomicXAdd(refcount, 0); }
	void            AddRef() const      { if(this) AtomicInc(refcount); }
	void            Release() const     { if(this && !AtomicDec(refcount)) delete this; }

private:
	mutable Atomic  refcount;
};

template <class T>
class VirtualArrayData : public RefBase
{
public:
	virtual ~VirtualArrayData() {}

	virtual void     SetCount(int count) = 0;
	virtual void     SetCountR(int count) = 0;
	virtual int      GetCount() const = 0;
	virtual void     Set(int i, T t) = 0;
	virtual T        Get(int i) const = 0;
};

template <class T>
class VirtualArray : Moveable< VirtualArray<T> >
{
public:
	VirtualArray() {}
	VirtualArray(VirtualArrayData<T> *data) : data(data) {}

	bool             IsNullInstance() const    { return !data; }
	void             SetCount(int count)       { data->SetCount(count); }
	void             SetCountR(int count)      { data->SetCountR(count); }
	int              GetCount() const          { return data->GetCount(); }
	void             Set(int i, T t)           { data->Set(i, t); }
	T                Get(int i) const          { return data->Get(i); }

	T                operator [] (int i) const { ASSERT(i >= 0 && i < GetCount()); return Get(i); }
	void             Add(T t)                  { int c = GetCount(); SetCountR(c + 1); Set(c, t); }

	void             Clear()                   { data->SetCount(0); }
	bool             IsEmpty() const           { return data->GetCount() == 0; }

	typedef T        ValueType;

	class ConstIterator
	{
	protected:
		const VirtualArray<T> *cont;
		int                    ii;

	public:
		T              operator*() const       { return (*cont)[ii]; }
		T              operator[](int i) const { return (*cont)[ii + i]; }

		ConstIterator& operator++()           { ++ii; return *this; }
		ConstIterator& operator--()           { --ii; return *this; }
		ConstIterator  operator++(int)        { ConstIterator t = *this; ++ii; return t; }
		ConstIterator  operator--(int)        { ConstIterator t = *this; --ii; return t; }

		ConstIterator& operator+=(int d)      { ii += d; return *this; }
		ConstIterator& operator-=(int d)      { ii -= d; return *this; }

		ConstIterator  operator+(int d) const { return ConstIterator(cont, ii + d); }
		ConstIterator  operator-(int d) const { return ConstIterator(cont, ii - d); }

		int  operator-(const ConstIterator& b) const   { return ii - b.ii; }

		bool operator==(const ConstIterator& b) const  { return ii == b.ii; }
		bool operator!=(const ConstIterator& b) const  { return ii != b.ii; }
		bool operator<(const ConstIterator& b) const   { return ii < b.ii; }
		bool operator>(const ConstIterator& b) const   { return ii > b.ii; }
		bool operator<=(const ConstIterator& b) const  { return ii <= b.ii; }
		bool operator>=(const ConstIterator& b) const  { return ii >= b.ii; }

//		operator bool() const                          { return ii < 0; }

		ConstIterator()                                { cont = NULL; ii = -1; }
		ConstIterator(const VirtualArray<T> *_cont, int ii) : cont(_cont), ii(ii) {}
	};

	ConstIterator    Begin() const             { return ConstIterator(this, 0); }
	ConstIterator    End() const               { return ConstIterator(this, GetCount()); }
	ConstIterator    GetIter(int i) const      { ASSERT(i >= 0 && i <= GetCount()); return ConstIterator(this, i); }

private:
	RefPtr< VirtualArrayData<T> > data;
};

template <class VT, class ST, int SN = 1024>
class VirtualSegtorArray : public VirtualArrayData<VT>
{
public:
	virtual void   SetCount(int count)  { container.SetCount(count); }
	virtual void   SetCountR(int count) { container.SetCount(count); } //!! Fidler: co takhle Segtor::SetCountR?
	virtual int    GetCount() const     { return container.GetCount(); }

protected:
	Segtor<ST, SN> container;
};

template <class VT, class ST, int SN = 1024>
class VirtualStdSegtorArray : public VirtualSegtorArray<VT, ST, SN>
{
public:
	virtual void     Set(int i, VT t)   { this->container[i] = t; }
	virtual VT       Get(int i) const   { return this->container[i]; }
};

template <class T>
class VirtualValueArray : public VirtualStdSegtorArray<Value, T>
{
};

END_UPP_NAMESPACE
