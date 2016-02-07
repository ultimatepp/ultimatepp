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
	Function() { ptr = NULL; }
	Function(const Function& a) { Copy(a); }
	
	Function(const bool &) {} _DBG_
	
	Function& operator=(const Function& other) { WrapperBase *b = ptr; Copy(other); Free(b); return *this; }
	
	template <class F> Function(F fn) { ptr = new Wrapper<F>(pick(fn)); }
	
	template <class F>
	Function& operator<<(F fn) {
		WrapperBase *b = ptr;
		ptr = new Wrapper2<F>(*this, pick(fn));
		Free(ptr);
		return *this;
	}
	
	Function Proxy() {
		return [=] (ArgTypes... args) { return (*this)(args...); };
	}
	
	Res operator()(ArgTypes... args) const { return ptr ? ptr->Execute(args...) : Res(); }
	
	operator bool() const { return ptr; }
	void Clear()          { Free(ptr); ptr = NULL; }

	~Function()           { Free(ptr); }
};

template<typename Res, typename... ArgTypes>
Function<Res (ArgTypes...)> Proxy(const Function<Res (ArgTypes...)>& target)
{
	return target.Proxy();
}

using Callback = Function<void ()>;

template <class... Args> using CallbackN = Function<void (Args...)>;

template <class P1> using Callback1 = CallbackN<P1>;
template <class P1, class P2> using Callback2 = CallbackN<P1, P2>;
template <class P1, class P2, class P3> using Callback3 = CallbackN<P1, P2, P3>;
template <class P1, class P2, class P3, class P4> using Callback4 = CallbackN<P1, P2, P3, P4>;
template <class P1, class P2, class P3, class P4, class P5> using Callback5 = CallbackN<P1, P2, P3, P4, P5>;

using Gate = Function<bool ()>;

template <class... Args> using GateN = Function<bool (Args...)>;

template <class P1> using Gate1 = GateN<P1>;
template <class P1, class P2> using Gate2 = GateN<P1, P2>;
template <class P1, class P2, class P3> using Gate3 = GateN<P1, P2, P3>;
template <class P1, class P2, class P3, class P4> using Gate4 = GateN<P1, P2, P3, P4>;
template <class P1, class P2, class P3, class P4, class P5> using Gate5 = GateN<P1, P2, P3, P4, P5>;

#define  Res void
#include "CallbackR.i"

#define  Res bool
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
