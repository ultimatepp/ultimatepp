#include <Core/Core.h>
#include <Plugin/Pcre/Pcre.h>

CONSOLE_APP_MAIN
{
	RegExp r0("(\\w+)\\s(\\w+)\\s(\\w+)");
	if(r0.Match("one two three"))
	{
		for(int i = 0; i < r0.GetCount(); i++)
		    Cout() << r0[i] << '\n';
    } else if(r0.IsError())
	    Cout() << r0.GetError() << '\n';

	Cout() << "------------------------------\n";

	RegExp r1("(\\w+)", RegExp::UNICODE);
	int i = 0;
	while(r1.GlobalMatch("hello 4 ho 55 uuu iii pp 99 baby too swistak"))
	{
		for(int i = 0; i < r1.GetCount(); i++)
			Cout() << r1[i] << '\n';
    }
	if(r1.IsError())
	    Cout() << r1.GetError() << '\n';
}
