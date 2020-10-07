#include <CtrlLib/CtrlLib.h>
#include <Painter/Painter.h>

using namespace Upp;

GUI_APP_MAIN
{
	PrinterJob job;
	if(job.Execute()){
		Draw &draw=job.GetDraw();

		draw.StartPage();
		draw.BeginNative();

		Size size=draw.GetPagePixels();
		Size DPI=draw.GetPixelsPerInch();
		
		Rect drawrect=size;
		PaintingPainter dp(size);
		
		dp.Clear(White());
		dp.Move(0,0);
		dp.Line(size.cx,size.cy);
		dp.Stroke(3,Black());
		dp.Move(0,size.cy);
		dp.Line(size.cx,0);
		dp.Stroke(3,Black());
		dp.Move(0,0).Line(size.cx,0).Line(size.cx,size.cy).Line(0,size.cy).Line(0,0);
		dp.Stroke(3,Black());

		dp.Circle(size.cx/2,size.cy/2,min(size.cx,size.cy)/2).Stroke(2,Black());

		// But clearly fails via direct DrawPainting
		draw.DrawPainting(drawrect,dp.GetResult());
		
		draw.EndNative();
		draw.EndPage();
	}
}
