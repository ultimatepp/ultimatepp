#include "ide.h"

static String GetSearchPhrase(AssistEditor& editor)
{
	return UrlEncode(Nvl(editor.GetSelection(), editor.GetWord()));
}

void Ide::OnlineSearch()
{
	LaunchWebBrowser("https://www.google.com/search?q=" + GetSearchPhrase(editor));
}

void Ide::OnlineSearchOnTheOfficialSite()
{
	LaunchWebBrowser("https://www.google.com/search?q=" + GetSearchPhrase(editor) +
	                 "&domains=www.ultimatepp.org&sitesearch=www.ultimatepp.org");
}
