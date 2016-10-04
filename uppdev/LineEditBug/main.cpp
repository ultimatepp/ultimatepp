#include <CtrlLib/CtrlLib.h>

#include <CtrlLib/CtrlLib.h>

class Test : public TopWindow
{
public:
	typedef Test CLASSNAME;
	Splitter     splitter;
	LineEdit     source;
	ArrayCtrl    chart;
	LineEdit     textMetric;

	Test();
};

Test::Test()
{
	splitter.Vert() << source << chart << textMetric;
	Add(splitter.SizePos());
	Sizeable().Zoomable();
}

GUI_APP_MAIN
{
	Test().Run();
}
