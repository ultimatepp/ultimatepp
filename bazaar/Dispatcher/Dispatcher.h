#ifndef _Dispatcher_Dispatcher_h
#define _Dispatcher_Dispatcher_h

#include <Core/Core.h>

using namespace Upp;

#include <Gen/Gen.h>

template<class T>
class Dispatcher;

class DispatcherGen;

template<class T, class B = EmptyClass>
class  Dispatchable
	: public EnableOption<>
{
	friend class Dispatcher<T>;
	friend class DispatcherGen;
public:
	typedef Dispatchable<T,B> CLASSNAME;
	Dispatchable();
	virtual ~Dispatchable();

public:
	virtual void Dispatch(const T & o) = 0;

	void Unregister(const Any & _src);
	void UnregisterAll();
	void Unregister(Dispatcher<T> & from) { int i = src.Find(GetPtrHashValue(&from)); if(i<0) return; Unregister(src[i]); }
	void Unregister(DispatcherGen & from) { int i = src.Find(GetPtrHashValue(&from)); if(i<0) return; Unregister(src[i]); }

	const VectorMap<unsigned, Any> & GetSrc() const { return src; }

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
	Dispatcher();
	virtual ~Dispatcher();

	void DoDispatch(const T & o) const;
	int GetCount() const { return dests.GetCount(); }

	void Register(Dispatchable<T> & d, unsigned key = 0);
	void Unregister(Dispatchable<T> & d, unsigned key = 0);
	Dispatchable<T> * GetDispatchable(unsigned key) const;
	const VectorMap<unsigned, Dispatchable<T> * > & GetDests() const { return dests; }
	void Clear() { while(dests.GetCount()>0) dests[0].Unregister(); }

private:
	VectorMap<unsigned, Dispatchable<T> * > dests;	
};

//a generic version that accepts any Dispatchable<T>
class DispatcherGen
{
public:
	typedef DispatcherGen CLASSNAME;
	DispatcherGen();
	virtual ~DispatcherGen();

	template<class T>
	void DoDispatch(const T & o) const;

	template<class T>
	void Register(Dispatchable<T> & d, unsigned key = 0);
	template<class T>
	void Unregister(Dispatchable<T> & d, unsigned key = 0);
	template<class T>
	Dispatcher<T> & GetDispatcher();

private:
	Vector<Any> dests;	//now does store the different Dispatcher<T>
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
	DispatcherCB();
	virtual ~DispatcherCB();

	void DoDispatch(const T & o) const;
	int GetCount() const { return dests.GetCount(); }

	void Register(Callback1<const T &> d, unsigned key);
	void Unregister(unsigned key);
	Callback1<const T &>* GetDispatchable(unsigned key);
	const VectorMap<unsigned, Callback1<const T &> > & GetDests() const { return dests; }
	void Clear() { dests.Clear(); }

private:
	VectorMap<unsigned, Callback1<const T &> > dests;	
};

//a generic version that accepts any Callback<cons T &, unsigned>
class DispatcherCBGen
{
public:
	typedef DispatcherCBGen CLASSNAME;
	DispatcherCBGen();
	virtual ~DispatcherCBGen();

	template<class T>
	void DoDispatch(const T & o) const;

	template<class T>
	void Register(Callback1<const T &> d, unsigned key);
	template<class T>
	void Unregister(unsigned key);
	template<class T>
	DispatcherCB<T> & GetDispatcherCB();

private:
	Vector<Any> dests;	//now does store the different DispatcherCB<T>
};


class Dispatcher0
	: public EnableOption<>
{
public:
	typedef Dispatcher0 CLASSNAME;
	typedef EnableOption<> R;
	Dispatcher0();
	virtual ~Dispatcher0();

	void DoDispatch() const;
	int GetCount() const { return dests.GetCount(); }

	void Register(Callback d, unsigned key);
	void Unregister(unsigned key);
	Callback* GetDispatchable(unsigned key);
	const VectorMap<unsigned, Callback> & GetDests() const { return dests; }
	void Clear() { dests.Clear(); }

private:
	VectorMap<unsigned, Callback> dests;	
};

template<class T>
int GetLinkCount(const T& l)
{
	int c = 0;
	const T *list = l.GetPtr(), *e = list;
	while((e = e->GetNext()) != list); 
		++c;
}

class DispatcherL0
	: public EnableOption<>
{
public:
	typedef DispatcherL0 CLASSNAME;
	typedef EnableOption<> R;
	struct Handler : public Link<Handler> { Callback h; };

	DispatcherL0();
	virtual ~DispatcherL0();

	void DoDispatch() const;
	int GetCount() const { GetLinkCount(dests); }

	void Clear() { dests.Unlink(); }

	Link<Handler> dests;
};

template<class T>
class DispatcherL
	: public EnableOption<>
{
public:
	typedef DispatcherL<T> CLASSNAME;
	typedef EnableOption<> R;
	struct Handler : public Link<Handler> { Callback h; };

	DispatcherL();
	virtual ~DispatcherL();

	void DoDispatch(const T& o) const;
	int GetCount() const { GetLinkCount(dests); }

	void Clear() { dests.Unlink(); }

	Link<Handler> dests;
};

#endif

