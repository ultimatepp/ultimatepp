#ifndef _PainterExamples_Examples_h_
#define _PainterExamples_Examples_h_

#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

#define IMAGECLASS TestImg
#define IMAGEFILE <PainterExamples/Test.iml>
#include <Draw/iml_header.h>

#define LAYOUTFILE <PainterExamples/Examples.lay>
#include <CtrlCore/lay.h>

void RegisterExample(const char *name, void (*ex)(Painter& pw));

struct App : TopWindow {
	virtual void Paint(Draw& w);
	virtual bool Key(dword key, int count);

	SplitterFrame  split;
	ArrayCtrl      list;
	FrameBottom< WithCtrlLayout<StaticRect> > ctrl;
	
	typedef App CLASSNAME;

	void DoPaint0(Painter& sw);
	void DoPaint(Painter& sw);
	void Print();
	void Benchmark();
	void Sync();
	void ToSlider(EditDouble *e, SliderCtrl *slider);
	void ToEdit(EditDouble *e, SliderCtrl *slider);
	void Pair(EditDouble& e, SliderCtrl& slider);
	void ToSlider();
	void Reset();
	void Serialize(Stream& s);

	App();
	~App();
};



#endif
