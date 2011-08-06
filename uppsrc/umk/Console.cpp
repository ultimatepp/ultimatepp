#include "umake.h"

void ProcessEvents() {}

Console::Console() {
	verbosebuild = false;
	processes.SetCount(1);
	console_lock = -1;
	wrap_text = true;
	console = false;
}

static int sAppf(int c) {
	return c >= ' ' || c == '\n' ? c : c == '\t' ? ' ' : 0;
}

static int sCharFilterNoCr(int c) {
	return c == '\r' ? 0 : c;
}

void Console::Append(const String& s) {
	if(s.IsEmpty()) return;
	if(console && !SilentMode) {
		String t = Filter(s, sCharFilterNoCr);
		if(*t.Last() == '\n')
			t.Trim(t.GetCount() - 1);
		puts(t);
		return;
	}
}

int Console::Flush()
{
	bool done_output = false;
	int num_running = 0;
	for(int i = 0; i < processes.GetCount(); i++)
		if(!!processes[i].process)
			num_running++;
	if(num_running) {
		int time = msecs();
		for(int i = 0; i < processes.GetCount(); i++) {
			Slot& slot = processes[i];
			if(!!slot.process) {
				Group& group = groups.GetAdd(slot.group);
				group.msecs += (time - slot.last_msecs) / num_running;
				group.raw_msecs += time - slot.last_msecs;
				slot.last_msecs = time;
			}
		}
	}
	bool running = false;
	for(int i = 0; i < processes.GetCount(); i++) {
		Slot& slot = processes[i];
		if(!slot.process)
			continue;
		String s;
		slot.process->Read(s);
		if(!IsNull(s)) {
			done_output = true;
			if(slot.outfile)
				slot.outfile->Put(s);
			if(!slot.quiet) {
				if(console_lock < 0 || console_lock == i) {
					console_lock = i;
					Append(s);
				}
				else
					slot.output.Cat(s);
			}
		}
		if(!slot.process->IsRunning()) {
			Kill(i);
			if(slot.exitcode != 0 && verbosebuild)
				spooled_output.Cat("Error executing " + slot.cmdline + "\n");
			if(console_lock == i)
				console_lock = -1;
			FlushConsole();
			CheckEndGroup();
			continue;
		}
		running = true;
	}
	return !running ? -1 : done_output ? 1 : 0;
}

int Console::Execute(One<SlaveProcess> p, const char *command, Stream *out, bool q)
{
	Wait();
	if(!Run(p, command, out, q, 0))
		return -1;
	Wait();
	return processes[0].exitcode;
}

int Console::Execute(const char *command, Stream *out, const char *envptr, bool q)
{
	try {
		Wait();
		return Execute(StartProcess(command, envptr, REMOTE_TIMEOUT), command, out, q);
	}
	catch(Exc e) {
		ProcessEvents();
		return Null;
	}
}

int Console::AllocSlot()
{
	int sleep = 0;
	for(;;) {
		for(int i = 0; i < processes.GetCount(); i++)
			if(!IsRunning(i))
				return i;
		switch(Flush()) {
			case -1: break;
			case  0: sleep = min(sleep + 5, 20); break;
			case +1: sleep = 0; break;
		}
		Sleep(sleep);
		ProcessEvents();
	}
}

bool Console::Run(const char *cmdline, Stream *out, const char *envptr, bool quiet, int slot, String key, int blitz_count)
{
	try {
		Wait(slot);
		One<SlaveProcess> sproc = StartProcess(cmdline, envptr, REMOTE_TIMEOUT);
		return !!sproc && Run(sproc, cmdline, out, quiet, slot, key, blitz_count);
	}
	catch(Exc e) {
		Append(e);
		ProcessEvents();
		return false;
	}
}

bool Console::Run(One<SlaveProcess> process, const char *cmdline, Stream *out, bool quiet, int slot, String key, int blitz_count)
{
	if(!process) {
		if(verbosebuild)
			spooled_output << "Error running " << cmdline << "\n";
		FlushConsole();
		return false;
	}
	else if(verbosebuild)
		spooled_output << cmdline << "\n";
	Wait(slot);
	Slot& pslot = processes[slot];
	pslot.process = process;
	pslot.cmdline = cmdline;
	pslot.outfile = out;
	pslot.output = Null;
	pslot.quiet = quiet;
	pslot.key = key;
	pslot.group = current_group;
	pslot.last_msecs = msecs();
	groups.GetAdd(pslot.group).count += blitz_count;
	if(processes.GetCount() == 1)
		Wait(slot);
	return true;
}

void Console::FlushConsole()
{
	if(console_lock < 0) {
		Append(spooled_output);
		spooled_output = Null;
	}
}

void Console::BeginGroup(String group)
{
	Flush();
	groups.GetAdd(current_group).finished = true;
	groups.GetAdd(current_group = group);
	CheckEndGroup();
}

void Console::EndGroup()
{
	groups.GetAdd(current_group).finished = true;
	CheckEndGroup();
	current_group = Null;
}

bool Console::IsRunning()
{
	for(int i = 0; i < processes.GetCount(); i++)
		if(IsRunning(i))
			return true;
	return false;
}

bool Console::IsRunning(int slot)
{
	if(slot < 0 || slot >= processes.GetCount() || !processes[slot].process)
		return false;
	return processes[slot].process->IsRunning();
}

void Console::Wait(int slot)
{
	int sleep = 0;
	while(processes[slot].process) {
		ProcessEvents();
		switch(Flush()) {
			case -1: return;
			case  0: sleep = min(sleep + 5, 20); break;
			case +1: sleep = 0; break;
		}
		Sleep(sleep);
		// Ctrl::GuiSleep(sleep);
	}
}

bool Console::Wait()
{
	int sleep = 0;
	for(;;) {
		ProcessEvents();
		switch(Flush()) {
			case -1: return error_keys.IsEmpty();
			case  0: sleep = min(sleep + 5, 20); break;
			case +1: sleep = 0; break;
		}
		Sleep(sleep);
		// Ctrl::GuiSleep(sleep);
	}
}

void Console::Kill()
{
	for(int i = 0; i < processes.GetCount(); i++)
		Kill(i);
}

void Console::Kill(int islot)
{
	Slot& slot = processes[islot];
	if(slot.process) {
		slot.process->Kill();
		slot.exitcode = slot.process->GetExitCode();
		if(slot.exitcode != 0 && !IsNull(slot.key))
			error_keys.Add(slot.key);
		slot.process.Clear();
		spooled_output.Cat(slot.output);
		if(console_lock == islot)
			console_lock = -1;
		FlushConsole();
	}
	CheckEndGroup();
}

void Console::SetSlots(int s)
{
	Kill();
	processes.SetCount(s);
}

void Console::CheckEndGroup()
{
	for(int g = groups.GetCount(); --g >= 0;) {
		String gname = groups.GetKey(g);
		Group& group = groups[g];
		if(!IsNull(gname) && group.finished) {
			int p = processes.GetCount();
			while(--p >= 0 && !(!!processes[p].process && processes[p].group == gname))
				;
			if(p < 0) {
				if(group.count > 0) {
					int duration = msecs(group.start_time);
					String msg = NFormat("%s: %d file(s) built in %s, %d msecs / file, duration = %d msecs",
						gname, group.count, PrintTime(group.msecs), group.msecs / group.count, duration);
					if(processes.GetCount() > 1) {
						int k = 100 * processes.GetCount() / (processes.GetCount() - 1);
						msg << NFormat(", parallelization %d%%", minmax(k - group.msecs * k / max(group.raw_msecs, 1), 0, 100));
					}
					msg << '\n';
					spooled_output.Cat(msg);
					if(console_lock < 0) {
						Append(spooled_output);
						spooled_output = Null;
					}
				}
				groups.Remove(g);
			}
		}
	}
}
