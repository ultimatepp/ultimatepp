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
	int              mask;
	int              unlinked;

	void  LinkBefore(int i, Link& l, int bi);
	void  LinkTo(int i, Link& l, int& m);
	void  Unlink(int i, Link& l, int& mi);
	void  Unlink(int i, Link& l);
	int&  Maph(unsigned _hash) const;
	int&  Mapi(int i) const;
	void  FinishIndex();
	void  Zero();
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
	int   GetNext(int i) const              { return link[i].next; }
	int   FindNext(int i, int first) const;
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

	HashBase(HashBase&& b);
	void operator=(HashBase&& b);
	HashBase(const HashBase& b, int);
	void operator<<=(const HashBase& b);

	const unsigned *begin() const           { return hash.begin(); }
	const unsigned *end() const             { return hash.end(); }

	void Swap(HashBase& b);
};

template <class T>
class Index : MoveableAndDeepCopyOption<Index<T>> {
protected:
	Vector<T> key;
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
	unsigned hashfn(const T& x) const       { return GetHashValue(x); }

	T&       Add(const T& x, unsigned _hash);
	T&       Add(T&& x, unsigned _hash);
	T&       Add(const T& x);
	T&       Add(T&& x);
	Index&   operator<<(const T& x)         { Add(x); return *this; }
	Index&   operator<<(T&& x)              { Add(pick(x)); return *this; }

	int      FindAdd(const T& key, unsigned _hash);
	int      FindAdd(const T& key);
	int      FindAdd(T&& key, unsigned _hash);
	int      FindAdd(T&& key);

	int      Put(const T& x, unsigned _hash);
	int      Put(const T& x);
	int      Put(T&& x, unsigned _hash);
	int      Put(T&& x);

	int      FindPut(const T& key, unsigned _hash);
	int      FindPut(const T& key);
	int      FindPut(T&& key, unsigned _hash);
	int      FindPut(T&& key);

	int      Find(const T& x, unsigned _hash) const;
	int      Find(const T& x) const;
	int      FindNext(int i) const;
	int      FindLast(const T& x, unsigned _hash) const;
	int      FindLast(const T& x) const;
	int      FindPrev(int i) const;

	T&       Set(int i, const T& x, unsigned _hash);
	T&       Set(int i, const T& x);
	T&       Set(int i, T&& x, unsigned _hash);
	T&       Set(int i, T&& x);

	const T& operator[](int i) const         { return key[i]; }
	int      GetCount() const                { return key.GetCount(); }
	bool     IsEmpty() const                 { return key.IsEmpty(); }
	
	unsigned GetHash(int i) const            { return hash[i]; }

	void     Clear()                         { hash.Clear(); key.Clear(); }

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
	T        Pop()                           { T x = pick(Top()); Drop(); return x; }

	void     Reserve(int n)                  { key.Reserve(n); hash.Reserve(n); }
	void     Shrink()                        { key.Shrink(); hash.Shrink(); }
	int      GetAlloc() const                { return key.GetAlloc(); }

	void     Serialize(Stream& s);
	void     Xmlize(XmlIO& xio, const char *itemtag = "key");
	void     Jsonize(JsonIO& jio);
	String   ToString() const;
	template <class B> bool operator==(const B& b) const { return IsEqualRange(*this, b); }
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
	template <class B> int  Compare(const B& b) const    { return CompareRanges(*this, b); }
	template <class B> bool operator<=(const B& x) const { return Compare(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return Compare(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return Compare(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return Compare(x) > 0; }

	Vector<T>        PickKeys()                          { Vector<T> r = pick(key); Clear(); return r; }
	const Vector<T>& GetKeys() const                     { return key; }

	Index()                                               {}
	Index(Index&& s) : key(pick(s.key)), hash(pick(s.hash)) {}
	Index(const Index& s, int) : key(s.key, 0), hash(s.hash, 0) {}
	explicit Index(Vector<T>&& s) : key(pick(s))          { Hash(); }
	Index(const Vector<T>& s, int) : key(s, 0)            { Hash(); }

	Index& operator=(Vector<T>&& x)                       { key = pick(x); Hash(); return *this; }
	Index& operator=(Index<T>&& x)                        { key = pick(x.key); hash = pick(x.hash); return *this; }

	Index(std::initializer_list<T> init) : key(init)      { Hash(); }

	typedef ConstIteratorOf<Vector<T>> ConstIterator;

// Standard container interface
	ConstIterator begin() const                           { return key.Begin(); }
	ConstIterator end() const                             { return key.End(); }

	friend void Swap(Index& a, Index& b)                  { UPP::Swap(a.hash, b.hash);
	                                                        UPP::Swap(a.key, b.key); }

#ifdef DEPRECATED
	Index& operator<<=(const Vector<T>& s)                { *this = clone(s); return *this; }
	typedef T                ValueType;
	typedef Vector<T>        ValueContainer;
	ConstIterator  GetIter(int pos) const                 { return key.GetIter(pos); }

	void     ClearIndex()                    { hash.ClearIndex(); }
	void     Reindex(int n)                  { hash.Reindex(n); }
	void     Reindex()                       { hash.Reindex(); }

	typedef T             value_type;
	typedef ConstIterator const_iterator;
	typedef const T&      const_reference;
	typedef int           size_type;
	typedef int           difference_type;
	const_iterator        Begin() const          { return begin(); }
	const_iterator        End() const            { return end(); }
	void                  clear()                { Clear(); }
	size_type             size()                 { return GetCount(); }
	bool                  empty() const          { return IsEmpty(); }
#endif
};
