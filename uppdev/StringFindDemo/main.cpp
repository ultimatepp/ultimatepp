#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <StringFindDemo/StringFindDemo.lay>
#include <CtrlCore/lay.h>


int Find(String &source, const String &pattern, int from = 0)
{
	return source.Find(pattern, from);
}

int ReverseFind(const String &source, const String &pattern, int from = 0)
{
	if(from < 0 || pattern.GetLength() > source.GetLength()) return -1;
	from = min(from, source.GetLength() - pattern.GetLength());
	while(   from >= 0
	      && memcmp(source.Begin() + from, pattern.Begin(), pattern.GetLength()) != 0)
		--from;
	return from;
}

bool ContainsAt(const String &source, const String &pattern, int pos = 0)
{
	return    pos >= 0
	       && pos + pattern.GetLength() <= source.GetLength()
	       && 0 == memcmp(source.Begin() + pos, pattern.Begin(), pattern.GetLength());
}

bool StartsWith(const String &source, const String &pattern)
{
	return ContainsAt(source, pattern, 0);
}

bool EndsWith(const String &source, const String &pattern)
{
	return ContainsAt(source, pattern, source.GetLength() - pattern.GetLength());
}



class StringFindDemo : public WithStringFindDemoLayout<TopWindow> {
public:
	typedef StringFindDemo CLASSNAME;
	StringFindDemo();
	void UpdateResults();
};


StringFindDemo::StringFindDemo()
{
	CtrlLayout(*this, "Window title");
	findString     << THISBACK(UpdateResults);
	reverseString  << THISBACK(UpdateResults);
	sourceString   << THISBACK(UpdateResults);
	containsString << THISBACK(UpdateResults);
	findFrom       << THISBACK(UpdateResults);
	reverseFrom    << THISBACK(UpdateResults);
	containsPos    << THISBACK(UpdateResults);
	sourceString.SetData("testString");
	findFrom.SetData(0);
	reverseFrom.SetData(0);
	containsPos.SetData(0);
	UpdateResults();
}


void StringFindDemo::UpdateResults()
{
	String source = ~sourceString;
	String find = AsString(~findString);
	String reverse = AsString(~reverseString);
	String contains = AsString(~containsString);
	int ffrom = ~findFrom;
	int rfrom = ~reverseFrom;
	int cpos = ~containsPos;

	if(ffrom >= 0 && ffrom < source.GetLength()) {
	  int findPos = Find(source, find, ffrom);
	  findResult.SetText( Sprintf("%d", findPos) );
	}
	else
	  findResult.SetText("ASSERT");

	if(rfrom >= 0 && rfrom < source.GetLength()) {
	  int reversePos = ReverseFind(source, reverse, ~reverseFrom);
	  reverseResult.SetText( Sprintf("%d", reversePos) );
	}
	else
	  reverseResult.SetText("ASSERT");

	if(cpos >= 0) {
	   bool c = ContainsAt(source, contains, cpos);
	   containsResult.SetText( c ? "true" : "false" );
	}
	else
	  containsResult.SetText("ASSERT");
}


GUI_APP_MAIN
{
	StringFindDemo().Run();
}
