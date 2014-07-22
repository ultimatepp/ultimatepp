template <class T>
T& Single() {
	static T *p;
	ONCELOCK {
		static T o;
		p = &o;
	}
	return *p;
}

int RegisterTypeNo__(const char *type);

template <class T>
int StaticTypeNo() {
	static int typeno;
	ONCELOCK {
		typeno = RegisterTypeNo__(typeid(T).name());
	}
	return typeno;
}

template <class T>
class One : MoveableAndDeepCopyOption< One<T> > {
	mutable T  *ptr;

	void        Free()                     { if(ptr && ptr != (T*)1) delete ptr; }
	void        Chk() const                { ASSERT(ptr != (T*)1); }
	void        ChkP() const               { Chk(); ASSERT(ptr); }
	void        Pick(One<T> rval_ data)    { T *p = data.ptr; data.ptr = (T*)1; ptr = p; }

public:
	void        Attach(T *data)            { Free(); ptr = data; }
	T          *Detach()                   { ChkP(); T *t = ptr; ptr = NULL; return t; }
	T          *operator-()                { return Detach(); }
	void        Clear()                    { Free(); ptr = NULL; }

	void        operator=(T *data)         { Attach(data); }
	void        operator=(One<T> rval_  d) { Free(); Pick(pick(d)); }

	const T    *operator->() const         { ChkP(); return ptr; }
	T          *operator->()               { ChkP(); return ptr; }
	const T    *operator~() const          { Chk(); return ptr; }
	T          *operator~()                { Chk(); return ptr; }
	const T&    operator*() const          { ChkP(); return *ptr; }
	T&          operator*()                { ChkP(); return *ptr; }

	template <class TT>
	TT&         Create()                   { TT *q = new TT; Attach(q); return *q; }
	T&          Create()                   { T *q = new T; Attach(q); return *q; }

	template <class TT>
	bool        Is() const                 { return dynamic_cast<const TT *>(ptr); }

	bool        IsPicked() const           { return ptr == (T*)1; }
	bool        IsEmpty() const            { Chk(); return !ptr; }

	operator bool() const                  { return ptr; }
	
	String ToString() const                { return ptr ? AsString(*ptr) : "<empty>"; }

	One()                                  { ptr = NULL; }
	One(T *newt)                           { ptr = newt; }
	One(One<T> rval_ p)                    { Pick(pick(p)); }
	One(const One<T>& p, int)              { ptr = p.IsEmpty() ? NULL : DeepCopyNew(*p); }
	~One()                                 { Free(); }
};

class Any : Moveable<Any> {
	struct BaseData {
		int      typeno;
		
		virtual ~BaseData() {}
	};

	template <class T>
	struct Data : BaseData {
		T        data;

		Data()                                    { typeno = StaticTypeNo<T>(); }
	};

	BaseData *ptr;

	void Chk() const                              { ASSERT(ptr != (void *)1); }
	void Pick(Any rval_ s)                        { ptr = s.ptr; const_cast<Any&>(s).ptr = (BaseData *)1; }

public:
	template <class T> T& Create()                { Clear(); Data<T> *x = new Data<T>; ptr = x; return x->data; }
	template <class T> bool Is() const            { return ptr && ptr->typeno == StaticTypeNo<T>(); }
	template <class T> T& Get()                   { ASSERT(Is<T>()); Chk(); return ((Data<T>*)ptr)->data; }
	template <class T> const T& Get() const       { ASSERT(Is<T>()); Chk(); return ((Data<T>*)ptr)->data; }

	void Clear()                                  { if(ptr && !IsPicked()) delete ptr; ptr = NULL; }

	bool IsEmpty() const                          { return ptr == NULL; }
	bool IsPicked() const                         { return ptr == (void *)1; }

	void operator=(Any rval_ s)                   { Clear(); Pick(pick(s)); }
	Any(Any rval_ s)                              { Pick(pick(s)); }

	Any()                                         { ptr = NULL; }
	~Any()                                        { Clear(); }
};

template <class T>
class Buffer : Moveable< Buffer<T> > {
	mutable T *ptr;

public:
	operator T*()                        { return ptr; }
	operator const T*() const            { return ptr; }
	T *operator~()                       { return ptr; }
	const T *operator~() const           { return ptr; }

	void Alloc(size_t size)              { Clear(); ptr = new T[size]; }
	void Alloc(size_t size, const T& in) { Clear(); ptr = new T[size]; Fill(ptr, ptr + size, in); }

	void Clear()                         { if(ptr) delete[] ptr; ptr = NULL; }

	Buffer()                             { ptr = NULL; }
	Buffer(size_t size)                  { ptr = new T[size]; }
	Buffer(size_t size, const T& init)   { ptr = new T[size]; Fill(ptr, ptr + size, init); }
	~Buffer()                            { if(ptr) delete[] ptr; }

	void operator=(Buffer rval_ v)      { if(ptr) delete[] ptr; ptr = v.ptr; v.ptr = NULL; }
	Buffer(Buffer rval_ v)              { ptr = v.ptr; v.ptr = NULL; }
};

class Bits : Moveable<Bits> {
	mutable int alloc;
	dword      *bp;

public:
	void   Clear();
	void   Set(int i, bool b = true);
	void   Set(int i, bool b, int count); // deprecated
	bool   Get(int i) const        { ASSERT(i >= 0 && alloc >= 0); int q = i >> 5;
	                                 return q < alloc ? bp[q] & (1 << (i & 31)) : false; }
	bool   operator[](int i) const { return Get(i); }

	Bits()                         { bp = NULL; alloc = 0; }
	~Bits()                        { Clear(); }

	Bits(Bits rval_ b)            { alloc = b.alloc; bp = b.bp; b.alloc = -1; }
	void operator=(Bits rval_ b)  { Clear(); alloc = b.alloc; bp = b.bp; b.alloc = -1; }
};

//# System dependent
template <class T>
class Mitor : Moveable< Mitor<T> > {
	union {
		mutable unsigned   count;
		mutable Vector<T> *vector;
	};
	byte elem0[sizeof(T)];

	T&        Get(int i) const;
	void      Pick(Mitor rval_ m);
	void      Copy(const Mitor& m);
	void      Chk() const               { ASSERT(count != 2); }

public:
	T&        operator[](int i)         { return Get(i); }
	const T&  operator[](int i) const   { return Get(i); }
	int       GetCount() const;
	T&        Add();
	void      Add(const T& x);
	void      Clear();
	void      Shrink();

	Mitor(Mitor rval_ m)               { Pick(m); }
	void operator=(Mitor rval_ m)      { Clear(); Pick(pick(m)); }

	Mitor(Mitor& m, int)                { Copy(m); }
	void operator<<=(const Mitor& m)    { Clear(); Copy(pick(m)); }

	Mitor()                             { count = 0; }
	~Mitor()                            { Clear(); }
};

template <class T>
T& Mitor<T>::Get(int i) const
{
	ASSERT(i >= 0 && i < GetCount());
	return i == 0 ? *(T*)elem0 : (*const_cast<Vector<T>*>(vector))[i - 1];
}

template <class T>
void Mitor<T>::Pick(Mitor rval_ m)
{
	m.Chk();
	vector = m.vector;
	memcpy(&elem0, &m.elem0, sizeof(T));
	m.count = 2;
}

template <class T>
void Mitor<T>::Copy(const Mitor& m)
{
	m.Chk();
	if(m.count > 0)
		DeepCopyConstruct(elem0, (const T*)m.elem0);
	if(m.count > 1)
		vector = new Vector<T>(*m.vector, 1);
}

template <class T>
int Mitor<T>::GetCount() const
{
	Chk();
	return count > 1 ? vector->GetCount() + 1 : count;
}

template <class T>
T& Mitor<T>::Add()
{
	Chk();
	if(count == 0) {
		count = 1;
		return *new(elem0) T;
	}
	if(count == 1)
		vector = new Vector<T>;
	return vector->Add();
}

template <class T>
void Mitor<T>::Add(const T& x)
{
	Chk();
	if(count == 0) {
		count = 1;
		new((T*) elem0) T(x);
	}
	else {
		if(count == 1)
			vector = new Vector<T>;
		vector->Add(x);
	}
}

template <class T>
void Mitor<T>::Clear()
{
	if(count > 2)
		delete vector;
	if(count && count != 2)
		((T*)elem0)->T::~T();
	count = 0;
}

template <class T>
void Mitor<T>::Shrink()
{
	if(count > 2)
		vector->Shrink();
}

//#
template <class T, int N = 1>
class Link {
protected:
	T *link_prev[N];
	T *link_next[N];

	void LPN(int i)                      { link_prev[i]->link_next[i] = link_next[i]->link_prev[i] = (T *)this; }

public:
	T       *GetPtr()                    { return (T *) this; }
	const T *GetPtr() const              { return (const T *) this; }
	T       *GetNext(int i = 0)          { return link_next[i]; }
	T       *GetPrev(int i = 0)          { return link_prev[i]; }
	const T *GetNext(int i = 0) const    { return link_next[i]; }
	const T *GetPrev(int i = 0) const    { return link_prev[i]; }

	void LinkSelf(int i = 0)             { link_next[i] = link_prev[i] = (T *)this; }
	void LinkSelfAll()                   { for(int i = 0; i < N; i++) LinkSelf(i); }
	void Unlink(int i = 0)               { link_next[i]->link_prev[i] = link_prev[i]; link_prev[i]->link_next[i] = link_next[i];
	                                       LinkSelf(i); }
	void UnlinkAll()                     { for(int i = 0; i < N; i++) Unlink(i); }
	void LinkBefore(Link *n, int i = 0)  { link_next[i] = (T *)n; link_prev[i] = link_next[i]->link_prev[i]; LPN(i); }
	void LinkAfter(Link *p, int i = 0)   { link_prev[i] = (T *)p; link_next[i] = link_prev[i]->link_next[i]; LPN(i); }

	T   *InsertNext(int i = 0)           { T *x = new T; x->LinkAfter(this, i); return x; }
	T   *InsertPrev(int i = 0)           { T *x = new T; x->LinkBefore(this, i); return x; }

	void DeleteList(int i = 0)           { while(link_next[i] != GetPtr()) delete link_next[i]; }

	bool InList(int i = 0) const         { return link_next[i] != GetPtr(); }
	bool IsEmpty(int i = 0) const        { return !InList(i); }

	Link()                               { LinkSelfAll(); }
	~Link()                              { UnlinkAll(); }

private:
	Link(const Link&);
	void operator=(const Link&);

public:
#ifdef _DEBUG
	void Dump() {
		for(T *t = GetNext(); t != this; t = t->GetNext())
			LOG(t);
		LOG("-------------------------------------");
	}
#endif
};

template <class T, int N = 1>
class LinkOwner : public Link<T, N> {
public:
	~LinkOwner()                         { Link<T, N>::DeleteList(); }
};

template <class T, class K = String>
class LRUCache {
public:
	struct Maker {
		virtual K      Key() const = 0;
		virtual int    Make(T& object) const = 0;
		virtual ~Maker() {}
	};

private:
	struct Item : Moveable<Item> {
		int    prev, next;
		int    size;
		One<T> data;
		bool   flag;
	};
	
	struct Key : Moveable<Key> {
		K            key;
		const void  *type;
		
		bool operator==(const Key& b) const { return key == b.key && type == b.type; }
		unsigned GetHashValue() const { return CombineHash(key, (uintptr_t)type); }
	};

	Index<Key>   key;
	Vector<Item> data;
	int  head;

	int  size;
	int  count;

	int  foundsize;
	int  newsize;
	bool flag;


	void Unlink(int i);
	void LinkHead(int i);

public:
	int  GetSize() const            { return size; }
	int  GetCount() const           { return count; }

	template <class P> void AdjustSize(P getsize);

	T&   GetLRU();
	void DropLRU();
	void Shrink(int maxsize, int maxcount = 30000);

	template <class P> int  Remove(P predicate);
	template <class P> bool RemoveOne(P predicate);

	T&   Get(const Maker& m);

	void Clear();

	void ClearCounters();
	int  GetFoundSize() const       { return foundsize; }
	int  GetNewSize() const         { return newsize; }

	LRUCache() { head = -1; size = 0; count = 0; ClearCounters(); }
};

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
			size += (data[i].size = getsize(*data[i].data));
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
			break;
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
T& LRUCache<T, K>::Get(const Maker& m)
{
	Key k;
	k.key = m.Key();
	k.type = &typeid(m);
	int q = key.Find(k);
	if(q < 0) {
		q = key.Put(k);
		Item& t = data.At(q);
		t.size = m.Make(t.data.Create());
		size += t.size;
		newsize += t.size;
		t.flag = flag;
	}
	else {
		Item& t = data[q];
		Unlink(q);
		if(t.flag != flag) {
			t.flag = flag;
			foundsize += t.size;
		}
	}
	LinkHead(q);
	return *data[q].data;
}
