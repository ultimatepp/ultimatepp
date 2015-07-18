#ifndef BUILDERS_H
#define BUILDERS_H

#include <ide/Core/Core.h>
#include <ide/Android/Android.h>
#include <ide/Java/Java.h>
//#include <coff/binobj/binobj.h>
#include <plugin/bz2/bz2.h>

#include "Android.h"
#include "BuilderComponents.h"
#include "Build.h"

void PutCompileTime(int time, int count);

String BrcToC(CParser& binscript, String basedir);

struct CppBuilder : Builder {
	virtual String GetTargetExt() const;

	const Workspace& wspc;
	Time             targettime;
	
	String                 GetSharedLibPath(const String& package) const;
	String                 GetLocalPath(const String& path) const;
	int                    AllocSlot();
	bool                   Run(const char *cmdline, int slot, String key, int blitz_count);
	bool                   Run(const char *cmdline, Stream& out, int slot, String key, int blitz_count);
	bool                   Wait();
	bool                   Wait(int slot);
	void                   OnFinish(Callback cb);
	bool                   Cp(const String& cmd, const String& package, bool& error);
	bool                   Cd(const String& cmd);
	Vector<String>         CustomStep(const String& path, const String& package, bool& error);

	String                 Includes(const char *sep, const String& package, const Package& pkg);
	String                 IncludesShort(const char *sep, const String& package, const Package& pkg);
	String                 DefinesTargetTime(const char *sep, const String& package, const Package& pkg);
	String                 IncludesDefinesTargetTime(const String& package, const Package& pkg);
	bool                   HasAnyDebug() const;

	String                 GetMakePath(String fn) const;
	Point                  ExtractVersion() const;
	
	// POSIX lib files has names in form of libXXXXXX.so.ver.minver(.rel)
	// so we can't simply get file extension
	String                 GetSrcType(String fn) const;

	// from complete lib name/path (libXXX.so.ver.minVer) gets the soname (libXXX.so.ver)
	String                 GetSoname(String libName) const;

	// from complete lib name/path (libXXX.so.ver.minVer) gets the link name (libXXX.so)
	String                 GetSoLinkName(String libName) const;

	void                   ShowTime(int count, int start_time);

	virtual void           AddMakeFile(MakeFile& makefile, String package,
		const Vector<String>& all_uses, const Vector<String>& all_libraries,
		const Index<String>& common_config, bool exporting);

	CppBuilder() : wspc(GetIdeWorkspace()) {}
};

struct GccBuilder : CppBuilder {
	virtual void   AddFlags(Index<String>& cfg);
	virtual bool   BuildPackage(const String& package, Vector<String>& linkfile, Vector<String>& immfile,
	    String& linkoptions, const Vector<String>& all_uses, const Vector<String>& all_libraries, int optimize);
	virtual bool   Link(const Vector<String>& linkfile, const String& linkoptions, bool createmap);
	virtual bool   Preprocess(const String& package, const String& file, const String& target, bool asmout);

	String CompilerName() const;
	String CmdLine(const String& package, const Package& pkg);
	void BinaryToObject(String objfile, CParser& binscript, String basedir, const String& package, const Package& pkg);
};

struct OwcBuilder : CppBuilder {
	typedef OwcBuilder CLASSNAME;

	virtual void   AddFlags(Index<String>& cfg);
	virtual bool   BuildPackage(const String& package, Vector<String>& linkfile, Vector<String>& immfile,
	    String& linkoptions, const Vector<String>& all_uses, const Vector<String>& all_libraries, int optimize);
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
	virtual bool   BuildPackage(const String& package, Vector<String>& linkfile, Vector<String>& immfile,
	    String& linkoptions, const Vector<String>& all_uses, const Vector<String>& all_libraries, int optimize);
	virtual bool   Link(const Vector<String>& linkfile, const String& linkoptions, bool createmap);
	virtual bool   Preprocess(const String& package, const String& file, const String& target, bool asmout);

	String CmdLine(const String& package, const Package& pkg);
	String MachineName() const;
	String LinkerName() const;
	String Pdb(String package, int slot, bool separate_pdb) const;
	void   BinObjConsole(String c) { PutConsole(c); }
	bool   IsMsc89() const;
	bool   IsMsc86() const;
	bool   IsMscArm() const;
	bool   IsMsc64() const;

	void BinaryToObject(String objfile, CParser& binscript, String basedir, const String& package, const Package& pkg);
};

struct JavaBuilder : CppBuilder {
	virtual bool   BuildPackage(const String& package, Vector<String>& linkfile, Vector<String>&, String& linkoptions,
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

	virtual bool   BuildPackage(const String& package, Vector<String>& linkfile, Vector<String>&, String& linkoptions,
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

class AndroidBuilder : public Builder {
public:
	AndroidSDK androidSDK;
	AndroidNDK androidNDK;
	Jdk jdk;
	
	bool           ndk_blitz;
	Vector<String> ndkArchitectures;
	String         ndkToolchain;
	String         ndkCppRuntime;
	String         ndkCppFlags;
	String         ndkCFlags; 
	
public:
	static Index<String> GetBuildersNames();
		
public:
	AndroidBuilder();
	
	String GetTargetExt() const;
	
	virtual bool BuildPackage(const String& packageName, Vector<String>& linkfile, Vector<String>& immfile, String& linkoptions,
		const Vector<String>& all_uses, const Vector<String>& all_libraries, int optimize);
	virtual bool Link(const Vector<String>& linkfile, const String& linkoptions, bool createmap);
	virtual bool Preprocess(const String& package, const String& file, const String& target, bool asmout);
	virtual void CleanPackage(const String& package);
	
protected:
	bool MovePackageFileToAndroidProject(const String& src, const String& dest); 
	bool RealizePackageJavaSourcesDirectory(const String& packageName);
	
	bool RealizeLinkDirectories() const;
	
protected:
	bool ValidateBuilderEnviorement();
	void PutErrorOnConsole(const String& msg);
	bool FileNeedsUpdate(const String& path, const String& data);
	void UpdateFile(const String& path, const String& data);
	void GenerateApplicationMakeFile();
	void GenerateMakeFile();
	bool GenerateRFile();
	
	bool AddSharedLibsToApk(const String& apkPath);
	
	bool PreprocessJava(const String& package, const String& file, const String& target);
	
protected:
	String GetSandboxDir() const;
	
	String GetAndroidProjectDir() const;
	String GetAndroidProjectJavaSourcesDir() const;
	String GetAndroidProjectJniSourcesDir() const;
	String GetAndroidProjectLibsDir() const;
	String GetAndroidProjectResourcesDir() const;
	String GetAndroidProjectBuildDir() const;
	String GetAndroidProjectClassesDir() const;
	String GetAndroidProjectBinDir() const;
	
	String GetAndroidProjectManifestPath() const;
	String GetAndroidProjectJniMakeFilePath() const;
	String GetAndroidProjectJniApplicationMakeFilePath() const;
	
protected:
	String GetFilePathInAndroidProject(const String& nestDir,
	                                   const String& packageName,
	                                   const String& fileName) const;
	
	String RemoveDirNameFromFileName(String fileName) const;
	String NormalizeModuleName(String moduleName) const;
};

void DeletePCHFile(const String& pch_file);

#endif
