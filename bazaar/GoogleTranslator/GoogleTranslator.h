#ifndef _GoogleTranslator_GoogleTranslator_h
#define _GoogleTranslator_GoogleTranslator_h

#include <Web/Web.h>

using namespace Upp;

class GoogleTranslator {
public:
	typedef GoogleTranslator CLASSNAME;
	GoogleTranslator();
	//~GoogleTranslator();
	bool Translate(Gate2<int, int> _progress = false);
	bool Translate(String source_text, Gate2<int, int> _progress = false)									{sourceText = source_text; return Translate(_progress);};
	bool Translate(String source_text, String lang_from, Gate2<int, int> _progress = false)					{sourceText = source_text; languageFrom = lang_from; return Translate(_progress);};
	bool Translate(String source_text, String lang_from, String lang_to, Gate2<int, int> _progress = false)	{sourceText = source_text; languageFrom = lang_from; languageTo = lang_to; return Translate(_progress);};
	String GetLanguageFrom()				{return languageFrom;};
	String GetLanguageTo()					{return languageTo;};
	String GetSourceText()					{return sourceText;};
	String GetTranslatedText()				{return translatedText;};
	String GetFormatedText();
	String GetCorrectionText()				{return correctionText;};
	String GetCorrectedText()				{return correctedText;};
	bool IsTranslated()						{return isTranslated;}
	bool SetCorrectionText(String correction_text, String& result_text, Gate2<int, int> _progress = false);
	bool SetProxy(bool use_proxy)			{return(useProxy = use_proxy);};
	void SetProxy(String proxy_address, int port)	{useProxy = true; proxyHTTPAddress = proxy_address; proxyHTTPPort = port;};
	bool UseProxy()									{return useProxy;}
	String GetProxyAddress()						{return proxyHTTPAddress;}
	int GetproxyPort()								{return proxyHTTPPort;}
	bool SetProxyAuth(bool use_proxy)				{return(useProxyAuth = use_proxy);};
	void SetProxyAuth(String proxy_u, String proxy_p)	{useProxyAuth = true; proxyHTTPUsername = proxy_u; proxyHTTPPassword = proxy_p;};
	bool UseProxyAuth()								{return (useProxyAuth&&useProxy);}
private:
	void refreshProxy();
	bool sendHttpGet(String& url, String& post, String& result_text, Gate2<int, int> _progress = false);
	bool sendHttpPost(String& url, String& post, String& result_text, Gate2<int, int> _progress = false);
	
	String sourceText;
	String translatedText;
	String formatedText; // If I use many time GetFormatedText, the result is cashed in this property
	String correctionText; // if is term get only first term, in other case is identic as sourcetranslatedtext
	String correctedText;
	
	String languageFrom;
	String languageTo;
	
	bool useProxy;
	String proxyHTTPAddress;
	int proxyHTTPPort;
	
	bool useProxyAuth;
	String proxyHTTPUsername;
	String proxyHTTPPassword;
	
	bool isTranslated;
	HttpClient httpClient;
};

// Need discuss about this and add in String or get core function
String replace_string (String& s1, String& find, String replace);

#endif