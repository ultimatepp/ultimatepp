#include "usvn.h"

GUI_APP_MAIN
{
/*	SysConsole con;
	for(int i = 0; i < 5; i++)
		con.System("ls");
	con.System("svn");
//	con.System("ls /usr/lib");
//	con.System("ls /usr/bin");
	con.System("ls *.wert");
	con.Execute();
	return;*/
	SvnWorks works;
	works.Load(LoadFile(ConfigFile("svnworks")));
	works.Execute();
	SaveFile(ConfigFile("svnworks"), works.Save());
	return;
	SvnSync sync;
	sync.Add("/home/cxl/uppsrc.svn/upp");
	sync.Add("/home/cxl/centrum/SocNet");
	sync.Run();
	return;
	DUMP(Sys("ls"));
	DUMP(Sys("svn status /home/cxl/centrum/SocNet"));
	DUMP(Sys("svn update /home/cxl/centrum/SocNet"));
	return;
}
