#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK
#ifdef GDK_WINDOWING_X11

#define Time        XTime
#define Font        XFont
#define Picture     XPicture

#ifndef PLATFORM_OPENBSD // avoid warning
#undef CurrentTime
#define CurrentTime XCurrentTime
#endif

#include <gdk/gdkx.h>

#undef Picture
#undef Time
#undef Font

#ifndef PLATFORM_OPENBSD // avoid warning
#undef CurrentTime
#endif

namespace Upp {

_XDisplay *Xdisplay()
{
	return GDK_DISPLAY_XDISPLAY(gdk_display_get_default());
}

Window Xroot()
{
	return GDK_WINDOW_XID(gdk_screen_get_root_window(gdk_screen_get_default()));
}

String GetProperty(Window w, Atom property, Atom rtype)
{
	GuiLock __;
	String result;
	int format;
	unsigned long nitems, after = 1;
	long offset = 0;
	Atom type = None;
	unsigned char *data;
	long rsize = minmax((long)(XMaxRequestSize(Xdisplay()) - 100), (long)256, (long)65536);
	while(after > 0) {
		if(XGetWindowProperty(Xdisplay(), w, property, offset, rsize, 0,
		                      rtype, &type, &format, &nitems, &after, &data) != Success)
			break;
		if(type == None)
		    break;
		if(data) {
			int len = format == 32 ? sizeof(unsigned long) * nitems : nitems * (format >> 3);
			result.Cat(data, len);
			XFree((char *)data);
			offset += nitems / (32 / format);
		}
		else
			break;
	}
	result.Shrink();
	XFlush(Xdisplay());
	return result;
}

Atom XAtomRaw(const char *name)
{
	return XInternAtom(Xdisplay(), name, 0);
}

Atom XAtom(const char *name)
{
	GuiLock __;
	Atom x;
	INTERLOCKED {
		static VectorMap<String, int> atoms;
		int q = atoms.Get(name, Null);
		if(IsNull(q)) {
			q = XAtomRaw(name);
			atoms.Add(name, q);
		}
		x = q;
	}
	return x;
}

Vector<int> GetPropertyInts(GdkWindow *w, const char *property)
{
	if(Ctrl::IsWayland())
		return {}; // Not supported on Wayland...
	
	GuiLock __;
	Vector<int> result;
	String p = GetProperty(GDK_WINDOW_XID(w), XAtom(property), AnyPropertyType);
	const long int *ptr = (const long int *)~p;
	const long int *lim = ptr + p.GetLength() / sizeof(long int);
	result.Reserve(p.GetLength() / sizeof(long int));
	while(ptr < lim)
		result.Add(*ptr++);
	return result;
}

dword X11mods(dword key)
{
	dword mod = 0;
	if(key & K_ALT)
		mod |= Mod1Mask;
	if(key & K_SHIFT)
		mod |= ShiftMask;
	if(key & K_CTRL)
		mod |= ControlMask;
	return mod;
}

int Ctrl::RegisterSystemHotKey(dword key, Function<void ()> cb)
{
	if(IsWayland())
		return -1; // Not supported on Wayland...
	
	GuiLock __;
	ASSERT(key >= K_DELTA);
	gdk_x11_display_error_trap_push(gdk_display_get_default());
	KeyCode k = XKeysymToKeycode(Xdisplay(), key & 0xffff);
	dword mod = X11mods(key);
	bool r = false;
	for(dword nlock = 0; nlock < 2; nlock++)
		for(dword clock = 0; clock < 2; clock++)
			for(dword slock = 0; slock < 2; slock++)
				r = XGrabKey(Xdisplay(), k,
				             mod | (nlock * Mod2Mask) | (clock * LockMask) | (slock * Mod3Mask),
				             Xroot(), true, GrabModeAsync, GrabModeAsync) || r;
	gdk_x11_display_error_trap_pop_ignored(gdk_display_get_default());
	if(!r) return -1;
	int q = hotkey.GetCount();
	for(int i = 0; i < hotkey.GetCount(); i++)
		if(!hotkey[i]) {
			q = i;
			break;
		}
	hotkey.At(q) = Event<> () << cb;
	keyhot.At(q) = k;
	modhot.At(q) = mod;
	return q;
}

void Ctrl::UnregisterSystemHotKey(int id)
{
	if(IsWayland())
		return; // Not supported on Wayland...
	
	GuiLock __;
	if(id >= 0 && id < hotkey.GetCount() && hotkey[id]) {
		gdk_x11_display_error_trap_push(gdk_display_get_default());
		for(dword nlock = 0; nlock < 2; nlock++)
			for(dword clock = 0; clock < 2; clock++)
				for(dword slock = 0; slock < 2; slock++)
					XUngrabKey(Xdisplay(), keyhot[id],
					           modhot[id] | (nlock * Mod2Mask) | (clock * LockMask) | (slock * Mod3Mask),
					           Xroot());
		gdk_x11_display_error_trap_pop_ignored(gdk_display_get_default());
		hotkey[id].Clear();
	}
}

GdkFilterReturn Ctrl::RootKeyFilter(GdkXEvent *xevent, GdkEvent *Xevent, gpointer data)
{
	if(IsWayland())
		return GDK_FILTER_CONTINUE; // Not supported on Wayland...
	
	XEvent *event = (XEvent *)xevent;
	if(event->type == KeyPress)
		for(int i = 0; i < hotkey.GetCount(); i++)
			if(hotkey[i] && keyhot[i] == event->xkey.keycode &&
			   modhot[i] == (event->xkey.state & (Mod1Mask|ShiftMask|ControlMask))) {
				hotkey[i]();
				return GDK_FILTER_REMOVE;
			}
	return GDK_FILTER_CONTINUE;
}

}

#endif
#endif
