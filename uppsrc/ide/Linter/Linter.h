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
	Linter() {}
	virtual ~Linter() {}
	
	virtual String GetConfigFilePath() = 0;
	virtual Value  LoadConfig() = 0;
	virtual void   SaveConfig(const Value& cfg) = 0;
	virtual bool   Exists()   = 0;
	virtual void   Settings() = 0;

	bool CanCheck();
	void CheckFile();
	void CheckPackage();
	void CheckProject();
	
	void StdMenu(Bar& menu);
	void FileMenu(Bar& menu);
	void PackageMenu(Bar& menu);
	
	struct Config {
		virtual void Load()  = 0;
		virtual void Save()  = 0;
		virtual void Reset() = 0;
	};

protected:
	enum class Scope { File, Package, Project };
	
	virtual String MakeCmdLine(Scope sc, Vector<String>& paths) = 0;
	virtual void   OnResults(const String& results) = 0;
	
	String GetFileName();
	String GetFilePath();
	String GetPackageName();
	String GetPackagePath();

private:
	void SysCmd(const String& cmd, const String& text, Stream& fs);
	void DoCheck(Scope sc, Vector<String>& paths);
};

Linter& GetLinter();
bool HasLinter();

// Static analyzer modules.
#include "CppCheck.h"
//#include "ClangTidy.h"

#define KEYGROUPNAME "Linter"
#define KEYNAMESPACE LinterKeys
#define KEYFILE <ide/Linter/Linter.key>
#include <CtrlLib/key_header.h>

#endif
