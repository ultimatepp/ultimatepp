class SuggestCtrl : public EditString {
	virtual bool  Key(dword key, int count);
	virtual void  CancelMode();
	virtual void  LostFocus();
	virtual void  GotFocus();

private:
	Vector<WString> data;
	ArrayCtrl       list;
	int             droplines;
	int             delimiter_char;
	int           (*delimiter_filter)(int c);
	int           (*compare_filter)(int c);
	bool            just_start;

	void            Cancel();
	void            Select();
	WString         ReadLast(int& h);
	int             IsDelimiter(int c);
	WString         CF(const WString& src);

	typedef SuggestCtrl CLASSNAME;

public:
	void ClearList()                                  { data.Clear(); }
	void AddList(const WString& s)                    { data.Add(s); }
	void AddList(const String& s)                     { data.Add(s.ToWString()); }
	void AddList(const char *s)                       { data.Add(s); }
	void FindAddList(const WString& s)                { if(FindIndex(data,s) < 0) data.Add(s); }
	void FindAddList(const String& s)                 { FindAddList(s.ToWString()); }
	void FindAddList(const char *s)                   { FindAddList(String(s)); }

	const Vector<WString>& GetList()                  { return data; }
	void Pick(Vector<WString>&& list)                 { data = pick(list); }
	
	SuggestCtrl& SetDropLines(int n)                  { droplines = n; return *this; }
	SuggestCtrl& Delimiter(int chr)                   { delimiter_char = chr; return *this; }
	SuggestCtrl& Delimiter(int (*filter)(int c))      { delimiter_filter = filter; return *this; }
	SuggestCtrl& CompareFilter(int (*filter)(int c))  { compare_filter = filter; return *this; }
	SuggestCtrl& JustStart(bool b = true)             { just_start = b; return *this; }
		
	SuggestCtrl();
};
