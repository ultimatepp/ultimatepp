inline int& HashBase::Maph(unsigned _hash) const
{
	unsigned h = _hash & ~UNSIGNED_HIBIT;
#ifdef FOLDHASH
	return map[mask & (h - SwapEndian32((dword)h))];
//	return map[(mcount - 1) & (((h >> 23) - (h >> 15) - (h >> 7) - h))];
#else
	return map[h % mcount];
#endif
}

inline int& HashBase::Mapi(int i) const
{
	return Maph(hash[i]);
}

inline void HashBase::LinkTo(int i, Link& l, int& m)
{
	if(m >= 0) {
		Link& bl = link[m];
		l.next = m;
		l.prev = bl.prev;
		bl.prev = i;
		link[l.prev].next = i;
	}
	else
		m = l.prev = l.next = i;
}

inline
void HashBase::Add(unsigned _hash)
{
	ASSERT(hash.GetCount() == link.GetCount());
	int i = hash.GetCount();
	hash.Add(_hash & ~UNSIGNED_HIBIT);
	if(hash.GetCount() <= mask)
		LinkTo(i, link.Add(), Maph(_hash));
	else
		Reindex();
}

inline void HashBase::Unlink(int i, Link& l, int& m)
{
	if(i == m) {
		if(l.next == i) {
			m = -1;
			return;
		}
		m = l.next;
	}
	link[l.next].prev = l.prev;
	link[l.prev].next = l.next;
}

inline void HashBase::Unlink(int i, Link& l)
{
	Unlink(i, l, hash[i] & UNSIGNED_HIBIT ? unlinked : Mapi(i));
}

inline int HashBase::Find(unsigned _hash) const
{
	return Maph(_hash);
}

inline int HashBase::FindNext(int i) const
{
	int q = link[i].next;
	return q == Mapi(i) ? -1 : q;
}

inline int HashBase::FindLast(unsigned _hash) const
{
	if(hash.GetCount() == 0) return - 1;
	int i = Find(_hash & ~UNSIGNED_HIBIT);
	return i >= 0 ? link[i].prev : -1;
}

inline int HashBase::FindPrev(int i) const
{
	int q = link[i].prev;
	return q == link[Mapi(i)].prev ? -1 : q;
}

inline void HashBase::Unlink(int i)
{
	ASSERT(!IsUnlinked(i));
	hash[i] |= UNSIGNED_HIBIT;
	if(i < link.GetCount()) {
		Link& lnk = link[i];
		Unlink(i, lnk, Mapi(i));
		LinkTo(i, lnk, unlinked);
	}
}

template <class T>
void Index<T>::Hash() {
	hash.Clear();
	for(int i = 0; i < key.GetCount(); i++)
		hash.Add(hashfn(key[i]));
}

template <class T>
T& Index<T>::Add(const T& x, unsigned _hash) {
	T& t = key.Add(x);
	hash.Add(_hash);
	return t;
}

template <class T>
T& Index<T>::Add(const T& x) {
	return Add(x, hashfn(x));
}

template <class T>
T& Index<T>::Add(T&& x, unsigned _hash)
{
	T& t = key.Add(pick(x));
	hash.Add(_hash);
	return t;
}

template <class T>
T& Index<T>::Add(T&& x)
{
	return Add(pick(x), hashfn(x));
}

template <class T>
int  Index<T>::FindAdd(const T& _key, unsigned _hash) {
	int i = Find(_key, _hash);
	if(i >= 0) return i;
	i = key.GetCount();
	Add(_key, _hash);
	return i;
}

template <class T>
int Index<T>::FindAdd(T&& _key, unsigned _hash)
{
	int i = Find(_key, _hash);
	if(i >= 0) return i;
	i = key.GetCount();
	Add(pick(_key), _hash);
	return i;
}

template <class T>
int Index<T>::FindAdd(T&& key)
{
	return FindAdd(pick(key), hashfn(key));
}

template <class T>
int Index<T>::Put(const T& x, unsigned _hash)
{
	int q = hash.Put(_hash);
	if(q < 0) {
		q = key.GetCount();
		Add(x, _hash);
	}
	else
		key[q] = x;
	return q;
}

template <class T>
int Index<T>::Put(const T& x)
{
	return Put(x, hashfn(x));
}

template <class T>
int Index<T>::Put(T&& x, unsigned _hash)
{
	int q = hash.Put(_hash);
	if(q < 0) {
		q = key.GetCount();
		Add(pick(x), _hash);
	}
	else
		key[q] = pick(x);
	return q;
}

template <class T>
int Index<T>::Put(T&& x)
{
	return Put(pick(x), hashfn(x));
}

template <class T>
int  Index<T>::FindPut(const T& _key, unsigned _hash)
{
	int i = Find(_key, _hash);
	if(i >= 0) return i;
	return Put(_key, _hash);
}

template <class T>
int  Index<T>::FindPut(const T& key)
{
	return FindPut(key, hashfn(key));
}

template <class T>
int Index<T>::FindPut(T&& key, unsigned hash)
{
	int i = Find(key, hash);
	if(i >= 0) return i;
	return Put(pick(key), hash);
}

template <class T>
int Index<T>::FindPut(T&& key)
{
	return FindPut(pick(key), hashfn(key));
}

template <class T>
inline
int Index<T>::Find(const T& x, unsigned hash_) const {
	int i0 = hash.Find(hash_);
	if(i0 >= 0) {
		int i = i0;
		do {
			if(x == key[i]) return i;
			i = hash.GetNext(i);
		}
		while(i0 != i);
	}
	return -1;
}

template <class T>
int Index<T>::Find(const T& x) const {
	return Find(x, hashfn(x));
}

template <class T>
int Index<T>::FindNext(int i) const {
	return Find0(key[i], hash.FindNext(i));
}

template <class T>
inline int Index<T>::FindLast(const T& x, unsigned _hash) const {
	return FindB(x, hash.FindLast(_hash));
}

template <class T>
int Index<T>::FindLast(const T& x) const {
	return FindLast(x, hashfn(x));
}

template <class T>
int Index<T>::FindPrev(int i) const {
	return FindB(key[i], hash.FindPrev(i));
}

template <class T>
int  Index<T>::FindAdd(const T& key) {
	return FindAdd(key, hashfn(key));
}

template <class T>
T&  Index<T>::Set(int i, const T& x, unsigned _hash) {
	T& t = key[i];
	t = x;
	hash.Set(i, _hash);
	return t;
}

template <class T>
T&  Index<T>::Set(int i, const T& x) {
	return Set(i, x, hashfn(x));
}

template <class T>
T& Index<T>::Set(int i, T&& x, unsigned _hash)
{
	T& t = key[i];
	t = pick(x);
	hash.Set(i, _hash);
	return t;
}

template <class T>
T& Index<T>::Set(int i, T&& x)
{
	return Set(i, pick(x), hashfn(x));
}

#ifdef UPP
template <class T>
void Index<T>::Serialize(Stream& s) {
	if(s.IsLoading()) hash.ClearIndex();
	key.Serialize(s);
	hash.Serialize(s);
}

template <class T>
void Index<T>::Xmlize(XmlIO& xio, const char *itemtag)
{
	XmlizeIndex<T, Index<T> >(xio, itemtag, *this);
}

template <class T>
void Index<T>::Jsonize(JsonIO& jio)
{
	JsonizeIndex<Index<T>, T>(jio, *this);
}

template <class T>
String Index<T>::ToString() const
{
	return AsStringArray(*this);
}

#endif

template <class T>
int Index<T>::UnlinkKey(const T& k, unsigned h)
{
	int n = 0;
	int q = hash.Find(h);
	while(q >= 0) {
		int w = q;
		q = hash.FindNext(q);
		if(k == key[w]) {
			hash.Unlink(w);
			n++;
		}
	}
	return n;
}

template <class T>
int Index<T>::UnlinkKey(const T& k)
{
	return UnlinkKey(k, hashfn(k));
}

template <class T>
void Index<T>::Sweep()
{
	Vector<int> b = hash.GetUnlinked();
	Sort(b);
	Remove(b);
}

template <class T>
T& Index<T>::Insert(int i, const T& k, unsigned h) {
	key.Insert(i, k);
	hash.Insert(i, h);
	return key[i];
}

template <class T>
T& Index<T>::Insert(int i, const T& k) {
	key.Insert(i, k);
	hash.Insert(i, hashfn(k));
	return key[i];
}

template <class T>
void Index<T>::Remove(int i)
{
	key.Remove(i);
	hash.Remove(i);
}

template <class T>
void Index<T>::Remove(int i, int count)
{
	key.Remove(i, count);
	hash.Remove(i, count);
}

template <class T>
void Index<T>::Remove(const int *sorted_list, int count)
{
	key.Remove(sorted_list, count);
	hash.Remove(sorted_list, count);
}

template <class T>
void Index<T>::Remove(const Vector<int>& sl)
{
	Remove(sl, sl.GetCount());
}

template <class T>
int Index<T>::RemoveKey(const T& k, unsigned h)
{
	Vector<int> rk;
	int q = Find(k, h);
	while(q >= 0) {
		rk.Add(q);
		q = FindNext(q);
	}
	Remove(rk);
	return rk.GetCount();
}

template <class T>
int Index<T>::RemoveKey(const T& k)
{
	return RemoveKey(k, hashfn(k));
}

// ------------------

/*
template <class T>
T& ArrayIndex<T>::Add(T *newt, unsigned _hash) {
	B::hash.Add(_hash);
	return B::key.Add(newt);
}

template <class T>
T& ArrayIndex<T>::Add(T *newt) {
	return Add(newt, B::hashfn(*newt));
}

template <class T>
T& ArrayIndex<T>::Set(int i, T *newt, unsigned _hash) {
	T& t = B::key.Set(i, newt);
	B::hash.Set(i, _hash);
	return t;
}

template <class T>
T& ArrayIndex<T>::Set(int i, T *newt) {
	return Set(i, newt, B::hashfn(*newt));
}
*/

// --------------------

template <class K, class T, class V>
int AMap<K, T, V>::FindAdd(const K& k) {
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i < 0) {
		i = GetCount();
		key.Add(k, hash);
		value.Add();
	}
	return i;
}

template <class K, class T, class V>
int AMap<K, T, V>::FindAdd(const K& k, const T& x) {
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i < 0) {
		i = GetCount();
		key.Add(k, hash);
		value.Add(x);
	}
	return i;
}

template <class K, class T, class V>
int AMap<K, T, V>::FindAdd(const K& k, T&& x) {
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i < 0) {
		i = GetCount();
		key.Add(k, hash);
		value.Add(pick(x));
	}
	return i;
}

template <class K, class T, class V>
int AMap<K, T, V>::FindAdd(K&& k) {
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i < 0) {
		i = GetCount();
		key.Add(pick(k), hash);
		value.Add();
	}
	return i;
}

template <class K, class T, class V>
int AMap<K, T, V>::FindAdd(K&& k, const T& x) {
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i < 0) {
		i = GetCount();
		key.Add(pick(k), hash);
		value.Add(x);
	}
	return i;
}

template <class K, class T, class V>
int AMap<K, T, V>::FindAdd(K&& k, T&& x) {
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i < 0) {
		i = GetCount();
		key.Add(pick(k), hash);
		value.Add(pick(x));
	}
	return i;
}

template <class K, class T, class V>
int AMap<K, T, V>::Put(const K& k, const T& x)
{
	int i = key.Put(k);
	if(i < value.GetCount())
		value[i] = x;
	else {
		ASSERT(i == value.GetCount());
		value.Add(x);
	}
	return i;
}

template <class K, class T, class V>
int AMap<K, T, V>::PutDefault(const K& k)
{
	int i = key.Put(k);
	if(i >= value.GetCount()) {
		ASSERT(i == value.GetCount());
		value.Add();
	}
	else {
		Destroy(&value[i], &value[i] + 1);
		Construct(&value[i], &value[i] + 1);
	}
	return i;
}

template <class K, class T, class V>
int AMap<K, T, V>::Put(const K& k, T&& x)
{
	int i = key.Put(k);
	if(i < value.GetCount())
		value[i] = pick(x);
	else {
		ASSERT(i == value.GetCount());
		value.Add(pick(x));
	}
	return i;
}

template <class K, class T, class V>
T&  AMap<K, T, V>::Put(const K& k)
{
	int i = key.Put(k);
	if(i < value.GetCount()) {
		Destroy(&value[i], &value[i] + 1);
		Construct(&value[i], &value[i] + 1);
		return value[i];
	}
	else {
		ASSERT(i == value.GetCount());
		return value.Add();
	}
}

template <class K, class T, class V>
int AMap<K, T, V>::Put(K&& k, const T& x)
{
	int i = key.Put(pick(k));
	if(i < value.GetCount())
		value[i] = x;
	else {
		ASSERT(i == value.GetCount());
		value.Add(x);
	}
	return i;
}

template <class K, class T, class V>
int AMap<K, T, V>::PutDefault(K&& k)
{
	int i = key.Put(pick(k));
	if(i >= value.GetCount()) {
		ASSERT(i == value.GetCount());
		value.Add();
	}
	else {
		Destroy(&value[i], &value[i] + 1);
		Construct(&value[i], &value[i] + 1);
	}
	return i;
}

template <class K, class T, class V>
int AMap<K, T, V>::Put(K&& k, T&& x)
{
	int i = key.Put(pick(k));
	if(i < value.GetCount())
		value[i] = pick(x);
	else {
		ASSERT(i == value.GetCount());
		value.Add(pick(x));
	}
	return i;
}

template <class K, class T, class V>
T&  AMap<K, T, V>::Put(K&& k)
{
	int i = key.Put(pick(k));
	if(i < value.GetCount()) {
		Destroy(&value[i], &value[i] + 1);
		Construct(&value[i], &value[i] + 1);
		return value[i];
	}
	else {
		ASSERT(i == value.GetCount());
		return value.Add();
	}
}

template <class K, class T, class V>
int AMap<K, T, V>::FindPut(const K& k)
{
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	return i < 0 ? PutDefault(k) : i;
}

template <class K, class T, class V>
int AMap<K, T, V>::FindPut(const K& k, const T& init)
{
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i < 0) {
		i = key.Put(k, hash);
		if(i >= value.GetCount()) {
			ASSERT(i == value.GetCount());
			i = value.GetCount();
			value.Add(init);
		}
		else
			value[i] = init;
	}
	return i;
}

template <class K, class T, class V>
int AMap<K, T, V>::FindPut(const K& k, T&& init)
{
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i < 0) {
		i = key.Put(k, hash);
		value.At(i) = pick(init);
	}
	return i;
}

template <class K, class T, class V>
int AMap<K, T, V>::FindPut(K&& k)
{
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	return i < 0 ? PutDefault(pick(k)) : i;
}

template <class K, class T, class V>
int AMap<K, T, V>::FindPut(K&& k, const T& init)
{
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i < 0) {
		i = key.Put(pick(k), hash);
		if(i >= value.GetCount()) {
			ASSERT(i == value.GetCount());
			i = value.GetCount();
			value.Add(init);
		}
		else
			value[i] = init;
	}
	return i;
}

template <class K, class T, class V>
int AMap<K, T, V>::FindPut(K&& k, T&& init)
{
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i < 0) {
		i = key.Put(pick(k), hash);
		value.At(i) = pick(init);
	}
	return i;
}

template <class K, class T, class V>
T&  AMap<K, T, V>::GetAdd(const K& k) {
	unsigned hash = key.hashfn(k);
	int i = key.Find(k, hash);
	if(i >= 0)
		return value[i];
	key.Add(k, hash);
	return value.Add();
}

template <class K, class T, class V>
T&  AMap<K, T, V>::GetAdd(const K& k, const T& x) {
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i >= 0) return value[i];
	key.Add(k, hash);
	value.Add(x);
	return value.Top();
}

template <class K, class T, class V>
T&  AMap<K, T, V>::GetAdd(const K& k, T&& x) {
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i >= 0) return value[i];
	key.Add(k, hash);
	value.Add(pick(x));
	return value.Top();
}

template <class K, class T, class V>
T&  AMap<K, T, V>::GetAdd(K&& k) {
	unsigned hash = key.hashfn(k);
	int i = key.Find(k, hash);
	if(i >= 0)
		return value[i];
	key.Add(pick(k), hash);
	return value.Add();
}

template <class K, class T, class V>
T&  AMap<K, T, V>::GetAdd(K&& k, const T& x) {
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i >= 0) return value[i];
	key.Add(pick(k), hash);
	value.Add(x);
	return value.Top();
}

template <class K, class T, class V>
T&  AMap<K, T, V>::GetAdd(K&& k, T&& x) {
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i >= 0) return value[i];
	key.Add(pick(k), hash);
	value.Add(pick(x));
	return value.Top();
}

template <class K, class T, class V>
T&  AMap<K, T, V>::GetPut(const K& k) {
	return value[FindPut(k)];
}

template <class K, class T, class V>
T&  AMap<K, T, V>::GetPut(const K& k, const T& x) {
	return value[FindPut(k, x)];
}

template <class K, class T, class V>
T&  AMap<K, T, V>::GetPut(const K& k, T&& x) {
	return value[FindPut(k, pick(x))];
}

template <class K, class T, class V>
T&  AMap<K, T, V>::GetPut(K&& k) {
	return value[FindPut(pick(k))];
}

template <class K, class T, class V>
T&  AMap<K, T, V>::GetPut(K&& k, const T& x) {
	return value[FindPut(pick(k), x)];
}

template <class K, class T, class V>
T&  AMap<K, T, V>::GetPut(K&& k, T&& x) {
	return value[FindPut(pick(k), pick(x))];
}

#ifdef UPP
template <class K, class T, class V>
void AMap<K, T, V>::Serialize(Stream& s) {
	int version = 0;
	s / version % key % value;
}

template <class K, class T, class V>
void AMap<K, T, V>::Xmlize(XmlIO& xio)
{
	XmlizeMap<K, T, AMap<K, T, V> >(xio, "key", "value", *this);
}

template <class K, class T, class V>
void AMap<K, T, V>::Jsonize(JsonIO& jio)
{
	JsonizeMap<AMap<K, T, V>, K, T>(jio, *this, "key", "value");
}

template <class K, class T, class V>
String AMap<K, T, V>::ToString() const
{
	String r;
	r = "{";
	for(int i = 0; i < GetCount(); i++) {
		if(i)
			r << ", ";
		if(IsUnlinked(i))
			r << "UNLINKED ";
		r << GetKey(i) << ": " << (*this)[i];
	}
	r << '}';
	return r;
}

#endif

template <class K, class T, class V>
int AMap<K, T, V>::RemoveKey(const K& k)
{
	Vector<int> rk;
	int q = Find(k);
	while(q >= 0) {
		rk.Add(q);
		q = FindNext(q);
	}
	Remove(rk);
	return rk.GetCount();
}

template <class K, class T, class V>
void AMap<K, T, V>::Sweep()
{
	Vector<int> b = key.GetUnlinked();
	Sort(b);
	key.Remove(b);
	value.Remove(b);
}

#ifdef UPP
template <class K, class T, class V, class Less>
void FixedAMap<K, T, V, Less>::Serialize(Stream& s) {
	s % key % value;
}

template <class K, class T, class V, class Less>
void FixedAMap<K, T, V, Less>::Xmlize(XmlIO& xio)
{
	XmlizeSortedMap<K, T, FixedAMap<K, T, V, Less> >(xio, "key", "value", *this);
}

template <class K, class T, class V, class Less>
void FixedAMap<K, T, V, Less>::Jsonize(JsonIO& jio)
{
	JsonizeSortedMap<FixedAMap<K, T, V, Less>, K, T>(jio, *this, "key", "value");
}

template <class K, class T, class V, class Less>
String FixedAMap<K, T, V, Less>::ToString() const
{
	String r;
	r = "{";
	for(int i = 0; i < GetCount(); i++) {
		if(i)
			r << ", ";
		r << GetKey(i) << ": " << (*this)[i];
	}
	r << '}';
	return r;
}

#endif
