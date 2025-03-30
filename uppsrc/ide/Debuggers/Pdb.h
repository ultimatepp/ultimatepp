#include <winver.h>
#include <dbghelp.h>
#include <psapi.h>
#include "cvconst.h"

#include <plugin/ndisasm/ndisasm.h>

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

	struct PdbHexView : HexView {
		Pdb *pdb;

		virtual int Byte(int64 addr)            { return pdb ? pdb->Byte((adr_t)addr) : 0; }

		PdbHexView()                            { pdb = NULL; }
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
	
	struct TypeInfo : Moveable<TypeInfo> {
		int    type = UNKNOWN;
		int    ref = 0; // this is pointer or reference
		bool   reference = false; // this is reference
	};

	struct Val : Moveable<Val>, TypeInfo {
		bool   array = false;
		bool   rvalue = false; // data is loaded from debugee (if false, data pointed to by address)
		bool   udt = false; // user defined type (e.g. struct..)
		byte   bitpos = 0;
		byte   bitcnt = 0;
		int    reported_size = 0; // size of symbol, can be 0 - unknown, useful for C fixed size arrays
		union {
			adr_t  address;
			int64  ival;
			double fval;
		};
		Context *context = NULL; // needed to retrieve register variables

#ifdef _DEBUG
		String ToString() const;
#endif

		Val() { address = 0; }
	};

	struct NamedVal : Moveable<NamedVal> {
		String name;
		Val    val;
		Val    key;
		int64  from = 0;
		int64  chunk = 10000;
		bool   exp = false;
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
		Vector<int>            member_type;
	};

	struct Frame : Moveable<Frame> {
		adr_t                  pc, frame, stack;
		FnInfo                 fn;
		VectorMap<String, Val> param;
		VectorMap<String, Val> local;
		String                 text;
	};

	struct VisualPart : Moveable<VisualPart> {
		String  text;
		Color   ink;
		bool    mark;
		
		Size    GetSize() const;
	};

	struct Visual {
		int                length;
		Vector<VisualPart> part;
		int                type = 0; // only need for Copy type for .dbg

		void   Cat(const String& text, Color ink = SColorText);
		void   Cat(const char *text, Color ink = SColorText);
		String GetString() const;
		void   Clear() { part.Clear(); length = 0; }
		Size   GetSize() const;

		Visual()       { length = 0; }
	};

	struct VisualDisplay : Display {
		Pdb *pdb;

		virtual Size GetStdSize(const Value& q) const;
		virtual void Paint(Draw& w, const Rect& r, const Value& q,
	                       Color ink, Color paper, dword style) const;

		VisualDisplay(Pdb *pdb) : pdb(pdb) {}
	} visual_display;

	struct PrettyImage {
		Size  size;
		adr_t pixels;
	};
	
	struct Thread : Context {
		HANDLE  hThread;
		adr_t   sp;
	};
	
	int                         lock;
	bool                        running;
	bool                        stop;
	HANDLE                      hProcess;
	HANDLE                      mainThread;
	DWORD                       processid;
	DWORD                       hProcessId;
	DWORD                       mainThreadId;
	ArrayMap<dword, Thread>     threads;
	bool                        terminated;
	bool                        refreshmodules;
	Vector<ModuleInfo>          module;
	DEBUG_EVENT                 event;
	DWORD                       debug_threadid;
	HWND                        hWnd;
	VectorMap<adr_t, byte>      bp_set; // breakpoints active for single RunToException

	bool                        clang; // we are in clang toolchain
	bool                        win64; // debugee is 64-bit, always false in 32-bit exe

	Context                     context;

	Index<adr_t>                invalidpage;
	VectorMap<adr_t, MemPg>     mempage;

	Index<adr_t>                breakpoint;
	Vector<String>              breakpoint_cond;

	ArrayMap<int, Type>         type; // maps pdb pSym->TypeIndex to type data
	VectorMap<String, int>      type_name; // maps the name of type to above 'type' index
	Index<adr_t>                type_bases; // index of modbases for types (usually one)
	VectorMap<String, int>      type_index; // maps the name of type to TypeIndex, loaded by SymEnumTypes

	String                      disas_name;

	Array<Frame>                frame;
	Frame                      *current_frame;
	String                      autotext;
	
	VectorMap<adr_t, FnInfo>    fninfo_cache;

	DbgDisas           disas;

	EditString         watchedit;
	
	enum { // Order in this enum has to be same as order of tab.Add
		TAB_AUTOS, TAB_LOCALS, TAB_THIS, TAB_WATCHES, TAB_CPU, TAB_MEMORY, TAB_BTS
	};

	TabCtrl            tab;
	DropList           threadlist;
	DropList           framelist;
	Button             frame_up, frame_down;
	Label              dlock;
	ArrayCtrl          locals;
	ArrayCtrl          self;
	ArrayCtrl          watches;
	ArrayCtrl          autos;
	ColumnList         cpu;
	EditString         expexp;
	Button             exback, exfw;
	StaticRect         explorer_pane;
	StaticRect         pane, rpane;
	TreeCtrl           tree;
	String             tree_exp;
	bool               first_exception = true;
	TreeCtrl           bts; // backtraces of all threads

	VectorMap<String, String> treetype;
	int restoring_tree = 0;

	Vector<String>      exprev, exnext;

	Index<String>          noglobal;
	VectorMap<String, Val> global;

		
	adr_t                       current_modbase; // so that we do not need to pass it as parameter to GetTypeInfo
	VectorMap<String, TypeInfo> typeinfo_cache;
	
	enum { SINGLE_VALUE, TEXT, CONTAINER };
	
	struct LengthLimit {};

	struct Pretty {
		int            kind; // SINGLE_VALUE, TEXT or CONTAINER
		int64          data_count = 0; // number of entries
		Vector<String> data_type; // type of data items (usually type_param), >1 - display as tuples, usually key : value
		Vector<adr_t>  data_ptr; // pointer to items (data_count.GetCount() * data_type.GetCount() items)
		Visual         text;
		bool           separated_types = false; // item tuples are in consequent arrays
		int            chunk = 10000; // maximum number of items to be displayed in treeview in single node

		void           Text(const char *s, Color color = SRed)   { text.Cat(s, color); }
		void           Text(const String& s, Color color = SRed) { text.Cat(s, color); }
		void           SetNull()                                 { Text("Null", SCyan); }
	};

	VectorMap<String, Tuple<int, Event<Val, const Vector<String>&, int64, int, Pdb::Pretty&>>> pretty;
	
	bool       break_running; // Needed for Wow64 BreakRunning to avoid ignoring breakpoint
	
	bool       show_type = false;
	bool       raw = false;
	
	int        bc_lvl = 0; // For coloring { } in pretty container display
	
	ArrayMap<String, EscValue> pretty_globals;
	ArrayMap<String, EscValue> pretty_scripts;

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
	void       SyncFrameButtons();

	adr_t      GetIP();

	void       WriteContext();

// mem
	int        Byte(adr_t addr);
	bool       Copy(adr_t addr, void *ptr, int count);
	String     ReadString(adr_t addr, int maxlen, bool allowzero = false);
	WString    ReadWString(adr_t addr, int maxlen, bool allowzero = false);

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
	int                   FindType(adr_t modbase, const String& name);

	String                TypeInfoAsString(TypeInfo tf);
	TypeInfo              GetTypeInfo(adr_t modbase, const String& name);
	TypeInfo              GetTypeInfo(const String& name) { return GetTypeInfo(current_modbase, name); } // only in Pretty...
	
	static String FormatString(const String& x) { return AsCString(x, INT_MAX, NULL, CheckUtf8(x) ? 0 : ASCSTRING_OCTALHI); }

// exp
	Val        MakeVal(const String& type, adr_t address);
	void       ThrowError(const char *s);
	int        SizeOfType(int ti);
	int        SizeOfType(const String& name);
	adr_t      Align(adr_t adr, int type);
	adr_t      PeekPtr(adr_t address);
	byte       PeekByte(adr_t address);
	word       PeekWord(adr_t address);
	dword      PeekDword(adr_t address);
	dword      Peek64(adr_t address);
	Val        GetRVal(Val v);
	Val        DeRef(Val v);
	Val        Ref(Val v);
	int64      GetInt64(Val v);
	int        GetInt(Val v)                                 { return (int)GetInt64(v); }
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
	
	bool       HasAttr(Pdb::Val record, const String& id);
	Val        GetAttr(Pdb::Val record, int i);
	Val        GetAttr(Pdb::Val record, const String& id);
	int64      GetInt64Attr(Pdb::Val v, const char *a);
	int        GetIntAttr(Pdb::Val v, const char *a)         { return (int)GetInt64Attr(v, a); }
	byte       GetByteAttr(Pdb::Val v, const char *a)        { return (byte)GetInt64Attr(v, a); }
	Val        At(Pdb::Val val, int i);
	Val        At(Pdb::Val record, const char *id, int i);
	int        IntAt(Pdb::Val record, const char *id, int i);
	void       CatInt(Visual& result, int64 val, dword flags = 0);

	enum       { MEMBER = 1, RAW = 2 };
	void       BaseFields(Visual& result, const Type& t, Pdb::Val val, dword flags, bool& cm, int depth);
	void       Visualise(Visual& result, Pdb::Val val, dword flags);
	Visual     Visualise(Val v, dword flags = 0);
	Visual     Visualise(const String& rexp, dword flags = 0);

	bool       VisualisePretty(Visual& result, Pdb::Val val, dword flags);

	bool       PrettyVal(Pdb::Val val, int64 from, int count, Pretty& p);

	void       PrettyString(Val val, const Vector<String>& tparam, int64 from, int count, Pretty& p);
	void       PrettyWString(Val val, const Vector<String>& tparam, int64 from, int count, Pretty& p);
	void       PrettyVector(Val val, const Vector<String>& tparam, int64 from, int count, Pretty& p);
	void       PrettyArray(Val val, const Vector<String>& tparam, int64 from, int count, Pretty& p);
	void       PrettyBiVector(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       PrettyBiArray(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       PrettyIndex(Val val, const Vector<String>& tparam, int64 from, int count, Pretty& p);
	void       PrettyMap(Pretty& p, Pretty& key, Pretty& value);
	void       PrettyVectorMap(Val val, const Vector<String>& tparam, int64 from, int count, Pretty& p);
	void       PrettyArrayMap(Val val, const Vector<String>& tparam, int64 from, int count, Pretty& p);
	void       PrettyDate(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       PrettyTime(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       PrettyColor(Pdb::Val val, const Vector<String>&, int64 from, int count, Pdb::Pretty& p);
	void       PrettyRGBA(Pdb::Val val, const Vector<String>&, int64 from, int count, Pdb::Pretty& p);
	void       PrettyImageBuffer(Pdb::Val val, const Vector<String>&, int64 from, int count, Pdb::Pretty& p);
	void       PrettyImg(Pdb::Val val, const Vector<String>&, int64 from, int count, Pdb::Pretty& p);
	void       PrettyFont(Pdb::Val val, const Vector<String>&, int64 from, int count, Pdb::Pretty& p);
	void       PrettyValueArray_(adr_t a, Pdb::Pretty& p);
	void       PrettyValueArray(Pdb::Val val, const Vector<String>&, int64 from, int count, Pdb::Pretty& p);
	void       PrettyValue(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pretty& p);
	void       PrettyValueMap_(adr_t a, Pdb::Pretty& p, int64 from, int count);
	void       PrettyValueMap(Pdb::Val val, const Vector<String>&, int64 from, int count, Pdb::Pretty& p);

	void       PrettyStdVector(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       PrettyStdString(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       TraverseTree(bool set, Val head, Val node, int64& from, int& count, Pdb::Pretty& p, int depth);
	void       TraverseTreeClang(bool set, int nodet, Val node, int64& from, int& count, Pdb::Pretty& p, int depth, int key_size, int value_size);
	void       PrettyStdTree(Pdb::Val val, bool set, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       PrettyStdListM(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p, bool map = false);
	void       PrettyStdList(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       PrettyStdForwardList(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       PrettyStdDeque(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       PrettyStdUnordered(Pdb::Val val, bool set, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p);
	void       PrettyStdAtomic(Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pdb::Pretty& p);

	void       LoadPrettyScripts();
	void       SetVal(EscValue& v, Pdb::Val val);
	bool       PrettyScript(const String& type, Pdb::Val val, const Vector<String>& tparam, int64 from, int count, Pretty& p);

// code
	Thread&    Current();
	Array<Frame> Backtrace(Thread& ctx, bool single_frame = false, bool thread_info = false);
	int        Disassemble(adr_t ip);
	bool       IsValidFrame(adr_t eip);
	void       Sync0(Thread& ctx);
	void       Sync();
	void       SetThread();
	void       FrameUpDown(int dir);
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
	                 Visual&& vis, bool& ch);
	static void  Vis(ArrayCtrl& a, const String& key,
	                 const VectorMap<String, Value>& prev, Visual&& vis);

	void      DisasCursor() {}
	void      DisasFocus() {}
	void      Locals();
	void      Watches();
	void      TryAuto(const String& exp, const VectorMap<String, Value>& prev);
	void      Autos();
	void      AddThis(const VectorMap<String, Val>& m, adr_t address, const VectorMap<String, Value>& prev);
	void      AddThis(int type, adr_t address, const VectorMap<String, Value>& prev);
	void      This();
	void      Data();
	void      ClearWatches();
	void      DropWatch(PasteClip& clip);
	void      AddWatch();
	void      AddWatch(const String& s);
	void      EditWatch();
	void      BTs();

	void      SetTab(int i);

	void      SetTree(const String& exp);
	void      SetTreeA(ArrayCtrl *data);
	void      PrettyTreeNode(int parent, Pdb::Val val, int64 from = 0);
	bool      TreeNode(int parent, const String& name, Val val, int64 from = 0, Color ink = SColorText(), bool exp = false);
	bool      TreeNodeExp(int parent, const String& name, Val val, int64 from = 0, Color ink = SColorText());
	void      TreeExpand(int node);
	String    GetTreeText(int id);
	void      GetTreeText(String& r, int id, int depth);
	void      TreeMenu(Bar& bar);
	void      TreeWatch();
	void      StoreTree(StringBuffer& r, int parent);
	void      SaveTree();
	int       FindChild(int parent, String id);
	void      ExpandTreeType(int parent, CParser& p);

	void      CopyStack();
	void      CopyStackAll();
	void      CopyDisas();
	void      CopyModules();

	void      MemoryGoto(const String& exp);
	
	void      CopyMenu(ArrayCtrl& array, Bar& bar);
	void      MemMenu(ArrayCtrl& array, Bar& bar, const String& exp);
	void      DataMenu(ArrayCtrl& array, Bar& bar);
	void      WatchMenu(Bar& bar, const String& exp);
	void      WatchesMenu(Bar& bar);
	
	void      CopyTypeScript(ArrayCtrl& array);
	void      CopyTypeScriptType(String& text, int type, const String& tabs, const String& id);

	void      SyncTreeDisas();

	void      Tab();

	bool      Create(Host& local, const String& exefile, const String& cmdline, bool clang_);

	void      SerializeSession(Stream& s);
	
	typedef Pdb CLASSNAME;

	Pdb();
	virtual ~Pdb();


	void LoadGlobals(DWORD64 base);
};

bool EditPDBExpression(const char *title, String& brk, Pdb *pdb);
