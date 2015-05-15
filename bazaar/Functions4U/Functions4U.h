#ifndef _Functions4U_Functions4U_h
#define _Functions4U_Functions4U_h

#include <float.h>
#include <Draw/Draw.h>
#ifdef flagGUI
#include <ide/Browser/Browser.h>
#include <Web/Web.h>
#include "GatherTpp.h"
#endif

#include <Functions4U/SvgColors.h>
#include "StaticPlugin.h"

NAMESPACE_UPP


enum EXT_FILE_FLAGS {NO_FLAG = 0, 
					 USE_TRASH_BIN = 1,
					 BROWSE_LINKS = 2,
					 DELETE_READ_ONLY = 4//,
					 //ASK_BEFORE_DELETE = 8
};

bool LaunchFile(const char *file);

bool FileCat(const char *file, const char *appendFile);

int FileCompare(const char *path1, const char *path2);

int64 FindStringInFile(const char *file, const String text, int64 pos0 = 0);

bool FileStrAppend(const char *file, const char *str);
bool AppendFile(const char *filename, const char *str);

String AppendFileName(const String& path1, const char *path2, const char *path3);
	
inline String Trim(const String& s) {return TrimBoth(s);};

String FitFileName(String fileName, int len);

String Tokenize(const String &str, const String &token, int &pos);
String Tokenize(const String &str, const String &token);
	
/////////
bool DirectoryExistsX(const char *path, EXT_FILE_FLAGS flags = NO_FLAG); 
bool DirectoryCopyX(const char *dir, const char *newPlace);
bool DirectoryMove(const char *dir, const char *newPlace);
bool DeleteDeepWildcardsX(const char *path, bool filefolder, EXT_FILE_FLAGS flags = NO_FLAG);
bool DeleteDeepWildcardsX(const char *pathwc, const char *namewc, bool filefolder, EXT_FILE_FLAGS flags = NO_FLAG);
bool DeleteFolderDeepWildcardsX(const char *path, EXT_FILE_FLAGS flags = NO_FLAG);
bool DeleteFileDeepWildcardsX(const char *path, EXT_FILE_FLAGS flags = NO_FLAG);
bool DeleteFolderDeepX(const char *path, EXT_FILE_FLAGS flags = NO_FLAG);
bool RenameDeepWildcardsX(const char *path, const char *namewc, const char *newname, bool forfile, bool forfolder, EXT_FILE_FLAGS flags = NO_FLAG);

bool FileMoveX(const char *oldpath, const char *newpath, EXT_FILE_FLAGS flags = NO_FLAG);
bool FileDeleteX(const char *path, EXT_FILE_FLAGS flags = NO_FLAG);

bool IsRootFolder(const char *folderName);
String GetUpperFolder(const String &folderName);
String GetNextFolder(const String &folder, const String &lastFolder);
String FileRealName(const char *fileName);
bool IsFile(const char *fileName);
bool IsFolder(const char *fileName);
String GetRelativePath(String &from, String &path);
	
bool IsSymLink(const char *path);

bool SetReadOnly(const char *path, bool readOnly);
bool SetReadOnly(const char *path, bool usr, bool grp, bool oth);
bool IsReadOnly(const char *path, bool &usr, bool &grp, bool &oth);

String LoadFile_Safe(const String fileName);
String LoadFile(const char *fileName, off_t from, size_t len = 0);

int64 GetLength(const char *fileDirName); 
int64 GetDirectoryLength(const char *directoryName);

///////////////////////////////
Upp::Array<String> SearchFile(String dir, const Upp::Array<String> &condFiles, const Upp::Array<String> &condFolders, 
								 const Upp::Array<String> &extFiles,  const Upp::Array<String> &extFolders, 
								 const String text, Upp::Array<String> &errorList);
Upp::Array<String> SearchFile(String dir, String condFile, String text, Upp::Array<String> &errorList);//, int flags = 0);
Upp::Array<String> SearchFile(String dir, String condFile, String text = "");//, int flags = 0);
///////////////////////////////

bool FileToTrashBin(const char *path);
int64 TrashBinGetCount();
bool TrashBinClear();

String GetPersonalFolder();
String GetRootFolder();
String GetTempFolder();
String GetOsFolder();
String GetSystemFolder();
bool SetEnv(const char *id, const char *val);

struct FileData : Moveable<FileData> {
	bool isFolder;
	String fileName;
	String relFilename;
	int64 length;
	struct Upp::Time t;
	int64 id;
	
	String ToString() const { return Format("%s %0n", fileName, length); }

	FileData(bool isFolder, String fileName, String relFilename, int64 length, 
		struct Upp::Time t, uint64 id) : isFolder(isFolder), fileName(fileName), 
		relFilename(relFilename), length(length), t(t), id(id) {}
	FileData() {}
};

struct FileDiffData {
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
	Upp::Array<String> &GetLastError()	{return errorList;};
	int Find(String &relFileName, String &fileName, bool isFolder);
	int Find(FileDataArray &data, int id);
	String FullFileName(int i)		{return AppendFileName(basePath, fileList[i].fileName);};
	bool SaveFile(const char *fileName);
	bool AppendFile(const char *fileName);
	bool LoadFile(const char *fileName);

private:
	void Search_Each(String dir, String condFile, bool recurse, String text);
	int64 GetFileId(String fileName);
	String GetRelativePath(const String &fullPath);
	String GetFileText();
	
	Upp::Array<FileData> fileList;
	Upp::Array<String> errorList;
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
	FileDiffData& operator[](long i)	{return diffList[i];}
	bool Compare(FileDataArray &master, FileDataArray &secondary, const String folderFrom, 
		Upp::Array<String> &excepFolders, Upp::Array<String> &excepFiles, int sensSecs = 0);
	bool Apply(String toFolder, String fromFolder, EXT_FILE_FLAGS flags = NO_FLAG);
	long GetCount()				{return diffList.GetCount();};
	bool SaveFile(const char *fileName);
	bool LoadFile(const char *fileName);
	String ToString();
	
private:
	Upp::Array<FileDiffData> diffList;
};

String Replace(String str, String find, String replace); 
String Replace(String str, char find, char replace);

int ReverseFind(const String& s, const String& toFind, int from = 0);

String FormatLong(long a); 
	
// const char *StrToTime(struct Upp::Time& d, const char *s);	Now included in TimeDate
Time StrToTime(const char *s);
Date StrToDate(const char *s);

String BytesToString(uint64 bytes, bool units = true);

String SecondsToString(double seconds, bool units = false, bool dec = true);
String HMSToString(int hour, int min, double seconds, bool units = false, bool dec = true); 
double StringToSeconds(String str);		// The opposite
void StringToHMS(String durat, int &hour, int &min, double &seconds); 

String SeasonName(int iseason);
	
String FormatDoubleAdjust(double d, double range);

String RemoveAccents(String str);
String RemoveAccent(wchar c);
bool IsPunctuation(wchar c);
String RemovePunctuation(String str);
	
inline double ToRad(double angle)	{return angle*M_PI/180.;}
inline double ToDeg(double angle)	{return angle*180./M_PI;}

inline bool Odd(int val)	  		{return val%2;}
inline bool Even(int val) 	  		{return !Odd(val);}
inline int RoundEven(int val) 		{return Even(val) ? val : val+1;}
template<class T>
inline int Sign(T a) 				{return (a > 0) - (a < 0);}
template<class T>
inline T Average(T a, T b) 			{return T(a+b)/2;}
template<class T>
inline T Average(T a, T b, T c)		{return T(a+b+c)/3;}
template<class T>
inline T Average(T a, T b, T c, T d){return T(a+b+c+d)/4;}
template<class T>
inline T pow2(T a) {return a*a;}
template<class T>
inline T pow3(T a) {return a*a*a;}
template<class T>
inline T pow4(T a) {return pow2(pow2(a));}
template <class T> 
inline bool Between(const T& val, const T& min, const T& max) { 
	return val >= min && val <= max;
}

template <class T> 
T AngleAdd360(T ang, T val) {
	ang += val;
	while (ang >= 360)
		ang -= 360;
	while (ang < 0)
		ang += 360;
	return ang;
}

template <class T> 
inline const T Distance(const T& x1, const T& y1, const T& x2, const T& y2)  { 
	return sqrt(pow2(x1-x2) + pow2(y1-y2)); }

template <class T> 
inline const T Distance(const Point_<T>& p1, const Point_<T>& p2)  { 
	return Distance<T>(p1.x, p1.y, p2.x, p2.y); }

template <class T> 
inline const T Distance(const T& x1, const T& y1, const T& z1, const T& x2, const T& y2, const T& z2)  { 
	return sqrt(pow2(x1-x2) + pow2(y1-y2) + pow2(z1-z2)); }
	
template <class T> 
inline const double Angle(const T& x1, const T& y1, const T& x2, const T& y2)  { 
	return atan2(y2-y2, x2-x1);
}

template <class T> 
inline const double Angle(const Point_<T>& p1, const Point_<T>& p2)  { 
	return Angle<T>(p1.x, p1.y, p2.x, p2.y);
}


Vector<Vector <Value> > ReadCSV(const String strFile, char separator = ',', bool bycols = true, bool removeRepeated = true, char decimalSign = '.', bool onlyStrings = false);
Vector<Vector <Value> > ReadCSVFile(const String fileName, char separator = ',', bool bycols = true, bool removeRepeated = true, char decimalSign = '.', bool onlyStrings = false);
bool ReadCSVFileByLine(const String fileName, Gate2<int, Vector<Value>&> WhenRow, char separator = ',', char decimalSign = '.', bool onlyStrings = false);
String WriteCSV(Vector<Vector <Value> > &data, char separator = ',', bool bycols = true, char decimalSign = '.');
bool WriteCSVFile(const String fileName, Vector<Vector <Value> > &data, char separator = ',', bool bycols = true, char decimalSign = '.');

	
// A String based class to parse into
class StringParse : public String {
public:
	void GoInit()	{pos = 0; lastSeparator='\0';};
	StringParse():String("") {GoInit();};
	StringParse(String s): String(s) {GoInit();};
	bool GoBefore(const String text) {
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
	bool GoAfter(const String text) {
		if(!GoBefore(text))
			return false;
		pos += int(strlen(text));
		return true;
	};
	bool GoAfter(const String text, const String text2) {
		if(!GoAfter(text))
			return false;
		if(!GoAfter(text2))
			return false;
		return true;
	};
	bool GoAfter(const String text, const String text2, const String text3) {
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
	
	void GoBeginLine() {
		for (; pos >= 0; --pos) {
			if ((ToString()[pos-1] == '\r') || (ToString()[pos-1] == '\n'))
				return;
		} 
	}
	bool IsBeginLine() {
		if (pos == 0)
			return true;
		if ((ToString()[pos-1] == '\r') || (ToString()[pos-1] == '\n'))
			return true;
		return false;
	}
	bool IsSpaceRN(int c) {
		if (IsSpace(c))
			return true;
		if ((c == '\r') || (c == '\n'))
		     return true;
		return false;
	}
	// Gets text between "" or just a word until an space
	// It considers special characters with \ if between ""
	// If not between "" it gets the word when it finds one of the separator characters
	String GetText(String separators = "") {
		String ret = "";
		if (pos > GetCount() || pos == -1)
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
	String GetLine() {
		String ret;
		if (pos > GetCount() || pos == -1)
			return String();
		while (ToString()[pos] != '\0') {
			if (ToString()[pos] == '\n') {
				pos++;
				return ret;
			}
			if (ToString()[pos] == '\r' && ToString()[pos+1] == '\n') {
				pos += 2;
				return ret;
			}
			ret.Cat(ToString()[pos]);
			pos++;
		}
		return ret;
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
String WideToString(LPCWSTR wcs, int len = -1);
#endif
 
#ifdef CTRLLIB_H
	#include "Functions4U/Functions4U_Gui.h"
#endif

String GetExtExecutable(const String ext);

Upp::Array<String> GetDriveList();


class Dl {
public:
	Dl();
	~Dl();
	bool Load(const String &fileDll);
	void *GetFunction(const String &functionName);
	
private:
#if defined(PLATFORM_WIN32) 
	HINSTANCE hinstLib;	
#else
	void *hinstLib;
#endif
};

typedef Dl Dll;

//bool RunFromMemory(const String &progBuffer, const String &name);


String BsGetLastError();
bool BSPatch(String oldfile, String newfile, String patchfile);
bool BSDiff(String oldfile, String newfile, String patchfile);


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

Image GetRect(const Image& orig, const Rect &r);

double tmGetTimeX();

class _NRFuse {
public:
	_NRFuse(bool *_inside) {inside = _inside; failed = true;}
	~_NRFuse() 			   {if (!failed) *inside = false;}
	bool failed;
private:
	bool *inside;
};

#define NON_REENTRANT_V	 	static bool _insideNR; _NRFuse _fuseNR(&_insideNR); \
							if(!_insideNR) {									\
								_insideNR = true; 								\
								_fuseNR.failed = false;							\
							} else 												\
								return
#define NON_REENTRANT(v) 	static bool _insideNR; _NRFuse _fuseNR(&_insideNR); \
							if(!_insideNR) {									\
								_insideNR = true; 								\
								_fuseNR.failed = false;							\
							} else 												\
								return v

END_UPP_NAMESPACE

#endif
