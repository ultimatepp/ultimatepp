#ifndef _Dim_Dim5_h_
#define _Dim_Dim5_h_

template <class T> class Dim5;
template <class T, class R> class Dim5y;
template <class T, class R> class Dim5z;
template <class T, class R> class Dim5v;

template <class T, class R>
class Dim5x {
protected:
	typename Dim5<T> *dim;
	int   x;
	friend typename Dim5<T>;
	friend typename Dim5v<T, R>;

public:
	Dim5y<T, R> operator[](int i)       { Dim5y<T, R> h; (Dim5x<T, R>&)h = *this; h.y = i; return h; }
};

template <class T, class R>
class Dim5y : protected Dim5x<T, R> {
protected:
	int   y;
	friend typename Dim5x<T, R>;
	
public:
	Dim5z<T, R> operator[](int i)       { Dim5z<T, R> h; (Dim5y<T, R>&)h = *this; h.z = i; return h; }
};

template <class T, class R>
class Dim5z : protected Dim5y<T, R> {
protected:
	int   z;
	friend typename Dim5y<T, R>;
	
public:
	Dim5v<T, R> operator[](int i)       { Dim5v<T, R> h; (Dim5z<T, R>&)h = *this; h.v = i; return h; }
};

template <class T, class R>
class Dim5v : protected Dim5z<T, R> {
	int   v;
	friend typename Dim5z<T, R>;
	
public:
	R& operator[](int i)                { return dim->At(x, y, z, v, i); }
};

template <class T>
class Dim5 {
	T  *data;
	int cx, cy, cz, cu, cv;
	
	T&  At0(int x, int y, int z, int u, int v) const;

public:
	void Create(int cx, int cy, int cz, int cu, int cv);

	T&       At(int x, int y, int z, int u, int v)                 { return At0(x, y, z, u, v); }
	const T& At(int x, int y, int z, int u, int v) const           { return At0(x, y, z, u, v); }
	T&       operator()(int x, int y, int z, int u, int v)         { return At0(x, y, z, u, v); }
	const T& operator()(int x, int y, int z, int u, int v) const   { return At0(x, y, z, u, v); }
	
	void Clear();
	void Set(const Dim5& src);
	
	Dim5& operator=(const Dim5& src);
	
	Dim5(const Dim5& src)                            { data = NULL; Set(src); }
	Dim5()                                           { data = NULL; }
	Dim5(int cx, int cy, int cz, int cu, int cv)     { data = NULL; Create(cx, cy, cz, cu, cv); }
	~Dim5()                                          { Clear(); }
	
	Dim5x<T, T>       operator[](int i)              { Dim5x<T, T> h; h.dim = this; h.x = i; return h; }
	Dim5x<T, const T> operator[](int i) const        { Dim5x<T, const T> h; h.dim = const_cast<Dim5<T> *>(this); h.x = i; return h; }
};

#endif
