#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct Downloader {
	Progress    pi;
	HttpRequest http;
	int64       loaded;
	String      url;
	FileOut     out;
	String      path;
	
	void Content(const void *ptr, int size);
	void Perform();
	void ShowProgress();
	void Start();
	
	Downloader();
};

Downloader::Downloader()
{
	http.MaxContentSize(INT_MAX);
	http.WhenContent = [=](const void *ptr, int size) { Content(ptr, size); };
	http.WhenWait = http.WhenDo = [=] { ShowProgress(); };
	http.WhenStart = [=] { Start(); };
}

void Downloader::Start()
{
	if(out.IsOpen()) {
		out.Close();
		DeleteFile(path);
	}
	loaded = 0;
}

void Downloader::Perform()
{
	url = "http://downloads.sourceforge.net/project/upp/upp/4179/upp-x11-src-4179.tar.gz";
	for(;;) {
		if(!EditText(url, "Download", "URL"))
			break;
		pi.Reset();
		http.New();
		path = AppendFileName(Nvl(GetDownloadFolder(), GetHomeDirFile("downloads")),
		                      GetFileName(url));
		http.Url(url).Execute();
		if(out.IsOpen())
			out.Close();
		if(!http.IsSuccess()) {
			DeleteFile(path);
			Exclamation("Download has failed.&\1" +
			            (http.IsError() ? http.GetErrorDesc()
			                            : AsString(http.GetStatusCode()) + ' ' + http.GetReasonPhrase()));
		}
	}
}

void Downloader::Content(const void *ptr, int size)
{
	loaded += size;
	if(!out.IsOpen()) {
		RealizePath(path);
		out.Open(path);
	}
	out.Put(ptr, size);
}

void Downloader::ShowProgress()
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
	Downloader().Perform();
}
