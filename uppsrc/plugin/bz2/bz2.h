#ifndef __Plugin_BZ2__
#define __Plugin_BZ2__

NAMESPACE_UPP

String        BZ2Compress(String s, Gate2<int, int> progress = false);
String        BZ2Decompress(String s, Gate2<int, int> progress = false);
String        BZ2Decompress(Stream& stream, Gate2<int, int> progress = false);
void          BZ2Compress(Stream& out, Stream& in, Gate2<int, int> progress = false);
void          BZ2Decompress(Stream& out, Stream& in, Gate2<int, int> progress = false);

END_UPP_NAMESPACE

#endif//__Plugin_Z__
