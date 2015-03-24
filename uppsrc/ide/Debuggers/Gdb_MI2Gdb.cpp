#include "Debuggers.h"

#ifdef PLATFORM_POSIX
// sends a ctrl-c to debugger, returns true on success, false otherwise
bool Gdb_MI2::InterruptDebugger(void)
{
	int killed = 0;
	for(int iProc = 0; iProc < processes.GetCount(); iProc++)
		if(kill(processes[iProc], SIGINT) == 0)
			killed++;
	return killed;
}
#endif

#ifdef PLATFORM_POSIX
// current command break support -- ONLY POSIX, by now
// used to speed up operations in MT mode
bool Gdb_MI2::InterruptCommand(void)
{
	try
	{
		LocalProcess &proc = dynamic_cast<LocalProcess &>(*dbg);
		pid_t pid = proc.GetPid();
		bool res = (kill(pid, SIGINT) == 0);
		return res;
	}
	catch(...)
	{
		return false;
	}
}
#else
bool Gdb_MI2::InterruptCommand(void)
{
	return false;
}
#endif

// read debugger output analyzing command responses and async output
// things are quite tricky because debugger output seems to be
// slow and we have almost no terminator to stop on -- (gdb) is not
// so reliable as it can happen (strangely) in middle of nothing
MIValue Gdb_MI2::ParseGdb(String const &output, bool wait)
{
	MIValue res;
	
	// parse result data
	StringStream ss(output);
	while(!ss.IsEof())
	{
		String s;
		String str = ss.GetLine();
		s = str;
		while(str.GetCount() == 1024 && !ss.IsEof())
		{
			str = ss.GetLine();
			s << str;
		}
		
		s = TrimBoth(s);
		
		// check 'running' and 'stopped' async output
		if(s.StartsWith("*running"))
		{
			started = true;
			stopReason.Clear();
			continue;
		}

		else if(s.StartsWith("*stopped"))
		{
			stopped = true;
			s = '{' + s.Mid(9) + '}';
			stopReason = MIValue(s);
			continue;
		}
		
		// catch process start/stop and store/remove pids
		else if(s.StartsWith("=thread-group-started,id="))
		{
			String id, pid;
			int i = s.Find("id=");
			if(i < 0)
				continue;
			i += 4;
			while(s[i] && s[i] != '"')
				id.Cat(s[i++]);
			i = s.Find("pid=");
			if(i < 0)
				continue;
			i += 5;
			while(s[i] && s[i] != '"')
				pid.Cat(s[i++]);
			
			processes.Add(id, atoi(pid));
			continue;
		}
		
		else if(s.StartsWith("=thread-group-exited,id="))
		{
			String id;
			int i = s.Find("id=");
			if(i < 0)
				continue;
			i += 4;
			while(s[i] && s[i] != '"')
				id.Cat(s[i++]);
			i = processes.Find(id);
			if(i >= 0)
				processes.Remove(i);
			continue;
		}

		// skip asynchronous responses
		// in future, we could be gather/use them
		if(s[0] == '*'|| s[0] == '=')
			continue;
		
		// here handling of command responses
		// we're not interested either, as we use MI interface
		if(s[0] == '~')
			continue;
		
		// here handling of target output
		// well, for now discard this one too, but it should go on console output
		if(s[0] == '~')
			continue;
	
		// here handling of gdb log/debug message
		// not interesting here
		if(s[0] == '&')
			continue;
		
		// now we SHALL have something starting with any of
		// // "^done", "^running", "^connected", "^error" or "^exit" records
		if(s.StartsWith("^done") || s.StartsWith("^running"))
		{
			// here we've got succesful command output in list form, if any
			// shall skip the comma; following can be a serie of pairs,
			// or directly an array of maps in form of :
			// [{key="value",key="value",...},{key="value"...}...]
			
			int i = 5; // just skip shortest, ^done
			while(s[i] && s[i] != ',')
				i++;
			if(!s[i])
				continue;
			i++;
			if(!s[i])
				continue;
			res = MIValue(s.Mid(i));
			continue;
		}
		else if(s.StartsWith("^error"))
		{
			// first array element is reserved for command result status
			s = s.Right(12); // '^error,msg=\"'
			s = s.Left(s.GetCount() - 1);
			res.SetError(s);
		}
		else
			continue;
	}

	return res;
}

MIValue Gdb_MI2::ReadGdb(bool wait)
{
	String output, s;
	MIValue res;
	
	// blocking path
	// waits for 2 minutes max, then return empty value
	// some commands (in particular if they return python exceptions)
	// have a delay between returned exception text and command result
	// so we shall wait up to the final (gdb)
	bool stop = false;
	int retries = 120 * 50;
	while(dbg && --retries && !stop)
	{
		dbg->Read(s);
		StringStream ss(s);
		while(!ss.IsEof())
		{
			String s2 = ss.GetLine();
			output << s2 << "\n";
		
			// wait till (gdb) end marker appears
			s2 = TrimBoth(s2);
			if(s2 == "(gdb)" || s2 == "&\"quit\\n\"")
			{
				stop = true;
				#ifdef flagMT
					// exit if in service threa and thread is stoppint
					if(!IsMainThread() && IsStopThread())
						throw BreakExc();
				#endif
			}
		}

		// non-blocking quick exit
		if(!wait)
			break;

		Sleep(20);
		continue;
	}

	if(output.IsEmpty())
		return res;

	return ParseGdb(output);
}

// new-way commands using GDB MI interface
// on input  : MI interface command line
// on output : an MIValue containing GDB output
// STREAM OUTPUT
// ~						command response
// @						target output
// &						gdb log/debug messages
//
// RESULT RECORDS
// "^done" [ "," results ]
// "^running"				same as "^done"
//	"^connected"			gdb has connected to a remote target.
//	"^error" "," c-string	The operation failed. The c-string contains the corresponding error message.
//	"^exit"					gdb has terminate
//
// ASYNCHRONOUS RECORDS
// *running,thread-id="thread"
// *stopped,reason="reason",thread-id="id",stopped-threads="stopped",core="core"
// =thread-group-added,id="id"
// =thread-group-removed,id="id"
// =thread-group-started,id="id",pid="pid"
// =thread-group-exited,id="id"[,exit-code="code"]
// =thread-created,id="id",group-id="gid"
// =thread-exited,id="id",group-id="gid"
// =thread-selected,id="id"
// =library-loaded,...
// =library-unloaded,...
// =breakpoint-created,bkpt={...}
// =breakpoint-modified,bkpt={...}
// =breakpoint-deleted,bkpt={...}
//
// FRAME INFO INSIDE RESPONSES
// level		The level of the stack frame. The innermost frame has the level of zero. This field is always present.
// func		The name of the function corresponding to the frame. This field may be absent if gdb is unable to determine the function name.
// addr		The code address for the frame. This field is always present.
// file		The name of the source files that correspond to the frame's code address. This field may be absent.
// line		The source line corresponding to the frames' code address. This field may be absent.
// from		The name of the binary file (either executable or shared library) the corresponds to the frame's code address. This field may be absent. 

// THREAD INFO INSIDE RESPONSES
// id			The numeric id assigned to the thread by gdb. This field is always present.
// target-id	Target-specific string identifying the thread. This field is always present.
// details		Additional information about the thread provided by the target. It is supposed to be human-readable and not interpreted by the frontend. This field is optional.
// state		Either `stopped' or `running', depending on whether the thread is presently running. This field is always present.
// core		The value of this field is an integer number of the processor core the thread was last seen on. This field is optional. 
//
// REMARKS : by now, we just handle synchronous output and check asynchronous one just to detect
// debugger run/stop status -- all remaining asynchrnonous output is discarded
MIValue Gdb_MI2::MICmd(const char *cmdLine)
{
	MIValue res;
	
#ifdef flagMT
	// on MT, we interrupt all non-main threads
	// issued GDB commands (which normally can lag several seconds...)
	// before issuing the command
	if(IsMainThread() && IsThreadRunning())
		ShutDownThreads();

	// quick exit for service thread
	if(!IsMainThread() && IsStopThread())
		throw BreakExc();

	// lock other thread's access
	INTERLOCKED {
#endif

	// sends command to debugger and get result data
	// should handle dbg unexpected termination ?
	if(!dbg || !dbg->IsRunning() /* || IdeIsDebugLock() */)
		return MIValue();

	// consume previous output from gdb... don't know why sometimes
	// is there and gives problems to MI interface. We shall maybe
	// parse and store it somewhere
	ReadGdb(false);

	dbg->Write(String("-") + cmdLine + "\n");
	res = ReadGdb();
#ifdef flagMT
	}
#endif
	return res;
}

// support for debugger variables cleanup
void Gdb_MI2::StoreVariable(String const &name)
{
	INTERLOCKED_(varMutex) {
		debugVariables.Add(name);
	}
}

void Gdb_MI2::CleanupVariables(void)
{
#ifdef flagMT
	// restart if called from main thread
	if(IsMainThread())
	{
		INTERLOCKED_(varMutex) {
			prevDebugVariables.Append(debugVariables);
			debugVariables.Clear();
		}
		debugThread.Start(THISBACK(CleanupVariables));
		return;
	}

	IncThreadRunning();

	String name;
	try
	{
		do
		{
			name = "";
			INTERLOCKED_(varMutex) {
				if(!prevDebugVariables.IsEmpty())
				{
					name = prevDebugVariables.Top();
					MICmd("var-delete " + name);
					prevDebugVariables.Pop();
				}
			}
			Sleep(50);
		}
		while(!IsStopThread() && name != "");
	}
	catch(...)
	{
	}
	
	DecThreadRunning();
	
#else
	if(!debugVariables.IsEmpty())
	{
		String name = debugVariables.Pop();
		MICmd("var-delete " + name);
		timeCallback.Set(50, THISBACK(CleanupVariables));
	}
#endif
}
