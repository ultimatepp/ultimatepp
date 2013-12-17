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

#ifdef PLATFORM_WIN32
const char *xmlfile = "d:/20131117_ST_ZZSZ.xml";
#else
const char *xmlfile = "/home/cxl/20131117_ST_ZZSZ.xml";
#endif
//const char *xmlfile = "/home/cxl/20131106_ST_ZZSZ.xml";

namespace Upp {
void *SysAllocRaw(size_t size, size_t reqsize);
};

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

//	MemoryLimitKb(550 * 1024);

	RLOG(MemoryProfile());
	{
		Vector<String> x;
		for(int i = 0; i < 1024; i++)
			x.Add(String('a', 30000));
		RLOG(MemoryUsedKb() << " KB used -------");
		RLOG(MemoryProfile());
	}
	RLOG("* After large " << MemoryUsedKb() << " KB used -------");
	RLOG(MemoryProfile());
	if(0) {
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
	RLOG(MemoryUsedKb() << " KB used -------");
	RLOG(MemoryProfile());
	RLOG("Press a key to continue");
	getchar();
	RLOG("----------- Shrink");
	MemoryShrink();
	RLOG(MemoryUsedKb() << " KB used -------");
	RLOG(MemoryProfile());
	RLOG("Press a key to continue");
	getchar();
}
