#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)

void    TopWindow::SyncSizeHints() {}

void TopWindow::SyncTitle0()
{
	GuiLock __;
}

void TopWindow::SyncCaption0()
{
	GuiLock __;
}

void TopWindow::Open(Ctrl *owner)
{
	GuiLock __;
	DDUMP(GetRect());
	if(GetSize().cx == 0)
		SetRect(10, 10, 400, 400);
	Create(false);
}

void TopWindow::Open()
{
	Open(NULL); //TODO!
}

void TopWindow::OpenMain()
{
	Open(NULL);
}

void TopWindow::Minimize(bool effect)
{
	state = MINIMIZED;
}

TopWindow& TopWindow::FullScreen(bool b)
{
	return *this;
}

void TopWindow::Maximize(bool effect)
{
	state = MAXIMIZED;
}

void TopWindow::Overlap(bool effect)
{
	GuiLock __;
	state = OVERLAPPED;
}

TopWindow& TopWindow::TopMost(bool b, bool stay_top)
{
	GuiLock __;
	return *this;
}

bool TopWindow::IsTopMost() const
{
	return true;
}

void TopWindow::GuiPlatformConstruct()
{
}

void TopWindow::GuiPlatformDestruct()
{
}

void TopWindow::SerializePlacement(Stream& s, bool reminimize)
{
	GuiLock __;
}

END_UPP_NAMESPACE

#endif
