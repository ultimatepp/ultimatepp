#ifndef __Plugin_BZ2__
#define __Plugin_BZ2__

namespace Upp {

String        BZ2Compress(String s, Gate<int, int> progress = Null);
String        BZ2Decompress(String s, Gate<int, int> progress = Null);
String        BZ2Decompress(Stream& stream, Gate<int, int> progress = Null);
void          BZ2Compress(Stream& out, Stream& in, Gate<int, int> progress = Null);
void          BZ2Decompress(Stream& out, Stream& in, Gate<int, int> progress = Null);

}

#endif//__Plugin_Z__
