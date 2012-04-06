class Crc32Stream : public OutStream {
	dword crc;

	virtual  void  Out(const void *data, dword size);

public:
	dword  Finish()            { Flush(); return crc; }
	operator dword()           { return Finish(); }
	
	Crc32Stream();
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
	
	void          PutOut(const void *ptr, dword size);
	void          Pump(bool finish);
	void          Begin();
	void          Free();
	void          Put0(const char *ptr, int size);
	int           GzipHeader(const char *ptr, int size);
	void          Init();

public:
	Callback2<const void *, dword> WhenOut;
	
	void Compress();
	void Decompress();
	void Put(const void *ptr, dword size);
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

	Zlib();
	~Zlib();
};

int    ZCompress(Stream& out, Stream& in, int size, Gate2<int, int> progress = false, bool nohdr = false, dword *crc = NULL);
int    ZDecompress(Stream& out, Stream& in, int size, Gate2<int, int> progress = false, bool nohdr = false, dword *crc = NULL);

int    ZCompress(Stream& out, Stream& in, Gate2<int, int> progress = false);
int    ZDecompress(Stream& out, Stream& in, Gate2<int, int> progress = false);

String ZCompress(const void *data, int len, Gate2<int, int> progress = false);
String ZDecompress(const void *data, int len, Gate2<int, int> progress = false);

String ZCompress(const String& s, Gate2<int, int> progress = false);
String ZDecompress(const String& s, Gate2<int, int> progress = false);

int    GZCompress(Stream& out, Stream& in, int size, Gate2<int, int> progress = false);
int    GZDecompress(Stream& out, Stream& in, int size, Gate2<int, int> progress = false);

String GZCompress(const void *data, int len, Gate2<int, int> progress = false);
String GZDecompress(const void *data, int len, Gate2<int, int> progress = false);

String GZCompress(const String& s, Gate2<int, int> progress = false);
String GZDecompress(const String& s, Gate2<int, int> progress = false);
