#include "PdfDraw.h"

NAMESPACE_UPP

#define LDUMP(x) // DUMP(x)
#define LLOG(x) // LOG(x)

#define PDF_COMPRESS

static const char ICC_ColorSpaceInfo[] =
"H\211\234\226yTSw\026\307\177o\311\236\220\225\260\303c\r[\200\260\006\2205la\221"
"\035\004Q\bI\b\001\022BH\330\005AD\005\024ED\204\252\2252\326mtFOE\235.\256c\255\016"
"\326}\352\322\003\3650\352\3508\264\026\327\216\235\0278G\235Ng\246\323\357\037\357"
"\3679\367w\357\357\335\337\275\367\235\363\000\240'\245\252\265\3250\v\000\215\326"
"\240\317J\214\305\026\025\024b\244\t\000\003\n \002\021\0002y\255.-;!\a\340\222\306"
"K\260Z\334\t\374\213\236^\a\220i\275\"L\312\3000\360\377\211-\327\351\r\000@\0318"
"\a(\224\265r\234;q\256\2527\350L\366\031\234y\245\225&\206Q\023\353\361\004q\2664"
"\261j\236\275\347|\3469\332\304\n\215V\201\263)g\235B\2430\361i\234W\327\031\2258"
"#\2518w\325\251\225\3658_\305\331\245\312\250Q\343\374\334\024\253Q\312j\001@\351"
"&\273A)/\307\331\017g\272>'K\202\363\002\000\310t\325;\\\372\016\033\224\r\006\323"
"\245$\325\272F\275ZUn\300\334\345\036\230(4T\214%)\353\253\224\006\2030C&\257\224"
"\351\025\230\244Z\243\223i\033\001\230\277\363\2348\246\332bx\221\203E\241\301\301"
"B\177\037\321;\205\372\257\233\277P\246\336\316\323\223\314\271\236A\374\vom?\347"
"W=\n\200x\026\257\315\372\267\266\322-\000\214\257\004\300\362\346[\233\313\373\000"
"0\361\276\035\276\370\316}\370\246y)7\030ta\276\276\365\365\365>j\245\334\307T\320"
"7\372\237\016\277@\357\274\317\307t\334\233\362`q\3122\231\261\312\200\231\352&\257"
"\256\2526\352\261Z\235L\256\304\204?\035\342_\035\370\363yxg)\313\224z\245\026\217"
"\310\303\247L\255U\341\355\326*\324\006u\265\026Sk\377S\023\177e\330O4?\327\270\270"
"c\257\001\257\330\a\260.\362\000\362\267\v\000\345\322\000R\264\r\337\201\336\364"
"-\225\222\a2\3605\337\341\336\374\334\317\t\372\367S\341>\323\243V\255\232\213\223"
"d\345`r\243\276n~\317\364Y\002\002\240\002&\340\001+`\017\234\201;\020\002\177\020"
"\002\302A4\210\a\311 \035\344\200\002\260\024\310A9\320\000=\250\a-\240\035t\201\036"
"\260\036l\002\303`;\030\003\273\301~p\020\214\203\217\301\t\360Gp\036|\t\256\201["
"`\022L\203\207`\006<\005\257 \b\"A\f\210\vYA\016\220+\344\005\371Cb(\022\212\207R"
"\241,\250\000*\201T\220\0262B-\320\n\250\a\352\207\206\241\035\320n\350\367\320Q\350"
"\004t\016\272\004}\005MA\017\240\357\240\2270\002\323a\036l\a\273\301\276\260\030"
"\216\201S\340\034x\t\254\202k\340&\270\023^\a\017\301\243\360>\3700|\002>\017_\203"
"'\341\207\360,\002\020\032\302G\034\021!\"F$H:R\210\224!z\244\025\351F\006\221Qd?"
"r\f9\213\\A&\221G\310\v\224\210rQ\f\025\242\341h\022\232\213\312\321\032\264\025\355"
"E\207\321]\350a\3644z\005\235Bg\320\327\004\006\301\226\340E\b#H\t\213\b*B=\241\213"
"0H\330I\370\210p\206p\2150MxJ$\022\371D\0011\204\230D, V\020\233\211\275\304\255\304"
"\003\304\343\304K\304\273\304Y\022\211dE\362\"E\220\322I2\222\201\324E\332B\332G\372"
"\214t\2314MzN\246\221\035\310\376\344\004r!YK\356 \017\222\367\220?%_&\337#\277\242"
"\260(\256\2240J:EAi\244\364Q\306(\307(\027)\323\224WT6U@\215\240\346P+\250\355\324"
"!\352~\352\031\352m\352\023\032\215\346D\v\245e\322\324\264\345\264!\332\357h\237"
"\323\246h/\350\034\272']B/\242\033\351\353\350\037\322\217\323\277\242?a0\030n\214"
"hF!\303\300X\307\330\3158\305\370\232\361\334\214k\346c&5S\230\265\231\215\230\035"
"6\273l\366\230Ia\2722c\230K\231M\314A\346!\346E\346#\026\205\345\306\222\260d\254"
"V\326\b\353(\353\006k\226\315e\213\330\351l\r\273\227\275\207}\216}\237C\342\270q"
"\3429\nN'\347\003\316)\316].\302u\346J\270r\356\n\356\030\367\fw\232G\344\txR^\005"
"\257\207\367[\336\004o\306\234c\036h\236g\336`>b\376\211\371$\037\341\273\361\245"
"\374*~\037\377 \377:\377\245\205\235E\214\205\322b\215\305~\213\313\026\317,m,\243"
"-\225\226\335\226\a,\257Y\276\264\302\254\342\255*\2556X\215[\335\261F\255=\2553\255"
"\353\255\267Y\237\261~d\303\263\t\267\221\333t\333\034\264\271i\v\333z\332f\3316\333"
"~`{\301v\326\316\336.\321Ng\267\305\356\224\335#{\276}\264}\205\375\200\375\247\366"
"\017\034\270\016\221\016j\207\001\207\317\034\376\212\231c1X\0256\204\235\306f\034"
"m\035\223\034\215\216;\034'\034_9\t\234r\235:\234\0168\335q\246:\213\235\313\234\a"
"\234O:\317\2708\270\244\271\264\270\354u\271\351Jq\025\273\226\273nv=\353\372\314"
"M\340\226\357\266\312m\334\355\276\300R \0254\t\366\nn\2733\334\243\334k\334G\335"
"\257z\020=\304\036\225\036[=\276\364\204=\203<\313=G</z\301^\301^j\257\255^\227\274"
"\t\336\241\336Z\357Q\357\033B\2720FX'\334+\234\362\341\373\244\372t\370\214\373<\366"
"u\361-\364\335\340{\326\367\265_\220_\225\337\230\337-\021G\224,\352\020\035\023}"
"\347\357\351/\367\037\361\277\032\300\bH\bh\v8\022\360m\240W\2402p[\340\237\203\270"
"AiA\253\202N\006\375#8$X\037\274?\370A\210KHI\310{!7\304<q\206\270W\374y(!46\264-"
"\364\343\320\027a\301a\206\260\203a\177\017\027\206W\206\357\t\277\277@\260@\271`"
"l\301\335\b\247\bY\304\216\210\311H,\262$\362\375\310\311(\307(Y\324h\3247\321\316"
"\321\212\350\235\321\367b<b*b\366\305<\216\365\213\325\307~\024\373L\022&Y&9\036\207"
"\304%\306u\307M\304s\342s\343\207\343\277NpJP%\354M\230I\fJlN<\236DHJI\332\220tCj"
"'\225KwKg\222C\222\227%\237N\241\247d\247\f\247|\223\352\231\252O=\226\006\247%\247"
"mL\273\275\320u\241v\341x:H\227\246oL\277\223!\310\250\311\370C&13#s$\363/Y\242\254"
"\226\254\263\331\334\354\342\354=\331Osbs\372rn\345\272\347\032sO\3461\363\212\362"
"v\347=\313\217\313\357\317\237\\\344\273h\331\242\363\005\326\005\352\202#\205\244"
"\302\274\302\235\205\263\213\343\027oZ<]\024T\324Ut}\211`I\303\222sK\255\227V-\375"
"\244\230Y,+>TB(\311/\331S\362\203,]6*\233-\225\226\276W:#\227\3107\313\037*\242\025"
"\003\212\a\312\be\277\362^YDY\177\331}U\204j\243\352AyT\371`\371#\265D=\254\376\266"
"\"\251b{\305\263\312\364\312\017+\177\254\312\257:\240!kJ4G\265\034m\245\366t\265"
"}uC\365%\235\227\256K7Y\023V\263\251fF\237\242\337Y\v\325.\251=b\340\341?S\027\214"
"\356\306\225\306\251\272\310\272\221\272\347\365y\365\207\032\330\r\332\206\v\215"
"\236\215k\032\3575%4\375\246\031m\2267\237lqlio\231Z\026\263lG+\324Z\332z\262\315"
"\271\255\263mzy\342\362]\355\324\366\312\366?u\370u\364w|\277\"\177\305\261N\273\316"
"\345\235wW&\256\334\333e\326\245\357\272\261*|\325\366\325\350j\365\352\2115\001k"
"\266\254y\335\255\350\376\242\307\257g\260\347\207^y\357\027kEk\207\326\376\270\256"
"l\335D_p\337\266\365\304\365\332\365\3277Dm\330\325\317\356o\352\277\2731m\343\341"
"\001l\240{\340\373M\305\233\316\r\006\016n\337L\335l\334<9\224\372O\000\244\001[\376"
"\230\270\231$\231\220\231\374\232h\232\325\233B\233\257\234\034\234\211\234\367\235"
"d\235\322\236@\236\256\237\035\237\213\237\372\240i\240\330\241G\241\266\242&\242"
"\226\243\006\243v\243\346\244V\244\307\2458\245\251\246\032\246\213\246\375\247n\247"
"\340\250R\250\304\2517\251\251\252\034\252\217\253\002\253u\253\351\254\\\254\320"
"\255D\255\270\256-\256\241\257\026\257\213\260\000\260u\260\352\261`\261\326\262K"
"\262\302\2638\263\256\264%\264\234\265\023\265\212\266\001\266y\266\360\267h\267\340"
"\270Y\270\321\271J\271\302\272;\272\265\273.\273\247\274!\274\233\275\025\275\217"
"\276\n\276\204\276\377\277z\277\365\300p\300\354\301g\301\343\302_\302\333\303X\303"
"\324\304Q\304\316\305K\305\310\306F\306\303\307A\307\277\310=\310\274\311:\311\271"
"\3128\312\267\3136\313\266\3145\314\265\3155\315\265\3166\316\266\3177\317\270\320"
"9\320\272\321<\321\276\322?\322\301\323D\323\306\324I\324\313\325N\325\321\326U\326"
"\330\327\\\327\340\330d\330\350\331l\331\361\332v\332\373\333\200\334\005\334\212"
"\335\020\335\226\336\034\336\242\337)\337\257\3406\340\275\341D\341\314\342S\342\333"
"\343c\343\353\344s\344\374\345\204\346\r\346\226\347\037\347\251\3502\350\274\351"
"F\351\320\352[\352\345\353p\353\373\354\206\355\021\355\234\356(\356\264\357@\357"
"\314\360X\360\345\361r\361\377\362\214\363\031\363\247\3644\364\302\365P\365\336\366"
"m\366\373\367\212\370\031\370\250\3718\371\307\372W\372\347\373w\374\a\374\230\375"
")\375\272\376K\376\334\377m\377\377\002\f\000\367\204\363\373";

dword PdfDraw::GetInfo() const
{
	return DOTS;
}

Size PdfDraw::GetPageSize() const
{
	return pgsz;
}

void PdfDraw::Init(int pagecx, int pagecy, int _margin, bool _pdfa)
{
	Clear();
	margin = _margin;
	pdfa = _pdfa;
	pgsz.cx = pagecx;
	pgsz.cy = pagecy;
	pgsz += margin;
	StartPage();
}

void  PdfDraw::Clear()
{
	out.Clear();
	page.Clear();
	offset.Clear();
	out << "%PDF-1.3\n";
	out << "%\xf1\xf2\xf3\xf4\n\n";
}

int PdfDraw::BeginObj()
{
	offset.Add(out.GetLength());
	out << offset.GetCount() << " 0 obj\n";
	return offset.GetCount();
}

void PdfDraw::EndObj()
{
	out << "endobj\n\n";
}

void PdfDraw::PutRect(const Rect& rc)
{
	page << Pt(rc.left) << ' ' << Pt(pgsz.cy - rc.bottom) << ' '
		<< Pt(rc.Width()) << ' ' << Pt(rc.Height()) << " re\n";
}

int PdfDraw::PutStream(const String& data, const String& keys, bool compress)
{
#ifdef PDF_COMPRESS
	if(compress) {
		String c = ZCompress(data);
		if(c.GetLength() < data.GetLength()) {
			BeginObj();
			out << "<< " << keys
			    << "/Length " << c.GetLength() << " "
			    << "/Length1 " << data.GetLength() << " "
			    << "/Filter /FlateDecode "
			    << " >>\n"
			    << "stream\r\n" << c << "\r\nendstream\n";
			EndObj();
			return offset.GetCount();
		}
	}
#endif
	BeginObj();
	out << "<< " << keys << " /Length " << data.GetLength() <<
	       " /Length1 "<< data.GetLength() << " >>\n"
	    << "stream\r\n" << data << "\r\nendstream\n";
	EndObj();
	return offset.GetCount();
}

void PdfDraw::PutrgColor(Color rg, uint64 pattern)
{
	if(IsNull(rgcolor) || rg != rgcolor || pattern != patternid) {
		if(!pattern) {
			page << PdfColor(rg) << " rg\n";
		}
		else {
			int f = patterns.FindAdd(pattern);
			if(!patternid)
				page << "/Cspat cs\n";
			page << PdfColor(rg) <<
			" /Pat" << (f + 1) << " scn\n";
		}

	}
	rgcolor = rg;
	patternid = pattern;
}

void PdfDraw::PutRGColor(Color RG)
{
	if(IsNull(RGcolor) || RG != RGcolor)
		page << PdfColor(RGcolor = RG) << " RG\n";
}

void PdfDraw::PutLineWidth(int lw)
{
	lw = max(Nvl(lw, 0), 5);
	if(linewidth != lw)
		page << Pt(linewidth = lw) << " w\n";
}

void PdfDraw::StartPage()
{
	rgcolor = RGcolor = Null;
	fontid = -1;
	patternid = 0;
	textht = Null;
	linewidth = -1;
	if(margin)
		OffsetOp(Point(margin, margin));
}

void PdfDraw::EndPage()
{
	if(margin)
		EndOp();
	PutStream(page);
	page.Clear();
}

void PdfDraw::BeginOp()
{
	page << "q\n";
}

void PdfDraw::EndOp()
{
	fontid = -1;
	patternid = 0;
	textht = Null;
	rgcolor = RGcolor = Null;
	linewidth = -1;
	page << "Q\n";
}

void PdfDraw::OffsetOp(Point p)
{
	page << "q ";
	if(p.x || p.y)
		page << "1 0 0 1 " << Pt(p.x) << ' ' << Pt(-p.y) << " cm\n";
}

bool PdfDraw::ClipOp(const Rect& r)
{
	page << "q ";
	PutRect(r);
	page << "W* n\n";
	return true;
}

bool PdfDraw::ClipoffOp(const Rect& r)
{
	page << "q ";
	PutRect(r);
	page << "W* n\n";
	if(r.left || r.top)
		page << "1 0 0 1 " << Pt(r.left) << ' ' << Pt(-r.top) << " cm\n";
	return true;
}

bool PdfDraw::ExcludeClipOp(const Rect& r)
{
	return true; // TODO
/*	
	if(r.left <= actual_clip.left && r.right >= actual_clip.right) {
		if(r.top <= actual_clip.top) actual_clip.top = max(actual_clip.top, r.bottom);
		if(r.bottom >= actual_clip.bottom) actual_clip.bottom = min(actual_clip.bottom, r.top);
	}
	if(r.top <= actual_clip.top && r.bottom >= actual_clip.bottom) {
		if(r.left <= actual_clip.left) actual_clip.left = max(actual_clip.left, r.right);
		if(r.right >= actual_clip.right) actual_clip.right = min(actual_clip.right, r.left);
	}

	PutRect(actual_clip);
	PutRect(r & actual_clip);
	page << "W* n\n";
	return !actual_clip.IsEmpty();
*/
}

bool PdfDraw::IntersectClipOp(const Rect& r)
{
	PutRect(r);
	page << "W* n\n";
	return true;
}

bool PdfDraw::IsPaintingOp(const Rect&) const
{
	return true;
}

PdfDraw::CharPos PdfDraw::GetCharPos(Font fnt, wchar chr)
{
	fnt.Underline(false);
	VectorMap<wchar, CharPos>& fc = fontchars.GetAdd(fnt);
	int q = fc.Find(chr);
	if(q >= 0)
		return fc[q];
	CharPos& p = fc.Add(chr);
	q = pdffont.FindLast(fnt);
	if(q < 0 || pdffont[q].GetCount() > 240) {
		p.fi = pdffont.GetCount();
		p.ci = 1; // PDF does not seem to like 0 character in text in some versions
		Vector<wchar>& x = pdffont.Add(fnt);
		x.Add(32);
		x.Add(chr);
	}
	else {
		p.fi = q;
		p.ci = pdffont[q].GetCount();
		pdffont[q].Add(chr);
	}
	return p;
}

void  PdfDraw::FlushText(int dx, int fi, int height, const String& txt)
{
	if(fi < 0)
		return;
	if(dx)
		page << Pt(dx) << " 0 Td ";
	PutFontHeight(fi, height);
	page << "<" << txt << "> Tj\n";
}

String PdfDraw::PdfColor(Color c)
{
	return NFormat("%3nf %3nf %3nf", c.GetR() / 255.0, c.GetG() / 255.0, c.GetB() / 255.0);
}

void PdfDraw::PutFontHeight(int fi, double ht)
{
	if(fi != fontid || IsNull(textht) || ht != textht)
		page << "/F" << ((fontid = fi) + 1) << ' ' << Pt(textht = ht) << " Tf\n";
}


PdfDraw::OutlineInfo PdfDraw::GetOutlineInfo(Font fnt)
{
	fnt.Height(0);
	int q = outline_info.Find(fnt);
	if(q >= 0)
		return outline_info[q];
	OutlineInfo of;
	of.sitalic = of.ttf = false;

	TTFReader ttf;
	if(ttf.Open(fnt.GetData(), false, true)) {
		of.ttf = true;
		of.sitalic = ttf.post.italicAngle == 0 && fnt.IsItalic();
	}

	outline_info.Add(fnt, of);

	return of;
}

enum { FONTHEIGHT_TTF = -9999 };

void PdfDraw::DrawTextOp(int x, int y, int angle, const wchar *s, Font fnt,
		                 Color ink, int n, const int *dx)
{
	if(!n) return;
	if(fnt.GetHeight() == 0)
		fnt.Height(100);
	Font ff = fnt;
	int fh = fnt.GetHeight();
	OutlineInfo of = GetOutlineInfo(fnt);
	if(of.ttf)
		fnt.Height(FONTHEIGHT_TTF);
	String txt;
	PutrgColor(ink);
	PutRGColor(ink);
	page << "BT ";
	double sina = 0, cosa = 1;
	int posx = 0;
	if(angle || dx || (of.sitalic && !fnt.IsItalic())) {
		M22 m;
		if(of.sitalic)
			m.c = 0.165;
		if(angle) {
			Draw::SinCos(angle, sina, cosa);
			m.Mul(cosa, sina, -sina, cosa);
		}
		int fi = -1;
		bool straight = (fabs(m.a - 1) <= 1e-8 && fabs(m.b) <= 1e-8 && fabs(m.c) <= 1e-8 && fabs(m.d - 1) <= 1e-8);
		Pointf prev(0, 0);
		for(int i = 0; i < n; i++) {
			Pointf next(Pt(x + posx * cosa + fround(ff.GetAscent() * sina)),
			            Pt(pgsz.cy - (y - posx * sina) - fround(ff.GetAscent() * cosa)));
			CharPos fp = GetCharPos(fnt, s[i]);
			if(fi != fp.fi) {
				fi = fp.fi;
				PutFontHeight(fi, fh);
			}
			if(straight)
				page << (next.x - prev.x) << ' ' << (next.y - prev.y) << " Td";
			else
				page << m.a << ' ' << m.b << ' ' << m.c << ' ' << m.d << ' ' << next.x << ' ' << next.y << " Tm";
			page << " <" << FormatIntHex(fp.ci, 2);
			while(i + 1 < n) {
				int cw = ff[s[i]];
				if(dx && dx[i] != cw)
					break;
				posx += cw;
				CharPos np = GetCharPos(fnt, s[i + 1]);
				if(np.fi != fp.fi)
					break;
				page << FormatIntHex(np.ci, 2);
				i++;
			}
			page << "> Tj\n";
			posx += dx ? dx[i] : ff[s[i]];
			prev = next;
		}
	}
	else {
		page << Pt(x) << " " << Pt(pgsz.cy - y - ff.GetAscent()) << " Td\n";
		int fi = -1;
		int ppos = 0;
		int np = 0;
		for(int i = 0; i < n; i++) {
			CharPos fp = GetCharPos(fnt, s[i]);
			if(fp.fi != fi) {
				FlushText(np, fi, fnt.GetHeight(), txt);
				txt.Clear();
				np = posx - ppos;
				ppos = posx;
				fi = fp.fi;
			}
			txt.Cat(Sprintf("%02.2X", fp.ci));
			posx += ff[s[i]];
		}
		FlushText(np, fi, fh, txt);
	}
	page << "ET\n";
	if(fnt.IsUnderline()) {
		int w = ff.GetAscent() / 15;
		int dy = ff.GetAscent() + max((ff.GetDescent() - w) / 2, ff.GetAscent() / 10);
		DrawLine(fround(x + sina * dy),
		         fround(y + cosa * dy),
		         fround(x + cosa * posx + sina * dy),
		         fround(y + cosa * dy - sina * posx), w, ink);
	}
}

Image RenderGlyph(int cx, int x, Font font, int chr, int py, int pcy);

PdfDraw::RGlyph PdfDraw::RasterGlyph(Font fnt, int chr)
{
	RGlyph rg;
	FontInfo fi = fnt.Info();
	rg.x = 0;
	rg.sz.cx = fi[chr];
	rg.sz.cy = fi.GetHeight();
	int l = fi.GetLeftSpace(chr);
	if(l < 0) {
		rg.x = -l;
		rg.sz.cx -= l;
	}
	int r = fi.GetRightSpace(chr);
	if(r < 0)
		rg.sz.cx -= r;
	RasterFormat fmt;
	fmt.Set1mf();
	int linebytes = fmt.GetByteCount(rg.sz.cx);
	Buffer<byte> ob(linebytes);
	int y = 0;
	while(y < rg.sz.cy) {
		int ccy = min(16, rg.sz.cy - y);
		Image m = RenderGlyph(rg.sz.cx, rg.x, fnt, chr, y, ccy);
		for(int i = 0; i < m.GetHeight(); i++) {
			fmt.Write(ob, m[i], rg.sz.cx, NULL);
			rg.data.Cat((const char *)~ob, linebytes);
		}
		y += ccy;
	}
	return rg;
}

void PdfDraw::DrawRectOp(int x, int y, int cx, int cy, Color color)
{
	if(IsNull(color) || cx <= 0 || cy <= 0) return;
	PutrgColor(color);
	PutRGColor(color);
	PutRect(RectC(x, y, cx, cy));
	page << "f\n";
}

void PdfDraw::DrawLineOp(int x1, int y1, int x2, int y2, int width, Color color)
{
	if(!IsNull(width)) {
		PutRGColor(color);
		PutLineWidth(width);
		page << " 1 J "
		     << Pt(x1) << ' ' << Pt(pgsz.cy - y1) << " m "
		     << Pt(x2) << ' ' << Pt(pgsz.cy - y2) << " l S\n";
	}
}

void PdfDraw::DrawImageOp(int x, int y, int cx, int cy, const Image& _img, const Rect& src, Color c)
{
	Image img = _img;
	if(!IsNull(c))
		img = SetColorKeepAlpha(img, c);
	image.Add(img);
	imagerect.Add(src);
	page << "q "
	     << Pt(cx) << " 0 0 " << Pt(cy) << ' '
	     << Pt(x) << ' ' << Pt(pgsz.cy - y - cy)
	     << " cm /Image" << image.GetCount() << " Do Q\n";
}

void PdfDraw::DrawPolyPolylineOp(const Point *vertices, int vertex_count,
	                    const int *counts, int count_count,
	                    int width, Color color, Color doxor)
{
	if(IsNull(color) || IsNull(width))
		return;
	PutRGColor(color);
	PutLineWidth(width);
	while(--count_count >= 0) {
		int part = *counts++;
		page << Pt(vertices->x) << ' ' << Pt(pgsz.cy - vertices->y) << " m\n";
		vertices++;
		for(; --part > 0; vertices++)
			page << Pt(vertices->x) << ' ' << Pt(pgsz.cy - vertices->y) << " l\n";
		page << "S\n";
	}
}

void PdfDraw::DrawEllipseOp(const Rect& r, Color color, int pen, Color outline)
{
	bool fill = !IsNull(color), stroke = !IsNull(outline) && !IsNull(pen);
	if(fill) PutrgColor(color);
	if(stroke) { PutRGColor(outline); PutLineWidth(pen); }
	if(!fill && !stroke)
		return;
	char closeop = (stroke && fill ? 'B' : fill ? 'f' : 'S');
	int sizelim = r.Width() | r.Height();
	int bits = 1;
	while(bits < 20 && sizelim > (1 << bits))
		bits++;
	int parts = 3 + max(((bits * (bits - 6)) >> 3) | 1, 1);
	double k = 2 * M_PI / 3 / parts;
	Pointf center(Pt((r.left + r.right) / 2.0), Pt(pgsz.cy - ((r.top + r.bottom) / 2.0)));
	Pointf size(Pt(r.Width() / 2.0), Pt(r.Height() / 2.0));
	for(int partid = 0; partid < parts; partid++) {
		double phi1 = (partid + 0) * (2 * M_PI / parts);
		double phi2 = (partid + 1) * (2 * M_PI / parts);
		double s1 = sin(phi1), c1 = cos(phi1), s2 = sin(phi2), c2 = cos(phi2);
		Pointf A = center + Pointf(c1, s1) * size;
		Pointf D = center + Pointf(c2, s2) * size;
		Pointf dA = Pointf(-s1, c1) * size;
		Pointf dD = Pointf(-s2, c2) * size;
		Pointf B = A + k * dA;
		Pointf C = D - k * dD;
		if(!partid)
			page << FormatDoubleFix(A.x, 2) << ' ' << FormatDoubleFix(A.y, 2) << " m\n";
		page << FormatDoubleFix(B.x, 2) << ' ' << FormatDoubleFix(B.y, 2) << ' '
			<< FormatDoubleFix(C.x, 2) << ' ' << FormatDoubleFix(C.y, 2) << ' '
			<< FormatDoubleFix(D.x, 2) << ' ' << FormatDoubleFix(D.y, 2) << " c\n";
	}
	page << closeop << '\n';
}

void PdfDraw::DrawArcOp(const Rect& rc, Point start, Point end, int width, Color color)
{
	//TODO!!
	NEVER();
}

void PdfDraw::DrawPolyPolyPolygonOp(const Point *vertices, int vertex_count,
	const int *subpolygon_counts, int subpolygon_count_count,
	const int *disjunct_polygon_counts, int disjunct_polygon_count_count,
	Color color, int width, Color outline, uint64 pattern, Color doxor)
{
	bool fill = !IsNull(color) && ~pattern, stroke = !IsNull(outline) && !IsNull(width);
	if(fill)   PutrgColor(color, pattern);
	if(stroke) { PutRGColor(outline); PutLineWidth(width); }
	if(!fill && !stroke) return;
	const char *closeop = (fill & stroke ? "B*" : fill ? "f*" : "S");
	while(--disjunct_polygon_count_count >= 0) {
		int disj = *disjunct_polygon_counts++;
		while(disj > 0) {
			int sub = *subpolygon_counts++;
			disj -= sub;
			page << Pt(vertices->x) << ' ' << Pt(pgsz.cy - vertices->y) << " m\n";
			vertices++;
			for(; --sub > 0; vertices++)
				page << Pt(vertices->x) << ' ' << Pt(pgsz.cy - vertices->y) << " l\n";
			page << "h\n";
		}
		page << closeop << '\n';
	}
}

String GetMonoPdfImage(const Image& m, const Rect& sr)
{
	String data;
	for(int y = sr.top; y < sr.bottom; y++) {
		const RGBA *p = m[y] + sr.left;
		const RGBA *e = m[y] + sr.right;
		while(p < e) {
			int bit = 0x80;
			byte b = 0;
			while(bit && p < e) {
				if(!((p->r | p->g | p->b) == 0 || (p->r & p->g & p->b) == 255))
					return Null;
				b |= bit & p->r;
				bit >>= 1;
				p++;
			}
			data.Cat(b);
		}
	}
	return data;
}

String GetGrayPdfImage(const Image& m, const Rect& sr)
{
	String data;
	for(int y = sr.top; y < sr.bottom; y++) {
		const RGBA *p = m[y] + sr.left;
		const RGBA *e = m[y] + sr.right;
		while(p < e)
			if(p->r == p->g && p->g == p->b)
				data.Cat((p++)->r);
			else
				return Null;
	}
	return data;
}

String PdfDraw::Finish()
{
	if(page.GetLength())
		PutStream(page);

	int pagecount = offset.GetCount();

	Vector<int> imageobj;
	for(int i = 0; i < image.GetCount(); i++) {
		Size sz = image[i].GetSize();
		Rect sr = sz & imagerect[i];
		String data;
		String wh;
		wh << " /Width " << sr.Width() << " /Height " << sr.Height();
		const Image& m = image[i];
		int mask = -1;
		int smask = -1;
		if(m.GetKind() == IMAGE_MASK) {
			for(int y = sr.top; y < sr.bottom; y++) {
				const RGBA *p = m[y] + sr.left;
				const RGBA *e = m[y] + sr.right;
				while(p < e) {
					int bit = 0x80;
					byte b = 0;
					while(bit && p < e) {
						if(p->a != 255)
							b |= bit;
						bit >>= 1;
						p++;
					}
					data.Cat(b);
				}
			}
			mask = PutStream(data, String().Cat()
			                    << "/Type /XObject /Subtype /Image" << wh
				                << " /BitsPerComponent 1 /ImageMask true /Decode [0 1] ");
		}
		if(m.GetKind() == IMAGE_ALPHA) {
			for(int y = sr.top; y < sr.bottom; y++) {
				const RGBA *p = m[y] + sr.left;
				const RGBA *e = m[y] + sr.right;
				while(p < e)
					data.Cat((p++)->a);
			}
			smask = PutStream(data, String().Cat()
			                    << "/Type /XObject /Subtype /Image" << wh
				                << " /BitsPerComponent 8 /ColorSpace /DeviceGray /Decode [0 1] ");
		}
		data.Clear();
		for(int y = sr.top; y < sr.bottom; y++) {
			const RGBA *p = m[y] + sr.left;
			const RGBA *e = m[y] + sr.right;
			while(p < e) {
				data.Cat(p->r);
				data.Cat(p->g);
				data.Cat(p->b);
				p++;
			}
		}
		String imgobj;
		data = GetMonoPdfImage(m, sr);
		if(data.GetCount())
			imgobj << "/Type /XObject /Subtype /Image" << wh
			       << " /BitsPerComponent 1 /Decode [0 1] /ColorSpace /DeviceGray ";
		else {
			data = GetGrayPdfImage(m, sr);
			if(data.GetCount())
				imgobj << "/Type /XObject /Subtype /Image" << wh
				       << " /BitsPerComponent 8 /ColorSpace /DeviceGray /Decode [0 1] ";
			else {
				data.Clear();
				for(int y = sr.top; y < sr.bottom; y++) {
					const RGBA *p = m[y] + sr.left;
					const RGBA *e = m[y] + sr.right;
					while(p < e) {
						data.Cat(p->r);
						data.Cat(p->g);
						data.Cat(p->b);
						p++;
					}
				}
				imgobj << "/Type /XObject /Subtype /Image" << wh
				       << " /BitsPerComponent 8 /ColorSpace /DeviceRGB /Intent /Perceptual";
			}
		}
		if(mask >= 0)
			imgobj << " /Mask " << mask << " 0 R";
		if(smask >= 0)
			imgobj << " /SMask " << smask << " 0 R";
		imageobj << PutStream(data, imgobj);
	}

	int patcsobj = -1;
	int patresobj = -1;
	if(!patterns.IsEmpty()) {
		patcsobj = BeginObj();
		out << "[/Pattern /DeviceRGB]\n";
		EndObj();
		patresobj = BeginObj();
		out << "<< >>\n";
		EndObj();
	}
	
	Vector<int> patternobj;
	patternobj.SetCount(patterns.GetCount(), -1);
	for(int i = 0; i < patterns.GetCount(); i++) {
		uint64 pat = patterns[i];
		StringBuffer ptk;
		ptk << 
		"/Type /Pattern\n"
		"/PatternType 1\n"
		"/PaintType 2\n"
		"/TilingType 3\n"
		"/BBox [-1 -1 9 9]\n"
		"/XStep 8\n"
		"/YStep 8\n"
		"/Resources " << patresobj << " 0 R\n"
		"/Matrix [0.75 0.0 0.0 0.75 0.0 0.0]\n" // pattern pixels -> dots
		;
		StringBuffer ptd;
		for(int y = 0; y < 8; y++) {
			for(int x = 0; x < 8; x++) {
				int b = 8 * y + x, e, lim;
				if(!((pat >> b) & 1)) {
					e = 0;
					lim = 8 - x;
					while(++e < lim && !((pat >> (b + e)) & 1))
						pat |= (uint64(1) << (b + e));
					if(e > 1)
						ptd << x << ' ' << (7 - y) << ' ' << e << " 1 re f\n";
					else {
						e = 0;
						lim = 8 - y;
						while(++e < lim && !((pat >> (b + 8 * e)) & 1))
							pat |= (uint64(1) << (b + 8 * e));
						if(e - y > 1)
							ptd << x << ' ' << (7 - y - e) << " 1 " << e << " re f\n";
						else {
							e = 0;
							lim = 8 - max(x, y);
							while(++e < lim && !((pat >> (b + 9 * e)) & 1))
								pat |= (uint64(1) << (b + 9 * e));
							if(e > 1) {
								ptd
								<< FormatDouble(x - 0.25, 2) << " "
								<< FormatDouble(7.75 - y, 2) << " m\n"
								<< FormatDouble(x + 0.25, 2) << " "
								<< FormatDouble(8.25 - y, 2) << " l\n"
								<< FormatDouble(x + e + 0.25, 2) << " "
								<< FormatDouble(8.25 - y - e, 2) << " l\n"
								<< FormatDouble(x + e - 0.25, 2) << " "
								<< FormatDouble(7.75 - y - e, 2) << " l\n"
								<< "f\n";
							}
							else {
								e = 0;
								lim = 8 - max(7 - x, y);
								while(++e < lim && !((pat >> (b + 7 * e)) & 1))
									pat |= (uint64(1) << (b + 7 * e));
								if(e > 1) {
									ptd
									<< FormatDouble(x + 1.25, 2) << " "
									<< FormatDouble(7.75 - y, 2) << " m\n"
									<< FormatDouble(x + 0.75, 2) << " "
									<< FormatDouble(8.25 - y, 2) << " l\n"
									<< FormatDouble(x - e + 0.75, 2) << " "
									<< FormatDouble(8.25 - y - e, 2) << " l\n"
									<< FormatDouble(x - e + 1.25, 2) << " "
									<< FormatDouble(7.75 - y - e, 2) << " l\n"
									<< "f\n";
								}
								else {
									ptd << x << ' ' << (7 - y) << " 1 1 re f\n";
								}
							}
						}
					}
				}
			}
		}
		patternobj[i] = PutStream(ptd, ptk);
		
	}

/*
	Vector<int>  rgobj;
	Vector<Size> rgsz;
	Vector<int>  rgx;
	for(int i = 0; i < pdffont.GetCount(); i++) {
		Font fnt = pdffont.GetKey(i);
		if(fnt.GetHeight()) {
			for(int c = 0; c < pdffont[i].GetCount(); c++) {
				RGlyph rg = RasterGlyph(fnt, pdffont[i][c]);
				int ii = rgobj.GetCount();
				rgobj << PutStream(rg.data, String().Cat()
				                   << "/Type /XObject /Subtype /Image "
				                   << " /Width " << rg.sz.cx << " /Height " << rg.sz.cy
					               << " /BitsPerComponent 1 /ImageMask true /Decode [0 1]");
				rgsz.Add(rg.sz);
				rgx.Add(rg.x);
			}
		}
	}
*/
	Vector<int> fontobj;
	for(int i = 0; i < pdffont.GetCount(); i++) {
		Font fnt = pdffont.GetKey(i);
		const Vector<wchar>& cs = pdffont[i];
		String cmap;
		cmap <<
			"/CIDInit /ProcSet findresource begin\n"
			"12 dict begin\n"
			"begincmap\n"
			"/CIDSystemInfo\n"
			"<< /Registry (Adobe)\n"
			"/Ordering (UCS)\n"
			"/Supplement 0\n"
			">> def\n"
			"/CMapName /UCS" << i << " def\n"
			"/CMapType 2 def\n"
			"1 begincodespacerange\n"
			"<00> <" << FormatIntHex(cs.GetCount() - 1, 2) << ">\n"
			"endcodespacerange\n"
			"1 beginbfrange\n"
			"<00> <" << FormatIntHex(cs.GetCount() - 1, 2) << ">\n"
			"[\n";
		for(int c = 0; c < cs.GetCount(); c++)
			cmap << '<' << FormatIntHex(cs[c], 4) << ">\n";
		cmap <<
			"]\n"
			"endbfrange\n"
			"endcmap\n"
			"CMapName currentdict /CMap defineresource pop\n"
			"end\n"
			"end\n";
		int cmapi = PutStream(cmap);
		if(fnt.GetHeight() != FONTHEIGHT_TTF) {
			FontInfo fi = fnt.Info();
			int t3ch = offset.GetCount() + 1;
			int fa = fi.GetHeight() - fi.GetInternal();
			String res;
			for(int c = 0; c < cs.GetCount(); c++) {
				RGlyph rg = RasterGlyph(fnt, pdffont[i][c]);
				String proc;
				proc
					<< 1000 * fi[cs[c]] / fa << " 0 0 "
					<< -1000 * fi.GetDescent() / fa << ' '
					<< 1000 * (rg.sz.cx + rg.x) / fa << ' '
					<< 1000 * rg.sz.cy / fa
					<< " d1\nq "
					<< 1000 * rg.sz.cx / fa
					<< " 0 0 "
					<< 1000 * rg.sz.cy / fa
					<< " "
					<< -1000 * rg.x / fa
					<< " "
					<< -1000 * fi.GetDescent() / fa
					<< " cm BI /W " << rg.sz.cx << " /H " << rg.sz.cy
					<< " /BPC 1 /IM true /D [0 1] ID\n"
					<< rg.data
					<< "\nEI Q"
				;
				PutStream(proc);
			}
			int charprocs = BeginObj();
			out << "<<";
			for(int c = 0; c < cs.GetCount(); c++)
				out << " /Rgch" << c << ' ' << t3ch + c<< " 0 R";
			out << " >>\n";
			EndObj();
			int encoding = BeginObj();
			out << "<< /Type /Encoding /Differences [0";
			for(int c = 0; c < cs.GetCount(); c++)
				out << " /Rgch" << c;
			out << "] >>\n";
			EndObj();
			fontobj.Add() = BeginObj();
			out <<
				"<< /Name /F" << i + 1 <<
				" /Type /Font\n"
				"/Subtype /Type3\n"
				"/FontBBox [0 0 0 0]\n"
				"/FontMatrix [0.001 0 0 0.001 0 0]\n"
				"/CharProcs " << charprocs << " 0 R\n"
				"/Encoding " << encoding << " 0 R\n"
				"/FirstChar 0\n"
				"/LastChar " << cs.GetCount() - 1 << "\n"
				"/Widths [";
			for(int i = 0; i < cs.GetCount(); i++)
				out << ' ' << 1000 * fi[cs[i]] / fa;
			out <<
				"]\n";
			out << "/Resources << /ProcSet [ /PDF /Text /ImageB  ] >>\n"
			    << "/FirstChar 0 /LastChar " << cs.GetCount() - 1 <<" /ToUnicode "
				<< cmapi
				<< " 0 R\n>>\n";
			EndObj();
		}
		else {
			String fontbuffer;
			fontbuffer = pdffont.GetKey(i).GetData();

			TTFReader ttf;
			if(!ttf.Open(fontbuffer))
				return Null;

			String name = FormatIntAlpha(i + 1, true);
			name.Cat('A', 6 - name.GetLength());
			name << '+' << ttf.ps_name;

			int fonti = PutStream(ttf.Subset(cs));

			BeginObj();
			int ascent = ttf.hhea.ascent * 1000 / ttf.head.unitsPerEm;
			int descent = ttf.hhea.descent * 1000 / ttf.head.unitsPerEm;
			out <<
				"<< /Type /FontDescriptor\n"
				"/FontName /" << name << "\n"
				"/Flags 4\n"
				"/FontBBox [ -1000 " << descent << " 3000 " << ascent << " ]\n" //?????
				"/ItalicAngle " << ttf.post.italicAngle / 65536.0 << "\n"
				"/Ascent " << ascent << "\n"
				"/Descent " << -descent << "\n"
				"/CapHeight " << ttf.hhea.ascent * 1000 / ttf.head.unitsPerEm << "\n"
				"/StemV 80\n"
				"/FontFile2 " << fonti << " 0 R\n" <<
				">>\n";
			EndObj();

			fontobj.Add() = BeginObj();
			out <<
				"<< /Type /Font\n"
				"/Subtype /TrueType\n"
				"/BaseFont /" << name << "\n"
				"/FirstChar 0\n"
				"/LastChar " << cs.GetCount() - 1 << "\n"
				"/Widths [ ";
			for(int i = 0; i < cs.GetCount(); i++)
				out << ttf.GetAdvanceWidth(cs[i]) * 1000 / ttf.head.unitsPerEm << ' ';
			out <<
				"]\n"
				"/FontDescriptor " << fonti + 1 << " 0 R\n"
				"/ToUnicode " << cmapi << " 0 R\n" <<
				">>\n";
			EndObj();
		}
	}

	int fonts = BeginObj();
	out << "<<\n";
	for(int i = 0; i < pdffont.GetCount(); i++)
		out << "/F" << i + 1 << ' ' << fontobj[i] << " 0 R \n";
	out << ">>\n";
	EndObj();
	
	int dflt_rgb_def = -1;
	if(pdfa) {
		int icc_info = BeginObj();
		static const int data_len = __countof(ICC_ColorSpaceInfo) - 1;
		out << "<</Length " << data_len << "/Filter/FlateDecode/N 3>>\n"
		"stream\n";
		out.Cat(ICC_ColorSpaceInfo, data_len);
		out << "\nendstream\n";
		EndObj();
		
		dflt_rgb_def = BeginObj();
		out << "[/ICCBased " << icc_info << " 0 R]\n";
		EndObj();
	}
	
	int resources = BeginObj();
	out << "<< /Font " << fonts << " 0 R\n"
	"/ProcSet [ /PDF /Text /ImageB /ImageC ]\n";
	if(imageobj.GetCount()) {
		out << "/XObject << ";
		for(int i = 0; i < imageobj.GetCount(); i++)
			out << "/Image" << (i + 1) << ' ' << imageobj[i] << " 0 R ";
		out << ">>\n";
	}
	if(!patternobj.IsEmpty()) {
		out << 
		"/ColorSpace << /Cspat " << patcsobj << " 0 R >>\n"
		"/Pattern << ";
		for(int i = 0; i < patterns.GetCount(); i++) 
			out << "/Pat" << (i + 1) << ' ' << patternobj[i] << " 0 R ";
		out << ">>\n";
	}
	if(pdfa) {
		out << "/ColorSpace <</DefaultRGB " << dflt_rgb_def << " 0 R>>\n";
	}
	out << ">>\n";
	EndObj();

	int pages = BeginObj();
	out << "<< /Type /Pages\n"
	    << "/Kids [";
	for(int i = 0; i < pagecount; i++)
		out << i + pages + 1 << " 0 R ";
	out << "]\n"
	    << "/Count " << pagecount << "\n";
	out << ">>\n";
	EndObj();
	for(int i = 0; i < pagecount; i++) {
		BeginObj();
		out << "<< /Type /Page\n"
		    << "/Parent " << pages << " 0 R\n"
		    << "/MediaBox [0 0 " << Pt(pgsz.cx) << ' ' << Pt(pgsz.cy) << "]\n"
		    << "/Contents " << i + 1 << " 0 R\n"
		    << "/Resources " << resources << " 0 R \n";
		out << ">>\n";
		EndObj();
	}
	int outlines = BeginObj();
	out << "<< /Type /Outlines\n"
	       "/Count 0\n"
	       ">>\n";
	EndObj();
	int pdfa_metadata = -1;
	if(pdfa) {
		StringBuffer metadata;
		metadata <<
		"<?xpacket id=\"" << Uuid::Create() << "\"?>\n"
		"<x:xmpmeta xmlns:x=\"adobe:ns:meta/\" x:xmptk=\"PDFNet\">\n"
		"<rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">\n"
		"<rdf:Description rdf:about=\"\"\n"
		"xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\">\n"
		"<xmp:CreateDate>0000-01-01</xmp:CreateDate>\n"
		"<xmp:ModifyDate>0000-01-01</xmp:ModifyDate>\n"
		"<xmp:CreatorTool/>\n"
		"</rdf:Description>\n"
		"<rdf:Description rdf:about=\"\"\n"
		"xmlns:dc=\"http://purl.org/dc/elements/1.1/\">\n"
		"<dc:title>\n"
		"<rdf:Alt>\n"
		"<rdf:li xml:lang=\"x-default\"/>\n"
		"</rdf:Alt>\n"
		"</dc:title>\n"
		"<dc:creator>\n"
		"<rdf:Seq>\n"
		"<rdf:li/>\n"
		"</rdf:Seq>\n"
		"</dc:creator>\n"
		"<dc:description>\n"
		"<rdf:Alt>\n"
		"<rdf:li xml:lang=\"x-default\"/>\n"
		"</rdf:Alt>\n"
		"</dc:description>\n"
		"</rdf:Description>\n"
		"<rdf:Description rdf:about=\"\"\n"
		"xmlns:pdf=\"http://ns.adobe.com/pdf/1.3/\">\n"
		"<pdf:Keywords/>\n"
		"<pdf:Producer/>\n"
		"</rdf:Description>\n"
		"<rdf:Description rdf:about=\"\"\n"
		"xmlns:pdfaid=\"http://www.aiim.org/pdfa/ns/id/\">\n"
		"<pdfaid:part>1</pdfaid:part>\n"
		"<pdfaid:conformance>B</pdfaid:conformance>\n"
		"</rdf:Description>\n"
		"</rdf:RDF>\n"
		"</x:xmpmeta>\n";
		
		StringBuffer meta_head;
		meta_head << "/Type/Metadata/Subtype/XML";
		
		pdfa_metadata = PutStream(metadata, meta_head, false);
	}
	
	int catalog = BeginObj();
	out << "<< /Type /Catalog\n"
	    << "/Outlines " << outlines << " 0 R\n"
	    << "/Pages " << pages << " 0 R\n";
	
	if(pdfa_metadata >= 0) {
		out << "/Metadata " << pdfa_metadata << " 0 R\n";
	}
	
	out << ">>\n";
	EndObj();
	int startxref = out.GetCount();
	out << "xref\n"
	    << "0 " << offset.GetCount() + 1 << "\n";
	out << "0000000000 65535 f\r\n";
	for(int i = 0; i < offset.GetCount(); i++)
		out << Sprintf("%010d 00000 n\r\n", offset[i]);
	out << "\n"
	    << "trailer\n"
	    << "<< /Size " << offset.GetCount() + 1 << "\n"
	    << "/Root " << catalog << " 0 R\n"
		<< "/ID [ <" << Uuid::Create() << "> <" << Uuid::Create() << "> ]\n"
	    << ">>\n"
	    << "startxref\r\n"
	    << startxref << "\r\n"
	    << "%%EOF\r\n";
	return out;
}

END_UPP_NAMESPACE
