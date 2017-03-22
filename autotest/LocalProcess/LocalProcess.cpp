#include <Core/Core.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	if(CommandLine().GetCount()) {
		if(CommandLine()[0] == "localprocess") {
			int ii = 0;
			for(;;) {
				int c = getchar();
				if(c == 'Q')
					break;
				Cout() << AsString(c) << ':' << (char)c << ':' << ii++ << ' ';
				Cerr() << " ERROR" << (char)c << ii++;
			}
		}
	
		if(CommandLine()[0] == "echo") {
			for(int i = 0; i < CommandLine().GetCount(); i++) {
				if(i)
					Cout() << ' ';
				Cout() << CommandLine()[i];
			}
		}
		return;
	}
	
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	LocalProcess p;
	for(int pass = 0; pass < 2; pass++) {
		LOG("========= PASS " << pass + 1);
		ASSERT(p.Start2(GetExeFilePath() + " localprocess"));
		ASSERT(p.IsRunning());
		
		for(int i = 0; i < 10; i++)
			p.Write(String(i + 'A', 1));
		p.Write("Q");
		
		String e, o, err, out;
		while(p.Read2(o, e)) {
			err << e;
			out << o;
		}
		
		DUMP(err);
		DUMP(out);
		DUMP(p.IsRunning());
		
		ASSERT(out == "65:A:0 66:B:2 67:C:4 68:D:6 69:E:8 70:F:10 71:G:12 72:H:14 73:I:16 74:J:18 ");
		ASSERT(err == " ERRORA1 ERRORB3 ERRORC5 ERRORD7 ERRORE9 ERRORF11 ERRORG13 ERRORH15 ERRORI17 ERRORJ19");
		ASSERT(!p.IsRunning());
		
		DUMP(p.GetExitCode());
		ASSERT(p.GetExitCode() == 0);
		p.Kill(); // check that the exit code is OK even after the Kill
		DUMP(p.GetExitCode());
		ASSERT(p.GetExitCode() == 0);
	
		o = Sys(GetExeFilePath() + " echo something different");
		DUMP(o);
		
		ASSERT(o == "echo something different");
		
		o = Sys(GetExeFilePath(), Vector<String>() << "echo" << "one" << "two" << "\"three");
		DUMP(o);
		ASSERT(o == "echo one two \"three");
		
	}
	LOG("============= OK");
}
