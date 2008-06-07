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

	void BinChanged()  { dec <<= ~bin; }
	void DecChanged()  { bin <<= ~dec; }

	typedef App CLASSNAME;

	App()
	{
		SetRect(0, 0, 200, 50);
		bin.SetConvert(Single<ConvertBin>());
		Add(bin.HSizePos().TopPos(0, Ctrl::STDSIZE));
		Add(dec.HSizePos().BottomPos(0, Ctrl::STDSIZE));
		bin <<= THISBACK(BinChanged);
		dec <<= THISBACK(DecChanged);
	}
};

GUI_APP_MAIN
{
	App().Run();
}
