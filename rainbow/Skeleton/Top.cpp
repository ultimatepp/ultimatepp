#include <CtrlCore/CtrlCore.h>

#ifdef GUI_SKELETON

NAMESPACE_UPP

#define LLOG(x)  // LOG(x)

void    TopWindow::SyncSizeHints() {}

void TopWindow::SyncTitle()
{
	GuiLock __;
}

void TopWindow::SyncCaption()
{
	GuiLock __;
}

void TopWindow::Open(Ctrl *owner)
{
	GuiLock __;
}

void TopWindow::Open()
{
}

void TopWindow::OpenMain()
{
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
