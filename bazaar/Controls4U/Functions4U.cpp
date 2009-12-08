#include <Core/Core.h>

#if defined(PLATFORM_WIN32) 
	#include <shellapi.h>
#endif

#include "Functions4U.h"

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

bool FileSetReadOnly(String fileName, bool readOnly)
{
#if defined(PLATFORM_WIN32) 
	WString wfile = fileName.ToWString();
	
	DWORD attr = GetFileAttributesW(wfile); 
	
	if (attr == INVALID_FILE_ATTRIBUTES) 
		return false; 

	DWORD newattr;
	if (readOnly)
		newattr = attr | FILE_ATTRIBUTE_READONLY;
	else
		newattr = attr & ~FILE_ATTRIBUTE_READONLY;
	
	if (attr != newattr)
		return SetFileAttributesW(wfile, newattr); 
	else
		return true;
#else
	struct stat buffer;
	int status;

	if(0 != stat(fileName, &buffer))
		return false;
	
	mode_t newmode;
	if (readOnly) {
		newmode = buffer.st_mode | S_IRUSR | S_IRGRP | S_IROTH;  
		newmode = newmode		 & ~S_IWUSR & ~S_IWGRP & ~S_IWOTH; 
	} else {
		newmode = buffer.st_mode | S_IRUSR | S_IRGRP | S_IROTH;  
		newmode = newmode		 | S_IWUSR | S_IWGRP | S_IWOTH; 
	}
	if (newmode != buffer.st_mode)
		return 0 == chmod(fileName, newmode);
	else
		return true;
#endif
}

#ifdef PLATFORM_POSIX

String GetTrashBinDirectory()
{
	return AppendFileName(GetHomeDirectory(), ".local/share/Trash/files");
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
#if defined(PLATFORM_WIN32)

bool FileToTrashBin(const char *path)
{	
    if (!FileExists(path) && !DirectoryExists(path))
        return false;
	
    WString ws(path);
    ws.Cat() << L'\0';	// This string must be double-null terminated.
		
    SHFILEOPSTRUCTW fileOp; 
    
    fileOp.hwnd = NULL;
    fileOp.wFunc = FO_DELETE;
    fileOp.pFrom = ~ws;
    fileOp.pTo = NULL;
    fileOp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT;

    int ret = SHFileOperationW(&fileOp);
    if (0 != ret)
        return false;
    return true;
}
int64 TrashBinGetCount()
{
	SHQUERYRBINFO shqbi; 
 
 	shqbi.cbSize = sizeof(SHQUERYRBINFO);
	if (S_OK != SHQueryRecycleBin(0, &shqbi))
		return -1;
	return shqbi.i64NumItems;
}
bool TrashBinClear()
{
	if (S_OK != SHEmptyRecycleBin(0, 0, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND))
		return false;
	return true;
}

#endif

String GetExtExecutable(String ext)
{
	String exeFile = "";
	if (ext[0] != '.')
		ext = String(".") + ext;
#if defined(PLATFORM_WIN32)
	String file = AppendFileName(GetHomeDirectory(), String("dummy") + ext); // Required by FindExecutableW
	SaveFile(file, "   ");
	if (!FileExists(file)) 
		return "";
	HINSTANCE ret;
	WString fileW(file);
	WCHAR exe[1024];
	ret = FindExecutableW(fileW, NULL, exe);
	if ((long)ret > 32)
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

String GetFirefoxDownloadFolder()
{
	String profilesPath = "mozilla/firefox";
#ifdef PLATFORM_POSIX	// Is hidden
	profilesPath = String(".") + profilesPath;
#endif
	StringParse profiles = LoadFile(AppendFileName(GetAppDataFolder(), AppendFileName(profilesPath, "profiles.ini")));
	if (!profiles.GoAfter("Path")) return "";
	if (!profiles.GoAfter("=")) return "";
	String path = profiles.GetText();
	String pathPrefs = AppendFileName(AppendFileName(AppendFileName(GetAppDataFolder(), profilesPath), path), "prefs.js");
	StringParse prefs = LoadFile(pathPrefs);
	if (!prefs.GoAfter("\"browser.download.dir\"")) {
		if (!prefs.GoAfter("\"browser.download.lastDir\""))
			return "";		
	}
	if (!prefs.GoAfter(",")) return "";
	return prefs.GetText();
}

#if defined(PLATFORM_WIN32)
String GetShellFolder(const char *local, const char *users) 
{
	String ret = FromSystemCharset(GetWinRegString(local, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 
									   HKEY_CURRENT_USER));
	if (ret == "" && *users != '\0')
		return FromSystemCharset(GetWinRegString(users, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 
									   HKEY_LOCAL_MACHINE));
	return ret;
}
String GetIEDownloadFolder() 
{
	String ret = FromSystemCharset(GetWinRegString("Download Directory", "Software\\Microsoft\\Internet Explorer", HKEY_CURRENT_USER));
	if (ret == "")
		ret =  FromSystemCharset(GetWinRegString("Save Directory", "Software\\Microsoft\\Internet Explorer\\Main", HKEY_CURRENT_USER)); 
	return ret;
}
String GetDesktopFolder()	{return GetShellFolder("Desktop", "Common Desktop");}
String GetProgramsFolder()	{return FromSystemCharset(GetWinRegString("ProgramFilesDir", "Software\\Microsoft\\Windows\\CurrentVersion", HKEY_LOCAL_MACHINE));}
String GetAppDataFolder()	{return GetShellFolder("AppData", "Common AppData");}
String GetMusicFolder()		{return GetShellFolder("My Music", "CommonMusic");}
String GetPicturesFolder()	{return GetShellFolder("My Pictures", "CommonPictures");}
String GetVideoFolder()		{return GetShellFolder("My Video", "CommonVideo");}
String GetPersonalFolder()	{return GetShellFolder("Personal", 0);}
String GetTemplatesFolder()	{return GetShellFolder("Templates", "Common Templates");}
String GetDownloadFolder()	
{
	String browser = GetExtExecutable("html");
	browser = ToLower(browser);
	if (browser.Find("iexplore") >= 0)
		return GetIEDownloadFolder();
	else if (browser.Find("firefox") >= 0)
		return GetFirefoxDownloadFolder();
	return GetDesktopFolder();		// I do not know to do it in other browsers !!
};
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
#endif
#ifdef PLATFORM_POSIX
String GetPathXdg(String xdgConfigHome, String xdgConfigDirs)
{
	String ret = "";
	if (FileExists(ret = AppendFileName(xdgConfigHome, "user-dirs.dirs"))) ;
  	else if (FileExists(ret = AppendFileName(xdgConfigDirs, "user-dirs.defaults"))) ;
  	else if (FileExists(ret = AppendFileName(xdgConfigDirs, "user-dirs.dirs"))) ;
  	return ret;
}
String GetPathDataXdg(String fileConfig, const char *folder) 
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
String GetShellFolder(const char *local, const char *users) 
{
 	String xdgConfigHome = GetEnv("XDG_CONFIG_HOME");
  	if (xdgConfigHome == "")		// By default
  		xdgConfigHome = AppendFileName(GetHomeDirectory(), ".config");
  	String xdgConfigDirs = GetEnv("XDG_CONFIG_DIRS");
  	if (xdgConfigDirs == "")			// By default
  		xdgConfigDirs = "/etc/xdg";
  	String xdgFileConfigData = GetPathXdg(xdgConfigHome, xdgConfigDirs);
  	String ret = GetPathDataXdg(xdgFileConfigData, local);
  	if (ret == "" && *users != '\0')
  		return GetPathDataXdg(xdgFileConfigData, users);
  	else
  		return ret;
}
String GetDesktopFolder()	
{
	String ret = GetShellFolder("XDG_DESKTOP_DIR", "DESKTOP");
	if (ret.IsEmpty())
		return AppendFileName(GetHomeDirectory(), "Desktop");
	else
		return ret;
}
String GetProgramsFolder()	{return String("/usr/bin");}
String GetAppDataFolder()	{return GetHomeDirectory();};
String GetMusicFolder()		{return GetShellFolder("XDG_MUSIC_DIR", "MUSIC");}
String GetPicturesFolder()	{return GetShellFolder("XDG_PICTURES_DIR", "PICTURES");}
String GetVideoFolder()		{return GetShellFolder("XDG_VIDEOS_DIR", "VIDEOS");}
String GetPersonalFolder()	{return GetShellFolder("XDG_DOCUMENTS_DIR", "DOCUMENTS");}
String GetTemplatesFolder()	{return GetShellFolder("XDG_TEMPLATES_DIR", "XDG_TEMPLATES_DIR");}
String GetDownloadFolder()	
{
	String browser = GetExtExecutable("html");
	browser = ToLower(browser);
	if (browser.Find("firefox") >= 0)
		return GetFirefoxDownloadFolder();
	return GetShellFolder("XDG_DOWNLOAD_DIR", "DOWNLOAD");
};
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

// Like StrToDate but using Time
const char *StrToTime(struct Upp::Time& d, const char *s) {
	s = StrToDate(d, s);

	d.hour = 0;
	d.minute = 0;
	d.second = 0;

	const char *fmt = "hms";

	while(*fmt) {
		while(*s && !IsDigit(*s))
			s++;
		int n;
		if(IsDigit(*s)) {
			char *q;
			n = strtoul(s, &q, 10);
			s = q;
		} else
			break;

		switch(*fmt) {
		case 'h':
			if(n < 0 || n > 23)
				return NULL;
			d.hour = n;
			break;
		case 'm':
			if(n < 0 || n > 59)
				return NULL;
			d.minute = n;
			break;
		case 's':
			if(n < 0 || n > 59)
				return NULL;
			d.second = n;
			break;
		default:
			NEVER();
		}
		fmt++;
	}
	return d.IsValid() ? s : NULL;
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
	return 60.*hour + 60.*min + secs;
}

String formatSeconds(double seconds) {
	String ret = FormatIntDec((int)seconds, 2, '0');
	double decs = seconds - (int)seconds;
	if (decs > 0) 
		ret << "." << FormatIntDec((int)(decs*100), 2, '0');
	return ret;
}

String HMSToString(int hour, int min, double seconds, bool units) {
	String sunits;
	if (units) {
		if (hour >= 2)
			sunits = t_("hours");
		else if (hour >= 1)
			sunits = t_("hour");
		else if (min >= 2)
			sunits = t_("mins");
		else if (min >= 60)
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
	ret <<  (ret.IsEmpty() ? FormatDouble(seconds, 2, FD_REL) : formatSeconds(seconds));
	if (units)
		ret << " " << sunits;
	return ret;
}

String SecondsToString(double seconds, bool units) {
	int hour, min;
	hour = (int)(seconds/3600.);
	seconds -= hour*3600;
	min = (int)(seconds/60.);
	seconds -= min*60;	
	return HMSToString(hour, min, seconds, units);
}

int DayOfYear(Date d)
{
	return 1 + d - FirstDayOfYear(d);
}

Color RandomColor() {
	int num = Random();
	return Color(num&0xFF, (num&0xFF00)>>8, (num&0xFF0000)>>16);
}

String GetUpperFolder(String folderName)
{
	if (folderName.IsEmpty())
		return "";
#ifdef PLATFORM_WIN32
	if (folderName.GetCount() == 3)
#else
	if (folderName.GetCount() == 1)
#endif
		return folderName;
	int len = folderName.GetCount();
	if (folderName[len-1] == DIR_SEP)
		len--;
	int pos = folderName.ReverseFind(DIR_SEP, len-1);
#ifdef PLATFORM_WIN32
	if (pos == 2)
#else
	if (pos == 0)
#endif
		pos++;
	return folderName.Left(pos);
}

bool CreateFolderDeep(const char *dir)
{
	if (DirectoryExists(dir))
		return true;
	String upper = GetUpperFolder(dir);
	if (CreateFolderDeep(upper))
		return DirectoryCreate(dir);
	else 
		return false;
}

bool DeleteFolderDeepWildcards(const char *path)
{
	FindFile ff(path);
	String dir = GetFileDirectory(path);
	while(ff) {
		String name = ff.GetName();
		String p = AppendFileName(dir, name);
		if(ff.IsFile())
			FileDelete(p);
		else
		if(ff.IsFolder())
			DeleteFolderDeep(p);
		ff.Next();
	}
	return DirectoryDelete(dir);
}

bool DirectoryCopy_Each(const char *dir, const char *newPlace, String relPath)
{
	String dirPath = AppendFileName(dir, relPath);
	String newPath = AppendFileName(newPlace, relPath);
	FindFile ff(AppendFileName(dirPath, "*.*"));
	while(ff) {
		if(ff.IsFile())
			FileCopy(AppendFileName(dirPath, ff.GetName()), AppendFileName(newPath, ff.GetName()));
		else if(ff.IsFolder()) {
			DirectoryCreate(AppendFileName(newPath, ff.GetName()));
			if(!DirectoryCopy_Each(dir, newPlace, AppendFileName(relPath, ff.GetName())))
				return false;
		}
		ff.Next();
	}
	return true;
}

bool DirectoryCopy(const char *dir, const char *newPlace)
{
	return DirectoryCopy_Each(dir, newPlace, "");
}

void SearchFile_Each(String dir, String condFile, String text, Array<String> &files, Array<String> &errorList)
{
	FindFile ff;
	if (ff.Search(AppendFileName(dir, condFile))) {
		do {
			if(ff.IsFile()) {
				String p = AppendFileName(dir, ff.GetName());
				if (text.IsEmpty())
					files.Add(p);
				else {
#ifdef PLATFORM_POSIX
					FILE *fp = fopen(p, "rb");
#else
					FILE *fp = _wfopen(p.ToWString(), L"rb");
#endif
					if (fp != NULL) {
						int i = 0, c;
						while ((c = fgetc(fp)) != EOF) {
							if (c == text[i]) {
								++i;
								if (i == text.GetCount()) {
									files.Add(p);
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
			SearchFile_Each(p, condFile, text, files, errorList);
		}
	} while (ff.Next()); 
}

Array<String> SearchFile(String dir, String condFile, String text, Array<String> &errorList)
{
	Array<String> ret;
	errorList.Clear();

	SearchFile_Each(dir, condFile, text, ret, errorList);	

	return ret;
}

Array<String> SearchFile(String dir, String condFile, String text)
{
	Array<String> errorList;
	Array<String> ret;
	
	SearchFile_Each(dir, condFile, text, ret, errorList);	
	
	return ret;
}

bool fileDataSortAscending;
char fileDataSortBy;

FileDataArray::FileDataArray(bool use)
{
	Clear();
	fileDataSortAscending = true;
	fileDataSortBy = 'n';
	useId = use;
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
			SetLastError(t_("Problem found"));
			return false;
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
	basePath = dir;
	Search_Each(dir, condFile, recurse, text);
	return errorList.IsEmpty();
}

void FileDataArray::Search_Each(String dir, String condFile, bool recurse, String text)
{
	FindFile ff;
	if (ff.Search(AppendFileName(dir, condFile))) {
		do {
			if(ff.IsFile()) {
				String p = AppendFileName(dir, ff.GetName());
				if (text.IsEmpty()) {
					uint64 len = ff.GetLength();
					fileList.Add(FileData(false, ff.GetName(), GetRelativePath(dir), len, ff.GetLastWriteTime(), 
											(useId && ff.GetLength() > 0) ? GetFileId(p) : 0));
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
		case 'n':	return (ToLower(a.fileName) < ToLower(b.fileName))&fileDataSortAscending; 
		case 'd':	return (a.t < b.t)&fileDataSortAscending; 
		default:	return (a.length < b.length)&fileDataSortAscending; 
		}
	} else 
		return a.isFolder;
}
int FileDataArray::Find(String &relFileName, String &fileName, bool isFolder)
{
	for (int i = 0; i < fileList.GetCount(); ++i) {
		if ((ToLower(fileList[i].relFilename) == ToLower(relFileName)) && (ToLower(fileList[i].fileName) == ToLower(fileName)) && (fileList[i].isFolder == isFolder))
		    return i;
	}
	return -1;
}

bool FileDataArray::SaveFile(const char *fileName)
{
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
	return ::SaveFile(fileName, ret);
}

bool FileDataArray::LoadFile(const char *fileName)
{
	Clear();
	StringParse in = ::LoadFile(fileName);
	
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

String FileDataArray::GetRelativePath(String fullPath)
{
	if (basePath != fullPath.Left(basePath.GetCount()))
		return "";
	return fullPath.Mid(basePath.GetCount());
}

int64 GetDirectoryLength(String directoryName)
{
	FileDataArray files;
	files.Search(directoryName, "*.*", true);
	return files.GetSize();
}

int64 GetLength(String fileName)
{
	if (FileExists(fileName))
		return GetFileLength(fileName);
	else	
		return GetDirectoryLength(fileName);
}

FileDiffArray::FileDiffArray()
{
	Clear();
}

void FileDiffArray::Clear()
{
	diffList.Clear();
}

// True if equal
bool FileDiffArray::Compare(FileDataArray &master, FileDataArray &secondary)
{
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
		int idSec = secondary.Find(master[i].relFilename, master[i].fileName, master[i].isFolder);
		if (idSec >= 0) {
			bool useId = master.UseId() && secondary.UseId();
			secReviewed[idSec] = true;
			if (master[i].isFolder) 
				;
			else if ((useId && (master[i].id == secondary[idSec].id)) ||
					 (!useId && (master[i].length == secondary[idSec].length) && (master[i].t == secondary[idSec].t)))
				;
			else {
				equal = false;
				FileDiff &f = diffList.Add();
				bool isf = f.isFolder = master[i].isFolder;
				f.relPath = master[i].relFilename;
				String name = f.fileName = master[i].fileName;
				f.idMaster = master[i].id;
				f.idSecondary = secondary[idSec].id;
				f.tMaster = master[i].t;
				f.tSecondary = secondary[idSec].t;
				f.lengthMaster = master[i].length;
				f.lengthSecondary = secondary[idSec].length;
				if (master[i].t >= secondary[idSec].t)
					f.action = 'u';
				else
					f.action = 'p';
			}
		} else {
			equal = false;
			FileDiff &f = diffList.Add();
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
	for (int i = 0; i < secReviewed.GetCount(); ++i) {
		if (!secReviewed[i]) {
			equal = false;
			FileDiff &f = diffList.Add();
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
	return equal;
}

bool FileDiffArray::Apply(String toFolder, String fromFolder)
{
	for (int i = 0; i < diffList.GetCount(); ++i) {
		bool ok = true;
		String dest = AppendFileName(toFolder, 
									 AppendFileName(diffList[i].relPath, diffList[i].fileName));
		switch (diffList[i].action) {
		case 'n': case 'u': 	
			if (diffList[i].isFolder) {
				if (!DirectoryExists(dest))
					ok = DirectoryCreate(dest);
			} else 
				ok = FileCopy(AppendFileName(fromFolder, FormatInt(i)), dest);
			if (!ok) {
				SetLastError(t_("Not possible to create") + String(" ") + dest);
				return false;
			}
			break;
		case 'd': 
			if (diffList[i].isFolder) {
				if (DirectoryExists(dest))
					ok = DeleteFolderDeep(dest);
			} else {
				if (FileExists(dest))
					ok = FileDelete(dest);
			}
			if (!ok) {
				SetLastError(t_("Not possible to delete") + String(" ") + dest);
				return false;
			}
			break;		
		case 'p': 
			SetLastError(t_("There was a problem in the copy"));
			return false;
		}
	}
	return true;
}

bool FileDiffArray::SaveFile(const char *fileName)
{
	return ::SaveFile(fileName, ToString());
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
	StringParse in = ::LoadFile(fileName);
	
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
	
	
#if defined(PLATFORM_WIN32) 
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
		{
		  	char dbcs[1024];
	        char *pbstr = (char *)result.bstrVal;
	      	int i = wcstombs(dbcs, result.bstrVal, *((DWORD *)(pbstr-4)));
	      	dbcs[i] = 0;
			ret = dbcs;
		}
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
#endif
