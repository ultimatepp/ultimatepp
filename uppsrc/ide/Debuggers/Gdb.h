class Gdb
	: public Debugger
	, public ParentCtrl
{
	typedef Gdb CLASSNAME;
	
public:
	virtual void DebugBar(Bar& bar) override;
	virtual bool SetBreakpoint(const String& filename, int line, const String& bp) override;
	virtual bool RunTo() override;
	virtual void Run() override;
	virtual bool Key(dword key, int count) override;
	virtual void Stop() override;
	virtual bool IsFinished() override;
	virtual bool Tip(const String& exp, CodeEditor::MouseTip& mt) override;

	void ShowException();

public:
	String DoRun();
	
	bool Result(String& result, const String& s);

	void      AddReg(const char *reg, Label *lbl) { regname.Add(reg); reglbl.Add(lbl); }

	void      Lock();
	void      Unlock();

	void      SyncFrameButtons();

	String    Cmd(const char *command, bool start = false);
	String    FastCmd(const char *command);
	
	bool      IsProcessExitedNormally(const String& cmd_output) const;
	String    ObtainThreadsInfo();
	
	bool      TryBreak(const char *command);
	void      CheckEnd(const char *result);

	void      Step(const char *cmd);

	String    Cmdp(const char *cmdline, bool fr = false, bool setframe = true);

	void      DoRunTo() { RunTo(); }

	void      SetDisas(const String& text);
	void      SyncDisas(bool fr);

	void      DisasCursor();
	void      DisasFocus();
	void      DropFrames();
	void      LoadFrames();
	
	void      SwitchFrame();
	void      FrameUpDown(int dir);
	void      SwitchThread();
	
	void      ClearCtrls();
	
	static void ReadGdbValues(CParser& p, VectorMap<String, String>& val);
	static void ReadGdbValues(const String& h, VectorMap<String, String>& val);

	String    Print0(const String& exp);
	String    Print(const String& exp);
	void      Locals();
	void      Watches();
	void      TryAuto(Index<String>& tested, const String& exp);
	void      Autos();
	void      ObtainData();
	void      Self();
	void      Cpu();
	void      ClearWatches();
	void      QuickWatch();
	void      SetTab(int q);
	void      SetTree(ArrayCtrl *a);
	void      OnTreeBar(Bar& bar);
	void      WatchMenu(Bar& bar);
	void      OnTreeExpand(int node);
	void      MemoryGoto();
	void      MemoryLoad(const String& adr, int count, bool showerror);
	void      Memory();
	void      MemoryMenu(Bar& bar, const String& exp);

	void      OnValueCopyToClipboard(const Value& val);
	
	Value     ObtainValueFromTreeCursor(int cursor) const;
	
	void      CopyStack();
	void      CopyStackAll();
	void      CopyDisas();
	
	void      BreakRunning();
	
	bool      Create(Host& host, const String& exefile, const String& cmdline, bool console);
	
	TimeCallback periodic; // Period check for killed console
	void Periodic();

	void SerializeSession(Stream& s);
	
	virtual ~Gdb() override;
	Gdb();
	
protected:
	LocalProcess       dbg;

	Vector<String>     regname;
	Vector<Label *>    reglbl;

	DbgDisas           disas;

	bool               nodebuginfo = false;
	StaticRect         nodebuginfo_bg;
	StaticText         nodebuginfo_text;

	EditString         watchedit;
	DropList           threads;
	DropList           frame;
	Button             frame_up, frame_down;
	TabCtrl            tab;
	ArrayCtrl          locals;
	ArrayCtrl          watches;
	ArrayCtrl          autos;
	ArrayCtrl          self;
	ColumnList         cpu;
	Label              dlock;
	StaticRect         pane;
	Splitter           split;
	TreeCtrl           tree;
	struct GdbHexView : HexView {
		uint64       start;
		Vector<byte> data;

		virtual int Byte(int64 i)            { return i >= 0 && i < data.GetCount() ? data[(int)i] : 0; }
	}
	memory;

	VectorMap<String, String> expression_cache;

	WithQuickwatchLayout<TopWindow> quickwatch;

	String             file;
	int                line;
	adr_t              addr;

	FileOut            log;
	
	String             autoline;
	bool               firstrun;
	bool               running_interrupted;
	
	int                pid = 0; // debugee pid

	String             bp_filename;
	int                bp_line;
	String             bp_val;
	
	String             exception;
	
	const int max_stack_trace_size = 400;
	
	One<IGdbUtils> gdb_utils;
};
