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
	bool  HasUnlinked() const				{ return unlinked >= 0; }

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

	HashBase(HashBase rval_ b);
	void operator=(HashBase rval_ b);
	HashBase(const HashBase& b, int);
	void operator<<=(const HashBase& b);

	bool  IsPicked() const                  { return hash.IsPicked(); }

	const unsigned *Begin() const           { return hash.Begin(); }
	const unsigned *End() const             { return hash.End(); }

	void Swap(HashBase& b);
};

template <class T>
struct StdHash {
	unsigned operator()(const T& x) const    { return GetHashValue(x); }
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
	unsigned hashfn(const T& x) const             { return HashFn()(x); }

	T&       Add(const T& x, unsigned _hash);
	T&       Add(const T& x);
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

	T&       Set(int i, const T& x, unsigned _hash);
	T&       Set(int i, const T& x);

	const T& operator[](int i) const         { return key[i]; }
	int      GetCount() const                { return key.GetCount(); }
	bool     IsEmpty() const                 { return key.IsEmpty(); }
	
	unsigned GetHash(int i) const            { return hash[i]; }

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
	bool     HasUnlinked() const             { return hash.HasUnlinked(); }

	T&       Insert(int i, const T& k, unsigned h);
	T&       Insert(int i, const T& k);
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
	void     Xmlize(XmlIO& xio, const char *itemtag = "key");
	void     Jsonize(JsonIO& jio);
	String   ToString() const;
	bool     operator==(const AIndex& b) const { return IsEqualArray(*this, b); }
	bool     operator!=(const AIndex& b) const { return !operator==(b); }
	int      Compare(const AIndex& b) const    { return CompareArray(*this, b); }
	bool     operator<=(const AIndex& x) const { return Compare(x) <= 0; }
	bool     operator>=(const AIndex& x) const { return Compare(x) >= 0; }
	bool     operator<(const AIndex& x) const  { return Compare(x) < 0; }
	bool     operator>(const AIndex& x) const  { return Compare(x) > 0; }
#endif

	V        PickKeys() pick_                  { return pick(key); }
	const V& GetKeys() const                   { return key; }
	bool     IsPicked(void) const              { return key.IsPicked(); }

// Pick assignment & copy. Picked source can only Clear(), ~AIndex(), operator=, operator<<=

	AIndex& operator=(V rval_ s);
//	AIndex& operator=(AIndex rval_ s) = default;
	AIndex& operator<<=(const V& s);

// Standard container interface
	typedef T                ValueType;
	typedef V                ValueContainer;
	typedef typename V::ConstIterator ConstIterator;
	ConstIterator  Begin() const                          { return key.Begin(); }
	ConstIterator  End() const                            { return key.End(); }
	ConstIterator  GetIter(int pos) const                 { return key.GetIter(pos); }

	void Swap(AIndex& b)                                  { UPP::Swap(hash, b.hash);
	                                                        UPP::Swap(key, b.key); }
// Optimalizations
	friend int  GetCount(const AIndex& v)                 { return v.GetCount(); }

protected:
	AIndex(V rval_ s);
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
	Index(Index rval_ s) : B(pick(s))        {}
	Index(const Index& s, int) : B(s, 1)     {}
	explicit Index(Vector<T> rval_ s) : B(pick(s)) {}
	Index(const Vector<T>& s, int) : B(s, 1) {}

	Index& operator=(Vector<T> rval_ x)      { B::operator=(pick(x)); return *this; }
	Index& operator=(Index<T> rval_ x)       { B::operator=(pick(x)); return *this; }

	friend void Swap(Index& a, Index& b)     { a.B::Swap(b); }

	typedef typename B::ConstIterator ConstIterator; // GCC bug (?)
	STL_INDEX_COMPATIBILITY(Index<T _cm_ HashFn>)
};

template <class T, class HashFn = StdHash<T> >
class ArrayIndex : MoveableAndDeepCopyOption< ArrayIndex<T, HashFn > >,
                   public AIndex<T, Array<T>, HashFn> {
	typedef AIndex< T, Array<T>, HashFn > B;
public:
	T&       Add(const T& x, unsigned _hash)        { return B::Add(x, _hash); }
	T&       Add(const T& x)                        { return B::Add(x); }
	T&       Set(int i, const T& x, unsigned _hash) { return B::Set(i, x, _hash); }
	T&       Set(int i, const T& x)                 { return B::Set(i, x); }

	T&       Add(T *newt, unsigned _hash);
	T&       Add(T *newt);
	T&       Set(int i, T *newt, unsigned _hash);
	T&       Set(int i, T *newt);

	T       *PopDetach()                            { B::hash.Drop(1); return B::key.PopDetach(); }
	T       *Detach(int i)                          { B::hash.Remove(i); return B::key.Detach(i); }

	ArrayIndex() {}
	ArrayIndex(ArrayIndex rval_ s) : B(pick(s))          {}
	ArrayIndex(const ArrayIndex& s, int) : B(s, 1)       {}
	explicit ArrayIndex(Array<T> rval_ s) : B(pick(s))   {}
	ArrayIndex(const Array<T>& s, int) : B(s, 1)         {}

	ArrayIndex& operator=(Array<T> rval_ x)              { B::operator=(pick(x)); return *this; }
	ArrayIndex& operator=(ArrayIndex<T> rval_ x)         { B::operator=(pick(x)); return *this; }

	friend void Swap(ArrayIndex& a, ArrayIndex& b)       { a.B::Swap(b); }

	typedef typename B::ConstIterator ConstIterator; // GCC bug (?)
	STL_INDEX_COMPATIBILITY(ArrayIndex<T _cm_ HashFn>)
};
