#ifndef _lz4_lz4_h_
#define _lz4_lz4_h_

#include <Core/Core.h>

// LZ4 really is included in Core, local lib only serves as copy

#include <Core/lib/lz4.h>

namespace Upp {

enum {
	LZ4F_MAGIC       = 0x184D2204,

	LZ4F_VERSIONMASK = 0b11000000,
	LZ4F_VERSION     = 0b01000000,
	LZ4F_BLOCKINDEPENDENCE = (1 << 5),
	LZ4F_BLOCKCHECKSUM     = (1 << 4),
	LZ4F_CONTENTSIZE       = (1 << 3),
	LZ4F_CONTENTCHECKSUM   = (1 << 2),
	
	LZ4F_MAXSIZEMASK      = 0x70,
    LZ4F_MAXSIZE_64KB     = 0x40,
    LZ4F_MAXSIZE_256KB    = 0x50,
    LZ4F_MAXSIZE_1024KB   = 0x60,
    LZ4F_MAXSIZE_4096KB   = 0x70,
};

class LZ4CompressStream : public Stream  {
public:
	virtual   void  Close();
	virtual   bool  IsOpen() const;

protected:
	virtual   void  _Put(int w);
	virtual   void  _Put(const void *data, dword size);
	
	Stream      *out;
	
	Buffer<byte> buffer;
	Buffer<byte> outbuf;
	Buffer<int>  outsz;

	enum { BLOCK_BYTES = 1024 * 1024 };
	
	xxHashStream xxh;

	bool          concurrent;
    
    void          Alloc();
	void          Init();
	void          FlushOut();

public:
	void Co(bool b = true);
	void Open(Stream& out_);

	LZ4CompressStream();
	LZ4CompressStream(Stream& out) : LZ4CompressStream() { Open(out); }
	~LZ4CompressStream();
};

class LZ4DecompressStream : public Stream {
public:
	virtual   bool  IsOpen() const;

protected:
	virtual   int   _Term();
	virtual   int   _Get();
	virtual   dword _Get(void *data, dword size);

private:
	Stream        *in;
	struct Workblock {
		Buffer<char> c, d; // compressed, decompressed data
		int   clen = 0, dlen = 0; // compressed, decompressed len
		
		void Clear() { c.Clear(); d.Clear(); }
	};
	Workblock wb[16];
	int       count; // count of workblocks fetched
	int       ii; // next workblock to be read
	int       dlen; // length of current workblock
	
	enum { BLOCK_BYTES = 1024*1024 };
	
	xxHashStream xxh;
	int          maxblock;
	int          blockchksumsz;
	byte         lz4hdr;
	bool         eof;
	
	bool         concurrent;

    void          TryHeader();

	void          Init();
	bool          Next();
	void          Fetch();
	bool          Ended() const { return IsError() || in->IsError() || ptr == rdlim && ii == count && eof; }

public:
	bool Open(Stream& in);

	void Co(bool b = true)                                  { concurrent = b; }

	LZ4DecompressStream();
	LZ4DecompressStream(Stream& in) : LZ4DecompressStream() { Open(in); }
	~LZ4DecompressStream();
};

int64  LZ4Compress(Stream& out, Stream& in, Gate<int64, int64> progress = Null);
int64  LZ4Decompress(Stream& out, Stream& in, Gate<int64, int64> progress = Null);
String LZ4Compress(Stream& in, Gate<int64, int64> progress = Null);
String LZ4Decompress(Stream& in, Gate<int64, int64> progress = Null);
String LZ4Compress(const void *data, int64 len, Gate<int64, int64> progress = Null);
String LZ4Compress(const String& s, Gate<int64, int64> progress = Null);
String LZ4Decompress(const void *data, int64 len, Gate<int64, int64> progress = Null);
String LZ4Decompress(const String& s, Gate<int64, int64> progress = Null);

int64  CoLZ4Compress(Stream& out, Stream& in, Gate<int64, int64> progress = Null);
int64  CoLZ4Decompress(Stream& out, Stream& in, Gate<int64, int64> progress = Null);
String CoLZ4Compress(Stream& in, Gate<int64, int64> progress = Null);
String CoLZ4Decompress(Stream& in, Gate<int64, int64> progress = Null);
String CoLZ4Compress(const void *data, int64 len, Gate<int64, int64> progress = Null);
String CoLZ4Compress(const String& s, Gate<int64, int64> progress = Null);
String CoLZ4Decompress(const void *data, int64 len, Gate<int64, int64> progress = Null);
String CoLZ4Decompress(const String& s, Gate<int64, int64> progress = Null);

bool IsLZ4(Stream& s);

};

#endif
