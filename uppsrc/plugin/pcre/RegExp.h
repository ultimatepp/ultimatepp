#ifndef _PcreTest_RegExp_h_
#define _PcreTest_RegExp_h_

//#include <Core/Core.h>

class RegExp : public Moveable<RegExp> {
public:
	enum
	{
		PARTIAL   = PCRE_PARTIAL,
		/* compile options */
		UNICODE   = PCRE_UTF8,
		UTF8      = PCRE_UTF8,
		CASELESS  = PCRE_CASELESS,
		MULTILINE = PCRE_MULTILINE,
		UNGREEDY  = PCRE_UNGREEDY
	};

private:
	String pattern;
	String text;
	pcre * cpattern;
	const char * error_string;
	int error_offset;
	int error_code;
	int pos[30];
	int rc;
	bool first;
	int compile_options;
	int execute_options;

public:
	void           Clear(bool freemem = false);
	void           SetOptions(int options);
	void           SetPattern(const char * p);
	void           SetPattern(const String &p);
	bool           Compile(bool recompile = false);
	int            Execute(const String &t, int offset = 0);
	bool           Match(const String &t, bool copy = true);
	bool           FastMatch(const String &t);
	bool           GlobalMatch(const String &t);
	String         operator[](const int i);
	int            GetCount();
	String         GetString(int i);
	void           GetMatchPos(int i, int& iPosStart, int& iPosAfterEnd);
	Vector<String> GetStrings();


	bool           IsError() { return error_code != 0; }
	const char    *GetError() { return error_string; }
	int            GetErrorCode() { return error_code; }

	RegExp(int options = UTF8);
	RegExp(const char * p, int options = UTF8);
	RegExp(const String &p, int options = UTF8);
	~RegExp();
};

#endif
