#include "RichEdit.h"

namespace Upp {

String AsSvgPath(Font font, int c, Sizef& sz) {
	struct TextToSvg : FontGlyphConsumer {
		String t; // here we accumulate the SVG path text
		Rectf  bounds = Rectf(DBL_MAX, DBL_MAX, -DBL_MAX, -DBL_MAX);
		Pointf offset = Pointf(0, 0);
		double scale = 1;
		
		void Put(Pointf p) {
			bounds.left = min(bounds.left, p.x);
			bounds.right = max(bounds.right, p.x);
			bounds.top = min(bounds.top, p.y);
			bounds.bottom = max(bounds.bottom, p.y);
			bounds.Union(p);
			p = (p + offset) * scale;
			t << Format("%.2f %.2f ", p.x, p.y);
		}
		
		virtual void Move(Pointf p)  { t << 'M'; Put(p); }
		virtual void Line(Pointf p)  { t << 'L'; Put(p); }
		virtual void Quadratic(Pointf p1, Pointf p2) { t << 'Q'; Put(p1); Put(p2); }
		virtual void Cubic(Pointf p1, Pointf p2, Pointf p3) { t << 'C'; Put(p1); Put(p2); Put(p2); }
		virtual void Close()         { t << 'Z'; }
	} t;
	
	font.Render(t, 0, 0, c);
	
	t.t.Clear();
	t.offset = -t.bounds.TopLeft();
	t.scale = 100 / t.bounds.GetHeight();
	sz = t.bounds.GetSize() * t.scale;

	font.Render(t, 0, 0, c);
		
	return t.t;
}

struct SelectSymbolDlg : WithSelectSymbolLayout<TopWindow> {
	Vector<Tuple<Sizef, String>> svg;
	int result;
	
	SelectSymbolDlg();
	
	void Sync();
	void Variants(int codepoint);
};

SelectSymbolDlg::SelectSymbolDlg()
{
	CtrlLayout(*this, "Insert symbol");
	search.NullText(t_("Search"));
	search.SetFilter(CharFilterDefaultToUpperAscii);
	group <<= 0;
	search ^= group ^= [=] { Sync(); };

	symbols.NoHyperlinkDecoration();
	symbols.WhenLink << [=](const String& s) { Variants(Atoi(s)); };
	symbols.MonoGlyphs();

	result = -1;
	variants.NoHyperlinkDecoration();
	variants.WhenLink << [=](const String& s) { result = Atoi(s); Break(IDOK); };
}

void SelectSymbolDlg::Sync()
{
	const VectorMap<String, Vector<Tuple<int, String>>>& syms = UnicodeSymbols();

	int g = ~group;
	String s = ToLower(~~search);
	String qtf = "[A5 ";
	for(int i = 0; i < syms.GetCount(); i++)
		if(g == 0 || g == i + 1)
			for(Tuple<int, String> h : syms[i])
				if(h.b.Find(s) >= 0 && StdFont().HasChar(h.a))
					qtf << "[^" << AsString(h.a) << "^ " << WString(h.a, 1) << "] ";
	symbols.SetQTF(qtf);
}

void SelectSymbolDlg::Variants(int codepoint)
{
	String qtf = "[A5 ";
	Index<Image> h;
	svg.Clear();
	for(int i = 0; i < Font::GetFaceCount(); i++) {
		Font fnt(i, 10);
		if(HasCodepoint(fnt, codepoint)) {
			Sizef szf0;
			String svgpath = AsSvgPath(fnt, codepoint, szf0);
			Size szf = szf0 * DPI(1);
			Size sz((int)ceil(szf.cx), (int)ceil(szf.cy));
			if(svgpath.GetCount() && sz.cx > 0 && sz.cy > 0) {
				ImagePainter sw(sz);
				sw.Clear();
				sw.Scale(DPI(1));
				sw.Path(svgpath).Fill(SBlack());
				Image img = sw;
				if(h.Find(img) < 0) {
					qtf << "[^" << h.GetCount() << "^ " << " " << AsQTF(CreatePNGObject(img, sz.cx, sz.cy)) << "], ";
					h.Add(img);
					svg.Add(MakeTuple(szf0, svgpath));
				}
			}
		}
	}
	variants.SetQTF(qtf);
}

String SelectFontSymbolSvg(Sizef& sz)
{
	SelectSymbolDlg dlg;
	dlg.Sync();
	if(dlg.Execute() != IDOK || dlg.result < 0 || dlg.result >= dlg.svg.GetCount())
		return Null;
	
	Tuple<Sizef, String> h = dlg.svg[dlg.result];
	sz = h.a;
	return h.b;
}

}