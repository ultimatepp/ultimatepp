#include "urepo.h"

using namespace Upp;

#ifdef flagMAIN
GUI_APP_MAIN
{
/*	
	RunRepoDiff("c:\\u\\foamiemaker\\foamiemaker\\View.cpp");
	return;
*/	
	RepoSync ss;
	String mp = ConfigFile("usvn.msg");
	ss.SetMsgs(LoadFile(mp));
//	ss.Dir("c:/u/foamiemaker");
	ss.Dir("C:\\u\\sandbox\\se");
	ss.Dir("C:\\u\\upp.src\\uppsrc");
	ss.Dir("C:\\u\\upp.src\\examples");
//	ss.Dir("C:\\u\\sandbox\\arduino");
//	ss.Dir("C:\\u\\sandbox\\uppdev");
	ss.DoSync();
	SaveFile(mp, ss.GetMsgs());
}
#endif
