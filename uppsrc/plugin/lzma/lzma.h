#ifndef _plugin_lzma_lzma_h_
#define _plugin_lzma_lzma_h_

#include <Core/Core.h>

namespace Upp {

int64  LZMACompress(Stream& out, Stream& in, int64 size, Gate2<int64, int64> progress = false, int lvl = 6);
int64  LZMACompress(Stream& out, Stream& in, Gate2<int64, int64> progress = false, int lvl = 6);
String LZMACompress(const void *data, int64 len, Gate2<int64, int64> progress = false, int lvl = 6);
String LZMACompress(const String& s, Gate2<int64, int64> progress = false, int lvl = 6);

int64  LZMADecompress(Stream& out, Stream& in, Gate2<int64, int64> progress = false);
String LZMADecompress(const void *data, int64 len, Gate2<int64, int64> progress = false);
String LZMADecompress(const String& s, Gate2<int64, int64> progress = false);

bool   LZMACompressFile(const char *dstfile, const char *srcfile, Gate2<int64, int64> progress = false, int lvl = 6);
bool   LZMACompressFile(const char *srcfile, Gate2<int64, int64> progress, int lvl = 6);
bool   LZMADecompressFile(const char *dstfile, const char *srcfile, Gate2<int64, int64> progress = false);
bool   LZMADecompressFile(const char *srcfile, Gate2<int64, int64> progress);

}

#endif
