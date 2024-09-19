#include "Browser.h"

#define LLOG(x)     // DLOG(x)
#define LTIMING(x)  // DTIMING(x)

#ifdef _DEBUG
#define LSLOW()     // Sleep(300)
#else
#define LSLOW()
#endif

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

String TppId(const String& ref, bool noscope)
{ // as clanglib tends to keep changing printing, let us make 'standard' id without qualified parameters
	const char *s = ref;
	StringBuffer r;
	while(*s) {
		if(*s == '(')
			noscope = true;
		if(iscib(*s)) {
			const char *id = s;
			while(iscib(*s))
				s++;
			while(*s == ' ')
				s++;
			if(s[0] == ':' && s[1] == ':' && noscope)
				s += 2;
			else
				r.Cat(id, s);
		}
		else
			r.Cat(*s++);
	}
	return String(r);
}

String CleanupTppId(const String& ref_)
{
	String ref = ref_;
	ref.TrimEnd("::struct"); // fix legacy format
	ref.TrimEnd("::class");
	ref.TrimEnd("::union");
	return TppId(CleanupId(ref));
}

void AddLinkRef(const String& link, const String& ref_)
{
	String ref = CleanupTppId(ref_);
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
					while(!IsLetter(*s) && !IsDigit(*s) && *s)
						s++;
					if(*s == '\0')
						break;
					StringBuffer sb;
					while(IsLetter(*s) || IsDigit(*s))
						sb.Cat(ToAscii(ToLower(*s++)));
					words.FindAdd(TopicWordIndex(sb));
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
	return CacheFile(ForceExt(Filter(path, NoSlashDot), ".tdx"));
}

const char *tdx_version = "tdx version 2.0";

void SyncTopicFile(const RichText& text, const String& link, const String& path, const String& title)
{
	LLOG("Scanning topic " << link);
	LTIMING("Scanning topic");
	
	ClearLinkRef(link);
	
	ScanTopicIterator sti;
	sti.link = link;
	text.Iterate(sti);
	
	TopicInfo& ti = topic_info().GetPut(link);
	ti.title = title;
	ti.path = path;
	ti.time = FileGetTime(path);
	ti.words = sti.words.PickKeys();
	
	FileOut out(TopicCacheName(path));
	out << tdx_version << "\n";
	out << title << '\n';
	for(int i = 0; i < sti.ref.GetCount(); i++)
		out << sti.ref[i] << '\n';
	out << '\n';
	const Index<String>& ws = TopicWords();
	for(int i = 0; i < ti.words.GetCount(); i++)
		out << ws[ti.words[i]] << '\n';
}

void SyncTopicFile(const String& link)
{
	String path = GetTopicPath(link);
	LLOG("SyncTopicFile " << link << " path: " << path);
	TopicInfo& ti = topic_info().GetPut(link);
	Time tm = FileGetTime(path);
	if(ti.path == ":ide:" || ti.path == path && ti.time == tm)
		return;
	String fn = TopicCacheName(path);
	if(FileGetTime(fn) > tm) {
		LLOG("Loading topic from cache");
		ClearLinkRef(link);
		FileIn in(fn);
		LTIMING("Loading topic from cache");
		if(in) {
			String s = in.GetLine();
			if(s == tdx_version) {
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
		}
	}
	Topic tp = ReadTopic(LoadFile(path));
	SyncTopicFile(ParseQTF(tp.text), link, path, tp.title);
}

void InvalidateTopicInfoPath(const String& path)
{
	VectorMap<String, TopicInfo>& t = topic_info();
	for(int i = 0; i < t.GetCount(); i++)
		if(!t.IsUnlinked(i) && t[i].path == path)
			t.Unlink(i);
}

bool SyncRefsFinished;
bool SyncRefsShowProgress;
int  SyncRefsRunning;

void SyncRefsDir(const char *dir, const String& rel, Progress& pi)
{
	SyncRefsRunning++;
	for(FindFile pff(AppendFileName(dir, "*.*")); pff && !IdeExit; pff.Next()) {
		if(pff.IsFolder() && *pff.GetName() != '.') {
			if(SyncRefsShowProgress)
				pi.Step();
			TopicLink tl;
			tl.package = rel + pff.GetName();
			String pdir = AppendFileName(dir, pff.GetName());
			for(FindFile ff(AppendFileName(pdir, "*.tpp")); ff && !IdeExit; ff.Next()) {
				if(ff.IsFolder()) {
					String group = GetFileTitle(ff.GetName());
					tl.group = group;
					String dir = AppendFileName(pdir, ff.GetName());
					LSLOW();
					for(FindFile ft(AppendFileName(dir, "*.tpp")); ft && !IdeExit; ft.Next()) {
						if(ft.IsFile()) {
							String path = AppendFileName(dir, ft.GetName());
							tl.topic = GetFileTitle(ft.GetName());
							String link = TopicLinkString(tl);
							if(SyncRefsShowProgress)
								pi.SetText("Indexing topic " + tl.topic);
							SyncTopicFile(link);
						}
						if(!SyncRefsFinished && !SyncRefsShowProgress && !IdeExit)
							Ctrl::ProcessEvents();
					}
				}
			}
			SyncRefsDir(pdir, tl.package + '/', pi);
		}
	}
	SyncRefsRunning--;
}

void SyncRefs()
{
	if(!SyncRefsFinished && SyncRefsRunning) {
		SyncRefsShowProgress = true;
		return;
	}
	Progress pi;
	pi.AlignText(ALIGN_LEFT);
	Vector<String> upp = GetUppDirs();
	for(int i = 0; i < upp.GetCount(); i++)
		SyncRefsDir(upp[i], String(), pi);
	SyncRefsFinished = true;
}

String LegacyRef(String ref)
{ // Fix links with legacy docs
	ref.TrimStart("Upp::");
	ref.Replace(" Upp::", " ");
	ref.Replace("(Upp::", "(");
	ref.Replace(",Upp::", ",");
	ref.Replace("<Upp::", "<");
	ref.Replace("const Upp::", "const ");
	return ref;
}

Vector<String> AnnotationCandidates(const String& ref)
{ // Make older refs compatible
	Index<String> l;
	l.FindAdd(ref);
	String lr = LegacyRef(ref);
	l.FindAdd(lr);
	int lvl = 0;
	StringBuffer h;
	for(const char *s = lr; *s; s++) { // remove template arguments - Vector<T> -> Vector
		if(*s == '<')
			lvl++;
		else
		if(*s == '>')
			lvl--;
		else
		if(lvl == 0)
			h.Cat(*s);
	}
	l.FindAdd(h);
	return l.PickKeys();
}

Vector<String> GetRefLinks(const String& ref)
{
	Index<String> l;
	for(String cr : AnnotationCandidates(ref)) {
		int q = ref_ref().Find(cr);
		while(q >= 0) {
			l.FindAdd(ref_link()[q]);
			q = ref_ref().FindNext(q);
		}
	};
	return l.PickKeys();
}

String GetTopicTitle(const String& link)
{
	SyncTopicFile(link);
	int q = topic_info().Find(link);
	return q >= 0 ? topic_info()[q].title : Null;
}

int MatchWord(const Vector<int>& w, const String& pattern)
{
	const Index<String>& ws = TopicWords();
	for(int i = 0; i < w.GetCount(); i++) {
		String wrd = ws[w[i]];
		if(wrd.GetCount() >= pattern.GetCount() && memcmp(wrd, pattern, pattern.GetCount()) == 0)
			return i;
	}
	return -1;
}

bool MatchTopicLink(const String& link, const Vector<String>& query)
{
	SyncTopicFile(link);
	int q = topic_info().Find(link);
	if(q < 0)
		return false;
	TopicInfo& f = topic_info()[q];
	for(int i = 0; i < query.GetCount(); i++)
		if(MatchWord(f.words, query[i]) < 0)
			return false;
	return true;
}
