#include "CtrlCore.h"

#ifdef GDK_WINDOWING_X11

NAMESPACE_UPP

#define Time    XTime
#define Font    XFont
#define Display XDisplay
#define Picture XPicture

#include <gdk/gdkx.h>

#undef Picture
#undef Time
#undef Font
#undef Display

XDisplay *Xdisplay()
{
	return GDK_DISPLAY_XDISPLAY(gdk_display_get_default());
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
	GuiLock __; 
	Vector<int> result;
	String p = GetProperty(GDK_WINDOW_XWINDOW(w), XAtom(property), AnyPropertyType);
	const long int *ptr = (const long int *)~p;
	const long int *lim = ptr + p.GetLength() / sizeof(long int);
	result.Reserve(p.GetLength() / sizeof(long int));
	while(ptr < lim)
		result.Add(*ptr++);
	return result;
}

END_UPP_NAMESPACE

#endif