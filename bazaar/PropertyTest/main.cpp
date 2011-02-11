#include "PropertyTest.h"

PropertyTest::PropertyTest()

//initializer space for constant Properties or Accessors

#pragma warning(push)
#pragma warning(disable:4355)
	: vp( THISBACK(SetD), THISBACK(GetD) )
	, va( THISBACK(SetDa), THISBACK(GetDa) )
#pragma warning(pop)

{
	CtrlLayout(*this, "Window title");

	//setting, will call SetD
	vp = 123;

	//getting, will call GetD
	Value v = vp;
	RLOG(v);
	
	///
	
	bool b;
	b = va.Set(456); //will call SetDa
	
	v = Value();
	b = va.Get(v); //will call GetDa
	if(b) RLOG(v);
}

GUI_APP_MAIN
{
	PropertyTest().Run();
}

