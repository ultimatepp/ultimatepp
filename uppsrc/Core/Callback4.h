//4 args to Callback

template <class O, class M, class T1, class T2, class T3, class T4>
struct CallbackMethodActionArg4Pte : public CallbackAction {
	Ptr<O>  object;
	M       method;
	T1      arg1;
	T2      arg2;
	T3      arg3;
	T4      arg4;
	void    Execute() { if(object) (object->*method)(arg1, arg2, arg3, arg4); }

	CallbackMethodActionArg4Pte(O *object, M method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(pick(arg1)), arg2(pick(arg2)), arg3(pick(arg3)), arg4(pick(arg4)) {}
};

template <class O, class M, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Callback pteback4(O *object, void (M::*method)(Q1, Q2, Q3, Q4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback(new CallbackMethodActionArg4Pte<O, void (M::*)(Q1, Q2, Q3, Q4), T1, T2, T3, T4>
	                    (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Callback pteback4(O *object, void (M::*method)(Q1, Q2, Q3, Q4) const, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback(new CallbackMethodActionArg4Pte<O, void (M::*)(Q1, Q2, Q3, Q4) const, T1, T2, T3, T4>
	                    (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class T1, class T2, class T3, class T4>
struct CallbackMethodActionArg4 : public CallbackAction 
{
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	T4        arg4;
	
	void    Execute() { (object->*method)(arg1, arg2, arg3, arg4); }

	CallbackMethodActionArg4(O *object, M method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(pick(arg1)), arg2(pick(arg2)), arg3(pick(arg3)), arg4(pick(arg4)) {}
};

template <class O, class M, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4> 
Callback callback4(O *object, void (M::*method)(Q1, Q2, Q3, Q4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) 
{
	return Callback(
		new CallbackMethodActionArg4<O, void (M::*)(Q1, Q2, Q3, Q4), T1, T2, T3, T4>(object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4> 
Callback callback4(O *object, void (M::*method)(Q1, Q2, Q3, Q4) const, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback(new CallbackMethodActionArg4<O, void (M::*)(Q1, Q2, Q3, Q4) const, T1, T2, T3, T4>
	                    (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class X, class T1, class T2, class T3, class T4, class HC = X>
struct CallbackActionCallArg4 : public CallbackAction {
	X       x;
	T1      arg1;
	T2      arg2;
	T3      arg3;
	T4      arg4;
	void    Execute() { x(arg1, arg2, arg3, arg4); }

	CallbackActionCallArg4(X x, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
		: x(x), arg1(pick(arg1)), arg2(pick(arg2)), arg3(pick(arg3)), arg4(pick(arg4)) {}
};

template <class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Callback callback4(void (*fn)(Q1, Q2, Q3, Q4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback(
		new CallbackActionCallArg4<void (*)(Q1, Q2, Q3, Q4), T1, T2, T3, T4, uintptr_t>(fn, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Callback callback4(Callback4<Q1, Q2, Q3, Q4> cb, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback(
		new CallbackActionCallArg4<Callback4<Q1, Q2, Q3, Q4>, T1, T2, T3, T4>(cb, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

//4 args to Callback1

template <class X, class P1, class T1, class T2, class T3, class T4>
struct Callback1FnActionArg4 : public Callback1Action<P1> {
	X       fn;
	T1      arg1;
	T2      arg2;
	T3      arg3;
	T4      arg4;
	void    Execute(P1 p1) { fn(p1, arg1, arg2, arg3, arg4); }

	Callback1FnActionArg4(X fn, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: fn(fn), arg1(pick(arg1)), arg2(pick(arg2)), arg3(pick(arg3)), arg4(pick(arg4)) {}
};

template <class P1, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Callback1<P1> callback4(void (*fn)(P1, Q1, Q2, Q3, Q4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback1<P1>(new Callback1FnActionArg4<void (*)(P1, Q1, Q2, Q3, Q4), P1, T1, T2, T3, T4>
	                         (fn, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class P1, class T1, class T2, class T3, class T4>
struct Callback1MethodActionArg4 : public Callback1Action<P1> {
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	T4        arg4;
	void    Execute(P1 p1) { (object->*method)(p1, arg1, arg2, arg3, arg4); }

	Callback1MethodActionArg4(O *object, M method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(pick(arg1)), arg2(pick(arg2)), arg3(pick(arg3)), arg4(pick(arg4)) {}
};

template <class O, class M, class P1, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Callback1<P1> callback4(O *object, void (M::*method)(P1, Q1, Q2, Q3, Q4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback1<P1>(new Callback1MethodActionArg4<O, void (M::*)(P1, Q1, Q2, Q3, Q4), P1, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class P1, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Callback1<P1> callback4(O *object, void (M::*method)(P1, Q1, Q2, Q3, Q4) const, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback1<P1>(new Callback1MethodActionArg4<O, void (M::*)(P1, Q1, Q2, Q3, Q4) const, P1, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class P1, class T1, class T2, class T3, class T4>
struct Callback1MethodActionArg4Pte : public Callback1Action<P1> {
	Ptr<O>  object;
	M       method;
	T1      arg1;
	T2      arg2;
	T3      arg3;
	T4      arg4;
	void    Execute(P1 p1) { (object->*method)(p1, arg1, arg2, arg3, arg4); }

	Callback1MethodActionArg4Pte(O *object, M method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(pick(arg1)), arg2(pick(arg2)), arg3(pick(arg3)), arg4(pick(arg4)) {}
};

template <class O, class M, class P1, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Callback1<P1> pteback4(O *object, void (M::*method)(P1, Q1, Q2, Q3, Q4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback1<P1>(new Callback1MethodActionArg4Pte<O, void (M::*)(P1, Q1, Q2, Q3, Q4), P1, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class P1, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Callback1<P1> pteback4(O *object, void (M::*method)(P1, Q1, Q2, Q3, Q4) const, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback1<P1>(new Callback1MethodActionArg4Pte<O, void (M::*)(P1, Q1, Q2, Q3, Q4) const, P1, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

//4 args to Callback2 (obsolete - we only provide up to 5 parameters of receiving function)

template <class O, class M, class P1, class P2, class T1, class T2, class T3, class T4>
struct Callback2MethodActionArg4 : public Callback2Action<P1, P2> {
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	T4        arg4;
	void    Execute(P1 p1, P2 p2) { (object->*method)(p1, p2, arg1, arg2, arg3, arg4); }

	Callback2MethodActionArg4(O *object, M method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(pick(arg1)), arg2(pick(arg2)), arg3(pick(arg3)), arg4(pick(arg4)) {}
};

template <class O, class M, class P1, class P2, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Callback2<P1, P2> callback4(O *object, void (M::*method)(P1, P2, Q1, Q2, Q3, Q4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback2<P1, P2>(new Callback2MethodActionArg4<O, void (M::*)(P1, P2, Q1, Q2, Q3, Q4), P1, P2, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class P1, class P2, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Callback2<P1, P2> callback4(O *object, void (M::*method)(P1, P2, Q1, Q2, Q3, Q4) const, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback2<P1, P2>(new Callback2MethodActionArg4<O, void (M::*)(P1, P2, Q1, Q2, Q3, Q4) const, P1, P2, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

//4 args to Callback3 (obsolete - we only provide up to 5 parameters of receiving function)

template <class O, class M, class P1, class P2, class P3, class T1, class T2, class T3, class T4>
struct Callback3MethodActionArg4 : public Callback3Action<P1, P2, P3> {
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	T3        arg4;
	void    Execute(P1 p1, P2 p2, P3 p3) { (object->*method)(p1, p2, p3, arg1, arg2, arg3, arg4); }

	Callback3MethodActionArg4(O *object, M method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(pick(arg1)), arg2(pick(arg2)), arg3(pick(arg3)), arg4(pick(arg4)) {}
};

template <class O, class M, class P1, class P2, class P3, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Callback3<P1, P2, P3> callback4(O *object, void (M::*method)(P1, P2, P3, Q1, Q2, Q3, Q4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback3<P1, P2, P3>(new Callback3MethodActionArg4<O, void (M::*)(P1, P2, P3, Q1, Q2, Q3, Q4), P1, P2, P3, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class P1, class P2, class P3, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Callback3<P1, P2, P3> callback4(O *object, void (M::*method)(P1, P2, P3, Q1, Q2, Q3, Q4) const, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback3<P1, P2, P3>(new Callback3MethodActionArg4<O, void (M::*)(P1, P2, P3, Q1, Q2, Q3, Q4) const, P1, P2, P3, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

//4 args to Callback4 (obsolete - we only provide up to 5 parameters of receiving function)

template <class O, class M, class P1, class P2, class P3, class P4, class T1, class T2, class T3, class T4>
struct Callback4MethodActionArg4 : public Callback4Action<P1, P2, P3, P4> {
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	T4        arg4;
	void    Execute(P1 p1, P2 p2, P3 p3, P4 p4) { (object->*method)(p1, p2, p3, p4, arg1, arg2, arg3, arg4); }

	Callback4MethodActionArg4(O *object, M method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(pick(arg1)), arg2(pick(arg2)), arg3(pick(arg3)), arg4(pick(arg4)) {}
};

template <class O, class M, class P1, class P2, class P3, class P4, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Callback4<P1, P2, P3, P4> callback4(O *object, void (M::*method)(P1, P2, P3, P4, Q1, Q2, Q3, Q4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback4<P1, P2, P3, P4>(new Callback4MethodActionArg4<O, void (M::*)(P1, P2, P3, P4, Q1, Q2, Q3, Q4), P1, P2, P3, P4, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class P1, class P2, class P3, class P4, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Callback4<P1, P2, P3, P4> callback4(O *object, void (M::*method)(P1, P2, P3, P4, Q1, Q2, Q3, Q4) const, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Callback4<P1, P2, P3, P4>(new Callback4MethodActionArg4<O, void (M::*)(P1, P2, P3, P4, Q1, Q2, Q3, Q4) const, P1, P2, P3, P4, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

//----------------------------------------------------------

//4 args to Gate

template <class O, class M, class T1, class T2, class T3, class T4>
struct GateMethodActionArg4Pte : public GateAction {
	Ptr<O>  object;
	M       method;
	T1            arg1;
	T2            arg2;
	T3            arg3;
	T4            arg4;
	bool    Execute() { return object ? (object->*method)(arg1, arg2, arg3, arg4) : false; }

	GateMethodActionArg4Pte(O *object, M method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(pick(arg1)), arg2(pick(arg2)), arg3(pick(arg3)), arg4(pick(arg4)) {}
};

template <class O, class M, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Gate pteback4(O *object, bool (M::*method)(Q1, Q2, Q3, Q4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Gate(new GateMethodActionArg4Pte<O, bool (M::*)(Q1, Q2, Q3, Q4), T1, T2, T3, T4>
	                    (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Gate pteback4(O *object, bool (M::*method)(Q1, Q2, Q3, Q4) const, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Gate(new GateMethodActionArg4Pte<O, bool (M::*)(Q1, Q2, Q3, Q4) const, T1, T2, T3, T4>
	                    (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class T1, class T2, class T3, class T4>
struct GateMethodActionArg4 : public GateAction 
{
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	T4        arg4;
	
	bool    Execute() { return object ? (object->*method)(arg1, arg2, arg3, arg4) : false; }

	GateMethodActionArg4(O *object, M method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(pick(arg1)), arg2(pick(arg2)), arg3(pick(arg3)), arg4(pick(arg4)) {}
};

template <class O, class M, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4> 
Gate callback4(O *object, bool (M::*method)(Q1, Q2, Q3, Q4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) 
{
	return Gate(
		new GateMethodActionArg4<O, bool (M::*)(Q1, Q2, Q3, Q4), T1, T2, T3, T4>(object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4> 
Gate callback4(O *object, bool (M::*method)(Q1, Q2, Q3, Q4) const, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Gate(new GateMethodActionArg4<O, bool (M::*)(Q1, Q2, Q3, Q4) const, T1, T2, T3, T4>
	                    (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class X, class T1, class T2, class T3, class T4, class HC = X>
struct GateActionCallArg4 : public GateAction {
	X         x;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	T4        arg4;
	bool    Execute() { return x(arg1, arg2, arg3, arg4); }

	GateActionCallArg4(X x, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
		: x(x), arg1(pick(arg1)), arg2(pick(arg2)), arg3(pick(arg3)), arg4(pick(arg4)) {}
};

template <class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Gate callback4(bool (*fn)(Q1, Q2, Q3, Q4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Gate(
		new GateActionCallArg4<bool (*)(Q1, Q2, Q3, Q4), T1, T2, T3, T4, uintptr_t>(fn, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Gate callback4(Gate4<Q1, Q2, Q3, Q4> cb, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Gate(
		new GateActionCallArg4<Gate4<Q1, Q2, Q3, Q4>, T1, T2, T3, T4>(cb, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

//4 args to Gate1

template <class X, class P1, class T1, class T2, class T3, class T4>
struct Gate1FnActionArg4 : public Gate1Action<P1> {
	X       fn;
	T1      arg1;
	T2      arg2;
	T3      arg3;
	T4      arg4;
	bool    Execute(P1 p1) { return fn ? fn(p1, arg1, arg2, arg3, arg4) : false; }

	Gate1FnActionArg4(X fn, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: fn(fn), arg1(pick(arg1)), arg2(pick(arg2)), arg3(pick(arg3)), arg4(pick(arg4)) {}
};

template <class P1, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Gate1<P1> callback4(bool (*fn)(P1, Q1, Q2, Q3, Q4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Gate1<P1>(new Gate1FnActionArg4<bool (*)(P1, Q1, Q2, Q3, Q4), P1, T1, T2, T3, T4>
	                         (fn, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class P1, class T1, class T2, class T3, class T4>
struct Gate1MethodActionArg4 : public Gate1Action<P1> {
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	T4        arg4;
	bool    Execute(P1 p1) { return object ? (object->*method)(p1, arg1, arg2, arg3, arg4) : false; }

	Gate1MethodActionArg4(O *object, M method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(pick(arg1)), arg2(pick(arg2)), arg3(pick(arg3)), arg4(pick(arg4)) {}
};

template <class O, class M, class P1, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Gate1<P1> callback4(O *object, bool (M::*method)(P1, Q1, Q2, Q3, Q4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Gate1<P1>(new Gate1MethodActionArg4<O, bool (M::*)(P1, Q1, Q2, Q3, Q4), P1, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class P1, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Gate1<P1> callback4(O *object, bool (M::*method)(P1, Q1, Q2, Q3, Q4) const, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Gate1<P1>(new Gate1MethodActionArg4<O, bool (M::*)(P1, Q1, Q2, Q3, Q4) const, P1, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class P1, class T1, class T2, class T3, class T4>
struct Gate1MethodActionArg4Pte : public Gate1Action<P1> {
	Ptr<O>  object;
	M       method;
	T1      arg1;
	T2      arg2;
	T3      arg3;
	T4      arg4;
	bool    Execute(P1 p1) { return object ? (object->*method)(p1, arg1, arg2, arg3, arg4) : false; }

	Gate1MethodActionArg4Pte(O *object, M method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(pick(arg1)), arg2(pick(arg2)), arg3(pick(arg3)), arg4(pick(arg4)) {}
};

template <class O, class M, class P1, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Gate1<P1> pteback4(O *object, bool (M::*method)(P1, Q1, Q2, Q3, Q4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Gate1<P1>(new Gate1MethodActionArg4Pte<O, bool (M::*)(P1, Q1, Q2, Q3, Q4), P1, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class P1, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Gate1<P1> pteback4(O *object, bool (M::*method)(P1, Q1, Q2, Q3, Q4) const, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Gate1<P1>(new Gate1MethodActionArg4Pte<O, bool (M::*)(P1, Q1, Q2, Q3, Q4) const, P1, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

//4 args to Gate2

template <class O, class M, class P1, class P2, class T1, class T2, class T3, class T4>
struct Gate2MethodActionArg4 : public Gate2Action<P1, P2> {
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	T4        arg4;
	bool    Execute(P1 p1, P2 p2) { return object ? (object->*method)(p1, p2, arg1, arg2, arg3, arg4) : false; }

	Gate2MethodActionArg4(O *object, M method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(pick(arg1)), arg2(pick(arg2)), arg3(pick(arg3)), arg4(pick(arg4)) {}
};

template <class O, class M, class P1, class P2, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Gate2<P1, P2> callback4(O *object, bool (M::*method)(P1, P2, Q1, Q2, Q3, Q4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Gate2<P1, P2>(new Gate2MethodActionArg4<O, bool (M::*)(P1, P2, Q1, Q2, Q3, Q4), P1, P2, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class P1, class P2, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Gate2<P1, P2> callback4(O *object, bool (M::*method)(P1, P2, Q1, Q2, Q3, Q4) const, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Gate2<P1, P2>(new Gate2MethodActionArg4<O, bool (M::*)(P1, P2, Q1, Q2, Q3, Q4) const, P1, P2, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

//4 args to Gate3

template <class O, class M, class P1, class P2, class P3, class T1, class T2, class T3, class T4>
struct Gate3MethodActionArg4 : public Gate3Action<P1, P2, P3> {
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	T3        arg4;
	bool    Execute(P1 p1, P2 p2, P3 p3) { return object ? (object->*method)(p1, p2, p3, arg1, arg2, arg3, arg4) : false; }

	Gate3MethodActionArg4(O *object, M method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(pick(arg1)), arg2(pick(arg2)), arg3(pick(arg3)), arg4(pick(arg4)) {}
};

template <class O, class M, class P1, class P2, class P3, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Gate3<P1, P2, P3> callback4(O *object, bool (M::*method)(P1, P2, P3, Q1, Q2, Q3, Q4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Gate3<P1, P2, P3>(new Gate3MethodActionArg4<O, bool (M::*)(P1, P2, P3, Q1, Q2, Q3, Q4), P1, P2, P3, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class P1, class P2, class P3, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Gate3<P1, P2, P3> callback4(O *object, bool (M::*method)(P1, P2, P3, Q1, Q2, Q3, Q4) const, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Gate3<P1, P2, P3>(new Gate3MethodActionArg4<O, bool (M::*)(P1, P2, P3, Q1, Q2, Q3, Q4) const, P1, P2, P3, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

//4 args to Gate4

template <class O, class M, class P1, class P2, class P3, class P4, class T1, class T2, class T3, class T4>
struct Gate4MethodActionArg4 : public Gate4Action<P1, P2, P3, P4> {
	O  *object;
	M   method;
	T1        arg1;
	T2        arg2;
	T3        arg3;
	T4        arg4;
	bool    Execute(P1 p1, P2 p2, P3 p3, P4 p4) { return object ? (object->*method)(p1, p2, p3, p4, arg1, arg2, arg3, arg4) : false; }

	Gate4MethodActionArg4(O *object, M method, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	: object(object), method(method), arg1(pick(arg1)), arg2(pick(arg2)), arg3(pick(arg3)), arg4(pick(arg4)) {}
};

template <class O, class M, class P1, class P2, class P3, class P4, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Gate4<P1, P2, P3, P4> callback4(O *object, bool (M::*method)(P1, P2, P3, P4, Q1, Q2, Q3, Q4), T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Gate4<P1, P2, P3, P4>(new Gate4MethodActionArg4<O, bool (M::*)(P1, P2, P3, P4, Q1, Q2, Q3, Q4), P1, P2, P3, P4, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}

template <class O, class M, class P1, class P2, class P3, class P4, class Q1, class Q2, class Q3, class Q4, class T1, class T2, class T3, class T4>
Gate4<P1, P2, P3, P4> callback4(O *object, bool (M::*method)(P1, P2, P3, P4, Q1, Q2, Q3, Q4) const, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
	return Gate4<P1, P2, P3, P4>(new Gate4MethodActionArg4<O, bool (M::*)(P1, P2, P3, P4, Q1, Q2, Q3, Q4) const, P1, P2, P3, P4, T1, T2, T3, T4>
	                         (object, method, pick(arg1), pick(arg2), pick(arg3), pick(arg4)));
}
