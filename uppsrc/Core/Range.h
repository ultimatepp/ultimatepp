template <class I>
class RangeRef {
	I   l;
	int count;

public:
	typedef typename std::remove_reference<decltype(*l)>::type value_type;

	int GetCount() const { return count; }
	
	RangeRef& operator~()               { return *this; }

	value_type& operator[](int i) const { ASSERT(i >= 0 && i < count); return l[i]; }
	I  begin() const { return l; }
	I  end() const { return l + count; }
#ifdef DEPRECATED
	I  Begin() const { return l; }
	I  End() const { return l + count; }
#endif
	RangeRef(I begin, int count) : l(begin), count(count) {}
	RangeRef(I begin, I end)                              { l = begin; count = end - begin; }
};

template <class I>
RangeRef<I> Range(I l, I h)
{
	return RangeRef<I>(l, h);
}

template <class I>
RangeRef<I> Range(I l, int count)
{
	return RangeRef<I>(l, count);
}

template <class C>
auto Range(C& c, int pos, int count) -> decltype(Range(c.begin() + pos, count))
{
	return Range(c.begin() + pos, count);
}

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

	Iterator Begin() const { return Iterator(*this, 0); }
	Iterator End() const { return Iterator(*this, count); }

	ConstRangeClass(const T& value, int count) : value(value), count(count) {}
};

template <class T>
ConstRangeClass<T> ConstRange(const T& value, int count)
{
	return ConstRangeClass<T>(value, count);
}
