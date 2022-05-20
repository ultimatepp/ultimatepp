#include "TextDiffCtrl.h"

namespace Upp {

PatchDiff::PatchDiff()
{
	Icon(DiffImg::PatchDiff());

	hidden.Hide();
	
	copyright.Remove();
	removeright.Remove();
	revertright.Remove();

	recent.Hide();

	left.Height(EditField::GetStdHeight());
	left.Add(lfile.HSizePosZ(0, 148));
	left.Add(removeleft.VSizePos().RightPosZ(0, 70));
	left.Add(revertleft.VSizePos().RightPosZ(74, 70));

	right.Add(rfile.VSizePos().HSizePosZ(74, 0));
	right.Add(copyleft.VSizePos().LeftPosZ(0, 70));
	
	copyleft.SetLabel("Patch");
	
	files.WhenSel = [=] { File(); };
	
	dir1.Ctrl::Remove();
	dir2.Ctrl::Remove();

	int cy = patch_file.GetStdSize().cy;
	int div = Zy(4);
	files_pane.Add(target_dir.TopPos(0, cy).HSizePos());
	files_pane.Add(patch_file.TopPos(cy + div, cy).HSizePos());
	patch_file.SetReadOnly();
	target_dir.SetReadOnly();
	
	seldir.Attach(target_dir);
	seldir.Title("Target directory");
	selfile.Attach(patch_file);
	selfile.Title("Patch file");
	selfile.Types("Patch files (*.diff *.patch)\t*.diff *.patch\nAll files\t*.*");
	
	seldir.WhenSelected = selfile.WhenSelected = [=] {
		Open(~~patch_file, Vector<String>() << ~seldir);
	};
	
	files_pane.Add(failed.TopPos(2 * cy + 2 * div, cy).HSizePos());
	
	compare.SetLabel("Patch All");
	compare ^= [=] {
		String msg = "Patch everything?";
		if(failed_count)
			msg << "&[/ (" << failed_count << " files cannot be patched)";
		if(files.GetCount() == 0 || !PromptYesNo(msg))
			return;
		Progress pi("Patching", patch.GetCount());
		for(int i = 0; i < patch.GetCount(); i++) {
			if(pi.StepCanceled())
				return;
			String p = patch.GetPath(i);
			String h = patch.GetPatchedFile(i, GetBackup(p));
			if(!h.IsVoid()) {
				if(IsNull(h))
					FileDelete(p);
				else
					SaveFile(p, h);
			}
		}
		Break(IDOK);
	};

	removeleft ^= [=] {
		Backup(file_path);
		SaveFile(file_path, diff.left.RemoveSelected(HasCrs(file_path)));
		Refresh();
	};

	copyleft ^= [=] {
		int ii = GetFileIndex();
		if(ii < 0 || patched_file.IsVoid())
			return;
		if(diff.right.IsSelection()) {
			Backup(file_path);
			SaveFile(file_path, diff.Merge(true, HasCrs(file_path)));
			Refresh();
			return;
		}
		if(PromptYesNo("Patch [* \1" + file_path + "\1] ?")) {
			Backup(file_path);
			if(IsNull(patched_file))
				FileDelete(file_path);
			else
				SaveFile(file_path, patched_file);
			list[ii].d = 4;
			files.Set(files.GetCursor(), MakeFile(ii));
			Refresh();
		}
	};

	revertleft ^= [=] {
		int q = backup.Find(file_path);
		if(q >= 0 && PromptYesNo("Revert changes?")) {
			SaveFile(file_path, ZDecompress(backup[q]));
			backup.Remove(q);
			Refresh();
		}
	};

	Title("Patch");
}

bool PatchDiff::Open(const char *patch_path, const Vector<String>& target_dirs0)
{
	failed_count = 0;
	list.Clear();
	failed.Hide();
	ShowResult();
	patch_file <<= Null;
	target_dir <<= Null;
	
	Vector<String> target_dirs;
	for(String s : target_dirs0)
		target_dirs.Add(UnixPath(s));

	Progress pi;
	if(!patch.Load(patch_path, pi)) {
		Exclamation("Failed to load the patch file!");
		return false;
	}
	
	patch_file <<= patch_path;
	

	if(!patch.MatchFiles(target_dirs, pi)) {
		Exclamation("Unable to match the directory structure!");
		return true;
	}

	String target_dir = patch.GetTargetDir();

	this->target_dir <<= target_dir;

	list.Clear();
	pi.SetText("Checking files");
	pi.SetTotal(patch.GetCount());
	for(int i = 0; i < patch.GetCount(); i++) {
		if(pi.StepCanceled())
			return false;
		String fn = patch.GetFile(i);
		String p = patch.GetPath(i);
		String h = patch.GetPatchedFile(i, GetBackup(p));
		bool pe = h.GetCount();
		bool x = FileExists(p);
		bool failed = h.IsVoid();
		list.Add(MakeTuple(fn, p, p, failed ? 3 : pe && x ? 0 : pe ? 2 : 1));
		if(failed)
			failed_count++;
	}
	
	failed.Show(failed_count);
	failed.SetInk(SRed());
	failed = String() << failed_count << " failed";

	ShowResult();
	
	if(files.GetCount())
		files.SetCursor(0);
	
	return true;
}

int PatchDiff::GetFileIndex() const
{
	int ii = files.GetCursor();
	return ii >= 0 ? (int)files.Get(ii).data : -1;
}

String PatchDiff::GetBackup(const String& path)
{
	int q = backup.Find(path);
	return q >= 0 ? ZDecompress(backup[q]) : String();
}

void PatchDiff::File()
{
	diff.Set(Null, Null);
	String p2;
	String op = " [PATCHED]";
	copyleft.Disable();
	file_path.Clear();
	patched_file.Clear();
	int ii = GetFileIndex();
	if(ii >= 0) {
		file_path = patch.GetPath(ii);
		if(GetFileLength(file_path) < 4 * 1024 * 1024) {
			String content = LoadFile(file_path);
			if(list[ii].d == PATCHED_FILE) {
				p2 = "[FILE IS PATCHED]";
				diff.Set(content, content);
			}
			else {
				patched_file = patch.GetPatchedFile(ii, GetBackup(file_path));
				if(patched_file.IsVoid()) {
					diff.Set(content, patch.GetPatch(ii));
					p2 = "[FAILED TO APPLY THE PATCH]";
				}
				else {
					diff.Set(content, patched_file);
					p2 = file_path + " [PATCHED]";
					copyleft.Enable();
				}
			}
		}
	}
	lfile <<= file_path;
	rfile <<= p2;
	revertleft.Enable(backup.Find(file_path) >= 0);
}

};