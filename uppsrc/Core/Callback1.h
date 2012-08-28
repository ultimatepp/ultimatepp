//1 arg to Callback

template <class O, class M, class T>
struct CallbackMethodActionArgPte : public CallbackAction {
	Ptr<O>  object;
	M       method;
	T             arg;

	void    Execute() { if(object) (object->*method)(arg); }

	CallbackMethodActionArgPte(O *object, M method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class O, class M, class Q, class T>
Callback pteback1(O *object, void (M::*method)(Q), T arg) {
	return Callback(new CallbackMethodActionArgPte<O, void (M::*)(Q), T>(object, method, arg));
}

template <class O, class M, class T>
struct CallbackMethodActionArg : public CallbackAction {
	O  *object;
	M   method;
	T   arg;

	void    Execute() { (object->*method)(arg); }

	CallbackMethodActionArg(O *object, M method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class O, class M, class Q, class T>
Callback callback1(O *object, void (M::*method)(Q), T arg) {
	return Callback(new CallbackMethodActionArg<O, void (M::*)(Q), T>
	                    (object, method, arg));
}

template <class O, class M, class Q, class T>
Callback callback1(const O *object, void (M::*method)(Q) const, T arg) {
	return Callback(new CallbackMethodActionArg<const O, void (M::*)(Q) const, T>
	                    (object, method, arg));
}

template <class F, class T, class HC = F>
struct CallbackActionCallArg : public CallbackAction {
	F         fn;
	T         arg;
	void    Execute() { fn(arg); }

	CallbackActionCallArg(F fn, T arg) : fn(fn), arg(arg) {}
};

template <class Q, class T>
Callback callback1(void (*fn)(Q), T arg)
{
	return Callback(new CallbackActionCallArg<void (*)(Q), T, uintptr_t>(fn, arg));
}

//reduct
template <class Q, class T>
Callback callback1(Callback1<Q> cb, T arg)
{
	return Callback(new CallbackActionCallArg<Callback1<Q>, T>(cb, arg));
}

//1 arg to Callback1

template <class O, class M, class P1, class T>
struct Callback1MethodActionArgPte : public Callback1Action<P1> {
	Ptr<O>  object;
	M       method;
	T             arg;
	void    Execute(P1 p1) { if(object) (object->*method)(p1, arg); }

	Callback1MethodActionArgPte(O *object, M method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class O, class M, class P1, class Q, class T>
Callback1<P1> pteback1(O *object, void (M::*method)(P1, Q), T arg) {
	return Callback1<P1>(new Callback1MethodActionArgPte<O, void (M::*)(P1, Q), P1, T> (object, method, arg));
}

template <class O, class M, class P1, class T>
struct Callback1MethodActionArg : public Callback1Action<P1> {
	O  *object;
	M   method;
	T         arg;
	void    Execute(P1 p1) { (object->*method)(p1, arg); }

	Callback1MethodActionArg(O *object, M method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class O, class M, class P1, class Q, class T>
Callback1<P1> callback1(O *object, void (M::*method)(P1, Q), T arg) {
	return Callback1<P1>(new Callback1MethodActionArg<O, void (M::*)(P1, Q), P1, T>
	                         (object, method, arg));
}

template <class O, class M, class P1, class Q, class T>
Callback1<P1> callback1(const O *object, void (M::*method)(P1, Q) const, T arg) {
	return Callback1<P1>(new Callback1MethodActionArg<const O, void (M::*)(P1, Q) const, P1, T>
	                         (object, method, arg));
}

template <class F, class P1, class T, class HC = F>
struct Callback1ActionCallArg : public Callback1Action<P1> {
	F         fn;
	T         arg;
	void    Execute(P1 p1) { fn(p1, arg); }

	Callback1ActionCallArg(F fn, T arg) : fn(fn), arg(arg) {}
};

template <class P1, class Q, class T>
Callback1<P1> callback1(void (*fn)(P1, Q), T arg)
{
	return Callback1<P1>(new Callback1ActionCallArg<void (*)(P1, Q), P1, T, uintptr_t>(fn, arg));
}

//reduct
template <class P1, class Q, class T>
Callback1<P1> callback1(Callback2<P1, Q> cb, T arg) {
	return Callback1<P1>(new Callback1ActionCallArg<Callback2<P1, Q>, P1, T>(cb, arg));
}

//1 arg to Callback2

template <class O, class M, class P1, class P2, class T>
struct Callback2MethodActionArg : public Callback2Action<P1, P2> {
	O  *object;
	M   method;
	T         arg;
	void    Execute(P1 p1, P2 p2) { (object->*method)(p1, p2, arg); }

	Callback2MethodActionArg(O *object, M method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class O, class M, class P1, class P2, class Q, class T>
Callback2<P1, P2> callback1(O *object, void (M::*method)(P1, P2, Q), T arg) {
	return Callback2<P1, P2>(new Callback2MethodActionArg<O, void (M::*)(P1, P2, Q), P1, P2, T>
	                         (object, method, arg));
}

template <class O, class M, class P1, class P2, class Q, class T>
Callback2<P1, P2> callback1(const O *object, void (M::*method)(P1, P2, Q) const, T arg) {
	return Callback2<P1, P2>(new Callback2MethodActionArg<const O, void (M::*)(P1, P2, Q) const, P1, P2, T>
	                         (object, method, arg));
}

template <class F, class P1, class P2, class T, class HC = F>
struct Callback2ActionCallArg : Callback2Action<P1, P2> {
	F         fn;
	T         arg;
	void    Execute(P1 p1, P2 p2) { fn(p1, p2, arg); }

	Callback2ActionCallArg(F fn, T arg) : fn(fn), arg(arg) {}
};

template <class P1, class P2, class Q, class T>
Callback2<P1, P2> callback1(void (*fn)(P1, P2, Q), T arg)
{
	return Callback2<P1, P2>(new Callback2ActionCallArg<void (*)(P1, P2, Q), P1, P2, T, uintptr_t>(fn, arg));
}

//reduct
template <class P1, class P2, class Q, class T>
Callback2<P1, P2> callback1(Callback3<P1, P2, Q> cb, T arg) {
	return Callback2<P1, P2>(new Callback2ActionCallArg<Callback3<P1, P2, Q>, P1, P2, T>(cb, arg));
}

//1 arg to Callback3

template <class O, class M, class P1, class P2, class P3, class T>
struct Callback3MethodActionArg : public Callback3Action<P1, P2, P3> {
	O  *object;
	M   method;
	T         arg;
	void    Execute(P1 p1, P2 p2, P3 p3) { (object->*method)(p1, p2, p3, arg); }

	Callback3MethodActionArg(O *object, M method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class O, class M, class P1, class P2, class P3, class Q, class T>
Callback3<P1, P2, P3> callback1(O *object, void (M::*method)(P1, P2, P3, Q), T arg) {
	return Callback3<P1, P2, P3>(new Callback3MethodActionArg<O, void (M::*)(P1, P2, P3, Q), P1, P2, P3, T>
	                         (object, method, arg));
}

template <class O, class M, class P1, class P2, class P3, class Q, class T>
Callback3<P1, P2, P3> callback1(const O *object, void (M::*method)(P1, P2, P3, Q) const, T arg) {
	return Callback3<P1, P2, P3>(new Callback3MethodActionArg<const O, void (M::*)(P1, P2, P3, Q) const, P1, P2, P3, T>
	                         (object, method, arg));
}

template <class F, class P1, class P2, class P3, class T, class HC = F>
struct Callback3ActionCallArg : Callback3Action<P1, P2, P3> {
	F         fn;
	T         arg;
	void    Execute(P1 p1, P2 p2, P3 p3) { fn(p1, p2, p3, arg); }

	Callback3ActionCallArg(F fn, T arg) : fn(fn), arg(arg) {}
};

template <class P1, class P2, class P3, class Q, class T>
Callback3<P1, P2, P3> callback1(void (*fn)(P1, P2, P3, Q), T arg)
{
	return Callback3<P1, P2, P3>(new Callback3ActionCallArg<void (*)(P1, P2, P3, Q), P1, P2, P3, T, uintptr_t>(fn, arg));
}

//reduct
template <class P1, class P2, class P3, class Q, class T>
Callback3<P1, P2, P3> callback1(Callback4<P1, P2, P3, Q> cb, T arg) {
	return Callback3<P1, P2, P3>(new Callback3ActionCallArg<Callback4<P1, P2, P3, Q>, P1, P2, P3, T>(cb, arg));
}

//1 arg to Callback4

template <class O, class M, class P1, class P2, class P3, class P4, class T>
struct Callback4MethodActionArg : public Callback4Action<P1, P2, P3, P4> {
	O  *object;
	M   method;
	T         arg;
	void    Execute(P1 p1, P2 p2, P3 p3, P4 p4) { (object->*method)(p1, p2, p3, p4, arg); }

	Callback4MethodActionArg(O *object, M method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class O, class M, class P1, class P2, class P3, class P4, class Q, class T>
Callback4<P1, P2, P3, P4> callback1(O *object, void (M::*method)(P1, P2, P3, P4, Q), T arg) {
	return Callback4<P1, P2, P3, P4>(new Callback4MethodActionArg<O, void (M::*)(P1, P2, P3, P4, Q), P1, P2, P3, P4, T>
	                         (object, method, arg));
}

template <class O, class M, class P1, class P2, class P3, class P4, class Q, class T>
Callback4<P1, P2, P3, P4> callback1(const O *object, void (M::*method)(P1, P2, P3, P4, Q) const, T arg) {
	return Callback4<P1, P2, P3, P4>(new Callback4MethodActionArg<const O, void (M::*)(P1, P2, P3, P4, Q) const, P1, P2, P3, P4, T>
	                         (object, method, arg));
}

template <class F, class P1, class P2, class P3, class P4, class T, class HC = F>
struct Callback4ActionCallArg : Callback4Action<P1, P2, P3, P4> {
	F         fn;
	T         arg;
	void    Execute(P1 p1, P2 p2, P3 p3, P4 p4) { fn(p1, p2, p3, p4, arg); }

	Callback4ActionCallArg(F fn, T arg) : fn(fn), arg(arg) {}
};

template <class P1, class P2, class P3, class P4, class Q, class T>
Callback4<P1, P2, P3, P4> callback1(void (*fn)(P1, P2, P3, P4, Q), T arg)
{
	return Callback4<P1, P2, P3, P4>(new Callback4ActionCallArg<void (*)(P1, P2, P3, P4, Q), P1, P2, P3, P4, T, uintptr_t>(fn, arg));
}

// -------------------------------------------------------

//1 arg to Gate

template <class O, class M, class T>
struct GateMethodActionArgPte : public GateAction {
	Ptr<O>  object;
	M       method;
	T             arg;

	bool    Execute() { return object? (object->*method)(arg) : false; }

	GateMethodActionArgPte(O *object, M method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class O, class M, class Q, class T>
Gate pteback1(O *object, bool (M::*method)(Q), T arg) {
	return Gate(new GateMethodActionArgPte<O, bool (M::*)(Q), T>(object, method, arg));
}

template <class O, class M, class T>
struct GateMethodActionArg : public GateAction {
	O  *object;
	M   method;
	T         arg;
	bool      Execute() { return object ? (object->*method)(arg) : false; }

	GateMethodActionArg(O *object, M method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class O, class M, class Q, class T>
Gate callback1(O *object, bool (M::*method)(Q), T arg) {
	return Gate(new GateMethodActionArg<O, bool (M::*)(Q), T>
	                         (object, method, arg));
}

template <class O, class M, class Q, class T>
Gate callback1(const O *object, bool (M::*method)(Q) const, T arg) {
	return Gate(new GateMethodActionArg<const O, bool (M::*)(Q) const, T>
	                         (object, method, arg));
}

template <class F, class T, class HC = F>
struct GateActionCallArg : public GateAction {
	F         fn;
	T         arg;
	bool    Execute() { return fn(arg); }

	GateActionCallArg(F fn, T arg) : fn(fn), arg(arg) {}
};

template <class Q, class T>
Gate callback1(bool (*fn)(Q), T arg)
{
	return Gate(new GateActionCallArg<bool (*)(Q), T, uintptr_t>(fn, arg));
}

//reduct
template <class Q, class T>
Gate callback1(Gate1<Q> cb, T arg)
{
	return Gate(new GateActionCallArg<Gate1<Q>, T>(cb, arg));
}

//1 arg to Gate1

template <class O, class M, class P1, class T>
struct Gate1MethodActionArgPte : public Gate1Action<P1> {
	Ptr<O>  object;
	M       method;
	T             arg;
	bool    Execute(P1 p1) { return object ? (object->*method)(p1, arg) : false; }

	Gate1MethodActionArgPte(O *object, M method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class O, class M, class P1, class Q, class T>
Gate1<P1> pteback1(O *object, bool (M::*method)(P1, Q), T arg) {
	return Gate1<P1>(new Gate1MethodActionArgPte<O, bool (M::*)(P1, Q), P1, T> (object, method, arg));
}

template <class O, class M, class P1, class T>
struct Gate1MethodActionArg : public Gate1Action<P1> {
	O  *object;
	M   method;
	T         arg;
	bool      Execute(P1 p1) { return object ? (object->*method)(p1, arg) : false; }

	Gate1MethodActionArg(O *object, M method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class O, class M, class P1, class Q, class T>
Gate1<P1> callback1(O *object, bool (M::*method)(P1, Q), T arg) {
	return Gate1<P1>(new Gate1MethodActionArg<O, bool (M::*)(P1, Q), P1, T>
	                         (object, method, arg));
}

template <class O, class M, class P1, class Q, class T>
Gate1<P1> callback1(const O *object, bool (M::*method)(P1, Q) const, T arg) {
	return Gate1<P1>(new Gate1MethodActionArg<const O, bool (M::*)(P1, Q) const, P1, T>
	                         (object, method, arg));
}

template <class F, class P1, class T, class HC = F>
struct Gate1ActionCallArg : public Gate1Action<P1> {
	F         fn;
	T         arg;
	bool    Execute(P1 p1) { return fn(p1, arg); }

	Gate1ActionCallArg(F fn, T arg) : fn(fn), arg(arg) {}
};

template <class P1, class Q, class T>
Gate1<P1> callback1(bool (*fn)(P1, Q), T arg)
{
	return Gate1<P1>(new Gate1ActionCallArg<bool (*)(P1, Q), P1, T, uintptr_t>(fn, arg));
}

//reduct
template <class P1, class Q, class T>
Gate1<P1> callback1(Gate2<P1, Q> cb, T arg) {
	return Gate1<P1>(new Gate1ActionCallArg<Gate2<P1, Q>, P1, T>(cb, arg));
}

//1 arg to Gate2

template <class O, class M, class P1, class P2, class T>
struct Gate2MethodActionArg : public Gate2Action<P1, P2> {
	O  *object;
	M   method;
	T         arg;
	bool    Execute(P1 p1, P2 p2) { return object ? (object->*method)(p1, p2, arg) : false; }

	Gate2MethodActionArg(O *object, M method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class O, class M, class P1, class P2, class Q, class T>
Gate2<P1, P2> callback1(O *object, bool (M::*method)(P1, P2, Q), T arg) {
	return Gate2<P1, P2>(new Gate2MethodActionArg<O, bool (M::*)(P1, P2, Q), P1, P2, T>
	                         (object, method, arg));
}

template <class O, class M, class P1, class P2, class Q, class T>
Gate2<P1, P2> callback1(const O *object, bool (M::*method)(P1, P2, Q) const, T arg) {
	return Gate2<P1, P2>(new Gate2MethodActionArg<const O, bool (M::*)(P1, P2, Q) const, P1, P2, T>
	                         (object, method, arg));
}

template <class F, class P1, class P2, class T, class HC = F>
struct Gate2ActionCallArg : Gate2Action<P1, P2> {
	F         fn;
	T         arg;
	bool    Execute(P1 p1, P2 p2) { return fn(p1, p2, arg); }

	Gate2ActionCallArg(F fn, T arg) : fn(fn), arg(arg) {}
};

template <class P1, class P2, class Q, class T>
Gate2<P1, P2> callback1(bool (*fn)(P1, P2, Q), T arg)
{
	return Gate2<P1, P2>(new Gate2ActionCallArg<bool (*)(P1, P2, Q), P1, P2, T, uintptr_t>(fn, arg));
}

//reduct
template <class P1, class P2, class Q, class T>
Gate2<P1, P2> callback1(Gate3<P1, P2, Q> cb, T arg) {
	return Gate2<P1, P2>(new Gate2ActionCallArg<Gate3<P1, P2, Q>, P1, P2, T>(cb, arg));
}

//1 arg to Gate3

template <class O, class M, class P1, class P2, class P3, class T>
struct Gate3MethodActionArg : public Gate3Action<P1, P2, P3> {
	O  *object;
	M   method;
	T         arg;
	bool    Execute(P1 p1, P2 p2, P3 p3) { return object ? (object->*method)(p1, p2, p3, arg) : false; }

	Gate3MethodActionArg(O *object, M method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class O, class M, class P1, class P2, class P3, class Q, class T>
Gate3<P1, P2, P3> callback1(O *object, bool (M::*method)(P1, P2, P3, Q), T arg) {
	return Gate3<P1, P2, P3>(new Gate3MethodActionArg<O, bool (M::*)(P1, P2, P3, Q), P1, P2, P3, T>
	                         (object, method, arg));
}

template <class O, class M, class P1, class P2, class P3, class Q, class T>
Gate3<P1, P2, P3> callback1(const O *object, bool (M::*method)(P1, P2, P3, Q) const, T arg) {
	return Gate3<P1, P2, P3>(new Gate3MethodActionArg<const O, bool (M::*)(P1, P2, P3, Q) const, P1, P2, P3, T>
	                         (object, method, arg));
}

template <class F, class P1, class P2, class P3, class T, class HC = F>
struct Gate3ActionCallArg : Gate3Action<P1, P2, P3> {
	F         fn;
	T         arg;
	bool    Execute(P1 p1, P2 p2, P3 p3) { return fn(p1, p2, p3, arg); }

	Gate3ActionCallArg(F fn, T arg) : fn(fn), arg(arg) {}
};

template <class P1, class P2, class P3, class Q, class T>
Gate3<P1, P2, P3> callback1(bool (*fn)(P1, P2, P3, Q), T arg)
{
	return Gate3<P1, P2, P3>(new Gate3ActionCallArg<bool (*)(P1, P2, P3, Q), P1, P2, P3, T, uintptr_t>(fn, arg));
}

//reduct
template <class P1, class P2, class P3, class Q, class T>
Gate3<P1, P2, P3> callback1(Gate4<P1, P2, P3, Q> cb, T arg) {
	return Gate3<P1, P2, P3>(new Gate3ActionCallArg<Gate4<P1, P2, P3, Q>, P1, P2, P3, T>(cb, arg));
}

//1 arg to Gate4

template <class O, class M, class P1, class P2, class P3, class P4, class T>
struct Gate4MethodActionArg : public Gate4Action<P1, P2, P3, P4> {
	O  *object;
	M   method;
	T         arg;
	bool    Execute(P1 p1, P2 p2, P3 p3, P4 p4) { return object ? (object->*method)(p1, p2, p3, p4, arg) : false; }

	Gate4MethodActionArg(O *object, M method, T arg)
	: object(object), method(method), arg(arg) {}
};

template <class O, class M, class P1, class P2, class P3, class P4, class Q, class T>
Gate4<P1, P2, P3, P4> callback1(O *object, bool (M::*method)(P1, P2, P3, P4, Q), T arg) {
	return Gate4<P1, P2, P3, P4>(new Gate4MethodActionArg<O, bool (M::*)(P1, P2, P3, P4, Q), P1, P2, P3, P4, T>
	                         (object, method, arg));
}

template <class O, class M, class P1, class P2, class P3, class P4, class Q, class T>
Gate4<P1, P2, P3, P4> callback1(const O *object, bool (M::*method)(P1, P2, P3, P4, Q) const, T arg) {
	return Gate4<P1, P2, P3, P4>(new Gate4MethodActionArg<const O, bool (M::*)(P1, P2, P3, P4, Q) const, P1, P2, P3, P4, T>
	                         (object, method, arg));
}

template <class F, class P1, class P2, class P3, class P4, class T, class HC = F>
struct Gate4ActionCallArg : Gate4Action<P1, P2, P3, P4> {
	F         fn;
	T         arg;
	bool    Execute(P1 p1, P2 p2, P3 p3, P4 p4) { return fn(p1, p2, p3, p4, arg); }

	Gate4ActionCallArg(F fn, T arg) : fn(fn), arg(arg) {}
};

template <class P1, class P2, class P3, class P4, class Q, class T>
Gate4<P1, P2, P3, P4> callback1(bool (*fn)(P1, P2, P3, P4, Q), T arg)
{
	return Gate4<P1, P2, P3, P4>(new Gate4ActionCallArg<bool (*)(P1, P2, P3, P4, Q), P1, P2, P3, P4, T, uintptr_t>(fn, arg));
}
