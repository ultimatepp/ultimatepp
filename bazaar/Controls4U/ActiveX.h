#ifndef _hdIE_ActiveX_h_
#define _hdIE_ActiveX_h_


#ifndef OLERENDER_DRAW	
	#define OLERENDER_DRAW	1
#endif

#define CY tagCY

#include <windows.h>
#include <Mshtml.h>  
#include <winnls.h>

#include "ActiveX_base.h"
#include <Exdisp.h> 
#include "plugin/vlc/axvlc_idl.h"

NAMESPACE_UPP

static const CLSID CLSID_MozillaBrowser = {0x1339B54C, 0x3453, 0x11D2, {0x93, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};	//http://www.iol.ie/~locka/mozilla/control.htm

class FirefoxBrowser : public DHCtrlActiveX {
public:
	FirefoxBrowser(bool status = true) : DHCtrlActiveX(CLSID_MozillaBrowser, "Firefox", status) {};

	bool Browse(const String &url);
	bool ShowHTML(const String &html);
	bool GoForward();
	bool GoBack();
	String GetLocation();
	String GetTitle();
	bool GetOffline(bool &offline);
	String GetReadyState();
	bool GoHome();
	bool RefreshPage();
	bool Stop();
	bool SetSilent(bool val);

private:	
	class IIWebBrowser {
	public:
		IIWebBrowser(FirefoxBrowser *obj) {
			if (obj->GetStatus())
				web = (IWebBrowser2 *)obj->QueryInterface(IID_IWebBrowser2);
			else
				web = 0;
		}
		~IIWebBrowser() {
			if (web)
				web->Release();
		}
		operator IWebBrowser2 *() {return web;}
		IWebBrowser2 * operator ->() {return web;}
	private:
		IWebBrowser2 *web;
	};
};

class InternetExplorerBrowser : public DHCtrlActiveX {
public:
	InternetExplorerBrowser(bool status = true) : DHCtrlActiveX(CLSID_WebBrowser, "IExplorer", status) {};

	bool Browse(const String &url);
	bool ShowHTML(const String &html);
	bool GoForward();
	bool GoBack();
	String GetLocation();
	String GetTitle();
	bool GetOffline(bool &offline);
	String GetReadyState();
	bool GoHome();
	bool RefreshPage();
	bool Stop();
	bool SetSilent(bool val = true);

private:
	class IIWebBrowser {
	public:
		IIWebBrowser(InternetExplorerBrowser *obj) {
			if (obj->GetStatus())
				web = (IWebBrowser2 *)obj->QueryInterface(IID_IWebBrowser2);
			else
				web = 0;
		}
		~IIWebBrowser() {
			if (web)
				web->Release();
		}
		operator IWebBrowser2 *() 	 {return web;}
		IWebBrowser2 * operator ->() {return web;}
	private:
		IWebBrowser2 *web;
	};
};


static const CLSID CLSID_VLCPLayer = {0x9BE31822, 0xFDAD, 0x461B, {0xAD, 0x51, 0xBE, 0x1D, 0x1C, 0x15, 0x99, 0x21}};

class VLCPlayer : public DHCtrlActiveX {
public:
	VLCPlayer(bool status = true) : DHCtrlActiveX(CLSID_VLCPLayer, "VLCPlayer", status) {};

	bool AddTarget(const String movie);
	bool Play();
	bool Pause();
	bool PlayFaster();
	bool PlaySlower();
	bool Stop();
	bool Shuttle(long secs);
	bool IsPlaying();
	int  GetVolume();
	bool PutVolume(int volume);
	int  GetTime();
	bool PutTime(int mseconds);
	float GetPosition();
	bool PutPosition(float position);
	int  GetLength();
	
private:
	class IIVLC {
	public:
		IIVLC(VLCPlayer *obj) {
			if (obj->GetStatus())
				vlc = (IVLCControl *)obj->QueryInterface(IID_IVLCControl);
			else
				vlc = 0;
		}
		~IIVLC() {
			if (vlc)
				vlc->Release();
		}
		operator IVLCControl *() {return vlc;}
		IVLCControl * operator ->() {return vlc;}
	private:
		IVLCControl *vlc;
	};
};

END_UPP_NAMESPACE

#endif
