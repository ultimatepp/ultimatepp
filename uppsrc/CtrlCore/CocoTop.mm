#include <CtrlCore/CtrlCore.h>

#ifdef GUI_COCOA

#include <CtrlCore/CocoMM.h>

namespace Upp {

#define LLOG(x)  // LOG(x)

TopWindow& TopWindow::FullScreen(bool b)
{
	return *this;
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

void TopWindow::SetBadgeLabel(const String& label)
{
	NSString* nlabel = [NSString stringWithUTF8String:~label];
	[[[NSApplication sharedApplication] dockTile] setBadgeLabel:nlabel];
}

}

#endif
