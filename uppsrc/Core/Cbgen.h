// -----------------------------------------------------------

struct CallbackAction {
	Atomic  count;

	virtual void Execute() = 0;
	virtual bool IsValid() const { return true; }

	CallbackAction()          { count = 1; }
	virtual ~CallbackAction() {}
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

// -----------------------------------------------------------

template <class P1>
struct Callback1Action {
	Atomic  count;

	virtual void Execute(P1 p1) = 0;
	virtual bool IsValid() const { return true; }

	Callback1Action()          { count = 1; }
	virtual ~Callback1Action() {}
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

// -----------------------------------------------------------

template <class P1>
struct Gate1Action {
	Atomic  count;

	virtual bool Execute(P1 p1) = 0;
	virtual bool IsValid() const { return true; }

	Gate1Action()          { count = 1; }
	virtual ~Gate1Action() {}
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
	bool Execute(P1 p1) const	 { return (void *)action == (void *)1 ? true : action ? action->Execute(p1) : false; }

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
	bool Execute(P1 p1, P2 p2) const { return (void *)action == (void *)1 ? true : action ? action->Execute(p1, p2) : false; }
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
	bool Execute(P1 p1, P2 p2, P3 p3) const { return (void *)action == (void *)1 ? true : action ? action->Execute(p1, p2, p3) : false; }
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
	bool Execute(P1 p1, P2 p2, P3 p3, P4 p4) const { return (void *)action == (void *)1 ? true : action ? action->Execute(p1, p2, p3, p4) : false; }
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
