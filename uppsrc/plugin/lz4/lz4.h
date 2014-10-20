#ifndef _lz4_lz4_h_
#define _lz4_lz4_h_

#include <Core/Core.h>

#include "lib/lz4.h"

namespace Upp {

class Lz4 {
	enum { BLOCK_BYTES = 64 * 1024 };
	char        *block[2];
	Buffer<char> buffer;
	int8         compress;
	bool         error;
	
	int          cbi;
	int          pos;

    LZ4_stream_t        lz4Stream[1];
    LZ4_streamDecode_t  lz4StreamDecode[1];
    
    String       out;

	void          Init();
	void          FlushOut();

	void          PutOut(const void *ptr, int size);

public:	
	Callback2<const void *, int> WhenOut;

	void Put(const void *ptr, int size);
	void Put(const String& s)              { Put(~s, s.GetCount()); }
	void End();
	void Clear();
	
	const String& Get() const              { return out; }
	operator const String&() const         { return out; }
	const String& operator~() const        { return out; }
	void   ClearOut()                      { out.Clear(); }

	void Compress();
	void Decompress();

	bool   IsError() const                 { return error; }

	Lz4();
	~Lz4();
};

int64  LZ4Compress(Stream& out, Stream& in, Gate2<int64, int64> progress = false);
int64  LZ4Decompress(Stream& out, Stream& in, Gate2<int64, int64> progress = false);
String LZ4Compress(const void *data, int64 len, Gate2<int64, int64> progress = false);
String LZ4Compress(const String& s, Gate2<int64, int64> progress = false);
String LZ4Decompress(const void *data, int64 len, Gate2<int64, int64> progress = false);
String LZ4Decompress(const String& s, Gate2<int64, int64> progress = false);

};

#endif
