#include "TextDiffCtrl.h"

namespace Upp {

void DirDiffDlg::GatherFilesDeep(Index<String>& files, const String& base, const String& path)
{
	FindFile ff(AppendFileName(AppendFileName(base, path), "*.*"));
	while(ff) {
		String p = (path.GetCount() ? path + '/' : String()) + ff.GetName();
		if(hidden || !ff.IsHidden()) {
			if(ff.IsFile())
				files.FindAdd(p);
			else
			if(ff.IsFolder())
				GatherFilesDeep(files, base, p);
		}
		ff.Next();
	}
}

bool FileEqual(const String& f1, const String& f2)
{
	FileIn in1(f1);
	FileIn in2(f2);
	if(in1 && in2) {
		while(!in1.IsEof() && !in2.IsEof()) {
			String a = in1.Get(256 * 1024);
			String b = in2.Get(256 * 1024);
			if(a != b)
				return false;
		}
		return true;
	}
	return false;
}

void DirDiffDlg::Compare()
{
	Index<String> fs;
	GatherFilesDeep(fs, ~dir1, Null);
	GatherFilesDeep(fs, ~dir2, Null);

	copyleft.Disable();
	copyright.Disable();
	
	files.Clear();
	Vector<String> f = fs.PickKeys();
	Sort(f);
	Progress pi(t_("Comparing.."));
	pi.SetTotal(f.GetCount());
	for(int i = 0; i < f.GetCount(); i++) {
		if(pi.StepCanceled())
			break;
		String p1 = AppendFileName(~dir1, f[i]);
		String p2 = AppendFileName(~dir2, f[i]);
		if(!FileEqual(p1, p2))
			files.Add(f[i], NativePathIcon(FileExists(p1) ? p1 : p2));
		info = AsString(files.GetCount()) + " files";
	}
}

void DirDiffDlg::ClearFiles()
{
	files.Clear();
	compare.Enable(!IsNull(dir1) && !IsNull(dir2));
}

void DirDiffDlg::File()
{
	String fn = files.GetCurrentName();
	String p1 = AppendFileName(~dir1, fn);
	String p2 = AppendFileName(~dir2, fn);
	diff.Set(Null, Null);
	if(GetFileLength(p1) < 4 * 1024 * 1024 && GetFileLength(p2) < 4 * 1024 * 1024)
		diff.Set(LoadFile(p1), LoadFile(p2));
	lfile <<= p1;
	rfile <<= p2;
	copyleft.Enable();
	copyright.Enable();
}

void DirDiffDlg::Copy(bool left)
{
	String src = ~lfile;
	String dst = ~rfile;
	if(left)
		Swap(src, dst);
	if(PromptYesNo("Copy [* \1" + src + "\1]&to [* \1" + dst + "\1] ?")) {
		FileIn  in(src);
		FileOut out(dst);
		CopyStream(out, in);
		out.Close();
		File();
	}
}

DirDiffDlg::DirDiffDlg()
{
	int div = HorzLayoutZoom(4);
	int cy = dir1.GetStdSize().cy;
	int bcx = GetTextSize(t_("Compare"), StdFont()).cx * 12 / 10 + 2 * div;

	hidden.SetLabel(t_("Hidden"));
	compare.SetLabel(t_("Compare"));
	int bcy = compare.GetStdSize().cy;
	
	files_pane.Add(dir1.TopPos(0, cy).HSizePos());
	files_pane.Add(dir2.TopPos(cy + div, cy).HSizePos());
	files_pane.Add(hidden.TopPos(2 * cy + 2 * div, bcy).LeftPos(0, bcx));
	files_pane.Add(info.TopPos(2 * cy + 2 * div, bcy).HSizePos(bcx + 2 * div, bcx + div));
	files_pane.Add(compare.TopPos(2 * cy + 2 * div, bcy).RightPos(0, bcx));
	files_pane.Add(files.VSizePos(2 * cy + bcy + 3 * div, 0).HSizePos());

	Add(files_diff.SizePos());
	files_diff.Set(files_pane, diff);
	files_diff.SetPos(2000);
	
	Sizeable().Zoomable();
	
	seldir1.Attach(dir1);
	seldir2.Attach(dir2);
	
	compare <<= THISBACK(Compare);
	dir1 <<= THISBACK(ClearFiles);
	dir2 <<= THISBACK(ClearFiles);
	
	files.WhenSel = THISBACK(File);

	diff.InsertFrameLeft(left);
	diff.InsertFrameRight(right);

	bcx = GetTextSize(t_("Copy"), StdFont()).cx + HorzLayoutZoom(40);

	left.Height(EditField::GetStdHeight());
	lfile.SetReadOnly();
	left.Add(lfile.VSizePos().HSizePos(0, bcx));
	left.Add(copyright.VSizePos().RightPos(0, bcx));
	copyright.SetImage(DiffImg::CopyRight());
	copyright.SetLabel("Copy");
	copyright <<= THISBACK1(Copy, false);

	right.Height(EditField::GetStdHeight());
	rfile.SetReadOnly();
	right.Add(rfile.VSizePos().HSizePos(bcx, 0));
	right.Add(copyleft.VSizePos().LeftPos(0, bcx));
	copyleft.SetImage(DiffImg::CopyLeft());
	copyleft.SetLabel("Copy");
	copyleft <<= THISBACK1(Copy, true);
	
	copyleft.Disable();
	copyright.Disable();

	Icon(DiffImg::DirDiff());
}

};
