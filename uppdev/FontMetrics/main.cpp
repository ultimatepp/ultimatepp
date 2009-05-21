#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct CharMetrics {
	int16 width;
	int16 lspc;
	int16 rspc;
};

struct FontMetrics {
		int          refcount;
		Font         font;
		int          angle;
		int          device;
	#ifdef PLATFORM_WIN32
		HFONT        hfont;
	#endif
	#ifdef PLATFORM_XFT
		XftFont     *xftfont;
		XftFont     *xftfont0;
	#endif
		int          ascent;
		int          descent;
		int          external;
		int          internal;
		int          height;
		int          lineheight;
		int          overhang;
		Size         offset;
		int          avewidth;
		int          maxwidth;
		int          firstchar;
		int          charcount;
		int          default_char;

		CharMetrics *base[64];

		Mutex         xmutex;
		Vector<Kinfo> kinfo;
		VectorMap<dword, CharMetrics> xx;

		bool         fixedpitch;
		bool         scaleable;
		int          spacebefore;
		int          spaceafter;
	#ifdef PLATFORM_X11
		int          underline_position;
		int          underline_thickness;
		double       sina;
		double       cosa;
		bool         twobyte;
		String       filename;
	#endif
};

FontMetrics GetFontMetrics(Font fnt);
CharMetrics GetCharMetrics(Font fnt, int chr);

GUI_APP_MAIN
{
}

