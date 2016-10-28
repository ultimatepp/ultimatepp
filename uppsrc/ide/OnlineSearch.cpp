#include "ide.h"

String Ide::GetSearchPhrase()
{
	return UrlEncode(Nvl(editor.GetSelection(), editor.GetWord()));
}

void Ide::OnlineSearch()
{
	LaunchWebBrowser("https://www.google.com/search?q=" + GetSearchPhrase());
}

void Ide::OnlineSearchOnTheOfficialSite()
{
	LaunchWebBrowser("https://www.google.com/search?q=" + GetSearchPhrase() +
	                 "&domains=www.ultimatepp.org&sitesearch=www.ultimatepp.org");
}
