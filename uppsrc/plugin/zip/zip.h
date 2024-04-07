#ifndef _zip_zip_h
#define _zip_zip_h

#include <Core/Core.h>

namespace Upp {

class UnZip {
	struct File : Moveable<File> {
		word   bit;
		String path;
		dword  time;
		int    method;
		dword  crc;
		qword  csize;
		qword  usize;
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
	bool   IsFile(int i) const    { return !IsFolder(i); }
	int64  GetLength(int i) const { return file[i].usize; }
	Time   GetTime(int i) const   { return GetZipTime(file[i].time); }

	void   Seek(int i)            { ASSERT(i >= 0 && i < file.GetCount()); current = i; }

	bool   IsFolder() const       { return IsFolder(current); }
	bool   IsFile() const         { return !IsFolder(); }
	String GetPath() const        { return GetPath(current); }
	int64  GetLength() const      { return GetLength(current); }
	Time   GetTime() const        { return GetTime(current); }

	void   Skip()                 { current++; }
	void   SkipFile()             { current++; }
	bool   ReadFile(Stream& out, Gate<int, int> progress = Null);
	String ReadFile(Gate<int, int> progress = Null);

	String ReadFile(const char *path, Gate<int, int> progress = Null);

	void   Create(Stream& in);
	void   Close()                { file.Clear(); zip->Close(); }

	UnZip(Stream& in);
	UnZip();
	virtual ~UnZip();
};

class FileUnZip : public UnZip {
	FileIn zip;

public:
	bool Create(const char *name);
	
	FileUnZip(const char *name)                 { Create(name); }
	FileUnZip()                                 {}
};

class MemUnZip : public UnZip {
	MemReadStream zip;

public:
	void Create(const void *ptr, int count);

	MemUnZip(const void *ptr, int count)       { Create(ptr, count); }
	MemUnZip();
};

class StringUnZip : public UnZip {
	StringStream zip;

public:
	void Create(const String& s);

	StringUnZip(const String& s)                { Create(s); }
	StringUnZip();
};

class Zip {
	Stream *zip;

	struct File {
		String path;
		dword  time;
		int    version;
		int    gpflag;
		int    method;
		bool   zip64;
		dword  crc;
		qword  csize;
		qword  usize;
	};
	
	Array<File> file;

	qword   done;

	One<Zlib> pipeZLib;
	Crc32Stream crc32; // for uncompressed files
	bool        uncompressed;

	void WriteFile0(const void *ptr, int size, const char *path, Gate<int, int> progress, Time tm, int method);

	void FileHeader(const char *path, Time tm);

	void PutCompressed(const void *data, int size);
	
	typedef Zip CLASSNAME;

public:
	Callback WhenError;

	void BeginFile(const char *path, Time tm = GetSysTime(), bool deflate = true, bool zip64 = false);
	void BeginFile(OutFilterStream& oz, const char *path, Time tm = GetSysTime(), bool deflate = true, bool zip64 = false);
	void Put(const void *data, int size);
	void Put64(const void *data, int64 size);
	void EndFile();
	bool IsFileOpened() const                 { return pipeZLib || uncompressed; }

	void WriteFolder(const char *path, Time tm = GetSysTime());
	void WriteFile(const void *ptr, int size, const char *path, Gate<int, int> progress = Null, Time tm = GetSysTime(), bool deflate = true);
	void WriteFile(const String& s, const char *path, Gate<int, int> progress = Null, Time tm = GetSysTime(), bool deflate = true);

	void Create(Stream& out);
	void Finish();
	
	bool IsError()                           { return zip && zip->IsError(); }

	qword  GetLength() const                 { return done; }
	
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
	FileZip()                                 {}
	~FileZip()                                { Finish(); }
};

class StringZip : public Zip {
	StringStream zip;

public:
	void   Create();
	String Finish();

	StringZip()                               { Create(); }
};

}

#endif
