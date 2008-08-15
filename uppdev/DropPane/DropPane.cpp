#include "DropPane.h"

DropPane::DropPane()
{
	Add(btnDrop);
	btnDrop.WhenAction = THISBACK(Togle);
	btnDrop.Set(true);
	btnDrop.Add(lblText);
	lblText.SetAlign(ALIGN_CENTER);
	lastHeight = 20;
}

DropPane::~DropPane()
{
	
}

void DropPane::SetRect(int x, int y, int cx, int cy)
{
	Ctrl::SetRect(x, y, cx, cy);
	if (btnDrop.Get())
		lastHeight = cy;
	btnDrop.SetRect(3, 0, cx - 7, 20);
	lblText.SetRect(0, 0, btnDrop.GetRect().Width(), btnDrop.GetRect().Height());
}

void DropPane::Paint(Draw& w)
{
	Size sz = GetSize();
	btnDrop.SetRect(3, 0, sz.cx - 7, 20);
	lblText.SetRect(0, 0, btnDrop.GetRect().Width(), btnDrop.GetRect().Height());
	if (btnDrop.Get())
		lastHeight = sz.cy;
	if(!IsTransparent())
		w.DrawRect(sz, /*SColorInfoText()*/SColorFace);
	
	int d = 5;
	if (btnDrop.Get() == false)
		sz.cy = 16;
		
	w.Begin();
	
	w.DrawRect(1, d, sz.cx - 2, 1, SColorShadow);
	w.DrawRect(1, d + 1, sz.cx - 2, 1, SColorLight);
	
	w.DrawRect(0, d, 1, sz.cy - d - 1, SColorShadow);
	w.DrawRect(1, d + 1, 1, sz.cy - d - 2, SColorLight);

	w.DrawRect(sz.cx - 2, d, 1, sz.cy - d, SColorShadow);
	w.DrawRect(sz.cx - 1, d, 1, sz.cy - d, SColorLight);

	w.DrawRect(1, sz.cy - 2, sz.cx - 2, 1, SColorShadow);
	w.DrawRect(1, sz.cy - 1, sz.cx - 2, 1, SColorLight);
	
	w.End();
}

void DropPane::SetLabel(const char* label)
{
	lblText.SetLabel(label);
}

void DropPane::Togle()
{
	Rect r = GetRect();
	if (btnDrop.Get())
		SetRect(r.left, r.top, r.Width(), lastHeight);
	else
		SetRect(r.left, r.top, r.Width(), 20);
	WhenAction();
}