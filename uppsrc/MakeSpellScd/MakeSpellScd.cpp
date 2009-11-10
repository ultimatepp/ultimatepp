#include <Core/Core.h>

#define LLOG(x)           // LOG(x)
#define LLOGHEXDUMP(a, b) // LOGHEXDUMP(a, b)
#define CREATEINFO

using namespace Upp;

String Utf8ToUpperAscii(const String& x)
{
	return ToUpper(ToAscii(FromUtf8(x)).ToString());
}

bool ScdOrder(const String& a, const String& b)
{
	int q = SgnCompare(Utf8ToUpperAscii(a), Utf8ToUpperAscii(b));
	if(q)
		return q < 0;
	return a < b;
};

struct Block : Moveable<Block> {
	String first;
	int    ctrl_len;
	int    text_len;
};

void Make()
{
	FileIn in(CommandLine()[0]);
	if(!in) {
		Cout() << "Unable to open " << CommandLine()[0] << " for reading\n";
		SetExitCode(1);
		return;
	}
	FileOut out(CommandLine()[1]);
	if(!out) {
		Cout() << "Unable to open " << CommandLine()[1] << " for writing\n";
		SetExitCode(1);
		return;
	}

#ifdef CREATEINFO
	FileOut info(ForceExt(CommandLine()[1], ".info.txt"));
	if(!out) {
		Cout() << "Unable to open info file for writing\n";
		SetExitCode(1);
		return;
	}
	info << "    First |      len |    lenz  |     text |    textz \r\n"
	        "------------------------------------------------------\r\n";
#endif

	SetDefaultCharset(CHARSET_UTF8);

	Vector<String> w;
	while(!in.IsEof())
		w.Add(in.GetLine());
	
	Cout() << w.GetCount() << " words loaded, now sorting...\n";
	
	ASSERT(maxlen < 64);
	
	LLOG("Maximal length:" << maxlen << "  " << maxl);

	Sort(w, ScdOrder);
	
	Cout() << "Sorted, now compressing..\n";

	Vector<Block> block;
	String data;
	int i = 0;

	while(i < w.GetCount()) {
		Block& t = block.Add();
		t.first = Utf8ToUpperAscii(w[i]);
		String ctrl;
		String text;
		String pw;
		while(i < w.GetCount() && text.GetCount() < 65000) {
			String cw = w[i];
			int j = 0;
			while(j < pw.GetCount() && j < cw.GetCount() && j < 31 && pw[j] == cw[j])
				j++;
			ctrl.Cat(j);
			text.Cat(cw.Mid(j));
			text.Cat(0);
			pw = cw;
			i++;
		}
		String ztext = ZCompress(text);
		String zctrl = ZCompress(ctrl);
#ifdef CREATEINFO
		info << Format("%-9.9s |%9d |%9d |%9d |%9d\r\n", t.first,
		               ctrl.GetCount(), zctrl.GetCount(), text.GetCount(), ztext.GetCount());
#endif
		t.ctrl_len = zctrl.GetCount();
		t.text_len = ztext.GetCount();
		data.Cat(zctrl);
		data.Cat(ztext);
	}

	Cout() << "Compressed, writing file directory..\n";
	out.Put(255);
	out.PutL(block.GetCount());
	for(int i = 0; i < block.GetCount(); i++) {
		Block& t = block[i];
		out.Put(t.first.GetCount());
		out.Put(t.first);
		out.PutL(t.ctrl_len);
		out.PutL(t.text_len);
	}

	Cout() << "Writing data..\n";
	out.Put(data);
}

CONSOLE_APP_MAIN
{
	if(CommandLine().GetCount() != 2) {
		Cout() << "Usage: MakeSpellScd <inputfile> <outputfile>\n";
		exit(1);
	}
	Make();
	Cout() << "* Finished\n";
}
