#include <CtrlLib/CtrlLib.h>

using namespace Upp;

int  GlyphIndex(Font fnt, int ch);
bool HasCodepoint(Font fnt, int ch);
int  GetAdvanceWidth(Font fnt, int ch);
double fx_to_dbl(const FIXED& p);
Pointf fx_to_dbl(const Pointf& pp, const POINTFX& p);
void RenderCodepoint(FontGlyphConsumer& sw, double x, double y, int ch, Font fnt);
void DrawCodepoint(Draw& w, int x, int y, int ch, Font font);

struct MyApp : TopWindow {
	SplitterFrame sf;
	StaticRect pane;
	DropList   face;
	ArrayCtrl  list;
	EditString code;

	virtual void Paint(Draw& w) {
		Font font = Font(~face, 100); // Font().Height(100).FaceName("MingLiU-ExtB");
		font.Italic();
//		Font font = Font().Height(100).FaceName("MingLiU-ExtB");
		w.DrawRect(GetSize(), White());
		int x = 0;
		int y = 40;
		int ch = ScanInt(~~code, NULL, 16);
		if(SystemDraw *sw = dynamic_cast<SystemDraw *>(&w)) {
			int cx = GetAdvanceWidth(font, ch);
			w.DrawRect(100, 200, cx, 20, WhiteGray());
			
			DrawCodepoint(w, 100, 200, ch, font);

			WString h = ToUtf16(ch);
			String f;
			for(wchar ch : h)
				f << Format("%x ", ch);

		    f << ", glyph: " << GlyphIndex(font, ch);
		    
		    if(HasCodepoint(font, ch))
		        f << " EXISTS";
			
			DDUMP(GetLastErrorMessage());

			w.DrawText(20, 300, f);
		}

		struct PaintCharPath : FontGlyphConsumer {
			Painter *sw;
			
			virtual void Move(Pointf p) {
				sw->Move(p);
			}
			virtual void Line(Pointf p) {
				sw->Line(p);
			}
			virtual void Quadratic(Pointf p1, Pointf p2) {
				sw->Quadratic(p1, p2);
			}
			virtual void Cubic(Pointf p1, Pointf p2, Pointf p3) {
				sw->Cubic(p1, p2, p3);
			}
			virtual void Close() {
				sw->Close();
			}
		};
		
		ImagePainter ip(200, 200);
		ip.Clear(Yellow());
		PaintCharPath h;
		h.sw = &ip;
		RenderCodepoint(h, 0, 0, ch, font);
		ip.Fill(Black());
		
		w.DrawImage(100, 400, ip);
	}
	
	MyApp() {
		pane.Add(face.TopPosZ(0, 20).HSizePos());
		pane.Add(code.TopPosZ(20, 20).HSizePos());
		pane.Add(list.HSizePos().VSizePosZ(40, 0));
		AddFrame(sf.Left(pane, Zx(250)));
		list.AddColumn("List");
		FileIn in(GetDataFile("UnicodeData.txt"));
		while(!in.IsEof())
			list.Add(in.GetLine());
		AddFrame(InsetFrame());
		sf.SizeMin(40).MinSize(40);
		Sizeable();

		int q = -1;
	    for(int i = 1; i < Font::GetFaceCount(); i++) {
	        if(Font::GetFaceInfo(i) & Font::SCALEABLE) {
	            String n = Font::GetFaceName(i);
	            DDUMP(n.StartsWith("MingLiU"));
	            if(n.StartsWith("MingLiU"))
	                q = i;
	            face.Add(i, n);
	        }
	    }
	    if(q >= 0)
	        face <<= q;
	        
		list.WhenSel = [=] { code <<= Format("%x", ScanInt(~list.GetKey(), NULL, 16)); Refresh(); };
		code << [=] { Refresh(); };
		
		face << [=] {
			DLOG("================");
			int i = 32;
			int pi = 0;
			while(i < 0x30000) {
				Font font = Font(~face, 100);
				if(HasCodepoint(font, i)) {
					int i0 = i;
					while(HasCodepoint(font, i))
						i++;
					DLOG(i0 << " " << i0 - pi << "  " << i - i0);
					pi = i;
				}
				else
					i++;
			}
		};
		
		code <<= "20010";
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
