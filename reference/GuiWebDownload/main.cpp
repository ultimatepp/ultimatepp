#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct Loader {
	Progress    pi;
	HttpRequest http;
	int64       loaded;
	String      url;
	FileOut     out;
	
	typedef Loader CLASSNAME;
	
	void ProcessContent(const void *ptr, int size);
	void Perform();
	void ShowProgress();
};

void Loader::Perform()
{
	url = "http://www.samgrob.ch/images/video/m60/m60_mtzion-nw-music1.divx";
	for(;;) {
		if(!EditText(url, "Download", "URL"))
			break;
		loaded = 0;
		pi.Reset();
		http.New();
		http.Url(url).MaxContentSize(INT_MAX);
		http.WhenContent = THISBACK(ProcessContent);
		http.WhenWait = http.WhenDo = THISBACK(ShowProgress);
		http.Execute();
		if(!http.IsSuccess())
			Exclamation("Failed !&\1" + http.GetErrorDesc());
	}
}

void Loader::ProcessContent(const void *ptr, int size)
{
	loaded += size;
	if(!out.IsOpen())
}

void Loader::ShowProgress()
{
	if(http.GetContentLength() >= 0) {
		pi.SetText("Downloading " + GetFileName(url));
		pi.Set((int)loaded, (int)http.GetContentLength());
	}
	else {
		pi.Set(0, 0);
		pi.SetText(http.GetPhaseName());
	}
	if(pi.Canceled())
		http.Abort();
}

GUI_APP_MAIN
{
	HttpRequest::Trace();
	Loader().Perform();
}
