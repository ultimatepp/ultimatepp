#include <CtrlLib/CtrlLib.h>

#define  TFILE <China/main.t>
#include <Core/t.h>

class Test1 : public TopWindow  {
public:
	typedef Test1 CLASSNAME;
	Test1();
	Label lbl1;
	Label lbl2;
	Label lbl3;
};

Test1::Test1()
{
	Title("Test");
	Add(lbl1.SetText(t_("Hello")).TopPos(2,22).HSizePos(22,22));
	Add(lbl2.SetText(t_("World")).VSizePos(24,24).HSizePos(2,2));
	Add(lbl3.SetText(t_("Save")).BottomPos(2,22).HSizePos(22,22));

	Sizeable().Zoomable();
}

GUI_APP_MAIN
{
	SetLanguage(LNG_('Z','H','C','N'));
	Test1().Run();
}
