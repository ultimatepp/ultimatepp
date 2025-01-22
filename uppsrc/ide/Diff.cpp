#include "ide.h"

struct RepoDiff : DiffDlg {
	FrameTop<ParentCtrl> pane;
	DropList r, branch;
	Button   copy_hash;
	
	String repo_dir;
	int    kind;
	bool   modal = false;
	int    line = 0;
	
	void LoadGit();
	void Load();
	void Set(const String& f);
	void SyncTitle();
	
	typedef RepoDiff CLASSNAME;
	
	RepoDiff();
};


void RepoDiff::SyncTitle()
{
	String h = GetFileName(editfile);
	String m = ~r.GetValue();
	int q = m.FindAfter("@b \1");
	if(q >= 0) {
		m = m.Mid(q);
		q = m.Find('\1');
		if(q >= 0)
			h << " " << m.Mid(0, q);
	}
	Title(h);
}

void RepoDiff::Set(const String& f)
{
	repo_dir = f;
	kind = GetRepo(repo_dir);
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
		     << r.HSizePos(Zx(100) + DPI(2), Zx(80) + DPI(2)).VSizePos()
		     << copy_hash.RightPos(0, Zx(80)).VSizePos();
		copy_hash.SetLabel("Copy Hash");
		
		copy_hash << [=] {
			String h = ~~r;
			String commit, path;
			SplitTo(h, ':', commit, path);
			WriteClipboardText(commit);
		};

		LoadBranches(branch, GetFileFolder(f));
		LoadGit();
	}

	DiffDlg::Set(f);
	
	SyncTitle();
}

void LoadBranches(DropList& branch, const String& dir)
{
	branch.Clear();
	String branches = GitCmd(dir, "branch --all");
	StringStream ss(branches);
	String author, date, commit;
	int ci = -1;
	auto Add = [&](const String& l) {
		String s = l.Mid(2);
		int q = s.ReverseFind('/');
		if(q >= 0)
			branch.Add(s, s.Mid(q));
		else
			branch.Add(s);
	};
	while(!ss.IsEof()) {
		String l = ss.GetLine();
		if(l.StartsWith("* ")) {
			ci = branch.GetCount();
			Add(l);
		}
		if(l.StartsWith("  "))
			Add(l);
	}

	if(ci >= 0)
		branch.SetIndex(ci);
}

void LoadGitRevisions(DropList& r, const String& dir, const String& branch, const String& file)
{
	String gitcmd = "log --format=medium --date=short --name-only ";
	if(file.GetCount())
		gitcmd << " --follow ";
	gitcmd << branch;
	if(file.GetCount())
		gitcmd << " -- " << GetFileName(file);
	String log = GitCmd(dir, gitcmd);
	StringStream ss(log);
	String author, date, commit, path, msg;
	r.ClearList();
	auto AddCommit = [&] {
		if(commit.GetCount()) {
			String h = commit;
			if(h.GetCount() > 6)
				h.Trim(6);
			r.Add(IsNull(file) ? commit : commit + ":" + path,
			      "\1[g [@b \1" + date + "\1] [@g \1" + h + "\1] [@r \1" + author + "\1]: "
			      "[* \1" + Join(Split(msg, CharFilterWhitespace), " "));
		}
		date = commit = author = msg = Null;
	};
	while(!ss.IsEof()) {
		String l = TrimBoth(ss.GetLine());
		if(l.GetCount() == 0 && msg.GetCount() == 0) {
			while(!ss.IsEof()) {
				l = ss.GetLine();
				if(l.GetCount())
					break;
			}
			msg = l;
			while(!ss.IsEof()) {
				l = ss.GetLine();
				if(l.GetCount() == 0)
					break;
				msg << l;
			}
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
		if(l.TrimStart("commit")) {
			AddCommit();
			commit = TrimBoth(l);
		}
		else
		if(l.GetCount())
			path = l;
	}
	AddCommit();
	
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
	Vector<TextCompareCtrl::Blame> bl;
	if(kind == GIT_DIR) {
		String h = ~~r;
		String commit, path;
		SplitTo(h, ':', commit, path);
		if(path.GetCount())
			extfile = GitCmd(repo_dir, "show " + h);
		else
			extfile = GitCmd(GetFileFolder(editfile), "show " + commit + ":./" + GetFileName(editfile));
	#if 1
		extfile.Clear();
		StringStream ss(GitCmd(GetFileFolder(editfile), "blame -p " + GetFileName(editfile) + " " + commit));
		VectorMap<String, TextCompareCtrl::Blame> blame;
		while(!ss.IsEof()) {
			String ln = ss.GetLine();
			Vector<String> h = Split(ln, ' ');
			if(h.GetCount() == 0) break;
			String hash = h[0];
			TextCompareCtrl::Blame& b = blame.GetAdd(hash);
			while(!ss.IsEof()) {
				String ln = ss.GetLine();
				if(ln.TrimStart("\t")) {
					auto& m = bl.Add();
					m = b;
					m.hash = hash;
					extfile << ln << "\n";
					break;
				}
				if(ln.TrimStart("author "))
					b.author = ln;
				else
				if(ln.TrimStart("summary "))
					b.summary = ln;
				else
				if(ln.TrimStart("author-time "))
					b.time = Time(1970, 1, 1) + Atoi(ln);
			}
		}
	#endif
	}
	
	diff.Set(backup = LoadFile(editfile), extfile);
	if(bl.GetCount()) {
		diff.SetPos(4700);
		diff.right.WhenBlame = [=](const String& hash) {
			auto FindHash = [&](RepoDiff& d) {
				for(int i = 0; i < d.r.GetCount(); i++) {
					String h = d.r.GetKey(i);
					String commit, path;
					SplitTo(h, ':', commit, path);
					if(commit == hash) {
						d.r.SetIndex(i);
						d.Load();
						break;
					}
				}
			};
			if(modal)
				FindHash(*this);
			else {
				RepoDiff *rd = TheIde()->RunRepoDiff(editfile, line);
				if(rd)
					FindHash(*rd);
			}
		};
	}
	diff.right.PickBlame(pick(bl));
	SyncTitle();
}

RepoDiff::RepoDiff()
{
	pane.Height(EditField::GetStdHeight());
	r.SetDropLines(32);
	branch.SetDropLines(32);
	Icon(IdeImg::SvnDiff());
	diff.InsertFrameRight(pane);
	r << [=] { Load(); };
	branch << [=] { LoadGit(); };
	Sizeable().Zoomable();
	serialize_placement = false;
	Rect r = TheIde()->GetWorkArea();
	r.Deflate(DPI(30), DPI(40));
	SetRect(r);
}

void RunRepoDiff(const String& filepath, int line)
{
	if(IsNull(filepath))
		return;
	RepoDiff dlg;
	dlg.modal = true;
	dlg.Set(filepath);
	if(line >= 0)
		dlg.diff.left.SetCursor(line + 1);
	dlg.Execute();
}

RepoDiff *Ide::RunRepoDiff(const String& filepath, int line)
{
	if(IsNull(filepath))
		return nullptr;
	RepoDiff& dlg = CreateNewWindow<RepoDiff>();
	dlg.line = line;
	dlg.Set(filepath);
	if(line >= 0)
		dlg.diff.left.SetCursor(line + 1);
	dlg.diff.WhenRightLine =
	dlg.diff.WhenLeftLine = [=](int line) {
		EditFile(filepath);
		editor.SetCursor(editor.GetPos64(line));
		editor.SetFocus();
	};
	dlg.OpenMain();
	return &dlg;
}
