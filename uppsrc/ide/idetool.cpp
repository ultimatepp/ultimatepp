#include "ide.h"

#define LLOG(x) // DLOG(x)

void Ide::GotoPos(Point pos)
{
	int sc = editor.GetScrollPos().y;
	editor.SetCursor(editor.GetPos64(pos.y, pos.x));
	if(sc != editor.GetScrollPos().y)
		editor.TopCursor(4);
	editor.SetFocus();
	AddHistory();
	editor.SyncCursor();
}

void Ide::GotoPos(String path, int line)
{
	GotoPos(path, Point(0, line));
}

void Ide::GotoPos(String path, Point pos)
{
	LLOG("GotoPos " << path << ':' << line);
	if(path.GetCount()) {
		AddHistory();
		if(IsDesignerFile(path))
			DoEditAsText(path);
		EditFile(path);
	}
	GotoPos(pos);
}

String PosFn(const String& pkg, const String& n)
{
	return String() << Filter(IsExternalMode() ? GetFileName(pkg) : pkg, [](int c) { return c == '\\' ? '/' : c; }) << '/' << n;
}

void Ide::CopyPosition()
{
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pk = wspc.GetPackage(i);
		String pkg = wspc[i];
		for(String n : pk.file)
			if(PathIsEqual(SourcePath(pkg, n), editfile)) {
				WriteClipboardText(PosFn(pkg, n) << ":" << editor.GetCurrentLine());
				return;
			}
	}
	
	WriteClipboardText(GetFileName(editfile) << ":" << editor.GetCurrentLine());
}

void Ide::GotoPosition()
{
	String cs = ReadClipboardText();
	String f;
	int line = 0;
	for(char c : ":( \t") {
		String l;
		if(SplitTo(cs, c, f, l)) {
			f = TrimBoth(f);
			line = atoi(l);
			if(line)
				break;
		}
	}
	if(!line)
		return;
	f.Replace("\\", "/");
	const Workspace& wspc = GetIdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& pk = wspc.GetPackage(i);
		String pkg = wspc[i];
		for(String n : pk.file) {
			String pf = PosFn(pkg, n);
			int q = f.GetCount() - pf.GetCount() - 1;
			if(pf == f || q >= 0 && f.EndsWith(PosFn(pkg, n)) && f[q] == '/') {
				GotoPos(SourcePath(pkg, n), line - 1);
				return;
			}
		}
	}
}

void Ide::OpenTopic(const String& topic, const String& create_id, bool before)
{
	TopicLink tl = ParseTopicLink(topic);
	if(tl) {
		EditFile(AppendFileName(PackageDirectory(tl.package), tl.group + ".tpp"));
		if(designer) {
			TopicEditor *te = dynamic_cast<TopicEditor *>(&designer->DesignerCtrl());
			if(te)
				te->GoTo(tl.topic, tl.label, editor.GetCodeAnnotation(create_id), before);
		}
	}
}

void Ide::OpenTopic(const String& topic)
{
	OpenTopic(topic, String(), false);
}

void Ide::OpenATopic()
{
	String t = doc.GetCurrentLink();
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
			te->GoTo(GetFileTitle(file), "", AnnotationItem(), false);
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

void sPut(const String& name, String& qtf, const FileStat& fs)
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

void sPut(String& qtf, ArrayMap<String, FileStat>& pfs, ArrayMap<String, FileStat>& all) {
	FileStat pall;
	for(int i = 0; i < pfs.GetCount(); i++) {
		FileStat& fs = pfs[i];
		sPut(pfs.GetKey(i), qtf, fs);
		pall.Add(fs);
		all.GetAdd(pfs.GetKey(i)).Add(fs);
	}
	sPut("All files", qtf, pall);
	qtf << "}}&&";
}


void ShowQTF(const String& qtf, const char *title)
{
	RichText txt = ParseQTF(qtf);

	WithStatLayout<TopWindow> dlg;
	CtrlLayoutOK(dlg, title);
	dlg.copy << [=] {
		ClearClipboard();
		AppendClipboard(ParseQTF(qtf));
		PromptOK("The whole content of the text view has been successfully copied to cliboard!");
	};
	dlg.stat = qtf;
	dlg.Sizeable().Zoomable();
	dlg.Run();
}

void Ide::Licenses()
{
	Progress pi;
	const Workspace& wspc = IdeWorkspace();
	pi.SetTotal(wspc.GetCount());
	VectorMap<String, String> license_package;
	for(int i = 0; i < wspc.GetCount(); i++) {
		String n = wspc[i];
		pi.SetText(n);
		if(pi.StepCanceled()) return;
		String l = LoadFile(SourcePath(n, "Copying"));
		if(l.GetCount())
			MergeWith(license_package.GetAdd(l), ", ", n);
	}
	if(license_package.GetCount() == 0) {
		Exclamation("No license files ('Copying') have been found.");
		return;
	}
	String qtf;
	for(int i = 0; i < license_package.GetCount(); i++) {
		bool m = license_package[i].Find(',') >= 0;
		qtf << (m ? "Packages [* \1" : "Package [* \1")
		    << license_package[i]
		    << (m ? "\1] have" : "\1] has")
		    << " following licence notice:&"
		    << "{{@Y [C1 " << DeQtf(license_package.GetKey(i)) << "]}}&&";
	}
	
	ShowQTF(qtf, "Licenses");
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
		sPut(qtf, stat[i], all);
	}

	qtf << tab << "All packages" << hdr;
	sPut(qtf, all, all);

	ShowQTF(qtf, "Statistics");
}

String FormatElapsedTime(double run)
{
	String rtime;
	double hrs = floor(run / 3600);
	if(hrs > 0)
		rtime << Format("%0n hours, ", hrs);
	int minsec = fround(run - 3600 * hrs);
	int min = minsec / 60, sec = minsec % 60;
	if(min || hrs)
		rtime << Format("%d min, ", min);
	rtime << Format("%d sec", sec);
	return rtime;
}

void Ide::AlterText(WString (*op)(const WString& in))
{
	if(designer || !editor.IsSelection() || editor.IsReadOnly())
		return;
	editor.NextUndo();
	WString w = editor.GetSelectionW();
	editor.RemoveSelection();
	int l = editor.GetCursor();
	editor.Paste((*op)(w));
	editor.SetSelection(l, editor.GetCursor64());
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
	return WString(r);
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

static WString sComment(const WString& s) 
{
	return "/*" + s + "*/";
}

void Ide::ToComment()
{
	AlterText(sComment);
}

static WString sCommentLines(const WString& s)
{
	String r;
	StringStream ss(s.ToString());
	for(;;) {
		String line = ss.GetLine();
		if(ss.IsError())
			return s;
		else
		if(!line.IsVoid())
			r << "//" << line << "\n";
		if(ss.IsEof())
			break;
	}
	return r.ToWString();
}

void Ide::CommentLines()
{
	AlterText(sCommentLines);
}

static WString sUncomment(const WString& s)
{
	WString h = s;
	h.Replace("/*", "");
	h.Replace("//", "");
	h.Replace("*/", "");
	return h;
}

void Ide::UnComment()
{
	AlterText(sUncomment);
}

void Ide::Times()
{
	WithStatisticsLayout<TopWindow> statdlg;
	CtrlLayout(statdlg, "Elapsed times");
	statdlg.ok.Ok();
	statdlg.ok << [&] { statdlg.Break(); };
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

INITBLOCK {
	RegisterGlobalConfig("svn-msgs");
}

void RepoSyncDirs(const Vector<String>& working)
{
	Ptr<Ctrl> f = Ctrl::GetFocusCtrl();
	RepoSync repo;
	String repocfg = ConfigFile("repo.cfg");
	repo.SetMsgs(LoadFile(repocfg));
	for(String d : working)
		repo.Dir(d);
	repo.DoSync();
	SaveFile(repocfg, repo.GetMsgs());
	if(f)
		f->SetFocus();
}

void Ide::SyncRepoDirs(const Vector<String>& working)
{
	SaveFile();
	RepoSyncDirs(working);
	ScanWorkspace();
	SyncWorkspace();
}

void Ide::SyncRepo(){
	Vector<String> d = RepoDirs();
	if(d.GetCount())
		SyncRepoDirs(d);
	else
		SyncRepoDirs(RepoDirs(true));
}

void Ide::SyncRepoDir(const String& working)
{
	SyncRepoDirs(Vector<String>() << working);
}

void Ide::GotoDirDiffLeft(int line, DirDiffDlg *df)
{
	if(df->GetLMid()) return;
	EditFile(df->GetLeftFile());
	editor.SetCursor(editor.GetPos64(line));
	editor.SetFocus();
}

void Ide::GotoDirDiffRight(int line, DirDiffDlg *df)
{
	if(df->GetRMid()) return;
	EditFile(df->GetRightFile());
	editor.SetCursor(editor.GetPos64(line));
	editor.SetFocus();
}

void Ide::DoDirDiff()
{
	Index<String> dir;

	String n = GetFileFolder(editfile);
	if(n.GetCount())
		dir.FindAdd(n);
	for(String d : GetUppDirs())
		dir.FindAdd(d);

	ForAllNests([&](const Vector<String>& nests) {
		for(String d : nests) {
			dir.FindAdd(d);
		}
	});

	DirRepoDiffDlg& dlg = CreateNewWindow<DirRepoDiffDlg>();
	dlg.diff.WhenLeftLine = THISBACK1(GotoDirDiffLeft, &dlg);
	dlg.diff.WhenRightLine = THISBACK1(GotoDirDiffRight, &dlg);
	for(String d : dir) {
		dlg.Dir1AddList(d);
		dlg.Dir2AddList(d);
	}
	if(dir.GetCount() > 1)
		dlg.Dir1(dir[1]);
	dlg.SetFont(veditorfont);
	dlg.Maximize();
	dlg.OpenMain();
}

void Ide::DoPatchDiff()
{
	String patch = SelectFileOpen("Patch files (*.diff *.patch)\t*.diff *.patch\nAll files\t*.*");
	if(IsNull(patch))
		return;
	Index<String> dir;
	if(editfile.GetCount())
		dir.Add(GetFileFolder(editfile));
	Vector<String> d = GetUppDirs();
	for(int i = 0; i < d.GetCount(); i++)
		dir.FindAdd(d[i]);
	const Workspace& wspc = IdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++)
		dir.FindAdd(PackageDirectory(wspc[i]));
	static PatchDiff dlg;
	dlg.diff.WhenLeftLine = THISBACK1(GotoDirDiffLeft, &dlg);
	if(!dlg.IsOpen()) {
		dlg.SetFont(veditorfont);
		dlg.Maximize();
		if(dlg.Open(patch, dir.GetKeys()))
			dlg.OpenMain();
	}
	else
		dlg.SetFocus();
}

void Ide::AsErrors()
{
	ClearErrorsPane();
	SetBottom(BERRORS);
	String s = editor.IsSelection() ? editor.GetSelection() : editor.Get();
	StringStream ss(s);
	while(!ss.IsEof())
		ConsoleLine(ss.GetLine(), true);
	SetErrorEditor();
}

void Ide::RemoveDs()
{
	if(designer || editor.IsReadOnly())
		return;
	static Index<String> ds = { "DLOG", "DDUMP", "DDUMPC", "DDUMPM", "DTIMING",
	                            "DLOGHEX", "DDUMPHEX", "DTIMESTOP", "DHITCOUNT" };
	editor.NextUndo();
	int l = 0;
	int h = editor.GetLineCount() - 1;
	int ll, hh;
	if(editor.GetSelection(ll, hh)) {
		l = editor.GetLine(ll);
		h = editor.GetLine(hh);
	}
	for(int i = h; i >= l; i--) {
		String ln = editor.GetUtf8Line(i);
		try {
			CParser p(ln);
			if(p.IsId()) {
				String id = p.ReadId();
				if(ds.Find(id) >= 0 && p.Char('(')) {
					int pos = editor.GetPos(i);
					int end = min(editor.GetLength(), editor.GetPos(i) + editor.GetLineLength(i) + 1);
					editor.Remove(editor.GetPos(i), end - pos);
				}
			}
		}
		catch(CParser::Error) {}
	}
	editor.GotoLine(l);
}

void Ide::CopyRich()
{
	String qtf = "[C0 ";
	for(WString l0 : Split(editor.GetSelectionW(), '\n', false)) {
		WString ln;
		int tabsize = editor.GetTabSize();
		for(wchar c : l0)
			if(c == '\t')
				ln.Cat(' ', tabsize - ln.GetLength() % tabsize);
			else
			if(c >= ' ')
				ln.Cat(c);

		if(ln.GetCount() > 20000)
			ln.Trim(20000);

		Vector<LineEdit::Highlight> hln;
		hln.SetCount(ln.GetCount() + 1);
		for(int i = 0; i < hln.GetCount(); i++) {
			LineEdit::Highlight& h = hln[i];
			h.paper = White();
			h.ink = editor.GetColor(TextCtrl::INK_NORMAL);
			h.chr = ln[i];
			h.font = StdFont();
		}

		HighlightLine(editfile, hln, ln);

		int ii = 0;
		while(ii < hln.GetCount() - 1) {
			Color ink = hln[ii].ink;
			Font font = hln[ii].font;

			int n = 1;
			while(ii + n < hln.GetCount() - 1 && hln[ii + n].ink == hln[ii].ink && hln[ii + n].font == font)
				n++;
			
			qtf << "[@(" << ink.GetR() << "." << ink.GetG() << "." << ink.GetB() << ")";
			if(font.IsBold())
				qtf << "*";
			if(font.IsItalic())
				qtf << "/";
			if(font.IsUnderline())
				qtf << "_";
			qtf << " \1" << ln.Mid(ii, n).ToString() << "\1]";
			ii += n;
		}
		qtf << "&\n";
	}
	
	ClearClipboard();
	AppendClipboard(ParseQTF(qtf));
}

void Ide::LaunchAndroidSDKManager(const AndroidSDK& androidSDK)
{
	PromptOK("SDK managment is not yet implemented in TheIDE. Use Android Studio for this purpose instead.");
}

void Ide::LaunchAndroidAVDManager(const AndroidSDK& androidSDK)
{
	PromptOK("AVD managment is not yet implemented in TheIDE. Use Android Studio for this purpose instead.");
}
