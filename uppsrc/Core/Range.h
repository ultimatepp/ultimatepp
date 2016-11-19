template <class Range>
using ValueTypeOf = typename std::remove_reference<decltype(*((Range *)0)->begin())>::type;

template <class Range>
using IteratorOf = decltype(((Range *)0)->begin());

template <class Range>
using ConstIteratorOf = decltype(((const Range *)0)->begin());

template <class I>
class SubRangeClass {
	I   l;
	int count;

public:
	typedef typename std::remove_reference<decltype(*l)>::type value_type;

	int GetCount() const { return count; }
	
	SubRangeClass& Write()                   { return *this; }

	value_type& operator[](int i) const { ASSERT(i >= 0 && i < count); return l[i]; }
	I  begin() const { return l; }
	I  end() const { return l + count; }

	String   ToString() const                            { return AsStringArray(*this); }
	template <class B> bool operator==(const B& b) const { return IsEqualRange(*this, b); }
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
	template <class B> int  Compare(const B& b) const    { return CompareRanges(*this, b); }
	template <class B> bool operator<=(const B& x) const { return Compare(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return Compare(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return Compare(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return Compare(x) > 0; }

	SubRangeClass(I begin, int count) : l(begin), count(count)   {}
	SubRangeClass(I begin, I end) : l(begin), count(int(end - begin)) {}
};

template <class I>
SubRangeClass<I> SubRange(I l, I h)
{
	return SubRangeClass<I>(l, h);
}

template <class I>
SubRangeClass<I> SubRange(I l, int count)
{
	return SubRangeClass<I>(l, count);
}

template <class C>
auto SubRange(C& c, int pos, int count) -> decltype(SubRange(c.begin() + pos, count))
{
	return SubRange(c.begin() + pos, count);
}

template <class C>
auto SubRange(const C& c, int pos, int count) -> decltype(SubRange(c.begin() + pos, count))
{
	return SubRange(c.begin() + pos, count);
}

template <class C> using SubRangeOf = decltype(SubRange(((C *)0)->begin(), ((C *)0)->end()));

template <class T>
struct ConstRangeClass {
	T   value;
	int count;
	
	typedef T value_type;
	typedef value_type ValueType;
	
	const value_type& operator[](int i) const { return value; }
	int GetCount() const                      { return count; }
	
	typedef ConstIIterator<ConstRangeClass> Iterator;
	
	Iterator begin() const { return Iterator(*this, 0); }
	Iterator end() const { return Iterator(*this, count); }

	String   ToString() const                            { return AsStringArray(*this); }
	template <class B> bool operator==(const B& b) const { return IsEqualRange(*this, b); }
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
	template <class B> int  Compare(const B& b) const    { return CompareRanges(*this, b); }
	template <class B> bool operator<=(const B& x) const { return Compare(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return Compare(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return Compare(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return Compare(x) > 0; }

	ConstRangeClass(const T& value, int count) : value(value), count(count) {}
	ConstRangeClass(int count) : count(count) {}
};

template <class T>
ConstRangeClass<T> ConstRange(const T& value, int count)
{
	return ConstRangeClass<T>(value, count);
}

template <class T>
ConstRangeClass<T> ConstRange(int count)
{
	return ConstRangeClass<T>(count);
}

template <class BaseRange>
struct ViewRangeClass {
	BaseRange  *r;
	Vector<int> ndx;
	
	typedef typename BaseRange::value_type value_type;
	typedef value_type ValueType;
	
	const value_type& operator[](int i) const { return (*r)[ndx[i]]; }
	value_type& operator[](int i)             { return (*r)[ndx[i]]; }
	int GetCount() const                      { return ndx.GetCount(); }
	
	typedef IIterator<ViewRangeClass> Iterator;
	typedef ConstIIterator<ViewRangeClass> ConstIterator;

	ViewRangeClass& Write()                   { return *this; }

	ConstIterator begin() const { return ConstIterator(*this, 0); }
	ConstIterator end() const  { return ConstIterator(*this, ndx.GetCount()); }
	
	Iterator begin() { return Iterator(*this, 0); }
	Iterator end()   { return Iterator(*this, ndx.GetCount()); }

	String   ToString() const                            { return AsStringArray(*this); }
	template <class B> bool operator==(const B& b) const { return IsEqualRange(*this, b); }
	template <class B> bool operator!=(const B& b) const { return !operator==(b); }
	template <class B> int  Compare(const B& b) const    { return CompareRanges(*this, b); }
	template <class B> bool operator<=(const B& x) const { return Compare(x) <= 0; }
	template <class B> bool operator>=(const B& x) const { return Compare(x) >= 0; }
	template <class B> bool operator<(const B& x) const  { return Compare(x) < 0; }
	template <class B> bool operator>(const B& x) const  { return Compare(x) > 0; }

	ViewRangeClass(BaseRange& r, Vector<int>&& ndx) : r(&r), ndx(pick(ndx)) {}
};

template <class BaseRange>
ViewRangeClass<BaseRange> ViewRange(BaseRange& r, Vector<int>&& ndx)
{
	return ViewRangeClass<BaseRange>(r, pick(ndx));
}

template <class BaseRange, class Predicate>
ViewRangeClass<BaseRange> FilterRange(BaseRange& r, Predicate p)
{
	return ViewRangeClass<BaseRange>(r, FindAll(r, p));
}
