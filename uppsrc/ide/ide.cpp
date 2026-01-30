#include "ide.h"

VectorMap<String, String> git_branch_cache;

String GetGitBranchRaw(const String& dir)
{
	String branch = GitCmd(dir, "branch --show");
	branch.TrimEnd("\n");
	return branch;
}

String GetGitBranch(const String& dir)
{
	int q = git_branch_cache.Find(dir);
	String branch;
	if(q < 0) {
		String git_dir = dir;
		if(GetRepo(git_dir) == GIT_DIR) {
			q = git_branch_cache.Find(git_dir);
			if(q < 0) {
				branch = GetGitBranchRaw(git_dir);
				q = git_branch_cache.GetCount();
				git_branch_cache.Add(git_dir, branch);
			}
			branch = git_branch_cache[q];
		}
		q = git_branch_cache.Find(dir);
		if(q < 0) {
			q = git_branch_cache.GetCount();
			git_branch_cache.Add(dir, branch);
		}
	}
	return git_branch_cache[q];
}

void Ide::MakeTitle()
{
	if(replace_in_files) return;
	
	String title;
	title << Nvl(main, "TheIDE");

	Vector<String> dirs = GetUppDirs();
	if(dirs.GetCount()) {
		String branch = GetGitBranch(dirs[0]);
		if(branch.GetCount())
			title << " [ " << branch << " ] ";
	}

	if(!mainconfigname.IsEmpty() &&  mainconfigname == mainconfigparam)
		title << " - " << mainconfigname;
	else
	if(!mainconfigname.IsEmpty()) {
		title << " - " << mainconfigname;
		title << " ( " << mainconfigparam << " )";
	}
	if(designer) {
		title << " - " << designer->GetFileName();
		int cs = designer->GetCharset();
		if(cs >= 0)
			title << " " << CharsetName(cs);
	}
	else
	if(!editfile.IsEmpty()) {
		title << " - " << editfile;
		String branch = GetGitBranch(GetFileFolder(editfile));
		if(branch.GetCount())
			title << " [ " << branch << " ] ";
		int chrset = editor.GetCharset();
		title << " " << IdeCharsetName(chrset)
		      << " " << (findarg(Nvl(editfile_line_endings, line_endings), LF, DETECT_LF) >= 0 ? "LF" : "CRLF");
		if(editor.IsTruncated())
			title << " [Truncated]";
		if(editor.IsView())
			title << " [View]";
		else
		if(editor.IsReadOnly())
			title << " [Read Only]";
		if(editor.IsDirty())
			title << " *";
		if(!bar_branch)
			branch = Null;
		if(findarg(branch, "", "master", "main") >= 0)
			editor.BarText(branch, GrayColor(IsDarkTheme() ? 100 : 220));
		else {
			auto h = GetHashValue(branch);
			int r = h & 31; h >>= 5;
			int g = h & 15; h >>= 4;
			int b = h & 63;
			editor.BarText(branch, IsDarkTheme() ? Color(150 + r, 150 + g, 150 + b) : Color(200 - r, 200 - g, 200 - b));
		}

	}
	if(!IsNull(editfile))
		for(int i = 0; i < 10; i++)
			if(NormalizePath(editfile) == NormalizePath(bookmark[i].file))
				title << Format(" <%d>", i);
	
	title << " { " << GetAssemblyId() << " }";
	if(isscanning)
		title << " (scanning files)";
	
	Title(title.ToWString());
	DoDisplay();
}

bool Ide::CanToggleReadOnly()
{
	return NormalizePath(GetActiveFilePath()) == NormalizePath(editfile) && !editor.IsView();
}

void Ide::ToggleReadOnly()
{
	if(CanToggleReadOnly() && IsActiveFile()) {
#ifdef PLATFORM_WIN32
		FindFile ff(editfile);
		if(ff && ff.IsReadOnly()) {
			dword attrib = GetFileAttributes(editfile);
			attrib &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(editfile, attrib);
		}
#endif
		editor.SetEditable(editor.IsReadOnly());
		ActiveFile().readonly = editor.IsReadOnly();
		SavePackage();
		MakeTitle();
		SetBar();
	}
}

void Ide::AdjustMainConfig()
{
	const Workspace& wspc = IdeWorkspace();
	if(!wspc.GetCount())
		return;
	const Array<Package::Config>& f = wspc.GetPackage(0).config;
	for(int i = 0; i < f.GetCount(); i++)
		if(f[i].param == mainconfigparam)
			return;
	if(f.GetCount()) {
		mainconfigparam = f[0].param;
		mainconfigname = f[0].name;
		SetMainConfigList();
	}
}

String Ide::GetFirstFile()
{
	const Workspace& wspc = IdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++) {
		const Package& p = wspc.GetPackage(i);
		for(int j = 0; j < p.GetCount(); j++)
			if(!p[j].separator)
				return SourcePath(wspc[i], p[j]);
	}
	return Null;
}

void Ide::SetMain(const String& package)
{
	FlushFile();
	SaveWorkspace();
	transferfilecache.Clear();
	main = package;
	export_dir = GetHomeDirFile(main);
	history.Clear();
	mainconfigname.Clear();
	mainconfigparam.Clear();

	UppHubAuto(main);

	ScanWorkspace();
	editfile.Clear();
	LoadFromFile(THISBACK(SerializeWorkspace), WorkspaceFile());
	tabs.FixIcons();
	editorsplit.Zoom(0);
	String e = editfile;
	UpdateFormat();
	editfile.Clear();
	MakeTitle();
	MakeIcon();
	SyncMainConfigList();
	AdjustMainConfig();
	SyncBuildMode();
	SetHdependDirs();
	SetBar();
	SyncUsc();
	if(IsNull(e))
		e = GetFirstFile();
	EditFile(e);
}

void Ide::Exit()
{
	if(debugger)
		debugger->Stop();
	SaveFile();
	SaveWorkspace();
	FlushFile();
	console.Kill();
	Break(IDOK);
	IdeExit = true;
}

bool Ide::OpenMainPackage()
{
	String version = SplashCtrl::GenerateVersionNumber();
	String tt = "Select main package";
#ifdef bmYEAR
	tt << " (TheIDE " << version
	   << Format(" %d-%02d-%02d %d:%02d)", bmYEAR , bmMONTH, bmDAY, bmHOUR, bmMINUTE);
#else
	tt << " (TheIDE " << version << ')';
#endif
	String nest;
	String p = SelectPackage(nest, tt, main, true, true);
	if(p.IsEmpty()) return false;
	InvalidatePackageCache();
	SetMainNest(nest);
	main.Clear();
	if(!IsOpen())
		Open();
	if(IsExternalMode())
		SyncEmptyPackage(p);
	SetMain(p);
	return true;
}

void Ide::NewMainPackage()
{
	if(setmain_newide) {
		Host h;
		CreateHost(h, false, false);
		h.Launch(GetExeFilePath() + " --nosplash");
	}
	else {
		OpenMainPackage();
		StopBuild();
	}
}

void Ide::PackageCursor()
{
	WorkspaceWork::PackageCursor();
	String p = GetActivePackage();
	if(p.IsEmpty()) return;
	String ef = ToLower(NormalizePath(editfile));
	for(int i = 0; i < filelist.GetCount(); i++)
		if(ToLower(NormalizePath(SourcePath(p, filelist[i]))) == ef) {
			filelist.SetCursor(i);
			break;
		}
	SetBar();
}

void Ide::EditWorkspace()
{
	EditPackages(main, GetActivePackage(), pocfg);
	ScanWorkspace();
	SyncWorkspace();
	InvalidateIncludes();
}

String Ide::WorkspaceFile()
{
	String nm;
	for(const char *s = main; *s; s++)
		nm.Cat(*s == '\\' || *s == '/' ? '$' : *s);
	String cfg = ConfigFile("cfg");
	RealizeDirectory(cfg);
	return AppendFileName(cfg, ForceExt(nm + '@' + GetVarsName(), ".cfg"));
}

void Ide::SaveWorkspace()
{
	if(console.console) return;
	if(main.IsEmpty()) return;
	StoreToFile(THISBACK(SerializeWorkspace), WorkspaceFile());
}

void Ide::UscFile(const String& file)
{
	try {
		ParseUscFile(file);
	}
	catch(CParser::Error& e) {
		ShowConsole();
		console << e << "\n";
	}
}

void Ide::UscProcessDir(const String& dir)
{
	for(FindFile ff(AppendFileName(dir, "*.usc")); ff; ff.Next())
		UscFile(AppendFileName(dir, ff.GetName()));
}

void Ide::UscProcessDirDeep(const String& dir)
{
	UscProcessDir(dir);
	for(FindFile ff(AppendFileName(dir, "*")); ff; ff.Next())
		if(ff.IsFolder())
			UscProcessDirDeep(ff.GetPath());
}

void Ide::SyncUsc()
{
	CleanUsc();
	UscProcessDir(GetLocalDir());
	UscProcessDir(GetFileFolder(ConfigFile("x")));

	if(IsNull(main))
		return;
	::Workspace wspc;
	wspc.Scan(main);
	int i;
	for(i = 0; i < wspc.GetCount(); i++) {
		const Package& p = wspc.GetPackage(i);
		for(int j = 0; j < p.file.GetCount(); j++) {
			String file = SourcePath(wspc[i], p.file[j]);
			if(ToLower(GetFileExt(file)) == ".usc")
				UscFile(file);
		}
	}
}

void Ide::SyncWorkspace()
{
	SyncUsc();
}

bool IsTextFile(const String& file, int maxline) {
	byte buffer[16384];
	FileIn fi(file);
	if(!fi.IsOpen())
		return false;
	int count = fi.Get(buffer, sizeof(buffer) - 1);
	buffer[count] = 0;
	const byte *end = buffer + count;
	const byte *q = buffer;
	const byte *x = q;
	while(q < end) {
		if(*q < 32) {
			int c = *q;
			if(c == '\n') {
				if(q - x > maxline) return false;
				x = q;
			}
			else
			if(c != '\r' && c != '\t' && c != '\v' && c != '\f' && c != 0x1a)
				return false;
		}
		q++;
	}
	return true;
}

/*
Console& Ide::GetConsole()
{
	int q = btabs.GetCursor();
	return q == BFINDINFILES ? console2 : console;
}
*/
void Ide::Renumber() {
	for(int i = 0; i < filedata.GetCount(); i++)
		::Renumber(filedata[i].lineinfo);
	editor.Renumber();
}

void Ide::CycleFiles()
{
	if(++tabi >= tablru.GetCount())
		tabi = 0;
	if(tabi < tablru.GetCount()) {
		blocktabs = true;
		EditFile(tablru[tabi]);
	}
}

void Ide::DeactivateBy(Ctrl *new_focus)
{
	if(deactivate_save && issaving == 0 && !new_focus && editor.GetLength64() < 1000000) {
		DeactivationSave(true);
		SaveFile();
		DeactivationSave(false);
	}
	TopWindow::DeactivateBy(new_focus);
	win_deactivated = !new_focus || dynamic_cast<TopWindow *>(new_focus);
}

void Ide::Activate()
{
	if(win_deactivated) {
		TriggerIndexer();
		editor.TriggerSyncFile(0);
		TriggerIdeBackgroundThread(5000);
		git_branch_cache.Clear();
		win_deactivated = false;
	}
	TopWindow::Activate();
	MakeTitle();
}

bool Ide::Key(dword key, int count)
{
	switch(key) {
	case K_ALT|K_CTRL_UP:
	case K_ALT|K_CTRL_DOWN:
	case K_ALT_DELETE:
		return filelist.Key(key, count);
	case K_ALT_UP:
		return filelist.Key(K_UP, 0);
	case K_ALT_DOWN:
		return filelist.Key(K_DOWN, 0);
	case K_ALT_PAGEUP:
		return package.Key(K_UP, 0);
	case K_ALT_PAGEDOWN:
		return package.Key(K_DOWN, 0);
	case K_CTRL|K_ALT_LEFT:
		TabsLR( TabBar::JumpDirLeft );
		return true;
	case K_CTRL|K_ALT_RIGHT:
		TabsLR( TabBar::JumpDirRight );
		return true;
	case K_CTRL|K_ALT_B:
		TabsStackLR( TabBar::JumpDirLeft );
		return true;
	case K_CTRL|K_ALT_N:
		TabsStackLR( TabBar::JumpDirRight );
		return true;
	case K_SHIFT|K_CTRL_O:
		AddFile(WorkspaceWork::ANY_FILE);
		return true;
#ifdef PLATFORM_COCOA
	case K_ALT_KEY|K_KEYUP:
	case K_OPTION_KEY|K_KEYUP:
#endif
	case K_CTRL_KEY|K_KEYUP:
		if(tabi) {
			tabi = 0;
			AddLru();
		}
		return true;
	case K_CTRL_TAB:
#ifdef PLATFORM_COCOA
	case K_ALT|K_TAB:
	case K_OPTION|K_TAB:
#endif
		CycleFiles();
		return true;
	case K_MOUSE_BACKWARD:
		History(-1);
		return true;
	case K_MOUSE_FORWARD:
		History(1);
		return true;
	#ifndef PLATFORM_COCOA
	default:
		if(key >= K_SHIFT_CTRL_0 && key <= K_SHIFT_CTRL_9) {
			Bookmark& b = bookmark[key - K_SHIFT_CTRL_0];
			b.file = editfile;
			b.pos = editor.GetEditPos();
			MakeTitle();
			return true;
		}
		if(key >= K_CTRL_0 && key <= K_CTRL_9) {
			GotoBookmark(bookmark[key - K_CTRL_0]);
			return true;
		}
	#endif
	}
	return false;
}

void Ide::GotoBookmark(const Bookmark& b)
{
	if(b.file.IsEmpty()) return;
	EditFile(b.file);
	if(bookmark_pos)
		editor.SetEditPos(b.pos);
}

bool Ide::IsHistDiff(int i)
{
	if(i < 0 || i >= history.GetCount())
		return false;
	Bookmark& b = history[i];
	return b.file != editfile || abs(editor.GetCursor64() - b.pos.cursor) > 20;
}

void Ide::IdePaste(String& data)
{
	data.Clear();
	if(AcceptFiles(Clipboard())) {
		Vector<String> s = GetFiles(Clipboard());
		for(int i = 0; i < s.GetCount(); i++)
			if(FileExists(s[i]) && IsTextFile(s[i], 10000)) {
				int64 len = GetFileLength(s[i]);
				if(data.GetLength() + len > 104857600) {
					Exclamation("The paste size breaks the 100MB limit.");
					return;
				}
				data.Cat(LoadFile(s[i]));
			}
	}
}

void Ide::AddHistory()
{
	if(history.GetCount()) {
		if(IsHistDiff(histi))
			++histi;
	}
	else
		histi = 0;
	history.At(histi);
	Bookmark& b = history.Top();
	b.file = editfile;
	b.pos = editor.GetEditPos();
	SetBar();
}

void Ide::EditorEdit()
{
	AddHistory();
	TouchFile(editfile);
}

int  Ide::GetHistory(int d)
{
	if(history.GetCount())
		for(int i = histi + (d > 0); i >= 0 && i < history.GetCount(); i += d)
			if(IsHistDiff(i))
				return i;
	return -1;
}

void Ide::History(int d)
{
	int i = GetHistory(d);
	if(i >= 0) {
		histi = i;
		GotoBookmark(history[histi]);
		SetBar();
	}
}

void Ide::BookKey(int key)
{
	Key(key, 1);
}

void Ide::SetIdeState(int newstate)
{
	if(newstate != idestate)
	{
		if(newstate == BUILDING)
			build_start_time = GetSysTime();
		else
		{
			if(idestate == BUILDING && !IsNull(build_start_time))
				stat_build_time += int(GetSysTime() - build_start_time);
			build_start_time = Null;
		}
	}
	idestate = newstate;
	MakeTitle();
	SetBar();
}

void Ide::MakeIcon() {
#ifdef PLATFORM_COCOA
	if(!macos_update_icon) {
		String badge = IsOpen() ? main : "";
		SetBadgeLabel(badge);
		return;
	}
#endif

	Image li = IdeImg::Icon256();
#ifndef PLATFORM_LINUX // Kubuntu is using this icon for window while ignoring it in taskbar...
	WString mp = main.ToWString();
	if(!IsNull(mp))
	{
		Size isz = li.GetSize();
		ImageDraw idraw(isz);
		Draw& mdraw = idraw.Alpha();
		idraw.DrawImage(0, 0, li);
		mdraw.DrawImage(0, 0, li, White);
		int fh = 112;
		Size sz(0, 0);
		Font font;
		while(fh > (IsUHDMode() ? 64 : 80)) {
			font = StdFont(fh);
			sz = GetTextSize(mp, font) + 8 * Size(4, 2);
			if(sz.cx <= isz.cx)
				break;
			fh--;
		}
		int x = max((isz.cx - sz.cx) / 2, 0);
		int y = isz.cy - sz.cy;
		idraw.DrawRect(x, y, sz.cx, sz.cy, White);
		mdraw.DrawRect(x, y, sz.cx, sz.cy, White);
		idraw.DrawText(x + 12, y + 1, mp, font, Black);
		DrawFrame(idraw, x, y, sz.cx, sz.cy, LtBlue);
		if(state_icon)
			idraw.DrawImage(0, 0, decode(state_icon, 1, IdeImg::IconDebuggingLarge256(),
			                                         2, IdeImg::IconRunningLarge256(),
			                                         IdeImg::IconBuildingLarge256()));
		li = idraw;
	}
#endif
	LargeIcon(li);
}

void Ide::SetIcon()
{
	int new_state_icon = 0;
	if((bool)debugger && !IdeIsDebugLock())
		new_state_icon = 1;
	else
	if((GetTimeClick() / 800) & 1) {
		if(debugger)
			new_state_icon = 2;
		else
		if(idestate == BUILDING)
			new_state_icon = 3;
	}
	if(state_icon == new_state_icon)
		return;
	state_icon = new_state_icon;
	MakeIcon();
#ifdef PLATFORM_WIN32
	switch(state_icon) {
	case 1:  Icon(DPI(IdeImg::IconDebugging(), IdeImg::IconDebuggingLarge())); break;
	case 2:  Icon(DPI(IdeImg::IconRunning(), IdeImg::IconRunningLarge())); break;
	case 3:  Icon(DPI(IdeImg::IconBuilding(), IdeImg::IconBuildingLarge())); break;
	default: Icon(DPI(IdeImg::Icon(), IdeImg::PackageLarge()));
	}
#else
	switch(state_icon) {
	case 1:  Icon(IdeImg::IconDebugging()); break;
	case 2:  Icon(IdeImg::IconRunning()); break;
	case 3:  Icon(IdeImg::IconBuilding()); break;
	default: Icon(IdeImg::Icon());
	}
#endif
}

Rect Ide::GetFileInfoRect()
{
	Rect r = display.GetScreenRect();
	r.top = r.bottom - GetStdFontCy() / 2;
	r.bottom = r.top + GetStdFontCy() + DPI(2);
	r.left = r.right - Zx(400);
	return r - GetScreenRect().TopLeft();
}

void Ide::PaintTitlebarInfo(Draw& w)
{
	if(fileinfo_visible) {
		Time tm(edittime);
		String qtf = "[g ";
		
		qtf << AsString(Format("  [@b* %d]-[@b* %02d]-[@b* %02d] [* %02d]:[* %02d]:%02d",
		                (int)tm.year, (int)tm.month, (int)tm.day,
		                (int)tm.hour, (int)tm.minute, (int)tm.second));
		                
		double d = double(GetSysTime() - tm) / 60;
		String unit = "minute";
		auto DoUnit = [&](double m, const char *s) {
			if(d > m) {
				d /= m;
				unit = s;
				return true;
			}
			return false;
		};
		DoUnit(60, "hour") && DoUnit(24, "day") && DoUnit(30.5, "month") && DoUnit(12, "year");
		int n = (int)round(d);
		if(n != 1)
			unit << "s";
		qtf << " ([* " << n << ' ' << unit << "] ago), size [* " << FormatFileSize(editfile_length);
		
		if(editfile_isreadonly)
			qtf << "][@B read only  ";

		RichText txt = ParseQTF(qtf);
		txt.ApplyZoom(GetRichTextStdScreenZoom());
		Size tsz(txt.GetWidth(), txt.GetHeight(INT_MAX));

		Rect r = GetFileInfoRect();
		r.left = r.right - tsz.cx - DPI(4);
		DrawFrame(w, r, SBlack());
		r.Deflate(1, 1);
		w.DrawRect(r, SColorPaper());
		Size sz = r.GetSize();
		txt.Paint(w, DPI(2) + r.left, (sz.cy - tsz.cy) / 2 + r.top, INT_MAX / 2);
	}
	if(assemblyinfo_visible) {
		Vector<String> nests = GetUppDirs();
		Size tsz;
		Rect r = GetAssemblyInfoRect(&tsz);
		RichText txt = ParseQTF(GetAssemblyInfoQtf());
		txt.ApplyZoom(GetRichTextStdScreenZoom());
		DrawFrame(w, r, SBlack());
		r.Deflate(1, 1);
		w.DrawRect(r, SColorPaper());
		Size sz = r.GetSize();
		txt.Paint(w, DPI(2) + r.left, (sz.cy - tsz.cy) / 2 + r.top, tsz.cx);
	}
}

String Ide::GetAssemblyInfoQtf()
{
	String qtf  = "[g ";
	const Workspace& wspc = GetIdeWorkspace();
	Vector<String> nests = GetUppDirs();
	struct PkgInfo : Moveable<PkgInfo> {
		String name;
		bool   bold, italic;
		Color  ink;
	};
	VectorMap<String, Vector<PkgInfo>> map;
	for(String s : nests) {
		String nest = UnixPath(s);
		for(int i = 0; i < wspc.GetCount(); i++)
			if(UnixPath(NormalizePath(PackageDirectory(wspc[i]))).StartsWith(nest)) {
				PkgInfo& m = map.GetAdd(nest).Add();
				m.name = wspc[i];
				const Package& p = wspc.GetPackage(i);
				m.bold = p.bold || i == 0;
				m.italic = p.italic;
				m.ink = p.ink;
			}
	}
	for(int i = 0; i < map.GetCount(); i++) {
		String nest = map.GetKey(i);
		Vector<PkgInfo>& pkgs = map[i];
		Sort(pkgs, [=](const PkgInfo& a, const PkgInfo& b) {
			return a.name < b.name;
		});
		qtf << "\1" << nest << "\1&";
		String g = GetGitBranch(nest);
		if(g.GetCount())
			qtf << "[$L@bl100 \1" << g << "\1&]";
		else
			qtf << "[@K/l100 not a git repo&]";
		qtf << "[l100 ";
		for(int i = 0; i < pkgs.GetCount(); i++) {
			const PkgInfo& p = pkgs[i];
			if(i)
				qtf << ", ";
			qtf << "[";
			if(p.bold)
				qtf << "*";
			if(p.italic)
				qtf << "/";
			if(!IsNull(p.ink))
				qtf << "@(" << p.ink.GetR() << "." << p.ink.GetG() << "." << p.ink.GetB() << ")";
			qtf << " \1" << p.name << "\1]";
		}
		qtf << "&]";
	}
	qtf.TrimEnd("&]");
	return qtf;
}

Rect Ide::GetAssemblyInfoRect(Size *tp)
{
	Rect r = display_main.GetScreenRect();
	r.top = r.bottom - GetStdFontCy() / 2;
	RichText txt = ParseQTF(GetAssemblyInfoQtf());
	txt.ApplyZoom(GetRichTextStdScreenZoom());
	Size tsz;
	tsz.cx = min(txt.GetWidth(), DPI(450));
	tsz.cy = txt.GetHeight(tsz.cx);
	if(tp)
		*tp = tsz;
	r.left = r.right - tsz.cx - DPI(4) - 2;
	r.bottom = r.top + tsz.cy + DPI(4) + 2;
	return r - GetScreenRect().TopLeft();
}

void Ide::Periodic()
{
	CheckFileUpdate();
	SetIcon();
	if(debugger && debugger->IsFinished() && !IdeIsDebugLock())
		IdeEndDebug();
	SyncClang();
	Rect r = display.GetScreenRect();
	r.left = max(r.left, r.right - Zx(150));
	bool b = r.Contains(GetMousePos());
	if(fileinfo_visible != b) {
		RefreshFrame(GetFileInfoRect());
		fileinfo_visible = b;
	}
	b = display_main.IsVisible() && display_main.GetScreenRect().Contains(GetMousePos());
	if(assemblyinfo_visible != b) {
		RefreshFrame(GetAssemblyInfoRect());
		assemblyinfo_visible = b;
	}
}

struct IndexerProgress : ImageMaker {
	double pos;

	String Key() const override {
		String h;
		RawCat(h, pos);
		return h;
	}
	
	Image Make() const override {
		Size sz = IdeImg::Indexer().GetSize();
		ImagePainter iw(sz);
		iw.Clear(RGBAZero());
		iw.Move(sz.cx / 2, sz.cy / 2).Arc(sz.cx / 2, sz.cy / 2, sz.cx / 2, -M_PI/2, pos * M_2PI).Line(sz.cx / 2, sz.cy / 2).Fill(SGray());
		return iw;
	}
};

void Ide::SyncClang()
{
	Vector<Color> a;
	int phase = msecs() / 30;
	auto AnimColor = [](int animator) {
		return Blend(IsDarkTheme() ? GrayColor(70) : SColorLtFace(), Color(198, 170, 0), animator);
	};
	auto Animate = [=](int& animator, int& dir, bool animate) -> Color {
		if(animator <= 0 && !animate) return Null;
		if(animate)
			animator = 20;
		else
			animator -= 3;
		return AnimColor(animator);
	};
	Color bg = Animate(animate_current_file, animate_current_file_dir,
	                   (editor.annotating || IsCurrentFileParsing()) && HasLibClang());
	int cx = editor.GetBarSize().cx;
	if(!IsNull(bg)) {
		for(int i = 0; i < cx; i++)
			a.Add(i > cx - DPI(6) ? bg : Null);
	}
	if(IsAutocompleteParsing() && HasLibClang())
		a.At((phase % DPI(6)) + cx - DPI(6)) = SGray();
	editor.AnimateBar(pick(a));
	editor.search.SetBackground(Animate(animate_indexer, animate_indexer_dir, Indexer::IsRunning()));
	if(Indexer::IsRunning()) {
		IndexerProgress mi;
		mi.pos = Indexer::Progress();
		indeximage.SetImage(MakeImage(mi));
		
		static Image waitani[64];
		ONCELOCK {
			int sz = DPI(16);
			for(int ani = 0; ani < 64; ani++) {
				ImagePainter sw(sz, sz);
				sw.Clear(RGBAZero());
				Pointf prev = Null;
				for(int i = 0; i <= 128; i++) {
					Pointf next = (sz / 2 - 1) * Polar((i + ani * 4) * M_2PI / 256) + Pointf(sz / 2, sz / 2);
					if(!IsNull(prev))
						sw.Move(prev).Line(next).Stroke(DPI(i) / 128.0, Blend(SLtGray, SLtRed, 2 * i));
					prev = next;
				}
				waitani[ani] = sw.GetResult();
			}
		}
		indeximage2.SetImage(waitani[(msecs() / 20) & 63]);
	}
	else {
		indeximage.SetImage(Null);
		indeximage2.SetImage(Null);
	}
	animate_phase = phase;
}

const Workspace& Ide::IdeWorkspace() const
{
	static Workspace wspc;
	static String _main;
	if(main != _main || wspc.GetCount() == 0) {
		wspc.Scan(main);
		_main = main;
	}
	else {
		for(int i = 0; i < wspc.GetCount(); i++)
			if(wspc.GetPackage(i).time != FileGetTime(PackageFile(wspc[i]))) {
				wspc.Scan(main);
				break;
			}
	}
	return wspc;
}

const Workspace& Ide::AssistWorkspace() const
{
	static Workspace wspc;
	static String _main;
	bool update = false;
	if(main != _main || wspc.GetCount() == 0) {
		update = true;
		_main = main;
	}
	else {
		for(int i = 0; i < wspc.GetCount(); i++)
			if(wspc.GetPackage(i).time != FileGetTime(PackageFile(wspc[i]))) {
				update = true;
				break;
			}
	}
	if(update)
		wspc.Scan(main, SplitFlags(mainconfigparam, false));
	return wspc;
}

void Ide::AddPackage(const String& p)
{
	const Workspace& wspc = IdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++){
		if(wspc[i] == p)
			return;
	}
	if(!PromptOKCancel("Package [* " + p + "] is not yet in the workspace.&Do you want to add it?"))
		return;
	OptItem& m = actual.uses.Add();
	m.text = p;
	SaveLoadPackage();
}

int Ide::GetPackageIndex()
{
	const Workspace& wspc = IdeWorkspace();
	for(int i = 0; i < wspc.GetCount(); i++)
		if(wspc[i] == package.GetCurrentName())
			return i;
	return -1;
}

void Ide::DoDiff(FileDiff *df)
{
	auto Do = [=](const String& file, int line) {
		EditFile(file);
		editor.SetCursor(editor.GetPos64(line));
		editor.SetFocus();
	};
	df->diff.WhenLeftLine = [=](int line) { Do(df->editfile, line); };
	df->diff.WhenRightLine = [=](int line) { Do(df->GetExtPath(), line); };
	df->OpenMain();
}

void Ide::Diff()
{
	if(IsNull(editfile))
		return;
	FileDiff& diffdlg = CreateNewWindow<FileDiff>(AnySourceFs());
	diffdlg.Set(editfile);
	String s = diffdlg.GetExtPath();
	if(FileExists(s))
		LruAdd(difflru, s);
	DoDiff(&diffdlg);
}

void Ide::DiffWith(const String& path)
{
#ifdef CPU_64
	int64 maxsize = 2000*1024*1024;
#else
	int64 maxsize = 100*1024*1024;
#endif
	if(IsNull(editfile) || IsNull(path))
		return;
	if(max(GetFileLength(editfile), GetFileLength(path)) > maxsize) {
		Exclamation("Too big to compare");
		return;
	}
	FileDiff& diffdlg = CreateNewWindow<FileDiff>(AnySourceFs());
	diffdlg.Set(editfile, path);
	DoDiff(&diffdlg);
}

struct ConflictDiff : TopWindow {
	Label        left, right;
	TextDiffCtrl diff;
	
	virtual void Layout()
	{
		Size sz = GetSize();
		int  fy = GetStdFont().GetCy() + DPI(5);
		left.LeftPos(0, sz.cx / 2).TopPos(0, fy);
		right.RightPos(0, sz.cx / 2).TopPos(0, fy);
		diff.HSizePos().VSizePos(fy, 0);
	}

	void Set(const char *lname, const String& l, const char *rname, const String& r)
	{
		left = "\1[=* \1" + String(lname);
		right = "\1[=* \1" + String(rname);
		diff.Set(l, r);
	}
	
	ConflictDiff() {
		SetRect(GetWorkArea().Deflated(DPI(32)));
		Sizeable().Zoomable();
		Add(left);
		Add(right);
		Add(diff);
	}
};

String Ide::LoadConflictFile(const String& n)
{
	return n.GetCount() == 1 ? GitCmd(GetFileFolder(editfile), "show :" + n + ":./" + GetFileName(editfile))
	                         : LoadFile(n);
}

void Ide::DiffFiles(const char *lname, const String& l, const char *rname, const String& r)
{
	ConflictDiff diff;
	diff.Set(lname, LoadConflictFile(l), rname, LoadConflictFile(r));
	diff.Execute();
}

void Ide::TriggerIndexer0()
{
	Indexer::Start(main, GetCurrentIncludePath(), GetCurrentDefines());
}

void Ide::TriggerIndexer()
{
	if(AutoIndexer)
		TriggerIndexer0();
}