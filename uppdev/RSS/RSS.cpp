#include <Web/Web.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	HttpClient client;
	client.URL("http://losangeles.craigslist.org/search/jjj?query=php&format=rss");
	String out = client.ExecuteRedirect();
	XmlNode n = ParseXML(out, 0);
	int q = n.FindTag("rdf:RDF");
	if(q >= 0) {
		const XmlNode& channel = n[q];
		for(int i = channel.FindTag("item"); i<channel.GetCount(); i++) {
			if(channel[i].GetTag() == "item") {
				const XmlNode &item=channel[i];
				int tag = item.FindTag("title");
				if(tag >= 0) {
					const XmlNode& title = item[tag];
					for(int j = 0; j < title.GetCount(); j++) {
						DUMP(title[j].GetType());
						DUMP(title[j].GetText());
					}
				}
			}
		}
	}
}
