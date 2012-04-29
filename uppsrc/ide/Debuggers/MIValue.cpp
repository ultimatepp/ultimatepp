#include "MIValue.h"

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

int MIValue::ParsePair(String &name, MIValue &val, String const &s, int i)
{
	name.Clear();
	val.Clear();
	while(s[i] && (s[i] != '=') && s[i] != ']' && s[i] != '}')
		name.Cat(s[i++]);
	if(s[i] != '=')
		return i;
	i++;
	switch(s[i])
	{
		case '"':
		{
			i = val.ParseString(s, i);
			break;
		}
		break;
		
		case '[':
			i = val.ParseArray(s, i);
			break;
			
		case '{':
			i = val.ParseTuple(s, i);
			break;
			
		default:
			NEVER();
	}
	return i;
}

int MIValue::ParseTuple(String const &s, int i)
{
	Clear();
	type = MITuple;
	
	// drop opening delimiter
	if(s[i] != '{')
	{
		SetError(Format("Expected '{' at pos %d in '%s'", i, s));
		return s.GetCount();
	}
	i++;
	while(s[i] && s[i] != '}')
	{
		String name;
		MIValue val;
		i = ParsePair(name, val, s, i);
		tuple.Add(name, val);
		if(s[i] == '}')
			break;
		if(s[i] != ',')
		{
			SetError(Format("Expected ',' at pos %d in '%s'", i, s));
			return s.GetCount();
		}
		i++;
	}
	return i + 1;
}

int MIValue::ParseArray(String const &s, int i)
{
	Clear();
	type = MIArray;
	
	// drop opening delimiter
	if(s[i] != '[')
	{
		SetError(Format("Expected '[' at pos %d in '%s'", i, s));
		return s.GetCount();
	}
	i++;
	while(s[i] && s[i] != ']')
	{
		String name;
		MIValue val;
		if(s[i] == '[')
			i = val.ParseArray(s, i);
		else if(s[i] == '{')
			i = val.ParseTuple(s, i);
		else if(s[i] == '"')
			i = val.ParseString(s, i);
		else
			i = ParsePair(name, val, s, i);
		array.Add(val);
		if(s[i] == ']')
			break;
		if(s[i] != ',')
		{
			SetError(Format("Expected ',' at pos %d in '%s'", i, s));
			return s.GetCount();
		}
		i++;
	}
	return i + 1;
}

int MIValue::ParseString(String const &s, int i)
{
	Clear();
	type = MIString;

	char c;
	if(s[i] != '"')
	{
		SetError(Format("Expected '\"' at pos %d in '%s'", i, s));
		return s.GetCount();
	}
	i++;
	while( (c = s[i++]) != 0)
	{
		// verbatim if escaped
		if(c == '\\')
			string.Cat(s[i++]);
		else if(c == '"')
			break;
		else
			string.Cat(c);
	}
	if(c != '"')
	{
		SetError(Format("Expected '\"' at pos %d in '%s'", i, s));
		return s.GetCount();
	}
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
	if(s[i] == '"')
		return ParseString(s, i);
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
			tuple.Add(name, val);
			if(s[i] != ',')
				break;
			i++;
		}
		return i;
	}
}

MIValue &MIValue::operator=(pick_ MIValue &v)
{
	Clear();
	type = v.type;
	switch(type)
	{
		case MIString:
			string = v.string;
			break;
		case MIArray:
			array = v.array;
			break;
		case MITuple:
			tuple = v.tuple;
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

MIValue::MIValue(MIValue pick_ &v)
{
	Clear();
	type = v.type;
	switch(type)
	{
		case MIString:
			string = v.string;
			break;
		case MIArray:
			array = v.array;
			break;
		case MITuple:
			tuple = v.tuple;
			break;
		default:
			SetError("Unknown MIValue type");
	}
}

MIValue::MIValue(String const &s)
{
	Parse(s);
}

MIValue &MIValue::operator=(String const &s)
{
	Parse(s);
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
bool MIValue::IsError(void)
{
	return type == MIString && string.StartsWith("error:");
}
		
// check for emptyness
bool MIValue::IsEmpty(void)
{
	return type == MIString && string == "";
}

// simple accessors
int MIValue::GetCount(void) const
{
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
	if(type != MIArray)
		return ErrorMIValue("Not an Array value type");
	return array[i];
}

MIValue &MIValue::Get(const char *key)
{
	if(type != MITuple)
		return ErrorMIValue("Not a Tuple value type");
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
		return def;
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

// data dump
String MIValue::Dump(int level)
{
	String spacer(' ', level);
	switch(type)
	{
		case MIString:
			return spacer + string;

		case MITuple:
		{
			String s = spacer + "{\n";
			level += 4;
			spacer = String(' ', level);
			for(int i = 0; i < tuple.GetCount(); i++)
			{
				String s1 = spacer + tuple.GetKey(i) + "=";
				s += s1;
				MIValue &val = tuple[i];
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
			String s = spacer + "[ \n";
			level += 4;
			for(int i = 0; i < array.GetCount(); i++)
			{
				MIValue &val = array[i];
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
