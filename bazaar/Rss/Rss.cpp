//**********************************************************************************************
//* Rss Class
//* ---------
//* RSS 2.0 Web content syndication format encapsulation for U++
//* For more information on RSS format, 
//* See: 
//*		http://www.rss-specifications.com/
//*		http://www.rssboard.org/rss-specification		
//*
#include "Rss.h"

RssNode RssItem(const char* title, const char* link, const char* desc, const char* author, 
	const char* comments, const char* pubdate, RssTag* category, RssTag* guid, RssTag* source, RssTag* enclosure)
{
 	return RssNode(RSS_ITEM)
 		<< RssTag(RSS_TITLE, title)
 		<< RssTag(RSS_LINK, link)
 		<< RssTag(RSS_DESCRIPTION, desc)
		<< (author 		? RssTag(RSS_AUTHOR, author) : Null) 
 		<< (comments 	? RssTag(RSS_COMMENTS, comments) : Null)
 		<< (pubdate 	? RssTag(RSS_PUBDATE, pubdate) : Null)
 		<< (category	? *category : NULL)
 		<< (guid		? *guid : NULL)
 		<< (source		? *source : NULL)
 		<< (enclosure	? *enclosure : NULL);
}

RssNode RssImage(const char* title, const char* link, const char* url, const char* desc, Size sz)
{
	return RssNode(RSS_IMAGE)
		<< RssTag(RSS_TITLE, title)
		<< RssTag(RSS_LINK, link)
		<< RssTag(RSS_URL, url)
		<< (desc 		? RssTag(RSS_DESCRIPTION, desc) : Null)
		<< (sz.cx > 0 	? RssTag("width", AsString(sz.cx)) : Null)
		<< (sz.cy > 0 	? RssTag("height", AsString(sz.cy)) : Null);
}

RssNode RssTextInput(const char* title, const char* link, const char* desc, const char* name)
{
	return RssNode(RSS_TEXTINPUT)
		<< RssTag(RSS_TITLE, title)
		<< RssTag(RSS_LINK, link)
		<< RssTag(RSS_DESCRIPTION, desc)
		<< RssTag(RSS_NAME, name);
}

RssTag RssCloud(const char* domain, int port, const char* path, const char* registerProcedure, const char* protocol)
{
	return RssTag(RSS_CLOUD)
		("domain", domain)
		("port", port) 
		("path", path) 
		("registerProcedure", registerProcedure)
		("protocol", protocol);
}

RssTag RssCategory(const char* category, const char* domain)
{
	return RssTag(RSS_CATEGORY, category)
		("domain", domain);
}

RssTag RssGuid(const char *id, bool islink)
{
	return RssTag(RSS_GUID, id)
		("isPermaLink", (islink ? "true" : "false"));
}

RssTag RssSource(const char* url, const char* src)
{
	return RssTag(RSS_SOURCE, src)
		(RSS_URL, url);
}

RssTag RssEnclosure(const char* url, double length, const char* type)
{
	return RssTag(RSS_ENCLOSURE)
		("url", url)
		("length", length)
		("type", type);
}

void RssTag::Copy(const RssTag& src)
{
	tag0 = src.tag0;
	txt0 = src.txt0;
	attrs <<= src.attrs;
}

void RssNode::Copy(const RssNode& src) 
{
	RssTag::operator=(src);
	tags <<= src.tags;
	nodes <<= src.nodes;
}

void RssChannel::Copy(const RssChannel& src)
{
	tags <<= src.tags;
	nodes <<= src.nodes;
	skipdays <<= src.skipdays;
	skiphours <<= src.skiphours;
}

void Rss::Copy(const Rss& src)
{
	channels <<= src.channels;
	charset = src.charset;
	version = src.version;
	error	= src.error;
}

RssTag RssChannel::GetTag(VectorMap<String, RssTag>& t, const char* tag)
{
	RssTag tg;
	return tg = t.Get(tag, Null);
}

RssNode RssChannel::GetNode(VectorMap<String, RssNode>& n, const char* tag)
{
	RssNode nd;
	return nd = n.Get(tag, Null);
}

RssChannel& RssChannel::Tag(const RssTag& t)
{
	
	int n = tags.Find(t.Tag());
	if(n >= 0 && 
		t.Tag() == RSS_SKIPDAYS || 
		t.Tag() == RSS_SKIPHOURS) 
			tags.Get(t.Tag()) = t;
	else
		tags.Add(t.Tag(), t);
	return *this;	
}

RssChannel& RssChannel::Node(const RssNode& t)
{
	int n = nodes.Find(t.Tag());
	if(n >= 0 && 
		t.Tag() == RSS_SKIPDAYS || 
		t.Tag() == RSS_SKIPHOURS) 
			nodes.Get(t.Tag()) = t;
	else
		nodes.Add(t.Tag(), t);
	return *this;	
}

RssChannel& RssChannel::SkipDays(const char* d)
{
	int n = skipdays.GetCount();
	if(n < 7)	{
		if(n > 0) {
		for(int i = 0; i < n; i++)
			if(skipdays.At(i) == d)
				return *this;
		}
		skipdays.Add(d);
	}
	return *this;
}

RssChannel& RssChannel::SkipHours(int h)
{
	int n = skiphours.GetCount();
	if(n <  24 && (h < 25 && h > 0)) {
		if(n > 0) {
			for(int i = 0; i < n; i++)
				if(skiphours.At(i) == h)
					return *this;
		}
		skiphours.Add(h);
	}
	return *this;
}

//**********************************************************************************************
//* Rss class implementation
//* 

bool Rss::Compose(String& out)
{
	try {
		String feed, channels, nodes, items;
		if(IsNull(Version()))
			Version("2.0");
		if(IsNull(Encoding()))
		   	Encoding(CHARSET_DEFAULT);
		if(!HasChannel())
			throw XmlError(t_("No channel node."));
		RssChannels &chs = Channels();
		for(int i = 0; i < chs.GetCount(); i++) {
			if(!chs[i].HasItem())
				throw XmlError(Format(t_("Channel '%d' has not items"), i));
			items = MakeTags(chs[i].Tags());
			nodes = MakeNodes(chs[i].Nodes());
			Vector<String>& skipdays = chs[i].SkipDays();
			Vector<int>& skiphours = chs[i].SkipHours();
			for(int sdc = 0; sdc < skipdays.GetCount(); sdc++)
				items << XmlTag(RSS_SKIPDAYS)
					.Text(skipdays[sdc],
					CharsetByName(Encoding()));
			for(int shc = 0; shc < skiphours.GetCount(); shc++)
				items << XmlTag(RSS_SKIPHOURS)
					.Text(AsString(skiphours[shc]), 
					CharsetByName(Encoding())); 
			XmlTag ch = XmlTag();
			channels << ch.Tag(RSS_CHANNEL)(items + nodes);
		}
		out << XmlHeader(Encoding());
		out << XmlTag(RSS_HEADER)(RSS_VERSION, Version())(channels);
		
	}
	catch(XmlError e) {
		error = e;
		return false;
	}
	return true;
}

bool Rss::Parse(String& in)
{

	Clear();
	try {
		XmlNode n = ParseXML(in);
		if(n.GetCount() == 0 || n[0].GetTag() != RSS_HEADER)
			throw XmlError(t_("<Rss> node not found."));

		const XmlNode& rssnode = n[0];
		const char* ver = "<?xml version=\""; const char* enc = "encoding=\"";
		const  char* buf = in;
		String xmlver, encoding;
		int ver0 = in.Find(ver);
		int enc0 = in.Find(enc);
		if(ver0 >= 0)
			for(int i = ver0 + strlen(ver); i; i++) {
				if(buf[i] == '\"' || buf[i] == '>')
					break;
				else
					xmlver.Cat(buf[i]);															// TODO: add Xml version var.
			}
		if(enc0 >= 0)
			for(int i = enc0 + strlen(enc); i; i++) {
				if(buf[i] == '\"' || buf[i] == '>')
					break;
				else
					encoding.Cat(buf[i]);
			}
		if(!IsNull(encoding))
			charset = encoding;
		version = rssnode.Attr(RSS_VERSION);
		
		for(int i = 0; i < rssnode.GetCount(); i++) {
			const XmlNode& channelnode = rssnode[i];
			if(channelnode.IsTag(RSS_CHANNEL)) {
				RssChannel& channel = channels.Add();
				for(int cc = 0; cc < channelnode.GetCount(); cc++) {
					const XmlNode& ch = channelnode[cc];
					if(ch.IsTag(RSS_SKIPDAYS))
					   channel.SkipDays(ch[0].GetText());
					else
					if(ch.IsTag(RSS_SKIPHOURS))
					   channel.SkipHours(StrInt(ch[0].GetText()));
					else
					if(ch.IsTag(RSS_CATEGORY)) 
						channel.Category(RssCategory(ch[0].GetText(), ch.Attr("domain")));
					else
					if(ch.IsTag(RSS_CLOUD)) 
						channel.Cloud(RssCloud(ch.Attr("domain"),
							StrInt(ch.Attr("port")), 
							ch.Attr("path"), 
							ch.Attr("registerProcedure"),
							ch.Attr("protocol"))
						);					
					else
					if(ch.IsTag(RSS_IMAGE)) {
						RssNode imgnode;
						imgnode.Tag(RSS_IMAGE);
						for(int iic = 0; iic < ch.GetCount(); iic++) {
							const XmlNode& img = ch[iic];
							imgnode << RssTag(img.GetTag(), img[0].GetText());                  
						}
						channel.Image(imgnode);
					}
					else
					if(ch.IsTag(RSS_TEXTINPUT)) {
						RssNode tinode;
						tinode.Tag(RSS_TEXTINPUT);
						for(int tic = 0; tic < ch.GetCount(); tic++) {
							const XmlNode& tex = ch[tic];
							tinode << RssTag(tex.GetTag(), tex[0].GetText());
						}
						channel.TextInput(tinode);
					}
					else
					if(ch.IsTag(RSS_ITEM)) {
						RssNode inode;
						inode.Tag(RSS_ITEM);
						for(int itc = 0; itc < ch.GetCount(); itc++) {
							const XmlNode& itm = ch[itc];
							if(itm.IsTag(RSS_GUID))
								inode << RssTag(itm.GetTag(), itm[0].GetText())
									("isPermaLink", itm.Attr("isPermaLink"));
							else
							if(itm.IsTag(RSS_CATEGORY))
								inode << RssCategory(itm[0].GetText(), itm.Attr("domain"));
							else
							if(itm.IsTag(RSS_SOURCE))
								inode << RssSource(itm.Attr("url"), itm[0].GetText());
							else
							if(itm.IsTag(RSS_ENCLOSURE))
								inode << RssEnclosure(itm.Attr("url"), ::StrDbl(itm.Attr("length")), itm.Attr("type"));
							else
								inode << RssTag(itm.GetTag(), itm[0].GetText());									
						}
						channel.Item(inode);
					}
					else
						channel.Tag(RssTag(ch.GetTag(), ch[0].GetText()));
				}
			}
			else
				throw XmlError(t_("<Channel> node not found. Faulty Rss structure."));
		}
	}
	catch(XmlError e) {
		error = e;
		return false;
	}

return true;
}

String Rss::MakeTags(RssTags& tags)
{
	String out;

	for(int tc = 0; tc < tags.GetCount(); tc++) {				
		RssTag& t = tags[tc];
		XmlTag  x;
		if(t.HasAttr()) {
			RssAttributes& attrs = t.Attrs();
			for(int ac = 0; ac < attrs.GetCount(); ac++) {
				const char* attr = attrs.GetKey(ac);
				const char *val	= attrs[ac];
				 x(attr, val);
			}
		}
		out << x.Tag(t.Tag()).Text(t.Text(), CharsetByName(Encoding())); 
	}
	return out;	
}

String Rss::MakeNodes(RssNodes& nodes)
{
	String out, items;
	for(int nc = 0; nc < nodes.GetCount(); nc++) {
		RssNode& n = nodes[nc];
		XmlTag x;
		items = Null;
		if(n.HasTag())
			items << MakeTags(n.Tags());
		if(n.HasNode())
			items << MakeNodes(n.Nodes());
		out << x.Tag(n.Tag())(items); 
	}
	return out;
}

Rss& Rss::Channel(const RssChannel& channel)
{
	for(int i = 0; i < ChannelCount(); i++) {
		RssChannel& ch = channels.At(i);
		if(&ch == &channel)
			return *this;
	}
	channels.Add(channel);
	return *this;
}
