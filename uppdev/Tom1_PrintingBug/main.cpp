#include <CtrlLib/CtrlLib.h>

using namespace Upp;

class Testcase2 : public TopWindow{
	
public:
	typedef Testcase2 CLASSNAME;
	
	Testcase2(){
		Sizeable();
		MinimizeBox();
		MaximizeBox();
		BackPaint();
	}
	
	Image MakeImage2(Size sz) const {
		ImageBuffer ib(sz.cx, sz.cy);
		Fill(ib, LtGray(), ib.GetLength());
		const Color c[] = { LtBlue() };
		for(int y = 0; y < ib.GetWidth(); y++) {
			for(int x = 0; x < ib.GetHeight(); x++)
				ib[x][y] = c[2 * (x > ib.GetHeight() / 2) + (y > ib.GetWidth() / 2)];
		}
		return ib;
	}

	void DrawIt(Draw& w, Size sz) const {
		w.DrawRect(sz, White);
		w.DrawRect(sz / 4, LtGray);
		w.DrawText(0, 0, "O", Roman(sz.cy / 3).Italic(), Red());
	}

	Image MakeImage(Size sz) const {
		ImageDraw iw(sz);
		DrawIt(iw, sz);
		return iw;
	}
	
	Image MakeImage1(Size sz) const {
		ImageBuffer ib(sz);
		for(int y=0;y<sz.cy;y++){
			for(int x=0;x<sz.cx;x++){
				ib[y][x]=(RGBA)Color((16*y/sz.cy)<<4,(16*y/sz.cy)<<4,(16*x/sz.cx)<<4);
			}
		}
		return ib;
	}

	void print(){
	}
	
	virtual void Paint(Draw &w){
		Size sz = GetSize();
		w.DrawRect(sz, White);
		w.DrawImage(sz.cx / 3, sz.cy / 3, MakeImage(GetSize()));
		w.DrawText(0, 0, "O", Roman(GetSize().cy / 3).Italic(), Red());
		w.DrawText(10,500,"Press 'P' to print image, 'R' to print rectangle test, 'C' to another test...");
		w.DrawText(10,530,"(This really drains your ink/toner cartridge, so try e.g. Microsoft XPS Document Writer.)");
	}
	
	bool Key(dword key, int count){
		PrinterJob job;
		switch(key){
			case K_P:
				if(job.Execute()){
					Draw &draw = job;
					draw.StartPage();
					Size sz = draw.GetPagePixels();
//					draw.DrawText(draw.NativeX(700), draw.NativeY(700), "O", Roman(draw.NativeY(600)).Italic(), Red());
					DrawIt(draw, sz);
					draw.DrawImage(sz.cx / 3, sz.cy / 3, MakeImage(sz));
					draw.Offset(200, 1000);
					draw.DrawImage(sz.cx / 3, sz.cy / 3, MakeImage(sz));
					draw.EndPage();
				}
				return true;
			case K_T:
				if(job.Execute()) {
					Draw &draw = job;
				}
				return true;
			case K_R:
				if(job.Execute()) {
					Draw &draw = job;
					draw.StartPage();
					bool flag = false;
					for(int y = 0; y < 100; y++)
						for(int x = 0; x < 100; x++) {
							draw.DrawRect(16 * x, 16 * y, 19, 19, flag ? Blue : Red);
							if((Random() & 31) == 0)
								flag = !flag;
						}
					draw.EndPage();
				}
				return true;
			case K_C:
				if(job.Execute()){
					Draw &draw = job;
					draw.StartPage();
					for(int y = 0; y < 100; y++)
						for(int x = 0; x < 100; x++)
							draw.DrawRect(16 * x, 16 * y, 16, 16, Blue);
					draw.EndPage();
				}
				return true;
		}
		return false;
	}

};

GUI_APP_MAIN
{
	Testcase2().Run();
}
