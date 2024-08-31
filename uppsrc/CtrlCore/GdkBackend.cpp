#include "CtrlCore.h"

#ifdef GUI_GTK

#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#endif
#ifdef GDK_WINDOWING_WAYLAND
#include <gdk/gdkwayland.h>
#endif

namespace Upp {
namespace GdkBackend {

Type Get()
{
	static auto backend = Type::UNKNOWN;
	if (backend != Type::UNKNOWN) {
		return backend;
	}
	
	auto* display = gdk_display_get_default();
#ifdef GDK_WINDOWING_X11
	if (GDK_IS_X11_DISPLAY(display)) {
		backend = Type::X11;
		return backend;
	}
#endif
#ifdef GDK_WINDOWING_WAYLAND
	if (GDK_IS_WAYLAND_DISPLAY(display)) {
		backend = Type::WAYLAND;
		return backend;
	}
#endif
	return backend;
}

bool IsWayland()
{
	return Get() == Type::WAYLAND;
}

bool IsX11()
{
	return Get() == Type::X11;
}

}

String ToString(GdkBackend::Type t)
{
	switch (t) {
		case GdkBackend::Type::X11: return "X11";
		case GdkBackend::Type::WAYLAND: return "Wayland";
		default: return "Unknown";
	}
}

}

#endif
