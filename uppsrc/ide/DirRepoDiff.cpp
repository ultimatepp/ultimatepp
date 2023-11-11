#include "ide.h"

DirRepoDiffDlg::DirRepoDiffDlg()
{
	Title("Compare directories / commits");

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

	for(int i = 0; i < 2; i++) {
		files_pane.Add(mode[i].TopPos(y, cy).HSizePos());
		y += lcy;
		files_pane.Add((i ? dir2 : dir1).TopPos(y, cy).HSizePos());
		files_pane << branch[i].HSizePos(0, Zx(80)).TopPos(y, cy);
		files_pane << hash[i].RightPos(0, Zx(80) - DPI(2)).TopPos(y, cy);
		hash[i].SetLabel("Copy hash");
		hash[i] << [=] { WriteClipboardText(~~r[i]); };
		y += lcy;
		files_pane << r[i].HSizePos().TopPos(y, cy);
		y += lcy + lcy / 2;
	}

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
	
	session_id = String() << Random() << Random() << Random() << Random();
	
	branch[0].SetDropLines(32);
	branch[1].SetDropLines(32);
	r[0].SetDropLines(32);
	r[1].SetDropLines(32);
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
	hash[i].Show(!b);
	r[i].Show(!b);

	if(!b) {
		LoadBranches(branch[i], ~~dl);
		Revs(i);
	}
	
	ClearFiles();
	SyncCompare();
	
	editable_left = !IsGit(0);
	editable_right = !IsGit(1);
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
	compare.Enable((!IsNull(dir1) || IsGit(0)) && (!IsNull(dir2) || IsGit(1)));
}

void DirRepoDiffDlg::Compare()
{
	auto Clone = [&](int i, EditString& es, int& mid) -> bool {
		mid = 0;
		if(IsGit(i)) {
			String repo = ~~mode[i];
			String d = CacheFile("git-" + SHA1String(repo + "\1" + session_id + "\1" + AsString(i)));
			if(!DirectoryExists(d)) {
				gitd << d;
				Progress pi;
				if(!CopyFolder(d, ~~mode[i], &pi))
					return false;
			}
			GitCmd(d, "checkout --force " + ~~r[i]);
			es <<= d;
			mid = d.GetCount();
			return true;
		}
		return true;
	};
	if(Clone(0, dir1, lmid) && Clone(1, dir2, rmid))
		DirDiffDlg::Compare();
}
