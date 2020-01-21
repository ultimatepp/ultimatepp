#ifndef _plugin_zstd_zstd_h_
#define _plugin_zstd_zstd_h_

#include <Core/Core.h>

#define ZSTD_STATIC_LINKING_ONLY
#include "lib/zstd.h"

namespace Upp {

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
	void Co(bool b = true);
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
		String       compressed_data; // can contain more frames
		int          frame_at, frame_sz; // position and compressed size of frame
		int          decompressed_sz;
		Buffer<char> decompressed_data; // decompressed data
		bool         irregular_d = false; // d reallocated to accomodate bigger result
		
		const void *FramePtr() { return ~compressed_data + frame_at; }
		void Clear()           { compressed_data.Clear(); decompressed_data.Clear(); irregular_d = false; }
	};
	
	String compressed_data; // buffer to store compressed data
	int    compressed_at; // where are we in above buffer
	
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
	void Co(bool b = true)                                    { concurrent = b; }

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

#endif
