#include "SkylarkGuiExample.h"
#include "Config.h"

Callback1<int> SkylarkGuiExample::WhenSignal;

SkylarkGuiExample::SkylarkGuiExample()
{
	CtrlLayout(*this, "Skylark + GUI example");
	SomeVariable.SetData("Some text");
	WhenSignal					= THISBACK(Break);
	ExitButton.WhenAction		= THISBACK1(Break, IDCANCEL);
	OpenWebBrowser.WhenAction	<< []() { LaunchWebBrowser(Format("http://127.0.0.1:%d", (int)Config::WebInterfacePort)); };
	SendToWeb.WhenAction		<< [this]() { web.SharedVariable.Set(~SomeVariable);};
	web.MessageFromWeb			<< [this](String data) { GuiLock __; MessageFromWeb.SetData(data); BeepInformation(); };
}


void SkylarkGuiExample::SignalHandler(int signal)
{
	RLOG("SIGNAL RECEIVED");
	WhenSignal((int)IDCANCEL);
}


void SkylarkGuiExample::Run()
{
#ifdef PLATFORM_POSIX
	signal(SIGTERM, SignalHandler);
#endif
	TopWindow::Run();
	web.StopServer();
	Thread::ShutdownThreads();
    RLOG("Application stop");
}


GUI_APP_MAIN
{
	StdLogSetup(LOG_FILE | LOG_TIMESTAMP | LOG_ROTATE(2));
	SetLanguage( SetLNGCharset( GetSystemLNG(), CHARSET_UTF8 ) );
	SetIniFile(ConfigFile());
	if (!FileExists(GetIniFile())) SaveFile(GetIniFile(), CurrentIniFileContent(true));
	SkylarkGuiExample().Run();
}
