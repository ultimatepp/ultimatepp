#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

#define LLOG(x) // DLOG(x)

NAMESPACE_UPP

void Ctrl::GuiPlatformConstruct()
{
}

void Ctrl::GuiPlatformDestruct()
{
}

void Ctrl::GuiPlatformGetTopRect(Rect& r) const
{
}

bool Ctrl::GuiPlatformRefreshFrameSpecial(const Rect& r)
{
	return false;
}

bool Ctrl::GuiPlatformSetFullRefreshSpecial()
{
	return false;
}

String GuiPlatformGetKeyDesc(dword key)
{
	static Tuple2<dword, const char *> nkey[] = {
		{ K_DELTA|GDKEY(grave), "[`]" },
		{ K_DELTA|GDKEY(minus), "[-]" },
		{ K_DELTA|GDKEY(equal), "[=]" },
		{ K_DELTA|GDKEY(backslash), "[\\]" },
		{ K_DELTA|GDKEY(bracketleft), "[[]" },
		{ K_DELTA|GDKEY(bracketright), "[]]" },
		{ K_DELTA|GDKEY(semicolon), "[;]" },
		{ K_DELTA|GDKEY(apostrophe), "[']" },
		{ K_DELTA|GDKEY(comma), "[,]" },
		{ K_DELTA|GDKEY(period), "[.]" },
		{ K_DELTA|GDKEY(underscore), "[/]" },
		{ 0, NULL }
	};
	const Tuple2<dword, const char *> *x = FindTuple(nkey, __countof(nkey), key);
	return x ? String(x->b) : String();
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
	if(q->top) {
		Point p = GetMousePos();
		gtk_window_begin_resize_drag(q->gtk(), GDK_WINDOW_EDGE_SOUTH_EAST,
		                             1, p.x, p.y, Ctrl::CurrentTime);
	}
}

Color GuiPlatformGetScreenPixel(int x, int y)
{
	return Black;
}

void GuiPlatformAfterMenuPopUp()
{
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

String Ctrl::Name(Ctrl *ctrl)
{
	return Upp::Name(ctrl);
}

void Ctrl::InstallPanicBox()
{
}

END_UPP_NAMESPACE

#endif
