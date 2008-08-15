#ifndef _src_dbase_compare_h_
#define _src_dbase_compare_h_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

bool 	IsEqual(const Value &val1, const String &s);
bool 	IsBigger(const Value &val1, const String &s);
bool 	IsSmaller(const Value &val1, const String &s);
bool 	IsBiggerEqual(const Value &val1, const String &s);
bool 	IsSmallerEqual(const Value &val1, const String &s);

#endif
