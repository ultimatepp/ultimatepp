#ifndef _Dispatcher_Dispatcher_h
#define _Dispatcher_Dispatcher_h

#include <Core/Core.h>

using namespace Upp;

template<class T>
class Dispatcher;

class DispatcherGen;

template<class T>
class  Dispatchable
{
	friend class Dispatcher<T>;
	friend class DispatcherGen;
public:
	typedef Dispatchable<T> CLASSNAME;
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
	VectorMap<unsigned, Any> src;
	VectorMap<unsigned, unsigned> key; //cache under which key registered for removal
};

template<class T>
class Dispatcher
{
public:
	typedef Dispatcher<T> CLASSNAME;
	Dispatcher();
	virtual ~Dispatcher();

	void DoDispatch(const T & o) const;

	void Register(Dispatchable<T> & d, unsigned key = 0);
	void Unregister(Dispatchable<T> & d, unsigned key = 0);
	Dispatchable<T> * GetDispatchable(unsigned key) const;
	const VectorMap<unsigned, Dispatchable<T> * > & GetDests() const { return dests; }

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
{
public:
	typedef DispatcherCB<T> CLASSNAME;
	DispatcherCB();
	virtual ~DispatcherCB();

	void DoDispatch(const T & o) const;

	void Register(Callback1<const T &> d, unsigned key);
	void Unregister(unsigned key);
	Callback1<const T &> GetDispatchable(unsigned key);
	const VectorMap<unsigned, Callback1<const T &> > & GetDests() const { return dests; }

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

#endif

