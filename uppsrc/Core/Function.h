enum CNULLer { CNULL }; // Deprecated, use plain Null

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

		virtual Res Execute(ArgTypes... args) {
			struct AtExit { // to void problems with returning the value
				Wrapper2 *ptr;
				~AtExit() { if(--ptr->refcount == 0) delete ptr; };
			};
			AtExit exit;
			exit.ptr = this;
			++WrapperBase::refcount; // prevent deletion of self
			l(args...);
			return fn(args...); // after this, AtExit does cleanup
		}

		Wrapper2(const Function& l, F&& fn) : l(l), fn(pick(fn)) {}
		Wrapper2(const Function& l, const F& fn) : l(l), fn(fn) {}
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
	
	void Pick(Function&& src) {
		ptr = src.ptr;
		src.ptr = NULL;
	}

public:
	Function()                                 { ptr = NULL; }
	Function(CNULLer)                          { ptr = NULL; }
	Function(const Nuller&)                    { ptr = NULL; }
	
	template <class F> Function(F fn)          { ptr = new Wrapper<F>(pick(fn)); }
	
	Function(const Function& src)              { Copy(src); }
	Function& operator=(const Function& src)   { auto b = ptr; Copy(src); Free(b); return *this; }

	Function(Function&& src)                   { Pick(pick(src)); }
	Function& operator=(Function&& src)        { if(&src != this) { Free(ptr); ptr = src.ptr; src.ptr = NULL; } return *this; }
	
	Function Proxy() const                     { return [=] (ArgTypes... args) { return (*this)(args...); }; }

	template <class F>
	Function& operator<<(F fn)                 { if(!ptr) { Pick(pick(fn)); return *this; }
	                                             WrapperBase *b = ptr; ptr = new Wrapper2<F>(*this, pick(fn)); Free(b); return *this; }

	Function& operator<<(const Function& fn)   { if(!ptr) { Copy(fn); return *this; }
	                                             WrapperBase *b = ptr; ptr = new Wrapper2<Function>(*this, fn); Free(b); return *this; }

	Function& operator<<(Function&& fn)        { if(!ptr) { Pick(pick(fn)); return *this; }
	                                             WrapperBase *b = ptr; ptr = new Wrapper2<Function>(*this, pick(fn)); Free(b); return *this; }

	Res operator()(ArgTypes... args) const     { return ptr ? ptr->Execute(args...) : Res(); }

	operator bool() const                      { return ptr; }
	void Clear()                               { Free(ptr); ptr = NULL; }

	~Function()                                { Free(ptr); }

	friend Function Proxy(const Function& a)   { return a.Proxy(); }
	friend void Swap(Function& a, Function& b) { UPP::Swap(a.ptr, b.ptr); }
};

template <typename... ArgTypes>
using Event = Function<void (ArgTypes...)>;

template <typename... ArgTypes>
using Gate = Function<bool (ArgTypes...)>;

template <class Ptr, class Class, class Res, class... ArgTypes>
Function<Res (ArgTypes...)> MemFn(Ptr object, Res (Class::*method)(ArgTypes...))
{
	return [=](ArgTypes... args) { return (object->*method)(args...); };
}

#define THISFN(x)   MemFn(this, &CLASSNAME::x)
