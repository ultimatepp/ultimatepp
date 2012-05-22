#include "ide.h"

#define LLOG(x) // DLOG(x)

void Ide::ResolveUvsConflict() {
	String result;
	editor.NextUndo();
	bool copy = true;
	for(int i = 0; i < editor.GetLineCount(); i++) {
		String ln = editor.GetUtf8Line(i);
		if(strncmp(ln, "$uvs: ", 6) == 0) {
			ln = ln.Mid(6);
			if(ln == "YOUR DELETE")
				copy = false;
			else
			if(ln == "END YOUR DELETE")
				copy = true;
			else
			if(ln == "REPOSITORY DELETE")
				copy = false;
			else
			if(ln == "END REPOSITORY DELETE")
				copy = true;
			else
			if(ln != "REPOSITORY INSERT" &&
			   ln != "YOUR INSERT" &&
			   ln != "END YOUR INSERT" &&
			   ln != "END REPOSITORY INSERT" &&
			   ln != "PENDING CONFLICT") {
				Exclamation("Cannot resolve uvs conflict -&conflicting modifications found");
				editor.SetCursor(editor.GetPos(i));
				return;
			}
		}
		else
		if(copy)
			result << ln << "\r\n";
	}
	editor.SelectAll();
	editor.Paste(result.ToWString());
}

void Ide::GotoPos(String path, int line)
{
	LLOG("GotoPos " << path << ':' << line);
	AddHistory();
	editastext.FindAdd(path);
	EditFile(path);
	editor.SetCursor(editor.GetPos(line - 1));
	editor.TopCursor(4);
	editor.SetFocus();
	AddHistory();
}

void Ide::GotoCpp(const CppItem& pos)
{
	GotoPos(GetCppFile(pos.file), pos.line);
}

void Ide::RescanCode()
{
/*
	TimeStop tm;
	for(int i = 0; i < 10; i++)
		ReQualifyCodeBase();
	LOG(tm);
	PutConsole(AsString(tm));
//*/
//*
	SaveFile();
	TimeStop t;
	console.Clear();
	RescanCodeBase();
	SyncRefs();
	editor.SyncNavigator();
//*/
}

void Ide::OpenTopic(const String& topic, const String& createafter, bool before)
{
	TopicLink tl = ParseTopicLink(topic);
	if(tl) {
		EditFile(AppendFileName(PackageDirectory(tl.package), tl.group + ".tpp"));
		if(designer) {
			TopicEditor *te = dynamic_cast<TopicEditor *>(&designer->DesignerCtrl());
			if(te)
				te->GoTo(tl.topic, tl.label, createafter, before);
		}
	}
}

void Ide::OpenTopic(const String& topic)
{
	OpenTopic(topic, String(), false);
}

void Ide::OpenATopic()
{
	String t = doc.GetCurrent();
	if(!t.StartsWith("topic:"))
		return;
	OpenTopic(t);
}

void Ide::IdeFlushFile()
{
	FlushFile();
}

void Ide::IdeOpenTopicFile(const String& file)
{
	EditFile(GetFileFolder(file));
	if(designer) {
		TopicEditor *te = dynamic_cast<TopicEditor *>(&designer->DesignerCtrl());
		if(te)
			te->GoTo(GetFileTitle(file), "", "", false);
	}
}

struct FileStat {
	int  count;
	int  len;
	int  lines;
	int  oldest;
	int  newest;
	int  days;

	void Add(const FileStat& a) {
		count += a.count;
		len += a.len;
		lines += a.lines;
		oldest = max(a.oldest, oldest);
		newest = min(a.newest, newest);
		days += a.days;
	}

	FileStat() { count = 0; len = lines = 0; oldest = 0; newest = INT_MAX; days = 0; }
};

String StatLen(int len)
{
	return Format("%d.%d KB", len >> 10, (len & 1023) / 103);
}

String StatDate(int d)
{
	return String().Cat() << d << " days";
}

void Put(const String& name, String& qtf, const FileStat& fs)
{
	qtf << "::@W " << DeQtf(Nvl(name, ".<none>"))
	    << ":: [> " << fs.count
	    << ":: " << fs.lines
	    << ":: " << (fs.count ? fs.lines / fs.count : 0)
	    << ":: " << StatLen(fs.len)
	    << ":: " << StatLen(fs.len ? fs.len / fs.count : 0)
	    << ":: " << StatDate(fs.oldest)
	    << ":: " << StatDate(fs.newest)
	    << ":: " << (fs.count ? fs.days / fs.count : 0) << " days]";
}

void Put(String& qtf, ArrayMap<String, FileStat>& pfs, ArrayMap<String, FileStat>& all) {
	FileStat pall;
	for(int i = 0; i < pfs.GetCount(); i++) {
		FileStat& fs = pfs[i];
		Put(pfs.GetKey(i), qtf, fs);
		pall.Add(fs);
		all.GetAdd(pfs.GetKey(i)).Add(fs);
	}
	Put("All files", qtf, pall);
	qtf << "}}&&";
}


void Ide::Statistics()
{
	Vector< ArrayMap<String, FileStat> > stat;
	Progress pi;
	const Workspace& wspc = IdeWorkspace();
	pi.SetTotal(wspc.GetCount());
	Date now = GetSysDate();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pk = wspc.GetPackage(i);
		String n = wspc[i];
		pi.SetText(n);
		if(pi.StepCanceled()) return;
		ArrayMap<String, FileStat>& pfs = stat.Add();
		for(int i = 0; i < pk.GetCount(); i++)
			if(!pk[i].separator) {
				String file = SourcePath(n, pk[i]);
				if(FileExists(file)) {
					FileStat& fs = pfs.GetAdd(GetFileExt(file));
					int d = minmax(now - FileGetTime(file), 0, 9999);
					fs.oldest = max(d, fs.oldest);
					fs.newest = min(d, fs.newest);
					String data = LoadFile(file);
					for(const char *s = data; *s; s++)
						if(*s == '\n')
							fs.lines++;
					fs.len += data.GetCount();
					fs.days += d;
					fs.count++;
				}
			}
	}
	String qtf = "[1 ";
	ArrayMap<String, FileStat> all;
	String tab = "{{45:20:25:20:35:30:30:30:30@L [* ";
	String hdr = "]:: [= Files:: Lines:: - avg.:: Length:: - avg.:: Oldest:: Newest:: Avg. age]";
	for(int i = 0; i < wspc.GetCount(); i++) {
		qtf << tab << DeQtf(wspc[i]) << hdr;
		Put(qtf, stat[i], all);
	}

	qtf << tab << "All packages" << hdr;
	Put(qtf, all, all);

	WithStatLayout<TopWindow> dlg;
	CtrlLayoutOK(dlg, "Statistics");
	dlg.stat = qtf;
	dlg.Sizeable().Zoomable();
	dlg.Run();
}

String FormatElapsedTime(double run)
{
	String rtime;
	double hrs = floor(run / 3600);
	if(hrs > 0)
		rtime << NFormat("%0n hours, ", hrs);
	int minsec = fround(run - 3600 * hrs);
	int min = minsec / 60, sec = minsec % 60;
	if(min || hrs)
		rtime << NFormat("%d min, ", min);
	rtime << NFormat("%d sec", sec);
	return rtime;
}

void Ide::AlterText(WString (*op)(const WString& in))
{
	if(designer)
		return;
	editor.NextUndo();
	if(!editor.IsSelection())
		editor.SelectAll();
	WString w = editor.GetSelectionW();
	editor.RemoveSelection();
	editor.Paste((*op)(w));
}

void Ide::TextToUpper()
{
	AlterText(UPP::ToUpper);
}

void Ide::TextToLower()
{
	AlterText(UPP::ToLower);
}

void Ide::TextToAscii()
{
	AlterText(UPP::ToAscii);
}

void Ide::TextInitCaps()
{
	AlterText(UPP::InitCaps);
}

static WString sSwapCase(const WString& s)
{
	WStringBuffer r;
	r.SetCount(s.GetCount());
	for(int i = 0; i < s.GetCount(); i++)
		r[i] = IsUpper(s[i]) ? ToLower(s[i]) : ToUpper(s[i]);
	return r;
}

void Ide::SwapCase()
{
	AlterText(sSwapCase);
}

static WString sCString(const WString& s)
{
	return AsCString(s.ToString()).ToWString();
}

void Ide::ToCString()
{
	AlterText(sCString);
}

void Ide::Times()
{
	WithStatisticsLayout<TopWindow> statdlg;
	CtrlLayout(statdlg, "Elapsed times");
	statdlg.SetTimeCallback(-1000, statdlg.Breaker(IDRETRY), 50);
	do
	{
		int session_time = int(GetSysTime() - start_time);
		int idle_time = int(session_time - editor.GetStatEditTime() - stat_build_time);
		statdlg.session_time <<= FormatElapsedTime(session_time);
		statdlg.edit_time    <<= FormatElapsedTime(editor.GetStatEditTime());
		statdlg.build_time   <<= FormatElapsedTime(stat_build_time);
		statdlg.idle_time    <<= FormatElapsedTime(idle_time);
	}
	while(statdlg.Run() == IDRETRY);
}

Vector<String> Ide::SvnDirs()
{
	Vector<String> d = GetUppDirs();
	Vector<String> r;
	for(int i = 0; i < d.GetCount(); i++)
		if(IsSvnDir(d[i]))
			r.Add(d[i]);
	return r;
}

INITBLOCK {
	RegisterGlobalConfig("svn-msgs");
}

void SvnSyncDirs(const Vector<String>& working)
{
	if(!CheckSvn())
		return;
	SvnSync svn;
	String msgs;
	LoadFromGlobal(msgs, "svn-msgs");
	svn.SetMsgs(msgs);
	for(int i = 0; i < working.GetCount(); i++)
		svn.Dir(working[i]);
	svn.DoSync();
	msgs = svn.GetMsgs();
	StoreToGlobal(msgs, "svn-msgs");
}

void Ide::SyncSvnDirs(const Vector<String>& working)
{
	SaveFile();
	SvnSyncDirs(working);
}

void Ide::SyncSvn()
{
	SyncSvnDirs(SvnDirs());
}

void Ide::SyncSvnDir(const String& working)
{
	SyncSvnDirs(Vector<String>() << working);
}
