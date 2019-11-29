#include "Debuggers.h"

#ifdef PLATFORM_WIN32

#define LLOG(x)  // DLOG(x)

Pdb::Thread& Pdb::Current()
{
	int q = threads.Find((int)~threadlist);
	if(q >= 0)
		return threads[q];

	static Thread zero;
	return zero;
}

Array<Pdb::Frame> Pdb::Backtrace(Thread& ctx, bool single_frame)
{
	Array<Frame> frame;
	STACKFRAME64 stackFrame = {0};
	stackFrame.AddrPC.Mode = stackFrame.AddrFrame.Mode = stackFrame.AddrStack.Mode = AddrModeFlat;
	DWORD machineType = IMAGE_FILE_MACHINE_I386;
	void *c;
	Thread cctx = ctx;
#ifdef CPU_64
	if(win64) {
		machineType = IMAGE_FILE_MACHINE_AMD64;
		stackFrame.AddrPC.Offset = ctx.context64.Rip;
		stackFrame.AddrFrame.Offset = ctx.context64.Rbp;
		stackFrame.AddrStack.Offset = ctx.context64.Rsp;
		c = &cctx.context64;
	}
	else
#endif
	{
		machineType = IMAGE_FILE_MACHINE_I386;
		stackFrame.AddrPC.Offset = ctx.context32.Eip;
		stackFrame.AddrFrame.Offset = ctx.context32.Ebp;
		stackFrame.AddrStack.Offset = ctx.context32.Esp;
		c = &cctx.context32;
	}

	DWORD64 lastFrame = 0; // Prevent loops with optimised stackframes
	while(::StackWalk64(machineType, hProcess, ctx.hThread, &stackFrame, c,
	      0, ::SymFunctionTableAccess64, ::SymGetModuleBase64, 0)) {
	    if(stackFrame.AddrPC.Offset == 0 || lastFrame >= stackFrame.AddrFrame.Offset) {
			LLOG("Invalid stack frame");
			break;
	    }
		lastFrame = stackFrame.AddrFrame.Offset;
		LLOG("PC: " << Hex(stackFrame.AddrPC.Offset));
		Frame& f = frame.Add();
		f.pc = stackFrame.AddrPC.Offset;
		f.frame = stackFrame.AddrFrame.Offset;
		f.stack = stackFrame.AddrStack.Offset;
		f.fn = GetFnInfo(f.pc);
		if(IsNull(f.fn.name)) {
			if(single_frame)
				return frame;
			f.text = Hex(f.pc);
			for(int i = 0; i < module.GetCount(); i++) {
				const ModuleInfo& m = module[i];
				if(f.pc >= m.base && f.pc < m.base + m.size) {
					f.text << " (" << GetFileName(m.path) << ")";
					break;
				}
			}
		}
		else {
			GetLocals(f, cctx, f.param, f.local);
			if(single_frame)
				return frame;
			f.text = f.fn.name;
			f.text << '(';
			for(int i = 0; i < f.param.GetCount(); i++) {
				if(i)
					f.text << ", ";
				f.text << f.param.GetKey(i) << "=" << Visualise(f.param[i], MEMBER).GetString();
			}
			f.text << ')';
		}
	}
	return frame;
}

void Pdb::Sync0(Thread& ctx)
{
	stop = false;

	framelist.Clear();
	frame.Clear();
	current_frame = NULL;

	frame = Backtrace(ctx);
	for(const Frame& f : frame)
		framelist.Add(framelist.GetCount(), f.text);
	framelist.GoBegin();
}

#endif
