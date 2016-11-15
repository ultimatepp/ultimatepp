#include <Core/Core.h>

using namespace Upp;

struct Test {
	int sum;

	Vector<Event<>> cb;
	
	void Method1() {
		sum++;
	}
	
	void Method2(int d) {
		sum += d;
	}
	
	typedef Test CLASSNAME;
	
	Test() {
		const int N = 1000000;
		for(int j = 0; j < 50; j++) {
			cb.Clear();
			for(int i = 0; i < N; i++) {
				RTIMING("Adding Method1");
				cb.Add([=] { Method1(); });
			}
			sum = 0;
			for(int i = 0; i < cb.GetCount(); i++) {
				RTIMING("Calling Method1");
				cb[i]();
			}
			RDUMP(sum);
			cb.Clear();
			for(int i = 0; i < N; i++) {
				RTIMING("Adding Method2");
				cb.Add([=] { Method2(i); });
			}
			sum = 0;
			for(int i = 0; i < cb.GetCount(); i++) {
				RTIMING("Calling Method2");
				cb[i]();
			}
			RDUMP(sum);
		}
	}
};

CONSOLE_APP_MAIN
{
	Test();
}
