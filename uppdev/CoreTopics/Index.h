#define FOLDHASH

enum { UNSIGNED_HIBIT = 0x80000000 };

class HashBase : Moveable<HashBase> {
	struct Link : Moveable<Link> {
		int   next;
		int   prev;
	};
	Vector<unsigned> hash;
	Vector<Link>     link;
	int             *map;
	int              mcount;
	int              unlinked;

	void  LinkBefore(int i, Link& l, int bi);
	void  LinkTo(int i, Link& l, int& m);
	void  Unlink(int i, Link& l, int& mi);
	void  Unlink(int i, Link& l);
	int&  Maph(unsigned _hash) const;
	int&  Mapi(int i) const;
	void  FinishIndex();
	void  DoIndex();
	void  Free();

public:
	void  ClearIndex();
	void  Reindex(int n);
	void  Reindex();

	void  Add(unsigned hash);
	void  Set(int i, unsigned hash);
	void  SetUn(int i, unsigned hash);
	unsigned operator [] (int i) const      { return hash[i]; }
	int   Find(unsigned hash) const;
	int   FindNext(int i) const;
	int   FindLast(unsigned hash) const;
	int   FindPrev(int i) const;
	int   Put(unsigned hash);

	bool  IsUnlinked(int i) const           { return hash[i] & UNSIGNED_HIBIT; }
	void  Unlink(int i);
	Vector<int> GetUnlinked() const;

	void  Remove(int i);
	void  Remove(int i, int count);
	void  Remove(const int *sorted_list, int count);
	void  Insert(int i, unsigned hash);

	int   GetCount() const                  { return hash.GetCount(); }
	void  Trim(int count);
	void  Drop(int n);
	void  Clear()                           { hash.Clear(); ClearIndex(); }

	void  Reserve(int n);
	void  Shrink();

#ifdef UPP
	void  Serialize(Stream& s);
#endif

	HashBase();
	~HashBase();

	HashBase(pick_ HashBase& b);
	void operator=(pick_ HashBase& b);
	HashBase(const HashBase& b, int);
	void operator<<=(const HashBase& b);

	bool  IsPicked() const                  { return hash.IsPicked(); }

	const unsigned *Begin() const           { return hash.Begin(); }
	const unsigned *End() const             { return hash.End(); }

	void Swap(HashBase& b);
};

template <class T>
struct StdHash {
	unsigned operator()(const T& x) const { return GetHashValue(x); }
};

struct PtrHash {
	unsigned operator()(const void *x) const { return GetHashValue((unsigned)(uintptr_t)x); }
};

template <class T, class V, class HashFn>
class AIndex {
protected:
	V         key;
	HashBase  hash;

	int      Find0(const T& x, int i) const {
		while(i >= 0 && !(x == key[i])) i = hash.FindNext(i);
		return i;
	}
	int      FindB(const T& x, int i) const {
		while(i >= 0 && !(x == key[i])) i = hash.FindPrev(i);
		return i;
	}
	void     Hash();

public:
	HashFn   hashfn;

	void     Add(const T& x, unsigned _hash);
	void     Add(const T& x);
	int      FindAdd(const T& key, unsigned _hash);
	int      FindAdd(const T& key);
	AIndex&  operator<<(const T& x)          { Add(x); return *this; }

	int      Put(const T& x, unsigned _hash);
	int      Put(const T& x);
	int      FindPut(const T& key, unsigned _hash);
	int      FindPut(const T& key);

	int      Find(const T& x, unsigned _hash) const;
	int      Find(const T& x) const;
	int      FindNext(int i) const;
	int      FindLast(const T& x, unsigned _hash) const;
	int      FindLast(const T& x) const;
	int      FindPrev(int i) const;

	void     Set(int i, const T& x, unsigned _hash);
	void     Set(int i, const T& x);

	const T& operator[](int i) const         { return key[i]; }
	int      GetCount() const                { return key.GetCount(); }
	bool     IsEmpty() const                 { return key.IsEmpty(); }

	void     Clear()                         { hash.Clear(); key.Clear(); }

	void     ClearIndex()                    { hash.ClearIndex(); }
	void     Reindex(int n)                  { hash.Reindex(n); }
	void     Reindex()                       { hash.Reindex(); }

	void     Unlink(int i)                   { hash.Unlink(i); }
	int      UnlinkKey(const T& k, unsigned h);
	int      UnlinkKey(const T& k);
	bool     IsUnlinked(int i) const         { return hash.IsUnlinked(i); }
	Vector<int> GetUnlinked() const          { return hash.GetUnlinked(); }
	void     Sweep();

	void     Insert(int i, const T& k, unsigned h);
	void     Insert(int i, const T& k);
	void     Remove(int i);
	void     Remove(int i, int count);
	void     Remove(const int *sorted_list, int count);
	void     Remove(const Vector<int>& sorted_list);
	int      RemoveKey(const T& k, unsigned h);
	int      RemoveKey(const T& k);

	void     Trim(int n)                     { key.SetCount(n); hash.Trim(n); }
	void     Drop(int n = 1)                 { key.Drop(n); hash.Drop(n); }
	const T& Top() const                     { return key.Top(); }

	void     Reserve(int n)                  { key.Reserve(n); hash.Reserve(n); }
	void     Shrink()                        { key.Shrink(); hash.Shrink(); }
	int      GetAlloc() const                { return key.GetAlloc(); }

#ifdef UPP
	void     Serialize(Stream& s);
#endif

	V        PickKeys() pick_                 { return key; }
	const V& GetKeys() const                  { return key; }

// Pick assignment & copy. Picked source can only Clear(), ~AIndex(), operator=, operator<<=

	AIndex& operator=(pick_ V& s);
	AIndex& operator<<=(const V& s);

// Standard container interface
	typedef T                ValueType;
	typedef typename V::ConstIterator ConstIterator;
	ConstIterator  Begin() const                          { return key.Begin(); }
	ConstIterator  End() const                            { return key.End(); }
	ConstIterator  GetIter(int pos) const                 { return key.GetIter(pos); }

	void Swap(AIndex& b)                                  { UPP::Swap(hash, b.hash);
	                                                        UPP::Swap(key, b.key); }
// Optimalizations
	friend int  GetCount(const AIndex& v)                 { return v.GetCount(); }

protected:
	AIndex(pick_ V& s);
	AIndex(const V& s, int);
	AIndex() {}
	AIndex(const AIndex& s, int);
};

template <class T, class HashFn = StdHash<T> >
class Index : MoveableAndDeepCopyOption< Index<T, HashFn > >,
              public AIndex<T, Vector<T>, HashFn> {
	typedef AIndex< T, Vector<T>, HashFn > B;
public:
	T        Pop()                           { T x = B::Top(); B::Drop(); return x; }

	Index() {}
	Index(pick_ Index& s) : B(s)             {}
	Index(const Index& s, int) : B(s, 1)     {}
	Index(pick_ Vector<T>& s) : B(s)         {}
	Index(const Vector<T>& s, int) : B(s, 1) {}

	Index& operator=(pick_ Vector<T>& x)     { B::operator=(x); return *this; }

	friend void Swap(Index& a, Index& b)     { a.B::Swap(b); }

	typedef typename B::ConstIterator ConstIterator; // GCC bug (?)
	STL_INDEX_COMPATIBILITY(Index<T _cm_ HashFn>)
};

template <class T, class HashFn = StdHash<T> >
class ArrayIndex : MoveableAndDeepCopyOption< ArrayIndex<T, HashFn > >,
                   public AIndex<T, Array<T>, HashFn> {
	typedef AIndex< T, Array<T>, HashFn > B;
public:
	void     Add(const T& x, unsigned _hash)        { B::Add(x, _hash); }
	void     Add(const T& x)                        { B::Add(x); }
	void     Set(int i, const T& x, unsigned _hash) { B::Set(i, x, _hash); }
	void     Set(int i, const T& x)                 { B::Set(i, x); }

	void     Add(T *newt, unsigned _hash);
	void     Add(T *newt);
	void     Set(int i, T *newt, unsigned _hash);
	void     Set(int i, T *newt);

	ArrayIndex() {}
	ArrayIndex(pick_ ArrayIndex& s) : B(s)          {}
	ArrayIndex(const ArrayIndex& s, int) : B(s, 1)  {}
	ArrayIndex(pick_ Array<T>& s) : B(s)            {}
	ArrayIndex(const Array<T>& s, int) : B(s, 1)    {}

	ArrayIndex& operator=(pick_ Array<T>& x)        { B::operator=(x); return *this; }

	friend void Swap(ArrayIndex& a, ArrayIndex& b)  { a.B::Swap(b); }

	typedef typename B::ConstIterator ConstIterator; // GCC bug (?)
	STL_INDEX_COMPATIBILITY(ArrayIndex<T _cm_ HashFn>)
};
