#include <CtrlLib/CtrlLib.h>

using namespace Upp;

// http://www.ictp.trieste.it/~manuals/programming/Java/tutorial/uiswing/overview/anatomy.html

#define LAYOUTFILE <Converter/converter.lay>
#include <CtrlCore/lay.h>

struct ConverterPane : WithConverterPaneLayout<StaticRect> {
	ConverterPane *other;
	void Sync()
	{
		other->slider <<= other->value <<= (double)~unit * Nvl((double)~value) / (double)~other->unit;
	}

	ConverterPane()
	{
		CtrlLayout(*this);
		slider.Range(10000);
		slider <<= value <<= 0;
		value << [=] { slider <<= ~value; Sync(); };
		slider << [=] { value <<= ~slider; Sync(); };
		unit << [=] { Sync(); };
	}
};

struct Converter : TopWindow
{
	ConverterPane metric, us;

	typedef Converter CLASSNAME;

	Converter()
	{
		Title("Converter");
		Size sz = metric.GetLayoutSize();
		SetRect(0, 0, metric.GetSize().cx, metric.GetSize().cy * 2);
		Add(metric.HSizePos().TopPos(0, sz.cy));
		Add(us.HSizePos().BottomPos(0, sz.cy));

		metric.title = "Metric units";
		metric.unit.Add(0.01, "Centimeters");
		metric.unit.Add(1.0, "Meters");
		metric.unit.Add(1000.0, "Kilometers");
		metric.unit <<= 1.0;

		us.title = "U.S. units";
		us.unit.Add(0.0254, "Inches");
		us.unit.Add(0.305, "Feet");
		us.unit.Add(0.914, "Yards");
		us.unit.Add(1613.0, "Miles");
		us.unit <<= 0.305;

		us.other = &metric;
		metric.other = &us;

		metric.Sync();
	}
};

GUI_APP_MAIN
{
	Converter().Run();
}
