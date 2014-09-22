template <class K, class T, class V, class Less>
class FixedAMap {
protected:
	Vector<K> key;
	V value;

public:
	T&       Add(const K& k, const T& x)       { key.Add(k); return value.Add(x); }
	T&       AddPick(const K& k, T rval_ x)    { key.Add(k); return value.AddPick(x); }
	T&       Add(const K& k)                   { key.Add(k); return value.Add(); }
	
	void     Finish()                          { IndexSort(key, value, Less()); Shrink(); }

	int      Find(const K& k) const            { return FindBinary(key, k, Less()); }
	int      FindNext(int i) const             { return i + 1 < key.GetCount() && key[i] == key[i + 1] ? i + 1 : -1; }

	T&       Get(const K& k)                   { return value[Find(k)]; }
	const T& Get(const K& k) const             { return value[Find(k)]; }
	const T& Get(const K& k, const T& d) const { int i = Find(k); return i >= 0 ? value[i] : d; }

	T       *FindPtr(const K& k)               { int i = Find(k); return i >= 0 ? &value[i] : NULL; }
	const T *FindPtr(const K& k) const         { int i = Find(k); return i >= 0 ? &value[i] : NULL; }

	const T& operator[](int i) const           { return value[i]; }
	T&       operator[](int i)                 { return value[i]; }
	int      GetCount() const                  { return value.GetCount(); }
	bool     IsEmpty() const                   { return value.IsEmpty(); }
	void     Clear()                           { key.Clear(); value.Clear(); }
	void     Shrink()                          { value.Shrink(); key.Shrink(); }
	void     Reserve(int xtra)                 { value.Reserve(xtra); key.Reserve(xtra); }
	int      GetAlloc() const                  { return value.GetAlloc(); }

	const K& GetKey(int i) const               { return key[i]; }

#ifdef UPP
	void     Serialize(Stream& s);
	void     Xmlize(XmlIO& xio);
	void     Jsonize(JsonIO& jio);
	String   ToString() const;
#endif

	void     Swap(FixedAMap& x)                { UPP::Swap(value, x.value); UPP::Swap(key, x.key); }

	const Vector<K>& GetKeys() const           { return key; }
	Vector<K>        PickKeys()                { return pick(key); }

	const V&         GetValues() const         { return value; }
	V&               GetValues()               { return value; }
	V                PickValues()              { return pick(value); }
	
	bool             IsPicked() const          { return value.IsPicked() || key.IsPicked(); }

	FixedAMap& operator()(const K& k, const T& v)       { Add(k, v); return *this; }

	FixedAMap()                                         {}
	FixedAMap(const FixedAMap& s, int) : key(s.key, 0), value(s.value, 0) {}
	FixedAMap(Vector<K> rval_  key, V rval_ val) : key(key), value(val) {}

	typedef Vector<K> KeyContainer;
	typedef K         KeyType;
	typedef typename Vector<K>::ConstIterator KeyConstIterator;

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
};

template <class K, class T, class Less = StdLess<K> >
class FixedVectorMap : public MoveableAndDeepCopyOption<FixedVectorMap<K, T, Less> >,
                       public FixedAMap< K, T, Vector<T>, Less > {
    typedef FixedAMap< K, T, Vector<T>, Less > B;
public:
	FixedVectorMap(const FixedVectorMap& s, int) : FixedAMap<K, T, Vector<T>, Less>(s, 1) {}
	FixedVectorMap(Vector<K> rval_  key, Vector<T> rval_ val) : FixedAMap<K, T, Vector<T>, Less>(key, val) {}
	FixedVectorMap()                                                       {}

	friend void    Swap(FixedVectorMap& a, FixedVectorMap& b)      { a.B::Swap(b); }

	typedef typename FixedAMap< K, T, Vector<T>, Less >::ConstIterator ConstIterator; // GCC bug (?)
	typedef typename FixedAMap< K, T, Vector<T>, Less >::Iterator      Iterator; // GCC bug (?)
	STL_MAP_COMPATIBILITY(FixedVectorMap<K _cm_ T _cm_ Less>)
};

template <class K, class T, class Less = StdLess<K> >
class FixedArrayMap : public MoveableAndDeepCopyOption< FixedArrayMap<K, T, Less> >,
                      public FixedAMap< K, T, Array<T>, Less > {
	typedef FixedAMap< K, T, Array<T>, Less > B;
public:
	T&        Add(const K& k, const T& x)          { return B::Add(k, x); }
	T&        Add(const K& k)                      { return B::Add(k); }
	T&        Add(const K& k, T *newt)             { B::key.Add(k); return B::value.Add(newt); }
	template <class TT> TT& Create(const K& k)     { TT *q = new TT; B::key.Add(k); return static_cast<TT&>(B::value.Add(q)); }

	FixedArrayMap(const FixedArrayMap& s, int) : FixedAMap<K, T, Array<T>, Less>(s, 1) {}
	FixedArrayMap(Vector<K> rval_ ndx, Array<T> rval_ val) : FixedAMap<K, T, Array<T>, Less>(ndx, val) {}
	FixedArrayMap() {}

	friend void    Swap(FixedArrayMap& a, FixedArrayMap& b)        { a.B::Swap(b); }

	typedef typename FixedAMap< K, T, Array<T>, Less >::ConstIterator ConstIterator; // GCC bug (?)
	typedef typename FixedAMap< K, T, Array<T>, Less >::Iterator      Iterator; // GCC bug (?)
	STL_MAP_COMPATIBILITY(FixedArrayMap<K _cm_ T _cm_ Less>)
};
