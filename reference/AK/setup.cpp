#include "AK.h"

#define KEYGROUPNAME "Setup"
#define KEYFILE      <AK/Setup.key>
#include             <CtrlLib/key_source.h>

void App::SetLang(int lang)
{
	SetLanguage(lang);
	menubar.Set([=] (Bar& bar) { MainMenu(bar); });
}

void App::SetupMenu(Bar& menu)
{
	menu.Add(AK_ENGLISH, [=] { SetLang(LNGC_('E','N','U','S', CHARSET_UTF8)); })
	    .Radio(GetCurrentLanguage() == LNGC_('E','N','U','S', CHARSET_UTF8));
	menu.Add(AK_CZECH, [=] { SetLang(LNGC_('C','S','C','Z', CHARSET_UTF8)); })
	    .Radio(GetCurrentLanguage() == LNGC_('C','S','C','Z', CHARSET_UTF8));
	menu.Separator();
	menu.Add(AK_KEYS, [=] { EditKeys(); });
}
