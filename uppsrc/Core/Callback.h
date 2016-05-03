template<typename Res, typename... ArgTypes>
class Function<Res(ArgTypes...)> : Moveable<Function<Res(ArgTypes...)>> {
	struct WrapperBase {
		Atomic  refcount;

		virtual Res Execute(ArgTypes... args) = 0;
		
		WrapperBase() { refcount = 1; }
		virtual ~WrapperBase() {}
	};

	template <class F>
	struct Wrapper : WrapperBase {
		F fn;
		virtual Res Execute(ArgTypes... args) { return fn(args...); }

		Wrapper(F&& fn) : fn(pick(fn)) {}
	};

	template <class F>
	struct Wrapper2 : WrapperBase {
		Function l;
		F        fn;

		virtual Res Execute(ArgTypes... args) { l(args...); return fn(args...); }

		Wrapper2(const Function& l, F&& fn) : l(l), fn(pick(fn)) {}
	};

	WrapperBase *ptr;
	
	static void Free(WrapperBase *ptr) {
		if(ptr && AtomicDec(ptr->refcount) == 0)
			delete ptr;
	}
	
	void Copy(const Function& a) {
		ptr = a.ptr;
		if(ptr)
			AtomicInc(ptr->refcount);
	}

public:
	Function()                                 { ptr = NULL; }
	
	template <class F> Function(F fn)          { ptr = new Wrapper<F>(pick(fn)); }
	
	Function(const Function& src)              { Copy(src); }
	Function& operator=(const Function& src)   { auto b = ptr; Copy(src); Free(b); return *this; }

	Function(Function&& src)                   { ptr = src.ptr; src.ptr = NULL; }
	Function& operator=(Function&& src)        { if(&src != this) { Free(ptr); ptr = src.ptr; src.ptr = NULL; } return *this; }
	
	Function Proxy() const                     { return [=] (ArgTypes... args) { return (*this)(args...); }; }

	template <class F>
	Function& operator<<(F fn)                 { if(!ptr) { Copy(fn); return *this; }
	                                             WrapperBase *b = ptr; ptr = new Wrapper2<F>(*this, pick(fn)); Free(b); return *this; }

	Res operator()(ArgTypes... args) const     { return ptr ? ptr->Execute(args...) : Res(); }
	
	operator bool() const                      { return ptr; }
	void Clear()                               { Free(ptr); ptr = NULL; }

	friend void Swap(Function& a, Function& b) { UPP::Swap(a.ptr, b.ptr); }

	~Function()                                { Free(ptr); }
};

enum CNULLer { CNULL };

// we need "isolation level" to avoid overloading issues
template <class... ArgTypes>
class Event : Moveable<Event<ArgTypes...>> {
	typedef Function<void (ArgTypes...)> Fn;

	Fn fn;

public:
	Event() {}
	Event(CNULLer)                             {}
	Event(const Event& src) : fn(src.fn)       {}
	Event(Event&& src) : fn(pick(src.fn))      {}
	Event(Fn&& src, int) : fn(src)             {}
	template <class F>
	Event(F src, int) : fn(src)                {}
//	Event(Fn&& src) : fn(src)                  {}
//	Event& operator=(const Fn& src)                   { fn = src.fn; return *this; }
	Event& operator=(const Event& src)         { fn = src.fn; return *this; }
	Event& operator=(Event&& src)              { fn = pick(src.fn); return *this; }
	Event& operator=(CNULLer)                  { fn.Clear(); return *this; }
	Event Proxy() const                        { return Event(fn.Proxy(), 1); }

	Event& operator<<(const Event& b)          { fn << b.fn; return *this; }
	Event& operator<<(const Fn& b)             { fn << b; return *this; }

	Event& operator<<(Event&& b)               { fn << pick(b.fn); return *this; }
	Event& operator<<(Fn&& b)                  { fn << pick(b); return *this; }
	
	template <class F>
	Event& operator<<(F f)                     { fn << f; return *this; }
	
	void operator()(ArgTypes... args) const    { return fn(args...); }

	operator Fn() const                        { return fn; }
	operator bool() const                      { return fn; }
	void Clear()                               { fn.Clear(); }
	
	friend Event Proxy(const Event& a)   { return a.Proxy(); }
	friend void Swap(Event& a, Event& b) { UPP::Swap(a.fn, b.fn); }
};

// we need "isolation level" to avoid overloading issues
template <class... ArgTypes>
class EventGate : Moveable<EventGate<ArgTypes...>> {
	typedef Function<bool (ArgTypes...)> Fn;

	Fn fn;
	
	void Set(bool b) { if(b) fn = [](ArgTypes...) { return true; }; else fn.Clear(); }

public:
	EventGate()                                {}
	EventGate(bool b)                          { Set(b); }
	EventGate(CNULLer)                         {}
	EventGate(const EventGate& a) : fn(a.fn)       {}
//	EventGate(EventGate&& a) : fn(pick(a.fn))      {}
	EventGate(Fn&& src, int) : fn(src)         {}
	EventGate& operator=(const EventGate& a)   { fn = a.fn; return *this; }
	EventGate& operator=(EventGate&& a)        { fn = pick(a.fn); return *this; }
	EventGate& operator=(CNULLer)              { fn.Clear(); return *this; }
	EventGate& operator=(bool b)               { Set(b); return *this; }
	EventGate Proxy() const                    { return fn.Proxy(); }

	EventGate& operator<<(const EventGate& b)  { fn << b.fn; return *this; }
	EventGate& operator<<(const Fn& b)         { fn << b; return *this; }

	EventGate& operator<<(EventGate&& b)       { fn << pick(b.fn); return *this; }
	EventGate& operator<<(Fn&& b)              { fn << pick(b); return *this; }

	template <class F>
	EventGate& operator<<(F f)                 { fn << f; return *this; }
	
	bool operator()(ArgTypes... args) const { return fn(args...); }

	operator Fn() const                    { return fn; }
	operator bool() const                  { return fn; }
	void Clear()                           { fn.Clear(); }

	friend EventGate Proxy(const EventGate& a)     { return a.Proxy(); }
	friend void Swap(EventGate& a, EventGate& b)   { UPP::Swap(a.fn, b.fn); }
};

// backward compatibility
typedef Event<> Callback;
template <class P1> using Callback1 = Event<P1>;
template <class P1, class P2> using Callback2 = Event<P1, P2>;
template <class P1, class P2, class P3> using Callback3 = Event<P1, P2, P3>;
template <class P1, class P2, class P3, class P4> using Callback4 = Event<P1, P2, P3, P4>;
template <class P1, class P2, class P3, class P4, class P5> using Callback5 = Event<P1, P2, P3, P4, P5>;

#define  Res void
#define  Cb_ Event
#include "CallbackR.i"

using Gate = EventGate<>;
template <class P1> using Gate1 = EventGate<P1>;
template <class P1, class P2> using Gate2 = EventGate<P1, P2>;
template <class P1, class P2, class P3> using Gate3 = EventGate<P1, P2, P3>;
template <class P1, class P2, class P3, class P4> using Gate4 = EventGate<P1, P2, P3, P4>;
template <class P1, class P2, class P3, class P4, class P5> using Gate5 = EventGate<P1, P2, P3, P4, P5>;

#define  Res bool
#define  Cb_ EventGate
#include "CallbackR.i"

#define THISBACK(x)                  callback(this, &CLASSNAME::x)
#define THISBACK1(x, arg)            callback1(this, &CLASSNAME::x, arg)
#define THISBACK2(m, a, b)           callback2(this, &CLASSNAME::m, a, b)
#define THISBACK3(m, a, b, c)        callback3(this, &CLASSNAME::m, a, b, c)
#define THISBACK4(m, a, b, c, d)     callback4(this, &CLASSNAME::m, a, b, c, d)
#define THISBACK5(m, a, b, c, d, e)  callback5(this, &CLASSNAME::m, a, b, c, d,e)

#define PTEBACK(x)                   pteback(this, &CLASSNAME::x)
#define PTEBACK1(x, arg)             pteback1(this, &CLASSNAME::x, arg)
#define PTEBACK2(m, a, b)            pteback2(this, &CLASSNAME::m, a, b)
#define PTEBACK3(m, a, b, c)         pteback3(this, &CLASSNAME::m, a, b, c)
#define PTEBACK4(m, a, b, c, d)      pteback4(this, &CLASSNAME::m, a, b, c, d)
#define PTEBACK5(m, a, b, c, d, e)   pteback5(this, &CLASSNAME::m, a, b, c, d, e)

#define STDBACK(x)                   callback(&x)
#define STDBACK1(x, arg)             callback1(&x, arg)
#define STDBACK2(m, a, b)            callback2(&m, a, b)
#define STDBACK3(m, a, b, c)         callback3(&m, a, b, c)
#define STDBACK4(m, a, b, c, d)      callback4(&m, a, b, c, d)
#define STDBACK5(m, a, b, c, d, e)   callback5(&m, a, b, c, d, e)


template <class T>
class CallbackArgTarget
{
	T result;

	void SetResult(const T& value)      { result = value; }
	void Set(T value)                   { result = value; }

public:
	typedef CallbackArgTarget CLASSNAME;

	operator const T&() const           { return result; }
	bool IsNullInstance() const         { return IsNull(result); }

	Callback operator[](const T& value) { return THISBACK1(SetResult, value); }
	operator Callback1<const T&>()      { return THISBACK(SetResult); }
	operator Callback1<T>()             { return THISBACK(Set); }

	CallbackArgTarget()                 { result = Null; }
};
