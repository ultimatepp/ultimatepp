/*
struct alpha {
	int beta;
};

struct oper2 {
	int oper2m;
};

struct oper3 {
	int oper3m;
};

struct X {
	alpha a;
	
	alpha operator[](int i);
};

struct Y : X{
	X b;
	int a;
	
	struct H {
		int x;
	};
	
	H x;
};

template <class T>
struct Tm {
	template <class V>
	struct H {
		T Test();
		V Test2();
	};
	
	T x;
};


void Test(int a, long b)
{
	X().;
	Y x;
	x.;
	x.a. ; // should not work...
	x.X::a. ;
	x.b.a. ;
	x[]. ;
	x(). ;
	x. ;
	x-> ;
	y. ;
	Tm<X> tm;
	Tm<  X > :: H <alpha > tt;
	tt. ;
	tt.Test(). ;
	tt.Test2(). ;
	X:: ;
	;
	
}

void Foo(volatile ::String)
{
	
}
*/
SqlInsert::operator SqlStatement() const {}

struct String {
	operator std::string() const                           { return std::string(Begin(), End()); }
};