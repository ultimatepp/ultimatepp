#include <CtrlLib/CtrlLib.h>
#include "DateTimeCtrl.h"
#define TFILE <DateTimeCtrl/DateTimeCtrl.t>
#include <Core/t.h>
#define IMAGEFILE "DateTimeCtrl.iml"
#include <Draw/iml_source.h>

const Point DateTimePopUp::nullday = Point(-1, -1);

const char *BtnTip(int n)
{
	switch(n)
	{
		case 0: return t_("Previous month"); break;
		case 1: return t_("Next month");  break;
		case 2: return t_("Previous year"); break;
		case 3: return t_("Next year");  break;
		default: return "??";
	}
}

const char *Days(int day)
{
	switch(day)
	{
		case 0: return t_("Mo");
		case 1: return t_("Tu");
		case 2: return t_("We");
		case 3: return t_("Th");
		case 4: return t_("Fr");
		case 5: return t_("Sa");
		case 6: return t_("Su");
		default: return "??";
	}
}

const char *Months(int month)
{
	switch(month)
	{
		case 0:  return t_("January");
		case 1:  return t_("February");
		case 2:  return t_("March");
		case 3:  return t_("April");
		case 4:  return t_("May");
		case 5:  return t_("June");
		case 6:  return t_("July");
		case 7:  return t_("August");
		case 8:  return t_("September");
		case 9:  return t_("October");
		case 10: return t_("November");
		case 11: return t_("December");
		default: return "??";
	}
}


DateTimePopUp::DateTimePopUp()
{
	SetFrame(BlackFrame());
	Add(mleft);
	Add(mright);
	Add(yleft);
	Add(yright);

	mleft  <<= THISBACK(OnMonthLeft);
	mright <<= THISBACK(OnMonthRight);
	yleft  <<= THISBACK(OnYearLeft);
	yright <<= THISBACK(OnYearRight);

	open = false;
	userdate = false;
	selall = false;

	colHeader	= Color(46, 149, 210);
	colBg		= White;
	colFg		= Black;
	colCurDayBg	= Color(200, 250, 0);
	colCurDayFg	= Black;
	colCurDayBr	= Black;
	colSelDayBg	= Color(255, 254, 220);
	colSelDayFg	= Black;
	colSelDayBr	= Black;
	colGrayDay	= Color(200, 200, 200);
	colCurDate	= White;
	colToday	= Black;
	colSelToday = Green;
	colSelDay	= Green;
	colHLine    = Gray;
	colVLine    = Gray;
	colDayNames	= Black;
	colWeek		= Black;

	bs = 5;
	lx = 30;
	ly = 15;
	hs = 18;
	ts = 15;
	ls = 3;

}

void DateTimePopUp::Reset()
{
	Date tdate = GetSysDate();
	tday   = tdate.day;
	tmonth = tdate.month;
	tyear  = tdate.year;

	Date cdate = (Date) (*date);
	if(cdate.IsValid())
	{
		cday   = cdate.day;
		cmonth = cdate.month;
		cyear  = cdate.year;
	}
	else
	{
		cday   = tday;
		cmonth = tmonth;
		cyear  = tyear;
	}

	if(!userdate)
	{
		day   = cday;
		month = cmonth;
		year  = cyear;
	}

	userdate = false;

	selday = 0;

	newday = oldday = nullday;
	col = row = -1;
	today = Format("%s: %.2d.%.2d.%.4d", t_("Today"), tdate.day, tdate.month, tdate.year);

	istoday = false;

	mleft.Tip(BtnTip(0));
	mright.Tip(BtnTip(1));
	yleft.Tip(BtnTip(2));
	yright.Tip(BtnTip(3));

}

void DateTimePopUp::OnMonthLeft()
{
	if(--month < 1)
	{
		month = 12;
		if(year > 0)
			year--;
	}
	col = row = -1;
	Refresh();
}

void DateTimePopUp::OnMonthRight()
{
	if(++month > 12)
	{
		month = 1;
		year++;
	}
	col = row = -1;
	Refresh();
}

void DateTimePopUp::OnYearLeft()
{
	if(year > 0) year--;
	col = row = -1;
	Refresh();
}

void DateTimePopUp::OnYearRight()
{
	year++;
	col = row = -1;
	Refresh();
}

int	DateTimePopUp::DayOfWeek(int day, int month, int year, int zelleroffset)
{
	if(month < 3)
    {
        month += 12 ;
        year -= 1 ;
    }

	int n1 = (26 * (month + 1)) / 10;
	int n2 = (125 * year) / 100;

	return ((day + n1 + n2 - (year / 100) + (year / 400) - zelleroffset) % 7);
}

int DateTimePopUp::WeekOfYear(int day, int month, int year) /* ISO-8601 */
{
	const static int mnth[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
	int weeknum;

	bool cleapyear = IsLeapYear(year);
	bool bleapyear = IsLeapYear(year - 1);

	int daynum = day + mnth[month - 1];
	if(cleapyear && month > 2)
		daynum += 1;

	int yy = (year - 1) % 100;
	int c = (year - 1) - yy;
	int g = yy + yy / 4;
	int jan1weekday = 1 + (((((c / 100) % 4) * 5) + g) % 7);
	int h = daynum + (jan1weekday - 1);
	int weekday = 1 + ((h -1) % 7);

	int yearnum;

	if(daynum <= (8 - jan1weekday) && jan1weekday > 4)
	{
	     yearnum = year - 1;
	     if(jan1weekday == 5 || (jan1weekday == 6 && bleapyear))
	     	weeknum = 53;
	     else
	     	weeknum = 52;
	}
	else yearnum = year;

	if(yearnum == year)
	{
		int yeardays = cleapyear ? 366 : 365;
		if((yeardays - daynum) < (4 - weekday))
		{
		   yearnum = year + 1;
		   weeknum = 1;
		}
	}
	if(yearnum == year)
	{
		int fday = daynum + (7 - weekday) + (jan1weekday -1);
		weeknum = fday / 7;
		if(jan1weekday > 4)
			weeknum -= 1;
	}

	return weeknum;
}

void DateTimePopUp::LeftDown(Point p, dword keyflags)
{
	int day = Day(newday);

	if(!selall && day < 0) return;

	GetParent()->IgnoreMouseClick();
	Close();

	if(newday != nullday)
	{
		if(day < 0)
		{
			day = -day;
			if(lastrow < 3)
				month--;
			else
				month++;

			if(month < 1) month = 12;
			if(month > 12) month = 1;
		}
	}
	else
		day = 1;

	if(p.y <= hs)
		day = 1;

	date->RemoveSelection();

	if(istoday)
		*date <<= Date(tyear, tmonth, tday);
	else
		*date <<= Date(year, month, day);

	date->WhenAction();
}

void DateTimePopUp::MouseMove(Point p, dword keyflags)
{
	newday = GetDay(p);
	if(newday != oldday)
	{
		if(oldday != nullday)
		{
			bool b = (selall == false ? Day(oldday) > 0 : true);
			if(b)
			{
				selday = 0;
				col = oldday.x;
				row = oldday.y;
				RefreshDay(oldday);
			}
		}
		if(newday != nullday)
		{
			bool b = (selall == false ? Day(newday) > 0 : true);
			if(b)
			{
				selday = Day(newday);
				col = newday.x;
				row = newday.y;
				RefreshDay(newday);
			}
		}
		oldday = newday;
	}
	istoday = MouseOnToday(p);
	if(istoday != wastoday)
	{
		wastoday = istoday;
		Size sz = GetSize();
		Refresh(0, sz.cy - ts - bs, sz.cx, ts + bs);
	}
}

Image DateTimePopUp::CursorImage(Point p, dword keyflags)
{
	bool b = (selall == false ? Day(newday) > 0 : true);
	if((newday != nullday && b) || istoday)
		return CtrlImg::HandCursor();
	else
		return Image::Arrow();
}

bool DateTimePopUp::MouseOnToday(Point p)
{
	Size sz = GetSize();
	int x0 = (sz.cx - sztoday.cx) / 2;
	int x1 = x0 + sztoday.cx;
	int y0 = sz.cy - (ts + bs + sztoday.cy) / 2;
	int y1 = y0 + sztoday.cy;
	return (p.x >= x0 && p.x <= x1 && p.y >= y0 && p.y <= y1);
}

void DateTimePopUp::RefreshDay(Point p)
{
	int y0 = hs + ly + ls + p.y * rowh;
	int y1 = y0 + rowh;

	int x0 = lx + ls + p.x * colw;
	int x1 = x0 + colw;

	Refresh(Rect(x0, y0, x1, y1));
	Sync();
}

Point DateTimePopUp::GetDay(Point p)
{

	for(int i = 0; i < rows; i++)
	{
		int y0 = hs + ly + ls + i * rowh;
		int y1 = y0 + rowh;

		if(p.y >= y0 && p.y < y1)
		for(int j = 0; j < cols; j++)
		{
			int x0 = lx + ls + j * colw;
			int x1 = x0 + colw;
			if(p.x >= x0 && p.x < x1)
				return Point(j, i);
		}
	}
	return Point(-1, -1);
}

void DateTimePopUp::Paint(Draw &w)
{
	Size sz = GetSize();
	Size tsz;
	String str;
	int yp;
	int d, m, y;
	int im, jm, is, js;

	m = month;
	y = year;

	colw = (sz.cx - lx - ls * 2) / cols;
	rowh = (sz.cy - hs - ly - ts - ls * 2) / rows;

	Font fnt = StdFont();
	Color color;

	bool refall = (col < 0 && row < 0);

	if(refall)
	{
		w.DrawRect(0, 0, sz.cx, hs, colHeader);
		w.DrawRect(Rect(0, hs, sz.cx, sz.cy - ts - bs), colBg);

		w.DrawLine(bs, ly + hs, sz.cx - bs, ly + hs, 0, colHLine);
		w.DrawLine(lx, hs + bs, lx, sz.cy - bs - ts, 0, colVLine);

		yp = hs + ly / 2;

		str = Format("%s %d", Months(month - 1), year);
		tsz = w.GetTextSize(str);
		w.DrawText((sz.cx - tsz.cx) / 2, (hs - tsz.cy) / 2, str, StdFont().Bold(), colCurDate);
		tsz = w.GetTextSize(t_("Wk"));
		w.DrawText((lx - tsz.cx) / 2, yp - tsz.cy / 2, t_("Wk"), StdFont(), colWeek);


		for(int i = 0; i < cols; i++)
		{
			tsz = w.GetTextSize(Days(i));
			w.DrawText(lx + ls + i * colw + (colw - tsz.cx) / 2, yp - tsz.cy / 2,
			           Days(i),  StdFont(), colDayNames);
		}

		color = colToday;
		fnt.Bold().NoUnderline();
		if(istoday)
		{
			color = colSelToday;
			fnt.Underline();
		}
		sztoday = w.GetTextSize(today, fnt);
		w.DrawRect(Rect(0, sz.cy - ts - bs, sz.cx, sz.cy), colBg);
		w.DrawText((sz.cx - sztoday.cx) / 2, sz.cy - (ts + bs + sztoday.cy) / 2, today, fnt, color);

		d = 1;
		int fd = DayOfWeek(1, month, year);

		if(fd > 0)
		{
			if(--m < 1)
			{
				m = 12;
				y -= 1;
			}
			d = GetDaysOfMonth(m, y) - fd + 1;
		}
	}
	color = colFg;

	int fh = w.GetFontInfo().GetHeight();

	if(refall)
	{
		im = rows;
		jm = cols;
		is = js = 0;
	}
	else
	{
		im = row + 1;
		jm = col + 1;
		is = row;
		js = col;
		d = Day(col, row);
		if(d < 0) m = 0;
	}

	for(int i = is; i < im; i++)
	{
		int yp = hs + ly + ls + i * rowh;
		int yc = (rowh - fh) / 2;

		str = AsString(WeekOfYear(d, m, y));
		w.DrawText((lx - w.GetTextSize(str).cx) / 2, yp + yc, str, StdFont(), colWeek);

		for(int j = js; j < jm; j++)
		{
			fnt.NoBold().NoUnderline();

			int xp = lx + ls + j * colw;

			bool special = false;

			if(m == month)
			{
				Day(j, i) = d;

				color = Black;

				if(j == 6)
				{
					color = Red;
				}
				if(d == tday && m == tmonth && y == tyear)
				{
					int x0 = xp;
					int x1 = xp + colw;
					w.DrawRect(x0 + 1, yp - 0, colw - 1, rowh - 1, colCurDayBg);
					DrawFrame(w, x0 + 1, yp - 0, colw - 1, rowh - 1, colCurDayBr);
					color = colCurDayFg;
					fnt.Bold();
					special = true;
				}
				if(d == cday && m == cmonth && y == cyear)
				{
					int x0 = xp;
					int x1 = xp + colw;
					w.DrawRect(x0 + 1, yp - 0, colw - 1, rowh - 1, colSelDayBg);
					DrawFrame(w, x0 + 1, yp - 0, colw - 1, rowh - 1, colSelDayBr);
					color = colSelDayFg;
					fnt.Bold();
					special = true;
				}
			}
			else
			{
				color = colGrayDay;
				Day(j, i) = d > 0 ? -d : d;
			}

			if(d == selday)
			{
				if(d < 0 && selall)
				{
					color = colGrayDay;
					fnt.Bold();
					if(!special) fnt.Underline();
				}
				if(d > 0 && month == m)
				{
					color = colSelDay;
					fnt.Bold();
					if(!special) fnt.Underline();
				}
				selmonth = m;
			}

			if(col >= 0)
			{
				if(special)
				{
					w.DrawRect(xp, yp, 1, rowh, colBg);
					w.DrawRect(xp, yp + rowh - 1, colw, 1, colBg);
				}
				else
					w.DrawRect(xp, yp, colw, rowh, colBg);
			}

			str = AsString(abs(d));
			w.DrawText(xp + (colw - w.GetTextSize(str, fnt).cx) / 2, yp + yc , str, fnt, color);

			if(++d > GetDaysOfMonth(m, y))
			{
				d = 1;
				if(++m > 12)
				{
					m = 1;
					y += 1;
				}
			}
		}
	}
	lastrow = row;
	col = row = -1;
}


void DateTimePopUp::Deactivate()
{
	if(open)
	{
		open = false;
		Close();
		IgnoreMouseClick();
	}
}

bool DateTimePopUp::Key(dword key, int count)
{
	if(key == K_ESCAPE)
		Deactivate();

	return true;
}


void DateTimePopUp::PopUp(Ctrl *owner, Rect &rt)
{
	Close();
	Reset();

	mleft.LeftPos(2, 15).TopPos(2, 13);
	mleft.SetLeft().DrawEdge(0).SetImage(ImgLeft2());
	mright.LeftPos(17, 15).TopPos(2, 13);
	mright.SetRight().DrawEdge(0).SetImage(ImgRight2());

	yright.RightPos(2, 15).TopPos(2, 13);
	yright.SetRight().DrawEdge(0).SetImage(ImgRight2());
	yleft.RightPos(17, 15).TopPos(2, 13);
	yleft.SetLeft().DrawEdge(0).SetImage(ImgLeft2());

	SetRect(rt);
	Ctrl::PopUp(owner, true, true, IsXPStyle());
	open = true;
}


DateTimeCtrl::DateTimeCtrl()
{
	AddFrame(drop);
	drop.SetMonoImage(IsXPStyle() ? CtrlImg::SmallDown() : CtrlImg::smalldown());
	drop.NoWantFocus();
	calendar.date = this;
	width = defWidth;
	height = defHeight;

	drop <<= THISBACK(OnDrop);
}

DateTimeCtrl& DateTimeCtrl::SetDate(int d, int m, int y)
{
	calendar.day   = d;
	calendar.month = m;
	calendar.year  = y;

	calendar.userdate = true;

	SetData(Date(y, m, d));

	return *this;
}

DateTimeCtrl& DateTimeCtrl::SetSize(int w, int h)
{
	if(w >= defWidth)
		width = w;
	if(h >= defHeight)
		height = h;

	return *this;
}

void DateTimeCtrl::OnDrop()
{
    Rect rw = Ctrl::GetWorkArea();
	Rect rs = GetScreenRect();
	Rect r;
	r.left   = rs.left;
	r.right  = rs.left + width;
	r.top    = rs.bottom;
	r.bottom = rs.bottom + height;

	if(r.bottom > rw.bottom)
	{
		r.top = rs.top - height;
		r.bottom = rs.top;
	}
	if(r.right > rw.right)
	{
		int diff;
		if(rs.right <= rw.right)
			diff = rs.right - r.right;
		else
			diff = rw.right - r.right;

		r.left += diff;
		r.right += diff;
	}
	if(r.left < rw.left)
	{
		int diff = rw.left - r.left;
		r.left += diff;
		r.right += diff;

	}
	calendar.PopUp(this, r);
}


FlatButton::FlatButton()
{
	hl = Color(235, 240, 243);
	bg = Color(179, 220, 253);
	fg = White;
	left = true;
}

void FlatButton::DrawFrame(Draw &w, const Rect &r, Color lc, Color tc, Color rc, Color bc)
{
	w.DrawRect(r.left, r.top, r.left + 1, r.bottom, lc);
	w.DrawRect(r.right - 1, r.top, r.right, r.bottom, rc);
	w.DrawRect(r.left, r.top, r.right, r.top + 1, tc);
	w.DrawRect(r.left, r.bottom - 1, r.right, r.bottom, bc);
}

void FlatButton::Paint(Draw &w)
{
	Size sz = GetSize();
	Size isz = img.GetSize();

	if(HasMouse())
		w.DrawRect(sz, hl);
	else
		w.DrawRect(sz, bg);

	if(drawedge)
		::DrawFrame(w, sz, fg);
	else
	{
		if(!left)
			DrawFrame(w, sz, bg, fg, fg, fg);
		else
			DrawFrame(w, sz, fg, fg, bg, fg);
	}

	if(!IsPush())
		w.DrawImage((sz.cx - isz.cx) / 2, (sz.cy - isz.cy) / 2, img);
	else
	{
		if(!left)
			w.DrawImage((sz.cx - isz.cx) / 2 + 1, (sz.cy - isz.cy) / 2 + 1, img);
		else
			w.DrawImage((sz.cx - isz.cx) / 2 - 1, (sz.cy - isz.cy) / 2 + 1, img);
	}
}