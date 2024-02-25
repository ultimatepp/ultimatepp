// --------------------

template <class K, class T, class V>
template <class KK>
int AMap<K, T, V>::FindAdd_(KK&& k) {
	return key.FindAdd(std::forward<KK>(k), [&] { value.Add(); });
}

template <class K, class T, class V>
template <class KK, class TT>
int AMap<K, T, V>::FindAdd_(KK&& k, TT&& x) {
	return key.FindAdd(std::forward<KK>(k), [&] { value.Add(std::forward<TT>(x)); });
}


template <class K, class T, class V>
template <class KK>
T&  AMap<K, T, V>::Put_(KK&& k)
{
	int i = key.Put(std::forward<KK>(k));
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
template <class KK, class TT>
int AMap<K, T, V>::Put_(KK&& k, TT&& x)
{
	int i = key.Put(std::forward<KK>(k));
	if(i < value.GetCount())
		value[i] = std::forward<TT>(x);
	else {
		ASSERT(i == value.GetCount());
		value.Add(std::forward<TT>(x));
	}
	return i;
}

template <class K, class T, class V>
template <class KK>
int AMap<K, T, V>::PutDefault_(KK&& k)
{
	int i = key.Put(std::forward<KK>(k));
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
template <class KK>
int AMap<K, T, V>::FindPut_(KK&& k)
{
	int i = key.FindPut(std::forward<KK>(k));
	if(i >= value.GetCount())
		value.Add();
	return i;
}

template <class K, class T, class V>
template <class KK, class TT>
int AMap<K, T, V>::FindPut_(KK&& k, TT&& init)
{
	int i = key.FindPut(std::forward<KK>(k));
	if(i >= value.GetCount())
		value.Add(std::forward<TT>(init));
	return i;
}

template <class K, class T, class V>
template <class KK>
T&  AMap<K, T, V>::GetAdd_(KK&& k)
{
	return value[FindAdd(std::forward<KK>(k))];
}

template <class K, class T, class V>
template <class KK, class TT>
T&  AMap<K, T, V>::GetAdd_(KK&& k, TT&& x)
{
	return value[FindAdd(std::forward<KK>(k), std::forward<TT>(x))];
}

template <class K, class T, class V>
template <class KK>
T&  AMap<K, T, V>::GetPut_(KK&& k)
{
	int i = key.FindPut(std::forward<KK>(k));
	return i < value.GetCount() ? value[i] : value.Add();
}

template <class K, class T, class V>
template <class KK, class TT>
T&  AMap<K, T, V>::GetPut_(KK&& k, TT&& x)
{
	bool put = false;
	int i = key.FindPut(std::forward<KK>(k), put);
	if(i < value.GetCount()) {
		if(put)
			value[i] = std::forward<TT>(x);
		return value[i];
	}
	return value.Add(std::forward<TT>(x));
}

template <class K, class T, class V>
void AMap<K, T, V>::Serialize(Stream& s) {
	int version = 0;
	s / version % key % value;
	if(key.GetCount() != value.GetCount()) {
		Clear();
		s.LoadError();
	}
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
	value.RemoveIf([&](int i) { return key.IsUnlinked(i); });
	key.Sweep();
}

#ifdef UPP
template <class K, class T, class V, class Less>
void FixedAMap<K, T, V, Less>::Serialize(Stream& s) {
	s % key % value;
	if(key.GetCount() != value.GetCount())
		s.LoadError();
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
