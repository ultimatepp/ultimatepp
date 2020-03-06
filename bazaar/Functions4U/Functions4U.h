#ifndef _Functions4U_Functions4U_h
#define _Functions4U_Functions4U_h

#include <float.h>
#include <Draw/Draw.h>
#ifdef flagGUI
#include <Web/Web.h>
#include "GatherTpp.h"
#endif

#include <ScatterDraw/Pedantic.h>
#include <Functions4U/SvgColors.h>
#include "StaticPlugin.h"
#include "LocalProcess2.h"
#include <random>

namespace Upp {


enum EXT_FILE_FLAGS {NO_FLAG = 0, 
					 USE_TRASH_BIN = 1,
					 BROWSE_LINKS = 2,
					 DELETE_READ_ONLY = 4
};

String GetDesktopManagerNew();

bool LaunchFile(const char *file, const char *params = 0, const char *directory = ".");

bool FileCat(const char *file, const char *appendFile);

int FileCompare(const char *path1, const char *path2);

int64 FindStringInFile(const char *file, const String text, int64 pos0 = 0);

bool FileStrAppend(const char *file, const char *str);
bool AppendFile(const char *filename, const char *str);

template<typename T>
String AppendFileNameX(T t) {
    return t;
}

template<typename T, typename... Args>
String AppendFileNameX(T t, Args... args) {
    return AppendFileName(t, AppendFileNameX(args...));
}
	
inline String Trim(const String& s)   {return TrimBoth(s);}
//inline WString Trim(const WString& s) {return TrimLeft(TrimRight(s));}

String FitFileName(String fileName, int len);

Vector<String> Tokenize(const String &str, const String &token, int pos = 0);
void Tokenize(const String &str, const String &token, Vector<String> &ret, int pos = 0);
String Tokenize2(const String &str, const String &token, int &pos);
String Tokenize2(const String &str, const String &token);
//String Tokenize(const String &str, const String &token, int &pos);
//String Tokenize(const String &str, const String &token);
	
/////////
bool DirectoryExistsX(const char *path, EXT_FILE_FLAGS flags = NO_FLAG); 
void DirectoryCopyX(const char *dir, const char *newPlace, bool replaceOnlyNew, String filesToExclude, String &erroList);
bool DirectoryMove(const char *dir, const char *newPlace);
bool DeleteDeepWildcardsX(const char *path, bool filefolder, EXT_FILE_FLAGS flags = NO_FLAG, bool deep = true);
bool DeleteDeepWildcardsX(const char *pathwc, const char *namewc, bool filefolder, EXT_FILE_FLAGS flags = NO_FLAG, bool deep = true);
bool DeleteFolderDeepWildcardsX(const char *path, EXT_FILE_FLAGS flags = NO_FLAG);
bool DeleteFolderDeepWildcardsX(const char *path, const char *name, EXT_FILE_FLAGS flags = NO_FLAG);
bool DeleteFileDeepWildcardsX(const char *path, EXT_FILE_FLAGS flags = NO_FLAG);
bool DeleteFileWildcardsX(const char *path, EXT_FILE_FLAGS flags = NO_FLAG);
bool DeleteFolderDeepX(const char *path, EXT_FILE_FLAGS flags = NO_FLAG);
bool RenameDeepWildcardsX(const char *path, const char *namewc, const char *newname, bool forfile, bool forfolder, EXT_FILE_FLAGS flags = NO_FLAG);
bool FolderIsEmpty(const char *path);

bool FileMoveX(const char *oldpath, const char *newpath, EXT_FILE_FLAGS flags = NO_FLAG);
bool FileDeleteX(const char *path, EXT_FILE_FLAGS flags = NO_FLAG);

bool IsRootFolder(const char *folderName);
String GetUpperFolder(const String &folderName);
String GetNextFolder(const String &folder, const String &lastFolder);
String FileRealName(const char *fileName);
bool IsFile(const char *fileName);
bool IsFolder(const char *fileName);
bool GetRelativePath(String from, String path, String& ret, bool normalize = true);
	
bool IsSymLink(const char *path);

bool SetReadOnly(const char *path, bool readOnly);
bool SetReadOnly(const char *path, bool usr, bool grp, bool oth);
bool IsReadOnly(const char *path, bool &usr, bool &grp, bool &oth);

String LoadFile_Safe(const String fileName);
String LoadFile(const char *fileName, off_t from, size_t len = 0);

int64 GetLength(const char *fileDirName); 
int64 GetDirectoryLength(const char *directoryName);

///////////////////////////////
Vector<String> SearchFile(String dir, const Vector<String> &condFiles, const Vector<String> &condFolders, 
								 const Vector<String> &extFiles,  const Vector<String> &extFolders, 
								 const String text, Vector<String> &errorList);
Vector<String> SearchFile(String dir, String condFile, String text, Vector<String> &errorList);//, int flags = 0);
Vector<String> SearchFile(String dir, String condFile = "*.*", String text = "");//, int flags = 0);
///////////////////////////////

bool FileToTrashBin(const char *path);
int64 TrashBinGetCount();
bool TrashBinClear();

String GetPersonalFolder();
String GetRootFolder();
String GetTempFolder();
String GetOsFolder();
String GetSystemFolder();
#ifdef PLATFORM_WIN32
String GetCommonAppDataFolder();
#endif
bool SetEnv(const char *id, const char *val);

struct FileData : Moveable<FileData> {
	bool isFolder;
	String fileName;
	String relFilename;
	int64 length;
	struct Upp::Time t;
	int64 id;
	
	String ToString() const { return Format("%s %0n", fileName, length); }

	FileData(bool _isFolder, String _fileName, String _relFilename, int64 _length, 
		struct Upp::Time _t, int64 _id) : isFolder(_isFolder), fileName(_fileName), 
		relFilename(_relFilename), length(_length), t(_t), id(_id) {}
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

class ErrorHandling {
public:
	void SetLastError(String _lastError)	{lastError = _lastError;};
	String GetLastError()					{return lastError;};
	
private:
	String lastError;
};

class FileDiffArray;

class FileDataArray : public ErrorHandling {
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
	Vector<String> &GetLastError()	{return errorList;};
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
	Vector<String> errorList;
	String basePath;
	long fileCount, folderCount;
	int64 fileSize;
	bool useId;
	int fileFlags;
};

class FileDiffArray : public ErrorHandling {
public:
	FileDiffArray();
	void Clear();
	FileDiffData& operator[](long i)	{return diffList[i];}
	bool Compare(FileDataArray &master, FileDataArray &secondary, const String folderFrom, 
		Vector<String> &excepFolders, Vector<String> &excepFiles, int sensSecs = 0);
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
	
Upp::Time StrToTime(const char *s);
Date StrToDate(const char *s);

String BytesToString(uint64 bytes, bool units = true);

String SecondsToString(double seconds, int dec = 2, bool units = false, bool space = false, 
						bool tp = false, bool longUnits = false, bool forceSec = false);
String HMSToString(int hour, int min, double seconds, int dec = 2, bool units = false, bool space = false, 
						bool tp = false, bool longUnits = false, bool forceSec = false); 																
double StringToSeconds(String str);		
void StringToHMS(String durat, int &hour, int &min, double &seconds); 

String SeasonName(int iseason);
int GetSeason(Date &date);
	
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
inline T Neg(T a) 					{return a > 0 ? -a : a;}
template<class T>
inline T Average(T a, T b) 			{return T(a+b)/2;}
template<class T>
inline T Avg(T a, T b) 				{return Average(a, b);}
template<class T>
inline T Average(T a, T b, T c)		{return T(a+b+c)/3;}
template<class T>
inline T Avg(T a, T b, T c) 		{return Average(a, b, c);}
template<class T>
inline T Average(T a, T b, T c, T d){return T(a+b+c+d)/4;}
template<class T>
inline T Avg(T a, T b, T c, T d)	{return Average(a, b, c, d);}
template<class T>
inline T pow2(T a) {return a*a;}
template<class T>
inline T pow3(T a) {return a*a*a;}
template<class T>
inline T pow4(T a) {return pow2(pow2(a));}
template<class T>
inline T fround(T x, int numdec) {
	int64 mult = 10*numdec;
	return T(int64(x*mult + .5))/mult;	
}
template <class T> 
inline bool Between(const T& val, const T& min, const T& max) { 
	return val >= min && val <= max;
}
template <class T> 
inline T BetweenVal(const T& val, const T& _min, const T& _max) { 
	return max(_min, min(_max, val));
}
template <class T> 
inline bool IsNAN(T val) {return std::isnan(val);}

template <class T> 
inline T FixFloat(T val) {
	if(std::isnan(val) || std::isinf(val) || val == HUGE_VAL || val == -HUGE_VAL)
		return Null;
	return val;
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
inline const T Norm(const T& dx, const T& dy)  { 
	return static_cast<T>(sqrt(dx*dx + dy*dy)); }
	
template <class T> 
inline const T Distance(const T& x1, const T& y1, const T& x2, const T& y2)  { 
	return Norm(x1-x2, y1-y2); }

template <class T> 
inline const T Distance(const Point_<T>& p1, const Point_<T>& p2)  { 
	return Distance<T>(p1.x, p1.y, p2.x, p2.y); }

template <class T> 
inline const T Distance(const T& x1, const T& y1, const T& z1, const T& x2, const T& y2, const T& z2)  { 
	return static_cast<T>(sqrt(pow2(x1-x2) + pow2(y1-y2) + pow2(z1-z2))); }
	
template <class T> 
inline const T Angle(const T& x1, const T& y1, const T& x2, const T& y2)  { 
	return static_cast<T>(atan2(y2-y1, x2-x1));
}

template <class T> 
inline const T Angle(const Point_<T>& p1, const Point_<T>& p2)  { 
	return Angle<T>(p1.x, p1.y, p2.x, p2.y);
}


Vector<Vector <Value> > ReadCSV(const String strFile, char separator = ',', bool bycols = true, bool removeRepeated = true, char decimalSign = '.', bool onlyStrings = false, int fromRow = 0);
Vector<Vector <Value> > ReadCSVFile(const String fileName, char separator = ',', bool bycols = true, bool removeRepeated = true, char decimalSign = '.', bool onlyStrings = false, int fromRow = 0);
bool ReadCSVFileByLine(const String fileName, Gate<int, Vector<Value>&, String &> WhenRow, char separator = ',', char decimalSign = '.', bool onlyStrings = false, int fromRow = 0);
String WriteCSV(Vector<Vector <Value> > &data, char separator = ',', bool bycols = true, char decimalSign = '.');
bool WriteCSVFile(const String fileName, Vector<Vector <Value> > &data, char separator = ',', bool bycols = true, char decimalSign = '.');

	
// A String based class to parse into
class StringParse : public String {
public:
	void GoInit()	{pos = 0; lastSeparator='\0';};
	StringParse():String("") {GoInit();};
	StringParse(String _s): String(_s) {GoInit();};
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
	double GetDouble(String separators = "")  	{return FixFloat(atof(GetText(separators)));};
	int GetInt(String separators = "")			{return static_cast<int>(FixFloat(atof(GetText(separators))));};
	long GetLong(String separators = "")		{return static_cast<long>(FixFloat(atof(GetText(separators))));};
	uint64 GetUInt64(String separators = "")	{return static_cast<uint64>(FixFloat(atof(GetText(separators))));};
	
	String Right() 			{return String::Mid(pos+1);}
	int GetLastSeparator() 	{return lastSeparator;}
	void MoveRel(int val) {
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
	unsigned Count(String _s)
	{
		int from = 0;
		unsigned count = 0;
		
		while ((from = ToString().Find(_s, from)) >= 0) {
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
 

String GetExtExecutable(const String ext);

Vector<String> GetDriveList();


class Dl {
public:
	Dl();
	virtual ~Dl();
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
	double frameAspect  = frame.cx/static_cast<double>(frame.cy); 
	double objectAspect = object.cx/static_cast<double>(object.cy);	
	
	if (frameAspect > objectAspect) {
		double x = (frame.cx - objectAspect*frame.cy)/2.;
		return Rect_<T>(static_cast<T>(x), 0, static_cast<T>(x + objectAspect*frame.cy), frame.cy);
	} else {
		double y = (frame.cy - frame.cx/objectAspect)/2.;
		return Rect_<T>(0, static_cast<T>(y), frame.cx, static_cast<T>(y + frame.cx/objectAspect));
	}
}

Color RandomColor();

Image GetRect(const Image& orig, const Rect &r);

double tmGetTimeX();

int SysX(const char *cmd, String& out, String& err, double timeOut = Null, 
			Gate3<double, String&, String&> progress = false, bool convertcharset = true);
			
	
class _NRFuse {
public:
	_NRFuse(bool *_inside) {inside = _inside; failed = true;}
	virtual ~_NRFuse() 			   {if (!failed) *inside = false;}
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

template <class T>
struct TempAssign {
	TempAssign(T &_val, T set) {
		old = _val;
		_val = set;
		val = &_val;
	}
	virtual ~TempAssign() {
		*val = old;
	}
	
	T *val, old;
};

template <class T>
class ThreadSafe {
public:
	inline ThreadSafe()    {val = Null;}
	inline ThreadSafe(T v) {operator=(v);}
	inline void operator=(T v) {
		mutex.Enter();
		val = v;
		mutex.Leave();
	}
	inline void operator+=(T v) {
		mutex.Enter();
		val += v;
		mutex.Leave();
	}
	inline void operator-=(T v) {
		mutex.Enter();
		val -= v;
		mutex.Leave();
	}
	inline operator T() {
		T ret;
		mutex.Enter();
		ret = val;
		mutex.Leave();
		return ret;
	}
	Value GetData() {
		Value ret;
		mutex.Enter();
		ret = val;
		mutex.Leave();
		return ret;		
	}
	Value operator~() const 		{return GetData();}
	inline ThreadSafe& operator++() {
		mutex.Enter();
		val++;
		mutex.Leave();
		return *this;
	}
   	inline ThreadSafe operator++(int) {
		ThreadSafe tmp = *this;
   		++*this;
   		return tmp;
	}
   
private:
	Mutex mutex;
	T val;
};

template <class C>
static void ShuffleAscending(C &data, std::default_random_engine &generator) {
	for (int i = 0; i < data.GetCount() - 2; i++) {
	  	std::uniform_int_distribution<int> distribution(i, data.GetCount() - 1);
        Swap(data[i], data[distribution(generator)]);
    }
}

template <class C>
static void ShuffleDescending(C &data, std::default_random_engine &generator) {
	for (int i = data.GetCount() - 1; i > 0; i--) {
	  	std::uniform_int_distribution<int> distribution(0, i);
        Swap(data[i], data[distribution(generator)]);
    }
}

template <class C>
void Shuffle(C &data, int randomSeed = Null) {
	if (IsNull(randomSeed))	{
		std::random_device rd;
		randomSeed = rd();
	}
	std::mt19937 generator(randomSeed);
  
	ShuffleAscending(data, generator);
	ShuffleDescending(data, generator);	
}

template <class T>
inline T TruncDecimals(T num, int decimals) {
	long double val = num*10*decimals;
	long int ival = static_cast<long int>(val);
	val = ival;
	return static_cast<T>(val/(10*decimals));
}

template <class T>
inline String RoundDecimals(T num, int decimals) {
	return FormatDouble(num, decimals);
}

template <class T>
bool EqualRatio(const T& a, const T& b, const T& ratio) {
	if (a == 0) {
		if (b == 0)
			return true;
	} else if (b == 0) {
		if(abs((a - b)/a) <= ratio) 
			return true;
		else
			return false;
	}
	if(abs((a - b)/b) <= ratio) 
		return true;
	return false;
}

template <class Range, class V>
int FindAdd(Range& r, const V& value, int from = 0) {
	for(int i = from; i < r.GetCount(); i++)
		if(r[i] == value) 
			return i;
	r.Add(value);
	return r.GetCount()-1;
}

template <class Range, class V>
void FindAddRatio(Range& r, const V& value, const V& ratio, int from = 0) {
	for(int i = from; i < r.GetCount(); i++)
		if(EqualRatio(r[i], value, ratio)) 
			return;
	r.Add(value);
}

template <class Range, class V>
int FindIndexDelta(const Range& r, const V& value, const V& delta, int from = 0) {
	for(int i = from; i < r.GetCount(); i++) 
		if(abs(r[i] - value) <= delta) 
			return i;
	return -1;
}

template <class Range, class V>
int FindIndexRoundDecimals(const Range& r, const V& value, int numDecimals, int from = 0) {
	String svalue = RoundDecimals(value, numDecimals);
	for(int i = from; i < r.GetCount(); i++) {
		String s = RoundDecimals(r[i], numDecimals);
		if(s == svalue) 
			return i;
	}
	return -1;
}

template <class Range, class V>
int FindIndexTruncDecimals(const Range& r, const V& value, int numDecimals, int from = 0) {
	V svalue = TruncDecimals(value, numDecimals);
	for(int i = from; i < r.GetCount(); i++) {
		V s = TruncDecimals(r[i], numDecimals);
		if(s == svalue) 
			return i;
	}
	return -1;
}

template <class Range, class V>
int FindIndexCloser(const Range& r, const V& value, int from = 0) {
	int minId = -1;
	V minDiff = FLT_MAX;
	for(int i = from; i < r.GetCount(); i++) {
		V diff = abs(value - r[i]);
		if (diff < minDiff) {
			minDiff = diff;	
			minId = i;		
		}
	}
	return minId;
}

template <class Range, class V>
int FindIndexRatio(const Range& r, const V& value, const V& ratio, int from = 0) {
	for(int i = from; i < r.GetCount(); i++) {
		if (EqualRatio(r[i], value, ratio))
			return i;
	}
	return -1;
}

template <class Range, class V>
bool Compare(const Range& a, const Range& b, const V& ratio = 0) {
	if (a.GetCount() != b.GetCount())
		return false;
	for(int i = 0; i < a.GetCount(); i++) {
		V div = (b[i] != 0) ? b[i] : (a[i] != 0 ? a[i] : 1);
		if(abs(a[i] - b[i])/div > ratio) 
			return false;
	}
	return true;
}

template <class Range>
String ToString(const Range& a) {
	String ret;
	for(int i = 0; i < a.GetCount(); i++) {
		if (i > 0)
			ret << ";";
		ret << a[i]; 
	}
	return ret;
}

class RealTimeStop {  
typedef RealTimeStop CLASSNAME;
public:
	RealTimeStop() {
#ifdef CTRLLIB_H	
		callbackOn = false;
		lastTick = -1;
#endif 
		Start();
	}
	void Reset() {
		timeElapsed = lastTimeElapsed = 0;
#ifdef CTRLLIB_H
		if (!callbackOn) {
			timeCallback.Set(-5*1000, THISBACK(Tick));
			callbackOn = true;
		}
#endif
		isPaused = true;
		Continue();
	}
	void Start() {Reset();}
	void Pause(bool pause) {
		if (pause)
			Pause();
		else
			Continue();
	}
	void Pause() {
		if (!isPaused) { 		
			timeElapsed += (tmGetTimeX() - time0);
			isPaused = true;
		}
	}
	void Continue() {
		if (isPaused) {
			time0 = tmGetTimeX();
			isPaused = false;
		}
	}
	double Seconds() {
		if (isPaused)
			return timeElapsed;
		else
			return timeElapsed + (tmGetTimeX() - time0);
	}
	double Elapsed() {
		double t = Seconds();
		double elapsed = t - lastTimeElapsed;
		lastTimeElapsed = t;
		return elapsed;
	}
	void SetBack(double secs) {
		timeElapsed -= secs;
	}
	bool IsPaused()		{return isPaused;}
		
private:
	double timeElapsed;				// Time elapsed
	double time0;					// Time of last Continue()
	double lastTimeElapsed;
	bool isPaused;
#ifdef CTRLLIB_H
	bool callbackOn;
	double lastTick;
	TimeCallback timeCallback;
	
	void Tick() {
		double tActual = tmGetTimeX();
		if (!isPaused && lastTick > -1) {
			double deltaLastTick = tActual - lastTick;
			if (deltaLastTick > 5*10) 	// Some external issue has stopped normal running
				SetBack(deltaLastTick);	// Timeout timer is fixed accordingly
		}
		lastTick = tActual;
	}
#endif
};

class LocalProcessX
#ifdef CTRLLIB_H	
 : public Ctrl 
 #endif
 {
typedef LocalProcessX CLASSNAME;
public:
	LocalProcessX() : status(STOP_OK), callbackOn(false) {}
	virtual ~LocalProcessX() 		{Stop();}
	enum ProcessStatus {RUNNING = 1, STOP_OK = 0, STOP_TIMEOUT = -1, STOP_USER = -2, STOP_NORESPONSE = -3};
	bool Start(const char *cmd, const char *envptr = 0, const char *dir = 0, double _refreshTime = -1, 
		double _maxTimeWithoutOutput = -1, double _maxRunTime = -1, bool convertcharset = true) {
		status = STOP_OK;
		p.ConvertCharset(convertcharset);
		timeElapsed.Start();
		timeWithoutOutput.Start();
		if(!p.Start(cmd, envptr, dir))
			return false;
		status = RUNNING;
		maxTimeWithoutOutput = _maxTimeWithoutOutput;
		maxRunTime = _maxRunTime;
		refreshTime = _refreshTime;
	
#ifdef CTRLLIB_H
		if (refreshTime > 0) {
			if (!callbackOn) {
				timeCallback.Set(-int(refreshTime*1000), THISBACK(Perform));
				callbackOn = true;
			}
		}
#endif
		return true;
	}
	void Perform() {
		if (status <= 0)
			return;
		String out;
		p.Read(out);
		if(p.IsRunning()) {
#ifdef PLATFORM_WIN32			
			if (!p.IsPaused()) {
#endif
				if (maxTimeWithoutOutput > 0 && timeWithoutOutput.Seconds() > maxTimeWithoutOutput) 
					status = STOP_NORESPONSE;
				else if (maxRunTime > 0 && timeElapsed.Seconds() > maxRunTime) 
					status = STOP_TIMEOUT;
#ifdef PLATFORM_WIN32				
			}
#endif
		} else 
			status = STOP_OK;
		
		bool resetTimeout = false;
		if (!out.IsEmpty())
			resetTimeout = true;
		
		if (!WhenTimer(timeElapsed.Seconds(), out, status <= 0, resetTimeout))
			status = STOP_USER;
		
		if (resetTimeout)
			timeWithoutOutput.Reset();
		
		if (status < 0)
			p.Kill();

#ifdef CTRLLIB_H		
		if (callbackOn) {
			timeCallback.Kill();
			callbackOn = false;
		}
#endif
	}
	void Stop(ProcessStatus _status = STOP_USER) {
		if (!IsRunning())
			return;
		status = _status;
		p.Kill();		
#ifdef CTRLLIB_H		
		if (callbackOn) {
			timeCallback.Kill();
			callbackOn = false;
		}
#endif		
	}
#ifdef PLATFORM_WIN32
	void Pause() {
		p.Pause();
		if (p.IsRunning()) {
			timeElapsed.Pause(p.IsPaused());
			timeWithoutOutput.Pause(p.IsPaused());
		}
	}
	bool IsPaused()			{return p.IsPaused();}
	double Seconds()		{return timeElapsed.Seconds();}
#endif
	void Write(String str) 	{p.Write(str);}
	int GetStatus()  		{return status;}
	bool IsRunning() 		{return status > 0;}
	Gate4<double, String&, bool, bool&> WhenTimer;
	#ifdef PLATFORM_WIN32
	DWORD GetPid()	{return p.GetPid();}
	#endif
	
private:
	LocalProcess2 p;
	RealTimeStop timeElapsed, timeWithoutOutput;
	ProcessStatus status;
	double maxTimeWithoutOutput, maxRunTime;
	double refreshTime;
	bool callbackOn;
#ifdef CTRLLIB_H	
	TimeCallback timeCallback;
#endif
};

int LevenshteinDistance(const char *s, const char *t);
int DamerauLevenshteinDistance(const char *s, const char *t, int alphabetLength = 256);
int SentenceSimilitude(const char *s, const char *t);

#define S(y)	Upp::String(y)

	
template<class T>
void Jsonize(JsonIO& io, std::complex<T>& var) {
	T re, im;
	if (io.IsStoring()) {
		re = var.real();
		im = var.imag();
	}
	io("re", re)("im", im);
	if (io.IsLoading()) {
		var.real(re);
		var.imag(im);
	}
}

}

#endif
