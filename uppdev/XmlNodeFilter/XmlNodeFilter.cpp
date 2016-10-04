#include <Core/Core.h>

using namespace Upp;

Tuple2<int, String> mm[] = {
	{XML_DOC, "XML_DOC"},
	{XML_TAG, "XML_TAG"},
	{XML_END, "XML_END"},
	{XML_TEXT, "XML_TEXT"},
	{XML_DECL, "XML_DECL"},
	{XML_PI, "XML_PI"},
	{XML_COMMENT, "XML_COMMENT"},
	{XML_EOF, "XML_EOF"},
};

VectorMap<String, int> count;

void MakeMap(VectorMap<String, int>& map, const XmlNode& n, const String& path, int& tag_count, int& other_count)
{
	if(map.GetCount() > 1000)
		return;
	for(int i = 0; i < n.GetCount(); i++) {
		const Tuple2<int, String> *m = FindTuple(mm, __countof(mm), n[i].GetType());
		if(m) {
			count.GetAdd(m->b, 0)++;
			count.GetAdd(String(m->b) + ".len", 0) += n[i].GetText().GetCount();
		}
		if(n[i].IsTag()) {
			String np = path + "/" + n[i].GetTag();
			map.GetAdd(np, 0)++;
			MakeMap(map, n[i], np, tag_count, other_count);
			count.GetAdd("attr", 0) += n[i].GetAttrCount();
		}
		else
			map.GetAdd(path + ':' + (m ? m->b : "?"), 0)++;
	}
}

void DumpMap(const XmlNode& n)
{
	VectorMap<String, int> map;
	int tag_count = 0;
	int other_count = 0;
	MakeMap(map, n, String(), tag_count, other_count);
	SortByKey(map);
	RDUMPM(map);
	RDUMPM(count);
}

//const char *xmlfile = "/home/cxl/20131117_ST_ZZSZ.xml";

namespace Upp {
void *SysAllocRaw(size_t size, size_t reqsize);
};

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);

	String xmlfile = GetHomeDirFile("ruian_file/20131201_ST_ZZSZ.xml");
	RLOG(MemoryProfile());
	{
		RTIMING("Parse with ignore and stream!");
		IgnoreXmlPaths ignore(
			"/vf:VymennyFormat/vf:Data/vf:AdresniMista/vf:AdresniMisto/ami:Geometrie;"
			"/vf:VymennyFormat/vf:Data/vf:AdresniMista/vf:AdresniMisto/ami:GlobalniIdNavrhuZmeny;"
			"/vf:VymennyFormat/vf:Data/vf:AdresniMista/vf:AdresniMisto/ami:IdTransakce;"
			"/vf:VymennyFormat/vf:Data/vf:AdresniMista/vf:AdresniMisto/ami:PlatiOd;"
			"/vf:VymennyFormat/vf:Data/vf:AdresniMista/vf:AdresniMisto/ami:VOKod;"
			"/vf:VymennyFormat/vf:Data/vf:Parcely;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:Geometrie;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:IdentifikacniParcela;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:ZpusobyOchrany;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:DetailniTEA;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:DruhKonstrukceKod;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:GlobalniIdNavrhuZmeny;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:IdTransakce;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:PlatiOd;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:PocetBytu;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:PocetPodlazi;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:PodlahovaPlocha;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:PripojeniKanalizaceKod;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:PripojeniPlynKod;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:PripojeniVodovodKod;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:VybaveniVytahemKod;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:ZastavenaPlocha;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:ZpusobVytapeniKod;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:ZpusobVyuzitiKod;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:ObestavenyProstor;"
		);
		RLOG("-----------");
		RLOG("With ignore and stream!");
		XmlNode n = ParseXMLFile(xmlfile, ignore);
		RLOG(MemoryUsedKb() << " KB used -------");
		RLOG(MemoryProfile());
		DumpMap(n);
	}
	{
		RTIMING("Parse with ignore!");
		IgnoreXmlPaths ignore(
			"/vf:VymennyFormat/vf:Data/vf:AdresniMista/vf:AdresniMisto/ami:Geometrie;"
			"/vf:VymennyFormat/vf:Data/vf:AdresniMista/vf:AdresniMisto/ami:GlobalniIdNavrhuZmeny;"
			"/vf:VymennyFormat/vf:Data/vf:AdresniMista/vf:AdresniMisto/ami:IdTransakce;"
			"/vf:VymennyFormat/vf:Data/vf:AdresniMista/vf:AdresniMisto/ami:PlatiOd;"
			"/vf:VymennyFormat/vf:Data/vf:AdresniMista/vf:AdresniMisto/ami:VOKod;"
			"/vf:VymennyFormat/vf:Data/vf:Parcely;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:Geometrie;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:IdentifikacniParcela;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:ZpusobyOchrany;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:DetailniTEA;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:DruhKonstrukceKod;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:GlobalniIdNavrhuZmeny;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:IdTransakce;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:PlatiOd;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:PocetBytu;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:PocetPodlazi;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:PodlahovaPlocha;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:PripojeniKanalizaceKod;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:PripojeniPlynKod;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:PripojeniVodovodKod;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:VybaveniVytahemKod;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:ZastavenaPlocha;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:ZpusobVytapeniKod;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:ZpusobVyuzitiKod;"
			"/vf:VymennyFormat/vf:Data/vf:StavebniObjekty/vf:StavebniObjekt/soi:ObestavenyProstor;"
		);
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
