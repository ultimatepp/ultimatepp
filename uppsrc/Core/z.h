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
