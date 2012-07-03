/***********************************************************************************************
 * StarIndicator
 *
 * Displays stars rating with number of votes
 *
 * Colors are used to show the confidence we can give to the grade, according to the number of 
 * votes :
 * - Light blue if undefined,
 * - Red if less than m_nVotesLow,
 * - Yellow if more than m_nVotesLow and less than m_nVotesHigh,
 * - Light Green if more than m_nVotesHigh
 *
 * Autors : jibe
 * Last modified : November 28, 2011
 *
 * 2do : Make better graphics
 * Known bugs : Invalid memory access when choosing SetFontColor with .lay designer (only under 
 *              linux ?)
 **********************************************************************************************/

#include <CtrlLib/CtrlLib.h>
#include "StarIndicator.h"

NAMESPACE_UPP

#define IMAGECLASS StarsImg
#define IMAGEFILE <Controls4U/StarIndicator.iml>
#include <Draw/iml.h>

StarIndicator::StarIndicator()
:ProgressIndicator()
, m_bAlwaysShowValue(false)
, m_bVotes(true)
, m_FontColor(Black)
, m_nVotes(0)
, m_nVotesHigh(50)
, m_nVotesLow(5)
/***********************************************************************************************
 * Constructor
 * public
 **********************************************************************************************/
{
	m_bMouseInside=false;
}

StarIndicator::~StarIndicator() 
/***********************************************************************************************
 * Destructor
 * virtual - public
 **********************************************************************************************/
{
}

void StarIndicator::Layout() {
/***********************************************************************************************
 * virtual - public
 **********************************************************************************************/
	Rect r=GetRect();
	Size size=r.GetSize();
	int x=size.cx;
	int y=int(ceil(double(x/5)));
	x=5*y;
	r.SetSize(x, y);
	Set(actual, total);
}

void StarIndicator::Paint(Draw& w) {
/***********************************************************************************************
 * Paints the control
 * virtual - public
 **********************************************************************************************/
	Font fnt = SansSerif();
	Size sz = GetSize();
	Size msz = GetMsz();
	Rect r;
	String pt;
	int p0 = 0;
	int p = pxp;
	fnt.Bold();
	if(total <= 0) {
		int l = max(msz.cx, msz.cy) & ~7;
		p0 = pxp - l / 4;
		p = min(p - p0, max(msz.cx, msz.cy) - p0);
		if(style->bound && p0 < 0) {
			p += p0;
			p0 = 0;
		}
	}
	ChPaintEdge(w, sz, EditFieldEdge());
	Rect mg = ChMargins(EditFieldEdge());
	sz -= Size(mg.left + mg.right, mg.top + mg.bottom);
	Rect r1, r2, r3;
	r = r1 = r2 = r3 = RectC(mg.left, mg.top, sz.cx, sz.cy);
	w.Clip(r1);
	if(sz.cx > sz.cy) {
		r1.right = r2.left = min(p, sz.cx) + mg.left + p0;
		r3.right = mg.left + p0;
		r3.bottom = r1.bottom;
	}
	else {
		r2.bottom = r1.top = sz.cy - min(p, sz.cy) + mg.left + p0;
		r3.bottom = mg.top + p0;
		r3.right = r1.right;
	}
	if (m_nVotes) {
	w.DrawRect(r1, Nvl(color, SColorHighlight()));
	w.DrawRect(r2, SColorPaper);
	w.DrawRect(r3, SColorPaper);
		w.DrawImage(r,StarsImg::Get(0),Nvl(color, SColorHighlight()));
		w.DrawImage(r,StarsImg::Get(1),SColorPaper);
	}
	if (percent) {
		pt = Format("%d %%", 100 * actual / max(total, 1));
	}
	else {
		pt = Format("%3.1f", 5. * actual / max(total, 1));
		if (m_bTotal) pt += " / 5";
	}
	if (m_bVotes && m_nVotes > 0) {
		pt = Format("%s - %d Votes", pt, m_nVotes);
	}
	if (m_bMouseInside || m_bAlwaysShowValue) {
		Size psz = GetTextSize(pt, fnt());
		sz = GetSize();
		while (psz.cy < sz.cy) {
			fnt.Height(fnt.GetHeight()+1);
			psz = GetTextSize(pt, fnt());
		}
		while (psz.cx > sz.cx - 8 || psz.cy > sz.cy -4) {
			fnt.Height(fnt.GetHeight()-1);
			psz = GetTextSize(pt, fnt());
		}
		int px = (sz.cx - psz.cx) / 2;
		int py = (sz.cy - psz.cy) / 2;
		w.DrawText(px, py, pt, fnt(), m_FontColor);
	}
	w.End();
}

void StarIndicator::MouseEnter(Point p, dword keyflags)
/***********************************************************************************************
 * Mouse got inside the control
 * virtual - public
 **********************************************************************************************/
{ 
	m_bMouseInside = true;
}

void StarIndicator::MouseLeave()
/***********************************************************************************************
 * Mouse got outside the control
 * virtual - public
 **********************************************************************************************/
{ 
	m_bMouseInside = false;
	Refresh();
}

void StarIndicator::Set(double n)
/***********************************************************************************************
 * Sets the value
 * virtual - public
 **********************************************************************************************/
{ 
	Set(fceil(n*20),100);
}

StarIndicator&	StarIndicator::SetVotes(int n)
/***********************************************************************************************
 * Sets the number of votes and the associated color
 * virtual - public
 **********************************************************************************************/
{ 
	m_nVotes=n;
	if (n<0) SetColor(LtCyan);
	else if (n < m_nVotesLow) SetColor(Color(255,128,128));
	else if (n < m_nVotesHigh) SetColor(Yellow);
	else SetColor(LtGreen);
	Refresh();
	return *this;
}

END_UPP_NAMESPACE
