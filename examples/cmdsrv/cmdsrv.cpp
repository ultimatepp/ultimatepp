#include <Web/Web.h>

#ifdef PLATFORM_WIN32
#include <wincon.h>
#endif

using namespace Upp;

#define APP_VERSION "1.0.r2"
#define APP_DATE    Date(2008, 9, 22)

bool service_stopped = false;

#ifdef PLATFORM_WIN32
static BOOL WINAPI EventHandler(DWORD CtrlType)
{
	RLOG("EventHandler (CtrlType = " << (int)CtrlType << ")");
	if(CtrlType == CTRL_LOGOFF_EVENT)
		RLOG("LOGOFF_EVENT (ignored)");
	else {
		RLOG("exiting");
		service_stopped = true;
	}
	return true;
}

void InstallSignalHandler()
{
	SetConsoleCtrlHandler(&EventHandler, true);
}
#endif

#ifdef PLATFORM_POSIX
static void QuitSignal(int signal)
{
	RLOG("Received signal " << signal << ", quitting...");
	service_stopped = true;
}

void InstallSignalHandler()
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, &QuitSignal);
	signal(SIGTERM, &QuitSignal);
	signal(SIGKILL, &QuitSignal);
}
#endif

CONSOLE_APP_MAIN
{
	InstallSignalHandler();
	
	HttpServer server;
	int next_id = 0;
	server.Logging();

	Vector<String> cmd;
	cmd <<= CommandLine();
	int port = 1536;
	if(cmd.GetCount() >= 1)
		port = Nvl(ScanInt(cmd[0]), port);

	if(!server.Open(port)) {
		puts(Socket::GetErrorText());
		return;
	}

	while(!service_stopped) {
		server.DelayedWrite();
		server.Wait(1000);
		if(server.IsError()) {
			puts("Server error: " + Socket::GetErrorText());
			puts("Reopening socket...");
			fflush(stdout);
			if(!server.Reopen()) {
				puts("Reopen on server socket failed: " + Socket::GetErrorText());
				return;
			}
		}
		fputs(NFormat("\rserver(%d) running at %`", port, GetSysTime()), stdout);
		fflush(stdout);
		One<HttpRequest> request = server.GetRequest();
		if(!request)
			continue;
		HttpQuery query = request->GetQuery();
		String header, body;
		int items = query.GetInt("ITEMS", 0, 100000, 10);
		String svr = query.GetString("SVR");
		Htmls image_page;
		int i;
		image_page <<
		"CMDSRV, version " << APP_VERSION << ", release date: " << Format(APP_DATE) << "<br>\n"
		"Copyright &copy; 2005 Tomas Rylek<p>\n"
		"CMDSRV parameters: &lt;port number, default = 1536&gt;<p>\n"
		"Current parameters: <b>cmdsrv " << port << "</b><p>\n"
		"<b>items</b> (<b>" << FormatInt(items) << "</b>) = number of output lines to generate (default = 10)<br>\n"
		"<b>svr</b> (" << (IsNull(svr) ? "<i>empty</i>" : "<b>" + svr + "</b>")
		<< ") = identifier for generated output lines (default = test)<br>\n"
		"Example: <b>http://localhost";
		String wisapi = GetFileName(query.GetString("$$WISAPI"));
		String sample_query;
		sample_query << "items=" << minmax(2 * items, 10, 100000) << "&amp;svr=A";
		if(!IsNull(wisapi))
			image_page << "/dir/<a href=\"" << wisapi << "?" << sample_query << "\">" << wisapi << "?";
		else
			image_page << ':' << port << "/<a href=\"any?" << sample_query << "\">" << "any?";
		image_page << sample_query << "</a><p>\n";
		for(i = 0; i < items; i++)
			image_page << "Line #" << (i + 1) << ", svr=" << UrlEncode(svr) << "<br>\n";
		image_page << "<p>\n"
			"Current request:<br>\n";
		for(i = 0; i < query.GetCount(); i++)
			image_page << ToHtml(query.GetKey(i)) << "=<b>" << ToHtml(query.GetValue(i)) << "</b><br>\n";

		header = HttpContentType(HttpTextHtml(CHARSET_WIN1252));
		body = HtmlTitlePage("Simple text HTTP server", image_page);
		request->Write(header, body);
		request.Clear();
	}
}
