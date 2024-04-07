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

class Huge;

class Stream {
protected:
	int64  pos;
	byte  *buffer;
	byte  *ptr;
	byte  *rdlim;
	byte  *wrlim;

	unsigned style:6;
	unsigned errorcode:16;
	
	int      version = 0;

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
	
	void      SetVersion(int ver)    { version = ver; }
	int       GetVersion() const     { return version; }

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
	String    GetErrorText() const;
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

	void      Put(const void *data, int size)  { ASSERT(size >= 0); if(size) { if(ptr + size <= wrlim) { memcpy8(ptr, data, size); ptr += size; } else _Put(data, size); } }
	int       Get(void *data, int size)        { ASSERT(size >= 0); if(ptr + size <= rdlim) { memcpy8(data, ptr, size); ptr += size; return size; } return _Get(data, size); }

	void      Put(const String& s)   { Put((const char *) s, s.GetLength()); }
	String    Get(int size);
	String    GetAll(int size);
	
	int       Skip(int size);
	
	void      LoadThrowing()         { style |= STRM_THROW; }
	void      LoadError();

	bool      GetAll(void *data, int size);

	void      Put64(const void *data, int64 size);
	int64     Get64(void *data, int64 size);
	bool      GetAll64(void *data, int64 size);

	size_t    Get(Huge& h, size_t size);
	bool      GetAll(Huge& h, size_t size);

	int       Get8()                 { return ptr < rdlim ? *ptr++ : _Get8(); }
	int       Get16()                { if(ptr + 1 >= rdlim) return _Get16(); int q = Peek16(ptr); ptr += 2; return q; }
	int       Get32()                { if(ptr + 3 >= rdlim) return _Get32(); int q = Peek32(ptr); ptr += 4; return q; }
	int64     Get64()                { if(ptr + 7 >= rdlim) return _Get64(); int64 q = Peek64(ptr); ptr += 8; return q; }

	int       GetUtf8();

	String    GetLine();

	void      Put16(word q)          { if(ptr + 1 < wrlim) { Poke16(ptr, q); ptr += 2; } else Put(&q, 2); }
	void      Put32(dword q)         { if(ptr + 3 < wrlim) { Poke32(ptr, q); ptr += 4; } else Put(&q, 4); }
	void      Put64(int64 q)         { if(ptr + 7 < wrlim) { Poke64(ptr, q); ptr += 8; } else Put(&q, 8); }

#ifdef CPU_LE
	int       Get16le()              { return Get16(); }
	int       Get32le()              { return Get32(); }
	int64     Get64le()              { return Get64(); }
	int       Get16be()              { return SwapEndian16(Get16()); }
	int       Get32be()              { return SwapEndian32(Get32()); }
	int64     Get64be()              { return SwapEndian64(Get64()); }

	void      Put16le(word q)        { Put16(q); }
	void      Put32le(dword q)       { Put32(q); }
	void      Put64le(int64 q)       { Put64(q); }
	void      Put16be(word q)        { Put16(SwapEndian16(q)); }
	void      Put32be(dword q)       { Put32(SwapEndian32(q)); }
	void      Put64be(int64 q)       { Put64(SwapEndian64(q)); }
#else
	int       Get16le()              { return SwapEndian16(Get16()); }
	int       Get32le()              { return SwapEndian32(Get32()); }
	int64     Get64le()              { return SwapEndian64(Get64()); }
	int       Get16be()              { return Get16(); }
	int       Get32be()              { return Get32(); }
	int64     Get64be()              { return Get64(); }

	void      Put16le(word q)        { Put16(SwapEndian16(q)); }
	void      Put32le(dword q)       { Put32(SwapEndian32(q)); }
	void      Put64le(int64 q)       { Put64(SwapEndian64(q)); }
	void      Put16be(word q)        { Put16(q); }
	void      Put32be(dword q)       { Put32(q); }
	void      Put64be(int64 q)       { Put64(q); }
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

	void      Put(Stream& s, int64 size = INT64_MAX, dword click = 4096);

//  Stream as serialization streamer
	void      SetLoading()                 { ASSERT(style & STRM_READ); style |= STRM_LOADING; }
	void      SetStoring()                 { ASSERT(style & STRM_WRITE); style &= ~STRM_LOADING; }
	bool      IsLoading() const            { return style & STRM_LOADING; }
	bool      IsStoring() const            { return !IsLoading(); }

	void      SerializeRaw(byte *data, int64 count);
	void      SerializeRaw(word *data, int64 count);
	void      SerializeRaw(int16 *data, int64 count);
	void      SerializeRaw(dword *data, int64 count);
	void      SerializeRaw(int *data, int64 count);
	void      SerializeRaw(uint64 *data, int64 count);
	void      SerializeRaw(float *data, int64 count);
	void      SerializeRaw(double *data, int64 count);

	String    GetAllRLE(int size);
	void      SerializeRLE(byte *data, int count);
	
	Stream&   SerializeRaw(byte *data)     { if(IsLoading()) *data = Get(); else Put(*data); return *this; }
	Stream&   SerializeRaw(word *data)     { if(IsLoading()) *data = Get16le(); else Put16le(*data); return *this; }
	Stream&   SerializeRaw(dword *data)    { if(IsLoading()) *data = Get32le(); else Put32le(*data); return *this; }
	Stream&   SerializeRaw(uint64 *data)   { if(IsLoading()) *data = Get64le(); else Put64le(*data); return *this; }

	Stream&   operator%(bool& d)           { byte b; if(IsStoring()) b = d; SerializeRaw(&b); d = b; return *this; }
	Stream&   operator%(char& d)           { return SerializeRaw((byte *)&d); }
	Stream&   operator%(signed char& d)    { return SerializeRaw((byte *)&d); }
	Stream&   operator%(unsigned char& d)  { return SerializeRaw((byte *)&d); }
	Stream&   operator%(short& d)          { return SerializeRaw((word *)&d); }
	Stream&   operator%(unsigned short& d) { return SerializeRaw((word *)&d); }
	Stream&   operator%(int& d)            { return SerializeRaw((dword *)&d); }
	Stream&   operator%(unsigned int& d)   { return SerializeRaw((dword *)&d); }
	Stream&   operator%(long& d)           { return SerializeRaw((dword *)&d); }
	Stream&   operator%(unsigned long& d)  { return SerializeRaw((dword *)&d); }
	Stream&   operator%(float& d)          { return SerializeRaw((dword *)&d); }
	Stream&   operator%(double& d)         { return SerializeRaw((uint64 *)&d); }
	Stream&   operator%(int64& d)          { return SerializeRaw((uint64 *)&d); }
	Stream&   operator%(uint64& d)         { return SerializeRaw((uint64 *)&d); }

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

#ifdef DEPRECATED
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
	void      PutW(const char16 *s, int count)       { Put(s, count * 2); }
	dword     GetW(char16 *s, int count)             { return Get(s, count * 2) / 2; }
	bool      GetAllW(char16 *s, int count)          { return GetAll(s, count * 2); }
#endif
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
	int            limit = INT_MAX;

	void           InitReadMode();
	void           SetWriteBuffer();
	void           SetReadMode();
	void           SetWriteMode();

public:
	void        Open(const String& data);
	void        Create();
	void        Reserve(int n);

	String      GetResult();
	operator    String()                              { return GetResult(); }
	
	void        Limit(int sz)                         { limit = sz; }
	
	struct LimitExc : public StreamError {};

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
		SHAREMASK = 0x70,
#ifdef DEPRECATED
		DELETESHARE = 0x20,
		NOREADSHARE = 0x40,
#endif
	};

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
	static bool OpenHandle(const char *name, dword mode, HANDLE& handle, int64& fsz);
#else
	int       handle;
	static bool OpenHandle(const char *name, dword mode, int& handle, int64& fsz, mode_t tmode);
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
	virtual int64 GetSize() const;
	virtual bool  IsOpen() const;

protected:
	byte    h[256];

public:
	operator int64() const            { return GetSize(); }

	void     Open()                   { ptr = buffer; ClearError(); }

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
	byte     h[1024];

	void     Compare(int64 pos, const void *data, int size);

public:
	void     Open(Stream& aStream);

	bool     IsEqual()                         { Flush(); return equal; }
	operator bool()                            { Flush(); return equal; }

	CompareStream();
	CompareStream(Stream& aStream);
};

class OutStream : public Stream {
	byte *h;

protected:
	virtual  void  _Put(int w);
	virtual  void  _Put(const void *data, dword size);
	virtual  bool  IsOpen() const;

	virtual  void  Out(const void *data, dword size) = 0;

public:
	virtual  void  Close();
	
	void     Flush();
	
	OutStream();
	~OutStream();
};

class TeeStream : public OutStream {
protected:
	virtual  void  Out(const void *data, dword size);

private:
	Stream& a;
	Stream& b;

public:
	TeeStream(Stream& a, Stream& b) : a(a), b(b) {}
	~TeeStream()                                 { Close(); }
};

class FileMapping {
public:
	FileMapping(const char *file = NULL);
	~FileMapping() { Close(); }

	bool        Open(const char *file);
	bool        Create(const char *file, int64 filesize, bool delete_share = false);

	bool        Expand(int64 filesize);
	byte       *Map(int64 mapoffset, size_t maplen);
	byte       *Map()                     { return Map(0, GetFileSize()); }
	bool        Unmap();
	bool        Close();

	bool        IsOpen() const            { return hfile != INVALID_HANDLE_VALUE; }

	int64       GetFileSize() const       { return filesize; }
	Time        GetTime() const;
	String      GetData(int64 offset, int len);

	int64       GetOffset() const         { return offset; }
	size_t      GetCount() const          { return size; }

	int64       GetRawOffset() const      { return rawoffset; }
	size_t      GetRawCount() const       { return rawsize; }

	const byte *operator ~ () const       { ASSERT(IsOpen()); return base; }
	const byte *begin() const             { ASSERT(IsOpen()); return base; }
	const byte *end() const               { ASSERT(IsOpen()); return base + size; }
	const byte& operator [] (int i) const { ASSERT(IsOpen() && i >= 0 && (size_t)i < size); return base[i]; }

	byte       *operator ~ ()             { ASSERT(IsOpen()); return base; }
	byte       *begin()                   { ASSERT(IsOpen()); return base; }
	byte       *end()                     { ASSERT(IsOpen()); return base + size; }
	byte&       operator [] (int i)       { ASSERT(IsOpen() && i >= 0 && (size_t)i < size); return base[i]; }

	const byte *Begin() const             { ASSERT(IsOpen()); return base; }
	const byte *End() const               { ASSERT(IsOpen()); return base + size; }
	const byte *GetIter(int i) const      { ASSERT(IsOpen() && i >= 0 && (size_t)i <= size); return base + i; }

	byte       *Begin()                   { ASSERT(IsOpen()); return base; }
	byte       *End()                     { ASSERT(IsOpen()); return base + size; }
	byte       *GetIter(int i)            { ASSERT(IsOpen() && i >= 0 && (size_t)i <= size); return base + i; }

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
	size_t      size;
	size_t      rawsize;
	bool        write;

	static int MappingGranularity();
};


String LoadStream(Stream& in);
bool   SaveStream(Stream& out, const String& data);

int64 CopyStream(Stream& dest, Stream& src, int64 count = INT64_MAX);

#ifndef PLATFORM_WINCE
void    CoutUTF8();
Stream& Cout();
Stream& Cerr();
String  ReadStdIn();
String  ReadSecret();
void    EnableEcho(bool b = true);
void    DisableEcho();
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
