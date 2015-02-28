#ifndef _ide_GuiCommon_GuiCommon_h_
#define _ide_GuiCommon_GuiCommon_h_

#include <ide/Core/Core.h>
#include <CtrlLib/CtrlLib.h>
#include <CodeEditor/CodeEditor.h>

#define IMAGECLASS IdeCommonImg
#define IMAGEFILE  <ide/Common/common.iml>
#include <Draw/iml_header.h>

struct Debugger {
	virtual void DebugBar(Bar& bar) = 0;
	virtual bool SetBreakpoint(const String& filename, int line, const String& bp) = 0;
	virtual bool RunTo() = 0;
	virtual void Run() = 0;
	virtual void Stop() = 0;
	virtual bool IsFinished() = 0;
	virtual bool Tip(const String& exp, CodeEditor::MouseTip& mt);

	virtual ~Debugger() {}
};

void      SourceFs(FileSel& fsel);
FileSel&  AnySourceFs();
FileSel&  AnyPackageFs();
FileSel&  BasedSourceFs();
FileSel&  OutputFs();

void      ShellOpenFolder(const String& dir);

Image     ImageOver(const Image& back, const Image& over);

Image     IdeFileImage(const String& filename, bool fast, bool include_path, bool pch);

bool FinishSave(String tmpfile, String outfile);
void DeactivationSave(bool b);
bool IsDeactivationSave();

bool FinishSave(String outfile);
bool SaveFileFinish(const String& filename, const String& data);
bool SaveChangedFileFinish(const String& filename, const String& data);

struct IdeDesigner  {
	virtual String GetFileName() const = 0;
	virtual void   Save() = 0;
	virtual void   SyncUsc()                                        {}
	virtual void   SaveEditPos()                                    {}
	virtual void   RestoreEditPos()                                 {}
	virtual void   EditMenu(Bar& menu) = 0;
	virtual int    GetCharset() const                               { return -1; }
	virtual Ctrl&  DesignerCtrl() = 0;
	virtual void   SetFocus()                                       { DesignerCtrl().SetFocus(); }

	virtual ~IdeDesigner() {}
};

struct IdeModule {
	virtual void         CleanUsc() {}
	virtual bool         ParseUsc(CParser&)                                       { return false; }
	virtual Image        FileIcon(const char *filename)                           { return Null; }
	virtual IdeDesigner *CreateDesigner(Ide *ide, const char *path, byte charset) { return CreateDesigner(path, charset); }
	virtual IdeDesigner *CreateDesigner(const char *path, byte charset)           { return NULL; }
	virtual void         Serialize(Stream& s) {}

	virtual ~IdeModule() {}
};

void       RegisterIdeModule(IdeModule& module);
int        GetIdeModuleCount();
IdeModule& GetIdeModule(int q);

enum {
	DEBUG_NONE, DEBUG_MINIMAL, DEBUG_FULL
};

void             RegisterWorkspaceConfig(const char *name);
void             RegisterWorkspaceConfig(const char *name, Callback WhenFlush);
String&          WorkspaceConfigData(const char *name);

template <class T>
bool LoadFromWorkspace(T& x, const char *name)
{
	StringStream ss(WorkspaceConfigData(name));
	return ss.IsEof() || Load(x, ss);
}

template <class T>
void StoreToWorkspace(T& x, const char *name)
{
	StringStream ss;
	Store(x, ss);
	WorkspaceConfigData(name) = ss;
}

void SerializeWorkspaceConfigs(Stream& s);

extern bool IdeExit;

#endif
