#ifndef BUILDERS_H
#define BUILDERS_H

#include <ide/Core/Core.h>
#include <coff/binobj/binobj.h>
#include <plugin/bz2/bz2.h>

#include "Build.h"

void PutCompileTime(int time, int count);

struct Blitz {
	bool   build;
	int    count;
	String path;
	String object;
	String info;
};

struct CppBuilder : Builder {
	virtual String GetTargetExt() const;

	const Workspace& wspc;
	Time             targettime;

	String                 GetSharedLibPath(const String& package) const;
	String                 GetHostPath(const String& path) const;
	String                 GetHostPathShort(const String& path) const;
	String                 GetHostPathQ(const String& path) const;
	String                 GetHostPathShortQ(const String& path) const;
	String                 GetLocalPath(const String& path) const;
	Vector<Host::FileInfo> GetFileInfo(const Vector<String>& path) const;
	Host::FileInfo         GetFileInfo(const String& path) const;
	Time                   GetFileTime(const String& path) const;
	bool                   FileExists(const String& path) const;
	void                   DeleteFile(const Vector<String>& path);
	void                   DeleteFile(const String& path);
	void                   ChDir(const String& path);
	void                   SaveFile(const String& path, const String& data);
	String                 LoadFile(const String& path);
	int                    Execute(const char *cmdline);
	int                    Execute(const char *cl, Stream& out);
	int                    AllocSlot();
	bool                   Run(const char *cmdline, int slot, String key, int blitz_count);
	bool                   Run(const char *cmdline, Stream& out, int slot, String key, int blitz_count);
	bool                   Wait();
	bool                   HasFlag(const char *f) const        { return config.Find(f) >= 0; }
	Vector<String>         CustomStep(const String& file);

	String                 Includes(const char *sep, const String& package, const Package& pkg);
	String                 IncludesShort(const char *sep, const String& package, const Package& pkg);
	String                 IncludesDefinesTargetTime(const String& package, const Package& pkg);

	String                 GetMakePath(String fn) const;
	Point                  ExtractVersion();

	void                   ShowTime(int count, int start_time);

	String BrcToC(String objfile, CParser& binscript, String basedir, const String& package, const Package& pkg);

	Blitz BlitzStep(Vector<String>& sfile, Vector<String>& soptions,
	                Vector<String>& obj, const char *objext, Vector<bool>& optimize);

	virtual void           AddMakeFile(MakeFile& makefile, String package,
		const Vector<String>& all_uses, const Vector<String>& all_libraries,
		const Index<String>& common_config, bool exporting);

	CppBuilder() : wspc(GetIdeWorkspace()) {}
};

struct GccBuilder : CppBuilder {
	virtual void   AddFlags(Index<String>& cfg);
	virtual bool   BuildPackage(const String& package, Vector<String>& linkfile, String& linkoptions,
		const Vector<String>& all_uses, const Vector<String>& all_libraries, int optimize);
	virtual bool   Link(const Vector<String>& linkfile, const String& linkoptions, bool createmap);
	virtual bool   Preprocess(const String& package, const String& file, const String& target, bool asmout);

	String CompilerName() const;
	String CmdLine(const String& package, const Package& pkg);
	void BinaryToObject(String objfile, CParser& binscript, String basedir, const String& package, const Package& pkg);
};

struct OwcBuilder : CppBuilder {
	typedef OwcBuilder CLASSNAME;

	virtual void   AddFlags(Index<String>& cfg);
	virtual bool   BuildPackage(const String& package, Vector<String>& linkfile, String& linkoptions,
		const Vector<String>& all_uses, const Vector<String>& all_libraries, int optimize);
	virtual bool   Link(const Vector<String>& linkfile, const String& linkoptions, bool createmap);
	virtual bool   Preprocess(const String& package, const String& file, const String& target, bool asmout);
	
	String         IncludesDefinesTargetTime(const String& package, const Package& pkg);

	String CompilerName(bool isCpp = true) const;
	String LinkerName() const;
	String CmdLine(const String& package, const Package& pkg);
	void   BinObjConsole(String c) { PutConsole(c); }
};

struct MscBuilder : CppBuilder {
	typedef MscBuilder CLASSNAME;

	virtual void   AddFlags(Index<String>& cfg);
	virtual bool   BuildPackage(const String& package, Vector<String>& linkfile, String& linkoptions,
		const Vector<String>& all_uses, const Vector<String>& all_libraries, int optimize);
	virtual bool   Link(const Vector<String>& linkfile, const String& linkoptions, bool createmap);
	virtual bool   Preprocess(const String& package, const String& file, const String& target, bool asmout);

	String CmdLine(const String& package, const Package& pkg);
	String MachineName() const;
	String LinkerName() const;
	String PdbPch(String package, int slot, bool do_pch) const;
	bool   HasAnyDebug() const;
	void   BinObjConsole(String c) { PutConsole(c); }
	bool   IsMsc89() const;
	bool   IsMsc86() const;
	bool   IsMscArm() const;
	bool   IsMsc64() const;

	void BinaryToObject(String objfile, CParser& binscript, String basedir, const String& package, const Package& pkg);
};

struct JavaBuilder : CppBuilder {
	virtual bool   BuildPackage(const String& package, Vector<String>& linkfile, String& linkoptions,
		const Vector<String>& all_uses, const Vector<String>& all_libraries, int optimize);
	virtual bool   Link(const Vector<String>& linkfile, const String& linkoptions, bool createmap);
	virtual bool   Preprocess(const String& package, const String& file, const String& target, bool asmout);

	bool           PreprocessJava(String file, String target, String options, String package, const Package& pkg);
	Time           AddClassDeep(String& link, String dir, String reldir);

	String         JavaLine();
	String         JarLine();
};

struct ScriptBuilder : CppBuilder {
public:
	typedef ScriptBuilder CLASSNAME;
	ScriptBuilder() : is_parsed(false), script_error(false) {}

	virtual bool   BuildPackage(const String& package, Vector<String>& linkfile, String& linkoptions,
		const Vector<String>& all_uses, const Vector<String>& all_libraries, int optimize);
	virtual bool   Link(const Vector<String>& linkfile, const String& linkoptions, bool createmap);
	virtual bool   Preprocess(const String& package, const String& file, const String& target, bool asmout);

	String         CmdLine();

private:
	void           CheckParse();

	EscValue       ExecuteIf(const char *fn, Vector<EscValue>& args);
	EscValue       ExecuteIf(const char *fn);
	EscValue       ExecuteIf(const char *fn, EscValue arg);
	EscValue       ExecuteIf(const char *fn, EscValue arg1, EscValue arg2);
	EscValue       ExecuteIf(const char *fn, EscValue arg1, EscValue arg2, EscValue arg3);

	void           ESC_Execute(EscEscape& e);
	void           ESC_PutConsole(EscEscape& e);
	void           ESC_PutVerbose(EscEscape& e);

private:
	ArrayMap<String, EscValue> globals;
	bool is_parsed;
	bool script_error;
};

#endif
