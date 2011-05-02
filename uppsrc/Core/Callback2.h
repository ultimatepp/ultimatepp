// 2 args to Callback

template <class O, class M, class T1, class T2>
struct CallbackMethodActionArg2Pte : public CallbackAction {
	Ptr<O>  object;
	M       method;
	T1            arg1;
	T2            arg2;
	void    Execute() { if(object) (object->*method)(arg1, arg2); }

	CallbackMethodActionArg2Pte(O *object, M method, T1 arg1, T2 arg2)
	: object(object), method(method), arg1(arg1), arg2(arg2) {}
};

/* wrong type R?
template <class O, class R, class M, class Q1, class Q2, class T1, class T2>
Callback pteback2(O *object, R (M::*method)(Q1, Q2), T1 arg1, T2 arg2) {
	return Callback(new CallbackMethodActionArg2Pte<O, R (M::*)(Q1, Q2), T1, T2>
	                    (object, method, arg1, arg2));
}
*/

template <class O, class M, class Q1, class Q2, class T1, class T2>
Callback pteback2(O *object, void (M::*method)(Q1, Q2), T1 arg1, T2 arg2) {
	return Callback(new CallbackMethodActionArg2Pte<O, void (M::*)(Q1, Q2), T1, T2>
	                    (object, method, arg1, arg2));
}

template <class O, class M, class T1, class T2>
struct CallbackMethodActionArg2 : public CallbackAction {
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	void    Execute() { (object->*method)(arg1, arg2); }

	CallbackMethodActionArg2(O *object, M method, T1 arg1, T2 arg2)
	: object(object), method(method), arg1(arg1), arg2(arg2) {}
};

/* wrong type R?
template <class O, class R, class M, class Q1, class Q2, class T1, class T2>
Callback callback2(O *object, R (M::*method)(Q1, Q2), T1 arg1, T2 arg2) {
	return Callback(new CallbackMethodActionArg2<O, R (M::*)(Q1, Q2), T1, T2>
	                    (object, method, arg1, arg2));
}

template <class O, class R, class M, class Q1, class Q2, class T1, class T2>
Callback callback2(const O *object, R (M::*method)(Q1, Q2) const, T1 arg1, T2 arg2) {
	return Callback(new CallbackMethodActionArg2<const O, R (M::*)(Q1, Q2) const, T1, T2>
	                    (object, method, arg1, arg2));
}
*/

template <class O, class M, class Q1, class Q2, class T1, class T2>
Callback callback2(O *object, void (M::*method)(Q1, Q2), T1 arg1, T2 arg2) {
	return Callback(new CallbackMethodActionArg2<O, void (M::*)(Q1, Q2), T1, T2>
	                    (object, method, arg1, arg2));
}

template <class O, class M, class Q1, class Q2, class T1, class T2>
Callback callback2(const O *object, void (M::*method)(Q1, Q2) const, T1 arg1, T2 arg2) {
	return Callback(new CallbackMethodActionArg2<const O, void (M::*)(Q1, Q2) const, T1, T2>
	                    (object, method, arg1, arg2));
}

template <class X, class T1, class T2, class HC = X>
struct CallbackActionCallArg2 : public CallbackAction {
	X         x;
	T1        arg1;
	T2        arg2;
	void    Execute() { x(arg1, arg2); }

	CallbackActionCallArg2(X x, T1 arg1, T2 arg2) : x(x), arg1(arg1), arg2(arg2) {}
};

/* wrong type R?
template <class R, class Q1, class Q2, class T1, class T2>
Callback callback2(R (*fn)(Q1, Q2), T1 arg1, T2 arg2) {
	return Callback(new CallbackActionCallArg2<R (*)(Q1, Q2), T1, T2, uintptr_t>(fn, arg1, arg2));
}
*/

template <class Q1, class Q2, class T1, class T2>
Callback callback2(void (*fn)(Q1, Q2), T1 arg1, T2 arg2) {
	return Callback(new CallbackActionCallArg2<void (*)(Q1, Q2), T1, T2, uintptr_t>(fn, arg1, arg2));
}

template <class Q1, class Q2, class T1, class T2>
Callback callback2(Callback2<Q1, Q2> cb, T1 arg1, T2 arg2) {
	return Callback(new CallbackActionCallArg2<Callback2<Q1, Q2>, T1, T2>(cb, arg1, arg2));
}

//2 args to Callback1

template <class O, class M, class P1, class T1, class T2>
struct Callback1MethodActionArg2 : public Callback1Action<P1> {
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	void    Execute(P1 p1) { (object->*method)(p1, arg1, arg2); }

	Callback1MethodActionArg2(O *object, M method, T1 arg1, T2 arg2)
	: object(object), method(method), arg1(arg1), arg2(arg2) {}
};

template <class O, class M, class P1, class Q1, class Q2, class T1, class T2>
Callback1<P1> callback2(O *object, void (M::*method)(P1, Q1, Q2), T1 arg1, T2 arg2) {
	return Callback1<P1>(new Callback1MethodActionArg2<O, void (M::*)(P1, Q1, Q2), P1, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class O, class M, class P1, class Q1, class Q2, class T1, class T2>
Callback1<P1> callback2(const O *object, void (M::*method)(P1, Q1, Q2) const, T1 arg1, T2 arg2) {
	return Callback1<P1>(new Callback1MethodActionArg2<const O, void (M::*)(P1, Q1, Q2) const, P1, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class X, class P1, class T1, class T2, class HC = X>
struct Callback1ActionCallArg2 : public Callback1Action<P1> {
	X         x;
	T1        arg1;
	T2        arg2;
	void    Execute(P1 p1) { x(p1, arg1, arg2); }

	Callback1ActionCallArg2(X x, T1 arg1, T2 arg2) : x(x), arg1(arg1), arg2(arg2) {}
};

/* wrong type R?
template <class R, class P1, class Q1, class Q2, class T1, class T2>
Callback1<P1> callback2(R (*fn)(P1, Q1, Q2), T1 arg1, T2 arg2) {
	return Callback1<P1>(new Callback1ActionCallArg2<R (*)(P1, Q1, Q2), P1, T1, T2, uintptr_t>(fn, arg1, arg2));
}
*/

template <class P1, class Q1, class Q2, class T1, class T2>
Callback1<P1> callback2(void (*fn)(P1, Q1, Q2), T1 arg1, T2 arg2) {
	return Callback1<P1>(new Callback1ActionCallArg2<void (*)(P1, Q1, Q2), P1, T1, T2, uintptr_t>(fn, arg1, arg2));
}

template <class P1, class Q1, class Q2, class T1, class T2>
Callback1<P1> callback2(Callback3<P1, Q1, Q2> cb, T1 arg1, T2 arg2) {
	return Callback1<P1>(new Callback1ActionCallArg2<Callback3<P1, Q1, Q2>, P1, T1, T2>(cb, arg1, arg2));
}

//2 args to Callback2

template <class O, class M, class P1, class P2, class T1, class T2>
struct Callback2MethodActionArg2 : public Callback2Action<P1, P2> {
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	void    Execute(P1 p1, P2 p2) { (object->*method)(p1, p2, arg1, arg2); }

	Callback2MethodActionArg2(O *object, M method, T1 arg1, T2 arg2)
	: object(object), method(method), arg1(arg1), arg2(arg2) {}
};

template <class O, class M, class P1, class P2, class Q1, class Q2, class T1, class T2>
Callback2<P1, P2> callback2(O *object, void (M::*method)(P1, P2, Q1, Q2), T1 arg1, T2 arg2) {
	return Callback2<P1, P2>(new Callback2MethodActionArg2<O, void (M::*)(P1, P2, Q1, Q2), P1, P2, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class O, class M, class P1, class P2, class Q1, class Q2, class T1, class T2>
Callback2<P1, P2> callback2(const O *object, void (M::*method)(P1, P2, Q1, Q2) const, T1 arg1, T2 arg2) {
	return Callback2<P1, P2>(new Callback2MethodActionArg2<const O, void (M::*)(P1, P2, Q1, Q2) const, P1, P2, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class X, class P1, class P2, class T1, class T2, class HC = X>
struct Callback2ActionCallArg2 : public Callback2Action<P1, P2> {
	X         x;
	T1        arg1;
	T2        arg2;
	void    Execute(P1 p1, P2 p2) { x(p1, p2, arg1, arg2); }

	Callback2ActionCallArg2(X x, T1 arg1, T2 arg2) : x(x), arg1(arg1), arg2(arg2) {}
};

/* wrong type R?
template <class R, class P1, class P2, class Q1, class Q2, class T1, class T2>
Callback2<P1, P2> callback2(R (*fn)(P1, P2, Q1, Q2), T1 arg1, T2 arg2) {
	return Callback2<P1, P2>(new Callback2ActionCallArg2<R (*)(P1, P2, Q1, Q2), P1, P2, T1, T2, uintptr_t>(fn, arg1, arg2));
}
*/

template <class P1, class P2, class Q1, class Q2, class T1, class T2>
Callback2<P1, P2> callback2(void (*fn)(P1, P2, Q1, Q2), T1 arg1, T2 arg2) {
	return Callback2<P1, P2>(new Callback2ActionCallArg2<void (*)(P1, P2, Q1, Q2), P1, P2, T1, T2, uintptr_t>(fn, arg1, arg2));
}

template <class P1, class P2, class Q1, class Q2, class T1, class T2>
Callback2<P1, P2> callback2(Callback4<P1, P2, Q1, Q2> cb, T1 arg1, T2 arg2) {
	return Callback2<P1, P2>(new Callback2ActionCallArg2<Callback4<P1, P2, Q1, Q2>, P1, P2, T1, T2>(cb, arg1, arg2));
}

//2 args to Callback3

template <class O, class M, class P1, class P2, class P3, class T1, class T2>
struct Callback3MethodActionArg2 : public Callback3Action<P1, P2, P3> {
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	void    Execute(P1 p1, P2 p2, P3 p3) { (object->*method)(p1, p2, p3, arg1, arg2); }

	Callback3MethodActionArg2(O *object, M method, T1 arg1, T2 arg2)
	: object(object), method(method), arg1(arg1), arg2(arg2) {}
};

template <class O, class M, class P1, class P2, class P3, class Q1, class Q2, class T1, class T2>
Callback3<P1, P2, P3> callback2(O *object, void (M::*method)(P1, P2, P3, Q1, Q2), T1 arg1, T2 arg2) {
	return Callback3<P1, P2, P3>(new Callback3MethodActionArg2<O, void (M::*)(P1, P2, P3, Q1, Q2), P1, P2, P3, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class O, class M, class P1, class P2, class P3, class Q1, class Q2, class T1, class T2>
Callback3<P1, P2, P3> callback2(const O *object, void (M::*method)(P1, P2, P3, Q1, Q2) const, T1 arg1, T2 arg2) {
	return Callback3<P1, P2, P3>(new Callback3MethodActionArg2<const O, void (M::*)(P1, P2, P3, Q1, Q2) const, P1, P2, P3, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class X, class P1, class P2, class P3, class T1, class T2, class HC = X>
struct Callback3ActionCallArg2 : public Callback3Action<P1, P2, P3> {
	X         x;
	T1        arg1;
	T2        arg2;
	void    Execute(P1 p1, P2 p2, P3 p3) { x(p1, p2, p3, arg1, arg2); }

	Callback3ActionCallArg2(X x, T1 arg1, T2 arg2) : x(x), arg1(arg1), arg2(arg2) {}
};

/* wrong type R?
template <class R, class P1, class P2, class P3, class Q1, class Q2, class T1, class T2>
Callback3<P1, P2, P3> callback2(R (*fn)(P1, P2, P3, Q1, Q2), T1 arg1, T2 arg2) {
	return Callback3<P1, P2, P3>(new Callback3ActionCallArg2<R (*)(P1, P2, P3, Q1, Q2), P1, P2, P3, T1, T2, uintptr_t>(fn, arg1, arg2));
}
*/

template <class P1, class P2, class P3, class Q1, class Q2, class T1, class T2>
Callback3<P1, P2, P3> callback2(void (*fn)(P1, P2, P3, Q1, Q2), T1 arg1, T2 arg2) {
	return Callback3<P1, P2, P3>(new Callback3ActionCallArg2<void (*)(P1, P2, P3, Q1, Q2), P1, P2, P3, T1, T2, uintptr_t>(fn, arg1, arg2));
}

//2 args to Callback4

template <class O, class M, class P1, class P2, class P3, class P4, class T1, class T2>
struct Callback4MethodActionArg2 : public Callback4Action<P1, P2, P3, P4> {
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	void    Execute(P1 p1, P2 p2, P3 p3, P4 p4) { (object->*method)(p1, p2, p3, p4, arg1, arg2); }

	Callback4MethodActionArg2(O *object, M method, T1 arg1, T2 arg2)
	: object(object), method(method), arg1(arg1), arg2(arg2) {}
};

template <class O, class M, class P1, class P2, class P3, class P4, class Q1, class Q2, class T1, class T2>
Callback4<P1, P2, P3, P4> callback2(O *object, void (M::*method)(P1, P2, P3, P4, Q1, Q2), T1 arg1, T2 arg2) {
	return Callback4<P1, P2, P3, P4>(new Callback4MethodActionArg2<O, void (M::*)(P1, P2, P3, P4, Q1, Q2), P1, P2, P3, P4, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class O, class M, class P1, class P2, class P3, class P4, class Q1, class Q2, class T1, class T2>
Callback4<P1, P2, P3, P4> callback2(const O *object, void (M::*method)(P1, P2, P3, P4, Q1, Q2) const, T1 arg1, T2 arg2) {
	return Callback4<P1, P2, P3, P4>(new Callback4MethodActionArg2<const O, void (M::*)(P1, P2, P3, P4, Q1, Q2) const, P1, P2, P3, P4, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class X, class P1, class P2, class P3, class P4, class T1, class T2, class HC = X>
struct Callback4ActionCallArg2 : public Callback4Action<P1, P2, P3, P4> {
	X         x;
	T1        arg1;
	T2        arg2;
	void    Execute(P1 p1, P2 p2, P3 p3, P4 p4) { x(p1, p2, p3, p4, arg1, arg2); }

	Callback4ActionCallArg2(X x, T1 arg1, T2 arg2) : x(x), arg1(arg1), arg2(arg2) {}
};

/* wrong type R?
template <class R, class P1, class P2, class P3, class P4, class Q1, class Q2, class T1, class T2>
Callback4<P1, P2, P3, P4> callback2(R (*fn)(P1, P2, P3, P4, Q1, Q2), T1 arg1, T2 arg2) {
	return Callback4<P1, P2, P3, P4>(new Callback4ActionCallArg2<R (*)(P1, P2, P3, P4, Q1, Q2), P1, P2, P3, P4, T1, T2, uintptr_t>(fn, arg1, arg2));
}
*/

template <class P1, class P2, class P3, class P4, class Q1, class Q2, class T1, class T2>
Callback4<P1, P2, P3, P4> callback2(void (*fn)(P1, P2, P3, P4, Q1, Q2), T1 arg1, T2 arg2) {
	return Callback4<P1, P2, P3, P4>(new Callback4ActionCallArg2<void (*)(P1, P2, P3, P4, Q1, Q2), P1, P2, P3, P4, T1, T2, uintptr_t>(fn, arg1, arg2));
}

// ---------------------------------------------------------

// 2 args to Gate

template <class O, class M, class T1, class T2>
struct GateMethodActionArg2Pte : public GateAction {
	Ptr<O>  object;
	M       method;
	T1            arg1;
	T2            arg2;
	bool    Execute() { return object ? (object->*method)(arg1, arg2) : false; }

	GateMethodActionArg2Pte(O *object, M method, T1 arg1, T2 arg2)
	: object(object), method(method), arg1(arg1), arg2(arg2) {}
};

/* ambigiuous with Callback versions

template <class O, class R, class M, class Q1, class Q2, class T1, class T2>
Gate pteback2(O *object, R (M::*method)(Q1, Q2), T1 arg1, T2 arg2) {
	return Gate(new GateMethodActionArg2Pte<O, R (M::*)(Q1, Q2), T1, T2>
	                    (object, method, arg1, arg2));
}
*/

template <class O, class M, class Q1, class Q2, class T1, class T2>
Gate pteback2(O *object, bool (M::*method)(Q1, Q2), T1 arg1, T2 arg2) {
	return Gate(new GateMethodActionArg2Pte<O, bool (M::*)(Q1, Q2), T1, T2>
	                    (object, method, arg1, arg2));
}

template <class O, class M, class T1, class T2>
struct GateMethodActionArg2 : public GateAction {
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	bool    Execute() { return object ? (object->*method)(arg1, arg2) : false; }

	GateMethodActionArg2(O *object, M method, T1 arg1, T2 arg2)
	: object(object), method(method), arg1(arg1), arg2(arg2) {}
};

/* ambigiuous with Callback versions

template <class O, class R, class M, class Q1, class Q2, class T1, class T2>
Gate callback2(O *object, R (M::*method)(Q1, Q2), T1 arg1, T2 arg2) {
	return Gate(new GateMethodActionArg2<O, R (M::*)(Q1, Q2), T1, T2>
	                    (object, method, arg1, arg2));
}

template <class O, class R, class M, class Q1, class Q2, class T1, class T2>
Gate callback2(const O *object, R (M::*method)(Q1, Q2) const, T1 arg1, T2 arg2) {
	return Gate(new GateMethodActionArg2<const O, R (M::*)(Q1, Q2) const, T1, T2>
	                    (object, method, arg1, arg2));
}
*/

template <class O, class M, class Q1, class Q2, class T1, class T2>
Gate callback2(O *object, bool (M::*method)(Q1, Q2), T1 arg1, T2 arg2) {
	return Gate(new GateMethodActionArg2<O, bool (M::*)(Q1, Q2), T1, T2>
	                    (object, method, arg1, arg2));
}

template <class O, class M, class Q1, class Q2, class T1, class T2>
Gate callback2(const O *object, bool (M::*method)(Q1, Q2) const, T1 arg1, T2 arg2) {
	return Gate(new GateMethodActionArg2<const O, bool (M::*)(Q1, Q2) const, T1, T2>
	                    (object, method, arg1, arg2));
}

template <class X, class T1, class T2, class HC = X>
struct GateActionCallArg2 : public GateAction {
	X         x;
	T1        arg1;
	T2        arg2;
	bool    Execute() { return x(arg1, arg2); }

	GateActionCallArg2(X x, T1 arg1, T2 arg2) : x(x), arg1(arg1), arg2(arg2) {}
};

/* ambigiuous with Callback versions

template <class R, class Q1, class Q2, class T1, class T2>
Gate callback2(R (*fn)(Q1, Q2), T1 arg1, T2 arg2) {
	return Gate(new GateActionCallArg2<R (*)(Q1, Q2), T1, T2, uintptr_t>(fn, arg1, arg2));
}
*/

template <class Q1, class Q2, class T1, class T2>
Gate callback2(bool (*fn)(Q1, Q2), T1 arg1, T2 arg2) {
	return Gate(new GateActionCallArg2<bool (*)(Q1, Q2), T1, T2, uintptr_t>(fn, arg1, arg2));
}

template <class Q1, class Q2, class T1, class T2>
Gate callback2(Gate2<Q1, Q2> cb, T1 arg1, T2 arg2) {
	return Gate(new GateActionCallArg2<Gate2<Q1, Q2>, T1, T2>(cb, arg1, arg2));
}

//2 args to Gate1

template <class O, class M, class P1, class T1, class T2>
struct Gate1MethodActionArg2 : public Gate1Action<P1> {
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	bool    Execute(P1 p1) { return object ? (object->*method)(p1, arg1, arg2) : false; }

	Gate1MethodActionArg2(O *object, M method, T1 arg1, T2 arg2)
	: object(object), method(method), arg1(arg1), arg2(arg2) {}
};

template <class O, class M, class P1, class Q1, class Q2, class T1, class T2>
Gate1<P1> callback2(O *object, bool (M::*method)(P1, Q1, Q2), T1 arg1, T2 arg2) {
	return Gate1<P1>(new Gate1MethodActionArg2<O, bool (M::*)(P1, Q1, Q2), P1, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class O, class M, class P1, class Q1, class Q2, class T1, class T2>
Gate1<P1> callback2(const O *object, bool (M::*method)(P1, Q1, Q2) const, T1 arg1, T2 arg2) {
	return Gate1<P1>(new Gate1MethodActionArg2<const O, bool (M::*)(P1, Q1, Q2) const, P1, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class X, class P1, class T1, class T2, class HC = X>
struct Gate1ActionCallArg2 : public Gate1Action<P1> {
	X         x;
	T1        arg1;
	T2        arg2;
	bool    Execute(P1 p1) { return x(p1, arg1, arg2); }

	Gate1ActionCallArg2(X x, T1 arg1, T2 arg2) : x(x), arg1(arg1), arg2(arg2) {}
};

/* ambigiuous with Callback versions

template <class R, class P1, class Q1, class Q2, class T1, class T2>
Gate1<P1> callback2(R (*fn)(P1, Q1, Q2), T1 arg1, T2 arg2) {
	return Gate1<P1>(new Gate1ActionCallArg2<R (*)(P1, P2, Q3), P1, T1, T2, uintptr_t>(fn, arg1, arg2));
}
*/

template <class P1, class Q1, class Q2, class T1, class T2>
Gate1<P1> callback2(bool (*fn)(P1, Q1, Q2), T1 arg1, T2 arg2) {
	return Gate1<P1>(new Gate1ActionCallArg2<bool (*)(P1, Q1, Q2), P1, T1, T2, uintptr_t>(fn, arg1, arg2));
}

template <class P1, class Q1, class Q2, class T1, class T2>
Gate1<P1> callback2(Gate3<P1, Q1, Q2> cb, T1 arg1, T2 arg2) {
	return Gate1<P1>(new Gate1ActionCallArg2<Gate3<P1, Q1, Q2>, P1, T1, T2>(cb, arg1, arg2));
}

//2 args to Gate2

template <class O, class M, class P1, class P2, class T1, class T2>
struct Gate2MethodActionArg2 : public Gate2Action<P1, P2> {
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	bool    Execute(P1 p1, P2 p2) { return object ? (object->*method)(p1, p2, arg1, arg2) : false; }

	Gate2MethodActionArg2(O *object, M method, T1 arg1, T2 arg2)
	: object(object), method(method), arg1(arg1), arg2(arg2) {}
};

template <class O, class M, class P1, class P2, class Q1, class Q2, class T1, class T2>
Gate2<P1, P2> callback2(O *object, bool (M::*method)(P1, P2, Q1, Q2), T1 arg1, T2 arg2) {
	return Gate2<P1, P2>(new Gate2MethodActionArg2<O, bool (M::*)(P1, P2, Q1, Q2), P1, P2, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class O, class M, class P1, class P2, class Q1, class Q2, class T1, class T2>
Gate2<P1, P2> callback2(const O *object, bool (M::*method)(P1, P2, Q1, Q2) const, T1 arg1, T2 arg2) {
	return Gate2<P1, P2>(new Gate2MethodActionArg2<const O, bool (M::*)(P1, P2, Q1, Q2) const, P1, P2, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class X, class P1, class P2, class T1, class T2, class HC = X>
struct Gate2ActionCallArg2 : public Gate2Action<P1, P2> {
	X         x;
	T1        arg1;
	T2        arg2;
	bool    Execute(P1 p1, P2 p2) { return x(p1, p2, arg1, arg2); }

	Gate2ActionCallArg2(X x, T1 arg1, T2 arg2) : x(x), arg1(arg1), arg2(arg2) {}
};

/* ambigiuous with Callback versions

template <class R, class P1, class P2, class Q1, class Q2, class T1, class T2>
Gate2<P1, P2> callback2(R (*fn)(P1, P2, Q1, Q2), T1 arg1, T2 arg2) {
	return Gate2<P1, P2>(new Gate2ActionCallArg2<R (*)(P1, P2, Q1, Q1), P1, P2, T1, T2, uintptr_t>(fn, arg1, arg2));
}
*/

template <class P1, class P2, class Q1, class Q2, class T1, class T2>
Gate2<P1, P2> callback2(bool (*fn)(P1, P2, Q1, Q2), T1 arg1, T2 arg2) {
	return Gate2<P1, P2>(new Gate2ActionCallArg2<bool (*)(P1, P2, Q1, Q2), P1, P2, T1, T2, uintptr_t>(fn, arg1, arg2));
}

template <class P1, class P2, class Q1, class Q2, class T1, class T2>
Gate2<P1, P2> callback2(Gate4<P1, P2, Q1, Q2> cb, T1 arg1, T2 arg2) {
	return Gate2<P1, P2>(new Gate2ActionCallArg2<Gate4<P1, P2, Q1, Q2>, P1, P2, T1, T2>(cb, arg1, arg2));
}

//2 args to Gate3

template <class O, class M, class P1, class P2, class P3, class T1, class T2>
struct Gate3MethodActionArg2 : public Gate3Action<P1, P2, P3> {
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	bool    Execute(P1 p1, P2 p2, P3 p3) { return object ? (object->*method)(p1, p2, p3, arg1, arg2) : false; }

	Gate3MethodActionArg2(O *object, M method, T1 arg1, T2 arg2)
	: object(object), method(method), arg1(arg1), arg2(arg2) {}
};

template <class O, class M, class P1, class P2, class P3, class Q1, class Q2, class T1, class T2>
Gate3<P1, P2, P3> callback2(O *object, bool (M::*method)(P1, P2, P3, Q1, Q2), T1 arg1, T2 arg2) {
	return Gate3<P1, P2, P3>(new Gate3MethodActionArg2<O, bool (M::*)(P1, P2, P3, Q1, Q2), P1, P2, P3, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class O, class M, class P1, class P2, class P3, class Q1, class Q2, class T1, class T2>
Gate3<P1, P2, P3> callback2(const O *object, bool (M::*method)(P1, P2, P3, Q1, Q2) const, T1 arg1, T2 arg2) {
	return Gate3<P1, P2, P3>(new Gate3MethodActionArg2<const O, bool (M::*)(P1, P2, P3, Q1, Q2) const, P1, P2, P3, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class X, class P1, class P2, class P3, class T1, class T2, class HC = X>
struct Gate3ActionCallArg2 : public Gate3Action<P1, P2, P3> {
	X         x;
	T1        arg1;
	T2        arg2;
	bool    Execute(P1 p1, P2 p2, P3 p3) { return x(p1, p2, p3, arg1, arg2); }

	Gate3ActionCallArg2(X x, T1 arg1, T2 arg2) : x(x), arg1(arg1), arg2(arg2) {}
};

/* ambigiuous with Callback versions

template <class R, class P1, class P2, class P3, class Q1, class Q2, class T1, class T2>
Gate3<P1, P2, P3> callback2(R (*fn)(P1, P2, P3, Q1, Q2), T1 arg1, T2 arg2) {
	return Gate3<P1, P2, P3>(new Gate3ActionCallArg2<R (*)(P1, P2, P3, Q1, Q2), P1, P2, P3, T1, T2, uintptr_t>(fn, arg1, arg2));
}
*/

template <class P1, class P2, class P3, class Q1, class Q2, class T1, class T2>
Gate3<P1, P2, P3> callback2(bool (*fn)(P1, P2, P3, Q1, Q2), T1 arg1, T2 arg2) {
	return Gate3<P1, P2, P3>(new Gate3ActionCallArg2<bool (*)(P1, P2, P3, Q1, Q2), P1, P2, P3, T1, T2, uintptr_t>(fn, arg1, arg2));
}

//2 args to Gate4

template <class O, class M, class P1, class P2, class P3, class P4, class T1, class T2>
struct Gate4MethodActionArg2 : public Gate4Action<P1, P2, P3, P4> {
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	bool    Execute(P1 p1, P2 p2, P3 p3, P4 p4) { return object ? (object->*method)(p1, p2, p3, p4, arg1, arg2) : false; }

	Gate4MethodActionArg2(O *object, M method, T1 arg1, T2 arg2)
	: object(object), method(method), arg1(arg1), arg2(arg2) {}
};

template <class O, class M, class P1, class P2, class P3, class P4, class Q1, class Q2, class T1, class T2>
Gate4<P1, P2, P3, P4> callback2(O *object, bool (M::*method)(P1, P2, P3, P4, Q1, Q2), T1 arg1, T2 arg2) {
	return Gate4<P1, P2, P3, P4>(new Gate4MethodActionArg2<O, bool (M::*)(P1, P2, P3, P4, Q1, Q2), P1, P2, P3, P4, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class O, class M, class P1, class P2, class P3, class P4, class Q1, class Q2, class T1, class T2>
Gate4<P1, P2, P3, P4> callback2(const O *object, bool (M::*method)(P1, P2, P3, P4, Q1, Q2) const, T1 arg1, T2 arg2) {
	return Gate4<P1, P2, P3, P4>(new Gate4MethodActionArg2<const O, bool (M::*)(P1, P2, P3, P4, Q1, Q2) const, P1, P2, P3, P4, T1, T2>
	                         (object, method, arg1, arg2));
}

template <class X, class P1, class P2, class P3, class P4, class T1, class T2, class HC = X>
struct Gate4ActionCallArg2 : public Gate4Action<P1, P2, P3, P4> {
	X         x;
	T1        arg1;
	T2        arg2;
	bool    Execute(P1 p1, P2 p2, P3 p3, P4 p4) { return x(p1, p2, p3, p4, arg1, arg2); }

	Gate4ActionCallArg2(X x, T1 arg1, T2 arg2) : x(x), arg1(arg1), arg2(arg2) {}
};

/* ambigiuous with Callback versions

template <class R, class P1, class P2, class P3, class P4, class Q1, class Q2, class T1, class T2>
Gate4<P1, P2, P3, P4> callback2(R (*fn)(P1, P2, P3, P4, Q1, Q2), T1 arg1, T2 arg2) {
	return Gate4<P1, P2, P3, P4>(new Gate4ActionCallArg2<R (*)(P1, P2, P3, P4, Q1, Q2), P1, P2, P3, P4, T1, T2, uintptr_t>(fn, arg1, arg2));
}
*/

template <class P1, class P2, class P3, class P4, class Q1, class Q2, class T1, class T2>
Gate4<P1, P2, P3, P4> callback2(bool (*fn)(P1, P2, P3, P4, Q1, Q2), T1 arg1, T2 arg2) {
	return Gate4<P1, P2, P3, P4>(new Gate4ActionCallArg2<bool (*)(P1, P2, P3, P4, Q1, Q2), P1, P2, P3, P4, T1, T2, uintptr_t>(fn, arg1, arg2));
}
