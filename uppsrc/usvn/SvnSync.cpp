#include "usvn.h"

bool CheckSvn()
{
	String h;
	if(Sys("svn", h) >= 0)
		return true;
#ifdef PLATFORM_WIN32
	Exclamation("Unable to execute svn.exe!&"
	            "You can download svn client here: [^http://www.sliksvn.com/en/download^ http://www.sliksvn.com/en/download]");
#else
	Exclamation("Unable to execute 'svn' binary!&Please install svn client.");
#endif
	return false;
}

SvnSync::SvnSync()
{
	CtrlLayoutOKCancel(*this, "SvnSynchronize SVN repositories");
	list.AddIndex();
	list.AddIndex();
	list.AddColumn("Action");
	list.AddColumn("Path");
	list.AddColumn("Changes");
	list.ColumnWidths("220 500 100");
	list.NoCursor().EvenRowColor();
	list.SetLineCy(max(Draw::GetStdFontCy(), 20));
	list.WhenLeftClick = THISBACK(Diff);
	Sizeable().Zoomable();
	setup <<= THISBACK(Setup);
	BackPaint();
}

void SvnSync::Setup()
{
	works.Execute();
	SyncList();
}

int CharFilterSvnMsg(int c)
{
	return c >= 32 && c < 128 && c != '\"' ? c : 0;
}

void SvnSync::SyncList()
{
	list.Clear();
	for(int i = 0; i < works.GetCount(); i++) {
		SvnWork w = works[i];
		String path = GetFullPath(w.working);
		list.Add(REPOSITORY, path,
		         AttrText("Working directory").SetFont(StdFont().Bold()).Ink(White).Paper(Blue),
		         AttrText(path).SetFont(Arial(20).Bold()).Paper(Blue).Ink(White),
		         AttrText("").SetFont(Arial(20).Bold()).Paper(Blue).Ink(White));
		list.SetLineCy(list.GetCount() - 1, 26);
		Vector<String> ln = Split(Sys("svn status " + path), CharFilterCrLf);
		bool actions = false;
		for(int pass = 0; pass < 2; pass++)
			for(int i = 0; i < ln.GetCount(); i++) {
				String h = ln[i];
				if(h.GetCount() > 7) {
					String file = GetFullPath(TrimLeft(h.Mid(7)));
					if(IsFullPath(file)) {
						actions = true;
						h.Trim(7);
						bool simple = h.Mid(1, 6) == "      ";
						int action = simple ? String("MC?!~").Find(h[0]) : SVN_IGNORE;
						if(h == "    S  ")
							action = REPLACE;
						String an;
						Color  color;
						if(action == SVN_IGNORE) {
							color = Black;
							if(simple && h[0] == 'A') {
								an = "svn add";
								action = SVN_ACTION;
							}
							else
							if(simple && h[0] == 'D') {
								an = "svn delete";
								action = SVN_ACTION;
							}
							else {
								an = h.Mid(0, 7);
								color = Gray;
							}
						}
						else {
							int q = file.ReverseFind('.');
							if(action == ADD && q >= 0 && (file.Mid(q + 1) == "mine" ||
							   file[q + 1] == 'r' && IsDigit(file[q + 2]))
							   && FileExists(file.Mid(0, q))) {
								action = DELETEC;
								an = "Delete (conflict resolved)";
								color = Black;
							}
							else {
								static const char *as[] = {
									"Modify", "Resolved", "Add", "Remove", "Replace"
								};
								static Color c[] = { LtBlue, Magenta, Green, LtRed, LtMagenta };
								an = as[action];
								color = c[action];
							}
						}
						if(pass == action < 0) {
							int ii = list.GetCount();
							list.Add(action, file,
							         action < 0 ? Value(AttrText(an).Ink(color)) : Value(true),
							         AttrText("  " + file.Mid(path.GetCount() + 1)).Ink(color));
							if(action >= 0) {
								list.SetCtrl(ii, 0, revert.Add().SetLabel("Revert\n" + an + "\nSkip").NoWantFocus());
								Ctrl& b = diff.Add().SetLabel("Changes..").SizePos().NoWantFocus();
								b <<= THISBACK1(DoDiff, ii);
								list.SetCtrl(ii, 2, b);
							}
						}
					}
				}
		}
		if(actions) {
			list.Add(MESSAGE, Null, AttrText("Commit message:").SetFont(StdFont().Bold()));
			list.SetLineCy(list.GetCount() - 1, (3 * EditField::GetStdHeight()) + 4);
			list.SetCtrl(list.GetCount() - 1, 1, message.Add().SetFilter(CharFilterSvnMsg).VSizePos(2, 2).HSizePos());
			int q = msgmap.Find(w.working);
			if(q >= 0) {
				message.Top() <<= msgmap[q];
				msgmap.Unlink(q);
			}
		}
		else
			list.Add(-1, Null, "", AttrText("Nothing to do").SetFont(StdFont().Italic()));
	}
}

void SvnSync::DoDiff(int ii)
{
	String f = list.Get(ii, 1);
	if(!IsNull(f))
		RunSvnDiff(f);
}

void SvnSync::Diff()
{
	int cr = list.GetClickRow();
	if(cr >= 0)
		DoDiff(cr);
}

#ifdef PLATFORM_WIN32
void sDeleteFolderDeep(const char *dir)
{
	{
		FindFile ff(AppendFileName(dir, "*.*"));
		while(ff) {
			String name = ff.GetName();
			String p = AppendFileName(dir, name);
			if(ff.IsFile()) {
				SetFileAttributes(p, GetFileAttributes(p) & ~FILE_ATTRIBUTE_READONLY);
				FileDelete(p);
			}
			else
			if(ff.IsFolder())
				sDeleteFolderDeep(p);
			ff.Next();
		}
	}
	DirectoryDelete(dir);
}
#else
void sDeleteFolderDeep(const char *path)
{
	DeleteFolderDeep(path);
}
#endif

void SvnDel(const char *path)
{
	FindFile ff(AppendFileName(path, "*.*"));
	while(ff) {
		if(ff.IsFolder()) {
			String dir = AppendFileName(path, ff.GetName());
			if(ff.GetName() == ".svn")
				sDeleteFolderDeep(dir);
			else
				SvnDel(dir);
		}
		ff.Next();
	}
}

void SvnSync::Dir(const char *dir, bool readonly)
{
	setup.Hide();
	works.Add(dir, Null, Null, readonly);
}

void SvnSync::FixRevision(const char *dir, int revision){
	for(int i=0;i<works.GetCount();i++)
		if(works[i].working==dir){
			works.SetRevision(i,revision);
			return;
		}
}

void SvnSync::Perform()
{
	if(!CheckSvn())
		return;
	const Vector<String>& cl = CommandLine();
	if(cl.GetCount())
		for(int i = 0; i < cl.GetCount(); i++) {
			if(cl[i] == "-") {
				works.Load(LoadFile(ConfigFile("svnworks")));
				DoSync();
				SaveFile(ConfigFile("svnworks"), works.Save());
				return;
			}
			String d = GetFullPath(cl[i]);
			if(!DirectoryExists(cl[i])) {
				Cerr() << cl[i] << " not a directory\n";
				SetExitCode(1);
				return;
			}
			works.Add(d, "", "");
		}
	else
		works.Add(GetCurrentDirectory(), "", "");
	setup.Hide();
	DoSync();
}

void MoveSvn(const String& path, const String& tp)
{
	FindFile ff(AppendFileName(path, "*.*"));
	while(ff) {
		String nm = ff.GetName();
		String s = AppendFileName(path, nm);
		String t = AppendFileName(tp, nm);
		if(ff.IsFolder()) {
			if(nm == ".svn")
				FileMove(s, t);
			else
				MoveSvn(s, t);
		}
		ff.Next();
	}
}

void SvnSync::DoSync()
{
	SyncList();
	msgmap.Sweep();
again:
	Enable();
	if(Run() != IDOK || list.GetCount() == 0) {
		int repoi = 0;
		for(int i = 0; i < list.GetCount(); i++)
			if(list.Get(i, 0) == MESSAGE)
				msgmap.GetAdd(works[repoi++].working) = list.Get(i, 3);
		return;
	}
	Disable();
	bool changes = false;
	for(int i = 0; i < list.GetCount(); i++) {
		int action = list.Get(i, 0);
		Value v = list.Get(i, 2);
		if(action == MESSAGE) {
			if(changes && IsNull(list.Get(i, 3))
			&& !PromptYesNo("Commit message is empty.&Do you want to continue?"))
				goto again;
			changes = false;
		}
		else if(action != REPOSITORY && IsNumber(v) && v==1)
			changes = true;
	}
	SysConsole sys;
	int repoi = 0;
	int l = 0;
	bool commit = false;
	while(l < list.GetCount()) {
		SvnWork w = works[repoi++];
		l++;
		String message;
		String filelist;   // <-- list of files to update
		while(l < list.GetCount()) {
			int action = list.Get(l, 0);
			String path = list.Get(l, 1);
			if(action == MESSAGE && commit) {
				String msg = list.Get(l, 3);
				// now we commit only files in filelist
				if(sys.CheckSystem(SvnCmd("commit", w).Cat() << filelist << " -m \"" << msg << "\""))
					msgmap.GetAdd(w.working) = msg;
				l++;
				break;
			}
			if(action == REPOSITORY)
				break;
			Value v = list.Get(l, 2);
			if(IsNumber(v) && (int)v == 0) {
				if(action == REPLACE || action == ADD)
					DeleteFolderDeep(path);
				if(action != ADD)
					sys.CheckSystem("svn revert \"" + path + "\"");
			}
			else
			if(IsNumber(v) && (int)v==2 || action == SVN_IGNORE) {
				l++;
				continue;
			}
			else {
				if(action != DELETEC)
					filelist << " \"" << path << "\"";   // <-- add the file to the list
				commit = true;
				switch(action) {
				case ADD:
					SvnDel(path);
					sys.CheckSystem("svn add --force \"" + path + "\"");
					break;
				case REMOVE:
					sys.CheckSystem("svn delete \"" + path + "\"");
					break;
				case CONFLICT:
					sys.CheckSystem("svn resolved \"" + path + "\"");
					break;
				case REPLACE: {
						SvnDel(path);
						String tp = AppendFileName(GetFileFolder(path), Format(Uuid::Create()));
						FileMove(path, tp);
						sys.CheckSystem(SvnCmd("update", w).Cat() << " \"" << path << "\"");
						MoveSvn(path, tp);
						sDeleteFolderDeep(path);
						FileMove(tp, path);
						Vector<String> ln = Split(Sys("svn status \"" + path + "\""), CharFilterCrLf);
						for(int l = 0; l < ln.GetCount(); l++) {
							String h = ln[l];
							if(h.GetCount() > 7) {
								String file = h.Mid(7);
								if(IsFullPath(file)) {
									h.Trim(7);
									if(h == "?      ")
										sys.CheckSystem("svn add --force \"" + file + "\"");
									if(h == "!      ")
										sys.CheckSystem("svn delete \"" + file + "\"");
								}
							}
						}
					}
					break;
				case DELETEC:
					FileDelete(path);
					break;
				}
			}
			l++;
		}
		sys.CheckSystem(SvnCmd("update", w).Cat() << w.working);
	}
	sys.Perform();
}

void SvnSync::SetMsgs(const String& s)
{
	LoadFromString(msgmap, s);
}

String SvnSync::GetMsgs()
{
	return StoreAsString(msgmap);
}

bool IsSvnDir(const String& p)
{
	if(IsNull(p))
		return false;
	if(DirectoryExists(AppendFileName(p, ".svn")) || DirectoryExists(AppendFileName(p, "_svn")))
		return true;
	String path = p;
	String path0;
	while(path != path0) {
		path0 = path;
		DirectoryUp(path);
		if(DirectoryExists(AppendFileName(path, ".svn")))
			return true;
	}
	return false;
}

#ifdef flagMAIN
GUI_APP_MAIN
{
	if(!CheckSvn())
		return;
	SvnSync ss;
	String mp = ConfigFile("usvn.msg");
	ss.SetMsgs(LoadFile(mp));
	ss.Perform();
	SaveFile(mp, ss.GetMsgs());
}
#endif
