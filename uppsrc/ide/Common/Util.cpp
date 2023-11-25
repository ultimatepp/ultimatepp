#include "Common.h"

bool Debugger::Tip(const String&, CodeEditor::MouseTip&)
{
	return false;
}

bool deactivation_save;

void DeactivationSave(bool b) // On deactivation, make no prompts, ignore save errors
{
	deactivation_save = b;
}

bool IsDeactivationSave()
{
	return deactivation_save;
}

bool FinishSave(String tmpfile, String outfile)
{
	if(IsDeactivationSave()) {
		FileMove(tmpfile, outfile);
		return true;
	}
	Progress progress;
	int time = msecs();
	for(;;) {
		progress.SetTotal(10000);
		progress.SetText("Saving '" + GetFileName(outfile) + "'");
		if(!FileExists(tmpfile))
			return false;
		FileDelete(outfile);
		if(FileMove(tmpfile, outfile))
			return true;
		IdeConsoleFlush();
		Sleep(200);
		if(progress.SetPosCanceled((msecs() - time) % progress.GetTotal())) {
			int art = Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(),
				"Unable to save current file.&"
				"Retry save, ignore it or save file to another location?",
				"Save as...", "Retry", "Ignore");
			if(art < 0)
				return false;
			if(art && AnySourceFs().ExecuteSaveAs())
				outfile = AnySourceFs();
			progress.SetPos(0);
		}
	}
}

bool FinishSave(String outfile)
{
	return FinishSave(outfile + ".$tmp", outfile);
}

bool SaveFileFinish(const String& filename, const String& data)
{
	if(!SaveFile(filename + ".$tmp", data)) {
		if(IsDeactivationSave())
			return true;
		Exclamation("Error creating temporary file " + filename);
		return false;
	}
	return FinishSave(filename);
}

bool SaveChangedFileFinish(const String& filename, const String& data)
{
	if(data == LoadFile(filename))
		return true;
	return SaveFileFinish(filename, data);
}

VectorMap<String, String>& sWorkspaceCfg()
{
	static VectorMap<String, String> h;
	return h;
}

Vector<Event<> >& sWorkspaceCfgFlush()
{
	static Vector<Event<> > h;
	return h;
}

void    RegisterWorkspaceConfig(const char *name)
{
	ASSERT(sWorkspaceCfg().Find(name) < 0);
	sWorkspaceCfg().Add(name);
}

void    RegisterWorkspaceConfig(const char *name, Event<>  WhenFlush)
{
	RegisterWorkspaceConfig(name);
	sWorkspaceCfgFlush().Add(WhenFlush);
}


String& WorkspaceConfigData(const char *name)
{
	return sWorkspaceCfg().GetAdd(name);
}

void  SerializeWorkspaceConfigs(Stream& s)
{
	int i;
	for(i = 0; i < sWorkspaceCfgFlush().GetCount(); i++)
		sWorkspaceCfgFlush()[i]();
	VectorMap<String, String>& cfg = sWorkspaceCfg();
	int version = 0;
	s / version;
	int count = cfg.GetCount();
	s / count;
	for(i = 0; i < count; i++) {
		String name;
		if(s.IsStoring())
			name = cfg.GetKey(i);
		s % name;
		int q = cfg.Find(name);
		if(q >= 0)
			s % cfg[q];
		else
		{
			String dummy;
			s % dummy;
		}
	}
	s.Magic();
}

bool GuiPackageResolver(const String& error, const String& path, int line)
{
prompt:
	switch(Prompt(Ctrl::GetAppName(), CtrlImg::exclamation(),
	              error + "&while parsing package " + DeQtf(path),
		          "Edit \\& Retry", "Ignore",  "Stop")) {
	case 0:
		if(!PromptYesNo("Ignoring will damage package. Everything past the "
			            "point of error will be lost.&Do you want to continue ?"))
			goto prompt;
		return false;
	case 1: {
			TopWindow win;
			LineEdit edit;
			edit.Set(LoadFile(path));
			edit.SetCursor(edit.GetPos(line));
			win.Title(path);
			win.Add(edit.SizePos());
			win.Run();
			SaveFile(path, edit.Get());
		}
		return true;;
	case -1:
		exit(1);
	}
	return false;
}

void CleanModules()
{
	for(int i = 0; i < GetIdeModuleCount(); i++)
		GetIdeModule(i).CleanUsc();
}

bool IdeModuleUsc(CParser& p, String& current_namespace)
{
	for(int i = 0; i < GetIdeModuleCount(); i++)
		if(GetIdeModule(i).ParseUsc(p, current_namespace))
			return true;
	return false;
}

static void ReadMacro(CParser& p)
{
	IdeMacro macro;
	if(p.IsString()) {
		macro.menu = p.ReadString();
		if(p.Char(':'))
			macro.submenu = p.ReadString();
	}
	if(!p.IsChar('{'))
		macro.hotkey = ParseKeyDesc(p);
	EscLambda& l = macro.code.CreateLambda();
	const char *t = p.GetPtr();
	l.filename = p.GetFileName();
	l.line = p.GetLine();
	if(!p.Char('{'))
		p.ThrowError("missing '{'");
	SkipBlock(p);
	l.code = String(t, p.GetPtr());
	Array<IdeMacro>& mlist = UscMacros();
	if(macro.hotkey) {
		int f = FindFieldIndex(mlist, &IdeMacro::hotkey, macro.hotkey);
		if(f >= 0) {
			PutConsole(Format("%s(%d): duplicate macro hotkey %s\n", l.filename, l.line, GetKeyDesc(macro.hotkey)));
			const EscLambda& lambda = UscMacros()[f].code.GetLambda();
			PutConsole(Format("%s(%d): previously defined here\n", lambda.filename, lambda.line));
		}
	}
	if(!IsNull(macro.menu)) {
		for(int i = 0; i < mlist.GetCount(); i++)
			if(mlist[i].menu == macro.menu && mlist[i].submenu == macro.submenu) {
				PutConsole(Format("%s(%d): duplicate macro menu item (%s:%s)\n",
					l.filename, l.line, macro.menu, macro.submenu));
				const EscLambda& lambda = UscMacros()[i].code.GetLambda();
				PutConsole(Format("%s(%d): previously defined here\n", lambda.filename, lambda.line));
				break;
			}
	}
	mlist.Add(macro);
}

INITBLOCK {
	Package::SetPackageResolver(GuiPackageResolver);
	UscSetCleanModules(CleanModules);
	SetIdeModuleUsc(IdeModuleUsc);
	UscSetReadMacro(ReadMacro);
}

bool IdeExit;
bool IdeAgain;

bool CopyFolder(const char *dst, const char *src, Progress *pi)
{
	if(strcmp(src, dst) == 0)
		return true;
	RealizeDirectory(dst);
	if(pi)
		pi->SetText(dst);
	FindFile ff(AppendFileName(src, "*"));
	while(ff) {
		if(pi && pi->StepCanceled())
			return false;
		String s = AppendFileName(src, ff.GetName());
		String d = AppendFileName(dst, ff.GetName());
		if(ff.IsFolder())
			if(!CopyFolder(d, s, pi))
				return false;
		if(ff.IsFile())
			if(!SaveFile(d, LoadFile(s)))
				return false;
		ff.Next();
	}
	return true;
}

bool HasSvn()
{
	String dummy;
	static bool b = HostSys("svn", dummy) >= 0;
	return b;
}

bool HasGit()
{
	String dummy;
	static bool b = HostSys("git", dummy) >= 0;
	return b;
}

int MaxAscent(Font f)
{
	return max(f.GetAscent(), f().Bold().GetAscent(),
	           f().Italic().GetAscent(), f().Bold().Italic().GetAscent());
}
