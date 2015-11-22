#if defined(flagWINGL) || defined(flagLINUXGL)

#include "TopFrame.h" 

#define LLOG(x) // DLOG(x)

NAMESPACE_UPP

int       Ctrl::Xbuttons;
int       Ctrl::Xbuttontime;
Point     Ctrl::Xbuttonpos;

void Ctrl::GuiPlatformConstruct()
{
	cliptobounds = true;
}

void Ctrl::GuiPlatformRemove()
{
}

void Ctrl::GuiPlatformGetTopRect(Rect& r) const
{
}

bool Ctrl::GuiPlatformRefreshFrameSpecial(const Rect& r)
{
	return true;
}

bool Ctrl::GuiPlatformSetFullRefreshSpecial()
{
	return false;
}

void Ctrl::PaintCaret(SystemDraw& w)
{
}

String GuiPlatformGetKeyDesc(dword key)
{
	return Null;
}

void Ctrl::GuiPlatformSelection(PasteClip&)
{
}

void GuiPlatformAdjustDragImage(ImageBuffer&)
{
}

bool GuiPlatformHasSizeGrip()
{
	return true;
}

void GuiPlatformGripResize(TopWindow *q)
{
	q->GripResize();
}

Color GuiPlatformGetScreenPixel(int x, int y)
{
	return Black;
}

void GuiPlatformAfterMenuPopUp()
{
}

void Ctrl::InstallPanicBox()
{
}

void Ctrl::ApplyLayout()
{
	GuiLock __;
	for(Ctrl *q = GetFirstChild(); q; q = q->GetNext())
		q->ApplyLayout();
}

String Ctrl::Name() const {
	GuiLock __;
#ifdef CPU_64
	String s = String(typeid(*this).name()) + " : 0x" + FormatIntHex(this);
#else
	String s = String(typeid(*this).name()) + " : " + Format("0x%x", (int) this);
#endif
	if(IsChild())
		s << "(parent " << String(typeid(*parent).name()) << ")";
	return s;
}

END_UPP_NAMESPACE

#endif
