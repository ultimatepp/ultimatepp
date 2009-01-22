#include "Examples.h"

#define IMAGECLASS TestImg
#define IMAGEFILE <PainterExamples/Test.iml>
#include <Draw/iml_source.h>

struct Example {
	void   (*example)(Painter& pw);
	String name;
};

Array<Example>& Examples()
{
	static Array<Example> x;
	return x;
};

void RegisterExample(const char *name, void (*ex)(Painter& pw))
{
	Example& x = Examples().Add();
	x.name = name;
	x.example = ex;
}

struct App : TopWindow {
	SplitterFrame  split;
	ArrayCtrl      list;
	FrameBottom< WithCtrlLayout<StaticRect> > ctrl;
	
	void DoPaint(Painter& sw)
	{
		sw.Translate(~ctrl.translate_x, ~ctrl.translate_y);
		sw.Rotate(~ctrl.rotate);
		sw.Scale(~ctrl.scale, (double)~ctrl.scale * (double)~ctrl.scale_x);
		sw.Opacity(~ctrl.opacity);
		sw.Clear(White());
		if(list.IsCursor())
			Examples()[list.GetCursor()].example(sw);
	}
	
	void Print()
	{
		PaintingPainter sw(1000, 1000);
		DoPaint(sw);
		PrinterJob pb;
		if(pb.Execute())
			pb.GetDraw().DrawPainting(0, 0, 4000, 4000, sw);
	}
	
	void Benchmark()
	{
		int time;
		int time0 = GetTickCount();
		int n = 0;
		for(;;) {
			time = GetTickCount();
			if(time - time0 > 1000) break;
			ImageBuffer ib(GetSize());
			BufferPainter sw(ib);
			DoPaint(sw);
			n++;
		}
		PromptOK("Benchmark: " + AsString(double(time - time0) / n) + " ms");
	}
	
	virtual void Paint(Draw& w)
	{
		ImageBuffer ib(GetSize());
		BufferPainter sw(ib);
		DoPaint(sw);
		w.DrawImage(0, 0, ib);
	}
	
	void Sync()
	{
		Refresh();
	}
	
	void ToSlider(EditDouble *e, SliderCtrl *slider)
	{
		double v = ~*e;
		*slider <<= fround(1000.0 * (v - e->GetMin()) / (e->GetMax() - e->GetMin()));
		Refresh();
	}
	
	void ToEdit(EditDouble *e, SliderCtrl *slider)
	{
		int x = ~*slider;
		*e <<= x * (e->GetMax() - e->GetMin()) / 1000.0 + e->GetMin();
		Refresh();
	}
	
	void Pair(EditDouble& e, SliderCtrl& slider)
	{
		e <<= THISBACK2(ToSlider, &e, &slider);
		slider <<= THISBACK2(ToEdit, &e, &slider);
		slider.MinMax(0, 1000);
	}

	void ToSlider()
	{
		ToSlider(&ctrl.rotate, &ctrl.rotate_slider);
		ToSlider(&ctrl.scale, &ctrl.scale_slider);
		ToSlider(&ctrl.scale_x, &ctrl.scale_x_slider);
		ToSlider(&ctrl.translate_x, &ctrl.translate_x_slider);
		ToSlider(&ctrl.translate_y, &ctrl.translate_y_slider);
		ToSlider(&ctrl.opacity, &ctrl.opacity_slider);
	}
	
	void Reset()
	{
		ctrl.rotate <<= ctrl.translate_x <<= ctrl.translate_y <<= 0;
		ctrl.scale <<= ctrl.scale_x <<= ctrl.opacity <<= 1.0;
		ToSlider();
	}
	
	typedef App CLASSNAME;
	
	App() {
		AddFrame(split);
		split.Left(list, HorzLayoutZoom(200));
		list.WhenSel = THISBACK(Sync);
		list.AddColumn();
		list.NoHeader();
		for(int i = 0; i < Examples().GetCount(); i++)
			list.Add(Examples()[i].name);
		list.FindSetCursor(LoadFile(ConfigFile("last")));
		Sync();
		Sizeable().Zoomable();
		ctrl.Height(ctrl.GetLayoutSize().cy);
		CtrlLayout(ctrl);
		list.InsertFrame(0, ctrl);
		Reset();
		Pair(ctrl.rotate, ctrl.rotate_slider);
		Pair(ctrl.scale, ctrl.scale_slider);
		Pair(ctrl.scale_x, ctrl.scale_x_slider);
		Pair(ctrl.translate_x, ctrl.translate_x_slider);
		Pair(ctrl.translate_y, ctrl.translate_y_slider);
		Pair(ctrl.opacity, ctrl.opacity_slider);
		ctrl.reset <<= THISBACK(Reset);
		ctrl.benchmark <<= THISBACK(Benchmark);
		ctrl.print <<= THISBACK(Print);
	}
	~App()
	{
		if(list.IsCursor())
			SaveFile(ConfigFile("last"), list.GetKey());
	}
};

GUI_APP_MAIN
{
	App().Run();
}

