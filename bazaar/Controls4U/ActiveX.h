#ifndef _hdIE_ActiveX_h_
#define _hdIE_ActiveX_h_


#ifndef OLERENDER_DRAW	
	#define OLERENDER_DRAW	1
#endif

#define CY void *

#include <windows.h>
#include <Mshtml.h>  
#include <winnls.h>

#include "ActiveX_base.h"
#include <Exdisp.h> 
#include "plugin/vlc/axvlc_idl.h"

static const CLSID CLSID_MozillaBrowser = {0x1339B54C, 0x3453, 0x11D2, {0x93, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};	//http://www.iol.ie/~locka/mozilla/control.htm

class FirefoxBrowser : public DHCtrlActiveX {
public:
	FirefoxBrowser() : DHCtrlActiveX(CLSID_MozillaBrowser, "Firefox") {};

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

private:	
	class IIWebBrowser {
	public:
		IIWebBrowser(FirefoxBrowser *obj) {
			web = (IWebBrowser2 *)obj->QueryInterface(IID_IWebBrowser2);
		}
		~IIWebBrowser() {
			web->Release();
		}
		operator IWebBrowser2 *() {return web;}
		IWebBrowser2 * operator ->() {return web;}
	private:
		IWebBrowser2 *web;
	};
};

class IExplorerBrowser : public DHCtrlActiveX {
public:
	IExplorerBrowser() : DHCtrlActiveX(CLSID_WebBrowser, "IExplorer") {};

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

private:
	class IIWebBrowser {
	public:
		IIWebBrowser(IExplorerBrowser *obj) {
			web = (IWebBrowser2 *)obj->QueryInterface(IID_IWebBrowser2);
		}
		~IIWebBrowser() {
			web->Release();
		}
		operator IWebBrowser2 *() {return web;}
		IWebBrowser2 * operator ->() {return web;}
	private:
		IWebBrowser2 *web;
	};
};


static const CLSID CLSID_VLCPLayer = {0x9BE31822, 0xFDAD, 0x461B, {0xAD, 0x51, 0xBE, 0x1D, 0x1C, 0x15, 0x99, 0x21}};

class VLCPlayer : public DHCtrlActiveX {
public:
	VLCPlayer() : DHCtrlActiveX(CLSID_VLCPLayer, "VLCPlayer") {};

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
			vlc = (IVLCControl *)obj->QueryInterface(IID_IVLCControl);
		}
		~IIVLC() {
			vlc->Release();
		}
		operator IVLCControl *() {return vlc;}
		IVLCControl * operator ->() {return vlc;}
	private:
		IVLCControl *vlc;
	};
};


#endif