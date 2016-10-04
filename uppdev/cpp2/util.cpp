#include "cpp.h"

void SetSpaces(String& l, int pos, int count)
{
	StringBuffer s = l;
	memset(~s + pos, ' ', count);
	l = s;
}

void RemoveComments(String& l, bool& incomment)
{
	int q = -1;
	int w = -1;
	int lim = l.Find("//");
	if(incomment)
		q = w = 0;
	else {
		q = l.Find("/*");
		if(q >= 0 && q < lim)
			w = q + 2;
	}
	while(q >= 0 && q < lim) {
		int eq = l.Find("*/", w);
		if(eq < 0) {
			incomment = true;
			SetSpaces(l, q, l.GetCount() - q);
			return;
		}
		SetSpaces(l, q, eq + 2 - q);
		incomment = false;
		q = l.Find("/*");
		w = q + 2;
	}
}

const Index<String>& CppKeywordsIndex()
{
	static Index<String> id;
	ONCELOCK {
		const char **cppk = CppKeyword();
		for(int i = 0; cppk[i]; i++)
			id.Add(cppk[i]);
	}
	return id;
}