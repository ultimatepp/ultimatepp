#include "chrset.h"



chrset::chrset()
{
	CtrlLayout(*this, "Window title");
	lbl1 = "řeřicha česky";
	lbl2 = "Революция";
	lbl3 = "∑€∂¼œđ";
}

GUI_APP_MAIN
{
	DUMP((int)GetDefaultCharset());
	SetDefaultCharset(CHARSET_UTF8);
	chrset().Run();
}
