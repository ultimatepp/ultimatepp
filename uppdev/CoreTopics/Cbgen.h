#ifndef CPP_PART__

// -----------------------------------------------------------

struct CallbackAction {
	Atomic  count;

	virtual void Execute() = 0;
	virtual bool IsValid() const { return true; }

	CallbackAction()          { count = 1; }
	virtual ~CallbackAction() {}
};

template <class OBJECT, class METHOD>
struct CallbackMethodActionPte : public CallbackAction {
	Ptr<OBJECT>  object;
	METHOD       method;

	void Execute() { if(object) (object->*method)(); }
	bool IsValid() const { return object; }

	CallbackMethodActionPte(OBJECT *object, METHOD method) : object(object), method(method) {}
};

template <class OBJECT, class METHOD>
struct CallbackMethodAction : public CallbackAction {
	OBJECT  *object;
	METHOD   method;

	void Execute() { (object->*method)(); }

	CallbackMethodAction(OBJECT *object, METHOD method) : object(object), method(method) {}
};

struct CallbackFnAction : public CallbackAction {
	void (*fn)();

	void Execute() { (*fn)(); }

	CallbackFnAction(void (*fn)()) : fn(fn) {}
};

class Callback : Moveable< Callback > {
	CallbackAction *action;

	void Retain() const { if(action ) AtomicInc(action->count); }
	void Release()      { if(action  && AtomicDec(action->count) == 0) delete action; }

	bool operator==(const Callback&);
	bool operator!=(const Callback&);

public:
	typedef Callback CLASSNAME;

	Callback& operator=(const Callback& c);
	Callback(const Callback& c);
	void Clear()        { Release(); action = NULL; }


	operator bool() const   { return action && action->IsValid(); }
	void Execute() const;
	void operator()() const { Execute(); }

	explicit Callback(CallbackAction  *newaction) { action = newaction; }
	Callback() { action = NULL; }
	Callback(_CNULL) { action = NULL; }
	~Callback();

	static Callback Empty() { return CNULL; }

};

template <class OBJECT, class METHOD>
Callback pteback(OBJECT *object, void (METHOD::*method)()) {
	return Callback(new CallbackMethodActionPte<OBJECT, void (METHOD::*)()>(object, method));
}

template <class OBJECT, class METHOD>
Callback callback(OBJECT *object, void (METHOD::*method)()) {
	return Callback(new CallbackMethodAction<OBJECT, void (METHOD::*)()>(object, method));
}

template <class OBJECT, class METHOD>
Callback callback(const OBJECT *object, void (METHOD::*method)() const) {
	return Callback(new CallbackMethodAction<const OBJECT, void (METHOD::*)() const>(object, method));
}

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

#endif
#ifdef CPP_PART__

Callback callback(Callback cb1, Callback cb2)
{
	return Callback(new CallbackForkAction (cb1, cb2));
}

Callback& operator<<(Callback& a, Callback b)
{
	if(a)
		a = callback(a, b);
	else
		a = b;
	return a;
}

Callback& Callback::operator=(const Callback& c)
{
	c.Retain();
	Release();
	action = c.action;
	return *this;
}

Callback::Callback(const Callback& c)
{
	action = c.action;
	Retain();
}

Callback::~Callback()
{
	Release();
}

#endif
#ifndef CPP_PART__

// -----------------------------------------------------------

template <class P1>
struct Callback1Action {
	Atomic  count;

	virtual void Execute(P1 p1) = 0;
	virtual bool IsValid() const { return true; }

	Callback1Action()          { count = 1; }
	virtual ~Callback1Action() {}
};

template <class OBJECT, class METHOD, class P1>
struct Callback1MethodActionPte : public Callback1Action<P1> {
	Ptr<OBJECT>  object;
	METHOD       method;

	void Execute(P1 p1) { if(object) (object->*method)(p1); }
	bool IsValid() const { return object; }

	Callback1MethodActionPte(OBJECT *object, METHOD method) : object(object), method(method) {}
};

template <class OBJECT, class METHOD, class P1>
struct Callback1MethodAction : public Callback1Action<P1> {
	OBJECT  *object;
	METHOD   method;

	void Execute(P1 p1) { (object->*method)(p1); }

	Callback1MethodAction(OBJECT *object, METHOD method) : object(object), method(method) {}
};

template <class P1>
struct Callback1FnAction : public Callback1Action<P1> {
	void (*fn)(P1 p1);

	void Execute(P1 p1) { (*fn)(p1); }

	Callback1FnAction(void (*fn)(P1 p1)) : fn(fn) {}
};

template <class P1>
class Callback1 : Moveable< Callback1<P1> > {
	Callback1Action<P1> *action;

	void Retain() const { if(action ) AtomicInc(action->count); }
	void Release()      { if(action  && AtomicDec(action->count) == 0) delete action; }

	bool operator==(const Callback1&);
	bool operator!=(const Callback1&);

public:
	typedef Callback1 CLASSNAME;

	Callback1& operator=(const Callback1& c);
	Callback1(const Callback1& c);
	void Clear()        { Release(); action = NULL; }


	operator bool() const          { return action && action->IsValid(); }
	void Execute(P1 p1) const      { if(action) action->Execute(p1); }
	void operator()(P1 p1) const   { Execute(p1); }

	explicit Callback1(Callback1Action <P1> *newaction) { action = newaction; }
	Callback1() { action = NULL; }
	Callback1(_CNULL) { action = NULL; }
	~Callback1();

	static Callback1 Empty() { return CNULL; }

};

template <class OBJECT, class METHOD, class P1>
Callback1<P1> pteback(OBJECT *object, void (METHOD::*method)(P1 p1)) {
	return Callback1<P1>(new Callback1MethodActionPte<OBJECT, void (METHOD::*)(P1 p1), P1>(object, method));
}

template <class OBJECT, class METHOD, class P1>
Callback1<P1> callback(OBJECT *object, void (METHOD::*method)(P1 p1)) {
	return Callback1<P1>(new Callback1MethodAction<OBJECT, void (METHOD::*)(P1 p1), P1>(object, method));
}

template <class OBJECT, class METHOD, class P1>
Callback1<P1> callback(const OBJECT *object, void (METHOD::*method)(P1 p1) const) {
	return Callback1<P1>(new Callback1MethodAction<const OBJECT, void (METHOD::*)(P1 p1) const, P1>(object, method));
}

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

template <class P1>
Callback1<P1>& Callback1<P1>::operator=(const Callback1& c)
{
	c.Retain();
	Release();
	action = c.action;
	return *this;
}

template <class P1>
Callback1<P1>::Callback1(const Callback1& c)
{
	action = c.action;
	Retain();
}

template <class P1>
Callback1<P1>::~Callback1()
{
	Release();
}

// -----------------------------------------------------------

template <class P1, class P2>
struct Callback2Action {
	Atomic  count;

	virtual void Execute(P1 p1, P2 p2) = 0;
	virtual bool IsValid() const { return true; }

	Callback2Action()          { count = 1; }
	virtual ~Callback2Action() {}
};

template <class OBJECT, class METHOD, class P1, class P2>
struct Callback2MethodActionPte : public Callback2Action<P1, P2> {
	Ptr<OBJECT>  object;
	METHOD       method;

	void Execute(P1 p1, P2 p2) { if(object) (object->*method)(p1, p2); }
	bool IsValid() const { return object; }

	Callback2MethodActionPte(OBJECT *object, METHOD method) : object(object), method(method) {}
};

template <class OBJECT, class METHOD, class P1, class P2>
struct Callback2MethodAction : public Callback2Action<P1, P2> {
	OBJECT  *object;
	METHOD   method;

	void Execute(P1 p1, P2 p2) { (object->*method)(p1, p2); }

	Callback2MethodAction(OBJECT *object, METHOD method) : object(object), method(method) {}
};

template <class P1, class P2>
struct Callback2FnAction : public Callback2Action<P1, P2> {
	void (*fn)(P1 p1, P2 p2);

	void Execute(P1 p1, P2 p2) { (*fn)(p1, p2); }

	Callback2FnAction(void (*fn)(P1 p1, P2 p2)) : fn(fn) {}
};

template <class P1, class P2>
class Callback2 : Moveable< Callback2<P1, P2> > {
	Callback2Action<P1, P2> *action;

	void Retain() const { if(action ) AtomicInc(action->count); }
	void Release()      { if(action  && AtomicDec(action->count) == 0) delete action; }

	bool operator==(const Callback2&);
	bool operator!=(const Callback2&);

public:
	typedef Callback2 CLASSNAME;

	Callback2& operator=(const Callback2& c);
	Callback2(const Callback2& c);
	void Clear()        { Release(); action = NULL; }


	operator bool() const                { return action && action->IsValid(); }
	void Execute(P1 p1, P2 p2) const     { if(action) action->Execute(p1, p2); }
	void operator()(P1 p1, P2 p2) const  { Execute(p1, p2); }

	explicit Callback2(Callback2Action <P1, P2> *newaction) { action = newaction; }
	Callback2() { action = NULL; }
	Callback2(_CNULL) { action = NULL; }
	~Callback2();

	static Callback2 Empty() { return CNULL; }

};

template <class OBJECT, class METHOD, class P1, class P2>
Callback2<P1, P2> pteback(OBJECT *object, void (METHOD::*method)(P1 p1, P2 p2)) {
	return Callback2<P1, P2>(new Callback2MethodActionPte<OBJECT, void (METHOD::*)(P1 p1, P2 p2), P1, P2>(object, method));
}

template <class OBJECT, class METHOD, class P1, class P2>
Callback2<P1, P2> callback(OBJECT *object, void (METHOD::*method)(P1 p1, P2 p2)) {
	return Callback2<P1, P2>(new Callback2MethodAction<OBJECT, void (METHOD::*)(P1 p1, P2 p2), P1, P2>(object, method));
}

template <class OBJECT, class METHOD, class P1, class P2>
Callback2<P1, P2> callback(const OBJECT *object, void (METHOD::*method)(P1 p1, P2 p2) const) {
	return Callback2<P1, P2>(new Callback2MethodAction<const OBJECT, void (METHOD::*)(P1 p1, P2 p2) const, P1, P2>(object, method));
}

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

template <class P1, class P2>
Callback2<P1, P2>& Callback2<P1, P2>::operator=(const Callback2& c)
{
	c.Retain();
	Release();
	action = c.action;
	return *this;
}

template <class P1, class P2>
Callback2<P1, P2>::Callback2(const Callback2& c)
{
	action = c.action;
	Retain();
}

template <class P1, class P2>
Callback2<P1, P2>::~Callback2()
{
	Release();
}

// -----------------------------------------------------------

template <class P1, class P2, class P3>
struct Callback3Action {
	Atomic  count;

	virtual void Execute(P1 p1, P2 p2, P3 p3) = 0;
	virtual bool IsValid() const { return true; }

	Callback3Action()          { count = 1; }
	virtual ~Callback3Action() {}
};

template <class OBJECT, class METHOD, class P1, class P2, class P3>
struct Callback3MethodActionPte : public Callback3Action<P1, P2, P3> {
	Ptr<OBJECT>  object;
	METHOD       method;

	void Execute(P1 p1, P2 p2, P3 p3) { if(object) (object->*method)(p1, p2, p3); }
	bool IsValid() const { return object; }

	Callback3MethodActionPte(OBJECT *object, METHOD method) : object(object), method(method) {}
};

template <class OBJECT, class METHOD, class P1, class P2, class P3>
struct Callback3MethodAction : public Callback3Action<P1, P2, P3> {
	OBJECT  *object;
	METHOD   method;

	void Execute(P1 p1, P2 p2, P3 p3) { (object->*method)(p1, p2, p3); }

	Callback3MethodAction(OBJECT *object, METHOD method) : object(object), method(method) {}
};

template <class P1, class P2, class P3>
struct Callback3FnAction : public Callback3Action<P1, P2, P3> {
	void (*fn)(P1 p1, P2 p2, P3 p3);

	void Execute(P1 p1, P2 p2, P3 p3) { (*fn)(p1, p2, p3); }

	Callback3FnAction(void (*fn)(P1 p1, P2 p2, P3 p3)) : fn(fn) {}
};

template <class P1, class P2, class P3>
class Callback3 : Moveable< Callback3<P1, P2, P3> > {
	Callback3Action<P1, P2, P3> *action;

	void Retain() const { if(action ) AtomicInc(action->count); }
	void Release()      { if(action  && AtomicDec(action->count) == 0) delete action; }

	bool operator==(const Callback3&);
	bool operator!=(const Callback3&);

public:
	typedef Callback3 CLASSNAME;

	Callback3& operator=(const Callback3& c);
	Callback3(const Callback3& c);
	void Clear()        { Release(); action = NULL; }


	operator bool() const                       { return action && action->IsValid(); }
	void Execute(P1 p1, P2 p2, P3 p3) const     { if(action) action->Execute(p1, p2, p3); }
	void operator()(P1 p1, P2 p2, P3 p3) const  { Execute(p1, p2, p3); }

	explicit Callback3(Callback3Action <P1, P2, P3> *newaction) { action = newaction; }
	Callback3() { action = NULL; }
	Callback3(_CNULL) { action = NULL; }
	~Callback3();

	static Callback3 Empty() { return CNULL; }

};

template <class OBJECT, class METHOD, class P1, class P2, class P3>
Callback3<P1, P2, P3> pteback(OBJECT *object, void (METHOD::*method)(P1 p1, P2 p2, P3 p3)) {
	return Callback3<P1, P2, P3>(new Callback3MethodActionPte<OBJECT, void (METHOD::*)(P1 p1, P2 p2, P3 p3), P1, P2, P3>(object, method));
}

template <class OBJECT, class METHOD, class P1, class P2, class P3>
Callback3<P1, P2, P3> callback(OBJECT *object, void (METHOD::*method)(P1 p1, P2 p2, P3 p3)) {
	return Callback3<P1, P2, P3>(new Callback3MethodAction<OBJECT, void (METHOD::*)(P1 p1, P2 p2, P3 p3), P1, P2, P3>(object, method));
}

template <class OBJECT, class METHOD, class P1, class P2, class P3>
Callback3<P1, P2, P3> callback(const OBJECT *object, void (METHOD::*method)(P1 p1, P2 p2, P3 p3) const) {
	return Callback3<P1, P2, P3>(new Callback3MethodAction<const OBJECT, void (METHOD::*)(P1 p1, P2 p2, P3 p3) const, P1, P2, P3>(object, method));
}

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

template <class P1, class P2, class P3>
Callback3<P1, P2, P3>& Callback3<P1, P2, P3>::operator=(const Callback3& c)
{
	c.Retain();
	Release();
	action = c.action;
	return *this;
}

template <class P1, class P2, class P3>
Callback3<P1, P2, P3>::Callback3(const Callback3& c)
{
	action = c.action;
	Retain();
}

template <class P1, class P2, class P3>
Callback3<P1, P2, P3>::~Callback3()
{
	Release();
}

// -----------------------------------------------------------

template <class P1, class P2, class P3, class P4>
struct Callback4Action {
	Atomic  count;

	virtual void Execute(P1 p1, P2 p2, P3 p3, P4 p4) = 0;
	virtual bool IsValid() const { return true; }

	Callback4Action()          { count = 1; }
	virtual ~Callback4Action() {}
};

template <class OBJECT, class METHOD, class P1, class P2, class P3, class P4>
struct Callback4MethodActionPte : public Callback4Action<P1, P2, P3, P4> {
	Ptr<OBJECT>  object;
	METHOD       method;

	void Execute(P1 p1, P2 p2, P3 p3, P4 p4) { if(object) (object->*method)(p1, p2, p3, p4); }
	bool IsValid() const { return object; }

	Callback4MethodActionPte(OBJECT *object, METHOD method) : object(object), method(method) {}
};

template <class OBJECT, class METHOD, class P1, class P2, class P3, class P4>
struct Callback4MethodAction : public Callback4Action<P1, P2, P3, P4> {
	OBJECT  *object;
	METHOD   method;

	void Execute(P1 p1, P2 p2, P3 p3, P4 p4) { (object->*method)(p1, p2, p3, p4); }

	Callback4MethodAction(OBJECT *object, METHOD method) : object(object), method(method) {}
};

template <class P1, class P2, class P3, class P4>
struct Callback4FnAction : public Callback4Action<P1, P2, P3, P4> {
	void (*fn)(P1 p1, P2 p2, P3 p3, P4 p4);

	void Execute(P1 p1, P2 p2, P3 p3, P4 p4) { (*fn)(p1, p2, p3, p4); }

	Callback4FnAction(void (*fn)(P1 p1, P2 p2, P3 p3, P4 p4)) : fn(fn) {}
};

template <class P1, class P2, class P3, class P4>
class Callback4 : Moveable< Callback4<P1, P2, P3, P4> > {
	Callback4Action<P1, P2, P3, P4> *action;

	void Retain() const { if(action ) AtomicInc(action->count); }
	void Release()      { if(action  && AtomicDec(action->count) == 0) delete action; }

	bool operator==(const Callback4&);
	bool operator!=(const Callback4&);

public:
	typedef Callback4 CLASSNAME;

	Callback4& operator=(const Callback4& c);
	Callback4(const Callback4& c);
	void Clear()        { Release(); action = NULL; }


	operator bool() const                              { return action && action->IsValid(); }
	void Execute(P1 p1, P2 p2, P3 p3, P4 p4) const     { if(action) action->Execute(p1, p2, p3, p4); }
	void operator()(P1 p1, P2 p2, P3 p3, P4 p4) const  { Execute(p1, p2, p3, p4); }

	explicit Callback4(Callback4Action <P1, P2, P3, P4> *newaction) { action = newaction; }
	Callback4() { action = NULL; }
	Callback4(_CNULL) { action = NULL; }
	~Callback4();

	static Callback4 Empty() { return CNULL; }

};

template <class OBJECT, class METHOD, class P1, class P2, class P3, class P4>
Callback4<P1, P2, P3, P4> pteback(OBJECT *object, void (METHOD::*method)(P1 p1, P2 p2, P3 p3, P4 p4)) {
	return Callback4<P1, P2, P3, P4>(new Callback4MethodActionPte<OBJECT, void (METHOD::*)(P1 p1, P2 p2, P3 p3, P4 p4), P1, P2, P3, P4>(object, method));
}

template <class OBJECT, class METHOD, class P1, class P2, class P3, class P4>
Callback4<P1, P2, P3, P4> callback(OBJECT *object, void (METHOD::*method)(P1 p1, P2 p2, P3 p3, P4 p4)) {
	return Callback4<P1, P2, P3, P4>(new Callback4MethodAction<OBJECT, void (METHOD::*)(P1 p1, P2 p2, P3 p3, P4 p4), P1, P2, P3, P4>(object, method));
}

template <class OBJECT, class METHOD, class P1, class P2, class P3, class P4>
Callback4<P1, P2, P3, P4> callback(const OBJECT *object, void (METHOD::*method)(P1 p1, P2 p2, P3 p3, P4 p4) const) {
	return Callback4<P1, P2, P3, P4>(new Callback4MethodAction<const OBJECT, void (METHOD::*)(P1 p1, P2 p2, P3 p3, P4 p4) const, P1, P2, P3, P4>(object, method));
}

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

template <class P1, class P2, class P3, class P4>
Callback4<P1, P2, P3, P4>& Callback4<P1, P2, P3, P4>::operator=(const Callback4& c)
{
	c.Retain();
	Release();
	action = c.action;
	return *this;
}

template <class P1, class P2, class P3, class P4>
Callback4<P1, P2, P3, P4>::Callback4(const Callback4& c)
{
	action = c.action;
	Retain();
}

template <class P1, class P2, class P3, class P4>
Callback4<P1, P2, P3, P4>::~Callback4()
{
	Release();
}

// -----------------------------------------------------------

struct GateAction {
	Atomic  count;

	virtual bool Execute() = 0;
	virtual bool IsValid() const { return true; }

	GateAction()          { count = 1; }
	virtual ~GateAction() {}
};

template <class OBJECT, class METHOD>
struct GateMethodActionPte : public GateAction {
	Ptr<OBJECT>  object;
	METHOD       method;

	bool Execute() { return object ? (object->*method)() : false; }
	bool IsValid() const { return object; }

	GateMethodActionPte(OBJECT *object, METHOD method) : object(object), method(method) {}
};

template <class OBJECT, class METHOD>
struct GateMethodAction : public GateAction {
	OBJECT  *object;
	METHOD   method;

	bool Execute() { return (object->*method)(); }

	GateMethodAction(OBJECT *object, METHOD method) : object(object), method(method) {}
};

struct GateFnAction : public GateAction {
	bool (*fn)();

	bool Execute() { return (*fn)(); }

	GateFnAction(bool (*fn)()) : fn(fn) {}
};

class Gate : Moveable< Gate > {
	GateAction *action;

	void Retain() const { if(action && (void *)action != (void *)1) AtomicInc(action->count); }
	void Release()      { if(action && (void *)action != (void *)1 && AtomicDec(action->count) == 0) delete action; }

	bool operator==(const Gate&);
	bool operator!=(const Gate&);

public:
	typedef Gate CLASSNAME;

	Gate& operator=(const Gate& c);
	Gate(const Gate& c);
	void Clear()        { Release(); action = NULL; }


	operator bool() const        { return (void *)action != (void *)1 && action && action->IsValid(); }
	bool Execute() const;
	bool operator()() const      { return Execute(); }
	void ClearTrue()             { Clear(); action = (GateAction *)(uintptr_t)1; }
	void ClearFalse()            { Clear(); }

	Gate(bool b)                 { action = (GateAction *)(uintptr_t)b; }

	explicit Gate(GateAction  *newaction) { action = newaction; }
	Gate() { action = NULL; }
	Gate(_CNULL) { action = NULL; }
	~Gate();

	static Gate Empty() { return CNULL; }

};

template <class OBJECT, class METHOD>
Gate pteback(OBJECT *object, bool (METHOD::*method)()) {
	return Gate(new GateMethodActionPte<OBJECT, bool (METHOD::*)()>(object, method));
}

template <class OBJECT, class METHOD>
Gate callback(OBJECT *object, bool (METHOD::*method)()) {
	return Gate(new GateMethodAction<OBJECT, bool (METHOD::*)()>(object, method));
}

template <class OBJECT, class METHOD>
Gate callback(const OBJECT *object, bool (METHOD::*method)() const) {
	return Gate(new GateMethodAction<const OBJECT, bool (METHOD::*)() const>(object, method));
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

#endif
#ifdef CPP_PART__

Gate callback(Gate cb1, Gate cb2)
{
	return Gate(new GateForkAction (cb1, cb2));
}

Gate& operator<<(Gate& a, Gate b)
{
	if(a)
		a = callback(a, b);
	else
		a = b;
	return a;
}

Gate& Gate::operator=(const Gate& c)
{
	c.Retain();
	Release();
	action = c.action;
	return *this;
}

Gate::Gate(const Gate& c)
{
	action = c.action;
	Retain();
}

Gate::~Gate()
{
	Release();
}

#endif
#ifndef CPP_PART__

// -----------------------------------------------------------

template <class P1>
struct Gate1Action {
	Atomic  count;

	virtual bool Execute(P1 p1) = 0;
	virtual bool IsValid() const { return true; }

	Gate1Action()          { count = 1; }
	virtual ~Gate1Action() {}
};

template <class OBJECT, class METHOD, class P1>
struct Gate1MethodActionPte : public Gate1Action<P1> {
	Ptr<OBJECT>  object;
	METHOD       method;

	bool Execute(P1 p1) { return object ? (object->*method)(p1) : false; }
	bool IsValid() const { return object; }

	Gate1MethodActionPte(OBJECT *object, METHOD method) : object(object), method(method) {}
};

template <class OBJECT, class METHOD, class P1>
struct Gate1MethodAction : public Gate1Action<P1> {
	OBJECT  *object;
	METHOD   method;

	bool Execute(P1 p1) { return (object->*method)(p1); }

	Gate1MethodAction(OBJECT *object, METHOD method) : object(object), method(method) {}
};

template <class P1>
struct Gate1FnAction : public Gate1Action<P1> {
	bool (*fn)(P1 p1);

	bool Execute(P1 p1) { return (*fn)(p1); }

	Gate1FnAction(bool (*fn)(P1 p1)) : fn(fn) {}
};

template <class P1>
class Gate1 : Moveable< Gate1<P1> > {
	Gate1Action<P1> *action;

	void Retain() const { if(action && (void *)action != (void *)1) AtomicInc(action->count); }
	void Release()      { if(action && (void *)action != (void *)1 && AtomicDec(action->count) == 0) delete action; }

	bool operator==(const Gate1&);
	bool operator!=(const Gate1&);

public:
	typedef Gate1 CLASSNAME;

	Gate1& operator=(const Gate1& c);
	Gate1(const Gate1& c);
	void Clear()        { Release(); action = NULL; }


	operator bool() const        { return (void *)action != (void *)1 && action && action->IsValid(); }
	bool Execute(P1 p1) const;
	bool operator()(P1 p1) const { return Execute(p1); }
	void ClearTrue()             { Clear(); action = (Gate1Action<P1> *)1; }
	void ClearFalse()            { Clear(); }

	Gate1(bool b)                { action = (Gate1Action<P1> *)(uintptr_t)b; }

	explicit Gate1(Gate1Action <P1> *newaction) { action = newaction; }
	Gate1() { action = NULL; }
	Gate1(_CNULL) { action = NULL; }
	~Gate1();

	static Gate1 Empty() { return CNULL; }

};

template <class OBJECT, class METHOD, class P1>
Gate1<P1> pteback(OBJECT *object, bool (METHOD::*method)(P1 p1)) {
	return Gate1<P1>(new Gate1MethodActionPte<OBJECT, bool (METHOD::*)(P1 p1), P1>(object, method));
}

template <class OBJECT, class METHOD, class P1>
Gate1<P1> callback(OBJECT *object, bool (METHOD::*method)(P1 p1)) {
	return Gate1<P1>(new Gate1MethodAction<OBJECT, bool (METHOD::*)(P1 p1), P1>(object, method));
}

template <class OBJECT, class METHOD, class P1>
Gate1<P1> callback(const OBJECT *object, bool (METHOD::*method)(P1 p1) const) {
	return Gate1<P1>(new Gate1MethodAction<const OBJECT, bool (METHOD::*)(P1 p1) const, P1>(object, method));
}

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

template <class P1>
Gate1<P1>& Gate1<P1>::operator=(const Gate1& c)
{
	c.Retain();
	Release();
	action = c.action;
	return *this;
}

template <class P1>
Gate1<P1>::Gate1(const Gate1& c)
{
	action = c.action;
	Retain();
}

template <class P1>
Gate1<P1>::~Gate1()
{
	Release();
}

// -----------------------------------------------------------

template <class P1, class P2>
struct Gate2Action {
	Atomic  count;

	virtual bool Execute(P1 p1, P2 p2) = 0;
	virtual bool IsValid() const { return true; }

	Gate2Action()          { count = 1; }
	virtual ~Gate2Action() {}
};

template <class OBJECT, class METHOD, class P1, class P2>
struct Gate2MethodActionPte : public Gate2Action<P1, P2> {
	Ptr<OBJECT>  object;
	METHOD       method;

	bool Execute(P1 p1, P2 p2) { return object ? (object->*method)(p1, p2) : false; }
	bool IsValid() const { return object; }

	Gate2MethodActionPte(OBJECT *object, METHOD method) : object(object), method(method) {}
};

template <class OBJECT, class METHOD, class P1, class P2>
struct Gate2MethodAction : public Gate2Action<P1, P2> {
	OBJECT  *object;
	METHOD   method;

	bool Execute(P1 p1, P2 p2) { return (object->*method)(p1, p2); }

	Gate2MethodAction(OBJECT *object, METHOD method) : object(object), method(method) {}
};

template <class P1, class P2>
struct Gate2FnAction : public Gate2Action<P1, P2> {
	bool (*fn)(P1 p1, P2 p2);

	bool Execute(P1 p1, P2 p2) { return (*fn)(p1, p2); }

	Gate2FnAction(bool (*fn)(P1 p1, P2 p2)) : fn(fn) {}
};

template <class P1, class P2>
class Gate2 : Moveable< Gate2<P1, P2> > {
	Gate2Action<P1, P2> *action;

	void Retain() const { if(action && (void *)action != (void *)1) AtomicInc(action->count); }
	void Release()      { if(action && (void *)action != (void *)1 && AtomicDec(action->count) == 0) delete action; }

	bool operator==(const Gate2&);
	bool operator!=(const Gate2&);

public:
	typedef Gate2 CLASSNAME;

	Gate2& operator=(const Gate2& c);
	Gate2(const Gate2& c);
	void Clear()        { Release(); action = NULL; }


	operator bool() const        { return (void *)action != (void *)1 && action && action->IsValid(); }
	bool Execute(P1 p1, P2 p2) const;
	bool operator()(P1 p1, P2 p2) const { return Execute(p1, p2); }
	void ClearTrue()             { Clear(); action = (Gate2Action<P1, P2> *)1; }
	void ClearFalse()            { Clear(); }

	Gate2(bool b)                { action = (Gate2Action<P1, P2> *)(uintptr_t)b; }

	explicit Gate2(Gate2Action <P1, P2> *newaction) { action = newaction; }
	Gate2() { action = NULL; }
	Gate2(_CNULL) { action = NULL; }
	~Gate2();

	static Gate2 Empty() { return CNULL; }

};

template <class OBJECT, class METHOD, class P1, class P2>
Gate2<P1, P2> pteback(OBJECT *object, bool (METHOD::*method)(P1 p1, P2 p2)) {
	return Gate2<P1, P2>(new Gate2MethodActionPte<OBJECT, bool (METHOD::*)(P1 p1, P2 p2), P1, P2>(object, method));
}

template <class OBJECT, class METHOD, class P1, class P2>
Gate2<P1, P2> callback(OBJECT *object, bool (METHOD::*method)(P1 p1, P2 p2)) {
	return Gate2<P1, P2>(new Gate2MethodAction<OBJECT, bool (METHOD::*)(P1 p1, P2 p2), P1, P2>(object, method));
}

template <class OBJECT, class METHOD, class P1, class P2>
Gate2<P1, P2> callback(const OBJECT *object, bool (METHOD::*method)(P1 p1, P2 p2) const) {
	return Gate2<P1, P2>(new Gate2MethodAction<const OBJECT, bool (METHOD::*)(P1 p1, P2 p2) const, P1, P2>(object, method));
}

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

template <class P1, class P2>
Gate2<P1, P2>& Gate2<P1, P2>::operator=(const Gate2& c)
{
	c.Retain();
	Release();
	action = c.action;
	return *this;
}

template <class P1, class P2>
Gate2<P1, P2>::Gate2(const Gate2& c)
{
	action = c.action;
	Retain();
}

template <class P1, class P2>
Gate2<P1, P2>::~Gate2()
{
	Release();
}

// -----------------------------------------------------------

template <class P1, class P2, class P3>
struct Gate3Action {
	Atomic  count;

	virtual bool Execute(P1 p1, P2 p2, P3 p3) = 0;
	virtual bool IsValid() const { return true; }

	Gate3Action()          { count = 1; }
	virtual ~Gate3Action() {}
};

template <class OBJECT, class METHOD, class P1, class P2, class P3>
struct Gate3MethodActionPte : public Gate3Action<P1, P2, P3> {
	Ptr<OBJECT>  object;
	METHOD       method;

	bool Execute(P1 p1, P2 p2, P3 p3) { return object ? (object->*method)(p1, p2, p3) : false; }
	bool IsValid() const { return object; }

	Gate3MethodActionPte(OBJECT *object, METHOD method) : object(object), method(method) {}
};

template <class OBJECT, class METHOD, class P1, class P2, class P3>
struct Gate3MethodAction : public Gate3Action<P1, P2, P3> {
	OBJECT  *object;
	METHOD   method;

	bool Execute(P1 p1, P2 p2, P3 p3) { return (object->*method)(p1, p2, p3); }

	Gate3MethodAction(OBJECT *object, METHOD method) : object(object), method(method) {}
};

template <class P1, class P2, class P3>
struct Gate3FnAction : public Gate3Action<P1, P2, P3> {
	bool (*fn)(P1 p1, P2 p2, P3 p3);

	bool Execute(P1 p1, P2 p2, P3 p3) { return (*fn)(p1, p2, p3); }

	Gate3FnAction(bool (*fn)(P1 p1, P2 p2, P3 p3)) : fn(fn) {}
};

template <class P1, class P2, class P3>
class Gate3 : Moveable< Gate3<P1, P2, P3> > {
	Gate3Action<P1, P2, P3> *action;

	void Retain() const { if(action && (void *)action != (void *)1) AtomicInc(action->count); }
	void Release()      { if(action && (void *)action != (void *)1 && AtomicDec(action->count) == 0) delete action; }

	bool operator==(const Gate3&);
	bool operator!=(const Gate3&);

public:
	typedef Gate3 CLASSNAME;

	Gate3& operator=(const Gate3& c);
	Gate3(const Gate3& c);
	void Clear()        { Release(); action = NULL; }


	operator bool() const        { return (void *)action != (void *)1 && action && action->IsValid(); }
	bool Execute(P1 p1, P2 p2, P3 p3) const;
	bool operator()(P1 p1, P2 p2, P3 p3) const { return Execute(p1, p2, p3); }
	void ClearTrue()             { Clear(); action = (Gate3Action<P1, P2, P3> *)1; }
	void ClearFalse()            { Clear(); }

	Gate3(bool b)                { action = (Gate3Action<P1, P2, P3> *)(uintptr_t)b; }

	explicit Gate3(Gate3Action <P1, P2, P3> *newaction) { action = newaction; }
	Gate3() { action = NULL; }
	Gate3(_CNULL) { action = NULL; }
	~Gate3();

	static Gate3 Empty() { return CNULL; }

};

template <class OBJECT, class METHOD, class P1, class P2, class P3>
Gate3<P1, P2, P3> pteback(OBJECT *object, bool (METHOD::*method)(P1 p1, P2 p2, P3 p3)) {
	return Gate3<P1, P2, P3>(new Gate3MethodActionPte<OBJECT, bool (METHOD::*)(P1 p1, P2 p2, P3 p3), P1, P2, P3>(object, method));
}

template <class OBJECT, class METHOD, class P1, class P2, class P3>
Gate3<P1, P2, P3> callback(OBJECT *object, bool (METHOD::*method)(P1 p1, P2 p2, P3 p3)) {
	return Gate3<P1, P2, P3>(new Gate3MethodAction<OBJECT, bool (METHOD::*)(P1 p1, P2 p2, P3 p3), P1, P2, P3>(object, method));
}

template <class OBJECT, class METHOD, class P1, class P2, class P3>
Gate3<P1, P2, P3> callback(const OBJECT *object, bool (METHOD::*method)(P1 p1, P2 p2, P3 p3) const) {
	return Gate3<P1, P2, P3>(new Gate3MethodAction<const OBJECT, bool (METHOD::*)(P1 p1, P2 p2, P3 p3) const, P1, P2, P3>(object, method));
}

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

template <class P1, class P2, class P3>
Gate3<P1, P2, P3>& Gate3<P1, P2, P3>::operator=(const Gate3& c)
{
	c.Retain();
	Release();
	action = c.action;
	return *this;
}

template <class P1, class P2, class P3>
Gate3<P1, P2, P3>::Gate3(const Gate3& c)
{
	action = c.action;
	Retain();
}

template <class P1, class P2, class P3>
Gate3<P1, P2, P3>::~Gate3()
{
	Release();
}

// -----------------------------------------------------------

template <class P1, class P2, class P3, class P4>
struct Gate4Action {
	Atomic  count;

	virtual bool Execute(P1 p1, P2 p2, P3 p3, P4 p4) = 0;
	virtual bool IsValid() const { return true; }

	Gate4Action()          { count = 1; }
	virtual ~Gate4Action() {}
};

template <class OBJECT, class METHOD, class P1, class P2, class P3, class P4>
struct Gate4MethodActionPte : public Gate4Action<P1, P2, P3, P4> {
	Ptr<OBJECT>  object;
	METHOD       method;

	bool Execute(P1 p1, P2 p2, P3 p3, P4 p4) { return object ? (object->*method)(p1, p2, p3, p4) : false; }
	bool IsValid() const { return object; }

	Gate4MethodActionPte(OBJECT *object, METHOD method) : object(object), method(method) {}
};

template <class OBJECT, class METHOD, class P1, class P2, class P3, class P4>
struct Gate4MethodAction : public Gate4Action<P1, P2, P3, P4> {
	OBJECT  *object;
	METHOD   method;

	bool Execute(P1 p1, P2 p2, P3 p3, P4 p4) { return (object->*method)(p1, p2, p3, p4); }

	Gate4MethodAction(OBJECT *object, METHOD method) : object(object), method(method) {}
};

template <class P1, class P2, class P3, class P4>
struct Gate4FnAction : public Gate4Action<P1, P2, P3, P4> {
	bool (*fn)(P1 p1, P2 p2, P3 p3, P4 p4);

	bool Execute(P1 p1, P2 p2, P3 p3, P4 p4) { return (*fn)(p1, p2, p3, p4); }

	Gate4FnAction(bool (*fn)(P1 p1, P2 p2, P3 p3, P4 p4)) : fn(fn) {}
};

template <class P1, class P2, class P3, class P4>
class Gate4 : Moveable< Gate4<P1, P2, P3, P4> > {
	Gate4Action<P1, P2, P3, P4> *action;

	void Retain() const { if(action && (void *)action != (void *)1) AtomicInc(action->count); }
	void Release()      { if(action && (void *)action != (void *)1 && AtomicDec(action->count) == 0) delete action; }

	bool operator==(const Gate4&);
	bool operator!=(const Gate4&);

public:
	typedef Gate4 CLASSNAME;

	Gate4& operator=(const Gate4& c);
	Gate4(const Gate4& c);
	void Clear()        { Release(); action = NULL; }


	operator bool() const        { return (void *)action != (void *)1 && action && action->IsValid(); }
	bool Execute(P1 p1, P2 p2, P3 p3, P4 p4) const;
	bool operator()(P1 p1, P2 p2, P3 p3, P4 p4) const { return Execute(p1, p2, p3, p4); }
	void ClearTrue()             { Clear(); action = (Gate4Action<P1, P2, P3, P4> *)1; }
	void ClearFalse()            { Clear(); }

	Gate4(bool b)                { action = (Gate4Action<P1, P2, P3, P4> *)(uintptr_t)b; }

	explicit Gate4(Gate4Action <P1, P2, P3, P4> *newaction) { action = newaction; }
	Gate4() { action = NULL; }
	Gate4(_CNULL) { action = NULL; }
	~Gate4();

	static Gate4 Empty() { return CNULL; }

};

template <class OBJECT, class METHOD, class P1, class P2, class P3, class P4>
Gate4<P1, P2, P3, P4> pteback(OBJECT *object, bool (METHOD::*method)(P1 p1, P2 p2, P3 p3, P4 p4)) {
	return Gate4<P1, P2, P3, P4>(new Gate4MethodActionPte<OBJECT, bool (METHOD::*)(P1 p1, P2 p2, P3 p3, P4 p4), P1, P2, P3, P4>(object, method));
}

template <class OBJECT, class METHOD, class P1, class P2, class P3, class P4>
Gate4<P1, P2, P3, P4> callback(OBJECT *object, bool (METHOD::*method)(P1 p1, P2 p2, P3 p3, P4 p4)) {
	return Gate4<P1, P2, P3, P4>(new Gate4MethodAction<OBJECT, bool (METHOD::*)(P1 p1, P2 p2, P3 p3, P4 p4), P1, P2, P3, P4>(object, method));
}

template <class OBJECT, class METHOD, class P1, class P2, class P3, class P4>
Gate4<P1, P2, P3, P4> callback(const OBJECT *object, bool (METHOD::*method)(P1 p1, P2 p2, P3 p3, P4 p4) const) {
	return Gate4<P1, P2, P3, P4>(new Gate4MethodAction<const OBJECT, bool (METHOD::*)(P1 p1, P2 p2, P3 p3, P4 p4) const, P1, P2, P3, P4>(object, method));
}

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

template <class P1, class P2, class P3, class P4>
Gate4<P1, P2, P3, P4>& Gate4<P1, P2, P3, P4>::operator=(const Gate4& c)
{
	c.Retain();
	Release();
	action = c.action;
	return *this;
}

template <class P1, class P2, class P3, class P4>
Gate4<P1, P2, P3, P4>::Gate4(const Gate4& c)
{
	action = c.action;
	Retain();
}

template <class P1, class P2, class P3, class P4>
Gate4<P1, P2, P3, P4>::~Gate4()
{
	Release();
}


#endif
