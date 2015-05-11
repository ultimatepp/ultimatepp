#include "Browser.h"

void SplitCodeRef(const String& s, String& scope, String& item)
{
	int q = s.FindFirstOf("( ");
	q = q >= 0 ? s.ReverseFind(':', q) : s.ReverseFind(':');
	if(q < 0) {
		scope.Clear();
		item = s;
	}
	else {
		scope = s.Mid(0, max(q - 1, 0));
		item = s.Mid(q + 1);
	}
}

String MakeCodeRef(const String& nest, const String& item)
{
	if(nest.GetCount())
		return nest + "::" + item;
	return item;
}

int GetMatchLen(const char *s, const char *t)
{
	int i = 0;
	while(s[i] == t[i] && s[i])
		i++;
	return i;
}

const CppItem *GetCodeRefItem(const String& ref, const String& rfile)
{
	String scope;
	String item;
	SplitCodeRef(ref, scope, item);
	int q = CodeBase().Find(scope);
	if(q < 0)
		return NULL;
	const Array<CppItem>& n = CodeBase()[q];
	q = FindItem(n, item);
	if(q < 0)
		return NULL;
	if(!IsNull(rfile)) {
		int i = q;
		int qml = 0;
		while(i < n.GetCount() && n[i].qitem == item) {
			int ml = GetMatchLen(GetSourceFilePath(n[i].file), rfile);
			if(ml > qml) {
				q = i;
				qml = ml;
			}
			i++;
		}
	}
	return &n[q];
}

const CppItem *GetCodeRefItem(const String& ref)
{
	return GetCodeRefItem(ref, Null);
}
