#include <stdio.h>

struct A
{
	virtual void afn() { puts("A::afn"); }
	// A must have vtbl to make this crash
};

struct B
{
	virtual void bfn() { puts("B::bfn"); }
};

struct C : A, B
{
	virtual void bfn() { puts("C::B::bfn"); }
};

void Bcall(B& b)
{
	b.bfn();
}

int main()
{
	C c;
	puts("calling C::B::bfn crashes...");
	Bcall(c);
	return 0;
}
