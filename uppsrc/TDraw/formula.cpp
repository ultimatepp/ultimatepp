#include <TDraw/TDraw.h>
#pragma hdrstop

#include "formula.h"

NAMESPACE_UPP

#define LLOG(x)  // RLOG(x)
#define LDUMP(x) // RDUMP(x)

#ifdef _DEBUG
	#define SHOW_BOXES 0 // 1 = on, 0 = off
#else
	#define SHOW_BOXES 0
#endif

static Point Bezier(Point A, Point B, Point C, double t)
{
	double s = 1 - t;
	return Point(
		fround(A.x * s * s + B.x * 2 * t * s + C.x * t * t),
		fround(A.y * s * s + B.y * 2 * t * s + C.y * t * t));
}

static void Bezier(Vector<Point>& out, Point A, Point B, Point C, int n)
{
	if(out.IsEmpty() || out.Top() != A)
		out.Add(A);
	for(int i = 0; i <= n; i++)
		out.Add(Bezier(A, B, C, i / double(n)));
	if(out.Top() != C)
		out.Add(C);
}

static void Bezier(Vector<Point>& out, const Vector<Point>& in, int n)
{
	for(int i = 2; i < in.GetCount(); i += 2)
		Bezier(out, in[i - 2], in[i - 1], in[i - 0], n);
}

static void Rectangle(Vector<Point>& out, Rect rc)
{
	out << rc.TopLeft() << rc.TopRight() << rc.BottomRight() << rc.BottomLeft();
}

static void BezierEllipse(Vector<Point>& out, Point C, Size radius, int n)
{
	Vector<Point> temp;
	temp
		<< Point(C.x + radius.cx, C.y)
			<< Point(C.x + radius.cx, C.y + radius.cy)
		<< Point(C.x, C.y + radius.cy)
			<< Point(C.x - radius.cx, C.y + radius.cy)
		<< Point(C.x - radius.cx, C.y)
			<< Point(C.x - radius.cx, C.y - radius.cy)
		<< Point(C.x, C.y - radius.cy)
			<< Point(C.x + radius.cx, C.y - radius.cy)
		<< Point(C.x + radius.cx, C.y);
	Bezier(out, temp, n);
}

int SymbolWidth(FORMSYMBOL symbol, int height, int textheight)
{
	int wd = max(Percent(20, textheight), 4);
	int lw = Formula::GetLineWidth(textheight) >> 2;
	switch(symbol)
	{
	default:        NEVER();
	case FS_EMPTY:        return 0;
	case FS_LSHARP:
	case FS_RSHARP:       wd = textheight / 4; break;
	case FS_LBRACE:
	case FS_RBRACE:
	case FS_LPAREN:
	case FS_RPAREN:
	case FS_LBRACKET:
	case FS_RBRACKET:
	case FS_LTBRACKET:
	case FS_RTBRACKET:
	case FS_LBBRACKET:
	case FS_RBBRACKET:    break;
	case FS_VBAR:         wd = textheight / 8; break;
	case FS_SUM:
	case FS_PRODUCT:
	case FS_UNION:
	case FS_INTERSECTION: wd = (height * 10) >> 4; break;
	case FS_INT:          wd = lw + height / 8 + 1 * (textheight / 4); break;
	case FS_IINT:         wd = lw + height / 8 + 2 * (textheight / 4); break;
	case FS_IIINT:        wd = lw + height / 8 + 3 * (textheight / 4); break;
	case FS_OINT:         wd = lw + height / 8 + 2 * (textheight / 4); break;
	case FS_OIINT:        wd = lw + height / 8 + 3 * (textheight / 4); break;
	case FS_LOG_NOT:      wd = Percent(40, textheight); break;
	case FS_LOG_OR:       wd = Percent(60, textheight); break;
	case FS_MEMBER: case FS_NOT_MEMBER:
	case FS_SUBSET: case FS_NOT_SUBSET:
	case FS_PROPER_SUBSET: case FS_NOT_PROPER_SUBSET:
		wd = Percent(30, textheight);
		break;
	}
	return wd;
}

void PaintSymbol(Draw& draw, FORMSYMBOL symbol, const Rect& rc, int textheight, Color color)
{
//	draw.DrawRect(rc, LtMagenta);
	int wd = rc.Width(), ht = rc.Height();
	int thick = Formula::GetLineWidth(textheight) >> 1;
	int half = thick >> 1;
	Vector<Point> polygon;
	Vector<int> counts;

	bool xmirror = false;

	if(symbol > FS_BRACKET_BEGIN && symbol < FS_BRACKET_END) {
		xmirror = (symbol == FS_RBRACKET || symbol == FS_RTBRACKET || symbol == FS_RBBRACKET);
		bool top = (symbol != FS_LBBRACKET && symbol != FS_RBBRACKET);
		bool bot = (symbol != FS_LTBRACKET && symbol != FS_RTBRACKET);
		polygon
			<< Point(0, 0)
			<< Point(0, ht);
		if(bot)
			polygon
				<< Point(wd, ht)
				<< Point(wd, ht - half)
				<< Point(thick, ht - half);
		else
			polygon
				<< Point(thick, ht);
		if(top)
			polygon
				<< Point(thick, half)
				<< Point(wd, half)
				<< Point(wd, 0);
		else
			polygon
				<< Point(thick, 0);
	}
	else if(symbol == FS_LPAREN || symbol == FS_RPAREN || symbol == FS_LBRACE || symbol == FS_RBRACE) {
		bool bra = (symbol == FS_LBRACE || symbol == FS_RBRACE);
		xmirror = (symbol == FS_RPAREN || symbol == FS_RBRACE);
		int xo, xv, xs;
		int yo, yv, yw, yi, yj, ys;
		xo = wd;
		xs = xv = thick;
		yo = 0;
		yi = yj = ys = ht >> 1;
		yw = min(textheight / 3, yi);
		if(bra) {
			xv = (xv + xo) >> 1;
			yi = max(Percent(42, ht), ys - textheight / 5);
			yj = (yi + ys) >> 1;
			yw /= 3;
		}
		yv = yw >> 1;
		enum { PREC = 6 };
		Bezier(polygon, Point(xo, yo), Point(xv, yv), Point(xv, yw), PREC);
		if(bra) {
			Bezier(polygon, Point(xv, yi), Point(xv, yj), Point(xs, ys), PREC);
			Bezier(polygon, Point(xs, ht - ys), Point(xv, ht - yj), Point(xv, ht - yi), PREC);
		}
		Bezier(polygon, Point(xv, ht - yw), Point(xv, ht - yv), Point(xo, ht - yo), PREC);
		xo -= half;
		xv -= thick;
		xs -= thick;
		Bezier(polygon, Point(xo, ht - yo), Point(xv, ht - yv), Point(xv, ht - yw), PREC);
		if(bra) {
			Bezier(polygon, Point(xv, ht - yi), Point(xv, ht - yj), Point(xs, ht - ys), PREC);
			Bezier(polygon, Point(xs, ys), Point(xv, yj), Point(xv, yi), PREC);
		}
		Bezier(polygon, Point(xv, yw), Point(xv, yv), Point(xo, yo), PREC);
	}
	else if(symbol == FS_LSHARP || symbol == FS_RSHARP) {
	}
	else if(symbol == FS_VBAR) {
		int lw = Formula::GetLineWidth(textheight);
		draw.DrawRect((rc.left + rc.right - lw) >> 1, rc.top, lw, rc.Height(), color);
		return;
	}
	else if(symbol == FS_SUM) {
		int tt = (thick * 20) >> 4;
		int cx = (wd * 8) >> 4;
		int hh = ht >> 1;
		int dx = tt + iscale(thick, cx, hh);
		polygon
			<< Point(wd, 0) << Point(0, 0)
				<< Point(cx, hh)
			<< Point(0, ht) << Point(wd, ht)
			<< Point(wd, ht - thick) << Point(dx, ht - thick)
				<< Point(cx + tt, hh)
			<< Point(dx, thick) << Point(wd, thick);
	}
	else if(symbol == FS_PRODUCT) {
	}
	else if(symbol == FS_UNION) {
	}
	else if(symbol == FS_INTERSECTION) {
	}
	else if(symbol > FS_INT_BEGIN && symbol < FS_INT_END) {
		int count = (symbol == FS_IIINT ? 3 : symbol == FS_IINT || symbol == FS_OIINT ? 2 : 1);
		int lw = thick;
		int rcap = min(textheight / 12, (wd - lw) / (2 * count)), dcap = 2 * rcap, vcap = rcap;
		int capdist = (dcap * 25) >> 4;
		int dcl = dcap - lw, rcl = dcl >> 1;
		int slant = wd - capdist * (count - 1);
		int comp = dcap - iscale(rcap, slant - 2 * dcap + lw, ht - 2 * rcap);
		for(int i = 0; i < count; i++) {
			int x = i * capdist;
			int n = polygon.GetCount();
			BezierEllipse(polygon, Point(x + rcap, ht - rcap), Size(rcap, vcap), 4);
			counts.Add(polygon.GetCount() - n);
			n = polygon.GetCount();
			BezierEllipse(polygon, Point(x + slant - rcap, rcap), Size(rcap, vcap), 4);
			counts.Add(polygon.GetCount() - n);
			n = polygon.GetCount();
			polygon << Point(x + slant - dcap, rcap) << Point(x + slant - dcl, rcap)
				<< Point(x + dcap, ht - rcap) << Point(x + dcl, ht - rcap);
			counts.Add(polygon.GetCount() - n);
		}
		if(symbol == FS_OINT || symbol == FS_OIINT) {
			int n = polygon.GetCount();
			int rad = min(wd >> 1, count * textheight / 4);
			BezierEllipse(polygon, Point(wd >> 1, ht >> 1), Size(rad, rad), 8);
			BezierEllipse(polygon, Point(wd >> 1, ht >> 1), Size(rad - lw, -rad + lw), 8);
			counts.Add(polygon.GetCount() - n);
		}
	}
	else if(symbol == FS_LOG_NOT) {
		polygon << Point(0, 0) << Point(wd, 0) << Point(wd, ht)
		<< Point(wd - thick, ht) << Point(wd - thick, thick) << Point(0, thick);
	}
	else if(symbol == FS_LOG_OR) {
		int top = Percent(30, ht);
		int bot = Percent(80, ht);
		int wd2 = wd >> 1;
		polygon << Point(0, top) << Point(wd2, bot) << Point(wd, top)
		<< Point(wd - thick, top) << Point(wd2, bot - thick - iscale(thick, bot - top, wd2)) << Point(thick, top);
	}
	else if(symbol > FS_SETOP_BEGIN && symbol < FS_SETOP_END) {
		int ht2 = Percent(60, ht), ht4 = Percent(20, ht), wd2 = wd >> 1;
		polygon << Point(wd, ht2 - ht4);
		Bezier(polygon, Point(wd2, ht2 - ht4), Point(0, ht2 - ht4), Point(0, ht2), 8);
		Bezier(polygon, Point(0, ht2), Point(0, ht2 + ht4), Point(wd2, ht2 + ht4), 8);
		polygon << Point(wd, ht2 + ht4) << Point(wd, ht2 + ht4 - thick);
		Bezier(polygon, Point(wd2 + half, ht2 + ht4 - thick), Point(thick, ht2 + ht4 - thick), Point(thick, ht2), 8);
		Bezier(polygon, Point(thick, ht2), Point(thick, ht2 - ht4 + thick), Point(wd2 + half, ht2 - ht4 + thick), 8);
		polygon << Point(wd, ht2 - ht4 + thick);
		counts.Add(polygon.GetCount());
		if(symbol == FS_MEMBER || symbol == FS_NOT_MEMBER) {
			Rectangle(polygon, Rect(thick, ht2 - half, wd2, ht2 + half));
			counts.Add(4);
		}
		if(symbol == FS_SUBSET || symbol == FS_NOT_SUBSET) {
			int y = ht2 + ht4 + 2 * thick;
			Rectangle(polygon, RectC(0, y, wd, thick));
			counts.Add(4);
		}

		if(symbol == FS_NOT_MEMBER || symbol == FS_NOT_SUBSET || symbol == FS_NOT_PROPER_SUBSET) {
			polygon << Point(wd, 0) << Point(thick, ht) << Point(0, ht) << Point(wd - thick, 0);
			counts.Add(4);
		}
	}
	else {
		ASSERT(symbol == FS_EMPTY);
		return;
	}

	if(xmirror)
		for(int i = 0; i < polygon.GetCount(); i++)
			polygon[i].x = wd - polygon[i].x;

	draw.Offset(rc.TopLeft());
	if(counts.IsEmpty())
		counts.Add(polygon.GetCount());
	DrawPolygons(draw, polygon, counts, color, 0, color);
	draw.End();
}

void Formula::PaintRect0(RefCon<Formula> f, Draw& draw, int x_left, int y_baseline)
{
	if(!!f)
		f->PaintRect(draw, x_left, y_baseline);
}

void Formula::PaintRect(Draw& draw, int x_left, int y_baseline) const
{
#if SHOW_BOXES
	int wd = DotsToPixels(draw, 20);
	draw.DrawRect(x_left, y_baseline - ascent, width, wd, Yellow);
	draw.DrawRect(x_left, y_baseline + descent - wd, width, wd, Yellow);
	draw.DrawRect(x_left + width - wd, y_baseline - ascent, wd, ascent + descent, Yellow);
	draw.DrawRect(x_left, y_baseline - (wd >> 1), width, wd, LtMagenta);
	draw.DrawRect(x_left, y_baseline, width, 1, LtBlue);
	draw.DrawRect(x_left, y_baseline, wd, descent, Color(0, 192, 0));
	draw.DrawRect(x_left, y_baseline - ascent, wd, ascent, LtCyan);
#endif
	Paint(draw, x_left, y_baseline);
}

int Formula::GetTextDeltaY(Font font)
{
	FontInfo fi = font.Info();
	return Percent(fi.GetHeight(), 32) - fi.GetAscent();
}

Size GetScaledSize(Size from, Size to)
{
	if(to.cx == 0 || to.cy == 0)
		return to;
	if(from.cx == 0 || from.cy == 0)
		return Size(0, 0);
	if(from.cx * to.cy < to.cx * from.cy)
		return Size(iscale(to.cy, from.cx, from.cy), to.cy);
	else
		return Size(to.cx, iscale(to.cx, from.cy, from.cx));
}

FormulaBox::FormulaBox(RefCon<Formula> in_, Color color, int flags)
: in(in_), color(color), flags(flags)
{
	textheight = in->GetTextHeight();
	boxwidth = GetLineWidth(textheight);
	offset = (flags & INNER ? -boxwidth : flags & TIGHT ? 0 : GetLineOffset(textheight));
	if(flags & THICK)
		boxwidth *= 2;
	ascent = in->GetAscent() + offset + boxwidth;
	descent = in->GetDescent() + offset + boxwidth;
	width = in->GetWidth() + 2 * (offset + boxwidth);
}

void FormulaBox::Paint(Draw& draw, int x_left, int y_baseline) const
{
	DrawFatFrame(draw, x_left, y_baseline - ascent, width, ascent + descent, color, boxwidth);
	in->PaintRect(draw, x_left + boxwidth + offset, y_baseline);
}

FormulaSymbol::FormulaSymbol(FORMSYMBOL symbol_, int height_, int textheight_, Color color_)
: symbol(symbol_), height(height_), color(color_)
{
	textheight = textheight_;
	leading = GetLeading(textheight);
	ascent = height >> 1;
	descent = height - ascent;
	width = SymbolWidth(symbol, height - 2 * leading.cy, textheight) + 2 * leading.cx;
}

void FormulaSymbol::Paint(Draw& draw, int x_left, int y_baseline) const
{
	PaintSymbol(draw, symbol,
		Rect(x_left, y_baseline - ascent, x_left + width, y_baseline + descent).Deflated(leading),
		textheight, color);
}

FormulaText::FormulaText(WString text, Font font, Color color, int dy)
: text(text), font(font), color(color)
{
	width = GetTextSize(text, font).cx;
	FontInfo fi = font.Info();
	int fht = fi.GetHeight();
	const wchar *p = text;
	while(*p && IsLower(*p))
		p++;
	textheight = font.GetHeight();
	if(font.IsItalic())
		width += GetItalicDelta(fht);
	ascent = fht >> 1;
	descent = fht - ascent;
	dy = Percent(dy, textheight);
	deltay = GetTextDeltaY(font) + dy;
	if(*p == 0)
		ascent -= textheight / 9;
	ascent -= max(dy, 0);
	descent += min(dy, 0);
	LLOG("FormulaText::FormulaText");
	LDUMP(text);
	LDUMP(width);
	LDUMP(ascent);
	LDUMP(descent);
	LDUMP(textheight);
	LDUMP(deltay);
	LDUMP(fi.GetAscent());
	LDUMP(fi.GetDescent());
	LDUMP(fi.GetHeight());
}

void FormulaText::Paint(Draw& draw, int x_left, int y_baseline) const
{
	LLOG("FormulaText::Paint");
	LDUMP(text);
	LDUMP(x_left);
	LDUMP(y_baseline);
	draw.DrawText(x_left, y_baseline + deltay, text, font, color);
}

FormulaRatio::FormulaRatio(RefCon<Formula> top_, RefCon<Formula> bottom_, Color color)
: top(top_), bottom(bottom_), color(color)
{
	textheight = GetTextHeight0(top, bottom);
	linewidth = GetLineWidth(textheight);
	width = max(top->GetWidth(), bottom->GetWidth());
	ascent = top->GetHeight() + (linewidth >> 1);
	descent = bottom->GetHeight() + ((linewidth + 1) >> 1);
}

void FormulaRatio::Paint(Draw& draw, int x_left, int y_baseline) const
{
	int half = linewidth >> 1;
	top->PaintRect(draw, x_left + ((width - top->GetWidth()) >> 1),
		y_baseline - half - top->GetDescent());
	draw.DrawLine(x_left + half, y_baseline, x_left + width - half, y_baseline, linewidth, color);
	draw.DrawRect(x_left, y_baseline - half, width, linewidth, color);
	bottom->PaintRect(draw, x_left + ((width - bottom->GetWidth()) >> 1),
		y_baseline + ((linewidth + 1) >> 1) + bottom->GetAscent());
}

FormulaSqrt::FormulaSqrt(RefCon<Formula> in_, RefCon<Formula> power_, Color color)
: in(in_), power(power_), color(color)
{
	int ht = in->GetHeight(), tht = in->GetTextHeight();
	leading = GetLeading(tht);
	linewidth = GetLineWidth(tht);
	half = linewidth >> 1;
	descent = in->GetDescent() + leading.cy;
	ascent = in->GetAscent() + linewidth + leading.cy;
	width = in->GetWidth();
	leftpos = -half; //(ht * 6) >> 4;
	roofpos = -ascent + leading.cy;
	slashwd = bslashwd = ht / 10;
	leftwidth = ht / 10;
	if(!!power)
	{
		ascent = max(ascent, -leftpos + power->GetHeight());
		leftwidth = max(leftwidth >> 1, power->GetWidth()) - half;
	}
	width += slashwd + bslashwd + leftwidth + half + 2 * leading.cx;
	textheight = in->GetTextHeight();
}

void FormulaSqrt::Paint(Draw& draw, int x_left, int y_baseline) const
{
	int ly = y_baseline + leftpos;
	int by = y_baseline + descent - leading.cy - half;
	int ty = y_baseline + roofpos;
	if(!!power)
		power->PaintRect(draw, x_left + leading.cx, ly - power->GetDescent());
	x_left += leading.cx;
	in->PaintRect(draw, x_left + leftwidth + slashwd + bslashwd + half, y_baseline);
	draw.DrawRect(x_left, ly, leftwidth, linewidth, color);
	x_left += leftwidth;
	draw.DrawLine(x_left, ly + half, x_left + bslashwd, by, linewidth, color);
	x_left += bslashwd;
	draw.DrawLine(x_left, by, x_left + slashwd, ty + half, linewidth, color);
	x_left += slashwd;
	draw.DrawRect(x_left, ty, in->GetWidth() + half, linewidth, color);
}

FormulaIndex::FormulaIndex(RefCon<Formula> in_,
RefCon<Formula> topright_, RefCon<Formula> bottomright_,
RefCon<Formula> top_,      RefCon<Formula> bottom_,
RefCon<Formula> topleft_,  RefCon<Formula> bottomleft_)
: in(in_)
, topright(topright_), bottomright(bottomright_)
, top(top_), bottom(bottom_)
, topleft(topleft_), bottomleft(bottomleft_)
{
	int trw = 0, tra = 0, trd = 0, brw = 0, bra = 0, brd = 0;
	int tw = 0, ta = 0, td = 0, bw = 0, ba = 0, bd = 0;
	int tlw = 0, tla = 0, tld = 0, blw = 0, bla = 0, bld = 0;
	tlead = blead = 0;
	if(!!topright)    { trw = topright   ->GetWidth(); tra = topright   ->GetAscent(); trd = topright   ->GetDescent(); }
	if(!!bottomright) { brw = bottomright->GetWidth(); bra = bottomright->GetAscent(); brd = bottomright->GetDescent(); }
	if(!!top)         { tw  = top        ->GetWidth(); ta  = top        ->GetAscent(); td  = top        ->GetDescent(); tlead = GetVertLeading(in->GetTextHeight()); }
	if(!!bottom)      { bw  = bottom     ->GetWidth(); ba  = bottom     ->GetAscent(); bd  = bottom     ->GetDescent(); blead = GetVertLeading(in->GetTextHeight()); }
	if(!!topleft)     { tlw = topleft    ->GetWidth(); tla = topleft    ->GetAscent(); tld = topleft    ->GetDescent(); }
	if(!!bottomleft)  { blw = bottomleft ->GetWidth(); bla = bottomleft ->GetAscent(); bld = bottomleft ->GetDescent(); }
	rwidth = max(trw, brw);
	lwidth = max(tlw, blw);
	bwidth = max(in->GetWidth(), max(tw, bw));
	ltdy = rtdy = -max(in->GetAscent() - max(tla, tra), max(tld, trd));
	lbdy = rbdy = +max(in->GetDescent() - max(bld, brd), max(bla, bra));
	int over = trd + brd - (rbdy - rtdy);
	if(over > 0)
	{
		rtdy -= over >> 1;
		rbdy += over >> 1;
	}
	over = tld + bld - (lbdy - ltdy);
	if(over > 0)
	{
		ltdy -= over >> 1;
		lbdy += over >> 1;
	}
	width = lwidth + bwidth + rwidth;
	ascent = max(in->GetAscent() + ta + td - tlead, max(!!topleft ? -ltdy + tla : 0, !!topright ? -rtdy + tra : 0));
	descent = max(in->GetDescent() + ba + bd - blead, max(!!bottomleft ? lbdy + bld : 0, !!bottomright ? rbdy + brd : 0));
	textheight = in->GetTextHeight();
}

void FormulaIndex::Paint(Draw& draw, int x_left, int y_baseline) const
{
	in->PaintRect(draw, x_left + lwidth + ((bwidth - in->GetWidth()) >> 1), y_baseline);
	if(!!topleft)     topleft    ->PaintRect(draw, x_left + lwidth - topleft    ->GetWidth(), y_baseline + ltdy);
	if(!!bottomleft)  bottomleft ->PaintRect(draw, x_left + lwidth - bottomleft ->GetWidth(), y_baseline + lbdy);
	x_left += lwidth;
	if(!!top)         top        ->PaintRect(draw, x_left + ((bwidth - top   ->GetWidth()) >> 1), y_baseline - in->GetAscent() - top->GetDescent() + tlead);
	if(!!bottom)      bottom     ->PaintRect(draw, x_left + ((bwidth - bottom->GetWidth()) >> 1), y_baseline + in->GetDescent() + bottom->GetAscent() - blead);
	x_left += bwidth;
	if(!!topright)    topright   ->PaintRect(draw, x_left, y_baseline + rtdy);
	if(!!bottomright) bottomright->PaintRect(draw, x_left, y_baseline + rbdy);
}

FormulaParen::FormulaParen(RefCon<Formula> in_, FORMSYMBOL lsym, FORMSYMBOL rsym, Color color)
: in(in_), lsymbol(lsym), rsymbol(rsym), color(color)
{
	textheight = in->GetTextHeight();
	int rad = max(in->GetAscent(), in->GetDescent());
	leading = GetLeading(textheight);
	Size sz = in->GetSize();
	sz.cy -= 2 * leading.cy;
	if(rad <= textheight)
		height = textheight + 2 * leading.cy;
	else
		height = sz.cy;
	ascent = max(height >> 1, in->GetAscent());
	descent = max(height >> 1, in->GetDescent());
	lwidth = SymbolWidth(lsymbol, sz.cy, textheight) + 2 * leading.cx;
	rwidth = SymbolWidth(rsymbol, sz.cy, textheight) + 2 * leading.cx;
	width = lwidth + rwidth + sz.cx;
}

void FormulaParen::Paint(Draw& draw, int x_left, int y_baseline) const
{
	in->PaintRect(draw, x_left + lwidth, y_baseline);
	int x_right = x_left + lwidth + in->GetWidth();
	int hh = height >> 1;
	PaintSymbol(draw, lsymbol, RectC(x_left, y_baseline - hh, lwidth, height).Deflated(leading), textheight, color);
	PaintSymbol(draw, rsymbol, RectC(x_right, y_baseline - hh, rwidth, height).Deflated(leading), textheight, color);
}

FormulaMatrix::FormulaMatrix(pick_ Vector< Vector< RefCon<Formula> > >& array, pick_ RefCon<Formula> sep)
{
	Vector<Point> points;
	Vector< RefCon<Formula> > formulas;
	for(int i = 0; i < array.GetCount(); i++)
		for(int j = 0; j < array[i].GetCount(); j++)
		{
			points.Add(Point(j, i));
			formulas.Add() = array[i][j];
		}
	Init(points, formulas, sep);
}

FormulaMatrix::FormulaMatrix(pick_ Vector< RefCon<Formula> >& formulas, RefCon<Formula> sep)
{
	Vector<Point> points;
	for(int i = 0; i < formulas.GetCount(); i++)
		points.Add(Point(i, 0));
	Init(points, formulas, sep);
}

void FormulaMatrix::Init(pick_ Vector<Point>& pos_, pick_ Vector< RefCon<Formula> >& formulas_, RefCon<Formula> sep_)
{
	pos = pos_;
	formulas = formulas_;
	sep = sep_;
	int sw = 0, sa = 0, sd = 0;
	if(!!sep) { sw = sep->GetWidth(); sa = sep->GetAscent(); sd = sep->GetDescent(); }
	int i;
	for(i = 0; i < pos.GetCount(); i++)
	{
		const Formula *f = ~formulas[i];
		int& wd = widths.DoIndex(pos[i].x, 0);
		wd = max(wd, f->GetWidth());
		int& as = ascents.DoIndex(pos[i].y, sa);
		as = max(as, f->GetAscent());
		int& ds = descents.DoIndex(pos[i].y, sd);
		ds = max(ds, f->GetDescent());
		textheight = max(textheight, f->GetTextHeight());
	}
	Size size(0, 0);
	xpos.SetCount(widths.GetCount());
	for(i = 0; i < widths.GetCount(); i++)
	{
		xpos[i] = size.cx;
		size.cx += widths[i] + sw;
	}
	size.cx -= sw;
	ypos.SetCount(ascents.GetCount());
	for(i = 0; i < ascents.GetCount(); i++)
	{
		ypos[i] = size.cy;
		size.cy += ascents[i] + descents[i];
	}
	width = size.cx;
	if(ascents.GetCount() == 1)
	{
		ascent = ascents[0];
		descent = descents[0];
	}
	else
	{
		ascent = size.cy >> 1;
		descent = size.cy - ascent;
	}
}

void FormulaMatrix::Paint(Draw& draw, int x_left, int y_baseline) const
{
	y_baseline -= ascent;
//	int max_x = widths.GetCount() - 1;
	for(int i = 0; i < pos.GetCount(); i++)
	{
		const Formula *f = ~formulas[i];
		Point xy = pos[i];
		int y = y_baseline + ypos[xy.y] + ascents[xy.y];
		f->PaintRect(draw, x_left + xpos[xy.x] + ((widths[xy.x] - f->GetWidth()) >> 1), y);
		int iitem = xy.y * widths.GetCount() + xy.x;
		if(!!sep && pos.Find(xy + Size(1, 0)) >= 0)
			sep->PaintRect(draw, x_left + xpos[xy.x] + widths[xy.x], y);
	}
}

FormulaIntegral::FormulaIntegral(RefCon<Formula> integrand, RefCon<Formula> bottom, RefCon<Formula> top, bool right_limits)
{
}

void FormulaIntegral::Paint(Draw& draw, int x_left, int y_baseline) const
{
}

inline int GetOperSpace(RefCon<Formula> a, RefCon<Formula> b)
{
	return Formula::GetOperSpace(max(a->GetHeight(), b->GetHeight()));
}

class FormulaParser
{
public:
	FormulaParser(const char *ptr, Font font = Arial(100), Color color = Color());

	RefCon<Formula>  Run();

private:
	RefCon<Formula>  RunLow();
	RefCon<Formula>  RunSemicolon();
	RefCon<Formula>  RunComma();
	RefCon<Formula>  RunTextBin();
	RefCon<Formula>  RunComp();
	RefCon<Formula>  RunMul();
	RefCon<Formula>  RunDiv();
	RefCon<Formula>  RunPostfix();
	RefCon<Formula>  RunTerm();

	RefCon<Formula>  RunParen();
	bool                RunIndex(RefCon<Formula>& topright, RefCon<Formula>& bottomright,
		RefCon<Formula>& top, RefCon<Formula>& bottom, RefCon<Formula>& topleft, RefCon<Formula>& bottomleft);

	RefCon<Formula>  GetError(String text, Font font) const;
	RefCon<Formula>  GetError(String text) const { return GetError(text, state.font); }
	void                AddError(RefCon<Formula>& item, String text) const;

	byte                Skip();
	bool                Check(char c);
	bool                Check(char c1, char c2);
	bool                Check(char c1, char c2, char c3);
	void                Force(char c, RefCon<Formula>& errout);
	void                Force(char c1, char c2, RefCon<Formula>& errout);
	String              ScanIdent();

	Font                GetFont(int ht) { return Font(state.font).Height(ht); }

	void                Push(bool makeindex);
	bool                Pop();

	RefCon<Formula>  MakeBinary(RefCon<Formula> left, const WString& text, bool is_symbol,
		RefCon<Formula> right, int gap = Null, int perc_shift = 0) const;
	RefCon<Formula>  MakeUnary(String text, bool is_symbol,
		RefCon<Formula> form, int gap = Null, int perc_shift = 0) const;

private:
	struct State
	{
		Font  font;
		Color color;
	};

	const char          *ptr;
	Array<State>         stack;
	State                state;
};

RefCon<Formula> ParseFormula(const char *formula, Font font, Color color)
{
	return FormulaParser(formula, font, color).Run();
}

FormulaParser::FormulaParser(const char *ptr, Font font, Color color)
: ptr(ptr)
{
	if(font.GetHeight() == 0)
		font.Height(font.Info().GetHeight());
	state.font = font;
	state.color = color;
}

RefCon<Formula> FormulaParser::Run()
{
	if(!Skip())
		return NULL;
	RefCon<Formula> item = RunLow();
	if(Skip())
		AddError(item, "ignored: " + StringSample(ptr, 20));
	return item;
}

RefCon<Formula> FormulaParser::RunLow()
{
	return RunSemicolon();
}

RefCon<Formula> FormulaParser::RunSemicolon()
{
	RefCon<Formula> item = RunComma();
	while(Check(';'))
	{
		RefCon<Formula> right = RunComma();
		item = MakeBinary(item, ";", false, right, Formula::GetTextHeight0(item, right) >> 1);
	}
	return item;
}

RefCon<Formula> FormulaParser::RunComma()
{
	RefCon<Formula> item = RunTextBin();
	if(Check('&', '&'))
	{ // must run before '&', aargh!
		RefCon<Formula> right = RunComma();
		int ht = Formula::GetTextHeight0(item, right);
		return MakeBinary(item, "&", false, right, Percent(50, ht), ht);
	}
	bool row = false;
	if(Check('&'))
		row = true;
	else if(!Check(','))
		return item;
	Vector< Vector< RefCon<Formula> > > formulas;
	formulas.Add().Add() = item;
	int tht = 0;
	do
	{
		tht = max(tht, formulas.Top().Top()->GetTextHeight());
		if(row)
			formulas.Add();
		formulas.Top().Add() = RunTextBin();
	}
	while((row = Check('&')) != 0 || Check(','));
	return new FormulaMatrix(formulas,
		new FormulaText(", ", Font(state.font)(tht), state.color));
}

RefCon<Formula> FormulaParser::RunTextBin()
{
	RefCon<Formula> form = RunComp();
	for(String op; !IsNull(op = ScanIdent());)
	{
		FORMSYMBOL sym = FS_EMPTY;
		if     (op == "in")           sym = (Check('!') ? FS_NOT_MEMBER : FS_MEMBER);
		else if(op == "subseteq")     sym = (Check('!') ? FS_NOT_SUBSET : FS_SUBSET);
		else if(op == "subset")       sym = (Check('!') ? FS_NOT_PROPER_SUBSET : FS_PROPER_SUBSET);
		else
		{
			AddError(form, "??" + op + "??");
			continue;
		}
		RefCon<Formula> right = RunComp();
		int ht = Percent(150, Formula::GetTextHeight0(form, right));
		RefCon<Formula> body = new FormulaSymbol(sym, ht, ht, state.color);
		Vector< RefCon<Formula> > list;
		list << form << body << right;
		return new FormulaMatrix(list, new FormulaSpace(Percent(20, ht)));
	}
	return form;
}

RefCon<Formula> FormulaParser::RunComp()
{
	int tht = state.font.GetHeight();
	int gap = 3 * Formula::GetOperSpace(tht);
	RefCon<Formula> item = RunMul();
	for(;;)
		if(Check('=', '~'))
			item = MakeBinary(item, WString(0x2248, 1), false, RunComp(), gap);
		else if(Check('<', '-', '>'))
			item = MakeBinary(item, WString(0x2194, 1), false, RunComp(), gap, -5);
		else if(Check('<', '=', '>'))
			item = MakeBinary(item, "\xDB", true, RunComp(), 2 * gap, -5);
		else if(Check('<', '>'))
			item = MakeBinary(item, WString(0x2260, 1), false, RunComp(), gap, -5);
		else if(Check('<', '='))
			item = MakeBinary(item, WString(0x2264, 1), false, RunComp(), gap, -5);
		else if(Check('>', '='))
			item = MakeBinary(item, WString(0x2265, 1), false, RunComp(), gap, -5);
		else if(Check('<', '-'))
			item = MakeBinary(item, WString(0x2190, 1), false, RunComp(), gap, -5);
		else if(Check('-', '>'))
			item = MakeBinary(item, WString(0x2192, 1), false, RunComp(), gap, -5);
		else if(Check('=', '>'))
			item = MakeBinary(item, "\xDE", true, RunComp(), 2 * gap, -10);
		else if(Check('=', '<'))
			item = MakeBinary(item, "\xDC", true, RunComp(), 2 * gap, -10);
		else if(Skip() == '.' && ptr[1] == '.' && ptr[2] != '.') {
			ptr += 2;
			item = MakeBinary(item, "..", false, RunComp(), gap, 0);
		}
		else if(Check('+', '-'))
			item = MakeBinary(item, WString(0x00B1, 1), false, RunComp(), gap);
		else if(Check('<'))
			item = MakeBinary(item, "<", false, RunComp(), gap);
		else if(Check('>'))
			item = MakeBinary(item, ">", false, RunComp(), gap);
		else if(Check('='))
			item = MakeBinary(item, "=", false, RunComp(), gap);
		else if(Check('+'))
			item = MakeBinary(item, "+", false, RunComp());
		else if(Check('-'))
			item = MakeBinary(item, "-", false, RunComp(), Null, -11);
		else if(Check('|', '|'))
		{
			RefCon<Formula> right = RunComp();
			Vector< RefCon<Formula> > list;
			list << item;
			int ht = Formula::GetTextHeight0(item, right);
			list << new FormulaSymbol(FS_LOG_OR, ht, ht, state.color);
			list << right;
			return new FormulaMatrix(list, new FormulaSpace(Percent(50, ht)));
		}
		else if(Check('|'))
			item = MakeBinary(item, Null, false, RunComp(), 0);
		else
			return item;
}

RefCon<Formula> FormulaParser::RunMul()
{
	RefCon<Formula> item = RunDiv();
	for(;;)
		if(Check('*'))
			item = MakeBinary(item, "*", false, RunDiv());
		else if(Skip() == '.' && ptr[1] != '.') {
			ptr++;
			item = MakeBinary(item, ".", false, RunDiv(), Null, -20);
		}
		else if(Check('~'))
			item = MakeBinary(item, Null, false, RunDiv());
		else
			return item;
}

RefCon<Formula> FormulaParser::RunDiv()
{
	RefCon<Formula> item = RunPostfix();
	for(;;)
		if(Check('/', '_'))
			item = MakeBinary(item, "/", false, RunPostfix());
		else if(Check('/'))
			item = new FormulaRatio(item, RunPostfix());
		else
			return item;
}

RefCon<Formula> FormulaParser::RunPostfix()
{
	RefCon<Formula> item = RunTerm();
	for(;;)
	{
		RefCon<Formula> topright, bottomright, top, bottom, topleft, bottomleft, in;
		if(RunIndex(topright, bottomright, top, bottom, topleft, bottomleft))
		{
			if(!!topright || !!bottomright || !!top || !!bottom || !!topleft || !!bottomleft)
				item = new FormulaIndex(item, topright, bottomright, top, bottom, topleft, bottomleft);
		}
		else if(!!(in = RunParen()))
		{
			int tht = Formula::GetTextHeight0(item, in);
			item = MakeBinary(item, Null, false, in, Formula::GetFuncSpace(tht));
		}
		else if(Check('!'))
			item = MakeBinary(item, Null, false, new FormulaText("!", state.font), state.color);
		else if(Check(':'))
			item = MakeBinary(item, Null, false, RunTerm(), 0);
		else
			return item;
	}
}

bool FormulaParser::RunIndex(RefCon<Formula>& topright, RefCon<Formula>& bottomright,
	RefCon<Formula>& top, RefCon<Formula>& bottom, RefCon<Formula>& topleft, RefCon<Formula>& bottomleft)
{
	bool done = false;
	for(;;)
	{
		enum KIND { NONE, TR, BR, T, B, TL, BL };
		KIND kind = NONE;
		if(Check('_', '<'))      kind = BL;
		else if(Check('^', '<')) kind = TL;
		else if(Check('_', '|')) kind = B;
		else if(Check('^', '|')) kind = T;
		else if(Check('_'))      kind = BR;
		else if(Check('^'))      kind = TR;
		else if(Check('\''))
		{
			RefPtr<Formula> form = new FormulaText("\'", Font(state.font).Italic(), state.color, 40);
			if(!topright)
				topright = form;
			else
				topright = MakeBinary(topright, Null, false, form, 0);
			done = true;
			continue;
		}
		if(kind == NONE)
			return done;
		Push(true);
		RefCon<Formula> ix = RunTerm();
		Pop();
		switch(kind)
		{
		case TR: topright = ix; break;
		case BR: bottomright = ix; break;
		case T:  top = ix; break;
		case B:  bottom = ix; break;
		case TL: topleft = ix; break;
		case BL: bottomleft = ix; break;
		default: NEVER();
		}
		done = true;
	}
}

RefCon<Formula> FormulaParser::RunParen()
{
	FORMSYMBOL lsym = FS_EMPTY, rsym = FS_EMPTY;
	char finish = 0;
	if(Check('('))                { lsym = FS_LPAREN; rsym = FS_RPAREN; finish = ')'; }
	else if(Check('{', '<'))      { lsym = FS_LSHARP; rsym = FS_RSHARP; finish = '}'; }
	else if(Check('[', '_'))      { lsym = FS_LTBRACKET; rsym = FS_RTBRACKET; finish = ']'; }
	else if(Check('[', '^'))      { lsym = FS_LBBRACKET, rsym = FS_RBBRACKET; finish = ']'; }
	else if(Check('['))           { lsym = FS_LBRACKET; rsym = FS_RBRACKET; finish = ']'; }
	else if(Check('{', '[', '_')) { lsym = FS_LTBRACKET; rsym = FS_RTBRACKET; finish = '}'; }
	else if(Check('{', '[', '^')) { lsym = FS_LBBRACKET, rsym = FS_RBBRACKET; finish = '}'; }
	else if(Check('{', '['))      { lsym = FS_LBRACKET; rsym = FS_RBRACKET; finish = '}'; }
	else if(Check('{', '|'))      { lsym = FS_VBAR; rsym = FS_VBAR; finish = '}'; }
	else if(Check('{', '('))      { lsym = FS_LPAREN; rsym = FS_RPAREN; finish = '}'; }
	else if(Check('{', '_'))      { lsym = FS_LBRACE; rsym = FS_RBRACE; finish = '}'; }
	else if(Check('{', '~'))      { lsym = FS_EMPTY; rsym = FS_RBRACE; finish = '}'; }
	else if(Check('{'))           { finish = '}'; }
	else
		return NULL;
	RefCon<Formula> item = RunLow();
	if(finish == '}')
	{
		if(Check('}', ']', '_')) rsym = FS_RBBRACKET;
		else if(Check('}', ']', '^')) rsym = FS_RTBRACKET;
		else if(Check('}', ']')) rsym = FS_RBRACKET;
		else if(Check('}', ')')) rsym = FS_RPAREN;
		else if(Check('}', '>')) rsym = FS_RSHARP;
		else if(Check('}', '|')) rsym = FS_VBAR;
		else if(Check(finish, '~')) rsym = FS_EMPTY;
		else Force(finish, item);
	}
	else
		Force(finish, item);
	if(lsym != FS_EMPTY || rsym != FS_EMPTY)
		return new FormulaParen(item, lsym, rsym, state.color);
	return item;
}

RefCon<Formula> FormulaParser::RunTerm()
{
	RefCon<Formula> paren = RunParen();
	if(!!paren)
		return paren;
	if(Check('@'))
		return MakeUnary("\xB6", true, RunTerm(), 0, true);
	if(Check('-'))
		return MakeUnary("-", false, RunTerm(), 0, true);
	if(Check('+'))
		return MakeUnary("+", false, RunTerm(), 0, true);
	if(Check('.', '.', '.'))
		return new FormulaText("...", state.font, state.color);
	if(Check('~', '~'))
	{
		int blanks = 1;
		for(; *ptr == '~'; ptr++)
			blanks++;
		RefCon<Formula> item = RunTerm();
		return MakeUnary(Null, false, item, (blanks * Formula::GetTextHeight0(item)) >> 1);
	}

	Font font = state.font;
	bool noitalic = false;
	for(;;)
		if(Check('*'))
			font.Bold();
		else if(Check('/'))
			font.Italic();
		else if(Check('\\'))
			noitalic = true;
		else
			break;

	if(Check(':')) {
		static const wchar Alpha[26] = {
			0x391, // A -> Alpha
			0x392, // B -> Beta
			0x3A7, // C -> Chi
			0x394, // D -> Delta
			0x395, // E -> Epsilon
			0x3A6, // F -> Phi
			0x393, // G -> Gamma
			0x397, // H -> Eta
			0x399, // I -> Iota
			0x300, // J
			0x39A, // K -> Kappa
			0x39B, // L -> Lambda
			0x39C, // M -> Mu
			0x39D, // N -> Nu
			0x39F, // O -> Omikron
			0x3A0, // P -> Pi
			0x398, // Q -> Theta
			0x3A1, // R -> Rho
			0x3A3, // S -> Sigma
			0x3A4, // T -> Tau
			0x300, // U
			0x3A9, // V -> Omega
			0x3A8, // W -> Psi
			0x39E, // X -> Xi
			0x3A5, // Y -> Ypsilon
			0x396, // Z -> Zeta
		};
		static const wchar alpha[26] = {
			0x3B1, // a -> alpha
			0x3B2, // b -> beta
			0x3C7, // c -> chi
			0x3B4, // d -> delta
			0x3B5, // e -> epsilon
			0x3C6, // f -> phi
			0x3B3, // g -> gamma
			0x3B7, // h -> eta
			0x3B9, // i -> iota
			0x300, // j
			0x3BA, // k -> kappa
			0x3BB, // l -> lambda
			0x3BC, // m -> mu
			0x3BD, // n -> nu
			0x3BF, // o -> omikron
			0x3C0, // p -> pi
			0x3B8, // q -> theta
			0x3C1, // r -> rho
			0x3C3, // s -> sigma
			0x3C4, // t -> tau
			0x3C2, // u -> final sigma
			0x3C9, // v -> omega
			0x3C8, // w -> psi
			0x3BE, // x -> xi
			0x3C5, // y -> ypsilon
			0x3B6, // z -> zeta
		};
		wchar wc = 0;
		if(IsUpper(*ptr))
			wc = Alpha[*ptr++ - 'A'];
		else if(IsLower(*ptr))
			wc = alpha[*ptr++ - 'a'];
		else
			return GetError("invalid greek character");
		return new FormulaText(WString(wc, 1), font, state.color);
	}
	char c = Skip();
	if(IsAlpha(c))
	{
		const char *b = ptr;
		while(IsAlNum(*++ptr))
			;
		String ident(b, ptr);
		FORMSYMBOL group_sym = FS_EMPTY;
		if(ident == "sqrt")
		{
			RefCon<Formula> exp;
			if(Check('_'))
			{
				Push(true);
				exp = RunTerm();
				Pop();
			}
			RefCon<Formula> arg = RunTerm();
			return new FormulaSqrt(arg, exp);
		}
		else if(ident == "not")          group_sym = FS_LOG_NOT;
		else if(ident == "sum")          group_sym = FS_SUM;
		else if(ident == "product")      group_sym = FS_PRODUCT;
		else if(ident == "union")        group_sym = FS_UNION;
		else if(ident == "intersection") group_sym = FS_INTERSECTION;
		else if(ident == "int")          group_sym = FS_INT;
		else if(ident == "iint")         group_sym = FS_IINT;
		else if(ident == "iiint")        group_sym = FS_IIINT;
		else if(ident == "oint")         group_sym = FS_OINT;
		else if(ident == "oiint")        group_sym = FS_OIINT;

		if(group_sym != FS_EMPTY) {
			RefCon<Formula> topright, bottomright, top, bottom, topleft, bottomleft;
			RunIndex(topright, bottomright, top, bottom, topleft, bottomleft);
			RefCon<Formula> arg = RunTerm();
			int ht = arg->GetTextHeight();
			if(group_sym != FS_LOG_NOT)
				ht = max(ht, Percent(150, max(arg->GetAscent(), arg->GetDescent())));
			RefCon<Formula> body = new FormulaSymbol(group_sym, ht, arg->GetTextHeight(), state.color);
			if(!!topright || !!bottomright || !!top || !!bottom || !!topleft || !!bottomleft)
				body = new FormulaIndex(body, topright, bottomright, top, bottom, topleft, bottomleft);
			return MakeBinary(body, Null, false, arg);
		}
		else if(ident == "oo")
			return new FormulaText(WString(0x221E, 1), font, state.color, -11);

		if(!font.IsItalic() && !noitalic && ptr - b == 2 && *b == 'd'
		&& (b[1] < 'd' || b[1] > 'h'))
			return MakeBinary(
				new FormulaText("d", font, state.color),
				Null,
				new FormulaText(String(b + 1, ptr).ToWString(), Font(font).Italic(), state.color),
				0);
		if(ptr - b == 1 && (*b < 'd' || *b > 'h'))
			font.Italic();
		if(noitalic)
			font.NoItalic();
		return new FormulaText(String(b, ptr).ToWString(), font, state.color);
	}
	if(IsDigit(c))
	{
		const char *b = ptr;
		while(IsDigit(*++ptr))
			;
		if(*ptr == '.' && IsDigit(ptr[1])) {
			while(IsDigit(*++ptr))
				;
		}
		RefCon<Formula> number = new FormulaText(String(b, ptr).ToWString(), state.font, state.color);
		if(*ptr == 'e' || *ptr == 'E') {
			b = ++ptr;
			if(*ptr == '+' || *ptr == '-')
				ptr++;
			RefCon<Formula> exp;
			Font fx = Font(font).Height(Formula::GetIndexSize(font.GetHeight()));
			if(IsDigit(*ptr))
			{
				while(IsDigit(*++ptr))
					;
				exp = new FormulaText(String(b, ptr).ToWString(), fx, state.color);
			}
			else
				exp = GetError("missing exponent", fx);
			number = MakeBinary(number, ".",
					new FormulaIndex(new FormulaText("10", font, state.color), exp), Null, -20);
		}
		return number;
	}
	if(c == '\"')
	{
		String out;
		while(*++ptr && *ptr != '\"')
			if(*ptr == '\\' && ptr[1])
				out.Cat(*++ptr);
			else
				out.Cat(*ptr);
		RefPtr<Formula> item = new FormulaText(out.ToWString(), font, state.color);
		Force('\"', item);
		return item;
	}
	if(c == '~')
	{
		ptr++;
		return new FormulaSpace(0, 0, 0, state.font.GetHeight());
	}
	String err;
	if(*ptr)
		err << (char)c << " (0x" << Format("%02x", c) << ")?";
	else
		err = "EOF";
	return GetError(err, state.font);
}

void FormulaParser::Push(bool makeindex)
{
	stack.Add(state);
	if(makeindex)
		state.font.Height(Formula::GetIndexSize(state.font.GetHeight()));
}

bool FormulaParser::Pop()
{
	if(stack.IsEmpty())
		return false;
	state = stack.Top();
	stack.Drop();
	return true;
}

byte FormulaParser::Skip()
{
	for(;;)
		if(*ptr && (byte)*ptr <= ' ')
			ptr++;
		else if(*ptr == '/' && ptr[1] == '*')
		{
			ptr += 2;
			while(*ptr && !(ptr[0] == '*' && ptr[1] == '/'))
				ptr++;
			if(*ptr)
				ptr += 2;
		}
		else if(ptr[0] == '/' && ptr[1] == '/')
		{
			ptr += 2;
			while(*ptr && *ptr != '\n')
				ptr++;
		}
		else
			return *ptr;
}

String FormulaParser::ScanIdent()
{
	byte c = Skip();
	if(!IsAlpha(c))
		return Null;
	const char *b = ptr;
	while(IsAlNum(*ptr))
		ptr++;
	return String(b, ptr);
}

bool FormulaParser::Check(char c)
{
	if(Skip() != c)
		return false;
	ptr++;
	return true;
}

bool FormulaParser::Check(char c1, char c2)
{
	if(Skip() != c1 || ptr[1] != c2)
		return false;
	ptr += 2;
	return true;
}

bool FormulaParser::Check(char c1, char c2, char c3)
{
	if(Skip() != c1 || ptr[1] != c2 || ptr[2] != c3)
		return false;
	ptr += 3;
	return true;
}

RefCon<Formula> FormulaParser::MakeBinary(RefCon<Formula> left, const WString& text, bool is_symbol,
	RefCon<Formula> right, int gap, int perc_shift) const
{
	int ht = Formula::GetTextHeight0(left, right);
	if(IsNull(gap))
		gap = Formula::GetOperSpace(ht);
	Vector< RefCon<Formula> > formula;
	formula.Add() = left;
	if(!IsNull(text)) {
		Font f = state.font;
		if(is_symbol)
			f.Face(Font::SYMBOL);
		f.Height(ht);
		formula.Add() = new FormulaText(text, f, state.color, perc_shift);
	}
	formula.Add() = right;
	RefPtr<Formula> form = new FormulaMatrix(formula, new FormulaSpace(gap));
	//form = new FormulaBox(form, LtRed);
	return form;
}

RefCon<Formula> FormulaParser::MakeUnary(String text, bool is_symbol,
	RefCon<Formula> form, int gap, int perc_shift) const
{
	int ht = form->GetTextHeight();
	Vector< RefCon<Formula> > formula;
	if(!IsNull(text)) {
		Font f = state.font;
		if(is_symbol)
			f.Face(Font::SYMBOL);
		f.Height(ht);
		formula.Add() = new FormulaText(text.ToWString(), f, state.color, perc_shift);
	}
	formula.Add() = form;
	return new FormulaMatrix(formula, new FormulaSpace(gap));
}

RefCon<Formula> FormulaParser::GetError(String text, Font font) const
{
	return new FormulaText(text.ToWString(), font.Bold(), LtRed);
}

void FormulaParser::AddError(RefCon<Formula>& item, String text) const
{
	int ht = item->GetTextHeight();
	item = MakeBinary(item, Null, false, GetError(text, Font(state.font).Height(ht)));
}

void FormulaParser::Force(char c, RefCon<Formula>& errout)
{
	if(!Check(c))
	{
		char temp[] = { c, 0 };
		Vector< RefCon<Formula> > list;
		list.Add() = errout;
		Font fb = Font(state.font).Bold();
		list.Add() = new FormulaText(temp, fb, LtBlue);
		list.Add() = new FormulaText(*ptr ? ~StringSample(ptr, 1) : "EOF", fb, LtRed);
		errout = new FormulaMatrix(list);
	}
}

void FormulaParser::Force(char c1, char c2, RefCon<Formula>& errout)
{
	if(!Check(c1, c2))
	{
		char temp[] = { c1, c2, 0 };
		Vector< RefCon<Formula> > list;
		list.Add() = errout;
		Font fb = Font(state.font).Bold();
		list.Add() = new FormulaText(temp, fb, LtBlue);
		list.Add() = new FormulaText(*ptr ? ~StringSample(ptr, 2) : "EOF", fb, LtRed);
		errout = new FormulaMatrix(list);
	}
}

FormulaDisplay::FormulaDisplay(int sh)
: std_height(sh)
{
}

void FormulaDisplay::Paint(Draw& draw, const Rect& rc, const Value& value, Color ink, Color paper, dword style) const
{
	draw.DrawRect(rc, paper);
	RefCon<Formula> form = Get(value, std_height ? std_height : 300, ink);
	if(!!form) {
		if(std_height)
			form->PaintRect(draw, (rc.left + rc.right - form->GetWidth()) >> 1,
				(rc.top + rc.bottom - form->GetHeight() + 2 * form->GetAscent()) >> 1);
		else {
			DrawingDraw ddraw(form->GetSize());
			form->PaintRect(ddraw, 0, form->GetAscent());
			Drawing dwg = ddraw;
			Size outsize = GetScaledSize(dwg.GetSize(), rc.Size());
			draw.DrawDrawing(rc.CenterRect(outsize), dwg);
		}
	}
}

Size FormulaDisplay::GetStdSize(const Value& value) const
{
	int ht = std_height;
	if(!ht) {
		FontInfo fi = StdFont().Info();
		ht = fi.GetHeight() - fi.GetExternal();
	}
	RefCon<Formula> form = Get(value, ht, SBlack);
	if(!!form)
		return form->GetSize();
	return Size(0, 0);
}

RefCon<Formula> FormulaDisplay::Get(const Value& value, int ht, Color ink) const
{
	if(IsNull(value))
		return NULL;
	return ParseFormula(StdFormat(value), Roman(ht), ink);
//	return new FormulaBox(ParseFormula(StdFormat(value), Roman(ht)), LtRed, FormulaBox::INNER);
}

const Display& GLOBAL_V(FormulaDisplay, StdFormulaDisplay);

#ifdef flagMAIN
#include <CtrlLib/CtrlLib.h>

void AppMain()
{
	FormulaDisplay fd(0);
	TopWindow tw;
	tw.Sizeable().Zoomable();
	const char *t;
//	t = "[_9 & 8 & 7]+[^9 & 8 & 7]+{_6 & 7 & 8}";
//	t = "(1,2 & 3 + 2 ,4 & 3,5 & 4) + ( x & y & z & w)=(0 & 0 & 0 & 0)";
//	t = "t^3=sqrt(3^2+2^2)";
//	t = "t^3=1/sqrt{{3^2+2^2}/a_A^sqrt_5(t^3+1)}";
//	t = "t^3=1/sqrt_{4 & 5 & 6 & 7 & 8 & 9}{{3^2+2^2}/a_A^sqrt_5(t^3+1)}";
//	t = "B(:a, :b, :c, t) = :a*(1-t)^2+:b*2*t*(1-t)+:c*t^2";
//	t = "sin{:a+:b}=sin{:a}*cos{:b}+sin{:a}*cos{:b}";
//	t = "a^n+b^n=c^n";
//	t = "a^2+b^2 <= 2*a*b";
//	t = "/cosh{x}=sqrt{1+sinh_|2^|2^2{x}}";
//	t = "sum_|{i=0}^|10{i^2}";
//	t = "iint_|{x=0}^|10{i~x^2~dx} & int{x^2~dx} & iiint{x^2~d/x}";
//	t = "n!=int_|{x=0}^|oo{x^{n-1}~e^x~dx}";
//	t = "n!<>0=iint_|{x,y=0}^|oo{x^{n-1}~e^x~@x~@y}";
//	t = "0=0+0-0<>0<-0 & x->oo<->0 & 0<=0>=0<0>0";
//	t = "e=lim_|{x->oo}(1^x+1/x)^x";
//	t = ":x(x)=sum_|{k=0}^|oo{{(x-a)^k~:x^(k)(a)}/k!} & \"\"=:x(a)+(x-a)~d::x(a)/dx"
//	"+(x-a)^2/2!~d^2::x(a)/dx^2+(x-a)^3/3!~d^3::x(a)/dx^3+...";
	t = "oiint_|{:j}{r~dA}={:p~r^2}/4";
	tw.Background(PaintRect(fd, t));
	tw.Run();
}
#endif

END_UPP_NAMESPACE
