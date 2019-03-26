#ifndef _plugin_zstd_zstd_legacy_h_
#define _plugin_zstd_zstd_legacy_h_

#include <Core/Core.h>

namespace Upp {
	
namespace Legacy {

#define ZSTD_STATIC_LINKING_ONLY
#include "lib/zstd.h"

class ZstdCompressStream : public Stream  {
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

	enum { BLOCK_BYTES = 1024*1024 };
	
	int           level;
	
	bool          concurrent;
    
    void          Alloc();
	void          Init();
	void          FlushOut();

public:
#ifdef _MULTITHREADED
	void Co(bool b = true);
#endif
	void Open(Stream& out, int level = 1);

	ZstdCompressStream();
	ZstdCompressStream(Stream& out, int level = 1) : ZstdCompressStream() { Open(out, level); }
	~ZstdCompressStream();
};

class ZstdDecompressStream : public Stream {
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
		int          clen = 0, dlen = 0; // compressed, decompressed len
		String       lc; // used when compressed length is too big
		bool         irregular_d = false; // d reallocated to accomodate bigger result
		
		void Clear() { c.Clear(); d.Clear(); lc.Clear(); }
	};
	Workblock wb[16];
	int       count; // count of workblocks fetched
	int       ii; // next workblock to be read
	int       dlen; // length of current workblock
	
	enum { BLOCK_BYTES = 1024*1024 }; // expected decompressed size
	
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

#ifdef _MULTITHREADED
	void Co(bool b = true)                                    { concurrent = b; }
#endif

	ZstdDecompressStream();
	ZstdDecompressStream(Stream& in) : ZstdDecompressStream() { Open(in); }
	~ZstdDecompressStream();
};

int64 ZstdCompress(Stream& out, Stream& in, Gate<int64, int64> progress = Null);
int64 ZstdDecompress(Stream& out, Stream& in, Gate<int64, int64> progress = Null);
String ZstdCompress(const void *data, int64 len, Gate<int64, int64> progress = Null);
String ZstdCompress(const String& s, Gate<int64, int64> progress = Null);
String ZstdDecompress(const void *data, int64 len, Gate<int64, int64> progress = Null);
String ZstdDecompress(const String& s, Gate<int64, int64> progress = Null);

int64 CoZstdCompress(Stream& out, Stream& in, Gate<int64, int64> progress = Null);
int64 CoZstdDecompress(Stream& out, Stream& in, Gate<int64, int64> progress = Null);
String CoZstdCompress(const void *data, int64 len, Gate<int64, int64> progress = Null);
String CoZstdCompress(const String& s, Gate<int64, int64> progress = Null);
String CoZstdDecompress(const void *data, int64 len, Gate<int64, int64> progress = Null);
String CoZstdDecompress(const String& s, Gate<int64, int64> progress = Null);

bool IsZstd(Stream& s);

}

}

#endif
