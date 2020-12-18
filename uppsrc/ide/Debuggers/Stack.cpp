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

Array<Pdb::Frame> Pdb::Backtrace(Thread& ctx, bool single_frame, bool thread_info)
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
		if(thread_info) {
			if(frame.GetCount() > 20)
				break;
		}
		else {
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

void Pdb::BTs()
{
	static bool lock;
	
	if(lock) return;
	
	int cid = bts.GetCursor();
	Value cursor = bts.Get();
	Point sc = bts.GetScroll();

	Index<String> open;
	for(int i = 0; i < bts.GetChildCount(0); i++) {
		int id = bts.GetChild(0, i);
		if(bts.IsOpen(id))
			open.FindAdd(~bts.Get(id));
	}
	
	bts.Clear();
	bts.NoRoot();

	bts.WhenOpen = [=](int id) {
		if(bts.GetChildCount(id) == 0) {
			String thid = ~bts.Get(id);
			int ii = threads.Find(atoi(thid));
			if(ii >= 0) {
				int i = 0;
				for(const auto& f : Backtrace(threads[ii])) {
					bts.Add(id, i == 0 ? DbgImg::IpLinePtr() : DbgImg::FrameLinePtr(),
					        "#" + AsString(i) + "#" + thid, f.text);
					i++;
				}
			}
		}
	};

	for(int i = 0; i < threads.GetCount(); i++) {
		dword thid = threads.GetKey(i);
		String stid = AsString(thid);
		String info;
		bool waiting = false;
		bool cowork = false;
		for(const Frame& f : Backtrace(threads[i], false, true)) {
			if(info.GetCount() + f.fn.name.GetCount() > 160) {
				info << "...";
				break;
			}
			bool zwait = f.fn.name.StartsWith("ZwWait");
			waiting |= zwait;
			if(!f.fn.name.StartsWith("RtlSleep") && !f.fn.name.StartsWith("ZwWaitForAlertByThreadId"))
				MergeWith(info, ", ", f.fn.name);
			if(f.fn.name == "Upp::CoWork::Pool::ThreadRun") {
				cowork = true;
				break;
			}
		}
		int id = bts.Add(0, DbgImg::Thread(), stid,
		                 AttrText(Format("0x%x ", (int)thid) + info)
		                 .NormalInk(waiting ? cowork ? Blend(SLtBlue, SGray(), 200) : SGray()
		                                    : cowork ? SLtBlue() : SColorText()),
		                 true);
		if(open.Find(stid) >= 0)
			bts.Open(id);
	}
	
	bts.FindSetCursor(cursor);
	bts.ScrollTo(sc);
	
	bts.WhenBar = [=](Bar& bar) {
		bar.Add("Open All", [=] {
			for(int i = 0; i < bts.GetChildCount(0); i++)
				bts.Open(bts.GetChild(0, i));
		});
		bar.Add("Close All", [=] {
			for(int i = 0; i < bts.GetChildCount(0); i++)
				bts.Close(bts.GetChild(0, i));
		});
	};
	
	bts.WhenSel = [=] {
		String k = ~bts.Get();
		if(*k == '#') {
			int id = bts.GetParent(bts.GetCursor());
			String thid = ~bts.Get(id);
			int tid = atoi(thid);
			int ii = threads.Find(tid);
			if(ii >= 0) {
				int i = 0;
				for(auto& f : Backtrace(threads[ii])) {
					if(k == "#" + AsString(i) + "#" + thid) {
						int tid = atoi(thid);
						if(threadlist.HasKey(tid)) {
							lock = true;
							threadlist <<= tid;
							SetThread();
							if(ii < framelist.GetCount()) {
								framelist.SetIndex(i);
								SetFrame();
								lock = false;
								return;
							}
							lock = false;
						}
					}
					i++;
				}
			}
		}
	};
}

#endif
