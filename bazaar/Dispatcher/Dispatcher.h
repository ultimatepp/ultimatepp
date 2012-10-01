#ifndef _Dispatcher_Dispatcher_h
#define _Dispatcher_Dispatcher_h

#include <Core/Core.h>
using namespace Upp;

#include <Gen/Gen.h>
#include <Gen/Misc.h>

template<class T> class Dispatcher;

class DispatcherGen;

template<class T, class B = EmptyClass>
class  Dispatchable : public EnableOption<B>
{
	friend class Dispatcher<T>;
	friend class DispatcherGen;
public:
	typedef Dispatchable<T,B> CLASSNAME;
	typedef EnableOption<B> R;

	Dispatchable() : act(true) {}
	virtual ~Dispatchable() { UnregisterAll(); }

public:
	virtual void Dispatch(const T& o) = 0;

	void Unregister(const Any& _src);
	void UnregisterAll() { while(src.GetCount()>0) Unregister(src[0]); } //will remove elements

	void Unregister(Dispatcher<T>& from) { int i = src.Find(GetPtrHashValue(&from)); if(i<0) return; Unregister(src[i]); }
	void Unregister(DispatcherGen& from) { int i = src.Find(GetPtrHashValue(&from)); if(i<0) return; Unregister(src[i]); }

	const VectorMap<unsigned, Any>& GetSrc() const { return src; }

private:
	bool act;
	VectorMap<unsigned, Any> src;
	VectorMap<unsigned, unsigned> key; //cache under which key registered for removal
};

template<class T>
class Dispatcher
	: public EnableOption<>
{
public:
	typedef Dispatcher<T> CLASSNAME;
	typedef EnableOption<> R;

	virtual ~Dispatcher() { ASSERT(dests.IsEmpty()); }

	void DoDispatch(const T& o) const;
	int GetCount() const { return dests.GetCount(); }

	void Register(Dispatchable<T>& d, unsigned key = 0);
	void Unregister(Dispatchable<T>& d, unsigned key = 0);
	Dispatchable<T>* GetDispatchable(unsigned key) const;
	const VectorMap<unsigned, Dispatchable<T>*>& GetDests() const { return dests; }
	void Clear() { while(dests.GetCount()>0) dests[0]->Unregister(*this); }

private:
	VectorMap<unsigned, Dispatchable<T>*> dests;	
};

//a generic version that accepts any Dispatchable<T>
class DispatcherGen
{
public:
	typedef DispatcherGen CLASSNAME;

	template<class T>
	void DoDispatch(const T& o) const;

	template<class T>
	void Register(Dispatchable<T>& d, unsigned key = 0);
	template<class T>
	void Unregister(Dispatchable<T>& d, unsigned key = 0);
	template<class T>
	Dispatcher<T>& GetDispatcher();

private:
	Vector<Any> dests; //now does store the different Dispatcher<T>
};

//Callback variant
//spares out Dispatchable<T> interface

template<class T>
class DispatcherCB
	: public EnableOption<>
{
public:
	typedef DispatcherCB<T> CLASSNAME;
	typedef EnableOption<> R;

	virtual ~DispatcherCB() { ASSERT(dests.IsEmpty()); }


	void DoDispatch(const T& o) const;
	int GetCount() const { return dests.GetCount(); }

	void Register(Callback1<const T&> d, unsigned key);
	void Unregister(unsigned key);
	Callback1<const T&>* GetDispatchable(unsigned key);
	const VectorMap<unsigned, Callback1<const T&> >& GetDests() const { return dests; }
	void Clear() { dests.Clear(); }

private:
	VectorMap<unsigned, Callback1<const T&> > dests;	
};

//a generic version that accepts any Callback<cons T&, unsigned>
class DispatcherCBGen
{
public:
	typedef DispatcherCBGen CLASSNAME;

	template<class T>
	void DoDispatch(const T& o) const;

	template<class T>
	void Register(Callback1<const T&> d, unsigned key);
	template<class T>
	void Unregister(unsigned key);
	template<class T>
	DispatcherCB<T>& GetDispatcherCB();

private:
	Vector<Any> dests; //now does store the different DispatcherCB<T>
};

//void param variant
class Dispatcher0
	: public EnableOption<>
{
public:
	typedef Dispatcher0 CLASSNAME;
	typedef EnableOption<> R;

	virtual ~Dispatcher0() { ASSERT(dests.IsEmpty()); }

	void DoDispatch() const;
	int GetCount() const { return dests.GetCount(); }

	void Register(Callback d, unsigned key);
	void Unregister(unsigned key);
	Callback* GetDispatchable(unsigned key);
	const VectorMap<unsigned, Callback>& GetDests() const { return dests; }
	void Clear() { dests.Clear(); }

private:
	VectorMap<unsigned, Callback> dests;	
};

//Link<> list as managed data structure for easy handling
template<class T>
class DispatcherL
	: public EnableOption<>
{
public:
	typedef DispatcherL<T> CLASSNAME;
	typedef EnableOption<> R;
	struct Handler : public Link<Handler> { Callback h; };

	virtual ~DispatcherL() { ASSERT(dests.IsEmpty()); }

	void DoDispatch(const T& o) const;
	int GetCount() const { return GetLinkCount(dests); }

	void Clear() { dests.Unlink(); }

	Link<Handler> dests;
};

//Link<> list as managed data structure for easy handling
//void param list
class DispatcherL0
	: public EnableOption<>
{
public:
	typedef DispatcherL0 CLASSNAME;
	typedef EnableOption<> R;
	struct Handler : public Link<Handler> { Callback h; };

	virtual ~DispatcherL0() { ASSERT(dests.IsEmpty()); }

	void DoDispatch() const;
	int GetCount() const { return GetLinkCount(dests); }

	void Clear() { dests.Unlink(); }

	Link<Handler> dests;
};

#endif

