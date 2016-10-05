#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct ConvertBin : Convert {
	virtual Value  Format(const Value& q) const
	{
		return FormatIntBase(q, 2, 0, 0, 0);
	}

	virtual Value Scan(const Value& text) const
	{
		String txt = text;
		int r = 0;
		for(const char *s = txt; *s; s++)
			if(*s == '1')
				r = (r << 1) | 1;
			else
			if(*s == '0')
				r <<= 1;
		return r;
	}

	virtual int   Filter(int chr) const
	{
		return chr == '0' || chr == '1' ? chr : 0;
	}
};

struct App : TopWindow {
	EditField bin;
	EditInt   dec;

	App()
	{
		SetRect(0, 0, 200, 50);
		bin.SetConvert(Single<ConvertBin>());
		Add(bin.HSizePos().TopPos(0, Ctrl::STDSIZE));
		Add(dec.HSizePos().BottomPos(0, Ctrl::STDSIZE));
		bin << [=] { dec <<= ~bin; };
		dec << [=] { bin <<= ~dec; };
	}
};

GUI_APP_MAIN
{
	App().Run();
}
