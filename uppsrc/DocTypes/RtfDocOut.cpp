#include <DocTypes/DocTypes.h>

namespace Upp {

RtfDocOut::RtfDocOut() {
	color.Add(Black());
	pgcx = 3968;
	imagescale = 6;
	page = false;
	rtf.Reserve(8192);
}

#ifdef flagSO
RtfDocOut::~RtfDocOut() {}
#endif

void RtfDocOut::Clear() {
	color.Clear();
	color.Add(Black());
	pgcx = 3968;
	imagescale = 6;
	page = false;
	rtf.Clear();
}

void RtfDocOut::PutColor(const char *cmd, dword col) {
	rtf.Cat(Format("\\%s%d", cmd, color.FindAdd(col) + 1));
}

void RtfDocOut::Border(const char *cmd, int width, dword color) {
	if(width) {
		rtf.Cat(Format("\\%s\\brdrth\\brdrw%d", cmd, Dow(width)));
		PutColor("brdrcf", color);
	}
}

SystemDraw& ScreenInfo();

void RtfDocOut::PutPicture(const Drawing& iw, Size sz) {
#ifdef PLATFORM_WIN32
#if SYSTEMDRAW
	WinMetaFile wmf = AsWMF(iw);
#else
	WinMetaFile wmf = iw.AsWMF();
#endif
	HENHMETAFILE hemf = wmf.GetHEMF();
	if(!hemf) return;
	dword size = GetWinMetaFileBits(hemf, 0, NULL, MM_ANISOTROPIC, ScreenHDC());
	if(size <= 0) return;
	StringBuffer b(size);
	GetWinMetaFileBits(hemf, size, (byte *) ~b, MM_ANISOTROPIC, ScreenHDC());
	String data = b;
	rtf.Cat(Format("\r\n{\\pict\\wmetafile8\\picw%d\\pich%d\\picwgoal%d\\pichgoal%d \r\n",
			       254 * sz.cx / 60, 254 * sz.cy / 60, Dow(sz.cx), Dow(sz.cy)));
	static const char hex[] = "0123456789ABCDEF";
	const byte *lim = (byte *) data.End();
	for(const byte *s = data; s < lim; s++)
		rtf << hex[*s >> 4] << hex[*s & 15];
	rtf.Cat("\r\n}\r\n");
#endif
}

void RtfDocOut::PutPara(const Paragraph& p, bool par, bool table, int ls, int rs, int ts, int bs) {
	int i;
	const Paragraph::Style& st = p.GetStyle();
	String bt;
	String bf;
	String bfs;
	Font bsf = p.GetParaFont();
	for(i = 0; i < p.GetCount(); i++) {
		const Paragraph::Part& pi = p.GetParts()[i];
		if(!pi.pr) {
			bsf = pi.font;
			break;
		}
	}
	bfs = Format("%d ", 6 * abs(bsf.GetHeight()) / 25);
	if(st.bullet) {
		const Display *d = &st.bullet.GetDisplay();
		bf = "4";
		if(d == &Paragraph::WhiteBullet().GetDisplay())
			bt = "\\'a1";
		else
		if(d == &Paragraph::BoxBullet().GetDisplay())
			bt = "n";
		else
		if(d == &Paragraph::WhiteBoxBullet().GetDisplay())
			bt = "o";
		else {
			bf = "3";
			bt = "\\'b7";
		}
		rtf.Cat("{\\pntext\\pard\\plain\\f");
		rtf.Cat(bf);
		rtf.Cat("\\fs");
		rtf.Cat(bfs);
		rtf.Cat(bt);
		rtf.Cat("\\tab}\r\n");
	}
	rtf.Cat("\\pard\\lang1029");
	if(page) {
		rtf.Cat("\\pagebb");
		page = false;
	}
	rtf.Cat("\\q");
	switch(st.align) {
	default: rtf.Cat('l'); break;
	case ALIGN_RIGHT: rtf.Cat('r'); break;
	case ALIGN_JUSTIFY: rtf.Cat('j'); break;
	case ALIGN_CENTER: rtf.Cat('c'); break;
	}
	if(table)
		rtf.Cat("\\intbl");
	int bi = st.bullet ? st.bulletindent : 0;
	rtf.Cat(Format("\\fi%d\\li%d\\ri%d\\sb%d\\sa%d \r\n",
		Dow(st.bullet ? -bi : st.indent), Dow(st.lm + ls + bi), Dow(st.rm + rs),
			Dow(st.before + ts), Dow(st.after + bs)));
	if(st.bullet) {
		rtf.Cat("{\\*\\pn \\pnlvlblt\\pnf");
		rtf.Cat(bf);
		rtf.Cat("\\pnfs");
		rtf.Cat(bfs);
		rtf.Cat(Format("\\pnindent%d\\pnhang{\\pntxtb ", Dow(bi + st.indent)));
		rtf.Cat(bt);
		rtf.Cat("}}\r\n");
	}
 	for(i = 0; i < p.GetCount(); i++) {
		const Paragraph::Part& pi = p.GetParts()[i];
		if(pi.pr) {
			Size sz = (int)imagescale * pi.pr.GetStdSize();
			if(pi.sz.cx) sz.cx = pi.sz.cx;
			if(pi.sz.cy) sz.cy = pi.sz.cy;
			DrawingDraw wd(sz);
			pi.pr.Paint(wd, 0, 0, sz.cx, sz.cy, Black, White);
			PutPicture(wd, sz);
		}
		else {
			rtf.Cat('{');
			if(pi.font.IsBold()) rtf.Cat("\\b");
			if(pi.font.IsItalic()) rtf.Cat("\\i");
			if(pi.font.IsUnderline()) rtf.Cat("\\ul");
			if(pi.font.IsStrikeout()) rtf.Cat("\\strike");
			switch(pi.font.GetFace()) {
			case Font::ROMAN:   rtf.Cat("\\f1"); break;
			case Font::COURIER: rtf.Cat("\\f2"); break;
			case Font::SYMBOL:  rtf.Cat("\\f3"); break;
			}
			rtf.Cat(Format("\\fs%d", 6 * abs(pi.font.GetHeight()) / 25));
			if(pi.color != Black)
				PutColor("cf", pi.color);
			rtf.Cat(' ');
			for(const char *s = pi.text; *s; s++) {
				int c = byte(*s);
				if(c == 31)
					rtf.Cat("\\~");
				else
				if(c < 128 && c != '{' && c != '\\' && c != '}')
					rtf.Cat(c);
				else
					rtf.Cat(Format("\\'%2x", c));
			}
			rtf.Cat("}\r\n");
		}
	}
	if(par) rtf.Cat("\\par ");
	rtf.Cat("\r\n");
}

void RtfDocOut::Put(const Paragraph& paragraph) {
	PutPara(paragraph);
}

struct RtfDocOut::VSpan : Moveable<RtfDocOut::VSpan> {
	int x1, x2;
	int span;
};

void RtfDocOut::Put(const Table& tab) {
	const Table::Style& tst = tab.GetStyle();
	int cx = pgcx - tst.lm - tst.rm;
	Vector<VSpan> vspan;
	rtf.Cat("\\par \r\n");
	int lastrowi = tab.GetRows() - 1;
	while(lastrowi > 0) {
		if(tab[lastrowi].GetCount()) break;
		lastrowi--;
	}
	for(int rowi = 0; rowi < tab.GetRows(); rowi++) {
		int j;
		const Array<TableCell>& row = tab[rowi];
		if(row.GetCount()) {
			rtf.Cat(Format("\\trowd \\trgaph0\\trleft%d", Dow(tst.lm)));
			if(rowi < tab.GetHeaderRows())
				rtf.Cat("\\trhdr");
			rtf.Cat("\\trkeep \r\n");
			int x = 0;
			int n = GetTotalRatio(row);
			for(j = 0; j < row.GetCount(); j++) {
				const TableCell& cell = row[j];
				const TableCell::Style& st = cell.GetStyle();
				int x1 = cx * x / n + tst.lm;
				int x2 = cx * (x + row[j].GetRatio()) / n + tst.lm;
				if(cell.GetSpan() > 1) {
					VSpan& v = vspan.Add();
					v.span = cell.GetSpan();
					v.x1 = x1;
					v.x2 = x2;
					rtf.Cat("\\clvmgf");
				}
				else
					for(int i = 0; i < vspan.GetCount(); i++) {
						VSpan& v = vspan[i];
						if(v.x1 == x1 && v.x2 == x2)
							rtf.Cat("\\clvmrg");
					}
				rtf.Cat("\\clvertal");
				rtf.Cat(st.align == ALIGN_TOP ? 't' : st.align == ALIGN_CENTER ? 'c' : 'b');
				Border("clbrdrt", st.topwidth + (rowi == 0) * tst.framewidth, tst.framecolor);
				Border("clbrdrl", st.leftwidth + (j == 0) * tst.framewidth, tst.framecolor);
				Border("clbrdrb", st.bottomwidth + (rowi == lastrowi) * tst.framewidth,
					   tst.framecolor);
				Border("clbrdrr", st.rightwidth + (j == row.GetCount() - 1) * tst.framewidth,
					   tst.framecolor);
				if(GetRValue(st.paper) == GetGValue(st.paper) &&
				   GetGValue(st.paper) == GetBValue(st.paper))
					rtf.Cat(Format("\\clshdng%d", 10000 - 10000 * GetBValue(st.paper) / 255));
				else
					PutColor("clcbpat", st.paper);
				rtf.Cat(Format("\\cellx%d \n", Dow(x2)));
				x += row[j].GetRatio();
			}
			for(j = 0; j < row.GetCount(); j++) {
				const TableCell& cell = row[j];
				const TableCell::Style& st = row[j].GetStyle();
				for(int i = 0; i < cell.GetCount(); i++)
					PutPara(cell[i], i < cell.GetCount() - 1, true, st.leftspace, st.rightspace,
							(i == 0) * st.topspace, (i == cell.GetCount() - 1) * st.bottomspace);
				rtf.Cat("\\cell \r\n");
			}
			rtf.Cat("\\pard\\intbl\\row \r\n");
			for(int i = 0; i < vspan.GetCount();)
				if(--vspan[i].span <= 0)
					vspan.Remove(i);
				else
					i++;
		}
	}
	rtf.Cat("\\pard \r\n");
}

void RtfDocOut::Page() {
	page = true;
}

String RtfDocOut::Get() {
	String out =
		"{\\rtf1\\ansi\\ansicpg1250\r\n"
	       "{\\fonttbl\r\n"
				"{\\f0\\fswiss\\fcharset238\\cpg1250 Arial CE;Arial;}\r\n"
				"{\\f1\\froman\\fcharset238\\cpg1250 Times New Roman CE;Times New Roman;}\r\n"
				"{\\f2\\fmodern\\fcharset238\\cpg1250 Courier New CE;Courier New;}\r\n"
				"{\\f3\\ftech\\fcharset238\\cpg1250 Symbol;}\r\n"
				"{\\f4\\ftech\\fcharset238\\cpg1250 WingDings;}\r\n"
			"}\r\n"
			"{\\colortbl;\r\n";
	for(int i = 0; i < color.GetCount(); i++)
		out.Cat(Format("\\red%d\\green%d\\blue%d;\r\n",
		               GetRValue(color[i]), GetGValue(color[i]), GetBValue(color[i])));
	out +=  "}\n"
		    "\\f0\\pard\\plain \r\n";
	out += rtf;
	out += "}\r\n";
	return out;
}

}
