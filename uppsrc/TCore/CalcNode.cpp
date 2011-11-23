#pragma warning(disable: 4786)

#include "TCoreCalc.h"
#pragma hdrstop

NAMESPACE_UPP

#define LLOG(x) // LOG(x)

String CalcType<const CalcNode *>::Describe() { return t_("lambda-expression"); }
RegisterStdCalcTypeName(const CalcNode *)

String CalcType<CalcNodePtr>::Describe() { return t_("lambda-expression"); }
RegisterStdCalcTypeName(CalcNodePtr)

int CharFilterCalcIdent(int c)      { return IsIdent(c) ? c : 0; }
int CharFilterCalcUpperIdent(int c) { return IsIdent(c) ? ToUpper(c) : 0; }

RegisterHelpTopicTitle("CalcNode", t_("Expression evaluator"))

enum
{
	MAX_NESTING_DEPTH = 1000,
};

HelpCalc::HelpCalc(CalcGate proc, const char *ident_, const char *topic_,
	String (*groupfn_)(), const char *function_)
//: generator(gen)
{
	Init(proc, ident_, topic_, groupfn_, function_);
	AddHelpCalc(*this);
//	ord = HelpCalcIndex::Get().Add(*this);
}

void HelpCalc::Init(CalcGate proc_, const char *ident_, const char *topic_,
	String (*groupfn_)(), const char *function_)
{
	proc = proc_;
	ident = ident_;
	topic = topic_;
	groupfn = groupfn_;
	function = function_;
/*
	sortname = id = _id;

	const char *p = (head ? head : "");
	const char *b = p;

	if(*p == '|')
	{ // sort name
		b = ++p;
		sortname.Clear();
		while(*p && *p != '|' && *p != '\n')
		{
			if((*p == '\\' || *p == '`') && p[1])
				p++;
			sortname << *p++;
		}
		if(*p == '|')
			p++;
		while(*p && *p != '\n' && *p != '\t' && (byte)*p <= ' ')
			p++;
		b = p;
	}

	while(*p && *p != '\n' && *p != '\t' && *p != '(')
		p++;
	const char *e = p;
	while(e > b && e[-1] == ' ')
		e--;
	if(e > b)
		result = String(b, e);

	if(*p == '(')
	{ // arguments given
		p++;
		do
		{
			while(*p && (byte)*p <= ' ')
				p++;
			if(*p == ')')
			{
				p++;
				break;
			}
			if(*p == ':')
			{ // everything is type
				b = ++p;
				while(*p && *p != '\n' && *p != ',' && *p != ')')
					p++;
				arg_types.Add(String(b, p));
				arg_names.Add("");
			}
			else
			{ // name or type + name
				const char *s = b = p;
				while(*p && *p != '\n' && *p != ',' && *p != ')')
					if(*p == '\1')
					{ // skip inline part
						while(*p && *p++ != '\1')
							;
					}
					else if((*p == '`' || *p == '\\') && p[1])
						p += 2;
					else if(*p == '[')
					{ // skip QTF leader
						p++;
						while(*p && *p++ != ' ')
							;
					}
					else if(*p++ == ' ')
						s = p;

				for(e = p; e > b && (byte)e[-1] <= ' '; e--)
					;

				if(s == b)
				{ // name only
					arg_types.Add("");
					arg_names.Add(String(b, e));
				}
				else
				{ // type name
					arg_names.Add(String(s, e));
					while(s > b && (byte)*s <= ' ')
						s--;
					arg_types.Add(String(b, s));
				}
			}
		}
		while(*p++ == ',');
		if(p[-1] != ')')
		{
			p--;
			NEVER(); // missing right parenthesis in function declaration (found p)
		}
	}
	while(*p && *p != '\t' && *p != '\n' && (byte)*p <= ' ')
		p++;
	if(*p == '\t')
	{ // group / subgroup given
		while(*++p && (byte)*p <= ' ')
			p++;
		b = p;
		while(*p && *p != '/' && *p != '\n')
			if((*p == '\\' || *p == '`') && *++p)
				p++; // explicit char
			else if(*p == '[')
			{ // skip qtf tags
				while(*p && *p++ != ' ')
					;
			}
			else
				p++;

		for(e = p; e > b && (byte)e[-1] <= ' '; e--)
			;
		group = String(b, e);
		stripped_group = StripQtf(group);

		if(*p == '/')
			p++;

		while(*p && *p != '\n' && (byte)*p <= ' ')
			p++;
		b = p;
		while(*p && *p != '\n')
			p++;
		for(e = p; e > b && (byte)e[-1] <= ' '; e--)
			;
		subgroup = String(b, e);
		stripped_subgroup = StripQtf(subgroup);
	}
	if(*p && *p != '\n')
		NEVER(); // end of line expected after arguments & group spec (found p)
	if(*p == '\n')
		p++;
	index_entries = p;

	CalcPacket packet(*(CalcContext *)0, id);
	packet.help = true;
	VERIFY(proc(packet));
	if(packet.args.GetCount() > arg_types.GetCount())
		arg_types.SetCount(packet.args.GetCount());
	if(arg_types.GetCount() > arg_names.GetCount())
		arg_names.SetCount(arg_types.GetCount());
	else
		arg_types.SetCount(arg_names.GetCount());

	for(int i = 0; i < packet.args.GetCount(); i++)
		if(IsNull(arg_types[i]))
			arg_types[i] = packet.args[i].Format();
	if(IsNull(result))
		result = packet.result.Format();
*/
}

String HelpCalc::GetTitle() const
{
	CalcPacket packet(*(CalcContext *)0, ident);
	packet.help = true;
	VERIFY(proc(packet));
	Vector<String> arg_types;
	arg_types.SetCount(packet.args.GetCount());
	for(int i = 0; i < packet.args.GetCount(); i++)
		arg_types[i] = StdFormat(packet.args[i]);
	String result = StdFormat(packet.result);
	String out;
	if(!IsAlpha(*ident))
		out << "operátor \1" << ident << "\1 ";
	else
		out << "\1" << ident << "\1";
	if(!arg_types.IsEmpty())
	{
		for(int i = 0; i < arg_types.GetCount(); i++)
			out << (i ? "\1, \1" : "(\1") << arg_types[i];
		out << "\1)";
	}
	out << " [S \xAE] \1" << result << "\1";
	return out;
}

HelpCalcMap& GetHelpCalcMap()
{
	static HelpCalcMap map;
	return map;
}

void AddHelpCalc(const HelpCalc& calc)
{
	GetHelpCalcMap().Add(&calc);
}

Vector<String> GetHelpCalcGroups()
{
	Index<String> group;
	const HelpCalcMap& map = GetHelpCalcMap();
	for(int i = 0; i < map.GetCount(); i++)
		if(map[i]->groupfn)
			group.FindAdd(map[i]->groupfn());
	Vector<String> gnames = group.PickKeys();
	Sort(gnames, GetLanguageInfo());
	if(gnames.GetCount() >= 2 && gnames[0].IsEmpty())
	{
		gnames.Remove(0);
		gnames.Add(Null);
	}
	return gnames;
}

static void HelpCalcAutoIndex(Index<WString>& entries, Vector<String>& drls)
{
	const HelpCalcMap& map = GetHelpCalcMap();
	for(int i = 0; i < map.GetCount(); i++)
	{
		const HelpCalc& hc = *map[i];
		WString entry;
		entry << WString("[/ \1") << hc.ident.ToWString()
			<< WString(IsAlpha(*hc.ident) ? "\1], funkce" : "\1], operátor");
		int f = entries.Find(entry);
		String drl = HelpAppDPP(hc.topic);
		if(f >= 0)
			drls[f] << ";!" << drl;
		else
		{
			entries.Add(entry);
			drls.Add('!' + drl);
		}
	}
}

HELPINITBLOCK { RegisterHelpAutoIndex(&HelpCalcAutoIndex); }

//////////////////////////////////////////////////////////////////////
// CalcFunctionNode::

Value CalcFunctionNode::Calc(CalcContext& context) const
{
	One<CalcPacket> packet = new CalcPacket(context, name);
	ScanArgs(*packet);
	return context.Calc(*packet);
}

String CalcFunctionNode::Format() const
{
	String out;
	bool is_oper = !IsAlpha(*name);
	if(is_oper)
		out << "operator ";
	out << name;
	if(!args.IsEmpty())
	{
		for(int i = 0; i < args.GetCount(); i++)
		{
			String s = args[i]->Format();
			if(IsNull(s))
				return Null;
			out << (i ? ", " : "(") << s;
		}
		out << ")";
	}
	else if(is_oper)
		out << "()";
	return out;
}

String CalcConstNode::Format() const
{
	if(IsNull(value))
		return "null";
	if(IsNumber(value))
		return FormatDouble(value, 15);
	if(IsString(value))
		return AsCString(String(value));
	if(IsDateTime(value))
	{
		Time tm(value);
		if(tm.hour == 0 && tm.minute == 0 && tm.second == 0)
			return NFormat("date(%d, %d, %d)", tm.year, tm.month, tm.day);
		else
			return NFormat("time(%d, %d, %d, %d, %d, %d)", tm.year, tm.month, tm.day, tm.hour, tm.minute, tm.second);
	}
	return Null;
}

Value CalcConstNode::Calc(CalcContext& context) const
{
	return value;
}

Value CalcLambdaNode::Calc(CalcContext& context) const
{
	return CalcType<const CalcNode *>::ToValue(~args[0]);
}

String CalcLambdaNode::Format() const
{
	String e = args[0]->Format();
	if(IsNull(e))
		return Null;
	return NFormat("#(%s)", e);
}

String CalcLogOrNode::Format() const
{
	String l = args[0]->Format(), r = args[1]->Format();
	if(IsNull(l) || IsNull(r))
		return Null;
	return NFormat("(%s) || (%s)", l, r);
}

CalcNodePtr CalcLogOrNode::Optimize(CalcContext& context)
{
	if(!args[0]->IsConst())
		return this;
	if(CalcType<bool>::ValueTo(args[0]->Calc(context)))
		return new CalcConstNode(1);
	return args[1];
}

Value CalcLogOrNode::Calc(CalcContext& context) const
{
	Value var = args[0]->Calc(context);
	if(!CalcType<bool>::IsType(var))
		Expect(t_("boolean value"), context.FormatNull(var));
	if(CalcType<bool>::ValueTo(var))
		return CalcType<bool>::ToValue(true);
	var = args[1]->Calc(context);
	if(!CalcType<bool>::IsType(var))
		Expect(t_("boolean value"), context.FormatNull(var));
	return CalcType<bool>::ToValue(CalcType<bool>::ValueTo(var));
}

String CalcLogAndNode::Format() const
{
	String l = args[0]->Format(), r = args[1]->Format();
	if(IsNull(l) || IsNull(r))
		return Null;
	return NFormat("(%s) && (%s)", l, r);
}

Value CalcLogAndNode::Calc(CalcContext& context) const
{
	Value var = args[0]->Calc(context);
	if(!CalcType<bool>::IsType(var))
		Expect(t_("boolean value"), context.FormatNull(var));
	if(!CalcType<bool>::ValueTo(var))
		return CalcType<bool>::ToValue(false);
	var = args[1]->Calc(context);
	if(!CalcType<bool>::IsType(var))
		Expect(t_("boolean value"), context.FormatNull(var));
	return CalcType<bool>::ToValue(CalcType<bool>::ValueTo(var));
}

CalcNodePtr CalcLogAndNode::Optimize(CalcContext& context)
{
	if(!args[0]->IsConst())
		return this;
	if(!CalcType<bool>::ValueTo(args[0]->Calc(context)))
		return new CalcConstNode(0);
	return args[1];
}

String CalcSelectNode::Format() const
{
	String c = args[COND]->Format(), t = args[IFTRUE]->Format(), f = args[IFFALSE]->Format();
	if(IsNull(c) || IsNull(t) || IsNull(f))
		return Null;
	return NFormat("(%s) ? (%s) : (%s)", c, t, f);
}

Value CalcSelectNode::Calc(CalcContext& context) const
{
	Value var = args[COND]->Calc(context);
	if(!CalcType<bool>::IsType(var))
		Expect(t_("boolean value"), context.FormatNull(var));
	return CalcType<bool>::ValueTo(var) ? args[IFTRUE]->Calc(context) : args[IFFALSE]->Calc(context);
}

CalcNodePtr CalcSelectNode::Optimize(CalcContext& context)
{
	if(!args[0]->IsConst())
		return this;
	return args[CalcType<bool>::ValueTo(args[0]->Calc(context)) ? 1 : 2];
}

String CalcSwitchNode::Format() const
{
	String out;
	String c = args[0]->Format();
	if(IsNull(c))
		return Null;
	out = NFormat("(%s){", c);
	for(int i = 1; i < args.GetCount(); i++)
	{
		String s = args[i]->Format();
		if(IsNull(s))
			return Null;
		out << (i == 1 ? "(" : ", (") << s << ')';
	}
	out << '}';
	return out;
}

Value CalcSwitchNode::Calc(CalcContext& context) const
{
	Value value = args[0]->Calc(context);
	if(args.GetCount() == 2)
	{ // special case - 1-arg switch works as Nvl
		if(!IsNull(value))
			return value;
		return args[1]->Calc(context);
	}
	int i = 0, t = (args.GetCount() - 1) & ~1;
	for(; i < t; i += 2)
		if(value == args[i + 1]->Calc(context))
			return args[i + 2]->Calc(context);
	if(i + 1 < args.GetCount())
		return args[i + 1]->Calc(context);
	else
		return value;
}

CalcNodePtr CalcSwitchNode::Optimize(CalcContext& context)
{
	if(!args[0]->IsConst())
		return this;
	Value value = args[0]->Calc(context);
	if(args.GetCount() == 2)
	{ // special case - 1-arg switch works as Nvl
		if(!IsNull(value))
			return new CalcConstNode(value);
		return args[1];
	}
	int i = 0, t = (args.GetCount() - 1) & ~1;
	for(; i < t; i += 2)
	{
		if(!args[i + 1]->IsConst())
			return this;
		if(value == args[i + 1]->Calc(context))
			return args[i + 2];
	}
	if(i + 1 < args.GetCount())
		return args[i + 1];
	else
		return new CalcConstNode(value);
}

CalcSequenceNode::CalcSequenceNode(pick_ Vector<CalcNodePtr> nodes)
: CalcNode(",")
{
	args = nodes;
}

CalcSequenceNode::CalcSequenceNode(CalcNodePtr node1, CalcNodePtr node2)
: CalcNode(",")
{
	args.SetCount(2);
	args[0] = node1;
	args[1] = node2;
}

Value CalcSequenceNode::Calc(CalcContext& context) const
{
	Value v;
	for(int i = 0; i < args.GetCount(); i++)
		if(!!args[i])
			v = args[i]->Calc(context);
	return v;
}

String CalcSequenceNode::Format() const
{
	StringBuffer out;
	for(int i = 0; i < args.GetCount(); i++)
		out << (i ? ", (" : "(") << args[i]->Format() << ')';
	return out;
}

CalcNodePtr CalcSequenceNode::Optimize(CalcContext& context)
{
	Vector<CalcNodePtr> opt;
	bool diff = false;
	opt.SetCount(args.GetCount());
	for(int i = 0; i < args.GetCount(); i++) {
		if(!!args[i] && args[i] != (opt[i] = args[i]->Optimize(context)))
			diff = true;
	}
	if(diff)
		return new CalcSequenceNode(opt);
	return this;
}

void CalcSymbols::Clear()
{
	var_index.Clear();
	var_value.Clear();
	var_const.Clear();
	functions.Clear();
}

void CalcSymbols::Remove(String name)
{
	int i = var_index.Find(name);
	if(i >= 0)
	{
		var_index.Remove(i);
		var_value.Remove(i);
		var_const.Remove(i);
	}
}

CalcContext::Global::Global(const String& name, CalcProc proc)
{
	GetGlobals().Add(name, proc);
}

CalcContext::CalcContext()
: nesting(0)
, opt_const(false)
{
	language = GetCurrentLanguage();
	UseCalcBasic();
	stack.Add();
}

CalcContext::CalcContext(const CalcContext& context, int) : nesting(context.nesting), stack(context.stack, 0) {}
CalcContext::~CalcContext() {}

Value CalcContext::Get(String name) const
{
	for(int l = stack.GetCount(); --l >= 0;)
	{
		const CalcSymbols& level = stack[l];
		int f = level.var_index.Find(name);
		if(f >= 0)
			return level.var_value[f];
	}
	return Value();
}

Value CalcContext::TryEvaluate(String expr)
{
	try {
		return Evaluate(expr);
	}
	catch(Exc e) {
		return ErrorValue(e);
	}
}

Value CalcContext::Evaluate(String expr)
{
	CalcNodePtr node = CalcParser().ScanVoid(expr);
	return !!node ? node->Calc(*this) : Value();
}

double CalcContext::EvaluateDouble(String expr)
{
	CalcNodePtr node = CalcParser().ScanVoid(expr);
	return !!node ? node->CalcDouble(*this) : double(Null);
}

String CalcContext::EvaluateString(String expr)
{
	CalcNodePtr node = CalcParser().ScanVoid(expr);
	return !!node ? node->CalcString(*this) : String(Null);
}

int CalcContext::EvaluateInt(String expr)
{
	CalcNodePtr node = CalcParser().ScanVoid(expr);
	return !!node ? node->CalcInt(*this) : int(Null);
}

int64 CalcContext::EvaluateInt64(String expr)
{
	CalcNodePtr node = CalcParser().ScanVoid(expr);
	return !!node ? node->CalcInt64(*this) : int(Null);
}

Date CalcContext::EvaluateDate(String expr)
{
	CalcNodePtr node = CalcParser().ScanVoid(expr);
	return !!node ? node->CalcDate(*this) : Date(Null);
}

Time CalcContext::EvaluateTime(String expr)
{
	CalcNodePtr node = CalcParser().ScanVoid(expr);
	return !!node ? node->CalcTime(*this) : Time(Null);
}

bool CalcContext::EvaluateBool(String expr, bool null_value)
{
	CalcNodePtr node = CalcParser().ScanVoid(expr);
	return !!node ? node->CalcBool(*this) : null_value;
}

String CalcContext::OptimizeConstant(String expr)
{
	CalcNodePtr in_expr = CalcParser().ScanVoid(expr);
	if(!in_expr)
		return expr;
	CalcNodePtr out_expr = OptimizeConstant(in_expr);
	if(out_expr == in_expr)
		return expr;
	return Nvl(out_expr->Format(), expr);
}

static CalcNodePtr OptimizeConstantRaw(CalcNodePtr src, CalcContext& context)
{
	CalcNodePtr clone = src->Clone();
	if(!clone)
		return src;
	bool modf = false;
	for(int i = 0; i < clone->args.GetCount(); i++)
	{
		CalcNodePtr opt = OptimizeConstantRaw(src->args[i], context);
		if(!!opt && opt != src->args[i])
		{
			modf = true;
			clone->args[i] = opt;
		}
	}
	if(modf)
		src = clone;
	src = src->Optimize(context);
	if(src->IsConst())
		return src;

	try
	{
		Value v = src->Calc(context);
		if(!v.IsError())
			return new CalcConstNode(v);
	}
	catch(Exc e)
	{
	}

	return src;
}

CalcNodePtr CalcContext::OptimizeConstant(CalcNodePtr src)
{
	if(!src)
		return src;
	try
	{
		opt_const = true;
		CalcNodePtr out = OptimizeConstantRaw(src, *this);
		opt_const = false;
		return out;
	}
	catch(Exc e)
	{
		opt_const = false;
		return src;
	}
}

String CalcContext::Convert(String s, bool throw_errors, const UPP::Convert& convert)
{
	if(s.IsEmpty())
		return Null;
	Vector<String> sparts;
	Vector<CalcNodePtr> cparts;
	ParseConvert(s, sparts, cparts, throw_errors);
	return CalcConvert(sparts, cparts, throw_errors, convert);
}

void CalcContext::ParseConvert(String s, Vector<String>& sparts, Vector<CalcNodePtr>& cparts, bool throw_errors)
{
	Vector<String> ctexts;
	ParseConvert(s, sparts, ctexts);
	cparts.SetCount(ctexts.GetCount());
	for(int i = 0; i < ctexts.GetCount(); i++)
		try {
			cparts[i] = CalcParser().ScanVoid(ctexts[i]);
		}
		catch(Exc e) {
			if(throw_errors)
				throw;
			cparts[i] = new CalcConstNode(e);
		}
}

void CalcContext::ParseConvert(String s, Vector<String>& sparts, Vector<String>& cparts)
{
	const char *p = s;
	sparts.Add();
	while(*p) {
		const char *b = p;
		while(*p && (*p != '<' || p[1] != ':'))
			p++;
		sparts.Top().Cat(b, p - b);
		if(*p) {
			p += 2;
			b = p;
			while(*p && (*p != ':' || p[1] != '>'))
				if(*p++ == '\"') {
					while(*p && *p != '\"')
						if(*p++ == '\\' && *p)
							p++;
					if(*p)
						p++;
				}
			cparts.Add(String(b, p));
			sparts.Add();
			if(*p)
				p += 2;
		}
	}
}

String CalcContext::CalcConvert(const Vector<String>& sparts, const Vector<CalcNodePtr>& cparts, bool throw_errors, const UPP::Convert& convert)
{
	String out;
	for(int i = 0; i < sparts.GetCount(); i++)
	{
		out.Cat(sparts[i]);
		if(i < cparts.GetCount())
			try
			{
				out.Cat(FormatText(cparts[i]->Calc(*this), convert));
			}
			catch(Exc e)
			{
				if(throw_errors)
					throw;
				out.Cat(e);
			}
	}
	return out;
}

Value CalcContext::Calc(CalcPacket& packet)
{
	try {
#ifdef _DEBUG
		int level = nesting;
		Value out = TryCalc(packet);
		ASSERT(nesting == level);
		return out;
#else
		return TryCalc(packet);
#endif
	}
	catch(Exc e) {
		LOG(e);
		throw Exc(packet.name + ": " + e);
	}
}

Value CalcContext::TryCalc(CalcPacket& packet)
{
	LLOG("TryCalc <- " << packet.name << " + " << packet.args.GetCount() << " args" << BeginIndent);

	// calculate locals & globals
	if(++nesting > MAX_NESTING_DEPTH) {
		nesting--;
		throw Exc(NFormat(t_("function nesting too deep (%d levels)"), MAX_NESTING_DEPTH));
	}

	bool none = true;

	for(int l = stack.GetCount(); --l >= 0;) {
		const CalcSymbols& level = stack[l];
		int i = level.functions.Find(packet.name);
		if(i >= 0) { // try functions
			none = false;
			for(; i >= 0; i = level.functions.FindNext(i))
				try {
					if(level.functions[i](packet)) {
						LLOG(EndIndent << "//TryCalc->" << StdFormat(packet.result));
						nesting--;
						return packet.result;
					}
				}
				catch(...) {
					nesting--;
					throw;
				}
		}
		if(packet.args.IsEmpty() && (i = level.var_index.Find(packet.name)) >= 0) {
			if(opt_const && !level.var_const[i]) {
				nesting--;
				throw Exc(NFormat(t_("%s is not a constant"), packet.name));
			}
			LLOG(EndIndent << "//TryCalc->" << StdFormat(level.var_value[i]));
			nesting--;
			return level.var_value[i];
		}
	}

	for(int e = externals.GetCount(); --e >= 0;) {
		const CalcSymbols& level = *externals[e];
		int i = level.functions.Find(packet.name);
		if(i >= 0) { // try functions
			none = false;
			for(; i >= 0; i = level.functions.FindNext(i))
				try {
					if(level.functions[i](packet)) {
						LLOG(EndIndent << "//TryCalc->" << StdFormat(packet.result));
						nesting--;
						return packet.result;
					}
				}
				catch(...) {
					nesting--;
					throw;
				}
		}
		if(packet.args.IsEmpty() && (i = level.var_index.Find(packet.name)) >= 0) {
			if(opt_const && !level.var_const[i]) {
				nesting--;
				throw Exc(NFormat(t_("%s is not a constant"), packet.name));
			}
			LLOG(EndIndent << "//TryCalc->" << StdFormat(level.var_value[i]));
			nesting--;
			return level.var_value[i];
		}
	}

	{ // try globals
		const CalcProcMap& globals = GetGlobals();
		int i = globals.Find(packet.name);
		if(i >= 0) {
			none = false;
			for(; i >= 0; i = globals.FindNext(i))
				try {
					if(globals[i](packet)) {
						LLOG(EndIndent << "//TryCalc->" << StdFormat(packet.result));
						nesting--;
						return packet.result;
					}
				}
				catch(...) {
					nesting--;
					throw;
				}
		}
	}

	nesting--;

	if(none)
		throw Exc((packet.args.IsEmpty() ? t_("variable/constant '") : t_("function '")) + packet.name + t_("' not found"));

	String s = packet.name;
	if(!packet.args.IsEmpty()) {
		bool no_text = (s == "text");
		s << '(' << FormatNull(packet.args[0], no_text);
		for(const Value *p = packet.args.Begin() + 1, *e = packet.args.End(); p < e; p++)
			s << ", " << FormatNull(*p, no_text);
		s << ')';
	}
	s << ": ";
	switch(packet.args.GetCount()) {
		case 0:  s << t_("missing arguments"); break;
		case 1:  s << t_("illegal type ") << packet.GetTypeNames() << t_(" or number of parameters"); break;
		default: s << t_("illegal type combination ") << packet.GetTypeNames(); break;
	}

	throw Exc(s);
}

CalcProcMap& CalcContext::GetGlobals()
{
	static CalcProcMap list;
	return list;
}

String CalcContext::GetTypeName(Value value) const
{
	if(IsNull(value))
		return "Null";
/*
	switch(value.GetType())
	{
	case INT_V:    return CalcType<int>::Describe();
	case DOUBLE_V: return CalcType<double>::Describe();
	case STRING_V: return CalcType<String>::Describe();
	case TIME_V:   return CalcType<Time>::Describe();
	case DATE_V:   return CalcType<Date>::Describe();
	}
*/	return CalcTypeNameConvert::Format(typeid(*value.GetVoidPtr()).name());
}

String CalcContext::FormatNull(Value value, bool no_text)
{
	if(value.IsError())
		return GetErrorText(value);
	if(IsNull(value))
		return "Null";
	if(IsString(value))
		return '\"' + (String)value + '\"';
	if(no_text)
	{
		if(IsDateTime(value) || IsNumber(value))
			return StdFormat(value);
		return GetTypeName(value) + "(?)";
	}

	try
	{
		return FormatText(value);
	}
	catch(Exc e)
	{
		return e;
	}
}

String CalcContext::FormatText(Value value, const UPP::Convert& convert)
{
	if(value.IsError())
		throw Exc(GetErrorText(value));
	Value v = convert.Format(value);
	if(IsNull(v) || IsString(v))
		return v;
	CalcPacket packet(*this, "text");
	packet.args.SetCount(1);
	packet.args[0] = v;
	return StdFormat(Calc(packet));
}

void CalcContext::PushLevel()
{
	if(stack.GetCount() >= MAX_STACK_DEPTH)
		throw Exc(t_("Maximum nesting depth (") + IntStr(MAX_STACK_DEPTH) + t_(") exceeded (infinite recursion?)"));
	stack.Add();
}

void CalcContext::PopLevel()
{
	if(stack.GetCount() <= 1)
		throw Exc(t_("Expression stack underflow"));
	stack.Drop();
}

void CalcContext::RemoveExternal(CalcSymbols& e)
{
	int i = FindIndex(externals, &e);
	if(i >= 0)
		externals.Remove(i);
}

//////////////////////////////////////////////////////////////////////
// CalcPacket::

String CalcPacket::GetTypeNames() const
{
	String result;
	result << '(';
	for(int i = 0; i < args.GetCount(); i++)
	{
		if(i)
			result << ", ";
		result << context.GetTypeName(args[i]);
	}
	result << ')';
	return result;
}

//////////////////////////////////////////////////////////////////////
// CalcNode::

CalcNode::~CalcNode()
{
}

Value CalcNode::TryCalc(CalcContext& context) const
{
	try
	{
		return Calc(context);
	}
	catch(Exc e)
	{
		return ErrorValue(e);
	}
}

double CalcNode::CalcDouble(CalcContext& context) const
{
	if(this == 0)
		return Null;
	Value v = Calc(context);
	if(!IsNull(v) && !IsNumber(v))
		throw Exc(t_("Number expected, found: ") + context.FormatNull(v));
	return v;
}

String CalcNode::CalcString(CalcContext& context) const
{
	if(this == 0)
		return Null;
	Value v = Calc(context);
	if(!IsNull(v) && !IsString(v))
		throw Exc(t_("String expected, found: ") + context.FormatNull(v));
	return v;
}

int CalcNode::CalcInt(CalcContext& context) const
{
	if(this == 0)
		return Null;
	Value v = Calc(context);
	if(!IsNull(v) && !IsNumber(v))
		throw Exc(t_("Number expected, found: ") + context.FormatNull(v));
	return v;
}

int64 CalcNode::CalcInt64(CalcContext& context) const
{
	if(this == 0)
		return Null;
	Value v = Calc(context);
	if(!IsNull(v) && !IsNumber(v))
		throw Exc(t_("Number expected, found: ") + context.FormatNull(v));
	return v;
}

Date CalcNode::CalcDate(CalcContext& context) const
{
	if(this == 0)
		return Null;
	Value v = Calc(context);
	if(!IsNull(v) && !IsDateTime(v))
		throw Exc(t_("Date expected, found: ") + context.FormatNull(v));
	return v;
}

Time CalcNode::CalcTime(CalcContext& context) const
{
	if(this == 0)
		return Null;
	Value v = Calc(context);
	if(!IsNull(v) && !IsDateTime(v))
		throw Exc(t_("Date/time expected, found: ") + context.FormatNull(v));
	return v;
}

bool CalcNode::CalcBool(CalcContext& context) const
{
	if(this == 0)
		return false;
	Value v = Calc(context);
	if(IsNull(v))
		return false;
	if(IsString(v))
		return !String(v).IsEmpty();
	if(IsNumber(v))
		return double(v) != 0;
	throw Exc(t_("Date/time expected, found: ") + context.FormatNull(v));
}

void CalcNode::Expect(const char *s, String found) const
{
	String e;
	e << name << t_(": expected: ") << s << t_(", found: ") << found;
	throw Exc(e);
}

CalcNodePtr GetVariableNode(String varname)
{
	return new CalcFunctionNode(varname);
}

CalcNodePtr GetFunctionNode(String fnname, pick_ Vector<CalcNodePtr>& args)
{
	return new CalcFunctionNode(fnname, args);
}

//////////////////////////////////////////////////////////////////////
// CalcFunctionNode::

void CalcFunctionNode::ScanArgs(CalcPacket& packet) const
{
	packet.args.SetCount(args.GetCount());
	Value *a = packet.args.Begin();
	for(const CalcNodePtr *p = args.Begin(), *e = args.End(); p < e; p++, a++)
		if((*a = (*p)->Calc(packet.context)).IsError())
			throw Exc(GetErrorText(*a));
}

//////////////////////////////////////////////////////////////////////
// CalcParser::

CalcParser::CalcParser(bool sql_style_)
: start(0), pos(0)
, op_begin(0), op_end(0), op_last(OP_NONE)
, sql_style(sql_style_)
{
}

int CalcParser::Skip()
{
REPEAT:
	while(*pos && *pos <= ' ')
		pos++;
	if(*pos == '/')
		if(pos[1] == '/')
		{ // line comment
			while(*pos && *pos != '\n')
				pos++;
			goto REPEAT;
		}
		else if(pos[1] == '*')
		{
			const char *comment = pos++;
			while(*++pos && !(*pos == '*' && pos[1] == '/'))
				;
			if(*pos)
			{
				pos += 2;
				goto REPEAT;
			}
			else
				throw Exc(t_("/* without matching */"));
		}
	return *pos;
}

bool CalcParser::Expect(const String& s)
{
	String err = t_("expected ");
	err << (s.GetLength() > 1 ? t_("symbol") : t_("character")) << t_(" '") << s << t_("', found: ");
	int l = strlen(pos);
	if(l > 15)
		err.Cat(pos, 15);
	else if(l)
		err.Cat(pos);
	else
		err.Cat(t_("end of line"));
	throw Exc(err);
}

String CalcParser::GetIdent()
{
	Skip();
	const char *begin = pos;
	while(IsIdent(*pos))
		pos++;
	const char *end = pos;
	while(end > begin && end[-1] == ' ')
		end--;
	if(end - begin == 8 && !memcmp(begin, "operator", 8))
	{
		while(*pos && (byte)*pos <= ' ')
			pos++;
		begin = pos;
		while(*pos && (byte)*pos > ' ' && *pos != '(')
			pos++;
		end = pos;
	}
	return String(begin, end);
}

Value CalcParser::GetNumberOrDate()
{
	Skip();
	ASSERT(IsDigit(*pos));
	const char *begin = pos;
	bool comma = false;
	while(IsDigit(*pos))
		pos++;
	if(*pos == '.' && pos[1] != '.')
	{ // try date
		if(!IsDigit(*++pos) && *pos != 'E' && *pos != 'e')
		{ // day
			Date date = GetSysDate();
			int day = atoi(begin);
			if(day < 1 || day > LastDayOfMonth(date).day)
				throw Exc(NFormat(t_("invalid day number ("), day));
			else
				date.day = day;
			return date;
		}
		// might be a decimal
		const char *begin2 = pos;
		while(IsDigit(*++pos))
			;
		if(*pos == '.')
		{ // no, it's a date
			Date date = GetSysDate();
			int day = atoi(begin);
			int month = atoi(begin2);
			int year = date.year;
			if(*++pos == '-' || *pos == '+' || IsDigit(*pos))
			{ // year number given
				const char *begin3 = pos;
				while(IsDigit(*++pos))
					;
				year = atoi(begin3);
				if(pos - begin3 <= 2)
					date.year = year + (year < 80 ? 2000 : 1900);
				else if(date.year >= -4000 && date.year <= 4000)
					date.year = year;
				else
					throw Exc(NFormat("neplatné èíslo roku (%d)", year));
			}
			if(month >= 1 && month <= 12)
				date.month = month;
			else
				throw Exc(NFormat("neplatné èíslo mìsíce (%d)", month));
			if(day >= 1 && day <= LastDayOfMonth(date).day)
				date.day = day;
			else
				throw Exc(NFormat("neplatné èíslo dne (%d)", day));
			return date;
		}
	}
/*	else if(*pos == ',' && IsDigit(pos[1]))
	{ // alternative decimal separator
		comma = true;
		while(IsDigit(*++pos))
			;
	}
*/
	if(*pos == 'E' || *pos == 'e')
	{ // exponent
		if(*++pos == '+' || *pos == '-')
			pos++;
		while(IsDigit(*pos))
			pos++;
	}

	const char *temp = NULL;
	errno = 0;
	double value;
	String copy(begin, pos);
	value = ScanDouble(copy, &temp);
	if(IsNull(value) || temp != copy.End())
		throw Exc(NFormat("neplatná èíselná konstanta '%s\'", copy));
	return value;
}

String CalcParser::GetString()
{
	CParser parser(pos);
	try {
		String result = parser.ReadOneString();
		pos = parser.GetPtr();
		return result;
	}
	catch(CParser::Error e) {
		throw Exc(e);
	}
}

String CalcParser::GetSqlString()
{
	ASSERT(*pos == '\'');
	pos++;
	StringBuffer out;
	for(;;) {
		if(*pos == '\'') {
			if(*++pos != '\'')
				break;
			out.Cat(*pos++);
		}
		else if(*pos)
			out.Cat(*pos++);
		else
			throw Exc(t_("Unterminated string constant"));
	}
	return out;
}

CalcNodePtr CalcParser::Scan(const char *t)
{
	Clear(t);
	CalcNodePtr result = ScanAny();
	if(Skip())
		throw Exc(NFormat(t_("illegal characters after expression: %s (expression: %s)"), StringSample(pos, 10), StringSample(t, 10)));
	return result;
}

CalcNodePtr CalcParser::ScanVoid(const char *t)
{
	Clear(t);
	if(!Skip())
		return CalcNodePtr();
	CalcNodePtr result = ScanAny();
	if(Skip())
		throw Exc(NFormat(t_("illegal characters after expression: %s (expression: %s)"), StringSample(pos, 10), StringSample(t, 10)));
	return result;
}

void CalcParser::Clear(const char *t)
{
	start = pos = t;
}

CalcNodePtr CalcParser::ScanAny()
{
	return ScanSequence();
}

/*
CalcNodePtr CalcParser::ScanLambda()
{
	return Check(OP_LAMBDA) ? new CalcLambdaNode(ScanAny()) : ScanSelect();
}
*/

CalcNodePtr CalcParser::ScanSequence()
{
	Vector<CalcNodePtr> seq;
	seq.Add(ScanSelect());
	while(Check(OP_COMMA) || Check(OP_SEMICOLON))
		seq.Add(ScanSelect());
	if(seq.GetCount() == 1)
		return seq[0];
	return new CalcSequenceNode(seq);
}

CalcNodePtr CalcParser::ScanSelect()
{
	CalcNodePtr node = ScanLogOr();
	if(Check(OP_QUESTION)) { // ?:
		CalcNodePtr when1 = ScanSelect();
		Force(OP_COLON, ":");
		CalcNodePtr when0 = ScanSelect();
		node = new CalcSelectNode(node, when1, when0);
	}
	return node;
}

CalcNodePtr CalcParser::ScanLogOr()
{
	CalcNodePtr node = ScanLogAnd();
	while(Check(OP_LOG_OR))
		node = new CalcLogOrNode(node, ScanLogAnd());
	return node;
}

CalcNodePtr CalcParser::ScanLogAnd()
{
	CalcNodePtr node = ScanEq();
	while(Check(OP_LOG_AND))
		node = new CalcLogAndNode(node, ScanEq());
	return node;
}

CalcNodePtr CalcParser::ScanEq()
{
	CalcNodePtr node = ScanCompare();
	for(;;)
		if(Check(OP_EQ))
			node = new CalcFunctionNode("==", node, ScanCompare());
		else if(Check(OP_NE))
			node = new CalcFunctionNode("!=", node, ScanCompare());
		else
			return node;
}

CalcNodePtr CalcParser::ScanCompare()
{
	CalcNodePtr node = ScanBitOr();
	for(;;)
		if(Check(OP_LE))
			node = new CalcFunctionNode("<=", node, ScanBitOr());
		else if(Check(OP_GE))
			node = new CalcFunctionNode(">=", node, ScanBitOr());
		else if(Check(OP_LT))
			node = new CalcFunctionNode("<", node, ScanBitOr());
		else if(Check(OP_GT))
			node = new CalcFunctionNode(">", node, ScanBitOr());
		else {
			bool isnot = Check(OP_SQL_NOT);
			bool isbtw = Check(OP_SQL_BETWEEN);
			bool isin = !isbtw && Check(OP_SQL_IN);
			bool islike = !isbtw && !isin && Check(OP_SQL_LIKE);
			bool isis = !isbtw && !isin && Check(OP_SQL_IS);

			CalcNodePtr out_node;
			if(isbtw) {
				CalcNodePtr lbound = ScanBitOr();
				Force(OP_LOG_AND, "and");
				CalcNodePtr ubound = ScanBitOr();
				out_node = new CalcLogAndNode(
					new CalcFunctionNode(">=", node, lbound),
					new CalcFunctionNode("<=", node, ubound));
			}
			else if(isin) {
				Vector<CalcNodePtr> lov;
				if(Check(OP_LPAR)) {
					do
						lov.Add() = ScanSelect();
					while(Check(OP_COMMA));
					Force(OP_RPAR, ")");
					out_node = new CalcFunctionNode("in", node,
						new CalcFunctionNode("[,,]", lov));
				}
				else
					out_node = new CalcConstNode(Value());
			}
			else if(islike) {
				CalcNodePtr lexpr = ScanSelect();
				out_node = new CalcFunctionNode("like", node, lexpr);
			}
			else if(isis) {
				Force(OP_SQL_NULL, "NULL");
				out_node = new CalcFunctionNode("is_null", node);
			}
			else if(isnot)
				throw Exc(t_("expected 'between', 'in' or 'like'"));
			else
				return node;

			if(!isnot)
				return out_node;
			return new CalcFunctionNode("!", out_node);
		}
}

CalcNodePtr CalcParser::ScanBitOr()
{
	CalcNodePtr node = ScanBitXor();
	while(Check(OP_BIT_OR))
		node = new CalcFunctionNode("|", node, ScanBitXor());
	return node;
}

CalcNodePtr CalcParser::ScanBitXor()
{
	CalcNodePtr node = ScanBitAnd();
	while(Check(OP_BIT_XOR))
		node = new CalcFunctionNode("^", node, ScanBitAnd());
	return node;
}

CalcNodePtr CalcParser::ScanBitAnd()
{
	CalcNodePtr node = ScanAdditive();
	while(Check(OP_BIT_AND))
		node = new CalcFunctionNode("&", node, ScanAdditive());
	return node;
}

CalcNodePtr CalcParser::ScanAdditive()
{
	CalcNodePtr node = ScanShift();
	for(;;)
		if(Check(OP_ADD))
			node = new CalcFunctionNode("+", node, ScanShift());
		else if(Check(OP_SUB))
			node = new CalcFunctionNode("-", node, ScanShift());
		else
			return node;
}

CalcNodePtr CalcParser::ScanShift()
{
	CalcNodePtr node = ScanMultiplicative();
	for(;;)
		if(Check(OP_LSHIFT))
			node = new CalcFunctionNode("<<", node, ScanMultiplicative());
		else if(Check(OP_RSHIFT))
			node = new CalcFunctionNode(">>", node, ScanMultiplicative());
		else
			return node;
}

CalcNodePtr CalcParser::ScanMultiplicative()
{
	CalcNodePtr node = ScanPow();
	for(;;)
		if(Check(OP_MUL))
			node = new CalcFunctionNode("*", node, ScanPow());
		else if(Check(OP_DIV))
			node = new CalcFunctionNode("/", node, ScanPow());
		else if(Check(OP_MOD))
			node = new CalcFunctionNode("%", node, ScanPow());
		else
			return node;
}

CalcNodePtr CalcParser::ScanPow()
{
	CalcNodePtr node = ScanPrefix();
	while(Check(OP_POW))
		node = new CalcFunctionNode("pow", node, ScanPow());
	return node;
}

CalcNodePtr CalcParser::ScanPrefix()
{
	Skip();
	const char *start = pos;
	switch(GetOperator())
	{
	case OP_LAMBDA: SkipOperator(); return new CalcLambdaNode(ScanPrefix());
	case OP_ADD: SkipOperator(); return new CalcFunctionNode("+", ScanPrefix());
	case OP_SUB: SkipOperator(); return new CalcFunctionNode("-", ScanPrefix());
	case OP_BIT_NOT: SkipOperator(); return new CalcFunctionNode("~", ScanPrefix());
	case OP_LOG_NOT: SkipOperator(); return new CalcFunctionNode("!", ScanPrefix());
	case OP_LPAR:
		{ // parenthesised subexpression
			const char *begin = pos;
			SkipOperator();
			CalcNodePtr exp = ScanAny();
			if(!Force(OP_RPAR, ")"))
				throw Exc(NFormat(t_("left parenthesis not matched; subexpression starts at %s"), StringSample(begin, 20)));
			return ScanPostfix(exp);
		}
	case OP_LBRACKET:
		SkipOperator();
		{ // array constructor
			CalcNodePtr ctr;
			if(Check(OP_RBRACKET))
				ctr = new CalcFunctionNode("[,,]");
			else
			{
				CalcNodePtr arg = ScanSelect();
				if(Check(OP_DOTS)) {
					CalcNodePtr maxrng = ScanSelect();
					ctr = new CalcFunctionNode("[..]", arg, maxrng);
				}
				else {
					Vector<CalcNodePtr> arglist;
					arglist.Add() = arg;
					while(Check(OP_COMMA))
						arglist.Add() = ScanSelect();
					ctr = new CalcFunctionNode("[,,]", arglist);
				}
				if(!Force(OP_RBRACKET, "]"))
					throw Exc("chybí pravá hranatá závorka ']'");
			}
			return ScanPostfix(ctr);
		}
	}
	if(*pos == '\"')
		return ScanPostfix(new CalcConstNode(GetString()));
	if(sql_style && *pos == '\'')
		return ScanPostfix(new CalcConstNode(GetSqlString()));
	if(IsDigit(*pos)) // numeric or date constant
		return ScanPostfix(new CalcConstNode(GetNumberOrDate()));
	if(IsIdent(*pos))
	{
		String ident = GetIdent();
		Vector<CalcNodePtr> arguments;
		ScanArgs(arguments);
		return ScanPostfix(new CalcFunctionNode(ident, arguments));
	}
	const char *p = pos;
	if(*p == 0)
		p = t_("end of line");
	throw Exc(t_("constant or function expected, found: ") + StringSample(p, 15));
	return 0;
}

CalcNodePtr CalcParser::ScanPostfix(CalcNodePtr node)
{
	for(;;)
		if(Check(OP_DOT)) { // member access
			byte c = Skip();
			String id = GetIdent();
			Vector<CalcNodePtr> nodes;
			nodes | node;
			ScanArgs(nodes);
			node = new CalcFunctionNode(id, nodes);
		}
		else if(Check(OP_LBRACE)) {
			Vector<CalcNodePtr> list;
			do {
				list.Add() = ScanSelect();
				if(!Check(OP_COMMA)) // default value
					break;
				list.Add() = ScanSelect();
			}
			while(Check(OP_COMMA));
			Force(OP_RBRACE, "}");
			node = new CalcSwitchNode(node, list);
		}
		else if(Check(OP_LBRACKET)) {
			CalcNodePtr index = ScanSelect();
			Force(OP_RBRACKET, "]");
			node = new CalcFunctionNode("[]", node, index);
		}
		else
			return node;
}

void CalcParser::ScanArgs(Vector<CalcNodePtr>& dest)
{
	if(Check(OP_LPAR) && !Check(OP_RPAR)) {
		const char *begin = pos;
		do
			dest | ScanSelect();
		while(Check(OP_COMMA));
		if(!Check(OP_RPAR))
			Expect(NFormat(t_("')' (subexpression starts at: %s)"), StringSample(begin - 1, 20)));
	}
}

int CalcParser::GetOperator()
{
	if(op_begin == pos)
		return op_last;
	Skip();
	op_begin = op_end = pos;
	switch(*op_end++) {
		case '?': op_last = OP_QUESTION; break;
		case ':': op_last = OP_COLON; break;
		case ';': op_last = OP_SEMICOLON; break;
		case '&': op_last = (*op_end == '&' ? op_end++, OP_LOG_AND : OP_BIT_AND); break;
		case '|': op_last = (*op_end == '|' ? op_end++, OP_LOG_OR  : OP_BIT_OR); break;
		case '^': op_last = OP_BIT_XOR; break;
		case '<': {
			if(*op_end == '=') {
				op_end++;
				op_last = OP_LE;
			}
			else if(*op_end == '<') {
				op_end++;
				op_last = OP_LSHIFT;
			}
			else if(sql_style && *op_end == '>') {
				op_end++;
				op_last = OP_NE;
			}
			else
				op_last = OP_LT;
			break;
		}
		case '>': op_last = (*op_end == '=' ? op_end++, OP_GE : *op_end == '>' ? op_end++, OP_RSHIFT : OP_GT); break;
		case '!': op_last = (*op_end == '=' ? op_end++, OP_NE : OP_LOG_NOT); break;
		case '=': {
			if(*op_end == '=') {
				op_end++;
				op_last = OP_EQ;
			}
			else if(sql_style)
				op_last = OP_EQ;
			else {
				op_end--;
				op_last = OP_NONE;
			}
			break;
		}
		case '+': op_last = OP_ADD; break;
		case '-': op_last = OP_SUB; break;
		case '*': op_last = (*op_end == '*' ? op_end++, OP_POW : OP_MUL); break;
		case '/': op_last = OP_DIV; break;
		case '%': op_last = OP_MOD; break;
		case '~': op_last = OP_BIT_NOT; break;
		case '#': op_last = OP_LAMBDA; break;
		case '(': op_last = OP_LPAR; break;
		case ')': op_last = OP_RPAR; break;
		case '[': op_last = OP_LBRACKET; break;
		case ']': op_last = OP_RBRACKET; break;
		case '{': op_last = OP_LBRACE; break;
		case '}': op_last = OP_RBRACE; break;
		case ',': op_last = OP_COMMA; break;
		case '.': op_last = (*op_end == '.' ? op_end++, OP_DOTS : OP_DOT); break;
		default: {
			op_end--;
			op_last = OP_NONE;
			if(sql_style && IsAlpha(*op_end)) {
				const char *b = op_end;
				while(IsAlNum(*++op_end) || *op_end == '_')
					;
				int len = (int)(op_end - b);
				if(len == 3 && !MemICmp(b, "and", 3))
					op_last = OP_LOG_AND;
				else if(len == 2 && !MemICmp(b, "or", 2))
					op_last = OP_LOG_OR;
				else if(len == 7 && !MemICmp(b, "between", 7))
					op_last = OP_SQL_BETWEEN;
				else if(len == 2 && !MemICmp(b, "in", 2))
					op_last = OP_SQL_IN;
				else if(len == 4 && !MemICmp(b, "like", 4))
					op_last = OP_SQL_LIKE;
				else if(len == 3 && !MemICmp(b, "not", 3))
					op_last = OP_SQL_NOT;
				else if(len == 2 && !MemICmp(b, "is", 2))
					op_last = OP_SQL_IS;
				else
					op_end = b;
			}
			break;
		}
	}
	return op_last;
}

int CalcParser::SkipOperator()
{
	int op = GetOperator();
	pos = op_end;
	return op;
}

bool CalcParser::Check(int op)
{
	if(op != GetOperator())
		return false;
	SkipOperator();
	return true;
}

bool CalcParser::Force(int op, const char *expect)
{
	if(Check(op))
		return true;
	Expect(expect);
	return false;
}

END_UPP_NAMESPACE
