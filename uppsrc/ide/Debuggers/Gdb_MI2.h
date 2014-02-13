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

		// item for a GDB variable
		struct VarItem : Moveable<VarItem>
		{
			typedef enum { SIMPLE, ARRAY, MAP } VarKind;
			
			// error/empty state
			bool error;
			
			// gdb internal variable name
			String varName;

			// short name
			String shortExpression;
			
			// evaluable expression
			String evaluableExpression;
			
			// type
			String type;
			
			// kind
			int kind;
			
			// value of expression for non-sequence types
			String value;
			
			// children
			int numChildren;
			
			// dynamic varobj--number of children must be queried
			bool dynamic;
			
			// check if value contains an error
			bool IsError(void) const;
			bool operator!(void) const { return IsError(); }
			operator bool() { return !IsError(); }

			// constructor
			VarItem();
		};
		
		// evaluate an expression usign gdb variables
		VarItem EvalGdb(String const &expr);
		
		// remove GDB variable for item
		void KillVariable(VarItem &v);
		
		// fetch variable children
		Vector<VarItem> GetChildren(MIValue const &val, String const &prePath = "");

		// fetch variable children
		Vector<VarItem> GetChildren(VarItem &v, int minChild = -1, int maxChild = -1);
		

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
		
		// stored local variable expressions, and values
		Index<String>localVarNames;
		Vector<String>localVarValues;
		
		// stored watches expressions and values
		Index<String>watchesExpressions;
		Vector<String>watchesValues;
		
		// 'this' variable inspection data
		Vector<String>thisExpressions;
		Vector<String>thisValues;
		Vector<int>thisHints;
		
		// stored autos expressions, values and types
		String autoLine;

		// update local variables on demand
		void UpdateLocalVars(void);
		
		// update 'this' inspector data
		void UpdateThis(void);
		
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
		void SyncLocals(void);

		// sync 'this' members pane
		void SyncMembers(void);
		
		// sync watches treectrl
		void SyncWatches();

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

		// sends pretty-prinding scripts
		void SendPrettyPrinters(void);
	
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

		// known types simplifier
		// takes a MIValue from '-data-evaluate-expression' command and try
		// do simplify diplay of known types
		void TypeSimplify(MIValue &val);

		// collects evaluated variables got with Evaluate
		// hints are used to choose the visualizer when deep-inspecting members
		// 0 for simple values, 1 for arrays, 2 for map
		void CollectVariables(MIValue &val, Vector<String> &exprs, Vector<String> &vals, Vector<int> &hints);

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


		// variable inspection support
		// returns a MIValue with inspected data and some info fields added
		// and known types simplified and cathegorized
		// unknown and simple types are left as they are
		MIValue Evaluate(String expr);
};

#endif
