#include <Core/Core.h>

#include "Functions4U.h"

#ifdef PLATFORM_WIN32 // || defined (PLATFORM_WIN64)
	#define Ptr Ptr_
	#define byte byte_
	#ifndef win32_CY_
		#define win32_CY_ long
	#endif
	#define CY win32_CY_
	
	#include <shellapi.h>
	#include <wincon.h>
	#include <shlobj.h>
	
	#undef Ptr
	#undef byte
	#undef CY
#endif

NAMESPACE_UPP

#define TFILE <Functions4U/Functions4U.t>
#include <Core/t.h>

	
/*
Hi Koldo,

I checked the functions in Functions4U. Here are some notes about trashing:

    * On older systems, the trash folder was $HOME/.Trash
    * Your implementation of disregards the folder $HOME/.local/share/trash/info. You should create 
    there a .trashinfo file when moving something in trash and delete it when deleting corresponding file permanently.
    * If you delete something on different partition than $HOME, you should also check if .Trash-XXXX 
    exists in root of that partition (XXXX is id of user who deleted the files in it). 

.local/share/Trash/files
.local/share/Trash/info

A file every time a file is removed with

KK.trashinfo
[Trash Info]
Path=/home/pubuntu/KK
DeletionDate=2010-05-19T18:00:52


You might also be interested in following:

    * Official trash specification from freedesktop.org
    * Project implementing command line access to trash (unfortunately in python) according to the specification mentioned above 


Hope this might help Smile It definitely learned me a lot of new things Very Happy

Best regards, 
Honza
*/

/////////////////////////////////////////////////////////////////////
// LaunchFile

#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
bool LaunchFileCreateProcess(const char *file) {
	STARTUPINFOW startInfo;
    PROCESS_INFORMATION procInfo;

    ZeroMemory(&startInfo, sizeof(startInfo));
    startInfo.cb = sizeof(startInfo);
    ZeroMemory(&procInfo, sizeof(procInfo));

	WString wexec;
	wexec = Format("\"%s\" \"%s\"", GetExtExecutable(GetFileExt(file)), file).ToWString();
	WStringBuffer wsbexec(wexec);
	
    if(!CreateProcessW(NULL, wsbexec, NULL, NULL, FALSE, 0, NULL, NULL, &startInfo, &procInfo))  
        return false;

   	WaitForSingleObject(procInfo.hProcess, 0);

    CloseHandle(procInfo.hProcess);
    CloseHandle(procInfo.hThread);	
	return true;
}

bool LaunchFileShellExecute(const char *file) {
	return 32 < uint64(ShellExecuteW(NULL, L"open", ToSystemCharsetW(file), NULL, L".", SW_SHOWNORMAL));		 
}

bool LaunchFile(const char *_file) {
	String file = WinPath(_file);
	if (!LaunchFileShellExecute(file))			// First try
	   	return LaunchFileCreateProcess(file);	// Second try
	return true;
}
#endif
#ifdef PLATFORM_POSIX

String GetDesktopManagerNew() {
	if(GetEnv("GNOME_DESKTOP_SESSION_ID").GetCount() || GetEnv("GNOME_KEYRING_SOCKET").GetCount()) 
		return "gnome";
	else if(GetEnv("KDE_FULL_SESSION").GetCount() || GetEnv("KDEDIR").GetCount() || GetEnv("KDE_MULTIHEAD").GetCount()) 
        return "kde"; 
	else {
		StringParse desktopStr;
		if (Sys("xprop -root _DT_SAVE_MODE").Find("xfce") >= 0)
			return "xfce";
		else if ((desktopStr = Sys("xprop -root")).Find("ENLIGHTENMENT") >= 0) 
			return "enlightenment";
		else
			return GetEnv("DESKTOP_SESSION");
	}
}

bool LaunchFile(const char *_file) {
	String file = UnixPath(_file);
	int ret;
	if (GetDesktopManagerNew() == "gnome") 
		ret = system("gnome-open \"" + String(file) + "\"");
	else if (GetDesktopManagerNew() == "kde") 
		ret = system("kfmclient exec \"" + String(file) + "\" &"); 
	else if (GetDesktopManagerNew() == "enlightenment") {
		String mime = GetExtExecutable(GetFileExt(file));
		String program = mime.Left(mime.Find("."));		// Left side of mime executable is the program to run
		ret = system(program + " \"" + String(file) + "\" &"); 
	} else 
		ret = system("xdg-open \"" + String(file) + "\"");
	return (ret >= 0);
}
#endif

/////////////////////////////////////////////////////////////////////
// General utilities

bool FileCat(const char *file, const char *appendFile) {
	if (!FileExists(file)) 
		return FileCopy(appendFile, file);
	FileAppend f(file);
	if(!f.IsOpen())
		return false;
	FileIn fi(appendFile);
	if(!fi.IsOpen())
		return false;
	CopyStream(f, fi, fi.GetLeft());
	fi.Close();
	f.Close();
	if(f.IsError()) 
		return false;
	return true;
}

bool FileStrAppend(const char *file, const char *str) {
	FileAppend f(file);
	if(!f.IsOpen())
		return false;
	f << str;
	f.Close();
	if(f.IsError()) 
		return false;
	return true;
}

bool AppendFile(const char *file, const char *str) {return FileStrAppend(file, str);};

String AppendFileName(const String& path1, const char *path2, const char *path3) {
	String result = path1;
	if(result.GetLength() && *result.Last() != DIR_SEP && *path2 != DIR_SEP)
		result += DIR_SEP;
	result += path2;
	if(result.GetLength() && *result.Last() != DIR_SEP && *path3 != DIR_SEP)
		result += DIR_SEP;
	result += path3; 
	return result;
}

String FormatLong(long a) { 
	return Sprintf("%ld", a);
}

String Replace(String str, String find, String replace) {
	String ret;
	
	int lenStr = str.GetCount();
	int lenFind = find.GetCount();
	int i = 0, j;
	while ((j = str.Find(find, i)) >= i) {
		ret += str.Mid(i, j-i) + replace;
		i = j + lenFind;
		if (i >= lenStr)
			break;
	}
	ret += str.Mid(i);
	return ret;
}

String Replace(String str, char find, char replace) {
	StringBuffer ret(str);
	for (int i = 0; i < ret.GetCount(); ++i) {
		if (ret[i] == find)
			ret[i] = replace;
	}
	return ret;
}

// Rename file or folder
bool FileMoveX(const char *oldpath, const char *newpath, EXT_FILE_FLAGS flags) {
	bool usr, grp, oth;
	if (flags & DELETE_READ_ONLY) {
		if (IsReadOnly(oldpath, usr, grp, oth))
			SetReadOnly(oldpath, false, false, false);
	}
	bool ret = FileMove(oldpath, newpath);
	if (flags & DELETE_READ_ONLY) 
		SetReadOnly(newpath, usr, grp, oth);
	return ret;
}

bool FileDeleteX(const char *path, EXT_FILE_FLAGS flags) {
	if (flags & USE_TRASH_BIN)
		return FileToTrashBin(path);
	else {
		if (flags & DELETE_READ_ONLY) 
			SetReadOnly(path, false, false, false);
		return FileDelete(path);
	}
}

bool FolderDeleteX(const char *path, EXT_FILE_FLAGS flags) {
	if (flags & USE_TRASH_BIN)
		return FileToTrashBin(path);
	else {
		if (flags & DELETE_READ_ONLY) 
			SetReadOnly(path, false, false, false);
		return DirectoryDelete(path);
	}
}

bool DirectoryExistsX(const char *path, EXT_FILE_FLAGS flags) {
	String spath = path;
	if (spath.EndsWith(DIR_SEPS))
		spath = spath.Left(spath.GetCount() - 1);
	if (!(flags & BROWSE_LINKS))
		return  DirectoryExists(spath);
	if (DirectoryExists(spath))
		return true;
	if (!IsSymLink(spath))
		return false;
	String filePath;
	filePath = GetSymLinkPath(spath);
	if (filePath.IsEmpty())
		return false;
	return DirectoryExists(filePath); 
}

bool IsFile(const char *fileName) {
	FindFile ff;
	if(ff.Search(fileName) && ff.IsFile()) 
		return true;
	return false;
}
	
bool IsFolder(const char *fileName) {
	FindFile ff;
	if(ff.Search(fileName) && ff.IsFolder()) 
		return true;
	return false;
}

String GetRelativePath(String &from, String &path) {
	String ret;
	int pos_from = 0, pos_path = 0;
	bool first = true;
	while (pos_from < from.GetCount()) {
		String f_from = Tokenize(from, DIR_SEPS, pos_from);
		String f_path = Tokenize(path, DIR_SEPS, pos_path);
		if (f_from != f_path) {
			if (first) 
				return Null;
			String fileName = path.Mid(pos_path);
			if (fileName.IsEmpty()) 
				ret << ".." << DIR_SEPS << f_path;	
			else
				ret << ".." << DIR_SEPS << f_path << DIR_SEPS << fileName;	
			while (pos_from < from.GetCount()) {
				ret.Insert(0, String("..") + DIR_SEPS);
				Tokenize(from, DIR_SEPS, pos_from);		
			}
			return ret;
		}
		first = false;
	}
	return path.Mid(pos_path);
}

bool SetReadOnly(const char *path, bool readOnly) {
	return SetReadOnly(path, readOnly, readOnly, readOnly);
}

bool SetReadOnly(const char *path, bool usr, bool grp, bool oth) {
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
	DWORD attr = GetFileAttributesW(ToSystemCharsetW(path)); 
	
	if (attr == INVALID_FILE_ATTRIBUTES) 
		return false; 

	DWORD newattr;
	if (usr)
		newattr = attr | FILE_ATTRIBUTE_READONLY;
	else
		newattr = attr & ~FILE_ATTRIBUTE_READONLY;
	
	if (attr != newattr)
		return SetFileAttributesW(ToSystemCharsetW(path), newattr); 
	else
		return true;
#else
	struct stat buffer;
	int status;

	if(0 != stat(ToSystemCharset(path), &buffer))
		return false;
	
	mode_t newmode = (usr & S_IRUSR) | (grp & S_IRGRP) | (oth & S_IROTH);
	
	if (newmode != buffer.st_mode)
		return 0 == chmod(ToSystemCharset(path), newmode);
	else
		return true;
#endif
}

bool IsReadOnly(const char *path, bool &usr, bool &grp, bool &oth) {
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
	DWORD attr = GetFileAttributesW(ToSystemCharsetW(path)); 
	
	if (attr == INVALID_FILE_ATTRIBUTES) 
		return false; 

	usr = grp = oth = attr & FILE_ATTRIBUTE_READONLY;
	return true;
#else
	struct stat buffer;

	if(0 != stat(ToSystemCharset(path), &buffer))
		return false;
	
	usr = buffer.st_mode & S_IRUSR;
	grp = buffer.st_mode & S_IRGRP;
	oth = buffer.st_mode & S_IROTH;
	
	return true;
#endif
}

#ifdef PLATFORM_POSIX

int GetUid() {
	String proc = LoadFile_Safe("/etc/passwd");
	int pos = proc.Find(GetUserName());
	if (pos < 0)
		return -1;
	pos = proc.Find(':', pos);
	if (pos < 0)
		return -1;
	pos = proc.Find(':', pos+1);
	if (pos < 0)
		return -1;
	int posend = proc.Find(':', pos+1);
	if (posend < 0)
		return -1;
	return ScanInt(proc.Mid(pos+1, posend-pos-1));
}

String GetMountDirectory(const String &path) {
	Array<String> drives = GetDriveList();	
	for (int i = 0; i < drives.GetCount(); ++i) {		
		if (path.Find(drives[i]) == 0)
			return drives[i];
	}
	String localPath = AppendFileName(GetCurrentDirectory(), path);
	if (!FileExists(localPath) && !DirectoryExists(localPath))
		return "";
	for (int i = 0; i < drives.GetCount(); ++i) {
		if (localPath.Find(drives[i]) == 0)
			return drives[i];
	}
	return "";
}
	
String GetTrashBinDirectory()
{	
	String ret = GetEnv("XDG_DATA_HOME");
	if (ret.IsEmpty())
		ret = AppendFileName(GetHomeDirectory(), ".local/share/Trash");
	else
		ret = AppendFileName(ret, "Trash");
	return ret;
}

bool FileToTrashBin(const char *path)
{	
	String newPath = AppendFileName(GetTrashBinDirectory(), GetFileName(path));
	return FileMove(path, newPath);
}

int64 TrashBinGetCount()
{
	int64 ret = 0;
	FindFile ff;
	if(ff.Search(AppendFileName(GetTrashBinDirectory(), "*"))) {
		do {
			String name = ff.GetName();
			if (name != "." && name != "..")
				ret++;
		} while(ff.Next());
	}
	return ret;
}

bool TrashBinClear()
{
	FindFile ff;
	String trashBinDirectory = GetTrashBinDirectory();
	if(ff.Search(AppendFileName(trashBinDirectory, "*"))) {
		do {
			String name = ff.GetName();
			if (name != "." && name != "..") {
				String path = AppendFileName(trashBinDirectory, name);
				if (ff.IsFile())
					FileDelete(path);
				else if (ff.IsDirectory())
					DeleteFolderDeep(path);
			}
		} while(ff.Next());
	}
	return true;
}

#endif
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)

bool DirectoryMove(const char *dir, const char *newPlace) {
	if (strcmp(dir, newPlace) == 0)
		return true;
	
	WString wDir(dir), wNewPlace(newPlace);
    wDir.Cat() << L'\0';	
    wNewPlace.Cat() << L'\0';	
	
    SHFILEOPSTRUCTW fileOp = {};
  	fileOp.hwnd = NULL;
    fileOp.wFunc = FO_MOVE;
    fileOp.pFrom = ~wDir;
    fileOp.pTo = ~wNewPlace;
    fileOp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT;
 
    int ret = SHFileOperationW(&fileOp);
 	return ret == 0;
}

bool FileToTrashBin(const char *path) {	
    if (!FileExists(path) && !DirectoryExists(path))
        return false;
	
    WString wpath(path);
    wpath.Cat() << L'\0';	
		
    SHFILEOPSTRUCTW fileOp = {}; 
    fileOp.hwnd = NULL;
    fileOp.wFunc = FO_DELETE;
    fileOp.pFrom = ~wpath;
    fileOp.pTo = NULL;
    fileOp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT;

    int ret = SHFileOperationW(&fileOp);
 	return ret == 0;
}

int64 TrashBinGetCount() {
	SHQUERYRBINFO shqbi; 
 
 	shqbi.cbSize = sizeof(SHQUERYRBINFO);
	if (S_OK != SHQueryRecycleBin(0, &shqbi))
		return -1;
	return shqbi.i64NumItems;
}

bool TrashBinClear() {
	if (S_OK != SHEmptyRecycleBin(0, 0, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND))
		return false;
	return true;
}

#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

String LoadFile_Safe(const String fileName)
{
#ifdef PLATFORM_POSIX
	int fid = open(fileName, O_RDONLY);
#else
	int fid = _wopen(fileName.ToWString(), O_RDONLY|O_BINARY);
#endif
	if (fid < 0) 
		return String();
	const int size = 1024;
	int nsize;
	StringBuffer s;
	char buf[size];
	while((nsize = read(fid, buf, size)) == size) 
		s.Cat(buf, size);
	if (nsize > 1)
		s.Cat(buf, nsize-1);
	close(fid);
	return s;
}

String LoadFile(const char *fileName, off_t from, size_t len)
{
#ifdef PLATFORM_POSIX
	int fid = open(fileName, O_RDONLY);
#else
	int fid = _wopen(String(fileName).ToWString(), O_RDONLY|O_BINARY);
#endif
	if (fid < 0) 
		return String();
	if (0 > lseek(fid, from, SEEK_SET))
		return String();
	size_t size = 1024;
	if (len != 0 && size > len)
		size = len;
	size_t nsize;
	StringBuffer s;
	Buffer<char> buf(size);
	size_t loaded;
	for (loaded = 0; (nsize = read(fid, buf, unsigned(size))) == size && (len == 0 || loaded < len); loaded += nsize) {
		if (len != 0 && loaded + size > len)
			size = len - loaded;
		s.Cat(buf, int(size));
	}
	if (nsize > 1 && (len == 0 || loaded < len))
		s.Cat(buf, int(nsize-1));
	close(fid);
	return s;
}

String GetExtExecutable(const String _ext)
{
	String ext = _ext;
	String exeFile = "";
	if (ext[0] != '.')
		ext = String(".") + ext;
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
	String file = AppendFileName(GetHomeDirectory(), String("dummy") + ext); // Required by FindExecutableW
	SaveFile(file, "   ");
	if (!FileExists(file)) 
		return "";
	HINSTANCE ret;
	WString fileW(file);
	WCHAR exe[1024];
	ret = FindExecutableW(fileW, NULL, exe);
	if ((uint64)ret > 32)
		exeFile = WString(exe).ToString();
	DeleteFile(file);
#endif
#ifdef PLATFORM_POSIX
	StringParse mime;
	//if (LaunchCommand(String("xdg-mime query filetype ") + file, mime) >= 0) 	// xdg-mime query filetype does not work properly in Enlightenment
	mime = LoadFile_Safe("/etc/mime.types");	// Search in /etc/mime.types the mime type for the extension
	if ((mime.GoAfter_Init(String(" ") + ext.Right(ext.GetCount()-1))) || (mime.GoAfter_Init(String("\t") + ext.Right(ext.GetCount()-1)))) {
		mime.GoBeginLine();
		mime = mime.GetText();
		exeFile = TrimRight(Sys(String("xdg-mime query default ") + mime));
	}
#endif
	return exeFile;
}

#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
Array<String> GetDriveList() {
	char drvStr[26*4+1];		// A, B, C, ...
	Array<String> ret;
	
	int lenDrvStrs = ::GetLogicalDriveStrings(sizeof(drvStr), drvStr);
	// To get the error call GetLastError()
	if (lenDrvStrs == 0)
		return ret;
	
	ret.Add(drvStr);
	for (int i = 0; i < lenDrvStrs-1; ++i) {
		if (drvStr[i] == '\0') 
			ret.Add(drvStr + i + 1);
	}
	return ret;
}
#endif
#if defined(PLATFORM_POSIX)
Array<String> GetDriveList() {
	Array<String> ret;
	// Search for mountable file systems
	String mountableFS = "cofs.";
	StringParse sfileSystems(LoadFile_Safe("/proc/filesystems"));
	String str;
	while (true) {
		str = sfileSystems.GetText();	
		if (str == "")
			break;
		else if (str != "nodev")
			mountableFS << str << ".";
		else 
			str = sfileSystems.GetText();
	}
	// Get mounted drives
	StringParse smounts(LoadFile_Safe("/proc/mounts"));
	StringParse smountLine(Trim(smounts.GetText("\r\n")));
	do {
		String devPath 	 = smountLine.GetText();
		String mountPath = smountLine.GetText();
		String fs        = smountLine.GetText();
		if ((mountableFS.Find(fs) >= 0) && (mountPath.Find("/dev") < 0) 
		 && (mountPath.Find("/rofs") < 0) && (mountPath != "/"))	// Is mountable 
			ret.Add(mountPath);
		smountLine = Trim(smounts.GetText("\r\n"));
	} while (smountLine != "");
	ret.Add("/");	// Last but not least
	return ret;
}
#endif


#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
String GetShellFolder2(int clsid) 
{
	wchar path[MAX_PATH];
	if(SHGetFolderPathW(NULL, clsid, NULL, //SHGFP_TYPE_CURRENT
											0, path) == S_OK)
		return FromUnicodeBuffer(path);
	return Null;
}

String GetShellFolder2(const char *local, const char *users) 
{
	String ret = FromSystemCharset(GetWinRegString(local, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 
									   HKEY_CURRENT_USER));
	if (ret == "" && *users != '\0')
		return FromSystemCharset(GetWinRegString(users, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 
									   HKEY_LOCAL_MACHINE));
	return ret;
}

String GetPersonalFolder()	{return GetShellFolder2("Personal", 0);}
String GetStartupFolder()	{return GetShellFolder2(CSIDL_STARTUP);}

String GetTempFolder()
{
	String ret;
	if ((ret = GetEnv("TEMP")) == "")	// One or the other one
		ret = GetEnv("TMP");
	return ret;
}

String GetOsFolder()
{
	char ret[MAX_PATH];
	::GetWindowsDirectory(ret, MAX_PATH);
	return String(ret);
}
String GetSystemFolder()
{
	char ret[MAX_PATH];
	::GetSystemDirectory(ret, MAX_PATH);
	return String(ret);
}

bool SetEnv(const char *id, const char *val) 
{
//	EnvMap().Put(WString(id), WString(val));
#ifdef PLATFORM_POSIX
	return setenv(id, val, 1) == 0;
#else
	return _wputenv(WString(id) + "=" + WString(val)) == 0;
#endif
}


#endif
#ifdef PLATFORM_POSIX

String GetPathXdg2(String xdgConfigHome, String xdgConfigDirs)
{
	String ret = "";
	if (FileExists(ret = AppendFileName(xdgConfigHome, "user-dirs.dirs"))) ;
  	else if (FileExists(ret = AppendFileName(xdgConfigDirs, "user-dirs.defaults"))) ;
  	else if (FileExists(ret = AppendFileName(xdgConfigDirs, "user-dirs.dirs"))) ;
  	return ret;
}
String GetPathDataXdg2(String fileConfig, const char *folder) 
{
	StringParse fileData = LoadFile(fileConfig);
	
	if (!fileData.GoAfter(folder)) return "";
	if (!fileData.GoAfter("=")) return "";
	
	String ret = "";
	StringParse path = fileData.GetText();
	if(path.GoAfter("$HOME/")) 
		ret = AppendFileName(GetHomeDirectory(), path.Right());
	else if (!FileExists(path))
		ret = AppendFileName(GetHomeDirectory(), path);
	
	return ret;		
}
String GetShellFolder2(const char *local, const char *users) 
{
 	String xdgConfigHome = GetEnv("XDG_CONFIG_HOME");
  	if (xdgConfigHome == "")		// By default
  		xdgConfigHome = AppendFileName(GetHomeDirectory(), ".config");
  	String xdgConfigDirs = GetEnv("XDG_CONFIG_DIRS");
  	if (xdgConfigDirs == "")			// By default
  		xdgConfigDirs = "/etc/xdg";
  	String xdgFileConfigData = GetPathXdg2(xdgConfigHome, xdgConfigDirs);
  	String ret = GetPathDataXdg2(xdgFileConfigData, local);
  	if (ret == "" && *users != '\0')
  		return GetPathDataXdg2(xdgFileConfigData, users);
  	else
  		return ret;
}

String GetTempFolder()
{
	return GetHomeDirectory();		
}
String GetOsFolder()
{
	return String("/bin");
}
String GetSystemFolder()
{
	return String("");
}

String GetPersonalFolder()	{return GetShellFolder2("XDG_DOCUMENTS_DIR", "DOCUMENTS");}

#endif

struct StringNormalCompare__ {
	int operator()(char a, char b) const { return a - b; }
};

int Compare(const String& a, int i0, const String& b, int len) {
	return IterCompare(a.Begin() + i0, a.Begin() + i0 + len, b.Begin(), b.Begin() + len, StringNormalCompare__());
}

int ReverseFind(const String& s, const String& toFind, int from) {
	ASSERT(from >= 0 && from <= s.GetLength());
	int lc = toFind.GetLength();
	for (int i = from; i >= 0; --i) {
		if (Compare(s, i, toFind, lc) == 0)
			return i;
	}
	return -1;
}

Time StrToTime(const char *s) {
	Time ret;
	if (StrToTime(ret, s))
		return ret;
	else
		return Null;
}

Date StrToDate(const char *s) {
	Time ret;
	if (StrToDate(ret, s))
		return ret;
	else
		return Null;
}

void StringToHMS(String durat, int &hour, int &min, double &seconds) {
	StringParse duration(durat);
	String s1, s2, s3;
	s1 = duration.GetText(":");
	s2 = duration.GetText(":");
	s3 = duration.GetText();
	
	if (s3 != "") {
		hour = atoi(s1);
		min = atoi(s2);
		seconds = atof(s3);
	} else if (s2 != "") {
		hour = 0;
		min = atoi(s1);
		seconds = atof(s2);
	} else {
		hour = 0;
		min = 0;
		seconds = atof(s1);		
	}
}
double StringToSeconds(String duration) {
	int hour, min;
	double secs;
	StringToHMS(duration, hour, min, secs);
	return 3600.*hour + 60.*min + secs;
}

String formatSeconds(double seconds, bool fill) {
	int iseconds = int(seconds);
	String ret;
	if (fill)
		ret = FormatIntDec(iseconds, 2, '0');
	else
		ret = FormatInt(iseconds);
	double decs = seconds - iseconds;
	if (decs > 0) 
		ret << "." << FormatIntDec((int)(decs*100), 2, '0');
	return ret;
}

String HMSToString(int hour, int min, double seconds, bool units, bool dec) {
	String sunits;
	if (units) {
		if (hour >= 1)
			sunits = t_("hours");
		else if (min >= 2)
			sunits = t_("mins");
		else if (min == 1)
			sunits = t_("min");
		else if (seconds > 1)
			sunits = t_("secs");
		else
			sunits = t_("sec");
	}
	String ret;
	if (hour > 0)
		ret << hour << ":";
	if (min > 0 || hour > 0) 
	    ret << (ret.IsEmpty() ? FormatInt(min) : FormatIntDec(min, 2, '0')) << ":";
	if (!dec)
		seconds = int(seconds);
	
	ret << formatSeconds(seconds, min > 0 || hour > 0);
	if (units)
		ret << " " << sunits;
	return ret;
}

String SecondsToString(double seconds, bool units, bool dec) {
	int hour, min;
	hour = (int)(seconds/3600.);
	seconds -= hour*3600;
	min = (int)(seconds/60.);
	seconds -= min*60;	
	return HMSToString(hour, min, seconds, units, dec);
}

String SeasonName(int iseason) {
	static const char *season[] = {t_("winter"), t_("spring"), t_("summer"), t_("autumn")};
	return iseason >= 0 && iseason < 4 ? season[iseason] : "";
}

String BytesToString(uint64 _bytes, bool units)
{
	String ret;
	uint64 bytes = _bytes;
	
	if (bytes >= 1024) {
		bytes /= 1024;
		if (bytes >= 1024) {
			bytes /= 1024;
			if (bytes >= 1024) {
				bytes /= 1024;
				if (bytes >= 1024) {
					bytes /= 1024;
					ret = Format("%.1f %s", _bytes/(1024*1024*1024*1024.), units ? "Tb" : "");
				} else
					ret = Format("%.1f %s", _bytes/(1024*1024*1024.), units ? "Gb" : "");
			} else
				ret = Format("%.1f %s", _bytes/(1024*1024.), units ? "Mb" : "");
		} else
			ret = Format("%.1f %s", _bytes/1024., units ? "Kb" : "");
	} else
		ret << _bytes << (units ? "b" : "");
	return ret;
}

String FormatDoubleAdjust(double d, double range) {
	if (fabs(d) <= 1e-15)
		d = 0;
	if 		(0.001 <= range && range < 0.01) 	return FormatDouble(d,5);
	else if (0.01  <= range && range < 0.1) 	return FormatDouble(d,4);
	else if (0.1   <= range && range < 1) 		return FormatDouble(d,3);
	else if (1     <= range && range < 10) 		return FormatDouble(d,2);
	else if (10	   <= range && range < 100) 	return FormatDouble(d,1);
	else if (100   <= range && range < 100000) 	return FormatDouble(d,0);
	else return FormatDoubleExp(d,2);	
}

String RemoveAccent(wchar c) {
	WString wsret;

	if (IsAlNum(c) || IsSpace(c)) {
		wsret.Cat(c);
		return wsret.ToString();
	}
	//const WString accented = "ÂÃÀÁÇÈÉÊËẼÌÍÎÏÑÒÓÔÕÙÚÛÝàáâãçèéêëẽìíîïñòóôõøùúûýÿ";
	const WString accented = "\303\202\303\203\303\200\303\201\303\207\303\210\303\211\303\212\303\213\341\272\274\303\214\303\215\303\216\303\217\303\221\303\222\303\223\303\224\303\225\303\231\303\232\303\233\303\235\303\240\303\241\303\242\303\243\303\247\303\250\303\251\303\252\303\253\341\272\275\303\254\303\255\303\256\303\257\303\261\303\262\303\263\303\264\303\265\303\270\303\271\303\272\303\273\303\275\303\277";
	const char *unaccented = "AAAACEEEEEIIIINOOOOUUUYaaaaceeeeeiiiinooooouuuyy";
	
	for (int i = 0; accented[i]; ++i) {
		if (accented[i] == c) {
			wsret.Cat(unaccented[i]);
			return wsret.ToString();	
		}
	}
	//const WString maccented = "ÅåÆæØøþÞßÐðÄäÖöÜü";
	const WString maccented = "\303\205\303\245\303\206\303\246\303\230\303\270\303\276\303\236\303\237\303\220\303\260\303\204\303\244\303\226\303\266\303\234\303\274";
	const char *unmaccented[] = {"AA", "aa", "AE", "ae", "OE", "oe", "TH", "th", "SS", "ETH", 
								 "eth", "AE", "ae", "OE", "oe", "UE", "ue"};
	for (int i = 0; maccented[i]; ++i) {
		if (maccented[i] == c) 
			return unmaccented[i];
	}
	wsret.Cat(c);
	return wsret.ToString();
}

bool IsPunctuation(wchar c) {
	//const WString punct = "\"’'()[]{}<>:;,‒–—―….,¡!¿?«»-‐‘’“”/\\&@*\\•^©¤฿¢$€ƒ£₦¥₩₪†‡〃#№ºª\%‰‱ ¶′®§℠℗~™|¦=";
	const WString punct = "\"\342\200\231'()[]{}<>:;,\342\200\222\342\200\223\342\200\224\342\200\225\342\200\246.,\302\241!\302\277?\302\253\302\273-\342\200\220\342\200\230\342\200\231\342\200\234\342\200\235/\\&@*\\\342\200\242^\302\251\302\244\340\270\277\302\242$\342\202\254\306\222\302\243\342\202\246\302\245\342\202\251\342\202\252\342\200\240\342\200\241\343\200\203#\342\204\226\302\272\302\252%\342\200\260\342\200\261 "
     					  "\302\266\342\200\262\302\256\302\247\342\204\240\342\204\227~\342\204\242|\302\246=";
	for (int i = 0; punct[i]; ++i) {
		if (punct[i] == c) 
			return true;
	}
	return false;	 
}

String RemoveAccents(String str) {
	String ret;
	WString wstr = str.ToWString();
	for (int i = 0; i < wstr.GetCount(); ++i) {
		String schar = RemoveAccent(wstr[i]);
		if (i == 0 || ((i > 0) && ((IsSpace(wstr[i-1]) || IsPunctuation(wstr[i-1]))))) {
			if (schar.GetCount() > 1) {
				if (IsUpper(schar[0]) && IsLower(wstr[1]))
				 	schar = String(schar[0], 1) + ToLower(schar.Mid(1));
			}
		} 
		ret += schar;
	}
	return ret;
}

String RemovePunctuation(String str) {
	String ret;
	WString wstr = str.ToWString();
	for (int i = 0; i < wstr.GetCount(); ++i) {
		if (!IsPunctuation(wstr[i]))
		    ret += wstr[i];
	}
	return ret;
}

String FitFileName(const String fileName, int len) {
	if (fileName.GetCount() <= len)
		return fileName;
	
	Array<String> path;
	
	const char *s = fileName;
	char c;
	int pos0 = 0;
	for (int pos1 = 0; (c = s[pos1]) != '\0'; ++pos1) {
	#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
		if(c == '\\' || c == '/') {
	#else
		if(c == '/') {
	#endif
			path.Add(fileName.Mid(pos0, pos1-pos0));
			pos0 = ++pos1;
		}
	}
	path.Add(fileName.Mid(pos0));

	String begin, end;
	int iBegin = 0;
	int iEnd = path.GetCount() - 1;
	
	if (path[iEnd].GetCount() >= len)
		return path[iEnd].Left(len);
	if (path[iEnd].GetCount() >= len-4)
		return path[iEnd];
	
	len -= 3;	// ...
	
	for (; iBegin <= iEnd; iBegin++, iEnd--) {
		if (path[iEnd].GetCount() + 1 > len)
			break;
		end = DIR_SEPS + path[iEnd] + end;
		len -= path[iEnd].GetCount() + 1;
		if (path[iBegin].GetCount() + 1 > len)
			break;
		begin += path[iBegin] + DIR_SEPS;
		len -= path[iBegin].GetCount() + 1;
	}
	return begin + "..." + end;
}

String Tokenize(const String &str, const String &token, int &pos) {
	int npos;
	for (int i = 0; i < token.GetCount(); ++i) {
		if ((npos = str.Find(token[i], pos)) >= 0) 
			break;
	}
	int oldpos = pos;
	if (npos < 0) {
		pos = str.GetCount();
		return str.Mid(oldpos);
	} else {
		pos = npos + 1;
		return str.Mid(oldpos, npos-oldpos);
	}
}

String Tokenize(const String &str, const String &token) {
	int dummy = 0;
	return Tokenize(str, token, dummy);
}

String GetLine(const String &str, int &pos) {
	String ret;
	int npos = str.Find("\n", pos);
	if (npos == -1) {
		ret = str.Mid(pos);
		pos = -1;
	} else {
		ret = str.Mid(pos, npos - pos);
		pos = npos + 1;
	}
	return TrimBoth(ret);
}

Value GetField(const String &str, int &pos, char separator, char decimalSign, bool onlyStrings) {
	ASSERT(separator != '\"');	
	String sret;
	int npos = str.Find(separator, pos);
	int spos = str.Find('\"', pos);
	if (spos < 0 || spos > npos) {
		if (npos <= -1) {
			sret = str.Mid(pos);
			pos = -1;
		} else {
			sret = str.Mid(pos, npos - pos);
			pos = npos + 1;
		}
	} else {
		int lspos = str.Find('\"', spos + 1);
		if (lspos < 0) {
			sret = str.Mid(spos);
			pos = -1;
		} else {
			sret = str.Mid(spos + 1, lspos - spos - 1);
			npos = str.Find(separator, lspos);
			pos = npos + 1;
		}
	}	
	if (onlyStrings)
		return sret;
	
	if (sret.IsEmpty())
		return Null;
		
	double dbl = ScanDouble(sret, NULL, decimalSign == ',');
		
	if (IsNull(dbl)) {
		Time t = ScanTime(sret);
		if (IsNull(t))
			return sret;
		else if (t.hour == t.minute == t.second == 0)
			return Date(t);
		else
			return t;
	} else {
		int it = int(dbl);
		if (dbl - double(it) != 0)
			return dbl;				
		else 
			return it;
	}
}

Vector<Vector <Value> > ReadCSV(const String strFile, char separator, bool bycols, bool removeRepeated, char decimalSign, bool onlyStrings) {
	Vector<Vector<Value> > result;

	String line;
	int posLine = 0;
	int pos = 0;
	if (bycols) {
		line = GetLine(strFile, posLine);
		while (pos >= 0) {
			Value name = GetField(line, pos, separator, decimalSign, onlyStrings);
			if (pos >= 0 && !IsNull(name)) {
				Vector<Value> &data = result.Add();
				data.Add(name);
			}
		}
		while (posLine >= 0) {
			pos = 0;
			line = GetLine(strFile, posLine);
			if (!line.IsEmpty()) {
				bool repeated = removeRepeated;
				int row = result[0].GetCount() - 1;
				for (int col = 0; col < result.GetCount(); col++) {
					if (pos >= 0) {
						Value data = GetField(line, pos, separator, decimalSign, onlyStrings);
						result[col].Add(data);
						if (row > 0 && result[col][row] != data)
							repeated = false;
					} else
						result[col].Add();
				}
				if (row > 0 && repeated) {
					for (int col = 0; col < result.GetCount(); col++) 
						result[col].Remove(row+1);
				}
			} else
				break;
		}
	} else {
		int row = 0;
		while (posLine >= 0) {
			pos = 0;
			line = GetLine(strFile, posLine);
			bool repeated = removeRepeated;
			if (!line.IsEmpty()) {
				Vector <Value> &linedata = result.Add();
				int col = 0;
				while (pos >= 0) {
					Value val = GetField(line, pos, separator, decimalSign, onlyStrings);
					if (val.IsNull())
						linedata << "";
					else
						linedata << val;
					if (row > 0 && (result[row - 1].GetCount() <= col || result[row - 1][col] != val))
						repeated = false;
					col++;
				}
			} else
				break;
			if (row > 0 && repeated) 
				result.Remove(row);
			else
				row++;
		}
	}
	return result;
}

Vector<Vector <Value> > ReadCSVFile(const String fileName, char separator, bool bycols, bool removeRepeated, char decimalSign, bool onlyStrings) {
	return ReadCSV(LoadFile(fileName), separator, bycols, removeRepeated,  decimalSign, onlyStrings);
}

bool ReadCSVFileByLine(const String fileName, Gate2<int, Vector<Value>&> WhenRow, char separator, char decimalSign, bool onlyStrings) {
	Vector<Value> result;

	FindFile ff(fileName);
	if(!ff || !ff.IsFile()) 
		return false;
	
	FileIn in(fileName);
	in.ClearError();
	
	for (int row = 0; true; row++) {
		String line = in.GetLine();
		if (line.IsVoid()) {
			WhenRow(Null, result);
			return true;
		}
		int pos = 0;
		while (pos >= 0) {
			Value val = GetField(line, pos, separator, decimalSign, onlyStrings);
			if (val.IsNull())
				result << "";
			else
				result << val;
		}
		if (!WhenRow(row, result))
			return true;
		result.Clear();
	}
	return false;
}
	
String ToStringDecimalSign(Value &val, const String &decimalSign) {
	String ret = val.ToString();
	if (val.Is<double>() && decimalSign != ".") 
		ret.Replace(".", decimalSign);
	return ret;
}

String WriteCSV(Vector<Vector <Value> > &data, char separator, bool bycols, char decimalSign) {
	String ret;
	
	String _decimalSign(decimalSign, 1);
	
	if (bycols) {
		for (int r = 0; r < data[0].GetCount(); ++r) {
			if (r > 0)
				ret << "\n";
			for (int c = 0; c < data.GetCount(); ++c) {
				if (c > 0)
					ret << separator;
				String str = ToStringDecimalSign(data[c][r], _decimalSign);
				if (str.Find(separator) >= 0)
					ret << '\"' << str << '\"';
				else
					ret << str;
			}
		}		
	} else {
		for (int r = 0; r < data.GetCount(); ++r) {
			if (r > 0)
				ret << "\n";
			for (int c = 0; c < data[r].GetCount(); ++c) {
				if (c > 0)
					ret << separator;
				String str = ToStringDecimalSign(data[r][c], _decimalSign);
				if (str.Find(separator) >= 0)
					ret << '\"' << str << '\"';
				else
					ret << str;
			}
		}
	}
	return ret;
}

bool WriteCSVFile(const String fileName, Vector<Vector <Value> > &data, char separator, bool bycols, char decimalSign) {
	String str = WriteCSV(data, separator, bycols, decimalSign);
	return SaveFile(fileName, str);
}


#ifdef PLATFORM_POSIX
String FileRealName(const char *_fileName) {
	String fileName = GetFullPath(_fileName);
	FindFile ff(fileName);
	if (!ff)
		return String(""); 
	else
		return fileName;
}
#endif
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
bool GetRealName_Next(String &real, String file) {
	bool ret;
	String old;
	int from = real.GetCount()+1;
	int to = file.Find(DIR_SEP, from);
	if (to >= 0) {
		old = file.Mid(from, to-from);
		ret = true;
	} else {
		old = file.Mid(from);
		ret = false;
	}
	real += DIR_SEP;
	FindFile ff(real + old);
	real += ff.GetName();
	return ret;
}

String FileRealName(const char *_fileName) {
	String fileName = GetFullPath(_fileName);
	int len = fileName.GetCount();
	
	if (len == 3) {
		FindFile ff(fileName + "*");
		if (!ff)
			return String(""); 	
		else
			return fileName;
	}
	FindFile ff(fileName);
	if (!ff)
		return String(""); 
	String ret;
	
	ret.Reserve(len);
	
	if (fileName.Left(2) == "\\\\")
		return String("");	// Not valid for UNC paths

	ret = ToUpper(fileName.Left(1)) + ":";
	
	while (GetRealName_Next(ret, fileName)) ;
	
	return ret;
}
#endif

#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)

#define Ptr Ptr_
#define byte byte_
#define CY win32_CY_

#include <winnls.h>
#include <winnetwk.h>

#include <wincon.h>
#include <shlobj.h>

#undef Ptr
#undef byte
#undef CY

#endif
/*
bool GetSymLinkPath(const char *linkPath, String &filePath)
{
#ifdef PLATFORM_WIN32	
	HRESULT hres;
	IShellLink* psl;
	IPersistFile* ppf;
	CoInitialize(NULL);
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink,
	                        (PVOID *) &psl);
	if(SUCCEEDED(hres)) {
		hres = psl->QueryInterface(IID_IPersistFile, (PVOID *) &ppf);
		if(SUCCEEDED(hres)) {
			hres = ppf->Load(ToSystemCharsetW(linkPath), STGM_READ);
			if(SUCCEEDED(hres)) {
				char fileW[_MAX_PATH] = {0};
				psl->GetPath(fileW, _MAX_PATH, NULL, 0); 
				filePath = FromSystemCharset(fileW);
			} else
				return false;
			ppf->Release();
		} else
			return false;
		psl->Release();
	} else
		return false;
	CoUninitialize();
	return true;
#else
	char buff[_MAX_PATH + 1];
	bool ret;
	int len = readlink(linkPath, buff, _MAX_PATH);
	if (ret = (len > 0 && len < _MAX_PATH))
		buff[len] = '\0';
	else 
		*buff = '\0';
	filePath = buff;
	return ret;
#endif
}				
*/	
bool IsSymLink(const char *path) {
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
	return GetFileExt(path) == ".lnk";
#else
	struct stat stf;
	lstat(path, &stf);
	return S_ISLNK(stf.st_mode);
#endif
}	

String GetNextFolder(const String &folder, const String &lastFolder) {
	int pos = lastFolder.Find(DIR_SEP, folder.GetCount()+1);
	if (pos >= 0)
		return lastFolder.Left(pos);
	else
		return lastFolder;
}

bool IsRootFolder(const char *folderName) {
	if (!folderName)
		return false;
	if (folderName[0] == '\0')
		return false;
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
	if (strlen(folderName) == 3 && folderName[1] == ':' && folderName[2] == DIR_SEP)
#else
	if (strlen(folderName) == 1 && folderName[0] == DIR_SEP)
#endif
		return true;
	return false;
}

String GetUpperFolder(const String &folderName) {
	if (IsRootFolder(folderName))
		return folderName;
	int len = folderName.GetCount();
	if (folderName[len-1] == DIR_SEP)
		len--;
	int pos = folderName.ReverseFind(DIR_SEP, len-1);
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
	if (pos == 2)
#else
	if (pos == 0)
#endif
		pos++;
	return folderName.Left(pos);
}
/*
bool CreateFolderDeep(const char *dir)
{
	if (RealizePath(dir))
		return DirectoryCreate(dir);
	else
		return false;
}*/

bool DeleteDeepWildcardsX(const char *pathwc, bool filefolder, EXT_FILE_FLAGS flags)
{
	return DeleteDeepWildcardsX(GetFileFolder(pathwc), GetFileName(pathwc), filefolder, flags);	
}

bool DeleteDeepWildcardsX(const char *path, const char *namewc, bool filefolder, EXT_FILE_FLAGS flags)
{
	FindFile ff(AppendFileName(path, "*.*"));
	while(ff) {
		String name = ff.GetName();
		String full = AppendFileName(path, name);
		if (PatternMatch(namewc, name)) {
			if (ff.IsFolder() && !filefolder) {
				if (!DeleteFolderDeepX(full, flags)) {
					dword error = GetLastError();
					return false;
				}
			} else if (ff.IsFile() && filefolder) {
				if (!FileDeleteX(full, flags)) {
					dword error = GetLastError();
					return false;
				}
			}
		} else if(ff.IsFolder()) {
			if (!DeleteDeepWildcardsX(full, namewc, filefolder, flags))
				return false;
		}	
		ff.Next();
	}
	return true;
}

bool DeleteFolderDeepWildcardsX(const char *path, EXT_FILE_FLAGS flags) 	
{
	return DeleteDeepWildcardsX(path, false, flags);
}

bool DeleteFileDeepWildcardsX(const char *path, EXT_FILE_FLAGS flags) 	
{
	return DeleteDeepWildcardsX(path, true, flags);
}

bool DeleteFolderDeepX_Folder(const char *dir, EXT_FILE_FLAGS flags)
{
	FindFile ff(AppendFileName(dir, "*.*"));
	while(ff) {
		String name = ff.GetName();
		String p = AppendFileName(dir, name);
		if(ff.IsFile())
			FileDeleteX(p, flags);
		else
		if(ff.IsFolder())
			DeleteFolderDeepX_Folder(p, flags);
		ff.Next();
	}
	return FolderDeleteX(dir, flags);
}

bool DeleteFolderDeepX(const char *path, EXT_FILE_FLAGS flags)
{
	if (flags & USE_TRASH_BIN)
		return FileToTrashBin(path);
	return DeleteFolderDeepX_Folder(path, flags);
}

bool RenameDeepWildcardsX(const char *path, const char *namewc, const char *newname, bool forfile, bool forfolder, EXT_FILE_FLAGS flags)
{
	FindFile ff(AppendFileName(path, "*.*"));
	while(ff) {
		String name = ff.GetName();
		String full = AppendFileName(path, name);
		if(ff.IsFolder()) {
			if (!RenameDeepWildcardsX(full, namewc, newname, forfile, forfolder, flags))
				return false;
		}
		if (PatternMatch(namewc, name)) {
			if ((ff.IsFolder() && forfolder) || (ff.IsFile() && forfile)) {
				if (!FileMoveX(full, AppendFileName(path, newname)), flags) {
					dword error = GetLastError();
					return false;
				}
			}
		}
		ff.Next();
	}
	return true;
}

bool DirectoryCopy_Each(const char *dir, const char *newPlace, String relPath)
{
	String dirPath = AppendFileName(dir, relPath);
	String newPath = AppendFileName(newPlace, relPath);
	FindFile ff(AppendFileName(dirPath, "*.*"));
	while(ff) {
		if(ff.IsFile()) {
			if (!FileCopy(AppendFileName(dirPath, ff.GetName()), AppendFileName(newPath, ff.GetName())))
				return false;
		} else if(ff.IsFolder()) {
			DirectoryCreate(AppendFileName(newPath, ff.GetName()));
			if(!DirectoryCopy_Each(dir, newPlace, AppendFileName(relPath, ff.GetName())))
				return false;
		}
		ff.Next();
	}
	return true;
}

bool DirectoryCopyX(const char *dir, const char *newPlace) {
	return DirectoryCopy_Each(dir, newPlace, "");
}

#if defined(__MINGW32__)
	#define _SH_DENYNO 0x40 
#endif

int FileCompare(const char *path1, const char *path2) {
	int fp1;
#ifdef PLATFORM_POSIX
	fp1 = open(ToSystemCharset(path1), O_RDONLY, S_IWRITE|S_IREAD);
#else
	fp1 = _wsopen(ToSystemCharsetW(path1), O_RDONLY|O_BINARY, _SH_DENYNO, _S_IREAD|_S_IWRITE);
#endif
	if (fp1 == -1)
		return -2;
	int fp2;
#ifdef PLATFORM_POSIX
	fp2 = open(ToSystemCharset(path2), O_RDONLY, S_IWRITE|S_IREAD);
#else
	fp2 = _wsopen(ToSystemCharsetW(path2), O_RDONLY|O_BINARY, _SH_DENYNO, _S_IREAD|_S_IWRITE);
#endif
	if (fp2 == -1) {
		close(fp1);
		return -2;	
	}
	Buffer <byte> c1(8192), c2(8192);
	int ret = -1;
	while (true) {
		int n1 = read(fp1, c1, 8192);
		int n2 = read(fp2, c2, 8192);
		if (n1 == -1 || n2 == -1) {
			ret = -2;
			break;
		}
		if (n1 != n2)
			break;
		if (memcmp(c1, c2, n1) != 0)
			break;
		if (n1 == 0) {
			ret = 1;
			break;
		}
	}
#ifdef PLATFORM_POSIX	
	if (-1 == close(fp1))
		ret = -2;
	if (-1 == close(fp2))
		ret = -2;
#else
	if (-1 == _close(fp1))
		ret = -2;
	if (-1 == _close(fp2))
		ret = -2;
#endif
	return ret;
}

int64 FindStringInFile(const char *file, const String text, int64 pos0) {
#ifdef PLATFORM_POSIX
	FILE *fp = fopen(file, "rb");
#else
	FILE *fp = _wfopen(String(file).ToWString(), L"rb");
#endif
	if (fp != NULL) {
		int64 pos = 0;
		if (pos0 > 0) {
			pos = pos0;
			if (0 == fseek(fp, long(pos0), SEEK_SET))
				return -2;
		}
		int i = 0, c;
		for (; (c = fgetc(fp)) != EOF; pos++) {
			if (c == text[i]) {
				++i;
				if (i == text.GetCount()) 
					return pos - i;
			} else {
				if (i != 0) 
					if (0 == fseek(fp, -(i-1), SEEK_CUR))
						return -2;
				i = 0;
			}
		}
		fclose(fp);
	} else
		return -2;
	return -1;	
}

bool MatchPathName(const char *name, const Array<String> &cond, const Array<String> &ext) {
	for (int i = 0; i < cond.GetCount(); ++i) {
		if(!PatternMatch(cond[i], name))
			return false;
	}
	for (int i = 0; i < ext.GetCount(); ++i) {
		if(PatternMatch(ext[i], name))
			return false;
	}
	return true;
}

void SearchFile_Each(String dir, const Array<String> &condFiles, const Array<String> &condFolders, 
								 const Array<String> &extFiles,  const Array<String> &extFolders, 
								 const String text, Array<String> &files, Array<String> &errorList) {
	FindFile ff;
	if (ff.Search(AppendFileName(dir, "*"))) {
		do {
			if(ff.IsFile()) {
				String name = AppendFileName(dir, ff.GetName());
				if (MatchPathName(ff.GetName(), condFiles, extFiles)) {
					if (text.IsEmpty())
						files.Add(name);
					else {
						switch(FindStringInFile(name, text)) {
						case 1:	files.Add(name);
								break;
						case -1:errorList.Add(AppendFileName(dir, ff.GetName()) + String(": ") + 
																	t_("Impossible to open file"));
								break;
						}
					}
				}
			} else if(ff.IsDirectory()) {
				String folder = ff.GetName();
				if (folder != "." && folder != "..") {
					String name = AppendFileName(dir, folder);
					if (MatchPathName(name, condFolders, extFolders)) 
						SearchFile_Each(name, condFiles, condFolders, extFiles, extFolders, text, files, errorList);
				}
			} 
		} while (ff.Next());
	}
}

Array<String> SearchFile(String dir, const Array<String> &condFiles, const Array<String> &condFolders, 
								 const Array<String> &extFiles,  const Array<String> &extFolders, 
								 const String text, Array<String> &errorList) {
	Array<String> files;								     
	//errorList.Clear();

	SearchFile_Each(dir, condFiles, condFolders, extFiles, extFolders, text, files, errorList);	
	
	
	return files;
}

Array<String> SearchFile(String dir, String condFile, String text, Array<String> &errorList)
{
	Array<String> condFiles, condFolders, extFiles, extFolders, files;
	//errorList.Clear();

	condFiles.Add(condFile);
	SearchFile_Each(dir, condFiles, condFolders, extFiles, extFolders, text, files, errorList);	

	return files;
}

Array<String> SearchFile(String dir, String condFile, String text)
{
	Array<String> errorList;
	Array<String> condFiles, condFolders, extFiles, extFolders, files;
	
	condFiles.Add(condFile);
	SearchFile_Each(dir, condFiles, condFolders, extFiles, extFolders, text, files, errorList);	
	
	return files;
}

bool fileDataSortAscending;
char fileDataSortBy;

FileDataArray::FileDataArray(bool use, int _fileFlags)
{
	Clear();
	fileDataSortAscending = true;
	fileDataSortBy = 'n';
	useId = use;
	fileFlags = _fileFlags;
}

bool FileDataArray::Init(String folder, FileDataArray &orig, FileDiffArray &diff)
{
	basePath = orig.basePath;
	fileCount = orig.fileCount;
	folderCount = orig.folderCount;
	fileSize = orig.fileSize;
	useId = orig.useId;
	fileList.SetCount(orig.GetCount());
	for (int i = 0; i < orig.GetCount(); ++i)
		fileList[i] = orig[i];
	
	for (int i = 0; i < diff.GetCount(); ++i) {
		long id;
		if (diff[i].action != 'n') {
		 	id = Find(diff[i].relPath, diff[i].fileName, diff[i].isFolder);
			if (id < 0)
				return false;
		}
		switch(diff[i].action) {
		case 'u':
			fileList[id].id = diff[i].idMaster;
			fileList[id].length = diff[i].lengthMaster;
			fileList[id].t = diff[i].tMaster;
			break;
		case 'n':
			fileList.Add(FileData(diff[i].isFolder, diff[i].fileName, diff[i].relPath, diff[i].lengthMaster, diff[i].tMaster, diff[i].idMaster));
			if (diff[i].isFolder) 
				folderCount++;
			else
				fileCount++;
			break;
		case 'd':
			fileList.Remove(id);
			if (diff[i].isFolder) 
				folderCount--;
			else
				fileCount--;
			break;
			break;
		case 'p':
			SetLastError(t_("Problem found"));		// To Fix				
			//return false;
		}
	}
	return true;
}

void FileDataArray::Clear()
{
	fileList.Clear();
	errorList.Clear();
	fileCount = folderCount = 0;
	fileSize = 0;
	basePath = "";
}

bool FileDataArray::Search(String dir, String condFile, bool recurse, String text)
{
	Clear();
	if (fileFlags & BROWSE_LINKS) {
		if (IsSymLink(dir)) 
			dir = basePath = GetSymLinkPath(dir);
		else
			basePath = dir;
	} else
		basePath = dir;
	Search_Each(dir, condFile, recurse, text);
	return errorList.IsEmpty();
}

void FileDataArray::Search_Each(String dir, String condFile, bool recurse, String text)
{
	FindFile ff;
	if (dir == "C:\\Desarrollo\\Packages\\ffmpeg_source\\tests\\ref\\vsynth2\\rgb")
		int kk = 1;
	if (ff.Search(AppendFileName(dir, condFile))) {
		do {
			if(ff.IsFile()) {
				String p = AppendFileName(dir, ff.GetName());
				//if (ff.IsSymLink()) {
				//	p = p;
				//}	
				/*
					fileList.Add(FileData(true, ff.GetName(), GetRelativePath(dir), 0, ff.GetLastWriteTime(), 0));
					folderCount++;
					if (recurse)
						Search_Each(p, condFile, recurse, text);
				} else */ if (text.IsEmpty()) {
					uint64 len = ff.GetLength();
					fileList.Add(FileData(false, ff.GetName(), GetRelativePath(dir), len, ff.GetLastWriteTime(), 
											(useId && len > 0) ? GetFileId(p) : 0));
					fileCount++;
					fileSize += len;
				} else {
					FILE *fp = fopen(p, "r");
					if (fp != NULL) {
						int i = 0, c;
						while ((c = fgetc(fp)) != EOF) {
							if (c == text[i]) {
								++i;
								if (i == text.GetCount()) {
									uint64 len = ff.GetLength();
									fileList.Add(FileData(false, ff.GetName(), GetRelativePath(dir), len, ff.GetLastWriteTime(), useId ? GetFileId(p) : 0));
									fileCount++;
									fileSize += len;
									break;
								}
							} else {
								if (i != 0) 
									fseek(fp, -(i-1), SEEK_CUR);
								i = 0;
							}
						}
						fclose(fp);
					} else
						errorList.Add(AppendFileName(dir, ff.GetName()) + String(": ") + t_("Impossible to open file"));
				}
			} 
		} while (ff.Next());
	}
	ff.Search(AppendFileName(dir, "*"));
	do {
		String name = ff.GetName();
		if(ff.IsDirectory() && name != "." && name != "..") {
			String p = AppendFileName(dir, name);
			fileList.Add(FileData(true, name, GetRelativePath(dir), 0, ff.GetLastWriteTime(), 0));
			folderCount++;
			if (recurse)
				Search_Each(p, condFile, recurse, text);
		}
	} while (ff.Next()); 
}

int64 FileDataArray::GetFileId(String fileName)
{
	int64 id = -1;
#ifdef PLATFORM_POSIX
	FILE *fp = fopen(fileName, "rb");
#else
	FILE *fp = _wfopen(fileName.ToWString(), L"rb");
#endif
	if (fp != NULL) {
		int c;
		long i = 0;
		while ((c = fgetc(fp)) != EOF) {
			id += c*i;
			i++;
		}
		fclose(fp);
	}
	return id;
}

void FileDataArray::SortByName(bool ascending)
{
	fileDataSortBy = 'n';
	fileDataSortAscending = ascending;
	Sort(fileList);
}
void FileDataArray::SortByDate(bool ascending)
{
	fileDataSortBy = 'd';
	fileDataSortAscending = ascending;
	Sort(fileList);
}
void FileDataArray::SortBySize(bool ascending)
{
	fileDataSortBy = 's';
	fileDataSortAscending = ascending;
	Sort(fileList);
}

bool operator<(const FileData& a, const FileData& b)
{
	if ((a.isFolder && b.isFolder) || (!a.isFolder && !b.isFolder)) {
		switch (fileDataSortBy) {
		case 'n':	{
						bool ais = IsDigit(a.fileName[0]);
						bool bis = IsDigit(b.fileName[0]);
						if (ais && bis)
							return atoi(a.fileName) < atoi(b.fileName);
						if (ais)
							return true;
						if (bis)
							return false;
					}	
#ifdef PLATFORM_POSIX				
					return (a.fileName < b.fileName)&fileDataSortAscending; 
#else
					return (ToLower(a.fileName) < ToLower(b.fileName))&fileDataSortAscending; 
#endif
		case 'd':	return (a.t < b.t)&fileDataSortAscending; 
		default:	return (a.length < b.length)&fileDataSortAscending; 
		}
	} else 
		return a.isFolder;
}

bool CheckFileData(FileData &data, String &relFileName, String &fileName, String &lowrelFileName, String &lowfileName, bool isFolder) {
	if (data.isFolder == isFolder) {
		if (ToLower(data.fileName) == lowfileName) {
			if (ToLower(data.relFilename) == lowrelFileName) 
	    		return true;
		}
	}	
	return false;
}

int FileDataArray::Find(String &relFileName, String &fileName, bool isFolder) {
	String lowrelFileName = ToLower(relFileName);
	String lowfileName = ToLower(fileName);
	for (int i = 0; i < fileList.GetCount(); ++i) {
		if (CheckFileData(fileList[i], relFileName, fileName, lowrelFileName, lowfileName, isFolder))
		    return i;
	}
	return -1;
}

/*
int FileDataArray::Find(FileDataArray &data, int id) {
	return Find(data[id].relFilename, data[id].fileName, data[id].isFolder);
}
*/

int FileDataArray::Find(FileDataArray &data, int id) {
	String lowrelFileName = ToLower(data[id].relFilename);
	String lowfileName = ToLower(data[id].fileName);
	bool isFolder = data[id].isFolder;
	
	int num = fileList.GetCount();
	if (num == 0)
		return -1;
	if (num == 1) {
		if (CheckFileData(fileList[0], data[id].relFilename, data[id].fileName, lowrelFileName, lowfileName, isFolder))
			return 0;
		else
			return -1;
	}
	int down, up;
	down = id < num-1 ? id : num-2;
	up = down + 1; 
	while (down >= 0 || up < num) {
		if (down >= 0) {
			if (CheckFileData(fileList[down], data[id].relFilename, data[id].fileName, lowrelFileName, lowfileName, isFolder))
		    	return down;
			down--;
		}
		if (up < num) {
			if (CheckFileData(fileList[up], data[id].relFilename, data[id].fileName, lowrelFileName, lowfileName, isFolder))
		    	return up;
			up++;
		}
	}
	return -1;
}

String FileDataArray::GetFileText() {
	String ret;
	
	for (int i = 0; i < fileList.GetCount(); ++i) {
		ret << fileList[i].relFilename << "; ";
		ret << fileList[i].fileName << "; ";
		ret << fileList[i].isFolder << "; ";
		ret << fileList[i].length << "; ";
		ret << fileList[i].t << "; ";
		ret << fileList[i].id << "; ";
		ret << "\n";
	}
	return ret;	
}

bool FileDataArray::SaveFile(const char *fileName) {
	return Upp::SaveFile(fileName, GetFileText());
}

bool FileDataArray::AppendFile(const char *fileName) {
	return Upp::AppendFile(fileName, GetFileText());
}

bool FileDataArray::LoadFile(const char *fileName)
{
	Clear();
	StringParse in = Upp::LoadFile(fileName);
	
	if (in == "")
		return false;

	int numData = in.Count("\n");
	fileList.SetCount(numData);	
	for (int row = 0; row < numData; ++row) {		
		fileList[row].relFilename = in.GetText(";");	
		fileList[row].fileName = in.GetText(";");	
		fileList[row].isFolder = in.GetText(";") == "true" ? true : false;	
		if (fileList[row].isFolder)
			folderCount++;
		else
			fileCount++; 
		fileList[row].length = in.GetUInt64(";");	
		struct Upp::Time t;
		StrToTime(t, in.GetText(";"));	
		fileList[row].t = t;
		fileList[row].id = in.GetUInt64(";");	
	}
	return true;
}

String FileDataArray::GetRelativePath(const String &fullPath) {
	if (basePath != fullPath.Left(basePath.GetCount()))
		return "";
	return fullPath.Mid(basePath.GetCount());
}

int64 GetDirectoryLength(const char *directoryName) {
	FileDataArray files;
	files.Search(directoryName, "*.*", true);
	return files.GetSize();
}

int64 GetLength(const char *fileName) {
	if (FileExists(fileName))
		return GetFileLength(fileName);
	else	
		return GetDirectoryLength(fileName);
}

FileDiffArray::FileDiffArray() {
	Clear();
}

void FileDiffArray::Clear()
{
	diffList.Clear();
}

// True if equal
bool FileDiffArray::Compare(FileDataArray &master, FileDataArray &secondary, const String folderFrom,
						 Array<String> &excepFolders, Array<String> &excepFiles, int sensSecs) {
	if (master.GetCount() == 0) {
		if (secondary.GetCount() == 0)
			return true;
		else
			return false;
	} else if (secondary.GetCount() == 0)
		return false;
	
	bool equal = true;
	diffList.Clear();
	Array<bool> secReviewed;
	secReviewed.SetCount(secondary.GetCount(), false);
	
	for (int i = 0; i < master.GetCount(); ++i) {
		bool cont = true;
		if (master[i].isFolder) {
			String fullfolder = AppendFileName(AppendFileName(folderFrom, master[i].relFilename), master[i].fileName);
			for (int iex = 0; iex < excepFolders.GetCount(); ++iex)
				if (PatternMatch(excepFolders[iex] + "*", fullfolder)) {// Subfolders included
					cont = false;
					break;
				}
		} else {
			String fullfolder = AppendFileName(folderFrom, master[i].relFilename);
			for (int iex = 0; iex < excepFolders.GetCount(); ++iex)
				if (PatternMatch(excepFolders[iex] + "*", fullfolder)) {
					cont = false;
					break;
				}
			for (int iex = 0; iex < excepFiles.GetCount(); ++iex)
				if (PatternMatch(excepFiles[iex], master[i].fileName)) {
					cont = false;
					break;
				}
		}		
		if (cont) {		
			int idSec = secondary.Find(master, i);
			if (idSec >= 0) {
				bool useId = master.UseId() && secondary.UseId();
				secReviewed[idSec] = true;
				if (master[i].isFolder) 
					;
				else if ((useId && (master[i].id == secondary[idSec].id)) ||
						 (!useId && (master[i].length == secondary[idSec].length) && 
						 			 (abs(master[i].t - secondary[idSec].t) <= sensSecs)))
					;
				else {
					equal = false;
					FileDiffData &f = diffList.Add();
					bool isf = f.isFolder = master[i].isFolder;
					f.relPath = master[i].relFilename;
					String name = f.fileName = master[i].fileName;
					f.idMaster = master[i].id;
					f.idSecondary = secondary[idSec].id;
					f.tMaster = master[i].t;
					f.tSecondary = secondary[idSec].t;
					f.lengthMaster = master[i].length;
					f.lengthSecondary = secondary[idSec].length;
					if (master[i].t > secondary[idSec].t)
						f.action = 'u';
					else
						f.action = 'p';
				}
			} else {
				equal = false;
				FileDiffData &f = diffList.Add();
				f.isFolder = master[i].isFolder;
				f.relPath = master[i].relFilename;
				f.fileName = master[i].fileName;
				f.idMaster = master[i].id;
				f.idSecondary = 0;
				f.tMaster = master[i].t;
				f.tSecondary = Null;
				f.lengthMaster = master[i].length;
				f.lengthSecondary = 0;
				f.action = 'n';
			}	
		}
	}
	for (int i = 0; i < secReviewed.GetCount(); ++i) {
		if (!secReviewed[i]) {
			bool cont = true;
			if (secondary[i].isFolder) {
				String fullfolder = AppendFileName(AppendFileName(folderFrom, secondary[i].relFilename), secondary[i].fileName);
				for (int iex = 0; iex < excepFolders.GetCount(); ++iex)
					if (PatternMatch(excepFolders[iex] + "*", fullfolder)) {
						cont = false;
						break;
					}
			} else {
				String fullfolder = AppendFileName(folderFrom, secondary[i].relFilename);
				for (int iex = 0; iex < excepFolders.GetCount(); ++iex)
					if (PatternMatch(excepFolders[iex] + "*", fullfolder)) {
						cont = false;
						break;
					}
				for (int iex = 0; iex < excepFiles.GetCount(); ++iex)
					if (PatternMatch(excepFiles[iex], secondary[i].fileName)) {
						cont = false;
						break;
					}
			}
			if (cont) {
				equal = false;
				FileDiffData &f = diffList.Add();
				f.isFolder = secondary[i].isFolder;
				f.relPath = secondary[i].relFilename;
				f.fileName = secondary[i].fileName;
				f.idMaster = 0;
				f.idSecondary = secondary[i].id;
				f.tMaster = Null;
				f.tSecondary = secondary[i].t;
				f.lengthMaster = 0;
				f.lengthSecondary = secondary[i].length;
				f.action = 'd';
			}
		}
	}
	return equal;
}

#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
String WinLastError() {
	LPVOID lpMsgBuf;
	String ret;
	
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
        		  FORMAT_MESSAGE_IGNORE_INSERTS,
        		  NULL, ::GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        		  (LPTSTR) &lpMsgBuf, 0, NULL);
  	ret = (char *)lpMsgBuf;
  	LocalFree(lpMsgBuf);	
	return ret;
}
#endif

bool FileDiffArray::Apply(String toFolder, String fromFolder, EXT_FILE_FLAGS flags)
{
	for (int i = 0; i < diffList.GetCount(); ++i) {
		bool ok = true;
		String dest = AppendFileName(toFolder, 
									 AppendFileName(diffList[i].relPath, diffList[i].fileName));		
		if (diffList[i].action == 'u' || diffList[i].action == 'd') {
			if (diffList[i].isFolder) {
				if (DirectoryExists(dest)) {
					if (!SetReadOnly(dest, false))
						ok = false;
				}
			} else {
				if (FileExists(dest)) {
					if (!SetReadOnly(dest, false))
						ok = false;
				}
			}
		}
		if (!ok) {
			String strError = t_("Not possible to modify ") + dest;	
			SetLastError(strError);
			//return false;
		}

		switch (diffList[i].action) {
		case 'n': case 'u': 	
			if (diffList[i].isFolder) {
				if (!DirectoryExists(dest)) {
					ok = DirectoryCreate(dest);	////////////////////////////////////////////////////////////////////////////////////////
				}
			} else {
				if (FileExists(dest)) {
					if (!SetReadOnly(dest, false))
						ok = false;
				}
				if (ok) {
					ok = FileCopy(AppendFileName(fromFolder, FormatInt(i)), dest);
					diffList[i].tSecondary = diffList[i].tMaster;
				}
			}
			
			if (!ok) {
				String strError = t_("Not possible to create ") + dest;
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
			  	strError += ". " + WinLastError();
#endif
				SetLastError(strError);
				//return false;
			}
			break;
		case 'd': 
			if (diffList[i].isFolder) {
				if (DirectoryExists(dest))
					ok = DeleteFolderDeep(dest);	// Necessary to add the "X"
			} else {
				if (FileExists(dest))
					ok = FileDeleteX(dest, flags);
			}
			if (!ok) {
				String strError = t_("Not possible to delete") + String(" ") + dest;
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
			  	strError += ". " + WinLastError();
#endif
				SetLastError(strError);				
				//return false;
			}
			break;		
		case 'p': 
			SetLastError(t_("There was a problem in the copy"));
			//return false;
			break;
		}
	}
	return true;
}

bool FileDiffArray::SaveFile(const char *fileName)
{
	return Upp::SaveFile(fileName, ToString());
}

String FileDiffArray::ToString()
{
	String ret;
	
	for (int i = 0; i < diffList.GetCount(); ++i) {
		ret << diffList[i].action << "; ";
		ret << diffList[i].isFolder << "; ";
		ret << diffList[i].relPath << "; ";
		ret << diffList[i].fileName << "; ";
		ret << diffList[i].idMaster << "; ";
		ret << diffList[i].idSecondary << "; ";
		ret << diffList[i].tMaster << "; ";
		ret << diffList[i].tSecondary << "; ";
		ret << diffList[i].lengthMaster << "; ";
		ret << diffList[i].lengthSecondary << "; ";
		ret << "\n";
	}
	return  ret;
}

bool FileDiffArray::LoadFile(const char *fileName)
{
	Clear();
	StringParse in = Upp::LoadFile(fileName);
	
	if (in == "")
		return false;

	int numData = in.Count("\n");
	diffList.SetCount(numData);	
	for (int row = 0; row < numData; ++row) {		
		diffList[row].action = TrimLeft(in.GetText(";"))[0];	
		diffList[row].isFolder = in.GetText(";") == "true" ? true : false;	
		diffList[row].relPath = in.GetText(";");	
		diffList[row].fileName = in.GetText(";");	
		diffList[row].idMaster = in.GetUInt64(";");
		diffList[row].idSecondary = in.GetUInt64(";");
		struct Upp::Time t;
		StrToTime(t, in.GetText(";"));	
		diffList[row].tMaster = t;
		StrToTime(t, in.GetText(";"));	
		diffList[row].tSecondary = t;
		diffList[row].lengthMaster = in.GetUInt64(";");
		diffList[row].lengthSecondary = in.GetUInt64(";");
	}
	return true;
}
	
	
#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)
Value GetVARIANT(VARIANT &result)
{
	Value ret;
	switch(result.vt) {
	case VT_EMPTY:
	case VT_NULL:
	case VT_BLOB:
		break;
	case VT_BOOL:
         ret = result.boolVal;// ? "true" : "false";
         break;
   	case VT_I2:
         ret = result.iVal;
         break;
	case VT_I4:
		ret = (int64)result.lVal; 
		break;
	case VT_R4:
		ret = AsString(result.fltVal);
		break;
	case VT_R8:
		ret = AsString(result.dblVal);
		break;
	case VT_BSTR:  
		ret = WideToString(result.bstrVal);
		break;
	case VT_LPSTR:
         //ret = result.pszVal;
         ret = "Unknown VT_LPSTR";
    case VT_DATE:
  		SYSTEMTIME stime;
     	VariantTimeToSystemTime(result.date, &stime);
     	{
	     	Time t;
	     	t.day    = (Upp::byte)stime.wDay;
	     	t.month  = (Upp::byte)stime.wMonth;
	     	t.year   = stime.wYear;
	     	t.hour   = (Upp::byte)stime.wHour; 
	     	t.minute = (Upp::byte)stime.wMinute;
	     	t.second = (Upp::byte)stime.wSecond;		
			ret = t;
     	}
    	break;
 	case VT_CF:
     	ret = "(Clipboard format)";
     	break;
	}
	return ret;
}

String WideToString(LPCWSTR wcs, int len) {
	if (len == -1) {
		len = WideCharToMultiByte(CP_UTF8, 0, wcs, len, NULL, 0, NULL, NULL);	
		if (len == 0)
			return Null;
	}
	Buffer<char> w(len);
	WideCharToMultiByte(CP_UTF8, 0, wcs, len, w, len, NULL, NULL);
	return ~w;	
}

#endif

#if defined(PLATFORM_WIN32) || defined (PLATFORM_WIN64)

Dl::Dl() {
	hinstLib = 0;
}

Dl::~Dl() {
	if (hinstLib) 
		if (FreeLibrary(hinstLib) == 0)
			throw Exc(t_("Dl cannot be released"));
}

#ifndef LOAD_IGNORE_CODE_AUTHZ_LEVEL
	#define LOAD_IGNORE_CODE_AUTHZ_LEVEL	0x00000010
#endif

bool Dl::Load(const String &fileDll) {
	if (hinstLib) 
		if (FreeLibrary(hinstLib) == 0)
			return false;
	
	hinstLib = LoadLibraryEx(TEXT(fileDll), NULL, LOAD_IGNORE_CODE_AUTHZ_LEVEL);
	if (!hinstLib) 
		return false;
	return true;
}

void *Dl::GetFunction(const String &functionName) {
	if (!hinstLib) 
		return NULL;
	return (void *)GetProcAddress(hinstLib, functionName);
}

#else

#include <dlfcn.h>

Dl::Dl() {
	hinstLib = 0;
}

Dl::~Dl() {
	if (hinstLib) 
		if (dlclose(hinstLib) == 0)
			throw Exc(t_("Dl cannot be released"));
}

bool Dl::Load(const String &fileDll) {
	if (hinstLib) 
		if (dlclose(hinstLib) == 0)
			return false;
	
	hinstLib = dlopen(fileDll, RTLD_LAZY);
	if (!hinstLib) 
		return false;
	return true;
}

void *Dl::GetFunction(const String &functionName) {
	if (!hinstLib) 
		return NULL;
	return dlsym(hinstLib, functionName);
}	

#endif

Color RandomColor() {
	return Color(Random(), 0);
}

Image GetRect(const Image& orig, const Rect &r) {
	if(r.IsEmpty())
		return Image(); 
	ImageBuffer ib(r.GetSize());
	for(int y = r.top; y < r.bottom; y++) {
		const RGBA *s = orig[y] + r.left;
		const RGBA *e = orig[y] + r.right;
		RGBA *t = ib[y - r.top];
		while(s < e) {
			*t = *s;
			t++;
			s++;
		}
	}
	return ib;
}


double tmGetTimeX() {
#ifdef __linux__
	struct timeval t;
	if(gettimeofday(&t, 0) != 0)
	    return Null;
	return t.tv_sec + t.tv_usec/1E6;
#elif defined(_WIN32) || defined(WIN32)
	LARGE_INTEGER clock;
	LARGE_INTEGER freq;
	if(!QueryPerformanceCounter(&clock) || !QueryPerformanceFrequency(&freq))
	    return Null;
	return double(clock.QuadPart)/freq.QuadPart;
#else
	return double(time(0));		// Low resolution
#endif
}



END_UPP_NAMESPACE

// Dummy functions added after TheIDE change
Upp::String GetCurrentMainPackage() {return "dummy";}
Upp::String GetCurrentBuildMethod()	{return "dummy";}
void IdePutErrorLine(const Upp::String& line) {}
