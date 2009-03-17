#include "ide.h"

struct DiffDlg : public TopWindow {
	TextDiffCtrl         diff;
	FrameTop<StaticRect> p;
	DataPusher           l;
	Button               write;
	String               editfile;
	String               extfile;
	
	typedef DiffDlg CLASSNAME;
	
	void Write();
	void Execute(const String& f);
	
	DiffDlg();
};

INITBLOCK {
	RegisterGlobalConfig("diff");
}

void DiffDlg::Execute(const String& f)
{
	editfile = f;
	l <<= editfile;
	Title(editfile);
	String h;
	{
		LoadFromGlobal(h, "diff");
		StringStream ss(h);
		SerializePlacement(ss);
	}
	TopWindow::Execute();
	{
		StringStream ss;
		SerializePlacement(ss);
		h = ss;
		StoreToGlobal(h, "diff");
	}
}

void DiffDlg::Write()
{
	if(PromptYesNo("Do you want to overwrite&[* " + DeQtf(editfile) + "] ?")) {
		SaveFile(editfile, extfile);
		Break(IDOK);
	}
}

DiffDlg::DiffDlg()
{
	Add(diff.SizePos());
	Sizeable().Zoomable();
	diff.AddFrameLeft(p);
	int cy = EditField::GetStdHeight();
	p.Height(cy);
	p.Add(l.VSizePos().HSizePos(0, 6 * cy));
	p.Add(write.VSizePos().RightPos(0, 6 * cy));
	write <<= THISBACK(Write);
	write.SetLabel("Overwrite <-");
	l.SetReadOnly();
}

struct FileDiff : DiffDlg {
	FrameTop<DataPusher> r;
	
	void Open();
	void Execute(const String& f);
	
	typedef FileDiff CLASSNAME;
	
	FileDiff();
};

void FileDiff::Open()
{
	if(!AnySourceFs().ExecuteOpen())
		return;
	String f = ~AnySourceFs();
	r <<= f;
	diff.Set(LoadFile(editfile), extfile = LoadFile(f));
}

void FileDiff::Execute(const String& f)
{
	editfile = f;
	Open();
	if(IsNull(r))
		return;
	DiffDlg::Execute(f);
}

FileDiff::FileDiff()
{
	r.Height(EditField::GetStdHeight());
	Icon(IdeImg::Diff());
	diff.AddFrameRight(r);
	r <<= THISBACK(Open);
}

void Ide::Diff()
{
	if(IsNull(editfile))
		return;
	FileDiff dlg;
	dlg.Execute(editfile);
}

struct SvnDiff : DiffDlg {
	FrameTop<DropList> r;
	
	void Load();
	void Execute(const String& f);
	
	typedef SvnDiff CLASSNAME;
	
	SvnDiff();
};

void SvnDiff::Execute(const String& f)
{
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
	DiffDlg::Execute(f);
}

void SvnDiff::Load()
{
	diff.Set(LoadFile(editfile), extfile = Sys("svn cat " + editfile + '@' + AsString(~r)));
}

SvnDiff::SvnDiff()
{
	r.Height(EditField::GetStdHeight());
	r.SetDropLines(32);
	Icon(IdeImg::Diff());
	diff.AddFrameRight(r);
	r <<= THISBACK(Load);
}

void Ide::SvnHistory()
{
	if(IsNull(editfile))
		return;
	SvnDiff dlg;
	dlg.Execute(editfile);	
}
