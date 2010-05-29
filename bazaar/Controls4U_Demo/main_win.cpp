#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include "Controls4U/Controls4U.h"
#include "Functions4U/Functions4U.h"

#if defined(PLATFORM_WIN32) 

#include "Controls4U_Demo_win.h"
#include "Controls4U_Demo.h"


Firefox_Demo::Firefox_Demo() {
	CtrlLayout(*this);
	
	browse.WhenPush = THISBACK(Browse);
	showHTML.WhenPush = THISBACK(ShowHTML);
	forward.WhenPush = THISBACK(Forward);
	backward.WhenPush = THISBACK(Backward);
	home.WhenPush = THISBACK(Home);
	stop.WhenPush = THISBACK(Stop);
	refresh.WhenPush = THISBACK(RefreshPage);
	
	url  <<= "http://www.ultimatepp.org/";
	html <<= "<H2><CENTER>HTML string test</CENTER></H2><P><FONT COLOR=RED>This is a <U>HTML string</U> in memory.</FONT>";
}

void Firefox_Demo::Browse() {
	if (!iexplorer.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	iexplorer.Browse(url);
}

void Firefox_Demo::ShowHTML() {
	if (!iexplorer.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	iexplorer.ShowHTML(html);
	title.SetText("");
}

void Firefox_Demo::Forward() {
	if (!iexplorer.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	iexplorer.GoForward();
}

void Firefox_Demo::Backward() {
	if (!iexplorer.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	iexplorer.GoBack();
}

void Firefox_Demo::Home() {
	if (!iexplorer.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	iexplorer.GoHome();
}

void Firefox_Demo::Stop() {
	if (!iexplorer.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	iexplorer.Stop();
}

void Firefox_Demo::RefreshPage() {
	if (!iexplorer.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	iexplorer.RefreshPage();
}

void Firefox_Demo::UpdateInfo() {
	if (!iexplorer.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	static String location;
	String newLocation = iexplorer.GetLocation();
	if (newLocation != "")
		if (location != newLocation) {
			url <<= newLocation;
			location = newLocation;
		}
	title.SetText(iexplorer.GetTitle());
	bool off;
	iexplorer.GetOffline(off);
	offline.SetText(off ? "Offline" : "Online");
	status.SetText(iexplorer.GetReadyState());
}

IExplorer_Demo::IExplorer_Demo() {
	CtrlLayout(*this);
	
	browse.WhenPush = THISBACK(Browse);
	showHTML.WhenPush = THISBACK(ShowHTML);
	forward.WhenPush = THISBACK(Forward);
	backward.WhenPush = THISBACK(Backward);
	home.WhenPush = THISBACK(Home);
	stop.WhenPush = THISBACK(Stop);
	refresh.WhenPush = THISBACK(RefreshPage);
	
	url  <<= "http://www.ultimatepp.org/";
	html <<= "<H2><CENTER>HTML string test</CENTER></H2><P><FONT COLOR=RED>This is a <U>HTML string</U> in memory.</FONT>";
}

void IExplorer_Demo::Browse() {
	if (!iexplorer.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	iexplorer.Browse(url);
}

void IExplorer_Demo::ShowHTML() {
	if (!iexplorer.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	iexplorer.ShowHTML(html);
	title.SetText("");
}

void IExplorer_Demo::Forward() {
	if (!iexplorer.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	iexplorer.GoForward();
}

void IExplorer_Demo::Backward() {
	if (!iexplorer.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	iexplorer.GoBack();
}

void IExplorer_Demo::Home() {
	if (!iexplorer.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	iexplorer.GoHome();
}

void IExplorer_Demo::Stop() {
	if (!iexplorer.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	iexplorer.Stop();
}

void IExplorer_Demo::RefreshPage() {
	if (!iexplorer.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	iexplorer.RefreshPage();
}

void IExplorer_Demo::UpdateInfo() {
	if (!iexplorer.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	static String location;
	String newLocation = iexplorer.GetLocation();
	if (newLocation != "")
		if (location != newLocation) {
			url <<= newLocation;
			location = newLocation;
		}
	title.SetText(iexplorer.GetTitle());
	bool off;
	iexplorer.GetOffline(off);
	offline.SetText(off ? "Offline" : "Online");
	status.SetText(iexplorer.GetReadyState());
}

VLC_Demo::VLC_Demo() {
	CtrlLayout(*this);
	
	file.WhenChange = THISBACK(Load);
	play.WhenPush = THISBACK(Play);
	pause.WhenPush = THISBACK(Pause);	
	stop.WhenPush = THISBACK(Stop);
}

void VLC_Demo::Load() {
	if (!player.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	player.AddTarget(file);
}

void VLC_Demo::Play() {
	if (!player.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	player.Play();
	file.Enable(false);
}

void VLC_Demo::Pause() {
	if (!player.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	player.Pause();
	if (player.IsPlaying())
		file.Enable(false);
	else
		file.Enable(true);
}

void VLC_Demo::Stop() {
	if (!player.IsLoaded())
		Exclamation("Sorry. ActiveX or program not available");	
	player.Stop();
	file.Enable(true);
}

void VLC_Demo::UpdateInfo() {
	if (!player.IsLoaded())
		return;
	int t = player.GetTime();
	if (t == -1)
		time <<= "-";	
	else
		time <<= FormatDouble(t/1000., 2, FD_ZERO);
	int l = player.GetLength();
	if (l == -1)
		length.SetText("-");	
	else
		length.SetText(FormatDouble(l/1000., 2, FD_ZERO));
}

#endif
