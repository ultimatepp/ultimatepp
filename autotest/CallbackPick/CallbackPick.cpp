#include <Core/Core.h>

using namespace Upp;

void Check(const Vector<int>& h)
{
	ASSERT(h.GetCount() == 1 && h[0] == 12345);
}

typedef const Vector<int>& V;

struct Test : Pte<Test> {
	void Method() {}
	void Method1(V p1) { Check(p1); }
	void Method2(V p1, V p2) { Check(p1); Check(p2); }
	void Method3(V p1, V p2, V p3) { Check(p1); Check(p2); Check(p3); }
	void Method4(V p1, V p2, V p3, V p4) { Check(p1); Check(p2); Check(p3); Check(p4); }
	void Method5(V p1, V p2, V p3, V p4, V p5) { Check(p1); Check(p2); Check(p3); Check(p4); ; Check(p5); }

	bool Bool() { return false; }
	bool Bool1(V p1) { Check(p1); return false; }
	bool Bool2(V p1, V p2) { Check(p1); Check(p2); return false; }
	bool Bool3(V p1, V p2, V p3) { Check(p1); Check(p2); Check(p3); return false; }
	bool Bool4(V p1, V p2, V p3, V p4) { Check(p1); Check(p2); Check(p3); Check(p4); return false; }
	bool Bool5(V p1, V p2, V p3, V p4, V p5) { Check(p1); Check(p2); Check(p3); Check(p4); ; Check(p5); return false; }

	void MethodC() {}
	void Method1C(V p1) const { Check(p1); }
	void Method2C(V p1, V p2) const { Check(p1); Check(p2); }
	void Method3C(V p1, V p2, V p3) const { Check(p1); Check(p2); Check(p3); }
	void Method4C(V p1, V p2, V p3, V p4) const { Check(p1); Check(p2); Check(p3); Check(p4); }
	void Method5C(V p1, V p2, V p3, V p4, V p5) const { Check(p1); Check(p2); Check(p3); Check(p4); ; Check(p5); }

	bool BoolC() { return false; }
	bool Bool1C(V p1) const { Check(p1); return false; }
	bool Bool2C(V p1, V p2) const { Check(p1); Check(p2); return false; }
	bool Bool3C(V p1, V p2, V p3) const { Check(p1); Check(p2); Check(p3); return false; }
	bool Bool4C(V p1, V p2, V p3, V p4) const { Check(p1); Check(p2); Check(p3); Check(p4); return false; }
	bool Bool5C(V p1, V p2, V p3, V p4, V p5) const { Check(p1); Check(p2); Check(p3); Check(p4); ; Check(p5); return false; }
};

void Function0() {}
void Function1(V p1) { Check(p1); }
void Function2(V p1, V p2) { Check(p1); Check(p2); }
void Function3(V p1, V p2, V p3) { Check(p1); Check(p2); Check(p3); }
void Function4(V p1, V p2, V p3, V p4) { Check(p1); Check(p2); Check(p3); Check(p4); }
void Function5(V p1, V p2, V p3, V p4, V p5) { Check(p1); Check(p2); Check(p3); Check(p4); Check(p5); }

bool BoolFn() { return false; }
bool BoolFn1(V p1) { Check(p1); return false; }
bool BoolFn2(V p1, V p2) { Check(p1); Check(p2); return false; }
bool BoolFn3(V p1, V p2, V p3) { Check(p1); Check(p2); Check(p3); return false; }
bool BoolFn4(V p1, V p2, V p3, V p4) { Check(p1); Check(p2); Check(p3); Check(p4); return false; }
bool BoolFn5(V p1, V p2, V p3, V p4, V p5) { Check(p1); Check(p2); Check(p3); Check(p4); Check(p5); return false; }

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	Vector<int> h;
	h << 12345;
		
	Test x;
	
	// No Args callbacks

	pteback(&x, &Test::Method)();
	callback(&x, &Test::Method)();
	pteback(&x, &Test::MethodC)();
	callback(&x, &Test::MethodC)();
	callback(Function0)();
	
	pteback(&x, &Test::Method1)(clone(h));
	callback(&x, &Test::Method1)(clone(h));
	pteback(&x, &Test::Method1C)(clone(h));
	callback(&x, &Test::Method1C)(clone(h));
	callback(Function1)(clone(h));
	
	pteback(&x, &Test::Method2)(clone(h), clone(h));
	callback(&x, &Test::Method2)(clone(h), clone(h));
	pteback(&x, &Test::Method2C)(clone(h), clone(h));
	callback(&x, &Test::Method2C)(clone(h), clone(h));
	callback(Function2)(clone(h), clone(h));
	
	pteback(&x, &Test::Method3)(clone(h), clone(h), clone(h));
	callback(&x, &Test::Method3)(clone(h), clone(h), clone(h));
	pteback(&x, &Test::Method3C)(clone(h), clone(h), clone(h));
	callback(&x, &Test::Method3C)(clone(h), clone(h), clone(h));
	callback(Function3)(clone(h), clone(h), clone(h));
	
	pteback(&x, &Test::Method4)(clone(h), clone(h), clone(h), clone(h));
	callback(&x, &Test::Method4)(clone(h), clone(h), clone(h), clone(h));
	pteback(&x, &Test::Method4C)(clone(h), clone(h), clone(h), clone(h));
	callback(&x, &Test::Method4C)(clone(h), clone(h), clone(h), clone(h));
	callback(Function4)(clone(h), clone(h), clone(h), clone(h));
	
	// No Args gates
	
	pteback(&x, &Test::Bool)();
	callback(&x, &Test::Bool)();
	pteback(&x, &Test::BoolC)();
	callback(&x, &Test::BoolC)();
	callback(BoolFn)();

	pteback(&x, &Test::Bool1)(clone(h));
	callback(&x, &Test::Bool1)(clone(h));
	pteback(&x, &Test::Bool1C)(clone(h));
	callback(&x, &Test::Bool1C)(clone(h));
	callback(BoolFn1)(clone(h));
	
	pteback(&x, &Test::Bool2)(clone(h), clone(h));
	callback(&x, &Test::Bool2)(clone(h), clone(h));
	pteback(&x, &Test::Bool2C)(clone(h), clone(h));
	callback(&x, &Test::Bool2C)(clone(h), clone(h));
	callback(BoolFn2)(clone(h), clone(h));
	
	pteback(&x, &Test::Bool3)(clone(h), clone(h), clone(h));
	callback(&x, &Test::Bool3)(clone(h), clone(h), clone(h));
	pteback(&x, &Test::Bool3C)(clone(h), clone(h), clone(h));
	callback(&x, &Test::Bool3C)(clone(h), clone(h), clone(h));
	callback(BoolFn3)(clone(h), clone(h), clone(h));
	
	pteback(&x, &Test::Bool4)(clone(h), clone(h), clone(h), clone(h));
	callback(&x, &Test::Bool4)(clone(h), clone(h), clone(h), clone(h));
	pteback(&x, &Test::Bool4C)(clone(h), clone(h), clone(h), clone(h));
	callback(&x, &Test::Bool4C)(clone(h), clone(h), clone(h), clone(h));
	callback(BoolFn4)(clone(h), clone(h), clone(h), clone(h));
	
	// Arg1 callbacks - does not work anymore
/*	
	pteback1(&x, &Test::Method1, clone(h))();
	callback1(&x, &Test::Method1, clone(h))();
	pteback1(&x, &Test::Method1C, clone(h))();
	callback1(&x, &Test::Method1C, clone(h))();
	callback1(Function1, clone(h))();
	
	pteback1(&x, &Test::Method2, clone(h))(clone(h));
	callback1(&x, &Test::Method2, clone(h))(clone(h));
	pteback1(&x, &Test::Method2C, clone(h))(clone(h));
	callback1(&x, &Test::Method2C, clone(h))(clone(h));
	callback1(Function2, clone(h))(clone(h));
	
	pteback1(&x, &Test::Method3, clone(h))(clone(h), clone(h));
	callback1(&x, &Test::Method3, clone(h))(clone(h), clone(h));
	pteback1(&x, &Test::Method3C, clone(h))(clone(h), clone(h));
	callback1(&x, &Test::Method3C, clone(h))(clone(h), clone(h));
	callback1(Function3, clone(h))(clone(h), clone(h));
	
	pteback1(&x, &Test::Method4, clone(h))(clone(h), clone(h), clone(h));
	callback1(&x, &Test::Method4, clone(h))(clone(h), clone(h), clone(h));
	pteback1(&x, &Test::Method4C, clone(h))(clone(h), clone(h), clone(h));
	callback1(&x, &Test::Method4C, clone(h))(clone(h), clone(h), clone(h));
	callback1(Function4, clone(h))(clone(h), clone(h), clone(h));
	
	pteback1(&x, &Test::Method5, clone(h))(clone(h), clone(h), clone(h), clone(h));
	callback1(&x, &Test::Method5, clone(h))(clone(h), clone(h), clone(h), clone(h));
	pteback1(&x, &Test::Method5C, clone(h))(clone(h), clone(h), clone(h), clone(h));
	callback1(&x, &Test::Method5C, clone(h))(clone(h), clone(h), clone(h), clone(h));
	callback1(Function5, clone(h))(clone(h), clone(h), clone(h), clone(h));

	// Arg1 gates
	
	pteback1(&x, &Test::Bool1, clone(h))();
	callback1(&x, &Test::Bool1, clone(h))();
	pteback1(&x, &Test::Bool1C, clone(h))();
	callback1(&x, &Test::Bool1C, clone(h))();
	callback1(BoolFn1, clone(h))();
	
	pteback1(&x, &Test::Bool2, clone(h))(clone(h));
	callback1(&x, &Test::Bool2, clone(h))(clone(h));
	pteback1(&x, &Test::Bool2C, clone(h))(clone(h));
	callback1(&x, &Test::Bool2C, clone(h))(clone(h));
	callback1(BoolFn2, clone(h))(clone(h));
	
	pteback1(&x, &Test::Bool3, clone(h))(clone(h), clone(h));
	callback1(&x, &Test::Bool3, clone(h))(clone(h), clone(h));
	pteback1(&x, &Test::Bool3C, clone(h))(clone(h), clone(h));
	callback1(&x, &Test::Bool3C, clone(h))(clone(h), clone(h));
	callback1(BoolFn3, clone(h))(clone(h), clone(h));
	
	pteback1(&x, &Test::Bool4, clone(h))(clone(h), clone(h), clone(h));
	callback1(&x, &Test::Bool4, clone(h))(clone(h), clone(h), clone(h));
	pteback1(&x, &Test::Bool4C, clone(h))(clone(h), clone(h), clone(h));
	callback1(&x, &Test::Bool4C, clone(h))(clone(h), clone(h), clone(h));
	callback1(BoolFn4, clone(h))(clone(h), clone(h), clone(h));
	
	pteback1(&x, &Test::Bool5, clone(h))(clone(h), clone(h), clone(h), clone(h));
	callback1(&x, &Test::Bool5, clone(h))(clone(h), clone(h), clone(h), clone(h));
	pteback1(&x, &Test::Bool5C, clone(h))(clone(h), clone(h), clone(h), clone(h));
	callback1(&x, &Test::Bool5C, clone(h))(clone(h), clone(h), clone(h), clone(h));
	callback1(BoolFn5, clone(h))(clone(h), clone(h), clone(h), clone(h));
	
	// Arg2 callbacks
	
	pteback2(&x, &Test::Method2, clone(h), clone(h))();
	callback2(&x, &Test::Method2, clone(h), clone(h))();
	pteback2(&x, &Test::Method2C, clone(h), clone(h))();
	callback2(&x, &Test::Method2C, clone(h), clone(h))();
	callback2(Function2, clone(h), clone(h))();
	
	pteback2(&x, &Test::Method3, clone(h), clone(h))(clone(h));
	callback2(&x, &Test::Method3, clone(h), clone(h))(clone(h));
	pteback2(&x, &Test::Method3C, clone(h), clone(h))(clone(h));
	callback2(&x, &Test::Method3C, clone(h), clone(h))(clone(h));
	callback2(Function3, clone(h), clone(h))(clone(h));
	
	pteback2(&x, &Test::Method4, clone(h), clone(h))(clone(h), clone(h));
	callback2(&x, &Test::Method4, clone(h), clone(h))(clone(h), clone(h));
	pteback2(&x, &Test::Method4C, clone(h), clone(h))(clone(h), clone(h));
	callback2(&x, &Test::Method4C, clone(h), clone(h))(clone(h), clone(h));
	callback2(Function4, clone(h), clone(h))(clone(h), clone(h));
	
	pteback2(&x, &Test::Method5, clone(h), clone(h))(clone(h), clone(h), clone(h));
	callback2(&x, &Test::Method5, clone(h), clone(h))(clone(h), clone(h), clone(h));
	pteback2(&x, &Test::Method5C, clone(h), clone(h))(clone(h), clone(h), clone(h));
	callback2(&x, &Test::Method5C, clone(h), clone(h))(clone(h), clone(h), clone(h));
	callback2(Function5, clone(h), clone(h))(clone(h), clone(h), clone(h));

	// Arg2 gates
	
	pteback2(&x, &Test::Bool2, clone(h), clone(h))();
	callback2(&x, &Test::Bool2, clone(h), clone(h))();
	pteback2(&x, &Test::Bool2C, clone(h), clone(h))();
	callback2(&x, &Test::Bool2C, clone(h), clone(h))();
	callback2(BoolFn2, clone(h), clone(h))();
	
	pteback2(&x, &Test::Bool3, clone(h), clone(h))(clone(h));
	callback2(&x, &Test::Bool3, clone(h), clone(h))(clone(h));
	pteback2(&x, &Test::Bool3C, clone(h), clone(h))(clone(h));
	callback2(&x, &Test::Bool3C, clone(h), clone(h))(clone(h));
	callback2(BoolFn3, clone(h), clone(h))(clone(h));
	
	pteback2(&x, &Test::Bool4, clone(h), clone(h))(clone(h), clone(h));
	callback2(&x, &Test::Bool4, clone(h), clone(h))(clone(h), clone(h));
	pteback2(&x, &Test::Bool4C, clone(h), clone(h))(clone(h), clone(h));
	callback2(&x, &Test::Bool4C, clone(h), clone(h))(clone(h), clone(h));
	callback2(BoolFn4, clone(h), clone(h))(clone(h), clone(h));
	
	pteback2(&x, &Test::Bool5, clone(h), clone(h))(clone(h), clone(h), clone(h));
	callback2(&x, &Test::Bool5, clone(h), clone(h))(clone(h), clone(h), clone(h));
	pteback2(&x, &Test::Bool5C, clone(h), clone(h))(clone(h), clone(h), clone(h));
	callback2(&x, &Test::Bool5C, clone(h), clone(h))(clone(h), clone(h), clone(h));
	callback2(BoolFn5, clone(h), clone(h))(clone(h), clone(h), clone(h));

	// Arg3 callbacks
	
	pteback3(&x, &Test::Method3, clone(h), clone(h), clone(h))();
	callback3(&x, &Test::Method3, clone(h), clone(h), clone(h))();
	pteback3(&x, &Test::Method3C, clone(h), clone(h), clone(h))();
	callback3(&x, &Test::Method3C, clone(h), clone(h), clone(h))();
	callback3(Function3, clone(h), clone(h), clone(h))();
	
	pteback3(&x, &Test::Method4, clone(h), clone(h), clone(h))(clone(h));
	callback3(&x, &Test::Method4, clone(h), clone(h), clone(h))(clone(h));
	pteback3(&x, &Test::Method4C, clone(h), clone(h), clone(h))(clone(h));
	callback3(&x, &Test::Method4C, clone(h), clone(h), clone(h))(clone(h));
	callback3(Function4, clone(h), clone(h), clone(h))(clone(h));
	
	pteback3(&x, &Test::Method5, clone(h), clone(h), clone(h))(clone(h), clone(h));
	callback3(&x, &Test::Method5, clone(h), clone(h), clone(h))(clone(h), clone(h));
	pteback3(&x, &Test::Method5C, clone(h), clone(h), clone(h))(clone(h), clone(h));
	callback3(&x, &Test::Method5C, clone(h), clone(h), clone(h))(clone(h), clone(h));
	callback3(Function5, clone(h), clone(h), clone(h))(clone(h), clone(h));

	// Arg3 gates
	
	pteback3(&x, &Test::Bool3, clone(h), clone(h), clone(h))();
	callback3(&x, &Test::Bool3, clone(h), clone(h), clone(h))();
	pteback3(&x, &Test::Bool3C, clone(h), clone(h), clone(h))();
	callback3(&x, &Test::Bool3C, clone(h), clone(h), clone(h))();
	callback3(BoolFn3, clone(h), clone(h), clone(h))();
	
	pteback3(&x, &Test::Bool4, clone(h), clone(h), clone(h))(clone(h));
	callback3(&x, &Test::Bool4, clone(h), clone(h), clone(h))(clone(h));
	pteback3(&x, &Test::Bool4C, clone(h), clone(h), clone(h))(clone(h));
	callback3(&x, &Test::Bool4C, clone(h), clone(h), clone(h))(clone(h));
	callback3(BoolFn4, clone(h), clone(h), clone(h))(clone(h));
	
	pteback3(&x, &Test::Bool5, clone(h), clone(h), clone(h))(clone(h), clone(h));
	callback3(&x, &Test::Bool5, clone(h), clone(h), clone(h))(clone(h), clone(h));
	pteback3(&x, &Test::Bool5C, clone(h), clone(h), clone(h))(clone(h), clone(h));
	callback3(&x, &Test::Bool5C, clone(h), clone(h), clone(h))(clone(h), clone(h));
	callback3(BoolFn5, clone(h), clone(h), clone(h))(clone(h), clone(h));

	// Arg4 callbacks
	
	pteback4(&x, &Test::Method4, clone(h), clone(h), clone(h), clone(h))();
	callback4(&x, &Test::Method4, clone(h), clone(h), clone(h), clone(h))();
	pteback4(&x, &Test::Method4C, clone(h), clone(h), clone(h), clone(h))();
	callback4(&x, &Test::Method4C, clone(h), clone(h), clone(h), clone(h))();
	callback4(Function4, clone(h), clone(h), clone(h), clone(h))();
	
	pteback4(&x, &Test::Method5, clone(h), clone(h), clone(h), clone(h))(clone(h));
	callback4(&x, &Test::Method5, clone(h), clone(h), clone(h), clone(h))(clone(h));
	pteback4(&x, &Test::Method5C, clone(h), clone(h), clone(h), clone(h))(clone(h));
	callback4(&x, &Test::Method5C, clone(h), clone(h), clone(h), clone(h))(clone(h));
	callback4(Function5, clone(h), clone(h), clone(h), clone(h))(clone(h));

	// Arg4 gates
	
	pteback4(&x, &Test::Bool4, clone(h), clone(h), clone(h), clone(h))();
	callback4(&x, &Test::Bool4, clone(h), clone(h), clone(h), clone(h))();
	pteback4(&x, &Test::Bool4C, clone(h), clone(h), clone(h), clone(h))();
	callback4(&x, &Test::Bool4C, clone(h), clone(h), clone(h), clone(h))();
	callback4(BoolFn4, clone(h), clone(h), clone(h), clone(h))();
	
	pteback4(&x, &Test::Bool5, clone(h), clone(h), clone(h), clone(h))(clone(h));
	callback4(&x, &Test::Bool5, clone(h), clone(h), clone(h), clone(h))(clone(h));
	pteback4(&x, &Test::Bool5C, clone(h), clone(h), clone(h), clone(h))(clone(h));
	callback4(&x, &Test::Bool5C, clone(h), clone(h), clone(h), clone(h))(clone(h));
	callback4(BoolFn5, clone(h), clone(h), clone(h), clone(h))(clone(h));
	
	// Arg5 callbacks
	
	pteback5(&x, &Test::Method5, clone(h), clone(h), clone(h), clone(h), clone(h))();
	callback5(&x, &Test::Method5, clone(h), clone(h), clone(h), clone(h), clone(h))();
	pteback5(&x, &Test::Method5C, clone(h), clone(h), clone(h), clone(h), clone(h))();
	callback5(&x, &Test::Method5C, clone(h), clone(h), clone(h), clone(h), clone(h))();
	callback5(Function5, clone(h), clone(h), clone(h), clone(h), clone(h))();
	
	// Arg5 gates
	
	pteback5(&x, &Test::Bool5, clone(h), clone(h), clone(h), clone(h), clone(h))();
	callback5(&x, &Test::Bool5, clone(h), clone(h), clone(h), clone(h), clone(h))();
	pteback5(&x, &Test::Bool5C, clone(h), clone(h), clone(h), clone(h), clone(h))();
	callback5(&x, &Test::Bool5C, clone(h), clone(h), clone(h), clone(h), clone(h))();
	callback5(BoolFn5, clone(h), clone(h), clone(h), clone(h), clone(h))();
*/	
	LOG("--------------- OK");
}
