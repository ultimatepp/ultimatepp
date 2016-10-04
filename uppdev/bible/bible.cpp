#include <Core/Core.h>

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <set>

using namespace Upp;
using namespace std;

#ifdef PLATFORM_POSIX
#define DIR "/home/cxl/"
#else
#define DIR "c:/"
#endif

void StdSortBible()
{
   vector<string> buf;
   string linBuf;
   { RTIMING("InStd");
   ifstream inFile(DIR "bible.txt");
   buf.reserve(50000);
   while(getline(inFile,linBuf)) buf.insert(buf.end(), linBuf);
   }
   { RTIMING("SortStd");
   sort(buf.begin(), buf.end());
   }
   { RTIMING("OutStd");
   ofstream outFile(DIR "output1.txt");
   copy(buf.begin(),buf.end(),ostream_iterator<string>(outFile,"\n"));
   }
}

void StdSortBible2()
{
   multiset<string> buf;
   string linBuf;
   ifstream inFile(DIR "bible.txt");
   while(getline(inFile,linBuf))
       buf.insert(linBuf);
   ofstream outFile("output.txt");
   copy(buf.begin(),buf.end(),ostream_iterator<string>(outFile,"\n"));
}

void UppSortBible()
{
	Vector<String> v;
	{ RTIMING("InUpp");
		FileIn in(DIR "bible.txt");
		{
			while(!in.IsEof())
				v.Add(in.GetLine());
		}
	}
	{ RTIMING("SortUpp");
	Sort(v);
	}
	{ RTIMING("OutUpp");
	FileOut out(DIR "output2.txt");
	for(int i = 0; i < v.GetCount(); i++) {
		{
		  out << v[i];
		}
		{ out << "\r\n"; }
	}
	}
}

CONSOLE_APP_MAIN
{
	{
		RTIMING("std");
		StdSortBible();
	}
	{
		RTIMING("std multiset");
		StdSortBible2();
	}
	{
		RTIMING("upp");
		UppSortBible();
	}
	ASSERT(LoadFile(DIR "output.txt") == LoadFile(DIR "output2.txt"));
}
