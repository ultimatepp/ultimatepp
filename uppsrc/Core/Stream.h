#ifdef  _DEBUG
#define NEWBLOCKSTREAM
#endif

enum {
	STRM_ERROR   =  0x20,
	STRM_READ    =  0x10,
	STRM_WRITE   =  0x08,
	STRM_SEEK    =  0x04,
	STRM_LOADING =  0x02,
	STRM_THROW   =  0x01,

	ERROR_NOT_ENOUGH_SPACE = -1,
	ERROR_LOADING_FAILED   = -2
};

struct StreamError {};
struct LoadingError : StreamError {};

enum EOLenum { EOL };

class Stream {
protected:
	int64  pos;
	byte  *buffer;
	byte  *ptr;
	byte  *rdlim;
	byte  *wrlim;

	unsigned style:6;
	unsigned errorcode:16;

	enum {
		BEGINOFLINE = 0x02,
	};

	virtual   void  _Put(int w);
	virtual   int   _Term();
	virtual   int   _Get();
	virtual   void  _Put(const void *data, dword size);
	virtual   dword _Get(void *data, dword size);

private:
	int       _Get8();
	int       _Get16();
	int       _Get32();
	int64     _Get64();

public:
	virtual   void  Seek(int64 pos);
	virtual   int64 GetSize() const;
	virtual   void  SetSize(int64 size);
	virtual   void  Flush();
	virtual   void  Close();
	virtual   bool  IsOpen() const = 0;

	Stream();
	virtual  ~Stream();

	word      GetStyle() const       { return style; }

	bool      IsError() const        { return style & STRM_ERROR; }
	bool      IsOK() const           { return !IsError(); }
	void      SetError(int c = 0)    { style |= STRM_ERROR; errorcode = c; }
#ifdef PLATFORM_WIN32
	void      SetLastError()         { SetError(GetLastError()); }
#endif
#ifdef PLATFORM_POSIX
	void      SetLastError()         { SetError(errno); }
#endif
	int       GetError() const       { return errorcode; }
	void      ClearError()           { style = style & ~STRM_ERROR; errorcode = 0; }

	int64     GetPos() const         { return dword(ptr - buffer) + pos; }
	int64     GetLeft() const        { return GetSize() - GetPos(); }
	void      SeekEnd(int64 rel = 0) { Seek(GetSize() + rel); }
	void      SeekCur(int64 rel)     { Seek(GetPos() + rel); }

	bool      IsEof()                { return ptr >= rdlim && _Term() < 0; }

	void      Put(int c)             { if(ptr < wrlim) *ptr++ = c; else _Put(c); }
	int       Term()                 { return ptr < rdlim ? *ptr : _Term(); }
	int       Peek()                 { return Term(); }
	int       Get()                  { return ptr < rdlim ? *ptr++ : _Get(); }

	const byte *PeekPtr(int size = 1){ ASSERT(size > 0); return ptr + size <= rdlim ? ptr : NULL; }
	const byte *GetPtr(int size = 1) { ASSERT(size > 0); if(ptr + size <= rdlim) { byte *p = ptr; ptr += size; return p; }; return NULL; }
	byte       *PutPtr(int size = 1) { ASSERT(size > 0); if(ptr + size <= wrlim) { byte *p = ptr; ptr += size; return p; }; return NULL; }
	const byte *GetSzPtr(int& size)  { Term(); size = int(rdlim - ptr); byte *p = ptr; ptr += size; return p; }

	void      Put(const void *data, int size)  { ASSERT(size >= 0); if(ptr + size <= wrlim) { memcpy(ptr, data, size); ptr += size; } else _Put(data, size); }
	int       Get(void *data, int size)        { ASSERT(size >= 0); if(ptr + size <= rdlim) { memcpy(data, ptr, size); ptr += size; return size; } return _Get(data, size); }

	void      Put(const String& s)   { Put((const char *) s, s.GetLength()); }
	String    Get(int size);
	String GetAll(int size);

	void      LoadThrowing()         { style |= STRM_THROW; }
	void      LoadError();

	bool      GetAll(void *data, int size);

	void      Put64(const void *data, int64 size);
	int64     Get64(void *data, int64 size);
	bool      GetAll64(void *data, int64 size);

	int       Get8()                 { return ptr < rdlim ? *ptr++ : _Get8(); }
#ifdef CPU_X86
	int       Get16()                { if(ptr + 1 >= rdlim) return _Get16(); int q = *(word *)ptr; ptr += 2; return q; }
	int       Get32()                { if(ptr + 3 >= rdlim) return _Get32(); int q = *(dword *)ptr; ptr += 4; return q; }
	int64     Get64()                { if(ptr + 7 >= rdlim) return _Get64(); int64 q = *(int64 *)ptr; ptr += 8; return q; }
#else
	int       Get16()                { return _Get16(); }
	int       Get32()                { return _Get32(); }
	int       Get64()                { return _Get32(); }
#endif

#ifdef CPU_LE
	int       Get16le()              { return Get16(); }
	int       Get32le()              { return Get32(); }
	int64     Get64le()              { return Get64(); }
	int       Get16be();
	int       Get32be();
	int64     Get64be();
#else
	int       Get16le();
	int       Get32le();
	int64     Get64le();
	int       Get16be()              { return Get16(); }
	int       Get32be()              { return Get32(); }
	int64     Get64be()              { return Get64(); }
#endif

	int       GetUtf8();

	String    GetLine();

#ifdef CPU_X86
	void      Put16(word q)          { if(ptr + 1 < wrlim) { *(word *)ptr = q; ptr += 2; } else Put(&q, 2); }
	void      Put32(dword q)         { if(ptr + 3 < wrlim) { *(dword *)ptr = q; ptr += 4; } else Put(&q, 4); }
	void      Put64(int64 q)         { if(ptr + 7 < wrlim) { *(int64 *)ptr = q; ptr += 8; } else Put(&q, 8); }
#else
	void      Put16(word q)          { Put(&q, 2); }
	void      Put32(dword q)         { Put(&q, 4); }
	void      Put64(int64 q)         { Put(&q, 8); }
#endif

#ifdef CPU_LE
	void      Put16le(word q)        { Put(&q, 2); }
	void      Put32le(dword q)       { Put(&q, 4); }
	void      Put64le(int64 q)       { Put(&q, 8); }
	void      Put16be(word q);
	void      Put32be(dword q);
	void      Put64be(int64 q);
#else
	void      Put16le(word q);
	void      Put32le(dword q);
	void      Put64le(int64 q);
	void      Put16be(word q)        { Put(&q, 2); }
	void      Put32be(dword q)       { Put(&q, 4); }
	void      Put64be(int64 q)       { Put(&q, 8); }
#endif

	void      PutUtf8(int c);

	void      Put(const char *s);
	void      Put(int c, int count);
	void      Put0(int count)        { Put(0, count); }

	void      PutCrLf()              { Put16(MAKEWORD('\r', '\n')); }
#ifdef PLATFORM_WIN32
	void      PutEol()               { PutCrLf(); }
#else
	void      PutEol()               { Put('\n'); }
#endif

	Stream&   operator<<(EOLenum)    { PutEol(); return *this; }

	void      PutLine(const char *s);
	void      PutLine(const String& s);

	void      PutW(const wchar *s, int count)       { Put(s, count * 2); }
	dword     GetW(wchar *s, int count)             { return Get(s, count * 2) / 2; }
	bool      GetAllW(wchar *s, int count)          { return GetAll(s, count * 2); }

	void      Put(Stream& s, int64 size = INT64_MAX, dword click = 4096);

//  Stream as serialization streamer
	void      SetLoading()                 { ASSERT(style & STRM_READ); style |= STRM_LOADING; }
	void      SetStoring()                 { ASSERT(style & STRM_WRITE); style &= ~STRM_LOADING; }
	bool      IsLoading() const            { return style & STRM_LOADING; }
	bool      IsStoring() const            { return !IsLoading(); }

	void      SerializeRaw(byte *data, int count);
	void      SerializeRaw(word *data, int count);
	void      SerializeRaw(dword *data, int count);
	void      SerializeRaw(uint64 *data, int count);

	String    GetAllRLE(int size);
	void      SerializeRLE(byte *data, int count);

	Stream&   operator%(bool& d);
	Stream&   operator%(char& d);
	Stream&   operator%(signed char& d);
	Stream&   operator%(unsigned char& d);
	Stream&   operator%(short& d);
	Stream&   operator%(unsigned short& d);
	Stream&   operator%(int& d);
	Stream&   operator%(unsigned int& d);
	Stream&   operator%(long& d);
	Stream&   operator%(unsigned long& d);
	Stream&   operator%(float& d);
	Stream&   operator%(double& d);
	Stream&   operator%(int64& d);
	Stream&   operator%(uint64& d);

	Stream&   operator%(String& s);
	Stream&   operator/(String& s);

	Stream&   operator%(WString& s);
	Stream&   operator/(WString& s);

	void      Pack(dword& i);
	Stream&   operator/(int& i);
	Stream&   operator/(unsigned int& i);
	Stream&   operator/(long& i);
	Stream&   operator/(unsigned long& i);

	void      Magic(dword magic = 0x7d674d7b);

	void      Pack(bool& a, bool& b, bool& c, bool& d, bool& e, bool& f, bool& g, bool& h);
	void      Pack(bool& a, bool& b, bool& c, bool& d, bool& e, bool& f, bool& g);
	void      Pack(bool& a, bool& b, bool& c, bool& d, bool& e, bool& f);
	void      Pack(bool& a, bool& b, bool& c, bool& d, bool& e);
	void      Pack(bool& a, bool& b, bool& c, bool& d);
	void      Pack(bool& a, bool& b, bool& c);
	void      Pack(bool& a, bool& b);

//* deprecated
	int       GetW()                 { return Get16(); }
	int       GetL()                 { return Get32(); }
	int       GetIW()                { return Get16le(); }
	int       GetIL()                { return Get32le(); }
	int       GetMW()                { return Get16be(); }
	int       GetML()                { return Get32be(); }
	void      PutW(int c)            { Put16(c); }
	void      PutL(int c)            { Put32(c); }
	void      PutIW(int c)           { Put16le(c); }
	void      PutIL(int c)           { Put32le(c); }
	void      PutMW(int c)           { Put16be(c); }
	void      PutML(int c)           { Put32be(c); }
//*/
private: // No copy
	Stream(const Stream& s);
	void operator=(const Stream& s);
};

class StringStream : public Stream {
protected:
	virtual  void  _Put(int w);
	virtual  int   _Term();
	virtual  int   _Get();
	virtual  void  _Put(const void *data, dword size);
	virtual  dword _Get(void *data, dword size);

public:
	virtual  void  Seek(int64 pos);
	virtual  int64 GetSize() const;
	virtual  void  SetSize(int64 size);
	virtual  bool  IsOpen() const;

protected:
	bool           writemode;
	String         data;
	StringBuffer   wdata;
	dword          size;

	void           InitReadMode();
	void           SetWriteBuffer();
	void           SetReadMode();
	void           SetWriteMode();

public:
	void        Open(const String& data);
	void        Create();
	void        Reserve(int n);

	String      GetResult();
	operator    String()                     { return GetResult(); }

	StringStream()                           { Create(); }
	StringStream(const String& data)         { Open(data); }
};

class MemStream : public Stream {
protected:
	virtual   void  _Put(const void *data, dword size);
	virtual   dword _Get(void *data, dword size);

public:
	virtual   void  Seek(int64 pos);
	virtual   int64 GetSize() const;
	virtual   bool  IsOpen() const;

public:
	void Create(void *data, int64 size);

	MemStream();
	MemStream(void *data, int64 size);
#ifdef flagSO
	virtual ~MemStream();
#endif
};

class MemReadStream : public MemStream {
public:
	void Create(const void *data, int64 size);

	MemReadStream(const void *data, int64 size);
	MemReadStream();
};

class BlockStream : public Stream {
protected:
	virtual  void  _Put(int w);
	virtual  int   _Term();
	virtual  int   _Get();
	virtual  void  _Put(const void *data, dword size);
	virtual  dword _Get(void *data, dword size);

public:
	virtual  void  Seek(int64 pos);
	virtual  int64 GetSize() const;
	virtual  void  SetSize(int64 size);
	virtual  void  Flush();

private:
	int           pagesize;
	int64         pagemask;
	int64         pagepos;
	bool          pagedirty;

	int64         streamsize;

	void          SetPos(int64 p);
	void          SyncSize();
	bool          SyncPage();
	bool          SyncPos();
	void          ReadData(void *data, int64 at, int size);

protected:
	virtual  dword Read(int64 at, void *ptr, dword size);
	virtual  void  Write(int64 at, const void *data, dword size);
	virtual  void  SetStreamSize(int64 size);

public:
	enum {
		READ, CREATE, APPEND, READWRITE,

		NOWRITESHARE = 0x10,
		DELETESHARE = 0x20, // deprecated
		NOREADSHARE = 0x40, // deprecated
		SHAREMASK = 0x70,
	};
//	typedef int OpenMode; // obsolete, use dword

	dword     GetBufferSize() const           { return pagesize; }
	void      SetBufferSize(dword newsize);
	int64     GetStreamSize() const           { return streamsize; }

	BlockStream();
	virtual ~BlockStream();

protected:
	void     OpenInit(dword mode, int64 file_size);
};

class FileStream : public BlockStream {
protected:
	virtual  void  SetStreamSize(int64 size);
	virtual  dword Read(int64 at, void *ptr, dword size);
	virtual  void  Write(int64 at, const void *data, dword size);

public:
	virtual  void  Close();
	virtual  bool  IsOpen() const;

protected:
#ifdef PLATFORM_WIN32
	HANDLE    handle;
#endif
#ifdef PLATFORM_POSIX
	int       handle;
#endif

	void      SetPos(int64 pos);
	void      Init(int64 size);

public:
	operator  bool() const                 { return IsOpen(); }
	FileTime  GetTime() const;

#ifdef PLATFORM_WIN32
	void      SetTime(const FileTime& tm);
	bool      Open(const char *filename, dword mode);

	FileStream(const char *filename, dword mode);
#endif

#ifdef PLATFORM_POSIX
	bool      Open(const char *filename, dword mode, mode_t acm = 0644);
	FileStream(const char *filename, dword mode, mode_t acm = 0644);
	FileStream(int std_handle);
#endif

	FileStream();
	~FileStream();

#ifdef PLATFORM_WIN32
	HANDLE    GetHandle() const            { return handle; }
#endif
#ifdef PLATFORM_POSIX
	int       GetHandle() const            { return handle; }
#endif
};

class FileOut : public FileStream {
public:
#ifdef PLATFORM_POSIX
	bool Open(const char *fn, mode_t acm = 0644);
#endif
#ifdef PLATFORM_WIN32
	bool Open(const char *fn);
#endif

	FileOut(const char *fn)                { Open(fn); }
	FileOut()                              {}
};

class FileAppend : public FileStream {
public:
	bool Open(const char *fn)         { return FileStream::Open(fn, FileStream::APPEND|FileStream::NOWRITESHARE); }

	FileAppend(const char *fn)        { Open(fn); }
	FileAppend()                      {}
};

class FileIn : public FileStream {
public:
	bool Open(const char *fn)         { return FileStream::Open(fn, FileStream::READ); }

	FileIn(const char *fn)            { Open(fn); }
	FileIn()                          {}
};

class SizeStream : public Stream {
protected:
	virtual void  _Put(int w);
	virtual void  _Put(const void *data, dword size);

public:
	virtual void  Seek(int64 pos);
	virtual int64 GetSize() const;
	virtual void  SetSize(int64 size);
	virtual bool  IsOpen() const;

protected:
	int64   size;
	byte    h[128];

public:
	operator int64() const            { return GetSize(); }

	void     Open()                   { ptr = buffer; size = 0; ClearError(); }

	SizeStream();
};

class CompareStream : public Stream {
protected:
	virtual  void  _Put(int w);
	virtual  void  _Put(const void *data, dword size);

public:
	virtual  void  Seek(int64 pos);
	virtual  int64 GetSize() const;
	virtual  void  SetSize(int64 size);
	virtual  void  Close();
	virtual  bool  IsOpen() const;
	virtual  void  Flush();

private:
	Stream  *stream;
	bool     equal;
	int64    size;
	byte     h[128];

	void     Compare(int64 pos, const void *data, int size);

public:
	void     Open(Stream& aStream);

	bool     IsEqual()                         { Flush(); return equal; }
	operator bool()                            { Flush(); return equal; }

	CompareStream();
	CompareStream(Stream& aStream);
};

class OutStream : public Stream {
	byte     h[128];
protected:
	virtual  void  _Put(int w);
	virtual  void  _Put(const void *data, dword size);
	virtual  bool  IsOpen() const;
	
	virtual  void  Out(const void *data, dword size) = 0;
	
	void     Flush();
	
	OutStream();
};

class TeeStream : public OutStream {
protected:
	virtual  void  Out(const void *data, dword size);

private:
	Stream& a;
	Stream& b;

public:
	void    Close()                              { Flush(); }

	TeeStream(Stream& a, Stream& b) : a(a), b(b) {}
	~TeeStream()                                 { Close(); }
};

class FileMapping
{
public:
	FileMapping(const char *file = NULL, bool delete_share = false);
	~FileMapping() { Close(); }

	bool        Open(const char *file, bool delete_share = false);
	bool        Create(const char *file, int64 filesize, bool delete_share = false);
	bool        Expand(int64 filesize);
	bool        Map(int64 offset, dword len);
	bool        Unmap();
	bool        Close();

	bool        IsOpen() const            { return hfile != INVALID_HANDLE_VALUE; }

	int64       GetFileSize() const       { return filesize; }
	Time        GetTime() const;
	String      GetData(int64 offset, dword len);

	int64       GetOffset() const         { return offset; }
	dword       GetCount() const          { return size; }

	int64       GetRawOffset() const      { return rawoffset; }
	dword       GetRawCount() const       { return rawsize; }

	const byte *operator ~ () const       { ASSERT(IsOpen()); return base; }
	const byte *Begin() const             { ASSERT(IsOpen()); return base; }
	const byte *End() const               { ASSERT(IsOpen()); return base + size; }
	const byte *GetIter(int i) const      { ASSERT(IsOpen() && i >= 0 && (dword)i <= size); return base + i; }
	const byte& operator [] (int i) const { ASSERT(IsOpen() && i >= 0 && (dword)i < size); return base[i]; }

	byte       *operator ~ ()             { ASSERT(IsOpen()); return base; }
	byte       *Begin()                   { ASSERT(IsOpen()); return base; }
	byte       *End()                     { ASSERT(IsOpen()); return base + size; }
	byte       *GetIter(int i)            { ASSERT(IsOpen() && i >= 0 && (dword)i <= size); return base + i; }
	byte&       operator [] (int i)       { ASSERT(IsOpen() && i >= 0 && (dword)i < size); return base[i]; }

private:
#ifdef PLATFORM_WIN32
	HANDLE      hfile;
	HANDLE      hmap;
#endif
#ifdef PLATFORM_POSIX
	enum { INVALID_HANDLE_VALUE = -1 };
	int	        hfile;
	struct stat hfstat;
#endif
	byte       *base;
	byte       *rawbase;
	int64       filesize;
	int64       offset;
	int64       rawoffset;
	dword       size;
	dword       rawsize;
	bool        write;
};


String LoadStream(Stream& in);
bool   SaveStream(Stream& out, const String& data);

int64        CopyStream(Stream& dest, Stream& src, int64 count);
inline int64 CopyStream(Stream& dest, Stream& src) { return CopyStream(dest, src, src.GetLeft()); }

#ifndef PLATFORM_WINCE
void    CoutUTF8();
Stream& Cout();
Stream& Cerr();
String  ReadStdIn();
#endif

Stream& NilStream();

String LoadFile(const char *filename);
bool   SaveFile(const char *filename, const String& data);

template <class T>
inline Stream& operator%(Stream& s, T& x)
{
	x.Serialize(s);
	return s;
}

inline Stream& operator<<(Stream& s, const char *x)
{
	s.Put(x);
	return s;
}

inline Stream& operator<<(Stream& s, char *x)
{
	s.Put(x);
	return s;
}

inline Stream& operator<<(Stream& s, const String &x)
{
	s.Put(x);
	return s;
}

inline Stream& operator<<(Stream& s, char x)
{
	s.Put((int) x);
	return s;
}

inline Stream& operator<<(Stream& s, const void *x)
{
	s << FormatPtr(x);
	return s;
}

inline Stream& operator<<(Stream& s, void *x)
{
	s << FormatPtr(x);
	return s;
}

template <class T>
inline Stream& operator<<(Stream& s, const T& x) {
	s << AsString(x);
	return s;
}

// ------

Stream& Pack16(Stream& s, int& i);
Stream& Pack16(Stream& s, int& i1, int& i2);
Stream& Pack16(Stream& s, int& i1, int& i2, int& i3);
Stream& Pack16(Stream& s, int& i1, int& i2, int& i3, int& i4);
Stream& Pack16(Stream& s, int& i1, int& i2, int& i3, int& i4, int& i5);

int     StreamHeading(Stream& stream, int ver, int minver, int maxver, const char* tag);
