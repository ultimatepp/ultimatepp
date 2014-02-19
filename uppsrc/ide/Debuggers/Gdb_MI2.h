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
	
		// used to post and kill timed callbacks
		TimeCallback timeCallback;
	
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

		// sync local variables pane
		void SyncLocals(MIValue val = MIValue());

		// Sync 'this' inspector data
		void SyncThis(MIValue val = MIValue());
		
		// sync watches treectrl
		void SyncWatches(MIValue val = MIValue());

		// sync data tabs, depending on which tab is shown
		bool dataSynced;
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

		// now we scan the result and add some info
		// so, for example, if we find  tuple like this one:
		//    data = simplevalue
		// this will be modified as
		//    data = { value = simplevalue, expr = evaluable_expression }
		// and for a complex value
		//    data = { some=complex, not_simple=val }
		// woll be modified as
		//    data = { <!value> = { some=complex, not_simple=val }, <!expr> = evaluable_expression }
		// More attributes will be added by type simplifier phase
		void AddAttribs(String const &expr, MIValue &valExpr);

		// collects evaluated variables got with Evaluate
		// hints are used to choose the visualizer when deep-inspecting members
		// 0 for simple values, 1 for arrays, 2 for map
		void CollectVariables(MIValue &val, Index<String> &exprs, Vector<String> &vals, Vector<int> &hints);
		
		// collect evaluated variables got with Evaluate
		// into a single-line string for short display
		String CollectVariablesShort(MIValue &val);

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
		bool Create(One<Host> _host, const String& exefile, const String& cmdline, bool console);

		Gdb_MI2();
		virtual ~Gdb_MI2();
		
		// sends an MI command and get answer back
		MIValue MICmd(const char *cmdLine);
		
		// known types simplifier
		// takes a MIValue from '-data-evaluate-expression' command and try
		// do simplify diplay of known types
		// with deep = false it does just type simplification, no deep evaluation of containers
		// with deep = true it does ONE deep evaluation step
		// returns true if more deep evaluation steps are needed, false otherwise
		bool TypeSimplify(MIValue &val, bool deep);

		// variable inspection support
		// returns a MIValue with inspected data and some info fields added
		// and known types simplified and cathegorized
		// unknown and simple types are left as they are
		// deep is true if we shall have a complete sub-elements evaluation
		MIValue Evaluate(String expr, bool deep = false);
};

#endif
