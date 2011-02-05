#include "UniqTest.h"



UniqTest::UniqTest()
{
	CtrlLayout(*this, "Window title");
}

// handles news app instances
void UniqTest::handleInstance(Vector<String> const &v)
{
	String s = edit.Get() + "NEW INSTANCE LAUNCHED\n  Command line : '";
	for(int i = 0; i < v.GetCount(); i++)
		s += v[i] + ",";
	if(s[s.GetCount() - 1] == ',')
		s = s.Left(s.GetCount()-1);
	s += "\n";
	
	edit <<= s;
	ProcessEvents();
}

GUI_APP_MAIN
{
	// create Uniq object
	Uniq uniq;
	
	// if not inside first instance, send commandline
	// to first instance and leave
	if(!uniq)
		return;
	
	UniqTest uniqTest;

	// setup callback handling other app's instances
	uniq.WhenInstance = callback(&uniqTest, &UniqTest::handleInstance);
		
	uniqTest.Run();
}

