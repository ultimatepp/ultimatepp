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


#endif
