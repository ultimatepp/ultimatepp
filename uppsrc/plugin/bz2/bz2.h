#ifndef __Plugin_BZ2__
#define __Plugin_BZ2__

namespace Upp {

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
