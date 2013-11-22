#ifdef _WIN32

#include <CtrlLib/CtrlLib.h>
#include "ActiveX.h" 

NAMESPACE_UPP

bool FirefoxBrowser::Browse(const String &strurl) {
	IIWebBrowser webBrowser(this);
	if (!webBrowser) 
		return false;
	
	OleStr url;
	if (!url.Set(strurl)) 
		return false;
	
	webBrowser->Navigate2(url, 0, 0, 0, 0);

	return true;
}

bool FirefoxBrowser::ShowHTML(const String &html) {
	IIWebBrowser webBrowser(this);
	if (!webBrowser) 
		return false;
	
	CBSTR pbstrLocationURL;
	if (webBrowser->get_LocationURL((BSTR *)pbstrLocationURL) != S_OK) {
		OleStr url;
		url.Set("about:blank");
		webBrowser->Navigate2(url, 0, 0, 0, 0);
	}
	bool ret = false;
	LPDISPATCH lpDisp;
	if (!webBrowser->get_Document(&lpDisp)) {
		IHTMLDocument2 *htmDoc2;
		if (!lpDisp->QueryInterface(IID_IHTMLDocument2, (void**)&htmDoc2)) {
			SAFEARRAY *sfArray;
			static const SAFEARRAYBOUND abound = {1, 0};
			if ((sfArray = SafeArrayCreate(VT_VARIANT, 1, (SAFEARRAYBOUND*)&abound))) {
				VARIANT	*pVar;
				if (!SafeArrayAccessData(sfArray, (void**)&pVar)) {
					pVar->vt = VT_BSTR;
					if(!BSTRSet(html, pVar->bstrVal))
						ret = false;
					else {					
						ret = true;
						htmDoc2->write(sfArray);
						htmDoc2->close();
					}
				}
				SafeArrayDestroy(sfArray);
			}
			htmDoc2->Release();
		}
		lpDisp->Release();
	}
	return ret;
}

bool FirefoxBrowser::GoForward() {
	IIWebBrowser webBrowser(this);
	if (!webBrowser) 
		return false;
	
	if (webBrowser->GoForward() == S_OK) 
		return true;
	return false;
}

bool FirefoxBrowser::GoBack() {
	IIWebBrowser webBrowser(this);
	if (!webBrowser) 
		return false;
	
	if (webBrowser->GoBack() == S_OK) 
		return true;
	return false;
}

String FirefoxBrowser::GetLocation() {
	IIWebBrowser webBrowser(this);
	if (!webBrowser) 
		return "";
	
	CBSTR pbstrLocationURL;
	if (webBrowser->get_LocationURL((BSTR *)pbstrLocationURL) != S_OK) 
		return "";
	
	return pbstrLocationURL.ToString();
}

String FirefoxBrowser::GetTitle() {
	IIWebBrowser webBrowser(this);
	if (!webBrowser) 
		return "";
	
	CBSTR pbstrLocationURL;
	if (webBrowser->get_LocationName((BSTR *)pbstrLocationURL) != S_OK) 
		return "";
	
	return pbstrLocationURL.ToString();
}

bool FirefoxBrowser::GetOffline(bool &offline) {
	IIWebBrowser webBrowser(this);
	if (!webBrowser) 
		return false;
	
	VARIANT_BOOL voffline;
	if (webBrowser->get_Offline(&voffline) != S_OK) 
		return false;
	
	offline = (voffline == VARIANT_TRUE) ? true : false;
	return true;
}

String FirefoxBrowser::GetReadyState() {
	IIWebBrowser webBrowser(this);
	if (!webBrowser) 
		return "";
	
	READYSTATE readystate;
	if (webBrowser->get_ReadyState(&readystate) != S_OK) 
		return "";
	
	switch (readystate) {
	case READYSTATE_UNINITIALIZED:	return "Uninitialized";	// Default initialization state
  	case READYSTATE_LOADING:		return "Loading";		// Object is currently loading its properties
  	case READYSTATE_LOADED:			return "Loaded";		// Object was initialized
  	case READYSTATE_INTERACTIVE:	return "Interactive";	// Object is interactive, but not all its data is available
  	case READYSTATE_COMPLETE:		return "Complete";		// Object has received all its data
  	default:						return "Unknown";
	}
}

bool FirefoxBrowser::GoHome() {
	IIWebBrowser webBrowser(this);
	if (!webBrowser) 
		return false;
	
	CBSTR pbstrLocationURL;
	if (webBrowser->GoHome() == S_OK) 
		return true;
	
	return false;
}

bool FirefoxBrowser::RefreshPage() {
	IIWebBrowser webBrowser(this);
	if (!webBrowser) 
		return false;
	
	CBSTR pbstrLocationURL;
	if (webBrowser->Refresh() == S_OK) 
		return true;
	
	return false;
}

bool FirefoxBrowser::Stop() {
	IIWebBrowser webBrowser(this);
	if (!webBrowser) 
		return false;
	
	CBSTR pbstrLocationURL;
	if (webBrowser->Stop() == S_OK) 
		return true;
	
	return false;
}

bool FirefoxBrowser::SetSilent(bool val) {
	IIWebBrowser webBrowser(this);
	if (!webBrowser) 
		return false;	
	
	VARIANT_BOOL vb = val ? VARIANT_TRUE : VARIANT_FALSE;
	if (webBrowser->put_Silent(vb) == S_OK) 
		return true;
	
	return false;
}

END_UPP_NAMESPACE

#endif
