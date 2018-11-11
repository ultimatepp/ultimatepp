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

void App::DoPaint0(Painter& sw)
{
	sw.Translate(~ctrl.translate_x, ~ctrl.translate_y);
	sw.Rotate(~ctrl.rotate);
	sw.Scale(~ctrl.scale, (double)~ctrl.scale * (double)~ctrl.scale_x);
	sw.Opacity(~ctrl.opacity);
	sw.LineCap(~ctrl.linecap);
	sw.LineJoin(~ctrl.linejoin);
	if(list.IsCursor())
		Examples()[list.GetCursor()].example(sw);
}

void App::DoPaint(Painter& sw)
{
	if(ctrl.painting) {
		PaintingPainter h(2000, 2000);
		DoPaint0(h);
		sw.Paint(h);
	}
	else
		DoPaint0(sw);
}

void App::Print()
{
	PaintingPainter sw(1000, 1000);
	DoPaint(sw);
	PrinterJob pb;
	if(pb.Execute())
		pb.GetDraw().DrawPainting(0, 0, 4000, 4000, sw);
}

void App::Benchmark()
{
	double tm[2];
	ImageBuffer ib(2000, 2000);
	for(int pass = 0; pass < 1 + !!ctrl.mt; pass++) {
		int time0 = GetTickCount();
		int n = 0;
		int time;
		BufferPainter sw(ib, ctrl.quality);
		for(;;) {
			time = GetTickCount();
			if(time - time0 > 1000) break;
			sw.Co(pass);
			sw.Begin();
			DoPaint(sw);
			sw.End();
			n++;
		}
		tm[pass] = double(time - time0) / n;
	}
	if(ctrl.mt)
		PromptOK(Format("ST %.3f ms, MT %.3f ms, ST/MT %.3f", tm[0], tm[1], tm[0] / tm[1]));
	else
		PromptOK(Format("%.3f ms", tm[0]));
}

void App::Paint(Draw& w)
{
	Size sz = GetSize();
	if(ctrl.transparent) {
		for(int y = 0; y < sz.cy; y += 32)
			for(int x = 0; x < sz.cx; x += 32)
				w.DrawRect(x, y, 32, 32, (x ^ y) & 32 ? Color(254, 172, 120) : Color(124, 135, 253));
	}
	ImageBuffer ib(sz);
	{
		BufferPainter sw(ib, ctrl.quality);
		if(ctrl.transparent)
			sw.Clear(RGBAZero());
		else
			sw.Clear(White());
		sw.Co(ctrl.mt);
		DoPaint(sw);
	}
	w.DrawImage(0, 0, ib);
}

void App::Sync()
{
	Refresh();
}

void App::ToSlider(EditDouble *e, SliderCtrl *slider)
{
	double v = ~*e;
	*slider <<= fround(1000.0 * (v - e->GetMin()) / (e->GetMax() - e->GetMin()));
	Refresh();
}

void App::ToEdit(EditDouble *e, SliderCtrl *slider)
{
	int x = ~*slider;
	*e <<= x * (e->GetMax() - e->GetMin()) / 1000.0 + e->GetMin();
	Refresh();
}

void App::Pair(EditDouble& e, SliderCtrl& slider)
{
	e <<= THISBACK2(ToSlider, &e, &slider);
	slider <<= THISBACK2(ToEdit, &e, &slider);
	slider.MinMax(0, 1000);
}

void App::ToSlider()
{
	ToSlider(&ctrl.rotate, &ctrl.rotate_slider);
	ToSlider(&ctrl.scale, &ctrl.scale_slider);
	ToSlider(&ctrl.scale_x, &ctrl.scale_x_slider);
	ToSlider(&ctrl.translate_x, &ctrl.translate_x_slider);
	ToSlider(&ctrl.translate_y, &ctrl.translate_y_slider);
	ToSlider(&ctrl.opacity, &ctrl.opacity_slider);
}

void App::Reset()
{
	ctrl.rotate <<= ctrl.translate_x <<= ctrl.translate_y <<= 0;
	ctrl.scale <<= ctrl.scale_x <<= ctrl.opacity <<= 1.0;
	ctrl.painting = false;
	ctrl.quality = MODE_ANTIALIASED;
	ctrl.linejoin <<= LINEJOIN_MITER;
	ctrl.linecap <<= LINECAP_BUTT;
	ToSlider();
}

void App::Serialize(Stream& s)
{
	s
		% ctrl.rotate % ctrl.rotate_slider
		% ctrl.scale % ctrl.scale_slider
		% ctrl.scale_x % ctrl.scale_x_slider
		% ctrl.translate_x % ctrl.translate_x_slider
		% ctrl.translate_y % ctrl.translate_y_slider
		% ctrl.opacity % ctrl.opacity_slider
		% ctrl.painting % ctrl.quality % ctrl.transparent
	;
}

App::App() {
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
	Pair(ctrl.rotate, ctrl.rotate_slider);
	Pair(ctrl.scale, ctrl.scale_slider);
	Pair(ctrl.scale_x, ctrl.scale_x_slider);
	Pair(ctrl.translate_x, ctrl.translate_x_slider);
	Pair(ctrl.translate_y, ctrl.translate_y_slider);
	Pair(ctrl.opacity, ctrl.opacity_slider);
	ctrl.linecap.Add(LINECAP_BUTT, "Butt caps");
	ctrl.linecap.Add(LINECAP_SQUARE, "Square caps");
	ctrl.linecap.Add(LINECAP_ROUND, "Round caps");
	ctrl.linejoin.Add(LINEJOIN_MITER, "Miter joins");
	ctrl.linejoin.Add(LINEJOIN_ROUND, "Round joins");
	ctrl.linejoin.Add(LINEJOIN_BEVEL, "Bevel joins");
	ctrl.linecap <<= ctrl.linejoin <<= ctrl.painting <<= ctrl.quality <<= ctrl.transparent <<= ctrl.mt <<= THISBACK(Sync);
	ctrl.reset <<= THISBACK(Reset);
	ctrl.benchmark <<= THISBACK(Benchmark);
	ctrl.print <<= THISBACK(Print);
	Reset();
	LoadFromFile(*this);
	Title("Painter");
}

App::~App()
{
	if(list.IsCursor())
		SaveFile(ConfigFile("last"), list.GetKey());
	StoreToFile(*this);
}

GUI_APP_MAIN
{
	App().Run();
}
