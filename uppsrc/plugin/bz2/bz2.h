#ifndef __Plugin_BZ2__
#define __Plugin_BZ2__

#ifdef flagWIN32
	#include "lib/bzlib.h"
#else
	#include <bzlib.h>
#endif

namespace Upp {
	
namespace bz2 {
	class Lib {
		enum { NONE, DEFLATE, INFLATE };
	
		bz_stream     z;
		Buffer<char>  output;
		int           chunk;
		int           mode;
		int           total;
		int           compression_level;
		bool          error;
		bool          rdall;
		bool          eos;
		String        out;
		
		void          SetError(bool v) { error = v; }
		void          PutOut(const void *ptr, int size);
		void          Pump(bool finish);
		void          Begin();
		void          Free();
		int           GzipHeader(const char *ptr, int size);
		void          Init();
	
	public:
		Event<const void *, int>  WhenOut;
		
		void Compress();
		void Decompress(bool all = true);
		void Put(const void *ptr, int size);
		void Put(const String& s)              { Put(~s, s.GetCount()); }
		void End();
		void Clear();
		
		const String& Get() const              { return out; }
		operator const String&() const         { return out; }
		const String& operator~() const        { return out; }
		void ClearOut()                        { out.Clear(); }
	
		bool   IsError() const                 { return error; }
		bool   IsEOS() const                   { return eos; }
		
		Lib& ChunkSize(int n);
		Lib& Level(int compression_lvl)        { compression_level = compression_lvl; return *this; }
	
		Lib();
		~Lib();
	};

	class CompressStream : public OutFilterStream {
		Lib z;
	
	public:
		void Open(Stream& out)                 { z.Compress(); Set(out, z); }
	
		Lib& ChunkSize(int n)                  { return z.ChunkSize(n); }
		Lib& Level(int compression_lvl)        { return z.Level(compression_lvl); }
	
		CompressStream()                       {}
		CompressStream(Stream& out)            { Open(out); }
		~CompressStream()                      { Close(); }
	};
	
	class DecompressStream : public InFilterStream {
		Lib z;
	
	public:
		void Open(Stream& in, bool all = true) { z.Decompress(all); Set(in, z); FilterEof = [=]() -> bool { return z.IsEOS(); }; }
		Lib& ChunkSize(int n)                  { return z.ChunkSize(n); }
	
		DecompressStream()                             {}
		DecompressStream(Stream& out, bool all = true) { Open(out, all); }
		~DecompressStream()                            { Close(); }
	};
}

typedef bz2::CompressStream BZ2CompressStream;
typedef bz2::DecompressStream BZ2DecompressStream;

void          BZ2Compress(Stream& out, Stream& in, Gate<int, int> progress = Null);
void          BZ2Decompress(Stream& out, Stream& in, Gate<int, int> progress = Null);

String        BZ2Compress(Stream& in, Gate<int, int> progress = Null);
String        BZ2Decompress(Stream& in, Gate<int, int> progress = Null);

String        BZ2Compress(const void *data, int64 len, Gate<int, int> progress);
String        BZ2Decompress(const void *data, int64 len, Gate<int, int> progress = Null);

String        BZ2Compress(const String& data, Gate<int, int> progress = Null);
String        BZ2Decompress(const String& data, Gate<int, int> progress = Null);

}

#endif//__Plugin_Z__
