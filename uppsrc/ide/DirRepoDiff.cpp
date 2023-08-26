#include "ide.h"

DirRepoDiffDlg::DirRepoDiffDlg()
{
	int div = HorzLayoutZoom(4);
	int cy = dir1.GetStdSize().cy;
	int lcy = cy + div;
	int bcx = GetTextSize(t_("Compare"), StdFont()).cx * 12 / 10 + 2 * div;
	int bcy = max(cy, compare.GetStdSize().cy);
	int y = div;

	Index<String> git;

	for(String d : GetUppDirs())
		if(GetRepo(d) == GIT_DIR)
			git.FindAdd(d);

	ForAllNests([&](const Vector<String>& nests) {
		for(String d : nests) {
			if(GetRepo(d) == GIT_DIR)
				git.FindAdd(d);
		}
	});

	for(int i = 0; i < 2; i++) {
		DropList& l = mode[i];
		l << [=] { Mode(i); };

		l.Add(Null, AttrText("Directory").NormalInk(LtBlue()).Italic());
		for(String s : git)
			l.Add(s, "git " + s);
		AddSelectGit(l);
		
		branch[i] << [=] { Revs(i); };
	};

	files_pane.Add(mode[0].TopPos(y, cy).HSizePos());
	y += lcy;
	files_pane.Add(dir1.TopPos(y, cy).HSizePos());
	files_pane << branch[0].HSizePos().TopPos(y, cy);
	y += lcy;
	files_pane << r[0].HSizePos().TopPos(y, cy);
	y += lcy;

	y += lcy / 2;

	files_pane.Add(mode[1].TopPos(y, cy).HSizePos());
	y += lcy;
	files_pane.Add(dir2.TopPos(y, cy).HSizePos());
	files_pane << branch[1].HSizePos().TopPos(y, cy);
	y += lcy;
	files_pane << r[1].HSizePos().TopPos(y, cy);
	y += lcy;

	y += lcy / 2;

	files_pane.Add(hidden.TopPos(y, bcy).LeftPos(0, bcx));
	files_pane.Add(split_lines.TopPos(y, bcy).LeftPosZ(52, 100));
	files_pane.Add(compare.TopPos(y, bcy).RightPos(0, bcx));
	y += bcy + div;

	files_pane.Add(added.TopPos(y, bcy).LeftPosZ(2, 60));
	files_pane.Add(modified.TopPos(y, bcy).LeftPosZ(52, 70));
	files_pane.Add(removed.TopPos(y, bcy).LeftPosZ(128, 80));
	files_pane.Add(recent.TopPos(y, bcy).RightPos(0, bcx));
	y += bcy + div;

	files_pane.Add(files.VSizePos(y, Zy(24)).HSizePos());
	files_pane.Add(find.BottomPosZ(6, 19).HSizePosZ());

	Mode(0);
	Mode(1);
	
	compare ^= [=] { Compare(); };
	
	dir1 << [=] { SyncCompare(); };
	dir2 << [=] { SyncCompare(); };
}

DirRepoDiffDlg::~DirRepoDiffDlg()
{
	Cleanup();
}

void DirRepoDiffDlg::AddSelectGit(DropList& dl)
{
	dl.Add(0, AttrText("Select git repo").Italic().NormalInk(Magenta()));
}

void DirRepoDiffDlg::Mode(int i)
{
	DropList& dl = mode[i];
	if(~dl == 0) {
		String dir = SelectDirectory();
		if(GetRepo(dir) == GIT_DIR) {
			if(dl.FindKey(dir) < 0) {
				dl.Trim(dl.GetCount() - 1);
				dl.Add(dir);
				AddSelectGit(dl);
			}
			dl <<= dir;
		}
		else {
			if(!IsNull(~dl))
				Exclamation("Not a git repo");
			dl <<= Null;
		}
	}

	bool b = IsNull(dl);
	(i ? dir2 : dir1).Show(b);
	branch[i].Show(!b);
	r[i].Show(!b);

	if(!b) {
		LoadBranches(branch[i], ~~dl);
		Revs(i);
	}
	
	ClearFiles();
	SyncCompare();
}

void DirRepoDiffDlg::Revs(int i)
{
	if(IsString(~mode[i]) && !IsNull(mode[i]))
		LoadGitRevisions(r[i], ~~mode[i], ~~branch[i], Null);
}

void DirRepoDiffDlg::Cleanup()
{
	for(String d : gitd)
		DeleteFolderDeep(d);
	gitd.Clear();
}

void DirRepoDiffDlg::SyncCompare()
{
	compare.Enable((!IsNull(dir1) || !IsNull(mode[0]) && IsString(mode[0])) &&
	               (!IsNull(dir2) || !IsNull(mode[1]) && IsString(mode[1])));
}

void DirRepoDiffDlg::Compare()
{
	Cleanup();
	Progress pi;
	auto Clone = [&](int i, EditString& es)->bool {
		if(!IsNull(mode[i]) && IsString(mode[i])) {
			String d = CacheFile(String() << "git-" << Random() << Random() << Random());
			gitd << d;
			if(!CopyFolder(d, ~~mode[i], &pi))
				return false;
			GitCmd(d, "checkout " + ~~r[i]);
			es <<= d;
			return true;
		}
		return true;
	};
	pi.Close();
	if(Clone(0, dir1) && Clone(1, dir2))
		DirDiffDlg::Compare();
}

