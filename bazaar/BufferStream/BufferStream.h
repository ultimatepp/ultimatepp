#ifndef _BufferStream_BufferStream_h
#define _BufferStream_BufferStream_h

#include <Core/Core.h>

using namespace Upp;

class BufferStream : public MemStream {
protected:
	virtual  void  _Put(int w)               { byte h = w; _Put(&h, 1); }
	virtual  void  _Put(const void *data, dword size);

public:
	virtual  void  Seek(int64 pos);
	virtual  void  SetSize(int64 asize);

protected:
	Vector<byte> data;

public:
	void           Open(Vector<byte>& d);
	void           Create();
	void           Reserve(int n)            { SetSize(GetSize() + n); }
	void           Crop()                    { SetSize(GetPos()); }

	Vector<byte>   GetResult();
	operator    Vector<byte>()               { return GetResult(); }

	BufferStream()                           { Create(); }
	BufferStream(Vector<byte>& d)            { Open(d); }
};

typedef BufferStream VectorStream;

#endif
