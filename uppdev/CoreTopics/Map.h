template <class K, class T, class V, class HashFn>
class AMap {
protected:
	Index<K, HashFn> key;
	V                value;

public:
	void     Add(const K& k, const T& x)            { key.Add(k); value.Add(x); }
	void     AddPick(const K& k, pick_ T& x)        { key.Add(k); value.AddPick(x); }
	T&       Add(const K& k)                        { key.Add(k); return value.Add(); }

	int      Find(const K& k, unsigned h) const     { return key.Find(k, h); }
	int      Find(const K& k) const                 { return key.Find(k); }
	int      FindNext(int i) const                  { return key.FindNext(i); }
	int      FindLast(const K& k, unsigned h) const { return key.FindLast(k, h); }
	int      FindLast(const K& k) const             { return key.FindLast(k); }
	int      FindPrev(int i) const                  { return key.FindPrev(i); }

	int      FindAdd(const K& k);
	int      FindAdd(const K& k, const T& init);
	int      FindAddPick(const K& k, pick_ T& init);

	int      Put(const K& k, const T& x);
	int      PutPick(const K& k, pick_ T& x);
	T&       Put(const K& k);

	int      FindPut(const K& k);
	int      FindPut(const K& k, const T& init);
	int      FindPutPick(const K& k, pick_ T& init);

	T&       Get(const K& k)                     { return value[Find(k)]; }
	const T& Get(const K& k) const               { return value[Find(k)]; }
	const T& Get(const K& k, const T& d) const   { int i = Find(k); return i >= 0 ? value[i] : d; }

	T&       GetAdd(const K& k);

	T&       GetAdd(const K& k, const T& x);
	T&       GetAddPick(const K& k, pick_ T& x);

	T&       GetPut(const K& k);

	T&       GetPut(const K& k, const T& x);
	T&       GetPutPick(const K& k, pick_ T& x);

	void     SetKey(int i, const K& k)           { key.Set(i, k); }

	T       *FindPtr(const K& k)       { int i = Find(k); return i >= 0 ? &value[i] : NULL; }
	const T *FindPtr(const K& k) const { int i = Find(k); return i >= 0 ? &value[i] : NULL; }

//  PRECHANGE VACATION :)
//	T&       operator()(const K& k)                   { return Get(k); }
//	const T& operator()(const K& k) const             { return Get(k); }
//	const T& operator()(const K& k, const T& d) const { return Get(k, d); }

	void     Unlink(int i)                            { key.Unlink(i); }
	int      UnlinkKey(const K& k, unsigned h)        { return key.UnlinkKey(k, h); }
	int      UnlinkKey(const K& k)                    { return key.UnlinkKey(k); }
	bool     IsUnlinked(int i) const                  { return key.IsUnlinked(i); }
	void     Sweep();

	T&       Insert(int i, const K& k)             { key.Insert(i, k); return value.Insert(i); }
	void     Insert(int i, const K& k, const T& x) { key.Insert(i, k); value.Insert(i, x); }
	void     Remove(int i)                         { key.Remove(i); value.Remove(i); }
	void     Remove(int i, int count)              { key.Remove(i, count); value.Remove(i, count); }
	void     Remove(const int *sl, int n)          { key.Remove(sl, n); value.Remove(sl, n); }
	void     Remove(const Vector<int>& sl)         { Remove(sl, sl.GetCount()); }
	int      RemoveKey(const K& k);

	const T& operator[](int i) const               { return value[i]; }
	T&       operator[](int i)                     { return value[i]; }
	int      GetCount() const                      { return value.GetCount(); }
	bool     IsEmpty() const                       { return value.IsEmpty(); }
	void     Clear()                               { key.Clear(); value.Clear(); }
	void     Shrink()                              { value.Shrink(); key.Shrink(); }
	void     Reserve(int xtra)                     { value.Reserve(xtra); key.Reserve(xtra); }
	int      GetAlloc() const                      { return value.GetAlloc(); }

	void     Drop(int n = 1)                       { key.Drop(n); value.Drop(n); }
	T&       Top()                                 { return value.Top(); }
	const T& Top() const                           { return value.Top(); }
	const K& TopKey() const                        { return key.Top(); }
//	T        Pop()                                 { T h = Top(); Drop(); return h; }
	K        PopKey()                              { K h = TopKey(); Drop(); return h; }
	void     Trim(int n)                           { key.Trim(n); value.SetCount(n); }

	const K& GetKey(int i) const                   { return key[i]; }

#ifdef UPP
	void     Serialize(Stream& s);
#endif

	void     Swap(AMap& x)                         { UPP::Swap(value, x.value);
	                                                 UPP::Swap(key, x.key); }
	const Index<K, HashFn>&  GetIndex() const      { return key; }
	Index<K, HashFn>         PickIndex() pick_     { return key; }

	const Vector<K>& GetKeys() const               { return key.GetKeys(); }
	Vector<K>        PickKeys() pick_              { return key.PickKeys(); }

	const V&         GetValues() const             { return value; }
	V                PickValues() pick_            { return value; }

	AMap()                                         {}
	AMap(const AMap& s, int) : key(s.key, 0), value(s.value, 0) {}
	AMap(pick_ Index<K, HashFn>& ndx, pick_ V& val) : value(val), key(ndx) {}
	AMap(pick_ Vector<K>& ndx, pick_ V& val) : value(val), key(ndx) {}

	typedef K        KeyType;
	typedef typename Index<K, HashFn>::ConstIterator KeyConstIterator;

	KeyConstIterator KeyBegin() const                             { return key.Begin(); }
	KeyConstIterator KeyEnd() const                               { return key.End(); }
	KeyConstIterator KeyGetIter(int pos) const                    { return key.GetIter(pos); }

	typedef T                          ValueType;
	typedef typename V::ConstIterator  ConstIterator;
	typedef typename V::Iterator       Iterator;

	Iterator         Begin()                                      { return value.Begin(); }
	Iterator         End()                                        { return value.End(); }
	Iterator         GetIter(int pos)                             { return value.GetIter(pos); }
	ConstIterator    Begin() const                                { return value.Begin(); }
	ConstIterator    End() const                                  { return value.End(); }
	ConstIterator    GetIter(int pos) const                       { return value.GetIter(pos); }

	friend int     GetCount(const AMap& v)                        { return v.GetCount(); }
};

template <class K, class T, class HashFn = StdHash<K> >
class VectorMap : public MoveableAndDeepCopyOption<VectorMap<K, T, HashFn> >,
                  public AMap< K, T, Vector<T>, HashFn > {
    typedef AMap< K, T, Vector<T>, HashFn > B;
public:
	T        Pop()                            { T h = B::Top(); B::Drop(); return h; }

	VectorMap(const VectorMap& s, int) : AMap<K, T, Vector<T>, HashFn>(s, 1) {}
	VectorMap(pick_ Index<K, HashFn>& ndx, pick_ Vector<T>& val) : AMap<K, T, Vector<T>, HashFn>(ndx, val) {}
	VectorMap(pick_ Vector<K>& ndx, pick_ Vector<T>& val) : AMap<K, T, Vector<T>, HashFn>(ndx, val) {}
	VectorMap()                                                       {}

	friend void    Swap(VectorMap& a, VectorMap& b)      { a.B::Swap(b); }

	typedef typename AMap< K, T, Vector<T>, HashFn >::ConstIterator ConstIterator; // GCC bug (?)
	typedef typename AMap< K, T, Vector<T>, HashFn >::Iterator      Iterator; // GCC bug (?)
	STL_MAP_COMPATIBILITY(VectorMap<K _cm_ T _cm_ HashFn>)
};

template <class K, class T, class HashFn = StdHash<K> >
class ArrayMap : public MoveableAndDeepCopyOption< ArrayMap<K, T, HashFn> >,
                 public AMap< K, T, Array<T>, HashFn > {
	typedef AMap< K, T, Array<T>, HashFn > B;
public:
	void      Add(const K& k, const T& x)          { B::Add(k, x); }
	T&        Add(const K& k)                      { return B::Add(k); }
	T&        Add(const K& k, T *newt)             { B::key.Add(k); B::value.Add(newt); return *newt; }
	template <class TT> TT& Create(const K& k)     { TT *q = new TT; B::key.Add(k); B::value.Add(q); return *q; }

	void      Set(int i, T *ptr)                   { B::value.Set(i, ptr); }
	T        *PopDetach()                          { B::key.Drop(); return B::value.PopDetach(); }

	ArrayMap(const ArrayMap& s, int) : AMap<K, T, Array<T>, HashFn>(s, 1) {}
	ArrayMap(pick_ Index<K, HashFn>& ndx, pick_ Array<T>& val) : AMap<K, T, Array<T>, HashFn>(ndx, val) {}
	ArrayMap(pick_ Vector<K>& ndx, pick_ Array<T>& val) : AMap<K, T, Array<T>, HashFn>(ndx, val) {}
	ArrayMap() {}

	friend void    Swap(ArrayMap& a, ArrayMap& b)        { a.B::Swap(b); }

	typedef typename AMap< K, T, Array<T>, HashFn >::ConstIterator ConstIterator; // GCC bug (?)
	typedef typename AMap< K, T, Array<T>, HashFn >::Iterator      Iterator; // GCC bug (?)
	STL_MAP_COMPATIBILITY(ArrayMap<K _cm_ T _cm_ HashFn>)
};

template <class K, class T, int NBLK = 16, class HashFn = StdHash<K> >
class SegtorMap : public MoveableAndDeepCopyOption< SegtorMap<K, T, NBLK, HashFn > >,
                  public AMap< K, T, Segtor<T, NBLK>, HashFn > {
	typedef AMap< K, T, Segtor<T, NBLK>, HashFn > B;
public:
	SegtorMap(const SegtorMap& s, int) : AMap<K, T, Segtor<T, NBLK>, HashFn>(s, 1) {}
	SegtorMap(pick_ Index<K, HashFn>& ndx, pick_ Segtor<T>& val) : AMap<K, T, Segtor<T, NBLK>, HashFn>(ndx, val) {}
	SegtorMap(pick_ Vector<K>& ndx, pick_ Segtor<T>& val) : AMap<K, T, Segtor<T, NBLK>, HashFn>(ndx, val) {}
	SegtorMap()                                              {}

	friend void Swap(SegtorMap& a, SegtorMap& b)             { a.B::Swap(b); }

	typedef typename B::ConstIterator ConstIterator; // GCC bug (?)
	typedef typename B::Iterator      Iterator; // GCC bug (?)
	STL_MAP_COMPATIBILITY(SegtorMap<K _cm_ T _cm_ NBLK _cm_ HashFn>)
};
