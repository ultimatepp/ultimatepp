struct ExeContext {
	const Renderer *renderer;
	Vector<Value>&  stack;
	StringBuffer    out;

	ExeContext(Vector<Value>& stack, const Renderer *r = NULL) : renderer(r), stack(stack) {}
};

struct Exe {
	virtual Value Eval(ExeContext& x) const { return Value(); }
	virtual ~Exe() {}
};

struct RawHtmlText : ValueType<RawHtmlText, 54321> {
	String ToString() const;
	String text;
};

Value  Raw(const String& s);

void   EscapeHtml(StringBuffer& out, const String& txt);
String EscapeHtml(const String& s);

struct Compiler {
	static VectorMap<String, Value (*)(const Vector<Value>&, const Renderer *)>& functions();

	static bool IsTrue(const Value& v);
	
	struct Exe1 : Exe {
		One<Exe> a;
	};

	template <class T>
	static One<Exe> Create(One<Exe> a) {
		One<Exe> rr;
		T& m = rr.Create<T>();
		m.a = pick(a);
		return rr;
	}

	struct Exe2 : Exe {
		One<Exe> a;
		One<Exe> b;
	};

	template <class T>
	static One<Exe> Create(One<Exe> a, One<Exe> b) {
		One<Exe> rr;
		T& m = rr.Create<T>();
		m.a = pick(a);
		m.b = pick(b);
		return rr;
	}

	struct ExeVar : Exe {
		int var_index;

		virtual Value Eval(ExeContext& x) const;
	};

	struct ExeConst : Exe {
		Value value;
		
		virtual Value Eval(ExeContext& x) const;
	};

	struct ExeArray : Exe {
		Vector< One<Exe> > item;
		
		virtual Value Eval(ExeContext& x) const;
	};

	struct ExeMap : Exe {
		Vector< One<Exe> > key;
		Vector< One<Exe> > value;
		
		virtual Value Eval(ExeContext& x) const;
	};
	
	struct ExeNot : Exe1 { virtual Value Eval(ExeContext& x) const; };
	struct ExeNeg : Exe1 { virtual Value Eval(ExeContext& x) const; };
	struct ExeMul : Exe2 { virtual Value Eval(ExeContext& x) const; };
	struct ExeDiv : Exe2 { virtual Value Eval(ExeContext& x) const; };
	struct ExeMod : Exe2 { virtual Value Eval(ExeContext& x) const; };
	struct ExeAdd : Exe2 { virtual Value Eval(ExeContext& x) const; };
	struct ExeSub : Exe2 { virtual Value Eval(ExeContext& x) const; };
	struct ExeSll : Exe2 { virtual Value Eval(ExeContext& x) const; };
	struct ExeSra : Exe2 { virtual Value Eval(ExeContext& x) const; };
	struct ExeSrl : Exe2 { virtual Value Eval(ExeContext& x) const; };
	struct ExeLt  : Exe2 { virtual Value Eval(ExeContext& x) const; };
	struct ExeLte : Exe2 { virtual Value Eval(ExeContext& x) const; };
	struct ExeEq  : Exe2 { virtual Value Eval(ExeContext& x) const; };
	struct ExeNeq : Exe2 { virtual Value Eval(ExeContext& x) const; };
	struct ExeAnd : Exe2 { virtual Value Eval(ExeContext& x) const; };
	struct ExeXor : Exe2 { virtual Value Eval(ExeContext& x) const; };
	struct ExeOr  : Exe2 { virtual Value Eval(ExeContext& x) const; };
	struct ExeAnl : Exe2 { virtual Value Eval(ExeContext& x) const; };
	struct ExeOrl : Exe2 { virtual Value Eval(ExeContext& x) const; };
	
	struct ExeCond : Exe {
		One<Exe> cond;
		One<Exe> ontrue;
		One<Exe> onfalse;
		
		virtual Value Eval(ExeContext& x) const;
	};

	struct ExeField : Exe {
		One<Exe> value;
		String   id;

		virtual Value Eval(ExeContext& x) const;
	};
	
	struct ExeBracket : Exe {
		One<Exe> value;
		One<Exe> index;

		virtual Value Eval(ExeContext& x) const;
	};
	
	struct ExeVarField : Exe {
		int    var_index;
		String id;

		virtual Value Eval(ExeContext& x) const;
	};
	
	struct ExeFn : Exe {
		Value (*fn)(const Vector<Value>&, const Renderer *);
		
		Vector< One<Exe> > arg;

		virtual Value Eval(ExeContext& x) const;
	};

	struct ExeLink : Exe {
		const Vector<String> *part;
		
		Vector< One<Exe> > arg;

		virtual Value Eval(ExeContext& x) const;
	};

	struct ExeLinkVarField1 : Exe {
		const Vector<String> *part;
		
		int    var_index;
		String id;

		virtual Value Eval(ExeContext& x) const;
	};
	
	struct LoopInfo {
		bool  first;
		bool  last;
		int   index;
		Value key;
	};
	
	struct ExeFirst : ExeVar { virtual Value Eval(ExeContext& x) const; };
	struct ExeLast  : ExeVar { virtual Value Eval(ExeContext& x) const; };
	struct ExeIndex : ExeVar { virtual Value Eval(ExeContext& x) const; };
	struct ExeKey   : ExeVar { virtual Value Eval(ExeContext& x) const; };
	
	struct ExeFor : Exe {
		One<Exe> value;
		One<Exe> body;
		One<Exe> onempty;

		virtual Value Eval(ExeContext& x) const;
	};

	struct ExeBlock : Exe {
		Vector< One<Exe> > item;
		
		void AddText(const char *b, const char *s);

		virtual Value Eval(ExeContext& x) const;
	};

	struct CompiledTemplate {
		String path;
		Index<String> var;

		Exe    program;
		
		Vector<Value> data;
		Value Eval();
	};
	
	CParser       p;
	Index<String> var;
	Vector<bool>  forvar;
	bool          optimized;
	int           count_node;

	int ForVar(String id, int i);

	void     DoExeField(One<Exe>& result);
	void     DoIndicies(One<Exe>& result);

	One<Exe> Prim();
	One<Exe> Mul();
	One<Exe> Add();
	One<Exe> Shift();
	One<Exe> Rel();
	One<Exe> Eq();
	One<Exe> And();
	One<Exe> Xor();
	One<Exe> Or();
	One<Exe> LogAnd();
	One<Exe> LogOr();
	One<Exe> Conditional();
	
	One<Exe> Exp();
	
	One<Exe> Block();
	
	typedef Compiler CLASSNAME;

	void Iterate(Vector< One<Exe> >& a, Callback1< One<Exe>& > op);
	void Iterate(One<Exe>& exe, Callback1< One<Exe>& > op);
	void OptimizeConst(One<Exe>& exe);
	void Optimize(One<Exe>& exe);

	void CountNodes(One<Exe>& exe);

	int  GetNodeCount(One<Exe>& exe);

	static void Register(const String& id, Value (*fn)(const Vector<Value>&, const Renderer *));

	Compiler(const char *code, const Index<String>& var) : p(code), var(var, 1) { forvar.SetCount(var.GetCount(), false); }
};

One<Exe> Compile(const char *code, const Index<String>& vars);

String   Render(const One<Exe>& exe, Renderer *r, Vector<Value>& var);

String   GetPreprocessedTemplate(const String& name, int lang);

String   ReplaceVars(const String& src, const VectorMap<String, String>& def, int chr);
