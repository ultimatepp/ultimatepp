String DumpLanguage(int language);
String DumpColor(Color color);
String DumpFont(Font font);
String DumpFontInfo(FontInfo fi);
String DumpAlign(int align);
#ifdef PLATFORM_X11
String DumpPixmap(Pixmap pixmap);
String DumpXFont(XFont font);
String DumpXFontStruct(XFontStruct *fs);
String DumpGC(GC gc);
String DumpGCValues(const XGCValues& gc_values, int mask = -1);
String DumpEvent(XEvent *event);
#endif//PLATFORM_X11

class SimpleTiming
{
public:
	SimpleTiming(const char *name) : name(name)
	{
		last_ticks = start_ticks = GetTickCount();
		RLOG(name << " (open) & " << start_ticks);
	}

	void Show(const char *part)
	{
		int ticks = GetTickCount();
		RLOG(name << " in " << (ticks - start_ticks) <<
		", " << part << " in " << (ticks - last_ticks));
		last_ticks = ticks;
	}

	~SimpleTiming()
	{
		int ticks = GetTickCount();
		String out;
		RLOG(name << " done in " << (ticks - start_ticks)
		<< ", last part in " << (ticks - last_ticks));
	}

private:
	String name;
	int    start_ticks;
	int    last_ticks;
};

int GdiGetFreeSpace();
