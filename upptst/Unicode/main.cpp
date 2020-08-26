#include <CtrlLib/CtrlLib.h>

using namespace Upp;

namespace Upp {
HFONT GetWin32Font(Font fnt, int angle);
};

int GlyphIndex(HDC hdc, int ch)
{
	WString h = ToUtf16(ch);
	GCP_RESULTSW gcp_results = { sizeof(GCP_RESULTS) };
	gcp_results.nGlyphs = 4;
	wchar gi[4];
	gcp_results.lpGlyphs = gi;
//	RTIMING("GetCharacterPlacement");
	GetCharacterPlacementW(hdc, ~h, h.GetCount(), 0, &gcp_results, GCP_GLYPHSHAPE);
	return (word)gi[0];
}

int GlyphIndex(int ch, Font fnt)
{
	RTIMING("GlyphIndex");
	int r = -1;
	HFONT hfont = GetWin32Font(fnt, 0);
	VERIFY(hfont);
	if(hfont) {
		static HDC hdc = CreateIC("DISPLAY", NULL, NULL, NULL);
		HFONT ohfont;
		{
			RTIMING("Select");
			ohfont = (HFONT) ::SelectObject(hdc, hfont);
		}
		r = GlyphIndex(hdc, ch);
//		::SelectObject(hdc, ohfont);
//		::DeleteDC(hdc);
	}
	return r;
}

bool HasCodepoint(int ch, Font fnt)
{
	static Vector<int> tofu;
	int& q = tofu.At(fnt.GetFace(), Null);
	if(IsNull(q))
		q = GlyphIndex(1, fnt);
	return ch == ' ' || GlyphIndex(ch, fnt) != q;
}

int GetAdvanceWidth(int ch, Font fnt)
{
	TIMING("Glyph");
	HFONT hfont = GetWin32Font(fnt, 0);
	VERIFY(hfont);
	int r = -1;
	if(hfont) {
		HDC hdc = CreateIC("DISPLAY", NULL, NULL, NULL);
		HFONT ohfont = (HFONT) ::SelectObject(hdc, hfont);
		GLYPHMETRICS gm;
		memset(&gm, 0, sizeof(gm));
		MAT2 m_matrix;
		memset8(&m_matrix, 0, sizeof(m_matrix));
		m_matrix.eM11.value = 1;
		m_matrix.eM22.value = 1;
		ch = GlyphIndex(hdc, ch);
		int gsz = GetGlyphOutlineW(hdc, ch, GGO_NATIVE|GGO_METRICS|GGO_GLYPH_INDEX, &gm, 0, NULL, &m_matrix);
		if(gsz != GDI_ERROR && gm.gmCellIncX != 75)
			r = gm.gmCellIncX;
		::SelectObject(hdc, ohfont);
		::DeleteDC(hdc);
	}
	return r;
}

double fx_to_dbl(const FIXED& p) {
	return double(p.value) + double(p.fract) * (1.0 / 65536.0);
}

Pointf fx_to_dbl(const Pointf& pp, const POINTFX& p) {
	return Pointf(pp.x + fx_to_dbl(p.x), pp.y - fx_to_dbl(p.y));
}

void RenderCharPath2(const char* gbuf, unsigned total_size, FontGlyphConsumer& sw, double xx, double yy)
{
	const char* cur_glyph = gbuf;
	const char* end_glyph = gbuf + total_size;
	Pointf pp(xx, yy);
	while(cur_glyph < end_glyph) {
		const TTPOLYGONHEADER* th = (TTPOLYGONHEADER*)cur_glyph;
		const char* end_poly = cur_glyph + th->cb;
		const char* cur_poly = cur_glyph + sizeof(TTPOLYGONHEADER);
		sw.Move(fx_to_dbl(pp, th->pfxStart));
		while(cur_poly < end_poly) {
			const TTPOLYCURVE* pc = (const TTPOLYCURVE*)cur_poly;
			if (pc->wType == TT_PRIM_LINE)
				for(int i = 0; i < pc->cpfx; i++)
					sw.Line(fx_to_dbl(pp, pc->apfx[i]));
			if (pc->wType == TT_PRIM_QSPLINE)
				for(int u = 0; u < pc->cpfx - 1; u++) {
					Pointf b = fx_to_dbl(pp, pc->apfx[u]);
					Pointf c = fx_to_dbl(pp, pc->apfx[u + 1]);
					if (u < pc->cpfx - 2)
						c = Mid(b, c);
					sw.Quadratic(b, c);
				}
			cur_poly += sizeof(WORD) * 2 + sizeof(POINTFX) * pc->cpfx;
		}
		sw.Close();
		cur_glyph += th->cb;
    }
}

void RenderGlyph2(FontGlyphConsumer& sw, double x, double y, int ch, Font fnt)
{
	HFONT hfont = GetWin32Font(fnt, 0);
	VERIFY(hfont);
	if(hfont) {
		HDC hdc = CreateIC("DISPLAY", NULL, NULL, NULL);
		HFONT ohfont = (HFONT) ::SelectObject(hdc, hfont);
		GLYPHMETRICS gm;
		MAT2 m_matrix;
		memset8(&m_matrix, 0, sizeof(m_matrix));
		m_matrix.eM11.value = 1;
		m_matrix.eM22.value = 1;
		ch = GlyphIndex(hdc, ch);
		int gsz = GetGlyphOutlineW(hdc, ch, GGO_NATIVE|GGO_UNHINTED|GGO_GLYPH_INDEX, &gm, 0, NULL, &m_matrix);
		if(gsz >= 0) {
			StringBuffer gb(gsz);
			gsz = GetGlyphOutlineW(hdc, ch, GGO_NATIVE|GGO_UNHINTED|GGO_GLYPH_INDEX, &gm, gsz, ~gb, &m_matrix);
			if(gsz >= 0)
				RenderCharPath2(~gb, gsz, sw, x, y + fnt.GetAscent());
		}
		::SelectObject(hdc, ohfont);
		::DeleteDC(hdc);
	}
}

struct MyApp : TopWindow {
	SplitterFrame sf;
	StaticRect pane;
	DropList   face;
	ArrayCtrl  list;
	EditString code;

	virtual void Paint(Draw& w) {
		Font font = Font(~face, 100); // Font().Height(100).FaceName("MingLiU-ExtB");
//		Font font = Font().Height(100).FaceName("MingLiU-ExtB");
		w.DrawRect(GetSize(), White());
		int x = 0;
		int y = 40;
		int ch = ScanInt(~~code, NULL, 16);
		if(SystemDraw *sw = dynamic_cast<SystemDraw *>(&w)) {
			WString h = ToUtf16(ch);
			String f;
			for(wchar ch : h)
				f << Format("%x ", ch);

		    // Get glyph indices for the string
		 
			DDUMP(ch);
			int cx = GetAdvanceWidth(ch, font);
			w.DrawRect(100, 200, cx, 20, WhiteGray());

			HFONT hfont = GetWin32Font(font, 0);
			DDUMP(hfont);
			HDC hdc = sw->BeginGdi();
			DDUMP(hdc);
			HGDIOBJ orgfont = ::SelectObject(hdc, hfont);
			DDUMP(GetLastErrorMessage());
			DDUMP(h.GetCount());
			::SetTextColor(hdc, sw->GetColor(Black()));
			::ExtTextOutW(hdc, 100, 200, 0, NULL, ~h, 2, NULL);
			DDUMP(GetLastErrorMessage());

		    GCP_RESULTSW gcp_results = { sizeof(GCP_RESULTS) };
		    gcp_results.nGlyphs = 2;
		    wchar gi[4];
		    gcp_results.lpGlyphs = gi;
		    GetCharacterPlacementW(hdc, ~h, h.GetCount(), 0, &gcp_results, GCP_GLYPHSHAPE);
		    f << ", glyph: " << (int)gi[0];
		    
		    if(HasCodepoint(ch, font))
		        f << " EXISTS";
			
			::SelectObject(hdc, orgfont);
			DDUMP(GetLastErrorMessage());
			sw->EndGdi();
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
		RenderGlyph2(h, 0, 0, ch, font);
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
			for(int i = 0; i < 0x2ffff; i++) {
				Font font = Font(~face, 100);
				if(HasCodepoint(i, font))
					DDUMPHEX(i);
			}
		};
		
		code <<= "20010";
	}
};

GUI_APP_MAIN
{
	MyApp().Run();
}
