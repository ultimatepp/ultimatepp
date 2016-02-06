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
	
	Function& operator=(const Function& other) { WrapperBase *b = ptr; Copy(other); Free(b); return *this; }
	
	template <class F> Function(F fn) { ptr = new Wrapper<F>(pick(fn)); }
	
	template <class F>
	Function& operator<<(F fn) {
		WrapperBase *b = ptr;
		ptr = new Wrapper2<F>(*this, pick(fn));
		Free(ptr);
		return *this;
	}
	
	Res operator()(ArgTypes... args) const { return ptr ? ptr->Execute(args...) : Res(); }
	
	operator bool() const { return ptr; }
	void Clear()          { Free(ptr); ptr = NULL; }

	~Function()           { Free(ptr); }
};

typedef Function<void ()> Callback;

inline Callback callback(void (*fn)()) { return [=] { (*fn)(); }; }

template <class O, class M>
Callback callback(O *object, void (M::*method)())
{ return [=] { (object->*method)(); }; }

template <class O, class M>
Callback callback(O *object, void (M::*method)() const)
{ return [=] { (object->*method)(); }; }

template <class O, class M>
Callback pteback(O *object, void (M::*method)())
{ Ptr<O> ptr = object; return [=] { if(ptr) (ptr->*method)(); }; }

template <class O, class M>
Callback pteback(O *object, void (M::*method)() const)
{ Ptr<O> ptr = object; return [=] { if(ptr) (ptr->*method)(); } }

inline Callback callback(Callback cb1, Callback cb2)   { cb1 << cb2; return cb1; }

// Callback1 ----------------------------------------------------------------

template <class P1>
using Callback1 = Function<void (P1)>;

template <class P1>
Callback1<P1> callback(void (*fn)(P1)) { return [=](P1 p) { (*fn)(p); }; }

template <class O, class M, class P1>
Callback1<P1> callback(O *object, void (M::*method)(P1))
{ return [=](P1 p) { (object->*method)(p); }; }

template <class O, class M, class P1>
Callback1<P1> callback(O *object, void (M::*method)(P1) const)
{ return [=](P1 p) { (object->*method)(p); }; }

template <class O, class M, class P1>
Callback1<P1> pteback(O *object, void (M::*method)(P1))
{ Ptr<O> ptr = object; return [=](P1 p) { if(ptr) (ptr->*method)(p); }; }

template <class O, class M, class P1>
Callback1<P1> pteback(O *object, void (M::*method)(P1) const)
{ Ptr<O> ptr = object; return [=](P1 p) { if(ptr) (ptr->*method)(p); } }

template <class P1>
Callback1<P1> callback(Callback1<P1> cb1, Callback1<P1> cb2)   { cb1 << cb2; return cb1; }

// Callback2 ----------------------------------------------------------------

template <class P1, class P2>
using Callback2 = Function<void (P1, P2)>;

template <class P1, class P2>
Callback2<P1, P2> callback(void (*fn)(P1, P2)) { return [=](P1 p1, P2 p2) { (*fn)(p1, p2); }; }

template <class O, class M, class P1, class P2>
Callback2<P1, P2> callback(O *object, void (M::*method)(P1, P2))
{ return [=](P1 p1, P2 p2) { (object->*method)(p1, p2); }; }

template <class O, class M, class P1, class P2>
Callback2<P1, P2> callback(O *object, void (M::*method)(P1, P2) const)
{ return [=](P1 p1, P2 p2) { (object->*method)(p1, p2); }; }

template <class O, class M, class P1, class P2>
Callback2<P1, P2> pteback(O *object, void (M::*method)(P1, P2))
{ Ptr<O> ptr = object; return [=](P1 p1, P2 p2) { if(ptr) (ptr->*method)(p1, p2); }; }

template <class O, class M, class P1, class P2>
Callback2<P1, P2> pteback(O *object, void (M::*method)(P1, P2) const)
{ Ptr<O> ptr = object; return [=](P1 p1, P2 p2) { if(ptr) (ptr->*method)(p1, p2); } }

template <class P1, class P2>
Callback2<P1, P2> callback(Callback2<P1, P2> cb1, Callback2<P1, P2> cb2)   { cb1 << cb2; return cb1; }

// Callback3 ----------------------------------------------------------------

template <class P1, class P2, class P3>
using Callback3 = Function<void (P1, P2, P3)>;

template <class P1, class P2, class P3, class P4>
using Callback4 = Function<void (P1, P2, P3, P4)>;

typedef Function<bool ()> Gate;

template <class P1>
using Gate1 = Function<bool (P1)>;

template <class P1, class P2>
struct Gate2 : Function<bool (P1, P2)> {
	typedef Function<bool (P1, P2)> B;
	template <class F> Gate2(F fn) : B(fn) {}
	template <class F> Gate2& operator<<(F fn) { operator<<(fn); return *this; }
	
	Gate2(bool b) : B([=](P1, P2){ return b; }) {} // Optimize?
};

template <class P1, class P2, class P3>
using Gate3 = Function<bool (P1, P2, P3)>;

template <class P1, class P2, class P3, class P4>
using Gate4 = Function<bool (P1, P2, P3, P4)>;

// ----

template <class F, class... Args, class... BindArgs>
auto LastArgs(F fn, BindArgs... bind_args)
{
	return [=](Args... args) { return fn(args..., bind_args...); };
}

// ----

template <class... Args, class P1>
auto callback1(void (*fn)(Args...), P1 p1) { return LastArgs(fn, p1); }

template <class... Args, class P1, class P2>
auto callback2(void (*fn)(Args...), P1 p1, P2 p2) { return LastArgs(fn, p1, p2); }

template <class... Args, class P1, class P2, class P3>
auto callback3(void (*fn)(Args...), P1 p1, P2 p2, P3 p3) { return LastArgs(fn, p1, p2, p3); }

template <class... Args, class P1, class P2, class P3, class P4>
auto callback4(void (*fn)(Args...), P1 p1, P2 p2, P3 p3, P4 p4) { return LastArgs(fn, p1, p2, p3, p4); }

template <class... Args, class P1, class P2, class P3, class P4, class P5>
auto callback5(void (*fn)(Args...), P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) { return LastArgs(fn, p1, p2, p3, p4, p5); }

// ----

template <class O, class M, class... Args, class P1>
auto callback1(assassfkat.cr
, P1 p1) { return LastArgs(fn, p1); }

template <class O, class M, class... Args, class P1, class P2>
auto callback2(void (*fn)(Args...), P1 p1, P2 p2) { return LastArgs(fn, p1, p2); }

template <class O, class M, class... Args, class P1, class P2, class P3>
auto callback3(void (*fn)(Args...), P1 p1, P2 p2, P3 p3) { return LastArgs(fn, p1, p2, p3); }

template <class O, class M, class... Args, class P1, class P2, class P3, class P4>
auto callback4(void (*fn)(Args...), P1 p1, P2 p2, P3 p3, P4 p4) { return LastArgs(fn, p1, p2, p3, p4); }

template <class O, class M, class... Args, class P1, class P2, class P3, class P4, class P5>
auto callback5(void (*fn)(Args...), P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) { return LastArgs(fn, p1, p2, p3, p4, p5); }

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


inline Callback Proxy(Callback& cb)
{
	return [&] { cb(); };
}

template <class P1>
inline Callback1<P1> Proxy(Callback1<P1>& cb)
{
	return callback(&cb, &Callback1<P1>::Execute);
}

template <class P1, class P2>
inline Callback2<P1, P2> Proxy(Callback2<P1, P2>& cb)
{
	return callback(&cb, &Callback2<P1, P2>::Execute);
}

template <class P1, class P2, class P3>
inline Callback3<P1, P2, P3> Proxy(Callback3<P1, P2, P3>& cb)
{
	return callback(&cb, &Callback3<P1, P2, P3>::Execute);
}

template <class P1, class P2, class P3, class P4>
inline Callback4<P1, P2, P3, P4> Proxy(Callback4<P1, P2, P3, P4>& cb)
{
	return callback(&cb, &Callback4<P1, P2, P3, P4>::Execute);
}

inline Gate Proxy(Gate& cb)
{
	return [&] { return cb(); };
}

template <class P1>
inline Gate1<P1> Proxy(Gate1<P1>& cb)
{
	return callback(&cb, &Gate1<P1>::Execute);
}

template <class P1, class P2>
inline Gate2<P1, P2> Proxy(Gate2<P1, P2>& cb)
{
	return callback(&cb, &Gate2<P1, P2>::Execute);
}

template <class P1, class P2, class P3>
inline Gate3<P1, P2, P3> Proxy(Gate3<P1, P2, P3>& cb)
{
	return callback(&cb, &Gate3<P1, P2, P3>::Execute);
}

template <class P1, class P2, class P3, class P4>
inline Gate4<P1, P2, P3, P4> Proxy(Gate4<P1, P2, P3, P4>& cb)
{
	return callback(&cb, &Gate4<P1, P2, P3, P4>::Execute);
}

//----- bw

const nullptr_t CNULL = 0;
