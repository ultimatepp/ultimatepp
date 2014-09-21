// Copyright (C) 2003 Mirek Fidler, Tomas Rylek. This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License (see COPYING) along with this program; if not, write to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "coff.h"

#ifdef PLATFORM_WIN32
#include <winver.h>
#include <imagehlp.h>
#endif

NAMESPACE_UPP

#ifdef PLATFORM_WIN32

#define DLLFILENAME "imagehlp.dll"
#define DLIHEADER   <coff/imagehlp.dli>
#define DLIMODULE   ImageHlp
#include            <Core/dli.h>

Vector<String> EnumWinRegSubkeys(const char *path, HKEY base_key) {
	HKEY key = 0;
	Vector<String> keys;
	if(RegOpenKeyEx(base_key, path, 0, KEY_READ, &key) != ERROR_SUCCESS)
		return keys;
	for(DWORD index = 0;; index++) {
		char name[1000];
		dword length = sizeof(name);
		FILETIME time;
		if(RegEnumKeyEx(key, index, name, &length, 0, NULL, NULL, &time) != ERROR_SUCCESS)
			break;
		keys.Add(String(name, length));
	}
	RegCloseKey(key);
	return keys;
}

String TranslateCygWinPath(const char *path)
{
	static String base_key = "SOFTWARE\\Cygnus Solutions\\Cygwin\\mounts v2";
	static String cygdrive = GetWinRegString("cygdrive prefix", base_key, HKEY_LOCAL_MACHINE);
	static Vector<String> root = EnumWinRegSubkeys(base_key, HKEY_LOCAL_MACHINE);
	static Vector<String> mount;
	if(mount.IsEmpty()) {
		mount.SetCount(root.GetCount());
		for(int i = 0; i < root.GetCount(); i++)
			mount[i] = GetWinRegString("native", base_key + "\\" + root[i], HKEY_LOCAL_MACHINE);
	}
	if(!memcmp(path, cygdrive, cygdrive.GetLength()) && path[cygdrive.GetLength()] == '/') {
		const char *drv = path + cygdrive.GetLength() + 1;
		if(IsAlpha(*drv) && drv[1] == '/')
			return AppendFileName(String(*drv, 1) + ":\\", drv + 2);
	}
	String bestroot, bestmount, bestrel;
	for(int i = 0; i < root.GetCount(); i++) {
		int l = root[i].GetLength();
		if(!memcmp(path, root[i], l) && (*root[i].Last() == '/' || path[l] == 0 || path[l] == '/')) {
			if(l > bestroot.GetLength()) {
				bestroot = root[i];
				bestmount = mount[i];
				const char *p = path + l;
				if(*p == '/')
					p++;
				bestrel = p;
			}
		}
	}
	if(!IsNull(bestroot))
		return AppendFileName(bestmount, bestrel);

	return Null;
}
#endif

const char *EatPrefix(const char *string, const char *prefix)
{
	int len = (int)strlen(prefix);
	return (!MemICmp(string, prefix, len) ? string + len : NULL);
}

Vector<String> SplitCmdArgs(const char *cmd)
{
	Vector<String> out;
	while(*cmd)
		if((byte)*cmd <= ' ')
			cmd++;
		else
		{
			String arg;
			while(*cmd > ' ')
				if(*cmd == '\"')
				{
					cmd++;
					while(*cmd && (*cmd != '\"' || *++cmd == '\"'))
						arg.Cat(*cmd++);
				}
				else
					arg.Cat(*cmd++);
			out.Add(arg);
		}
	return out;
}

const char *FetchCmdArg(const Vector<String>& cmdlist, int& cmdi)
{
	if(cmdi + 1 >= cmdlist.GetCount())
		throw Exc(NFormat("Argument missing for option '%s'.", cmdlist[cmdi]));
	return cmdlist[++cmdi];
}

const char *FetchCmdArg(const Vector<String>& cmdlist, int& cmdi, const char *ptr)
{
	return (ptr && *ptr ? ptr : FetchCmdArg(cmdlist, cmdi));
}

int ScanCInt(const char *line)
{
	while(*line && (byte)*line <= ' ')
		line++;
	bool sign = (*line == '-');
	if(sign || *line == '+')
		line++;
	if(!IsDigit(*line))
		return Null;
	int out = 0;
	if(*line == '0')
	{
		if(line[1] == 'x')
		{ // hexadecimal
			line += 2;
			if(!IsXDigit(*line))
				return Null;
			for(; IsXDigit(*line); line++)
				out = 16 * out + (*line <= '9' ? *line - '0' : (*line & ~0x20) - 'A' + 10);
		}
		else
		{ // octal
			while(unsigned(*++line - '0') < 8)
				out = 8 * out + *line - '0';
		}
	}
	else
	{ // decimal
		do
			out = 10 * out + *line++ - '0';
		while(IsDigit(*line));
	}
	return sign ? -out : out;
}

static inline bool IsDirSep(char c) { return c == '/' || c == '\\'; }

String NormalizeRelPath(const char *path)
{
	String out;
	while(*path)
	{
		while(IsDirSep(*path))
		{
			out.Cat('\\');
			path++;
		}
		const char *b = path;
		while(*path && !IsDirSep(*path))
			path++;
		if(path - b == 1 && *b == '.')
		{
			if(!out.IsEmpty() && !IsDirSep(*out.Last()))
				out.Cat('\\');
			while(IsDirSep(*path))
				path++;
			continue;
		}
		if(path - b == 2 && *b == '.' && b[1] == '.')
		{
			const char *o = out.Begin(), *e = out.End();
			if(e - o >= 2 && IsDirSep(e[-1]) && !IsDirSep(e[-2]))
			{
				e -= 2;
				while(e > o && !IsDirSep(e[-1]))
					e--;
				out.Trim(int(e - o));
				while(IsDirSep(*path))
					path++;
				continue;
			}
		}
		out.Cat(b, int(path - b));
	}
	return out;
}

int CatN0(Vector<byte>& out, int length)
{
	int l = out.GetCount();
	out.AddN(length);
	memset(&out[l], 0, length);
	return l;
}

int CatN(Vector<byte>& out, int length, const void *in)
{
	int l = out.GetCount();
	if(length > 0)
	{
		out.AddN(length);
		if(in)
			memcpy(&out[l], in, length);
	}
	return l;
}

#ifdef PLATFORM_WIN32
static String WipeClass(const char *p)
{
	String out;
	while(*p)
		if(IsAlpha(*p) || *p == '_')
		{
			const char *b = p++;
			while(IsAlNum(*p) || *p == '_')
				p++;
			if(p - b == 5 && !memcmp(b, "class", 5))
			{
				while(*p == ' ')
					p++;
			}
			else
				out.Cat(b, int(p - b));
		}
		else
			out.Cat(*p++);
	return out;
}
#endif

static inline bool IsIdent(char c) { return IsAlNum(c) || c == '_'; }

static inline const char *ReadIdent(const char *p, int len)
{
	const char *b = p;
	while(IsIdent(*p) && ++p - b < len)
		;
	return p;
}

static const char *DemangleGccDeclarator(String& base, String& prefix, const char *p,
	Vector<String>& templmap, Vector<String>& argmap, bool is_template);

static const char *DemangleGccDeclarator(String& out, const char *p,
	Vector<String>& templmap, Vector<String>& argmap, bool is_template)
{
	String base, prefix;
	p = DemangleGccDeclarator(base, prefix, p, templmap, argmap, is_template);
	out << base;
	if(!IsNull(prefix))
		out << ' ' << prefix;
	return p;
}

static const char *DemangleGccDeclarator(String& out, String& prefix, const char *p,
	Vector<String>& templmap, Vector<String>& argmap, bool is_template)
{
	String base;
	switch(*p++)
	{
	case 'b':
		base << "bool";
		break;

	case 'c':
		base << "char";
		break;

	case 'd':
		base << "double";
		break;

	case 'F':
		p = DemangleGccDeclarator(base, p, templmap, argmap, false);
		prefix = "(" + prefix + ")(";
		while(p && *p && *p != 'E')
		{
			if(*prefix.Last() != '(')
				prefix << ", ";
			p = DemangleGccDeclarator(prefix, p, templmap, argmap, false);
		}
		if(p && *p == 'E')
			p++;
		prefix.Cat(')');
		break;

	case 'h':
		base << "byte";
		break;

	case 'E': // single argument
		p = DemangleGccDeclarator(base, p + 1, templmap, argmap, is_template);
		break;

	case 'I':
		{ // multiple arguments
			String tmpargs;
//			Vector<String> auxmap;
			while(p && *p && *p != 'E')
			{
				if(!tmpargs.IsEmpty())
					tmpargs.Cat(", ");
				String arg;
				p = DemangleGccDeclarator(arg, p, templmap, argmap, is_template);
				tmpargs.Cat(arg);
			}
			if(tmpargs[0] == '<' || !tmpargs.IsEmpty() && *tmpargs.Last() == '>')
				base << "< " << tmpargs << " >";
			else
				base << '<' << tmpargs << '>';
			if(p && *p == 'E')
				p++;
		}
		break;

	case 'i':
		base << "int";
		break;

	case 'j':
		base << "unsigned";
		break;

	case 'K':
		if(!IsNull(prefix))
			prefix = "const " + prefix;
		else
			base << "const ";
		p = DemangleGccDeclarator(base, prefix, p, templmap, argmap, is_template);
		break;

	case 'L':
		if(*p == 'i')
		{
			int num = ScanInt(p + 1, &p);
			if(!IsNull(num))
			{
				base << num;
				break;
			}
		}
		goto UNKNOWN;

	case 'm':
		base << "dword";
		break;

	case 'P':
		prefix = "*" + prefix;
		p = DemangleGccDeclarator(base, prefix, p, templmap, argmap, is_template);
		break;

	case 'R':
		prefix = "&" + prefix;
		p = DemangleGccDeclarator(base, prefix, p, templmap, argmap, is_template);
		break;

	case 'S':
		{
			int equiv = 0;
			if(IsDigit(*p))
			{
				equiv = ScanInt(p, &p);
				if(equiv < 0)
				{
					base << "error(S)";
					return NULL;
				}
			}
			if(*p++ != '_')
			{
				base << "error(S_)";
				return NULL;
			}
			if(!argmap.IsEmpty() && equiv == argmap.GetCount()) // ??
				base.Cat(argmap.Top());
			else if(equiv >= argmap.GetCount())
			{
				base << "error(S" << equiv << "_)";
				return NULL;
			}
			else
				base.Cat(argmap[equiv]);
		}
		break;

	case 'T':
		{
			int equiv = 0;
			if(IsDigit(*p))
			{
				equiv = ScanInt(p, &p);
				if(equiv < 0)
				{
					base << "error(T)";
					return NULL;
				}
			}
			if(*p++ != '_')
			{
				base << "error(T_)";
				return NULL;
			}
			if(!templmap.IsEmpty() && equiv == templmap.GetCount()) // ??
				base.Cat(templmap.Top());
			else if(equiv >= templmap.GetCount())
			{
				base << "error(T" << equiv << "_)";
				return NULL;
			}
			else
				base.Cat(templmap[equiv]);
		}
		break;

	case 't':
		base << "word";
		break;

	case 'v': // void
		base << "void";
		break;

	case 'z':
		base << "...";
		break;

	case 'N':
		{
			bool first = true;
			while(p && *p != 'E')
			{
				if(!IsDigit(*p))
					break;
				int ident = ScanInt(p, &p);
				if(ident <= 0)
					break;
				const char *b = p;
				p = ReadIdent(p, ident);
				if(p - b < ident)
					break;
				if(!first)
					base.Cat("::");
				base.Cat(b, int(p - b));
				if(*p == 'I')
					p = DemangleGccDeclarator(base, p, templmap, argmap, true);
				first = false;
			}
			if(!p)
				base << "(error)";
			else if(*p == 'E')
				p++;
		}
		break;

	default:
		if(IsDigit(*--p))
		{
			int ident = ScanInt(p, &p);
			if(ident <= 0)
				break;
			const char *b = p;
			p = ReadIdent(p, ident);
			if(p - b < ident)
				break;
			base.Cat(b, int(p - b));
			if(*p == 'I')
				p = DemangleGccDeclarator(base, p, templmap, argmap, true);
			break;
		}

	UNKNOWN:
		base << "unknown " << *p;
		return NULL;
	}
	if(is_template)
		templmap.Add(base);
	argmap.Add(base);
	out.Cat(base);
	return p;
}

const char *DemangleOperator(char a, char b)
{
	const char *functor = NULL;
	/**/ if(a == 'e' && b == 'q') functor = "==";
	else if(a == 'n' && b == 'e') functor = "!=";
	else if(a == 'l' && b == 't') functor = "<";
	else if(a == 'l' && b == 'e') functor = "<=";
	else if(a == 'g' && b == 't') functor = ">";
	else if(a == 'g' && b == 'e') functor = ">=";
	else if(a == 'i' && b == 'x') functor = "[]";
	else if(a == 'r' && b == 'm') functor = "%";
	else if(a == 'p' && b == 'L') functor = "+=";
	else if(a == 'a' && b == 'S') functor = "=";
	else if(a == 'p' && b == 'l') functor = "+";
	else if(a == 'l' && b == 's') functor = "<<";
	else if(a == 'l' && b == 'S') functor = "<<=";
	return functor;
}

String DemangleGccName(const char *name)
{
	const char *p = name;
	if(*p++ != '_' || *p++ != '_' || *p++ != 'Z')
		return name;
	String out;
	bool method = (*p == 'N');
	if(method)
		p++;
	bool methconst = (*p == 'K');
	if(methconst)
		p++;
	if(IsDigit(*p))
	{
		if(!IsDigit(*p))
			return name;
		int ident = ScanInt(p, &p);
		if(ident <= 0)
			return name;
		const char *b = p;
		p = ReadIdent(p, ident);
		if(p - b < ident)
			return name;
		out.Cat(b, ident);
	}
	else
	{
		const char *functor = (*p ? DemangleOperator(p[0], p[1]) : NULL);
		if(!functor)
			return name;
		p += 2;
		out << "operator " << functor << ' ';
	}
	Vector<String> templmap, argmap;
	bool retval = false;
	if(*p == 'I')
	{
		String arg;
		p = DemangleGccDeclarator(arg, p, templmap, argmap, true);
		out.Cat(arg);
		retval = (!method);
	}
	if(!p)
	{
		out << "; " << name;
		return out;
	}
	String root = out;
	if(method)
		argmap.Add(out);
	if(method)
	{
		while(p && *p && *p != 'E' && *p != 'I')
		{
			int notempl = root.Find('<');
			if(notempl < 0)
				notempl = root.GetLength();
			if(*p == 'C')
			{
				retval = false;
				out.Cat("::");
				out.Cat(root, notempl);
				p++;
				if(*p != '1' && *p != '2')
					return name;
				p++;
				break;
			}
			if(*p == 'D')
			{
				retval = false;
				out.Cat("::~");
				out.Cat(root, notempl);
				p++;
				if(*p != '0' && *p != '1' && *p != '2')
					return name;
				p++;
				break;
			}
			String subname, functor;
			if(*p == 'c' && p[1] == 'v')
			{
				retval = false;
				p += 2;
				subname << "operator ";
				p = DemangleGccDeclarator(subname, p, templmap, argmap, false);
			}
			else if(*p && !IsNull(functor = DemangleOperator(p[0], p[1])))
				p += 2;
			else if(!IsDigit(*p))
				return name;
			else
			{
				int ident = ScanInt(p, &p);
				if(ident <= 0)
					return name;
				const char *b = p;
				p = ReadIdent(p, ident);
				if(p - b < ident)
					return name;
				subname = String(b, ident);
			}
			if(!IsNull(functor))
				subname << "operator " << functor << ' ';
			out.Cat("::");
			out.Cat(subname);
			root = subname;
//			if(*p == 'I' || *p == 'E')
//				p = DemangleGccDeclarator(subname, p, templmap, argmap, false);
		}
		if(!p || *p != 'E')
			return name;
		p++;
	}
	if(retval)
	{
		String rvl;
		p = DemangleGccDeclarator(rvl, p, templmap, argmap, false);
		if(!rvl.IsEmpty() && IsIdent(*rvl.Last()))
			rvl.Cat(' ');
		rvl.Cat(out);
		out = rvl;
	}
	out.Cat('(');
	while(p && *p)
	{
		if(*out.Last() != '(')
			out.Cat(", ");
		String arg;
		p = DemangleGccDeclarator(arg, p, templmap, argmap, false);
		out.Cat(arg);
		argmap.Add(arg);
	}
	out.Cat(')');
	if(methconst)
		out.Cat(" const");
//	if(!p)
		out << "; " << name;
	return out;
}

String DemangleName(String name, int mangling_style)
{
	String out = name;
#ifdef PLATFORM_WIN32
	if(mangling_style == MANGLING_MSC && ImageHlp())
	{
		char buffer[1024];
		ImageHlp().UnDecorateSymbolName(name, buffer, sizeof(buffer),
			UNDNAME_NO_ACCESS_SPECIFIERS | UNDNAME_NO_MEMBER_TYPE | UNDNAME_NO_MS_KEYWORDS);
		out = WipeClass(buffer);
	}
	else
#endif
	if(mangling_style == MANGLING_GCC)
		out = DemangleGccName(name);
	return out;
}


String NormalizePathCase(String file)
{
#if PATH_HAS_CASE
	return file;
#else
	return ToLower(file);
#endif
}

bool EqualsPathCase(String f1, String f2)
{
#if PATH_HAS_CASE
	return f1 == f2;
#else
	return !CompareNoCase(f1, f2);
#endif
}

void PutStdOut(const char *text)
{
	puts(text);
	fflush(stdout);
}

String MaxLenString(const byte *b, int maxlen)
{
	const byte *e = b + maxlen;
	while(e > b && e[-1] == 0)
		e--;
	return String(b, int(e - b));
}

END_UPP_NAMESPACE
