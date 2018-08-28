class Crc32Stream : public OutStream {
	dword crc;

	virtual  void  Out(const void *data, dword size);

public:
	dword  Finish()            { Flush(); return crc; }
	operator dword()           { return Finish(); }
	void   Clear();
	
	Crc32Stream()              { Clear(); }
};

dword CRC32(const void *ptr, dword count);
dword CRC32(const String& s);

class Zlib {
	enum { NONE, DEFLATE, INFLATE };

	z_stream      z;
	Buffer<Bytef> output;
	int           chunk;
	int           mode;
	dword         crc;
	int           total;
	int           compression_level;
	bool          docrc;
	bool          hdr;
	bool          error;
	bool          gzip;
	bool          gzip_header_done;
	bool          gzip_footer;
	String        footer;
	String        gzip_hs;
	String        gzip_name;
	String        gzip_comment;
	String        out;
	
	void          PutOut(const void *ptr, int size);
	void          Pump(bool finish);
	void          Begin();
	void          Free();
	void          Put0(const char *ptr, int size);
	int           GzipHeader(const char *ptr, int size);
	void          Init();

public:
	Event<const void *, int>  WhenOut;
	
	void Compress();
	void Decompress();
	void Put(const void *ptr, int size);
	void Put(const String& s)              { Put(~s, s.GetCount()); }
	void End();
	void Clear();
	
	const String& Get() const              { return out; }
	operator const String&() const         { return out; }
	const String& operator~() const        { return out; }
	void   ClearOut()                      { out.Clear(); }

	dword  GetCRC() const                  { return crc; }
	bool   IsError() const                 { return error; }
	String GetGZipName() const             { return gzip_name; }
	String GetGZipComment() const          { return gzip_comment; }
	
	Zlib& GZip(bool gzip_ = true)          { gzip = gzip_; return *this; }
	Zlib& Header(bool hdr_ = true)         { hdr = hdr_; return *this; }
	Zlib& NoHeader()                       { return Header(false); }
	Zlib& CRC(bool b = true)               { docrc = b; return *this; }
	Zlib& NoCRC()                          { return CRC(false); }
	Zlib& ChunkSize(int n);
	Zlib& Level(int compression_lvl)       { compression_level = compression_lvl; return *this; }

	Zlib();
	~Zlib();
};

class ZCompressStream : public OutFilterStream {
	Zlib         z;

public:
	void Open(Stream& out)                 { z.Compress(); Set(out, z); }

	dword  GetCRC() const                  { return z.GetCRC(); }
	
	Zlib& GZip(bool gzip_ = true)          { return z.GZip(); }
	Zlib& Header(bool hdr_ = true)         { return z.Header(); }
	Zlib& NoHeader()                       { return Header(false); }
	Zlib& CRC(bool b = true)               { return z.CRC(); }
	Zlib& NoCRC()                          { return CRC(false); }
	Zlib& ChunkSize(int n)                 { return z.ChunkSize(n); }
	Zlib& Level(int compression_lvl)       { return z.Level(compression_lvl); }

	ZCompressStream()                      {}
	ZCompressStream(Stream& out)           { Open(out); }
	~ZCompressStream()                     { Close(); }
};

class ZDecompressStream : public InFilterStream {
	Zlib         z;

public:
	void Open(Stream& in)                  { z.Decompress(); Set(in, z); }

	dword  GetCRC() const                  { return z.GetCRC(); }
	String GetGZipName() const             { return z.GetGZipName(); }
	String GetGZipComment() const          { return z.GetGZipComment(); }
	
	Zlib& GZip(bool gzip_ = true)          { return z.GZip(); }
	Zlib& Header(bool hdr_ = true)         { return z.Header(); }
	Zlib& NoHeader()                       { return Header(false); }
	Zlib& CRC(bool b = true)               { return z.CRC(); }
	Zlib& NoCRC()                          { return CRC(false); }
	Zlib& ChunkSize(int n)                 { return z.ChunkSize(n); }
	Zlib& Level(int compression_lvl)       { return z.Level(compression_lvl); }

	ZDecompressStream()                    {}
	ZDecompressStream(Stream& out)         { Open(out); }
	~ZDecompressStream()                   { Close(); }
};

int64  CopyStream(Stream& dest, Stream& src, int64 count, Gate<int64, int64> progress, int chunk_size = 65536);

int64  ZCompress(Stream& out, Stream& in, int64 size, Gate<int64, int64> progress = Null, bool hdr = true);
int64  ZCompress(Stream& out, Stream& in, Gate<int64, int64> progress = Null);
String ZCompress(const void *data, int64 len, Gate<int64, int64> progress = Null);
String ZCompress(const String& s, Gate<int64, int64> progress = Null);

int64  ZDecompress(Stream& out, Stream& in, int64 size, Gate<int64, int64> progress = Null, bool hdr = true);
int64  ZDecompress(Stream& out, Stream& in, Gate<int64, int64> progress = Null);
String ZDecompress(const void *data, int64 len, Gate<int64, int64> progress = Null);
String ZDecompress(const String& s, Gate<int64, int64> progress = Null);

int64  GZCompress(Stream& out, Stream& in, int64 size, Gate<int64, int64> progress = Null);
int64  GZCompress(Stream& out, Stream& in, Gate<int64, int64> progress = Null);
String GZCompress(const void *data, int len, Gate<int64, int64> progress = Null);
String GZCompress(const String& s, Gate<int64, int64> progress = Null);

int64  GZDecompress(Stream& out, Stream& in, int64 size, Gate<int64, int64> progress = Null);
int64  GZDecompress(Stream& out, Stream& in, Gate<int64, int64> progress = Null);
String GZDecompress(const void *data, int len, Gate<int64, int64> progress = Null);
String GZDecompress(const String& s, Gate<int64, int64> progress = Null);

bool   GZCompressFile(const char *dstfile, const char *srcfile, Gate<int64, int64> progress = Null);
bool   GZCompressFile(const char *srcfile, Gate<int64, int64> progress = Null);

bool   GZDecompressFile(const char *dstfile, const char *srcfile, Gate<int64, int64> progress = Null);
bool   GZDecompressFile(const char *srcfile, Gate<int64, int64> progress = Null);

inline
bool   GZCompressFile(const String& dstfile, const String& srcfile, Gate<int64, int64> progress = Null)
{ return GZCompressFile(~dstfile, ~srcfile, progress); }
inline
bool   GZCompressFile(const String& srcfile, Gate<int64, int64> progress = Null)
{ return GZCompressFile(~srcfile, progress); }

inline
bool   GZDecompressFile(const String& dstfile, const String& srcfile, Gate<int64, int64> progress = Null)
{ return GZDecompressFile(~dstfile, ~srcfile, progress); }
inline
bool   GZDecompressFile(const String& srcfile, Gate<int64, int64> progress = Null)
{ return GZDecompressFile(~srcfile, progress); }

/// Backward compatibility:

Gate<int64, int64> AsGate64(Gate<int, int> gate);

inline int ZCompress(Stream& out, Stream& in, Gate2<int, int> progress)        { return (int)ZCompress(out, in, AsGate64(progress)); }
inline String ZCompress(const void *data, int len, Gate2<int, int> progress)   { return ZCompress(data, len, AsGate64(progress)); }
inline String ZCompress(const String& s, Gate2<int, int> progress)             { return ZCompress(s, AsGate64(progress)); }

inline int    ZDecompress(Stream& out, Stream& in, Gate2<int, int> progress)   { return (int)ZDecompress(out, in, AsGate64(progress)); }
inline String ZDecompress(const String& s, Gate2<int, int> progress)           { return ZDecompress(s, AsGate64(progress)); }
inline String ZDecompress(const void *data, int len, Gate2<int, int> progress) { return ZDecompress(data, len, AsGate64(progress)); }

inline int    GZCompress(Stream& out, Stream& in, int size, Gate2<int, int> progress) { return (int)GZCompress(out, in, size, AsGate64(progress)); }
inline int    GZCompress(Stream& out, Stream& in, Gate2<int, int> progress)           { return (int)GZCompress(out, in, AsGate64(progress)); }
inline String GZCompress(const void *data, int len, Gate2<int, int> progress)         { return GZCompress(data, len, AsGate64(progress)); }
inline String GZCompress(const String& s, Gate2<int, int> progress)                   { return GZCompress(s, AsGate64(progress)); }

inline int    GZDecompress(Stream& out, Stream& in, int size, Gate2<int, int> progress) { return (int)GZDecompress(out, in, size, AsGate64(progress)); }
inline int    GZDecompress(Stream& out, Stream& in, Gate2<int, int> progress)           { return (int)GZDecompress(out, in, AsGate64(progress)); }
inline String GZDecompress(const void *data, int len, Gate2<int, int> progress)         { return GZDecompress(data, len, AsGate64(progress)); }
inline String GZDecompress(const String& s, Gate2<int, int> progress)                   { return GZDecompress(s, AsGate64(progress)); }

String FastCompress(const void *s, int sz);
String FastCompress(const String& s);
String FastDecompress(const String& data);
