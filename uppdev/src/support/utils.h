#ifndef _src_support_utils_h_
#define _src_support_utils_h_

// functions
String pathToString(const char *p, bool last_sep = true);
int StringSearch(const String &string, const String& key);
int CountRecurrence(const String &s, const String &k);
String RemoveQuote(const String &str);
bool IsQuoted(const String &s);

bool isBitSet(int _byte, int _bitno);
void bitSet(int *ptr, int _bitno, bool b=true);

String GetVolumeSn(const String &vol, int len);
unsigned int GetLockNumber(const String& pcID);

String MyKeyReplace(const String& raw, const String& key, const String& value);

#endif
