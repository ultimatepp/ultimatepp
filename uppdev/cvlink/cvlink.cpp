#include <Core/Core.h>

CONSOLE_APP_MAIN
{
	FindFile ff("d:/wwwupp/*.html");
	while(ff) {
		String url = "http://www.ultimatepp.org/" + ff.GetName();
		SaveFile(AppendFileName("d:/wwwlink", ff.GetName()),
			"<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">\r\n"
			"<meta HTTP-EQUIV=\"Refresh\" CONTENT=\"3; URL=" + url + "\">\r\n"
			"<title>Redirecting ... </title>\r\n"
			"</head>\r\n"
			"<body bgcolor=\"#FFFFFF\">\r\n"
			"<H4>\r\n"
			"<a href=\"" + url + "\">Ultimate++ homepage has moved</p>\r\n"
			"</body></html>"
		);
		ff.Next();
	}
}
