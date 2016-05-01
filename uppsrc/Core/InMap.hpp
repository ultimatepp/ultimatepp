template <class T, class Less>
int SortedIndex<T, Less>::FindAdd(const T& key)
{
	int i = FindLowerBound(key);
	if(i == GetCount() || Less()(key, iv[i]))
		iv.Insert(i, key);
	return i;
}

template <class T, class Less>
int SortedIndex<T, Less>::FindNext(int i) const
{
	return i + 1 < iv.GetCount() && !Less()(iv[i], iv[i + 1]) ? i + 1 : -1;
}

template <class T, class Less>
int SortedIndex<T, Less>::FindLast(const T& x) const
{
	int i = iv.FindUpperBound(x, Less());
	return i > 0 && !Less()(iv[i - 1], x) ? i - 1 : -1;
}

template <class T, class Less>
int SortedIndex<T, Less>::FindPrev(int i) const
{
	return i > 0 && !Less()(iv[i - 1], iv[i]) ? i - 1 : -1;
}

template <class T, class Less>
int SortedIndex<T, Less>::RemoveKey(const T& x)
{
	int l = FindLowerBound(x);
	int count = FindUpperBound(x) - l;
	Remove(l, count);
	return count;
}

template <class T, class Less>
String SortedIndex<T, Less>::ToString() const
{
	return AsStringArray(*this);
}

template <class T>
void Slaved_InVector__<T>::Insert(int blki, int pos)
{
	data.data[blki].Insert(pos);
	res = &data.data[blki][pos];
}

template <class T>
void Slaved_InVector__<T>::Split(int blki, int nextsize)
{
	Vector<T>& x = data.data.Insert(blki + 1);
	x.InsertSplit(0, data.data[blki], nextsize);
	data.data[blki].Shrink();
}

template <class T>
void Slaved_InVector__<T>::AddFirst()
{
	data.data.Add().Add();
	res = &data.data[0][0];
}

template <class K, class T, class Less, class Data>
String SortedAMap<K, T, Less, Data>::ToString() const
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

template <class K, class T, class Less>
SortedVectorMap<K, T, Less>::SortedVectorMap(SortedVectorMap&& s)
{
	B::key = pick(s.key);
	B::value.data = pick(s.value.data);
	B::SetSlave();
}

template <class K, class T, class Less>
SortedVectorMap<K, T, Less>& SortedVectorMap<K, T, Less>::operator=(SortedVectorMap&& s)
{
	B::key = pick(s.key);
	B::value.data = pick(s.value.data);
	B::SetSlave();
	return *this;
}

template <class K, class T, class Less>
SortedVectorMap<K, T, Less>::SortedVectorMap(const SortedVectorMap& s, int)
{
	B::key = clone(s.key);
	B::value.data = clone(s.value.data);
	B::SetSlave();
}

template <class K, class T, class Less>
int SortedVectorMap<K, T, Less>::FindAdd(const K& k, const T& init)
{
	B::value.res = NULL;
	int q = B::key.FindAdd(k);
	if(B::value.res)
		*B::value.res = init;
	return q;
}

template <class K, class T, class Less>
void SortedVectorMap<K, T, Less>::Swap(SortedVectorMap& x)
{
	B::value.data.Swap(x.value.data);
	B::key.Swap(x.B::key);
	B::SetSlave();
	x.SetSlave();
}
               
template <class T>
void Slaved_InArray__<T>::Insert(int blki, int pos)
{
	if(!res)
		res = new T;
	data.iv.data[blki].Insert(pos, res);
}

template <class T>
void Slaved_InArray__<T>::Split(int blki, int nextsize)
{
	Vector< typename InArray<T>::PointerType >& x = data.iv.data.Insert(blki + 1);
	x.InsertSplit(0, data.iv.data[blki], nextsize);
}

template <class T>
void Slaved_InArray__<T>::Remove(int blki, int pos, int n)
{
	Vector< typename InArray<T>::PointerType >& b = data.iv.data[blki];
	for(int i = 0; i < n; i++)
		if(b[i + pos])
			delete (T *)b[i + pos];
	b.Remove(pos, n);
}

template <class T>
void Slaved_InArray__<T>::AddFirst()
{
	if(!res)
		res = new T;
	data.iv.data.Add().Add(res);
}

template <class K, class T, class Less>
int SortedArrayMap<K, T, Less>::FindAdd(const K& k, const T& init)
{
	B::value.res = NULL;
	int q = B::key.FindAdd(k);
	if(B::value.res)
		*B::value.res = init;
	return q;
}

template <class K, class T, class Less>
SortedArrayMap<K, T, Less>::SortedArrayMap(SortedArrayMap&& s)
{
	B::key = pick(s.key);
	B::value.data = pick(s.value.data);
	B::SetSlave();
}

template <class K, class T, class Less>
SortedArrayMap<K, T, Less>& SortedArrayMap<K, T, Less>::operator=(SortedArrayMap&& s)
{
	B::key = pick(s.key);
	B::value.data = pick(s.value.data);
	B::SetSlave();
	return *this;
}

template <class K, class T, class Less>
SortedArrayMap<K, T, Less>::SortedArrayMap(const SortedArrayMap& s, int)
{
	B::key = clone(s.key);
	B::value.data = clone(s.value.data);
	B::SetSlave();
}

template <class K, class T, class Less>
void SortedArrayMap<K, T, Less>::Swap(SortedArrayMap& x)
{
	B::value.data.Swap(x.value.data);
	B::key.Swap(x.B::key);
	B::SetSlave();
	x.SetSlave();
}

#ifdef UPP
template <class K, class T>
void StreamSortedMap(Stream& s, T& cont)
{
	int n = cont.GetCount();
	s / n;
	if(n < 0) {
		s.LoadError();
		return;
	}
	if(s.IsLoading()) {
		cont.Clear();
		while(n--) {
			K key;
			s % key;
			s % cont.Add(key);
		}
	}
	else
		for(int i = 0; i < cont.GetCount(); i++) {
			K key = cont.GetKey(i);
			s % key;
			s % cont[i];
		}
}

template <class K, class T, class Less>
void SortedVectorMap<K, T, Less>::Serialize(Stream& s) {
	StreamSortedMap<K, SortedVectorMap<K, T, Less> >(s, *this);
}

template <class K, class T, class Less>
void SortedVectorMap<K, T, Less>::Xmlize(XmlIO& xio)
{
	XmlizeSortedMap<K, T, SortedVectorMap<K, T, Less> >(xio, "key", "value", *this);
}

template <class K, class T, class Less>
void SortedVectorMap<K, T, Less>::Jsonize(JsonIO& jio)
{
	JsonizeSortedMap<SortedVectorMap<K, T, Less>, K, T>(jio, *this, "key", "value");
}

template <class K, class T, class Less>
void SortedArrayMap<K, T, Less>::Serialize(Stream& s) {
	StreamSortedMap<K, SortedArrayMap<K, T, Less> >(s, *this);
}

template <class K, class T, class Less>
void SortedArrayMap<K, T, Less>::Xmlize(XmlIO& xio)
{
	XmlizeSortedMap<K, T, SortedArrayMap<K, T, Less> >(xio, "key", "value", *this);
}

template <class K, class T, class Less>
void SortedArrayMap<K, T, Less>::Jsonize(JsonIO& jio)
{
	JsonizeSortedMap<SortedArrayMap<K, T, Less>, K, T>(jio, *this, "key", "value");
}

#endif
