#include "ide.h"

class TopTextFrame : public CtrlFrame {
	virtual void FrameLayout(Rect& r)                   { r.top++; }
	virtual void FramePaint(Draw& w, const Rect& r) {
		w.DrawRect(r.left, r.top, r.Width(), 1, TopSeparator1());
	}
	virtual void FrameAddSize(Size& sz) { sz.cy++; }
};

Console::Console() {
	verbosebuild = false;
	processes.SetCount(1);
	console_lock = -1;
	wrap_text = true;
	console = false;
	SetReadOnly();
	NoHorzScrollbar();
	SetColor(PAPER_READONLY, SColorPaper);
	input.Height(EditString::GetStdHeight());
	input.SetFrame(Single<TopTextFrame>());
	AddFrame(input);
	input.Hide();
	serial = 0;
}

void Console::LeftDouble(Point p, dword) {
	WhenSelect();
}

void Console::RightDown(Point p, dword)
{
	MenuBar::Execute(WhenBar);
}

static int sAppf(int c) {
	return c >= ' ' || c == '\n' ? c : c == '\t' ? ' ' : 0;
}

static int sCharFilterNoCr(int c) {
	return c == '\r' ? 0 : c;
}

void Console::Append(const String& s) {
	if(s.IsEmpty()) return;
	if(console) {
		String t = Filter(s, sCharFilterNoCr);
		if(*t.Last() == '\n')
			t.Trim(t.GetCount() - 1);
		Puts(t);
		return;
	}
	int l, h;
	GetSelection(l, h);
	if(GetCursor() == GetLength()) l = -1;
	EditPos p = GetEditPos();
	SetEditable();
	MoveTextEnd();
	WString t = Filter(s, sAppf).ToWString();
	int mg = sb.GetReducedViewSize().cx / GetFont().GetAveWidth();
	if(wrap_text && mg > 4) {
		int x = GetColumnLine(GetCursor()).x;
		WStringBuffer tt;
		const wchar *q = t;
		while(*q) {
			if(x > mg - 1) {
				tt.Cat('\n');
				tt.Cat("    ");
				x = 4;
			}
			x++;
			if(*q == '\n')
				x = 0;
			tt.Cat(*q++);
		}
		Paste(tt);
	}
	else
		Paste(t);
	SetReadOnly();
	if(l >= 0) {
		SetEditPos(p);
		SetSelection(l, h);
	}
}

bool Console::Key(dword key, int count) {
	if(key == K_ENTER) {
		if(input.IsVisible()) {
			if(processes.GetCount() && processes[0].process)
#ifdef PLATFORM_POSIX
				processes[0].process->Write(String(~input) + "\n");
#else
				processes[0].process->Write(String(~input) + "\r\n");
#endif
			input <<= Null;
		}
		else
			WhenSelect();
		return true;
	}
	return LineEdit::Key(key, count);
}

void Console::ToErrors(const String& s)
{
	for(const char *q = s; *q; q++) {
		if(*q == '\n') {
			WhenLine(line);
			line.Clear();
		}
		else
		if((byte)*q >= ' ')
			line.Cat(*q);
	}
}

void Console::AppendOutput(const String& s)
{
	Append(s);
	ToErrors(s);
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
					AppendOutput(s);
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

int Console::Execute(One<AProcess> pick_ p, const char *command, Stream *out, bool q)
{
	Wait();
	if(!Run(pick(p), command, out, q, 0))
		return -1;
	Wait();
	return processes[0].exitcode;
}

int Console::Execute(const char *command, Stream *out, const char *envptr, bool q, bool noconvert)
{
	try {
		Wait();
		One<AProcess> p;
		if(p.Create<LocalProcess>().ConvertCharset(!noconvert).Start(command, envptr))
			return Execute(pick(p), command, out, q);
	}
	catch(Exc e) {
	}
	ProcessEvents();
	return Null;
}

int Console::AllocSlot()
{
	int ms0 = msecs();
	
	for(;;) {
		for(int i = 0; i < processes.GetCount(); i++)
			if(!IsRunning(i))
				return i;
		Flush();
		Sleep(0);
		if(ms0 != msecs()) {
			ProcessEvents();
			ms0 = msecs();
		}
	}
}

bool Console::Run(const char *cmdline, Stream *out, const char *envptr, bool quiet, int slot, String key, int blitz_count)
{
	try {
		Wait(slot);
		One<AProcess> sproc;
		return sproc.Create<LocalProcess>().Start(cmdline, envptr) &&
		       Run(pick(sproc), cmdline, out, quiet, slot, key, blitz_count);
	}
	catch(Exc e) {
		Append(e);
	}
	ProcessEvents();
	return false;
}

bool Console::Run(One<AProcess> pick_ process, const char *cmdline, Stream *out, bool quiet, int slot, String key, int blitz_count)
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
	pslot.process = pick(process);
	pslot.cmdline = cmdline;
	pslot.outfile = out;
	pslot.output = Null;
	pslot.quiet = quiet;
	pslot.key = key;
	pslot.group = current_group;
	pslot.last_msecs = msecs();
	pslot.serial = ++serial;
	groups.GetAdd(pslot.group).count += blitz_count;
	if(processes.GetCount() == 1)
		Wait(slot);
	return true;
}

void Console::OnFinish(Event<>  cb)
{
	Finisher& f = finisher.Add();
	f.serial = serial;
	f.cb = cb;
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
	int ms0 = msecs();
	while(processes[slot].process) {
		if(ms0 != msecs()) {
			ProcessEvents();
			ms0 = msecs();
		}
		if(Flush() == -1)
			return;
		Sleep(0);
	}
}

bool Console::Wait()
{
	int ms0 = msecs();
	for(;;) {
		if(ms0 != msecs()) {
			ProcessEvents();
			ms0 = msecs();
		}
		if(Flush() == -1) {
			return error_keys.IsEmpty();
		}
		Sleep(0);
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
		if(slot.process->IsRunning())
			slot.process->Kill();
		slot.exitcode = slot.process->GetExitCode();
		slot.serial = INT_MAX;
		if(slot.exitcode != 0 && !IsNull(slot.key))
			error_keys.Add(slot.key);
		slot.process.Clear();
		ToErrors(slot.output);
		WhenRunEnd();
		spooled_output.Cat(slot.output);
		if(console_lock == islot)
			console_lock = -1;
		FlushConsole();
	}
	CheckEndGroup();

	int minserial = INT_MAX;
	for(int i = 0; i < processes.GetCount(); i++)
		minserial = min(processes[i].serial, minserial);
	int i = 0;
	while(i < finisher.GetCount()) {
		const Finisher& f = finisher[i];
		if(f.serial > minserial)
			i++;
		else {
			f.cb();
			finisher.Remove(i);
		}
	}
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

void Console::Input(bool b)
{
	input.Show(b);
	if(b)
		input.SetFocus();
}
