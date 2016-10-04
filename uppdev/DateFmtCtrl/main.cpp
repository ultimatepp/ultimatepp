#include "DateFmtCtrl.h"

void DateFmtCtrl::Print()
{
	text = AsString(~date);
}

void DateFmtCtrl::Language()
{
	SetLanguage(~lang);
	Print();
}

DateFmtCtrl::DateFmtCtrl()
{
	CtrlLayout(*this, "Window title");
	lang.Add(LNG_('E','N','U','S'), "ENUS");
	lang.Add(LNG_('C','S','C','Z'), "CSCZ");
	date <<= THISBACK(Print);
	lang <<= THISBACK(Language);
	date = GetSysDate();
	Print();
}

GUI_APP_MAIN
{
	SetDateFormat("%4:DAY %3:02d.%2:month.%1:02.2d");
	SetDateFilter("A.\r,;:\r/\r:\\|");
	DateFmtCtrl().Run();
}
