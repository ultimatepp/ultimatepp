template <class T> class Ptr;

class PteBase {
protected:
	struct Prec {
		PteBase *ptr;
		Atomic   n;
	};

	volatile Prec  *prec;

	Prec           *PtrAdd();
	static void     PtrRelease(Prec *prec);
	static Prec    *PtrAdd(const Uuid& uuid);

	PteBase();
	~PteBase();

	friend class PtrBase;
};

class PtrBase {
protected:
	PteBase::Prec *prec;
	void Set(PteBase *p);
	void Release();
	void Assign(PteBase *p);

public:
	~PtrBase();
};

template <class T>
class Pte : public PteBase {
	friend class Ptr<T>;
};

template <class T>
class Ptr : public PtrBase, Moveable< Ptr<T> > {
	T   *Get() const                          { return prec ? static_cast<T *>(prec->ptr) : NULL; }

public:
	T       *operator->() const               { return Get(); }
	T       *operator~() const                { return Get(); }
	operator T*() const                       { return Get(); }

	Ptr& operator=(T *ptr)                    { Assign(ptr); return *this; }
	Ptr& operator=(const Ptr& ptr)            { Assign(ptr.Get()); return *this; }

	Ptr()                                     { prec = NULL; }
	Ptr(T *ptr)                               { Set(ptr); }
	Ptr(const Ptr& ptr)                       { Set(ptr.Get()); }

	String ToString() const;

	friend bool operator==(const Ptr& a, const T *b)   { return a.Get() == b; }
	friend bool operator==(const T *a, const Ptr& b)   { return a == b.Get(); }
	friend bool operator==(const Ptr& a, const Ptr& b) { return a.prec == b.prec; }

	friend bool operator==(const Ptr& a, T *b)         { return a.Get() == b; }
	friend bool operator==(T *a, const Ptr& b)         { return a == b.Get(); }

	friend bool operator!=(const Ptr& a, const T *b)   { return a.Get() != b; }
	friend bool operator!=(const T *a, const Ptr& b)   { return a != b.Get(); }
	friend bool operator!=(const Ptr& a, const Ptr& b) { return a.prec != b.prec; }

	friend bool operator!=(const Ptr& a, T *b)         { return a.Get() != b; }
	friend bool operator!=(T *a, const Ptr& b)         { return a != b.Get(); }
};

template <class T>
String Ptr<T>::ToString() const
{
	return prec ? FormatPtr(Get()) : String("0x0");
}
