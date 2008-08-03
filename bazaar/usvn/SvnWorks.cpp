#include "usvn.h"

FileSel& sSD() {
	static FileSel fs;
	ONCELOCK {
		fs.AllFilesType();
	}
	return fs;
}

String NormalizePathNN(const String& path)
{
	return IsNull(path) ? path : NormalizePath(path);
}

static void sSetFolder(EditField *f)
{
	if(!sSD().ExecuteSelectDir()) return;
	*f <<= NormalizePathNN(~sSD());
}

void SvnWorks::DirSel(EditField& f)
{
	f.AddFrame(dirsel);
	dirsel <<= callback1(&sSetFolder, &f);
	dirsel.SetImage(CtrlImg::smallright()).NoWantFocus();
}


void SvnWorks::Add(const String& working, const String& user, const String& data)
{
	list.Add(working, user, data);
	Sync();
}

void SvnWorks::New()
{
	WithSvnFolderLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Add SVN working directory");
	DirSel(dlg.working);
	if(dlg.Execute() != IDOK)
		return;
	Add(~dlg.working, ~dlg.user, ~dlg.password);
}

void SvnWorks::Edit()
{
	if(!list.IsCursor())
		return;
	WithSvnFolderLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Edit SVN working directory item");
	DirSel(dlg.working);
	dlg.working <<= list.Get(0);
	dlg.user <<= list.Get(1);
	dlg.password <<= list.Get(2);
	if(dlg.Execute() != IDOK)
		return;
	list.Set(0, ~dlg.working);
	list.Set(1, ~dlg.user);
	list.Set(2, ~dlg.password);
	Sync();
}

String SvnCmd(const char *cmd, const String& user, const String& pwd)
{
	String r = "svn ";
	r << cmd;
	r << " --non-interactive";
	if(!IsNull(user))
		r << " --user \"" << user << "\"";
	if(!IsNull(pwd))
		r << " --user \"" << pwd << "\"";
	r << ' ';
	return r;
}

String SvnCmd(const char *cmd, const SvnWork& w)
{
	return SvnCmd(cmd, w.user, w.password);
}

void SvnWorks::Checkout()
{
	WithSvnCheckoutLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Checkout SVN repository");
	DirSel(dlg.working);
	if(dlg.Execute() != IDOK)
		return;
	String working = ~dlg.working;
	working = GetFullPath(working);
	if(working.GetCount() == 0) {
		Exclamation("Empty working directory");
		return;
	}
	RealizeDirectory(working);
	Add(~dlg.working, ~dlg.user, ~dlg.password);
	SysConsole con;
	con.System(SvnCmd("checkout", ~dlg.user, ~dlg.password).Cat() << ~dlg.repository << ' ' << ~dlg.working);
	con.Execute();
}

void SvnWorks::Remove()
{
	if(list.IsCursor() && PromptYesNo("Remove the item?"))
		list.Remove(list.GetCursor());
	Sync();
}

void SvnWorks::Sync()
{
	edit.Enable(list.IsCursor());
	remove.Enable(list.IsCursor());
}

int SvnWorks::GetCount() const
{
	return list.GetCount();
}

SvnWork SvnWorks::operator[](int i) const
{
	SvnWork w;
	w.working = list.Get(i, 0);
	w.user = list.Get(i, 1);
	w.password = list.Get(i, 2);
	return w;
}

void SvnWorks::Clear()
{
	list.Clear();
	Sync();
}

void SvnWorks::Load(const String& text)
{
	list.Clear();
	Vector<String> ln = Split(text, '\n');
	for(int i = 0; i < ln.GetCount(); i++) {
		Vector<String> q = Split(ln[i], ';');
		if(q.GetCount() >= 1)
			Add(q[0], q.At(1), q.At(2));
	}
	Sync();
}

String SvnWorks::Save() const
{
	String h;
	for(int i = 0; i < list.GetCount(); i++)
		h << list.Get(i, 0) << ';' << list.Get(i, 1) << ';' << list.Get(i, 2) << '\n';
	return h;
}

SvnWorks::SvnWorks()
{
	CtrlLayoutExit(*this, "SVN Working directories");
	list.AddColumn("Working directory");
	list.AddColumn("User");
	list.AddColumn("Password");
	list.Moving();
	list.ColumnWidths("364 100 100");
	list.WhenCursor = THISBACK(Sync);
	add <<= THISBACK(New);
	remove <<= THISBACK(Remove);
	checkout <<= THISBACK(Checkout);
	edit <<= THISBACK(Edit);
	Sync();
}

