class InputFilterStream : public Stream {
public:
	virtual   bool  IsOpen() const;

protected:
	virtual   int   _Term();
	virtual   int   _Get();
	virtual   dword _Get(void *data, dword size);

	Vector<byte> buffer;
	bool eof;

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
		in = &in_;
		filter.WhenOut = callback(this, &InputFilterStream::Out);
		Filter = callback<F, F, const void *, int>(&filter, &F::Put);
		End = callback(&filter, &F::End);
	}
	
	InputFilterStream();
	template <class F> InputFilterStream(Stream& in, F& filter) { Init(); Set(in, filter); }
};

class OutputFilterStream : public Stream {
public:
	virtual   void  Close();
	virtual   bool  IsOpen() const;

protected:
	virtual   void  _Put(int w);
	virtual   void  _Put(const void *data, dword size);

	Buffer<byte> buffer;

	void   FlushOut();
	dword  Avail()               { return 4096 - (ptr - ~buffer); }
	void   Init();

public:
	Stream                      *out;
	Callback2<const void *, int> Filter;
	Callback                     End;
	void                         Out(const void *ptr, int size);

	template <class F>
	void Set(Stream& out_, F& filter) {
		out = &out_;
		filter.WhenOut = callback(this, &OutputFilterStream::Out);
		Filter = callback<F, F, const void *, int>(&filter, &F::Put);
		End = callback(&filter, &F::End);
	}
	
	OutputFilterStream();
	template <class F> OutputFilterStream(Stream& in, F& filter) { Init(); Set(in, filter); }
	~OutputFilterStream();
};
