#include "bm.h"

using namespace Upp;

int ffind(const char *haystack, int len, const char *needle, int nsize)
{
    if (!nsize) return -1;
    // nsize + pos can overflow (eg pos == npos), guard against that by checking
    // that nsize + pos does not wrap around.
    if (nsize > len) return -1;
    // Don't use std::search, use a Boyer-Moore-like trick by comparing
    // the last characters first
    int nsize_1 = nsize - 1;
    int lastNeedle = needle[nsize_1];

    // Boyer-Moore skip value for the last char in the needle. Zero is
    // not a valid value; skip will be computed the first time it's
    // needed.
    int skip = 0;

    const char *i = haystack;
    const char *iEnd = haystack + len - nsize_1;

    while (i < iEnd) {
      // Boyer-Moore: match the last element in the needle
      while (i[nsize_1] != lastNeedle) {
        if (++i == iEnd) {
          // not found
          return -1;
        }
      }
      // Here we know that the last char matches
      // Continue in pedestrian mode
      for (int j = 0; ; ) {
        if (i[j] != needle[j]) {
          // Not found, we can skip
          // Compute the skip value lazily
          if (skip == 0) {
            skip = 1;
            while (skip <= nsize_1 && needle[nsize_1 - skip] != lastNeedle) {
              ++skip;
            }
          }
          i += skip;
          break;
        }
        // Check if done searching
        if (++j == nsize) {
          // Yay
          return i - haystack;
        }
      }
    }
    return -1;
}

int ffind(const char *haystack, const char *needle)
{
	return ffind(haystack, strlen(haystack), needle, strlen(needle));
}

void test_find(const char *data, const char *pattern)
{
	LOG("=======================");
	LOG("Haystack: " << data);
	LOG("Needle: " << pattern);
	LOG("Folly: " << ffind(data, pattern));
	
	BMFinder bm(pattern);
	uint8_t *s = boyer_moore((uint8_t *)data, strlen(data), (uint8_t *)pattern, strlen(pattern));
	LOG("Boyer-Moore0: " << (s ? (char *)s - data : -1));
	LOG("Boyer-Moore1: " << bm.FindIn(data));
}

void DoTest(const String& data, const String& needle)
{
	RLOG("------------");
	RLOG("Needle: " << needle << " (" << needle.GetCount() << ")");
	{
		TimeStop tm;
		RLOG("U++ Brute force: " << data.Find(needle));
		RLOG("  Time elapsed: " << tm);
	}
	{
		TimeStop tm;
		void *t = memmem(data, data.GetLength(), needle, needle.GetLength());
		RLOG("memmem: " << (t ? (char *)t - ~data : -1));
		RLOG("  Time elapsed: " << tm);
	}
	{
		TimeStop tm;
		RLOG("Folly: " << ffind(data, data.GetLength(), needle, needle.GetCount()));
		RLOG("  Time elapsed: " << tm);
	}
#if 0
	{
		TimeStop tm;
		uint8_t* t = boyer_moore ((uint8_t *)~data, data.GetCount(),
		                          (uint8_t *)~needle, needle.GetCount());
		RLOG("Boyer-Moore C: " << (t ? (char *)t - ~data : -1));
		RLOG("  Time elapsed: " << tm);
	}
	{
		TimeStop tm;
		BMFinder bm(needle);
		RLOG("Boyer-Moore: " << bm.FindIn(data));
		RLOG("  Time elapsed: " << tm);
	}
#endif
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	String data, needle, n;

/*
	test_find("zxcvzxcvahoj", "ahoj");
	test_find("aahoj", "ahoj");
	test_find("jojajo", "ahoj");
	test_find("jojajohadjohajohahojaj", "ahoj");
	int n = String("0123456789").Find("012345");
	RDUMP(n);
*/	

	RLOG("====================================================");
	RLOG("**** xml file");
	data = LoadFile("/home/cxl/20131117_ST_ZZSZ.xml") + "Hello world!";
	DoTest(data, "Hello world");
	DoTest(data, "Hel");
	
	RLOG("====================================================");
	RLOG("**** a..ab");
	data = String('a', 100000000) + "b";
	DoTest(data, "b");
	DoTest(data, "ab");
	DoTest(data, "aab");
	DoTest(data, "aaab");
	DoTest(data, "aaaab");
	DoTest(data, "aaaaaaaaaaaaab");
	DoTest(data, "aaaaaaaaaaaaaaaaaab");
	DoTest(data, String('a', 100) + "b");

	RLOG("====================================================");
	RLOG("**** a..aba..a");
	data = String('a', 50000000) + "b" + String('a', 50000000);
	DoTest(data, "ab");
	DoTest(data, "aba");
	DoTest(data, "aaba");
	DoTest(data, "aabaa");
	DoTest(data, "aaaabaaaa");
	DoTest(data, "aaaaaabaaaaaa");
	DoTest(data, String('a', 20) + "b" + String('a', 20));
	DoTest(data, String('a', 100) + "b");
	
	RLOG("====================================================");
	RLOG("**** 10000000 * Hello_world!");
	data.Clear();
	for(int i = 0; i < 10000000; i++)
		data << "Hello_world!";
	data << "Hello world!";
	DoTest(data, "Hello world!");

	RLOG("====================================================");
	RLOG("**** all chars repeated");
	for(int i = 32; i < 127; i++)
		needle.Cat(i);
	data.Clear();
	for(int i = 0; i < 1000000; i++)
		data << needle;
	needle << 'x';
	data << needle;
	DoTest(data, needle);

	RLOG("====================================================");
	RLOG("**** all chars repeated, space in middle");
	needle.Clear();
	for(int i = 32; i < 127; i++)
		needle.Cat(i);
	data.Clear();
	n = needle;
	n.Set(60, ' ');
	for(int i = 0; i < 1000000; i++)
		data << n;
	data << needle << data;
	DoTest(data, needle);
}
