#include "urepo.h"

#ifdef flagMAIN
GUI_APP_MAIN
{
	DDUMP(GetRepoKind("c:/u/sandbox/se"));
	DDUMP(GetRepoKind("c:/u/foamiemaker"));
/*	
	RunRepoDiff("c:\\u\\foamiemaker\\foamiemaker\\View.cpp");
	return;
*/	
	RepoSync ss;
	String mp = ConfigFile("usvn.msg");
	ss.SetMsgs(LoadFile(mp));
	ss.Dir("c:/u/foamiemaker");
	ss.Dir("C:\\u\\sandbox\\se");
//	ss.Dir("C:\\u\\sandbox\\arduino");
//	ss.Dir("C:\\u\\sandbox\\uppdev");
	ss.DoSync();
	SaveFile(mp, ss.GetMsgs());
}
#endif
