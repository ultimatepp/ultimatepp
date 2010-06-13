#ifndef _Functions4U_Functions4U_h
#define _Functions4U_Functions4U_h

#include <ide/Browser/Browser.h>
#include <Web/Web.h>
#include "GatherTpp.h"
#include "SvgColors.h"

enum EXT_FILE_FLAGS {USE_TRASH_BIN = 1,
					 BROWSE_LINKS = 2,
					 DELETE_READ_ONLY = 4
};

bool LaunchFile(const char *file);

bool FileCat(const char *file, const char *appendFile);

bool FileStrAppend(const char *file, const char *str);
bool AppendFile(const char *filename, const char *str);

/////////
bool DirectoryExistsX(const char *path, int flags = 0);
///////////////////////////////
bool DirectoryDeleteX(const char *path, int flags = 0);
bool DirectoryDeleteDeepX(const char *path, int flags = 0);
bool DeleteFolderDeepX(const char *dir, int flags = 0);
bool DirectoryCopyX(const char *dir, const char *newPlace);
bool DeleteFolderDeepWildcards(const char *dir, int flags = 0);
///////////////////////////////

String GetUpperFolder(String folderName);

String GetNextFolder(String folder, String lastFolder);

String GetRealName(String fileName);

//bool GetSymLinkPath(const char *linkPath, String &filePath);
bool IsSymLink(const char *path);

bool CreateFolderDeep(const char *dir);

/////////
bool FileMoveX(const char *oldpath, const char *newpath, int flags = 0);
bool FileDeleteX(const char *path, int flags = 0);
/////////


//bool FileSetReadOnly(const char *fileName, bool readOnly);
bool FileSetReadOnly(const char *fileName, bool usr, bool grp = false, bool oth = false);
bool IsReadOnly(const char *fileName, bool &usr, bool &grp, bool &oth);

String LoadFile_Safe(String fileName);

int64 GetLength(String fileDirName);
int64 GetDirectoryLength(String directoryName);

///////////////////////////////
Array<String> SearchFile(String dir, String condFile, String text, Array<String> &errorList);//, int flags = 0);
Array<String> SearchFile(String dir, String condFile, String text = "");//, int flags = 0);
///////////////////////////////

bool FileToTrashBin(const char *path);
int64 TrashBinGetCount();
bool TrashBinClear();

//String GetDesktopFolder();
//String GetProgramsFolder();
//String GetAppDataFolder();
//String GetMusicFolder();
//String GetPicturesFolder();
//String GetVideoFolder();
String GetPersonalFolder();
//String GetTemplatesFolder();
//String GetDownloadFolder();
String GetRootFolder();
String GetTempFolder();
String GetOsFolder();
String GetSystemFolder();


struct FileData : Moveable<FileData> {
	bool isFolder;
	String fileName;
	String relFilename;
	int64 length;
	struct Upp::Time t;
	int64 id;
	
	String ToString() const { return Format("%s %0n", fileName, length); }

	FileData(bool isFolder, String fileName, String relFilename, int64 length, struct Upp::Time t, uint64 id) : isFolder(isFolder), fileName(fileName), relFilename(relFilename), length(length), t(t), id(id) {}
	FileData() {}
};

struct FileDiff {
	char action;	// 'n': New, 'u': Update, 'd': Delete, 'p': Problem
	bool isFolder;
	String relPath;
	String fileName;
	uint64 idMaster, idSecondary;
	struct Upp::Time tMaster, tSecondary;
	uint64 lengthMaster, lengthSecondary;
};

class ErrorHandling
{
public:
	void SetLastError(String _lastError)	{lastError = _lastError;};
	String GetLastError()					{return lastError;};
	
private:
	String lastError;
};

class FileDiffArray;

class FileDataArray : public ErrorHandling
{
public:
	FileDataArray(bool use = false, int fileFlags = 0);
	bool Init(String folder, FileDataArray &orig, FileDiffArray &diff);
	void Clear();
	bool Search(String dir, String condFile, bool recurse = false, String text = "");
	FileData& operator[](long i)	{return fileList[i];}
	long GetFileCount()				{return fileCount;};
	long GetFolderCount()			{return folderCount;};
	long GetCount() 				{return fileCount + folderCount;};
	int64 GetSize()					{return fileSize;};
	inline bool UseId() 			{return useId;};
	void SortByName(bool ascending = true);
	void SortByDate(bool ascending = true);
	void SortBySize(bool ascending = true);
	Array<String> &GetLastError()	{return errorList;};
	int Find(String &relFileName, String &fileName, bool isFolder);
	int Find(FileDataArray &data, int id);
	String FullFileName(int i)		{return AppendFileName(basePath, fileList[i].fileName);};
	bool SaveFile(const char *fileName);
	bool AppendFile(const char *fileName);
	bool LoadFile(const char *fileName);

private:
	void Search_Each(String dir, String condFile, bool recurse, String text);
	int64 GetFileId(String fileName);
	String GetRelativePath(String fullPath);
	String GetFileText();
	
	Array<FileData> fileList;
	Array<String> errorList;
	String basePath;
	long fileCount, folderCount;
	int64 fileSize;
	bool useId;
	int fileFlags;
};

class FileDiffArray : public ErrorHandling
{
public:
	FileDiffArray();
	void Clear();
	FileDiff& operator[](long i)	{return diffList[i];}
	bool Compare(FileDataArray &master, FileDataArray &secondary);
	bool Apply(String toFolder, String fromFolder, int flags = 0);
	long GetCount()				{return diffList.GetCount();};
	bool SaveFile(const char *fileName);
	bool LoadFile(const char *fileName);
	String ToString();
	
private:
	Array<FileDiff> diffList;
};


String Replace(String str, String find, String replace);

int ReverseFind(const String& s, const String& toFind, int from = 0);

String FormatLong(long a); 

const char *StrToTime(struct Upp::Time& d, const char *s);
::Time StrToTime(const char *s);
::Date StrToDate(const char *s);

String BytesToString(uint64 bytes);

String SecondsToString(double seconds, bool units = false);
String HMSToString(int hour, int min, double seconds, bool units = false); 
double StringToSeconds(String str);		// The opposite
void StringToHMS(String durat, int &hour, int &min, double &seconds); 

String RemoveAccents(String str);
bool IsPunctuation(wchar c);
	
inline double ToRad(double angle)	{return angle*M_PI/180;}

inline bool Odd(int val)	  	{return val%2;}
inline bool Even(int val) 	  	{return !Odd(val);}
inline int RoundEven(int val) 	{return Even(val) ? val : val+1;}
template<class T>
inline int Sign(T a) 			{return (a > 0) - (a < 0);}
template<class T>
inline T Average(T a, T b) 			{return T((a+b)/2);}
template<class T>
inline T Average(T a, T b, T c)		{return T((a+b+c)/3);}
template<class T>
inline T Average(T a, T b, T c, T d){return T((a+b+c+d)/4);}
template <class T> 
inline const T& min(const T& a, const T& b, const T& c) { 
	return a < b ? (a < c ? a : c) : ((b < c) ? b : c); }
template <class T> 
inline const T& min(const T& a, const T& b, const T& c, const T& d) { 
	T ab = min(a, b);
	T cd = min(c, d);
	return ab < cd ? ab : cd;
}
template <class T> 
inline const T& max(const T& a, const T& b, const T& c)  { 
	return a > b ? (a > c ? a : c) : ((b > c) ? b : c); }
template <class T> 
inline const T& max(const T& a, const T& b, const T& c, const T& d) { 
	T ab = max(a, b);
	T cd = max(c, d);
	return ab > cd ? ab : cd;
}

int DayOfYear(Date d);

// Fits object centered into frame maintaining the aspect
template <class T>
Rect_<T> FitInFrame(const Size_<T> &frame, const Size_<T> &object)
{
	double frameAspect  = frame.cx/(double)frame.cy; 
	double objectAspect = object.cx/(double)object.cy;	
	
	if (frameAspect > objectAspect) {
		double x = (frame.cx - objectAspect*frame.cy)/2.;
		return Rect_<T>((T)x, 0, (T)(x + objectAspect*frame.cy), frame.cy);
	} else {
		double y = (frame.cy - frame.cx/objectAspect)/2.;
		return Rect_<T>(0, (T)y, frame.cx, (T)(y + frame.cx/objectAspect));
	}
}

Color RandomColor();

// A String based class to parse into
class StringParse :  public String {
public:
	void GoInit()	{pos = 0; lastSeparator='\0';};
	StringParse():String("") {GoInit();};
	StringParse(String s): String(s) {GoInit();};
	bool GoBefore(const String text)
	{
		if (pos >= GetLength()) {
			pos = GetLength()-1;
			return false;
		}
		int newpos = String::Find(text, pos);
		if (newpos < 0)
			return false;	// If it does not find it, it does not move
		pos = newpos;
		return true;
	};	
	bool GoAfter(const String text)
	{
		if(!GoBefore(text))
			return false;
		pos += strlen(text);
		return true;
	};
	bool GoAfter(const String text, const String text2)
	{
		if(!GoAfter(text))
			return false;
		if(!GoAfter(text2))
			return false;
		return true;
	};
	bool GoAfter(const String text, const String text2, const String text3)
	{
		if(!GoAfter(text))
			return false;
		if(!GoAfter(text2))
			return false;
		if(!GoAfter(text3))
			return false;
		return true;
	};
	bool GoAfter_Init(const String text) {GoInit();	return GoAfter(text);};
	bool GoAfter_Init(const String text, const String text2) {GoInit();	return GoAfter(text, text2);};
	bool GoAfter_Init(const String text, const String text2, const String text3) {GoInit();	return GoAfter(text, text2, text3);};		
	
	void GoBeginLine()
	{
		for (; pos >= 0; --pos) {
			if ((ToString()[pos-1] == '\r') || (ToString()[pos-1] == '\n'))
				return;
		} 
	}
	bool IsBeginLine()
	{
		if (pos == 0)
			return true;
		if ((ToString()[pos-1] == '\r') || (ToString()[pos-1] == '\n'))
			return true;
		return false;
	}
	bool IsSpaceRN(int c)
	{
		if (IsSpace(c))
			return true;
		if ((c == '\r') || (c == '\n'))
		     return true;
		return false;
	}
	// Gets text between "" or just a word until an space
	// It considers special characters with \ if between ""
	// If not between "" it gets the word when it finds one of the separator characters
	String GetText(String separators = "")	
	{
		String ret = "";
		if (pos > GetCount())
			return ret;
		int newpos = pos;
		
		while ((IsSpaceRN(ToString()[newpos]) && (ToString()[newpos] != '\"') && 
			   (ToString()[newpos] != '\0')))
			newpos++;
		if (ToString()[newpos] == '\0') {
			pos = newpos;
			return "";
		}
	
		if (ToString()[newpos] == '\"') {	// Between ""
			newpos++;
			while (ToString()[newpos] != '\"' && ToString()[newpos] != '\0') {
				if (ToString()[newpos] == '\\') {
					newpos++;
					if (ToString()[newpos] == '\0')
						return "";
				} 
				ret.Cat(ToString()[newpos]);
				newpos++;
			}
			lastSeparator = '"';
		} else if (separators == "") {		// Simple word
			while (!IsSpaceRN(ToString()[newpos]) && ToString()[newpos] != '\0') {
				if (ToString()[newpos] == '\"') {
					newpos--;	// This " belongs to the next
					break;
				}
				ret.Cat(ToString()[newpos]);
				newpos++;
			}
			lastSeparator = ToString()[newpos];
		} else {							// Simple word, special separator
			while (ToString()[newpos] != '\0') {// Only consider included spaces (!IsSpaceRN(ToString()[newpos]) && ToString()[newpos] != '\0') {
				if (ToString()[newpos] == '\"') {
					newpos--;	// This " belongs to the next
					break;
				}				
				if (separators.Find(ToString()[newpos]) >= 0) {
					lastSeparator = ToString()[newpos];
					break;
				}
				ret.Cat(ToString()[newpos]);
				newpos++;
			} 
			lastSeparator = ToString()[newpos];
		}
		pos = ++newpos;		// After the separator: ", space or separator
		return ret;
	}
	String GetLine()
	{
		return GetText("\r\n");
	}
	double GetDouble(String separators = "")  	{return atof(GetText(separators));};
	int GetInt(String separators = "")			{return atoi(GetText(separators));};
	long GetLong(String separators = "")		{return atol(GetText(separators));};
	uint64 GetUInt64(String separators = "")	
#if defined(PLATFORM_WIN32) 
	{return _atoi64(GetText(separators));};
#endif
#ifdef PLATFORM_POSIX
	{return atoll(GetText(separators));};
#endif
	
	String Right() {return String::Mid(pos+1);}
	int GetLastSeparator() {return lastSeparator;}
	void MoveRel(int val)
	{
		pos += val;
		if (pos < 0)
			pos = 0;
		else if (pos >= GetCount())
			pos = GetCount() - 1;
	}
	int GetPos() {return pos;};
	bool SetPos(int i) 
	{
		if (i < 0 || i >= GetCount())
			return false;
		else {
			pos = i;
			return true;
		}
	}
	bool Eof()
	{
		return pos >= GetCount();
	}
	unsigned Count(String s)
	{
		int from = 0;
		unsigned count = 0;
		
		while ((from = ToString().Find(s, from)) >= 0) {
			count++;
			from++;
		}
		return count;
	}
private:
	int pos;
	int lastSeparator;
};

#if defined(PLATFORM_WIN32) 
Value GetVARIANT(VARIANT &result);
#endif
 
#ifdef CTRLLIB_H
	#include "Functions4U/Functions4U_Gui.h"
#endif

/*
// A ProcessEvents than can be used in non gui programs
inline void DoEvents() { 
#ifdef CTRLLIB_H
		Ctrl::ProcessEvents();
#endif
}
*/

String GetExtExecutable(String ext);

Array<String> GetDriveList();

String Getcwd();
bool Chdir (const String &folder);

//String Format(Time time, const char*fmt = "%2d:%2d");

#if defined(PLATFORM_WIN32) 
class Dll {
public:
	Dll();
	~Dll();
	bool Load(const String &fileDll);
	void *GetFunction(const String &functionName);
	
private:
	HINSTANCE hinstLib;	
};

//bool RunFromMemory(const String &progBuffer, const String &name);

#endif

String BsGetLastError();
bool BSPatch(String oldfile, String newfile, String patchfile);
bool BSDiff(String oldfile, String newfile, String patchfile);

bool LoadFromXMLFileAES(Callback1<XmlIO> xmlize, const char *file, const char *key);
template <class T>
bool LoadFromXMLFileAES(T& data, const char *file, const char *key)
{
	ParamHelper__<T> p(data);
	return LoadFromXMLFileAES(callback(&p, &ParamHelper__<T>::Invoke), file, key);
}

bool StoreAsXMLFileAES(Callback1<XmlIO> xmlize, const char *name, const char *file, const char *key);
template <class T>
bool StoreAsXMLFileAES(T& data, const char *name, const char *file, const char *key)
{
	ParamHelper__<T> p(data);
	return StoreAsXMLFileAES(callback(&p, &ParamHelper__<T>::Invoke), name, file, key);
}

#ifdef flagAES

#include <openssl/aes.h>
#include <AESStream/AESStream.h>

bool LoadFromXMLFileAES(Callback1<XmlIO> xmlize, const char *file, const char *key);
bool StoreAsXMLFileAES(Callback1<XmlIO> xmlize, const char *name, const char *file, const char *key);

#endif

#endif
