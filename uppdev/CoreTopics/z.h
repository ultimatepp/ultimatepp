class Crc32 {
	dword crc;

public:	
	void Put(const void *ptr, int count);
	void Put(char c);
	void Put(byte c);

	operator dword() const { return crc; }
	
	Crc32();
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
