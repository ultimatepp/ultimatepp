#ifndef TheIde_Linter_h
#define TheIde_Linter_h

#include <Core/Core.h>
#include <CtrlLib/CtrlLib.h>

#include <ide/Common/Common.h>
#include <ide/Core/Core.h>
#include <ide/ide.h>

#define LAYOUTFILE <ide/Linter/Linter.lay>
#include <CtrlCore/lay.h>

class Linter final {
public:
	Linter() {}
	~Linter() {}

	bool Exists();
	bool CanCheck();
	void CheckFile();
	void CheckPackage();
	void CheckAll();

	static Value LoadConfig();
	static String GetConfigFilePath();

	void StdMenu(Bar& menu);
	void FileMenu(Bar& menu);
	void PackageMenu(Bar& menu);

private:
	String GetFileName();
	String GetFilePath();
	String GetPackageName();
	String GetPackagePath();
	String GetCmdLine();

	Vector<String> GetPaths(const String& dir, const String& pattern);
	
	void SysCmd(const String& cmd, const String& text, Stream& fs);
	void DoCheck(Vector<String>& paths);
	void ParseResults(const XmlNode& results);
};

Linter& GetLinter();
bool HasLinter();

struct LinterConfigTab : WithLinterConfigLayout<Ctrl> {
	LinterConfigTab();

	void Load();
	void Save();
	void Reset();
};

#define KEYGROUPNAME "Linter"
#define KEYNAMESPACE LinterKeys
#define KEYFILE <ide/Linter/Linter.key>
#include <CtrlLib/key_header.h>

#define IMAGECLASS LinterImg
#define IMAGEFILE <ide/Linter/Linter.iml>
#include <Draw/iml_header.h>

#endif
