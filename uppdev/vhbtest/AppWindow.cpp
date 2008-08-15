#include <CtrlLib/CtrlLib.h>
#pragma hdrstop

#ifndef UPP605
using namespace Upp;
#endif

class DlgVodniToky : public TopWindow {
public:
	typedef DlgVodniToky CLASSNAME;
	DlgVodniToky();

	virtual bool       Key(dword key, int repcnt) {
		if(key == K_ENTER)
			PromptOK("Subdialog2!");
		return false;
	}
};

DlgVodniToky::DlgVodniToky()
{
	Title("Vodní toky");
	Sizeable()
		.Zoomable()
//		.MaximizeBox()
	;
//	Add(list.SizePos());
}

class AppWindow : public TopWindow {
public:
	typedef AppWindow CLASSNAME;
	AppWindow();

	void Paint(Draw& draw);
	bool Key(dword key, int);

private:
	RichText window_text;
};

bool AppWindow::Key(dword key, int)
{
	if(key == K_ENTER) {
		DlgVodniToky().Run();
		return true;
	}
	return false;
}

AppWindow::AppWindow()
{
	Title("Vodohospodáøská bilance povrchových vod");
	Sizeable().Zoomable();

	AddFrame(InsetFrame());

	String qtf;
	qtf << "[A3 [7*/ Demonstrace chyby s focusem]&&"
	"Pomocí menu otevøi okno [* Vodní toky].&"
	"Pak stiskni Enter.&"
	"Objeví se Prompt, ten "
	"odklepni a okno [* Vodní toky] opìt zavøi (myší nebo pomocí Alt+F4).&"
	"Správnì by se mìlo "
	"objevit opìt toto hlavní okno aplikace, ale to místo toho zaleze úplnì dolù "
	"pod okna ostatních aplikací."
	;

	window_text = ParseQTF(qtf);
	window_text.ApplyZoom(GetRichTextStdScreenZoom());
}

void AppWindow::Paint(Draw& draw)
{
	Size client = GetSize();
	draw.DrawRect(client, Yellow());
	window_text.Paint(draw, 2, 2, client.cx - 4);
}

GUI_APP_MAIN
{
	SetLanguage(LNGC_('C', 'S', 'C', 'Z', CHARSET_WIN1250));
	AppWindow().Run();
}
