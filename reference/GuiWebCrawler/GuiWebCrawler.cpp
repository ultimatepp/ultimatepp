#include <CtrlLib/CtrlLib.h>
#include <Core/SSL/SSL.h>

using namespace Upp;

#define LAYOUTFILE <GuiWebCrawler/GuiWebCrawler.lay>
#include <CtrlCore/lay.h>

struct WebCrawler : public WithCrawlerLayout<TopWindow> {
	BiVector<int>          todo;
	VectorMap<String, int> url;
	
	struct Work {
		HttpRequest http;
		int         urli;
	};
	Array<Work>      http;
	int64            total;
	
	void ExtractUrls(const String& html, int srci);
	void ShowPath();
	void OpenURL(ArrayCtrl& a);

public:
	void Run();

	WebCrawler();
};

bool IsUrlChar(int c)
{
	return c == ':' || c == '.' || IsAlNum(c) || c == '_' || c == '%' || c == '/';
}

void WebCrawler::ExtractUrls(const String& html, int srci)
{
	int q = 0;
	while(q < html.GetCount()) {
		int http = html.Find("http://", q);
		int https = html.Find("https://", q);
		q = min(http < 0 ? https : http, https < 0 ? http : https);
		if(q < 0)
			return;
		int b = q;
		while(q < html.GetCount() && IsUrlChar(html[q]))
			q++;
		String u = html.Mid(b, q - b);
		if(url.Find(u) < 0) {
			todo.AddTail(url.GetCount());
			url.Add(u, srci);
		}
	}
}

void WebCrawler::Run()
{
	String seed = "www.ultimatepp.org";
	if(!EditText(seed, "GuiWebSpider", "Seed URL"))
		return;
	todo.AddTail(0);
	url.Add(seed);
	Open();
	while(IsOpen()) {
		ProcessEvents();
		while(todo.GetCount() && http.GetCount() < 60) {
			int i = todo.Head();
			todo.DropHead();
			Work& w = http.Add();
			w.urli = i;
			w.http.Url(url.GetKey(i))
			      .UserAgent("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:11.0) Gecko/20100101 Firefox/11.0")
			      .Timeout(0);
			work.Add(url.GetKey(i));
			work.HeaderTab(0).SetText(Format("URL (%d)", work.GetCount()));
		}
		SocketWaitEvent we;
		for(int i = 0; i < http.GetCount(); i++)
			we.Add(http[i].http, http[i].http.GetWaitEvents());
		we.Wait(10);
		int i = 0;
		while(i < http.GetCount()) {
			Work& w = http[i];
			w.http.Do();
			String u = url.GetKey(w.urli);
			int q = work.Find(u);
			if(w.http.InProgress()) {
				if(q >= 0)
					work.Set(q, 1, w.http.GetPhaseName());
				i++;
			}
			else {
				String html = w.http;
				total += html.GetCount();
				finished.Add(u, w.http.IsError() ? String().Cat() << w.http.GetErrorDesc()
				                                 : String().Cat() << w.http.GetStatusCode()
				                                   << ' ' << w.http.GetReasonPhrase()
				                                   << " (" << html.GetCount() << " bytes)",
				             w.urli);
				finished.HeaderTab(0).SetText(Format("Finished (%d)", finished.GetCount()));
				finished.HeaderTab(1).SetText(Format("Response (%` KB)", total >> 10));
				if(w.http.IsSuccess()) {
					ExtractUrls(html, w.urli);
					Title(AsString(url.GetCount()) + " URLs found");
				}
				http.Remove(i);
				work.Remove(q);
			}
		}
	}
}

void WebCrawler::ShowPath()
{
	path.Clear();
	if(!finished.IsCursor())
		return;
	int i = finished.Get(2);
	Vector<String> p;
	for(;;) {
		p.Add(url.GetKey(i));
		if(i == 0)
			break;
		i = url[i];
	}
	for(int i = p.GetCount() - 1; i >= 0; i--)
		path.Add(p[i]);
}

void WebCrawler::OpenURL(ArrayCtrl& a)
{
	String u = a.GetKey();
	WriteClipboardText(u);
	LaunchWebBrowser(u);
}

WebCrawler::WebCrawler()
{
	CtrlLayout(*this, "WebCrawler");
	work.AddColumn("URL");
	work.AddColumn("Status");
	finished.AddColumn("Finished");
	finished.AddColumn("Response");
	finished.WhenCursor = [=] { ShowPath(); };
	finished.WhenLeftDouble = [=] { OpenURL(finished); };
	path.AddColumn("Path");
	path.WhenLeftDouble = [=] { OpenURL(path); };
	total = 0;
	Zoomable().Sizeable();
}

GUI_APP_MAIN
{
	HttpRequest::Trace();

	WebCrawler().Run();
}
