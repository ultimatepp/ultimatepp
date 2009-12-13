#include "main.h"

void GoogleTranslatorDemo::trayMenu(Bar& CurrentBar){
	if(isHidden)
		CurrentBar.Add(t_("Show application"), THISBACK(launch));
	else
		CurrentBar.Add(t_("Hide application"), THISBACK(hide));
	
	CurrentBar.MenuSeparator();
	MenuTextManagement(CurrentBar, true);
	CurrentBar.MenuSeparator();
	
	CurrentBar.Add(t_("Options"), THISBACK(Options));
	
	CurrentBar.Add(AsString(t_("About")) + "...", THISBACK(About));
	CurrentBar.Add(t_("Exit"), GoogleTranslatorDemoImg::ExitImg(), THISBACK(Exit));
}

void GoogleTranslatorDemo::MenuTextManagement(Bar& CurrentBar, bool need_translate){
	CurrentBar.Add(String(t_("Tranlsate direction")) + "(" + languagefrom + "->" + languageto + ")", THISBACK1(MenuSetLanguageFrom, need_translate));
	CurrentBar.Add(String(t_("Tranlsate current text")) + "(" + languagefrom + "->" + languageto + ")...", THISBACK(TranslateTextInBaloon));
}

void GoogleTranslatorDemo::trayBallonLeftClick(){
	
	MenuBar::Execute(this, THISBACK1(MenuTextManagement, true), GetMousePos());
}

void GoogleTranslatorDemo::MenuSetLanguageFrom(Bar& currentBar, bool& need_translate){
	String lang_from = outputwindow.fromlanguagectrl.GetKey(outputwindow.fromlanguagectrl.GetIndex());
	String cur_lang_key;
	
	for(int i = 0; i < languageslist.GetCount(); ++i){
		cur_lang_key = languageslist.GetKey(i);
		
		VectorMap<String, String> cur_parameter;
		
		cur_parameter.Add("LanguageFrom", cur_lang_key);
		cur_parameter.Add("NeedTranslate", need_translate ? "TRUE" : "FALSE");
		
		if(cur_lang_key == lang_from)
			currentBar.Add(languageslist[i], GoogleTranslatorDemoImg::CurrentLang(), THISBACK1(MenuSetLanguageTo, cur_parameter));
		else if (cur_lang_key == languagefrom)
			currentBar.Add(languageslist[i], GoogleTranslatorDemoImg::CurrentTranslatedLang(), THISBACK1(MenuSetLanguageTo, cur_parameter));
		else
			currentBar.Add(languageslist[i], THISBACK1(MenuSetLanguageTo, cur_parameter));
	}
}

void GoogleTranslatorDemo::MenuSetLanguageTo(Bar& currentBar, VectorMap<String, String> &cur_parameter){
	bool need_translate = (cur_parameter.Get("NeedTranslate") == "TRUE");
	String language_from = cur_parameter.Get("LanguageFrom");
	
	String lang_to = outputwindow.tolanguagectrl.GetKey(outputwindow.tolanguagectrl.GetIndex());
	String cur_lang_key;
	
	for(int i = 0; i < languageslist.GetCount(); ++i){
		cur_lang_key = languageslist.GetKey(i);
		
		if(cur_lang_key != language_from){
			if(cur_lang_key == lang_to)
				currentBar.Add(languageslist[i], GoogleTranslatorDemoImg::CurrentLang(), THISBACK3(SetLanguageFromToPos, language_from, cur_lang_key, need_translate));
			else if (cur_lang_key == languageto)
				currentBar.Add(languageslist[i], GoogleTranslatorDemoImg::CurrentTranslatedLang(), THISBACK3(SetLanguageFromToPos, language_from, cur_lang_key, need_translate));
			else
				currentBar.Add(languageslist[i], THISBACK3(SetLanguageFromToPos, language_from, cur_lang_key, need_translate));
		}
	}
}

void GoogleTranslatorDemo::SetLanguageFromToPos(String lang_from, String lang_to, bool& need_translate){
	SetLanguagePos(outputwindow.fromlanguagectrl, lang_from, String("en").Cat());
	SetLanguagePos(outputwindow.tolanguagectrl	, lang_to, String("fr").Cat());
	
	if(need_translate){
		TranslateTextInBaloon();
	}
}