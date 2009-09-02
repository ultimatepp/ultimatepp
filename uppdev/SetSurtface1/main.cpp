#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

class PainterCtrl : public Ctrl {
	ImageBuffer ib;

public:
	virtual void Paint(Painter &pntr,Size &sz)=0;

	virtual void Layout(){
		Size sz=GetSize();
		ib.Clear();
		ib.Create(sz);
		BufferPainter pntr(ib);
		Paint(pntr,sz);
	}

	virtual void Paint(Draw &draw) {
		Rect paintrect=draw.GetPaintRect();
		DDUMP(paintrect);
		Point p = paintrect.TopLeft();
		if(p.x < ib.GetSize().cx && p.y < ib.GetSize().cy) 
			SetSurface(draw, paintrect, ib, ib.GetSize(), p);
	}
	
};

struct ExampleCtrl : PainterCtrl {
	
	virtual void Paint(Painter &pntr,Size &sz){
		pntr.Move(0,0).Line(sz.cx,0).Line(sz.cx,sz.cy).Line(0,sz.cy).Close().Fill(White()).Stroke(5,Black());
		pntr.Move(0,0).Line(sz.cx,sz.cy).Stroke(5,Black());           
	}
};

class ExampleTopWindow: public TopWindow{
public:
	ExampleCtrl ec;
	
	virtual void Layout(){
		ec.SetRect(20,0,GetSize().cx-20,GetSize().cy);
	}

	ExampleTopWindow(){
		Add(ec);
	}
};

GUI_APP_MAIN
{
   ExampleTopWindow win;
   win.Sizeable();
   win.Open();
   win.Run();
}
