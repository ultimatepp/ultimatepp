#ifndef _ide_Debuggers_Gdb_MI2_h_
#define _ide_Debuggers_Gdb_MI2_h_

#include "MIValue.h"
#include "VarItem.h"

class WatchEdit : public LineEdit
{
	virtual void HighlightLine(int line, Vector<Highlight>& h, int pos);
};

#define  LAYOUTFILE    <ide/Debuggers/Gdb_MI2.lay>
#include <CtrlCore/lay.h>

// abort command exception - used to stop non-main threads
struct BreakExc : public Exc
{
	BreakExc() : Exc("break") {}

};

class Gdb_MI2 : public Debugger, public ParentCtrl
{
	friend class VarItem;
	private:
	
		// list of processes (and connected thread groups)
		// used to stop them
		VectorMap<String, int> processes;

		// list of debug variables created and thread-safe functions to
		// manage them -- used to clean up
		Vector<String>debugVariables;
#ifdef flagMT
		Vector<String>prevDebugVariables;
#endif
		Mutex varMutex;
		void StoreVariable(String const &name);
		void CleanupVariables(void);
	
		// multithread support
#ifdef flagMT
		// numbr of running debug threads
		int threadRunning;
		
		// flag to signal threads to stop
		bool stopThread;
		
		// mutex and thead object
		Mutex mutex;
		Thread debugThread;
		
		// mutex-protected functions
		bool IsThreadRunning(void);
		void IncThreadRunning();
		void DecThreadRunning();
		
	public:
		bool IsStopThread(void);
	private:
		void SetStopThread(bool b);
		
		// shut down threads and wait till done
		void ShutDownThreads(void);

#endif

#ifdef PLATFORM_POSIX
		// debug break support -- ONLY POSIX, by now
		bool InterruptDebugger(void);
#endif

		// current command break support -- ONLY POSIX, by now
		// used to speed up operations in MT mode
		bool InterruptCommand(void);
	
		// used to post and kill timed callbacks
		TimeCallback timeCallback;
		TimeCallback exploreCallback;
	
		One<Host> host;
		One<AProcess>  dbg;
		
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

		ArrayCtrl autos;
		ArrayCtrl locals;
		ArrayCtrl members;
		ArrayCtrl watches;
		ArrayCtrl explorer;

		// explorer stuffs -- just starting
		EditString explorerExprEdit;
		Button explorerBackBtn, explorerForwardBtn;
		StaticRect explorerPane;
		void onExploreExpr(ArrayCtrl *what = NULL);
		void onExplorerChild();
		void onExplorerBack();
		void onExplorerForward();
		void ExplorerMenu(Bar& bar);

		void doExplore(String const &expr, bool appendHistory);

		// explorer expressions and values
		Index<String>explorerExpressions;
		Vector<String>explorerValues;
		Index<String> explorerHistoryExpressions;
		int explorerHistoryPos;

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
		
		// stored local variable expressions, and values
		Index<String>localExpressions;
		Vector<String>localValues;
		Vector<int>localHints;
		
		// stored watches expressions and values
		Index<String>watchesExpressions;
		Vector<String>watchesValues;
		Vector<int>watchesHints;
		
		// 'this' variable inspection data
		Index<String>thisExpressions;
		Vector<String>thisValues;
		Vector<int>thisHints;
		Index<String>thisShortExpressions;
		
		// stored autos expressions, values and types
		String autoLine;

		// logs frame data on console
		void LogFrame(String const &msg, MIValue &frame);

		// check for stop reason
		void CheckStopReason(void);
		
		// stop all running threads and re-select previous current thread
		void StopAllThreads(void);
		
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
		
		// sync auto vars treectrl
		void SyncAutos();
		
#ifdef flagMT
		// sync local variables pane
		void SyncLocals(void);

		// Sync 'this' inspector data
		void SyncThis(void);
		
		// sync watches treectrl
		void SyncWatches(void);

		// sync explorer pane
		void SyncExplorer();
#else
		// sync local variables pane
		void SyncLocals(const Vector<VarItem>& localVars = Vector<VarItem>());

		// Sync 'this' inspector data
		void SyncThis(const Vector<VarItem>& children = Vector<VarItem>());
		
		// sync watches treectrl
		void SyncWatches(const Vector<VarItem>& children = Vector<VarItem>());

		// sync explorer pane
		void SyncExplorer(const Vector<VarItem>& children = Vector<VarItem>());
#endif

		// sync data tabs, depending on which tab is shown
		bool localSynced;
		bool thisSynced;
		bool watchesSynced;
		bool explorerSynced;
		void SyncData();
		
		// sync ide display with breakpoint position
		void SyncIde(bool frame = false);
		
		// watches arrayctrl key handling
		bool Key(dword key, int count);

		// create a string representation of frame given its info and args
		String FormatFrame(MIValue &fInfo, MIValue &fArgs);

		// re-fills frame's droplist when dropping it
		bool FillDropFrames(int min, int max, bool val);
		void DropFrames();
	
		// shows selected stack frame in editor
		void ShowFrame();
		
		// re-fills thread selector droplist on drop
		void dropThreads();
		
		// selects current thread
		void showThread(void);

		// opens quick watch dialog
		void QuickWatch(void);
		
		// format watch line
		String FormatWatchLine(String exp, String const &val, int level);

		// deep watch current quickwatch variable
		void WatchDeep0(String parentExp, String const &name, int level, int &maxRemaining);
		void WatchDeep(String parentExp, String const &name);

		// copy stack frame list to clipboard
		void CopyStack(void);

		// copy disassembly listing to clipboard
		void CopyDisas(void);
		
		// lock/unlock debugger controls
		void Lock();
		void Unlock();
		
		// Period check for killed console
		TimeCallback periodic;
		void Periodic();

		String GetHostPath(const String& path) { return host->GetHostPath(path); }
		String GetLocalPath(const String& path) { return host->GetLocalPath(path); }

		// fill a pane with data from a couple of arrays without erasing it first
		// (avoid re-painting and resetting scroll if not needed)
		void FillPane(ArrayCtrl &pane, Index<String> const &nam, Vector<String> const &val);

		void Setup(ArrayCtrl& a, int x = 1);
		void SyncWidth(ArrayCtrl& a);

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
		bool Create(One<Host>&& _host, const String& exefile, const String& cmdline, bool console);

		Gdb_MI2();
		virtual ~Gdb_MI2();
		
		// sends an MI command and get answer back
		MIValue MICmd(const char *cmdLine);
		
		// quick exit from service thread when called and 'stopThread' is set
		// throws a BreakExc exception
		void RaiseIfStop(void);
};

#endif
