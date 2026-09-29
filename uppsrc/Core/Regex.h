class RegexMatch {
	uint64               pattern_serial = 0;
    pcre2_match_data_8  *match_data_8 = nullptr;
    pcre2_match_data_32 *match_data_32 = nullptr;
	PCRE2_SIZE          *ovector;
	int                  count = 0;

    static std::atomic<int64> serial;

	friend class Regex;
	friend class WRegex;

public:
	void              Clear();
	int               GetCount() const;
	int               GetOffset(int i = 0) const;
	int               GetLength(int i = 0) const;
	String            GetString(const String& text, int i) const   { return text.Mid(GetOffset(i), GetLength(i)); }
	WString           GetString(const WString& text, int i) const  { return text.Mid(GetOffset(i), GetLength(i)); }
	
	~RegexMatch();
};

class Regex {
	uint64        pattern_serial = 0;
	pcre2_code_8 *re = nullptr;
	dword         options = 0;
    int           errorcode;

	void  Clear();
	bool  Match(const char *text, int len, RegexMatch *match) const;

public:
	String GetLastErrorText() const;
	
	bool Set(dword options, const char *pattern, int len);
	bool Set(dword options, const char *pattern)                    { return Set(options, pattern, strlen(pattern)); }
	bool Set(dword options, const String& pattern)                  { return Set(options, pattern, pattern.GetCount()); }
	
	bool Set(const char *pattern, int len)                          { return Set(PCRE2_UTF, pattern, len); }
	bool Set(const char *pattern)                                   { return Set(pattern, strlen(pattern)); }
	bool Set(const String& pattern)                                 { return Set(pattern, pattern.GetCount()); }
	
	bool Match(const char *text, int len, RegexMatch& match) const  { return Match(text, len, &match); }
	bool Match(const char *text, RegexMatch& match) const           { return Match(text, strlen(text), match); }
	bool Match(const String& text, RegexMatch& match) const         { return Match(text, text.GetCount(), match); }

	bool Match(const char *text, int len) const                     { return Match(text, len, nullptr); }
	bool Match(const char *text) const                              { return Match(text, strlen(text)); }
	bool Match(const String& text) const                            { return Match(text, text.GetCount()); }

	Regex() {}
	Regex(dword options, const char *pattern, int len)              { Set(pattern, len); }
	Regex(dword options, const char *pattern)                       { Set(options, pattern, strlen(pattern)); }
	Regex(dword options, const String& pattern)                     { Set(options, pattern, pattern.GetCount()); }

	Regex(const char *pattern, int len)                             { Set(PCRE2_UTF, pattern, len); }
	Regex(const char *pattern)                                      { Set(pattern, strlen(pattern)); }
	Regex(const String& pattern)                                    { Set(pattern, pattern.GetCount()); }
	~Regex()                                                        { Clear(); }
};

class WRegex {
	uint64         pattern_serial = 0;
	pcre2_code_32 *re = nullptr;
	dword          options = 0;
    int            errorcode;

	void  Clear();
	bool  Match(const wchar *text, int len, RegexMatch *match) const;

public:
	String GetLastErrorText() const;
	
	bool Set(dword options, const wchar *pattern, int len);
	bool Set(dword options, const wchar *pattern)                    { return Set(options, pattern, strlen32(pattern)); }
	bool Set(dword options, const WString& pattern)                  { return Set(options, pattern, pattern.GetCount()); }
	
	bool Set(const wchar *pattern, int len)                          { return Set(PCRE2_UTF, pattern, len); }
	bool Set(const wchar *pattern)                                   { return Set(pattern, strlen32(pattern)); }
	bool Set(const WString& pattern)                                 { return Set(pattern, pattern.GetCount()); }
	
	bool Match(const wchar *text, int len, RegexMatch& match) const  { return Match(text, len, &match); }
	bool Match(const wchar *text, RegexMatch& match) const           { return Match(text, strlen32(text), match); }
	bool Match(const WString& text, RegexMatch& match) const         { return Match(text, text.GetCount(), match); }

	bool Match(const wchar *text, int len) const                     { return Match(text, len, nullptr); }
	bool Match(const wchar *text) const                              { return Match(text, strlen32(text)); }
	bool Match(const WString& text) const                            { return Match(text, text.GetCount()); }

	WRegex() {}
	WRegex(dword options, const wchar *pattern, int len)             { Set(pattern, len); }
	WRegex(dword options, const wchar *pattern)                      { Set(options, pattern, strlen32(pattern)); }
	WRegex(dword options, const WString& pattern)                    { Set(options, pattern, pattern.GetCount()); }

	WRegex(const wchar *pattern, int len)                            { Set(PCRE2_UTF, pattern, len); }
	WRegex(const wchar *pattern)                                     { Set(pattern, strlen32(pattern)); }
	WRegex(const WString& pattern)                                   { Set(pattern, pattern.GetCount()); }
	~WRegex()                                                        { Clear(); }
};
