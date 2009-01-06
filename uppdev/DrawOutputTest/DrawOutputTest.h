#ifndef _DrawOutputTest_DrawOutputTest_h
#define _DrawOutputTest_DrawOutputTest_h

#include <CtrlLib/CtrlLib.h>
#include <PdfDraw/PdfDraw.h>
#include <plugin/cairo/CairoCtrl.h>
using namespace Upp;
#define LAYOUTFILE <DrawOutputTest/DrawOutputTest.lay>
#include <CtrlCore/lay.h>

class DrawOutputTest : public WithDrawOutputTestLayout<TopWindow>
{
	typedef DrawOutputTest CLASSNAME;
	
private:
	Drawing _drawing;
	String _fileName;
	int _currentTest;
	Vector<Callback> _callbacks;
	
public:
	DrawOutputTest()
	{
		CtrlLayout(*this, "Draw test");
		
		previousButton <<= THISBACK(PreviousButtonClicked);
		nextButton <<= THISBACK(NextButtonClicked);
		savePdfButton <<= THISBACK(SavePdfButtonClicked);
		saveSvgButton <<= THISBACK(SaveSvgButtonClicked);
		
		_callbacks.Add(THISBACK(TestProportions));
		_callbacks.Add(THISBACK(TestDrawLine));
		_callbacks.Add(THISBACK(TestDrawPolyline));
		_callbacks.Add(THISBACK(TestDrawRect));
		_callbacks.Add(THISBACK(TestDrawImage));
		_callbacks.Add(THISBACK(TestDrawEllipse));
		_callbacks.Add(THISBACK(TestDrawText));
		_callbacks.Add(THISBACK(TestOffset));
		_callbacks.Add(THISBACK(TestFontMetrics));
		_callbacks.Add(THISBACK(TestBasicDrawing));
		_callbacks.Add(THISBACK(TestExcludeClip));
		
		_currentTest = 0;
		NavButtonClicked();
	}
	
private:
	void PreviousButtonClicked()
	{
		_currentTest--;
		
		NavButtonClicked();
	}
	
	void NextButtonClicked()
	{
		_currentTest++;
		
		NavButtonClicked();
	}
	
	void NavButtonClicked()
	{
		previousButton.Enable(_currentTest > 0);
		nextButton.Enable(_currentTest < _callbacks.GetCount() - 1);
		
		_callbacks[_currentTest]();
	}
	
	void SavePdfButtonClicked()
	{
		Cairo c;
		c.CreatePdfSurface(_fileName + ".pdf", 450, 450);
		CairoDraw draw(c);
		draw.DrawDrawing(0, 0, 450, 450, _drawing);
	}
	
	void SaveSvgButtonClicked()
	{
		Cairo c;
		c.CreateSvgSurface(_fileName + ".svg", 450, 450);
		CairoDraw draw(c);
		draw.DrawDrawing(0, 0, 450, 450, _drawing);
	}
	
	void TestProportions() // Test0
	{
		DrawingDraw d(450, 450);
		
		d.DrawEllipse(10, 10, 10, 10);
		d.DrawEllipse(10, 100, 10, 10);
		
		d.Offset(90, 0);
		d.DrawEllipse(10, 10, 10, 10);
		d.DrawEllipse(10, 100, 10, 10);
		d.End();
		
		
		d.DrawEllipse(100, 10, 10, 10, Red());
		d.DrawEllipse(150, 10, 10, 10, Red());
		
		d.Offset(0, 50);
		d.DrawEllipse(100, 10, 10, 10, Red());
		d.DrawEllipse(150, 10, 10, 10, Red());
		d.End();
		
		_drawing = d;
		_cairoCtrl.Set(&_drawing);
		_picture.Set(_drawing);
		_fileName = "TestExcludeClip";
	}
	
	void TestDrawLine() // Test1
	{
		DrawingDraw d(450, 450);
		
		for (int i = 0; i < 17; i++)
			d.DrawLine(0 + (i * 13), (i + 1) * 25,
			           450 - (i * 13), (i + 1) * 25, 
			           i);
		
		_drawing = d;
		_cairoCtrl.Set(&_drawing);
		_picture.Set(_drawing);
		_fileName = "TestDrawLine";
	}
	
	void TestDrawPolyline() // Test2
	{
		DrawingDraw d(450, 450);
		
		Vector<Point> p;
		p << Point(3, 3) << Point(200, 100) << Point(430, 25);
		d.DrawPolyline(p, 1, Color(255, 0, 0));
		
		p.Clear();
		p << Point(6, 60) << Point(250, 150) << Point(420, 60);
		d.DrawPolyline(p, 10, Color(0, 255, 0));
		
		p.Clear();
		p << Point(6, 100) << Point(250, 390) << Point(420, 100);
		d.DrawPolyline(p, 15, Color(0, 255, 0), Color(0, 90, 0));
		
		_drawing = d;
		_cairoCtrl.Set(&_drawing);
		_picture.Set(_drawing);
		_fileName = "TestDrawPolyline";
	}
	
	void TestDrawRect() // Test3
	{
		DrawingDraw d(450, 450);
		
		for (int i = 0; i < 50; i++)
			d.DrawRect((i * 34) % 300, ((i + 14) * 15) % 300, (i * 40) % 450, (i * 142) % 450, 
			           Color((i * 12 + 23) % 205, (i * 29 + 53) % 255, (i * 10 + 3) % 255));
		
		_drawing = d;
		_cairoCtrl.Set(&_drawing);
		_picture.Set(_drawing);
		_fileName = "TestDrawRect";
	}
	
	void TestDrawImage() // Test4
	{
		DrawingDraw d(450, 450);
		
		d.DrawImage(40, 240, CtrlImg::save());
		d.DrawImage(110, 210, 80, 80, CtrlImg::save());
		d.DrawImage(240, 240, CtrlImg::save(), Blue);
		d.DrawImage(310, 210, 80, 80, CtrlImg::save(), Blue);
		
		_drawing = d;
		_cairoCtrl.Set(&_drawing);
		_picture.Set(_drawing);
		_fileName = "TestDrawImage";
	}
	
	void TestDrawEllipse() // Test5
	{
		DrawingDraw d(450, 450);
		
		for (int i = 1; i < 30; i++)
			d.DrawEllipse((i * 34) % 300, ((i + 14) * 15) % 300, (i * 40) % 450, (i * 142) % 450, 
			           Color((i * 12 + 23) % 205, (i * 29 + 53) % 255, (i * 10 + 3) % 255));
		
		d.DrawEllipse(40, 60, 36, 200, Color(23, 24, 25), 3, Color(255, 255, 123));
		
		_drawing = d;
		_cairoCtrl.Set(&_drawing);
		_picture.Set(_drawing);
		_fileName = "TestDrawEllipse";
	}
	
	void TestDrawText() // Test6
	{
		DrawingDraw d(450, 450);
		
		Font font = StdFont(15);
		d.DrawText(10, 10, "Standard text", font);
		font.FaceName("Arial");
		d.DrawText(10, 40, "Arial text: T U i I AVAVAVAVAVAVAVA", font);
		font.FaceName("Times New Roman");
		d.DrawText(10, 70, "Times New Roman text: T U i I AVAVAVAVAVAVAVA", font);
		font.Underline();
		d.DrawText(10, 100, "Underlined text", font);
		font.Italic();
		d.DrawText(10, 130, "Italic text", font);
		font.Strikeout();
		d.DrawText(10, 160, "Strikeout text", font);
		font.Bold();
		d.DrawText(10, 190, "Bold text", font);
		d.DrawText(10, 220, "Colored text", font, Color(123, 23, 248));
		d.DrawText(10, 250, "Special characters äöüßéàè#°§¬|¢", StdFont(15));
		
		d.DrawText(300, 230, 3200, "Rotated text", StdFont(15));
		
		d.DrawText(10, 400, "abcdefghijklmnopqrstuvwxyz", Arial(40), Color(0, 0, 255));
		
		_drawing = d;
		_cairoCtrl.Set(&_drawing);
		_picture.Set(_drawing);
		_fileName = "TestDrawText";
	}
	
	void DoPainting(Draw& d) 
	{
		d.DrawEllipse(0, 0, 100, 30, WhiteGray(), 1, Cyan);
		d.DrawText(0, 0, "Hello<w\"orld", Roman(30).Bold());
	}
	
	void TestOffset() // Test7
	{
		DrawingDraw d(450, 450);
		
		d.DrawRect(d.GetSize(), White());
		DoPainting(d);
		
		d.Offset(30, 50);
		DoPainting(d);
		d.End();
		
		d.Offset(20, 100);
		d.Clip(5, 5, 40, 20);
		DoPainting(d);
		d.End();
		d.End();
		
		d.Clipoff(10, 150, 60, 20);
		DoPainting(d);
		d.End();
		
		_drawing = d;
		_cairoCtrl.Set(&_drawing);
		_picture.Set(_drawing);
		_fileName = "TestOffset";
	}
	
	void TestFontMetrics() // Test8
	{
		DrawingDraw d(450, 450);
		
		d.Offset(10, 50);
		const char *text = "Programming is fun";
		Font fnt = StdFont(40);
		FontInfo fi = fnt.Info();
		int x = 0;
		Vector<int> dx;
		for(const char *s = text; *s; s++) 
		{
			int width = fi[*s];
			d.DrawRect(x, 0, width - 1, fi.GetAscent(), Color(255, 255, 200));
			d.DrawRect(x, fi.GetAscent(), width - 1, fi.GetDescent(), Color(255, 200, 255));
			d.DrawRect(x + width - 1, 0, 1, fi.GetHeight(), Black());
			dx.Add(width + 4);
			x += width;
		}
		d.DrawRect(0, 0, 4, 4, Black());
		d.DrawText(0, 0, text, fnt);
		d.DrawText(0, 70, text, fnt, Blue(), dx.GetCount(), dx.Begin());
		d.End();
		
		_drawing = d;
		_cairoCtrl.Set(&_drawing);
		_picture.Set(_drawing);
		_fileName = "TestFontMetrics";
	}
	
	void TestBasicDrawing() // Test9
	{
		DrawingDraw d(450, 450);
		
		d.DrawRect(10, 10, 60, 80, Green());
		
		d.DrawLine(100, 10, 160, 80, 0, Black());
		d.DrawLine(160, 10, 100, 80, 4, Red());
		d.DrawLine(160, 40, 100, 50, PEN_DOT, Red());
		
		d.DrawEllipse(210, 20, 80, 60, Blue());
		
		d.DrawEllipse(310, 20, 80, 60, LtBlue(), 5, Red());
		
		d.DrawArc(RectC(410, 20, 80, 60), Point(10, 10), Point(450, 80), 3, Cyan);
		
		Vector<Point> p;
		p << Point(30, 110) << Point(60, 180) << Point(10, 150) << Point(70, 150);
		d.DrawPolyline(p, 4, Black);
		
		p.Clear();
		p << Point(130, 110) << Point(160, 180) << Point(110, 150) << Point(170, 120)
		  << Point(130, 110);
		d.DrawPolygon(p, Blue);
		
		p.Clear();
		p << Point(230, 110) << Point(260, 180) << Point(210, 150) << Point(270, 120)
		  << Point(230, 110);
		d.DrawPolygon(p, Cyan, 5, Magenta);
		
		d.DrawImage(40, 240, CtrlImg::save());
		d.DrawImage(110, 210, 80, 80, CtrlImg::save());
		d.DrawImage(240, 240, CtrlImg::save(), Blue);
		d.DrawImage(310, 210, 80, 80, CtrlImg::save(), Blue);
		
		d.DrawText(20, 330, "Hello world!");
		d.DrawText(120, 330, "Hello world!", Arial(15).Bold());
		d.DrawText(220, 330, "Hello world!", Roman(15).Italic(), Red);
		d.DrawText(320, 380, 400, "Hello world!", Courier(15).Underline());
		
		_drawing = d;
		_cairoCtrl.Set(&_drawing);
		_picture.Set(_drawing);
		_fileName = "TestBasicDrawing";
	}
	
	void TestExcludeClip() // Test10
	{
		DrawingDraw d(450, 450);
		
		d.Clipoff(60, 60, 200, 200);
		
		d.ExcludeClip(120, 20, 200, 30);
		
		d.IntersectClip(10, 10, 200, 100);
		
		d.DrawRect(10, 10, 400, 400, Color(255, 0, 0));
		
		d.DrawEllipse(40, 60, 36, 200, Color(23, 24, 25), 3, Color(255, 255, 123));
		d.DrawEllipse(100, 80, 30, 100, Color(77, 44, 11), 6, Color(128, 128, 128));
		
		d.End();
		
		_drawing = d;
		_cairoCtrl.Set(&_drawing);
		_picture.Set(_drawing);
		_fileName = "TestExcludeClip";
	}
};

#endif

