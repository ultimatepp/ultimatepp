#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	//          01234567890123456789012345
	String s = "lorem ipsum dolor sit amet";

	DUMP(s);
	s.Cat('!');
	DUMP(s);
	
	DUMP(*s.Last());
	DUMP(s[0]);
	DUMP(s.GetCount());
	
	DUMP(s.Compare("abel"));
	DUMP(s.Compare("lorem ipsum dolor sit amet"));
	DUMP(s.Compare("xena"));
	
	DUMP(s.Find('l'));
	DUMP(s.ReverseFind('l'));

	DUMP(s.Find("ipsum"));
	DUMP(s.Find("xxx"));
	DUMP(s.FindAfter("ipsum"));
	DUMP(s.FindAfter("xxx"));

	DUMP(s.Find("lo"));
	DUMP(s.Find("lo", 4));

	DUMP(s.ReverseFind("lo"));
	DUMP(s.ReverseFind("lo", 14));
	DUMP(s.ReverseFindAfter("lo"));
	
	DUMP(s.StartsWith("lorem"));
	DUMP(s.EndsWith("!"));
	
	DUMP(s.FindFirstOf("xyzw"));
	
	DUMP(s.Mid(2, 3));
	DUMP(s.Left(2));
	DUMP(s.Right(2));
	
	s.Replace("dolor", "DOLOR");
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
	
	StringBuffer sb(s);
	DUMP(s);
	char *h = sb;
	*h = 'C';
	s = sb;
	DUMP(s);
	
	String z(0, 10);
	DUMPHEX(z);
}
