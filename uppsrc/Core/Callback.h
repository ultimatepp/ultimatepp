enum _CNULL { CNULL };

template <class D, class S>
inline D brutal_cast(const S& source) { return *(D *)&source; }

#include "Cbgen.h"
#include "Callback0.h"
#include "Callback1.h"
#include "Callback2.h"
#include "Callback3.h"
#include "Callback4.h"
#include "Callback5.h"

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

#ifdef CPP_11
inline
Callback& operator<<(Callback& a, std::function<void ()> fn) { return a << Callback(lambda(fn)); };

template <class P1, class L>
Callback1<P1>& operator<<(Callback1<P1>& a, L b)
{
	return a << Callback1<P1>(lambda(b));
}

template <class P1, class P2, class L>
Callback2<P1, P2>& operator<<(Callback2<P1, P2>& a, L b)
{
	return a << Callback2<P1, P2>(lambda(b));
}

template <class P1, class P2, class P3, class L>
Callback3<P1, P2, P3>& operator<<(Callback3<P1, P2, P3>& a, L b)
{
	return a << Callback3<P1, P2, P3>(lambda(b));
}

template <class P1, class P2, class P3, class P4, class L>
Callback4<P1, P2, P3, P4>& operator<<(Callback4<P1, P2, P3, P4>& a, L b)
{
	return a << Callback4<P1, P2, P3, P4>(lambda(b));
}

template <class L>
Gate& operator<<(Gate& a, L b)
{
	return a << Gate(lambda(b));
};

template <class P1, class L>
Gate1<P1>& operator<<(Gate1<P1>& a, L b)
{
	return a << Gate1<P1>(lambda(b));
}

template <class P1, class P2, class L>
Gate2<P1, P2>& operator<<(Gate2<P1, P2>& a, L b)
{
	return a << Gate2<P1, P2>(lambda(b));
}

template <class P1, class P2, class P3, class L>
Gate3<P1, P2, P3>& operator<<(Gate3<P1, P2, P3>& a, L b)
{
	return a << Gate3<P1, P2, P3>(lambda(b));
}

template <class P1, class P2, class P3, class P4, class L>
Gate4<P1, P2, P3, P4>& operator<<(Gate4<P1, P2, P3, P4>& a, L b)
{
	return a << Gate4<P1, P2, P3, P4>(lambda(b));
}
#endif
