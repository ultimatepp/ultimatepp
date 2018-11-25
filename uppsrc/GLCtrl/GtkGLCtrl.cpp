#include "GLCtrl.h"

#if 0

#ifdef GUI_GTK

#include <gtk/gtkgl.h>
#include <gdk/gdkgl.h>
#include <gdk/gdkglconfig.h>

namespace Upp {

void InitializeGlew() {}

void GLCtrl::Paint(Draw& w)
{
	static GdkGLConfig  *s_GdkGLConfig;
	static GdkGLContext *s_GdkGLContext;

	Size sz = GetSize();
	
	Ctrl *top = GetTopCtrl();
	if(!top)
		return;

	GtkWindow *gtk = top->gtk();
	GdkWindow *win = top->gdk();

	if(sz.cx <= 0 || sz.cy <= 0 || !win || !gtk)
		return;

	ONCELOCK {
		int samples = numberOfSamples;
		do {
			Vector<int> attr;
			attr << GLX_RGBA << GLX_DEPTH_SIZE << depthSize
			     << GLX_STENCIL_SIZE << stencilSize;
	//		if(doubleBuffering)
	//			attr << GLX_DOUBLEBUFFER;
			if(samples > 1)
				attr << GLX_SAMPLE_BUFFERS_ARB << 1 << GLX_SAMPLES_ARB << samples;

			attr << 0;
			s_GdkGLConfig = gdk_gl_config_new(attr);
			samples >>= 1;
		}
		while(s_GdkGLConfig == NULL && samples > 0);
	}

	GdkPixmap *pixmap = gdk_pixmap_new(win, sz.cx, sz.cy, -1);
	if(pixmap) {
		GdkGLDrawable *gldrawable = GDK_GL_DRAWABLE(gdk_pixmap_set_gl_capability(pixmap, s_GdkGLConfig, NULL));
		if(gldrawable) {
			ONCELOCK {
				s_GdkGLContext = gdk_gl_context_new(gldrawable, NULL, TRUE, GDK_GL_RGBA_TYPE);
			}
			if(s_GdkGLContext) {
				if(gdk_gl_drawable_gl_begin(gldrawable, s_GdkGLContext)) {
					ONCELOCK {
						glewInit();
					}
					DoGLPaint();
					glFlush();
					gdk_gl_drawable_gl_end(gldrawable);

					Rect r = GetScreenView() - GetTopCtrl()->GetScreenView().TopLeft();
					GtkWidget *widget = GTK_WIDGET(gtk);
					gdk_draw_drawable(win, widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
					                  pixmap, 0, 0, r.left, r.top, r.GetWidth(), r.GetHeight());
				}
			}
			g_object_unref(G_OBJECT(gldrawable));
		}
		g_object_unref(G_OBJECT(pixmap));
	}
}

}

#endif

#endif