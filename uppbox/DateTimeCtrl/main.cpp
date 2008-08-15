#ifdef flagMAIN

#include <CtrlLib/CtrlLib.h>
#include "DateTimeCtrl.h"

#define LAYOUTFILE "main.lay"
#include <CtrlCore/lay.h>


struct App : public WithAppLayout<TopWindow>
{
	typedef App CLASSNAME;

	App()
	{
		CtrlLayout(*this, "DateTimeCtrl test");
		datesmall <<= GetSysDate();
		datebig <<= GetSysDate();
		datebig.SetSize(400,300);
		lang.Add("English");
		lang.Add("Polish");
		lang.SetIndex(0);
		lang <<= THISBACK(ChangeLang);

		datesmall <<= THISBACK(OnSelect);
		repaint = 0;
		selall = 0;

		exit <<= THISBACK(Close);

		repaint <<= THISBACK(Repaint);
		selall <<= THISBACK(SelectAll);
    }

	void ChangeLang()
	{
		int i = lang.GetIndex();
		if(i == 1)
			SetLanguage(LNGC_('P','L','P','L', CHARSET_WIN1250));
		else
			SetLanguage(LNGC_('E','N','E','N', CHARSET_UTF8));

	}

	void Repaint()
	{
		Ctrl::ShowRepaint = repaint * 20;
	}

	void SelectAll()
	{
		datesmall.SelectAll(selall);
		datebig.SelectAll(selall);
	}

	void OnSelect()
	{
		PromptOK("Date was selected");
	}
};


GUI_APP_MAIN
{
	App().Run();
}

#endif