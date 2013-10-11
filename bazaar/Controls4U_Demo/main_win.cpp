#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#include <Controls4U/Controls4U.h>

#if defined(PLATFORM_WIN32) 

#include "Controls4U_Demo_win.h"
#include "JBControlsDemo.h"
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
	switchOn.WhenPush = THISBACK(SwitchOn);
	
	url  <<= "http://www.ultimatepp.org/";
	html <<= "<H2><CENTER>HTML string test</CENTER></H2><P><FONT COLOR=RED>This is a <U>HTML string</U> in memory.</FONT>";
}

void Firefox_Demo::SwitchOn() {
	if (switchOn.GetLabel() == "Switch the control on") {
		iexplorer.SetStatus(true);
		switchOn.SetLabel("Switch the control off");
	} else {
		iexplorer.SetStatus(false);
		switchOn.SetLabel("Switch the control on");
	}
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
	title = "";
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
	if (!iexplorer.IsLoaded()) {
		offline = "ActiveX or program not available";	
		status = "";
		return;
	}
	static String location;
	String newLocation = iexplorer.GetLocation();
	if (newLocation != "")
		if (location != newLocation) {
			url <<= newLocation;
			location = newLocation;
		}
	title = iexplorer.GetTitle();
	bool off;
	iexplorer.GetOffline(off);
	offline = off ? "Offline" : "Online";
	status = iexplorer.GetReadyState();
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
	switchOn.WhenPush = THISBACK(SwitchOn);
	
	url  <<= "http://www.ultimatepp.org/";
	html <<= "<H2><CENTER>HTML string test</CENTER></H2><P><FONT COLOR=RED>This is a <U>HTML string</U> in memory.</FONT>";
}

void IExplorer_Demo::SwitchOn() {
	if (switchOn.GetLabel() == "Switch the control on") {
		iexplorer.SetStatus(true);
		switchOn.SetLabel("Switch the control off");
	} else {
		iexplorer.SetStatus(false);
		switchOn.SetLabel("Switch the control on");
	}
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
	title = "";
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
	if (!iexplorer.IsLoaded()) {
		offline = "ActiveX or program not available";	
		status = "";
		return;
	}
	static String location;
	String newLocation = iexplorer.GetLocation();
	if (newLocation != "")
		if (location != newLocation) {
			url <<= newLocation;
			location = newLocation;
		}
	title = iexplorer.GetTitle();
	bool off;
	iexplorer.GetOffline(off);
	offline = off ? "Offline" : "Online";
	status = iexplorer.GetReadyState();
}

VLC_Demo::VLC_Demo() {
	CtrlLayout(*this);
	
	file.WhenChange = THISBACK(Load);
	play.WhenPush = THISBACK(Play);
	pause.WhenPush = THISBACK(Pause);	
	stop.WhenPush = THISBACK(Stop);
	switchOn.WhenPush = THISBACK(SwitchOn);
}

void VLC_Demo::SwitchOn() {
	if (switchOn.GetLabel() == "Switch the control on") {
		player.SetStatus(true);
		switchOn.SetLabel("Switch the control off");
	} else {
		player.SetStatus(false);
		switchOn.SetLabel("Switch the control on");
		file <<= "";
	}
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
	if (!player.IsLoaded()) {
		offline = "ActiveX or program not available";	
		return;
	} else
		offline = "";
	int t = player.GetTime();
	if (t == -1)
		time <<= "-";	
	else
		time <<= FormatDouble(t/1000., 2, FD_ZERO);
	int l = player.GetLength();
	if (l == -1)
		length = "-";	
	else
		length = FormatDouble(l/1000., 2, FD_ZERO);
}


#endif
