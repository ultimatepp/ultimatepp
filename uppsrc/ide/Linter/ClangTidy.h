#ifndef _ClangTidy_h_
#define _ClangTidy_h_

#include "Linter.h"

#define LAYOUTFILE <ide/Linter/ClangTidyConfig.lay>
#include <CtrlCore/lay.h>

class ClangTidy : public Linter {
public:
	ClangTidy(const String& name) : Linter(name) {}

	virtual String GetConfigFilePath() const final;
	virtual Value  LoadConfig()              final;
	virtual void   SaveConfig(const Value& cfg) final;
	virtual bool   Exists()   const          final;
	virtual void   Settings()                final;

private:
	Vector<String> ResolveProject(const String& ccjpath);
	Vector<String> ResolvePackage(const String& ccjpath, const Vector<String>& paths);
	Vector<String> ResolveFiles  (Scope sc, const String& ccjpath, const Vector<String>& paths);

	virtual String MakeCmdLine(Scope sc, Vector<String>& paths) final;
	virtual void   OnResults(const String& results)             final;
};

INITIALIZE(ClangTidy);

class ClangTidyConfigDlg : public WithClangTidyConfigLayout<TopWindow>, public Linter::Config {
public:
	ClangTidyConfigDlg(Linter& l);
	
	virtual void Load() final;
	virtual void Save() final;
	virtual void Reset() final;
};

#endif