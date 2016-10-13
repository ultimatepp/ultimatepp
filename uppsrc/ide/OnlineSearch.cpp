#include "ide.h"

void Ide::OnlineSearch()
{
	String selection = editor.GetSelection();
	if (selection.IsEmpty()) {
		return;
	}
	
	LaunchWebBrowser("https://www.google.pl/search?q=" + UrlEncode(selection));
}

void Ide::OnlineSearchOnTheOfficialSite()
{
	String selection = editor.GetSelection();
	if (selection.IsEmpty()) {
		return;
	}
	
	LaunchWebBrowser("https://www.google.pl/search?q=" + UrlEncode(selection) +
	                 "&domains=www.ultimatepp.org&sitesearch=www.ultimatepp.org");
}
