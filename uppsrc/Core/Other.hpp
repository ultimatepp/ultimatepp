template <class F>
bool PackedData::GetData(int ii, F out) const
{
	int i = 0;
	const byte *s = (const byte *)ptr;
	if(s)
		for(;;) {
			int len = *s++;
			if(len == 255)
				break;
			if(len == 254) {
				memcpy(&len, s, 4);
				s += 4;
			}
			if(i == ii) {
				out((const char *)s, len);
				return true;
			}
			s += len;
			i++;
		}
	out("", 0);
	return false;
}

template <class T>
T PackedData::Get(int ii, T def) const
{
	T q = def;
	GetData(ii, [&](const char *ptr, int len) {
		if(len) {
			ASSERT(len == sizeof(T));
			memcpy(&q, ptr, sizeof(T));
		}
	});
	return q;
}

template <class T, class K>
void LRUCache<T, K>::LinkHead(int i)
{
	Item& m = data[i];
	if(head >= 0) {
		int tail = data[head].prev;
		m.next = head;
		m.prev = tail;
		data[head].prev = i;
		data[tail].next = i;
	}
	else
		m.prev = m.next = i;
	head = i;
	count++;
}


template <class T, class K>
void LRUCache<T, K>::Unlink(int i)
{
	Item& m = data[i];
	if(m.prev == i)
		head = -1;
	else {
		if(head == i)
			head = m.next;
		data[m.next].prev = m.prev;
		data[m.prev].next = m.next;
	}
	count--;
}

template <class T, class K>
T& LRUCache<T, K>::GetLRU()
{
	int tail = data[head].prev;
	return *data[tail].data;
}

template <class T, class K>
const K& LRUCache<T, K>::GetLRUKey()
{
	int tail = data[head].prev;
	return key[tail].key;
}

template <class T, class K>
void LRUCache<T, K>::DropLRU()
{
	if(head >= 0) {
		int tail = data[head].prev;
		size -= data[tail].size;
		data[tail].data.Clear();
		Unlink(tail);
		key.Unlink(tail);
	}
}

template <class T, class K>
template <class P>
void LRUCache<T, K>::AdjustSize(P getsize)
{
	size = 0;
	count = 0;
	for(int i = 0; i < data.GetCount(); i++)
		if(!key.IsUnlinked(i)) {
			int sz = getsize(*data[i].data);
			if(sz >= 0)
				data[i].size = sz + InternalSize;
			size += data[i].size;
			count++;
		}
}

template <class T, class K>
template <class P>
int LRUCache<T, K>::Remove(P predicate)
{
	int n = 0;
	int i = 0;
	while(i < data.GetCount())
		if(!key.IsUnlinked(i) && predicate(*data[i].data)) {
			size -= data[i].size;
			Unlink(i);
			key.Unlink(i);
			n++;
		}
		else
			i++;
	return n;
}

template <class T, class K>
template <class P>
bool LRUCache<T, K>::RemoveOne(P predicate)
{
	int i = head;
	if(i >= 0)
		for(;;) {
			int next = data[i].next;
			if(predicate(*data[i].data)) {
				size -= data[i].size;
				Unlink(i);
				key.Unlink(i);
				return true;
			}
			if(i == next || next == head || next < 0)
				break;
			i = next;
		}
	return false;
}

template <class T, class K>
void LRUCache<T, K>::Shrink(int maxsize, int maxcount)
{
	if(maxsize >= 0 && maxcount >= 0)
		while(count > maxcount || size > maxsize)
			DropLRU();
}

template <class T, class K>
void LRUCache<T, K>::Clear()
{
	head = -1;
	size = 0;
	count = 0;
	newsize = foundsize = 0;
	key.Clear();
	data.Clear();
}

template <class T, class K>
void LRUCache<T, K>::ClearCounters()
{
	flag = !flag;
	newsize = foundsize = 0;
}

template <class T, class K>
template <class B, class A>
T& LRUCache<T, K>::Get(const Maker& m, B before_make, A after_make, int& sz)
{
	Key k;
	k.key = m.Key();
	k.type = typeid(m).name();
	int q = key.Find(k);
	if(q < 0) {
		One<T> val;
		before_make();
		int sz = m.Make(val.Create());
		after_make();
		q = key.Put(k);
		Item& t = data.At(q);
		t.data = pick(val);
		t.size = sz + InternalSize;
		size += t.size;
		newsize += t.size;
		t.flag = flag;
		sz = t.size;
	}
	else {
		Item& t = data[q];
		sz = t.size;
		Unlink(q);
		if(t.flag != flag) {
			t.flag = flag;
			foundsize += t.size;
		}
	}
	LinkHead(q);
	return *data[q].data;
}
