#include "ide.h"

#define IMAGECLASS UrepoImg
#define IMAGEFILE  <ide/urepo.iml>
#include <Draw/iml.h>

struct RepoDiff : DiffDlg {
	FrameTop<ParentCtrl> pane;
	DropList r, branch;
	
	int  kind;
	
	void LoadGit();
	void Load();
	void Set(const String& f);
	
	typedef RepoDiff CLASSNAME;
	
	RepoDiff();
};

void RepoDiff::Set(const String& f)
{
	kind = GetRepoKind(f);
	editfile = f;
	if(kind == SVN_DIR) {
		pane << r.SizePos();
		WaitCursor wait;
		String log = HostSys("svn log " + f);
		if(log.IsVoid()) {
			Exclamation("Error executing 'svn log'");
			return;
		}
		StringStream ss(log);
		while(!ss.IsEof()) {
			String l = ss.GetLine();
			if(l[0] == 'r') {
				Vector<String> h = Split(l, '|');
				if(h.GetCount() > 3) {
					String rev = TrimBoth(h[0]);
					String s = rev;
					Vector<String> t = Split(h[2], ' ');
					if(t.GetCount() > 1)
						s << ' ' << t[0];
					s << ' ' << TrimBoth(h[1]);
					while(!ss.IsEof()) {
						l = ss.GetLine();
						if(l.GetCount()) {
							if(l[0] != '-')
								s << ": " << l;
							break;
						}
					}
					r.Add(rev, s);
				}
			}
		}
		if(r.GetCount() == 0) {
			Exclamation("No parsable history for the file");
			return;
		}

		if(r.GetCount())
			r.SetIndex(0);
		Load();
	}

	if(kind == GIT_DIR) {
		pane << branch.LeftPos(0, Zx(100)).VSizePos()
		     << r.HSizePos(Zx(100) + DPI(2), 0).VSizePos();

		LoadBranches(branch, GetFileFolder(f));
		LoadGit();
	}

	DiffDlg::Set(f);
}

void LoadBranches(DropList& branch, const String& dir)
{
	String branches = GitCmd(dir, "branch");
	StringStream ss(branches);
	String author, date, commit;
	int ci = -1;
	while(!ss.IsEof()) {
		String l = ss.GetLine();
		if(l.StartsWith("* ")) {
			ci = branch.GetCount();
			branch.Add(l.Mid(2));
		}
		if(l.StartsWith("  "))
			branch.Add(l.Mid(2));
	}

	if(ci >= 0)
		branch.SetIndex(ci);
}

void LoadGitRevisions(DropList& r, const String& dir, const String& branch, const String& file)
{
	String gitcmd = "log --format=medium --date=short " + branch;
	if(file.GetCount())
		gitcmd << " -- " << GetFileName(file);
	String log = GitCmd(dir, gitcmd);
	StringStream ss(log);
	String author, date, commit;
	r.ClearList();
	while(!ss.IsEof()) {
		String l = TrimBoth(ss.GetLine());
		if(l.GetCount() == 0) {
			while(!ss.IsEof()) {
				l = ss.GetLine();
				if(l.GetCount())
					break;
			}
			String msg = l;
			while(!ss.IsEof()) {
				l = ss.GetLine();
				if(l.GetCount() == 0)
					break;
				msg << l;
			}
			if(commit.GetCount())
				r.Add(commit, "\1[g [@b \1" + date + "\1] [@r \1" + author + "\1]: [* \1" + Join(Split(msg, CharFilterWhitespace), " "));
			date = commit = author = Null;
		}
		else
		if(l.TrimStart("Author:")) {
			int q = l.Find('<');
			author = TrimBoth(q >= 0 ? l.Mid(0, q) : l);
		}
		else
		if(l.TrimStart("Date:"))
			date = TrimBoth(l);
		else
		if(l.TrimStart("commit"))
			commit = TrimBoth(l);
	}
	
	if(r.GetCount())
		r.SetIndex(0);
}

void RepoDiff::LoadGit()
{
	LoadGitRevisions(r, GetFileFolder(editfile), ~~branch, editfile);
	Load();
}

void RepoDiff::Load()
{
	if(IsNull(r))
		return;
	if(kind == SVN_DIR)
		extfile = HostSys("svn cat " + editfile + '@' + AsString(~r));
	if(kind == GIT_DIR)
		extfile = GitCmd(GetFileFolder(editfile), "show " + ~~r + ":./" + GetFileName(editfile));
	diff.Set(backup = LoadFile(editfile), extfile);
}

RepoDiff::RepoDiff()
{
	pane.Height(EditField::GetStdHeight());
	r.SetDropLines(32);
	branch.SetDropLines(32);
	Icon(UrepoImg::RepoDiff());
	diff.InsertFrameRight(pane);
	r << [=] { Load(); };
	branch << [=] { LoadGit(); };
	Sizeable().Zoomable();
	Rect r = TheIde()->GetWorkArea();
	r.Deflate(DPI(30), DPI(30));
	SetRect(r);
}

void RunRepoDiff(const String& filepath)
{
	if(IsNull(filepath))
		return;
	RepoDiff dlg;
	dlg.Set(filepath);
	dlg.Execute();
}

void Ide::RunRepoDiff(const String& filepath)
{
	if(IsNull(filepath))
		return;
	RepoDiff& dlg = CreateNewWindow<RepoDiff>();
	dlg.Set(filepath);
	dlg.diff.WhenRightLine =
	dlg.diff.WhenLeftLine = [=](int line) {
		EditFile(filepath);
		editor.SetCursor(editor.GetPos64(line));
		editor.SetFocus();
	};
	dlg.OpenMain();
}
