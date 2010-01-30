#include "main.h"
#include "GlobalShortCut.h"

#define IMAGECLASS GoogleTranslatorDemoImg
#define IMAGEFILE <GoogleTranslatorDemo/GoogleTranslatorDemo.iml>
#include <Draw/iml_source.h>

#define TOPICFILE <GoogleTranslatorDemo/app.tpp/all.i>
#include <Core/topic_group.h>

//static bool sGlobalGTranslateKeyHook(Ctrl *ctrl, dword key, int repcnt);

GoogleTranslatorDemo::GoogleTranslatorDemo(){
	shortcut_stage = 0;
	time_count_shortcut1 = 0;
	time_count_shortcut2 = 0;
	time_count_shortcut3 = 0;
	time_count_shortcut4 = 0;
	AtomicWrite(translating, 0);
	AtomicWrite(test_test,0);
	
	CtrlLayout(*this, "Google translator");
	
	Icon(GoogleTranslatorDemoImg::TrayIcon());
	
	// Add menu
	AddFrame(menu);
	
	CtrlLayout(inputwindow);
	CtrlLayout(outputwindow);
	
	// set languageslist
	languageslist.Add("af", t_("Afrikaans"));
	languageslist.Add("sq", t_("Albanian"));
	languageslist.Add("am", t_("Amharic"));
	languageslist.Add("ar", t_("Arabic"));
	languageslist.Add("hy", t_("Armenian"));
	languageslist.Add("az", t_("Azerbaijani"));
	languageslist.Add("eu", t_("Basque"));
	languageslist.Add("be", t_("Belarusian"));
	languageslist.Add("bn", t_("Bengali"));
	languageslist.Add("bh", t_("Bihari"));
	languageslist.Add("bg", t_("Bulgarian"));
	languageslist.Add("my", t_("Burmese"));
	languageslist.Add("ca", t_("Catalan"));
	languageslist.Add("chr", t_("CHEROKEE"));
	languageslist.Add("zh", t_("CHINESE"));
	languageslist.Add("zh-CN", t_("Chinese (Simplified)"));
	languageslist.Add("zh-TW", t_("Chinese (Traditional)"));
	languageslist.Add("hr", t_("Croatian"));
	languageslist.Add("cs", t_("Czech"));
	languageslist.Add("da", t_("Danish"));
	languageslist.Add("dv", t_("Dhivehi"));
	languageslist.Add("nl", t_("Dutch"));
	languageslist.Add("en", t_("English"));
	languageslist.Add("eo", t_("Esperanto"));
	languageslist.Add("et", t_("Estonian"));
	languageslist.Add("tl", t_("Filipino"));
	languageslist.Add("fi", t_("Finnish"));
	languageslist.Add("fr", t_("French"));
	languageslist.Add("gl", t_("Galician"));
	languageslist.Add("ka", t_("Georgian"));
	languageslist.Add("de", t_("German"));
	languageslist.Add("el", t_("Greek"));
	languageslist.Add("gn", t_("Guarani"));
	languageslist.Add("gu", t_("Gujarati"));
	languageslist.Add("iw", t_("Hebrew"));
	languageslist.Add("hi", t_("Hindi"));
	languageslist.Add("hu", t_("Hungarian"));
	languageslist.Add("is", t_("Icelandic"));
	languageslist.Add("id", t_("Indonesian"));
	languageslist.Add("iu", t_("Inuktitut"));
	languageslist.Add("it", t_("Italian"));
	languageslist.Add("ja", t_("Japanese"));
	languageslist.Add("kn", t_("Kannada"));
	languageslist.Add("kk", t_("Kazakh"));
	languageslist.Add("km", t_("Khmer"));
	languageslist.Add("ko", t_("Korean"));
	languageslist.Add("ku", t_("Kurdish"));
	languageslist.Add("ky", t_("Kyrgyz"));
	languageslist.Add("lo", t_("Laothian"));
	languageslist.Add("lv", t_("Latvian"));
	languageslist.Add("lt", t_("Lithuanian"));
	languageslist.Add("mk", t_("Macedonian"));
	languageslist.Add("ms", t_("Malay"));
	languageslist.Add("ml", t_("Malayalam"));
	languageslist.Add("mt", t_("Maltese"));
	languageslist.Add("mr", t_("Marathi"));
	languageslist.Add("mn", t_("Mongolian"));
	languageslist.Add("ne", t_("Nepali"));
	languageslist.Add("no", t_("Norwegian"));
	languageslist.Add("or", t_("Oriya"));
	languageslist.Add("ps", t_("Pashto"));
	languageslist.Add("fa", t_("Persian"));
	languageslist.Add("pl", t_("Polish"));
	languageslist.Add("pt-PT", t_("Portuguese"));
	languageslist.Add("pa", t_("Punjabi"));
	languageslist.Add("ro", t_("Romanian"));
	languageslist.Add("ru", t_("Russian"));
	languageslist.Add("sa", t_("Sanskrit"));
	languageslist.Add("sr", t_("Serbian"));
	languageslist.Add("sd", t_("Sindhi"));
	languageslist.Add("si", t_("Sinhalese"));
	languageslist.Add("sk", t_("Slovak"));
	languageslist.Add("sl", t_("Slovenian"));
	languageslist.Add("es", t_("Spanish"));
	languageslist.Add("sw", t_("Swahili"));
	languageslist.Add("sv", t_("Swedish"));
	languageslist.Add("tg", t_("Tajik"));
	languageslist.Add("ta", t_("Tamil"));
	languageslist.Add("tl", t_("Tagalog"));
	languageslist.Add("te", t_("Telugu"));
	languageslist.Add("th", t_("Thai"));
	languageslist.Add("bo", t_("Tibetan"));
	languageslist.Add("tr", t_("Turkish"));
	languageslist.Add("uk", t_("Ukrainian"));
	languageslist.Add("ur", t_("Urdu"));
	languageslist.Add("uz", t_("Uzbek"));
	languageslist.Add("ug", t_("Uighur"));
	languageslist.Add("vi", t_("Vietnamese"));
	languageslist.Add("", t_("UNKNOWN"));
	
	for(int i = 0; i<languageslist.GetCount(); i++){
		outputwindow.fromlanguagectrl.Add(languageslist.GetKey(i), languageslist[i]);
		outputwindow.tolanguagectrl.Add(languageslist.GetKey(i), languageslist[i]);
	}
	
	inputwindow.textedit.SetCharset(CHARSET_UTF8);
	outputwindow.textedit.SetCharset(CHARSET_UTF8);
	
	verticalsplitter.Vert(inputwindow, outputwindow);
	outputwindow.textedit.SetEditable(false);
	Add(verticalsplitter.SizePos());
	Sizeable(true).Zoomable(true);
	outputwindow.translatebutton<<THISBACK1(TranslateText, true);
	outputwindow.swap<<THISBACK(swap);
	outputwindow.correcttranslationbutton<<THISBACK(CorrectTranslatedText);
	
	WhenMinimize = THISBACK(onMinimize);
	WhenClose = THISBACK(onClose);
	
	suportedLanguagies.Add(LNGC_('E','N','U','S', CHARSET_UNICODE), ToUnicode("English", CHARSET_UNICODE));
	//suportedLanguagies.Add(LNGC_('R','O','R','O', CHARSET_UNICODE), ToUnicode("Română", CHARSET_UNICODE));
	//suportedLanguagies.Add(LNGC_('R','U','R','U', CHARSET_UNICODE), ToUnicode("Руский", CHARSET_UNICODE));
	currentLanguage = GetCurrentLanguage();
	
	if(suportedLanguagies.Find(currentLanguage)<=0){
		currentLanguage = suportedLanguagies.GetKey(0);
		SetLanguage(currentLanguage);
	}
	
	trayicon.WhenLeftDouble = THISBACK(launch);
	trayicon.WhenBar = THISBACK(trayMenu);
	trayicon.Icon(GoogleTranslatorDemoImg::TrayIcon());
	trayiconAlwaysShowTrayIcon = false;
	trayicon.Hide();
	trayicon.WhenBalloonLeftDown = THISBACK(trayBallonLeftClick);
	hideInTrayIconOnMinimize = false;
	hideInTrayIconOnClose = false;
	useProxy = false;
	proxyHTTPAddress = "192.168.1.1";
	proxyHTTPPort = 808;
	AskWhenCloseApplication = true;
	refreshData();
	
	SetTimeCallback(-70, THISBACK(GlobalShortCutHandle), TIMEID_KEYBOARD);
	//TranslateHotKeyId = Ctrl::RegisterSystemHotKey(K_CTRL_C|K_C, THISBACK(GlobalShortCutHandle));
	option_dlg_initialized = false;
}

GoogleTranslatorDemo::~GoogleTranslatorDemo(){
	KillTimeCallback(TIMEID_KEYBOARD);
	//Ctrl::UnregisterSystemHotKey(TranslateHotKeyId);
}

void GoogleTranslatorDemo::onClose(){
	if(hideInTrayIconOnClose)
		hide();
	else
		Exit();
}

void GoogleTranslatorDemo::Exit(){
	// Bug when prompt message
	if((!AskWhenCloseApplication)||(PromptOKCancel(t_("Exit from application?")))){
		Break();
	};
};

void GoogleTranslatorDemo::onMinimize(){
	if(hideInTrayIconOnMinimize)
		hide();
};

void GoogleTranslatorDemo::Options(){
	//...
	if(!option_dlg_initialized){
		option_dlg_initialized = true;
	
		// tab_options_general
		for(int i=0;i<suportedLanguagies.GetCount();i++){
			tab_options_general.lang.Add(suportedLanguagies.GetKey(i), suportedLanguagies[i].ToString());
		}
		tab_options_general.OptionAlwaysShowTrayIcon = trayiconAlwaysShowTrayIcon;
		tab_options_general.OptionHideInTrayIconOnMinimize = hideInTrayIconOnMinimize;
		tab_options_general.OptionHideInTrayIconOnClose = hideInTrayIconOnClose;
		tab_options_general.OptionAskWhenCloseApplication = AskWhenCloseApplication;
		tab_options_general.OptionDoNotFormatText = doNotFormatText;
			
		tab_options_connection.useProxy = useProxy;
		tab_options_connection.proxyHTTPAddress = proxyHTTPAddress;
		tab_options_connection.proxyHTTPPort = proxyHTTPPort;
		
		tab_options_connection.useProxyAuth = useProxyAuth;
		tab_options_connection.proxyHTTPUsername = proxyHTTPUsername;
		tab_options_connection.proxyHTTPPassword = proxyHTTPPassword;
		
		tab_options_connection.proxyHTTPPassword.Password();
		
		tab_options_connection.useProxy<<THISBACK(useProxyOnChange);
		tab_options_connection.useProxyAuth<<THISBACK(useProxyAuthOnChange);
		
		// Add
		option_dlg.Add(tab_options_general, t_("General"));
		option_dlg.Add(tab_options_connection, t_("Connection"));
		option_dlg.OKCancel()
			.Sizeable()
			.Title(t_("Options"));
	};
	
	int cur_index = tab_options_general.lang.FindKey(currentLanguage);
	if(cur_index>=0)
		tab_options_general.lang.SetIndex(cur_index);
	
	useProxyOnChange();
	
	if(option_dlg.Execute() != IDOK)
		return;
	
	currentLanguage = ~tab_options_general.lang;
	SetLanguage(currentLanguage);
	
	trayiconAlwaysShowTrayIcon = ~tab_options_general.OptionAlwaysShowTrayIcon;
	hideInTrayIconOnMinimize = ~tab_options_general.OptionHideInTrayIconOnMinimize;
	hideInTrayIconOnClose = ~tab_options_general.OptionHideInTrayIconOnClose;
	AskWhenCloseApplication = ~tab_options_general.OptionAskWhenCloseApplication;
	doNotFormatText = ~tab_options_general.OptionDoNotFormatText;
	
	refreshData();
	
	useProxy = ~tab_options_connection.useProxy;
	proxyHTTPAddress = ~tab_options_connection.proxyHTTPAddress;
	proxyHTTPPort = ~tab_options_connection.proxyHTTPPort;
	
	useProxyAuth = ~tab_options_connection.useProxyAuth;
	proxyHTTPUsername = ~tab_options_connection.proxyHTTPUsername;
	proxyHTTPPassword = ~tab_options_connection.proxyHTTPPassword;
};

void GoogleTranslatorDemo::useProxyOnChange(){
	bool l_flag = ~tab_options_connection.useProxy;
	tab_options_connection.proxyHTTPAddress.Enable(l_flag);
	tab_options_connection.proxyHTTPAddressLabel.Enable(l_flag);
	tab_options_connection.proxyHTTPPort.Enable(l_flag);
	tab_options_connection.proxyHTTPPortLabel.Enable(l_flag);
	tab_options_connection.useProxyAuth.Enable(l_flag);
	useProxyAuthOnChange();
};

void GoogleTranslatorDemo::useProxyAuthOnChange(){
	bool l_flag = (~tab_options_connection.useProxy) && (~tab_options_connection.useProxyAuth);
	tab_options_connection.proxyHTTPUsername.Enable(l_flag);
	tab_options_connection.proxyHTTPUsernameLabel.Enable(l_flag);
	tab_options_connection.proxyHTTPPassword.Enable(l_flag);
	tab_options_connection.proxyHTTPPasswordLabel.Enable(l_flag);
};

void GoogleTranslatorDemo::refreshData(){
	menu.Set(THISBACK(mainMenu));
	SetAppName(t_("Google translator"));
	Title(t_("Google translator"));
	
	trayicon.Tip(t_("Google translator"));
	
	// set default translate settings
	SetLanguageFromToPos();
	
	// Set default text
	if(!sourcetext.GetLength()){
		sourcetext="write here text to translate!";
	};
	
	inputwindow.textedit.Set(sourcetext);
	
	// Set TrayIcon
	if(trayiconAlwaysShowTrayIcon){
		trayicon.Show();
	}else
		trayicon.Hide();
}

void GoogleTranslatorDemo::SetLanguageFromToPos(){
	// Set language from possition
	SetLanguagePos(outputwindow.fromlanguagectrl, languagefrom, String("en").Cat());
	SetLanguagePos(outputwindow.tolanguagectrl	, languageto, String("fr").Cat());
}

void GoogleTranslatorDemo::SetLanguagePos(DropList& ctrl, String& lang_key, String& def_key){
	// Set language possition
	int temp_index = ctrl.FindKey(lang_key);
	
	if(temp_index==-1){
		temp_index = ctrl.FindKey(def_key);
	}
	
	ctrl.SetIndex(temp_index);
}

void GoogleTranslatorDemo::swap(){
	int i = outputwindow.fromlanguagectrl.GetIndex();
	outputwindow.fromlanguagectrl.SetIndex(outputwindow.tolanguagectrl.GetIndex());
	outputwindow.tolanguagectrl.SetIndex(i);
}

void GoogleTranslatorDemo::TranslateText(bool show_progress){
	if(AtomicRead(translating))
		return;
	
	sourcetext = inputwindow.textedit.Get(CHARSET_UTF8);
	languagefrom = outputwindow.fromlanguagectrl.GetKey(outputwindow.fromlanguagectrl.GetIndex());
	languageto = outputwindow.tolanguagectrl.GetKey(outputwindow.tolanguagectrl.GetIndex());
	
	if(show_progress){
		progress.Reset();
		progress.SetText("Contacting Google Translator");
		progress.Title("Translating");
		progress.Set(0, 50);
		progress.Open();
		SetTimeCallback(-200, THISBACK(UpdateProgress), TIMEID_PROGRESS);
		
		AtomicWrite(translating, 1);
		Thread().Run(THISBACK1(HttpThread, THISBACK(CheckCancel)));
		
		while(AtomicRead(translating)) {
			ProcessEvents();
			GuiSleep(300);
		}
		
		KillTimeCallback(TIMEID_PROGRESS);
		progress.Close();
	}
	else{
		AtomicWrite(translating, 1);
		HttpThread();
	}
		
	
	outputwindow.textedit.Set(result, CHARSET_UTF8);	
}

void GoogleTranslatorDemo::UpdateProgress(){
	int p = progress.GetPos() + 1;
	progress.Set((p > progress.GetTotal()) ? 0 : p, progress.GetTotal());
}

bool GoogleTranslatorDemo::CheckCancel(int, int){
	return progress.Canceled();
}

void GoogleTranslatorDemo::HttpThread(Gate2<int, int> _progress){
	// Proxy
	if(useProxy){
		googletranslator.SetProxy(proxyHTTPAddress, proxyHTTPPort);
	}
	else
		googletranslator.SetProxy(false);
	
	if(useProxyAuth){
		googletranslator.SetProxyAuth(proxyHTTPUsername, proxyHTTPPassword);
	}
	else
		googletranslator.SetProxyAuth(false);
	
	googletranslator.Translate(sourcetext, languagefrom, languageto, _progress);
	
	if(!doNotFormatText)
		result = googletranslator.GetFormatedText();
	else
		result = googletranslator.GetTranslatedText();
	
	AtomicWrite(translating, 0);
}

void GoogleTranslatorDemo::CorrectTranslatedText(){
	WithInputCorrectionTextForm<TopWindow> dlg;
	
	CtrlLayoutOKCancel(dlg, t_("Correction trasnlated text"));
	dlg.Sizeable();

	dlg.textedit.SetData(googletranslator.GetCorrectionText());
	
	if(dlg.Execute() != IDOK)
		return;
	
	String result_text;
	bool status = googletranslator.SetCorrectionText(dlg.textedit.GetData(), result_text);
	
	PromptOK(Nvl(result_text, t_("Thank you for contributing your translation suggestion to Google Translate.")));
}

void GoogleTranslatorDemo::hide(){
	if(!trayicon.IsVisible()){
		trayicon.Show();
	};
	isHidden = true;
	TopWindow::Hide();
};

void GoogleTranslatorDemo::launch(){
	perform(LAUNCH);
}

void GoogleTranslatorDemo::perform(int newstate){
	if(newstate==LAUNCH){
		if(!IsOpen())
			OpenMain();
		Show();
		SetForeground();
		isHidden = false;
		
		if(!trayiconAlwaysShowTrayIcon)
			trayicon.Hide();
	};
}

void GoogleTranslatorDemo::Serialize(Stream &s)
{
	//int version = 1;
	//s / version;
	SerializePlacement(s);
	s % useProxy;
	s % proxyHTTPAddress;
	s % proxyHTTPPort;
	s % currentLanguage;
	s % trayiconAlwaysShowTrayIcon;
	s % hideInTrayIconOnMinimize;
	s % hideInTrayIconOnClose;
	s % languagefrom;
	s % languageto;
	s % sourcetext;
	s % AskWhenCloseApplication;
	s % useProxyAuth;
	s % proxyHTTPUsername;
	s % proxyHTTPPassword;
	s % doNotFormatText;
	refreshData();
}

void GoogleTranslatorDemo::GlobalShortCutHandle(){
	if(!AtomicRead(test_test)){
		//ProcessEvents();
		AtomicWrite(test_test,1);
		
		//hide();
		//perform(LAUNCH);
		bool need_translate = false;
		
		if(GetCtrl()){
			if(get_key_state('C')){
				if(shortcut_stage==0){ // start time
					shortcut_stage = 1; // [Ctrl_D]+[C_D]
					time_stop_shortcut.Reset();
					time_count_shortcut1 = 0;
					//trayicon.Info("Translation:","1");
				}
				else if(shortcut_stage==2){
					shortcut_stage = 3; // [Ctrl_D]+[C_A]+[C_D]
					//trayicon.Info("Translation:","3");
					time_count_shortcut3 = time_stop_shortcut.Elapsed();
				}
			}
			else{
				if(shortcut_stage==1){
					shortcut_stage = 2; // [Ctrl_D]+[C_A]
					//trayicon.Info("Translation:","2");
					time_count_shortcut2 = time_stop_shortcut.Elapsed();
				}
				else if (shortcut_stage==3){
					shortcut_stage = 4; // final [Ctrl_D]+[C_A]+[C_A]
					//trayicon.Info("Translation:","4");
					time_count_shortcut4 = time_stop_shortcut.Elapsed();
					
					int t_diff = time_count_shortcut3-time_count_shortcut2;
					
					if((t_diff > 0)&&(t_diff<600)){
						need_translate = true;
					}
					else{
						shortcut_stage = 5;
						//trayicon.Info("Translation:","5");
					}
				}
				else if (shortcut_stage==2){
					int t_diff = time_stop_shortcut.Elapsed()-time_count_shortcut2;
					
					if(t_diff>600){
						shortcut_stage = 5; // time elapsed
						//trayicon.Info("Translation:","5");
					}
				}
			}
		}
		else if (shortcut_stage != 0){
			shortcut_stage = 0;
			time_count_shortcut1 = 0;
			time_count_shortcut2 = 0;
			time_count_shortcut3 = 0;
			time_count_shortcut4 = 0;
			//trayicon.Info("Translation:","0");
		}
		
		if(need_translate){
			WString cur_text = ReadClipboardUnicodeText();
			
			if(cur_text.GetLength()){
				inputwindow.textedit.Set(cur_text);
				TranslateTextInBaloon();
			}
		}
		AtomicWrite(test_test,0);
	}
}

void GoogleTranslatorDemo::TranslateTextInBaloon(){
	trayicon.Info("Translating...","Text:\n" + (String)(inputwindow.textedit.GetData()));
	TranslateText(false);
	trayicon.Info("Translation(" + languagefrom + "->" + languageto + "):",result + "\n--------------------\nSource text("+languagefrom+"):\n" + sourcetext, 300);
}

void GoogleTranslatorDemo::About(){
	WithAboutLayout<TopWindow> dlg;
	
	dlg.text = GetTopic("topic://GoogleTranslatorDemo/app/about$en-us").text;
	
	CtrlLayoutOK(dlg, t_("About"));
	dlg.Sizeable();
	
	if(dlg.Execute() != IDOK)
		return;
}

GUI_APP_MAIN
{
	
#ifdef PLATFORM_WIN32
	static const char unique_name[] = "GoogleTranslator_$_$_U++";
	if(::FindWindow(NULL, unique_name)) {
		Exclamation(t_("GoogleTranslator is already running!"));
		return;
	}
	TopWindow singlechk;
	singlechk.SetRect(-1, -1, 1, 1);
	singlechk.Hide();
	singlechk.Title(unique_name);
	singlechk.Open();
#endif
	GoogleTranslatorDemo app_main;
	LoadFromFile(app_main);
	app_main.Run();
	StoreToFile(app_main);
}