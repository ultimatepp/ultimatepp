#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

//$-template <class T> inline constexpr const T& min(const T& a, const T& b, ...);
//$-template <class T> inline constexpr const T& max(const T& a, const T& b, ...);

template <class T>
constexpr const T& min(const T& a, const T& b)
{
	return a < b ? a : b;
}

template <class T, typename... Args>
constexpr const T& min(const T& a, const T& b, const Args& ...args)
{
	return min(a, min(b, args...));
}

template <class T>
constexpr const T& max(const T& a, const T& b)
{
	return a > b ? a : b;
}

template <class T, typename... Args>
constexpr const T& max(const T& a, const T& b, const Args& ...args)
{
	return max(a, max(b, args...));
}

//$+

template <class T>
constexpr const T& clamp(const T& x, const T& min_, const T& max_)
{
	return min(max(x, min_), max_);
}

//$-constexpr int findarg(const T& x, const T1& p0, ...);

template <class T, class K>
constexpr int findarg(const T& x, const K& k)
{
	return x == k ? 0 : -1;
}

template <class T, class K, typename... L>
constexpr int findarg(const T& sel, const K& k, const L& ...args)
{
	if(sel == k)
		return 0;
	int q = findarg(sel, args...);
	return q >= 0 ? q + 1 : -1;
}

//$-constexpr auto decode(const T& x, const T1& p0, const V1& v0, ...);

template <class T, class V>
constexpr const V& decode(const T& sel, const V& def)
{
	return def;
}

template <class T, class K, class V, typename... L>
constexpr V decode(const T& sel, const K& k, const V& v, const L& ...args)
{
	return sel == k ? v : (V)decode(sel, args...);
}

template <class T>
constexpr const char *decode(const T& sel, const char *def)
{
	return def;
}

template <class T, class K, typename... L>
constexpr const char *decode(const T& sel, const K& k, const char *v, const L& ...args)
{
	return sel == k ? v : (const char *)decode(sel, args...);
}

//$-constexpr T get_i(int i, const T& p0, const T1& p1, ...);

template <typename A, typename... T>
constexpr A get_i(int i, const A& p0, const T& ...args)
{
	A list[] = { p0, args... };
	return list[clamp(i, 0, (int)sizeof...(args))];
}

template <typename P, typename... T>
constexpr const P *get_i(int i, const P* p0, const T& ...args)
{
	const P *list[] = { p0, args... };
	return list[clamp(i, 0, (int)sizeof...(args))];
}

//$-void foreach_arg(F fn, const T& p0, const T1& p1, ...);

template <class F, class V>
void foreach_arg(F fn, V&& v)
{
	fn(std::forward<V>(v));
}

template <class F, class V, typename... Args>
void foreach_arg(F fn, V&& v, Args&& ...args)
{
	fn(std::forward<V>(v));
	foreach_arg(fn, std::forward<Args>(args)...);
}

//$+

template <class I, typename... Args>
void iter_set(I t, Args&& ...args)
{
	foreach_arg([&](auto&& v) { *t++ = std::forward<decltype(v)>(v); }, std::forward<Args>(args)...);
}

template <class I, typename... Args>
void iter_get(I s, Args& ...args)
{
	foreach_arg([&](auto& v) { v = *s++; }, args...);
}

template <class C, typename... Args>
C gather(Args&& ...args)
{
	C x(sizeof...(args));
	iter_set(x.begin(), std::forward<Args>(args)...);
	return x;
}

template <class C, typename... Args>
int scatter_n(int n, const C& c, Args& ...args)
{
	if(n < sizeof...(args))
		return 0;
	iter_get(c.begin(), args...);
	return sizeof...(args);
}

template <class C, typename... Args>
int scatter(const C& c, Args& ...args)
{
	return scatter_n(c.size(), c, args...);
}

// DEPRECATED

template <class T> // deprecated name, use clamp
inline T minmax(T x, T _min, T _max)                           { return min(max(x, _min), _max); }
