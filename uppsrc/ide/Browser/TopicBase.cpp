#include "Browser.h"

Index<String>& ref_link()
{
	static Index<String> x;
	return x;
}

Index<String>& ref_ref()
{
	static Index<String> x;
	return x;
}

Index<String>& TopicWords()
{
	static Index<String> x;
	return x;
}

int TopicWordIndex(const String& w)
{
	return TopicWords().FindAdd(w);
}

String TopicIndexWord(int i)
{
	return TopicWords()[i];
}

VectorMap<String, TopicInfo>& topic_info()
{
	static VectorMap<String, TopicInfo> x;
	return x;
}

void AddLinkRef(const String& link, const String& ref)
{
	int q = ref_link().Put(link);
	if(q < ref_ref().GetCount())
		ref_ref().Set(q, ref);
	else
		ref_ref().Add(ref);
}

void ClearLinkRef(const String& link)
{
	int q = ref_link().Find(link);
	while(q >= 0) {
		int w = q;
		q = ref_link().FindNext(q);
		ref_link().Unlink(w);
	}
}

struct ScanTopicIterator : RichText::Iterator {
	String         link;
	Index<int>     words;
	Index<String>  ref;

	virtual bool operator()(int pos, const RichPara& para)
	{
		if(!IsNull(para.format.label)) {
			AddLinkRef(link, para.format.label);
			ref.FindAdd(para.format.label);
		}
		
		for(int i = 0; i < para.part.GetCount(); i++)
			if(para.part[i].IsText()) {
				const wchar *s = para.part[i].text;
				for(;;) {
					while(!IsLetter(*s) && *s)
						s++;
					if(*s == '\0')
						break;
					WStringBuffer wb;
					while(IsLetter(*s))
						wb.Cat(ToLower(*s++));
					words.FindAdd(TopicWordIndex(FromUnicode(wb)));
				}
			}
		return false;
	}
};

int NoSlashDot(int c)
{
	return c == '/' || c == '\\' || c == ':' ? '.' : c;
}

String TopicCacheName(const char *path)
{
	String cfg = ConfigFile("cfg");
	RealizeDirectory(cfg);
	return AppendFileName(cfg, ForceExt(Filter(path, NoSlashDot), ".tdx"));
}

void SyncTopicFile(const RichText& text, const String& link, const String& path, const String& title)
{
	ClearLinkRef(link);
	
	ScanTopicIterator sti;
	sti.link = link;
	text.Iterate(sti);
	
	TopicInfo& ti = topic_info().GetAdd(link);
	ti.title = title;
	ti.path = path;
	ti.time = FileGetTime(path);
	ti.words = sti.words.PickKeys();
	Sort(ti.words);
	
	FileOut out(TopicCacheName(path));
	out << title << '\n';
	for(int i = 0; i < sti.ref.GetCount(); i++)
		out << sti.ref[i] << '\n';
	out << '\n';
	for(int i = 0; i < ti.words.GetCount(); i++)
		out << TopicIndexWord(ti.words[i]) << '\n';
}

void SyncTopicFile(const String& link, const String& path)
{
	TopicInfo& ti = topic_info().GetAdd(link);
	Time tm = FileGetTime(path);
	if(ti.path == ":ide:" || ti.path == path && ti.time == tm)
		return;
	String fn = TopicCacheName(path);
	if(FileGetTime(fn) > tm) {
		ClearLinkRef(link);
		FileIn in(fn);
		ti.title = in.GetLine();
		ti.words.Clear();
		ti.path = path;
		ti.time = tm;
		while(!in.IsEof()) {
			String x = in.GetLine();
			if(IsNull(x))
				break;
			AddLinkRef(link, x);
		}
		while(!in.IsEof()) {
			String x = in.GetLine();
			if(IsNull(x))
				break;
			ti.words.Add(TopicWordIndex(x));
		}
		Sort(ti.words);
		return;
	}
	Topic tp = ReadTopic(LoadFile(path));
	SyncTopicFile(ParseQTF(tp.text), link, path, tp.title);
}

void SyncRefs()
{
	Progress pi;
	pi.AlignText(ALIGN_LEFT);
	Vector<String> upp = GetUppDirs();
	for(int i = 0; i < upp.GetCount(); i++) {
		for(FindFile pff(AppendFileName(upp[i], "*.*")); pff; pff.Next()) {
			if(pff.IsFolder()) {
				pi.Step();
				String package = pff.GetName();
				String pdir = PackageDirectory(package);
				TopicLink tl;
				tl.package = package;
				for(FindFile ff(AppendFileName(pdir, "*.tpp")); ff; ff.Next()) {
					if(ff.IsFolder()) {
						String group = GetFileTitle(ff.GetName());
						tl.group = group;
						String dir = AppendFileName(pdir, ff.GetName());
						for(FindFile ft(AppendFileName(dir, "*.tpp")); ft; ft.Next()) {
							if(ft.IsFile()) {
								String path = AppendFileName(dir, ft.GetName());
								tl.topic = GetFileTitle(ft.GetName());
								String link = TopicLinkString(tl);
								pi.SetText("Indexing topic " + tl.topic);
								SyncTopicFile(link, path);
							}
						}
					}
				}
			}
		}
	}
}

Vector<String> GetRefLinks(const String& ref)
{
	Vector<String> l;
	int q = ref_ref().Find(ref);
	while(q >= 0) {
		l.Add(ref_link()[q]);
		q = ref_ref().FindNext(q);
	}
	return l;
}

String GetTopicTitle(const String& link)
{
	int q = topic_info().Find(link);
	return q >= 0 ? topic_info()[q].title : Null;
}

bool MatchTopicLink(const String& link, const Vector<int>& query)
{
	int q = topic_info().Find(link);
	if(q < 0)
		return false;
	TopicInfo& f = topic_info()[q];
	for(int i = 0; i < query.GetCount(); i++)
		if(FindIndex(f.words, query[i]) < 0)
			return false;
	return true;
}
