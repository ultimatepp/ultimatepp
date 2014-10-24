#include "lzo.h"

namespace Upp {

String LZOCompress(const void *data, int len)
{
	int olen = len + (len >> 4) + 64 + 3 + 10;
	StringBuffer out(olen);
	Poke32le(~out, len);
	Buffer<byte> wrkmem(LZO1X_MEM_COMPRESS);
	lzo_uint out_len;
	CHECK(lzo1x_1_compress((lzo_bytep)data, len, (lzo_bytep)~out + 4, &out_len, wrkmem) == LZO_E_OK);
	out.SetLength(out_len + 4);
	return out;
}

String LZOCompress(const String& s)
{
	return LZOCompress(~s, s.GetLength());
}

String LZODecompress(const void *data, int len)
{
	int outlen = Peek32le(data);
	StringBuffer out(outlen);
    lzo_uint new_len;
    CHECK(lzo1x_decompress((lzo_bytep)data + 4, len, (lzo_bytep)~out, &new_len, NULL) == LZO_E_OK);
	ASSERT(new_len == outlen);
	return out;
}

String LZODecompress(const String& s)
{
	return LZODecompress(~s, s.GetLength());
}

}