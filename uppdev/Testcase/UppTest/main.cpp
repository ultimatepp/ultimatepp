#include "UppTest.h"

#define DLIMODULE   Test
#define DLIHEADER   <UppTest/UppTest.dli>
#include <Core/dli.h>

#if 0
INITBLOCK {
	Test().SetLibName("./libtest.so");
	if (Test() && !Test().IsInitted()) {
		Test().Init();
		LOG("Library Loaded");
	}
}

EXITBLOCK {
	if (Test() && Test().IsInitted()) {
		Test().UnInit();
		Test().Free();
		LOG("Library unloaded");
	}
}
#endif

UppTest::UppTest()
{
	CtrlLayout(*this, "Upp Test");
	#if 1
	Test().SetLibName("./libtest.so");
	if (Test() && !Test().IsInitted()) {
		Test().Init();
		LOG("Library Loaded");
	}
	#endif
}

UppTest::~UppTest()
{
	#if 1
	if (Test() && Test().IsInitted()) {
		Test().UnInit();
		Test().Free();
		LOG("Library unloaded");
	}
	#endif
}


GUI_APP_MAIN
{
	UppTest().Run();
}

