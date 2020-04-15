#include <CtrlLib/CtrlLib.h>

namespace Upp {

#include "GridDisplay.h"
#define  IMAGECLASS GridImg
#define  IMAGEFILE <GridCtrl/GridCtrl.iml>
#include <Draw/iml_source.h>
#include "GridUtils.h"


GridDisplay& StdGridDisplay()
{
	static GridDisplay h;
	return h;
}

static Image (*vhdr[])() =
{
	GridImg::VHdr0,
	GridImg::VHdr1,
	GridImg::VHdr2,
	GridImg::VHdr3,
	GridImg::VHdr4,
	GridImg::VHdr5,
	GridImg::VHdr6,
	GridImg::VHdr7
};

void GridDisplay::SetDefault()
{
	SetTextAlign();
	SetHorzMargin();
	SetVertMargin();
	SetTheme();
	ReverseSortIcon(false);
}

void GridDisplay::SetTheme(int th)
{
	theme = th;
	hdrhigh = Contrast(vhdr[theme](), 230);	
}

WString GridDisplay::GetStdConvertedValue(const Value &v) const
{
	return IsString(v) ? v : StdConvert().Format(v);
}

void GridDisplay::Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
						Color &fg, Color &bg, Font &fnt, bool found, int fs, int fe)
{
	real_size.cx = 0;
	real_size.cy = 0;

	if(cx == 0 || cy == 0) return;

	Color mg = bg;

	int al = style & GD::ALIGN ? style & GD::ALIGN : align;

	if(cx > lm + rm && cy > tm + bm)
	{
		int nx = x + lm;
		int ny = y + tm;
		int ncx = cx - lm - rm;
		int ncy = cy - tm - bm;

		if(IsNull(bgImg))
		{
			if(lm > 0) w.DrawRect(x, y, lm, cy, mg);
			if(rm > 0) w.DrawRect(x + cx - rm, y, rm, cy, mg);
			if(tm > 0) w.DrawRect(x, y, cx, tm, mg);
			if(bm > 0) w.DrawRect(x, y + cy - bm, cx, bm, mg);

			w.DrawRect(nx, ny, ncx, ncy, bg);
		}
		else
			w.DrawImage(x, y, cx, cy, bgImg);

		w.Clip(nx, ny, ncx, ncy);

		if(!leftImg.IsEmpty())
		{
			Size isz = leftImg.GetSize();
			w.DrawImage(nx, ny + (cy - isz.cy) / 2, style & GD::READONLY ? Grayscale(leftImg) : leftImg);
			nx += isz.cx + 3;
			ncx -= isz.cx + 3;
		}
		if(!rightImg.IsEmpty())
		{
			Size isz = rightImg.GetSize();
			w.DrawImage(nx + ncx - isz.cx, y + (cy - isz.cy) / 2, style & GD::READONLY ? Grayscale(rightImg) : rightImg);
		}
		if(!centerImg.IsEmpty())
		{
			Size isz = centerImg.GetSize();
			w.DrawImage(x + (cx - isz.cx) / 2, y + (cy - isz.cy) / 2, style & GD::READONLY ? Grayscale(centerImg) : centerImg);
		}

		if(!(style & GD::NOTEXT))
		{
			DrawText(w, nx, nx, ny, ncx, ncy, al, GetStdConvertedValue(val), fnt, fg, bg, found, fs, fe, style & GD::WRAP);
		}

		w.End();
	}
	else
		w.DrawRect(x, y, cx, cy, bg);
}

void GridDisplay::PaintFixed(Draw &w, bool firstx, bool firsty, int x, int y, int cx, int cy, const Value &val, dword style, Font &fnt,
		                     bool indicator, bool moved, int sortmode, int sortcol, int sortcnt, bool horizontal)
{
	real_size.cx = 0;
	real_size.cy = 0;
	
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
			if(sx > 0 && sy > 0)
				w.DrawImage(x, y, sx, sy, highlight ? hdrhigh : vhdr[theme]());
		}

		//Color dark(76, 83, 92);
		Color dark(155, 154, 153);
		//Color dark(155, 154, 153);
		Color bright(White);

		if(!firstx) w.DrawRect(x, y, 1, cy, bright);
		if(!firsty) w.DrawRect(x, y, cx, 1, bright);

		if(firstx) w.DrawRect(x, y, 1, cy, dark);
		if(firsty) w.DrawRect(x, y, cx, 1, dark);

		w.DrawRect(x + cx - 1, y, 1, cy, dark);
		w.DrawRect(x, y + cy - 1, cx, 1, dark);
	}

	int tx = x + lm;

	if(firsty)
	{
		if(!leftImg.IsEmpty())
		{
			Size isz = leftImg.GetSize();
			w.DrawImage(tx, y + (cy - isz.cy) / 2, leftImg);
			tx += isz.cx + 3;
		}
		if(!rightImg.IsEmpty())
		{
			Size isz = rightImg.GetSize();
			w.DrawImage(x + cx - isz.cx - 3, y + (cy - isz.cy) / 2, rightImg);
		}
		if(!centerImg.IsEmpty())
		{
			Size isz = centerImg.GetSize();
			w.DrawImage(x + (cx - isz.cx) / 2, y + (cy - isz.cy) / 2, centerImg);
		}
	}

	if(moved)
	    DrawBorder(w, x, y, cx, cy, BlackBorder);

	Color col = style & GD::READONLY ? SGray() : SBlack();

	if(sortmode > 0)
	{
		Size isz = GridImg::SortAsc().GetSize();

		int yf = y + (cy - isz.cy) / 2;
		int xf = tx + 2;
		tx = xf + isz.cx + 1;

		if(sortcol > 0 && sortcnt > 1)
		{
			String tcol = AsString(sortcol);

			Size tsz = GetTextSize(tcol, fnt);
			w.DrawText(tx, y + (cy - tsz.cy) / 2, tcol, fnt);
			tx += tsz.cx;
		}

		bool asc = sortmode == 1;
		if(reverse_sort_icon)
			asc = !asc;
		w.DrawImage(xf, yf, asc ? GridImg::SortAsc() : GridImg::SortDsc(), col);

		tx += 3;
	}
	
	if(indicator)
	{
		w.Clip(x, y, cx, cy);

		Image img;
		
		if((style & GD::CURSOR) && (style & GD::SELECT))
		{
			img = GridImg::FocSel();
		}
		else if(style & GD::CURSOR)
		{
			img = GridImg::Focused();
		}
		else if(style & GD::SELECT)
		{
			img = GridImg::Selected();
		}

		if(!img.IsEmpty())
		{
			Size isz = img.GetSize();
			int xp = IsNull(val)
				? x + (cx - isz.cx) / 2
				: tx;
			w.DrawImage(xp, y + (cy - isz.cy) / 2, img, col);
			if(!IsNull(val))
				tx += isz.cx + 3;
		}
		
		w.End();
	}
	
	if(cx > lm + rm && cy > tm + bm)
	{
		int nx = x + lm;
		int ny = y + tm;
		int ncx = cx - lm - rm;
		int ncy = cy - tm - bm;

		w.Clip(nx, ny, ncx, ncy);

		int al = style & GD::ALIGN ? style & GD::ALIGN : align;

		Color fg = style & GD::READONLY ? SColorDisabled() : SColorText();

		DrawText(w, tx, nx, ny, ncx, ncy, al, GetStdConvertedValue(val), fnt, fg, SColorPaper, 0, 0, 0, style & GD::WRAP);

		w.End();
	}
}

int GridDisplay::GetLinesCount(int cx, const wchar* s, const Font& font, bool wrap)
{
	if(*s == 0)
		return 0;

	const wchar *p = s;
	const wchar *t = s;
		
	const wchar * e = t;
	int ccx = max(5, cx);
	int enters = 0;
	int lines = 0;

	while(*p)
	{
		bool nextline = *p == '\n';
		enters += int(nextline);

		if(nextline || *(p + 1) == '\0')
		{
			if(p - e > 0 && wrap)
			{
				int tcx = GetTextSize(e, font, int(p - e)).cx;
				if(tcx > cx)
				{
					lines += tcx / ccx;
					if(tcx % ccx > 0)
						lines++;						
				}
				else
					lines++;
				
			}
			else
				lines++;
			
			e = p + 1;
		}
		p++;
	}
	return lines;
}

void GridDisplay::DrawText(Draw &w, int mx, int x, int y, int cx, int cy, int align, const wchar *s, const Font &font, const Color &fg, const Color &bg, bool found, int fs, int fe, bool wrap)
{
	if(*s == 0)
		return;

	int tcy = font.Info().GetHeight();

	const wchar *p = s;
	const wchar *t = s;

	int lines = 0;
//	int enters = 0;

	int ty = y;
	Size tsz;

	if((align & GD::VCENTER) || (align & GD::BOTTOM))
	{
		lines = GetLinesCount(cx, s, font, wrap);

		if(align & GD::VCENTER)
			ty = max(ty, y + (cy - tcy * lines) / 2);
		else
			ty = y + cy - tcy * lines;
	}

	//w.DrawText(x, y, Format("lines: %d, cx:%d, cy%d", lines, cx, cy));
	Size isz = GridImg::Dots2().GetSize();
	int gcx = cx - (wrap ? 0 : isz.cx);

	real_size.cy = lines > 1 ? lines * tcy : 0;

	while(true)
	{
		bool nextline = *p == '\n';
		bool caret    = *p == '\r';
		bool endtext  = *p == '\0';

		const wchar * pp = p;

		bool textbreak = false;
		
		if(nextline || endtext)
		{
			/*int kk = p - t;
			if(p - t <= 1)
			{
				//break;
				kk = 0;
			}*/

			int tx = x;
			tsz = GetTextSize(t, font, int(p - t));

			if(tsz.cx > gcx)
			{
				int size = 0;
				const wchar *e = t;
				while(e < p)
				{
					int tcx = GetTextSize(e, font, 1).cx;
					size += tcx;
					if(size > gcx)
						break;
					e++;
				}

				p = e;
				if(wrap)
				{
					textbreak = true;
					if(p == t)
						p = t + 1;
				}
			}

			if(align & GD::RIGHT)
				tx = x + cx - tsz.cx;
			else if(align & GD::HCENTER)
				tx = x + (cx - tsz.cx) / 2;

			Color tfg = fg;

			if(found)
			{
				int chs = int(t - s);
				int che = int(p - s - 1);

				if(fs <= che && fe >= chs)
				{
					int scx = GetTextSize(t, font, max(chs, fs) - chs).cx;
					int ecx = GetTextSize(t, font, min(che, fe) - chs + 1).cx;
					Color nbg(255 - fg.GetR(), 255 - fg.GetG(), 255 - fg.GetB());
					w.DrawRect(max(mx, tx) + scx, ty, ecx - scx, tcy, Blend(nbg, bg, 100));
				}
			}

			bool dots = !wrap && tsz.cx > gcx;
			if(dots)
			{
				real_size.cx = max(real_size.cx, tsz.cx);
				
				w.Clip(x, y, cx, cy);
				w.DrawImage(x + cx - isz.cx, ty + font.Info().GetAscent() - isz.cy, GridImg::Dots2, tfg);
				w.End();
				
				if(p > t)
				{
					w.Clip(x, y, cx - isz.cx , cy);
					w.DrawText(max(mx, tx), ty, t, font, tfg, int(p - t));
					w.End();
				}
			}
			else
				w.DrawText(max(mx, tx), ty, t, font, tfg, int(p - t));

			ty += tcy;
			t = textbreak ? p : (p = pp + 1);
		}
		
		if(caret)
			*(char *) p = ' ';

		if(!textbreak)
		{
			if(endtext)
				break;
			else
				p++;
		}
	}
}

}
