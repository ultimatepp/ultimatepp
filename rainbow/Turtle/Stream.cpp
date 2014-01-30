#include "Local.h"

#ifdef GUI_TURTLE

NAMESPACE_UPP

#define LLOG(x)     // LLOG(x)
#define LDUMP(x)    // RDUMP(x)
#define LTIMING(x)

void TurtleStream::Reset()
{
	zlib.Clear();
	zlib.Compress();
	hasdata = false;
}

void TurtleStream::Out(const void *data, dword size)
{
	zlib.Put(data, (int)size);
}

String TurtleStream::FlushStream()
{
	Flush();
	zlib.End();
	String s = zlib.Get();
	Reset();
	return s;
}

END_UPP_NAMESPACE

#endif
