#include "Debuggers.h"

void Gdb::MemoryGoto()
{
	WithGotoMemoryLayout<TopWindow> dlg;
	CtrlLayoutOKCancel(dlg, "Memory view");
	dlg.count <<= memory.data.GetCount();
	dlg.count.AddList(1024);
	dlg.count.AddList(2048);
	dlg.count.AddList(4096);
	dlg.count <<= 2048;
	dlg.adr <<= FormatHex((void *)memory.start);
	if(dlg.Execute() != IDOK)
		return;
	MemoryLoad(~dlg.adr, ~dlg.count, true);
}

void Gdb::MemoryLoad(const String& adr, int count, bool showerror)
{
	memory.start = 0;
	memory.data.Clear();
	String error;
	for(String s : Split(FastCmd("x /" + AsString(clamp(count, 1, 1024*1024)) + "xb " + adr), '\n')) {
		if(IsNull(error))
			error = TrimBoth(s);
		try {
			CParser p(s);
			while(!p.IsEof())
				if(p.Char2('0', 'x') && p.IsNumber(16)) {
					uint64 a = p.ReadNumber64(16);
					if(!memory.start)
						memory.start = a;
					while(!p.IsEof())
						if(p.Char2('0', 'x') && p.IsNumber(16))
							memory.data.Add((byte)p.ReadNumber(16));
						else
							p.SkipTerm();
				}
				else
					p.SkipTerm();
		}
		catch(CParser::Error e) {
			error = e;
			break;
		}
	}
	if(showerror && !memory.start && error.GetCount())
		Exclamation("\1" + error);
	memory.SetStart(memory.start);
	memory.SetTotal(memory.data.GetCount());
	memory.SetSc(0);
	memory.SetCursor(0);
}

void Gdb::Memory()
{
	if(memory.data.GetCount())
		MemoryLoad("0x" + FormatIntHex((void *)memory.start), memory.data.GetCount(), false);
}

void Gdb::MemoryMenu(Bar& bar, const String& s)
{
	bar.Add("Memory at &&" + s, [=] { tab.Set(memory); MemoryLoad('&' + s, 2048, true); });
	bar.Add("Memory at " + s, [=] { tab.Set(memory); MemoryLoad(s, 2048, true); });
}