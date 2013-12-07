#include <Core/Core.h>

using namespace Upp;

void MakeMap(VectorMap<String, int>& map, const XmlNode& n, const String& path, int& tag_count)
{
	if(map.GetCount() > 1000)
		return;
	for(int i = 0; i < n.GetCount(); i++) {
		if(n[i].IsTag()) {
			map.GetAdd(path, 0)++;
			MakeMap(map, n[i], path + "/" + n[i].GetTag(), tag_count);
			tag_count++;
		}
	}
}

void DumpMap(const XmlNode& n)
{
	VectorMap<String, int> map;
	int tag_count = 0;
	MakeMap(map, n, String(), tag_count);
	SortByKey(map);
	DUMPM(map);
	DUMP(tag_count);
}

//const char *xmlfile = "/home/cxl/20131117_ST_ZZSZ.xml";
const char *xmlfile = "/home/cxl/20131106_ST_ZZSZ.xml";

CONSOLE_APP_MAIN
{
	RLOG(MemoryProfile());
	{
		RTIMING("Parse with ignore!");
		IgnoreXmlPaths ignore("/vf:VymennyFormat/vf:Data/vf:AdresniMista/vf:AdresniMisto/ami:Geometrie;/vf:VymennyFormat/vf:Data/vf:Parcely");
		XmlNode n = ParseXML(LoadFile(xmlfile), ignore);
		RLOG("-----------");
		RLOG("With ignore");
		RLOG(MemoryUsedKb() << " KB used -------");
		RLOG(MemoryProfile());
		DumpMap(n);
	}
	{
		RTIMING("Parse all!");
		XmlNode n = ParseXML(LoadFile(xmlfile));
		RLOG("-----------");
		RLOG("ALL");
		RLOG(MemoryUsedKb() << " KB used -------");
		RLOG(MemoryProfile());
		DumpMap(n);
	}
	RLOG("-----------");
	RLOG(MemoryProfile());
}
