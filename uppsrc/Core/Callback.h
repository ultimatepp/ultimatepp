// Backward compatibility; use Function/Event in the new code

template <class... ArgTypes>
class CallbackN : Moveable<CallbackN<ArgTypes...>> {
	typedef Function<void (ArgTypes...)> Fn;

	Fn fn;

public:
	CallbackN() {}
	CallbackN(const CallbackN& src) : fn(src.fn)       {}
	CallbackN& operator=(const CallbackN& src)         { fn = src.fn; return *this; }

	CallbackN(Fn&& src, int) : fn(pick(src))       {} // Helper for callback compatibility code
	template <class F>
	CallbackN(F src, int) : fn(src)                {} // Helper for callback compatibility code
	
	CallbackN(CallbackN&& src) : fn(pick(src.fn))      {}
	CallbackN& operator=(CallbackN&& src)              { fn = pick(src.fn); return *this; }

	CallbackN(CNULLer)                             {}
	CallbackN& operator=(CNULLer)                  { fn.Clear(); return *this; }

	CallbackN Proxy() const                        { return CallbackN(fn.Proxy(), 1); }

	template <class F>
	CallbackN& operator<<(const F& f)              { fn << f; return *this; }

	template <class F>
	CallbackN& operator<<(F&& f)                   { fn << pick(f); return *this; }
	
	void operator()(ArgTypes... args) const    { return fn(args...); }

	operator Fn() const                        { return fn; }
	operator bool() const                      { return fn; }
	void Clear()                               { fn.Clear(); }
	
	friend CallbackN Proxy(const CallbackN& a)   { return a.Proxy(); }
	friend void Swap(CallbackN& a, CallbackN& b) { UPP::Swap(a.fn, b.fn); }
};

// we need "isolation level" to avoid overloading issues
template <class... ArgTypes>
class GateN : Moveable<GateN<ArgTypes...>> {
	typedef Function<bool (ArgTypes...)> Fn;

	Fn fn;
	
	void Set(bool b) { if(b) fn = [](ArgTypes...) { return true; }; else fn.Clear(); }

public:
	GateN()                                {}

	GateN(bool b)                          { Set(b); }
	GateN& operator=(bool b)               { Set(b); return *this; }

	GateN(const GateN& a) : fn(a.fn)   {}
	GateN& operator=(const GateN& a)   { fn = a.fn; return *this; }

	GateN(const Fn& src) : fn(src)   {}

	GateN(Fn&& src, int) : fn(pick(src))   {}
	GateN& operator=(GateN&& a)        { fn = pick(a.fn); return *this; }

	GateN(CNULLer)                         {}
	GateN& operator=(CNULLer)              { fn.Clear(); return *this; }

	GateN Proxy() const                    { return fn.Proxy(); }

	template <class F>
	GateN& operator<<(const F& f)          { fn << f; return *this; }

	template <class F>
	GateN& operator<<(F&& f)               { fn << pick(f); return *this; }
	
	bool operator()(ArgTypes... args) const    { return fn(args...); }

	operator Fn() const                        { return fn; }
	operator bool() const                      { return fn; }
	void Clear()                               { fn.Clear(); }

	friend GateN Proxy(const GateN& a)     { return a.Proxy(); }
	friend void Swap(GateN& a, GateN& b)   { UPP::Swap(a.fn, b.fn); }
};

// backward compatibility
typedef CallbackN<> Callback;
template <class P1> using Callback1 = CallbackN<P1>;
template <class P1, class P2> using Callback2 = CallbackN<P1, P2>;
template <class P1, class P2, class P3> using Callback3 = CallbackN<P1, P2, P3>;
template <class P1, class P2, class P3, class P4> using Callback4 = CallbackN<P1, P2, P3, P4>;
template <class P1, class P2, class P3, class P4, class P5> using Callback5 = CallbackN<P1, P2, P3, P4, P5>;

#define  Res void
#define  Cb_ CallbackN
#include "CallbackR.i"

using Gate0 = GateN<>;
template <class P1> using Gate1 = GateN<P1>;
template <class P1, class P2> using Gate2 = GateN<P1, P2>;
template <class P1, class P2, class P3> using Gate3 = GateN<P1, P2, P3>;
template <class P1, class P2, class P3, class P4> using Gate4 = GateN<P1, P2, P3, P4>;
template <class P1, class P2, class P3, class P4, class P5> using Gate5 = GateN<P1, P2, P3, P4, P5>;

#define  Res bool
#define  Cb_ GateN
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
class CallbackNArgTarget
{
	T result;

	void SetResult(const T& value)      { result = value; }
	void Set(T value)                   { result = value; }

public:
	typedef CallbackNArgTarget CLASSNAME;

	operator const T&() const           { return result; }
	bool IsNullInstance() const         { return IsNull(result); }

	Callback operator[](const T& value) { return THISBACK1(SetResult, value); }
	operator Callback1<const T&>()      { return THISBACK(SetResult); }
	operator Callback1<T>()             { return THISBACK(Set); }

	CallbackNArgTarget()                    { result = Null; }
};

template <class T>
using CallbackArgTarget = CallbackNArgTarget<T>;
