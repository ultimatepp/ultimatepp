template <class T> class Ptr;

class PteBase {
protected:
	struct Prec {
		const PteBase *ptr;
		Atomic         n;
	};

	mutable Prec   *prec = nullptr; // mutable as we need Ptr<const Foo> as well

	Prec           *PtrAdd() const;
	static void     PtrRelease(Prec *prec);

	~PteBase();

	friend class PtrBase;
};

class PtrBase {
protected:
	PteBase::Prec *prec;
	void Set(const PteBase *p);
	void Release();
	void Assign(const PteBase *p);

public:
	~PtrBase();
};

template <class T>
class Pte : public PteBase {
	friend class Ptr<T>;
};

template <class T>
class Ptr : public PtrBase, Moveable< Ptr<T> > {
	T   *Get() const                          { return prec ? static_cast<T *>(const_cast<PteBase *>(prec->ptr)) : NULL; }

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
};

template <class T>
String Ptr<T>::ToString() const
{
	return prec ? FormatPtr(Get()) : String("0x0");
}
