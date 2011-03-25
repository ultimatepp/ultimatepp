#include "usvn.h"

static FileSel& ssSD() {
	static FileSel fs;
	ONCELOCK {
		fs.AllFilesType();
	}
	return fs;
}

static void sSetFolder(EditField *f)
{
	if(!ssSD().ExecuteSelectDir()) return;
	String path = ~ssSD();
	*f <<= IsNull(path) ? path : NormalizePath(path);
}

void SvnWorks::DirSel(EditField& f)
{
	f.AddFrame(dirsel);
	dirsel <<= callback1(&sSetFolder, &f);
	dirsel.SetImage(CtrlImg::smallright()).NoWantFocus();
}


void SvnWorks::Add(const String& working, const String& user, const String& data, bool readonly)
{
	list.Add(working, user, data, readonly?"yes":"no",0);
	Sync();
}

void SvnWorks::New()
{
	WithSvnFolderLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Add SVN working directory");
	DirSel(dlg.working);
	if(dlg.Execute() != IDOK)
		return;
	Add(~dlg.working, ~dlg.user, ~dlg.password, ~dlg.readonly);
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
	dlg.readonly <<= (AsString(list.Get(3))=="yes");
	if(dlg.Execute() != IDOK)
		return;
	list.Set(0, ~dlg.working);
	list.Set(1, ~dlg.user);
	list.Set(2, ~dlg.password);
	list.Set(3, ~dlg.readonly?"yes":"no");
	Sync();
}

String SvnCmd(const char *cmd, const String& user, const String& pwd,int rev)
{
	String r = "svn ";
	r << cmd;
	r << " --non-interactive";
	if(!IsNull(user))
		r << " --username \"" << user << "\"";
	if(!IsNull(pwd))
		r << " --password \"" << pwd << "\"";
	if(rev!=0)
		r << " --revision " << rev;
	r << ' ';
	return r;
}

String SvnCmd(const char *cmd, const SvnWork& w)
{
	return SvnCmd(cmd, w.user, w.password, w.revision);
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
	w.readonly = AsString(list.Get(i, 3))=="yes";
	w.revision = (int)list.Get(i,4);
	return w;
}

void SvnWorks::SetRevision(int i, int revision)
{
	list.Set(i,4,revision);
}

void SvnWorks::Clear()
{
	list.Clear();
	Sync();
}

void SvnWorks::Load(const String& text)
{
	list.Clear();
	Vector<String> ln = Split(text, CharFilterCrLf);
	for(int i = 0; i < ln.GetCount(); i++) {
		Vector<String> q = Split(ln[i], ';');
		if(q.GetCount() >= 1)
			Add(q[0], q.At(1), q.At(2), q.At(3)!="no");
	}
	Sync();
}

String SvnWorks::Save() const
{
	String h;
	for(int i = 0; i < list.GetCount(); i++)
		h << list.Get(i, 0) << ';' << list.Get(i, 1) << ';' << list.Get(i, 2) << ';' << list.Get(i, 3) << '\n';
	return h;
}

SvnWorks::SvnWorks()
{
	CtrlLayoutExit(*this, "SVN Working directories");
	list.AddColumn("Working directory");
	list.AddColumn("User");
	list.AddColumn("Password");
	list.AddColumn("Read-only");
	list.AddIndex("revision");
	list.Moving();
	list.ColumnWidths("364 100 100 100");
	list.WhenCursor = THISBACK(Sync);
	add <<= THISBACK(New);
	remove <<= THISBACK(Remove);
	checkout <<= THISBACK(Checkout);
	edit <<= THISBACK(Edit);
	Sync();
}

