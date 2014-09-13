#include <WinVer.h>
#include <DbgHelp.h>
#include <psapi.h>
#include "cvconst.h"

#include <plugin/ndisasm/ndisasm.h>

#define KEYGROUPNAME "Pdb debugger"
#define KEYNAMESPACE PdbKeys
#define KEYFILE      <ide/Debuggers/Pdb.key>
#include             <CtrlLib/key_header.h>

using namespace PdbKeys;

struct Pdb : Debugger, ParentCtrl {
	virtual void Stop();
	virtual bool IsFinished();
	virtual bool Key(dword key, int count);
	virtual void DebugBar(Bar& bar);
	virtual bool SetBreakpoint(const String& filename, int line, const String& bp);
	virtual bool RunTo();
	virtual void Run();
	virtual bool Tip(const String& exp, CodeEditor::MouseTip& mt);
	virtual void Serialize(Stream& s);

	struct ModuleInfo : Moveable<ModuleInfo> {
		adr_t  base;
		dword  size;
		String path;
		bool   symbols;

		ModuleInfo() { base = size = 0; symbols = false; }
	};

	struct FilePos {
		String path;
		int    line;
		adr_t  address;

		operator bool() const                              { return !IsNull(path); }

		FilePos()                                          { line = 0; address = 0; }
		FilePos(const String& p, int l) : path(p), line(l) { address = 0; }
	};

	enum CpuRegisterKind {
		REG_L, REG_H, REG_X, REG_E, REG_R
	};
	
	struct CpuRegister : Moveable<CpuRegister> {
		int         sym;    // DbgHelp register symbol
		const char *name;   // NULL: Do not list (e.g. al, as it is printed as EAX or RAX
		int         kind;   // CpuRegisterKind
		dword       flags;  // Unused
	};
	
	struct MemPg : Moveable<MemPg> {
		char data[1024];
	};

	struct CdbHexView : HexView {
		Pdb *cdb;

		virtual int Byte(int64 addr)            { return cdb->Byte((adr_t)addr); }

		CdbHexView()                            { SetTotal(0x80000000); }
	}
	memory;

	struct FnInfo : Moveable<FnInfo> {
		String name;
		adr_t  address;
		dword  size;
		dword  pdbtype;

		FnInfo() { address = size = pdbtype = 0; }
	};

	enum { UNKNOWN = -99, BOOL1, SINT1, UINT1, SINT2, UINT2, SINT4, UINT4, SINT8, UINT8, FLT, DBL, PFUNC };

	struct Val : Moveable<Val> {
		int    type;
		int    ref;
		bool   array:1;
		bool   rvalue:1;
		byte   bitpos;
		byte   bitcnt;
		union {
			adr_t  address;
			int64  ival;
			double fval;
		};
		
#ifdef _DEBUG
		String ToString() const;
#endif

		Val() { type = UNKNOWN; rvalue = false; ref = 0; array = false; bitcnt = 0; address = 0; }
	};

	struct NamedVal : Moveable<NamedVal> {
		String name;
		Val    val;
	};

	struct Type : Moveable<Type> {
		Type() : size(-1), vtbl_typeindex(-1) {}

		adr_t  modbase;

		String name;
		int    size;

		int    vtbl_typeindex;
		int    vtbl_offset;

		Vector<Val>            base;
		VectorMap<String, Val> member;
		VectorMap<String, Val> static_member;
	};

	struct Frame : Moveable<Frame> {
		adr_t                  pc, frame, stack;
		FnInfo                 fn;
		VectorMap<String, Val> param;
		VectorMap<String, Val> local;
	};

	struct VisualPart : Moveable<VisualPart> {
		String  text;
		Color   ink;
		bool    mark;
	};

	struct Visual {
		int                length;
		Vector<VisualPart> part;

		void   Cat(const String& text, Color ink = SColorText);
		void   Cat(const char *text, Color ink = SColorText);
		String GetString() const;
		void   Clear() { part.Clear(); length = 0; }
		Size   GetSize() const;

		Visual()       { length = 0; }
	};

	struct VisualDisplay : Display {
		virtual void Paint(Draw& w, const Rect& r, const Value& q,
	                       Color ink, Color paper, dword style) const;
	};
	
	struct Context {
	#ifdef CPU_64
		union {
			CONTEXT context64;
			WOW64_CONTEXT context32;
		};
	#else
		CONTEXT context32;
	#endif
	};

	struct Thread : Context {
		HANDLE  hThread;
		adr_t   sp;
	};

	int                      	lock;
	bool                     	running;
	bool                     	stop;
	HANDLE                   	hProcess;
	HANDLE                      mainThread;
	DWORD                    	processid;
	DWORD                       mainThreadId;
	ArrayMap<dword, Thread>     threads;
	bool                     	terminated;
	bool                     	refreshmodules;
	Vector<ModuleInfo>       	module;
	DEBUG_EVENT              	event;
	DWORD                       debug_threadid;
	HWND                     	hWnd;
	VectorMap<adr_t, byte>      bp_set; // breakpoints active for single RunToException

	bool                        win64; // debugee is 64-bit, always false in 32-bit exe

	Context                     context;

	Index<adr_t>            	invalidpage;
	VectorMap<adr_t, MemPg> 	mempage;

	Index<adr_t>                breakpoint;
	Vector<String>              breakpoint_cond;

	ArrayMap<int, Type>     	type;

	String                  	disas_name;

	Array<Frame>            	frame;
	Frame                      *current_frame;
	String                  	autotext;
	
	VectorMap<adr_t, FnInfo>    fninfo_cache;


	DbgDisas           disas;

	EditString         watchedit;
	
	enum { // Order in this enum has to be same as order of tab.Add
		TAB_AUTOS, TAB_LOCALS, TAB_THIS, TAB_WATCHES, TAB_EXPLORER, TAB_CPU, TAB_MEMORY
	};

	TabCtrl            tab;
	DropList           threadlist;
	DropList           framelist;
	Label              dlock;
	ArrayCtrl          locals;
	ArrayCtrl          self;
	ArrayCtrl          watches;
	ArrayCtrl          autos;
	ArrayCtrl          explorer;
	ColumnList         cpu;
	EditString         expexp;
	Button             exback, exfw;
	StaticRect         explorer_pane;
	StaticRect         pane;
	Splitter           split;
	TreeCtrl           tree;

	VectorMap<String, String> treetype;

	Vector<String>      exprev, exnext;

	Index<String>          noglobal;
	VectorMap<String, Val> global;
	
	bool       break_running; // Needed for Wow64 BreakRunning to avoid ignoring breakpoint

	void       Error(const char *s = NULL);
	
	String     Hex(adr_t);

// CPU registers
	uint32 GetRegister32(const Context& ctx, int sym);
	uint64 GetRegister64(const Context& ctx, int sym);

	const VectorMap<int, CpuRegister>& GetRegisterList();
	uint64     GetCpuRegister(const Context& ctx, int sym);

// debug
	Context    ReadContext(HANDLE hThread);
	void       WriteContext(HANDLE hThread, Context& context);
	void       LoadModuleInfo();
	int        FindModuleIndex(adr_t base);
	void       UnloadModuleSymbols();
	void       AddThread(dword dwThreadId, HANDLE hThread);
	void       RemoveThread(dword dwThreadId);
	void       Lock();
	void       Unlock();
	void       ToForeground();
	bool       RunToException();
	bool       AddBp(adr_t address);
	bool       RemoveBp(adr_t address);
	bool       RemoveBp();
	bool       IsBpSet(adr_t address) const { return bp_set.Find(address) >= 0; }
	bool       Continue();
	bool       SingleStep();
	void       BreakRunning();
	bool       ConditionalPass();
	void       SetBreakpoints();
	void       SaveForeground();
	void       RestoreForeground();

	adr_t      GetIP();

	void       WriteContext();

// mem
	int        Byte(adr_t addr);
	bool       Copy(adr_t addr, void *ptr, int count);
	String     ReadString(adr_t addr, int maxlen = INT_MAX);
	WString    ReadWString(adr_t addr, int maxlen = INT_MAX);

// sym
	struct LocalsCtx;
	static BOOL CALLBACK  EnumLocals(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext);
	static BOOL CALLBACK  EnumGlobals(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext);
	void                  TypeVal(Pdb::Val& v, int typeId, adr_t modbase);
	String                GetSymName(adr_t modbase, dword typeindex);
	dword                 GetSymInfo(adr_t modbase, dword typeindex, IMAGEHLP_SYMBOL_TYPE_INFO info);
	const Type&           GetType(int ti);
	int                   GetTypeIndex(adr_t modbase, dword typeindex);
	Val                   GetGlobal(const String& name);

	adr_t                 GetAddress(FilePos p);
	FilePos               GetFilePos(adr_t address);
	FnInfo                GetFnInfo0(adr_t address);
	FnInfo                GetFnInfo(adr_t address);
	void                  GetLocals(Frame& frame, Context& context,
	                                VectorMap<String, Pdb::Val>& param,
	                                VectorMap<String, Pdb::Val>& local);
	String                TypeAsString(int ti, bool deep = true);

// exp
	void       ThrowError(const char *s);
	int        SizeOfType(int ti);
	Val        GetRVal(Val v);
	Val        DeRef(Val v);
	Val        Ref(Val v);
	int64      GetInt(Val v);
	double     GetFlt(Val v);
	void       ZeroDiv(double x);
	Val        Compute(Val v1, Val v2, int oper);
	Val        RValue(int64 v);
	Val        Field0(Pdb::Val v, const String& field);
	Val        Field(Pdb::Val v, const String& field);
	Val        Term(CParser& p);
	Val        Post(CParser& p);
	Val        Unary(CParser& p);
	Val        Additive(CParser& p);
	Val        Multiplicative(CParser& p);
	Val        Compare(Val v, CParser& p, int r1, int r2);
	void       GetBools(Val v1, Val v2, bool& a, bool& b);
	Val        LogAnd(CParser& p);
	Val        LogOr(CParser& p);
	Val        Comparison(CParser& p);
	Val        Exp0(CParser& p);
	Val        Exp(CParser& p);
	void       CatInt(Visual& result, int64 val);
	void       Visualise(Visual& result, Pdb::Val val, int expandptr, int slen);
	void       Visualise(Visual& result, Pdb::Val val, int expandptr);
	Visual     Visualise(Val v);
	Visual     Visualise(const String& rexp);

// code
	Thread&    Current();
	int        Disassemble(adr_t ip);
	bool       IsValidFrame(adr_t eip);
	void       Sync0(Thread& ctx, Frame *single_frame);
	void       Sync();
	void       SetThread();
	void       SetFrame();
	bool       Step(bool over);
	void       Trace(bool over);
	void       StepOut();
	void       DoRunTo() { RunTo(); }
	adr_t      CursorAdr();
	void       SetIp();

	void       Break();

// data
	static VectorMap<String, Value> DataMap(const ArrayCtrl& a);
	static Value Vis(const String& key, const VectorMap<String, Value>& prev,
	                 Visual rval_ vis, bool& ch);
	static void  Vis(ArrayCtrl& a, const String& key,
	                 const VectorMap<String, Value>& prev, Visual rval_ vis);

	void      DisasCursor() {}
	void      DisasFocus() {}
	void      Locals();
	void      Watches();
	void      TryAuto(const String& exp, const VectorMap<String, Value>& prev);
	void      Autos();
	void      AddThis(const VectorMap<String, Val>& m, adr_t address, const VectorMap<String, Value>& prev);
	void      AddThis(int type, adr_t address, const VectorMap<String, Value>& prev);
	void      This();
	void      Explore(const Val& val, const VectorMap<String, Value>& prev);
	void      Explore(const String& exp);
	void      ExploreKey(ArrayCtrl *a);
	void      Explorer();
	void      ExpExp();
	void      ExBack();
	void      ExFw();
	void      DoExplorer();
	String    GetExpExp();
	void      ExplorerTree();
	void      Data();
	void      ClearWatches();
	void      DropWatch(PasteClip& clip);
	void      AddWatch();
	void      EditWatch();
	void      RemoveWatch();

	void      SetTab(int i);

	void      SetTree(const String& exp);
	void      SetTreeA(ArrayCtrl *data);
	void      TreeNode(int parent, const String& name, Val val);
	void      TreeExpand(int node);
	String    StoreTree(int parent);
	void      SaveTree();
	void      ExpandTreeType(int parent, CParser& p);

	void      CopyStack();
	void      CopyDisas();

	void      MemoryGoto(const String& exp);
	
	void      MemMenu(ArrayCtrl& array, Bar& bar, const String& exp);
	void      DataMenu(ArrayCtrl& array, Bar& bar, const String& exp);
	void      AutosMenu(Bar& bar);
	void      LocalsMenu(Bar& bar);
	void      WatchesMenu(Bar& bar);
	void      ExplorerMenu(Bar& bar);

	void      Tab();

	bool      Create(One<Host> host, const String& exefile, const String& cmdline);

	void      SerializeSession(Stream& s);

	typedef Pdb CLASSNAME;

	Pdb();
	virtual ~Pdb();


	void LoadGlobals(DWORD64 base);
};
