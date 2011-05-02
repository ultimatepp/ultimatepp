//0 args to Callback

template <class O, class M>
struct CallbackMethodActionPte : public CallbackAction {
	Ptr<O>  object;
	M       method;

	void Execute() { if(object) (object->*method)(); }
	bool IsValid() const { return object; }

	CallbackMethodActionPte(O *object, M method) : object(object), method(method) {}
};

template <class O, class M>
Callback pteback(O *object, void (M::*method)()) {
	return Callback(new CallbackMethodActionPte<O, void (M::*)()>(object, method));
}

template <class O, class M>
struct CallbackMethodAction : public CallbackAction {
	O  *object;
	M   method;

	void Execute() { (object->*method)(); }

	CallbackMethodAction(O *object, M method) : object(object), method(method) {}
};

template <class O, class M>
Callback callback(O *object, void (M::*method)()) {
	return Callback(new CallbackMethodAction<O, void (M::*)()>(object, method));
}

template <class O, class M>
Callback callback(const O *object, void (M::*method)() const) {
	return Callback(new CallbackMethodAction<const O, void (M::*)() const>(object, method));
}

struct CallbackFnAction : public CallbackAction {
	void (*fn)();

	void Execute() { (*fn)(); }

	CallbackFnAction(void (*fn)()) : fn(fn) {}
};

inline Callback callback(void (*fn)()) {
	return Callback(new CallbackFnAction (fn));
}

struct CallbackForkAction : public CallbackAction {
	Callback cb1, cb2;

	void Execute() { cb1(); cb2(); }

	CallbackForkAction(Callback cb1, Callback cb2)
		 : cb1(cb1), cb2(cb2) {}
};

inline Callback Proxy(Callback& cb)
{
	return callback(&cb, &Callback::Execute);
}

Callback callback(Callback cb1, Callback cb2);
Callback& operator<<(Callback& a, Callback b);

//0 args to Callback1

template <class O, class M, class P1>
struct Callback1MethodActionPte : public Callback1Action<P1> {
	Ptr<O>  object;
	M       method;

	void Execute(P1 p1) { if(object) (object->*method)(p1); }
	bool IsValid() const { return object; }

	Callback1MethodActionPte(O *object, M method) : object(object), method(method) {}
};

template <class O, class M, class P1>
Callback1<P1> pteback(O *object, void (M::*method)(P1 p1)) {
	return Callback1<P1>(new Callback1MethodActionPte<O, void (M::*)(P1 p1), P1>(object, method));
}

template <class O, class M, class P1>
struct Callback1MethodAction : public Callback1Action<P1> {
	O  *object;
	M   method;

	void Execute(P1 p1) { (object->*method)(p1); }

	Callback1MethodAction(O *object, M method) : object(object), method(method) {}
};

template <class O, class M, class P1>
Callback1<P1> callback(O *object, void (M::*method)(P1 p1)) {
	return Callback1<P1>(new Callback1MethodAction<O, void (M::*)(P1 p1), P1>(object, method));
}

template <class O, class M, class P1>
Callback1<P1> callback(const O *object, void (M::*method)(P1 p1) const) {
	return Callback1<P1>(new Callback1MethodAction<const O, void (M::*)(P1 p1) const, P1>(object, method));
}

template <class P1>
struct Callback1FnAction : public Callback1Action<P1> {
	void (*fn)(P1 p1);

	void Execute(P1 p1) { (*fn)(p1); }

	Callback1FnAction(void (*fn)(P1 p1)) : fn(fn) {}
};

template <class P1>
inline Callback1<P1> callback(void (*fn)(P1 p1)) {
	return Callback1<P1>(new Callback1FnAction <P1>(fn));
}

template <class P1>
struct Callback1ForkAction : public Callback1Action<P1> {
	Callback1<P1> cb1, cb2;

	void Execute(P1 p1) { cb1(p1); cb2(p1); }

	Callback1ForkAction(Callback1<P1> cb1, Callback1<P1> cb2)
		 : cb1(cb1), cb2(cb2) {}
};

template <class P1>
inline Callback1<P1> Proxy(Callback1<P1>& cb)
{
	return callback(&cb, &Callback1<P1>::Execute);
}

template <class P1>
Callback1<P1> callback(Callback1<P1> cb1, Callback1<P1> cb2)
{
	return Callback1<P1>(new Callback1ForkAction <P1>(cb1, cb2));
}

template <class P1>
Callback1<P1>& operator<<(Callback1<P1>& a, Callback1<P1> b)
{
	if(a)
		a = callback(a, b);
	else
		a = b;
	return a;
}

//0 args to Callback2

template <class O, class M, class P1, class P2>
struct Callback2MethodActionPte : public Callback2Action<P1, P2> {
	Ptr<O>  object;
	M       method;

	void Execute(P1 p1, P2 p2) { if(object) (object->*method)(p1, p2); }
	bool IsValid() const { return object; }

	Callback2MethodActionPte(O *object, M method) : object(object), method(method) {}
};

template <class O, class M, class P1, class P2>
Callback2<P1, P2> pteback(O *object, void (M::*method)(P1 p1, P2 p2)) {
	return Callback2<P1, P2>(new Callback2MethodActionPte<O, void (M::*)(P1 p1, P2 p2), P1, P2>(object, method));
}

template <class O, class M, class P1, class P2>
struct Callback2MethodAction : public Callback2Action<P1, P2> {
	O  *object;
	M   method;

	void Execute(P1 p1, P2 p2) { (object->*method)(p1, p2); }

	Callback2MethodAction(O *object, M method) : object(object), method(method) {}
};

template <class O, class M, class P1, class P2>
Callback2<P1, P2> callback(O *object, void (M::*method)(P1 p1, P2 p2)) {
	return Callback2<P1, P2>(new Callback2MethodAction<O, void (M::*)(P1 p1, P2 p2), P1, P2>(object, method));
}

template <class O, class M, class P1, class P2>
Callback2<P1, P2> callback(const O *object, void (M::*method)(P1 p1, P2 p2) const) {
	return Callback2<P1, P2>(new Callback2MethodAction<const O, void (M::*)(P1 p1, P2 p2) const, P1, P2>(object, method));
}

template <class P1, class P2>
struct Callback2FnAction : public Callback2Action<P1, P2> {
	void (*fn)(P1 p1, P2 p2);

	void Execute(P1 p1, P2 p2) { (*fn)(p1, p2); }

	Callback2FnAction(void (*fn)(P1 p1, P2 p2)) : fn(fn) {}
};

template <class P1, class P2>
inline Callback2<P1, P2> callback(void (*fn)(P1 p1, P2 p2)) {
	return Callback2<P1, P2>(new Callback2FnAction <P1, P2>(fn));
}

template <class P1, class P2>
struct Callback2ForkAction : public Callback2Action<P1, P2> {
	Callback2<P1, P2> cb1, cb2;

	void Execute(P1 p1, P2 p2) { cb1(p1, p2); cb2(p1, p2); }

	Callback2ForkAction(Callback2<P1, P2> cb1, Callback2<P1, P2> cb2)
		 : cb1(cb1), cb2(cb2) {}
};

template <class P1, class P2>
inline Callback2<P1, P2> Proxy(Callback2<P1, P2>& cb)
{
	return callback(&cb, &Callback2<P1, P2>::Execute);
}

template <class P1, class P2>
Callback2<P1, P2> callback(Callback2<P1, P2> cb1, Callback2<P1, P2> cb2)
{
	return Callback2<P1, P2>(new Callback2ForkAction <P1, P2>(cb1, cb2));
}

template <class P1, class P2>
Callback2<P1, P2>& operator<<(Callback2<P1, P2>& a, Callback2<P1, P2> b)
{
	if(a)
		a = callback(a, b);
	else
		a = b;
	return a;
}

//0 args to Callback3

template <class O, class M, class P1, class P2, class P3>
struct Callback3MethodActionPte : public Callback3Action<P1, P2, P3> {
	Ptr<O>  object;
	M       method;

	void Execute(P1 p1, P2 p2, P3 p3) { if(object) (object->*method)(p1, p2, p3); }
	bool IsValid() const { return object; }

	Callback3MethodActionPte(O *object, M method) : object(object), method(method) {}
};

template <class O, class M, class P1, class P2, class P3>
Callback3<P1, P2, P3> pteback(O *object, void (M::*method)(P1 p1, P2 p2, P3 p3)) {
	return Callback3<P1, P2, P3>(new Callback3MethodActionPte<O, void (M::*)(P1 p1, P2 p2, P3 p3), P1, P2, P3>(object, method));
}

template <class O, class M, class P1, class P2, class P3>
struct Callback3MethodAction : public Callback3Action<P1, P2, P3> {
	O  *object;
	M   method;

	void Execute(P1 p1, P2 p2, P3 p3) { (object->*method)(p1, p2, p3); }

	Callback3MethodAction(O *object, M method) : object(object), method(method) {}
};

template <class O, class M, class P1, class P2, class P3>
Callback3<P1, P2, P3> callback(O *object, void (M::*method)(P1 p1, P2 p2, P3 p3)) {
	return Callback3<P1, P2, P3>(new Callback3MethodAction<O, void (M::*)(P1 p1, P2 p2, P3 p3), P1, P2, P3>(object, method));
}

template <class O, class M, class P1, class P2, class P3>
Callback3<P1, P2, P3> callback(const O *object, void (M::*method)(P1 p1, P2 p2, P3 p3) const) {
	return Callback3<P1, P2, P3>(new Callback3MethodAction<const O, void (M::*)(P1 p1, P2 p2, P3 p3) const, P1, P2, P3>(object, method));
}

template <class P1, class P2, class P3>
struct Callback3FnAction : public Callback3Action<P1, P2, P3> {
	void (*fn)(P1 p1, P2 p2, P3 p3);

	void Execute(P1 p1, P2 p2, P3 p3) { (*fn)(p1, p2, p3); }

	Callback3FnAction(void (*fn)(P1 p1, P2 p2, P3 p3)) : fn(fn) {}
};

template <class P1, class P2, class P3>
inline Callback3<P1, P2, P3> callback(void (*fn)(P1 p1, P2 p2, P3 p3)) {
	return Callback3<P1, P2, P3>(new Callback3FnAction <P1, P2, P3>(fn));
}

template <class P1, class P2, class P3>
struct Callback3ForkAction : public Callback3Action<P1, P2, P3> {
	Callback3<P1, P2, P3> cb1, cb2;

	void Execute(P1 p1, P2 p2, P3 p3) { cb1(p1, p2, p3); cb2(p1, p2, p3); }

	Callback3ForkAction(Callback3<P1, P2, P3> cb1, Callback3<P1, P2, P3> cb2)
		 : cb1(cb1), cb2(cb2) {}
};

template <class P1, class P2, class P3>
inline Callback3<P1, P2, P3> Proxy(Callback3<P1, P2, P3>& cb)
{
	return callback(&cb, &Callback3<P1, P2, P3>::Execute);
}

template <class P1, class P2, class P3>
Callback3<P1, P2, P3> callback(Callback3<P1, P2, P3> cb1, Callback3<P1, P2, P3> cb2)
{
	return Callback3<P1, P2, P3>(new Callback3ForkAction <P1, P2, P3>(cb1, cb2));
}

template <class P1, class P2, class P3>
Callback3<P1, P2, P3>& operator<<(Callback3<P1, P2, P3>& a, Callback3<P1, P2, P3> b)
{
	if(a)
		a = callback(a, b);
	else
		a = b;
	return a;
}

//0 args to Callback4

template <class O, class M, class P1, class P2, class P3, class P4>
struct Callback4MethodActionPte : public Callback4Action<P1, P2, P3, P4> {
	Ptr<O>  object;
	M       method;

	void Execute(P1 p1, P2 p2, P3 p3, P4 p4) { if(object) (object->*method)(p1, p2, p3, p4); }
	bool IsValid() const { return object; }

	Callback4MethodActionPte(O *object, M method) : object(object), method(method) {}
};

template <class O, class M, class P1, class P2, class P3, class P4>
Callback4<P1, P2, P3, P4> pteback(O *object, void (M::*method)(P1 p1, P2 p2, P3 p3, P4 p4)) {
	return Callback4<P1, P2, P3, P4>(new Callback4MethodActionPte<O, void (M::*)(P1 p1, P2 p2, P3 p3, P4 p4), P1, P2, P3, P4>(object, method));
}

template <class O, class M, class P1, class P2, class P3, class P4>
struct Callback4MethodAction : public Callback4Action<P1, P2, P3, P4> {
	O  *object;
	M   method;

	void Execute(P1 p1, P2 p2, P3 p3, P4 p4) { (object->*method)(p1, p2, p3, p4); }

	Callback4MethodAction(O *object, M method) : object(object), method(method) {}
};

template <class O, class M, class P1, class P2, class P3, class P4>
Callback4<P1, P2, P3, P4> callback(O *object, void (M::*method)(P1 p1, P2 p2, P3 p3, P4 p4)) {
	return Callback4<P1, P2, P3, P4>(new Callback4MethodAction<O, void (M::*)(P1 p1, P2 p2, P3 p3, P4 p4), P1, P2, P3, P4>(object, method));
}

template <class O, class M, class P1, class P2, class P3, class P4>
Callback4<P1, P2, P3, P4> callback(const O *object, void (M::*method)(P1 p1, P2 p2, P3 p3, P4 p4) const) {
	return Callback4<P1, P2, P3, P4>(new Callback4MethodAction<const O, void (M::*)(P1 p1, P2 p2, P3 p3, P4 p4) const, P1, P2, P3, P4>(object, method));
}

template <class P1, class P2, class P3, class P4>
struct Callback4FnAction : public Callback4Action<P1, P2, P3, P4> {
	void (*fn)(P1 p1, P2 p2, P3 p3, P4 p4);

	void Execute(P1 p1, P2 p2, P3 p3, P4 p4) { (*fn)(p1, p2, p3, p4); }

	Callback4FnAction(void (*fn)(P1 p1, P2 p2, P3 p3, P4 p4)) : fn(fn) {}
};

template <class P1, class P2, class P3, class P4>
inline Callback4<P1, P2, P3, P4> callback(void (*fn)(P1 p1, P2 p2, P3 p3, P4 p4)) {
	return Callback4<P1, P2, P3, P4>(new Callback4FnAction <P1, P2, P3, P4>(fn));
}

template <class P1, class P2, class P3, class P4>
struct Callback4ForkAction : public Callback4Action<P1, P2, P3, P4> {
	Callback4<P1, P2, P3, P4> cb1, cb2;

	void Execute(P1 p1, P2 p2, P3 p3, P4 p4) { cb1(p1, p2, p3, p4); cb2(p1, p2, p3, p4); }

	Callback4ForkAction(Callback4<P1, P2, P3, P4> cb1, Callback4<P1, P2, P3, P4> cb2)
		 : cb1(cb1), cb2(cb2) {}
};

template <class P1, class P2, class P3, class P4>
inline Callback4<P1, P2, P3, P4> Proxy(Callback4<P1, P2, P3, P4>& cb)
{
	return callback(&cb, &Callback4<P1, P2, P3, P4>::Execute);
}

template <class P1, class P2, class P3, class P4>
Callback4<P1, P2, P3, P4> callback(Callback4<P1, P2, P3, P4> cb1, Callback4<P1, P2, P3, P4> cb2)
{
	return Callback4<P1, P2, P3, P4>(new Callback4ForkAction <P1, P2, P3, P4>(cb1, cb2));
}

template <class P1, class P2, class P3, class P4>
Callback4<P1, P2, P3, P4>& operator<<(Callback4<P1, P2, P3, P4>& a, Callback4<P1, P2, P3, P4> b)
{
	if(a)
		a = callback(a, b);
	else
		a = b;
	return a;
}

// -----------------------------------------------------------

//0 args to Gate

template <class O, class M>
struct GateMethodActionPte : public GateAction {
	Ptr<O>  object;
	M       method;

	bool Execute() { return object ? (object->*method)() : false; }
	bool IsValid() const { return object; }

	GateMethodActionPte(O *object, M method) : object(object), method(method) {}
};

template <class O, class M>
struct GateMethodAction : public GateAction {
	O  *object;
	M   method;

	bool Execute() { return (object->*method)(); }

	GateMethodAction(O *object, M method) : object(object), method(method) {}
};

struct GateFnAction : public GateAction {
	bool (*fn)();

	bool Execute() { return (*fn)(); }

	GateFnAction(bool (*fn)()) : fn(fn) {}
};


template <class O, class M>
Gate pteback(O *object, bool (M::*method)()) {
	return Gate(new GateMethodActionPte<O, bool (M::*)()>(object, method));
}

template <class O, class M>
Gate callback(O *object, bool (M::*method)()) {
	return Gate(new GateMethodAction<O, bool (M::*)()>(object, method));
}

template <class O, class M>
Gate callback(const O *object, bool (M::*method)() const) {
	return Gate(new GateMethodAction<const O, bool (M::*)() const>(object, method));
}

inline Gate callback(bool (*fn)()) {
	return Gate(new GateFnAction (fn));
}

struct GateForkAction : public GateAction {
	Gate cb1, cb2;

	bool Execute() { cb1(); return cb2(); }

	GateForkAction(Gate cb1, Gate cb2)
		 : cb1(cb1), cb2(cb2) {}
};

inline Gate Proxy(Gate& cb)
{
	return callback(&cb, &Gate::Execute);
}

Gate callback(Gate cb1, Gate cb2);
Gate& operator<<(Gate& a, Gate b);

//0 args to Gate1

template <class O, class M, class P1>
struct Gate1MethodActionPte : public Gate1Action<P1> {
	Ptr<O>  object;
	M       method;

	bool Execute(P1 p1) { return object ? (object->*method)(p1) : false; }
	bool IsValid() const { return object; }

	Gate1MethodActionPte(O *object, M method) : object(object), method(method) {}
};

template <class O, class M, class P1>
Gate1<P1> pteback(O *object, bool (M::*method)(P1 p1)) {
	return Gate1<P1>(new Gate1MethodActionPte<O, bool (M::*)(P1 p1), P1>(object, method));
}

template <class O, class M, class P1>
struct Gate1MethodAction : public Gate1Action<P1> {
	O  *object;
	M   method;

	bool Execute(P1 p1) { return (object->*method)(p1); }

	Gate1MethodAction(O *object, M method) : object(object), method(method) {}
};

template <class O, class M, class P1>
Gate1<P1> callback(O *object, bool (M::*method)(P1 p1)) {
	return Gate1<P1>(new Gate1MethodAction<O, bool (M::*)(P1 p1), P1>(object, method));
}

template <class O, class M, class P1>
Gate1<P1> callback(const O *object, bool (M::*method)(P1 p1) const) {
	return Gate1<P1>(new Gate1MethodAction<const O, bool (M::*)(P1 p1) const, P1>(object, method));
}

template <class P1>
struct Gate1FnAction : public Gate1Action<P1> {
	bool (*fn)(P1 p1);

	bool Execute(P1 p1) { return (*fn)(p1); }

	Gate1FnAction(bool (*fn)(P1 p1)) : fn(fn) {}
};

template <class P1>
inline Gate1<P1> callback(bool (*fn)(P1 p1)) {
	return Gate1<P1>(new Gate1FnAction <P1>(fn));
}

template <class P1>
struct Gate1ForkAction : public Gate1Action<P1> {
	Gate1<P1> cb1, cb2;

	bool Execute(P1 p1) { cb1(p1); return cb2(p1); }

	Gate1ForkAction(Gate1<P1> cb1, Gate1<P1> cb2)
		 : cb1(cb1), cb2(cb2) {}
};

template <class P1>
inline Gate1<P1> Proxy(Gate1<P1>& cb)
{
	return callback(&cb, &Gate1<P1>::Execute);
}

template <class P1>
Gate1<P1> callback(Gate1<P1> cb1, Gate1<P1> cb2)
{
	return Gate1<P1>(new Gate1ForkAction <P1>(cb1, cb2));
}

template <class P1>
Gate1<P1>& operator<<(Gate1<P1>& a, Gate1<P1> b)
{
	if(a)
		a = callback(a, b);
	else
		a = b;
	return a;
}

//0 args to Gate2

template <class O, class M, class P1, class P2>
struct Gate2MethodActionPte : public Gate2Action<P1, P2> {
	Ptr<O>  object;
	M       method;

	bool Execute(P1 p1, P2 p2) { return object ? (object->*method)(p1, p2) : false; }
	bool IsValid() const { return object; }

	Gate2MethodActionPte(O *object, M method) : object(object), method(method) {}
};

template <class O, class M, class P1, class P2>
Gate2<P1, P2> pteback(O *object, bool (M::*method)(P1 p1, P2 p2)) {
	return Gate2<P1, P2>(new Gate2MethodActionPte<O, bool (M::*)(P1 p1, P2 p2), P1, P2>(object, method));
}

template <class O, class M, class P1, class P2>
struct Gate2MethodAction : public Gate2Action<P1, P2> {
	O  *object;
	M   method;

	bool Execute(P1 p1, P2 p2) { return (object->*method)(p1, p2); }

	Gate2MethodAction(O *object, M method) : object(object), method(method) {}
};

template <class O, class M, class P1, class P2>
Gate2<P1, P2> callback(O *object, bool (M::*method)(P1 p1, P2 p2)) {
	return Gate2<P1, P2>(new Gate2MethodAction<O, bool (M::*)(P1 p1, P2 p2), P1, P2>(object, method));
}

template <class O, class M, class P1, class P2>
Gate2<P1, P2> callback(const O *object, bool (M::*method)(P1 p1, P2 p2) const) {
	return Gate2<P1, P2>(new Gate2MethodAction<const O, bool (M::*)(P1 p1, P2 p2) const, P1, P2>(object, method));
}

template <class P1, class P2>
struct Gate2FnAction : public Gate2Action<P1, P2> {
	bool (*fn)(P1 p1, P2 p2);

	bool Execute(P1 p1, P2 p2) { return (*fn)(p1, p2); }

	Gate2FnAction(bool (*fn)(P1 p1, P2 p2)) : fn(fn) {}
};

template <class P1, class P2>
inline Gate2<P1, P2> callback(bool (*fn)(P1 p1, P2 p2)) {
	return Gate2<P1, P2>(new Gate2FnAction <P1, P2>(fn));
}

template <class P1, class P2>
struct Gate2ForkAction : public Gate2Action<P1, P2> {
	Gate2<P1, P2> cb1, cb2;

	bool Execute(P1 p1, P2 p2) { cb1(p1, p2); return cb2(p1, p2); }

	Gate2ForkAction(Gate2<P1, P2> cb1, Gate2<P1, P2> cb2)
		 : cb1(cb1), cb2(cb2) {}
};

template <class P1, class P2>
inline Gate2<P1, P2> Proxy(Gate2<P1, P2>& cb)
{
	return callback(&cb, &Gate2<P1, P2>::Execute);
}

template <class P1, class P2>
Gate2<P1, P2> callback(Gate2<P1, P2> cb1, Gate2<P1, P2> cb2)
{
	return Gate2<P1, P2>(new Gate2ForkAction <P1, P2>(cb1, cb2));
}

template <class P1, class P2>
Gate2<P1, P2>& operator<<(Gate2<P1, P2>& a, Gate2<P1, P2> b)
{
	if(a)
		a = callback(a, b);
	else
		a = b;
	return a;
}

//0 args to Gate3

template <class O, class M, class P1, class P2, class P3>
struct Gate3MethodActionPte : public Gate3Action<P1, P2, P3> {
	Ptr<O>  object;
	M       method;

	bool Execute(P1 p1, P2 p2, P3 p3) { return object ? (object->*method)(p1, p2, p3) : false; }
	bool IsValid() const { return object; }

	Gate3MethodActionPte(O *object, M method) : object(object), method(method) {}
};

template <class O, class M, class P1, class P2, class P3>
Gate3<P1, P2, P3> pteback(O *object, bool (M::*method)(P1 p1, P2 p2, P3 p3)) {
	return Gate3<P1, P2, P3>(new Gate3MethodActionPte<O, bool (M::*)(P1 p1, P2 p2, P3 p3), P1, P2, P3>(object, method));
}

template <class O, class M, class P1, class P2, class P3>
struct Gate3MethodAction : public Gate3Action<P1, P2, P3> {
	O  *object;
	M   method;

	bool Execute(P1 p1, P2 p2, P3 p3) { return (object->*method)(p1, p2, p3); }

	Gate3MethodAction(O *object, M method) : object(object), method(method) {}
};

template <class O, class M, class P1, class P2, class P3>
Gate3<P1, P2, P3> callback(O *object, bool (M::*method)(P1 p1, P2 p2, P3 p3)) {
	return Gate3<P1, P2, P3>(new Gate3MethodAction<O, bool (M::*)(P1 p1, P2 p2, P3 p3), P1, P2, P3>(object, method));
}

template <class O, class M, class P1, class P2, class P3>
Gate3<P1, P2, P3> callback(const O *object, bool (M::*method)(P1 p1, P2 p2, P3 p3) const) {
	return Gate3<P1, P2, P3>(new Gate3MethodAction<const O, bool (M::*)(P1 p1, P2 p2, P3 p3) const, P1, P2, P3>(object, method));
}

template <class P1, class P2, class P3>
struct Gate3FnAction : public Gate3Action<P1, P2, P3> {
	bool (*fn)(P1 p1, P2 p2, P3 p3);

	bool Execute(P1 p1, P2 p2, P3 p3) { return (*fn)(p1, p2, p3); }

	Gate3FnAction(bool (*fn)(P1 p1, P2 p2, P3 p3)) : fn(fn) {}
};

template <class P1, class P2, class P3>
inline Gate3<P1, P2, P3> callback(bool (*fn)(P1 p1, P2 p2, P3 p3)) {
	return Gate3<P1, P2, P3>(new Gate3FnAction <P1, P2, P3>(fn));
}

template <class P1, class P2, class P3>
struct Gate3ForkAction : public Gate3Action<P1, P2, P3> {
	Gate3<P1, P2, P3> cb1, cb2;

	bool Execute(P1 p1, P2 p2, P3 p3) { cb1(p1, p2, p3); return cb2(p1, p2, p3); }

	Gate3ForkAction(Gate3<P1, P2, P3> cb1, Gate3<P1, P2, P3> cb2)
		 : cb1(cb1), cb2(cb2) {}
};

template <class P1, class P2, class P3>
inline Gate3<P1, P2, P3> Proxy(Gate3<P1, P2, P3>& cb)
{
	return callback(&cb, &Gate3<P1, P2, P3>::Execute);
}

template <class P1, class P2, class P3>
Gate3<P1, P2, P3> callback(Gate3<P1, P2, P3> cb1, Gate3<P1, P2, P3> cb2)
{
	return Gate3<P1, P2, P3>(new Gate3ForkAction <P1, P2, P3>(cb1, cb2));
}

template <class P1, class P2, class P3>
Gate3<P1, P2, P3>& operator<<(Gate3<P1, P2, P3>& a, Gate3<P1, P2, P3> b)
{
	if(a)
		a = callback(a, b);
	else
		a = b;
	return a;
}

//0 args to Gate4

template <class O, class M, class P1, class P2, class P3, class P4>
struct Gate4MethodActionPte : public Gate4Action<P1, P2, P3, P4> {
	Ptr<O>  object;
	M       method;

	bool Execute(P1 p1, P2 p2, P3 p3, P4 p4) { return object ? (object->*method)(p1, p2, p3, p4) : false; }
	bool IsValid() const { return object; }

	Gate4MethodActionPte(O *object, M method) : object(object), method(method) {}
};

template <class O, class M, class P1, class P2, class P3, class P4>
Gate4<P1, P2, P3, P4> pteback(O *object, bool (M::*method)(P1 p1, P2 p2, P3 p3, P4 p4)) {
	return Gate4<P1, P2, P3, P4>(new Gate4MethodActionPte<O, bool (M::*)(P1 p1, P2 p2, P3 p3, P4 p4), P1, P2, P3, P4>(object, method));
}

template <class O, class M, class P1, class P2, class P3, class P4>
struct Gate4MethodAction : public Gate4Action<P1, P2, P3, P4> {
	O  *object;
	M   method;

	bool Execute(P1 p1, P2 p2, P3 p3, P4 p4) { return (object->*method)(p1, p2, p3, p4); }

	Gate4MethodAction(O *object, M method) : object(object), method(method) {}
};

template <class O, class M, class P1, class P2, class P3, class P4>
Gate4<P1, P2, P3, P4> callback(O *object, bool (M::*method)(P1 p1, P2 p2, P3 p3, P4 p4)) {
	return Gate4<P1, P2, P3, P4>(new Gate4MethodAction<O, bool (M::*)(P1 p1, P2 p2, P3 p3, P4 p4), P1, P2, P3, P4>(object, method));
}

template <class O, class M, class P1, class P2, class P3, class P4>
Gate4<P1, P2, P3, P4> callback(const O *object, bool (M::*method)(P1 p1, P2 p2, P3 p3, P4 p4) const) {
	return Gate4<P1, P2, P3, P4>(new Gate4MethodAction<const O, bool (M::*)(P1 p1, P2 p2, P3 p3, P4 p4) const, P1, P2, P3, P4>(object, method));
}

template <class P1, class P2, class P3, class P4>
struct Gate4FnAction : public Gate4Action<P1, P2, P3, P4> {
	bool (*fn)(P1 p1, P2 p2, P3 p3, P4 p4);

	bool Execute(P1 p1, P2 p2, P3 p3, P4 p4) { return (*fn)(p1, p2, p3, p4); }

	Gate4FnAction(bool (*fn)(P1 p1, P2 p2, P3 p3, P4 p4)) : fn(fn) {}
};

template <class P1, class P2, class P3, class P4>
inline Gate4<P1, P2, P3, P4> callback(bool (*fn)(P1 p1, P2 p2, P3 p3, P4 p4)) {
	return Gate4<P1, P2, P3, P4>(new Gate4FnAction <P1, P2, P3, P4>(fn));
}

template <class P1, class P2, class P3, class P4>
struct Gate4ForkAction : public Gate4Action<P1, P2, P3, P4> {
	Gate4<P1, P2, P3, P4> cb1, cb2;

	bool Execute(P1 p1, P2 p2, P3 p3, P4 p4) { cb1(p1, p2, p3, p4); return cb2(p1, p2, p3, p4); }

	Gate4ForkAction(Gate4<P1, P2, P3, P4> cb1, Gate4<P1, P2, P3, P4> cb2)
		 : cb1(cb1), cb2(cb2) {}
};

template <class P1, class P2, class P3, class P4>
inline Gate4<P1, P2, P3, P4> Proxy(Gate4<P1, P2, P3, P4>& cb)
{
	return callback(&cb, &Gate4<P1, P2, P3, P4>::Execute);
}

template <class P1, class P2, class P3, class P4>
Gate4<P1, P2, P3, P4> callback(Gate4<P1, P2, P3, P4> cb1, Gate4<P1, P2, P3, P4> cb2)
{
	return Gate4<P1, P2, P3, P4>(new Gate4ForkAction <P1, P2, P3, P4>(cb1, cb2));
}

template <class P1, class P2, class P3, class P4>
Gate4<P1, P2, P3, P4>& operator<<(Gate4<P1, P2, P3, P4>& a, Gate4<P1, P2, P3, P4> b)
{
	if(a)
		a = callback(a, b);
	else
		a = b;
	return a;
}
