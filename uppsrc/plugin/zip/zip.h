#ifndef _zip_zip_h
#define _zip_zip_h

#include <Core/Core.h>

NAMESPACE_UPP

class UnZip {
	struct File : Moveable<File> {
		word   bit;
		String path;
		dword  time;
		int    method;
		dword  crc;
		dword  csize;
		dword  usize;
		int64  offset;
	};
	
	Stream      *zip;
	bool         error;
	Vector<File> file;
	int          current;

	void   ReadDir();

	static Time   GetZipTime(dword time);

public:
	bool   IsEof() const          { return current >= file.GetCount(); }
	operator bool() const         { return !IsEof() && !IsError(); }
	
	bool   IsError() const        { return error; }
	void   ClearError()           { error = false; }

	int    GetCount() const       { return file.GetCount(); }
	String GetPath(int i) const   { return file[i].path; }
	bool   IsFolder(int i) const  { return *file[i].path.Last() == '/'; }
	int    GetLength(int i) const { return file[i].usize; }
	Time   GetTime(int i) const   { return GetZipTime(file[i].time); }

	void   Seek(int i)            { ASSERT(i >= 0 && i < file.GetCount()); current = i; }

	bool   IsFolder() const       { return IsFolder(current); }
	String GetPath() const        { return GetPath(current); }
	int    GetLength() const      { return GetLength(current); }
	Time   GetTime() const        { return GetTime(current); }

	void   Skip()                 { current++; }
	void   SkipFile()             { current++; }
	bool   ReadFile(Stream& out, Gate2<int, int> progress = false);
	String ReadFile(Gate2<int, int> progress = false);
	
	dword  GetPos() const;

	void   Create(Stream& in);

	UnZip(Stream& in);
	UnZip();
	virtual ~UnZip();
};

class FileUnZip : public UnZip {
	FileIn zip;

public:
	bool Create(const char *name);
	
	FileUnZip(const char *name)                 { Create(name); }
};

class MemUnZip : public UnZip {
	MemReadStream zip;

public:
	void Create(const void *ptr, int count);

	MemUnZip(const void *ptr, int count)   { Create(ptr, count); }
};

class StringUnZip : public UnZip {
	StringStream zip;

public:
	void Create(const String& s);

	StringUnZip(const String& s)                { Create(s); }
};

class Zip {
	Stream *zip;

	struct File {
		String path;
		dword  time;
		int    version;
		int    gpflag;
		int    method;
		dword  crc;
		dword  csize;
		dword  usize;
	};
	Array<File> file;

	dword   done;

	One<Zlib> pipeZLib;

	void WriteFile0(const void *ptr, int size, const char *path, Gate2<int, int> progress, Time tm, int method);

	void FileHeader(const char *path, Time tm);

	void PutCompressed(const void *data, int size);
	
	typedef Zip CLASSNAME;

public:
	Callback WhenError;

	void BeginFile(const char *path, Time tm = GetSysTime());
	void BeginFile(OutFilterStream& oz, const char *path, Time tm = GetSysTime());
	void Put(const void *data, int size);
	void EndFile();
	bool IsFileOpened() const                 { return pipeZLib; }

	void WriteFolder(const char *path, Time tm);
	void WriteFile(const void *ptr, int size, const char *path, Gate2<int, int> progress = false, Time tm = GetSysTime());
	void WriteFile(const String& s, const char *path, Gate2<int, int> progress = false, Time tm = GetSysTime());

	void Create(Stream& out);
	void Finish();
	
	bool IsError()                           { return zip && zip->IsError(); }

	dword  GetLength() const                 { return done; }
	
	Zip();
	Zip(Stream& out);
	virtual ~Zip();
};

class FileZip : public Zip {
	FileOut zip;

public:
	bool Create(const char *name);
	bool Finish();
	
	FileZip(const char *name)                 { Create(name); }
	~FileZip()                                { Finish(); }
};

class StringZip : public Zip {
	StringStream zip;

public:
	void   Create();
	String Finish();

	StringZip()                               { Create(); }
};

END_UPP_NAMESPACE

#endif
