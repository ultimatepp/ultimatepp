class InFilterStream : public Stream {
public:
	virtual   bool  IsOpen() const;

protected:
	virtual   int   _Term();
	virtual   int   _Get();
	virtual   dword _Get(void *data, dword size);

	Vector<byte> buffer;
	bool         eof;
	int          buffersize = 4096;
	Buffer<int>  inbuffer;
	byte        *t; // target pointer for block _Get
	dword        todo; // target count

	void   Init();
	void   Fetch();
	void   SetRd();

private:
	void   SetSize(int64 size)  { NEVER(); } // removed
	int64  GetSize() const      { NEVER(); return 0; }

public:
	Stream                      *in;
	Event<const void *, int>     Filter;
	Gate<>                       FilterEof;
	Event<>                      End;
	Gate<>                       More;
	void                         Out(const void *ptr, int size);
	
	Event<>                      WhenOut;
	
	template <class F>
	void Set(Stream& in_, F& filter) {
		Init();
		in = &in_;
		filter.WhenOut = [=](const void *ptr, int size) { Out(ptr, size); };
		Filter = [&filter](const void *ptr, int size) { filter.Put(ptr, size); };
		End = [&filter] { filter.End(); };
	}
	
	void SetBufferSize(int size) { buffersize = size; inbuffer.Clear(); }
	
	InFilterStream();
	template <class F> InFilterStream(Stream& in, F& filter) { Set(in, filter); }
};

class OutFilterStream : public Stream {
public:
	virtual   void  Close();
	virtual   bool  IsOpen() const;

protected:
	virtual   void  _Put(int w);
	virtual   void  _Put(const void *data, dword size);

	Buffer<byte> buffer;
	int64        count;

	void   FlushOut();
	dword  Avail()               { return dword(4096 - (ptr - ~buffer)); }
	void   Init();

public:
	Stream                      *out;
	Event<const void *, int>     Filter;
	Event<>                      End;
	void                         Out(const void *ptr, int size);
	
	int64                        GetCount() const             { return count; }
	
	Event<int64>                 WhenPos;

	template <class F>
	void Set(Stream& out_, F& filter) {
		Init();
		out = &out_;
		filter.WhenOut = callback(this, &OutFilterStream::Out);
		Filter = callback<F, F, const void *, int>(&filter, &F::Put);
		End = callback(&filter, &F::End);
		count = 0;
	}
	
	OutFilterStream();
	template <class F> OutFilterStream(Stream& in, F& filter) { Set(in, filter); }
	~OutFilterStream();
};
