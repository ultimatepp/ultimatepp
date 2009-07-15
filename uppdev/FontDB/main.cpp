#include "FontDB.h"

Image LetterImg(char c, Font fnt)
{
	ImagePainter iw(128, 128);
	iw.DrawRect(0, 0, 128, 128, White());
	iw.DrawText(0, 0, &c, fnt, Black(), 1);
	return iw;
}

String FormatBin(dword w)
{
	String s;
	for(int i = 0; i < 32; i++)
		s << (((1 << i) & w) ? "1" : "0");
	return s;
}

dword ClassifyFontFace(int face)
{
	Font fnt(face, 100);
	Image I = LetterImg('I', fnt);
	Image V = LetterImg('V', fnt);
	int density = 0;
	int complexity = 0;
	byte cc = 0;
	for(const RGBA *s = I; s < ~I + I.GetLength(); s++) {
		if(s->r == 0)
			density++;
		if(s->r != cc) {
			cc = s->r;
			complexity++;
		}
	}
	for(const RGBA *s = V; s < ~V + V.GetLength(); s++) {
		if(s->r != cc) {
			cc = s->r;
			complexity++;
		}
	}
	int thickness = 0;
	for(const RGBA *s = V[45]; s < V[45] + 100; s++) {
		if(s->r == 0)
			thickness++;
	}
	int ithickness = 0;
	for(const RGBA *s = I[45]; s < I[45] + 100; s++) {
		if(s->r == 0)
			ithickness++;
	}
	int vserif = 0;
	int iserif = 0;
	for(int i = 0; i < 100; i++) {
		int v = 0;
		int w = 0;
		for(const RGBA *s = V[i]; s < V[i] + 100; s++) {
			if(s->r == 0)
				v++;
		}
		for(const RGBA *s = I[i]; s < I[i] + 100; s++) {
			if(s->r == 0)
				w++;
		}
		vserif = max(vserif, v);
		iserif = max(iserif, w);
	}
	dword lower = 0;
	dword all = 0;
	vserif -= thickness;
	iserif -= ithickness;
	vserif = 15 * minmax(vserif, 0, 40) / 40;
	iserif = 15 * minmax(iserif, 0, 20) / 20;
	density = 15 * minmax(density, 0, 2000) / 2000;
	thickness = 15 * minmax(thickness - ithickness, 0, 20) / 20;
	ithickness = 15 * minmax(ithickness, 0, 60) / 60;
	complexity = 15 * minmax(complexity - 600, 0, 600) / 600;
	int vw = 15 * minmax(fnt['V'], 0, 100) / 100;
	return vserif | (iserif << 4) | (density << 8) | (thickness << 12) | (ithickness << 16) |
	       (complexity << 20) | (vw << 24);
}

int FontClassDistance(dword a, dword b)
{
	int d = 0;
	for(int i = 0; i < 8; i++) {
		int q = int(a & 15) - int(b & 15);
		d = d + q * q;
		a >>= 4;
		b >>= 4;
	}
	d++;
	return d;
}

Vector<dword> fclass;

void FontDB::Perform()
{
	Progress pi("Scanning fonts");
	OpenMain();
	pi.SetTotal(Font::GetFaceCount());
	for(int face = 0; face < Font::GetFaceCount(); face++) {
		fclass.Add(ClassifyFontFace(face));
	}
	for(int face = 0; face < Font::GetFaceCount(); face++) {
		if(pi.StepCanceled())
			return;
		Font fnt(face, 100);
		if(fnt.IsScaleable() && fnt.IsUnicode()) {
			pi.SetText(fnt.GetFaceName());
			Image I = LetterImg('I', fnt);
			Image V = LetterImg('V', fnt);
			int density = 0;
			int complexity = 0;
			byte cc = 0;
			for(const RGBA *s = I; s < ~I + I.GetLength(); s++) {
				if(s->r == 0)
					density++;
				if(s->r != cc) {
					cc = s->r;
					complexity++;
				}
			}
			for(const RGBA *s = V; s < ~V + V.GetLength(); s++) {
				if(s->r != cc) {
					cc = s->r;
					complexity++;
				}
			}
			int thickness = 0;
			for(const RGBA *s = V[45]; s < V[45] + 100; s++) {
				if(s->r == 0)
					thickness++;
			}
			int ithickness = 0;
			for(const RGBA *s = I[45]; s < I[45] + 100; s++) {
				if(s->r == 0)
					ithickness++;
			}
			int vserif = 0;
			int iserif = 0;
			for(int i = 0; i < 100; i++) {
				int v = 0;
				int w = 0;
				for(const RGBA *s = V[i]; s < V[i] + 100; s++) {
					if(s->r == 0)
						v++;
				}
				for(const RGBA *s = I[i]; s < I[i] + 100; s++) {
					if(s->r == 0)
						w++;
				}
				vserif = max(vserif, v);
				iserif = max(iserif, w);
			}
			dword lower = 0;
			dword all = 0;
		#if 0
			Progress pi(fnt.GetFaceName() << " glyphs");
			pi.SetTotal(65536);
			for(int i = 0; i < 65536;) {
				if(pi.SetPosCanceled(i)) {
					Execute();
					return;
				}
				if(fnt.IsNormal(i)) {
					if(i < 8192)
						lower |= 1 << (i / 256);
					all |= 1 << (i / 2048);
					i = (i + 256) & 0xffff00;
				}
				else
					i++;
			}
		#endif
			vserif -= thickness;
			iserif -= ithickness;
			vserif = 15 * minmax(vserif, 0, 40) / 40;
			iserif = 15 * minmax(iserif, 0, 20) / 20;
			density = 15 * minmax(density, 0, 2000) / 2000;
			thickness = 15 * minmax(thickness - ithickness, 0, 20) / 20;
			ithickness = 15 * minmax(ithickness, 0, 60) / 60;
			complexity = 15 * minmax(complexity - 600, 0, 600) / 600;
			int vw = 15 * minmax(fnt['V'], 0, 100) / 100;
			int qc = ClassifyFontFace(face);
			int q = 0;
			int min = INT_MAX;
			for(int i = 0; i < fclass.GetCount(); i++) {
				dword w = Font::GetFaceInfo(i);
				if((w & Font::SCALEABLE) && !(w & Font::LOCAL) && i != face && (w & Font::FIXEDPITCH) == fnt.IsFixedPitch()) {
					int d = FontClassDistance(qc, fclass[i]);
					if(d < min) {
						q = i;
						min = d;
					}
				}
			}
			list.Add(fnt.GetFaceName(), Rescale(I, 40, 40), Rescale(V, 40, 40),
			         density, complexity, thickness, ithickness, vserif, iserif, vw,
			         FormatIntHex(qc), Font::GetFaceName(q),
			         FormatBin(lower), FormatBin(all));
		}
	}
	pi.Close();
	Execute();
}

FontDB::FontDB()
{
	for(int i = 0; i < Font::GetFaceCount(); i++) {
		DDUMP(Font::GetFaceName(i));
		DDUMP(Font::GetFaceInfo(i) & Font::SCALEABLE);
		DDUMP(Font::GetFaceInfo(i) & Font::LOCAL);
	}
	Add(list.SizePos());
	list.AddColumn("Face");
	list.AddColumn("V").SetDisplay(CenteredImageDisplay());
	list.AddColumn("I").SetDisplay(CenteredImageDisplay());
	list.AddColumn("Density");
	list.AddColumn("Complexity");
	list.AddColumn("VT");
	list.AddColumn("IT");
	list.AddColumn("VS");
	list.AddColumn("IS");
	list.AddColumn("VW");
	list.AddColumn("Number");
	list.AddColumn("Similar");
//	list.AddColumn("Lower");
//	list.AddColumn("Upper");
	list.ColumnWidths("114 57 55 56 35 33 27 30 40 37 111 338");
	list.SetLineCy(40);
}

GUI_APP_MAIN
{
	FontDB().Perform();
}

