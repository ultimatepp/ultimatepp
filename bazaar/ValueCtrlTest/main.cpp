#include "ValueCtrlTest.h"

ValueCtrlTest::ValueCtrlTest()
{
	CtrlLayout(*this, "Window title");
	clear <<= THISBACK(Clear);

	vc <<= THISBACK(ActionCB);
	Add(vc.HSizePos().BottomPos(0, 20));

	Value v = ValueArray(Vector<Value>() << 123.8 << "Test");
	vc.SetData(v);
}

void ValueCtrlTest::ActionCB()
{
	Value v = vc.GetData();

	ValueEditKey vk(v.GetType());
	String s;
	s << "(" << v.GetType() << ") " << vk.ToString();
	s << " = " << AsString(v);
	ToInfo(s);
	RLOG(s);
}

GUI_APP_MAIN
{
	ValueCtrlTest().Run();
}

