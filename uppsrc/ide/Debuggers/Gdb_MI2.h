#ifndef _ide_Debuggers_Gdb_MI2_h_
#define _ide_Debuggers_Gdb_MI2_h_

#include "MIValue.h"

#define  LAYOUTFILE    <ide/Debuggers/Gdb_MI2.lay>
#include <CtrlCore/lay.h>

class Gdb_MI2 : public Debugger, public ParentCtrl
{
	private:
	
		One<Host> host;
		One<SlaveProcess>  dbg;
		
		bool firstRun;

		// the disassembler window
		DbgDisas disas;

		// the registers pane
		FrameBottom<WithGdb_MI2RegistersLayout<StaticRect> > regs;

		// the quick watch dialog
		WithGdb_MI2QuickwatchLayout<TopWindow> quickwatch;

		EditString watchedit;
		DropList frame;
		TabCtrl tab;
		ArrayCtrl locals;
		ArrayCtrl watches;
		ArrayCtrl autos;
		Label dlock;
	
		Vector<String> regname;
		Vector<Label *> reglbl;
		void AddReg(const char *reg, Label *lbl) { regname.Add(reg); reglbl.Add(lbl); }

		// running flags -- setup by ReadGdb function when async input is catched
		bool started;
		bool stopped;
		MIValue stopReason;
	
		// read debugger output analyzing command responses
		// and async output
		MIValue ParseGdb(String const &s, bool wait = true);
		MIValue ReadGdb(bool wait = true);
	
		// sends an MI command and get answer back
		MIValue MICmd(const char *cmdLine);
		
		// format breakpoint line from ide file and line
		String BreakPos(String const &file, int line);
	
		// get breakpoints info
		MIValue GetBreakpoints(void);
		MIValue GetBreakpoint(int id);
		MIValue GetBreakPoint(const char *file, int line);
		
		// try to set a breakpoint - returns false if no code there
		bool TryBreak(String const &file, int line, bool temp);

		// set breakpoint
		MIValue InsertBreakpoint(const char *file, int line);
		
		// local variable names -- used to synch ide locals with internal GDB variables
		Index<String>localVarNames;
		
		// stored local variable expressions, values and types
		// just changed variables gets re-updated from GDB
		// on frame change, all values are wiped and re-read from GDB
		Vector<String>localVarTypes;
		Vector<String>localVarExpressions;
		Vector<String>localVarValues;
		
		// update local variables on demand
		void UpdateLocalVars(void);
		
		// logs frame data on console
		void LogFrame(String const &msg, MIValue &frame);

		// check for stop reason
		void CheckStopReason(void);
		
		// single step command handler
		void Step(const char *cmd);
		
		// run to command for menu -- just removes return value
		void doRunTo(void) { RunTo(); }

		// setup ide cursor based on disassembler one
		void DisasCursor();

		// reset ide default cursor image when disassembler loose focus
		void DisasFocus();

		// sync disassembler pane
		void SyncDisas(MIValue &fInfo, bool fr);
		
		// sync local variables pane
		void SyncLocals(void);

		// sync watches treectrl
		void SyncWatches();

		// sync auto vars treectrl
		void SyncAutos();

		// sync data tabs, depending on which tab is shown
		void SyncData();

		// sync ide display with breakpoint position
		void SyncIde(bool frame = false);
		
		// watches arrayctrl key handling
		bool Key(dword key, int count);

		// create a string representation of frame given its info and args
		String FormatFrame(MIValue &fInfo, MIValue &fArgs);

		// re-fills frame's droplist when dropping it
		void DropFrames();
	
		// shows selected stack frame in editor
		void ShowFrame();

		// sends pretty-prinding scripts
		void SendPrettyPrinters(void);
	
		// opens quick watch dialog
		void QuickWatch(void);

		// copy stack frame list to clipboard
		void CopyStack(void);

		// copy disassembly listing to clipboard
		void CopyDisas(void);

		// lock/unlock debugger controls
		void Lock();
		void Unlock();

		String GetHostPath(const String& path) { return host->GetHostPath(path); }
		String GetLocalPath(const String& path) { return host->GetLocalPath(path); }

	protected:
	
	public :

		typedef Gdb_MI2 CLASSNAME;
	
		// debugger IDE inteface
		virtual void DebugBar(Bar& bar);
		virtual bool SetBreakpoint(const String& filename, int line, const String& bp);
		virtual bool RunTo();
		virtual void Run();
		virtual void Stop();
		virtual bool IsFinished();
		virtual bool Tip(const String& exp, CodeEditor::MouseTip& mt);

		// create GDB process and initializes it
		bool Create(One<Host> _host, const String& exefile, const String& cmdline);

		Gdb_MI2();
		virtual ~Gdb_MI2();
};

#endif
