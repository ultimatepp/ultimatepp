#include <Core/Core.h>

using namespace Upp;

void Check(const Vector<int>& h)
{
	LOG(h);
	ASSERT(h.GetCount() == 1 && h[0] == 12345);
}

typedef const Vector<int>& V;

struct Test : Pte<Test> {
	void Method1(V p1) { Check(p1); }
	void Method2(V p1, V p2) { Check(p1); Check(p2); }
	void Method3(V p1, V p2, V p3) { Check(p1); Check(p2); Check(p3); }
	void Method4(V p1, V p2, V p3, V p4) { Check(p1); Check(p2); Check(p3); Check(p4); }
	void Method5(V p1, V p2, V p3, V p4, V p5) { Check(p1); Check(p2); Check(p3); Check(p4); ; Check(p5); }
};

void Function1(V p1) { Check(p1); }
void Function2(V p1, V p2) { Check(p1); Check(p2); }
void Function3(V p1, V p2, V p3) { Check(p1); Check(p2); Check(p3); }
void Function4(V p1, V p2, V p3, V p4) { Check(p1); Check(p2); Check(p3); Check(p4); }
void Function5(V p1, V p2, V p3, V p4, V p5) { Check(p1); Check(p2); Check(p3); Check(p4); Check(p5); }

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	Vector<int> h;
	h << 12345;
	
	Test x;
	pteback1(&x, &Test::Method1, clone(h))();
	callback1(&x, &Test::Method1, clone(h))();
	callback1(Function1, clone(h))();
	DLOG("---");
	pteback1(&x, &Test::Method2, clone(h))(clone(h));
	callback1(&x, &Test::Method2, clone(h))(clone(h));
	callback1(Function2, clone(h))(clone(h));
	DLOG("---");
	pteback1(&x, &Test::Method3, clone(h))(clone(h), clone(h));
	callback1(&x, &Test::Method3, clone(h))(clone(h), clone(h));
	callback1(Function3, clone(h))(clone(h), clone(h));
	DLOG("---");
	pteback1(&x, &Test::Method4, clone(h))(clone(h), clone(h), clone(h));
	callback1(&x, &Test::Method4, clone(h))(clone(h), clone(h), clone(h));
	callback1(Function4, clone(h))(clone(h), clone(h), clone(h));
	DLOG("---");
	pteback1(&x, &Test::Method5, clone(h))(clone(h), clone(h), clone(h), clone(h));
	callback1(&x, &Test::Method5, clone(h))(clone(h), clone(h), clone(h), clone(h));
	callback1(Function5, clone(h))(clone(h), clone(h), clone(h), clone(h));
	
	LOG("--------------- OK");
}
