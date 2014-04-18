#include	<Core/Core.h>

#include "Oce2Upp.h"

#include "Oce2Upp.brc"

int iTk, numTk;

////////////////////////////////////////////////////////////////////////////////////////////////
// GETS DIRECTORY CONTENT
Vector<String> Oce2Upp::GetDirectoryContent(const String &Path, bool Folders)
{
	
	// GETS INCLUDE FOLDER CONTENTS
	Array<FileSystemInfo::FileInfo> DirFiles;
	DirFiles = StdFileSystemInfo().Find(Path);
	
	// COPIES ALL NAMES INSIDE AN ARRAY
	Vector<String> DirArray;
	for(int i = 0 ; i < DirFiles.GetCount() ; i++)
	{
		if( (Folders && DirFiles[i].is_folder && DirFiles[i].filename != ".svn" && DirFiles[i].filename != ".git") || (!Folders &&  !DirFiles[i].is_folder) && DirFiles[i].filename != "." && DirFiles[i].filename != "..")
		  DirArray << DirFiles[i].filename;
	}
	
	return DirArray;

} // END OpenCascadePackager::GetDirectoryContent()

// reads an UDLIST file to gather modules/toolkits mapping
bool Oce2Upp::ReadUDLIST(void)
{
	FileIn f(AppendFileName(admDir, "UDLIST"));
	if(!f)
		return false;
	
	// get all toolkits from file
	while(!f.IsEof())
	{
		String l = f.GetLine();
		if(!l.StartsWith("t "))
			continue;
		toolkits.Add(l.Mid(2));
	}
	return true;
}

// builds database
bool Oce2Upp::BuildDb(void)
{
	// read all toolkits names
	if(!ReadUDLIST())
		return false;

	Sort(toolkits);
	for(int iTool = 0; iTool < toolkits.GetCount(); iTool++)
	{
		String tkName = toolkits.GetKey(iTool);
		Vector<String> &p = toolkits[iTool];
		FileIn f(AppendFileName(srcDir, tkName + "/PACKAGES"));
		if(!f)
			return false;
		while(!f.IsEof())
		{
			String l = f.GetLine();
			if(l == "")
				continue;
			p.Add(l);
		}
		f.Close();
		Sort(p);

		Vector<String> &dep = dependencies.Add(tkName);
		f.Open(AppendFileName(srcDir, tkName + "/EXTERNLIB"));
		if(!f)
			continue; // I guess toolkit can have no dependencies....
		while(!f.IsEof())
		{
			String l = f.GetLine();
			if(l == "")
				continue;
			dep.Add(l);
		}
		f.Close();
	}
	
	return true;
}

// copy include files from inc and various src folders
// in 'include' folder
bool Oce2Upp::CopyIncludes(String const &pacRoot)
{
	Vector<String> files;

	String destPath = AppendFileName(pacRoot, "include");
	RealizeDirectory(destPath);
	
	action = "Copying include files";
	Ctrl::ProcessEvents();
	
	files = GetDirectoryContent(AppendFileName(incDir, "*"), false);
	numTk = files.GetCount();
	iTk = 0;
	progress.Set(iTk, numTk);
	Ctrl::ProcessEvents();
	for(int iFile = 0; iFile < files.GetCount(); iFile++)
	{
		FileCopy(AppendFileName(incDir, files[iFile]), AppendFileName(destPath, files[iFile]));
		iTk++;
		progress.Set(iTk, numTk);
		Ctrl::ProcessEvents();
	}
	
	Vector<String> folders = GetDirectoryContent(AppendFileName(srcDir, "*"), true);

	numTk = folders.GetCount();
	iTk = 0;
	progress.Set(iTk, numTk);
	Ctrl::ProcessEvents();
	for(int iFolder = 0; iFolder < folders.GetCount(); iFolder++)
	{
		String sPath = AppendFileName(srcDir, folders[iFolder]);
		files = GetDirectoryContent(AppendFileName(sPath, "*.hxx"), false);
		files.Append(GetDirectoryContent(AppendFileName(sPath, "*.h"), false));
		files.Append(GetDirectoryContent(AppendFileName(sPath, "*.lxx"), false));
		files.Append(GetDirectoryContent(AppendFileName(sPath, "*.gxx"), false));
		for(int iFile = 0; iFile < files.GetCount(); iFile++)
			FileCopy(AppendFileName(sPath, files[iFile]), AppendFileName(destPath, files[iFile]));
		iTk++;
		progress.Set(iTk, numTk);
		Ctrl::ProcessEvents();
	}
	
	return true;
}

// create the config file
// this file will be force included on each copiler call
// using -FI (MSC) or -include (GCC), so we shall keep short
bool Oce2Upp::BuildConfig(String const &pacRoot)
{
	// build OCE.h file
	FileOut f(AppendFileName(pacRoot, "OCE_UPP_CONFIG.h"));
	f <<
		"#ifndef _OCE_UPP_CONFIG_H\n"
		"#define _OCE_UPP_CONFIG_H\n"
			"#ifndef _CPPRTTI\n"
				"#define _CPPRTTI\n"
			"#endif\n"
			"#include <Core/config.h>\n"
			"#ifdef PLATFORM_POSIX\n"
				"#define OCE_HAVE_FSTREAM\n"
				"#define OCE_HAVE_IOSTREAM\n"
				"#define OCE_HAVE_IOMANIP\n"
				"#define OCE_HAVE_IOS\n"
				"#define OCE_HAVE_LIMITS_H\n"
				"#define HAVE_SYS_STAT_H\n"
				"#define HAVE_STRING_H\n"
				"#define HAVE_UNISTD_H\n"
				"#define HAVE_SIGNAL_H\n"
				"#define HAVE_SYS_SEM_H\n"
				"#define HAVE_STATFS\n"
				"#define HAVE_SYS_VFS_H\n"
				"#define HAVE_SYS_TIME_H\n"
				"#define HAVE_SYS_TIMES_H\n"
				"#define HAVE_DIRENT_H\n"
				"#define HAVE_DLFCN_H\n"
				"#define HAVE_PWD_H\n"
				"#define HAVE_SYS_UTSNAME_H\n"
				"#define HAVE_NETDB_H\n"
				"#define HAVE_SYS_MMAN_H\n"
				"#define OCE_LIBRARY_PREFIX \"lib\"\n"
				"#define OCE_LIBRARY_DEBUG_POSTFIX \"\"\n"
				"#define OCE_LIBRARY_EXTENSION \"\"\n"
			"#else\n"
				"#define WNT\n"
				"#define OCE_IS_DUPLICATE_UINT32_T\n"
				"#ifdef __APPLE__\n"
					"typedef void *GLhandleARB;\n"
				"#else\n"
					"typedef unsigned int GLhandleARB;\n"
				"#endif\n"
				"typedef char GLcharARB;\n"
				"#define OCE_LIBRARY_PREFIX \"\"\n"
				"#define OCE_LIBRARY_DEBUG_POSTFIX \"d\"\n"
				"#define OCE_LIBRARY_EXTENSION \"lib\"\n"
			"#endif\n"
	
			"#define CSFDB\n"

		"#endif\n"
	;
	f.Close();
	
	return true;
}

// create a main package file -- by now, dummy, just to avoid build error
bool Oce2Upp::BuildMain(String const &pacRoot)
{
	FileOut f(AppendFileName(pacRoot, builderName + ".cpp"));
	f <<
		"int main()\n"
		"{\n"
		"	return 0;\n"
		"}\n"
	;
	f.Close();
	return true;
}

// creates the UPP package
bool Oce2Upp::MakeBuilderPackage(void)
{
	String builderFolder = AppendFileName(destRoot, builderName);
	
	RealizeDirectory(builderFolder);

	CopyIncludes(builderFolder);
	
	// overwrite 'Image_AlienPixMap.hxx file with custom one
	// and create Image_AlienPixMap.cxx file in replacement of OCC one
	// to use Upp imaging code
	String s = (const char *)Image_AlienPixMap_HXX;
	SaveFile(AppendFileName(builderFolder, "include/Image_AlienPixMap.hxx"), s);
	
	// overwrite Standard_MMgrRoot.hxx file with custom one
	s = (const char *)Standard_MMgrRoot_HXX;
	SaveFile(AppendFileName(builderFolder, "include/Standard_MMgrRoot.hxx"), s);
	
	// create upp package file and populate starting part
	FileOut uppFile(AppendFileName(builderFolder, builderName + ".upp"));
	if(!uppFile)
		return false;
	
	uppFile << "noblitz;\n";
	uppFile
		<< "options(MSC) \"-FI \\\"OCE_UPP_CONFIG.h\\\"\";\n"
		<< "options(GCC) \"--include \\\"OCE_UPP_CONFIG.h\\\"\";\n"
	;

	uppFile <<
		"include\n"
		"	./include;\n"
	;
	uppFile <<
		"mainconfig\n"
		"	\"\" = \"SO MT\";\n"
	;
	uppFile
		<< "file\n"
		<< "	OCE_UPP_CONFIG.h,\n"
		<< "	" + builderName + ".cpp;\n"
	;

	// build dependent toolkits as packages
	String sUpp;

	action = "Generating packages";
	numTk = toolkits.GetCount();
	iTk = 0;
	progress.Set(iTk, numTk);
	Ctrl::ProcessEvents();

	for(int iTool = 0; iTool < toolkits.GetCount(); iTool++)
	{
		iTk++;
		progress.Set(iTk);
		Ctrl::ProcessEvents();
		
		// get toolkit name
		String toolkit = toolkits.GetKey(iTool);
		
		// skip unneeded toolkits
		if(
				toolkit == "TKDraw"
			||	toolkit == "TKViewerTest"
			||	toolkit == "TKDCAF"
			||	toolkit == "TKQADraw"
			||	toolkit == "TKTObjDRAW"
			||	toolkit == "TKTopTest"
			||	toolkit == "TKXDEDRAW"
			||	toolkit == "TKXSDRAW"
		)
			continue;
			
		// realize toolkit package path
		String tkFolder = AppendFileName(destRoot, libPrefix + toolkit);
		RealizeDirectory(tkFolder);
		
		// add toolkit 'use' to main package
		sUpp << "uses " + libPrefix + toolkit + ";\n";
		
		// create toolkit package file
		FileOut tkFile(AppendFileName(tkFolder, libPrefix + toolkit + ".upp"));
		if(!tkFile)
			return false;
		String sTk;

		tkFile << "noblitz;\n";
		tkFile
			<< "options(MSC) \"-FI \\\"../OCE_UPP_CONFIG.h\\\"\";\n"
			<< "options(GCC) \"--include \\\"../OCE_UPP_CONFIG.h\\\"\";\n"
		;
		
		// opengl toolkit needs GL and GLU libs...
		if(toolkit == "TKOpenGl")
			tkFile <<
				"library(WIN32) \"glu32 opengl32\";\n"
				"library(LINUX) \"GL GLU\";\n"
				"library(FREEBSD) \"GL GLU\";\n"
			;
		
		if(toolkit == "TKernel")
			tkFile << "library(WIN32) \"user32 advapi32\";\n";
		
		if(toolkit == "TKGeomAlgo")
			tkFile << "options(WIN32) -D__AppBlend_DLL;\n";
		
		if(toolkit == "TKFillet")
			tkFile << "options(WIN32) -D__Blend_DLL;\n";

		if(toolkit == "TKSTEP")
			tkFile << "library(WIN32) \"Ws2_32\";\n";
		
		if(toolkit == "TKV3d")
			tkFile << "library(WIN32) \"Gdi32\";\n";

		if(toolkit == "TKNIS")
			tkFile <<
				"library(WIN32) \"glu32 opengl32\";\n"
				"library(LINUX) \"GL GLU\";\n"
				"library(FREEBSD) \"GL GLU\";\n"
			;

		// packages (toolkits) used by this one -- aka dependencies
		String tkUpp;
		Vector<String> const &deps = dependencies.Get(toolkit);
		for(int iDep = 0; iDep < deps.GetCount(); iDep++)
		{
			String const &dep = deps[iDep];
			if(!dep.StartsWith("TK") && !dep.StartsWith("PTK"))
				continue;
			tkUpp << "\t" << libPrefix + dep << ",\n"; 
		}
		
		if(toolkit == "TKService")
			tkUpp << "\tFreeType,\n";
		
		if(tkUpp != "")
		{
			tkUpp = tkUpp.Left(tkUpp.GetCount() - 2) + ";\n";
			tkFile << "uses\n" << tkUpp;
		}
		
		tkFile <<
			"file\n"
		;

		// gather all packages for this toolkit
		Vector<String> &pk = toolkits[iTool];
		for(int iPack = 0; iPack < pk.GetCount(); iPack++)
		{
			String const &package = pk[iPack];
			
#ifdef PLATFORM_POSIX
			if(package == "WNT")
				continue;
#else
			if(package == "Xw")
				continue;
#endif
			sTk << "	\"  PACKAGE:" << package << "\" readonly separator,\n";

			// get source folders for current package
			String srcPath = AppendFileName(srcDir, package) + "/";
			String drvPath = AppendFileName(drvDir, package) + "/";
			
			Vector<String> files;
			files = GetDirectoryContent(srcPath + "*.cxx", false);
			files.Append(GetDirectoryContent(srcPath + "*.cpp", false));
			files.Append(GetDirectoryContent(srcPath + "*.c", false));
			Sort(files);
			for(int iFile = 0; iFile < files.GetCount(); iFile++)
			{
				String fName = files[iFile];
				
				// special handling for Image_AlienPixMap.cxx file
				// it must be replaced with the one patched for Upp
				if(fName == "Image_AlienPixMap.cxx")
				{
					String s = (const char *)Image_AlienPixMap_CXX;
					String path = AppendFileName(tkFolder, "IMAGE/");
					RealizeDirectory(path);
					path = AppendFileName(path, fName);
					SaveFile(path, s);
					sTk << "	" << path << "\n";
					sTk << "		options() -I" << srcPath << "\n";
					sTk << "		options() -I" << drvPath << ",\n";
					continue;
				}
				
				// special handling for AIS_InteractiveContext.cxx
				// must be patched to correctly update AIS_MultipleConnectedInteractive
				// objects if they contains shapes
				else if(fName == "AIS_InteractiveContext.cxx")
				{
					String pattern = "anObj->IsKind(STANDARD_TYPE(AIS_MultipleConnectedShape)) );";
					String rPattern = "anObj->IsKind(STANDARD_TYPE(AIS_MultipleConnectedShape)) ||";
					String aPattern = "                                  anObj->IsKind(\"AIS_MultiInteractive\") );";
					String sourcePath = AppendFileName(srcPath, fName);
					String destPath = AppendFileName(tkFolder, "AIS/");
					RealizeDirectory(destPath);
					destPath = AppendFileName(destPath, fName);
					FileIn sFile(sourcePath);
					FileOut dFile(destPath);
					while(!sFile.IsEof())
					{
						String s = sFile.GetLine();
						int pos = s.Find(pattern);
						if(pos >= 0)
						{
							s.Replace(pattern, rPattern);
							dFile.PutLine(s);
							s = aPattern;
						}
						dFile.PutLine(s);
					}
					sFile.Close();
					dFile.Close();
					sTk << "	" << destPath << "\n";
					sTk << "		options() -I" << srcPath << "\n";
					sTk << "		options() -I" << drvPath << ",\n";
					continue;
				}
				sTk << "	" << AppendFileName(srcPath, fName) << "\n";
				sTk << "		options() -I" << srcPath << "\n";
				sTk << "		options() -I" << drvPath << ",\n";
			}
			files = GetDirectoryContent(drvPath + "*.cxx", false);
			files.Append(GetDirectoryContent(drvPath + "*.cpp", false));
			files.Append(GetDirectoryContent(drvPath + "*.c", false));
			Sort(files);
			for(int iFile = 0; iFile < files.GetCount(); iFile++)
			{
				String fName = files[iFile];
				sTk << "	" << AppendFileName(drvPath, fName) << "\n";
				sTk << "		options() -I" << srcPath << "\n";
				sTk << "		options() -I" << drvPath << ",\n";
			}
		}
		if(sTk != "")
		{
			sTk = sTk.Left(sTk.GetCount() - 2) + ";\n";
			tkFile << sTk;
		}
		tkFile.Close();

	}

	if(sUpp != "")
	{
		sUpp = sUpp.Left(sUpp.GetCount() - 2) + ";\n";
		uppFile << sUpp;
	}
	uppFile.Close();

	// create the config file
	BuildConfig(builderFolder);
	
	// create main file
	BuildMain(builderFolder);

	return true;
}

// creates the UPP OCE package
bool Oce2Upp::MakeOCEPackage(void)
{
	String pacFolder = AppendFileName(destRoot, oceName);
	
	RealizeDirectory(pacFolder);
	String includeDest = AppendFileName(pacFolder, "include");
	RealizeDirectory(includeDest);

	// copy all include files from builder package here
	// so we don't need them in some system place....
	String includeFolder = AppendFileName(destRoot, builderName + "/include");
	Vector<String> includes = GetDirectoryContent(AppendFileName(includeFolder, "*"), false);
	iTk = 0;
	numTk = includes.GetCount();
	progress.Set(iTk, numTk);
	action = "Copying include files";
	Ctrl::ProcessEvents();
	for(int iInc = 0; iInc < includes.GetCount(); iInc++)
	{
		FileCopy(AppendFileName(includeFolder, includes[iInc]), AppendFileName(includeDest, includes[iInc]));
		iTk++;
		progress.Set(iTk);
		Ctrl::ProcessEvents();
	}
	
	// patch a file wich clashes with UPP
	// problem is in global IsEqual template that clashes with UPP::IsEqual one
	String s = LoadFile(AppendFileName(includeDest, "NCollection_DefaultHasher.hxx"));
	int pos = s.Find("return IsEqual(theKey1, theKey2);");
	if(pos >= 0)
		s.Insert(pos+7, "::");
	SaveFile(AppendFileName(includeDest, "NCollection_DefaultHasher.hxx"), s);
	
	// overwrite 'Image_AlienPixMap.hxx file with custom one
	// and create Image_AlienPixMap.cxx file in replacement of OCC one
	// to use Upp imaging code
	s = (const char *)Image_AlienPixMap_HXX;
	SaveFile(AppendFileName(includeDest, "Image_AlienPixMap.hxx"), s);
	
	// overwrite Standard_MMgrRoot.hxx file with custom one
	s = (const char *)Standard_MMgrRoot_HXX;
	SaveFile(AppendFileName(includeDest, "Standard_MMgrRoot.hxx"), s);
	
	// build OCE.h file
	FileOut f(AppendFileName(pacFolder, "OCE.h"));
	f <<
		"#ifndef _OCE_H\n"
		"#define _OCE_H\n"
		"#include \"include/oce-config.h\"\n"
		"#endif\n"
	;
	f.Close();

	// create upp package files
	action = "Creating package files";
	iTk = 0;
	numTk = 6;
	progress.Set(iTk, numTk);
	Ctrl::ProcessEvents();
	FileOut uppFile;
	
	uppFile.Open(AppendFileName(pacFolder, oceName + ".upp"));
	s.Clear();
	s <<
		"noblitz;\n"
		"uses\n"
		"	CtrlCore,\n"
		"	plugin/bmp,\n"
		"	plugin/gif,\n"
		"	plugin/jpg,\n"
		"	plugin/pcx,\n"
		"	plugin/png,\n"
		"	plugin/tif;\n"
		"library\n"
	;
	s << "	\"";
	for(int iTool = 0; iTool < toolkits.GetCount(); iTool++)
	{
		// get toolkit name
		String toolkit = toolkits.GetKey(iTool);
		
		// skip unneeded toolkits
		if(
				toolkit == "TKDraw"
			||	toolkit == "TKViewerTest"
			||	toolkit == "TKDCAF"
			||	toolkit == "TKQADraw"
			||	toolkit == "TKTObjDRAW"
			||	toolkit == "TKTopTest"
			||	toolkit == "TKXDEDRAW"
			||	toolkit == "TKXSDRAW"
		)
			continue;
		s << libPrefix << toolkit << " ";
	}
	s << "\";\n";
	s <<
		"library(POSIX) \"GL GLU\";\n"
		"library(WIN32) opengl32;\n"
		"include\n"
		"	./include;\n"
		"file\n"
		"	OCE.h,\n"
		"	OCECtrl.h,\n"
		"	OCEDoc.h,\n"
		"	OCECtrl.cpp,\n"
		"	OCEDoc.cpp,\n"
		"	OCE_AlienPixMap.icpp;\n"
	;
	uppFile << s;
	uppFile.Close();
	progress.Set(++iTk);
	Ctrl::ProcessEvents();
	
	uppFile.Open(AppendFileName(pacFolder, "OCE.h"));
	s = (const char *)OCE_H;
	uppFile << s;
	uppFile.Close();
	progress.Set(++iTk);
	Ctrl::ProcessEvents();
	
	uppFile.Open(AppendFileName(pacFolder, "OCEDoc.h"));
	s = (const char *)OCEDoc_H;
	uppFile << s;
	uppFile.Close();
	progress.Set(++iTk);
	Ctrl::ProcessEvents();
	
	uppFile.Open(AppendFileName(pacFolder, "OCEDoc.cpp"));
	s = (const char *)OCEDoc_CPP;
	uppFile << s;
	uppFile.Close();
	progress.Set(++iTk);
	Ctrl::ProcessEvents();
	
	uppFile.Open(AppendFileName(pacFolder, "OCECtrl.h"));
	s = (const char *)OCECtrl_H;
	uppFile << s;
	uppFile.Close();
	progress.Set(++iTk);
	Ctrl::ProcessEvents();
	
	uppFile.Open(AppendFileName(pacFolder, "OCECtrl.cpp"));
	s = (const char *)OCECtrl_CPP;
	uppFile << s;
	uppFile.Close();
	progress.Set(++iTk);
	Ctrl::ProcessEvents();

	uppFile.Open(AppendFileName(pacFolder, "OCE_AlienPixMap.icpp"));
	s = (const char *)OCE_AlienPixMap_ICPP;
	uppFile << s;
	uppFile.Close();
	progress.Set(++iTk);
	Ctrl::ProcessEvents();

	return true;
}

// callbacks
void Oce2Upp::cancelCb(void)
{
	canceled = true;
	Break(0);
}

void Oce2Upp::okCb(void)
{
	progress.Set(0, 100);
	progress.Show();
	action = "";
	
	// check if source folder exists and is an OCE root
	if(
		!DirectoryExists(~sourceFolder) ||
		!DirectoryExists(AppendFileName(~sourceFolder, "inc")) ||
		!DirectoryExists(AppendFileName(~sourceFolder, "src")) ||
		!DirectoryExists(AppendFileName(~sourceFolder, "drv")) ||
		!DirectoryExists(AppendFileName(~sourceFolder, "adm"))
	)
	{
		Exclamation("[= Error&Source folder is not an OCE root]");
		return;
	}
	
	oceRoot = ~sourceFolder;
	destRoot = ~destFolder;
	
	// we got source and dest path, we can proceed
	// build various path names
	admDir = AppendFileName(oceRoot, "adm");
	incDir = AppendFileName(oceRoot, "inc");
	srcDir = AppendFileName(oceRoot, "src");
	drvDir = AppendFileName(oceRoot, "drv");
	
	if(prefixOption)
		libPrefix = "UPP_";
	else
		libPrefix = "";
	
	progress.Show();
	
	// generated pakages
	builderName = "OCEBUILDER";
	oceName = "OCE";
	
	destRoot = AppendFileName(destRoot, builderName);

	// create database
	action = "Creating database";
	Ctrl::ProcessEvents();
	if(!BuildDb())
	{
		action = "Error building database";
		Ctrl::ProcessEvents();
		Exclamation(String(~action));
		return;
	}
	
	if(!MakeBuilderPackage())
	{
		action = "ERRROR BUILDING UPP PACKAGE";
		Ctrl::ProcessEvents();
		return;
	}
	
	if(!MakeOCEPackage())
	{
		action = "ERRROR BUILDING OCE PACKAGE";
		Ctrl::ProcessEvents();
		return;
	}
	
	action = "PACKAGE OCE GENERATED";
	Ctrl::ProcessEvents();
}

Oce2Upp::Oce2Upp()
{
	CtrlLayout(*this);
	goButton.Ok() <<= THISBACK(okCb);
	cancelButton.Cancel() <<= THISBACK(cancelCb);
	
	// hide progess and empty action
	progress.Hide();
	action = "";
	
	prefixOption = 1;
	
	// not canceling
	canceled = false;
}

Oce2Upp::~Oce2Upp()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
GUI_APP_MAIN
{
	Oce2Upp oce2Upp;
	
	String cfgfile = ConfigFile();
	if(FileExists(cfgfile))
		LoadFromFile(oce2Upp, cfgfile);
	oce2Upp.Run();
	StoreToFile(oce2Upp, cfgfile);
}
