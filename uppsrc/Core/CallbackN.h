#define CallbackTemplate(name, rettype, template_, template__, _template, _tsep_, classdef, \
                         classlist, paramdef, paramlist, return_, extension) \
  \
template_ classdef _template  \
struct name##Action {  \
	int     count;  \
	virtual rettype  Execute(paramdef) = 0;  \
	virtual bool     IsEqual(const name##Action *other) const = 0; \
	virtual unsigned GetHashValue() const = 0; \
  \
	name##Action()          { count = 1; }  \
	virtual ~name##Action() {}  \
};  \
  \
template <class OBJECT_, class METHOD_ _tsep_ classdef>  \
struct name##MethodAction : public name##Action template__ classlist _template {  \
	OBJECT_  *object;  \
	METHOD_   method; \
	rettype Execute(paramdef) { return_ (object->*method)(paramlist); }  \
	bool    IsEqual(const name##Action template__ classlist _template *other) const { \
		const name##MethodAction *q = dynamic_cast<const name##MethodAction *>(other); \
		return q ? q->object == object && q->method == method : false; \
	} \
	unsigned GetHashValue() const { \
		return (unsigned)object ^ brutal_cast<unsigned>(method); \
	} \
   \
	name##MethodAction(OBJECT_ *object, METHOD_ method) \
	: object(object), method(method) {}  \
};  \
  \
template_ classdef _template  \
struct name##FnAction : public name##Action template__ classlist _template {  \
	rettype (*fn)(paramdef);  \
	rettype Execute(paramdef) { return_ (*fn)(paramlist); }  \
	bool    IsEqual(const name##Action template__ classlist _template *other) const { \
		const name##FnAction *q = dynamic_cast<const name##FnAction *>(other); \
		return q ? q->fn == fn : false; \
	} \
	unsigned GetHashValue() const { \
		return (unsigned)fn; \
	} \
  \
	name##FnAction(rettype (*fn)(paramdef)) : fn(fn) {}  \
};  \
 \
template_ classdef _template  \
class name : Moveable< name template__ classlist _template > {  \
	name##Action template__ classlist _template *action;  \
  \
	void Retain() const                      { if(*this) action->count++; }  \
	void Release()                           { if(*this && --action->count == 0) delete action; }  \
  \
public:  \
	typedef name CLASSNAME; \
	name& operator=(const name& c); \
	name(const name& c); \
	void Clear()                             { Release(); action = NULL; } \
	extension \
\
	unsigned GetHashValue() const            { return action->GetHashValue(); } \
\
	explicit name(name##Action template__ classlist _template *newaction) { action = newaction; } \
	name& operator=(name##Action template__ classlist _template *newaction) { action = newaction; return *this; } \
	name()                                   { action = NULL; }  \
	name(_CNULL)                             { action = NULL; } \
	~name(); \
\
	static name Empty()                      { return CNULL; } \
\
	friend bool operator==(const name& a, const name& b) { return a.action ? a.action->IsEqual(b.action) : !b.action; } \
	friend bool operator!=(const name& a, const name& b) { return !(a == b); } \
	/*friend unsigned GetHashValue(const name& m)          { return m.action->GetHashValue(); }*/ \
}; \
 \
template <class Object, class M  _tsep_ classdef>  \
name template__ classlist _template callback(Object *object, rettype (M::*method)(paramdef)) {  \
	return name template__ classlist _template \
		(new name##MethodAction<M, rettype (M::*)(paramdef) _tsep_ classlist>(object, method)); \
} \
 \
template <class Object, class M  _tsep_ classdef>  \
name template__ classlist _template callback(const Object *object, rettype (M::*method)(paramdef) const) { \
	return name template__ classlist _template \
		(new name##MethodAction<const M, rettype (M::*)(paramdef) const _tsep_ classlist>(object, method)); \
} \
 \
template_ classdef _template  \
inline name template__ classlist _template callback(rettype (*fn)(paramdef)) {  \
	return name template__ classlist _template \
		(new name##FnAction template__ classlist _template(fn)); \
} \
template_ classdef _template  \
struct name##ForkAction : public name##Action template__ classlist _template {  \
	name template__ classlist _template cb1, cb2;  \
	rettype Execute(paramdef) { cb1(paramlist); return_ cb2(paramlist); }  \
	bool    IsEqual(const name##Action template__ classlist _template *other) const { \
		const name##ForkAction *q = dynamic_cast<const name##ForkAction *>(other); \
		return q ? q->cb1 == cb1 && q->cb2 == cb2 : false; \
	} \
	unsigned GetHashValue() const { \
		return ::GetHashValue(cb1) ^ ::GetHashValue(cb2); \
	} \
  \
	name##ForkAction(name template__ classlist _template cb1, \
	                 name template__ classlist _template cb2) : cb1(cb1), cb2(cb2) {}  \
};  \
 \
template_ classdef _template  \
inline \
name template__ classlist _template callback( \
                           name template__ classlist _template cb1, \
                           name template__ classlist _template cb2) {  \
	return name template__ classlist _template \
		(new name##ForkAction template__ classlist _template(cb1, cb2)); \
} \
 \
template_ classdef _template  \
inline \
name template__ classlist _template& operator<<( \
                          name template__ classlist _template& a, \
                          name template__ classlist _template b) { \
	if(a) \
		a = callback(a, b); \
	else \
		a = b; \
	return a; \
}

#define CallbackBodyTemplate(name, template_, tlist) \
template_ \
name tlist& name tlist::operator=(const name& c)  { \
	c.Retain(); Release(); action = c.action; return *this; \
} \
\
template_ \
name tlist::name(const name& c) { action = c.action; Retain(); } \
\
template_ \
name tlist::~name()             { Release(); }

#define _empty_

//$ struct Callback;

//$-

CallbackTemplate(
	/* name */       Callback,
	/* rettype */    void,
	/* template_ */  _empty_,
	/* template__ */ _empty_,
	/* _template */  _empty_,
	/* tsep */       _empty_,
	/* classdef */   _empty_,
	/* classlist */  _empty_,
	/* paramdef */   _empty_,
	/* paramlist */  _empty_,
	/* return_ */    _empty_,
	operator bool() const  { return action; }
	void operator()() const;
)

CallbackTemplate(
	/* name */       Callback1,
	/* rettype */    void,
	/* template_ */  template<,
	/* template__ */ <,
	/* _template */  >,
	/* tsep */       _cm_,
	/* classdef */   class P1,
	/* classlist */  P1,
	/* paramdef */   P1 p1,
	/* paramlist */  p1,
	/* return_ */    _empty_,
	operator bool() const          { return action; }
	void operator()(P1 p1) const   { if(action) action->Execute(p1); }
)

CallbackBodyTemplate(Callback1, template<class P1>, <P1>)
