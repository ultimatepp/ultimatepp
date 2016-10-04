#include <Core/Core.h>

using namespace Upp;

class AAA : public VectorMap<int, int>, public MoveableAndDeepCopyOption<AAA>
{
public:
	AAA() {}
	AAA(const AAA &, int) {}
	unsigned GetHashValue() const {return 0;/*proxy here*/;}
};

CONSOLE_APP_MAIN
{
	AAA a;
	a.Add(10,1);
	AAA a2(a);
	Cout() << a2.GetCount();
	
	Index<AAA> ai;
	ai.Add(a2);
	Cout() << " / " << ai[0].GetCount();
}