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

inline int& HashBase::Mapi(int i) const
{
	return Maph(hash[i]);
}

inline int& HashBase::Maph(unsigned _hash) const
{
	unsigned h = _hash & ~UNSIGNED_HIBIT;
#ifdef FOLDHASH
	return map[(mcount - 1) & (((h >> 23) - (h >> 15) - (h >> 7) - h))];
#else
	return map[h % mcount];
#endif
}

inline void HashBase::DoIndex()
{
	if(hash.GetCount() < mcount)
		FinishIndex();
	else
		Reindex();
}

inline int HashBase::Find(unsigned _hash) const
{
	if(hash.GetCount() == 0) return -1;
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

template <class T, class V, class HashFn>
AIndex<T, V, HashFn>::AIndex(const AIndex& s, int)
:	key(s.key, 0),
	hash(s.hash, 0) {}

template <class T, class V, class HashFn>
void AIndex<T, V, HashFn>::Hash() {
	for(int i = 0; i < key.GetCount(); i++)
		hash.Add(hashfn(key[i]));
}

template <class T, class V, class HashFn>
AIndex<T, V, HashFn>& AIndex<T, V, HashFn>::operator=(V rval_ s) {
	key = pick(s);
	hash.Clear();
	Hash();
	return *this;
}

template <class T, class V, class HashFn>
AIndex<T, V, HashFn>& AIndex<T, V, HashFn>::operator<<=(const V& s) {
	key <<= s;
	hash.Clear();
	Hash();
	return *this;
}

template <class T, class V, class HashFn>
AIndex<T, V, HashFn>::AIndex(V rval_ s) : key(pick(s)) {
	Hash();
}

template <class T, class V, class HashFn>
AIndex<T, V, HashFn>::AIndex(const V& s, int) : key(s, 1) {
	Hash();
}

template <class T, class V, class HashFn>
T& AIndex<T, V, HashFn>::Add(const T& x, unsigned _hash) {
	T& t = key.Add(x);
	hash.Add(_hash);
	return t;
}

template <class T, class V, class HashFn>
T& AIndex<T, V, HashFn>::Add(const T& x) {
	return Add(x, hashfn(x));
}

template <class T, class V, class HashFn>
int  AIndex<T, V, HashFn>::FindAdd(const T& _key, unsigned _hash) {
	int i = Find(_key, _hash);
	if(i >= 0) return i;
	i = key.GetCount();
	Add(_key, _hash);
	return i;
}

template <class T, class V, class HashFn>
int AIndex<T, V, HashFn>::Put(const T& x, unsigned _hash)
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

template <class T, class V, class HashFn>
int AIndex<T, V, HashFn>::Put(const T& x)
{
	return Put(x, hashfn(x));
}

template <class T, class V, class HashFn>
int  AIndex<T, V, HashFn>::FindPut(const T& _key, unsigned _hash)
{
	int i = Find(_key, _hash);
	if(i >= 0) return i;
	return Put(_key, _hash);
}

template <class T, class V, class HashFn>
int  AIndex<T, V, HashFn>::FindPut(const T& key)
{
	return FindPut(key, hashfn(key));
}

template <class T, class V, class HashFn>
inline int AIndex<T, V, HashFn>::Find(const T& x, unsigned _hash) const {
	return Find0(x, hash.Find(_hash));
}

template <class T, class V, class HashFn>
int AIndex<T, V, HashFn>::Find(const T& x) const {
	return Find(x, hashfn(x));
}

template <class T, class V, class HashFn>
int AIndex<T, V, HashFn>::FindNext(int i) const {
	return Find0(key[i], hash.FindNext(i));
}

template <class T, class V, class HashFn>
inline int AIndex<T, V, HashFn>::FindLast(const T& x, unsigned _hash) const {
	return FindB(x, hash.FindLast(_hash));
}

template <class T, class V, class HashFn>
int AIndex<T, V, HashFn>::FindLast(const T& x) const {
	return FindLast(x, hashfn(x));
}

template <class T, class V, class HashFn>
int AIndex<T, V, HashFn>::FindPrev(int i) const {
	return FindB(key[i], hash.FindPrev(i));
}

template <class T, class V, class HashFn>
int  AIndex<T, V, HashFn>::FindAdd(const T& key) {
	return FindAdd(key, hashfn(key));
}

template <class T, class V, class HashFn>
T&  AIndex<T, V, HashFn>::Set(int i, const T& x, unsigned _hash) {
	T& t = key[i];
	t = x;
	hash.Set(i, _hash);
	return t;
}

template <class T, class V, class HashFn>
T&  AIndex<T, V, HashFn>::Set(int i, const T& x) {
	return Set(i, x, hashfn(x));
}

#ifdef UPP
template <class T, class V, class HashFn>
void AIndex<T, V, HashFn>::Serialize(Stream& s) {
	if(s.IsLoading()) ClearIndex();
	key.Serialize(s);
	hash.Serialize(s);
}

template <class T, class V, class HashFn>
void AIndex<T, V, HashFn>::Xmlize(XmlIO& xio, const char *itemtag)
{
	XmlizeIndex<T, AIndex<T, V, HashFn> >(xio, itemtag, *this);
}

template <class T, class V, class HashFn>
void AIndex<T, V, HashFn>::Jsonize(JsonIO& jio)
{
	JsonizeIndex<AIndex<T, V, HashFn>, T>(jio, *this);
}

template <class T, class V, class HashFn>
String AIndex<T, V, HashFn>::ToString() const
{
	return AsStringArray(*this);
}

#endif

template <class T, class V, class HashFn>
int AIndex<T, V, HashFn>::UnlinkKey(const T& k, unsigned h)
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

template <class T, class V, class HashFn>
int AIndex<T, V, HashFn>::UnlinkKey(const T& k)
{
	return UnlinkKey(k, hashfn(k));
}

template <class T, class V, class HashFn>
void AIndex<T, V, HashFn>::Sweep()
{
	Vector<int> b = hash.GetUnlinked();
	Sort(b);
	Remove(b);
}

template <class T, class V, class HashFn>
T& AIndex<T, V, HashFn>::Insert(int i, const T& k, unsigned h) {
	key.Insert(i, k);
	hash.Insert(i, h);
	return key[i];
}

template <class T, class V, class HashFn>
T& AIndex<T, V, HashFn>::Insert(int i, const T& k) {
	key.Insert(i, k);
	hash.Insert(i, hashfn(k));
	return key[i];
}

template <class T, class V, class HashFn>
void AIndex<T, V, HashFn>::Remove(int i)
{
	key.Remove(i);
	hash.Remove(i);
}

template <class T, class V, class HashFn>
void AIndex<T, V, HashFn>::Remove(int i, int count)
{
	key.Remove(i, count);
	hash.Remove(i, count);
}

template <class T, class V, class HashFn>
void AIndex<T, V, HashFn>::Remove(const int *sorted_list, int count)
{
	key.Remove(sorted_list, count);
	hash.Remove(sorted_list, count);
}

template <class T, class V, class HashFn>
void AIndex<T, V, HashFn>::Remove(const Vector<int>& sl)
{
	Remove(sl, sl.GetCount());
}

template <class T, class V, class HashFn>
int AIndex<T, V, HashFn>::RemoveKey(const T& k, unsigned h)
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

template <class T, class V, class HashFn>
int AIndex<T, V, HashFn>::RemoveKey(const T& k)
{
	return RemoveKey(k, hashfn(k));
}

// ------------------

template <class T, class HashFn>
T& ArrayIndex<T, HashFn>::Add(T *newt, unsigned _hash) {
	B::hash.Add(_hash);
	return B::key.Add(newt);
}

template <class T, class HashFn>
T& ArrayIndex<T, HashFn>::Add(T *newt) {
	return Add(newt, B::hashfn(*newt));
}

template <class T, class HashFn>
T& ArrayIndex<T, HashFn>::Set(int i, T *newt, unsigned _hash) {
	T& t = B::key.Set(i, newt);
	B::hash.Set(i, _hash);
	return t;
}

template <class T, class HashFn>
T& ArrayIndex<T, HashFn>::Set(int i, T *newt) {
	return Set(i, newt, B::hashfn(*newt));
}

// --------------------

template <class K, class T, class V, class HashFn>
int AMap<K, T, V, HashFn>::FindAdd(const K& k) {
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i < 0) {
		i = GetCount();
		key.Add(k, hash);
		value.Add();
	}
	return i;
}

template <class K, class T, class V, class HashFn>
int AMap<K, T, V, HashFn>::FindAdd(const K& k, const T& x) {
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i < 0) {
		i = GetCount();
		key.Add(k, hash);
		value.Add(x);
	}
	return i;
}

template <class K, class T, class V, class HashFn>
int AMap<K, T, V, HashFn>::FindAddPick(const K& k, T rval_ x) {
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i < 0) {
		i = GetCount();
		key.Add(k, hash);
		value.Add(x);
	}
	return i;
}

template <class K, class T, class V, class HashFn>
int AMap<K, T, V, HashFn>::Put(const K& k, const T& x)
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

template <class K, class T, class V, class HashFn>
int AMap<K, T, V, HashFn>::PutDefault(const K& k)
{
	int i = key.Put(k);
	if(i >= value.GetCount()) {
		ASSERT(i == value.GetCount());
		value.Add();
	}
	else {
		DestroyArray(&value[i], &value[i] + 1);
		ConstructArray(&value[i], &value[i] + 1);
	}
	return i;
}

template <class K, class T, class V, class HashFn>
int AMap<K, T, V, HashFn>::PutPick(const K& k, T rval_ x)
{
	int i = key.Put(k);
	if(i < value.GetCount())
		value[i] = x;
	else {
		ASSERT(i == value.GetCount());
		value.AddPick(x);
	}
	return i;
}

template <class K, class T, class V, class HashFn>
T&  AMap<K, T, V, HashFn>::Put(const K& k)
{
	int i = key.Put(k);
	if(i < value.GetCount()) {
		DestroyArray(&value[i], &value[i] + 1);
		ConstructArray(&value[i], &value[i] + 1);
		return value[i];
	}
	else {
		ASSERT(i == value.GetCount());
		return value.Add();
	}
}

template <class K, class T, class V, class HashFn>
int AMap<K, T, V, HashFn>::FindPut(const K& k)
{
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	return i < 0 ? PutDefault(k) : i;
}

template <class K, class T, class V, class HashFn>
int AMap<K, T, V, HashFn>::FindPut(const K& k, const T& init)
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

template <class K, class T, class V, class HashFn>
int AMap<K, T, V, HashFn>::FindPutPick(const K& k, T rval_ init)
{
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i < 0) {
		i = key.Put(k, hash);
		value.At(i) = init;
	}
	return i;
}

template <class K, class T, class V, class HashFn>
T&  AMap<K, T, V, HashFn>::GetAdd(const K& k) {
	unsigned hash = key.hashfn(k);
	int i = key.Find(k, hash);
	if(i >= 0)
		return value[i];
	key.Add(k, hash);
	return value.Add();
}

template <class K, class T, class V, class HashFn>
T&  AMap<K, T, V, HashFn>::GetAdd(const K& k, const T& x) {
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i >= 0) return value[i];
	key.Add(k, hash);
	value.Add(x);
	return value.Top();
}

template <class K, class T, class V, class HashFn>
T&  AMap<K, T, V, HashFn>::GetAddPick(const K& k, T rval_ x) {
	unsigned hash = key.hashfn(k);
	int i = Find(k, hash);
	if(i >= 0) return value[i];
	key.Add(k, hash);
	value.AddPick(x);
	return value.Top();
}

template <class K, class T, class V, class HashFn>
T&  AMap<K, T, V, HashFn>::GetPut(const K& k) {
	return value[FindPut(k)];
}

template <class K, class T, class V, class HashFn>
T&  AMap<K, T, V, HashFn>::GetPut(const K& k, const T& x) {
	return value[FindPut(k, x)];
}

template <class K, class T, class V, class HashFn>
T&  AMap<K, T, V, HashFn>::GetPutPick(const K& k, T rval_ x) {
	return value[FindPutPick(k, x)];
}

#ifdef UPP
template <class K, class T, class V, class HashFn>
void AMap<K, T, V, HashFn>::Serialize(Stream& s) {
	int version = 0;
	s / version % key % value;
}

template <class K, class T, class V, class HashFn>
void AMap<K, T, V, HashFn>::Xmlize(XmlIO& xio)
{
	XmlizeMap<K, T, AMap<K, T, V, HashFn> >(xio, "key", "value", *this);
}

template <class K, class T, class V, class HashFn>
void AMap<K, T, V, HashFn>::Jsonize(JsonIO& jio)
{
	JsonizeMap<AMap<K, T, V, HashFn>, K, T>(jio, *this, "key", "value");
}

template <class K, class T, class V, class HashFn>
String AMap<K, T, V, HashFn>::ToString() const
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

template <class K, class T, class V, class HashFn>
int AMap<K, T, V, HashFn>::RemoveKey(const K& k)
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

template <class K, class T, class V, class HashFn>
void AMap<K, T, V, HashFn>::Sweep()
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
