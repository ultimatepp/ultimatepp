template <int N, typename... T>
struct TupleN;

template <int I>
struct IndexI__ {};

template <typename V, typename T, typename I>
const V& GetFromTuple(const T& t, const I&);

template <typename... T>
struct Tuple;

template <typename A>
struct TupleN<1, A>
{
	A a;
	
	using T1 = A;

	bool  operator==(const TupleN& x) const   { return a == x.a; }
	int   Compare(const TupleN& x) const      { return SgnCompare(a, x.a); }

	void  ToHash(CombineHash& h) const        { h << a; }
	void  ToString(String& r) const           { r << AsString(a); }
	
	void  Serialize(Stream& s)                { s % a; }
	
	int   GetCount() const                    { return 1; }

	Value Get(int i) const                    { ASSERT(i == 0); return a; }
	void  Set(int i, const Value& v)          { ASSERT(i == 0); a = v; }

	TupleN(const A& a) : a(a) {}
	TupleN() {}
	
	template <typename AA>
	operator Tuple<AA>()                      { Tuple<AA> t; t.a = (AA)a; return t; }
};

#define TUPLE_N_METHODS(M, I) \
	bool operator==(const TupleN& x) const    { return Base::operator==(x) && M == x.M; } \
	int  Compare(const TupleN& x) const       { int q = Base::Compare(x); return q ? q : SgnCompare(M, x.M); } \
 \
	void ToHash(CombineHash& h) const         { Base::ToHash(h); h << M; } \
	void ToString(String& r) const            { Base::ToString(r); r << ", " << M; } \
	 \
	void Serialize(Stream& s)                 { Base::Serialize(s); s % M; } \
	 \
	int   GetCount() const                    { return I + 1; } \
 \
	Value Get(int i) const                    { if(i == I) return M; return Base::Get(i); } \
	void  Set(int i, const Value& v)          { if(i == I) M = v; else Base::Set(i, v); } \
 \
	TupleN() {} \


template <typename A, typename B>
struct TupleN<2, A, B> : public TupleN<1, A> {
	typedef TupleN<1, A> Base;
	B b;

	using T2 = B;
	
	TUPLE_N_METHODS(b, 1);

	TupleN(const A& a, const B& b) : Base(a), b(b) {}

	template <typename AA, typename BB>
	operator Tuple<AA, BB>()                  { Tuple<AA, BB> t; t.a = (AA)Base::a; t.b = b; return t; }
};

template <typename A, typename B, typename C>
struct TupleN<3, A, B, C> : public TupleN<2, A, B>
{
	typedef TupleN<2, A, B> Base;
	C c;

	using T3 = C;

	TUPLE_N_METHODS(c, 2);

	TupleN(const A& a, const B& b, const C& c) : Base(a, b), c(c) {}

	template <typename AA, typename BB, typename CC>
	operator Tuple<AA, BB, CC>() { Tuple<AA, BB, CC> t; t.a = (AA)Base::a; t.b = (BB)Base::b; t.c = (CC)c; return t; }
};

template <typename A, typename B, typename C, typename D>
struct TupleN<4, A, B, C, D> : public TupleN<3, A, B, C>
{
	typedef TupleN<3, A, B, C> Base;
	D d;

	using T4 = D;

	TUPLE_N_METHODS(d, 3);
	
	TupleN(const A& a, const B& b, const C& c, const D& d) : Base(a, b, c), d(d) {}

	template <typename AA, typename BB, typename CC, typename DD>
	operator Tuple<AA, BB, CC, DD>() { Tuple<AA, BB, CC, DD> t; t.a = (AA)Base::a; t.b = (BB)Base::b; t.c = (CC)Base::c; t.d = (DD)d; return t; }
};


#define GET_FROM_TUPLE(M, I) \
 \
template <typename T> \
auto GetFromTuple(const T& t, const IndexI__<I>&) -> decltype(t.M)& \
{ \
	return const_cast<T&>(t).M; \
} \
 \
template <typename T> \
auto GetFromTupleByType(const T& t, decltype(t.M)*, const IndexI__<I>* = NULL) -> decltype(t.M)& \
{ \
	return const_cast<T&>(t).M; \
}

GET_FROM_TUPLE(a, 0)
GET_FROM_TUPLE(b, 1)
GET_FROM_TUPLE(c, 2)
GET_FROM_TUPLE(d, 3)

template <typename... Args>
struct Tuple : public TupleN<sizeof...(Args), Args...> {
private:
	typedef TupleN<sizeof...(Args), Args...> Base;

public:
	template <int I>
	const auto& Get() const { return GetFromTuple(*this, IndexI__<I>()); }
	template <int I>
	auto& Get() { return GetFromTuple(*this, IndexI__<I>()); }

	template <int I> // std compatibility & C++17 structured binding support
	const auto& get() const { return GetFromTuple(*this, IndexI__<I>()); }
	template <int I> // std compatibility & C++17 structured binding support
	auto& get() { return GetFromTuple(*this, IndexI__<I>()); }
	
	template <typename T> const T& Get() const { return GetFromTupleByType(*this, (T*)NULL); }
	template <typename T> T& Get()             { return GetFromTupleByType(*this, (T*)NULL); }

	int  GetCount() const                      { return Base::GetCount(); }

	bool operator==(const Tuple& x) const     { return Base::operator==(x); }
	bool operator!=(const Tuple& x) const     { return !operator==(x); }
	
	int  Compare(const Tuple& x) const        { return Base::Compare(x); }
	bool operator<=(const Tuple& x) const     { return Compare(x) <= 0; }
	bool operator>=(const Tuple& x) const     { return Compare(x) >= 0; }
	bool operator<(const Tuple& x) const      { return Compare(x) < 0; }
	bool operator>(const Tuple& x) const      { return Compare(x) > 0; }

	hash_t GetHashValue() const               { CombineHash h; Base::ToHash(h); return h; }
	
	void Serialize(Stream& s)                 { Base::Serialize(s); }

	String ToString() const                   { String h = "("; Base::ToString(h); h << ")"; return h; }
	
	Value Get(int i) const                    { return Base::Get(i); }
	void  Set(int i, const Value& v)          { return Base::Set(i, v); }
	ValueArray GetArray() const               { ValueArray va; for(int i = 0; i < GetCount(); i++) va.Add(Get(i)); return va; }
	void  SetArray(const ValueArray& va)      { for(int i = 0; i < va.GetCount(); i++) Set(i, va[i]); }
	
	Tuple() {}
	Tuple(const Args... args) : Base(args...) {};
};

template <typename A, typename B>
inline constexpr bool is_trivially_relocatable<Tuple<A, B>> = is_trivially_relocatable<A> &&
                                                              is_trivially_relocatable<B>;

template <typename A, typename B, typename C>
inline constexpr bool is_trivially_relocatable<Tuple<A, B, C>> = is_trivially_relocatable<A> &&
                                                                 is_trivially_relocatable<B> &&
                                                                 is_trivially_relocatable<C>;

template <typename A, typename B, typename C, typename D>
inline constexpr bool is_trivially_relocatable<Tuple<A, B, C, D>> = is_trivially_relocatable<A> &&
                                                                    is_trivially_relocatable<B> &&
                                                                    is_trivially_relocatable<C> &&
                                                                    is_trivially_relocatable<D>;

template <typename... Args>
Tuple<Args...> MakeTuple(const Args... args) {
	return Tuple<Args...>(args...);
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
	
	void operator=(const Tuple<A, B>& s)  { a = s.a; b = s.b; }

	Tie2(A& a, B& b) : a(a), b(b) {}
};

template <typename A, typename B>
Tie2<A, B> Tie(A& a, B& b) { return Tie2<A, B>(a, b); }

template <typename A, typename B, typename C>
struct Tie3 {
	A& a;
	B& b;
	C& c;
	
	void operator=(const Tuple<A, B, C>& s) { a = s.a; b = s.b; c = s.c; }

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
	
	void operator=(const Tuple<A, B, C, D>& s) { a = s.a; b = s.b; c = s.c; d = s.d; }

	Tie4(A& a, B& b, C& c, D& d) : a(a), b(b), c(c), d(d) {}
};

template <typename A, typename B, typename C, typename D>
Tie4<A, B, C, D> Tie(A& a, B& b, C& c, D& d) { return Tie4<A, B, C, D>(a, b, c, d); }

}; // end Upp namespace

template<typename... Args>
	struct std::tuple_element<0, Upp::Tuple<Args...>> {
		using type = typename Upp::Tuple<Args...>::T1;
    };

template<typename... Args>
	struct std::tuple_element<1, Upp::Tuple<Args...>> {
		using type = typename Upp::Tuple<Args...>::T2;
	};

template<typename... Args>
	struct std::tuple_element<2, Upp::Tuple<Args...>> {
		using type = typename Upp::Tuple<Args...>::T3;
	};

template<typename... Args>
	struct std::tuple_element<3, Upp::Tuple<Args...>> {
		using type = typename Upp::Tuple<Args...>::T4;
	};

template<typename... Args>
	struct std::tuple_size<Upp::Tuple<Args...>> {
		static const int value = sizeof...(Args);
	};

namespace Upp {

// Backward compatibility

template <typename A, typename B> using Tuple2 = Tuple<A, B>;
template <typename A, typename B, typename C> using Tuple3 = Tuple<A, B, C>;
template <typename A, typename B, typename C, typename D> using Tuple4 = Tuple<A, B, C, D>;
