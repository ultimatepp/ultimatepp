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
	
	virtual bool Key(dword key, int count)
	{
		if(key == K_CTRL_P) {
			PaintingPainter sw(1000, 1000);
			sw.Clear(White());
			if(list.IsCursor())
				Examples()[list.GetCursor()].example(sw);
			PrinterJob pb;
			if(pb.Execute())
				pb.GetDraw().DrawPainting(0, 0, 4000, 4000, sw);
		}
		return TopWindow::Key(key, count);
	}
	
	virtual void Paint(Draw& w)
	{
		ImageBuffer ib(GetSize());
		BufferPainter sw(ib);
		sw.Clear(White());
		if(list.IsCursor())
			Examples()[list.GetCursor()].example(sw);
		w.DrawImage(0, 0, ib);
	}
	
	void Sync()
	{
		Refresh();
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

