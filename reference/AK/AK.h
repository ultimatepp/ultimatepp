#ifndef _AK_AK_h
#define _AK_AK_h

#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define KEYGROUPNAME "AK"
#define KEYNAMESPACE AKKeys
#define KEYFILE      <AK/AK.key>
#include             <CtrlLib/key_header.h>

#define KEYGROUPNAME "Setup"
#define KEYFILE      <AK/Setup.key>
#include             <CtrlLib/key_header.h>

class App : public TopWindow {
	MenuBar menubar;

	void Fn(String text);
	void AkMenu(Bar& menu);
	void SetupMenu(Bar& menu);
	void MainMenu(Bar& menu);
	void SetBar();
	void SetLang(int lang);

public:
	typedef App CLASSNAME;
	App();
};

#endif
