template <class K, class T, class V, class HashFn>
class AMap {
protected:
	Index<K, HashFn> key;
	V                value;

public:
	T&       Add(const K& k, const T& x)            { key.Add(k); return value.Add(x); }
	T&       AddPick(const K& k, T rval_ x)         { key.Add(k); return value.AddPick(pick(x)); }
	T&       Add(const K& k)                        { key.Add(k); return value.Add(); }

	int      Find(const K& k, unsigned h) const     { return key.Find(k, h); }
	int      Find(const K& k) const                 { return key.Find(k); }
	int      FindNext(int i) const                  { return key.FindNext(i); }
	int      FindLast(const K& k, unsigned h) const { return key.FindLast(k, h); }
	int      FindLast(const K& k) const             { return key.FindLast(k); }
	int      FindPrev(int i) const                  { return key.FindPrev(i); }

	int      FindAdd(const K& k);
	int      FindAdd(const K& k, const T& init);
	int      FindAddPick(const K& k, T rval_ init);

	int      Put(const K& k, const T& x);
	int      PutDefault(const K& k);
	int      PutPick(const K& k, T rval_ x);
	T&       Put(const K& k);

	int      FindPut(const K& k);
	int      FindPut(const K& k, const T& init);
	int      FindPutPick(const K& k, T rval_ init);

	T&       Get(const K& k)                     { return value[Find(k)]; }
	const T& Get(const K& k) const               { return value[Find(k)]; }
	const T& Get(const K& k, const T& d) const   { int i = Find(k); return i >= 0 ? value[i] : d; }

	T&       GetAdd(const K& k);

	T&       GetAdd(const K& k, const T& x);
	T&       GetAddPick(const K& k, T rval_ x);

	T&       GetPut(const K& k);

	T&       GetPut(const K& k, const T& x);
	T&       GetPutPick(const K& k, T rval_ x);

	void     SetKey(int i, const K& k)           { key.Set(i, k); }

	T       *FindPtr(const K& k)       { int i = Find(k); return i >= 0 ? &value[i] : NULL; }
	const T *FindPtr(const K& k) const { int i = Find(k); return i >= 0 ? &value[i] : NULL; }

	T       *FindLastPtr(const K& k)       { int i = FindLast(k); return i >= 0 ? &value[i] : NULL; }
	const T *FindLastPtr(const K& k) const { int i = FindLast(k); return i >= 0 ? &value[i] : NULL; }

	void     Unlink(int i)                            { key.Unlink(i); }
	int      UnlinkKey(const K& k, unsigned h)        { return key.UnlinkKey(k, h); }
	int      UnlinkKey(const K& k)                    { return key.UnlinkKey(k); }
	bool     IsUnlinked(int i) const                  { return key.IsUnlinked(i); }
	void     Sweep();
	bool     HasUnlinked() const                      { return key.HasUnlinked(); }

	T&       Insert(int i, const K& k)             { key.Insert(i, k); return value.Insert(i); }
	T&       Insert(int i, const K& k, const T& x) { key.Insert(i, k); return value.Insert(i, x); }
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

	unsigned GetHash(int i) const                  { return key.GetHash(i); }

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
	void     Xmlize(XmlIO& xio);
	void     Jsonize(JsonIO& jio);
	String   ToString() const;
	bool     operator==(const AMap& b) const       { ASSERT(!HasUnlinked()); return IsEqualMap(*this, b); }
	bool     operator!=(const AMap& b) const       { return !operator==(b); }
	int      Compare(const AMap& b) const          { ASSERT(!HasUnlinked()); return CompareMap(*this, b); }
	bool     operator<=(const AMap& x) const       { return Compare(x) <= 0; }
	bool     operator>=(const AMap& x) const       { return Compare(x) >= 0; }
	bool     operator<(const AMap& x) const        { return Compare(x) < 0; }
	bool     operator>(const AMap& x) const        { return Compare(x) > 0; }
#endif

	void     Swap(AMap& x)                         { UPP::Swap(value, x.value);
	                                                 UPP::Swap(key, x.key); }
	const Index<K, HashFn>&  GetIndex() const      { return key; }
	Index<K, HashFn>         PickIndex()           { return pick(key); }

	const Vector<K>& GetKeys() const               { return key.GetKeys(); }
	Vector<K>        PickKeys() pick_              { return key.PickKeys(); }

	const V&         GetValues() const             { return value; }
	V&               GetValues()                   { return value; }
	V                PickValues() pick_            { return pick(value); }
	
	bool             IsPicked() const              { return value.IsPicked() || key.IsPicked(); }

	AMap& operator()(const K& k, const T& v)       { Add(k, v); return *this; }

	AMap()                                         {}
	AMap(const AMap& s, int) : key(s.key, 0), value(s.value, 0) {}
	AMap(Index<K, HashFn> rval_ ndx, V rval_ val) : key(pick(ndx)), value(pick(val)) {}
	AMap(Vector<K> rval_ ndx, V rval_ val) : key(pick(ndx)), value(pick(val)) {}

	typedef Vector<K> KeyContainer;
	typedef K         KeyType;
	typedef typename Index<K, HashFn>::ConstIterator KeyConstIterator;

	KeyConstIterator KeyBegin() const                             { return key.Begin(); }
	KeyConstIterator KeyEnd() const                               { return key.End(); }
	KeyConstIterator KeyGetIter(int pos) const                    { return key.GetIter(pos); }

	typedef V                          ValueContainer;
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
	VectorMap(Index<K, HashFn> rval_  ndx, Vector<T> rval_ val) : AMap<K, T, Vector<T>, HashFn>(pick(ndx), pick(val)) {}
	VectorMap(Vector<K> rval_ ndx, Vector<T> rval_ val) : AMap<K, T, Vector<T>, HashFn>(pick(ndx), pick(val)) {}
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
	T&        Add(const K& k, const T& x)          { return B::Add(k, x); }
	T&        Add(const K& k)                      { return B::Add(k); }
	T&        Add(const K& k, T *newt)             { B::key.Add(k); return B::value.Add(newt); }
	T&        Insert(int i, const K& k, T *newt)   { B::key.Insert(i, k); return B::value.Insert(i, newt); }
	using B::Insert;
	template <class TT> TT& Create(const K& k)     { TT *q = new TT; B::key.Add(k); return static_cast<TT&>(B::value.Add(q)); }

	T&        Set(int i, T *ptr)                   { return B::value.Set(i, ptr); }
	T        *PopDetach()                          { B::key.Drop(); return B::value.PopDetach(); }
	T        *Detach(int i)                        { B::key.Remove(i); return B::value.Detach(i); }
	T        *Swap(int i, T *newt)                 { return B::value.Swap(i, newt); }

	ArrayMap(const ArrayMap& s, int) : AMap<K, T, Array<T>, HashFn>(s, 1) {}
	ArrayMap(Index<K, HashFn> rval_ ndx, Array<T> rval_ val) : AMap<K, T, Array<T>, HashFn>(pick(ndx), pick(val)) {}
	ArrayMap(Vector<K> rval_ ndx, Array<T> rval_  val) : AMap<K, T, Array<T>, HashFn>(pick(ndx), pick(val)) {}
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
	SegtorMap(Index<K, HashFn> rval_ ndx, Segtor<T> rval_ val) : AMap<K, T, Segtor<T, NBLK>, HashFn>(pick(ndx), pick(val)) {}
	SegtorMap(Vector<K> rval_ ndx, Segtor<T> rval_ val) : AMap<K, T, Segtor<T, NBLK>, HashFn>(pick(ndx), pick(val)) {}
	SegtorMap()                                              {}

	friend void Swap(SegtorMap& a, SegtorMap& b)             { a.B::Swap(b); }

	typedef typename B::ConstIterator ConstIterator; // GCC bug (?)
	typedef typename B::Iterator      Iterator; // GCC bug (?)
	STL_MAP_COMPATIBILITY(SegtorMap<K _cm_ T _cm_ NBLK _cm_ HashFn>)
};
