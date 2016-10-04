#ifndef _E011_console_h_
#define _E011_console_h_

class Console : public LineEdit {
public:
	virtual bool Key(dword key, int count);
	virtual void LeftDouble(Point p, dword);
	virtual void RightDown(Point p, dword);
	virtual void Append(const String& s);

protected:
	bool quiet;
	bool wrap_text;

public:
	Callback WhenSelect;
	Callback1<Bar&> WhenBar;

	Console& operator<<(const String& s)      { Append(s); return *this; }

	void WrapText(bool w)                     { wrap_text = w; }

	void Put(const String& s)									{ Append(s); Append("\n"); }
	void PutVerbose(const char *s)						{ Put(s); Sync(); }

	typedef Console CLASSNAME;

	Console();
};

#endif
