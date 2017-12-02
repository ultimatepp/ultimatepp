#include "Core.h"

namespace Upp {

struct TopicData__ : Moveable<TopicData__> {
	String      title;
	const byte *data;
	int         len;
};

VectorMap<String, VectorMap<String, VectorMap<String, TopicData__ > > >& Topics__()
{
	static VectorMap<String, VectorMap<String, VectorMap<String, TopicData__ > > > x;
	return x;
}

VectorMap<String, VectorMap<String, Vector<String> > >& TopicBase()
{
	static VectorMap<String, VectorMap<String, Vector<String> > > tb;
	return tb;
}

String TopicLinkString(const TopicLink& tl)
{
	String s;
	s << "topic://" << tl.package << '/' << tl.group << '/' << tl.topic;
	if(tl.label.GetCount())
		s << '#' << tl.label;
	return s;
}

TopicLink ParseTopicLink(const char *link)
{
	TopicLink tl;
	const char *end = link + strlen(link);
	const char *lbl = strchr(link, '#');
	if(lbl) {
		end = lbl;
		tl.label = lbl + 1;
	}
	if(memcmp(link, "topic://", 8) == 0)
		link += 8;
	do {
		if(!IsNull(tl.package))
			tl.package << '/';
		tl.package.Cat(tl.group);
		tl.group = tl.topic;
		const char *b = link;
		while(link < end && *link != '/')
			link++;
		tl.topic = String(b, link);
	}
	while(link++ < end);
	return tl;
}

static StaticMutex sTopicLock;

Topic GetTopic(const String& package, const String& group, const String& topic_)
{
	Mutex::Lock __(sTopicLock);
	VectorMap<String, VectorMap<String, TopicData__> > *p = Topics__().FindPtr(package);
	if(p) {
		VectorMap<String, TopicData__> *g = p->FindPtr(group);
		if(g) {
			String topic = topic_;
			for(int pass = 0; pass < 2; pass++) {
				const TopicData__ *d = g->FindPtr(topic);
				if(d) {
					Topic t;
					t.title = d->title;
					t.text = ZDecompress(d->data, d->len);
					return t;
				}
				
				int q = topic.ReverseFind('$'); // we have change lang separator from $ to _, keep compatibility
				if(q >= 0)
					topic.Set(q, '_');
				else
				if((q = topic.ReverseFind('_')) >= 0)
					topic.Set(q, '$');
				else
					break;
			}
		}
	}
	return Topic();
}

Topic GetTopic(const char *path)
{
	TopicLink tl = ParseTopicLink(path);
	Topic t = GetTopic(tl.package, tl.group, tl.topic);
	t.label = tl.label;
	tl.label.Clear();
	t.link = TopicLinkString(tl);
	return t;
}

Topic GetTopicLNG(const char *path)
{
	return GetTopic(path +
	                ("$" + ToLower(LNGAsText(SetLNGCharset(GetCurrentLanguage(), CHARSET_DEFAULT)))));
}

}

void RegisterTopic__(const char *topicfile, const char *topic, const char *title,
                     const UPP::byte *data, int len)
{
	Upp::Mutex::Lock __(Upp::sTopicLock);
	ASSERT(*topicfile == '<');
	ASSERT(*UPP::GetFileName(topicfile).Last() == '>');
	UPP::String q = UPP::GetFileFolder(topicfile + 1);
	UPP::String group = UPP::GetFileTitle(q);
	UPP::String package = UPP::UnixPath(UPP::GetFileFolder(q));
	UPP::TopicData__& d = UPP::Topics__().GetAdd(package).GetAdd(group).GetAdd(topic);
	d.title = title;
	d.data = data;
	d.len = len;
	UPP::TopicBase().GetAdd(package).GetAdd(group).Add(topic);
}
