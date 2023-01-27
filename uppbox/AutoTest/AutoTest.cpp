#include <Core/SMTP/SMTP.h>

using namespace Upp;

String infolog;
String errors;

String input;
String umk;
String exe;

Vector<String> exclude;

int Packages;
int Error;
int Tested;
int NoRun;
int Passed;
int Failed;
int Timeout;

void Do(const char *nest, const char *bm, bool release, bool test)
{
	String flags = release ? "r" : "b";
	String mn = release ? "R" : "D";
	String n = String().Cat() << nest << '-' << bm << '-' << mn;
	Cout() << n << '\n';
	infolog << "========== " << nest << " " << bm << (release ? "-R" : "") << " =============\n";
	FindFile ff(AppendFileName(AppendFileName(input, nest), "*.*"));
	bool first = true;
	while(ff) {
		String name = ff.GetName();
		String upp = LoadFile(AppendFileName(ff.GetPath(), name + ".upp"));
	    String h = String(nest) + "/" + name;
		if(ff.IsFolder() && !ff.IsHidden() && FindIndex(exclude, h) < 0 && upp.Find("mainconfig") >= 0) {
			Packages++;
		    String txt;
		    txt << bm;
		    if(release)
               txt << "-R ";
            txt << ' ' << h << ' ';
			String c;
			c << umk << " " << nest << ' ' << name << ' ' << bm << " -" << flags;
//			c << 'a';
#ifdef PLATFORM_POSIX
			c << 's';
#endif
			c << ' ' << exe;
			Cout() << c << '\n';
			infolog << txt;
			Tested += test;
			txt << ", " << GetSysTime();
			DeleteFile(exe);
			String out;
			if(Sys(c, out)) {
				Cout() << " *** ERROR\n";
				infolog << ": ERROR\n";
				errors << txt << ": ERROR\n";
				Error++;
			}
			else {
				infolog << ": BUILD OK";
				if(test) {
					Tested++;
					LocalProcess p;
					SetEnv("UPP_MAIN__", ff.GetPath());
					SetEnv("UPP_ASSEMBLY__", GetFileFolder(ff.GetPath()));
					if(!p.Start(exe)) {
						Cout() << "FAILED TO RUN\n";
						infolog << ", FAILED TO RUN";
						errors << txt << ": FAILED TO RUN\n";
						Failed++;
					}
					else {
						Cout() << "RUN\n";
						int timeout = 60000*3;
						int q = upp.FindAfter("#WAIT:");
						if(q >= 0) {
							timeout = max(60 * 1000 * atoi(~upp + q), timeout);
						}
						int msecs0 = msecs();
						for(;;) {
							if(p.IsRunning()) {
								if(msecs(msecs0) > timeout) {
									infolog << ", TIMEOUT";
									errors << txt << ": TIMEOUT\n";
									Cout() << "*** TIMEOUT\n";
									Timeout++;
									break;
								}
							}
							else {
								if(p.GetExitCode()) {
									infolog << ", FAILED";
									errors << txt << ": FAILED\n";
									Cout() << "*** FAILED\n";
									Failed++;
								}
								else {
									infolog << ", OK";
									Cout() << "OK\n";
									Passed++;
								}
								break;
							}
							String h;
							p.Read(h); // Empty the pipe to avoid blocking
							Sleep(1);
						}
						infolog << ", " << msecs(msecs0) / 1000.0 << " s";
					}
				}
				infolog << "\n";
			}
			first = false;
			DeleteFile(exe);
		}
		ff.Next();
	}
}

CONSOLE_APP_MAIN
{
	const Vector<String>& h = CommandLine();

	if(h.GetCount() != 2) {
		Cout() << "Usage: AutoTest test_file smtp_account\n";
		Exit(1);
	}
	Value ini = ParseJSON(LoadFile(h[0]));
	Value email = ParseJSON(LoadFile(h[1]));
	
	if(IsError(ini)) {
		Cout() << "Invalid test_file\n";
		Exit(1);
	}
		
	if(IsError(email)) {
		Cout() << "Invalid smtp_account\n";
		Exit(1);
	}
	
	Cout() << "INI: " << h[0] << "\n";
	Cout() << LoadFile(h[0]) << "\n";
	Cout() << (String)ini["build_test"] << "\n";
	
	input = ini["upp_sources"];
	Vector<String> test = Split((String)ini["auto_test"], ';');
	Vector<String> build = Split((String)ini["build_test"], ';');
	Vector<String> bm = Split((String)ini["build_method"], ';');
	exclude = Split((String)ini["exclude"], ';');
	String mode = ini["output_mode"];
	umk = ini["umk_path"];
	if (IsNull(umk))
		umk = GetHomeDirFile("bin/umk");
	exe = ini["exe_path"];
	if (IsNull(exe))
		exe = GetHomeDirFile("autotest.tst");
	
	Vector<bool> release;
	for(int i = 0; i < bm.GetCount(); i++) {
		bool r = bm[i].EndsWith("-R");
		if(r)
			bm[i].Trim(bm[i].GetCount() - 3);
		release.Add(r);
	}
	
	TimeStop tm;
	infolog << "Started " << GetSysTime() << "\n";
	
	for(int i = 0; i < bm.GetCount(); i++) {
		for(int j = 0; j < test.GetCount(); j++)
			Do(test[j], bm[i], release[i], true);
		for(int j = 0; j < build.GetCount(); j++)
			Do(build[j], bm[i], release[i], false);
	}
	

	infolog << "Finished " << GetSysTime() << "\n";
	
	String body;
	int tm0 = (int)(tm.Seconds() / 60);
	body << Packages << " packages to build\n"
	     << Tested << " packages to run\n"
	     << Error << " failed to compile (ERROR)\n";
	if(NoRun)
	     body << NoRun << " failed to run\n";
	body
	     << Timeout << " timeouts (TIMEOUT)\n"
	     << Failed << " crashed (FAILED)\n"
	     << Passed << " OK\n"
	     << "Total time to run tests: " << tm0 / 60 << ":" << Format("%02d", tm0 % 60) << "\n\n";
	
	if(errors.GetCount())
		body << "FAILED TESTS:\n" << errors << "\n---------------------------\n\n";
               SetExitCode(1);

	body << "TEST LOG:\n" << infolog;
	
	if (mode == "watchdog") {
	       Cout() << body
	              << "\n@ok=" << Passed
	              << "\n@errors=" << (Error + Timeout)
	              << "\n@failures=" << (Failed + NoRun)
	              << "\n@skipped=" << exclude.GetCount() << "\n";
	       return;
	}
	
	if(IsNull(email["smtp_server"]))
	       return;

	Smtp smtp;
	smtp.Trace();
	smtp.Host(email["smtp_server"])
	    .Port(Nvl((int)email["smtp_port"], 465))
	    .SSL()
		.Auth(email["smtp_user"], email["smtp_password"])
		.From("autotest@ultimatepp.org")
	    .Subject(String().Cat() << Nvl((String)ini["subject"], (String)"U++ autotest")
	             << ": " << (errors.GetCount() ? "** FAILED **" : "OK"))
	    .Body(body)
	;

	Vector<String> s = Split((String)ini["emails"], ';');
	for(int i = 0; i < s.GetCount(); i++)
		smtp.To(s[i]);
	smtp.Send();
}
