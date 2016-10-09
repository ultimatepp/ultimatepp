#include "AK.h"

#define KEYGROUPNAME "AK"
#define KEYNAMESPACE AKKeys
#define KEYFILE      <AK/AK.key>
#include             <CtrlLib/key_source.h>

using namespace AKKeys;

void App::Fn(String text)
{
	PromptOK(text);
}

void App::AkMenu(Bar& menu)
{
	menu.Add(AK_FN1, [=] { Fn("1"); });
	menu.Add(AK_FN2, [=] { Fn("2"); });
	menu.Add(AK_EXIT, [=] { Close(); });
}

void App::MainMenu(Bar& menu)
{
	menu.Sub("AK", [=](Bar& bar) { AkMenu(bar); });
	menu.Sub(t_("Setup"), [=](Bar& bar) { SetupMenu(bar); });
}

App::App()
{
	SetLang(LNGC_('E','N','U','S', CHARSET_UTF8));
	AddFrame(menubar);
	menubar.Set(THISFN(MainMenu));
	Sizeable();
}

GUI_APP_MAIN
{
	String keyfile = ConfigFile("keys.key");
	RestoreKeys(LoadFile(keyfile));
	App().Run();
	SaveFile(keyfile, StoreKeys());
}
