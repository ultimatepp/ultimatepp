#include "Builders.h"

const String AndroidBuilder::RES_PKG_FLAG = "ANDROID_RESOURCES_PACKAGE";

Index<String> AndroidBuilder::GetBuildersNames()
{
	Index<String> builders;
	
	builders.Add("ANDROID");
	
	return builders;
}

AndroidBuilder::AndroidBuilder() :
	wspc(GetIdeWorkspace())
{
}

String AndroidBuilder::GetTargetExt() const
{
	return ".apk";
}

bool AndroidBuilder::BuildPackage(const String& package, Vector<String>& linkfile,
	                              Vector<String>& immfile, String& linkoptions,
	                              const Vector<String>& all_uses,
	                              const Vector<String>& all_libraries,
	                              int)
{
	InitProject();
	if(!ValidateBuilderEnviorement())
		return false;
	
	const bool isMainPackage = HasFlag("MAIN");
	const bool isResourcesPackage = HasFlag(RES_PKG_FLAG);
	String uppManifestPath = PackagePath(package);
	String packageDir = GetFileFolder(uppManifestPath);
	String assemblyDir = GetAssemblyDir(packageDir, package);
	
	ChDir(packageDir);
	PutVerbose("cd " + packageDir);
	
	Package pkg;
	pkg.Load(uppManifestPath);
	
	Vector<String> javaSourceFiles;
	Vector<String> nativeFiles;
	Vector<String> nativeSourceFiles;
	Vector<String> nativeSourceFilesInPackage;
	Vector<String> nativeSourceFilesOptions;
	Vector<String> nativeObjects;
	
	Index<String> noBlitzNativeSourceFiles;
	
	bool error = false;
	String androidManifestPath;
	
	bool isBlitz = HasFlag("BLITZ") && ndk_blitz;
	
	String javaSourcesDir    = project.GetJavaDir();
	String jniSourcesDir     = project.GetJniDir();
	String pkgJavaSourcesDir = javaSourcesDir + DIR_SEPS + package;
	String pkgJniSourcesDir  = jniSourcesDir + DIR_SEPS + package;
	for(int i = 0; i < pkg.GetCount(); i++) {
		if(!IdeIsBuilding())
			return false;
		if(pkg[i].separator)
			continue;
		
		String globalOptions = Gather(pkg[i].option, config.GetKeys());
		
		String filePath       = SourcePath(package, pkg[i]);
		String fileExt        = ToLower(GetFileExt(filePath));
		String fileName       = NormalizePathSeparator(pkg[i]);
		String packageFile    = AppendFileName(package, fileName);
		String packageFileDir = GetFileFolder(packageFile);
		
		if(isResourcesPackage) {
			if(packageFileDir.Find(package + DIR_SEPS) != -1)
				packageFileDir.Remove(0, String(package + DIR_SEPS).GetCount());
			String filePathInAndroidProject = GetFilePathInAndroidProject(project.GetResDir(),
			                                                              packageFileDir,
			                                                              fileName);
			
			if(!MovePackageFileToAndroidProject(filePath, filePathInAndroidProject))
				error = true;
		}
		else
		if(fileExt == ".java") {
			String filePathInAndroidProject = GetFilePathInAndroidProject(javaSourcesDir,
			                                                              packageFileDir,
			                                                              fileName);
			
			if(!RealizePackageJavaSourcesDirectory(package))
				return false;
			if(!MovePackageFileToAndroidProject(filePath, filePathInAndroidProject))
				return false;
			
			javaSourceFiles.Add(filePathInAndroidProject);
		}
		else
		if(fileExt == ".icpp" || fileExt == ".cpp" || fileExt == ".cxx" ||
		   fileExt == ".c"    ||
		   fileExt == ".i"    || fileExt == ".t") {
			String filePathInAndroidProject = GetFilePathInAndroidProject(jniSourcesDir,
			                                                              packageFileDir,
			                                                              fileName);
			
			nativeFiles.Add(filePathInAndroidProject);
			nativeSourceFilesOptions.Add(globalOptions);
			if(pkg[i].noblitz)
				noBlitzNativeSourceFiles.Add(packageFile);
			
			if(!MovePackageFileToAndroidProject(filePath, filePathInAndroidProject))
				return false;
			
			if(fileExt == ".icpp" || fileExt == ".cpp" || fileExt == ".cxx" ||
			   fileExt == ".c") {
				nativeSourceFiles.Add(filePathInAndroidProject);
				nativeSourceFilesInPackage.Add(NormalizePathSeparator(packageFile));
			}
		}
		else
		if(fileExt == ".xml") {
			if(isMainPackage && fileName == "AndroidManifest.xml") {
				if(androidManifestPath.GetCount()) {
					PutConsole("AndroidManifest.xml is duplicated.");
					return false;
				}
				
				if(!FileCopy(filePath, project.GetManifestPath()))
					return false;
					
				androidManifestPath = filePath;
			}
		}
		else
		if(fileExt == ".o") {
			String filePathInAndroidProject = GetFilePathInAndroidProject(jniSourcesDir, packageFileDir, fileName);
			
			if(!MovePackageFileToAndroidProject(filePath, filePathInAndroidProject))
				return false;
			
			nativeObjects.Add(filePathInAndroidProject);
		}
	}
	
	if(isMainPackage && androidManifestPath.IsEmpty()) {
		PutConsole("Failed to find Android manifest file.");
		return false;
	}
	
	DeleteUnusedSourceFiles(pkgJavaSourcesDir, javaSourceFiles, ".java");
	DeleteUnusedSourceFiles(pkgJniSourcesDir,
	                        nativeFiles,
	                        ".icpp, .cpp, .cxx, .c, .hpp, .hxx, .h, .i, .t",
	                        "@blitz.cpp");
	
	if(!isResourcesPackage && !error && !javaSourceFiles.IsEmpty()) {
		if(!RealizeDirectory(project.GetClassesDir()))
			return false;
		
		String compileCmd;
		compileCmd << NormalizeExePath(jdk.GetJavacPath());
		compileCmd << (HasFlag("DEBUG") ? " -g" : " -g:none");
		compileCmd << " -d "<< project.GetClassesDir();
		compileCmd << " -classpath ";
		compileCmd << NormalizeExePath(androidSDK.AndroidJarPath()) << Java::GetDelimiter();
		compileCmd << project.GetBuildDir();
		compileCmd << " -sourcepath ";
		compileCmd << javaSourcesDir << " ";
		for(int i = 0; i < javaSourceFiles.GetCount(); i++) {
			compileCmd << javaSourceFiles[i];
			if(i < javaSourceFiles.GetCount() - 1)
				compileCmd << " ";
		}
		
		linkfile.Add(compileCmd);
	}
	
	if(!isResourcesPackage && !error && !nativeSourceFiles.IsEmpty()) {
		if(isBlitz) {
			BlitzBuilderComponent bc(this);
			Blitz blitz = bc.MakeBlitzStep(nativeSourceFilesInPackage,
			                               nativeSourceFilesOptions,
			                               nativeObjects,
			                               immfile,
			                               ".o",
			                               noBlitzNativeSourceFiles);
			
			String destinationFileName = GetFileName(blitz.path);
			destinationFileName.Replace("$blitz.cpp", "@blitz.cpp");
			
			String blitzDestinationFileInPackage;
			blitzDestinationFileInPackage << package << DIR_SEPS;
			blitzDestinationFileInPackage << destinationFileName;
			
			if(FileExists(blitz.path)) {
				String blitzDestinationFile;
				blitzDestinationFile << project.GetJniDir() << DIR_SEPS;
				blitzDestinationFile << blitzDestinationFileInPackage;
				
				CopyFile(blitzDestinationFile, blitz.path);
			
				nativeSourceFilesInPackage.Clear();
				nativeSourceFilesInPackage.Add(blitzDestinationFileInPackage);
				for(int i = 0; i < noBlitzNativeSourceFiles.GetCount(); i++)
					nativeSourceFilesInPackage.Add(noBlitzNativeSourceFiles[i]);
			}
		}
		
		AndroidModuleMakeFile pkgMakeFile(NormalizeModuleName(package));
		for(int i = 0; i < nativeSourceFilesInPackage.GetCount(); i++)
			pkgMakeFile.AddSourceFile(nativeSourceFilesInPackage[i]);
		
		// TODO: Check for path with space?
		pkgMakeFile.AddInclude(assemblyDir);
		pkgMakeFile.AddInclude(packageDir);
		for (int i = 0; i < pkg.uses.GetCount(); i++)
			pkgMakeFile.AddInclude(GetAssemblyDir(pkg.uses[i].text));
		
		for(int i = 0; i < pkg.flag.GetCount(); i++)
			pkgMakeFile.AddCppFlag(pkg.flag[i].text);
		
		Vector<String> libs = Split(Gather(pkg.library, config.GetKeys()), ' ');
		for(int i = 0; i < libs.GetCount(); i++)
			pkgMakeFile.AddLdLibrary(libs[i]);
		
		Vector<String> staticLibs = Split(Gather(pkg.static_library, config.GetKeys()), ' ');
		for(int i = 0; i < staticLibs.GetCount(); i++)
			pkgMakeFile.AddStaticModuleLibrary(staticLibs[i]);
		
		for(int i = 0; i < pkg.uses.GetCount(); i++)
			pkgMakeFile.AddSharedLibrary(pkg.uses[i].text);
		
		String pkgMakeFilePath = project.GetJniDir() + DIR_SEPS + package + DIR_SEPS + "Android.mk";
		UpdateFile(pkgMakeFilePath, pkgMakeFile.ToString());
	}

	return !error;
}

bool AndroidBuilder::Link(const Vector<String>& linkfile, const String& linkoptions,
	                      bool createmap)
{
	InitProject();
	if(!ValidateBuilderEnviorement())
		return false;
	
	ManageProjectCohesion();
	
	PutConsole("Building Android Project");
	StringStream ss;
	if(!GenerateRFile())
		return false;
	if(!RealizeLinkDirectories())
		return false;
	
	// We need to compile java packages in this place, because we need to generate "R.java" file before...
	// We don't know which packages contain resources.
	int time;
	if(linkfile.GetCount()) {
		PutConsole("-----");
		PutConsole("Compiling java sources...");
		bool error = false;
		time = GetTickCount();
		for(int i = 0; i < linkfile.GetCount(); i++) {
			if(Execute(linkfile[i], ss) != 0) {
				PutConsole(ss.GetResult());
				return false;
			}
		}
		PutConsole("Java sources compiled in " + GetPrintTime(time) + ".");
	}
	
	// Now, we are going to start compiling c/c++ sources
	if(DirectoryExists(project.GetJniDir())) {
		if(!androidNDK.Validate()) {
			PutErrorOnConsole("Android NDK was not detected");
			return false;
		}
		
		time = GetTickCount();
		PutConsole("-----");
		PutConsole("Compiling native sources...");
		
		GenerateApplicationMakeFile();
		GenerateMakeFile();
		
		NDKBuild ndkBuild(androidNDK.GetNdkBuildPath());
		ndkBuild.SetWorkingDir(project.GetDir());
		ndkBuild.SetJobs(GetHydraThreads());
		if(Execute(ndkBuild.MakeCmd(), ss) != 0 ) {
			PutConsole(ss.GetResult());
			return false;
		}
		PutConsole("Native sources compiled in " + GetPrintTime(time) + ".");
	}
	
	if(DirectoryExists(project.GetClassesDir())) {
		PutConsole("-----");
		PutConsole("Creating dex file...");
		String dxCmd;
		dxCmd << NormalizeExePath(androidSDK.DxPath());
		dxCmd << " --dex ";
		dxCmd << "--output=" << project.GetBinDir() << DIR_SEPS << "classes.dex ";
		dxCmd << project.GetClassesDir();
		// PutConsole(dxCmd);
		if(Execute(dxCmd, ss) != 0) {
			PutConsole(ss.GetResult());
			return false;
		}
	}
	
	PutConsole("Creating apk file...");
	String unsignedApkPath = GetSandboxDir() + DIR_SEPS + GetFileTitle(target) + ".unsigned.apk";
	DeleteFile(unsignedApkPath);
	String apkCmd;
	apkCmd << NormalizeExePath(androidSDK.AaptPath());
	apkCmd << " package -v -f";
	if(DirectoryExists(project.GetResDir()))
		apkCmd << " -S " << project.GetResDir();
	apkCmd << " -M " << project.GetManifestPath();
	apkCmd << " -I " << NormalizeExePath(androidSDK.AndroidJarPath());
	apkCmd << " -F " << unsignedApkPath;
	apkCmd << " " << project.GetBinDir();
	// PutConsole(apkCmd);
	if(Execute(apkCmd, ss) != 0) {
		PutConsole(ss.GetResult());
		return false;
	}
	
	if(DirectoryExists(project.GetLibsDir())) {
		PutConsole("Adding native libraries to apk...");
		if(!AddSharedLibsToApk(unsignedApkPath))
			return false;
	}
	
	// In release mode we definitly shouldn't signing apk!!!
	if(!SignApk(target, unsignedApkPath))
		return false;
	
	return true;
}

bool AndroidBuilder::Preprocess(const String& package,
                                const String& file,
                                const String& target,
                                bool asmout)
{
	InitProject();
	
	String ext = GetFileExt(file);
	if(ext == ".java")
		return PreprocessJava(package, file, target);
	return false;
}

void AndroidBuilder::CleanPackage(const String& package, const String& outdir)
{
	InitProject();
	
	Vector<String> pkgDirs;
	Vector<String> pkgFiles;
	if(HasFlag(RES_PKG_FLAG))
		pkgDirs.Add(project.GetResDir());
	else {
		// TODO: handle deletetion of (.class)es
		pkgDirs.Add(project.GetJavaDir(package));
		PutConsole(project.GetJavaDir(package));
		pkgDirs.Add(project.GetJniDir(package));
		for(FindFile ff(AppendFileName(project.GetObjLocalDir(), "*")); ff; ff.Next()) {
			if(!ff.IsHidden() && !ff.IsSymLink() && ff.IsFolder()) {
				pkgFiles.Add(ff.GetPath() + DIR_SEPS + "lib" + package + ".so");
				pkgDirs.Add(ff.GetPath() + DIR_SEPS + "objs" + DIR_SEPS + package);
			}
		}
		for(FindFile ff(AppendFileName(project.GetLibsDir(), "*")); ff; ff.Next())
			if(!ff.IsHidden() && !ff.IsSymLink() && ff.IsFolder())
				pkgFiles.Add(ff.GetPath() + DIR_SEPS + "lib" + package + ".so");
	}
	
	for(int i = 0; i < pkgDirs.GetCount(); i++) {
		String dir = pkgDirs[i];
		if(DirectoryExists(dir))
			DeleteFolderDeep(dir);
	}
	for(int i = 0; i < pkgFiles.GetCount(); i++) {
		String file = pkgFiles[i];
		if(FileExists(file))
			DeleteFile(file);
	}
}

void AndroidBuilder::AfterClean()
{
	InitProject();
	
	String sandboxDir = GetSandboxDir();
	if(DirectoryExists(sandboxDir))
		DeleteFolderDeep(sandboxDir);
}

void AndroidBuilder::ManageProjectCohesion()
{
	Index<String> packages;
	for(int i = 0; i < wspc.GetCount(); i++)
		packages.Add(wspc[i]);
	
	DetectAndManageUnusedPackages(project.GetJavaDir(), packages);
	DetectAndManageUnusedPackages(project.GetJniDir(), packages);
}

void AndroidBuilder::DetectAndManageUnusedPackages(const String& nest,
                                                   const Index<String>& packages)
{
	for(FindFile ff(AppendFileName(nest, "*")); ff; ff.Next()) {
		if(!ff.IsHidden() && ff.IsDirectory()) {
			String name = ff.GetName();
			if(packages.Find(name) == -1)
				CleanPackage(name, "");
		}
	}
}

void AndroidBuilder::DeleteUnusedSourceFiles(const String& nest,
                                             const Vector<String>& files,
                                             String exts,
                                             String excludedFiles)
{
	exts.Replace(" ", "");
	excludedFiles.Replace(" ", "");
	Index<String> extsIdx = Index<String>(Split(exts, ","));
	Index<String> excludedFilesIdx = Index<String>(Split(excludedFiles, ","));
	
	Vector<String> dirs;
	dirs.Add(nest);
	for(int i = 0; i < dirs.GetCount(); i++) {
		for(FindFile ff(AppendFileName(dirs[i], "*")); ff; ff.Next()) {
			if(!ff.IsHidden()) {
				String path = ff.GetPath();
				String name = ff.GetName();
				if(ff.IsFolder() && ff.IsSymLink())
					dirs.Add(ff.GetPath());
				else
				if(extsIdx.Find(GetFileExt(path)) == -1)
					continue;
				else
				if(excludedFilesIdx.Find(name) > -1)
					continue;
				else {
					bool exists = false;
					for(int j = 0; j < files.GetCount(); j++) {
						if(files[j] == path) {
							exists = true;
							break;
						}
					}
					if(!exists)
						DeleteFile(path);
				}
			}
		}
	}
}

bool AndroidBuilder::MovePackageFileToAndroidProject(const String& src, const String& dst)
{
	if(!RealizeDirectory(GetFileDirectory(dst)))
		return false;
	
	if(FileExists(dst)) {
		if(GetFileTime(dst) > GetFileTime(src))
			return true;
	}
	SaveFile(dst, LoadFile(src));
	
	return true;
}

bool AndroidBuilder::RealizePackageJavaSourcesDirectory(const String& packageName)
{
	String dir = project.GetJavaDir() + DIR_SEPS + packageName;
	
	return DirectoryExists(dir) || RealizeDirectory(dir);
}

bool AndroidBuilder::RealizeLinkDirectories() const
{
	if(!RealizeDirectory(project.GetBinDir()))
		return false;
	
	return true;
}

bool AndroidBuilder::SignApk(const String& target, const String& unsignedApkPath)
{
	StringStream ss;
	
	String signedApkPath = GetSandboxDir() + DIR_SEPS + GetFileTitle(target) + ".signed.apk";
	if(HasFlag("DEBUG")) {
		String keystorePath = GetSandboxDir() + DIR_SEPS + "debug.keystore";
		if(!GenerateDebugKey(keystorePath))
			return false;
	
		PutConsole("Signing apk file...");
		DeleteFile(signedApkPath);
		String jarsignerCmd;
		jarsignerCmd << NormalizeExePath(jdk.GetJarsignerPath());
		
		// Up to Java 6.0 below alogirms was by default
		// (In Java 7.0 and above we need to manually specific this algorithms)
		jarsignerCmd << " -sigalg SHA1withRSA";
		jarsignerCmd << " -digestalg SHA1";
		
		jarsignerCmd << " -keystore " + keystorePath;
		jarsignerCmd << " -storepass android";
		jarsignerCmd << " -keypass android";
		// TODO: not sure about below line. But I think for debug purpose we shouldn't use tsa.
		// http://en.wikipedia.org/wiki/Trusted_timestamping
		//jarsignerCmd << " -tsa https://timestamp.geotrust.com/tsa";
		jarsignerCmd << " -signedjar " << signedApkPath;
		jarsignerCmd << " " << unsignedApkPath;
		jarsignerCmd << " androiddebugkey";
		//PutConsole(jarsignerCmd);
		if(Execute(jarsignerCmd, ss) != 0) {
			PutConsole(ss.GetResult());
			return false;
		}
		
		PutConsole("Aliging apk file...");
		DeleteFile(target);
		String zipalignCmd;
		zipalignCmd << NormalizeExePath(androidSDK.ZipalignPath());
		zipalignCmd << " -f 4 ";
		zipalignCmd << (HasFlag("DEBUG") ? signedApkPath : unsignedApkPath) << " ";
		zipalignCmd << target;
		//PutConsole(zipalignCmd);
		if(Execute(zipalignCmd, ss) != 0) {
			PutConsole(ss.GetResult());
			return false;
		}
	}
	
	return true;
}

bool AndroidBuilder::GenerateDebugKey(const String& keystorePath)
{
	StringStream ss;
	
	if(!FileExists(keystorePath)) {
		PutConsole("Generating debug key...");
		
		String keytoolCmd;
		keytoolCmd << NormalizeExePath(jdk.GetKeytoolPath());
		keytoolCmd << " -genkeypair -alias androiddebugkey -keypass android";
		keytoolCmd << " -keystore " << keystorePath;
		keytoolCmd << " -storepass android -dname \"CN=Android Debug,O=Android,C=US\"";
		keytoolCmd << " -keyalg RSA";
		keytoolCmd << " -validity 100000";
		//PutConsole(keytoolCmd);
		if(Execute(keytoolCmd, ss) != 0) {
			PutConsole(ss.GetResult());
			return false;
		}
	}
	return true;
}

bool AndroidBuilder::AddSharedLibsToApk(const String& apkPath)
{
	// TODO: A little bit workearound (I know one thing that shared libs should be in "lib" directory not in "libs")
	// So, we need to create temporary lib directory with .so files :(
	const String libDir = project.GetDir() + DIR_SEPS + "lib";
	
	Vector<String> sharedLibsToAdd;
	for(FindFile ff(AppendFileName(project.GetLibsDir(), "*")); ff; ff.Next()) {
		if (!ff.IsHidden () && !ff.IsSymLink () && ff.IsDirectory()) {
			for(FindFile ffa(AppendFileName (ff.GetPath(), "*")); ffa; ffa.Next ()) {
				if(!ffa.IsHidden() && !ffa.IsSymLink() && !ffa.IsDirectory()) {
					// TODO: in libs directory we can find another java libs (.jar)
					String fileExt = ToLower(GetFileExt(ffa.GetPath()));
					if(fileExt == ".so") {
						const String libPath = String("lib") + DIR_SEPS + ff.GetName() + DIR_SEPS + ffa.GetName();
						const String destPath = project.GetDir() + DIR_SEPS + libPath;
						if(!RealizePath(destPath) || !FileCopy(ffa.GetPath(), destPath))
							return false;
						sharedLibsToAdd.Add(libPath);
					}
				}
			}
		}
	}
	
	ChDir(project.GetDir());
	String aaptAddCmd;
	aaptAddCmd << NormalizeExePath(androidSDK.AaptPath());
	aaptAddCmd << " add " << apkPath;
	for(int i = 0; i < sharedLibsToAdd.GetCount(); i++)
		aaptAddCmd << " " << sharedLibsToAdd[i];
	// PutConsole(aaptAddCmd);
	StringStream ss;
	if(Execute(aaptAddCmd, ss) != 0) {
		PutConsole(ss.GetResult());
		return false;
	}
	if(!DeleteFolderDeep(libDir))
		return false;
	
	return true;
}

bool AndroidBuilder::ValidateBuilderEnviorement()
{
	if(!androidSDK.Validate()) {
		PutErrorOnConsole("Android SDK was not detected");
		return false;
	}
	if(!androidSDK.ValidateBuildTools()) {
		PutErrorOnConsole("Android SDK build tools was not detected");
		return false;
	}
	if(!androidSDK.ValidatePlatform()) {
		PutErrorOnConsole("Android SDK platform was not detected");
		return false;
	}
	if(!jdk.Validate()) {
		PutErrorOnConsole("JDK was not detected");
		return false;
	}
	
	return true;
}

void AndroidBuilder::PutErrorOnConsole(const String& msg)
{
	PutConsole("Error: " + msg + ".");
}

bool AndroidBuilder::FileNeedsUpdate(const String& path, const String& data)
{
	return !(FileExists(path) && LoadFile(path).Compare(data) == 0);
}

void AndroidBuilder::UpdateFile(const String& path, const String& data)
{
	if(!FileNeedsUpdate(path, data))
		return;
	
	SaveFile(path, data);
}

void AndroidBuilder::GenerateApplicationMakeFile()
{
	AndroidApplicationMakeFile makeFile;
	makeFile.SetPlatform(androidSDK.GetPlatform());
	makeFile.SetArchitectures(ndkArchitectures);
	makeFile.SetCppRuntime(ndkCppRuntime);
	makeFile.SetCppFlags(ndkCppFlags);
	makeFile.SetCFlags(ndkCFlags);
	makeFile.SetOptim(HasFlag("DEBUG") ? "debug" : "release");
	makeFile.SetToolchain(ndkToolchain);
	
	PutVerbose("Architectures: " + AsString(ndkArchitectures));
	PutVerbose("CppRuntime: " + ndkCppRuntime);
	PutVerbose("CppFlags: " + ndkCppFlags);
	PutVerbose("CFlags: " + ndkCFlags);
	PutVerbose("Toolchain: " + ndkToolchain);
	
	UpdateFile(project.GetJniApplicationMakeFilePath(), makeFile.ToString());
}

void AndroidBuilder::GenerateMakeFile()
{
	const String makeFileName = "Android.mk";
	const String baseDir = project.GetJniDir();
	Vector<String> modules;
	
	BiVector<String> dirs;
	dirs.AddHead(baseDir);
	while(!dirs.IsEmpty()) {
		String currentDir = dirs.Head();
		for(FindFile ff1(AppendFileName(currentDir, "*")); ff1; ff1.Next()) {
			if(!ff1.IsHidden() && !ff1.IsSymLink() && ff1.IsFolder()) {
				bool isEmpty = true;
				for(FindFile ff2(AppendFileName(ff1.GetPath(), "*")); ff2; ff2.Next()) {
					if(!ff2.IsHidden() && !ff2.IsSymLink() && ff2.IsFile()) {
						isEmpty = false;
						break;
					}
				}
				
				if(!isEmpty) {
					String moduleName = ff1.GetPath();
					moduleName.Remove(moduleName.Find(baseDir), baseDir.GetCount() + 1);
					modules.Add(moduleName);
				}
				else {
					dirs.AddTail(ff1.GetPath());
				}
			}
		}
		dirs.DropHead();
	}
	
	AndroidMakeFile makeFile;
	makeFile.AddHeader();
	for(int i = 0; i < modules.GetCount(); i++)
		makeFile.AddInclusion(modules[i] + DIR_SEPS + makeFileName);
	
	UpdateFile(project.GetJniMakeFilePath(), makeFile.ToString());
}

bool AndroidBuilder::GenerateRFile()
{
	// TODO: gen in gen folder
	if(DirectoryExists(project.GetResDir())) {
		StringStream ss;
		String aaptCmd;
		aaptCmd << NormalizeExePath(androidSDK.AaptPath());
		aaptCmd << " package -v -f -m";
		aaptCmd << " -S " << project.GetResDir();
		aaptCmd << " -J " << project.GetJavaDir();
		aaptCmd << " -M " << project.GetManifestPath();
		aaptCmd << " -I " << NormalizeExePath(androidSDK.AndroidJarPath());
		
		if(Execute(aaptCmd, ss) != 0) {
			PutConsole(ss.GetResult());
			return false;
		}
	}
	
	return true;
}

bool AndroidBuilder::PreprocessJava(const String& package, const String& file, const String& target)
{
	StringStream ss;
	String ext = GetFileExt(file);
	if(ext != ".java")
		return false;
	String fileName = GetFileName(file);
	fileName.Replace(ext, "");
	String targetDir = GetFileDirectory(target);
	String classesDir = targetDir + "classes";
	String classFileName = fileName + ".class";
	
	if(DirectoryExists(classesDir))
		DeleteFolderDeep(classesDir);
	RealizeDirectory(classesDir);
	
	String compileCmd;
	compileCmd << NormalizeExePath(jdk.GetJavacPath());
	compileCmd << " -d "<< classesDir;
	compileCmd << " -classpath ";
	compileCmd << NormalizeExePath(androidSDK.AndroidJarPath()) << Java::GetDelimiter();
	compileCmd << project.GetBuildDir();
	compileCmd << " -sourcepath " << project.GetJavaDir();
	compileCmd << " " << file;
	if(Execute(compileCmd) != 0)
		return false;
	
	String classFileDir;
	BiVector<String> paths;
	paths.AddTail(classesDir);
	while(!paths.IsEmpty()) {
		for(FindFile ff(AppendFileName(paths.Head(), "*")); ff; ff.Next()) {
			if(!ff.IsHidden()) {
				if(!ff.IsSymLink() && ff.IsDirectory())
					paths.AddTail(ff.GetPath());
				else
				if(ff.GetName().Compare(classFileName) == 0) {
					classFileDir = GetFileDirectory(ff.GetPath());
					paths.Clear();
					break;
				}
			}
		}
		if(!paths.IsEmpty())
			paths.DropHead();
	}
	
	if(classFileDir.IsEmpty())
		return false;
	
	String relativeClassFileDir = classFileDir;
	relativeClassFileDir.Replace(classesDir + DIR_SEPS, "");
	String className = relativeClassFileDir + fileName;
	className.Replace(DIR_SEPS, ".");
	
	String javahCmd;
	javahCmd << NormalizeExePath(jdk.GetJavahPath());
	javahCmd << " -classpath ";
	javahCmd << classesDir << Java::GetDelimiter();
	javahCmd << NormalizeExePath(androidSDK.AndroidJarPath()) << Java::GetDelimiter();
	javahCmd << project.GetBuildDir();
	javahCmd << " -o " << target;
	javahCmd << " " << className;
	// PutConsole(javahCmd);
	if(Execute(javahCmd, ss) != 0) {
		PutConsole(ss.GetResult());
		return false;
	}
	
	return true;
}

// -------------------------------------------------------------------

void AndroidBuilder::InitProject()
{
	if(!project.HasDir())
		project.SetDir(GetSandboxDir() + DIR_SEPS + "AndroidProject");
}

String AndroidBuilder::GetSandboxDir() const
{
	if(target.IsEmpty())
		return String();
	
	int targetExtLen = GetTargetExt().GetCount();
	String mainPackageName = GetFileName(target);
	mainPackageName.Remove(mainPackageName.GetCount() - targetExtLen, targetExtLen);
	return GetFileFolder(target) + DIR_SEPS + "Sandbox" + DIR_SEPS + mainPackageName;
}

String AndroidBuilder::GetAssemblyDir(const String& package)
{
	String packageManifest = PackagePath(package);
	String packageDir = GetFileFolder(packageManifest);
	
	return GetAssemblyDir(packageDir, package);
}

String AndroidBuilder::GetAssemblyDir(const String& packageDir, const String& package)
{
	String assemblyDir = packageDir;
	
	int pos = packageDir.GetCount() - package.GetCount() - 1;
	if (pos >= 0 && pos < assemblyDir.GetCount())
		assemblyDir.Trim(pos);
	
	return assemblyDir != packageDir ? assemblyDir : "";
}

// -------------------------------------------------------------------

String AndroidBuilder::GetFilePathInAndroidProject(const String& nestDir,
	                                               const String& packageName,
	                                               const String& fileName) const
{
	return nestDir + DIR_SEPS + packageName + DIR_SEPS + RemoveDirNameFromFileName(fileName);
}

String AndroidBuilder::RemoveDirNameFromFileName(String fileName) const
{
	int idx = fileName.ReverseFind(DIR_SEPS);
	if(idx >= 0)
		fileName.Remove(0, idx);
	return fileName;
}

String AndroidBuilder::NormalizeModuleName(String moduleName) const
{
	moduleName.Replace(DIR_SEPS, "_");
	return moduleName;
}

// -------------------------------------------------------------------

Builder *CreateAndroidBuilder()
{
	return new AndroidBuilder();
}

INITIALIZER(AndroidBuilder)
{
	Index<String> builders = AndroidBuilder::GetBuildersNames();
	for(int i = 0; i < builders.GetCount(); i++)
		RegisterBuilder(builders[i], &CreateAndroidBuilder);
}
