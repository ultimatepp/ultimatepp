#include "TCtrlLibHelp.h"
#pragma hdrstop

NAMESPACE_UPP

void RegisterHelpStdItems() {}

RegisterHelpTopicHost("style", "", CNULL)
RegisterHelpTopicHost("toc", "", CNULL)
RegisterHelpTopicHost("book", "", CNULL)


struct sScanObjects : RichText::Iterator
{
	virtual bool operator()(int pos, const RichPara& para);
	void Add(const Array<RichPara::Part>& parts);
	String topic;
	VectorMap< String, VectorMap<String, int> > map;
};

void sScanObjects::Add(const Array<RichPara::Part>& parts)
{
	for(int i = 0; i < parts.GetCount(); i++)
		if(parts[i].object)
			map.GetAdd(parts[i].object.GetTypeName()).GetAdd(topic, 0)++;
		else if(!parts[i].fieldpart.IsEmpty())
			Add(parts[i].fieldpart);
}

bool sScanObjects::operator () (int pos, const RichPara& para)
{
	Add(para.part);
	return true;
}

static void sEnumObjects(String& qtf)
{
	static VectorMap<int, String> out;
	static int out_seq;
	if(out_seq < HelpTopicGetSeq())
		out.Clear();
	int lang = GetCurrentLanguage();
	int f = out.Find(lang);
	if(f >= 0) {
		qtf = out[f];
		return;
	}
	out_seq = HelpTopicGetSeq();
	const HelpTopicInfoMap& map = HelpTopicMap();
	Progress progress(t_("Preparing object list"), map.GetCount());
	sScanObjects scanner;
	String styles = HelpTopicGet(HelpDPPStyle()).GetText();
	int i;
	for(i = 0; i < map.GetCount(); i++) {
		String drl = map.GetKey(i);
		String space, nesting, topic;
		HelpParseDPP(drl, space, nesting, topic);
		if(nesting == "sys")
			continue;
		RichText rt = ParseQTF(styles + map[i].GetText());
		scanner.topic = drl;
		rt.Iterate(scanner);
		if(progress.StepCanceled())
			break;
	}
	qtf << NFormat("[A [a100 %d typ(y) objektù:&\n]", scanner.map.GetCount());
	for(i = 0; i < scanner.map.GetCount(); i++) {
		int total = Sum(scanner.map[i]);
		qtf << NFormat("[* \1%s\1] - [/ %d výskytù]&\n", scanner.map.GetKey(i), Sum(scanner.map[i]));
	}
	for(i = 0; i < scanner.map.GetCount(); i++) {
		qtf << NFormat("[3*_b200a100 \1%s\1]&\n", scanner.map.GetKey(i));
		Vector<String> topics = scanner.map[i].PickKeys();
		Vector<int> counts = scanner.map[i].PickValues();
		IndexSort(counts, topics, StdGreater<int>());
		for(int t = 0; t < topics.GetCount(); t++)
			qtf << NFormat("[o \1%s\1: [/ %d x]&\n", topics[t], counts[t]);
	}
	out.Add(lang, qtf);
}

RegisterHelpTopicSys("objects", t_("Graphic object types used by documentation"), callback(&sEnumObjects));

static void sEnumStyles(String& qtf)
{
	static VectorMap<int, String> out;
	static int out_seq;
	if(out_seq < HelpTopicGetSeq())
		out.Clear();
	int lang = GetCurrentLanguage();
	int f = out.Find(lang);
	if(f >= 0) {
		qtf = out[f];
		return;
	}
	out_seq = HelpTopicGetSeq();
	const HelpTopicInfoMap& map = HelpTopicMap();
	Progress progress(t_("Building style list"), map.GetCount());
	VectorMap< String, VectorMap<String, int> > style_map;
	String styles = HelpTopicGet(HelpDPPStyle()).GetText();
	int i;
	for(i = 0; i < map.GetCount(); i++) {
		String drl = map.GetKey(i);
		String space, nesting, topic;
		HelpParseDPP(drl, space, nesting, topic);
		if(nesting == "sys")
			continue;
		RichText rt = ParseQTF(styles + map[i].GetText());
		for(int p = 0; p < rt.GetPartCount(); p++)
			if(rt.IsPara(p))
				style_map.GetAdd(rt.GetStyle(rt.GetParaStyle(p)).name).GetAdd(topic, 0)++;
		if(progress.StepCanceled())
			break;
	}
	qtf << NFormat(t_("[A [a100 %d used styles:&\n]"), style_map.GetCount());
	for(i = 0; i < style_map.GetCount(); i++) {
		int total = Sum(style_map[i]);
		qtf << NFormat(t_("[* \1%s\1] - [/ %d occurrences]&\n"), style_map.GetKey(i), Sum(style_map[i]));
	}
	for(i = 0; i < style_map.GetCount(); i++) {
		qtf << NFormat(t_("[3*_b200a100 \1%s\1]&\n"), style_map.GetKey(i));
		Vector<String> topics = style_map[i].PickKeys();
		Vector<int> counts = style_map[i].PickValues();
		IndexSort(counts, topics, StdGreater<int>());
		for(int t = 0; t < topics.GetCount(); t++)
			qtf << NFormat(t_("[o \1%s\1: [/ %d x]&\n"), topics[t], counts[t]);
	}
	out.Add(lang, qtf);
}

RegisterHelpTopicSys("parastyles", t_("Used styles"), callback(&sEnumStyles));

static void sMakeIndex(String& qtf, bool book)
{
	qtf << "[{0} ";
	Array<HelpIndexFormattedEntry> index = HelpTopicFormatIndex();
	for(int i = 0; i < index.GetCount(); i++) {
		const HelpIndexFormattedEntry& entry = index[i];
		if(entry.nesting < 0)
			qtf << NFormat("[a100b200*5 %s&]", entry.text);
		else {
			qtf << "[l" << (100 * entry.nesting);
			if(book)
				qtf << ' ' << entry.text << " \r\n{:INDEX:" << DeQtf(entry.topics) << ":}\r\n";
			else {
				Vector<String> split = Split(entry.topics, ';');
				if(split.GetCount() == 1) {
					const char *sp = split[0];
					if(*sp == '!')
						sp++;
					qtf << '^' << sp << '^';
				}
				qtf << ' ' << entry.text;
				if(split.GetCount() > 1)
					for(int t = 0; t < split.GetCount(); t++) {
						qtf << (t ? ", " : " ") << '[';
						const char *sp = split[t];
						if(*sp == '!') {
							qtf << '*';
							sp++;
						}
						qtf << '^' << sp << "^ " << (t + 1) << ']';
					}
			}
			qtf << "&]\n";
		}
	}
}

static void sMakeIndexDyna(String& qtf) { sMakeIndex(qtf, false); }
static void sMakeIndexBook(String& qtf) { sMakeIndex(qtf, true); }

RegisterHelpTopicSys("index", t_("Index"), callback(&sMakeIndexDyna));
RegisterHelpTopicSys("index_book", "Rejstøík", callback(&sMakeIndexBook));

static void HelpTopicListDefault(String& out)
{
	Vector<String> ids;
	Vector<String> names;
	Vector<String> strip;
	const ArrayMap<String, HelpTopicInfo>& map = HelpTopicMap();
	int lang = GetCurrentLanguage();
	int i;
	for(i = 0; i < map.GetCount(); i++) {
		String topic = map.GetKey(i);
		if(*topic == '[')
			continue;
		const HelpTopicInfo& info = map[i];
//		if(map.GetKey(i) == "HelpTopicList" || !IsNull(info.GetText()))
		{
			String title = Nvl(info.GetTitle(), map.GetKey(i));
			names.Add(title);
			ids.Add(map.GetKey(i));
			strip.Add(MQTFStrip(title));
		}
	}
	Vector<int> order = GetSortOrder(strip, GetLanguageInfo());
	for(i = 1; i < order.GetCount(); i++)
		if(GetLanguageInfo().Compare(strip[order[i]], strip[order[i - 1]]) < 0) {
			RLOG("sort order error: " << strip[order[i - 1]] << " > " << strip[order[i]]);
		}

	RichText text;
	RichPara::CharFormat cformat;
	cformat.Face(Font::ARIAL).Height(180).Bold(); {
		RichPara para;
		para.Cat(WString("Automatický obsah"), cformat);
		para.format.after = 100;
		text.Cat(para);
	}

	cformat.Height(84).NoBold();
	for(i = 0; i < order.GetCount(); i++) {
		RichPara para;
		MQTFCat(para, names[order[i]], cformat);
		for(int p = 0; p < para.part.GetCount(); p++)
			para.part[p].format.link = ids[order[i]];
		text.Cat(para);
	}
	out.Cat(BodyAsQTF(text));
}

RegisterHelpTopicHost("topics_auto", CNULL, callback(&HelpTopicListDefault))

static void sBookExpand(String& qtf)
{
	RichText::FormatInfo empty;
	empty.charvalid = empty.paravalid = 0;
	RichText text = HelpTopicExpand(HelpHostDPP("book"), Null, empty);
	qtf = BodyAsQTF(text);
}

RegisterHelpTopicSys("book_expand", "", callback(&sBookExpand));

END_UPP_NAMESPACE
