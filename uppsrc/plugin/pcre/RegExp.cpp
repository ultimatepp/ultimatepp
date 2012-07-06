#include "Pcre.h"

NAMESPACE_UPP


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
	if(freemem && study)
		pcre_free(study);
	if(freemem && cpattern)
		pcre_free(cpattern);

	first = false;
	cpattern = NULL;
	study = NULL;
	rc = 0;
	compile_options = 0;
	execute_options = 0;
	error_code = 0;
}

void RegExp::SetOptions(int options)
{
	compile_options = options & COMPILE_OPTIONS;
	execute_options = options & EXECUTE_OPTIONS;
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

bool RegExp::Study(bool restudy)
{
	if(!cpattern)
		Compile();
	if(study){
		if(restudy)
			pcre_free(study);
		else
			return true;
	}
	study = pcre_study(cpattern, 0, &error_string);
	if(error_string != NULL)
		error_code = -1; // unfortunatelly, pcre_study doesn't return error codes...
	return error_code == 0;
}

int RegExp::Execute(const String &t, int offset)
{
	rc = pcre_exec(cpattern, study, t, t.GetLength(), offset, execute_options, pos, 30);
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

void RegExp::GetMatchPos(int i, int& iPosStart, int& iPosAfterEnd)
{
	i = 2 * (i + 1);
	iPosStart = pos[i];
	iPosAfterEnd = pos[i + 1];
}

END_UPP_NAMESPACE
