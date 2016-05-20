#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	String s = "Hello world";

	DUMP(s);
	s.Cat('!');
	DUMP(s);
	
	DUMP(*s.Last());
	DUMP(s[0]);
	DUMP(s.GetCount());
	
	DUMP(s.Compare("Abel"));
	DUMP(s.Compare("Hello world!"));
	DUMP(s.Compare("Goal"));
	
	DUMP(s.Find('l'));
	DUMP(s.ReverseFind('l'));

	DUMP(s.Find("llo"));
	DUMP(s.Find("xxx"));
	DUMP(s.FindAfter("llo"));
	DUMP(s.FindAfter("xxx"));

	DUMP(s.StartsWith("Hello"));
	DUMP(s.EndsWith("!"));
	
	DUMP(s.FindFirstOf("xyzw"));
	
	DUMP(s.Mid(2, 3));
	DUMP(s.Left(2));
	DUMP(s.Right(2));
	
	s.Replace("world", "Frank");
	DUMP(s);
	
	s.Trim(5);
	DUMP(s);
	s.TrimLast();
	DUMP(s);
	
	s.Insert(0, "String ");
	DUMP(s);
	
	s << Point(12, 123);
	DUMP(s);
	
	DUMP(s.TrimEnd("[12, 123]"));
	DUMP(s);
	DUMP(s.TrimEnd("xxx"));
	DUMP(s);
	DUMP(s.TrimStart("String "));
	DUMP(s);
	
	s.Set(0, 'Y');
	DUMP(s);
	
	WString ws = s.ToWString();
	ws << "o";
	DUMP(ws);
	
	s = ws.ToString();
	DUMP(s);
}
