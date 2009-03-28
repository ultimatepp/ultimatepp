#include "usvn.h"

#define IMAGECLASS UsvnImg
#define IMAGEFILE  "usvn.iml"
#include <Draw/iml.h>

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
	diff.InsertFrameLeft(p);
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
	
	virtual void Open();
	void Execute(const String& f);
	
	typedef FileDiff CLASSNAME;
	
	FileDiff(FileSel& fs);
	
	FileSel& fs;
};

void FileDiff::Open()
{
	if(!fs.ExecuteOpen())
		return;
	String f = ~fs;
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

FileDiff::FileDiff(FileSel& fs_)
: fs(fs_)
{
	r.Height(EditField::GetStdHeight());
	Icon(UsvnImg::Diff());
	diff.InsertFrameRight(r);
	r <<= THISBACK(Open);
}

void RunFileDiff(String editfile, FileSel& fs)
{
	if(IsNull(editfile))
		return;
	FileDiff dlg(fs);
	dlg.Execute(editfile);
}

FileSel& DiffFs() {
	static FileSel fs;
	ONCELOCK {
		fs.Type("Patch file (*.diff, *.patch)", "*.diff *.patch");
		fs.AllFilesType();
	}
	return fs;
}

struct PatchDiff : FileDiff {
	PatchDiff(FileSel& fs) : FileDiff(fs) {}
	
	virtual void Open();
	
	void Copy(FileIn& in, FileIn& oin, int& l, int ln, int n);
	void LoadDiff(const char *fn);
};

void PatchDiff::Copy(FileIn& in, FileIn& oin, int& l, int ln, int n)
{
	if(ln < l)
		throw CParser::Error("");
	while(l < ln) {
		if(oin.IsEof())
			throw CParser::Error("");
		extfile << oin.GetLine() << "\r\n";
		l++;
	}
	l += n;
	while(n--)
		oin.GetLine();
}

void PatchDiff::LoadDiff(const char *fn)
{
	try {
		FileIn in(fn);
		FileIn oin(editfile);
		extfile.Clear();
		int l = 1;
		String s = in.GetLine();
		if(IsDigit(*s)) {
			in.Seek(0);
			while(!in.IsEof()) {
				s = in.GetLine();
				if(IsDigit(*s)) {
					CParser p(s);
					int ln = p.ReadNumber();
					int n = 0;
					if(p.Char('a'))
						ln++;
					else {
						n = p.Char(',') ? p.ReadNumber() - ln + 1 : 1;
						if(!p.Char('c'))
							p.PassChar('d');
					}
					Copy(in, oin, l, ln, n);
				}
				else
				if(*s == '>') {
					if(s[1] != ' ')
						throw CParser::Error("");
					extfile << s.Mid(2) << "\r\n";
				}
				else
				if(*s != '<' && *s != '-')
					throw CParser::Error("");
			}
		}
		else {
			for(;;) {
				if(in.IsEof())
					throw CParser::Error("");
				if(in.GetLine().Mid(0, 4) == "+++ ")
					break;
			}
			while(!in.IsEof()) {
				String s = in.GetLine();
				if(*s == '@') {
					CParser p(s);
					p.PassChar2('@', '@');
					p.PassChar('-');
					int ln = p.ReadNumber();
					int n = 1;
					if(p.Char(','))
						n = p.ReadNumber();
					Copy(in, oin, l, ln, n);
				}
				else
				if(*s == '+' || *s == ' ')
					extfile << s.Mid(1) << "\r\n";
				else
				if(*s != '-')
					throw CParser::Error("");
			}
		}
		while(!oin.IsEof())
			extfile << oin.GetLine() << "\r\n";
	}
	catch(CParser::Error&) {
		Exclamation("Invalid file format!");
		extfile = LoadFile(fn);
	}
}

void PatchDiff::Open()
{
	if(!DiffFs().ExecuteOpen())
		return;
	String f = ~DiffFs();
	r <<= f;
	LoadDiff(f);
	diff.Set(LoadFile(editfile), extfile);
}

void RunPatchDiff(String editfile, FileSel& fs)
{
	if(IsNull(editfile))
		return;
	PatchDiff dlg(fs);
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
	diff.Set(LoadFile(editfile), extfile = Sys("svn cat " + editfile + '@' + AsString(~r)));
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
	if(IsNull(editfile))
		return;
	SvnDiff dlg;
	dlg.Execute(editfile);
}
