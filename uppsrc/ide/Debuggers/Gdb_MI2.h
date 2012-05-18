#ifndef _ide_Debuggers_Gdb_MI2_h_
#define _ide_Debuggers_Gdb_MI2_h_

#include "MIValue.h"

class WatchEdit : public LineEdit
{
	virtual void HighlightLine(int line, Vector<Highlight>& h, int pos);
};

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
#ifdef CPU_64
		FrameBottom<WithGdb_MI2Registers64Layout<StaticRect> > regs;
#define	RPREFIX "r"
#else
		FrameBottom<WithGdb_MI2RegistersLayout<StaticRect> > regs;
#define	RPREFIX "e"
#endif

		// the quick watch dialog
		WithGdb_MI2QuickwatchLayout<TopWindow> quickwatch;

		EditString watchedit;
		DropList frame;
		DropList threadSelector;
		TabCtrl tab;
		ArrayCtrl locals;
		ArrayCtrl watches;
		ArrayCtrl autos;

		// explorer stuffs -- just starting
		ArrayCtrl explorer;
		EditString explorerExprEdit;
		Button explorerBackBtn, explorerForwardBtn;
		StaticRect explorerPane;
		void onExploreExpr(ArrayCtrl *what = NULL);
		void onExplorerChild();
		void onExplorerBack();
		void onExplorerForward();
		void ExplorerMenu(Bar& bar);
		void doExplore(String const &expr, String var, bool isChild, bool appendHistory);
		Index<String> explorerHistoryExpressions;
		Index<String> explorerHistoryVars;
		Vector<bool> explorerHistoryChilds;
		int explorerHistoryPos;
		Vector<String> explorerChildVars;
		String explorerParentExpr;

		Label dlock;
	
		Vector<String> regname;
		Vector<Label *> reglbl;
		void AddReg(const char *reg, Label *lbl) { regname.Add(reg); reglbl.Add(lbl); }

		// find free space at right of tabs (we should probably add something to TabCtrl for that..)
		int FindTabsRight(void);

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
		bool TryBreak(adr_t addr, bool temp);
		bool TryBreak(String const &file, int line, bool temp);

		// set breakpoint
		MIValue InsertBreakpoint(const char *file, int line);
		
		// local variable names -- used to synch ide locals with internal GDB variables
		Index<String>localVarNames;
		
		// stored local variable expressions, values and types
		// just changed variables gets re-updated from GDB
		// on frame change, all values are wiped and re-read from GDB
		Vector<String>localVarTypes;
		Index<String>localVarExpressions;
		Vector<String>localVarValues;
		
		// stored watches expressions, values and types
		Index<String>watchesNames;
		Vector<String>watchesTypes;
		Index<String>watchesExpressions;
		Vector<String>watchesValues;
		
		// stored autos expressions, values and types
		String autoLine;
		Index<String>autosNames;
		Vector<String>autosTypes;
		Index<String>autosExpressions;
		Vector<String>autosValues;
		
		// update variables on demand (locals, watches....)
		void UpdateVars(void);

		// update local variables on demand
		void UpdateLocalVars(void);
		
		// update stored watches values on demand
		void UpdateWatches(void);
		
		// update stored auto values on demand
		void UpdateAutos(void);

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
		
		// re-fills thread selector droplist on drop
		void dropThreads();
		
		// selects current thread
		void showThread(void);

		// sends pretty-prinding scripts
		void SendPrettyPrinters(void);
	
		// opens quick watch dialog
		void QuickWatch(void);
		
		// format watch line
		String FormatWatchLine(String exp, String const &val, int level);

		// deep watch current quickwatch variable
		void WatchDeep(String parentExp, String const &name, int level = 0);

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
		virtual void AsyncBrk();
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
