#include <CtrlCore/CtrlCore.h>

#ifdef GUI_GTK

namespace Upp {

GdkRect::GdkRect(const Rect& r)
{
	x = r.left;
	y = r.top;
	width = r.GetWidth();
	height = r.GetHeight();
}

String ImageClipFromPixbufUnref(GdkPixbuf *pixbuf)
{
	Image img;
	if(pixbuf) {
		int chn = gdk_pixbuf_get_n_channels(pixbuf);
		if((chn == 3 && !gdk_pixbuf_get_has_alpha(pixbuf) ||
		    chn == 4 && gdk_pixbuf_get_has_alpha(pixbuf)) &&
		   gdk_pixbuf_get_colorspace(pixbuf) == GDK_COLORSPACE_RGB &&
		   gdk_pixbuf_get_bits_per_sample(pixbuf) == 8) {
			Size sz(gdk_pixbuf_get_width (pixbuf), gdk_pixbuf_get_height(pixbuf));
			ImageBuffer m(sz);
			int stride = gdk_pixbuf_get_rowstride(pixbuf);
			byte *l = (byte *)gdk_pixbuf_get_pixels(pixbuf);
			for(int y = 0; y < sz.cy; y++) {
				RGBA *s = m[y];
				const RGBA *e = s + sz.cx;
				const byte *t = l;
				if(chn == 4)
					while(s < e) {
						s->r = *t++;
						s->g = *t++;
						s->b = *t++;
						s->a = *t++;
						s++;
					}
				else
					while(s < e) {
						s->r = *t++;
						s->g = *t++;
						s->b = *t++;
						s->a = 255;
						s++;
					}
				l += stride;
			}
			img = m;
			img = Premultiply(img);
		}
		g_object_unref(pixbuf);
	}
	return StoreAsString(img);
}

String FilesClipFromUrisFree(gchar **uris)
{
	if(uris) {
		String h;
		for(int i = 0; uris[i]; i++)
			h << uris[i] << '\n';
		g_strfreev (uris);
		return h;
	}
	return Null;
}

GdkAtom GAtom(const String& id)
{
	GuiLock __;
	static VectorMap<String, GdkAtom> m;
	int q = m.Find(id);
	return q >= 0 ? m[q] : (m.Add(id) = gdk_atom_intern(~id, FALSE));
}

int rmsecs()
{
	static int msecs0 = msecs();
	return msecs(msecs0);
}
 
}

#endif
