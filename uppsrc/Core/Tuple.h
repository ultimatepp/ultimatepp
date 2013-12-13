template <typename A, typename B>
struct Tuple2 {
	A a;
	B b;
	
	bool operator==(const Tuple2& x) const    { return a == x.a && b == x.b; }
	bool operator!=(const Tuple2& x) const    { return !operator==(x); }

	int  Compare(const Tuple2& x) const       { return CombineCompare(a, x.a)(b, x.b); }
	bool operator<=(const Tuple2& x) const    { return Compare(x) <= 0; }
	bool operator>=(const Tuple2& x) const    { return Compare(x) >= 0; }
	bool operator<(const Tuple2& x) const     { return Compare(x) < 0; }
	bool operator>(const Tuple2& x) const     { return Compare(x) > 0; }
	
	unsigned GetHashValue() const             { return CombineHash(a, b); }
	
	void Serialize(Stream& s)                 { s % a % b; }

	String   ToString() const                 { return String().Cat() << '(' << a << ", " << b << ')'; }
};

template<typename A, typename B>
inline void AssertMoveable0(Tuple2<A, B> *)
{
	AssertMoveable<A>();
	AssertMoveable<B>();
}

template <typename A, typename B>
inline Tuple2<A, B> MakeTuple(const A& a, const B& b)
{
	Tuple2<A, B> r;
	r.a = a;
	r.b = b;
	return r;
}

template <typename A, typename B, typename C>
struct Tuple3 {
	A a;
	B b;
	C c;
	
	bool operator==(const Tuple3& x) const    { return a == x.a && b == x.b && c == x.c; }
	bool operator!=(const Tuple3& x) const    { return !operator==(x); }

	int  Compare(const Tuple3& x) const       { return CombineCompare(a, x.a)(b, x.b)(c, x.c); }
	bool operator<=(const Tuple3& x) const    { return Compare(x) <= 0; }
	bool operator>=(const Tuple3& x) const    { return Compare(x) >= 0; }
	bool operator<(const Tuple3& x) const     { return Compare(x) < 0; }
	bool operator>(const Tuple3& x) const     { return Compare(x) > 0; }
	
	unsigned GetHashValue() const             { return CombineHash(a, b, c); }

	void Serialize(Stream& s)                 { s % a % b % c; }
	
	String   ToString() const                 { return String().Cat() << '(' << a << ", " << b << ", " << c << ')'; }
};

template<typename A, typename B, typename C>
inline void AssertMoveable0(Tuple3<A, B, C> *)
{
	AssertMoveable<A>();
	AssertMoveable<B>();
	AssertMoveable<C>();
}

template <typename A, typename B, typename C>
inline Tuple3<A, B, C> MakeTuple(const A& a, const B& b, const C& c)
{
	Tuple3<A, B, C> r;
	r.a = a;
	r.b = b;
	r.c = c;
	return r;
}

template <typename A, typename B, typename C, typename D>
struct Tuple4 {
	A a;
	B b;
	C c;
	D d;
	
	bool operator==(const Tuple4& x) const    { return a == x.a && b == x.b && c == x.c && d == x.d; }
	bool operator!=(const Tuple4& x) const    { return !operator==(x); }

	int  Compare(const Tuple4& x) const       { return CombineCompare(a, x.a)(b, x.b)(c, x.c)(d, x.d); }
	bool operator<=(const Tuple4& x) const    { return Compare(x) <= 0; }
	bool operator>=(const Tuple4& x) const    { return Compare(x) >= 0; }
	bool operator<(const Tuple4& x) const     { return Compare(x) < 0; }
	bool operator>(const Tuple4& x) const     { return Compare(x) > 0; }
	
	unsigned GetHashValue() const             { return CombineHash(a, b, c, d); }

	void Serialize(Stream& s)                 { s % a % b % c % d; }

	String   ToString() const                 { return String().Cat() << '(' << a << ", " << b << ", " << c << ", " << d << ')'; }
};

template<typename A, typename B, typename C, typename D>
inline void AssertMoveable0(Tuple4<A, B, C, D> *)
{
	AssertMoveable<A>();
	AssertMoveable<B>();
	AssertMoveable<C>();
	AssertMoveable<D>();
}

template <typename A, typename B, typename C, typename D>
inline Tuple4<A, B, C, D> MakeTuple(const A& a, const B& b, const C& c, const D& d)
{
	Tuple4<A, B, C, D> r;
	r.a = a;
	r.b = b;
	r.c = c;
	r.d = d;
	return r;
}

template <typename T, typename U>
inline T *FindTuple(T *x, int n, const U& key) {
	while(n--) {
		if(x->a == key)
			return x;
		x++;
	}
	return NULL;
}
