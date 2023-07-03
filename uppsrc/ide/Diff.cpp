#include "ide.h"

#define IMAGECLASS UrepoImg
#define IMAGEFILE  <ide/urepo.iml>
#include <Draw/iml.h>

struct RepoDiff : DiffDlg {
	FrameTop<DropList> r;
	
	int  kind;
	
	void Load();
	void Execute(const String& f);
	
	typedef RepoDiff CLASSNAME;
	
	RepoDiff();
};

void RepoDiff::Execute(const String& f)
{
	kind = GetRepoKind(f);
	editfile = f;
	if(kind == SVN_DIR) {
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
	}

	if(kind == GIT_DIR) {
		String log = GitCmd(GetFileFolder(f), "log --format=medium --date=short " + GetFileName(f));
		StringStream ss(log);
		String author, date, commit;
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
		Load();
	}

	if(r.GetCount() == 0) {
		Exclamation("No parsable history for the file");
		return;
	}

	r.SetIndex(0);
	Load();
	
	DiffDlg::Execute(f);
}

void RepoDiff::Load()
{
	if(kind == SVN_DIR)
		extfile = HostSys("svn cat " + editfile + '@' + AsString(~r));
	if(kind == GIT_DIR)
		extfile = GitCmd(GetFileFolder(editfile), "show " + ~~r + ":./" + GetFileName(editfile));
	diff.Set(backup = LoadFile(editfile), extfile);
}

RepoDiff::RepoDiff()
{
	r.Height(EditField::GetStdHeight());
	r.SetDropLines(32);
	Icon(UrepoImg::RepoDiff());
	diff.InsertFrameRight(r);
	r <<= THISBACK(Load);
}

void RunRepoDiff(const String& filepath)
{
	if(IsNull(filepath))
		return;
	RepoDiff dlg;
	dlg.Execute(filepath);
}
