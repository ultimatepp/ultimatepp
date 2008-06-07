#include "Designer.h"

int FindDlIndex(const DropList& d, const Value& val) {
	for(int i = 0; i < d.GetCount(); i++)
		if(d.GetValue(i) == val) return i;
	return -1;
}

Vector<String> LayoutDesigner::CoreLangs() {
	Vector<String> r;
	return r;
}

void LayoutDesigner::AddLang(const String& lang, bool copy) {
	int li = langlist.GetCount();
	langlist.Add(li, lang);
	if(copy)
		for(int i = 0; i < layout.GetCount(); i++) {
			Form& l = layout[i];
			for(int i = 0; i < l.item.GetCount(); i++) {
				String s = l.item[i].label.At(langi);
				l.item[i].label.At(li) = s;
				s = l.item[i].help.At(langi);
				l.item[i].help.At(li) = s;
			}
		}
	SetLayout(~layoutlist, li);
}

void  LayoutDesigner::NewLanguage() {
	WithLangLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "New language");
	dlg.copy = true;
	Vector<String> cl = CoreLangs();
	dlg.language.AddList("LNG_ALBANIAN");
	dlg.language.AddList("LNG_BELARUSIAN");
	dlg.language.AddList("LNG_BULGARIAN");
	dlg.language.AddList("LNG_CROATIAN");
	dlg.language.AddList("LNG_CZECH");
	dlg.language.AddList("LNG_DANISH");
	dlg.language.AddList("LNG_DUTCH");
	dlg.language.AddList("LNG_ENGLISH");
	dlg.language.AddList("LNG_ESTONIAN");
	dlg.language.AddList("LNG_FINNISH");
	dlg.language.AddList("LNG_FRENCH");
	dlg.language.AddList("LNG_GERMAN");
	dlg.language.AddList("LNG_GREEK");
	dlg.language.AddList("LNG_HUNGARIAN");
	dlg.language.AddList("LNG_ICELANDIC");
	dlg.language.AddList("LNG_ITALIAN");
	dlg.language.AddList("LNG_LATVIAN");
	dlg.language.AddList("LNG_LITHUANIAN");
	dlg.language.AddList("LNG_MACEDONIAN");
	dlg.language.AddList("LNG_NORWEGIAN");
	dlg.language.AddList("LNG_POLISH");
	dlg.language.AddList("LNG_PORTUGUESE");
	dlg.language.AddList("LNG_ROMANIAN");
	dlg.language.AddList("LNG_RUSSIAN");
	dlg.language.AddList("LNG_SERBIAN");
	dlg.language.AddList("LNG_SLOVAK");
	dlg.language.AddList("LNG_SLOVENIAN");
	dlg.language.AddList("LNG_SPANISH");
	dlg.language.AddList("LNG_SWEDISH");
	dlg.language.AddList("LNG_TURKISH");
	dlg.language.AddList("LNG_UKRAINIAN");
	if(dlg.Run() != IDOK) return;
	AddLang(dlg.language, dlg.copy);
}

void  LayoutDesigner::RenameLanguage() {
	if(langlist.GetCount() == 0) return;
	String s = langlist.GetValue();
	if(!EditText(s, "Pøejmenovat jazyk", "Nový jazyk")) return;
	langlist.SetValue(s);
}

void  LayoutDesigner::RemoveLanguage() {
	if(langlist.GetCount() == 0) return;
	if(!PromptYesNo("Remove current language ?")) return;
	int i;
	Vector<String> s;
	for(i = 0; i < langlist.GetCount(); i++)
		if(i != langi)
			s.Add(langlist.GetValue(i));
	if(langlist.GetCount() > 1)
		for(i = 0; i < layout.GetCount(); i++) {
			Form& l = layout[i];
			for(int i = 0; i < l.item.GetCount(); i++) {
				l.item[i].label.Remove(langi);
				l.item[i].help.Remove(langi);
			}
		}
	langlist.Clear();
	for(i = 0; i < s.GetCount(); i++)
		langlist.Add(i, s[i]);
	SetLayout(~layoutlist, 0);
}

void  LayoutDesigner::AddCoreLanguages() {
	int id = PromptYesNoCancel("Copy current language ?");
	if(id == IDCANCEL) return;
	Vector<String> cl = CoreLangs();
	for(int i = 0; i < cl.GetCount(); i++)
		AddLang(cl[i], id == IDYES);
}

void  LayoutDesigner::PrevLang() {
	if(langlist.GetCount() == 0) return;
	langlist.SetIndex((langlist.GetIndex() + 1) % langlist.GetCount());
	EnterLanguage();
}

void  LayoutDesigner::NextLang() {
	if(langlist.GetCount() == 0) return;
	langlist.SetIndex((langlist.GetIndex() + langlist.GetCount() - 1) % langlist.GetCount());
	EnterLanguage();
}
