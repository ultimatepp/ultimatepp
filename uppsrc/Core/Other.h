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
	
	Any(const Any& s) = delete;
	void operator=(const Any& s) = delete;

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
	void   Set(int i, bool b = true) { ASSERT(i >= 0 && alloc >= 0); int q = i >> 5; if(q >= alloc) Expand(q);
	                                   i &= 31; bp[q] = (bp[q] & ~(1 << i)) | (b << i); }
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
	
	Bits(const Bits&) = delete;
	void operator=(const Bits&) = delete;
};

class PackedData {
	void *ptr = nullptr;
	
	template <class T>
	T Get(int ii, T def) const;

public:
	void   SetRawPtr(void *p)                   { ptr = p; }
	void  *GetRawPtr() const                    { return ptr; }

	void   SetData(int ii, const void *data, int datalen);

	template <class F>
	bool   GetData(int ii, F out) const;
	
	void   SetNull(int ii)                      { SetData(ii, NULL, 0); }

	void   SetString(int ii, const char *s)     { SetData(ii, s, (int)strlen(s)); }
	void   SetString(int ii, const String& s)   { SetData(ii, s, s.GetCount()); }
	String GetString(int ii) const              { String r; GetData(ii, [&](const char *s, int n) { r = String(s, n); }); return r; }
	
	void   SetInt(int ii, int val)              { SetData(ii, &val, sizeof(int)); }
	int    GetInt(int ii, int def) const        { return Get<int>(ii, def); }

	void   SetDword(int ii, dword val)          { SetData(ii, &val, sizeof(dword)); }
	int    GetDword(int ii, dword def) const    { return Get<dword>(ii, def); }

	void   SetInt64(int ii, int64 val)          { SetData(ii, &val, sizeof(int64)); }
	int64  GetInt64(int ii, int64 def) const    { return Get<int64>(ii, def); }

	void   SetPtr(int ii, void *val)            { SetData(ii, &val, sizeof(void *)); }
	void  *GetPtr(int ii) const                 { return Get<void *>(ii, nullptr); }
	
	void   Clear();

	Vector<String> Unpack() const;
	size_t         GetPackedSize() const;
	String         GetPacked() const           { return String((const char *)ptr, (int)GetPackedSize()); }

	PackedData() {}
	PackedData(const PackedData&) = delete;
	~PackedData();
};

/*
template <class T, int N = 1>
struct Link {
	T *link_prev[N];
	T *link_next[N];

protected:
	void LPN(int i)                      { link_prev[i]->link_next[i] = link_next[i]->link_prev[i] = (T *)this; }

public:
	NOUBSAN	T *GetPtr()                  { return (T *) this; }
	const T *GetPtr() const              { return (const T *) this; }
	T       *GetNext(int i = 0)          { return link_next[i]; }
	T       *GetPrev(int i = 0)          { return link_prev[i]; }
	const T *GetNext(int i = 0) const    { return link_next[i]; }
	const T *GetPrev(int i = 0) const    { return link_prev[i]; }

	NOUBSAN	void LinkSelf(int i = 0)     { link_next[i] = link_prev[i] = (T *)this; }
	void LinkSelfAll()                   { for(int i = 0; i < N; i++) LinkSelf(i); }
	void Unlink(int i = 0)               { link_next[i]->link_prev[i] = link_prev[i]; link_prev[i]->link_next[i] = link_next[i];
	                                       LinkSelf(i); }
	void UnlinkAll()                     { for(int i = 0; i < N; i++) Unlink(i); }
	NOUBSAN	void LinkBefore(Link *n, int i = 0)  { link_next[i] = (T *)n; link_prev[i] = link_next[i]->link_prev[i]; LPN(i); }
	NOUBSAN	void LinkAfter(Link *p, int i = 0)   { link_prev[i] = (T *)p; link_next[i] = link_prev[i]->link_next[i]; LPN(i); }

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
*/

template <int N = 1>
struct Link {
	Link *link_prev[N];
	Link *link_next[N];

protected:
	void LPN(int i)                      { link_prev[i]->link_next[i] = link_next[i]->link_prev[i] = this; }

public:
	Link       *GetNext(int i = 0)          { return link_next[i]; }
	Link       *GetPrev(int i = 0)          { return link_prev[i]; }
	const Link *GetNext(int i = 0) const    { return link_next[i]; }
	const Link *GetPrev(int i = 0) const    { return link_prev[i]; }

	void LinkSelf(int i = 0)     { link_next[i] = link_prev[i] = this; }
	void LinkSelfAll()                   { for(int i = 0; i < N; i++) LinkSelf(i); }
	void Unlink(int i = 0)               { link_next[i]->link_prev[i] = link_prev[i]; link_prev[i]->link_next[i] = link_next[i];
	                                       LinkSelf(i); }
	void UnlinkAll()                     { for(int i = 0; i < N; i++) Unlink(i); }
	void LinkBefore(Link *n, int i = 0)  { link_next[i] = n; link_prev[i] = link_next[i]->link_prev[i]; LPN(i); }
	void LinkAfter(Link *p, int i = 0)   { link_prev[i] = p; link_next[i] = link_prev[i]->link_next[i]; LPN(i); }

	bool InList(int i = 0) const         { return link_next[i] != this; }
	bool IsEmpty(int i = 0) const        { return !InList(i); }

	Link()                               { LinkSelfAll(); }
	~Link()                              { UnlinkAll(); }

private:
	Link(const Link&);
	void operator=(const Link&);

public:
#ifdef _DEBUG
	void Dump() {
		for(auto *t = GetNext(); t != this; t = t->GetNext())
			LOG(t);
		LOG("-------------------------------------");
	}
#endif
};
/*
template <class T, int N = 1>
class LinkOwner : public Link<T, N> {
public:
	~LinkOwner()                         { Link<T, N>::DeleteList(); }
};
*/
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
		String       type;
		
		bool operator==(const Key& b) const { return key == b.key && type == b.type; }
		hash_t GetHashValue() const { return CombineHash(key, type); }
	};

	Index<Key>   key;
	Vector<Item> data;
	int  head;

	int  size;
	int  count;

	int  foundsize;
	int  newsize;
	bool flag = false;
	
	const int InternalSize = 3 * (sizeof(Item) + sizeof(Key) + 24) / 2;

	void Unlink(int i);
	void LinkHead(int i);

public:
	int  GetSize() const            { return size; }
	int  GetCount() const           { return count; }

	template <class P> void AdjustSize(P getsize);

	T&       GetLRU();
	const K& GetLRUKey();
	void     DropLRU();
	void     Shrink(int maxsize, int maxcount = 30000);
	void     ShrinkCount(int maxcount = 30000)          { Shrink(INT_MAX, maxcount); }

	template <class P> int  Remove(P predicate);
	template <class P> bool RemoveOne(P predicate);

	template <class B, class A>
	T&   Get(const Maker& m, B before_make, A after_make, int& sz);
	template <class B, class A>
	T&   Get(const Maker& m, B before_make, A after_make) { int sz; return Get(m, before_make, after_make, sz); }
	T&   Get(const Maker& m)                              { return Get(m, []{}, []{}); }

	void Clear();

	void ClearCounters();
	int  GetFoundSize() const       { return foundsize; }
	int  GetNewSize() const         { return newsize; }

	LRUCache() { head = -1; size = 0; count = 0; ClearCounters(); }
};

#include "Other.hpp"
