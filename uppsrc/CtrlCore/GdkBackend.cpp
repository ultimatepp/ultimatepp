#include "CtrlCore.h"

#ifdef GUI_GTK

#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#endif
#ifdef GDK_WINDOWING_WAYLAND
#include <gdk/gdkwayland.h>
#endif

namespace Upp {

GdkBackend GetGdkBackend() {
	static auto backend = GdkBackend::UNKNOWN;
	if (backend != GdkBackend::UNKNOWN) {
		return backend;
	}
	
	auto* display = gdk_display_get_default();
#ifdef GDK_WINDOWING_X11
	if (GDK_IS_X11_DISPLAY(display)) {
		backend = GdkBackend::X11;
		return backend;
	}
#endif
#ifdef GDK_WINDOWING_WAYLAND
	if (GDK_IS_WAYLAND_DISPLAY(display)) {
		backend = GdkBackend::WAYLAND;
		return backend;
	}
#endif
	return backend;
}

String ToString(GdkBackend b) {
	switch (b) {
		case GdkBackend::X11: return "X11";
		case GdkBackend::WAYLAND: return "Wayland";
		default: return "Unknown";
	}
}

}

#endif
