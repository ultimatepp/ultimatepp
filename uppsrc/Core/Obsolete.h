#ifdef DEPRECATED

template<class T>
struct StdEqual
{
	bool operator () (const T& a, const T& b) const { return a == b; }
};

template<class T>
struct StdLess {
	bool operator () (const T& a, const T& b) const { return a < b; }
};

template<class T>
struct StdGreater
{
	bool operator () (const T& a, const T& b) const { return a > b; }
};

template <class M, class T, class R>
class MethodRelationCls {
	M        method;
	const R& relation;

public:
	MethodRelationCls(M method, const R& relation) : method(method), relation(relation) {}
	bool operator () (const T& t1, const T& t2) const {
		return relation((t1.*method)(), (t2.*method)());
	}
};

template <class O, class T, class R>
inline MethodRelationCls<O (T::*)(), T, R>
	MethodRelation(O (T::*method)(), const R& relation)
{ return MethodRelationCls<O (T::*)(), T, R>(method, relation); }

template <class O, class T, class R>
inline MethodRelationCls<O (T::*)() const, T, R>
	MethodRelation(O (T::*method)() const, const R& relation)
{ return MethodRelationCls<O (T::*)() const, T, R>(method, relation); }

template <class I, class C>
int IterCompare(I a, I a_end, I b, I b_end, const C& compare)
{
	for(;;) {
		if(a >= a_end)
			return b < b_end ? -1 : 0;
		if(b >= b_end)
			return a < a_end ? 1 : 0;
		int q = compare(*a++, *b++);
		if(q)
			return q;
	}
}

template <class I, class K, class X>
int BinFindCompIndex(I begin, I end, const K& key, const X& comp)
{
	if(begin == end) // empty array
		return 0;
	int min = 0;
	int max = end - begin;
	while(min < max)
	{
		int mid = (max + min) >> 1;
		if(comp.Compare(key, *(begin + mid)) > 0)
			min = mid + 1;
		else
			max = mid;
	}
	return min;
}

template <class C, class K, class X>
inline int BinFindCompIndex(const C& container, const K& key, const X& comp)
{
	return BinFindCompIndex(container.Begin(), container.End(), key, comp);
}

template <class I, class K, class X>
inline I BinFindComp(I begin, I end, const K& key, const X& comp)
{
	return begin + BinFindCompIndex(begin, end, key, comp);
}

template <class C, class K, class X>
inline typename C::ConstIterator BinFindComp(const C& container, const K& key, const X& comp)
{
	return BinFindComp(container.Begin(), container.End(), key, comp);
}

template <class T, class V>
void Append(T& dst, V ptr, V end)
{
	for(; ptr != end; ++ptr)
		dst.Add(*ptr);
}

template <class T, class V>
void Append(T& dst, V ptr, int n)
{
	for(; n--; ++ptr)
		dst.Add(*ptr);
}

template <class T, class V>
void Append(T& dst, const V& src)
{
	Append(dst, src.Begin(), src.End());
}

template <class C, class I>
C& FindAppend(C& dest, I begin, I end)
{
	for(; begin != end; ++begin)
		dest.FindAdd(*begin);
	return dest;
}

template <class C, class S>
inline C& FindAppend(C& dest, const S& source)
{
	return FindAppend(dest, source.Begin(), source.End());
}

template <class C>
int MinIndex(const C& c) { return FindMin(c); }

template <class C>
int MaxIndex(const C& c) { return FindMax(c); }

template <class C>
typename C::ValueType Sum0(const C& c) { return Sum(c); }

template <class I, class F, class O, class E>
I FindField(I begin, I end, F field, const O& object, const E& equal)
{
	for(; begin != end && !equal((*begin).*field, object); ++begin)
		;
	return begin;
}

template <class I, class F, class O>
I FindField(I begin, I end, F field, const O& object)
{ return FindField(begin, end, field, object, StdEqual<O>()); }

template <class C, class F, class O, class E>
int FindFieldIndex(const C& container, F field, const O& object, const E& equal)
{
	int i = 0;
	for(typename C::ConstIterator b = container.Begin(), e = container.End(); b != e; ++b, ++i)
		if(equal((*b).*field, object))
			return i;
	return -1;
}

template <class C, class F, class O>
int FindFieldIndex(const C& container, F field, const O& object)
{ return FindFieldIndex(container, field, object, StdEqual<O>()); }

template <class DC, class I, class F>
void GetFieldContainer(DC& dest, I begin, I end, F field)
{
	for(; begin != end; ++begin)
		dest.Add((*begin).*field);
}

template <class DC, class SC, class F>
void GetFieldContainer(DC& dest, const SC& src, F field)
{ GetFieldContainer<DC, typename SC::ConstIterator, F>(dest, src.Begin(), src.End(), field); }


template <class C>
int FindMin(const C& c, int pos, int count)
{
	return FindMin(SubRange(c, pos, count));
}

template <class T, class C>
bool IsEqual(T ptr1, T end1, T ptr2, T end2, const C& equal)
{
	for(; ptr1 != end1 && ptr2 != end2; ++ptr1, ++ptr2)
		if(!equal(*ptr1, *ptr2)) return false;
	return ptr1 == end1 && ptr2 == end2;
}

template <class T, class C>
bool IsEqual(const T& c1, const T& c2, const C& equal)
{
	return IsEqual(c1.Begin(), c1.End(), c2.Begin(), c2.End(), equal);
}

template <class T>
bool IsEqual(const T& c1, const T& c2)
{
	typedef typename T::ValueType VT;
	return IsEqual(c1, c2, std::equal<VT>());
}

template <class T, class V, class C>
T Find(T ptr, T end, const V& value, const C& equal)
{
	while(ptr != end) {
		if(equal(*ptr, value)) return ptr;
		ptr++;
	}
	return NULL;
}

template <class T, class V>
T Find(T ptr, T end, const V& value)
{
	return Find(ptr, end, value, std::equal<V>());
}

template <class I, class K, class L>
size_t BinFindIndex(I begin, I end, const K& key, const L& less)
{
	if(begin == end)
		return 0;
	size_t min = 0;
	size_t max = end - begin;

	while(min < max)
	{
		size_t mid = (max + min) >> 1;
		if(less(*(begin + mid), key))
			min = mid + 1;
		else
			max = mid;
	}
	return min;
}

template <class C, class K, class L>
inline int BinFindIndex(const C& container, const K& key, const L& less)
{
	return BinFindIndex(container.Begin(), container.End(), key, less);
}

template <class C, class K>
inline int BinFindIndex(const C& container, const K& key)
{
	typedef typename C::ValueType VT;
	return BinFindIndex(container, key, std::less<VT>());
}

template <class I, class K, class L>
inline I BinFind(I begin, I end, const K& key, const L& less)
{
	return begin + BinFindIndex(begin, end, key, less);
}

template <class C, class K, class L>
inline typename C::ConstIterator BinFind(const C& container, const K& key, const L& less)
{
	return BinFind(container.Begin(), container.End(), key, less);
}

template <class C, class K>
inline typename C::ConstIterator BinFind(const C& container, const K& key)
{
	typedef typename C::ValueType VT;
	return BinFind(container, key, std::less<VT>());
}

template <class C, class T, class L>
int FindLowerBound(const C& v, int pos, int count, const T& val, const L& less)
{
	return pos + FindLowerBound(SubRange(v, pos, count), val, less);
}

template <class I, class T, class L>
I FindLowerBoundIter(I begin, I end, const T& val, const L& less)
{
	return begin + FindLowerBound(SubRange(begin, end), val, less);
}

template <class I, class T>
I FindLowerBoundIter(I begin, I end, const T& val)
{
	return begin + FindLowerBound(SubRange(begin, end), val, std::less<T>());
}

template <class C, class T, class L>
int FindUpperBound(const C& v, int pos, int count, const T& val, const L& less)
{
	return pos + FindUpperBound(SubRange(v, pos, count), val, less);
}

template <class I, class T, class L>
I FindUpperBoundIter(I begin, I end, const T& val, const L& less)
{
	return begin + FindUpperBound(SubRange(begin, end), val, less);
}

template <class I, class T>
I FindUpperBoundIter(I begin, I end, const T& val)
{
	return begin + FindUpperBound(SubRange(begin, end), val, std::less<T>());
}

template <class C, class T, class L>
int FindBinary(const C& v, const T& val, int pos, int count, const L& less)
{
	return pos + FindBinary(SubRange(v, pos, count), val, less);
}

template <class I, class T, class L>
I FindBinaryIter(I begin, I end, const T& val, const L& less)
{
	return begin + FindBinary(SubRange(begin, end), val, less);
}

template <class C, class T>
int FindBinary(const C& v, const T& val, int pos, int count)
{
	return pos + FindBinary(SubRange(v, pos, count), val, std::less<T>());
}

template <class I, class T>
I FindBinaryIter(I begin, I end, const T& val)
{
	return begin + FindBinary(SubRange(begin, end), val, std::less<T>());
}

template <class T, class V>
void Sum(V& sum, T ptr, T end)
{
	while(ptr != end)
		sum += *ptr++;
}

// -----------------------------------------------------------

#if 0

template <class I, class Less>
void StableSort(I begin, I end, const Less& less)
{
	if(begin != end)
		StableSort__(begin, end, less, &*begin);
}

template <class T, class Cmp>
struct StableSortLessCmp_ {
	const Cmp& cmp;
	bool operator()(const StableSortItem<T>& a, const StableSortItem<T>& b) const {
		int q = cmp(a.value, b.value);
		return q ? q < 0 : a.index < b.index;
	}

	StableSortLessCmp_(const Cmp& cmp) : cmp(cmp) {}
};

template <class I, class Cmp, class T>
void StableSortCmp_(I begin, I end, const Cmp& cmp, const T *)
{
	int count = end - begin;
	Buffer<int> h(count);
	for(int i = 0; i < count; i++)
		h[i] = i;
	Sort(StableSortIterator<I, T>(begin, ~h),
	     StableSortIterator<I, T>(end, ~h + count),
	     StableSortLessCmp_<T, Cmp>(cmp));
}

template <class I, class Cmp>
void StableSortCmp(I begin, I end, const Cmp& cmp)
{
	if(begin != end)
		StableSortCmp_(begin, end, cmp, &*begin);
}

template <class T, class Cmp>
void StableSortCmp(T& c, const Cmp& cmp)
{
	StableSortCmp(c.begin(), c.end(), cmp);
}

template <class T>
struct StdCmp {
	int operator()(const T& a, const T& b) const {
		return SgnCompare(a, b);
	}
};

template <class T>
void StableSortCmp(T& c)
{
	StableSortCmp(c.begin(), c.end(), StdCmp<typename T::ValueType>());
}


template <class I, class T, class Cmp>
inline void __StableSortOrderCmp(int *ibegin, int *iend, I data, const Cmp& cmp, const T *)
{
	Sort(StableSortOrderIterator<I, T>(ibegin, data),
	     StableSortOrderIterator<I, T>(iend, data),
	     StableSortLessCmp_<T, Cmp>(cmp));
}

template <class I, class Cmp>
inline Vector<int> GetStableSortOrderCmp(I begin, I end, const Cmp& cmp)
{
	Vector<int> index;
	index.SetCount((int)(end - begin));
	for(int i = index.GetCount(); --i >= 0; index[i] = i)
		;
	if(begin != end)
		__StableSortOrderCmp(index.begin(), index.end(), begin, cmp, &*begin);
	return index;
}

template <class C, class Cmp>
inline Vector<int> GetStableSortOrderCmp(const C& container, const Cmp& cmp)
{
	return GetStableSortOrderCmp(container.begin(), container.end(), cmp);
}

template <class C>
inline Vector<int> GetStableSortOrderCmp(const C& container)
{
	typedef typename C::ValueType V;
	return GetStableSortOrderCmp(container.begin(), container.end(), StdCmp<V>());
}
#endif

template <class O, class T, class R>
class FieldRelationCls {
	O        T::*member;
	const R& relation;

public:
	FieldRelationCls(O (T::*member), const R& relation) : member(member), relation(relation) {}
	bool operator () (const T& t1, const T& t2) const { return relation(t1.*member, t2.*member); }
};

template <class O, class T, class R>
inline FieldRelationCls<O, T, R> FieldRelation(O (T::*member), const R& relation)
{ return FieldRelationCls<O, T, R>(member, relation); }

#endif
