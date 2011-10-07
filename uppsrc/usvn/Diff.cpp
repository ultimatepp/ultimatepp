#include "usvn.h"

#define IMAGECLASS UsvnImg
#define IMAGEFILE  <usvn/usvn.iml>
#include <Draw/iml.h>

struct SvnDiff : DiffDlg {
	FrameTop<DropList> r;
	
	void Load();
	void Execute(const String& f);
	
	typedef SvnDiff CLASSNAME;
	
	SvnDiff();
};

void SvnDiff::Execute(const String& f)
{
	{
		WaitCursor wait;
		editfile = f;
		String log = Sys("svn log " + f);
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
		r.SetIndex(0);
		Load();
	}
	DiffDlg::Execute(f);
}

void SvnDiff::Load()
{
	diff.Set(LoadFile(editfile), extfile = Sys("svn cat " + editfile + '@' + AsString(~r), false));
}

SvnDiff::SvnDiff()
{
	r.Height(EditField::GetStdHeight());
	r.SetDropLines(32);
	Icon(UsvnImg::SvnDiff());
	diff.InsertFrameRight(r);
	r <<= THISBACK(Load);
}

void RunSvnDiff(String editfile)
{
	if(!CheckSvn())
		return;
	if(IsNull(editfile))
		return;
	SvnDiff dlg;
	dlg.Execute(editfile);
}
