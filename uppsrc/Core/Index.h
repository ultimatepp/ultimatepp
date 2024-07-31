struct IndexCommon {
	enum { HIBIT = 0x80000000 };

	struct Hash : Moveable<Hash> {
		int   next; // also link for unlinked items...
		dword hash;
		int   prev;
	};
	
	int         *map;
	Hash        *hash;
	dword        mask;
	int          unlinked;

	
	static int empty[1];

	static dword Smear(hash_t h)          { return FoldHash(h) | HIBIT; }

	void Link(int& m, Hash& hh, int ii);
	void Link(int ii, dword sh);
	void Del(int& m, Hash& hh, int ii);
	void Ins(int& m, Hash& hh, int ii);

	void MakeMap(int count);
	void Remap(int count);
	void Reindex(int count);
	void GrowMap(int count);
	void FreeMap();
	void Free();

	void Set(int ii, dword h);
	
	Vector<int> GetUnlinked() const;
	
	void Clear();
	void Trim(int n, int count);
	void Sweep(int n);
	void Reserve(int n);
	void Shrink();
	void AdjustMap(int count, int alloc);
	
	void Copy(const IndexCommon& b, int count);
	void Pick(IndexCommon& b);
	void Swap(IndexCommon& b);
	
	IndexCommon();
	~IndexCommon();
};

template <class K, class T, class V> class AMap;

template <class T>
class Index : MoveableAndDeepCopyOption<Index<T>>, IndexCommon {
	Vector<T> key;

	static dword Smear(const T& k)   { return IndexCommon::Smear(GetHashValue(k)); }

	int  FindFrom(int i, dword sh, const T& k, int end) const;
	int  FindBack(int i, dword sh, const T& k, int end) const;


	void ReallocHash(int n);
	template <typename U> void GrowAdd(U&& k, dword sh);
	template <typename U> void AddS(int& m, U&& k, dword sh);
	template <typename U> void AddS(U&& k, dword sh);

	template <class OP, class U> int FindAdd(U&& k, OP add_op);
	template <class U> int FindPut0(U&& k, bool& put);

	template <typename U> int Put0(U&& k, dword sh);
	template <typename U> void Set0(int i, U&& k);

	template <typename, typename, typename> friend class AMap;
	
	void        FixHash(bool makemap = true);

public:
	void        Add(const T& k)             { AddS(k, Smear(k)); }
	void        Add(T&& k)                  { AddS(pick(k), Smear(k)); }
	Index&      operator<<(const T& x)      { Add(x); return *this; }
	Index&      operator<<(T&& x)           { Add(pick(x)); return *this; }

	int         Find(const T& k) const;
	int         FindNext(int i) const;
	int         FindLast(const T& k) const;
	int         FindPrev(int i) const;
	
	int         FindAdd(const T& k)         { return FindAdd(k, []{}); }
	int         FindAdd(T&& k)              { return FindAdd(pick(k), []{}); }

	int         Put(const T& k)             { return Put0(k, Smear(k)); }
	int         Put(T&& k)                  { return Put0(pick(k), Smear(k)); }
	int         FindPut(const T& k, bool& p){ return FindPut0(k, p); }
	int         FindPut(T&& k, bool& p)     { return FindPut0(pick(k), p); }
	int         FindPut(const T& k)         { bool p; return FindPut0(k, p); }
	int         FindPut(T&& k)              { bool p; return FindPut0(pick(k), p); }

	void        Unlink(int i);
	int         UnlinkKey(const T& k);
	bool        IsUnlinked(int i) const      { return hash[i].hash == 0; }
	bool        HasUnlinked() const          { return unlinked >= 0; }
	Vector<int> GetUnlinked() const          { return IndexCommon::GetUnlinked(); }

	void        Sweep();

	void        Set(int i, const T& k)       { Set0(i, k); }
	void        Set(int i, T&& k)            { Set0(i, pick(k)); }

	const T&    operator[](int i) const      { return key[i]; }
	int         GetCount() const             { return key.GetCount(); }
	bool        IsEmpty() const              { return key.IsEmpty(); }
	
	void        Clear()                      { key.Clear(); IndexCommon::Clear(); }

	void        Trim(int n = 0)              { IndexCommon::Trim(n, GetCount()); key.Trim(n); }
	void        Drop(int n = 1)              { Trim(GetCount() - 1); }
	const T&    Top() const                  { return key.Top(); }
	T           Pop()                        { T x = pick(Top()); Drop(); return x; }

	void        Reserve(int n);
	void        Shrink();
	int         GetAlloc() const             { return key.GetAlloc(); }

	Vector<T>        PickKeys()              { Vector<T> r = pick(key); Clear(); return r; }
	const Vector<T>& GetKeys() const         { return key; }

	void     Remove(const int *sorted_list, int count);
	void     Remove(const Vector<int>& sorted_list)         { Remove(sorted_list, sorted_list.GetCount()); }
	template <typename Pred> void RemoveIf(Pred p)          { Remove(FindAlli(key, p)); }
	
	Index()                                                 {}
	Index(Index&& s) : key(pick(s.key))                     { IndexCommon::Pick(s); }
	Index(const Index& s, int) : key(s.key, 0)              { ReallocHash(0); IndexCommon::Copy(s, key.GetCount()); } // TODO: Unlinked!
	explicit Index(Vector<T>&& s) : key(pick(s))            { FixHash(); }
	Index(const Vector<T>& s, int) : key(s, 0)              { FixHash(); }

	Index& operator=(Vector<T>&& x)                         { key = pick(x); FixHash(); return *this; }
	Index& operator=(Index<T>&& x)                          { key = pick(x.key); IndexCommon::Pick(x); return *this; }

	Index(std::initializer_list<T> init) : key(init)        { FixHash(); }

	void     Serialize(Stream& s);
	void     Xmlize(XmlIO& xio, const char *itemtag = "key");
	void     Jsonize(JsonIO& jio);
	String   ToString() const;
	template <class B> bool operator==(const B& b) const { return IsEqualRange(*this, b); }
#ifndef CPP_20
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
#endif
	template <class B> int  Compare(const B& b) const    { return CompareRanges(*this, b); }
	template <class B> bool operator<=(const B& x) const { return Compare(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return Compare(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return Compare(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return Compare(x) > 0; }

// Standard container interface
	typedef ConstIteratorOf<Vector<T>> ConstIterator;
	ConstIterator begin() const                             { return key.begin(); }
	ConstIterator end() const                               { return key.end(); }

	friend void Swap(Index& a, Index& b)                    { a.IndexCommon::Swap(b); UPP::Swap(a.key, b.key); }

// deprecated:
#ifdef DEPRECATED
	T&       Insert(int i, const T& k)                      { key.Insert(i, k); FixHash(); return key[i]; }
	void     Remove(int i, int count)                       { key.Remove(i, count); FixHash(); }
	void     Remove(int i)                                  { Remove(i, 1); }
	int      RemoveKey(const T& k)                          { int i = Find(k); if(i >= 0) Remove(i); return i; }

	unsigned GetHash(int i) const                           { return hash[i].hash; }

	Index& operator<<=(const Vector<T>& s)                  { *this = clone(s); return *this; }
	typedef T                ValueType;
	typedef Vector<T>        ValueContainer;
	ConstIterator  GetIter(int pos) const                   { return key.GetIter(pos); }

	void     ClearIndex()                    {}
	void     Reindex(int n)                  {}
	void     Reindex()                       {}

	typedef T             value_type;
	typedef ConstIterator const_iterator;
	typedef const T&      const_reference;
	typedef int           size_type;
	typedef int           difference_type;
	const_iterator        Begin() const          { return begin(); }
	const_iterator        End() const            { return end(); }
	void                  clear()                { Clear(); }
	size_type             size() const           { return GetCount(); }
	bool                  empty() const          { return IsEmpty(); }
#endif

#ifdef _DEBUG
	String Dump() const;
#endif
};
