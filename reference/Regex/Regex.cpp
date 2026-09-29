#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	RegexMatch matches;

	String text = "This is the last Helllo";
	
	DUMP(Regex("He([l]+)o").Match(text));
	DUMP(Regex("Che([l]+)o").Match(text));
	DUMP(Regex("He([l+)o").GetLastErrorText());

	DUMP(Regex("He([l]+)o").Match(text, matches));

	DUMP(matches.GetCount());

	for(int i = 0; i < matches.GetCount(); i++)
		DUMP(text.Mid(matches.GetOffset(i), matches.GetLength(i)));

    const char *fnames[] = {"foo.txt", "bar.txt", "baz.dat", "zoidberg"};
    
    for (const String& fname : fnames) {
        LOG("=================");
        DUMP(fname);
        DUMP(Regex("[a-z]+\\.txt").Match(fname));
		DUMP(Regex("([a-z]+)\\.([a-z]+)").Match(fname, matches));
		for(int i = 0; i < matches.GetCount(); i++)
			LOG("   " << matches.GetString(fname, i));
    }

}
