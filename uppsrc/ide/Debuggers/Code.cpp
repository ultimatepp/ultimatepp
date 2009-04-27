#include "Debuggers.h"

#ifdef COMPILER_MSC

#define LLOG(x) // LOG(x)

int Pdb::Disassemble(adr_t ip)
{
	char out[256];
	byte code[32];
	memset(code, 0, 32);
	int i;
	for(i = 0; i < 32; i++) {
		int q = Byte(i + ip);
		if(q < 0)
			break;
		code[i] = q;
	}
	int sz = NDisassemble(out, code, ip);
	if(sz > i)
		return -1;
	disas.Add(ip, out, Null);
	return sz;
}

void Pdb::Reg(Label& reg, adr_t val)
{
	String h = Sprintf("%08X", val);
	if(h != reg.GetText()) {
		reg.SetLabel(h);
		reg.SetInk(LtRed);
	}
	else
		reg.SetInk(SColorText);
}

bool Pdb::IsValidFrame(adr_t eip)
{
	for(int i = 0; i < module.GetCount(); i++) {
		const ModuleInfo& f = module[i];
		if(eip >= f.base && eip < f.base + f.size)
			return true;
	}
	return false;
}

void Pdb::Sync0()
{
	stop = false;
	const CONTEXT& context = threads.Get((int)~threadlist).context;
#ifdef CPU_32
	adr_t eip = context.Eip;
	adr_t ebp = context.Ebp;
#else
	adr_t eip = context.Rip;
	adr_t ebp = context.Rbp;
#endif
	adr_t spmax = threads.Get(event.dwThreadId).sp;
	framelist.Clear();
	frame.Clear();
	int ndx = 0;
	FnInfo fn = GetFnInfo(eip);
	int c = -1;
	for(;;) {
		Frame& f = frame.Add();
		f.reip = eip;
		f.rebp = ebp;
		f.fn = fn;
		String r;
		if(IsNull(fn.name)) {
			r = Sprintf("0x%08x", f.reip);
			for(int i = 0; i < module.GetCount(); i++) {
				const ModuleInfo& f = module[i];
				if(eip >= f.base && eip < f.base + f.size) {
					r << " (" << GetFileName(f.path) << ")";
					break;
				}
			}
		}
		else {
			GetLocals(eip, ebp, f.param, f.local);
			r = fn.name;
			r << '(';
			for(int i = 0; i < f.param.GetCount(); i++) {
				if(i)
					r << ", ";
				r << f.param.GetKey(i) << "=" << Visualise(f.param[i]).GetString();
			}
			r << ')';
			if(c < 0)
				c = frame.GetCount() - 1;
		}
		framelist.Add(frame.GetCount() - 1, r);
		int q = 0;
		for(;;) {
			if(ebp > spmax || ++q > 1024 * 64)
				goto end;
			adr_t neip, nebp;
			if(!Copy(ebp, &nebp, 4))
				goto end;
			if(!Copy(ebp + 4, &neip, 4))
				goto end;
			if(nebp >= ebp && nebp < spmax && IsValidFrame(neip)) {
				fn = GetFnInfo(neip);
				if(!IsNull(fn.name)) {
					eip = neip;
					ebp = nebp;
					break;
				}
			}
			ebp += 4;
		}
	}
end:
	framelist <<= max(c, 0);
}

void Pdb::Sync()
{
	threadlist.Clear();
	for(int i = 0; i < threads.GetCount(); i++) {
		int thid = threads.GetKey(i);
		AttrText x(Format("0x%x", thid));
		if(thid == event.dwThreadId)
			x.font = StdFont().Bold();
		threadlist.Add(thid, x);
	}
	threadlist <<= (int)event.dwThreadId;
	Sync0();
	SetFrame();
	IdeActivateBottom();
}

void Pdb::SetThread()
{
	Sync0();
	SetFrame();
	IdeActivateBottom();
}

void Pdb::SetFrame()
{
	int q = ~framelist;
	if(q >= 0 && q < frame.GetCount()) {
		Frame& f = frame[q];
		Image ptrimg = q == 0 ? DbgImg::IpLinePtr() : DbgImg::FrameLinePtr();
		bool df = disas.HasFocus();
		FilePos fp = GetFilePos(f.reip);
		IdeHidePtr();
		autotext.Clear();
		if(fp) {
			IdeSetDebugPos(fp.path, fp.line, ptrimg, 0);
			autotext = IdeGetLine(fp.line - 1) + ' ' + IdeGetLine(fp.line)
			           + ' ' + IdeGetLine(fp.line + 1);
		}
		if(!disas.InRange(f.reip) || f.fn.name != disas_name) {
			disas_name = f.fn.name;
			disas.Clear();
			adr_t ip = f.fn.address;
			adr_t h = f.fn.address + f.fn.size;
			if(f.reip < ip || f.reip >= h) {
				ip = f.reip;
				h = ip + 1024;
			}
			while(ip < h) {
				int q = Disassemble(ip);
				if(q < 0)
					break;
				ip += q;
			}
		}
		disas.SetCursor(f.reip);
		disas.SetIp(f.reip, ptrimg);
		const CONTEXT& context = threads.Get((int)~threadlist).context;
		#ifdef CPU_32
		Reg(regs.eax, context.Eax);
		Reg(regs.ebx, context.Ebx);
		Reg(regs.ecx, context.Ecx);
		Reg(regs.edx, context.Edx);
		Reg(regs.esi, context.Esi);
		Reg(regs.edi, context.Edi);
		Reg(regs.ebp, context.Ebp);
		Reg(regs.esp, context.Esp);
		#else
		Reg(regs.eax, context.Rax);
		Reg(regs.ebx, context.Rbx);
		Reg(regs.ecx, context.Rcx);
		Reg(regs.edx, context.Rdx);
		Reg(regs.esi, context.Rsi);
		Reg(regs.edi, context.Rdi);
		Reg(regs.ebp, context.Rbp);
		Reg(regs.esp, context.Rsp);
		#endif
		if(df)
			disas.SetFocus();
		Data();
	}
}

bool Pdb::SetBreakpoint(const String& filename, int line, const String& bp)
{
	adr_t a = GetAddress(FilePos(filename, line));
	if(!a)
		return false;
	int q = breakpoint.Find(a);
	if(bp.IsEmpty()) {
		if(q >= 0) {
			if(!RemoveBp(a))
				return false;
			breakpoint.Remove(q);
		}
	}
	else {
		if(q < 0) {
			if(!AddBp(a))
				return false;
			breakpoint.Add(a);
		}
	}
	return true;
}

adr_t Pdb::CursorAdr()
{
	adr_t a = disas.HasFocus() ? disas.GetCursor() : GetAddress(FilePos(IdeGetFileName(), IdeGetFileLine()));
	if(!a)
		Exclamation("No code at choosen location !");
	return a;
}

bool Pdb::RunTo()
{
	adr_t a = CursorAdr();
	if(!a)
		return false;
	if(!SingleStep())
		return false;
		#ifdef CPU_32
		if(context.Eip != a) {
		#else
		if(context.Rip != a) {
		#endif
		SetBreakpoints();
		AddBp(a);
		if(!Continue())
			return false;
	}
	Sync();
	return true;
}

void Pdb::Run()
{
	SingleStep();
	SetBreakpoints();
	if(!Continue())
		return;
	Sync();
}

void Pdb::SetIp()
{
	adr_t a = CursorAdr();
	if(!a)
		return;
	#ifdef CPU_32
	context.Eip = a;
	#else
	context.Rip = a;
	#endif
	WriteContext();
	frame[0].reip = a;
	framelist <<= 0;
	SetFrame();
}

bool Pdb::Step(bool over)
{
	TimeStop ts;
	#ifdef CPU_32
	byte b = Byte(context.Eip);
	byte b1 = (Byte(context.Eip + 1) >> 3) & 7;
	#else
	byte b = Byte(context.Rip);
	byte b1 = (Byte(context.Rip + 1) >> 3) & 7;
	#endif
	if(b == 0xe8 || b == 0x9a || b == 0xff && (b1 == 2 || b1 == 3)) {
		if(over) {
			int l = 5;
			if(b != 0xe8) {
				char out[256];
				byte code[32];
				memset(code, 0, 32);
				for(int i = 0; i < 32; i++) {
					#ifdef CPU_32
					int q = Byte(context.Eip + i);
					#else
					int q = Byte(context.Rip + i);
					#endif
					if(q < 0)
						break;
					code[i] = q;
				}
				#ifdef CPU_32
				l = NDisassemble(out, code, context.Eip);
				#else
				l = NDisassemble(out, code, context.Rip);
				#endif
			}
			#ifdef CPU_32
			adr_t bp0 = context.Eip;
			adr_t bp = context.Eip + l;
			#else
			adr_t bp0 = context.Rip;
			adr_t bp = context.Rip + l;
			#endif
			int lvl = 0;
			Lock();
			for(;;) {
				if(!SingleStep()) {
					Unlock();
					return false;
				}
				SetBreakpoints();
				if(breakpoint.Find(bp0) < 0)
					AddBp(bp0);
				else
					bp0 = 0;
				AddBp(bp);
				if(!Continue()) {
					Unlock();
					return false;
				}
				#ifdef CPU_32
				if(context.Eip == bp0)
				#else
				if(context.Rip == bp0)
				#endif
					lvl++;
				else
				#ifdef CPU_32
				if(context.Eip == bp) {
				#else
				if(context.Rip == bp) {
				#endif
					if(lvl <= 0) {
						Unlock();
						return true;
					}
					lvl--;
				}
				else {
					Unlock();
					return true;
				}
				if(ts.Elapsed() > 100)
					Ctrl::ProcessEvents();
			}
		}
		else {
			if(!SingleStep())
				return false;
			#ifdef CPU_32
			byte b = Byte(context.Eip);
			#else
			byte b = Byte(context.Rip);
			#endif
			if(b == 0xeb || b == 0xe9)
				return SingleStep();
			return true;
		}
	}
	else
		return SingleStep();
}

void Pdb::Trace(bool over)
{
	#ifdef CPU_32
	adr_t ip0 = context.Eip;
	#else
	adr_t ip0 = context.Rip;
	#endif
	FilePos p0 = GetFilePos(ip0);
	if(IsNull(p0.path) || disas.HasFocus()) {
		if(!Step(over))
			return;
		Sync();
		return;
	}
	bool locked = false;
	int n = 0;
	TimeStop ts;
	for(;;) {
		if(ts.Elapsed() > 100) {
			if(!locked) {
				Lock();
				locked = true;
			}
			ProcessEvents();
		}
		if(!Step(over))
			break;

		#ifdef CPU_32
		FilePos p = GetFilePos(context.Eip);
		if(context.Eip < ip0 || p.path != p0.path || p.line != p0.line || stop) {
		#else
		FilePos p = GetFilePos(context.Rip);
		if(context.Rip < ip0 || p.path != p0.path || p.line != p0.line || stop) {
		#endif
			Sync();
			break;
		}
	}
	if(locked)
		Unlock();
}

void Pdb::StepOut()
{
	Lock();
	TimeStop ts;
	for(;;) {
		#ifdef CPU_32
		if(Byte(context.Eip) == 0xc2 || Byte(context.Eip) == 0xc3) {
		#else
		if(Byte(context.Rip) == 0xc2 || Byte(context.Rip) == 0xc3) {
		#endif
			if(!SingleStep())
				break;
			Sync();
			break;
		}
		if(stop) {
			Sync();
			break;
		}
		if(!Step(true))
			break;
		if(ts.Elapsed() > 100)
			Ctrl::ProcessEvents();
	}
	Unlock();
}

#endif
