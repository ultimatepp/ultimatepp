#include "pcre.h"

RegExp::RegExp(int options)
{
	Clear();
	SetOptions(options);
}

RegExp::RegExp(const char * p, int options)
{
	Clear();
	SetOptions(options);
	SetPattern(p);
}

RegExp::RegExp(const String &p, int options)
{
	Clear();
	SetOptions(options);
	SetPattern(p);
}

RegExp::~RegExp()
{
	Clear(true);
}

void RegExp::Clear(bool freemem)
{
	if(freemem && cpattern)
		pcre_free(cpattern);

	first = false;
	cpattern = NULL;
	rc = 0;
	compile_options = 0;
	execute_options = 0;
	error_code = 0;
}

void RegExp::SetOptions(int options)
{
	if(options & UNICODE)
		compile_options |= UNICODE;
	if(options & CASELESS)
		compile_options |= CASELESS;
	if(options & UNGREEDY)
		compile_options |= UNGREEDY;
	if(options & MULTILINE)
		compile_options |= MULTILINE;
	if(options & PARTIAL)
		execute_options = PARTIAL;
}

void RegExp::SetPattern(const char * p)
{
	pattern = p;
}

void RegExp::SetPattern(const String &p)
{
	pattern = p;
}

bool RegExp::Compile(bool recompile)
{
	if(cpattern)
	{
		if(recompile)
			pcre_free(cpattern);
		else
			return true;
	}
	cpattern = pcre_compile2(pattern, compile_options, &error_code, &error_string, &error_offset, NULL);
	return error_code == 0;
}

int RegExp::Execute(const String &t, int offset)
{
	rc = pcre_exec(cpattern, NULL, t, t.GetLength(), offset, execute_options, pos, 30);
	if(rc <= 0)
		first = false;
	return rc;
}

bool RegExp::Match(const String &t, bool copy)
{
	if(copy)
		text = t;
	if(!Compile())
		return false;
	return Execute(t) > 0;
}

bool RegExp::FastMatch(const String &t)
{
	return Match(t, false);
}

bool RegExp::GlobalMatch(const String &t)
{
	if(!first)
	{
		first = true;
		return Match(t);
	}
	int offset = pos[1];
	if(pos[0] == pos[1])
	{
		if(pos[0] == t.GetLength())
		{
			first = false;
			return false;
		}
		execute_options |= PCRE_NOTEMPTY | PCRE_ANCHORED;
	}
	return Execute(t, offset) > 0;
}

String RegExp::operator[](const int i)
{
	return GetString(i);
}

int RegExp::GetCount()
{
	return rc - 1;
}

Vector<String> RegExp::GetStrings()
{
	Vector<String> subs;
	for(int i = 0; i < GetCount(); i++)
		subs.Add(GetString(i));
	return subs;
}

String RegExp::GetString(int i)
{
	i = 2 * (i + 1);
	return text.Mid(pos[i], pos[i + 1] - pos[i]);
}
