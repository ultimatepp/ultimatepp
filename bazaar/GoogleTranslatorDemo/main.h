#ifndef _GoogleTranslatorDemo_main_h_
#define _GoogleTranslatorDemo_main_h_

#include <CtrlLib/CtrlLib.h>

#include <GoogleTranslator/GoogleTranslator.h>
#include "NeedAdd.h"

using namespace Upp;

#define IMAGECLASS GoogleTranslatorDemoImg
#define IMAGEFILE <GoogleTranslatorDemo/GoogleTranslatorDemo.iml>
#include <Draw/iml_header.h>

#define LAYOUTFILE <GoogleTranslatorDemo/GoogleTranslatorDemo.lay>
#include <CtrlCore/lay.h>

class GoogleTranslatorDemo : public WithMainWindow<TopWindowSpecific> {
public:
	typedef GoogleTranslatorDemo CLASSNAME;
	GoogleTranslatorDemo();
	~GoogleTranslatorDemo();
	void TranslateText(bool show_progress=true);
	void CorrectTranslatedText();
	void Exit();
	void Options();
	void About();
	const VectorMap<String, String> GetLanguages;
	
	Splitter verticalsplitter;
	WithInputTextForm<StaticRect> inputwindow;
	WithGoogleTranslatorDemoLayout<StaticRect> outputwindow;
	VectorMap<String, String> languageslist;
	Progress progress;
	
	String sourcetext;
	String result;
	String languagefrom;
	String languageto;
	String resultuser;
	
	volatile Atomic translating, test_test;

	enum {
		TIMEID_PROGRESS = Ctrl::TIMEID_COUNT
		, TIMEID_COUNT
		, TIMEID_KEYBOARD
	};
	
	void HttpThread(Gate2<int, int> _progress = false);
	void UpdateProgress();
	bool CheckCancel(int, int);
	void swap();
	void Serialize(Stream &s);
	void OLD_GlobalShortCutHandle();
	void GlobalShortCutHandle();
	void SetLanguageFromToPos(String lang_from, String lang_to, bool& need_translate);
	void SetLanguageFromToPos();
	void SetLanguagePos(DropList& ctrl, String& lang_key, String& def_key);
	void TranslateTextInBaloon();
	GoogleTranslator googletranslator;
	//callbacks
	void useProxyOnChange();
	void useProxyAuthOnChange();
private:
	enum { LAUNCH };
	void refreshData();
	void mainMenu(Bar& CurrentBar);	
		void menuFile(Bar& CurrentBar);
		void menuTools(Bar& CurrentBar);
		void menuHelp(Bar& CurrentBar);
	void hide();
	void launch();
	void perform(int NewState);
	void trayMenu(Bar& CurrentBar);
		void trayBallonLeftClick();
			void MenuSetLanguageFrom(Bar& currentBar, bool& need_translate);
			void MenuSetLanguageTo(Bar& currentBar, VectorMap<String, String> &cur_parameter);
			void MenuTextManagement(Bar& currentBar, bool need_translate);
	bool trayiconvisible;
	bool trayiconAlwaysShowTrayIcon;
	bool hideInTrayIconOnMinimize;
	bool hideInTrayIconOnClose;
	bool doNotFormatText;
	bool isHidden;
	void onMinimize();
	void onClose();
	MenuBar menu;
	int currentLanguage;
	ArrayMap<int, WString> suportedLanguagies;
	TrayIcon trayicon;
	bool useProxy;
	String proxyHTTPAddress;
	int proxyHTTPPort;
	bool useProxyAuth;
	String proxyHTTPUsername;
	String proxyHTTPPassword;
	TimeStop time_stop_shortcut;
	int time_count_shortcut1, time_count_shortcut2, time_count_shortcut3, time_count_shortcut4;
	int shortcut_stage;
	bool AskWhenCloseApplication;
	int TranslateHotKeyId;
	TabDlg option_dlg;
		bool option_dlg_initialized;
		WithOptionsConnectionLayout<ParentCtrl> tab_options_connection;
		WithOptionsGeneralLayout<ParentCtrl> tab_options_general;
};

#endif
