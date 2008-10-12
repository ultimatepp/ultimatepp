#include "Browser.h"

void SplitCodeRef(const String& s, String& scope, String& item)
{
	int q = s.Find('(');
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

const CppItem *GetCodeRefItem(const String& ref)
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
	return &n[q];
}
