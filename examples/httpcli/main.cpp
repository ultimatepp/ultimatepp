#include <Core/Core.h>

using namespace Upp;

static String DumpSpecial(String s)
{
	String out;
	for(const char *p = s.Begin(), *e = s.End(); p < e; p++)
		if((byte)*p >= ' ' && *p != '\xFF')
			out.Cat(*p);
		else {
			switch(*p) {
				case '\a': out.Cat("[\\a]"); break;
				case '\b': out.Cat("[\\b]"); break;
				case '\f': out.Cat("[\\f]"); break;
				case '\v': out.Cat("[\\v]"); break;
				case '\t': out.Cat("[\\t]"); break;
				case '\r': out.Cat("[\\r]"); break;
				case '\n': out.Cat("[\\n]\n"); break;
				default:   out.Cat(NFormat("[\\x%02x", (byte)*p)); break;
			}
		}
	return out;
}

CONSOLE_APP_MAIN
{
	const Vector<String>& cmdline = CommandLine();
	for(int i = 0; i < cmdline.GetCount(); i++) {
		String proxy;
		if(*cmdline[i] == ':' && i + 1 < cmdline.GetCount())
			proxy = cmdline[i++].Mid(1);
		String url = cmdline[i];
		puts(url);
		fflush(stdout);
		HttpRequest request(url);
		request.Proxy(proxy);
		String content = request.Execute();
		puts("[error] " + Nvl(request.GetErrorDesc(), "OK (no error)"));
		puts(NFormat("[status %d] %s\n", request.GetStatusCode(), request.GetReasonPhrase()));
		puts(NFormat("[content] (%d bytes)\n%s", content.GetLength(), DumpSpecial(content)));
		fflush(stdout);
	}
}
