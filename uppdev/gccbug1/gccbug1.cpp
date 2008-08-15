#include <iostream>

struct Base {
	int x;
	int y;
};

struct Derived : Base {};
/*
struct Foo {
	int a;
	int b;

	operator Bar() const;
};

struct Bar : Foo {
};

Foo::operator Bar() const { return *(Bar *)this; }
*/
void Do(Base& a)
{
	Derived b = *(Derived *)&a;
	std::cout << b.x << ", " << b.y << '\n';
}

int main(int argc, const char **argv)
{
	Derived f;
	f.x = 12345;
	f.y = 54321;
	Do(f);

	return 0;
}
