#include "MainWorkSpace.h"

namespace UA{

#define IMAGECLASS UAImg
#define IMAGEFILE <UApplication/UApplication.iml>
#include <Draw/iml_source.h>

MainWorkSpace::MainWorkSpace() {
	isHidden = false;
	Sizeable();
	Zoomable();
	CtrlLayout(*this);
	AddFrame(menu);
	AddFrame(status);
	BackPaint();
	Icon(UAImg::MainIcon());
	
	whenMinimize = THISBACK(onMinimize);
	WhenClose = THISBACK(onClose);
	
	suportedLanguagies.Add(LNGC_('E','N','U','S', CHARSET_UNICODE), ToUnicode("English", CHARSET_UNICODE));
	suportedLanguagies.Add(LNGC_('R','O','R','O', CHARSET_UNICODE), ToUnicode("Română", CHARSET_UNICODE));
	suportedLanguagies.Add(LNGC_('R','U','R','U', CHARSET_UNICODE), ToUnicode("Руский", CHARSET_UNICODE));
	currentLanguage = GetCurrentLanguage();
	
	if(suportedLanguagies.Find(currentLanguage)<=0){
		currentLanguage = suportedLanguagies.GetKey(0);
		SetLanguage(currentLanguage);
	}
	trayicon.WhenLeftDouble = THISBACK(launch);
	trayicon.WhenBar = THISBACK(trayMenu);
	trayicon.Icon(UAImg::TrayIcon());
	trayiconAlwaysShowTrayIcon = false;
	trayicon.Hide();
	hideInTrayIconOnMinimize = false;
	hideInTrayIconOnClose = false;
	refreshData();
	
}
#ifdef PLATFORM_WIN32
LRESULT MainWorkSpace::WindowProc(UINT message, WPARAM wParam, LPARAM lParam){
	
	if (message == WM_SYSCOMMAND && wParam == SC_MINIMIZE){
		whenMinimize(); // callback
		return 0;
	}
	TopWindow::WindowProc(message, wParam, lParam);
}
#endif
void MainWorkSpace::onClose(){
	if(hideInTrayIconOnClose)
		hide();
	else
		Exit();
}

void MainWorkSpace::Exit(){
	// Bug when prompt message
	if(PromptOKCancel(t_("Exit from application?"))){
		Break();
	};
};

void MainWorkSpace::onMinimize(){
	if(hideInTrayIconOnMinimize)
		hide();
};

void MainWorkSpace::Options(){
	TabDlg dlg;
	WithOptionsGeneralLayout<ParentCtrl> tab_options_general;
	WithOptionsPluginsLayout<ParentCtrl> tab_options_plugins;
	
	tab_options_plugins.PluginList.AddColumn(t_("Module"), 30);
	tab_options_plugins.PluginList.AddColumn(t_("Name"), 35);
	tab_options_plugins.PluginList.AddColumn(t_("Version"), 30);
	tab_options_plugins.PluginList.AddColumn("", 5);
	
	// tab_options_general
	for(int i=0;i<suportedLanguagies.GetCount();i++){
		tab_options_general.lang.Add(suportedLanguagies.GetKey(i), suportedLanguagies[i].ToString());
	}
	
	int cur_index = tab_options_general.lang.FindKey(currentLanguage);
	if(cur_index>=0)
		tab_options_general.lang.SetIndex(cur_index);
	tab_options_general.OptionAlwaysShowTrayIcon = trayiconAlwaysShowTrayIcon;
	tab_options_general.OptionHideInTrayIconOnMinimize = hideInTrayIconOnMinimize;
	tab_options_general.OptionHideInTrayIconOnClose = hideInTrayIconOnClose;
	// Add
	
	dlg(tab_options_general, t_("General"))(tab_options_plugins, t_("Plugins"))
	   .OKCancel()
	   .Sizeable()
	   .Title(t_("Options"));
	
	if(dlg.Execute() != IDOK)
		return;
	
	currentLanguage = ~tab_options_general.lang;
	SetLanguage(currentLanguage);
	refreshData();
	trayiconAlwaysShowTrayIcon = ~tab_options_general.OptionAlwaysShowTrayIcon;
	hideInTrayIconOnMinimize = ~tab_options_general.OptionHideInTrayIconOnMinimize;
	hideInTrayIconOnClose = ~tab_options_general.OptionHideInTrayIconOnClose;
	
	if(trayiconAlwaysShowTrayIcon){
		trayicon.Show();
	}else
		trayicon.Hide();
};

void MainWorkSpace::refreshData(){
	menu.Set(THISBACK(mainMenu));
	SetAppName(t_("Ultimate Automation"));
	Title(t_("Ultimate Automation"));
	
	trayicon.Tip(t_("Ultimate Automation"));
};

void MainWorkSpace::hide(){
	if(!trayicon.IsVisible()){
		trayicon.Show();
	};
	isHidden = true;
	TopWindow::Hide();
};

void MainWorkSpace::launch()
{
	perform(LAUNCH);
}

void MainWorkSpace::perform(int newstate)
{
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

void MainWorkSpace::About(){
	WithAboutLayout<TopWindow> dlg;
	CtrlLayoutOK(dlg, t_("About"));
	
	if(dlg.Execute() != IDOK)
		return;
}
}
