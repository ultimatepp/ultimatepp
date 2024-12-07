#include "www.h"

#define LLOG(x) // LOG(x)

StaticCriticalSection     reflink_lock;
VectorMap<String, String> reflink;

int GetMaxDocLevel();

struct ScanTopicIterator : RichText::Iterator {
	String         link;
	String         prefix;

	virtual bool operator()(int pos, const RichPara& para)
	{
		if(!IsNull(para.format.label)) {
			LLOG("label: " << para.format.label);
			INTERLOCKED_(reflink_lock)
				reflink.Add(prefix + para.format.label, link);
		}
		return false;
	}
};

static void sDoFile(const char *path, const char *link, bool dev)
{
	ScanTopicIterator sti;
	sti.link = link;
	ParseQTF(ReadTopic(LoadFile(path))).Iterate(sti);
}

String TopicLinkString(TopicLink tl, bool dev)
{
	if(dev)
		tl.topic << "$dev";
	return TopicLinkString(tl);
}

void GatherRefLinks(const char *upp, bool dev)
{
	CoWork work;
	for(FindFile pff(AppendFileName(upp, "*.*")); pff; pff.Next()) {
		if(pff.IsFolder()) {
			String package = pff.GetName();
			String pdir = AppendFileName(upp, package);
			TopicLink tl;
			tl.package = package;
			for(FindFile ff(AppendFileName(pdir, "*.tpp")); ff; ff.Next()) {
				if(ff.IsFolder()) {
					String group = GetFileTitle(ff.GetName());
					if(group == "src.tpp") { // only translate links for src.tpp
						tl.group = group;
						String dir = AppendFileName(pdir, ff.GetName());
						for(FindFile ft(AppendFileName(dir, "*.tpp")); ft; ft.Next()) {
							if(ft.IsFile()) {
								String path = AppendFileName(dir, ft.GetName());
								tl.topic = GetFileTitle(ft.GetName());
								String link = TopicLinkString(tl, dev);
								work & callback3(sDoFile, path, link, dev);
								ScanTopicIterator sti;
								sti.link = link;
								LLOG("Indexing topic " << path << " link: " << link);
								ParseQTF(ReadTopic(LoadFile(path))).Iterate(sti);
							}
						}
					}
				}
			}
		}
	}
}

struct GatherLinkIterator : RichText::Iterator {
	Index<String> link;

	virtual bool operator()(int pos, const RichPara& para)
	{
		for(int i = 0; i < para.GetCount(); i++) {
			String l = para[i].format.link;
			if(!IsNull(l)) {
				LLOG("GatherLink " << l);
				if(l[0] == ':') {
					int q = reflink.Find(l);
					if(q < 0)
						q = reflink.Find(l + "::class");
					if(q < 0)
						q = reflink.Find(l + "::struct");
					if(q < 0)
						q = reflink.Find(l + "::union");
					if(q >= 0)
						l = reflink[q];
				}
				int q = l.Find('#');
				if(q >= 0)
					l.Trim(q);
				link.FindAdd(Nvl(reflink.Get(l, Null), l));
			}
		}
		return false;
	}
};

String TopicFileName(String dir, String topic)
{
	TopicLink tl = ParseTopicLink(topic);
	return AppendFileName(dir, AppendFileName(tl.package, AppendFileName(tl.group + ".tpp", tl.topic + ".tpp")));
}

String TopicFileName2(String topic, bool dev)
{
	int q = topic.ReverseFind('$');
	if(q >= 0)
		topic.Set(q, '_');
	String p = TopicFileName(uppbox, topic);
	if(FileExists(p))
		return p;
	return TopicFileName(dev ? devsrc : uppsrc, topic);
}

String TopicFileNameHtml(const char *topic, bool dev)
{
	TopicLink tl = ParseTopicLink(topic);
	tl.package.Replace("/","$");
	tl.package.Replace("\\","$");
	if(dev)
		return tl.group + "$" + tl.package+ "$" + tl.topic + "$dev.html";
	return tl.group + "$" + tl.package+ "$" + tl.topic + ".html";
}

static void sGatherTopics(ArrayMap<String, Topic> *map, Vector<String> *ttFullIds, const char *topic, String parentIds, bool dev)
{
	GatherTopics(*map, *ttFullIds, topic, parentIds, dev);
}
	
String ChangeTopicLanguage(const String &topic, int lang);
String GetTopicLanguage(const String &topic);

String GatherTopics(ArrayMap<String, Topic>& tt, Vector<String>& ttFullIds, const char *topic, String& title, String parentIds, bool dev)
{
	static StaticCriticalSection mapl;
	LLOG("Gather topics: " << topic);
	int q;
	INTERLOCKED_(mapl)
		q = tt.Find(topic);
	if(q < 0) {
		LLOG("GatherTopics " << topic);
		Topic p = ReadTopic(LoadFile(TopicFileName2(topic, dev)));
		title = p.title;
				
		String t = p;
		if(IsNull(t)) {
			String topicEng = ChangeTopicLanguage(topic, LNG_('E','N','U','S'));
			p = ReadTopic(LoadFile(TopicFileName2(topicEng, dev)));
			String tt = p;
			if(IsNull(tt))
				return "index.html";
			title = p.title;
			p.title += " (translated)";
			String help = "topic://uppweb/www/contribweb$" + GetTopicLanguage(topic);
			p.text = String("{{1f1t0/50b0/50@(240.240.240) [<A2 ") + t_("This page has not been translated yet") +
					"]. " + "[^" + help + "^ [<A2 " + t_("Do you want to translate it?") + "]]}}&&" + p.text;
		}
		String newParentIds;
		INTERLOCKED_(mapl)
		{
			tt.Add(topic) = p;
			q = tt.GetCount() - 1;
			newParentIds = parentIds + "/" + FormatInt(q);
			ttFullIds.Add(newParentIds);
		}
		GatherLinkIterator ti;
		ParseQTF(t).Iterate(ti);
/*#ifdef MTC
		CoWork work;
		for(int i = 0; i < ti.link.GetCount(); i++)
			work & callback2(sGatherTopics, &tt, ti.link[i]);
#else*/
		for(int i = 0; i < ti.link.GetCount(); i++)
			sGatherTopics(&tt, &ttFullIds, ti.link[i], newParentIds, dev);
//#endif
	} else {
		INTERLOCKED_(mapl)
			title = tt[q].title;
			ttFullIds.Add(parentIds + "/" + FormatInt(q));
	}
	return TopicFileNameHtml(topic, dev);
}

String GatherTopics(ArrayMap<String, Topic>& map, Vector<String>& ttFullIds, const char *topic, String parentIds, bool dev)
{
	String dummy;
	return GatherTopics(map, ttFullIds, topic, dummy, parentIds, dev);
}
