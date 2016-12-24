#ifndef _ide_Builders_AndroidBuilder_h_
#define _ide_Builders_AndroidBuilder_h_

#include "Android.h"

#include <ide/Core/Core.h>
#include <ide/Android/Android.h>
#include <ide/Java/Java.h>

namespace Upp {

class AndroidBuilder : public Builder {
public:
	AndroidSDK     sdk;
	AndroidNDK     ndk;
	Jdk            jdk;
	
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
	
	virtual String GetTargetExt() const override;
	virtual bool BuildPackage(
		const String& packageName,
		Vector<String>& linkfile,
		Vector<String>& immfile,
		String& linkoptions,
		const Vector<String>& all_uses,
		const Vector<String>& all_libraries,
	int optimize) override;
	virtual bool Link(
		const Vector<String>& linkfile, const String& linkoptions, bool createmap) override;
	virtual bool Preprocess(
		const String& package,
		const String& file,
		const String& target,
		bool asmout) override;
	virtual void CleanPackage(const String& package, const String& outdir) override;
	virtual void AfterClean() override;
	
protected:
	void ManageProjectCohesion();
	void DetectAndManageUnusedPackages(const String& nest, const Index<String>& packages);
	void DeleteUnusedSourceFiles(
		const String& nest,
		const Vector<String>& files,
		String exts,
		String excludedFiles = "");
	bool MovePackageFileToAndroidProject(const String& src, const String& dest);
	bool RealizePackageJavaSourcesDirectory(const String& packageName);
	bool RealizeLinkDirectories() const;
	
protected:
	bool FileNeedsUpdate(const String& path, const String& data);
	void UpdateFile(const String& path, const String& data);
	void GenerateApplicationMakeFile();
	void GenerateMakeFile();
	bool GenerateRFile();
	
protected:
	bool ValidateBuilderEnviorement();
	void PutErrorOnConsole(const String& msg);
	
	bool SignApk(const String& target, const String& unsignedApkPath);
	bool GenerateDebugKey(const String& keystorePath);
	
	bool AddSharedLibsToApk(const String& apkPath);
	bool PreprocessJava(const String& package, const String& file, const String& target);
	
protected:
	String GetFilePathInAndroidProject(
		const String& nestDir,
		const String& packageName,
		const String& fileName) const;
	
	String RemoveDirNameFromFileName(String fileName) const;
	String NormalizeModuleName(String moduleName) const;
	
	String GetModuleMakeFilePath(const String& packageName);
	
private:
	void   InitProject();
	String GetSandboxDir() const;
	
private:
	AndroidProject project;
	const Workspace& wspc;

private:
	static const String RES_PKG_FLAG;
};

class AndroidBuilderUtils final {
public:
	AndroidBuilderUtils() = delete;
	AndroidBuilderUtils(AndroidBuilderUtils&) = delete;
	AndroidBuilderUtils& operator=(AndroidBuilderUtils&) = delete;
	~AndroidBuilderUtils() = delete;
	
	static String GetAssemblyDir(const String& package);
	static String GetAssemblyDir(const String& packageDir, const String& package);
};

class AndroidModuleMakeFileCreator {
public:
	AndroidModuleMakeFileCreator(const Index<String>& builderConfig);
	virtual ~AndroidModuleMakeFileCreator() {}
	
	void SetModuleName(const String& name) { makeFile.SetName(name); }

	void AddSources(Vector<String>& sources);
	void AddInclude(const String& path);
	void AddIncludeWithSubdirs(const String& path);
	
	void AddIncludes(const Array<OptItem>& uses);
	void AddFlags(const Array<OptItem>& flags);
	void AddLdLibraries(const Array<OptItem>& libraries);
	void AddStaticModuleLibrary(Array<OptItem>& staticLibraries);
	void AddSharedLibraries(const Array<OptItem>& uses);
	
	String Create() { return makeFile.ToString(); }
	
private:
	AndroidModuleMakeFile makeFile;
	const Index<String>&  config;
};
	
}

#endif
