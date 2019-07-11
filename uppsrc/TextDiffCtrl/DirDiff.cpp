#include "TextDiffCtrl.h"

namespace Upp {

DirDiffDlg::DirDiffDlg()
{
	int div = HorzLayoutZoom(4);
	int cy = dir1.GetStdSize().cy;
	int bcx = GetTextSize(t_("Compare"), StdFont()).cx * 12 / 10 + 2 * div;

	hidden.SetLabel(t_("Hidden"));
	
	added.SetColor(Green()).SetLabel(t_("New"));
	modified.SetLabel(t_("Modified"));
	removed.SetColor(Red()).SetLabel(t_("Removed"));
	
	compare.SetLabel(t_("Compare"));
	int bcy = compare.GetStdSize().cy;
	
	files_pane.Add(dir1.TopPos(0, cy).HSizePos());
	files_pane.Add(dir2.TopPos(cy + div, cy).HSizePos());
	files_pane.Add(hidden.TopPos(2 * cy + 2 * div, bcy).LeftPos(0, bcx));
	
	files_pane.Add(   added.TopPos(3 * cy + 3 * div, bcy).LeftPosZ(2, 60));
	files_pane.Add(modified.TopPos(3 * cy + 3 * div, bcy).LeftPosZ(52, 70));
	files_pane.Add( removed.TopPos(3 * cy + 3 * div, bcy).LeftPosZ(128, 80));
	
	removed = 1;
	added = 1;
	modified = 1;
	find.NullText(t_("Find (Ctrl+F)"));
	clearFind.SetLabel("X");
	clearFind.RightPosZ(1, 16).VSizePosZ(1, 1);
	find.AddChild(&clearFind);
	
	files_pane.Add(info.SetAlign(ALIGN_RIGHT).TopPos(3 * cy + 3 * div, bcy).RightPos(1, Zx(70)));
	files_pane.Add(compare.TopPos(2 * cy + 2 * div, bcy).RightPos(0, bcx));
	files_pane.Add(files.VSizePos(3 * cy + bcy + 4 * div, Zy(24)).HSizePos());
	files_pane.Add(find.BottomPosZ(4, 19).HSizePosZ());

	Add(files_diff.SizePos());
	files_diff.Set(files_pane, diff);
	files_diff.SetPos(2000);
	files_diff.SetMinPixels(0, Zx(256));
	
	Sizeable().Zoomable();
	
	seldir1.Attach(dir1);
	seldir2.Attach(dir2);
	
	seldir1.Title("First directory to compare");
	seldir2.Title("Second directory to compare");
	
	compare <<= THISBACK(Compare);
	dir1 <<= THISBACK(ClearFiles);
	dir2 <<= THISBACK(ClearFiles);
	
	modified	<< [=] { ShowResult(); };
	removed		<< [=] { ShowResult(); };
	added		<< [=] { ShowResult(); };
	find		<< [=] { ShowResult(); };
	clearFind	<< [=] { find.Clear(); ShowResult();};
	
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

	WhenIcon = [](const char *path) -> Image { return NativePathIcon(path); };
	
	Title("Compare directories");
};

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

bool DirDiffDlg::FileEqual(const String& f1, const String& f2, int& n)
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
	else
	{
		n = (in1 ? DELETED_FILE : NEW_FILE);
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

	list.Clear();
	for(int i = 0; i < f.GetCount(); i++) {
		if(pi.StepCanceled())
			break;
		String p1 = AppendFileName(~dir1, f[i]);
		String p2 = AppendFileName(~dir2, f[i]);
		int n = NORMAL_FILE;
		if(!FileEqual(p1, p2, n))
			list.Add(MakeTuple(f[i], p1, p2, n));
	}
	
	ShowResult();
}

FileList::File DirDiffDlg::MakeFile(int i)
{
	static Color cs[] = { SColorText(), SRed(), SGreen(), SRed(), SLtBlue() };
	FileList::File m;
	m.isdir = false;
	m.unixexe = false;
	m.hidden = false;
	Image icn = WhenIcon(FileExists(list[i].b) ? list[i].b : list[i].c);
	int k = list[i].d;
	if(IsNull(icn))
		icn = CtrlImg::File();
	m.icon = decode(k, FAILED_FILE, MakeImage(icn, [] (const Image& m) { return GetOver(m, DiffImg::Failed()); }),
	                   PATCHED_FILE, MakeImage(icn, [] (const Image& m) { return GetOver(m, DiffImg::Patched()); }),
	                   icn);
	m.name = list[i].a;
	m.font = decode(k, FAILED_FILE, StdFont().Strikeout().Italic(),
	                   PATCHED_FILE, StdFont().Italic(), StdFont());
	m.ink = cs[k];
	m.length = 0;
	m.time = Null;
	m.extink = m.ink;
	m.data = i;
	return m;
}

void DirDiffDlg::ShowResult()
{
	files.Clear();
	String sFind = ToLower((String)~find);
	for(int i = 0; i < list.GetCount(); i++)
	{
		int n = list[i].d;
		if((n == NORMAL_FILE && modified || n == DELETED_FILE && removed
		    || n == NEW_FILE && added || n == FAILED_FILE || n == PATCHED_FILE)
		   && ToLower(list[i].a).Find(sFind) >= 0)
			files.Add(MakeFile(i));
	}
	info = AsString(files.GetCount()) + " files";
	clearFind.Show(!IsNull(find));
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

bool Upp::DirDiffDlg::HotKey(dword key)
{
	if(key == K_CTRL_F) {
		ActiveFocus(find);
		return true;
	}
	return false;
}

};