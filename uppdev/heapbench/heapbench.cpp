#include <Core/Core.h>

class A {
	A *a;
	A *b;

public:
	A(int n) {
		if(n) {
			a = new A(n - 1);
			b = new A(n - 1);
		}
		else
			a = b = NULL;
	}
	~A() {
		delete a;
		delete b;
	}
};

CONSOLE_APP_MAIN
{
	A *a[256];
	for(int i = 0; i < 256; i++)
		a[i] = new A(10);
	for (int i = 0; i < 10000; ++i) {
		delete a[i & 255];
		a[i & 255] = new A(10);
	}
	for(int i = 0; i < 256; i++)
		delete a[i];
}
