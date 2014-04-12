enum _CNULL { CNULL };

template <class D, class S>
inline D brutal_cast(const S& source) { return *(D *)&source; }

#include "Cbgen.h"
#include "Callback0.h"
#include "Callback1.h"
#include "Callback2.h"
#include "Callback3.h"
#include "Callback4.h"

#define THISBACK(x)               callback(this, &CLASSNAME::x)
#define THISBACK1(x, arg)         callback1(this, &CLASSNAME::x, arg)
#define THISBACK2(m, a, b)        callback2(this, &CLASSNAME::m, a, b)
#define THISBACK3(m, a, b, c)     callback3(this, &CLASSNAME::m, a, b, c)
#define THISBACK4(m, a, b, c, d)  callback4(this, &CLASSNAME::m, a, b, c, d)

#define PTEBACK(x)                pteback(this, &CLASSNAME::x)
#define PTEBACK1(x, arg)          pteback1(this, &CLASSNAME::x, arg)
#define PTEBACK2(m, a, b)         pteback2(this, &CLASSNAME::m, a, b)
#define PTEBACK3(m, a, b, c)      pteback3(this, &CLASSNAME::m, a, b, c)
#define PTEBACK4(m, a, b, c, d)   pteback4(this, &CLASSNAME::m, a, b, c, d)

#define STDBACK(x)                callback(&x)
#define STDBACK1(x, arg)          callback1(&x, arg)
#define STDBACK2(m, a, b)         callback2(&m, a, b)
#define STDBACK3(m, a, b, c)      callback3(&m, a, b, c)
#define STDBACK4(m, a, b, c, d)   callback4(&m, a, b, c, d)

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

/*
inline Callback lambda(std::function<void ()> fn) { return Callback(new LambdaCallback(fn)); }

template <typename P1>
inline Callback lambda(void (P1) fn) { return Callback(new LambdaCallback1<P1>(std::function<void (PI)>(fn))); }

template <class P1, class P2>
inline Callback lambda(std::function<void (P1, P2)> fn) { return Callback(new LambdaCallback2<P1, P2>(fn)); }

template <class P1, class P2, class P3>
inline Callback lambda(std::function<void (P1, P2, P3)> fn) { return Callback(new LambdaCallback3<P1, P2, P3>(fn)); }

template <class P1, class P2, class P3, class P4>
inline Callback lambda(std::function<void (P1, P2, P3, P4)> fn) { return Callback(new LambdaCallback4<P1, P2, P3, P4>(fn)); }
*/
