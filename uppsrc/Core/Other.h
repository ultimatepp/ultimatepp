template <class T, class... Args>
T& Single(Args... args) {
	static T o(args...);
	return o;
}

template <class T> // Workaround for GCC bug - specialization needed...
T& Single() {
	static T o;
	return o;
}

int RegisterTypeNo__(const char *type);

template <class T>
int StaticTypeNo() {
	static int typeno = RegisterTypeNo__(typeid(T).name());
	return typeno;
}

template <class T>
class One : MoveableAndDeepCopyOption< One<T> > {
	mutable T  *ptr;

	void        Free()                     { if(ptr && ptr != (T*)1) delete ptr; }
	template <class TT>
	void        Pick(One<TT>&& data)       { ptr = data.Detach(); }

public:
	void        Attach(T *data)            { Free(); ptr = data; }
	T          *Detach()                   { T *t = ptr; ptr = NULL; return t; }
	void        Clear()                    { Free(); ptr = NULL; }

	void        operator=(T *data)         { Attach(data); }
	
	template <class TT>
	void        operator=(One<TT>&& d)     { if((void *)this != (void *)&d) { Free(); Pick(pick(d)); }}

	const T    *operator->() const         { ASSERT(ptr); return ptr; }
	T          *operator->()               { ASSERT(ptr); return ptr; }
	const T    *operator~() const          { return ptr; }
	T          *operator~()                { return ptr; }
	const T    *Get() const                { return ptr; }
	T          *Get()                      { return ptr; }
	const T&    operator*() const          { ASSERT(ptr); return *ptr; }
	T&          operator*()                { ASSERT(ptr); return *ptr; }

	template <class TT, class... Args>
	TT&         Create(Args&&... args)     { TT *q = new TT(std::forward<Args>(args)...); Attach(q); return *q; }
	template <class TT> // with C++ conforming compiler, this would not be needed - GCC bug workaround
	TT&         Create()                   { TT *q = new TT; Attach(q); return *q; }
	template <class... Args>
	T&          Create(Args&&... args)     { T *q = new T(std::forward<Args>(args)...); Attach(q); return *q; }
	T&          Create()                   { T *q = new T; Attach(q); return *q; }

	template <class TT>
	bool        Is() const                 { return dynamic_cast<const TT *>(ptr); }

	bool        IsEmpty() const            { return !ptr; }

	operator bool() const                  { return ptr; }
	
	String ToString() const                { return ptr ? AsString(*ptr) : "<empty>"; }

	One()                                  { ptr = NULL; }
	One(T *newt)                           { ptr = newt; }
	template <class TT>
	One(One<TT>&& p)                       { Pick(pick(p)); }
	One(const One<T>& p, int)              { ptr = p.IsEmpty() ? NULL : DeepCopyNew(*p); }
	One(const One<T>& p) = delete;
	~One()                                 { Free(); }
};

template <class T, class... Args>
One<T> MakeOne(Args... args) {
	One<T> r;
	r.Create(args...);
	return r;
}

class Any : Moveable<Any> {
	struct BaseData {
		int      typeno;
		
		virtual ~BaseData() {}
	};

	template <class T>
	struct Data : BaseData {
		T        data;

		template <class... Args>
		Data(Args&&... args) : data(std::forward<Args>(args)...) { typeno = StaticTypeNo<T>(); }
	};

	BaseData *ptr;

	void Chk() const                              { ASSERT(ptr != (void *)1); }
	void Pick(Any&& s)                            { ptr = s.ptr; const_cast<Any&>(s).ptr = NULL; }

public:
	template <class T, class... Args> T& Create(Args&&... args) { Clear(); Data<T> *x = new Data<T>(std::forward<Args>(args)...); ptr = x; return x->data; }
	template <class T> bool Is() const            { return ptr && ptr->typeno == StaticTypeNo<T>(); }
	template <class T> T& Get()                   { ASSERT(Is<T>()); Chk(); return ((Data<T>*)ptr)->data; }
	template <class T> const T& Get() const       { ASSERT(Is<T>()); Chk(); return ((Data<T>*)ptr)->data; }

	void Clear()                                  { if(ptr) delete ptr; ptr = NULL; }

	bool IsEmpty() const                          { return ptr == NULL; }

	void operator=(Any&& s)                       { if(this != &s) { Clear(); Pick(pick(s)); } }
	Any(Any&& s)                                  { Pick(pick(s)); }

	Any()                                         { ptr = NULL; }
	~Any()                                        { Clear(); }
};

class Bits : Moveable<Bits> {
	int         alloc;
	dword      *bp;
	
	void Expand(int q);
	void Realloc(int nalloc);
	int  GetLast() const;

public:
	void   Clear();
	void   Set(int i, bool b = true) { ASSERT(i >= 0 && alloc >= 0); int q = i >> 5;
		                               if(q >= alloc) Expand(q); i &= 31; bp[q] = (bp[q] & ~(1 << i)) | (b << i); }
	bool   Get(int i) const        { ASSERT(i >= 0 && alloc >= 0); int q = i >> 5;
	                                 return q < alloc ? bp[q] & (1 << (i & 31)) : false; }
	bool   operator[](int i) const { return Get(i); }

	void   Set(int i, dword bits, int count);
	dword  Get(int i, int count);
	void   Set64(int i, uint64 bits, int count);
	uint64 Get64(int i, int count);

	void   SetN(int i, bool b, int count);
	void   SetN(int i, int count)         { SetN(i, true, count); }
	
	void   Reserve(int nbits);
	void   Shrink();
	
	String ToString() const;

	dword       *CreateRaw(int n_dwords);
	const dword *Raw(int& n_dwords) const { n_dwords = alloc; return bp; }
	dword       *Raw(int& n_dwords)       { n_dwords = alloc; return bp; }
	
	void         Serialize(Stream& s);

	Bits()                                { bp = NULL; alloc = 0; }
	~Bits()                               { Clear(); }

	Bits(Bits&& b)                        { alloc = b.alloc; bp = b.bp; b.bp = NULL; }
	void operator=(Bits&& b)              { if(this != &b) { Clear(); alloc = b.alloc; bp = b.bp; b.bp = NULL; } }
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
	void      Pick(Mitor&& m);
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

	Mitor(Mitor&& m)                    { Pick(pick(m)); }
	void operator=(Mitor&& m)           { if(this != &m) { Clear(); Pick(pick(m)); } }

	Mitor(Mitor& m, int)                { Copy(m); }

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
void Mitor<T>::Pick(Mitor&& m)
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
		((T*)elem0)->~T();
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
struct Link {
	T *link_prev[N];
	T *link_next[N];

protected:
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
