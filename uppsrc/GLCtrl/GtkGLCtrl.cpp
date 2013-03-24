#include "GLCtrl.h"

#ifdef GUI_GTK

#include <gtk/gtkgl.h>
#include <gdk/gdkgl.h>
#include <gdk/gdkglconfig.h>

NAMESPACE_UPP

GdkGLConfig *sGlconfig;

EXITBLOCK {
	if(sGlconfig)
	    g_object_unref(G_OBJECT(sGlconfig));
}

void GLCtrl::GLPaint()
{
	WhenGLPaint();
}

void GLCtrl::Paint(Draw& w)
{
	Size sz = GetSize();
	
	Ctrl *top = GetTopCtrl();
	if(!top)
		return;

	GtkWindow *gtk = top->gtk();
	GdkWindow *win = top->gdk();

	if(sz.cx <= 0 || sz.cy <= 0 || !win || !gtk)
		return;

	ONCELOCK {
		sGlconfig = gdk_gl_config_new_by_mode ((GdkGLConfigMode)(GDK_GL_MODE_RGB|GDK_GL_MODE_DEPTH|GDK_GL_MODE_SINGLE));
	}

	GdkPixmap *pixmap = gdk_pixmap_new(win, sz.cx, sz.cy, -1);
	if(pixmap) {
		GdkGLDrawable *gldrawable = GDK_GL_DRAWABLE(gdk_pixmap_set_gl_capability(pixmap, sGlconfig, NULL));
		if(gldrawable) {
			GdkGLContext *glcontext = gdk_gl_context_new(gldrawable, NULL, FALSE, GDK_GL_RGBA_TYPE);
			if(glcontext) {
				if(gdk_gl_drawable_gl_begin(gldrawable, glcontext)) {
					glViewport(0, 0, sz.cx, sz.cy);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					GLPaint();
					glFlush ();
					gdk_gl_drawable_gl_end(gldrawable);

					Rect r = GetScreenView() - GetTopCtrl()->GetScreenView().TopLeft();
					GtkWidget *widget = GTK_WIDGET(gtk);
					gdk_draw_drawable(win, widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
					                  pixmap, 0, 0, r.left, r.top, r.GetWidth(), r.GetHeight());
				}
				g_object_unref(G_OBJECT(glcontext));
			}
			g_object_unref(G_OBJECT(gldrawable));
		}
		g_object_unref(G_OBJECT(pixmap));
	}
}

END_UPP_NAMESPACE

#endif
