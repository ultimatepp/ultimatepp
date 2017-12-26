// Copyright (C) 2003 Mirek Fidler, Tomas Rylek. This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License (see COPYING) along with this program; if not, write to the Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef _console_coff_util_h_
#define _console_coff_util_h_

#ifdef __offsetof
	#undef __offsetof
#endif

#ifdef PLATFORM_BSD
#include <sys/types.h>
#include <sys/stat.h>
#endif

#define __offsetof(obj, item) (int(&reinterpret_cast<obj *>(1)->item) - 1)

inline bool    IsCIdent(char c) { return IsAlNum(c) || c == '_'; }

Vector<String> SplitCmdArgs(const char *cmdline);
const char    *FetchCmdArg(const Vector<String>& cmdlist, int& cmdi);
const char    *FetchCmdArg(const Vector<String>& cmdlist, int& cmdi, const char *ptr);
const char    *EatPrefix(const char *string, const char *prefix);
int            ScanCInt(const char *line);
String         NormalizeRelPath(const char *path);
int            CatN0(Vector<byte>& out, int length);
int            CatN(Vector<byte>& out, int length, const void *in);
String         NormalizePathCase(String file);
bool           EqualsPathCase(String f1, String f2);
enum { MANGLING_NONE, MANGLING_MSC, MANGLING_GCC };
String         DemangleName(String name, int mangling_style);
void           PutStdOut(const char *text);
String         MaxLenString(const byte *b, int maxlen);
#ifdef PLATFORM_WIN32
String         TranslateCygWinPath(const char *path);
#endif
#endif
