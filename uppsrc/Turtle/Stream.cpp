#include "Turtle.h"

#define LLOG(x)     // LLOG(x)
#define LDUMP(x)    // RDUMP(x)
#define LTIMING(x)

namespace Upp {

static TurtleServer::Stream sTurtleStream;

TurtleServer::Stream::Stream()
{
	Reset();
}

void TurtleServer::Stream::Out(const void *data, dword size)
{
	zlib.Put(data, (int) size);
}

String TurtleServer::Stream::FlushStream()
{
	Flush();
	zlib.End();
	String s = zlib.Get();
	Reset();
	return s;
}

void TurtleServer::Stream::Reset()
{
	zlib.Clear();
	zlib.Compress();
	hasdata = false;
}

void TurtleServer::Put8(int x)
{
	sTurtleStream.hasdata = true;
	sTurtleStream.Put(x);
}

void TurtleServer::Put16(int x)
{
	Put8(LOBYTE(x));
	Put8(HIBYTE(x));
}

void TurtleServer::Put32(int x)
{
	Put16(LOWORD(x));
	Put16(HIWORD(x));
}

void TurtleServer::Put(Point p)
{
	// TODO: Clamp?
	Put16(p.x);
	Put16(p.y);
}

void TurtleServer::Put(Size sz)
{
	Put((Point)sz);
}

void TurtleServer::Put(const Rect& r)
{
	Put(r.TopLeft());
	Put(r.GetSize());
}

void TurtleServer::Put(const String& s)
{
	Put32(s.GetLength());
	sTurtleStream.hasdata = true;
	sTurtleStream.Put(s);
}

//void Turtle_PutLink(const String& link) // FIXME
//{
//	TurtleServer::Put8(OPENLINK);
//	TurtleServer::Put(link);
//}

void TurtleServer::Flush()
{
	if(!sTurtleStream.hasdata || websocket.IsClosed())
		return;

	websocket.SendBinary(ZCompress(String(DISABLESENDING, 1))); // Do not send events until data transfered and processed
	int64 x = ++update_serial;
	if(IsNull(serial_time0)) {
		serial_time0 = msecs();
		serial_0 = update_serial;
	}
	Put8(UPDATESERIAL);
	Put32(LODWORD(x));
	Put32(HIDWORD(x));
	String s = sTurtleStream.FlushStream();
	stat_data_send += s.GetCount();
	LLOG("Sending " << s.GetLength());
	websocket.SendBinary(s);
}
}
