#ifndef _zip_zip_h
#define _zip_zip_h

#include <Core/Core.h>

NAMESPACE_UPP

class UnZip {
	Stream *zip;
	
	bool   error;
	bool   eof;
	String path;
	word   bit;
	word   method;
	Time   time;
	dword  crc32;
	dword  csize;
	dword  usize;
	dword  done;

	void   Init();
	void   ReadHeader();
	void   SetError()           { error = true; }

public:
	bool   IsEof() const;
	operator bool() const       { return !IsEof(); }
	
	bool   IsError() const      { return error; }

	bool   IsFolder() const;
	String GetPath() const      { return path; }
	int    GetLength() const    { return usize; }
	Time   GetTime() const      { return time; }

	void   SkipFile();
	bool   ReadFile(Stream& out, Gate2<int, int> progress = false);
	String ReadFile(Gate2<int, int> progress = false);
	
	dword  GetPos()             { return done; }

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
		int    method;
		dword  crc;
		dword  csize;
		dword  usize;
	};
	Array<File> file;

	dword   done;

	void WriteFile0(const void *ptr, int size, const char *path, Gate2<int, int> progress, Time tm, int method);

public:
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
