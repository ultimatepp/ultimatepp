class InFilterStream : public Stream {
public:
	virtual   bool  IsOpen() const;
	virtual   int64 GetSize() const;

protected:
	virtual   int   _Term();
	virtual   int   _Get();
	virtual   dword _Get(void *data, dword size);

	Vector<byte> buffer;
	bool         eof;
	int64        size;

	void   Init();
	void   Fetch(int size);
	dword  Avail();

public:
	Stream                      *in;
	Callback2<const void *, int> Filter;
	Callback                     End;
	Gate                         More;
	void                         Out(const void *ptr, int size);
	
	template <class F>
	void Set(Stream& in_, F& filter) {
		Init();
		in = &in_;
		filter.WhenOut = callback(this, &InFilterStream::Out);
		Filter = callback<F, F, const void *, int>(&filter, &F::Put);
		End = callback(&filter, &F::End);
	}
	
	void SetSize(int64 size_)    { size = size_; }
	
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
	Callback2<const void *, int> Filter;
	Callback                     End;
	void                         Out(const void *ptr, int size);
	
	int64                        GetCount() const             { return count; }

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
