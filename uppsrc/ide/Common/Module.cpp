#include "Common.h"

static Vector<IdeModule *>& sM()
{
	static Vector<IdeModule *> m;
	return m;
}

void RegisterIdeModule(IdeModule& module)
{
	sM().Add(&module);
}

int  GetIdeModuleCount()
{
	return sM().GetCount();
}

IdeModule& GetIdeModule(int q)
{
	return *sM()[q];
}
