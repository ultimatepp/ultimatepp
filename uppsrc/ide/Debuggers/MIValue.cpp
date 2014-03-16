#include "MIValue.h"

//#define MITUPLE_DUMP_MARKERS

static MIValue &NullMIValue(void)
{
	static MIValue v;
	return v;
}

static MIValue &ErrorMIValue(String const &msg)
{
	static MIValue v;
	v.SetError(msg);
	return v;
}

bool MIValue::expect(String const &where, char exp, int i, String const &s)
{
	if(s[i] == exp)
		return true;
	int start = i - 30;
	if(start < 0)
		start = 0;
	if(!s[i])
		SetError(Format(where + " : Expected '%c', got end of string at pos %d around '%s'", exp, i, s.Mid(start, 60)));
	else
		SetError(Format(where + " : Expected '%c', got '%c' at pos %d in '%s'", exp, s[i], i, s.Mid(start, 60)));
	return false;
}

static char backslash(String const &s, int &i)
{
	i++;
	if(IsDigit(s[i]))
	{
		char c = (s[i]-'0')*64 + (s[i+1]-'0')*8 + s[i+2]-'0';
		i += 2;
		return c;
	}

	// control chars and octals
	switch(s[i])
	{
		case 'a' :
			return '\a';
		case 'b' :
			return '\b';
		case 'f' :
			return '\f';
		case 'n' :
			return '\n';
		case 'r' :
			return '\r';
		case 't' :
			return '\t';
		case 'v' :
			return '\v';
		default:
			return s[i];
	}
}
	

int MIValue::ParsePair(String &name, MIValue &val, String const &s, int i)
{
	name.Clear();
	val.Clear();
	while(s[i] && isspace(s[i]))
		i++;
	if(!s[i])
	{
		SetError("ParsePair:Unexpected end of string");
		return i;
	}
	
	// is starting wirh '[' or '{' take it as a value with empty name
	if(s[i] == '{' || s[i] == '[')
	{
		name = "<UNNAMED>";
		return val.ParseTuple(s, i);
	}
	else
	{
		int aCount = 0;
		while(s[i] && ((s[i] != '=' && s[i] != '}' && s[i] != ']' && s[i] != ',') || aCount))
		{
			if(s[i] == '<')
				aCount++;
			else if(s[i] == '>')
				aCount--;
			if(s[i] == '\\')
				name.Cat(backslash(s, i));
			else
				name.Cat(s[i]);
			i++;
			
			// skip blanks if not inside <>
/*
			if(!aCount)
				while(s[i] && isspace(s[i]))
					i++;
*/
		}
		while(s[i] && isspace(s[i]))
			i++;

		if(s[i] != '=')
		{
			// we take the data without = as the value part
			// of keyless tuple...
			val.Set(name);
			name = "<UNNAMED>";
			return i;
		}
		i++;

		while(s[i] && isspace(s[i]))
			i++;
	}

	// skip address part before a tuple start, if any... it's useless and confuses the parser
	if(s[i] == '@')
	{
		int j = i;
		while(s[j] && s[j] != ':')
			j++;
		if(s[j] == ':')
			j++;
		while(s[j] && IsSpace(s[j]))
			j++;
		if(s[j] == '{')
			i = j;
	}

	switch(s[i])
	{
		case '"':
			i = val.ParseString(s, i);
			break;
		break;
		
		case '[':
			i = val.ParseArray(s, i);
			break;
			
		case '{':
			i = val.ParseTuple(s, i);
			break;
			
		default:
			i = val.ParseUnquotedString(s, i);
			break;
	}

	return i;
}

int MIValue::ParseTuple(String const &s, int i)
{
	Clear();
	type = MITuple;
	
	// drop opening delimiter
	if(!expect("ParseTuple", '{', i, s))
		return s.GetCount();
	i++;
	while(s[i] && s[i] != '}')
	{
		while(s[i] && isspace(s[i]))
			i++;
		String name;
		MIValue val;
		i = ParsePair(name, val, s, i);
		tuple.AddPick(name, pick(val));
		while(s[i] && isspace(s[i]))
			i++;
		if(s[i] == '}')
			break;
		if(!expect("ParseTuple", ',', i, s))
			return s.GetCount();
		i++;
	}

	return i + 1;
}

int MIValue::ParseArray(String const &s, int i)
{
	Clear();
	type = MIArray;
	
	// drop opening delimiter
	if(!expect("ParseArray", '[', i, s))
		return s.GetCount();
	i++;
	while(s[i] && isspace(s[i]))
		i++;
	while(s[i] && s[i] != ']')
	{
		while(s[i] && isspace(s[i]))
			i++;
		String name;
		MIValue val;
		if(s[i] == '[')
			i = val.ParseArray(s, i);
		else if(s[i] == '{')
			i = val.ParseTuple(s, i);
		else if(s[i] == '"')
			i = val.ParseString(s, i);
		else if(s[i] == '<')
			i = val.ParseAngle(s, i);
		else
			i = ParsePair(name, val, s, i);
		array.Add() = pick(val);
		while(s[i] && isspace(s[i]))
			i++;
		if(s[i] == ']')
			break;
		if(!expect("ParseArray", ',', i, s))
			return s.GetCount();
		i++;
	}
	return i + 1;
}

int MIValue::ParseString(String const &s, int i)
{
	Clear();
	type = MIString;

	if(!expect("ParseString", '"', i, s))
		return s.GetCount();
	i++;
	while(s[i])
	{
		// verbatim if escaped
		if(s[i] == '\\')
			string.Cat(backslash(s, i));
		else if(s[i] == '"')
		{
			i++;
			break;
		}
		else
			string.Cat(s[i]);
		i++;
	}
	if(!expect("ParseString", '"', i-1, s))
		return s.GetCount();

	return i;
}

int MIValue::ParseAngle(String const &s, int i)
{
	Clear();
	type = MIString;
	int aCount = 0;

	if(!expect("ParseAngle", '<', i, s))
		return s.GetCount();
	string = "<";
	aCount++;
	i++;
	while(s[i])
	{
		// verbatim if escaped
		if(s[i] == '\\')
			string.Cat(backslash(s, i));
		else if(s[i] == '>' && !--aCount)
		{
			i++;
			break;
		}
		else
		{
			string.Cat(s[i]);
			if(s[i] == '<')
				aCount++;
		}
		i++;
	}
	if(!expect("ParseAngle", '>', i-1, s))
		return s.GetCount();
	string.Cat('>');

	return i;
}

// sigh
static bool comma(String const &s, int i)
{
	if(s[i] != ',')
		return false;
	if(!i)
		return true;
	if(IsDigit(s[i-1]) && IsDigit(s[i+1]))
		return false;
	return true;
}

// we can hava a non-quoted string... so we read up
// to terminator, which can be '}', ']' or ','
int MIValue::ParseUnquotedString(String const &s, int i)
{
	String valStr;
	int aCount = 0;
	bool inQuote = false;

	while(s[i] && ((s[i] != '=' && s[i] != '}' && s[i] != ']' && !comma(s, i)) || inQuote || aCount))
	{
		valStr.Cat(s[i]);
		if(s[i] == '\\')
		{
			i++;
			if(s[i])
				valStr.Cat(s[i++]);
			continue;
		}
		if(s[i] == '<' && !inQuote)
			aCount++;
		else if(s[i] == '>' && !inQuote)
			aCount--;
		else if(s[i] == '"' && !aCount)
			inQuote = !inQuote;
		i++;
	}
	type = MIString;
	string = valStr;

	return i;
}

int MIValue::Parse(String const &s, int i)
{
	// if starts with '"', it's a string
	// if starts with '[', it's an array
	// if starts with '{', it's a tuple
	// otherwise, it can be a sequence of pair name="value" which is stored like a tuple
	// latter case is an example o bad design of MI interface....
	Clear();
	while(s[i] && isspace(s[i]))
		i++;
	if(s[i] == '"')
		return ParseString(s, i);
	else if(s[i] == '<')
		return ParseAngle(s, i);
	else if(s[i] == '[')
		return ParseArray(s, i);
	else if(s[i] == '{')
		return ParseTuple(s, i);
	else
	{
		String name;
		MIValue val;
		type = MITuple;
		while(s[i])
		{
			i = ParsePair(name, val, s, i);
			tuple.AddPick(name, pick(val));
			while(s[i] && isspace(s[i]))
				i++;
			if(s[i] != ',')
				break;
			i++;
		}

		return i;
	}
}

MIValue &MIValue::operator=(MIValue rval_ v)
{
	Clear();
	type = pick(v.type);
	switch(type)
	{
		case MIString:
			string = v.string;
			break;
		case MIArray:
			array = pick(v.array);
			break;
		case MITuple:
			tuple = pick(v.tuple);
			break;
		default:
			SetError("Unknown MIValue type");
	}
	return *this;
}

MIValue::MIValue()
{
	Clear();
}

MIValue::MIValue(MIValue rval_ v)
{
	Clear();
	type = v.type;
	switch(type)
	{
		case MIString:
			string = v.string;
			break;
		case MIArray:
			array = pick(v.array);
			break;
		case MITuple:
			tuple = pick(v.tuple);
			break;
		default:
			SetError("Unknown MIValue type");
	}
}

MIValue::MIValue(String const &s)
{
	Parse(s);
	
	// tuple with 1 element and unnamed key is a string
	if(IsTuple() && tuple.GetCount() == 1 && tuple.GetKey(0) == "<UNNAMED>")
	{
		type = MIString;
		string = tuple[0];
		tuple.Clear();
	}
}

MIValue &MIValue::operator=(String const &s)
{
	Parse(s);
	// tuple with 1 element and unnamed key is a string
	if(IsTuple() && tuple.GetCount() == 1 && tuple.GetKey(0) == "<UNNAMED>")
	{
		type = MIString;
		string = tuple[0];
		tuple.Clear();
	}
	return *this;
}

void MIValue::Clear()
{
	type = MIString;
	string = "";
	array.Clear();
	tuple.Clear();
}

// sets value to an error condition
MIValue &MIValue::SetError(String const &msg)
{
	type = MIString;
	string = "error:" + msg;
	return *this;
}

// check if value contains an error
bool MIValue::IsError(void) const
{
	return type == MIString && string.StartsWith("error:");
}
		
// check for emptyness
bool MIValue::IsEmpty(void) const
{
	return type == MIString && string == "";
}

// simple accessors
int MIValue::GetCount(void) const
{
	if(IsError())
		return 0;
	if(type == MIArray)
		return array.GetCount();
	else if(type == MITuple)
		return tuple.GetCount();
	else
		return string.GetCount();
}

int MIValue::Find(const char *key) const
{
	if(type != MITuple)
		return -1;
	return tuple.Find(key);
}

MIValue &MIValue::Get(int i)
{
	if(IsError())
		return *this;
	if(type == MIArray)
		return array[i];
	if(type == MITuple)
		return tuple[i];
	return ErrorMIValue("Not an Array value type");
}

MIValue const &MIValue::Get(int i) const
{
	if(IsError())
		return *this;
	if(type == MIArray)
		return array[i];
	if(type == MITuple)
		return tuple[i];
	return ErrorMIValue("Not an Array value type");
}

MIValue &MIValue::Get(const char *key)
{
	if(type != MITuple)
		return ErrorMIValue("Not a Tuple value type");
	if(tuple.Find(key) < 0)
		return ErrorMIValue(String("key '") + key + "' not found");
	return tuple.Get(key);
}

MIValue const &MIValue::Get(const char *key) const
{
	if(type != MITuple)
		return ErrorMIValue("Not a Tuple value type");
	if(tuple.Find(key) < 0)
		return ErrorMIValue(String("key '") + key + "' not found");
	return tuple.Get(key);
}

String &MIValue::Get(void)
{
	if(type != MIString)
		return ErrorMIValue("Not a String value type");
	return string;
}
		
String const &MIValue::Get(void) const
{
	if(type != MIString)
		return ErrorMIValue("Not a String value type");
	return string;
}
		
// tuple string member accessor with default value if not found
String MIValue::Get(const char *key, const char *def) const
{
	if(type != MITuple)
		return  ErrorMIValue("Not a Tuple value type");
	int i = tuple.Find(key);
	if(i >= 0)
	{
		if(tuple[i].type != MIString)
			return def;
		return tuple[i].Get();
	}
	else
		return def;
}

// gets key by index for tuple values
String MIValue::GetKey(int idx) const
{
	if(type != MITuple)
		return  ErrorMIValue("Not a Tuple value type");
	return tuple.GetKey(idx);
}

void MIValue::Set(String const &s)
{
	Clear();
	type = MIString;
	string = s;
}

// data dump
#ifdef MITUPLE_DUMP_MARKERS
	#define MARK_STRING	"<STRING>"
	#define MARK_ARRAY	"<ARRAY>"
	#define MARK_TUPLE	"<TUPLE>"
#else
	#define MARK_STRING	""
	#define MARK_ARRAY	""
	#define MARK_TUPLE	""
#endif

// dumps a string with special chars inside
String MIValue::Dump(String const &s)
{
	String res;
	for(int i = 0; i < s.GetCharCount(); i++)
	{
		byte c = s[i];
		if(isprint(c))
			res << c;
		else
			res += Format("\\%03o", c);
	}
	return res;
}
String MIValue::Dump(int level) const
{
	String spacer(' ', level);
	switch(type)
	{
		case MIString:
			return spacer + MARK_STRING + Dump(string);

		case MITuple:
		{
			String s = spacer + MARK_TUPLE + "{\n";
			level += 4;
			spacer = String(' ', level);
			for(int i = 0; i < tuple.GetCount(); i++)
			{
				String s1 = spacer + tuple.GetKey(i) + "=";
				s += s1;
				MIValue const &val = tuple[i];
				if(val.type == MIString)
					s += val.Dump();
				else
				{
					s += '\n' + val.Dump(level + 4);
					s = s.Left(s.GetCount()-1);
				}
				if(i < tuple.GetCount() - 1)
					s += ',';
				s += '\n';
			}
			level -= 4;
			spacer = String(' ', level);
			s += spacer + "}\n";
			return s;
		}

		case MIArray:
		{
			String s = spacer + MARK_ARRAY + "[ \n";
			level += 4;
			for(int i = 0; i < array.GetCount(); i++)
			{
				MIValue const &val = array[i];
				s += val.Dump(level);
				if(val.type != MIString)
					s = s.Left(s.GetCount()-1);
				if(i < array.GetCount() - 1)
					s += ',';
				s += '\n';
			}
			s += spacer + "]\n";
			return s;
		}
		
		default:
			return spacer + "*UNKNOWN MIVALUE TYPE*";
	}
}

// finds breakpoint data given file and line
MIValue &MIValue::FindBreakpoint(String const &file, int line)
{
	MIValue &body = Get("body");
	if(body.IsError() || !body.IsArray())
		return NullMIValue();
	for(int i = 0; i < body.GetCount(); i++)
	{
		MIValue &bp = body[i];
		if(bp.IsError() || !bp.IsTuple())
			return NullMIValue();
		if(bp["file"] == file && atoi(bp["line"].Get()) == line)
			return bp;
	}
	return NullMIValue();
}

static String PackName(String const &name)
{
	String res;
	const char *c = ~name;
	while(*c)
	{
		if(!IsSpace(*c))
			res.Cat(*c);
		c++;
	}
	return res;
}

// packs names inside tuples -- to make type recognition easy
void MIValue::PackNames(void)
{
	if(type == MITuple)
	{
		for(int i = 0; i < tuple.GetCount(); i++)
		{
			tuple.SetKey(i, PackName(tuple.GetKey(i)));
			tuple[i].PackNames();
		}
	}
	else if(type == MIArray)
	{
		for(int i = 0; i < array.GetCount(); i++)
			array[i].PackNames();
	}
}

// fix arrays -- i.e. replace a tuple containing ALL unnamed elements
// with the corresponding array
// ( gdb evaluator array data is returned as tuple with {} instead []=
void MIValue::FixArrays(void)
{
	
	bool named = false;
	if(IsTuple())
	{
		for(int iVal = 0; iVal < tuple.GetCount(); iVal++)
			if(tuple.GetKey(iVal) != "<UNNAMED>")
			{
				named = true;
				break;
			}
		if(!named)
		{
			array.Clear();
			for(int iVal = 0; iVal < tuple.GetCount(); iVal++)
				array.Add() = pick(tuple[iVal]);
			tuple.Clear();
			type = MIArray;
		}
	}
	if(IsTuple() || IsArray())
		for(int i = 0; i < GetCount(); i++)
			Get(i).FixArrays();
}

// add an item to a tuple
MIValue &MIValue::Add(String const &key, MIValue rval_ v)
{
	if(IsEmpty())
	{
		Clear();
		type = MITuple;
	}
	if(type != MITuple)
		return  ErrorMIValue("Not a Tuple value type");
	tuple.AddPick(key, pick(v));
	return *this;
}

MIValue &MIValue::Add(String const &key, String const &data)
{
	MIValue v;
	v.Set(data);
	return Add(key, v);
}
		
MIValue &MIValue::FindAdd(String const &key, String const &data)
{
	if(IsEmpty())
	{
		Clear();
		type = MITuple;
	}
	if(type != MITuple)
		return  ErrorMIValue("Not a Tuple value type");
	int idx = tuple.Find(key);
	MIValue v;
	v.Set(data);
	if(idx >= 0)
		tuple[idx] = pick(v);
	else
		tuple.AddPick(key, pick(v));
	return *this;
}

// add an item to an array
MIValue &MIValue::Add(MIValue rval_ v)
{
	if(IsEmpty())
	{
		Clear();
		type = MIArray;
	}
	if(type != MIArray)
		return  ErrorMIValue("Not a Array value type");
	array.AddPick(pick(v));
	return *this;
}

MIValue &MIValue::Add(String const &data)
{
	MIValue v;
	v.Set(data);
	return Add(v);
}

// remove a tuple key
MIValue &MIValue::Remove(String const &key)
{
	if(type != MITuple)
		return *this;
	tuple.RemoveKey(key);
	return *this;
}
