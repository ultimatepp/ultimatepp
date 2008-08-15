#include <Core/Core.h>

using namespace Upp;

static String s_ip = "ip";
static String s_obj = "obj";
static String s_fn = "fn";
static String s_dir = "dir";
static String s_file = "file";
static String s_line = "line";
static String s_stack = "stack";
static String s_frame = "frame";


void PS()
{
	String txt = LoadFile(GetHomeDirFile("test.xml"));
	for(int I = 0; I < 100; I++) {
		RTIMING("Parse String");
		XmlParser p(txt);
		while(!p.IsTag())
			p.Skip();
		try {
			p.PassTag("valgrindoutput");
			while(!p.End()) {
				if(p.Tag("error")) {
					String hdr;
					while(!p.End()) {
						if(p.Tag("what")) {
							hdr = p.ReadText();
							p.SkipEnd();
						}
						else
						if(p.Tag(s_stack)) {
							while(!p.End()) {
								String ip = "?";
								String obj;
								String fn;
								String dir;
								String file;
								String line;
								if(p.Tag(s_frame)) {
									bool hasdir = false;
									bool hasfile = false;
									bool hasline = false;
									bool haspos = false;
									while(!p.End()) {
										if(p.Tag(s_ip)) {
											ip = p.ReadText();
											p.SkipEnd();
										}
										else
										if(p.Tag(s_obj)) {
											obj = p.ReadText();
											p.SkipEnd();
											haspos = true;
										}
										else
										if(p.Tag(s_fn)) {
											fn = p.ReadText();
											p.SkipEnd();
										}
										else
										if(p.Tag(s_dir)) {
											dir = p.ReadText();
											p.SkipEnd();
											hasdir = true;
										}
										else
										if(p.Tag(s_file)) {
											file = p.ReadText();
											p.SkipEnd();
											hasfile = true;
										}
										else
										if(p.Tag(s_line)) {
											line = p.ReadText();
											p.SkipEnd();
											hasline = true;
										}
										else
											p.Skip();
									}
								}
								else
									p.Skip();
							}
						}
						else
							p.Skip();
					}
				}
				else
					p.Skip();
			}
		}
		catch(XmlError) {
		}
	}
}

void PC()
{
	String txt = LoadFile(GetHomeDirFile("test.xml"));
	for(int I = 0; I < 100; I++) {
		RTIMING("Parse char *");
		XmlParser p(txt);
		while(!p.IsTag())
			p.Skip();
		try {
			p.PassTag("valgrindoutput");
			while(!p.End()) {
				if(p.Tag("error")) {
					String hdr;
					while(!p.End()) {
						if(p.Tag("what")) {
							hdr = p.ReadText();
							p.SkipEnd();
						}
						else
						if(p.Tag("stack")) {
							while(!p.End()) {
								String ip = "?";
								String obj;
								String fn;
								String dir;
								String file;
								String line;
								if(p.Tag("frame")) {
									bool hasdir = false;
									bool hasfile = false;
									bool hasline = false;
									bool haspos = false;
									while(!p.End()) {
										if(p.Tag("ip")) {
											ip = p.ReadText();
											p.SkipEnd();
										}
										else
										if(p.Tag("obj")) {
											obj = p.ReadText();
											p.SkipEnd();
											haspos = true;
										}
										else
										if(p.Tag("fn")) {
											fn = p.ReadText();
											p.SkipEnd();
										}
										else
										if(p.Tag("dir")) {
											dir = p.ReadText();
											p.SkipEnd();
											hasdir = true;
										}
										else
										if(p.Tag("file")) {
											file = p.ReadText();
											p.SkipEnd();
											hasfile = true;
										}
										else
										if(p.Tag("line")) {
											line = p.ReadText();
											p.SkipEnd();
											hasline = true;
										}
										else
											p.Skip();
									}
								}
								else
									p.Skip();
							}
						}
						else
							p.Skip();
					}
				}
				else
					p.Skip();
			}
		}
		catch(XmlError) {
		}
	}
}

CONSOLE_APP_MAIN
{
	PS();
	PC();
}