#include "usvn.h"

SvnSync::SvnSync()
{
	CtrlLayoutOKCancel(*this, "SvnSynchronize SVN repositories");
	list.AddIndex();
	list.AddIndex();
	list.AddColumn("Action");
	list.AddColumn("Path");
	list.ColumnWidths("153 619");
	list.NoCursor().EvenRowColor();
	usr.NullText("use cached");
	pwd.NullText("use cached");
	Sizeable().Zoomable();
	setup <<= THISBACK(Setup);
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
		         AttrText(path).SetFont(Arial(20).Bold()).Paper(Blue).Ink(White));
		list.SetLineCy(list.GetCount() - 1, 26);
		Vector<String> ln = Split(Sys("svn status " + path), CharFilterCrLf);
		bool actions = false;
		for(int pass = 0; pass < 2; pass++)
			for(int i = 0; i < ln.GetCount(); i++) {
				String h = ln[i];
				if(h.GetCount() > 7) {
					String file = h.Mid(7);
					if(IsFullPath(file)) {
						actions = true;
						h.Trim(7);
						bool simple = h.Mid(1, 6) == "      ";
						int action = simple ? String("MC?!").Find(h[0]) : -1;
						String an;
						Color  color;
						if(action < 0) {
							color = Black;
							if(simple && h[0] == 'A')
								an = "svn add";
							else
							if(simple && h[0] == 'D')
								an = "svn delete";
							else {
								an = h.Mid(0, 7);
								color = Gray;
							}
						}
						else {
							static const char *as[] = { "Modify", "Conflict resolved", "Add", "Remove" };
							static Color c[] = { LtBlue, Magenta, Green, LtRed };
							an = as[action];
							color = c[action];
						}
						if(pass == action < 0) {
							int ii = list.GetCount();
							list.Add(action, file,
							         action <= 0 ? Value(AttrText(an).Ink(color)) : Value(true),
							         AttrText("  " + file.Mid(path.GetCount() + 1)).Ink(color));
							if(action > 0)
								list.SetCtrl(ii, 0, confirm.Add().SetLabel(an).NoWantFocus());
						}
					}
				}
		}
		if(actions) {
			list.Add(MESSAGE, Null, AttrText("Commit message:").SetFont(StdFont().Bold()));
			list.SetLineCy(list.GetCount() - 1, EditField::GetStdHeight() + 4);
			list.SetCtrl(list.GetCount() - 1, 1, message.Add().SetFilter(CharFilterSvnMsg));
		}
		else
			list.Add(-1, Null, "", AttrText("Nothing to do").SetFont(StdFont().Italic()));
	}
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

void SvnSync::Dir(const char *dir)
{
	setup.Hide();
	works.Add(dir, Null, Null);
}

void SvnSync::Perform()
{
	const Vector<String>& cl = CommandLine();
	if(cl.GetCount()) {
		for(int i = 0; i < cl.GetCount(); i++) {
			String d = GetFullPath(cl[i]);
			if(!DirectoryExists(cl[i])) {
				Cerr() << cl[i] << " not a directory\n";
				SetExitCode(1);
				return;
			}
			works.Add(d, ~usr, ~pwd);
		}
		setup.Hide();
		DoSync();
	}
	else {
		usr_lbl.Hide();
		usr.Hide();
		pwd_lbl.Hide();
		pwd.Hide();
		works.Load(LoadFile(ConfigFile("svnworks")));
		DoSync();
		SaveFile(ConfigFile("svnworks"), works.Save());
	}
}

void SvnSync::DoSync()
{
	SyncList();
	if(Execute() != IDOK || list.GetCount() == 0)
		return;
	SysConsole sys;
	int repoi = 0;
	int i = 0;
	while(i < list.GetCount()) {
		SvnWork w = works[repoi++];
		i++;
		String message;
		while(i < list.GetCount()) {
			int action = list.Get(i, 0);
			String path = list.Get(i, 1);
			if(action == MESSAGE) {
				sys.System(SvnCmd("commit", w).Cat() << w.working << " -m \"" << list.Get(i, 3) << "\"");
				i++;
				break;
			}
			if(action == REPOSITORY)
				break;
			switch(action) {
			case ADD:
				SvnDel(path);
				sys.System("svn add " + path);
				break;
			case REMOVE:
				sys.System("svn delete " + path);
				break;
			case CONFLICT:
				sys.System("svn resolved " + path);
				break;
			}
			i++;
		}
		sys.System(SvnCmd("update", w).Cat() << w.working);
	}
	sys.Perform();
}

#ifdef flagMAIN
GUI_APP_MAIN
{
//	SvnSel svn;
//	svn.Select();
//	svn.Select("svn://10.0.0.19/upp", "", "");
//	return;

	SvnSync().Perform();
}
#endif
