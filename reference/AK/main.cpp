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
	menu.Add(AK_FN1, THISBACK1(Fn, "1"));
	menu.Add(AK_FN2, THISBACK1(Fn, "2"));
	menu.Add(AK_EXIT, THISBACK(Close));
}

void App::MainMenu(Bar& menu)
{
	menu.Add("AK", THISBACK(AkMenu));
	menu.Add(t_("Setup"), THISBACK(SetupMenu));
}

App::App()
{
	SetLang(LNGC_('E','N','U','S', CHARSET_UTF8));
	AddFrame(menubar);
	menubar.Set(THISBACK(MainMenu));
	Sizeable();
}

GUI_APP_MAIN
{
	String keyfile = ConfigFile("keys.key");
	RestoreKeys(LoadFile(keyfile));
	App().Run();
	SaveFile(keyfile, StoreKeys());
}
