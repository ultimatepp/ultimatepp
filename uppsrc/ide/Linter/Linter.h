#ifndef TheIde_Linter_h
#define TheIde_Linter_h

#include <Core/Core.h>
#include <CtrlLib/CtrlLib.h>

#include <ide/Common/Common.h>
#include <ide/Core/Core.h>
#include <ide/ide.h>

#define IMAGECLASS LinterImg
#define IMAGEFILE <ide/Linter/Linter.iml>
#include <Draw/iml_header.h>

// Base class for command-line driven linter modules (static analyzers)
class Linter {
public:
	Linter(const String& name) : module_name(name) {}
	virtual ~Linter() {}
	
	virtual String GetConfigFilePath() const = 0;
	virtual Value  LoadConfig() = 0;
	virtual void   SaveConfig(const Value& cfg) = 0;
	virtual bool   Exists() const = 0;
	virtual void   Settings() = 0;

	String GetName() const { return module_name; }
	
	bool CanCheck() const;
	void CheckFile();
	void CheckPackage();
	void CheckProject();
	
	static void StdMenu(Bar& menu);
	static void FileMenu(Bar& menu);
	static void PackageMenu(Bar& menu);
	
	class Config {
	public:
		Config(Linter& l) : linter(l) {}
		virtual void Load()  = 0;
		virtual void Save()  = 0;
		virtual void Reset() = 0;
	protected:
		Linter& linter;
	};

protected:
	enum class Scope { File, Package, Project };
	
	virtual String MakeCmdLine(Scope sc, Vector<String>& paths) = 0;
	virtual void   OnResults(const String& results) = 0;
	
	String GetFileName() const;
	String GetFilePath() const;
	String GetPackageName() const;
	String GetPackagePath() const;

private:
	void SysCmd(const String& cmd, const String& text, Stream& fs);
	void DoCheck(Scope sc, Vector<String>& paths);
	String module_name;
};

INITIALIZE(Linter)

void    RegisterLinterModule(Linter& linter_module);
int     GetLinterModuleCount();
Linter& GetLinterModule(int i);
Linter* GetActiveLinterModulePtr();

// Available static analyzer modules.
#include "CppCheck.h"
//#include "ClangTidy.h"

#define KEYGROUPNAME "Linter"
#define KEYNAMESPACE LinterKeys
#define KEYFILE <ide/Linter/Linter.key>
#include <CtrlLib/key_header.h>

#endif
