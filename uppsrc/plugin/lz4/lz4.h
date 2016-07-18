#ifndef _lz4_lz4_h_
#define _lz4_lz4_h_

#include <Core/Core.h>

#include <Core/lib/lz4.h>

namespace Upp {

class Lz4 {
	StringBuffer buffer; // to be able to pass as String
	int8         compress;
	bool         error;
	
	enum { BLOCK_BYTES = 1024 * 1024 };
	
	xxHashStream xxh;
	int          maxblock;
	int          pos;
	int          blockchksumsz;
	bool         header;
	byte         lz4hdr;
	String       header_data;

    String       out;
	
	bool         parallel;
	CoWork       co;
	Mutex        lock;
	ConditionVariable cond;
	int          outblock;
	int          inblock;
    
    void          TryHeader();

	void          Init();
	void          FinishBlock(char *outbuf, int clen, const char *origdata, int origsize);
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
	
	void Parallel(bool b = true)           { parallel = b; }

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

int64  CoLZ4Compress(Stream& out, Stream& in, Gate2<int64, int64> progress = false);
int64  CoLZ4Decompress(Stream& out, Stream& in, Gate2<int64, int64> progress = false);
String CoLZ4Compress(const void *data, int64 len, Gate2<int64, int64> progress = false);
String CoLZ4Compress(const String& s, Gate2<int64, int64> progress = false);
String CoLZ4Decompress(const void *data, int64 len, Gate2<int64, int64> progress = false);
String CoLZ4Decompress(const String& s, Gate2<int64, int64> progress = false);

bool IsLZ4(Stream& s);

};

#endif
