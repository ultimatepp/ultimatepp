#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

#include "GridDisplay.h"
#define  IMAGEFILE <GridCtrl/GridDisplay.iml>
#define  IMAGECLASS GridDispImg
#include <Draw/iml_source.h>
#include "GridUtils.h"

GridDisplay StdGridDisplay;

static Image (*vhdr[])() =
{
	GridDispImg::ImgVHdr0,
	GridDispImg::ImgVHdr1,
	GridDispImg::ImgVHdr2,
	GridDispImg::ImgVHdr3,
	GridDispImg::ImgVHdr4,
	GridDispImg::ImgVHdr5
};


void GridDisplay::Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
						Color &fg, Color &bg, Font &fnt, bool found, int fs, int fe)
{
	if(cx == 0 || cy == 0) return;

	if(style & GD::READONLY)
	{
		bg = Blend(bg, SGray(), 40);
		fg = Blend(fg, SGray(), 200);
	}

	Color mg = bg;

	int al = style & GD::ALIGN ? style & GD::ALIGN : align;

	if(cx > lm + rm && cy > tm + bm)
	{
		if(lm > 0) w.DrawRect(x, y, lm, cy, mg);
		if(rm > 0) w.DrawRect(x + cx - rm, y, rm, cy, mg);
		if(tm > 0) w.DrawRect(x, y, cx, tm, mg);
		if(bm > 0) w.DrawRect(x, y + cy - bm, cx, bm, mg);

		int nx = x + lm;
		int ny = y + tm;
		int ncx = cx - lm - rm;
		int ncy = cy - tm - bm;

		w.DrawRect(nx, ny, cx - lm - rm, cy - tm - bm, bg);

		w.Clip(nx, ny, ncx, ncy);

		if(!leftImg.IsEmpty())
		{
			Size isz = leftImg.GetSize();
			w.DrawImage(nx, ny + (cy - isz.cy) / 2, leftImg);
			nx += isz.cx + 3;
		}
		if(!rightImg.IsEmpty())
		{
			Size isz = rightImg.GetSize();
			w.DrawImage(nx + ncx - isz.cx, y + (cy - isz.cy) / 2, rightImg);
		}
		if(!centerImg.IsEmpty())
		{
			Size isz = centerImg.GetSize();
			w.DrawImage(x + (cx - isz.cx) / 2, y + (cy - isz.cy) / 2, centerImg);
		}

		if(!(style & GD::NOTEXT))
			DrawText(w, nx, nx, ny, ncx, ncy, al, (const wchar *) (WString) val, fnt, fg, bg, found, fs, fe, style & GD::WRAP);

		w.End();
	}
	else
		w.DrawRect(x, y, cx, cy, bg);
}

void GridDisplay::PaintFixed(Draw &w, bool firstx, bool firsty, int x, int y, int cx, int cy, const Value &val, dword style,
		                     bool indicator, bool moved, int sortmode, int sortcol, int sortcnt, bool horizontal)
{
	static int ccy = -1;

	bool chameleon = style & GD::CHAMELEON;
	bool highlight = style & GD::HIGHLIGHT;
	bool readonly  = style & GD::READONLY;


	if(chameleon)
	{
		int ncx = cx;
		int nx = x;

		int ht = HeaderCtrl::StyleDefault().gridadjustment;

		if(firstx)
		{
			if(ncx >= -ht)
			{
				ncx -= ht;
				if(ncx < 0)
					ncx = 0;
			}
		}
		else
			nx -= ht;

		int q = CTRL_NORMAL;
		if(highlight)
			q = CTRL_HOT;
		if(readonly)
			q = CTRL_DISABLED;

		ChPaint(w, nx, y, ncx, cy, HeaderCtrl::StyleDefault().look[q]);
	}
	else
	{
		if(theme < 0)
			w.DrawRect(x, y, cx, cy, highlight ? Blend(SColorFace(), White, 128) : SColorFace());
		else
		{
			int sx = cx > 1 ? cx : 1;
			int sy = cy - 1;
			w.DrawImage(x, y, sx, sy, highlight ? Contrast(vhdr[theme](), 230) : vhdr[theme]());
		}

		Color dark(76, 83, 92);
		Color bright(White);


		if(!firstx) w.DrawRect(x, y, 1, cy, bright);
		if(!firsty) w.DrawRect(x, y, cx, 1, bright);

		if(firstx) w.DrawRect(x, y, 1, cy, dark);
		if(firsty) w.DrawRect(x, y, cx, 1, dark);

		w.DrawRect(x + cx - 1, y, 1, cy, dark);
		w.DrawRect(x, y + cy - 1, cx, 1, dark);

	}

	if(moved)
	    DrawBorder(w, x, y, cx, cy, BlackBorder);

	int tx = x + lm;

	Color col = style & GD::READONLY ? Gray : Black;

	if(sortmode > 0)
	{
		Size isz = GridDispImg::ImgSortAsc().GetSize();

		int yf = y + (cy - isz.cy) / 2;
		int xf = x + 2;
		tx = xf + isz.cx + 1;

		if(sortcol > 0 && sortcnt > 0)
		{
			String tcol = AsString(sortcol);

			Size tsz = GetTextSize(tcol, font);
			w.DrawText(tx, y + (cy - tsz.cy) / 2, tcol, font);
			tx += tsz.cx;
		}

		if(sortmode == 1)
			w.DrawImage(xf, yf, GridDispImg::ImgSortAsc(), col);
		else
			w.DrawImage(xf, yf, GridDispImg::ImgSortDsc(), col);

		tx += 3;

	}
	if(indicator)
	{
		w.Clip(x, y, cx, cy);
		if((style & GD::CURSOR) && (style & GD::SELECT))
		{
			Size isz = GridDispImg::ImgFocSel().GetSize();
			w.DrawImage(x + (cx - isz.cx) / 2, y + (cy - isz.cy) / 2, GridDispImg::ImgFocSel(), col);
		}
		else if(style & GD::CURSOR)
		{
			Size isz = GridDispImg::ImgFocused().GetSize();
			w.DrawImage(x + (cx - isz.cx) / 2, y + (cy - isz.cy) / 2, GridDispImg::ImgFocused(), col);
		}
		else if(style & GD::SELECT)
		{
			Size isz = GridDispImg::ImgSelected().GetSize();
			w.DrawImage(x + (cx - isz.cx) / 2, y + (cy - isz.cy) / 2, GridDispImg::ImgSelected(), col);
		}
		w.End();
	}
	else if(cx > lm + rm && cy > tm + bm)
	{
		int nx = x + lm;
		int ny = y + tm;
		int ncx = cx - lm - rm;
		int ncy = cy - tm - bm;

		w.Clip(nx, ny, ncx, ncy);

		int al = style & GD::ALIGN ? style & GD::ALIGN : align;

		Color fg = style & GD::READONLY ? SColorDisabled() : SColorText();

		DrawText(w, tx, nx, ny, ncx, ncy, al, (const wchar *) (WString) (val), font, fg, SColorPaper, 0, 0, 0, style & GD::WRAP);

		w.End();
	}
}

void GridDisplay::DrawText(Draw &w, int mx, int x, int y, int cx, int cy, int align, const wchar *s, const Font &font, const Color &fg, const Color &bg, bool found, int fs, int fe, bool wrap)
{
	int tcy = font.Info().GetHeight();
	//w.Clip(x, y, cx, cy);

	const wchar *p = s;
	const wchar *t = s;

	int lines = 0;

	int ty = y;
	Size tsz;

	if((align & GD::VCENTER) || (align & GD::BOTTOM))
	{
		const wchar * e = t;

		while(*p)
		{
			if(*p == '\n' || *(p + 1) == '\0')
			{
				if(wrap)
				{
					int tcx = GetTextSize(e, font, p - e).cx;
					if(tcx > cx)
					{
						lines += tcx / cx;// + ((tcx % cx) > 0 ? 1 : 0);
						if(tcx % cx > 0)
							lines++;
					}
					else
						lines++;
				}
				else
					lines++;
				e = p;
			}
			p++;
		}
		if(align & GD::VCENTER)
			ty = y + (cy - tcy * lines) / 2;
		else
			ty = y + cy - tcy * lines;
		p = s;
	}

	while(true)
	{
		bool nextline = *p == '\n';
		bool caret    = *p == '\r';
		bool endtext  = *p == '\0';

		if(nextline || endtext)
		{
			int tx = x;
			tsz = GetTextSize(t, font, p - t);

			bool textbreak = false;
			if(wrap && tsz.cx > cx)
			{
				int size = 0;
				const wchar * e = t;
				while(e < p)
				{
					size += GetTextSize(e, font, 1).cx;
					if(size > cx)
						break;
					e++;
				}
				p = e;
				if(p == t)
					p = t + 1;
				textbreak = true;
			}

			if(align & GD::RIGHT)
				tx = x + cx - tsz.cx;
			else if(align & GD::HCENTER)
				tx = x + (cx - tsz.cx) / 2;

			if(found)
			{
				int s = GetTextSize(t, font, fs).cx;
				int e = GetTextSize(t, font, fe + 1).cx;
				w.DrawRect(max(mx, tx) + s, y, e - s, cy, Color(255, 242, 0));
			}

			w.DrawText(max(mx, tx), ty, t, font, fg, p - t);
			ty += tcy;
			t = textbreak ? p : p + 1;
		}
		if(caret)
			*(char *) p = ' ';
		if(endtext)
			break;
		else
			p++;

	}

	//w.End();
}

END_UPP_NAMESPACE
