#include "Core.h"

static IdeContext *the_ide;

IdeContext *TheIdeContext()                    { return the_ide; }
void        SetTheIde(IdeContext *context)     { the_ide = context; }

bool IsVerbose()               { return the_ide ? the_ide->IsVerbose() : false; }
void PutConsole(const char *s) { if(the_ide) the_ide->PutConsole(s); }
void PutVerbose(const char *s) { if(the_ide) the_ide->PutVerbose(s); }
void PutLinking()              { if(the_ide) the_ide->PutLinking(); }
void PutLinkingEnd(bool ok)    { if(the_ide) the_ide->PutLinkingEnd(ok); }

void IdeProcessEvents()        { if(the_ide) the_ide->IdeProcessEvents(); }

const Workspace& GetIdeWorkspace()
{
	if(the_ide)
		return the_ide->IdeWorkspace();
	static Workspace x;
	return x;
}

String GetSourcePackage(const String& path)
{
	String p = ToLower(NormalizePath(path));
	const Workspace& w = GetIdeWorkspace();
	for(int i = 0; i < w.GetCount(); i++) {
		String pn = w[i];
		String pp = ToLower(PackageDirectory(pn));
		if(p.StartsWith(pp) && findarg(p[pp.GetCount()], '/', '\\') >= 0)
			return pn;
	}
	return Null;
}

String IdeContext::GetDefaultMethod()
{
	return LoadFile(ConfigFile("default_method"));
}

VectorMap<String, String> IdeContext::GetMethodVars(const String& method)
{
	VectorMap<String, String> map;
	LoadVarFile(ConfigFile(GetMethodName(method)), map);
	return map;
}

String IdeContext::GetMethodName(const String& method)
{
	return (String)~method + ".bm";
}

String GetDefaultMethod()
{
	return the_ide ? the_ide->GetDefaultMethod() : String();
}

VectorMap<String, String> GetMethodVars(const String& method)
{
	return the_ide ? the_ide->GetMethodVars(method) : VectorMap<String, String>();
}

String GetMethodPath(const String& method)
{
	return the_ide ? the_ide->GetMethodName(method) : String();
}

bool IdeIsBuilding()
{
	return the_ide && the_ide->IdeIsBuilding();
}

void IdeSetBar()
{
	if(the_ide)
		the_ide->IdeSetBar();
}

String IdeGetOneFile()
{
	return the_ide ? the_ide->IdeGetOneFile() : String(Null);
}

int IdeConsoleExecute(const char *cmdline, Stream *out, const char *envptr, bool quiet, bool noconvert)
{
	return the_ide ? the_ide->IdeConsoleExecute(cmdline, out, envptr, quiet, noconvert) : -1;
}

int IdeConsoleExecuteWithInput(const char *cmdline, Stream *out, const char *envptr, bool quiet, bool noconvert)
{
	return the_ide ? the_ide->IdeConsoleExecuteWithInput(cmdline, out, envptr, quiet, noconvert) : -1;
}

int IdeConsoleExecute(One<AProcess> process, const char *cmdline, Stream *out, bool quiet)
{
	return the_ide ? the_ide->IdeConsoleExecute(pick(process), cmdline, out, quiet) : -1;
}

int IdeConsoleAllocSlot()
{
	return the_ide ? the_ide->IdeConsoleAllocSlot() : 0;
}

bool IdeConsoleRun(const char *cmdline, Stream *out, const char *envptr, bool quiet, int slot, String key, int blitz_count)
{
	return the_ide && the_ide->IdeConsoleRun(cmdline, out, envptr, quiet, slot, key, blitz_count);
}

bool IdeConsoleRun(One<AProcess> pick_ process, const char *cmdline, Stream *out, bool quiet, int slot, String key, int blitz_count)
{
	return the_ide && the_ide->IdeConsoleRun(pick(process), cmdline, out, quiet, slot, key, blitz_count);
}

void IdeConsoleFlush()
{
	if(the_ide) the_ide->IdeConsoleFlush();
}

void IdeConsoleBeginGroup(String group)
{
	if(the_ide) the_ide->IdeConsoleBeginGroup(group);
}

void IdeConsoleEndGroup()
{
	if(the_ide) the_ide->IdeConsoleEndGroup();
}

bool IdeConsoleWait()
{
	return the_ide && the_ide->IdeConsoleWait();
}

bool IdeConsoleWait(int slot)
{
	return the_ide && the_ide->IdeConsoleWait(slot);
}

void IdeConsoleOnFinish(Event<>  cb)
{
	if(the_ide) the_ide->IdeConsoleOnFinish(cb);
}

void IdeSetBottom(Ctrl& ctrl)
{
	if(the_ide) the_ide->IdeSetBottom(ctrl);
}

void  IdeActivateBottom() {
	if(the_ide) the_ide->IdeActivateBottom();
}

void IdeRemoveBottom(Ctrl& ctrl)
{
	if(the_ide) the_ide->IdeRemoveBottom(ctrl);
}

void IdeSetRight(Ctrl& ctrl)
{
	if(the_ide) the_ide->IdeSetRight(ctrl);
}

void IdeRemoveRight(Ctrl& ctrl)
{
	if(the_ide) the_ide->IdeRemoveRight(ctrl);
}

bool IdeIsDebug()
{
	return the_ide && the_ide->IdeIsDebug();
}

void IdeEndDebug()
{
	if(the_ide)
		the_ide->IdeEndDebug();
}

void IdeSetDebugPos(const String& file, int line, const Image& img, int i)
{
	if(the_ide)
		the_ide->IdeSetDebugPos(file, line, img, i);
}

void IdeHidePtr()
{
	if(the_ide)
		the_ide->IdeHidePtr();
}

bool IdeDebugLock()
{
	return the_ide && the_ide->IdeDebugLock();
}

bool IdeDebugUnLock()
{
	return the_ide && the_ide->IdeDebugUnLock();
}

bool IdeIsDebugLock()
{
	return the_ide && the_ide->IdeIsDebugLock();
}

String IdeGetFileName()
{
	return the_ide ? the_ide->IdeGetFileName() : String(Null);
}

int    IdeGetFileLine()
{
	return the_ide ? the_ide->IdeGetFileLine() : 0;
}

String IdeGetLine(int i)
{
	return the_ide ? the_ide->IdeGetLine(i) : String(Null);
}

int IdeGetHydraThreads()
{
	return the_ide ? the_ide->IdeGetHydraThreads() : CPU_Cores();
}

String IdeGetCurrentBuildMethod()
{
	return the_ide ? the_ide->IdeGetCurrentBuildMethod() : String();
}

String IdeGetCurrentMainPackage()
{
	return the_ide ? the_ide->IdeGetCurrentMainPackage() : String();
}

void IdePutErrorLine(const String& s)
{
	if(the_ide)
		the_ide->IdePutErrorLine(s);
}
