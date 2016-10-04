#ifndef _src_CSV_csv_h_
#define _src_CSV_csv_h_

#include <Core/Core.h>
#include <MySupport/support.h>

using namespace Upp;

Vector<Value> CSVStringToToken(const String &s);
String CSVDequote(const String &s);
String CSVQuote(const String &s);

#endif
