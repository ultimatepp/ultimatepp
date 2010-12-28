#ifndef _Iter_Iter_h
#define _Iter_Iter_h

#include <Core/Core.h>
using namespace Upp;

#include <Gen/Gen.h>

template<class T>
class ConstIter : public Copyable<ConstIter<T> >
{
public:
	virtual void Reset() = 0;
	virtual const T& Get() const = 0;
	virtual bool Next() = 0;

	const T& operator*()              { return Get(); }
	const T *operator->()             { return &Get(); }

	ConstIter& operator++()           { Next(); return *this; }
	//cant use postfix because need to 'clone' the iter from base class
	//and this can only happen on heap, cant return on stack
	//ConstIter  operator++(int)        { ConstIter t = *this; ++ii; return t; }
};

template<class T>
class Iter : public Copyable<Iter<T> >
{
public:
	virtual void Reset() = 0;
	virtual T& Get() const = 0;
	virtual bool Next() = 0;

	T& operator*()              { return Get(); }
	T *operator->()             { return &Get(); }

	Iter& operator++()          { Next(); return *this; }
	//cant use postfix because need to 'clone' the iter from base class
	//and this can only happen on heap, cant return on stack
	//Iter  operator++(int)       { Iter t = *this; ++ii; return t; }
};

template<class T, class V>
class ConstIterCont : public ConstIter<T>
{
	friend class IterCreator;
protected:
	const V* cont;
	int ii;

public:
	virtual void Reset() { ii = -1; }
	virtual const T& Get() const { return (*cont)[ii]; }
	virtual bool Next() { return (++ii < cont->GetCount()); }

	virtual ConstIterCont* Copy() const { return new ConstIterCont(*this); }
	virtual ConstIterCont* PartialCopy() const { ConstIterCont* t = Copy(); t->Reset(); return t; }
	
	ConstIterCont() : cont(NULL), ii(-1) {}
	ConstIterCont(const V& _cont) : cont(&_cont), ii(-1) {}
	ConstIterCont(const V& _cont, int ii) : cont(&_cont), ii(ii) {}
};

template<class T, class V>
class IterCont : public Iter<T>
{
	friend class IterCreator;
protected:
	V* cont;
	int ii;

public:
	virtual void Reset() { ii = -1; }
	virtual T& Get() const { return (*cont)[ii]; }
	virtual bool Next() { return (++ii < cont->GetCount()); }

	virtual IterCont* Copy() const { return new IterCont(*this); }
	virtual IterCont* PartialCopy() const { IterCont* t = Copy(); t->Reset(); return t; }
	
	IterCont() : cont(NULL), ii(-1) {}
	IterCont(V& _cont) : cont(&_cont), ii(-1) {}
	IterCont(V& _cont, int ii) : cont(&_cont), ii(ii) {}
};

#define ITER_CONT(Cont) \
	template<class T> inline static Iter<T>* GetIter(Cont& cont) { return new IterCont<T,Cont >(cont); } \
	template<class T> inline static Iter<T>* GetIter(Cont& cont, int ii) { return new IterCont<T,Cont >(cont, ii); } \
	template<class T> inline static ConstIter<T>* GetIter(const Cont& cont) { return new ConstIterCont<T,Cont >(cont); } \
	template<class T> inline static ConstIter<T>* GetIter(const Cont& cont, int ii) { return new ConstIterCont<T,Cont >(cont, ii); } \


template<class T, class V>
class ConstIterPtr : public ConstIter<T>
{
	friend class IterCreator;
protected:
	const V* cont;
	int ii;

public:
	virtual void Reset() { ii = -1; }
	virtual const T& Get() const { ASSERT(ii==0); return *(*cont); }
	virtual bool Next() { return (++ii < 1); }

	virtual ConstIterPtr* Copy() const { return new ConstIterPtr(*this); }
	virtual ConstIterPtr* PartialCopy() const { ConstIterPtr* t = Copy(); t->Reset(); return t; }
	
	ConstIterPtr() : cont(NULL), ii(-1) {}
	ConstIterPtr(const V& _cont) : cont(&_cont), ii(-1) {}
	ConstIterPtr(const V& _cont, int ii) : cont(&_cont), ii(ii) {}
};

template<class T, class V>
class IterPtr : public Iter<T>
{
	friend class IterCreator;
protected:
	V* cont;
	int ii;

public:
	virtual void Reset() { ii = -1; }
	virtual T& Get() const { ASSERT(ii==0); return *(*cont); }
	virtual bool Next() { return (++ii < 1); }

	virtual IterPtr* Copy() const { return new IterPtr(*this); }
	virtual IterPtr* PartialCopy() const { IterPtr* t = Copy(); t->Reset(); return t; }
	
	IterPtr() : cont(NULL), ii(-1) {}
	IterPtr(V& _cont) : cont(&_cont), ii(-1) {}
	IterPtr(V& _cont, int ii) : cont(&_cont), ii(ii) {}
};

#define ITER_PTR(Cont) \
	template<class T> inline static Iter<T>* GetIter(Cont& cont) { return new IterPtr<T,Cont >(cont); } \
	template<class T> inline static ConstIter<T>* GetIter(const Cont& cont) { return new ConstIterPtr<T,Cont >(cont); } \


template<class T>
class ConstIterAny : public ConstIter<T>
{
	friend class IterCreator;
protected:
	const Any* cont;
	int ii;

public:
	virtual void Reset() { ii = -1; }
	virtual const T& Get() const { ASSERT(ii==0); ASSERT(cont->Is<T>()); return cont->Get<T>(); }
	virtual bool Next() { return (++ii < 1); }

	virtual ConstIterAny* Copy() const { return new ConstIterAny(*this); }
	virtual ConstIterAny* PartialCopy() const { ConstIterAny* t = Copy(); t->Reset(); return t; }
	
	ConstIterAny() : cont(NULL), ii(-1) {}
	ConstIterAny(const Any& _cont) : cont(&_cont), ii(-1) {}
	ConstIterAny(const Any& _cont, int ii) : cont(&_cont), ii(ii) {}
};

template<class T>
class IterAny : public Iter<T>
{
	friend class IterCreator;
protected:
	Any *cont;
	int ii;

public:
	virtual void Reset() { ii = -1; }
	virtual T& Get() const { ASSERT(ii==0); ASSERT(cont->Is<T>()); return cont->Get<T>(); }
	virtual bool Next() { return (++ii < 1); }

	virtual IterAny* Copy() const { return new IterAny(*this); }
	virtual IterAny* PartialCopy() const { IterAny* t = Copy(); t->Reset(); return t; }
	
	IterAny() : cont(NULL), ii(-1) {}
	IterAny(Any& _cont) : cont(&_cont), ii(-1) {}
	IterAny(Any& _cont, int ii) : cont(&_cont), ii(ii) {}
};

template<class T>
class ConstIterValue : public ConstIter<T>
{
	friend class IterCreator;
protected:
	const Value* cont;
	int ii;

public:
	virtual void Reset() { ii = -1; }
	virtual const T& Get() const { ASSERT(ii==0); ASSERT(cont->Is<T>()); return ValueTo<T>(*cont); }
	virtual bool Next() { return (++ii < 1); }

	virtual ConstIterValue* Copy() const { return new ConstIterValue(*this); }
	virtual ConstIterValue* PartialCopy() const { ConstIterValue* t = Copy(); t->Reset(); return t; }
	
	ConstIterValue() : cont(NULL), ii(-1) {}
	ConstIterValue(const Value& _cont) : cont(&_cont), ii(-1) {}
	ConstIterValue(const Value& _cont, int ii) : cont(&_cont), ii(ii) {}
};

template<class T>
class IterValue : public Iter<T>
{
	friend class IterCreator;
protected:
	Value* cont;
	int ii;

public:
	virtual void Reset() { ii = -1; }
	virtual T& Get() const { ASSERT(ii==0); ASSERT(cont->Is<T>()); return const_cast<T&>(ValueTo<T>(*cont)); }
	virtual bool Next() { return (++ii < 1); }

	virtual IterValue* Copy() const { return new IterValue(*this); }
	virtual IterValue* PartialCopy() const { IterValue* t = Copy(); t->Reset(); return t; }
	
	IterValue() : cont(NULL), ii(-1) {}
	IterValue(Value& _cont) : cont(&_cont), ii(-1) {}
	IterValue(Value& _cont, int ii) : cont(&_cont), ii(ii) {}
};

template<class T>
class ConstIterLink : public ConstIter<T>
{
	friend class IterCreator;
protected:
	const Link<T>* cont;
	const Link<T>* start;
	int ii;

public:
	virtual void Reset() { ii = -1; cont = start; }
	virtual const T& Get() const { ASSERT(ii>=0); return *cont->GetPtr(); }
	virtual bool Next() { ++ii; return ((cont = cont->GetNext()) != start); }

	virtual ConstIterLink* Copy() const { return new ConstIterLink(*this); }
	virtual ConstIterLink* PartialCopy() const { ConstIterLink* t = Copy(); t->Reset(); return t; }
	
	ConstIterLink() : cont(NULL), ii(-1) {}
	ConstIterLink(const Link<T>& _cont) : cont(&_cont), start(&_cont), ii(-1) {}
	ConstIterLink(const Link<T>& _cont, int ii) : cont(&_cont), start(&_cont), ii(ii) {}
};

template<class T>
class IterLink : public Iter<T>
{
	friend class IterCreator;
protected:
	Link<T>* cont;
	Link<T>* start;
	int ii;

public:
	virtual void Reset() { ii = -1; cont = start; }
	virtual T& Get() const { ASSERT(ii>=0); return *cont->GetPtr(); }
	virtual bool Next() { ++ii; return ((cont = cont->GetNext()) != start); }

	virtual IterLink* Copy() const { return new IterLink(*this); }
	virtual IterLink* PartialCopy() const { IterLink* t = Copy(); t->Reset(); return t; }
	
	IterLink() : cont(NULL), ii(-1) {}
	IterLink(Link<T>& _cont) : cont(&_cont), start(&_cont), ii(-1) {}
	IterLink(Link<T>& _cont, int ii) : cont(&_cont), start(&_cont), ii(ii) {}
};

class IterCreator
{
public:

ITER_CONT(Vector<T>)
ITER_CONT(Array<T>)
ITER_CONT(BiVector<T>)
ITER_CONT(BiArray<T>)
ITER_CONT(Index<T>)
ITER_CONT(ArrayIndex<T>)
ITER_CONT(Segtor<T>)

ITER_PTR(Ptr<T>)
ITER_PTR(One<T>)
ITER_PTR(T*)

	template<class T> inline static Iter<T>* GetIter(Any& cont) { ASSERT(cont.Is<T>()); return new IterAny<T>(cont); } \
	template<class T> inline static ConstIter<T>* GetIter(const Any& cont) { ASSERT(cont.Is<T>()); return new ConstIterAny<T>(cont); } \

	template<class T> inline static Iter<T>* GetIter(Value& cont) { ASSERT(cont.Is<T>()); return new IterValue<T>(cont); } \
	template<class T> inline static ConstIter<T>* GetIter(const Value& cont) { ASSERT(cont.Is<T>()); return new ConstIterValue<T>(cont); } \

	template<class T> inline static Iter<T>* GetIter(Link<T>& cont) { return new IterLink<T>(cont); } \
	template<class T> inline static ConstIter<T>* GetIter(const Link<T>& cont) { return new ConstIterLink<T>(cont); } \

private:
	IterCreator();
	IterCreator(const IterCreator&);
	~IterCreator();
};

//use these to manage scope destrying
#define ITER(type) One<Iter<type> >
#define ITERC(type) One<ConstIter<type> >

#define FOREACHCONT(type, var, cont) \
for(int __i = 0, __c = (cont).GetCount(), __b; __b = 1,__i < __c; ++__i) \
	for(type& var = (cont)[__i]; __b; __b = 0) \


#define FOREACHCONTC(type, var, cont) \
for(int __i = 0, __c = (cont).GetCount(), __b; __b = 1,__i < __c; ++__i) \
	for(const type& var = (cont)[__i]; __b; __b = 0) \


#define FOREACH(type, var, cont) \
if(int __b = 1) \
for(One<Iter<type> > __it = IterCreator::GetIter<type>(cont); __b = 1, __it->Next(); ) \
	for(type& var = __it->Get(); __b; __b = 0)\


#define FOREACHC(type, var, cont) \
if(int __b = 1) \
for(One<ConstIter<type> > __it = IterCreator::GetIter<type>(cont); __b = 1, __it->Next(); ) \
	for(const type& var = __it->Get(); __b; __b = 0)\

#endif
