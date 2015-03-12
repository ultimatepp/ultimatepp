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
	
	Tuple2&  SetA(const A& a_)                { a = a_; return *this; }
	Tuple2&  SetB(const B& b_)                { b = b_; return *this; }
	Tuple2&  Set(const A& a_, const B& b_)    { a = a_; b = b_; return *this; }
	
	template <typename AA, typename BB>
	operator Tuple2<AA, BB>() const           { Tuple2<AA, BB> t; t.a = (AA)a; t.b = (BB)b; return t; }
};

template<typename A, typename B>
inline void AssertMoveable0(Tuple2<A, B> *)
{
	AssertMoveable<A>();
	AssertMoveable<B>();
}

template <typename A, typename B>
inline Tuple2<A, B> Tuple(const A& a, const B& b)
{
	Tuple2<A, B> r;
	r.a = a;
	r.b = b;
	return r;
}

template <typename A, typename B>
inline Tuple2<A, B> MakeTuple(const A& a, const B& b) // Old name
{
	return Tuple(a, b);
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

	Tuple3&  SetA(const A& a_)                { a = a_; return *this; }
	Tuple3&  SetB(const B& b_)                { b = b_; return *this; }
	Tuple3&  SetC(const C& c_)                { c = c_; return *this; }
	Tuple3&  Set(const A& a_, const B& b_, const C& c_)
	                                          { a = a_; b = b_; c = c_; return *this; }

	template <typename AA, typename BB, typename CC>
	operator Tuple3<AA, BB, CC>() const       { Tuple3<AA, BB, CC> t; t.a = (AA)a; t.b = (BB)b; t.c = (CC)c; return t; }
};

template<typename A, typename B, typename C>
inline void AssertMoveable0(Tuple3<A, B, C> *)
{
	AssertMoveable<A>();
	AssertMoveable<B>();
	AssertMoveable<C>();
}

template <typename A, typename B, typename C>
inline Tuple3<A, B, C> Tuple(const A& a, const B& b, const C& c)
{
	Tuple3<A, B, C> r;
	r.a = a;
	r.b = b;
	r.c = c;
	return r;
}

template <typename A, typename B, typename C>
inline Tuple3<A, B, C> MakeTuple(const A& a, const B& b, const C& c)
{
	return Tuple(a, b, c);
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

	Tuple4&  SetA(const A& a_)                { a = a_; return *this; }
	Tuple4&  SetB(const B& b_)                { b = b_; return *this; }
	Tuple4&  SetC(const C& c_)                { c = c_; return *this; }
	Tuple4&  SetD(const D& d_)                { d = d_; return *this; }
	Tuple4&  Set(const A& a_, const B& b_, const C& c_, const D& d_)
	                                          { a = a_; b = b_; c = c_; d = d_; return *this; }

	template <typename AA, typename BB, typename CC, typename DD>
	operator Tuple4<AA, BB, CC, DD>() const    { Tuple4<AA, BB, CC, DD> t; t.a = (AA)a; t.b = (BB)b; t.c = (CC)c; t.d = (DD)d; return t; }
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
inline Tuple4<A, B, C, D> Tuple(const A& a, const B& b, const C& c, const D& d)
{
	Tuple4<A, B, C, D> r;
	r.a = a;
	r.b = b;
	r.c = c;
	r.d = d;
	return r;
}

template <typename A, typename B, typename C, typename D>
inline Tuple4<A, B, C, D> MakeTuple(const A& a, const B& b, const C& c, const D& d)
{
	return Tuple(a, b, c, d);
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

template <typename A, typename B>
struct Tie2 {
	A& a;
	B& b;
	
	void operator=(const Tuple2<A, B>& s)  { a = s.a; b = s.b; }

	Tie2(A& a, B& b) : a(a), b(b) {}
};

template <typename A, typename B>
Tie2<A, B> Tie(A& a, B& b) { return Tie2<A, B>(a, b); }

template <typename A, typename B, typename C>
struct Tie3 {
	A& a;
	B& b;
	C& c;
	
	void operator=(const Tuple3<A, B, C>& s) { a = s.a; b = s.b; c = s.c; }

	Tie3(A& a, B& b, C& c) : a(a), b(b), c(c) {}
};

template <typename A, typename B, typename C>
Tie3<A, B, C> Tie(A& a, B& b, C& c) { return Tie3<A, B, C>(a, b, c); }

template <typename A, typename B, typename C, typename D>
struct Tie4 {
	A& a;
	B& b;
	C& c;
	D& d;
	
	void operator=(const Tuple4<A, B, C, D>& s) { a = s.a; b = s.b; c = s.c; d = s.d; }

	Tie4(A& a, B& b, C& c, D& d) : a(a), b(b), c(c), d(d) {}
};

template <typename A, typename B, typename C, typename D>
Tie4<A, B, C, D> Tie(A& a, B& b, C& c, D& d) { return Tie4<A, B, C, D>(a, b, c, d); }
